# -*- coding: utf-8 -*-
"""
Created on Wed Aug 29 19:58:12 2018

@author: poso
"""
import numpy as np
import pandas as pd

def aggregate(input_file_dust, input_file_weather):
    
    with open(input_file_weather) as weather:
        data = pd.read_csv(weather, sep=';')
    filtered_data = data[['dt_iso', 'pressure', 'humidity', 'wind_speed', 'wind_deg']]
    filtered_data = filtered_data.head(31*24)

    with open(input_file_dust) as dust:
        data = pd.read_csv(dust, sep=';')
    filtered_dust_data = data[['sensor_id', 'timestamp', 'P1', 'P2']]
    sensor_id = filtered_dust_data[['sensor_id']].head(31*24)
    n_rows = filtered_dust_data.shape[0]
    p1_av = []
    p2_av = []
    n_measurements_per_day = 21
    print(n_measurements_per_day)
    p1_data = data.P1
    p2_data = data.P2
        
    n_aggregations = n_rows//n_measurements_per_day
    print(n_aggregations)
    offset = 0
    for i in range(n_aggregations):
        p1_av.append(np.mean(p1_data[offset:offset+n_measurements_per_day]))
        p2_av.append(np.mean(p2_data[offset:offset+n_measurements_per_day]))
        offset = (i+1)*n_measurements_per_day
            
    p1_av.append(np.mean(p1_data[offset:]))
    p2_av.append(np.mean(p2_data[offset:]))
        
    filtered_data['node_id'] = sensor_id
    filtered_data['P1'] = p1_av[0:31*24] 
    filtered_data['P2'] = p2_av[0:31*24] 
        
    with open('aggregated_dust_measurement.csv', 'w') as output:
        filtered_data.to_csv(output)
    
    return 'aggregated_dust_measurement.csv'