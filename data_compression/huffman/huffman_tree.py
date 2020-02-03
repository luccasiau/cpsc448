"""
Class that builds a Huffman Tree, and can be used to encode or decode a text.

It uses Huffman Coding with minimized variance.
"""

import heapq

class HuffmanTree:
    """
    Frequencies: a dict from character to frequency.
    """
    def __init__(self, frequencies):
        self.frequencies = frequencies
        self.char_to_code_map = {}
        self.code_to_char_map = {}
        self.buildTree()

    def buildTree(self):
        # These will substitute all the temporary variables that hold strings
        tmp_map = {}
        tmp_append_map = {}
        tmp_char = 128
        build_order = []

        # Initializing heap
        pq = []
        for k, v in self.frequencies.items():
            if v == 0: continue
            heapq.heappush(pq, (v, 1, k))

        # Constant merging of two least frequent
        while len(pq) > 1:
            p1 = heapq.heappop(pq)
            p2 = heapq.heappop(pq)

            tmp_char += 1
            tmp_append_map[p1[2]] = (tmp_char, '0')
            tmp_append_map[p2[2]] = (tmp_char, '1')
            build_order.append(p1[2])
            build_order.append(p2[2])

            # Add merge to heap
            heapq.heappush(pq, (p1[0] + p2[0], p1[1] + p2[1], tmp_char))

        tmp_map[tmp_char] = ''
        for key in reversed(build_order):
            p = tmp_append_map[key]
            tmp_map[key] = tmp_map[p[0]] + p[1]

        # Iterating over ascii characters
        for i in range(128):
            # TODO: Figure out what to do if text has less than 128 characters
            if chr(i) not in tmp_map: continue
            self.char_to_code_map[chr(i)] = tmp_map[chr(i)]
            self.code_to_char_map[tmp_map[chr(i)]] = chr(i)

