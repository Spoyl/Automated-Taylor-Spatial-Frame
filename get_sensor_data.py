# -*- coding: utf-8 -*-
"""
Created on Thu Apr 11 13:44:47 2019

@author: Oliver
"""

import csv
import serial
import time
 
#create filename
name="test_dat"

f=open(name+'.csv', 'wt');

#Initialize the CSV writer
writer = csv.writer(f,delimiter='\t')

#Add file header
writer.writerow(['[Header]'])
date=time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime())
writer.writerow(['date='+date])
writer.writerow(['Column1=X axis'])
writer.writerow(['Column2=Y axis'])
writer.writerow(['Column3=Z axis'])
writer.writerow(['[Data]'])
 
#Open Serial port
ser = serial.Serial('com10',9600,timeout=1)

#read data
for t in range(100):
    
    kk=ser.readline()
    
    #String in form <xval>\t<yval>\t<zval>\n
    kk2=kk.split()
    print(kk2[0], kk2[1], kk[2])
    x=kk2[0]
    y=kk2[1]
    z=kk2[2]
    
    #We put each data point in a new row of the data file.
    writer.writerow((x,y,z))

 
#When finish, close the file and the serial port.
f.close();
ser.close();