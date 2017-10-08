from collections import deque
import numpy as np
import argparse
import imutils
import cv2
import serial
import smbus
import struct
import time

ap = argparse.ArgumentParser()
ap.add_argument("-v","--video",
	help="path to the (oprtional) video file ")
ap.add_argument("-b","--buffer",type=int,default=100,
	help="max buffer size")
args = vars(ap.parse_args())
colorLower = (14,110,150)
colorUpper = (35,255,255)
bus = smbus.SMBus(1)
address = 0x04
def writeNumber(s,x,y):
	try:
		bus.write_i2c_block_data(address,0,[s,x,y])
	except IOError:
		print "IO Error"
	return -1

if not args.get("video",False):
	camera = cv2.VideoCapture(0)
else:
	camera = cv2.VideoCapture(args["video"])
while True:
	(grabbed,frame) = camera.read()
	if args.get("video")and not grabbed:
		break
	frame = imutils.resize(frame,width=400,height=300)
	hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
	mask = cv2.inRange(hsv,colorLower,colorUpper)
	mask = cv2.erode(mask,None,iterations=2)
	mask = cv2.dilate(mask,None,iterations=2)
	cnts = cv2.findContours(mask.copy(),cv2.RETR_EXTERNAL,
		cv2.CHAIN_APPROX_SIMPLE)[-2]
	
	if len(cnts)>0:
		c = max(cnts,key=cv2.contourArea)
		((x,y),radius) = cv2.minEnclosingCircle(c)
		M = cv2.moments(c)
		if radius >15:
			cv2.circle(frame,(int(x),int(y)),int(radius),
					(0,255,255),2)
			writeNumber(1,int(x*9/20),int(y*3/5))
	else:
		x = 0
		y = 0
		writeNumber(0,int(x),int(y))
	key = cv2.waitKey(1) & 0xFF
	if key == ord('q'):
		break
camera.release()
cv2.destroyAllWindows()

