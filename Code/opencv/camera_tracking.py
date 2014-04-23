import cv2
import numpy as np


#Camera parameters: resolution is 640px*320px and fps = 60.
frame_time = 0.01667 
pixel_to_m = 0.0005
cam_calib_x = 320
cam_calib_y = 160

#Color ranges and color filtering values:
kernel_dilation = np.ones((10,10),np.uint8)
kernel_erosion = np.ones((5,5),np.uint8)
max_num_objects = 50
#Puck is yellow and round (r = 22.5mm).
puck_color_lower = np.array([30,75,100])
puck_color_upper = np.array([40,255,255])
puck_min_area = 20*20
puck_max_area = 200*200

#Gameplay parameters:
#Default defense position:
defense_y = 100

#If set to True captured video is showed.
#Set False for headless usage (galileo).
debug = True


#Filter video with given color range
def color_mask(frame, color_lower, color_upper):
    #Convert BGR to HSV
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    #Threshold the HSV image to get only blue colors
    frame = cv2.inRange(frame, color_lower, color_upper)
    #Remove noise
    frame = cv2.erode(frame, kernel_erosion, iterations = 1)
    frame = cv2.dilate(frame, kernel_dilation, iterations = 1)
    return frame

#Find object from filtered image:
def find_object(frame, min_area, max_area):
    object_found = False
    previous_area = 0
    x = 0
    y = 0

    #Find contours
    contours, hierarchy = cv2.findContours(frame, cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    #Use moments method to find our filtered object
    if hierarchy != None:
        num_objects = len(hierarchy)
        if num_objects < max_num_objects:
            for i in range(num_objects):
                area = cv2.contourArea(contours[i])
                if area > min_area and area < max_area and area > previous_area:
                    moments = cv2.moments(contours[i])
                    x = int(moments['m10'] / area)
                    y = int(moments['m01'] / area)
                    object_found = True
                previous_area = area
    return object_found, x, y

#Convert from camera reference system to robot reference system
def reference_system_conversion(x, y):
    puck_x = (y-cam_calib_x)*pixel_to_m
    puck_y = (-x+cam_calib_y)*pixel_to_m
    return puck_x, puck_y

#Find puck's speed and generate trajectory prediction:
def puck_trajectory(previous_puck_x, previous_puck_y, puck_x, puck_y):
    # puck speed components:
    vector_x = (puck_x-previous_puck_x)/frame_time
    vector_y = (puck_y-previous_puck_y)/frame_time
    puck_speed = pow((pow(vector_x,2) + pow(vector_y,2)),0.5)
    impact_x = 0
    #Speed is too high or low
    if vector_y > 0:
        # impact point
        if vector_x == 0:
            impact_x = puck_x
        else:
            k = vector_y/vector_x
            b = puck_y - k * puck_x
            impact_x = (defense_y-b)/k
    return impact_x

def main():
    #Open the camera
    cap = cv2.VideoCapture(0)

    previous_puck_x = 0
    previous_puck_y = 0

    while True:
        #Take each frame
        ret,frame = cap.read()
        
        mask = color_mask(frame, puck_color_lower, puck_color_upper)
        object_found, x, y = find_object(mask, puck_min_area, puck_max_area)
        if object_found == True:
            puck_x, puck_y = reference_system_conversion(x, y)
            impact_x = puck_trajectory(previous_puck_x, previous_puck_y, puck_x, puck_y)
            previous_puck_x = puck_x
            previous_puck_y = puck_y

        #This shows captured video with some information on monitor,
        #used for debugging.
        if debug == True:
            if object_found == True:
                cv2.putText(frame,"Tracking Object",(100,100), cv2.FONT_HERSHEY_SIMPLEX, 2, 255)
                cv2.circle(frame,(x, y), 63, (0,0,255), -1)
                cv2.line(frame,(x, y),(int(impact_x), defense_y),255)
            else:
                cv2.putText(frame,"Too much noise",(100,100), cv2.FONT_HERSHEY_SIMPLEX, 2, 255)
            cv2.imshow('camera',frame)
            cv2.imshow('mask',mask)
            k = cv2.waitKey(5) & 0xFF
            if k == 27:
                break
            

main()
