# -*- coding: utf-8 -*-
"""
Created on Wed Aug 29 19:58:12 2018

@author: poso
"""
import numpy as np
import pandas as pd
import datetime as dt

def aggregate(input_file_dust, input_file_weather):
    with open(input_file_weather) as weather:
        data = pd.read_csv(weather, sep=',').drop_duplicates(subset='dt',keep='first').set_index('dt') # indexing by unix timestamp for faster lookups
    filtered_data = data[['dt_iso', 'pressure', 'humidity', 'wind_speed', 'wind_deg']]

    with open(input_file_dust) as dust:
        data = pd.read_csv(dust, sep=';')
    filtered_dust_data = data[['sensor_id', 'timestamp', 'P1', 'P2']]
    n_rows = filtered_dust_data.shape[0]

    # the main issue here is that we have multiple sensors in the dust input file
    # we are using a per-sensor accumulator to compute the running counts and then averages
    p1_accumulator = {}
    p2_accumulator = {}
    length_accumulator = {}

    sensors = filtered_dust_data[['sensor_id']]
    timestamps = filtered_dust_data[['timestamp']]
    p1s = filtered_dust_data[['P1']]
    p2s = filtered_dust_data[['P2']]

    current_time = None # for detection

    p1_avg = []
    p2_avg = []
    sensors_ids = []
    dt_isos = []
    pressures = []
    humidities = []
    wind_speeds = []
    wind_degs = []

    for i in range(n_rows):
        sensor = sensors.at[i, 'sensor_id']
        p1 = p1s.at[i, 'P1']
        p2 = p2s.at[i, 'P2']
        time_string = timestamps.at[i, 'timestamp']
        time = pd.to_datetime(time_string, infer_datetime_format=True, utc=True)

        if (current_time is not None) and (time.hour != current_time.hour):
            # flush the accumulators
            flush_accumulators( current_time, \
                                filtered_data, \
                                p1_accumulator, \
                                p1_avg, \
                                length_accumulator, \
                                p2_avg, \
                                p2_accumulator, \
                                sensors_ids, \
                                dt_isos, \
                                pressures, \
                                humidities, \
                                wind_speeds, \
                                wind_degs)

            p1_accumulator = {}
            p2_accumulator = {}
            length_accumulator = {}

        if sensor not in p1_accumulator:
            p1_accumulator[sensor] = p1
            p2_accumulator[sensor] = p2
            length_accumulator[sensor] = 1
        else:
            p1_accumulator[sensor] += p1
            p2_accumulator[sensor] += p2
            length_accumulator[sensor] += 1

        current_time = time
        
    # flush the final data points
    flush_accumulators( current_time, \
                                filtered_data, \
                                p1_accumulator, \
                                p1_avg, \
                                length_accumulator, \
                                p2_avg, \
                                p2_accumulator, \
                                sensors_ids, \
                                dt_isos, \
                                pressures, \
                                humidities, \
                                wind_speeds, \
                                wind_degs)
    
    # create a new dataframe
    combined_frame = pd.DataFrame({'dt_iso': dt_isos, 'pressure': pressures, 'humidity': humidities, 'wind_speed': wind_speeds, 'wind_deg': wind_degs, 'sensor_id': sensors_ids, 'P1': p1_avg, 'P2': p2_avg})

    # write the dataframe out
    with open('aggregated_dust_measurement.csv', 'w') as output:
        combined_frame.to_csv(output)
    
    return 'aggregated_dust_measurement.csv'

def flush_accumulators(current_time, filtered_data, p1_accumulator, p1_avg, length_accumulator, p2_avg, p2_accumulator, sensors_ids, dt_isos, pressures, humidities, wind_speeds, wind_degs):
    hourly_ts = dt.datetime(current_time.year, current_time.month, current_time.day, current_time.hour, 0, 0)
    unix_ts = int(hourly_ts.timestamp())

    print(hourly_ts)

    if unix_ts in filtered_data.index:
        for k in p1_accumulator:
            p1_avg.append(p1_accumulator[k] / length_accumulator[k])
            p2_avg.append(p2_accumulator[k] / length_accumulator[k])
            sensors_ids.append(k)
            dt_isos.append(hourly_ts)
            pressures.append(filtered_data.at[unix_ts, 'pressure'])
            humidities.append(filtered_data.at[unix_ts, 'humidity'])
            wind_speeds.append(filtered_data.at[unix_ts, 'wind_speed'])
            wind_degs.append(filtered_data.at[unix_ts, 'wind_deg'])

