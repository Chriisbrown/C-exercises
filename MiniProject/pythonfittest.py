import matplotlib.pyplot as plt
import numpy as np


X = np.array([
         [2,4],
         [0,4],
         [6,4],
         [6,4],
         [0,5],
         [6,5],
         [4,7],
         [2,5]])

for val in X:

    plt.scatter(val[0],val[1])
plt.xlim(-1,9)
plt.ylim(-1,9)
plt.show()



