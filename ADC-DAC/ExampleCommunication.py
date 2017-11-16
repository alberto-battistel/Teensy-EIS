# -*- coding: utf-8 -*-
"""
Example on how to use TeensyCommunication

Created on Thu Nov 16 20:50:00 2017

@author: Alberto Battistel
"""
import numpy as np
import pylab as pl
from scipy.fftpack import fft
import TeensyCommunication as TeeCom


#%%
"Parameters to control the DAC and the ADC."
parameters2send = ['A128 R16 N1000 F4000.000 D100 T0 S0']

#%% prepare list to receive the data
"to store the data as byte"
list4bytes = []   
"to store the info of the streaming"
list4info = [] 

#%%
"Start"
TeeCom.Init()

#%%
"Send parameter"
CorrectedParameters = TeeCom.SendParameters(parameters2send[0])
print(CorrectedParameters)

#%%
"Receive data"
list4info, list4bytes = TeeCom.ReceiveData()

#%%
"Finish experiment"
CorrectedParameters = TeeCom.SendParameters('S2')
print(CorrectedParameters)
 
#%%
"Free Teensy" 
#TeeCom.Close() 
TeeCom.Reset()

#%%
"Machine the data"
data = TeeCom.CollectData(list4bytes)

#%%
"Results"

"F-domain"
w = np.blackman(len(data[0]))
w = w/np.trapz(w);
A = fft(w*data[0])/len(data[0]);
w = np.blackman(len(data[1]))
w = w/np.trapz(w);
B = fft(w*data[1])/len(data[1]);

"Plot"
pl.figure(1)
pl.clf()

pl.subplot(2, 2, 1)
pl.plot(data[0], 'b')
pl.ylabel('Signal / a.u.')
pl.subplot(2, 2, 3)
pl.plot(data[1], 'g')
pl.ylabel('Signal / a.u.')
pl.xlabel('t / a.u.')

pl.subplot(2, 2, 2)
pl.semilogy(abs(A), 'b+')
pl.ylabel('Abs(Signal) / a.u.')
pl.subplot(2, 2, 4)
pl.semilogy(abs(B), 'g+')
pl.ylabel('Abs(Signal) / a.u.')
pl.xlabel('f / a.u.')
pl.show()
