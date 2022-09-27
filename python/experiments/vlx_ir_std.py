import os
import numpy as np
import matplotlib.pyplot as plt

EXPERIMENTS_FILENAME = os.path.join(
    'data',
    'experiments',
    os.path.splitext(os.path.basename(__file__))[0] + '.txt'
)
SENSOR_LIST = (
    "VLX",
    "IR",
)
POINTS_PER_EXPERIMENT = 1000

data_points = np.zeros((len(SENSOR_LIST), POINTS_PER_EXPERIMENT), dtype=np.int16)
mean_points = np.zeros((len(SENSOR_LIST)), dtype=np.float32)
std_points = np.zeros((len(SENSOR_LIST)), dtype=np.float32)

with open(EXPERIMENTS_FILENAME, 'r') as file:
    for i in range(POINTS_PER_EXPERIMENT):
        point = file.readline().strip().split()
        for j in range(1, len(point), 2):
            data_points[j // 2][i] = float(point[j])
    for i in range(len(SENSOR_LIST)):
        mean_points[i], std_points[i] = np.mean(data_points[i]), np.std(data_points[i])

print('Done!\n')
print('EXPERIMENTS_DISTANCES:\n', *SENSOR_LIST)
print('MEAN_DISTANCES:\n', *mean_points)
print('STD_OF_DISTANCES:\n', *std_points)
print()
