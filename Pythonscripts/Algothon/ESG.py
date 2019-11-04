RESOURCE_ENDPOINT = 'https://dsa-stg-edp-api.fr-nonprod.aws.thomsonreuters.com/data/environmental-social-governance/v1/views/measures-full'
access_token = '99LorCIWxd4GCVHhmOZqS6LRLQPSAMlK5CtWhv8j'
import requests
import json
import pandas as pd
import math
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

pd.set_option('display.max_columns', None)

def get_data_request(url, requestData):
    '''HTTP GET request'''
    dResp = requests.get(url, headers = {'X-api-key': access_token}, params = requestData)  

    if dResp.status_code != 200:
        raise ValueError("Unable to get data. Code %s, Message: %s" % (dResp.status_code, dResp.text))
    else:
        print("Data access successful")
        jResp = json.loads(dResp.text)
        return jResp

    
def get_data(ric):
    '''Gets ESG scores for a specific RIC (company) code'''
    
    requestData = {
    "universe": ric
    };

    jResp = get_data_request(RESOURCE_ENDPOINT, requestData)

    data = jResp["data"]
    headers = jResp["headers"]    

    names = [headers[x]['title'] for x in range(len(headers))]

    df = pd.DataFrame(data, columns=names )
    
    return df


esg = get_data("AAPL.OQ")

esg = esg.sort_values(by='Period End Date')
esg['Period End Date'] = pd.to_datetime(esg['Period End Date'])

cols = esg.columns.to_series().groupby(esg.dtypes).groups
floatcols = cols[np.dtype('float64')]
floatcols = floatcols[:-1]



