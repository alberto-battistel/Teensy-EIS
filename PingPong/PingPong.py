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

#%% 
"""Tell teensy you are ready (it is just waiting for some bytes)."""
teensy.reset_input_buffer()
teensy.write('S0'.encode('utf-8'))
teensy.flush()

print(1)
print(teensy.readline())
#time.sleep(1)
teensy.write('F2222 N1000 R32 A8 D100 T0 S0'.encode('utf-8'))
teensy.flush()

print(2)
print(teensy.readline())

teensy.write('S2'.encode('utf-8'))
teensy.flush()

print(3)
print(teensy.readline())

##%%
## change parameters
"""
letter + number (no spaces, write add end of line anyway)
S val   0-> start/running, 1-> finished aquisition/waiting for paramerter, 2 -> shut down
F val   ADC sampling frequency (float32)
N val   ADC number of samples (uint32)
R val   ADC resolution (8,12,16)
A val   ADC average (from 1 to 32 in power of 2)
D val   DAC delay (uint32)
T val   DAC type (0 to 5)

it corrects automatically if the values are not right
"""
#
## 'F123456'.encode('utf-8') = b'F123456'
#s.write('F123456'.encode('utf-8'))
#s.write('S1'.encode('utf-8'))
#
##%%
## read the parameters
#print(s.readline())



#%%
teensy.close()