# -*- coding: utf-8 -*-
"""
Module to communicate with Teensy

Created on Thu Nov 16 20:41:20 2017

@author: Alberto Battistel
"""

#%%

import serial
from serial.tools import list_ports
import time
import numpy as np

"Number of ADC channels"
numADC = 2


#%%
""" Function definition to communicate with Teensy"""

def SearchTeensy():
    """Collect serial port infor and look for Teensy."""
    
    ListPortInfo = list_ports.comports()
    
    for port in ListPortInfo:
        if port.manufacturer == 'Teensyduino':
            SerialName = port.device
            return SerialName
            return port
    

def Init():
    """Open serial communication and reset/start Teensy.
    
    Teensy should be on /dev/ttyACM0 (at least on my ubuntu 16.04).
    Remember to close the communication."""
    
    SerialName = SearchTeensy()    
    print(SerialName)
    
    global teensy
    teensy = serial.Serial(SerialName, 
                      timeout=0.2)
    #                  xonxoff=0,
    #                  rtscts=0,
    #                  bytesize=serial.EIGHTBITS,
    #                  parity=serial.PARITY_NONE,
    #                  stopbits=serial.STOPBITS_ONE)
#    "Toggle DTR to reset Teensy (This makes it start!!)"
#    teensy.setDTR(False)
#    time.sleep(1)
#    teensy.setDTR(True)    
    Reset()


def Reset():
    """Reset Teensy.
    
    This should make restart the microcontroller.
    Not totally sure how this is really working."""
    
    "Toggle DTR to reset Teensy (This makes it start!!)"
    teensy.setDTR(False)
    time.sleep(1)
    teensy.setDTR(True)

    
def SendParameters(parameters2send):
    """Send parameters to Teensy.
    
    Teensy is by default waiting to receive some bytes.
    When it receive a "S0" it gives back the working parameters (all of them) 
    and corrected if you send something wrong and starts the aquistion."""
    
    "Send the string of parameters and then fluch to be sure nothing is waiting." 
    teensy.reset_input_buffer()
    teensy.write(parameters2send.encode('utf-8'))
    teensy.flush()
    
    message = ''    
    while message != b'Go!\r\n': 
        "Go!\r\n means that it is sending back the parameters."
        message = teensy.readline()
        parametersReceived = message.decode('utf-8')
        print(parametersReceived)
        continue
    return parametersReceived


def ReceiveData():
    """Receive the data.
    
    Data come in blocks. Teensy send first an ascii value with end of line (info)
    to advice how many data are coming and then the bytes of the data twice, 
    one for each ADC channel."""

    "To store the number of values"    
    stackInfo = [[], []]
    "To store bytes"
    stackBytes = [[], []]
    "To track the length of the block of data"
    totalLength = [0,0]
    
    StreamNotCompleted = [True, True]
    
    while all(StreamNotCompleted):
        
        for ADC in range(numADC):  
            "Get info."
            #print("%d\tWaiting Byte: \t%d" % (ADC, teensy.inWaiting()))        
            blockInfo = teensy.readline()
            stackInfo[ADC].append(blockInfo)
            
            if blockInfo is b'':
                print("Done %d" % ADC)
                StreamNotCompleted[ADC] = False
                continue
            if len(blockInfo) > 6:
                print("Error %d" % ADC)
                StreamNotCompleted = False
                break    
            
            "Get bytes"
            byte2read = 2*int(blockInfo.decode('utf-8'))
            byteBlock = teensy.read(byte2read) # read bytes 
            stackBytes[ADC].append(byteBlock)
            totalLength[ADC] = totalLength[ADC] + len(byteBlock)
            #print("Byte to read \t%d" % (byte2read/2))
            #print("%d\tWaiting Byte: \t%d" % (ADC, teensy.inWaiting()))      
            #print("%d\tLength: \t%d" % (ADC, (totalLength[ADC]/2)))
    
#    "Attach info and bytes"
#    list4info.append(stackInfo)
#    list4bytes.append(stackBytes)
        
    for ADC in range(numADC):
        print("Byte aquired: \t%d" % totalLength[ADC])

    return stackInfo, stackBytes
    

def Close():
    """Close communication with Teensy.
    
    Now Teensy can interface with Arduino IDE, for example.""" 
    
    teensy.close()
    print("Teensy was closed")


#%%
""" Function definition to use the data coming from Teensy"""

def CollectData(stackBytes):
    """Convert the collected bytes into an array.
    
    Teensy send the data as block of variable length of 16-bit data, 
    one for every channel.
    stackBytes is a list made as [[ADC1_block1, ADC2_block1], [ADC1_block2,ADC2_block2], ...] 
    """  
    
    "Final data as numerical array"
    data = [[], []] 
#    "Intermediate store of the blocks of byte"
#    stack4data = [[], []]
 
    for ADC in range(numADC):
        "Check length of data"
        lengthList = 0
        for i in range(len(stackBytes[ADC])):
            lengthList = lengthList + len(stackBytes[ADC][i])/2
    
        "Make data array"
        print("Length of streamed data: \t%d" % ADC, lengthList)
        data[ADC].append(np.zeros([int(lengthList)], dtype=np.uint16))
        
        "Fill data array"
        l1 = 0;
        for i in range(0,len(stackBytes[ADC])):
            l2 = l1 + int(len(stackBytes[ADC][i])/2)
            data[ADC][l1:l2] = np.fromstring(stackBytes[ADC][i], dtype=np.uint16)
            l1 = l2
    
    return data