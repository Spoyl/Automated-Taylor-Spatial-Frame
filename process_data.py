# -*- coding: utf-8 -*-
"""
Created on Mon Apr 29 15:41:12 2019

@author: Oliver
"""

from readFile import *

BASE_DIR="C:\\Users\\Oliver\\Desktop\\GDPTestData\\"
FILENAME="TestData2019_04_25_19_25_39"
FILE_DIR=BASE_DIR+FILENAME
FC1 = 300
FC2 = 350
FS = 800

X_ARRAY, Y_ARRAY, Z_ARRAY=readDataFile(FILE_DIR)
X_ARRAY,Y_ARRAY,Z_ARRAY = rm_nans(X_ARRAY,Y_ARRAY,Z_ARRAY)
x_mean,y_mean,z_mean = find_means(X_ARRAY, Y_ARRAY, Z_ARRAY)

X_ARRAY,Y_ARRAY,Z_ARRAY = rm_means(X_ARRAY,Y_ARRAY,Z_ARRAY,x_mean,y_mean,z_mean)

b, a = butter_band(FC1,FC2,FS)
X_ARRAY_FILT,Y_ARRAY_FILT,Z_ARRAY_FILT = apply_filter(b,a,X_ARRAY,
                                                      Y_ARRAY,Z_ARRAY)

plot_signal_trio(X_ARRAY_FILT,Y_ARRAY_FILT,Z_ARRAY_FILT, FS)

# CALCULATE POWER SPECTRA
PSD_ARRAY = calc_power_spectra(X_ARRAY_FILT,Y_ARRAY_FILT,Z_ARRAY_FILT, FS)

plt.figure(figsize=(16,6))
plt.title("Power Spectral Density")
plt.plot(PSD_ARRAY[0],PSD_ARRAY[1])
plt.plot(PSD_ARRAY[2],PSD_ARRAY[3])
plt.plot(PSD_ARRAY[4],PSD_ARRAY[5])
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