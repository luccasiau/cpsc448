"""
This is a decoder that solves the fun puzzle.

It currently only supports lowercase characters (no spaces, please).

Argument input:
    - (optional): The encoded string. If this is not provided, the user
    will be prompted to provide one afterwards.
        -> The character # must be used to represent the end-of-string char.
"""

import sys


class DecodingHelper:
    def __init__(self):
        self.char_list = {}
        self.char_idx = {}

    def addPair(self, a, b):
        if a not in self.char_list:
            self.char_list[a] = []
            self.char_idx[a] = 0
        self.char_list[a].append(b)

    def getNext(self, c):
        self.char_idx[c] += 1
        return self.char_list[c][self.char_idx[c] - 1]

    def getFirstChar(self):
        return self.char_list['#'][0]


def decode(encoded_str):
    encoded_list = list(encoded_str)
    sorted_list = sorted(encoded_list, key=lambda x: ord(x) if x != '#' else 300)

    helper = DecodingHelper()
    for i in range(len(encoded_list)):
        helper.addPair(sorted_list[i], encoded_list[i])

    front_char = helper.getFirstChar()
    decoded = []
    while front_char != '#':
        decoded.append(front_char)
        front_char = helper.getNext(front_char)

    return ''.join(decoded)

def encode(original_str, verbose=False):
    if verbose: print('Original string:', original_str)

    original_str = original_str + '#'
    n = len(original_str)
    tuples = []
    for i in range(n):
        tuples.append((original_str[i], i, original_str[(i+1) % n]))

    tuples.sort(key = lambda x: x if x[0] != '#' else ('}', x[1], x[2]))

    encoded_list = []

    if verbose: print('Sorted tuples are:')
    for t in tuples:
        encoded_list.append(t[2])
        if verbose: print('(%s, %s)' % (t[0], t[2]))

    return ''.join(encoded_list)

if __name__ == '__main__':
    encoded_str = ""
    if len(sys.argv) == 1:
        print('Enter string to be decoded: ', end='')
        encoded_str = input()
    else:
        encoded_str = sys.argv[1]

    decoded_str = decode(encoded_str)
    print('Decoded string is: ' + decoded_str)

    print('Want to see a step-by-step encoding of it? [y/n]  ', end='')
    inp = input().lower()
    if (inp == 'y'):
        print('Encoded string: ' + encode(decoded_str, verbose=True))
    elif(inp != 'n'):
        print('I will assume that is a no.')
    print('Goodbye.')
