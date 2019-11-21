import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate

def truncated_v():
    vf = lambda v,v_e,v0: 4*np.pi*(np.exp((-(v+v_e)**2)/(v0)**2))*(v)**2

    sigma = []
    M_X = np.linspace(5e6,5000e6,10000)

    for i in range(len(M_X)):

        m_p = 0.9315e6#proton mass KeV
        A = (0.26909*129 + 0.21232*131 + 0.26909*132 + 0.10436*134 + 0.08857*136 + 0.04071*130 + 0.01910*128) 
        m_a = m_p*A 
        m_x = M_X[i]
        v_0 = 220
        v_e = 230

        c1 = 0.751
        c2 = 0.561

        v_esc = 544
        k = integrate.quad(vf,a=0,b=v_esc,args=(0,v_0))[0]

        rho = 0.3e6
        mu_a = m_x*m_a/(m_x+m_a)

        r = 4*m_a*m_x/(m_x+m_a)**2
        detector_mass = 5600*1/1.78266192e-27*1e6
        detector_time = 1000*86400


        events = 7.1

        rate = events * 1/detector_mass * 1/detector_time

        E_0 = (1/2*m_x*(v_0/3e5)**2)

        exponent = -(np.exp(-30*c2/(E_0*r)) -np.exp(-5*c2/(E_0*r)))  
 

        #print(exponent)

        prefactor = c1/c2 * exponent*rho*r*v_0**4*np.pi*1e5/(2*k*mu_a**2)
        #sigma.append(rate/prefactor)
        sigma.append((rate/prefactor)/A**4 * (m_x + m_a)**2/(m_x + m_p)**2)
        #print(sigma)

    plt.plot(M_X/1e6,sigma,label="trunc v approx")


def no_assumptions():


    sigma = []
    M_X = np.linspace(5e6,5000e6,10000)

    for i in range(len(M_X)):

        m_p = 0.9315e6#proton mass KeV
        A = (0.26909*129 + 0.21232*131 + 0.26909*132 + 0.10436*134 + 0.08857*136 + 0.04071*130 + 0.01910*128) 
        m_a = m_p*A 
        m_x = M_X[i]
        v_0 = 220

        rho = 0.3e6
        mu_a = m_x*m_a/(m_x+m_a)

        r = 4*m_a*m_x/(m_x+m_a)**2
        detector_mass = 5600*1/1.78266192e-27*1e6
        detector_time = 1000*86400


        events = 7.1

        rate = events * 1/detector_mass * 1/detector_time

        E_0 = (1/2*m_x*(v_0/3e5)**2)

        exponent = -(np.exp(-30/(E_0*r)) -np.exp(-5/(E_0*r)))   

        #print(exponent)

        prefactor = exponent*rho*r*v_0*1e5/(2*mu_a**2*np.sqrt(np.pi))
        #sigma.append(rate/prefactor)
        sigma.append((rate/prefactor)/A**4 * (m_x + m_a)**2/(m_x + m_p)**2)
        #print(sigma)

    plt.plot(M_X/1e6,sigma,label="no assumptions")

def  trunc_v():

    vf = lambda v,v_e,v0: 4*np.pi*(np.exp((-(v+v_e)**2)/(v0)**2))*(v)**2

    def integral_function (e,e_0):
        return np.exp(-e/(e_0))

    sigma = []
    M_X = np.linspace(5e6,5000e6,10000)

    for i in range(len(M_X)):

        m_p = 0.9315e6#proton mass KeV
        A = (0.26909*129 + 0.21232*131 + 0.26909*132 + 0.10436*134 + 0.08857*136 + 0.04071*130 + 0.01910*128) 


        m_a = m_p*A 
        m_x = M_X[i]
        v_0 = 220
        v_e = 230


        v_esc = 544
        k = integrate.quad(vf,a=0,b=v_esc,args=(v_e,v_0))[0]

        rho = 0.3e6
        mu_a = m_x*m_a/(m_x+m_a)

        r = 4*m_a*m_x/(m_x+m_a)**2
        detector_mass = 5600*1/1.78266192e-27*1e6
        detector_time = 1000*86400


        events = 7.1

        rate = events * 1/detector_mass * 1/detector_time

        E_0 = (1/2*m_x*(v_0/3e5)**2)

        exponent = integrate.quad(integral_function,a=5,b=30,args=(E_0*r))[0]

        #exponent = -(np.exp(-30*c2/(E_0*r)) -np.exp(-5*c2/(E_0*r)))*1/c2*E_0*r   
        #print(exponent)

        prefactor = exponent*rho*v_0**2*np.pi*5e15/(k*m_x*mu_a**2)
        #sigma.append(rate/prefactor)
        sigma.append((rate/prefactor)/A**4 * (m_x + m_a)**2/(m_x + m_p)**2)
        #print(sigma)

    plt.plot(M_X/1e6,sigma,label="trunc v")





def form_factor():
    #F = lambda q,r_n: (3/(q*r_n)**3*(np.sin(q*r_n) - q*r_n*np.cos(q*r_n)))**2 *np.exp(-q**2/0.9**2)
    F = lambda q,r_n: (((3/((q*r_n)**3)*(np.sin(q*r_n) - q*r_n*np.cos(q*r_n)))*np.exp(-(q*0.9)**2)/2))**2 
    #F = lambda q,r_n: np.exp((-(q*r_n)**2)/3)

    

    E = lambda en,e_0,r: np.exp(-en/(e_0*r))

    vf = lambda v,v_e,v0: 4*np.pi*(np.exp((-(v)**2)/(v0)**2))*(v)**2

    def integral_function (e,r_n,e_0,r,m_x):
        q = np.sqrt(m_x*2*e)/197.3e3
        a = F(q,r_n)
        return a*np.exp(-e/(e_0*r))

    
        

    sigma = []
    M_X = np.linspace(5e6,5000e6,10000)

    for i in range(len(M_X)):

        m_p = 0.9315e6#proton mass KeV
        A = (0.26909*129 + 0.21232*131 + 0.26909*132 + 0.10436*134 + 0.08857*136 + 0.04071*130 + 0.01910*128) 


        c = 1.23*A**(1/3) - 0.6
        a = 0.52
        s = 0.9
        R_n  = np.sqrt(c**2+7/3*np.pi**2*a**2 - 5*s**2)
        R_n = A**(1/3)

        m_a = m_p*A 
        m_x = M_X[i]
        v_0 = 220

        #G = np.linspace(5,30,10000)
        #Q = np.sqrt(m_x*2*G)/197.3e3
        #plt.plot(Q,F(Q,R_n))
        #plt.show()

        c1 = 0.751
        c2 = 0.561

        v_esc = 544
        k = integrate.quadrature(vf,a=0,b=v_esc,args=(0,v_0))[0]

        rho = 0.3e6
        mu_a = m_x*m_a/(m_x+m_a)

        r = 4*m_a*m_x/(m_x+m_a)**2
        detector_mass = 5600*1/1.78266192e-27*1e6
        detector_time = 1000*86400


        events = 7.1

        rate = events * 1/detector_mass * 1/detector_time

        E_0 = (1/2*m_x*(v_0/3e5)**2)

        exponent = integrate.quad(integral_function,a=5,b=30,args=(R_n,E_0/c2,r,m_x))[0]

        #exponent = -(np.exp(-30*c2/(E_0*r)) -np.exp(-5*c2/(E_0*r)))*1/c2*E_0*r   
        #print(exponent)

        prefactor = c1/c2 * exponent*rho*v_0**2*np.pi*5e15/(k*m_x*mu_a**2)
        #sigma.append(rate/prefactor)
        sigma.append((rate/prefactor)/A**4 * (m_x + m_a)**2/(m_x + m_p)**2)
        #print(sigma)

    plt.plot(M_X/1e6,sigma,label="form factor")




no_assumptions()
truncated_v()
trunc_v()
#form_factor()
plt.xlim(5,5e3)
plt.xscale("log")
plt.yscale("log")
plt.legend()
plt.show()