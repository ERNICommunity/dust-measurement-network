# -*- coding: utf-8 -*-
"""
Created on Wed Aug 29 19:47:49 2018

@author: poso
"""

import re

def select_node(node_number, input_file='2018-01_sds011.csv'):
    with open(input_file) as dust_input:
        writer = open('node_'+node_number+'_dust_measurements.csv', 'w')
        for i, line in enumerate(dust_input):
            if i == 0:
                writer.write(line)
            if re.search('^'+node_number+';', line) is not None:
                if i%1000 == 0:
                    print(line)
                writer.write(line)
        
        
        writer.close()
        return 'node_'+node_number+'_dust_measurements.csv'