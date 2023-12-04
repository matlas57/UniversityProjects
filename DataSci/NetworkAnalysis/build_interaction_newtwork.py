import networkx as nx
import pandas as pd
import argparse
import os
import json
import matplotlib.pyplot as plt

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", type=valid_csv, help="input csv script file", required=True)
    parser.add_argument("-o", type=str, help="output json file", required=True)
    args = parser.parse_args()

    # import csv file into pandas dataframe
    data = pd.read_csv(args.i)
    build_network(data, args.o)


def build_network(df, json_file):
    network = nx.Graph()

    # Find the top 101 valid ponies for nodes
    exclude = ['Ponies', 'Others', 'All']
    top_ponies = df['pony'].value_counts().index.tolist()
    top_ponies_filtered = [pony for pony in top_ponies if pony not in exclude]
    top_ponies_filtered = [pony for pony in top_ponies_filtered if ' and ' not in pony]
    top_101_ponies = top_ponies_filtered[:101]

    # Add nodes to network
    network.add_nodes_from(top_101_ponies)

    # Add edges to network
    for ind in df.index:
        if (df['pony'][ind] in top_101_ponies and df['pony'][ind + 1] in top_101_ponies and df['pony'][ind] != df['pony'][ind + 1] and df['title'][ind] == df['title'][ind + 1]):
            if (network.has_edge(df['pony'][ind], df['pony'][ind + 1])):
                network[df['pony'][ind]][df['pony'][ind + 1]]['weight'] += 1
            else:
                network.add_edge(df['pony'][ind], df['pony'][ind + 1], weight=1)
            
    json_data = {}
    for node in network.nodes:
        edges = network.edges(node, data=True)
        interactions = {}
        for edge in edges:
            interactions[edge[1]] = edge[2]['weight']
        json_data[node.lower()] = interactions
        
    with open(json_file, 'w') as f:
        json.dump(json_data, f, indent=4)

    #draw_network(network)

def draw_network(G):
    pos = nx.spring_layout(G)  # You can use different layout algorithms
    nx.draw(G, pos, with_labels=True, font_weight='bold', node_color='lightblue', node_size=800, font_size=10)
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
    plt.savefig("graph_image.png")

def valid_csv(path):
    try:
        pd.read_csv(path)
        return path
    except:
        raise argparse.ArgumentTypeError("Invalid csv file")

def valid_json(path):
    try:
        json.load(open(path))
        return path
    except:
        raise argparse.ArgumentTypeError("Invalid json file")

if __name__ == "__main__":
    main()