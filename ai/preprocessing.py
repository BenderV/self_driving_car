import cv2
import numpy as np
import glob
import sys
import detection

def get_mask_color(img, color='red'):
    # img = cv2.medianBlur(img, 5)
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    # roi = img[int(img.shape[0]/2):img.shape[0], int(img.shape[1]/2):img.shape[1]] # roi

    if color == 'red':
        mask1 = cv2.inRange(hsv, np.array((0,100,100)), np.array((20,255,255))) # yellow
        mask2 = cv2.inRange(hsv, np.array((80,100,100)), np.array((255,255,255))) # yellow
        mask = cv2.add(mask1, mask2)
    elif color == 'blue':
        mask = cv2.inRange(hsv, np.array((150,100,100)), np.array((190,255,255))) # blue
    else:
        mask = cv2.inRange(hsv, np.array((20,100,100)), np.array((60,255,255))) # yellow
    
    kernel = np.ones((5, 5), np.uint8)
    
    #filtering for image
    mask = cv2.erode(mask, kernel, iterations=1)
    mask = cv2.dilate(mask, kernel, iterations=1)
    mask = cv2.dilate(mask, kernel, iterations=1)
    mask = cv2.erode(mask, kernel, iterations=1)

    return mask

def get_size_mask(image, y_start, y_stop):
    return image[int(image.shape[0]*y_start):int(image.shape[0]*y_stop), :] # roi

def get_mask_info(mask):
    try:
        mask = mask.copy()
        # contours, hierarchy = cv2.findContours(mask, 1, 2)
        M = cv2.moments(mask)
        cx = int(M['m10']/M['m00'])
        cy = int(M['m01']/M['m00'])
        surface_size = M['m00']
    except Exception as e:
        print(e)
        cx, cy = 0, 0
        surface_size = 0
    return cx, cy, surface_size

if __name__ == '__main__':
    images_path = glob.glob(sys.argv[1])
    for image_path in images_path:
        image = cv2.imread(image_path)
        image = cv2.resize(image, (960, 720))

        # roi = mask[int(mask.shape[1]/2):mask.shape[0], 0:mask.shape[1]] # roi
        #lines = detection.lines_detection(image)
        # detection.draw_pattern(image, lines, style='line', color=(0, 0, 255 ))

        mask = get_mask_color(image, color='yellow')
        mask_color = cv2.cvtColor(mask, cv2.cv.CV_GRAY2RGB)

        a = np.linspace(0.5, 1, 11)
        for i in range(10):
          roi = get_size_mask(mask, a[i], a[i+1])
          cx, cy, surface_size = get_mask_info(roi)
          cv2.circle(mask_color, (cx, int(image.shape[0]*a[i] + cy)), 10, (0, 0, 255), 3)

        img = cv2.add(image, mask_color)
        cv2.imshow('img', img)
        cv2.waitKey(0)

    cv2.destroyAllWindows()