#!/usr/bin/env python3

import sys

LETTERS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

def print_usage():
    print("Usage: " + sys.argv[0] + " [MSG]")

try:
    msg = sys.argv[1]

except:
    print_usage()
    quit()

for key in range(len(LETTERS)):
    translated = ''
    for symbol in msg:
        if symbol in LETTERS:
            num = LETTERS.find(symbol)
            num -= key

            if num < 0:
                num += len(LETTERS)

            translated = translated + LETTERS[num]

        else:
            translated += symbol;

    print('Key #%s: %s' % (key, translated))
