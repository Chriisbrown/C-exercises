import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import quandl
import seaborn as sns
from pandas.plotting import register_matplotlib_converters
register_matplotlib_converters()

quandl.ApiConfig.api_key = 'L81xsbyCVXpsNvcAq81L'
#created_time={'gte': '2016-12-30','lte': '2016-12-31'}

start_date = '2016-12-28'
end_date = '2016-12-29'

sentiment_data = quandl.get_table('SMA/FBP', brand_ticker='GOOGL', created_time={'gte': start_date,'lte': end_date} , qopts = {'columns': ["date","engagement_score","impressions","total-reactions","shares","comments","sentiment"] } , paginate=True)
price_data = quandl.get('EOD/GOOGL', start_date=start_date, end_date=end_date)
sentiment_data.set_index(["date"])
dates  = price_data.index

for date in dates:
    print(date)

    sentiment_data_day = sentiment_data[sentiment_data["date"] == date]
    price_data_day = price_data[price_data.index == date]

    print(sentiment_data_day)

    parameters = np.array([sentiment_data_day["engagement_score"],sentiment_data_day["impressions"],sentiment_data_day["total-reactions"]])
    indicator = np.array(price_data_day["Open"]/price_data_day["Close"])

    print(parameters)
    print(indicator)