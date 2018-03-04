#!/usr/bin/env python  
from PIL import Image #pip install Image
import sys
im = Image.open(sys.argv[1])
width = im.size[0]  
height = im.size[1]  
cnt = len(im.getpixel((0, 0)))
sys.stdout.write('%d %d %d\n'%(height,width,cnt))
for h in xrange(0, height):  
    for w in xrange(0, width):  
        pixel = im.getpixel((w, h))
        for i in xrange(len(pixel)):
            if i>0:
                sys.stdout.write(' ')
            sys.stdout.write(str(pixel[i]))
        sys.stdout.write('\n')
    sys.stdout.write('\n')
sys.stdout.close()
im.close();
