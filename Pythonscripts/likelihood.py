import matplotlib.pyplot as plt
import numpy as np

decays = np.array([0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9])
mu = np.linspace(0.1,5,100000)
Likelihood = np.ones(100000)
Log_Likelihood = np.zeros(100000)

for i in range(len(mu)):
    for t_i in decays:
        Likelihood[i] *= mu[i]*np.exp(-mu[i]*t_i)
        Log_Likelihood[i] += (-t_i*mu[i] + np.log(mu[i]))

bound = np.zeros(len(Log_Likelihood))



for i in range(len(Log_Likelihood)):
    bound[i] = np.abs((Log_Likelihood[i] - (Log_Likelihood.max()-0.5)))

A,B = np.partition(bound,1)[0:2]
    
min_bound = mu[np.where(bound == A)]
max_bound = mu[np.where(bound == B)]
max_likelihood = mu[np.where(Log_Likelihood == Log_Likelihood.max())]

print(max_likelihood - min_bound)
print(max_likelihood - max_bound)


plt.title("Likelihood with bounds")
plt.vlines(mu[np.where(bound == A)],Likelihood.min()-0.5,Likelihood.max()+0.5)
plt.vlines(mu[np.where(bound == B)],Likelihood.min()-0.5,Likelihood.max()+0.5)
plt.vlines(mu[Log_Likelihood == Log_Likelihood.max()],Likelihood.min()-0.5,Likelihood.max()+0.5,color='r')
plt.xlabel("$\lambda$")
plt.ylabel("Likelihood")
plt.ylim(-0.005,0.07)
plt.xlim(0,5)
plt.grid()

plt.plot(mu,Likelihood)
plt.show()