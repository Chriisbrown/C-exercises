import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate

def exponent(E,E_0,r):
    return np.exp(-E/(E_0*r))

def velocity_func(v,v_e,v0):
    return 4*np.pi*(np.exp((-(v+v_e)**2)/(v0)**2))*v**2

vf = lambda v,v_e,v0: 4*np.pi*(np.exp((-(v+v_e/3e10)**2)/(v0/3e10)**2))*v**2


N = 931.5e3
A = (0.26909*129 + 0.21232*131 + 0.26909*132 + 0.10436*134 + 0.08857*136 + 0.04071*130 + 0.01910*128)
rho = 0.3*1e6
v_0 = 220*1e5


m_a = N*A



def line_plotter(lim,v_esc,v_e):

    k = integrate.quadrature(vf,a=0,b=lim/3e10,args=(v_e,v_0))[0]*(3e10)**3


    M_X = np.linspace(5e6,5000e6,10000)
    sigma = np.ones(10000)

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

        e = (exponent(E_max,E_0,r) - exponent(E_min,E_0,r)) - exponent(v_esc**2,v_0**2,1)

        sigma[i] = (-R*m_x*m_a) / (1/k*np.pi*((v_0**4)*2*rho*e))

        #sigma[i] = -R*m_x*m_a*(np.pi)**(1/2) / (2*rho*v_0*e)

        #print(sigma)


    plt.plot(M_X/1e6,sigma)
        

line_plotter(544*1e5,544*1e5,230*1e5)
line_plotter(544*1e5,np.inf,0)

plt.xlim(5e-1,1e3)
plt.xscale("log")
plt.yscale("log")
plt.show()



