#!/bin/sh

./caesar_cipher.py 13 "this is my shit" encrypt
./hack_caesar.py "`cat tmp.txt`"
