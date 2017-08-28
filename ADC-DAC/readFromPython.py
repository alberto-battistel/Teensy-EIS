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
from scipy.fftpack import fft

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
while teensy.readline() != b'Go!\r\n':
    continue

#%% 
"""Receive the data.

first an ascii value with end of line to advice how many data are coming
then the bytes of the data"""
while True:
    "Read data."
    byteStream = teensy.readline()
    if byteStream is b'':
        print("Done")
        break
    if len(byteStream) > 6:
        print("Error")
        break
    data2read = int(byteStream.decode('utf-8'))
    print(data2read)
    byteData = teensy.read(2*data2read) # read bytes
    NUM.append(np.fromstring(byteData, dtype=np.uint16))
    
        
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
pl.subplot(2, 1, 1)
pl.plot(data)
pl.subplot(2, 1, 2)
pl.semilogy(abs(fft(data))/len(data))
pl.show()
        
#%%
"""Close communication."""        
teensy.close()