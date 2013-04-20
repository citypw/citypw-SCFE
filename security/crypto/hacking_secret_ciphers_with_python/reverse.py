#!/usr/bin/env python3

plain_text = 'Those who are willing to dig the truth out of the surface, I ...'
cipher_text = ''

i = len( plain_text) - 1

while i >= 0:
    cipher_text += plain_text[i]
    i -= 1

print("The cipher text is: " + cipher_text)
 
