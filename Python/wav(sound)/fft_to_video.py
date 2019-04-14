#!/usr/bin/python
# -*- coding: utf-8 -*-

import wave
import numpy
import os

wf = wave.open("T0.wav", "rb")
nframes = wf.getnframes()
framerate = wf.getframerate()
nchannels = wf.getnchannels()
str_data = wf.readframes(nframes)
wf.close()
wave_data = numpy.frombuffer(str_data, dtype=numpy.short)
wave_data.shape = -1,nchannels
wave_data = wave_data.T[0]
time = numpy.arange(0,nframes)*(1.0/framerate)
N=4196
f=framerate//25
d=N//2
n=(wave_data.shape[0]+f-1)//f
df = framerate*1./N
freq = [df*n for n in range(0,N)]
while freq[d]>4000:
    d-=10
maxx=0
print("Preparing...")
wave_data=numpy.append(wave_data,[0]*N)
wave_data=numpy.append([0]*(f//2),wave_data)
data=[]
print("Computing...")
t=4
for i in range(n):
    data.append(abs(numpy.fft.fft(wave_data[i*f:i*f+N])[:d]*2/N))
    maxx=max(maxx,max(data[-1]))
    if (i+1)*100//n>t:
        print("%d%%"%((i+1)*100//n))
        t+=5
print('Done')
maxx+=100
import matplotlib.pyplot as plt
print("Printing...")
t=4
p=plt.subplot(111,facecolor='blue')
for i in range(n):
    plt.axis([-100,4100,-100,maxx])
    plt.plot(freq[:d],data[i],'#000000')
    plt.savefig('temp/feq%d.jpg'%i)
    plt.cla()
    if (i+1)*100//n>t:
        print("%d%%"%((i+1)*100//n))
        t+=5
print('Done')
print("Generating...")
os.system("ffmpeg -i temp/feq%d.jpg -i T0.wav -vcodec libx264 -r 25 out.mp4 -y")
for i in range(n):
    os.remove("temp/feq%d.jpg"%i)
