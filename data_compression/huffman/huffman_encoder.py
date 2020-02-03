"""
Gets a file path to a txt OR a string and outputs a binary string of its
Huffman Coding.

The encoding will start with a 2,048-character header. This header represents
the number of occurences of each of the 128 ascii character, encoded as a 16
bit number.
"""
from huffman_tree import HuffmanTree

class HuffmanEncoder:
    def __init__(self, content_source, type='PATH'):
        self.get_content(content_source, type)
        self.build_frequencies()
        self.build_tree()

    def get_content(self, content_source, type):
        if type == 'PATH':
            f = open(content_source, 'r')
            self.content = f.read()
            f.close()
        else:
            self.content = content_source
    
    def build_frequencies(self):
        self.frequencies = {}
        for c in self.content:
            if c not in self.frequencies:
                self.frequencies[c] = 0
            self.frequencies[c] += 1
    
    def build_tree(self):
        