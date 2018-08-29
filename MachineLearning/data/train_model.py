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

labels = ['P1', 'P2']
categoricals = [ 'weekday', 'hour', 'node_id']

data = pd.read_csv('data_to_use/transformed_dust_measurement_zurich_jan_2018.csv')
Y = data[labels].values

data = data.drop(labels=labels, axis=1)
data = data.drop(labels=['Unnamed: 0', 'dt_iso', 'wind_deg', 'day_in_year'], axis = 1)

for c in [col for col in data.columns if not col in categoricals]:
    data[[c]] = scale(data[c])

categories = None

for c in [col for col in data.columns if col in categoricals]:
    enc = OneHotEncoder()
    dummies = enc.fit_transform(data[[c]].values).toarray()
    if categories is None:
        categories = dummies
    else:
        categories = np.concatenate((categories, dummies), axis=1)
    data = data.drop(c, axis=1)

X = np.concatenate((data.values, categories), axis=1)

model = Sequential()
model.add(Dense(15, input_dim=X.shape[1], kernel_initializer='normal', activation='relu'))
model.add(BatchNormalization())
model.add(Dense(Y.shape[1], kernel_initializer='normal'))
model.compile(loss='mean_squared_error', optimizer='adam',metrics=['mae', 'acc'])

model.fit(X, Y, batch_size=32,epochs=750,validation_split=0.2)