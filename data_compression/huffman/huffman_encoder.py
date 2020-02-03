"""
Gets a file path to a txt OR a string and outputs a binary string of its
Huffman Coding.

The encoding will start with a 4,096-character header. This header represents
the number of occurences of each of the 128 ascii character, encoded as a 16
bit number.
"""
from huffman_tree import HuffmanTree

class HuffmanEncoder:
    def __init__(self, content_source, type='PATH'):
        self.get_content(content_source, type)
        self.build_frequencies()
        self.build_tree()
        self.encode()
        self.build_header()

    def get_content(self, content_source, type):
        if type == 'PATH':
            f = open(content_source, 'r')
            self.content = f.read()
            f.close()
        else:
            self.content = content_source

        # TODO: Figure out what to do with characters outside of 128 ASCII
        self.content = ''.join(list(filter(lambda x : ord(x) < 128, self.content)))
    
    def build_frequencies(self):
        self.frequencies = {}
        for c in self.content:
            if c not in self.frequencies:
                self.frequencies[c] = 0
            self.frequencies[c] += 1
    
    def build_tree(self):
        self.tree = HuffmanTree(self.frequencies)

    def encode(self):
        enc_list = [self.tree.getCode(c) for c in self.content]
        self.encoding = ''.join(enc_list)

    def build_header(self):
        header_list = []
        for i in range(128):
            v = 0 if chr(i) not in self.frequencies else self.frequencies[chr(i)]

            b = bin(v)[2:]
            to_append = '0' * (32 - len(b)) + b
            header_list.append(to_append)

            if chr(i) in self.frequencies:
                assert int(to_append, 2) == self.frequencies[chr(i)]

        self.header = ''.join(header_list)

    # Prints the encoding as a binary string.
    def get_encoding(self, header=True):
        if header: return self.header + self.encoding
        else: return self.encoding