from graph import Graph
import landmarks
from shortest_paths import *

g = Graph()
g.build_from_csv('Seoul_Edgelist.csv')
#g.build_from_csv('test_graph.csv')

experiment_nodes = [292873, 198108, 259003, 280844, 98162, 249220]
num_landmarks = [4, 8, 12, 16]

for method in ['FARTHEST', 'RANDOM']:
    for num in num_landmarks:
        print('Selecting', num, 'landmarks with method ' + method + '...')
        g.select_landmarks(num)
        print('Selection done!')

        for node in experiment_nodes:
            print('Current node:', node)
            print('Running Dijkstra...')
            dijkstra(g, source=0, dest=node, skip_memo=True)
            print('Distance Dijkstra =', g.get_dist(0, node))
            d = bidirectional_a_star(graph=g, source=0, dest=node)
            print('Distance A* =', d)
            print('---')
