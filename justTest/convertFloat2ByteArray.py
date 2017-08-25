# -*- coding: utf-8 -*-
"""
Created on Tue Aug 15 16:00:03 2017

@author: alberto
"""

import struct
import numpy as np

#value = np.float32(1)
value = 1
print(value)

ba = bytearray(struct.pack("h", value))  
print(ba)
print([ "0x%01x" % b for b in ba ])