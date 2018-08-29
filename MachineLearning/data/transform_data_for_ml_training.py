# -*- coding: utf-8 -*-
"""
Created on Wed Aug 29 19:44:52 2018

@author: poso
"""

from data_filters import select_detector_node
from aggregate import aggregate_in_time
import augment.interleaving as il 
import augment.transforming as trsfm

if __name__=='__main__':
    weather_data_file = 'Zurich_weather_historic_2018.csv'
    dust_file = '2018-01_sds011.csv'
    file_with_node = select_detector_node.select_node('574', input_file=dust_file)
    aggregated_file = aggregate_in_time.aggregate(file_with_node, weather_data_file)
    
    data = il.load_data(aggregated_file)

    data = il.bin_in_wind_deg(data)  
    data = il.interleave(data)
    data = trsfm.transform(data)
    
    with open('data_to_use/transformed_dust_measurement.csv', 'w') as output:
        data.to_csv(output)
    