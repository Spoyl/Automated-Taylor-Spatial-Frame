# -*- coding: utf-8 -*-
"""
Created on Mon Apr 29 15:41:12 2019

@author: Oliver
"""

from readFile import *
from os import listdir
from numpy import mean
import datetime


BASE_DIR="C:\\Users\\Oliver\\Desktop\\GDPTestData\\"
FILENAME="TestData2019_04_25_19_25_39"
FC1 = 360
FC2 = 390
FS = 800
Z_AVE_ARRAY = []
X_AVE_ARRAY = []
Y_AVE_ARRAY = []
DATETIME_ARRAY = []
TIME_DELTAS = []
TIME_AXIS = []


for file_name in listdir(BASE_DIR):
    
    FILE_DIR=BASE_DIR+file_name
    
    time = datetime.datetime.strptime(file_name[8:-4], "%Y_%m_%d_%H_%M_%S")
    DATETIME_ARRAY.append(time)
    
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
    
    ## PLOTS
    #plot_signal_trio(X_ARRAY_FILT,Y_ARRAY_FILT,Z_ARRAY_FILT, FS)
    #plot_power_spectra(PSD_ARRAY)
    
    X_AVE_ARRAY.append(mean(PSD_ARRAY[1]))
    Y_AVE_ARRAY.append(mean(PSD_ARRAY[3]))
    Z_AVE_ARRAY.append(mean(PSD_ARRAY[5]))

for time in DATETIME_ARRAY:
    TIME_DELTAS.append(time-DATETIME_ARRAY[0])

for i,t in enumerate(TIME_DELTAS):
    TIME_AXIS.append(t.total_seconds())

plot_ave_power(X_AVE_ARRAY,Y_AVE_ARRAY,Z_AVE_ARRAY, TIME_AXIS)
