from heapq import *
from lower_bound import get_average_potential_functions

def dijkstra(graph, source, rev=False):
    memo_idx = (source+1) * (-1 if rev else 1)
    if memo_idx in graph.dijkstra_memo:
        return

    dist = {source: 0}
    q = [(0, source)]
    while q:
        (cost, cur) = heappop(q)
        if dist[cur] < cost:
            continue

        for (v, w) in graph.get_neighbours(cur, rev):
            if (v not in dist) or (dist[v] > w + dist[cur]):
                dist[v] = w + dist[cur]
                heappush(q, (dist[v], v))

    for (v, d) in dist.items():
        if rev: graph.set_dist(v, source, dist[v])
        else: graph.set_dist(source, v, dist[v])

    graph.dijkstra_memo.add(memo_idx)

# For now: this REQUIRES all the landmarks to have been set.
def bidirectional_a_star(graph, source, dest):
    pi_s, pi_t = get_average_potential_functions(graph, source, dest)
    # TODO: Consider making potential functions an input.
    # TODO: Finish
    pass