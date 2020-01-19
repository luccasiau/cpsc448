import pandas as pd

class Graph:
    def __init__():
        self.N = 0
        self.adj_ = {}
        pass

    def build_from_csv(self, file_path):
        self.__init__()
        graph_df = pd.read_csv(file_path)

        for index, row in graph_df.iterrows():
            self.add_edge(row['START_NODE'], row['END_NODE'], row['LENGTH'])

    def add_edge(self, u, v, w):
        self.N = max(self.N, u, v)

        if u not in self.adj_:
            self.adj_[u] = []
        self.adj_[u].append((v, w))

