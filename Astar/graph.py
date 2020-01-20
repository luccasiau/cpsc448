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
            self.edge[u] = {}
        self.edge[u][v] = w

        if v not in self.rev_edge:
            self.rev_edge[v] = {}
        self.rev_edge[v][u] = w

    def get_edge(self, u, v, rev=False, pi=None):
        ref = self.edge
        if rev is True: rev = self.rev_edge

        if u not in ref: return float('inf')
        if v not in ref[u]: return float('inf')
        if pi is None: return ref[u][v]
        return ref[u][v] - pi[u] + pi[v]

    # Returns a list of pairs (v, w) representing an edge (u, v) with cost w
    def get_neighbours(self, u, rev=False):
        ref = self.edge
        if rev is True: ref = self.rev_edge
        
        if u not in ref: return []
        return ref[u].items()

    # If there is a shortest distance memoized, it will return it. Otherwise,
    # it will return inf.
    def get_dist(self, u, v):
        if u not in self.dist: return float('inf')
        if v not in self.dist[u]: return float('inf')
        return self.dist[u][v]

    # Sets directed distance from u to v as d
    def set_dist(self, u, v, d):
        if u not in self.dist:
            self.dist[u] = {}
        self.dist[u][v] = d
