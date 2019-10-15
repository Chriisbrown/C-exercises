import matplotlib.pyplot as plt
import numpy as np



def f(x,A,B):
    return B*x + A

def short_res(a,b,x,y):
    return np.abs(b*x-y+a)/np.sqrt(b**2+1)

def least_squares(x,y,r):
    product = 0
    X_mean = np.mean(x)
    Y_mean = np.mean(y)
    X_squared_mean = np.mean(x**2)
    for i in range(len(x)):
        product += x[i]*y[i]
    product = product/(len(x))

    b = (product - X_mean*Y_mean)/(X_squared_mean-X_mean**2)
    a = Y_mean - b*X_mean

    deletes = []

    for i in range(len(x)):
        res = (short_res(a,b,x[i],y[i]))
        if res > 1:
            deletes.append(i)
    x = np.delete(x,deletes)
    y = np.delete(y,deletes)
    r = np.delete(r,deletes)

    return(x,y,r,a,b)

def w(r):
    return (1000/(r**2 + 400))




def weighted_least_squares(x,y,r):
    x_w = np.sum(w(r)*x)/np.sum(w(r))
    y_w = np.sum(w(r)*y)/np.sum(w(r))

    numerator = 0
    denominator = 0

    for i in range(len(x)):
        numerator += w(r[i]) *(x[i]-x_w)*(y[i]-y_w)
        denominator += w(r[i]) * (x[i]-x_w)**2

    b = numerator/denominator
    a = y_w - b*x_w
    deletes = []

    for i in range(len(x)):
        res = (short_res(a,b,x[i],y[i]))
        print(res-w(r[i]))
        if res>2:
            deletes.append(i)

    x = np.delete(x,deletes)
    y = np.delete(y,deletes)
    r = np.delete(r,deletes)


    return(x,y,r,a,b)

fig = plt.figure()
ax = fig.add_subplot(111,aspect='equal')
x = np.array([0,4,2,6,1,5,3,7])
y = np.array([2,2,2,2,2.5,2.5,6.5,2.5])
r = np.array([576,212,1008,796,68,864,500,720])

for i in range(len(x)):
    circle1=plt.Circle((x[i],y[i]),r[i]/1000,color='r')
    plt.gcf().gca().add_artist(circle1)


x,y,r,a_own, b_own = least_squares(x,y,r)

    

x,y,r,w_a_own, w_b_own = weighted_least_squares(x,y,r)

    

X_1 = np.linspace(-1,9,10)
Y_1 = f(X_1,a_own,b_own)

X_2 = np.linspace(-1,9,10)
Y_2 = f(X_2,w_a_own,w_b_own)


plt.plot(X_1,Y_1)
plt.plot(X_2,Y_2)

plt.xlim(-1,9)
plt.ylim(-1,9)


plt.show()



