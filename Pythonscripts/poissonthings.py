import numpy as np
import math
import matplotlib.pyplot as plt




mus = [3.41,3.42]


for mu in mus: 
    data = np.zeros([2,10])
    j = 0
    f = 0
    n = 1
    for i in range(10):
        data[0][i] = n
        #print(n)
        #print("upper: ", n+np.sqrt(n), " lower  ", n-np.sqrt(n))
        if (n-np.sqrt(n) <= mu and mu <= n+np.sqrt(n)):
            #print("yes")
            for j in range(i):
                f += np.exp(-mu)*((mu**j)/(math.factorial(j)))
            data[1][i] = f
            
            #print(f)
            f = 0
        n += 1


    plt.plot(data[0],data[1])
plt.show()