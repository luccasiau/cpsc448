import random
from shortest_paths import dijkstra

# Graph is input graph, k is the number of landmarks to be defined.
# This is using farthest landmark selection algorithm.
# 
#
# Tweaks that I'm doing from the paper because of the condition of my data:
#    - I'm allowing the user to pick the initial vertex (instead of using rand)
#    - Afterwards, I'm only considering vertices in the same SCC from it
# These changes lead to more meaningful information (for me)
def find_farthest_landmarks(graph, k=4, initial=-1):
    k = min(k, graph.N)  # Sanity check
    start_vertex = (initial if (initial>=0) else random.choice(range(graph.N)))
    dijkstra(graph, start_vertex)
    
    max_dist, cur_choice = -1, -1
    for v in range(int(graph.N)):
        cur_dist = graph.get_dist(start_vertex, v)
        if cur_dist == float('inf'): continue
        if cur_dist > max_dist:
            cur_choice, max_dist = v, cur_dist 
            
    land = set([cur_choice])
    dijkstra(graph, cur_choice)
    while len(land) < k:
        max_dist, cur_choice = -1, -1
        for v in range(graph.N):
            cur_dist = float('inf')
            for l in land:
                cur_dist = min(cur_dist, graph.get_dist(l, v))
            if cur_dist == float('inf'): continue
            if cur_dist > max_dist:
                cur_choice, max_dist = v, cur_dist

        dijkstra(graph, cur_choice)
        land.add(cur_choice)

    return land

# Picks landmarks randomly from the graph.
def find_random_landmarks(graph, k=4):
    return random.sample(range(graph.N), k)