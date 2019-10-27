import uproot
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import numpy as np

"""
Python data analysis script that takes a root file of NTuples, loads them into a pandas dataframe and fits them to gaussians
"""

def fitting(name,data,error,range1,range2,mu,sigma,ax):
    data_entries,bins = np.histogram(data,range=[range1,range2],weights=data/error,bins=100,density=True) 
    # histogram data, in provided range, weighted on the realtive error, larger error has a smaller weight
    binscenters = np.array([0.5 * (bins[i] + bins[i+1]) for i in range(len(bins)-1)])
    # np.histogram provides bin edges not centres, this calculates them
    popt,pcov = curve_fit(fit_function,xdata=binscenters, ydata=data_entries, p0=[20000,mu,sigma])
    # Scipy curve fitting on a provided fit function - a guassian provided below based on the bin centres and the height of the bins. p0 is the best guess
    print(name)

    print(r'mu=%.6f' % (popt[1], ), r'sigma=%.6f' % (np.abs(popt[2], )))

    #Print the name and the gaussian parameters
    textstr = '\n'.join((
                    r'$\mu=%.6f$' % (popt[1], ),
                    r'$\sigma=%.6f$' % (np.abs(popt[2], ))))
    props = dict(facecolor='white', alpha=0.5)

    ax.text(0.05, 0.95, textstr, transform=ax.transAxes, fontsize=14,
                                    verticalalignment='top', bbox=props)

    # Plotting based features, allows the mean and sigma to be displayed on the plot

    x = np.linspace(range1,range2,1000)
    ax.hist(data,range=[range1,range2],weights=data/error,bins=100,color="darkblue",histtype = "step",density=True,label=r'Histogram entries')
    ax.plot(x, fit_function(x, *popt), color='darkorange', linewidth=1.0, label=r'Fitted function')
    #Histogram the data again with the same parameters, allows for nice formating against the alternative of using a bar chart. And plot the fitted line


def fit_function(x, A, mu, sigma):
    return (A * np.exp(-1.0 * (x - mu)**2 / (2 * sigma**2)))
    #Gaussian function for fitting

fig,[ax1,ax2] = plt.subplots(1,2)
#set the fig and axes object

events = uproot.open("tracks.root")['ntuple']
a = events.arrays(["velocity","v_e","fit","f_e"] , outputtype=pd.DataFrame)
#Open the root file specifying the name of the ntuple to be used
# Export it into a dataframe with the given column names
print("Analysing",len(a), "events")
a.velocity = a.velocity*10e4
a.v_e = a.v_e*10e4
#scale the velocity

fitting("Drift Velocity",a["velocity"],a["v_e"],261,265,262.7106,4.356848e-02,ax1)
fitting("Track Angle",a["fit"],a["f_e"],0.0,0.35,0.156749,0.040079456,ax2)
#fit the drift velocity and the track angle based on the data in the ntuple

ax1.set_title("Drift Velocity Distribution")
ax2.set_title("Track Angle Distribution")
ax1.set_xlabel("Drift Velocity $(\mu m /  ns)$")
ax1.set_ylabel("Normalised Events")
ax2.set_xlabel("Track Angle (radians)")
ax1.set_xlim(261,265)
ax2.set_xlim(0.0,0.35)
ax2.legend(loc='best')
ax1.grid()
ax2.grid()
fig.tight_layout()
plt.show()

#Plot it