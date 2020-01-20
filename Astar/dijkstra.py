from heapq import *

def dijkstra(graph, source, rev=False):
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
        
    return dist
