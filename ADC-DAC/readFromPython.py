# -*- coding: utf-8 -*-
""" Receive data from Teensy via Serial.
It works more or less.


Created on Mon Aug 28

@author: Alberto Battistel
"""

not sure it is still working...

#%%

import time
import numpy as np
import pylab as pl
from scipy.fftpack import fft
import TeensyCommunication as TeeCom


numADC = 2

#%%
TeeCom.Init()

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
#'A1 R8 N11982 F126000.000 D4 T0 S0',
#'A1 R8 N12019 F100400.000 D4 T0 S0',
#'A1 R8 N11997 F79600.000 D6 T0 S0',
#'A1 R8 N11991 F63200.000 D6 T0 S0',
#'A1 R8 N12039 F50400.000 D8 T0 S0',
#'A4 R16 N12028 F40000.000 D10 T0 S0',
#'A4 R16 N11963 F31600.000 D14 T0 S0',
#'A4 R16 N12010 F25200.000 D16 T0 S0',
#'A4 R16 N12000 F20000.000 D20 T0 S0',
#'A4 R16 N12086 F16000.000 D26 T0 S0',
#'A4 R16 N12172 F12800.000 D32 T0 S0',
#'A4 R16 N11972 F10000.000 D40 T0 S0',
#'A4 R16 N12057 F8000.000 D50 T0 S0',
#'A4 R16 N12143 F6400.000 D64 T0 S0',
#'A4 R16 N12421 F5200.000 D80 T0 S0',
#'A128 R16 N1000 F4000.000 D100 T0 S0',
#'A128 R16 N1000 F3200.000 D126 T0 S0',
#'A128 R16 N1000 F2400.000 D160 T0 S0',
#'A128 R16 N1000 F2000.000 D200 T0 S0',
#'A128 R16 N1000 F2000.000 D252 T0 S0',
#'A128 R16 N1000 F2000.000 D318 T0 S0',
#'A128 R16 N1000 F2000.000 D400 T0 S0',
#'A128 R16 N1000 F2000.000 D502 T0 S0',
#'A128 R16 N1000 F2000.000 D632 T0 S0',
#'A128 R16 N1000 F2000.000 D796 T0 S0',
#'A128 R16 N1000 F2000.000 D1002 T0 S0',
#'A128 R16 N1000 F2000.000 D1262 T0 S0',
#'A128 R16 N1017 F2000.000 D1590 T0 S0',
#'A128 R16 N1280 F2000.000 D2000 T0 S0',
#'A128 R16 N1611 F2000.000 D2518 T0 S0',
#'A128 R16 N2029 F2000.000 D3170 T0 S0',
#'A128 R16 N2554 F2000.000 D3992 T0 S0',
#'A128 R16 N3215 F2000.000 D5024 T0 S0',
#'A128 R16 N4048 F2000.000 D6326 T0 S0',
#'A128 R16 N5096 F2000.000 D7962 T0 S0',
#'A128 R16 N6415 F2000.000 D10024 T0 S0',
#'A128 R16 N8076 F2000.000 D12620 T0 S0',
#'A128 R16 N10167 F2000.000 D15888 T0 S0',
#'A128 R16 N12800 F2000.000 D20000 T0 S0',
#'A128 R16 N16114 F2000.000 D25180 T0 S0',
#'A128 R16 N20287 F2000.000 D31698 T0 S0',
#'A128 R16 N25539 F2000.000 D39906 T0 S0',
#'A128 R16 N32152 F2000.000 D50238 T0 S0',
#'A128 R16 N40477 F2000.000 D63246 T0 S0',
#'A128 R16 N50958 F2000.000 D79622 T0 S0',
#'A128 R16 N64152 F2000.000 D100238 T0 S0',
#'A128 R16 N80763 F2000.000 D126192 T0 S0',
#'A128 R16 N101674 F2000.000 D158866 T0 S0',
#'A128 R16 N128000 F2000.000 D200000 T0 S0',
                    'S2']
#%% N/F/(D*10): 10 points per period
#fftIndex = [2000/200e3/(8e-6*20)/2, 
#                 2000/100e3/(16e-6*20)/2,
#                 2000/50e3/(32e-6*20)/2]
                 
fftIndex = [1500, 944, 
#1189, 1496, 1256, 1581, 1493, 1504, 1352, 1489, 
#1500, 1453, 1486, 1497, 1507, 1482, 1493, 125, 124, 130, 
#125, 99, 79, 63, 50, 40, 31, 25, 20, 16, 
#16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 
#16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 
#16
]	

#%%
frequencyVector = [2.500000e+04, 1.985821e+04, 
#1.577393e+04, 1.252968e+04, 9.952679e+03, 7.905694e+03, 6.279716e+03, 4.988156e+03, 3.962233e+03, 3.147314e+03, 
#2.500000e+03, 1.985821e+03, 1.577393e+03, 1.252968e+03, 9.952679e+02, 7.905694e+02, 6.279716e+02, 4.988156e+02, 3.962233e+02, 3.147314e+02, 
#2.500000e+02, 1.985821e+02, 1.577393e+02, 1.252968e+02, 9.952679e+01, 7.905694e+01, 6.279716e+01, 4.988156e+01, 3.962233e+01, 3.147314e+01, 
#2.500000e+01, 1.985821e+01, 1.577393e+01, 1.252968e+01, 9.952679e+00, 7.905694e+00, 6.279716e+00, 4.988156e+00, 3.962233e+00, 3.147314e+00, 
#2.500000e+00, 1.985821e+00, 1.577393e+00, 1.252968e+00, 9.952679e-01, 7.905694e-01, 6.279716e-01, 4.988156e-01, 3.962233e-01, 3.147314e-01, 
#2.500000e-01
]

#%% 
t0 = time.time()
for i in range(len(ListOfParameters)-1):  
    parameters = TeeCom.SendParameters(ListOfParameters[i])     
    print(parameters)
 
    t1 = time.time()
    TeeCom.ReceiveData(ListOfInfo, ListOfBytes)  
    print("Aquisition time: %.1f s" % (time.time()-t1))
    print("\n")

TeeCom.SendParameters(ListOfParameters[-1])
print("Total time: %.1f s" % (time.time()-t0))

#%%
"""Close communication."""        
TeeCom.Close()

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
pl.clf
pl.semilogx(frequencyVector, abs(Z), 'bs')
#pl.ylim( 0.95, 1.05 )
pl.ylabel('Ratio between channels')
pl.subplot(2, 1, 2)
pl.clf
pl.semilogx(frequencyVector, np.angle(Z, deg=True), 'gs' ) 
maxY = np.round(np.max(2*np.angle(Z, deg=True))/10)*10
pl.ylim( [maxY, 0].sort() )
pl.xlabel('Frequency / Hz')
pl.ylabel('Phaseshift between channels / dg')
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
"""Nyquist plot"""
pl.figure(10000)
pl.clf
pl.subplot(1,1,1)
pl.plot(Z.real, -(Z.imag), 'b+')
#pl.axis()
pl.show()

#%%

#%%
i = 0;
w = np.blackman(len(data[i][0]))
w = w/np.trapz(w);
A = fft(w*(data[i][0]-np.mean(data[i][0])))/len(data[i][0]);
w = np.blackman(len(data[i][1]))
w = w/np.trapz(w);
B = fft(w*(data[i][1]-np.mean(data[i][1])))/len(data[i][1]);
index = int(fftIndex[i]-1)
Z[i] = A[index]/B[index]
# plot fft
pl.figure(1000)
pl.clf()
pl.subplot(2, 1, 1)
pl.title('25 kHz sinus recorded at 200 kHz')
pl.semilogy(abs(A), 'b-')
pl.ylabel('A')
pl.subplot(2, 1, 2)
pl.semilogy(abs(B), 'g-') 
pl.ylabel('B')
pl.xlabel('Bins')
pl.show()  
        
