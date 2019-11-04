import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import quandl
import seaborn as sns
from pandas.plotting import register_matplotlib_converters
register_matplotlib_converters()

quandl.ApiConfig.api_key = 'L81xsbyCVXpsNvcAq81L'
#created_time={'gte': '2016-12-30','lte': '2016-12-31'}

start_date = '2019-01-1'
end_date = '2019-09-1'

facebook_admin_data = quandl.get_table('SMA/FBP', brand_ticker='GOOGL', created_time={'gte': start_date,'lte': end_date} , qopts = {'columns': ["date","engagement_score","impressions","total-reactions","shares","comments","sentiment"] } , paginate=True)
facebook_user_data = quandl.get_table('SMA/FBUP', brand_ticker='GOOGL', created_time={'gte': start_date,'lte': end_date} , qopts = {'columns': ["date","likes","shares","comments","sentiment"] } , paginate=True)
instagram_data = quandl.get_table('SMA/INSP', brand_ticker='GOOGL', post_created_at={'gte': start_date,'lte': end_date} , qopts = {'columns': ["date","engagement_score","comments_count","likes_count"] } , paginate=True)
twitter_data = quandl.get_table('SMA/TWTT',brand_ticker='GOOGL', tweet_created_at={'gte': start_date,'lte': end_date} , qopts = {'columns': ["date","engagement_score","retweet_count","replies_count","favorite_count"] } , paginate=True)
youtube_data = quandl.get_table('SMA/YTVD',brand_ticker='GOOGL', date={'gte': start_date,'lte': end_date} , qopts = {'columns': ["date","views_count","likes_count","dislikes_count","comments_count"] } , paginate=True)

price_data = quandl.get('EOD/GOOGL', start_date=start_date, end_date=end_date)
dates  = price_data.index

total = np.zeros([len(dates)])
indicator = np.zeros([len(dates)])

difference = np.zeros([len(dates)])

for i,date in enumerate(dates):
    print(i,date)

    facebook_admin_data_day = facebook_admin_data[facebook_admin_data["date"] == date]
    facebook_user_data_day = facebook_user_data[facebook_user_data["date"] == date]
    instagram_data_day = instagram_data[instagram_data["date"] == date]
    twitter_data_day = twitter_data[twitter_data["date"] == date]
    youtube_data_day = youtube_data[youtube_data["date"] == date]

    price_data_day = price_data[price_data.index == date]

    parameters = np.array([instagram_data_day["likes_count"],twitter_data_day["favorite_count"],youtube_data_day["views_count"]])
    indicator[i] = np.array(price_data_day["Open"]/price_data_day["Close"])[0]

    total[i] = (np.sum(parameters[0]) + np.sum(parameters[1]) + np.sum(parameters[2]))
    difference[i] = indicator[i]*total[i]


fig, ax1 = plt.subplots()


ax1.set_xlabel('date')
ax1.set_ylabel('total', color='r')
ax1.plot(dates, total, color='r')
ax1.tick_params(axis='y', labelcolor='r')

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

color = 'tab:blue'
ax2.set_ylabel('indicator', color=color)  # we already handled the x-label with ax1
ax2.plot(dates, indicator, color=color)
ax2.tick_params(axis='y', labelcolor=color)

fig.tight_layout()  # otherwise the right y-label is slightly clipped
plt.show()

plt.hist(difference)
plt.show()

