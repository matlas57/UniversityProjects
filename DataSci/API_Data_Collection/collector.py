import argparse
import json
from newsapi import fetch_latest_news
from pathlib import Path
import os

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-k", type=str, help="api key", required=True)
    parser.add_argument("-b", type=int, help="optional: days to look back", required=False)
    parser.add_argument("-i", type=valid_json, help="input file with keywords", required=True)
    parser.add_argument("-o", type=str, help="output directory", required=True)
    args = parser.parse_args()

    print("All args are validated")

    if(not os.path.exists(args.o)):
        os.mkdir(args.o)

    input = json.load(open(args.i))
    for key, value in input.items():
        data = fetch_latest_news(args.k, value, args.b)
        output_path = Path(args.o) / (key + ".json")
        json.dump(data, open(output_path, "w"))

def valid_json(path):
    try:
        json.load(open(path))
        return path
    except:
        raise argparse.ArgumentTypeError("Invalid json file")


if __name__ == '__main__':
    main()
