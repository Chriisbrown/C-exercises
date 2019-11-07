import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate
from scipy import special

N = 1.276e-27*6.02e26*1e6
A = (0.26909*129 + 0.21232*131 + 0.26909*132 + 0.10436*134 + 0.08857*136 + 0.04071*130 + 0.01910*128)
rho = 0.3*1e6
v_0 = 22 / 3e4
v_e = 24 / 3e4
v_min = 0
v_max = np.inf


detector_mass = 5600*1/1.276e-27*1e6
detector_time = 1000*86400
detector_range = (5,30)
number_of_events = 4


v_max = np.inf

M_X = np.linspace(5e6,5000e6,10000)
sigma = np.ones(10000)

def velocity(v,v_e,v_0):
    return np.exp(-(v+v_e)**2/v_0**2)*v

I_V = integrate.quad(velocity,a=v_min,b=v_max,args=(v_e,v_0))

for i,m_x in enumerate(M_X):


    I_V = integrate.quad(velocity,a=v_min,b=v_max,args=(v_e,v_0))


    v_min = np.sqrt(2*detector_range[1]/m_x)

    R0 = (2/np.sqrt(np.pi) ) * (N/A )* (rho/m_x) * v_0

    Vs = (np.exp(-v_min/v_0)) - (np.exp(-v_max/v_0))
    

    Rate = number_of_events/detector_time/detector_mass

    sigma[i] = Rate/(R0*Vs)

plt.plot(M_X/1e6,sigma)
plt.xlim(5e-1,1e3)
plt.ylim(1e-50,1e-36)
plt.xscale("log")
plt.yscale("log")
plt.show()

