import numpy as np
import pandas as pd
import pickle
from sklearn.externals import joblib
from sklearn.preprocessing import StandardScaler, OneHotEncoder
import constants as const
from sklearn.utils import shuffle

# this will prevent TF from allocating the whole GPU
from keras import backend as K
import tensorflow as tf
config = tf.ConfigProto()
config.gpu_options.allow_growth=True
sess = tf.Session(config=config)
K.set_session(sess)

from keras.models import Sequential
from keras.layers import Dense, BatchNormalization
from keras.callbacks import ModelCheckpoint

# we might want to include day_in_year in categoricals WHEN we have at least a year of data, ideally two

data = pd.read_csv('data/data_to_use/transformed_dust_measurement.csv')
Y = data[const.label_column_names].values

data = data.drop(labels=const.label_column_names, axis=1)
data = data.drop(labels=['Unnamed: 0', 'Unnamed: 0.1', 'dt_iso', 'wind_deg', 'day_in_year'], axis = 1)

# normalization for float data
for c in [col for col in data.columns if not col in const.categorical_column_names]:
    scale = StandardScaler()
    data[[c]] = scale.fit_transform(data[c].values.reshape((-1, 1)))
    joblib.dump(scale, const.transformer_path + c + const.transformer_extension)

# here we use one-hot encoding for categorical features
# this greatly increases dimensionality of the feature vector
one_hot_categories = None

for c in [col for col in data.columns if col in const.categorical_column_names]:
    enc = OneHotEncoder()
    on_hot = enc.fit_transform(data[[c]].values).toarray()
    if one_hot_categories is None:
        one_hot_categories = on_hot
    else:
        one_hot_categories = np.concatenate((one_hot_categories, on_hot), axis=1)
    data = data.drop(c, axis=1)
    joblib.dump(enc, const.transformer_path + c + const.transformer_extension)

# categories + non categories
X = np.concatenate((data.values, one_hot_categories), axis=1)
X, Y = shuffle(X, Y) # shuffle to get rid of the start of month/end of month problem when selecting the validation set

# this will allow us to store the best fitting model we see during training
cp = ModelCheckpoint(const.model_path, monitor='val_mean_absolute_error', verbose=0, save_best_only=True, save_weights_only=False)

# build and fit the multivariate multiple regressor model
model = Sequential()
model.add(Dense(1000, input_dim=X.shape[1], kernel_initializer='glorot_normal', activation='relu'))
model.add(BatchNormalization())
model.add(Dense(100, kernel_initializer='glorot_normal', activation='relu'))
model.add(BatchNormalization())
model.add(Dense(30, kernel_initializer='glorot_normal', activation='relu'))
model.add(Dense(Y.shape[1], kernel_initializer='glorot_normal'))
model.compile(loss='mean_squared_error', optimizer='adam',metrics=['mae'])

history = model.fit(X, Y, batch_size=128,epochs=200,validation_split=0.15, callbacks=[cp])

with open(const.history_path, 'wb') as h: # saving the history of the model
    pickle.dump(history.history, h)
