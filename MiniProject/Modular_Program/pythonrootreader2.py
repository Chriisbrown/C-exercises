import uproot
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import numpy as np

def fit_function(x, A, mu, sigma):
    return (A * np.exp(-1.0 * (x - mu)**2 / (2 * sigma**2)))


fig,[ax1,ax2] = plt.subplots(1,2)

events = uproot.open("tracks.root")['ntuple']


a = events.arrays(["velocity","v_e","fit","f_e"] , outputtype=pd.DataFrame)

def fitting(name,data,error,range1,range2,mu,sigma,ax):
    data_entries,bins = np.histogram(data,range=[range1,range2],weights=error,bins=100,density=True)
    binscenters = np.array([0.5 * (bins[i] + bins[i+1]) for i in range(len(bins)-1)])

    popt,pcov = curve_fit(fit_function,xdata=binscenters, ydata=data_entries, p0=[20000,mu,sigma])

    print(name)

    print("Mean: ",popt[1], "Sigma: ",np.abs(popt[2]))

    textstr = '\n'.join((
                    r'$\mu=%.6f$' % (popt[1], ),
                    r'$\sigma=%.6f$' % (np.abs(popt[2], ))))
    props = dict(facecolor='white', alpha=0.5)

    ax.text(0.05, 0.95, textstr, transform=ax.transAxes, fontsize=14,
                                    verticalalignment='top', bbox=props)



    x = np.linspace(range1,range2,100)
    ax.hist(data,range=[range1,range2],weights=error,bins=100,color="darkblue",histtype = "step",density=True,label=r'Histogram entries')
    ax.plot(x, fit_function(x, *popt), color='darkorange', linewidth=1.0, label=r'Fitted function')



fitting("Drift Velocity",a["velocity"],a["v_e"],0.00260,0.002650,0.002627106,4.356848e-06,ax1)
fitting("Track Angle",a["fit"],a["f_e"],0.0,0.35,0.156749,0.040079456,ax2)



ax1.set_title("Drift Velocity Distribution")
ax2.set_title("Track Angle Distribution")


    

ax1.set_xlabel("Drift Velocity $(m / \mu s)$")
ax1.set_ylabel("Normalised Events")
ax2.set_xlabel("Track Angle (radians)")
ax1.set_xlim(0.00260,0.002650)
ax2.set_xlim(0.0,0.35)
ax2.legend(loc='best')
ax1.grid()
ax2.grid()
fig.tight_layout()
plt.show()


