import matplotlib.pyplot as plt
import numpy as np



def f(x,A,B):
    return B*x + A

def short_res(intercept,gradient,x,y):
    return np.abs(-gradient*x + y - intercept)/np.sqrt(gradient**2 + 1)

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

    print(b)

    return(x,y,r,a,b)

def w(r):
    return (1000/(r**2 + 400))

def w2(r):
    return r**2/1000




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
        if res>2:
            deletes.append(i)

    x = np.delete(x,deletes)
    y = np.delete(y,deletes)
    r = np.delete(r,deletes)


    return(x,y,r,a,b)
    
    

def drift_time(a,b,x,y,r):
    speed = np.zeros(8)
    res = np.zeros(8)
    for i in range(len(x)):
        res[i] = (short_res(a,b,x[i],y[i]))
        speed[i] = res[i]/r[i]

    mean_speed = (np.mean(speed))
    

    print("mean speed: ",mean_speed)
        
    return(mean_speed)




def drift_time_fit(x,y,r):
    x_w = np.sum(w2(r)*x)/np.sum(w2(r))
    y_w = np.sum(w2(r)*y)/np.sum(w2(r))

    numerator = 0
    denominator = 0

    for i in range(len(x)):
        numerator += w2(r[i]) *(x[i]-x_w)*(y[i]-y_w)
        denominator += w2(r[i]) * (x[i]-x_w)**2

    b = numerator/denominator
    a = y_w - b*x_w

    return(a,b)

def hit_finder(x,y,r,a,b,mean_speed):
    hit_x = np.zeros([8])
    hit_y = np.zeros([8])
    r = r*mean_speed

   

    for i in range(8):

        c = (x[i]/a + y[i])
        m = -1/a

        A = (1+m**2)
        B = 2*(m*c-m*y[i] - x[i])
        C = (y[i]**2 - r[i]**2 + x[i]**2 - 2*c*y[i] + c**2)

        

        x_plus = (-B+(B**2 - 4*A*C)**(1/2))/(2*A)
        x_minus = (-B-(B**2 - 4*A*C)**(1/2))/(2*A)

        y_plus = m*x_plus + c
        y_minus = m*x_minus + c

        if short_res(b,a,x_plus,y_plus) > short_res(b,a,x_minus,y_minus):
            hit_x[i] = x_minus
            hit_y[i] = y_minus
        else:
            hit_x[i] = x_plus
            hit_y[i] = y_plus
        
    return hit_x,hit_y


def plot(x,y,r):
    for i in range(len(x)):
        circle1=plt.Circle((x[i],y[i]),r[i],color='r')
        plt.gcf().gca().add_artist(circle1)


fig = plt.figure()
ax = fig.add_subplot(111,aspect='equal')
x = np.array([0,1,2,3,4,5,6,7])
y = np.array([2,2.5,2,2.5,2,2.5,3,2.5])
r = np.array([9,172,63,99,136,27,190,45])
hit_x = np.zeros([8])
hit_y = np.zeros([8])


x,y,r,a_own, b_own = least_squares(x,y,r)

print("gradient: " ,b_own)
    

x,y,r,w_a_own, w_b_own = weighted_least_squares(x,y,r)

print("gradient: " ,w_b_own)

mean_speed = drift_time(w_a_own,w_b_own,x,y,r)

plot(x,y,r*mean_speed)

for i in range(10):
    hit_x,hit_y = hit_finder(x,y,r,w_b_own,w_a_own,mean_speed)

    dt_a_own,dt_b_own = drift_time_fit(hit_x,hit_y,r)

    print(i)
    print("gradient: " ,dt_b_own)
    print("==============")

    mean_speed = drift_time(dt_a_own,dt_b_own,x,y,r)


    
X_1 = np.linspace(-1,9,10)
Y_1 = f(X_1,a_own,b_own)

X_2 = np.linspace(-1,9,10)
Y_2 = f(X_2,w_a_own,w_b_own)

X_3 = np.linspace(-1,9,10)
Y_3 = f(X_3,dt_a_own,dt_b_own)


#plt.plot(X_1,Y_1)
#plt.plot(X_2,Y_2)
plt.plot(X_3,Y_3)


plt.xlim(-1,9)
plt.ylim(-1,9)

plt.show()



