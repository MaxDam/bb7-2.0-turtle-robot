import bb7
import bb7_motion
import cv2 as cv
import time

bb7 = BB7(acquire_video=True, distance_callback=distance_callback, gyro_callback=gyro_callback)
bb7M = bb7Motion(bb7)

bb7.head(40).neck(20).armFrontRight(30).send()
time.sleep(0.5)
print("last distnace: " + bb7.lastDistance())
time.sleep(0.5)
bb7M.stepForward(2)
time.sleep(0.5)

while true:
    frame = bb7.getFrame()
    frame = cv.resize(frame, (800, 420), cv.INTER_AREA)
	cv.imshow("frame", frame)
    if cv.waitKey(20) & 0xFF == ord("q"):
        break
bb7.destroy()