import numpy as np # only for np.pi
import cv2
import glob
import sys

stop_sign_cascade = cv2.CascadeClassifier('./ressources/haar/stop_sign.xml')
traffic_lights_cascade = cv2.CascadeClassifier('./ressources/haar/traffic_light.xml')

def stop_sign_detection(img_gray):
    stop_signs = stop_sign_cascade.detectMultiScale(
        img_gray, 
        scaleFactor=1.05, 
        minNeighbors=3, 
        # minSize=(30, 30), 
        # flags=cv2.cv.CV_HAAR_SCALE_IMAGE
        )

    return stop_signs

def traffic_light_detection(img_gray):
    traffic_lights = traffic_lights_cascade.detectMultiScale(
        img_gray, 
        scaleFactor=1.05, 
        minNeighbors=1, 
        # minSize=(30, 30),
        flags=cv2.cv.CV_HAAR_SCALE_IMAGE
        )
    return traffic_lights

def lines_detection(img_gray):
    img_edges = cv2.Canny(img_gray, 50, 150, apertureSize = 3)
    minLineLength = 100
    maxLineGap = 10
    lines = cv2.HoughLinesP(img_edges, 1, np.pi/180, 100, minLineLength, maxLineGap)
    return lines

def display(img, detect=['stop', 'light', 'line']):
    img_ratio = img.shape[1] / float(img.shape[0])
    img = cv2.resize(img, (int(480*img_ratio), 480)) # resize every photo
    img_gray = img # cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    stop_signs = stop_sign_detection(img_gray)  if 'stop' in detect else []

    for (x,y,w,h) in stop_signs:
        cv2.rectangle(img, (x,y), (x+w,y+h), (255,0,0), 2)
        roi_gray = img_gray[y:y+h, x:x+w]
        roi_color = img[y:y+h, x:x+w]

    traffic_lights = traffic_light_detection(img_gray)

    for (x,y,w,h) in traffic_lights:
        cv2.rectangle(img, (x,y), (x+w,y+h), (0,255,0), 2)
        roi_gray = img_gray[y:y+h, x:x+w]
        roi_color = img[y:y+h, x:x+w]

    lines = lines_detection(img_gray)
    if lines is not None:
        for x1,y1,x2,y2 in lines[0]:
            cv2.line(img, (x1,y1), (x2,y2), (255, 0, 0), 2)

    cv2.imshow('img', img)
    

if __name__ == '__main__':
    paths_glob = './resources/traffic_signs/*'
    if len(sys.argv) > 1:
        paths_glob = sys.argv[1]

    print(paths_glob)
    paths = glob.glob(paths_glob)
    print(paths)
    for path in paths:
        if 'mp4' in path: 
            cam = cv2.VideoCapture(path)            
            while True:
                ret, img = cam.read()                      
                if (type(img) == type(None)):
                    break

                display(img)
                
                if (0xFF & cv2.waitKey(5) == 27) or img.size == 0:
                    break
        else:
            img = cv2.imread(path)
            img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            img = cv2.Canny(img_gray, 0, 200) # apertureSize = 3)
            display(img)
            cv2.waitKey(0)
    cv2.destroyAllWindows()
