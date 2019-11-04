import quandl
import pandas as  pd
import numpy as np
import matplotlib.pyplot as plt

from pandas.plotting import register_matplotlib_converters
register_matplotlib_converters()


start_date ='2018-10-01'
end_date='2019-10-01'


quandl.ApiConfig.api_key = 'L81xsbyCVXpsNvcAq81L'

def sentiment_compare(parent_ticker,daughter_ticker):

    parent_data = quandl.get_table('SMA/FBP', brand_ticker=parent_ticker, created_time={'gte': start_date,'lte': end_date} ,
                                   qopts = {'columns': ["date","engagement_score","impressions","total-reactions","shares","comments","sentiment"] },
                                   paginate=True)
    daughter_data = quandl.get_table('SMA/FBP', brand_ticker=daughter_ticker, created_time={'gte': start_date,'lte': end_date}, 
                                    qopts = {'columns': ["date","engagement_score","impressions","total-reactions","shares","comments","sentiment"] },
                                    paginate=True)

    parent_price_data = quandl.get('EOD/'+str(parent_ticker), start_date=start_date, end_date=end_date)
    daughter_price_data = quandl.get('EOD/'+str(daughter_ticker), start_date=start_date, end_date=end_date)
    dates  = parent_price_data.index

    parent_p = np.zeros([len(dates)])
    daughter_p = np.zeros([len(dates)])

    for i,date in enumerate(dates):
        print(i,date)

        parent_data_day = parent_data[parent_data["date"] == date]
        daughter_data_day = daughter_data[daughter_data["date"] == date]

        pos_parent_data_day = parent_data_day[parent_data_day["sentiment"] == "POSITIVE"]
        pos_daughter_data_day = daughter_data_day[daughter_data_day["sentiment"] == "POSITIVE"]

        try:

            pos_parent_p[i] = np.mean(np.array([pos_parent_data_day["total-reactions"]]))
            pos_daughter_p[i] = np.mean(np.array([pos_daughter_data_day["total-reactions"]]))
            _parent_p[i] = np.mean(np.array([pos_parent_data_day["total-reactions"]]))
            pos_daughter_p[i] = np.mean(np.array([pos_daughter_data_day["total-reactions"]]))

        except:
            parent_p[i] = 0
            daughter_p[i] = 0

        #print(parent_p[i])
        #print(daughter_p[i])



    fig, ax1 = plt.subplots()


    ax1.set_xlabel('date')
    ax1.set_ylabel('total-reactions: '+str(parent_ticker), color='r')
    ax1.plot(dates, parent_p, color='r')
    ax1.tick_params(axis='y', labelcolor='r')

    ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

    color = 'tab:blue'
    ax2.set_ylabel('total-reactions: '+str(daughter_ticker), color=color)  # we already handled the x-label with ax1
    ax2.plot(dates, daughter_p, color=color)
    ax2.tick_params(axis='y', labelcolor=color)

    fig.tight_layout()  # otherwise the right y-label is slightly clipped
    plt.show()


sentiment_compare("AAPL","GOOGL")

