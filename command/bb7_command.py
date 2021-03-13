from bb7 import BB7
from bb7_motion import BB7Motion
import cv2 as cv
import time

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

bb7 = BB7(acquire_video=False, video_callback=acquire_frame)
#bb7M = BB7Motion(bb7)
while not bb7.connected:
	time.sleep(1)

#bb7.head(40).neck(20).armFrontRight(30).send()
#time.sleep(0.5)

#print("last distnace: " + bb7.lastDistance())
#time.sleep(0.5)

#bb7M.stepForward(4, delay=0.1)
#time.sleep(0.5)

#frame = bb7.lastFrame()
#frame = detect_face(frame)
#cv.imshow("last frame", frame)

arm_zero_pos = 50
weight_side_diff = 50
delay = 500
bb7.zero(arm_zero_pos).send()
#time.sleep(delay)
#conta i passi in avanti
for i in range(2):
	#porta il peso indietro a sinistra
	bb7.armBackRight(arm_zero_pos-80).armBackLeft(arm_zero_pos-80-weight_side_diff) \
	.delay(delay) \
	#muove a gamba anteriore destra per il passo
	.armFrontRight(-60) \
	#time.sleep(delay)
	.shoulderFrontRight(60) \
	.delay(delay) \
	#reimposta l'assetto gambe
	.armFrontRight(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) \
	.delay(delay) \
	#porta il peso in avanti a destra
	.armFrontRight(arm_zero_pos-90-weight_side_diff).armFrontLeft(arm_zero_pos-90) \
	.delay(delay) \
	#muove a gamba posteriore sinistra per il passo
	.armBackLeft(-60) \
	.delay(delay) \
	.shoulderBackLeft(60) \
	.delay(delay) \
	#reimposta l'assetto gambe
	.armBackLeft(arm_zero_pos).armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos) \
	.delay(delay) \
	#effettua la spinta per il passo
	.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos) \
	.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) \
	.shoulderFrontRight(0).shoulderFrontLeft(-40) \
	.shoulderBackLeft(0).shoulderBackRight(-40) \
	.delay(delay) \

	
	#porta il peso indietro a destra
	.armBackRight(arm_zero_pos-80-weight_side_diff).armBackLeft(arm_zero_pos-80) \
	.delay(delay) \
	#muove a gamba anteriore sinistra per il passo
	.armFrontLeft(-60) \
	.delay(delay) \
	.shoulderFrontLeft(60) \
	.delay(delay) \
	#reimposta l'assetto gambe
	.armFrontLeft(arm_zero_pos) \
	.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) \
	.delay(delay) \
	#porta il peso in avanti a sinistra
	.armFrontRight(arm_zero_pos-90).armFrontLeft(arm_zero_pos-90-weight_side_diff) \
	.delay(delay) \
	#muove a gamba posteriore destra per il passo
	.armBackRight(-60) \
	.delay(delay) \
	.shoulderBackRight(60) \
	.delay(delay) \
	#reimposta l'assetto gambe
	.armFrontRight(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) \
	.delay(delay) \
	#effettua la spinta per il passo
	.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos) \
	.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) \
	.shoulderFrontRight(-60).shoulderFrontLeft(0) \
	.shoulderBackLeft(-60).shoulderBackRight(0) \
	.delay(delay) \
	.send()
	
time.sleep(10)
