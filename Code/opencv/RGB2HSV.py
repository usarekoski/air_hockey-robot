# Requires python2, OpenCV and numpy.

import numpy as np
import cv2

print("Convert RGB values to HSV.")
while True:
    red = int(input("Red\n"))
    green = int(input("Green\n"))
    blue = int(input("Blue\n"))

    color = np.uint8([[[red,green,blue]]])
    hsv_color = cv2.cvtColor(color,cv2.COLOR_RGB2HSV)
    print hsv_color
