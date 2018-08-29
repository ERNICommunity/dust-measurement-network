# -*- coding: utf-8 -*-
"""
Created on Sun Aug 26 11:25:29 2018

@author: poso
"""

import numpy as np
import pandas as pd
import dateutil.parser as ps

def parse_dt(string_to_parse):
    return ps.parse(string_to_parse)

with open('interleved_dust_measurement_zurich_jan_2018.csv') as input_file:
    data = pd.read_csv(input_file)
    date_column = data['dt_iso']
    
    date_column_list = [date_column.iloc[i] for i in range(date_column.shape[0])]
    print(date_column_list)
    
    date = ps.parse(date_column_list[0])
    print(date)
    
    #parse_list_of_strings = np.vectorize(parse_dt)
    #dates = parse_list_of_strings(date_column)
    
    #print(dates)
    