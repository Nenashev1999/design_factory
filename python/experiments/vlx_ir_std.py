import os
import numpy as np

from utils.stats import Data, Sample_statistics

import matplotlib.pyplot as plt


EXPERIMENTS_FILENAME = os.path.join(
    'data',
    'experiments',
    os.path.splitext(os.path.basename(__file__))[0] + '.txt'
)


sensors = []
data = Data()
points_set = []

with open(EXPERIMENTS_FILENAME, 'r') as file:
    first_line = file.readline().strip().split()
    for sensor in first_line[::2]:
        sensors.append(sensor)
    for point in first_line[1::2]:
        points_set.append(Sample_statistics())

    for line in file:
        line = line.strip().split()

        if line:
            for i, point in enumerate(line[1::2]):
                points_set[i].add(float(point))

    for point_set in points_set:
        data.mean_points.append(point_set.mean)
        data.var_points.append(point_set.var)
        data.std_points.append(point_set.std)


print('Done!\n')
print(data)
