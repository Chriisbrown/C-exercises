import quandl
import pandas as  pd
import numpy as np
import matplotlib.pyplot as plt

gender_stats = pd.read_csv("Gender_Stats_csv/Gender_StatsData.csv", sep=',',skiprows=4)


print(gender_stats.head())