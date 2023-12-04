import argparse
from pathlib import Path
import json
import os
import math

ponies = ['Twilight Sparkle', 'Applejack', 'Rarity', 'Pinkie Pie', 'Rainbow Dash', 'Fluttershy']

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-c", type=valid_json, help="output json file", required=True)
    parser.add_argument("-n", type=int, help="number of words to compute the tf-idf score of", required=True)
    args = parser.parse_args()

    with open(args.c) as f:
        word_counts = json.load(f)
    print("{")
    for pony in ponies:
        pony_word_counts = word_counts[pony.lower()]
        tfidf = {}
        for word in list(pony_word_counts):
            tfidf[word] = compute_tfidf(word, pony.lower(), args.c)
        
        sorted_dict = dict(sorted(tfidf.items(), key=lambda item: item[1], reverse=True))
        result_string = '"' + pony.lower() + '"' +  ": ["
        for i in range(args.n):
            if (i == args.n - 1):
                result_string += '"' + list(sorted_dict.keys())[i] + '"'
            else:               
                result_string += '"' + list(sorted_dict.keys())[i] + '"' + ", "
        result_string += "],"
        print(result_string)
    print("}")

def compute_tfidf(word, pony, data):
    with open (data) as f:
        word_counts = json.load(f)
    tf = word_counts[pony][word]
    num_ponies_used_word = 0
    for pony in word_counts:
        if (word in word_counts[pony]):
            num_ponies_used_word += 1
    idf = math.log(len(ponies) / num_ponies_used_word)
    return tf * idf
    

def valid_json(path):
    try:
        json.load(open(path))
        return path
    except:
        raise argparse.ArgumentTypeError("Invalid json file")

if __name__ == '__main__':
    main()