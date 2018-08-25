# -*- coding: utf-8 -*-
"""
Created on Sat Aug 25 15:00:53 2018

@author: poso
"""
import csv, json, sys

in_file = 'luftdaten_data.json'
out_file = 'luftdaten_data.csv'
inputFile = open(in_file) #open json file
outputFile = open(out_file, 'w') #load csv file
data = json.load(inputFile) #load json content
inputFile.close() #close the input file

output = csv.writer(outputFile) #create a csv.write

output.writerow(data[0].keys())  # header row

for row in data:
    output.writerow(row.values()) #values row