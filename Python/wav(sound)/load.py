# -*- coding: utf-8 -*-

import wave
# import pylab as pl
import numpy as np
import sys

# 打开WAV文档
f = wave.open(sys.argv[1], "rb")
# 读取格式信息
# (nchannels, sampwidth, framerate, nframes, comptype, compname)
params = f.getparams()
nchannels, sampwidth, framerate, nframes = params[:4]
# 读取波形数据
str_data = f.readframes(nframes)
f.close()
#将波形数据转换为数组
# wave_data = np.fromstring(str_data, dtype=np.short)
wave_data = np.frombuffer(str_data, dtype=np.dtype('i%d'%sampwidth))
wave_data.shape = -1, nchannels
wave_data = wave_data.T
print(nchannels, sampwidth, framerate, nframes)
for i in wave_data:
    for j in i:
        print(j,end=' ')
    print('')
# wave_data.shape = -1, 2
# wave_data = wave_data.T
# time = np.arange(0, nframes) * (1.0 / framerate)
# # 绘制波形
# pl.subplot(211) 
# pl.plot(time, wave_data[0])
# pl.subplot(212) 
# pl.plot(time, wave_data[1], c="g")
# pl.xlabel("time (seconds)")
# pl.show()
