import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate


M_X = np.linspace(5e6,5000e6,10000)
sigma = np.ones(10000)

for i,m_x in enumerate(M_X):

    rho = 0.3*1e6
    v0 = 220000 / 3e8
    m_t = 0.939e6*(0.26909*129 + 0.21232*131 + 0.26909*132 + 0.10436*134 + 0.08857*136 + 0.04071*130 + 0.01910*128)

    r = 4*m_x*m_t/(m_x+m_t)**2

    detector_mass = 5600*1/1.276e-27*1e6
    detector_time = 1000*86400
    detector_range = (5,30)
    number_of_events = 7.1

    v_m = np.sqrt(2*detector_range[1]/m_x)
    v_max = np.inf
    v_earth = 0

    form_factor = 1


    

    def velocity_function(v):
        return (np.exp(-(v+v_earth)**2/v0**2))*v*v

    #def integrated_velocity(v0,vmin,vmax,v_e):
    #    return 2*np.pi*v0**2*((velocity_function(vmin,-v_e,v0))-(velocity_function(vmax,-v_e,v0)))


    lb = v_m
    ub = v_max


    #I_V = integrate.quad(velocity_function,a=v_m,b=np.inf,args=(0,v0))

    I2_V = 4/(np.sqrt(np.pi)*v0**3)*integrate.quad(lambda v: integrate.quad(velocity_function,0,v)[0],lb,ub)[0]


    A = rho/(2*r)


    rate = number_of_events/detector_time/detector_mass

    sigma[i] = rate*1/A*1/I2_V

    #print(sigma[i])

plt.plot(M_X/1e6,sigma)
plt.xlim(5e-1,1e3)
plt.ylim(1e-50,1e-36)
plt.xscale("log")
plt.yscale("log")
plt.show()


