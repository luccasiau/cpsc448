from huffman_encoder import HuffmanEncoder
from huffman_decoder import HuffmanDecoder

f = open('../paradise_lost.txt')
original_book = f.read()
f.close()

he = HuffmanEncoder('../paradise_lost.txt')
hd = HuffmanDecoder(he.get_encoding())

assert he.header == hd.header
assert he.frequencies == hd.frequencies
print('Original size (in bits):', 8*len(original_book))
print('Encoding size (in bits):', len(he.get_encoding()))
print('Reduction percentage:', round(len(he.get_encoding())/(8*len(original_book)), 2))