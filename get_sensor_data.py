# -*- coding: utf-8 -*-
"""
Created on Thu Apr 11 13:44:47 2019

@author: Oliver
"""

import csv
import serial
import time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as anim
import threading


FILENAME="test_dat"
PORT="com10"
BAUD_RATE=9600
XYZ_ARRAY=np.empty(3)
    
    
def writeData2File(f,writer):
    """
    Reads a line of serial data (x, y, z) using access_serial_data and
    writes the data to a csv file.
    
        Parameters
        ----------
        
            f: (str) pointer to open file set to 'wt'
            
            writer: (str) pointer to a csv writer object
    """
    
    writer.writerow(['[Header]'])
    date=time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime())
    writer.writerow(['date='+date])
    writer.writerow(['Column1=X axis'])
    writer.writerow(['Column2=Y axis'])
    writer.writerow(['Column3=Z axis'])
    writer.writerow(['[Data]'])
    
    writer.writerow((x,y,z))


def access_serial_data(ser):
    """
    Reads data coming through the serial port assuming a three int format
    as defined in the corresponding arduino program.
    
    
        Parameters
        ----------
                    
            ser: (<Serial object>) An opened serial port
                    
        Returns
        -------
        
            [x, y, z]: (array) Holds the value for each axis
    """
    
    line=ser.readline()
    
    try:
        data=line.split()
        XYZ_ARRAY[0]=data[0]
        XYZ_ARRAY[1]=data[1]
        XYZ_ARRAY[2]=data[2]
        print(str(XYZ_ARRAY))

    except IndexError:
        XYZ_ARRAY[0]=np.nan
        XYZ_ARRAY[1]=np.nan
        XYZ_ARRAY[2]=np.nan
        print(str(XYZ_ARRAY)+"\t<< Didn't receive three points")
    
    except ValueError:
        XYZ_ARRAY[0]=np.nan
        XYZ_ARRAY[1]=np.nan
        XYZ_ARRAY[2]=np.nan
        print(str(XYZ_ARRAY)+"\t<< Weird data")
    
    return XYZ_ARRAY


if __name__ =="__main__":
    
#    f=open(FILENAME+'.csv', 'wt')
#    writer = csv.writer(f,delimiter='\t')
    ser = serial.Serial(PORT,BAUD_RATE,timeout=1)
    
#    fig, ax = plt.subplots(figsize=(8,5))    
    
    try:
        for i in range(32000):
            access_serial_data(ser)
            
    except KeyboardInterrupt:
        print("Read Halted")

#    f.close()
    ser.close()
