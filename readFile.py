# -*- coding: utf-8 -*-
"""
Data Processing Script for GDP group 47 - 
        Fracture Fixation Device
        University of Southampton

Created on Wed Apr 24 22:52:34 2019

@author: Oliver Stocks
"""

import os
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
import numpy as np

# GLOBAL VARIABLES
BASE_DIR="C:\\Users\\Oliver\\Desktop\\GDPTestData\\"
FILENAME="TestData2019_04_25_17_15_10"
FILE_DIR=BASE_DIR+FILENAME
FC1 = 300
FC2 = 350
FS = 800
# ----------------


def plot_signal_trio(X_ARRAY,Y_ARRAY,Z_ARRAY):
    """
    
    """
    plt.figure(figsize=(16,10))
    plt.subplot(311)
    plt.title("X axis")
    plt.grid()
    plt.plot(t, X_ARRAY)
    
    plt.subplot(312)
    plt.title("Y axis")
    plt.ylabel("Signal")
    plt.grid()
    plt.plot(t, Y_ARRAY)
    
    plt.subplot(313)
    plt.title("Z axis")
    plt.grid()
    plt.plot(t, Z_ARRAY)
    plt.xlabel("time, s")
    plt.show()


def apply_filter(b,a,X_ARRAY,Y_ARRAY,Z_ARRAY):
    """
    Applies a defined filter b a to the x y and z axes
    
    
    """
    X_ARRAY_FILT = signal.lfilter(b, a, X_ARRAY)
    
    Y_ARRAY_FILT = signal.lfilter(b, a, Y_ARRAY)
    
    Z_ARRAY_FILT = signal.lfilter(b, a, Z_ARRAY)
    
    return X_ARRAY_FILT, Y_ARRAY_FILT, Z_ARRAY_FILT


def rm_means(X_ARRAY,Y_ARRAY,Z_ARRAY):
    """
    Remove means from the x y and z arrays
    """
    for i,x in enumerate(X_ARRAY):
        X_ARRAY[i]=x-x_mean
    
    for i,z in enumerate(Z_ARRAY):
        Z_ARRAY[i]=z-z_mean
    
    for i,y in enumerate(Y_ARRAY):
        Y_ARRAY[i]=y-y_mean
    
    return X_ARRAY, Y_ARRAY, Z_ARRAY
    

def rm_nans(X_ARRAY,Y_ARRAY,Z_ARRAY):
    """ 
    Remove NaNs from the x y and z arrays
    
    When/if a 'nan' is found, the script replaces the value
    with the average of it's neighbours.
    """
    for i,x in enumerate(X_ARRAY):
        if str(x) == "nan":
            X_ARRAY[i]=(X_ARRAY[i-1]+X_ARRAY[i+1])/2.
    
    for i,z in enumerate(Z_ARRAY):
        if str(z) == "nan":
            Z_ARRAY[i]=(Z_ARRAY[i-1]+Z_ARRAY[i+1])/2.        
    
    for i,y in enumerate(Y_ARRAY):
        if str(y) == "nan":
            Y_ARRAY[i]=(Y_ARRAY[i-1]+Y_ARRAY[i+1])/2.
    
    return X_ARRAY, Y_ARRAY, Z_ARRAY


def butter_band(fc1, fc2, fs, order=5):
    """
    Create a butterworth bandpass filter between the frequencies
    of fc1 and fc2.
    
    Returns:
        b (array): numerator coefficients
        a (array): denominator coefficients
    """
    
    fnyq=fs/2
    passband = [fc1/fnyq, fc2/fnyq]     #numpy requires a normalized (?) frequency for digital signals
    
    b,a = signal.butter(order,passband,btype='band')    #make filter
    w,h=signal.freqz(b,a)   #find frequencies
    
    plt.figure(figsize=(10,6))
    plt.semilogx((fs*0.5/np.pi)*w, abs(h), label=str(order))
    plt.axvline(fc1,color= "orange")
    plt.axvline(fc2, color="orange")
    plt.xlabel("Frequency")
    plt.ylabel("Gain")
    plt.title("Butterworth Bandpass Filter")
    plt.legend(title="Order")
    plt.grid()
    plt.savefig("butterband.png")
    plt.show()
    
    return b,a


def readDataFile(FILE_DIR):
    """
    Function to read data from the experiment files.
    
    Returns:
        x_mean, y_mean, z_mean (float)
    """
    X_ARRAY = []
    Y_ARRAY = []
    Z_ARRAY = []
    
    f = open(FILE_DIR+'.txt', "r")
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
    
    return X_ARRAY, Y_ARRAY, Z_ARRAY
  

def find_means(X_ARRAY, Y_ARRAY, Z_ARRAY):
    x_mean=np.mean(X_ARRAY)
    y_mean=np.mean(Y_ARRAY)
    z_mean=np.mean(Z_ARRAY)
    
    return x_mean,y_mean,z_mean


X_ARRAY, Y_ARRAY, Z_ARRAY=readDataFile(FILE_DIR)
X_ARRAY,Y_ARRAY,Z_ARRAY = rm_nans(X_ARRAY,Y_ARRAY,Z_ARRAY)
x_mean,y_mean,z_mean = find_means(X_ARRAY, Y_ARRAY, Z_ARRAY)

print("Means:")
print("x:\t"+str(round(x_mean)))
print("y:\t"+str(round(y_mean)))
print("z:\t"+str(round(z_mean)))

t=np.arange(0,len(X_ARRAY))/FS # Create time axis

X_ARRAY,Y_ARRAY,Z_ARRAY = rm_means(X_ARRAY,Y_ARRAY,Z_ARRAY)

b, a = butter_band(FC1,FC2,FS)
X_ARRAY_FILT,Y_ARRAY_FILT,Z_ARRAY_FILT = apply_filter(b,a,X_ARRAY,
                                                      Y_ARRAY,Z_ARRAY)

plot_signal_trio(X_ARRAY_FILT,Y_ARRAY_FILT,Z_ARRAY_FILT)

# CALCULATE POWER SPECTRA
f1, psd_x = signal.welch(X_ARRAY_FILT, FS, nperseg=256, window = "hanning")
f2, psd_y = signal.welch(Y_ARRAY_FILT, FS, nperseg=256, window = "hanning")
f3, psd_z = signal.welch(Z_ARRAY_FILT, FS, nperseg=256, window = "hanning")

plt.figure(figsize=(16,6))
plt.title("Power Spectral Density")
plt.plot(f1, psd_x)
plt.plot(f2, psd_y)
plt.plot(f3, psd_z)
plt.ylabel("Power")
plt.xlabel("Frequency, Hz")
plt.grid()
plt.show()
# -----------------------

#x_fft=np.fft.fft(X_ARRAY)
#f=np.fft.fftfreq(len(X_ARRAY), 1/800)
#plt.figure(figsize=(16,6))
#plt.title("X axis Fft")
#plt.plot(f, x_fft)
#plt.xlabel("Frequency, Hz")
#plt.grid()
#plt.show()        