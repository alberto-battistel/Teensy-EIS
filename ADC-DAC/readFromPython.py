# -*- coding: utf-8 -*-
""" Recieve data from Teensy via Serial.
It works more or less.
There are problem with the flow of bytes.
Python can interpret some bytes as end of line and discard the rest, which is bad.

Created on Mon Aug 28

@author: alberto
"""

#%%
import serial
import time
import numpy as np
import pylab as pl
import struct

#%%
"""Open serial communication.

Teensy should be on /dev/ttyACM0 (at least on my ubuntu 16.04).
Remember to close the communication."""
teensy = serial.Serial('/dev/ttyACM0', 
                  timeout=3)
#                  xonxoff=0,
#                  rtscts=0,
#                  bytesize=serial.EIGHTBITS,
#                  parity=serial.PARITY_NONE,
#                  stopbits=serial.STOPBITS_ONE)
# Toggle DTR to reset Arduino (I don't know if it is usefull..)
teensy.setDTR(False)
time.sleep(1)
teensy.setDTR(True)

#%% prepare list to receive the data
NUM = []

#%% 
"""Tell teensy you are ready (it is just waiting for some bytes)."""
teensy.write('S0'.encode('utf-8'))
time.sleep(1)

#%% 
"""Receive the data.

problem: it happens that it recieve a byte (\n) corresponding at the end of line
and it messes up the interpretation on python..."""
while True:
    "Read data."
    a = teensy.readline()
    print(a)
    if not a:
        "There is an error in communication."
        print('Error')
        print(a)
        break
    elif a == b'\r\n':
        "Empty line."
        print('No more data')
        break
    elif a == b'Ready!\r\n':
        "First Ready."
        print('Teensy is Ready!')
        continue
    elif a == b'Waiting...\r\n':
        "Teensy is waiting."
        print('Teensy is waiting...')
        continue
    elif a == b'Go!\r\n':
        "It starts."
        print('It starts!')
        continue
    else:
        "Collect data."
#        print(len(a))
#        print(a)
        NUM.append(np.fromstring(a[:-2], dtype=np.uint16))
        
#%%
"""Convert the list into an array."""        
lengthList = 0
for i in range(0,len(NUM)-1):
    lengthList = lengthList+len(NUM[i])

data = np.zeros([lengthList], dtype=np.uint16)

position1 = 0;
for i in range(0,len(NUM)-1):
    position2 = position1+len(NUM[i])
    data[position1:position2] = NUM[i]
    position1 = position2

#%%
"""Plot."""    
pl.plot(data)
pl.show()
        
#%%
"""Close communication."""        
teensy.close()