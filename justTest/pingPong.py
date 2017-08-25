# -*- coding: utf-8 -*-
"""
Created on Tue Aug 15 16:48:22 2017

@author: alberto
"""

#%%
import serial
import time
#import numpy as np
#import pylab as pl
#import struct

#%%
# open serial
s = serial.Serial('/dev/ttyACM0', timeout=3)
#time.sleep(2)
#%%
# tell teensy you are ready (it is just waiting for a byte)
s.write('S0'.encode('utf-8'))
print(s.readline())

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