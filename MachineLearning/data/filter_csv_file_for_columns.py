# -*- coding: utf-8 -*-
"""
Created on Sat Aug 25 17:27:49 2018

@author: poso
"""

node_number = 574

import math
import re
import csv
import numpy as np
import pandas as pd
import dask.dataframe as dd

with open('Zurich_weather_historic_2018.csv') as input_file:
    data = pd.read_csv(input_file, sep=';')
    filtered_data = data[['dt_iso', 'pressure', 'humidity', 'wind_speed', 'wind_deg']]
    filtered_data = filtered_data.head(31*24)

    with open('node_574_dust_Zurich_January_2018.csv') as dust_input:
        data = pd.read_csv(dust_input, sep=';')
        filtered_dust_data = data[['sensor_id', 'timestamp', 'P1', 'P2']]
        sensor_id = filtered_dust_data[['sensor_id']].head(31*24)
        print(31*24)
        n_rows = filtered_dust_data.shape[0]
        print(n_rows)
        p1_tmp = []
        p2_tmp = []
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
        
            
        
        print(len(p1_av))
        
        filtered_data['node_id'] = sensor_id
        filtered_data['P1'] = p1_av[0:31*24] 
        filtered_data['P2'] = p2_av[0:31*24] 
        
        with open('aggregated_dust_measurement_data_zurich_january_2018.csv', 'w') as output:
            filtered_data.to_csv(output)
#with open('2018-01_sds011.csv') as dust_input:
#    writer = open('node_574_dust_Zurich_January_2018.csv', 'w')
#    for i, line in enumerate(dust_input):
#        if i == 0:
#            writer.write(line)
#        if re.search('^574;', line) is not None:
#            if i%1000 == 0:
#                print(line)
#            writer.write(line)
            
#    writer.close()
    #dust_data = pd.read_csv(dust_input, chun)
    #df = dd.read_csv(dust_input)
    #print(df.keys())
        
        