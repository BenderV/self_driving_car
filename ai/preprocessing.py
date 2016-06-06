import cv2
import numpy as np
import glob
import sys

def track_line(img):
    img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    #HSV Threshold
    lowT =(0, 150, 60)
    highT = (179, 255, 255)

    #HSV filtering
    mask = cv2.inRange(img, lowT, highT)

    kernel = np.ones((5, 5), np.uint8)
    
    #filtering for image
    mask = cv2.erode(mask, kernel, iterations=1)
    mask = cv2.dilate(mask, kernel, iterations=1)
    mask = cv2.dilate(mask, kernel, iterations=1)
    mask = cv2.erode(mask, kernel, iterations=1)


    height, width = mask.shape
    contours, hierarchy = cv2.findContours(mask, 1, 2)
    cnt = contours[0]
    [vx,vy,x,y] = cv2.fitLine(cnt, cv2.cv.CV_DIST_L2, 0,  0.01, 0.01)
    rows, cols = mask.shape[:2]
    lefty = int((-x*vy/vx) + y)
    righty = int(((cols-x)*vy/vx)+y)
    cv2.line(mask, (cols-1, righty), (0, lefty), (255, 0, 0), 2, cv2.CV_AA)
    print((cols-1, righty), (0, lefty))
    cv2.imshow('mask new', mask)
    # return gradient

    # Bitwise-AND mask and original image
    # res = cv2.bitwise_and(frame, frame, mask=mask)

images_path = glob.glob(sys.argv[1])
for image_path in images_path:
    image = cv2.imread(image_path)
    image = cv2.resize(image, (960, 720))

    track_line(image)

    cv2.waitKey(0)
cv2.destroyAllWin