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

numADC = 2
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
        for ADC in range(numADC):         
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
            #print("Byte to read \t%d" % (byte2read/2))
            #print("%d\tWaiting Byte: \t%d" % (ADC, teensy.inWaiting()))      
            #print("%d\tLength: \t%d" % (ADC, (totalLength[ADC]/2)))
    
    list4Info.append(StackOfInfo)
    list4Bytes.append(StackOfBytes)
        
    for ADC in range(numADC):
        print("Byte aquired: \t%d" % totalLength[ADC])


#%%
"""Open serial communication.

Teensy should be on /dev/ttyACM0 (at least on my ubuntu 16.04).
Remember to close the communication."""
teensy = serial.Serial('/dev/ttyACM0', 
                  timeout=0.2)
#                  xonxoff=0,
#                  rtscts=0,
#                  bytesize=serial.EIGHTBITS,
#                  parity=serial.PARITY_NONE,
#                  stopbits=serial.STOPBITS_ONE)
# Toggle DTR to reset Teensy (This makes it start!!)
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
#ListOfParameters = ['A1 R8 N2000 F200000 D8 T0 S0', 
#                    'A2 R8 N2000 F100000 D16 T0 S0', 
#                    'A4 R12 N2000 F50000 D32 T0 S0',
#                    'S2']
                    
ListOfParameters = ['A1 R8 N8000 F200000.000 D2 T0 S0',
'A1 R8 N7988 F126000.000 D4 T0 S0',
'A1 R8 N7998 F79600.000 D6 T0 S0',
'A1 R8 N8026 F50400.000 D8 T0 S0',
'A4 R16 N7975 F31600.000 D14 T0 S0',
'A4 R16 N8000 F20000.000 D20 T0 S0',
'A4 R16 N8115 F12800.000 D32 T0 S0',
'A4 R16 N8038 F8000.000 D50 T0 S0',
'A4 R16 N8281 F5200.000 D80 T0 S0',
'A32 R16 N1000 F3200.000 D126 T0 S0',
'A32 R16 N1000 F2000.000 D200 T0 S0',
'A32 R16 N1000 F1200.000 D318 T0 S0',
'A32 R16 N1000 F1000.000 D502 T0 S0',
'A32 R16 N1000 F1000.000 D796 T0 S0',
'A32 R16 N1000 F1000.000 D1262 T0 S0',
'A32 R16 N1000 F1000.000 D2000 T0 S0',
'A32 R16 N1000 F1000.000 D3170 T0 S0',
'A32 R16 N1206 F1000.000 D5024 T0 S0',
'A32 R16 N1911 F1000.000 D7962 T0 S0',
'A32 R16 N3029 F1000.000 D12620 T0 S0',
'A32 R16 N4800 F1000.000 D20000 T0 S0',
'A32 R16 N7607 F1000.000 D31698 T0 S0',
'A32 R16 N12057 F1000.000 D50238 T0 S0',
'A32 R16 N19109 F1000.000 D79622 T0 S0',
'A32 R16 N30286 F1000.000 D126192 T0 S0',
'A32 R16 N48000 F1000.000 D200000 T0 S0',
                    'S2']
#%% N/F/(D*10): 10 points per period
#fftIndex = [2000/200e3/(8e-6*20)/2, 
#                 2000/100e3/(16e-6*20)/2,
#                 2000/50e3/(32e-6*20)/2]
                 
fftIndex = [500, 396, 419, 498, 451, 500, 495, 502, 498, 62, 
63, 66, 50, 31, 20, 13, 8, 6, 6, 6, 
6, 6, 6, 6, 6, 6]	  
#%% 
t0 = time.time()
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
    t1 = time.time()
    ReceiveData(ListOfInfo, ListOfBytes)  
    print("Aquisition time: %.1f s" % (time.time()-t1))
    print("\n")

print("Total time: %.1f s" % (time.time()-t0))
#%%
"""Convert the list into an array."""  
data = [] 
for i in range(len(ListOfParameters)-1):    
    StackOfData = [[], []]
    for ADC in range(numADC):
        lengthList = 0
        for iBlock in range(len(ListOfBytes[i][ADC])):
            lengthList = lengthList + len(ListOfBytes[i][ADC][iBlock])/2
    
        print("Length of streamed data: \t%d" % ADC, lengthList)
        StackOfData[ADC].append(np.zeros([int(lengthList)], dtype=np.uint16))
    
        position1 = 0;
        for iBlock in range(0,len(ListOfBytes[i][ADC])):
            position2 = position1+int(len(ListOfBytes[i][ADC][iBlock])/2)
            StackOfData[ADC][position1:position2] = np.fromstring(ListOfBytes[i][ADC][iBlock], dtype=np.uint16)
            position1 = position2
    data.append(StackOfData)
            
#%%
"""Calculate Z"""
Z = np.zeros([len(ListOfParameters)-1,1], dtype=np.complex) 
for i in range(len(ListOfParameters)-1):
    w = np.blackman(len(data[i][0]))
    w = w/np.trapz(w);
    A = fft(w*(data[i][0]-np.mean(data[i][0])))/len(data[i][0]);
    w = np.blackman(len(data[i][1]))
    w = w/np.trapz(w);
    B = fft(w*(data[i][1]-np.mean(data[i][1])))/len(data[i][1]);
    index = int(fftIndex[i]-1)
    Z[i] = A[index]/B[index]
    # plot fft
    pl.figure(i)
    pl.clf()
    pl.subplot(3, 1, 1)
    pl.title(ListOfParameters[i])
    pl.plot(data[i][0],'b+-')
    pl.plot(data[i][1],'g+-')
    pl.subplot(3, 1, 2)
    pl.semilogy(abs(A), 'b-')
    pl.ylabel('A')
    pl.subplot(3, 1, 3)
    pl.semilogy(abs(B), 'g-') 
    pl.ylabel('B')
    pl.show()
    
#%%
"""Plot."""
pl.figure(10*i)
pl.clf
pl.subplot(2, 1, 1)
pl.plot(abs(Z), 'bs')
pl.ylim( 0, 1.5 )
pl.subplot(2, 1, 2)
pl.plot(np.angle(Z, deg=True), 'gs' ) 
maxY = np.round(np.max(2*np.angle(Z, deg=True))/10)*10
pl.ylim( [maxY, 0].sort() )
pl.show()
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