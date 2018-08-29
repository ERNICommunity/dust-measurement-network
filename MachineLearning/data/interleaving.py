# -*- coding: utf-8 -*-
"""
Created on Sun Aug 26 09:43:46 2018

@author: poso
"""

import pandas as pd
import numpy as np
import transforming

def bin_in_wind_deg(data, bin_width=10):
    # get the column with the bins
    wind_deg = np.array(data[['wind_deg']])
    min_deg = np.min(wind_deg)
    wind_deg_bins = (wind_deg-min_deg)//bin_width
    data['wind_deg_bin'] = pd.DataFrame(wind_deg_bins)
    return data
    # determine number of bins
    # put numbers in bins


def interleave(data):    
    list_of_columns = ['pressure', 'humidity', 'wind_speed',
        'wind_deg_bin', 'P1', 'P2']
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
    
    data = data.dropna()
    return data


if __name__=='__main__':
    with open('aggregated_dust_measurement_data_zurich_january_2018.csv') as input_file:
        data = pd.read_csv(input_file)

    data = bin_in_wind_deg(data)  
    data = interleave(data)
    data = transforming.transform(data)
    
    with open('data_to_use/transformed_dust_measurement_zurich_jan_2018.csv', 'w') as output:
        data.to_csv(output)

    #with open('interleved_binned_dust_measurement_zurich_jan_2018.csv', 'w') as output:
    #    data.to_csv(output)
    