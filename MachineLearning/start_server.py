import io, os, time
from flask import Flask, jsonify, request
from sklearn.externals import joblib
import numpy as np
import constants as const

app = Flask(__name__)

transformations = {}

def __initialize_model():   
    os.environ["CUDA_VISIBLE_DEVICES"] = "-1"
    files = [os.path.join(const.transformer_path, f) for f in os.listdir(const.transformer_path) if os.path.isfile(os.path.join(const.transformer_path, f))]

    global model
    model = None

    global transformations
    for f in files:
        name = os.path.splitext(os.path.basename(f))[0]
        t = joblib.load(f)
        transformations[name] = t

__initialize_model()

@app.route("/predict", methods=['POST'])
def predict():
    from keras.models import load_model
    p = request.get_json()

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

    for k in [k for k in p if k not in const.categorical_column_names]:
        p[k] = transformations[k].transform(np.array([p[k]]).reshape((1,-1)))[0, 0]

    # categoricals will be one hot encoded
    categories = None
    for k in const.categorical_column_names:
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

    # this is a bad workaround for the fact that TensorFlow and Keras don't place nice with multithreading
    # every worker thread will initialize its own 'model' object 
    global model
    if model is None:
        model = load_model(const.model_path)

    start = time.time()
    p1, p2  = model.predict(X).T
    end = time.time()

    resp = jsonify(p1 = np.float64(p1[0]), p2 = np.float64(p2[0]))
    resp.headers[const.model_version_header_name] = const.model_version
    resp.headers[const.prediction_time_header_name] = '{:0.3f} ms'.format((end - start) * 1000)

    return resp

if __name__ == "__main__":
    app.run(host='0.0.0.0', threaded=False, debug=False, port=80)