from flask import Flask, jsonify, request
import base64, json
import io
import os
import numpy as np
from sklearn.externals import joblib
from os import listdir
from os.path import isfile, join

from keras import backend as K
import tensorflow as tf
config = tf.ConfigProto()
config.gpu_options.allow_growth=True
sess = tf.Session(config=config)
K.set_session(sess)
import tensorflow as tf

from keras.models import load_model

app = Flask(__name__)

model = None
transformations = {}

def __initialize_model():   
    files = [join('models/preparation', f) for f in listdir('models/preparation') if isfile(join('models/preparation', f))]

    global transformations
    for f in files:
        name = os.path.splitext(os.path.basename(f))[0]
        t = joblib.load(f)
        transformations[name] = t

    global model
    model = load_model('models/best_model.h5')
    global graph
    graph = tf.get_default_graph()

__initialize_model()

@app.route("/predict", methods=['POST'])
def predict():
    p = request.get_json()

    categoricals = [ 'sensor_id', 'weekday', 'hour', 'wind_deg_bin_minus_1', 'wind_deg_bin_minus_2', 'wind_deg_bin_minus_3', 'wind_deg_bin_minus_4', 'wind_deg_bin']

    # assume dictionary on input
    # pressure,humidity,wind_speed,wind_deg,sensor_id
    # P1_minus_1,P2_minus_1,humidity_minus_1,pressure_minus_1,wind_deg_minus_1,wind_speed_minus_1,
    # P1_minus_2,P2_minus_2,humidity_minus_2,pressure_minus_2,wind_deg_minus_2,wind_speed_minus_2,
    # P1_minus_3,P2_minus_3,humidity_minus_3,pressure_minus_3,wind_deg_minus_3,wind_speed_minus_3,
    # P1_minus_4,P2_minus_4,humidity_minus_4,pressure_minus_4,wind_deg_minus_4,wind_speed_minus_4,
    # weekday, hour

    # bin wind directions
    for k_new, k_old in [(k.replace('wind_deg', 'wind_deg_bin'), k) for k in p if k.startswith('wind_deg') ]:
        p[k_new] = p[k_old] // 10
        del p[k_old]

    for k in [k for k in p if k not in categoricals]:
        p[k] = transformations[k].transform(np.array([p[k]]).reshape((1,-1)))[0, 0]

    # categoricals will be one hot encoded
    categories = None

    for k in categoricals:
        dummies = transformations[k].transform(np.array([p[k]]).reshape((1,-1))).toarray()
        if categories is None:
            categories = dummies
        else:
            categories = np.concatenate((categories, dummies), axis=1)

    X = np.array([p['pressure'], p['humidity'], p['wind_speed'], \
         p['P1_minus_1'], p['P2_minus_1'], p['humidity_minus_1'], p['pressure_minus_1'], p['wind_speed_minus_1'], \
         p['P1_minus_2'], p['P2_minus_2'], p['humidity_minus_2'], p['pressure_minus_2'], p['wind_speed_minus_2'], \
         p['P1_minus_3'], p['P2_minus_3'], p['humidity_minus_3'], p['pressure_minus_3'], p['wind_speed_minus_3'], \
         p['P1_minus_4'], p['P2_minus_4'], p['humidity_minus_4'], p['pressure_minus_4'], p['wind_speed_minus_4']])

    X = np.concatenate((X.reshape((1, 23)), categories), axis=1)

    with graph.as_default():
        p1, p2  = model.predict(X).T
        return jsonify(p1 = np.float64(p1[0]), p2 = np.float64(p2[0]))

if __name__ == "__main__":
    app.run(host='0.0.0.0', threaded=True, debug=False, port=5000)