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
                    
ListOfParameters = ['A1 R8 N12000 F200000.000 D2 T0 S0',
'A1 R8 N11995 F158800.000 D4 T0 S0',
'A1 R8 N11982 F126000.000 D4 T0 S0',
'A1 R8 N12019 F100400.000 D4 T0 S0',
'A1 R8 N11997 F79600.000 D6 T0 S0',
'A1 R8 N11991 F63200.000 D6 T0 S0',
'A1 R8 N12039 F50400.000 D8 T0 S0',
'A4 R16 N12028 F40000.000 D10 T0 S0',
'A4 R16 N11963 F31600.000 D14 T0 S0',
'A4 R16 N12010 F25200.000 D16 T0 S0',
'A4 R16 N12000 F20000.000 D20 T0 S0',
'A4 R16 N12086 F16000.000 D26 T0 S0',
'A4 R16 N12172 F12800.000 D32 T0 S0',
'A4 R16 N11972 F10000.000 D40 T0 S0',
'A4 R16 N12057 F8000.000 D50 T0 S0',
'A4 R16 N12143 F6400.000 D64 T0 S0',
'A4 R16 N12421 F5200.000 D80 T0 S0',
'A128 R16 N1000 F4000.000 D100 T0 S0',
'A128 R16 N1000 F3200.000 D126 T0 S0',
'A128 R16 N1000 F2400.000 D160 T0 S0',
'A128 R16 N1000 F2000.000 D200 T0 S0',
'A128 R16 N1000 F2000.000 D252 T0 S0',
'A128 R16 N1000 F2000.000 D318 T0 S0',
'A128 R16 N1000 F2000.000 D400 T0 S0',
'A128 R16 N1000 F2000.000 D502 T0 S0',
'A128 R16 N1000 F2000.000 D632 T0 S0',
'A128 R16 N1000 F2000.000 D796 T0 S0',
'A128 R16 N1000 F2000.000 D1002 T0 S0',
'A128 R16 N1000 F2000.000 D1262 T0 S0',
'A128 R16 N1017 F2000.000 D1590 T0 S0',
'A128 R16 N1280 F2000.000 D2000 T0 S0',
'A128 R16 N1611 F2000.000 D2518 T0 S0',
'A128 R16 N2029 F2000.000 D3170 T0 S0',
'A128 R16 N2554 F2000.000 D3992 T0 S0',
'A128 R16 N3215 F2000.000 D5024 T0 S0',
'A128 R16 N4048 F2000.000 D6326 T0 S0',
'A128 R16 N5096 F2000.000 D7962 T0 S0',
'A128 R16 N6415 F2000.000 D10024 T0 S0',
'A128 R16 N8076 F2000.000 D12620 T0 S0',
'A128 R16 N10167 F2000.000 D15888 T0 S0',
'A128 R16 N12800 F2000.000 D20000 T0 S0',
'A128 R16 N16114 F2000.000 D25180 T0 S0',
'A128 R16 N20287 F2000.000 D31698 T0 S0',
'A128 R16 N25539 F2000.000 D39906 T0 S0',
'A128 R16 N32152 F2000.000 D50238 T0 S0',
'A128 R16 N40477 F2000.000 D63246 T0 S0',
'A128 R16 N50958 F2000.000 D79622 T0 S0',
'A128 R16 N64152 F2000.000 D100238 T0 S0',
'A128 R16 N80763 F2000.000 D126192 T0 S0',
'A128 R16 N101674 F2000.000 D158866 T0 S0',
'A128 R16 N128000 F2000.000 D200000 T0 S0',
                    'S2']
#%% N/F/(D*10): 10 points per period
#fftIndex = [2000/200e3/(8e-6*20)/2, 
#                 2000/100e3/(16e-6*20)/2,
#                 2000/50e3/(32e-6*20)/2]
                 


#%% 
t0 = time.time()
while True:
    """Get parameters from console and feed Teensy."""
    InputParameters = input("\nParameters (like 'A1 R8 N12000 F200000.000 D2 T0 S0'), give 'S2' to stop:\n")  
               
    """Tell teensy you are ready (it is just waiting for some bytes)."""
    teensy.reset_input_buffer()
    teensy.write(InputParameters.encode('utf-8'))
    teensy.flush()
    if InputParameters == "'S2'":
        """End loop."""
        print("Ending!\n")
        break
    
    parameters = ''    
    while parameters != b'Go!\r\n':
        parameters = teensy.readline()
        print(parameters.decode('utf-8'))
        continue
    t1 = time.time()
    ReceiveData(ListOfInfo, ListOfBytes)  
    print("Aquisition time: %.1f s" % (time.time()-t1))
    print("\n")
    
    """Collect data."""
    print("Collecting data\n")
    StackOfData = [[], []]
    i = 0
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
    
    """Calculate fft."""
    print("Calculating FFT\n")
    w = np.blackman(len(StackOfData[0]))
    w = w/np.trapz(w);
    A = fft(w * StackOfData[0])/len(StackOfData[0]);
    w = np.blackman(len(StackOfData[1]))
    w = w/np.trapz(w);
    B = fft(w * StackOfData[1])/len(StackOfData[1]);
        
    """Plot."""
    print("Plotting\n")
    pl.figure(1)
    pl.clf()
    pl.subplot(3, 1, 1)
    pl.title(InputParameters)
    pl.plot(StackOfData[0],'b+-')
    pl.plot(StackOfData[1],'g+-')
    pl.subplot(3, 1, 2)
    pl.semilogy(abs(A), 'b-')
    pl.ylabel('A')
    pl.subplot(3, 1, 3)
    pl.semilogy(abs(B), 'g-') 
    pl.ylabel('B')
    pl.show()        
    


        
#%%
"""Close communication."""        
teensy.close()