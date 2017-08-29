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
import io
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
byteDataList = []
byteStreamList = []

#%% 
"""Tell teensy you are ready (it is just waiting for some bytes)."""
teensy.reset_input_buffer()
teensy.write('S0'.encode('utf-8'))
teensy.flush()


#%%
while teensy.readline() != b'Go!\r\n':
    continue

#%% 
"""Receive the data.

first an ascii value with end of line to advice how many data are coming
then the bytes of the data"""
totalLength = 0
while True:
    "Read data."
    #print("\tWaiting Byte: \t%d" % teensy.inWaiting())
    byteStream = teensy.readline()
    byteStreamList.append(byteStream)
    
    if byteStream is b'':
        print("Done")
        break
    if len(byteStream) > 6:
        print("Error")
        break    
    
    data2read = int(byteStream.decode('utf-8'))
    #print("Byte to read: \t%d" % data2read)
    #print("Waiting Byte: \t%d" % teensy.inWaiting())
    byteData = teensy.read(2*data2read) # read bytes 
    byteDataList.append(byteData)
    totalLength = totalLength + len(byteData)
    
print("Byte aquired: \t%d" % totalLength)        
#%%
"""Convert the list into an array."""        
lengthList = 0
for i in range(0,len(byteDataList)):
    lengthList = lengthList+len(byteDataList[i])/2

print("Length of streamed data: \t%d" % lengthList)
data = np.zeros([int(lengthList)], dtype=np.uint16)

position1 = 0;
for i in range(0,len(byteDataList)):
    position2 = position1+int(len(byteDataList[i])/2)
    data[position1:position2] = np.fromstring(byteDataList[i], dtype=np.uint16)
    position1 = position2

#%%
"""Plot."""    
pl.subplot(3, 1, 1)
pl.plot(data)
pl.subplot(3, 1, 2)
pl.semilogy(abs(fft(data))/len(data))
pl.subplot(3, 1, 3)
pl.plot(np.diff(np.float32(data)))
pl.show()
        
#%%
"""Close communication."""        
teensy.close()