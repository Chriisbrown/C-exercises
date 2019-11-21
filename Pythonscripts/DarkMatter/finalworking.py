import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate
import matplotlib.ticker as ticker

params = {'legend.fontsize': 'large',
         'figure.figsize': (15, 10),
         'axes.labelsize': 'xx-large',
         'axes.titlesize':'xx-large',
         'xtick.labelsize':'x-large',
         'ytick.labelsize':'x-large'}
plt.rcParams.update(params)






def velocity_norm_func(v,v_e,v_0):
    return 2*np.pi*(v_0**2)*np.exp(-(v+v_e)**2/v_0**2)

def energy_func(E,E_0,r,v_max,v_0):
    return np.exp(-E/(E_0*r)) - np.exp(-v_max**2/v_0**2)


F = lambda q,r_n: ((3/((q*r_n)**3))*(np.sin(q*r_n) - q*r_n*np.cos(q*r_n)))*np.exp((-(q*0.9e-12)**2)/2)
#F = lambda q,r_n: np.exp((-(q*r_n)**2)/3)
#F = lambda q,r_n: np.sin(-q*r_n)/(q*r_n)

def integral_function (e,r_n,e_0,r,m_a,v_max):
    q = np.sqrt(m_a*2*e)/197.3e3
    a = F(q,r_n)**2
    return a*energy_func(e,e_0,r,v_max,v_0)







v_0 = 220

rho = 0.3e6

E_min = 5
E_max = 30
events = 7.1
Detector_mass = 5600*1/1.782e-33
Detector_time = 1000*86400*0.5
Rate = events * 1/Detector_mass * 1/Detector_time


m_p = 0.9315e6#proton mass KeV
A = (0.26909*129 + 0.21232*131 + 0.26909*132 + 0.10436*134 + 0.08857*136 + 0.04071*130 + 0.01910*128) 
m_a = m_p*A
m_p = 0.938e6 
M_X = np.linspace(5e6,5000e6,10000)


def no_assumptions():
    sigma_p = np.zeros(10000)
    for i,m_x in enumerate(M_X):

        v_esc = np.inf
        v_e = 0

        mu_a = m_x*m_a/(m_x+m_a)
        mu_p = m_x*m_p/(m_x+m_p)


        E_0 = 1/2*m_x*(v_0/3e5)**2

        r = 4*m_x*m_a/(m_x+m_a)**2


        k = integrate.quad(velocity_norm_func,a = 0,b = v_esc,args=(v_e,v_0))[0]

        I = integrate.quad(energy_func, a = E_min, b= E_max, args=(E_0,r,v_esc,v_0))[0]


        prefactor = I*rho*np.pi*1/(k*1e5**3)*(3e10*v_0*1e5)**2/(m_x*mu_a**2) 

        sigma = Rate/prefactor

        sigma_p[i] = sigma*mu_p**2/(mu_a**2*A**2)
    plt.plot(M_X/1e6,sigma_p,label="All Assumptions                  V esc $= \infty$, V earth $= 0$, $F(q^2) = 1$")

def trunc_v():
    sigma_p = np.zeros(10000)
    for i,m_x in enumerate(M_X):

        v_esc = 544
        v_e = 0

        mu_a = m_x*m_a/(m_x+m_a)
        mu_p = m_x*m_p/(m_x+m_p)


        E_0 = 1/2*m_x*(v_0/3e5)**2

        r = 4*m_x*m_a/(m_x+m_a)**2


        k = integrate.quad(velocity_norm_func,a = v_e,b = v_esc,args=(v_e,v_0))[0]

        I = integrate.quad(energy_func, a = E_min, b= E_max, args=(E_0,r,v_esc,v_0))[0]


        prefactor = I*rho*np.pi*1/(k*1e5**3)*(3e10*v_0*1e5)**2/(m_x*mu_a**2) 

        sigma = Rate/prefactor

        sigma_p[i] = sigma*mu_p**2/(mu_a**2*A**2)

        if sigma_p[i] < 1e-49:
            sigma_p[i] = "nan"

    plt.plot(M_X/1e6,sigma_p,label="Truncated Velocity             V esc $ \\neq  \infty$, V earth $= 0$, $F(q^2) = 1$")


def simple_approx():
    sigma_p = np.zeros(10000)
    for i,m_x in enumerate(M_X):

        v_esc = 544
        v_e = 0

        mu_a = m_x*m_a/(m_x+m_a)
        mu_p = m_x*m_p/(m_x+m_p)
        c1 = 0.751
        c2 = 0.561

        E_0 = 1/2*m_x*(v_0/3e5)**2

        r = 4*m_x*m_a/(m_x+m_a)**2


        k = integrate.quad(velocity_norm_func,a = v_e,b = v_esc,args=(v_e,v_0))[0]

        I = integrate.quad(energy_func, a = E_min, b= E_max, args=(E_0/c2,r,v_esc,v_0))[0]


        prefactor = c2/c1*I*rho*np.pi*1/(k*1e5**3)*(3e10*v_0*1e5)**2/(m_x*mu_a**2) 

        sigma = Rate/prefactor

        sigma_p[i] = sigma*mu_p**2/(mu_a**2*A**2)

        if sigma_p[i] < 1e-49:
            sigma_p[i] = "nan"

    plt.plot(M_X/1e6,sigma_p,label="Non-Zero Earth Velocity     V esc $ \\neq  \infty$, V earth $ \\neq  0$, $F(q^2) = 1$")


def form_factor():
    sigma_p = np.zeros(10000)
    for i,m_x in enumerate(M_X):

        v_esc = 544
        v_e = 0

        mu_a = m_x*m_a/(m_x+m_a)
        mu_p = m_x*m_p/(m_x+m_p)
        c1 = 0.751
        c2 = 0.561

        r_n = np.sqrt((1.23*A**(1/3)-0.6)**2+ 7/3*np.pi**2*0.52**2 - 5*0.9**2 )
        """"

        Q = np.linspace(0,2*r_n,1000)
        plt.title("Form Factor")
        plt.plot(Q,F(Q,1)**2)
        plt.yscale("log")
        plt.ylim(1e-6,1)
        plt.xlim(0,10)
        plt.xlabel("$q r_n$")
        plt.ylabel("$F(q)^2$")
        plt.grid(which="major",linewidth=0.5)
        plt.grid(which="minor",linestyle="-",linewidth=0.2)
        plt.savefig("formfactor.png",dpi=600)
        plt.show()

        """

        E_0 = 1/2*m_x*(v_0/3e5)**2

        r = 4*m_x*m_a/(m_x+m_a)**2


        k = integrate.quad(velocity_norm_func,a = v_e,b = v_esc,args=(v_e,v_0))[0]

        I = integrate.quad(integral_function, a = E_min, b=E_max, args=(r_n,E_0/c2,r,m_a,v_esc))[0]


        prefactor = c2/c1*I*rho*np.pi*1/(k*1e5**3)*(3e10*v_0*1e5)**2/(m_x*mu_a**2) 

        sigma = Rate/prefactor

        

        sigma_p[i] = sigma*mu_p**2/(mu_a**2*A**2)

        if sigma_p[i] < 1e-49:
            sigma_p[i] = "nan"

    plt.plot(M_X/1e6,sigma_p,label="Non Zero Form Factor        V esc $ \\neq  \infty$, V earth $ \\neq  0$, $F(q^2) \\neq 1$")

no_assumptions()
trunc_v()
simple_approx()
form_factor()
plt.title("Spin-Independent Wimp-Nucleon Elastic Scattering Cross-Section Limits")
#plt.title("Spin-Independent Wimp-Nucleon Elastic Scattering Cross-Section Limits With Form Factor")
plt.xlim(5,5e3)
plt.ylim(1e-48,1e-44)
plt.xscale("log")
plt.yscale("log")
plt.ylabel("SI WIMP-nucleon cross section [cm$^2$]")
plt.xlabel("WIMP Mass [GeV]")
plt.legend()
plt.grid(which="major",linewidth=0.5)
plt.grid(which="minor",linestyle="-",linewidth=0.2)
#plt.savefig("every.png",dpi=600)
plt.show()







