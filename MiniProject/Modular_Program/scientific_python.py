import numpy as np

a = np.array([2,3,4,5,6,7])


print(a.data)


# @ operator is a matrix multiplication or .dot()

#reshape is returned, resize is in place

#hstack add them side by side, vstack add then on top of each other

#array split unequal size splitting

#e[..., 1] all : until last 1

#slice by array, can take individial elemnts and put them in a new shaped array - have to index with a numpy array

#can cretae boolean masks, e.g. a[a>4]


#python is always pass by reference!!

#b = a is not a copy, b refers to a

#c = a.view() - c is not a, changing c doesnt change a, share same data
#deep copy - .copy dont share data

#scipy .eig will return h-stacked eigenvectors and a list of eigenvaluues


#===========================================================================#
"""
HEP python

pyROOT - import ROOT basically root so no advantage to using it

rootpy - layer on top of pyROOT making it pythonic - can use python libraries with it drop in replacement for pyROOT
       - easier histogramming - Hist, Hist2D etc.need to give it keyword arguments for titles and names - can slice histograms
       - tree = Tree("test") tree.create_branches(dict of branches)
       - tree.b.evt_number etc
       - myhist=myfile.somedirectory.histname.Clone()
       - myfile.walk():
       
uproot - i/o reading and writing rootfiles - converts to numpy arrays
       - uproot.open() need request package for http reading
       - creates a dictionary structure
       - e.g. root_file["one"]["two"]["tree"] = root_file["one/two/tree"]
       - .show method shows branches
       - multiple different ways of getting branches  - e.g. ttree.arrays(["px1","py1","pz1"]) - can also use regex - p[xyz]1
       - namedecode="utf-8" gets me strings not bytes in names of branches
       - outputtype allows for unpacking - can use df = tree.pandas.DataFrame
       - outputtype also allows directly casting through a function
       - caching can cache results - like a dictionary ArrayCache("100 kB") stops reading when 100 kB
       - normal methods read a file and returns as a in-memory array
       - lazy arrays take same parameters and load all data as it is needed not all at once - can use caching to limit memory usage
       - persistvirtual=True - can then save the data but only the derived feature, original features are just pointers to original rootfiles - can select specific values within a dataset e.g data[data["mass"] > 80]
       - iteration - pulls out values as you go along can do events.iterate
       - can iterate through files in the same way like TChains - give name of tree and name of branches
       - can limit number of values read entrystart and entrystop can prevent everything being read in
       - can do steps with lazy arrays - memory size string
       - can create root files - uproot.create, uproot.recreate, uproot.update - numpy histograms convert directly to root histograms 
       - can create trees of basic types - can give the newtree a dictionary with names and types or data
       
MINUIT - iminuit minimisation package 
       - have to give the function you want minimising to minuit e.g. least_squares, takes function arguments and has them as keyword arguments
       - errordef = 1 for OLS 0.5 for negative log liklihood
       - call migrad() to fit
       - can limit - limit_<name> 
       - can fix - fix_<name> = True
       - can do Minuit.from_array_functions - no more names as keyword arguments - its all tuples  or lists of tuples
       - migrad returns MigradResult - some metadata and some parameters
       - can see correlations and covariances
       - m.get_param_states()
       - m.np_merrors() good for matplotlib errorbars
       - can get contours at specific sigma
       - m.draw_mncontour('a','b',nsigma=4)
       
       
       
       
    
       






"""
