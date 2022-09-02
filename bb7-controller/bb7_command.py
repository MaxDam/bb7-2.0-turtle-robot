#source activate py36

from bb7 import BB7
from bb7_motion import BB7Motion
import cv2 as cv
import time

INPUT_RTSP	= "rtsp://192.168.1.6:8554/mjpeg/1";
MQTT_BROKER	= "192.168.1.8"

faceDetector = cv.dnn.readNetFromCaffe("model/weights-prototxt.txt", "model/res_ssd_300Dim.caffeModel" )

def detect_face(frame):
	global faceDetector
	(height, width) = frame.shape[:2]
	blob = cv.dnn.blobFromImage(cv.resize(frame, (300, 300)), 1.0, (300, 300), (104.0, 177.0, 123.0))
	faceDetector.setInput(blob)
	detections = faceDetector.forward()
	if len(detections) > 0:
		for i in range(0, detections.shape[2]):
			confidence = detections[0, 0, i, 2]
			if confidence < 0.3:
				continue
			box = detections[0, 0, i, 3:7] * np.array([width, height, width, height])
			(x1, y1, x2, y2) = box.astype("int")
			y = y1 - 10 if y1 - 10 > 10 else y1 + 10
			cv.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 3)
	return frame

def acquire_frame(frm):
	#frm = detect_face(frm)
	cv.imshow("current frame", frm)

bb7  = BB7(acquire_video=True, video_callback=acquire_frame)
bb7M = BB7Motion(bb7)
while not bb7.connected:
	time.sleep(1)

bb7M.standup()
time.sleep(0.5)

bb7M.stepForward()

time.sleep(0.5)
bb7M.stepTurnLeft()

#frame = bb7.lastFrame()
#frame = detect_face(frame)
#cv.imshow("last frame", frame)
	
#bb7.head(40).neck(20).armFrontRight(30).send()
#time.sleep(0.5)

#print("last distnace: " + bb7.lastDistance())
#time.sleep(0.5)

#bb7M.stepForward(4, delay=0.1)
#time.sleep(0.5)
