#!/usr/bin/env python3

#import pyperclip
import sys

msg = 'This is my secret msg.'

key = 13

mode = 'encrypt'

LETTERS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

translated = ''

def print_usage():
    print("Usage: " + sys.argv[0] + " [KEY] [MSG] [ENC/DEC]")

try:
    key = int(sys.argv[1])
    msg = sys.argv[2]
    mode = sys.argv[3]
except:
    print_usage()
    quit()

msg = msg.upper()

for symbol in msg:
    if symbol in LETTERS:
        # get the encrypted/decrypted number for the symbol
        num = LETTERS.find(symbol)
        if mode == 'encrypt':
            num = num + key
        elif mode == 'decrypt':
            num = num - key

        if num >= len(LETTERS):
            num = num - len(LETTERS)
        elif num < 0:
            num = num + len(LETTERS)

        # add enc/dec number's symbol at the end of translated
        translated = translated + LETTERS[ num]

    else:
        # no enc/dec
        translated = translated + symbol

print('The result = ' + translated)

fd = open('tmp.txt', 'w')
fd.write( translated)
fd.close()

#pyperclip.copy(translated)
