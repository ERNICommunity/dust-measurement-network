# -*- coding: utf-8 -*-
"""
Created on Sun Aug 26 09:43:46 2018

@author: poso
"""

import pandas as pd
import numpy as np

with open('aggregated_dust_measurement_data_zurich_january_2018.csv') as input_file:
    data = pd.read_csv(input_file)
    print(data.keys())
    
    #minus_one_nan = data[['pressure', 'humidity', 'wind_speed',
    #   'wind_deg', 'node_id', 'P1', 'P2']][:1]
    #minus_one_nan.replace()
    
    list_of_columns = ['pressure', 'humidity', 'wind_speed',
       'wind_deg', 'P1', 'P2']
    nan_data = {}
    for key in list_of_columns:
        nan_data[key] = np.nan
    
    data_frame_cache = data
    for i in range(1, 5):
        minus_one = data_frame_cache[list_of_columns][:-1]
    
        new_data_minus_one = []
        new_data_minus_one.insert(0, nan_data)
        minus_one = pd.concat([pd.DataFrame(new_data_minus_one), minus_one], ignore_index=True)
        
        data[[key+'_minus_'+str(i) for key in minus_one.keys()]] = minus_one
        data_frame_cache = minus_one
    
    """
    minus_two = minus_one[['pressure', 'humidity', 'wind_speed',
       'wind_deg', 'node_id', 'P1', 'P2']][:-1]
    
    new_data_minus_two = []
    new_data_minus_two.insert(0, nan_data)
    minus_two = pd.concat([pd.DataFrame(new_data_minus_two), minus_two], ignore_index=True)
    
    minus_three = minus_two[['pressure', 'humidity', 'wind_speed',
       'wind_deg', 'node_id', 'P1', 'P2']][:-1]
    
    new_data_minus_three = []
    new_data_minus_three.insert(0, nan_data)
    minus_three = pd.concat([pd.DataFrame(new_data_minus_three), minus_three], ignore_index=True)
    
    minus_four = minus_three[['pressure', 'humidity', 'wind_speed',
       'wind_deg', 'node_id', 'P1', 'P2']][:-1]
    
    new_data_minus_four = []
    new_data_minus_four.insert(0, nan_data)
    minus_four = pd.concat([pd.DataFrame(new_data_minus_four), minus_four], ignore_index=True)
    
    data[[key+'_minus_1' for key in minus_one.keys()]] = minus_one
    data[[key+'_minus_2' for key in minus_two.keys()]] = minus_two
    data[[key+'_minus_3' for key in minus_three.keys()]] = minus_three
    data[[key+'_minus_4' for key in minus_four.keys()]] = minus_four
    """
    data = data.dropna()
    
    with open('interleved_dust_measurement_zurich_jan_2018.csv', 'w') as output:
        data.to_csv(output)
    
    print(data.head())