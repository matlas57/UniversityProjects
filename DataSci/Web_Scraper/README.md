# Web Scraper 
This script is a web scraper that scrapes and produces a summary of the top 5 trending articles on [montrealgazette.com](https://montrealgazette.com/category/news/).
To do this I used many python libraries for seamless use of the script

## Usage
This script uses argparse, therefore it runs similar to any CLI command:
  python collect_trending.py -o trending.json
Where trending.json is an output file where the article summaries will be written to

## Possible Issues
If you obtain a 403 error, this may because of the User-Agent for the GET request, to fix this simply replace the User-Agent on lines 10 and 22 with your own
