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
bb7.zero(arm_zero_pos).send()
#time.sleep(delay)
#conta i passi in avanti
for i in range(2):
	#porta il peso indietro a sinistra
	bb7.armBackRight(arm_zero_pos-80).armBackLeft(arm_zero_pos-80-weight_side_diff).send()
	#time.sleep(delay)
	#muove a gamba anteriore destra per il passo
	bb7.armFrontRight(-60).send()
	#time.sleep(delay)
	bb7.shoulderFrontRight(60).send()
	#time.sleep(delay)
	#reimposta l'assetto gambe
	bb7.armFrontRight(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos).send()
	#time.sleep(delay)
	#porta il peso in avanti a destra
	bb7.armFrontRight(arm_zero_pos-90-weight_side_diff).armFrontLeft(arm_zero_pos-90).send()
	#time.sleep(delay)
	#muove a gamba posteriore sinistra per il passo
	bb7.armBackLeft(-60).send()
	#time.sleep(delay)
	bb7.shoulderBackLeft(60).send()
	#time.sleep(delay)
	#reimposta l'assetto gambe
	bb7.armBackLeft(arm_zero_pos).armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos).send()
	#time.sleep(delay)
	#effettua la spinta per il passo
	bb7.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos) \
		.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) \
		.shoulderFrontRight(0).shoulderFrontLeft(-40) \
		.shoulderBackLeft(0).shoulderBackRight(-40).send()
	#time.sleep(delay)

	
	#porta il peso indietro a destra
	bb7.armBackRight(arm_zero_pos-80-weight_side_diff).armBackLeft(arm_zero_pos-80).send()
	#time.sleep(delay)
	#muove a gamba anteriore sinistra per il passo
	bb7.armFrontLeft(-60).send()
	#time.sleep(delay)
	bb7.shoulderFrontLeft(60).send()
	#time.sleep(delay)
	#reimposta l'assetto gambe
	bb7.armFrontLeft(arm_zero_pos) \
		.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos).send()
	#time.sleep(delay)
	#porta il peso in avanti a sinistra
	bb7.armFrontRight(arm_zero_pos-90).armFrontLeft(arm_zero_pos-90-weight_side_diff).send()
	#time.sleep(delay)
	#muove a gamba posteriore destra per il passo
	bb7.armBackRight(-60).send()
	#time.sleep(delay)
	bb7.shoulderBackRight(60).send()
	#time.sleep(delay)
	#reimposta l'assetto gambe
	bb7.armFrontRight(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos).send()
	#time.sleep(delay)
	#effettua la spinta per il passo
	bb7.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos) \
		.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) \
		.shoulderFrontRight(-60).shoulderFrontLeft(0) \
		.shoulderBackLeft(-60).shoulderBackRight(0).send()
	#time.sleep(delay)
	
time.sleep(10)
