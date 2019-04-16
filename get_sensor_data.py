# -*- coding: utf-8 -*-
"""
Created on Thu Apr 11 13:44:47 2019

@author: Oliver
"""

import csv
import serial
import time
 
FILENAME="test_dat"
PORT="com10"
BAUD_RATE=9600
    
    
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


def access_serial_data(baud_rate,port="com10"):
    """
    Reads data coming through the serial port assuming a three int format
    as defined in the corresponding arduino program.
    
    
        Parameters
        ----------
                    
            baud_rate: (int) Defines the baud rate of the port being read.
                    The rate can be found in the Arduino code - line should 
                    read "Serial.begin(<baud_rate>)".
                    
            port: (str) Define the com port to be read. Defaults to com10. 
                        The port can be found under "tools - port" in the 
                        Arduino interface.
                    
        Returns
        -------
        
            [x, y, z]: (array) Holds the value for each axis
    """
    
    kk=ser.readline()
    kk2=kk.split()
    print(kk2[0], kk2[1], kk[2])
    x=kk2[0]
    y=kk2[1]
    z=kk2[2]
        
    return [x,y,z]
    

if __name__ =="__main__":
    f=open(FILENAME+'.csv', 'wt')
    writer = csv.writer(f,delimiter='\t')
    writeData2File(f, writer)
    #ser = serial.Serial(PORT,BAUD_RATE,timeout=1)
    f.close()
    ser.close()
