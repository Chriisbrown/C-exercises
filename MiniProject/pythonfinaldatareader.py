import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("test.txt",sep=',')
print(df.describe())

plt.hist(df["drift_velocity"],bins=100,range=(0.0015,0.003))
plt.show()

plt.hist(df["fit_gradient"],bins=100,range=(-0.25,0.5))
plt.show()



