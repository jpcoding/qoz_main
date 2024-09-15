import sys
import ctypes
from ctypes.util import find_library
import numpy as np
import os



class QoZ:
    
    def __init__(self, QoZpath=None):
        
        self.compressor_path = QoZpath
         
    def comrpess_rel(self, data, rel_eb, config):
        