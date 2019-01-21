import numpy as np
import pandas as pd

def transform(data):

    #with open('interleved_dust_measurement_zurich_jan_2018.csv') as input_file:
    #    data = pd.read_csv(input_file)
    
#print(data.keys())
    datetimes = data['dt_iso'].str[:20]
    datetimes = pd.to_datetime(datetimes, format='%Y-%m-%d %H:%M:%S', utc=True)
    data['day_in_year'] = datetimes.dt.dayofyear
    data['weekday'] = datetimes.dt.weekday
    data['hour'] = datetimes.dt.hour
    return data

# with open('transformed_dust_measurement_zurich_jan_2018.csv', 'w') as output:
#    data.to_csv(output)

