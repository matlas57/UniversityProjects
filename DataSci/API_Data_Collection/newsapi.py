from datetime import datetime, timedelta
import requests
import json

API_KEY = ""
QUERY_STRING_TEMPLATE = "https://newsapi.org/v2/everything?q={}&from={}&sortBy=publishedAt&language=en&apiKey={}"

def fetch_latest_news(api_key, news_keywords, lookback_days=10):
    news_keywords_string = "+".join(news_keywords)
    today = datetime.now()
    ten_days_ago = (today - timedelta(days=10)).strftime("%Y-%m-%d")
    query_string = QUERY_STRING_TEMPLATE.format(news_keywords_string, ten_days_ago, api_key)
    response = requests.get(query_string)
    return response.json()
