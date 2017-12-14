# -*- coding: utf-8 -*-
"""
Created on Fri Dec 15 00:03:29 2017

@author: alberto
"""

#!/usr/bin/python
from usb.core import find as finddev
dev = finddev(idVendor=0x16c0, idProduct=0x0483)
dev.reset()