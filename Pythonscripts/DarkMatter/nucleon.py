import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate


N = 931.5e3
A = (0.26909*129 + 0.21232*131 + 0.26909*132 + 0.10436*134 + 0.08857*136 + 0.04071*130 + 0.01910*128)
rho = 0.3*1e6
v_0 = 220*1e5
m_a = N*A




def exponent(E,E_0,r):
    return np.exp(-E/(E_0*r))

M_X = np.linspace(5e6,5000e6,10000)
sigma = np.ones(10000)
N_sigma = np.ones(10000)

for i,m_x in enumerate(M_X):

    r = 4*m_x*m_a/(m_x+m_a)**2

    Mu_a = 1/4 * m_x*m_a*r

    E_0 = 1/2*m_x*(v_0/3e10)**2

    E_max = 30
    E_min = 5

    detector_mass = 5600*5.609588e35/1e3
    detector_time = 1000*24*3600
    number_of_events = 4



    R = number_of_events * 1/detector_mass * 1/detector_time

    e = exponent(E_max,E_0,r) - exponent(E_min,E_0,r)

    sigma[i] = -R*m_x*m_a*(np.pi)**(1/2) / (2*rho*v_0*e)

    N_sigma[i] = sigma[i]/A**4 * (m_x + m_a)**2/(m_x + N)**2

    #print(sigma)


plt.plot(M_X/1e6,N_sigma)
plt.xlim(5e-1,1e3)
plt.xscale("log")
plt.yscale("log")
plt.show()

