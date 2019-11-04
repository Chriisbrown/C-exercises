import pandas as pd
import quandl
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np
from sklearn.cluster import MeanShift

quandl.ApiConfig.api_key = 'L81xsbyCVXpsNvcAq81L'

start_date ='2018-10-01'
end_date='2019-10-01'

sentiment_data = quandl.get_table('SMA/FBP', brand_ticker='GOOGL', created_time={'gte': start_date,'lte': end_date} , paginate=True)

sentiment_data.drop(columns=['created_time','brand_ticker',"page_id","post_id","sector","geography","type","date"],inplace=True)
# )

ms=MeanShift()
ms.fit(sentiment_data)
labels=ms.labels_
cluster_centers=ms.cluster_centres_

n_clusters_=len(np.unique(labels))

print(n_clusters)


