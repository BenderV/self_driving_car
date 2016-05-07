"""
Record the video flux
https://www.raspberrypi.org/documentation/usage/camera/raspicam/raspivid.md
https://www.raspberrypi.org/documentation/usage/camera/python/README.md
"""
import picamera
from time import sleep

camera = picamera.PiCamera()

def record_video():
    camera.start_recording('records/test.h264')
    sleep(60*10) # 10 minutes o recording
    camera.stop_recording()

def take_photo():
    """TOFIX"""
    camera.resolution = (960, 720)
    for i, filename in enumerate(camera.capture_continuous('image{counter}.jpg')):
        print(filename)
        time.sleep(0.1)
