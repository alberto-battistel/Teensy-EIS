# -*- coding: utf-8 -*-
"""
Created on Tue Aug 15 12:09:37 2017

@author: alberto
"""

import serial
import numpy as np
import pylab as pl

s = serial.Serial('/dev/ttyACM1')

sdata = s.read(10000)

data = np.fromstring(sdata, dtype=np.uint16)

pl.plot(data)
pl.show()

s.close()

