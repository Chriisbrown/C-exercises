import matplotlib.pyplot as plt
import numpy as np

def func(x,m,c):
    return m*x+c

X = np.array([0,1,2,3,4,5,6,7])
Y = np.array([2,2.5,2,2.5,2,2.5,3,2.5])
T = np.array([9,172,63,99,136,27,190,45])


x = np.linspace(0, 7, 8)
y = np.linspace(0, 7, 8)

fig = plt.figure()
ax = fig.add_subplot(111,aspect='equal')


drift_velocity = 250.453 /1e5


def plot(x,y,r):
    for i in range(len(x)):
        circle1=plt.Circle((x[i],y[i]),r[i]*drift_velocity,fill=False,edgecolor='r')
        plt.gcf().gca().add_artist(circle1)


plot(X,Y,T)

for i in range(8):
    for j in range(8):
        if i%2 != 0:
            circle1=plt.Circle((x[i],y[j]+0.5),0.02,color='b',alpha = 0.5)
            plt.gcf().gca().add_artist(circle1)
        else :
            circle1=plt.Circle((x[i],y[j]),0.02,color='b',alpha = 0.5)
            plt.gcf().gca().add_artist(circle1)




X_3 = np.linspace(-1,9,100)
Y_3 = func(X_3,0.0909037,1.97734)
plt.plot(X_3,Y_3,color='g',linewidth=0.5)


Y_u = func(X_3,0.0909037+0.000104184,1.97734)
plt.plot(X_3,Y_u,color='g',ls='-',linewidth=0.1)

Y_l = func(X_3,0.0909037-0.000104184,1.97734)
plt.plot(X_3,Y_l,color='g',ls='-',linewidth=0.1)

plt.xlim(-0.5,7.5)
plt.ylim(0,8)
plt.grid()
plt.show()
