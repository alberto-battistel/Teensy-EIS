# -*- coding: utf-8 -*-
"""
Created on Tue Aug 15 16:48:22 2017

@author: alberto
"""

#%%
import serial
import time
import numpy as np
import pylab as pl
import struct

#os.system("clear")
#%%
# open serial
s = serial.Serial('/dev/ttyACM0', timeout=3)
#time.sleep(2)

#%% prepare list to receive the data
NUM = []

#%%
# tell teensy you are ready (it is just waiting for a byte)
s.write('S0'.encode('utf-8'))
time.sleep(1)
while True:
    "Read data."
    a = s.readline()
    if not a:
        "There is an error in communication."
        print('Error')
        break
    elif a == b'\r\n':
        "Empty line."
        print('No more data')
        break
    elif a == b'Ready!\r\n':
        "First Ready."
        print('Skip')
        continue
    else:
        NUM.append(np.fromstring(a[:-2], dtype=np.uint16))
        print(a)
        
lengthList = 0
for i in range(0,len(NUM)-1):
    lengthList = lengthList+len(NUM[i])

data = np.zeros([lengthList], dtype=np.uint16)

position1 = 0;
for i in range(0,len(NUM)-1):
    position2 = position1+len(NUM[i])
    data[position1:position2] = NUM[i]
    position1 = position2
    
pl.plot(data)
pl.show()
        

##%%
## change parameters
#"""
#letter + number (no spaces, write add end of line anyway)
#S       to start, it will also give the parameters back
#F val   sampling frequency (float32)
#N val   number of samples (uint32)
#R val   adc resolution (8,12,16)
#A val   adc average (from 1 to 32 in power of 2)
#it corrects automatically if the values are not right
#"""
#
## 'F123456'.encode('utf-8') = b'F123456'
#s.write('F123456'.encode('utf-8'))
#s.write('S1'.encode('utf-8'))
#
##%%
## read the parameters
#print(s.readline())

#%%
#s.write
#sdata = s.read(10000)
#
#data = np.fromstring(sdata, dtype=np.uint16)
#
#pl.plot(data)
#pl.show()

#%%
s.close()