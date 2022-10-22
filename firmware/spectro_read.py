# -*- coding: utf-8 -*-
import time
import csv
import serial
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

### Convert calibration file to wavelengths###

n_pix=range(1,289,1)
A_0= 3.124280036e+2
B_1= 2.687199904
B_2= -9.981271483e-4;
B_3= -9.240661251e-6
B_4= 1.582987378e-8
B_5= -4.286955339e-12;

b1pix = [ B_1*(i) for i in n_pix]
b1pix = np.array(b1pix)
b2pix_2 = [ B_2*((i)**2) for i in n_pix]
b2pix_2 = np.array(b2pix_2)
b3pix_3 = [ B_3*((i)**3) for i in n_pix]
b3pix_3 = np.array(b3pix_3)
b4pix_4 = [ B_4*((i)**4) for i in n_pix]
b4pix_4 = np.array(b4pix_4)
b5pix_5 = [ B_5*((i)**5) for i in n_pix]
b5pix_5 = np.array(b5pix_5)

wavelengths= (A_0) + b1pix + b2pix_2 + b3pix_3 + b4pix_4 + b5pix_5

##################################################

hdr_wvl = wavelengths.astype(str) #make wavelengths strings for plotting
header = ['Date', 'Wvl (nm)','Raw Value']


ser = serial.Serial("COM15", 115200)
ser.flushInput()

f = open("DC_50ms.csv", "a")
writer = csv.writer(f, delimiter=',')

def makeFig(): #Create a function that makes our desired plot
    plt.title('Raw 16-bit Signal')      
    plt.grid(True)                                  
    plt.ylabel('Signal')
    plt.xlabel('Wavelength (nm)')   
    plt.ylim([2750,3900])                         
    plt.plot(wavelengths,pixels)       
    plt.show()


while True:
    try:
        while (ser.inWaiting()==0): #Wait here until there is data
            pass #do nothing
        start_time = time.time()
        pixels=str(ser.readline())
        #print("readline took", time.time() - start_time, "to run")
        #print(pixels[2:][:-4])
        pixels=pixels[2:][:-4].split(',')
        df = pd.DataFrame(pixels).T
        df2 = pd.DataFrame({'date':[time.time()]})
        df2 = df2.join(df)
        df2.to_csv("DC_50ms.csv",date_format='%Y-%m-%d',mode='a', index=False, header=False)
        #print("Writefile took", time.time() - start_time, "to run")
        pixels = [int(i) for i in pixels]
        
        makeFig()
    except:
        print("Keyboard Interrupt")
        ser.close()
        f.close()
        break