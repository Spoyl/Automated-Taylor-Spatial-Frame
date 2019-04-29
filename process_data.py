# -*- coding: utf-8 -*-
"""
Created on Mon Apr 29 15:41:12 2019

@author: Oliver
"""

from readFile import *
from os import listdir
from numpy import mean

BASE_DIR="C:\\Users\\Oliver\\Desktop\\GDPTestData\\"
FILENAME="TestData2019_04_25_19_25_39"
FC1 = 300
FC2 = 350
FS = 800
Z_AVE_ARRAY = []
X_AVE_ARRAY = []
Y_AVE_ARRAY = []

for file_name in listdir(BASE_DIR):
    
    FILE_DIR=BASE_DIR+file_name
    
    # GET ZYX DATA
    X_ARRAY, Y_ARRAY, Z_ARRAY=readDataFile(FILE_DIR)
    X_ARRAY,Y_ARRAY,Z_ARRAY = rm_nans(X_ARRAY,Y_ARRAY,Z_ARRAY)
    x_mean,y_mean,z_mean = find_means(X_ARRAY, Y_ARRAY, Z_ARRAY)
    X_ARRAY,Y_ARRAY,Z_ARRAY = rm_means(X_ARRAY,Y_ARRAY,Z_ARRAY,x_mean,y_mean,z_mean)
    # ------------
    
    # FILTER THE DATA
    b, a = butter_band(FC1,FC2,FS)
    X_ARRAY_FILT,Y_ARRAY_FILT,Z_ARRAY_FILT = apply_filter(b,a,X_ARRAY,
                                                          Y_ARRAY,Z_ARRAY)
    
    # CALCULATE POWER SPECTRA
    PSD_ARRAY = calc_power_spectra(X_ARRAY_FILT,Y_ARRAY_FILT,Z_ARRAY_FILT, FS)
    
    # PLOTS
    #plot_signal_trio(X_ARRAY_FILT,Y_ARRAY_FILT,Z_ARRAY_FILT, FS)
    #plot_power_spectra(PSD_ARRAY)
    
    X_AVE_ARRAY.append(mean(PSD_ARRAY[1]))
    Y_AVE_ARRAY.append(mean(PSD_ARRAY[3]))
    Z_AVE_ARRAY.append(mean(PSD_ARRAY[5]))
    
plt.figure(figsize=(16,6))
t_ax = np.arange(0,len(X_AVE_ARRAY))
plt.plot(t_ax, X_AVE_ARRAY)
plt.grid()
plt.show()

plt.figure(figsize=(16,6))
t_ax = np.arange(0,len(Y_AVE_ARRAY))
plt.plot(t_ax, Y_AVE_ARRAY)
plt.grid()
plt.show()

plt.figure(figsize=(16,6))
t_ax = np.arange(0,len(Z_AVE_ARRAY))
plt.plot(t_ax, Z_AVE_ARRAY)
plt.grid()
plt.show()


#x_fft=np.fft.fft(X_ARRAY)
#f=np.fft.fftfreq(len(X_ARRAY), 1/800)
#plt.figure(figsize=(16,6))
#plt.title("X axis Fft")
#plt.plot(f, x_fft)
#plt.xlabel("Frequency, Hz")
#plt.grid()
#plt.show()  