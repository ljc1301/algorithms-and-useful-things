# -*- coding: utf-8 -*-
import wave
import numpy as np
# import scipy.signal as signal
import sys

nchannels,sampwidth,framerate = [int(i) for i in input().split()]

# time = 10

# # 产生10秒44.1kHz的100Hz - 1kHz的频率扫描波
# t = np.arange(0, time, 1.0/framerate)
# wave_data = signal.chirp(t, 100, time, 1000, method='linear') * 10000
# wave_data = wave_data.astype(np.short)

wave_data=np.array([])
for channel in range(nchannels):
    wave_data=np.append(wave_data,np.array([int(i) for i in input().split()]))
wave_data=wave_data.astype(np.dtype('i%d'%sampwidth))
wave_data.shape=nchannels,-1
wave_data=wave_data.T

# 打开WAV文档
f = wave.open(sys.argv[1], "wb")

# 配置声道数、量化位数和取样频率
f.setnchannels(nchannels)
f.setsampwidth(sampwidth)
f.setframerate(framerate)
# 将wav_data转换为二进制数据写入文件
f.writeframes(wave_data.tostring())
f.close()
