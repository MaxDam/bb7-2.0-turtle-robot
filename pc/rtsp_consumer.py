import cv2 as cv
import os

os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = "rtsp_transport;udp"
cap = cv.VideoCapture("rtsp://freja.hiof.no:1935/rtplive/_definst_/hessdalen03.stream", cv.CAP_FFMPEG)
cap.set(cv.CAP_PROP_BUFFERSIZE, 2)

while cap.isOpened():
    ret, frame = cap.read()
    frame = cv.resize(frame, (800, 420), cv.INTER_AREA)
    cv.imshow("frame", frame)
    if cv.waitKey(20) & 0xFF == ord("q"):
        break
cap.release()
cv.destroyAllWindows()
