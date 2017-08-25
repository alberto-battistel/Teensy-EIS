# -*- coding: utf-8 -*-
"""
Created on Tue Aug  8 23:32:05 2017

@author: alberto
"""
# check if teensy changed port
# for details: https://docs.python.org/2/library/subprocess.html 

import subprocess

# teensy are called ttyACM0, ttyACM1...
termOutput = subprocess.run("ls /dev/ttyACM*", stdout=subprocess.PIPE, shell=True)

print(termOutput.stdout) 

# termOutput contains ' and I am not able to parse as a string 
#import re 



#print(re.search('(?<=ttyACM)\w+', re.escape(termOutput.stdout)))

#m=re.search('(?<=b'/dev/ttyACM)\w+', termOutput)
#print(m.group(0)) 

