import cv2
import glob
import sys
import os

local_path = os.path.dirname(os.path.realpath(__file__))
stop_sign_cascade = cv2.CascadeClassifier(os.path.join(local_path, 'resources/haar/stop_sign.xml'))
traffic_lights_cascade = cv2.CascadeClassifier(os.path.join(local_path, 'resources/haar/traffic_light.xml'))

def stop_sign_detection(img_gray, scaleFactor=1.20, minNeighbors=3, minSize=(90, 90)):
    stop_signs = stop_sign_cascade.detectMultiScale(
        img_gray, 
        scaleFactor=scaleFactor, 
        minNeighbors=minNeighbors,
        minSize=minSize, 
        # flags=cv2.cv.CV_HAAR_SCALE_IMAGE
        )
    return stop_signs

def traffic_light_detection(img_gray): 
    traffic_lights = traffic_lights_cascade.detectMultiScale(
        img_gray, 
        scaleFactor=1.05, 
        minNeighbors=2, 
        minSize=(30, 30),
        flags=cv2.cv.CV_HAAR_SCALE_IMAGE
        )
    return traffic_lights

def lines_detection(img_gray):
    img_edges = cv2.Canny(img_gray, 50, 150, apertureSize = 3)
    minLineLength = 100
    maxLineGap = 10
    lines = cv2.HoughLinesP(img_edges, 1, 3.1415/180, 100, minLineLength, maxLineGap)
    return lines

def detections(img, detect=['stop', 'light', 'lines']): 
    # Preprocessing
    # img_ratio = img.shape[1] / float(img.shape[0])
    # img = cv2.resize(img, (int(480*img_ratio), 480)) # resize every photo
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    detected = {}

    if 'lines' in detect:
        detected['lines'] = lines_detection(img_gray)

    if 'stop' in detect:
        detected['stop'] = stop_sign_detection(img_gray)

    if 'light' in detect:
        detected['light'] = traffic_light_detection(img_gray)
    
    return detected

def draw_pattern(img, pattern, style='line', color=(0, 0, 0)):
    if style == 'line' and len(pattern) != 0:
        for (x1,y1,x2,y2) in pattern[0]:        
            cv2.line(img, (x1,y1), (x2,y2), color, 2)
    else:
        for (x,y,w,h) in pattern:
            cv2.rectangle(img, (x,y), (x+w,y+h), color, 2)

    return img 

def display(img, detect=['stop', 'light', 'lines']):
    img_ratio = img.shape[1] / float(img.shape[0])
    img = cv2.resize(img, (int(480*img_ratio), 480)) # resize every photo

    detected = detections(img, detect)
    print(detected)
    img = draw_pattern(img, detected.get('stop', []), style='roi', color=(255, 0, 0))
    img = draw_pattern(img, detected.get('light', []), style='roi', color=(0, 0, 255))
    img = draw_pattern(img, detected.get('lines', []))

    cv2.imshow('img', img)


if __name__ == '__main__':
    paths_glob = './resources/traffic_signs/*'
    if len(sys.argv) > 1:
        paths_glob = sys.argv[1]

    paths = glob.glob(paths_glob)
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
            #img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            #img = cv2.Canny(img_gray, 0, 200) # apertureSize = 3)
            display(img)
            cv2.waitKey(0)
    cv2.destroyAllWindows()