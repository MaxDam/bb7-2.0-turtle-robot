import bb7
import cv2 as cv

distance = 0;
axis = ''

def distance_callback(payload_distance):
	distance = payload_distance
	print('distance: ' + distance)

def gyro_callback(payload_gyro):
	axis = payload_gyro
	print('axis: ' + axis)

bb7 = BB7(acquire_video=True, distance_callback=distance_callback, gyro_callback=gyro_callback)
bb7.head(40).neck(20).armFrontRight(30).send()
while true:
    frame = bb7.getFrame()
    frame = cv.resize(frame, (800, 420), cv.INTER_AREA)
	cv.imshow("frame", frame)
    if cv.waitKey(20) & 0xFF == ord("q"):
        break
bb7.destroy()