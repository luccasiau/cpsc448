import numpy as np

# Gets the lower-bound describe in the paper from node s
def get_lower_bound(graph, s):
    pi = np.zeros(graph.N)

    # Note to self: If I change dist to be np arrays, this whole next step
    # would become easier and more efficient. However, if I do that for every
    # node, it'd be the same as having an adjacency matrix. I could try to come
    # up with an implementation that only uses np arrays for the landmarks.
    for v in range(graph.N):
        for l in graph.landmarks:
            pi[v] = max(pi[v], graph.get_dist(v, l) - graph.get_dist(s, l))
            pi[v] = max(pi[v], graph.get_dist(l, s) - graph.get_dist(l, v))

    np.nan_to_num(pi, nan=float('inf'))
    return pi

# Consistent potential functions
def get_average_potential_functions(graph, s, t):
    pi_s = get_lower_bound(graph, s)
    pi_t = get_lower_bound(graph, t)

    p_s = (pi_s - pi_t)/2.0
    return p_s, -p_t