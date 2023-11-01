import argparse
import json
import requests
from bs4 import BeautifulSoup
from pathlib import Path

def get_homepage():
    homepage_path = Path("homepage.html")
    if not homepage_path.exists():
        header = {'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36'}
        data = requests.get("https://montrealgazette.com/category/news/", headers=header)
        
        with open(homepage_path, "w") as f:
            f.write(data.text)
        
    with open (homepage_path, "r") as f:
        return f.read()
    
def get_trending_article(url, index):
    article_path = Path("article_" + str(index) + ".html")
    if not article_path.exists():
        header = {'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36'}
        data = requests.get(url, headers=header)
        with open(article_path, "w") as f:
            f.write(data.text)

    with open(article_path, "r") as f:
        return f.read()

def scrape_article(article_html):
    soup = BeautifulSoup(article_html, "html.parser")
    title = soup.find("h1", {"id":"articleTitle"}).text
    publication_date = soup.find("span", {"class":"published-date__since"}).text
    author_span = soup.find("span", {"class":"published-by__author"})
    author = author_span.find("a").text
    blurb = soup.find("p", {"class":"article-subtitle"}).text
    article_info = {
        "title":title,
        "publication_date":publication_date,
        "author":author,
        "blurb":blurb
    }
    return article_info
    
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", type=str, help="output file", required=True)
    args = parser.parse_args()

    html = get_homepage()
    
    soup = BeautifulSoup(html, "html.parser")

    trending_container = soup.find("div", {"data-aqa":"widget-top-trending"})
    
    trending_articles = trending_container.find_all("a", {"class":"article-card__link"})

    trending_articles_links = []
    for article in trending_articles:
        trending_articles_links.append("https://montrealgazette.com" + article["href"])

    article_info = []
    for i, link in enumerate(trending_articles_links):
        article_data = get_trending_article(link, i)
        article_info.append(scrape_article(article_data))

    json.dump(article_info, open(args.o, "w"))
          
if __name__ == '__main__':
    main()

    