# API Data Collection Project
This project uses the API at [newsapi.org](https://newsapi.org/) and retrieves articles based on an input file of search keywords and outputs them into directories for each keyword category as specified in the input file.

## Usage
This script uses argparse so it is run similar to a CLI command, the usage is as follows
  Python -m newscover.collector -k <api_key> [-b <# days to lookback>] -i <input_file> -o <output_dir>
You can get your own api key at [newsapi.org](https://newsapi.org/)
the input file has the following format, it is a json file, here's an example
  { “trump_fiasco”: [“trump”, “trial”], “swift”: [“taylor”, “swift”, “movie”] ]
For each keyword set with name N and keyword list X, the collector will execute a query for the keywords X and write the results to the <output_dir>/N.json. 
