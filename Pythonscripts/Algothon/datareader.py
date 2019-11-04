import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import quandl
import seaborn as sns
from pandas.plotting import register_matplotlib_converters
register_matplotlib_converters()

quandl.ApiConfig.api_key = 'L81xsbyCVXpsNvcAq81L'
"""
SMA/FBD - date,
SMA/FBP - created_time , 
SMA/FBUP - created_time,
SMA/INSD - date, 
SMA/INSP - post_created_at, 
SMA/TWTD - date 
SMA/TWTT - tweet_created_at, 
MA/YTCD -  date,
SMA/YTVD - date

"""



sentiment_data = quandl.get_table('SMA/FBP', brand_ticker='GOOGL', created_time={ '2016-12-31' }, qopts = {'columns': ["date","engagement_score","impressions","total-reactions","shares","comments","sentiment"] } , paginate=True)
price_data = quandl.get('EOD/GOOGL', start_date='2016-12-31', end_date='2016-12-31')

parameters = np.array[sentiment_data["engagement_score"],["impressions"],["total-reactions"]]
indicator = np.array[price_data]


#clean_data = data.pivot(columns='sentiment')
#print(clean_data.head())
print(price_data.head())

data = data[data["engagement_score"] > 500]
data_pos = data[data["sentiment"] == "POSITIVE"]
data_neg = data[data["sentiment"] == "NEGATIVE"]


h = sns.relplot(x="date", y="total_reactions", hue="sentiment", data=data,kind="line")
#h = sns.relplot(x_vars=["date"], y_vars=["engagement_score"], data=data_pos,kind="line")
h.set(xlim=(data["date"].min(),data["date"].max()))


#g = sns.pairplot(data, hue="sentiment", palette="Set2", diag_kind="kde", height=2.5)

#print(data.head())

#ax = sns.heatmap(data["engagement_score","impressions","reach","shares","comments"])



plt.show()


