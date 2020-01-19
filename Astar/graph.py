import pandas as pd

class Graph:
    def __init__(self):
        self.N = 0
        self.adj = {}
        self.dist = {}
        self.landmarks = set()
        pass

    def build_from_csv(self, file_path):
        self.__init__()
        graph_df = pd.read_csv(file_path)

        for index, row in graph_df.iterrows():
            self.add_edge(row['START_NODE'], row['END_NODE'], row['LENGTH'])

    def add_edge(self, u, v, w):
        self.N = max(self.N, u, v)

        if u not in self.adj:
            self.adj[u] = []
        self.adj[u].append((v, w))

    def get_neighbours(self, u):
        if u not in self.adj:
            return []
        return self.adj[u]

    def get_dist(self, u, v):
        if u not in self.dist: return float('inf')
        if v not in self.dist[u]: return float('inf')
        return self.dist[u][v]
