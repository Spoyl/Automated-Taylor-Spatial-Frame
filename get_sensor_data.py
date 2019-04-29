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
import scipy.signal as signal


FILENAME="test_dat"
PORT="com10"
BAUD_RATE=38400
XYZ_ARRAY=np.empty(3)
FILENAME="TestData"
X_ARRAY = []
Y_ARRAY = []
Z_ARRAY = []
DATETIME = time.strftime("%Y_%m_%d_%H_%M_%S", time.gmtime())


def writeData2File(f,writer,data):
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
    
    writer.writerow(data)
    
    return date


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


def readDataFile():
    f = open(FILENAME+DATETIME+'.txt', "r")
    lines = f.readlines()
    lines = lines[6:]
    
    for line in lines:
        
        line=line.rstrip("]\n")
        line=line.lstrip("[")
        line=line.split()
        X_ARRAY.append(np.float(line[0]))
        Y_ARRAY.append(np.float(line[1]))
        Z_ARRAY.append(np.float(line[2]))
    
    f.close()
    
    x_mean=np.nanmean(X_ARRAY)
    y_mean=np.nanmean(Y_ARRAY)
    z_mean=np.nanmean(Z_ARRAY)
    
    print(np.shape(X_ARRAY))
    print(np.shape(Y_ARRAY))
    
    return x_mean,y_mean,z_mean
    

if __name__ =="__main__":

    f=open(FILENAME+DATETIME+'.txt', 'w')
    writer = csv.writer(f,delimiter='\t')
    ser = serial.Serial(PORT,BAUD_RATE,timeout=1)
    date=time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime())

    f.write("HEADER\n")
    f.write('date='+date+"\n")
    f.write('Column1=X\n')
    f.write('Column2=Y\n')
    f.write('Column3=Z\n')
    f.write('DATA\n')
    
    try:
        for i in range(32000):
            data=access_serial_data(ser)
            f.writelines(str(data)+"\n")
            
    except KeyboardInterrupt:
        
        print("Read Halted")
        f.close()
        ser.close()
        x,y,z=readDataFile()
        print("Means:")
        print(x,y,z)
        t=np.arange(0,len(X_ARRAY))/800
        print(t)
        
        plt.figure(figsize=(16,6))
        for i,x in enumerate(X_ARRAY):
            if str(x) == "nan":
                X_ARRAY[i]=(X_ARRAY[i-1]+X_ARRAY[i+1])/2.
        plt.title("X axis")
        plt.grid()
        plt.plot(t, X_ARRAY)
        
        plt.figure(figsize=(16,6))
        for i,z in enumerate(Z_ARRAY):
            if str(z) == "nan":
                Z_ARRAY[i]=(Z_ARRAY[i-1]+Z_ARRAY[i+1])/2.   
        plt.title("Y axis")
        plt.grid()
        plt.plot(t, Y_ARRAY)
        plt.show()
        
        plt.figure(figsize=(16,6))
        for i,y in enumerate(Y_ARRAY):
            if str(y) == "nan":
                Y_ARRAY[i]=(Y_ARRAY[i-1]+Y_ARRAY[i+1])/2.
        
        plt.title("Z axis")
        plt.grid()
        plt.plot(t, Z_ARRAY)
        plt.show()
        
        f, psd_x_han = signal.welch(X_ARRAY, 800, nperseg=256, window = "hanning")

        plt.figure(figsize=(16,6))
        plt.title("X axis PSD")
        plt.plot(f, psd_x_han)
        plt.xlabel("Frequency, Hz")
        plt.grid()
        plt.show()


