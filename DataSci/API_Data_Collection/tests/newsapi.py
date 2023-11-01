import unittest
from .. import fetch_latest_news
from datetime import datetime, timedelta

API_KEY = "ec349f0bef4b4a1988f93fc8bfe5b71d"

class TestFetchNews(unittest.TestCase):
    def test_no_keywords(self):
        data = fetch_latest_news(API_KEY, [])
        self.assertEqual(data["status"], "error")

    def test_lookback_days(self):
        data = fetch_latest_news(API_KEY, ["trump"], lookback_days=5)
        self.assertEqual(data["status"], "ok")
        today = datetime.now()
        five_days_ago = (today - timedelta(days=5)).strftime("%Y-%m-%d")
        for article in data["articles"]:
            self.assertGreaterEqual(article["publishedAt"][:10], five_days_ago)

    def test_nonalpha_keywords(self):
        data = fetch_latest_news(API_KEY, ["Trump&"])
        self.assertEqual(data["message"], "The request is invalid.")

if __name__ == '__main__':
    unittest.main()