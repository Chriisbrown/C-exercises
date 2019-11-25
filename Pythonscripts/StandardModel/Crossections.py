import numpy as np
import matplotlib.pyplot as plt

params = {'legend.fontsize': 'large',
         'figure.figsize': (8, 10),
         'axes.labelsize': 'xx-large',
         'axes.titlesize':'x-large',
         'xtick.labelsize':'x-large',
         'ytick.labelsize':'x-large'}
plt.rcParams.update(params)

plus = lambda theta: 1 + np.cos(theta)
minus = lambda theta: 1 - np.cos(theta)



GG = lambda theta : 9/2*(3+(1/4*(plus(theta)*(minus(theta)) + 2*(plus(theta)/minus(theta)**2) + 2*minus(theta)/(plus(theta)**2))))

qq_ = lambda theta : 4/9*((4+ plus(theta)**2)/minus(theta)**2 + 1/4*minus(theta)**2 + 1/4*plus(theta)**2 - 8/27*plus(theta)**2/minus(theta))

qqd = lambda theta : 4/9*(1+np.cos(theta)**2)

THETA = np.linspace(np.pi/64,np.pi-np.pi/64,10000)




plt.plot(THETA,GG(THETA),label="$GG \\rightarrow GG$")
plt.plot(THETA,qq_(THETA),label="$q \\overline{q} \\rightarrow q \\overline{q}$")
plt.plot(THETA,qqd(THETA),label="$q \\overline{q} \\rightarrow q^\prime \\overline{q} ^\prime$")
plt.title("Differential Cross Sections at lowest order")
plt.legend()
plt.yscale("log")

plt.ylabel("$|\\overline{M}|^2$")
plt.ylabel("$\\frac{d\\sigma}{dcos(\\theta)}$")
plt.xlabel("$\\theta$ (radians)")

plt.grid(which="major",linewidth=0.5)
plt.grid(which="minor",linestyle="-",linewidth=0.2)

plt.show()
