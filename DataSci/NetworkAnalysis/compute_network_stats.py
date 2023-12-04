import argparse
import json
import os
import networkx as nx

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", type=valid_json, help="input json file", required=True)
    parser.add_argument("-o", type=str, help="output json file", required=True)
    args = parser.parse_args()

    with open(args.i, 'r') as file:
        adjacency_list = json.load(file)

    nodes = [{"id": node} for node in adjacency_list]
    links = [{"source": node, "target": neighbor, "weight": weight} 
         for node, neighbors in adjacency_list.items() 
         for neighbor, weight in neighbors.items()]

    data = {"nodes": nodes, "links": links}
    
    G = nx.json_graph.node_link_graph(data)
    compute_network_stats(G, args.o)

def compute_network_stats(G, output_file):
    stats = {}
    centrality = list(nx.degree_centrality(G).keys())[:3]
    stats["degree"] = centrality
    weighted_degree_centrality = list(dict(G.degree(weight='weight')).keys())[:3]   
    stats["weighted_degree"] = weighted_degree_centrality
    closeness = list(nx.closeness_centrality(G).keys())[:3]
    stats["closeness"] = closeness
    betweenness = list(nx.betweenness_centrality(G).keys())[:3]
    stats["betweenness"] = betweenness

    with open(output_file, 'w') as file:
        json.dump(stats, file, indent=4)

def valid_json(path):
    try:
        json.load(open(path))
        return path
    except:
        raise argparse.ArgumentTypeError("Invalid json file")


if __name__ == "__main__":
    main()