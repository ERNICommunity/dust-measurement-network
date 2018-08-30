import numpy as np
import pandas as pd
from sklearn.preprocessing import scale, OneHotEncoder

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

labels = ['P1', 'P2']
categoricals = [ 'weekday', 'hour', 'node_id', 'wind_deg_bin_minus_1', 'wind_deg_bin_minus_2', 'wind_deg_bin_minus_3', 'wind_deg_bin_minus_4', 'wind_deg_bin']
# we might want to include day_in_year in categoricals WHEN we have at least a year of data, ideally two

data = pd.read_csv('data_to_use/transformed_dust_measurement.csv')
Y = data[labels].values

data = data.drop(labels=labels, axis=1)
data = data.drop(labels=['Unnamed: 0', 'dt_iso', 'wind_deg', 'day_in_year'], axis = 1)

# normalization for float data
for c in [col for col in data.columns if not col in categoricals]:
    data[[c]] = scale(data[c])

# here we use one-hot encoding for categorical features
# # this greatly increases dimensionality of the feature vector
categories = None

for c in [col for col in data.columns if col in categoricals]:
    enc = OneHotEncoder()
    dummies = enc.fit_transform(data[[c]].values).toarray()
    if categories is None:
        categories = dummies
    else:
        categories = np.concatenate((categories, dummies), axis=1)
    data = data.drop(c, axis=1)

# categories + non categories
X = np.concatenate((data.values, categories), axis=1)
np.random.shuffle(X) # shuffle to get rid of the start oof month/end of month problem when selecting the validation set

# this will allow us to store the best fitting model we see during training
cp = ModelCheckpoint('models/best_model.h5', monitor='val_mean_absolute_error', verbose=0, save_best_only=True, save_weights_only=False)

# build and fit the multivariate multiple regressor model
model = Sequential()
model.add(Dense(1000, input_dim=X.shape[1], kernel_initializer='glorot_normal', activation='relu'))
model.add(BatchNormalization())
model.add(Dense(150, kernel_initializer='glorot_normal', activation='relu'))
model.add(BatchNormalization())
model.add(Dense(Y.shape[1], kernel_initializer='glorot_normal'))
model.compile(loss='mean_squared_error', optimizer='adam',metrics=['mae', 'acc'])

model.fit(X, Y, batch_size=32,epochs=100,validation_split=0.2, callbacks=[cp])