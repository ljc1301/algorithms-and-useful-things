#!/usr/bin/env python  
from PIL import Image #pip install Image
import sys
height,width=[int(_) for _ in sys.stdin.readline()[:-1].split()]
im = Image.new(sys.stdin.readline()[:-1],(width,height))
for h in xrange(0, height):  
    for w in xrange(0, width):
        im.putpixel((w, h),tuple([int(_) for _ in sys.stdin.readline()[:-1].split()]))
    sys.stdin.readline()
im.save(sys.argv[1]) 
im.close()
