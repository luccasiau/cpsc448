from heapq import *
from lower_bound import get_average_potential_functions

# If given a specific destination, Dijsktra will stop as soon as it is scanned.
def dijkstra(graph, source, dest=None, rev=False, skip_memo=False):
    memo_idx = (source+1) * (-1 if rev else 1)
    if skip_memo == False and memo_idx in graph.dijkstra_memo:
        return

    dist = {source: 0}
    q = [(0, source)]
    while q:
        (cost, cur) = heappop(q)
        if dist[cur] < cost:
            continue

        if cur == dest:
            break

        for (v, w) in graph.get_neighbours(cur, rev):
            if (v not in dist) or (dist[v] > w + dist[cur]):
                dist[v] = w + dist[cur]
                heappush(q, (dist[v], v))

    for (v, d) in dist.items():
        if rev: graph.set_dist(v, source, dist[v])
        else: graph.set_dist(source, v, dist[v])

    if dest is not None:
        print('Vertices scanned by Dijkstra:', len(dist))
    if dest is None:
        graph.dijkstra_memo.add(memo_idx)

# For now: this REQUIRES all the landmarks to have been set.
# Returns the shortest distance from source to dest.
def bidirectional_a_star(graph, source, dest, pi_s=None, pi_t=None):
    if pi_s == None and pi_t == None:
        pi_s, pi_t = get_average_potential_functions(graph, source, dest)
    
    forward_dist = {source: 0}
    reverse_dist = {dest: 0}
    forward_pq = [(0, source)]
    reverse_pq = [(0, dest)]
    forward_seen = set()
    reverse_seen = set()
    shortest_so_far = float('inf')

    while forward_pq or reverse_pq:
        cur_forward = -1
        while forward_pq:
            (_, cur_forward) = heappop(forward_pq)
            if cur_forward not in forward_seen: break 

        if cur_forward != -1:
            if cur_forward in reverse_seen: break
            forward_seen.add(cur_forward)
            for (v, w) in graph.get_neighbours(cur_forward):
                if v in reverse_seen or v == dest:
                    shortest_so_far = min(shortest_so_far,\
                        forward_dist[cur_forward] + w + reverse_dist[v])

                if (v not in forward_dist) or\
                    (forward_dist[v] > w + forward_dist[cur_forward]):
                    forward_dist[v] = w + forward_dist[cur_forward]
                    heappush(forward_pq, (forward_dist[v] + pi_t[v], v))

        cur_reverse = -1
        while reverse_pq:
            (_, cur_reverse) = heappop(reverse_pq)
            if cur_reverse not in reverse_seen: break
        
        if cur_reverse != -1:
            if cur_reverse in forward_seen: break
            reverse_seen.add(cur_reverse)

            for (v, w) in graph.get_neighbours(cur_reverse, rev=True):
                if v in forward_seen or v == source:
                    shortest_so_far = min(shortest_so_far,\
                        reverse_dist[cur_reverse] + w + forward_dist[v])
 
                if (v not in reverse_dist) or\
                    (reverse_dist[v] > w + reverse_dist[cur_reverse]):
                    reverse_dist[v] = w + reverse_dist[cur_reverse]
                    heappush(reverse_pq, (reverse_dist[v] + pi_s[v], v))

    print('Vertices scanned by bidirectional A*:', len(reverse_seen) + len(forward_seen))
    return shortest_so_far