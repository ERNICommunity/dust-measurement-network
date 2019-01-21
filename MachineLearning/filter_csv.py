from os import listdir
from os.path import isfile, join
import re
from joblib import Parallel, delayed
import multiprocessing
	     
onlyfiles = [join('D:/dust_data', f) for f in listdir('D:/dust_data') if isfile(join('D:/dust_data', f))]

zurich_sensors = [570, 627, 2009, 2868, 4577, 9872, 10403, 11584, 14320, 15169]
sensor_regex = re.compile('|'.join(['(^{};)'.format(i) for i in zurich_sensors]))

def process_file(f):
    with open(f) as dust_input:
        writer = open(f + '.filtered.csv', 'a')
        for i, line in enumerate(dust_input):
            if i == 0:
                writer.write(line)
            if sensor_regex.search(line) is not None:
                if i%1000 == 0:
                    print(line)
                writer.write(line)
                    
        writer.close()

num_cores = multiprocessing.cpu_count() - 2
results = Parallel(n_jobs=num_cores)(delayed(process_file)(f) for f in onlyfiles)