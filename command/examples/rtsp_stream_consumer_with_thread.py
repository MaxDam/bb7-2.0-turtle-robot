from threading import Thread
import cv2 as cv
import numpy as np
import time
import os

RTSP_STREAM = "rtsp://freja.hiof.no:1935/rtplive/_definst_/hessdalen03.stream"

class ThreadedCamera(object):
    def __init__(self, src=0):
        
        os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = "rtsp_transport;udp"
        self.capture = cv.VideoCapture(src, cv.CAP_FFMPEG)
        self.capture.set(cv.CAP_PROP_BUFFERSIZE, 2)

        if not self.capture.isOpened():
            print("RTSP stream not found")
            exit(-1)

        # FPS = 1/X
        # X = desired FPS
        self.FPS = 1 / 30
        self.FPS_MS = int(self.FPS * 1000)

        # Start frame retrieval thread
        self.thread = Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()

    def update(self):
        while True:
            if self.capture.isOpened():
                (self.status, self.frame) = self.capture.read()
            time.sleep(self.FPS)

    def show_frame(self):
        if self.status:
            frm = cv.resize(self.frame, (800, 420), cv.INTER_AREA)
            cv.imshow("frame", frm)
        cv.waitKey(self.FPS_MS)

    def end(self):
        self.capture.release()
        cv.destroyAllWindows()


if __name__ == "__main__":
    tcamera = ThreadedCamera(RTSP_STREAM)

    loop = True
    while loop:
        try:
            tcamera.show_frame()
            if cv.waitKey(1) == ord("q"):
                print("break")
                loop = False
                break;
        except AttributeError:
            pass

    tcamera.end()
