import os
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression


class DistanceError(Exception):
    pass

EXPERIMENTS_FILENAME = os.path.join(
    'data',
    'experiments',
    os.path.splitext(os.path.basename(__file__))[0] + '.txt'
)
EXPERIMENTS_DISTANCES = np.array([
    50,
    100,
    150,
    200,
    250,
    300,
    350,
    400,
    450,
    500,
])
POINTS_PER_EXPERIMENT = 100

data_points = np.zeros((len(EXPERIMENTS_DISTANCES), POINTS_PER_EXPERIMENT), dtype=np.int16)
mean_points = np.zeros((len(EXPERIMENTS_DISTANCES)), dtype=np.float32)
std_points = np.zeros((len(EXPERIMENTS_DISTANCES)), dtype=np.float32)

with open(EXPERIMENTS_FILENAME, 'r') as file:
    for d, distance in enumerate(EXPERIMENTS_DISTANCES):
        param, _ = file.readline().strip().split()
        param = int(param)
        if param != EXPERIMENTS_DISTANCES[d]:
            raise DistanceError
        s = file.readline()
        for i in range(POINTS_PER_EXPERIMENT):
            data_points[d][i] = int(file.readline().strip())
        mean_points[d], std_points[d] = np.mean(data_points[d]), np.std(data_points[d])
        s = file.readline()

print('Done!\n')
print('EXPERIMENTS_DISTANCES:\n', *EXPERIMENTS_DISTANCES)
print('MEAN_DISTANCES:\n', *mean_points)
print('STD_OF_DISTANCES:\n', *std_points)
print()

model = LinearRegression()
# model.fit(EXPERIMENTS_DISTANCES.reshape(-1, 1), mean_points)
model.fit(mean_points.reshape(-1, 1), EXPERIMENTS_DISTANCES)

print('MODEL_COEFFICIENTS:\n', *model.coef_, model.intercept_)
print()

plt.errorbar(EXPERIMENTS_DISTANCES, mean_points, std_points, linestyle='None', marker='o')
plt.title('Distance measurement experiment')
plt.xlabel('Real distance, mm')
plt.ylabel('Measurement, mm')
plt.xlim(0, 1.2 * EXPERIMENTS_DISTANCES[-1])
plt.ylim(0, 1.2 * EXPERIMENTS_DISTANCES[-1])
plt.grid()
plt.show()
