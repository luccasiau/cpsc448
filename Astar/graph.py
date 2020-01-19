import pandas as pd

"""
Main class for Graph structure in this demo.

An object of this class will contain:
    - N: Number of nodes
    - edge: adjacency list of edges
    - rev_edge: adjacency list of the reverse graph
    - dist: dictionary in which dist[u][v] is the shortest path from u to v
    - landmarks: set of landmarks
"""
class Graph:
    def __init__(self):
        self.N = 0
        self.edge = {}
        self.rev_edge = {}
        self.dist = {}
        self.landmarks = set()
        pass

    # TODO: Add documentation about file format
    def build_from_csv(self, file_path):
        self.__init__()
        graph_df = pd.read_csv(file_path)

        for index, row in graph_df.iterrows():
            self.add_edge(row['START_NODE'], row['END_NODE'], row['LENGTH'])

    def add_edge(self, u, v, w):
        self.N = max(self.N, u, v)

        if u not in self.edge:
            self.edge[u] = []
        self.edge[u].append((v, w))

        if n not in self.rev_edge:
            self.rev_edge[v] = []
        self.rev_edge[v].append((u, w))

    # Returns a list of pairs (v, w) representing an edge (u, v) with cost w
    def get_neighbours(self, u, rev=False):
        if not rev:
            if u not in self.edge:
                return []
            return self.edge[u]
        else:
            if u not in self.rev_edge
                return []
            return self.rev_edge[u]

    # If there is a shortest distance memoized, it will return it. Otherwise,
    # it will return inf.
    def get_dist(self, u, v):
        if u not in self.dist: return float('inf')
        if v not in self.dist[u]: return float('inf')
        return self.dist[u][v]
