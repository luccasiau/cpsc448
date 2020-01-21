from graph import Graph
import landmarks
from shortest_paths import *

g = Graph()
g.build_from_csv('Seoul_Edgelist.csv')
#g.build_from_csv('test_graph.csv')

g.select_landmarks()
print('Done selecting landmarks')

print(bidirectional_a_star(graph=g, source=0, dest=292873))