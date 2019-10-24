import uproot
import pandas as pd

events = uproot.open("FitParam.root")['T']


branches= events['Drift_Velocity']


print(events.keys())
print(events.show())

branches.arrays("Drift_Velocity" , outputtype=pd.DataFrame, entrystop=10)

print(events.head())

