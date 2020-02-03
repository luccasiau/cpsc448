"""
Huffman Decoder.

This is not nearly as sophisticated as I'd want it to be, but I only have so
much time (and energy.)
"""

from huffman_tree import HuffmanTree

class HuffmanDecoder:
    # Assuming content comes as a huge string.
    def __init__(self, content):
        self.header = content[:4096]
        self.content = content[4096:]
        self.get_frequencies()
        self.tree = HuffmanTree(self.frequencies)
    
    def get_frequencies(self):
        gap = int(len(self.header)/128)
        cnts = [int(self.header[i:i+gap], 2) for i in range(0, len(self.header), gap)]
        self.frequencies = {chr(x):cnts[x] for x in range(128) if cnts[x] > 0}

    def get_decoding(self):
        cur = ''
        decoded_list = []
        for c in self.content:
            cur += c
            if self.tree.isDecodable(cur):
                decoded_list.append(self.tree.getChar(cur))
                cur = ''

        return ''.join(decoded_list)