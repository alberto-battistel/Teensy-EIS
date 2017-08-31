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
byteDataList = [[], []]
byteStreamList = [[], []]

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
then the bytes of the data
twice, one for ADC channel"""
totalLength = [0,0]
StreamNotCompleted = [True, True]
while all(StreamNotCompleted):
    "Read data."
    byteIterations = teensy.readline()
    if byteIterations is b'':
        break
    Iterations = int(byteIterations.decode('utf-8'))
    print("\nIterations: %d" % Iterations)
    for ch in range(len(totalLength)):
        print("%d\tWaiting Byte: \t%d" % (ch, teensy.inWaiting()))        
        byteStream = teensy.readline()
        byteStreamList[ch].append(byteStream)
        
        if byteStream is b'':
            print("Done %d" % ch)
            StreamNotCompleted[ch] = False
            continue
        if len(byteStream) > 6:
            print("Error %d" % ch)
            StreamNotCompleted = False
            break    
        
        data2read = int(byteStream.decode('utf-8'))
        byteData = teensy.read(2*data2read) # read bytes 
        byteDataList[ch].append(byteData)
        totalLength[ch] = totalLength[ch] + len(byteData)
        print("Byte to read \t%d" % (data2read/2))
        print("%d\tWaiting Byte: \t%d" % (ch, teensy.inWaiting()))      
        print("%d\tLength: \t%d" % (ch, (totalLength[ch]/2)))
#%%      
for ch in range(len(totalLength)):
    print("Byte aquired: \t%d" % totalLength[ch])
        
#%%
"""Convert the list into an array."""  
data = [[], []] 
for ch in range(len(totalLength)):     
    lengthList = 0
    for i in range(0,len(byteDataList[ch])):
        lengthList = lengthList+len(byteDataList[ch][i])/2
    
    print("Length of streamed data: \t%d" % lengthList)
    data[ch] = np.zeros([int(lengthList)], dtype=np.uint16)
    
    position1 = 0;
    for i in range(0,len(byteDataList[ch])):
        position2 = position1+int(len(byteDataList[ch][i])/2)
        data[ch][position1:position2] = np.fromstring(byteDataList[ch][i], dtype=np.uint16)
        position1 = position2

#%%
"""Plot."""    
pl.subplot(3, 1, 1)
pl.plot(data[0])
pl.plot(data[1])
pl.subplot(3, 1, 2)
pl.semilogy(abs(fft(data[0]))/len(data[0]))
pl.semilogy(abs(fft(data[1]))/len(data[1]))
pl.subplot(3, 1, 3)
pl.plot(np.diff(np.float32(data[0])))
pl.plot(np.diff(np.float32(data[1])))
pl.show()
        
#%%
"""Close communication."""        
teensy.close()