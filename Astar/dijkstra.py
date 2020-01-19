from heapq import *

def dijkstra(graph, source):
    # Memoizing dijkstra in my graph
    if source in graph.dist:
        return

    dist = {source: 0}
    q = [(0, source)]
    while q:
        (cost, cur) = heappop(q)
        if dist[cur] < cost:
            continue

        for (v, w) in graph.get_neighbours(cur):
            if (v not in dist) or (dist[v] > w + dist[cur]):
                dist[v] = w + dist[cur]
                heappush(q, (dist[v], v))

    graph.dist[source] = dist
    return dist
