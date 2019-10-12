
import numpy as np
import matplotlib.pyplot as plt
from bitstring import ConstBitStream
from scipy import stats

def f(x,A,B):
    return A*x + B

# read file
b = ConstBitStream(filename='onetrack.raw')
x = np.zeros([8])
y = np.zeros([8])
r = np.zeros([8])

fig = plt.figure()
ax = fig.add_subplot(111,aspect='equal')

for j in range(1):
    for i in range(8):
        x[i] = (b.read(3)).uint
        y[i] = (b.read(3)).uint
        r[i] = (b.read(10)).uint

        print("X: ", x[i], " Y: ", y[i], "Drift Time: ", r[i])

        if x[i] % 2 != 0:
            y[i] += 0.5

        circle1=plt.Circle((x[i],y[i]),r[i]/1000,color='r')
        plt.gcf().gca().add_artist(circle1)

    slope,intercept,r_value,p_value,std_err= stats.linregress(x,y)

    X = np.linspace(-1,9,10)
    Y = f(X,slope,intercept)
    print(slope)

    plt.xlim(-1,9)
    plt.ylim(-1,9)
    plt.plot(X,Y)
    plt.show()

