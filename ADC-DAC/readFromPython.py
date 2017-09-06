# -*- coding: utf-8 -*-
""" Receive data from Teensy via Serial.
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
""" Function definition """
def ReceiveData(list4Info, list4Bytes):
    """Receive the data.
    
    first an ascii value with end of line to advice how many data are coming
    then the bytes of the data
    twice, one for ADC channel"""
    
    StackOfBytes = [[], []]
    StackOfInfo = [[], []]
    totalLength = [0,0]
    StreamNotCompleted = [True, True]
    while all(StreamNotCompleted):
        "Read data."
#        byteIterations = teensy.readline()
#        if byteIterations is b'':
#            break
#        Iterations = int(byteIterations.decode('utf-8'))
#        print("\nIterations: %d" % Iterations)
        for ADC in range(len(totalLength)):
            #print("%d\tWaiting Byte: \t%d" % (ADC, teensy.inWaiting()))        
            blockInfo = teensy.readline()
            StackOfInfo[ADC].append(blockInfo)
            
            if blockInfo is b'':
                print("Done %d" % ADC)
                StreamNotCompleted[ADC] = False
                continue
            if len(blockInfo) > 6:
                print("Error %d" % ADC)
                StreamNotCompleted = False
                break    
            
            byte2read = 2*int(blockInfo.decode('utf-8'))
            byteBlock = teensy.read(byte2read) # read bytes 
            StackOfBytes[ADC].append(byteBlock)
            totalLength[ADC] = totalLength[ADC] + len(byteBlock)
            print("Byte to read \t%d" % (byte2read/2))
            #print("%d\tWaiting Byte: \t%d" % (ADC, teensy.inWaiting()))      
            print("%d\tLength: \t%d" % (ADC, (totalLength[ADC]/2)))
    
    list4Info.append(StackOfInfo)
    list4Bytes.append(StackOfBytes)
        
    for ADC in range(len(totalLength)):
        print("Byte aquired: \t%d" % totalLength[ADC])


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
"to store the data as byte"
ListOfBytes = []   
"to store the info of the streaming"
ListOfInfo = []   

#%%
"List of Parameters to pass to Teensy"
ListOfParameters = ['A32 R16 N1000 F1000 D100 T0 S0', 
                    'A32 R16 N1500 F2000 D100 T0 S0', 
                    'A32 R16 N2000 F3000 D100 T0 S0',
                    'S2']
                    
#%% 
for i in range(len(ListOfParameters)-1):                   
    """Tell teensy you are ready (it is just waiting for some bytes)."""
    teensy.reset_input_buffer()
    teensy.write(ListOfParameters[i].encode('utf-8'))
    teensy.flush()
    
    parameters = ''    
    while parameters != b'Go!\r\n':
        parameters = teensy.readline()
        print(parameters.decode('utf-8'))
        continue
    
    ReceiveData(ListOfInfo, ListOfBytes)  

#%% 

      
    
      
##%%
#"""Convert the list into an array."""  
#data = [[], []] 
#for ch in range(len(totalLength)):     
#    lengthList = 0
#    for i in range(0,len(byteDataList[ch])):
#        lengthList = lengthList+len(byteDataList[ch][i])/2
#    
#    print("Length of streamed data: \t%d" % lengthList)
#    data[ch] = np.zeros([int(lengthList)], dtype=np.uint16)
#    
#    position1 = 0;
#    for i in range(0,len(byteDataList[ch])):
#        position2 = position1+int(len(byteDataList[ch][i])/2)
#        data[ch][position1:position2] = np.fromstring(byteDataList[ch][i], dtype=np.uint16)
#        position1 = position2
#
##%%
#"""Plot."""    
#pl.subplot(3, 1, 1)
#pl.plot(data[0])
#pl.plot(data[1])
#pl.subplot(3, 1, 2)
#pl.semilogy(abs(fft(data[0]))/len(data[0]))
#pl.semilogy(abs(fft(data[1]))/len(data[1]))
#pl.subplot(3, 1, 3)
#pl.plot(np.diff(np.float32(data[0])))
#pl.plot(np.diff(np.float32(data[1])))
#pl.show()
        
#%%
"""Close communication."""        
teensy.close()