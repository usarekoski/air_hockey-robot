import cv2
import numpy as np
import os

#Galileo = linux2
if os.uname()[1] == 'clanton':
    import serial
    debug = False
else:
    debug = True

#Camera parameters: resolution is 640px*320px and fps is 60.
frame_time = 0.01667 
cam_calib_x = 320
cam_calib_y = 480

#Table dimensions
side_distance = 0.01
puck_radius = 0.0225
side_offset = side_distance + puck_radius
table_width = 0.22
pixel_to_m = (table_width - 2*side_distance)/480

#Color ranges and color filtering values:
kernel_dilation = np.ones((10,10),np.uint8)
kernel_erosion = np.ones((5,5),np.uint8)
max_num_objects = 50
#Puck is yellow and round (r = 22.5mm).
puck_color_lower = np.array([40,100,150])
puck_color_upper = np.array([55,255,255])
puck_min_area = 20*20
puck_max_area = 200*200

#Gameplay parameters:
#Default defense position:
defense_y = 0.1
defense_x = 0.12 
treshold_speed_y = 0.1

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
                area = cv2.contourArea(contours[i].astype('int'))
                if area > min_area and area < max_area and area > previous_area:
                    moments = cv2.moments(contours[i].astype('int'))
                    x = int(moments['m10'] / area)
                    y = int(moments['m01'] / area)
                    object_found = True
                previous_area = area
    return object_found, x, y

#Convert from camera reference system to robot reference system
def reference_system_conversion(x, y, backwards):
    #pixels to metres
    if backwards == False:
        puck_x = (-y+cam_calib_y)*pixel_to_m
        puck_y = (x-cam_calib_x)*pixel_to_m
    #metres to pixels
    if backwards == True:
        puck_x = (y / pixel_to_m + cam_calib_x)
        puck_y = -x / pixel_to_m + cam_calib_y
        #Round to pixels
        puck_x, puck_y = int(puck_x), int(puck_y)
    return puck_x, puck_y

#Find puck's speed and generate trajectory prediction:
def puck_trajectory(previous_puck_x, previous_puck_y, puck_x, puck_y):
    bounce_x = 0
    bounce_y = 0
    # puck speed components:
    vector_x = (puck_x-previous_puck_x)/frame_time
    vector_y = (puck_y-previous_puck_y)/frame_time
    #puck_speed = pow((pow(vector_x,2) + pow(vector_y,2)),0.5)
    # if puck is stationary or absent return default defense x coordinate.
    impact_x = defense_x

    # Check speed
    if vector_y > treshold_speed_y:
        # impact point
        if vector_x == 0:
            impact_x = puck_x
        else:
            #puck has straight path: y=kx+b
            k = vector_y/vector_x
            b = puck_y - k * puck_x
            initial_impact_x = (defense_y-b)/k
            #Check if puck has a bounce with side wall
            if initial_impact_x < side_distance or initial_impact_x > (side_distance + table_width):
                #Left or right side wall
                #Right
                if initial_impact_x < side_distance:
                    bounce_x = side_distance
                else:
                    bounce_x = side_distance + table_width
                #New impact coordinate
                bounce_y = k*bounce_x + b
                bounce_b = bounce_y + k*bounce_x
                initial_impact_x = (defense_y - bounce_b)/-k
                #Second bounce
                if initial_impact_x > side_distance and initial_impact_x < (side_distance + table_width):
                    impact_x = initial_impact_x
            else:
                impact_x = initial_impact_x
    return impact_x, bounce_x, bounce_y


#Write coordinates to serial.
if not debug:
    def write_to_serial(x):
        x = int(x*1000) #metres to millimetres
        x = str(x)
        ttyS0 = serial.Serial('/dev/ttyS0', 9600)
        ttyS0.write(x)


def main():
    #Open the camera
    cap = cv2.VideoCapture(0)
    #Reset puck position
    previous_puck_x = 0
    previous_puck_y = 0
    #Defense position for drawing
    if debug:
        defense_x_pixels, defense_y_pixels = reference_system_conversion(defense_x, defense_y, True)
        _, side_distance_pixels  = reference_system_conversion(side_distance, 0, True)

    while True:
        #Take each frame
        ret,frame = cap.read()
        #Mask puck by color
        mask = color_mask(frame, puck_color_lower, puck_color_upper)
        #Find it's position
        object_found, puck_x_pixels, puck_y_pixels = find_object(mask, puck_min_area, puck_max_area)
        if object_found == True:
            #Convert pixels to metres
            puck_x, puck_y = reference_system_conversion(puck_x_pixels, puck_y_pixels, False)
            #Find defense position
            impact_x, bounce_x, bounce_y = puck_trajectory(previous_puck_x, previous_puck_y, puck_x, puck_y)
            #Update position
            previous_puck_x = puck_x
            previous_puck_y = puck_y

        #This shows captured video with some information on monitor,
        #used for debugging and calibrating.
        if debug:
            if object_found == True:
                cv2.putText(frame,"Tracking object",(50,100), cv2.FONT_HERSHEY_SIMPLEX, 1, 255)
                cv2.circle(frame,(puck_x_pixels,puck_y_pixels), 30, (0,0,255), -1)
                impact_x_pixels, impact_y_pixels = reference_system_conversion(impact_x,0,True)
                #If puck bounces, draw two lines
                if bounce_x != 0:
                    bounce_x_pixels, bounce_y_pixels = reference_system_conversion(bounce_x, bounce_y, True)
                    cv2.line(frame,(puck_x_pixels,puck_y_pixels),(bounce_x_pixels, bounce_y_pixels),255)
                    cv2.line(frame,(bounce_x_pixels, bounce_y_pixels),(defense_x_pixels, impact_y_pixels),255)
                #No bounce
                else:
                    cv2.line(frame,(puck_x_pixels,puck_y_pixels),(defense_x_pixels, impact_y_pixels),255)
                print(impact_x)
                print(impact_y_pixels)

            else:
                cv2.putText(frame,"Cannot find the object",(50,100), cv2.FONT_HERSHEY_SIMPLEX, 1, 255)

            cv2.line(frame,(defense_x_pixels,0),(defense_x_pixels,480),100,4)
            cv2.line(frame,(0,side_distance_pixels),(640,side_distance_pixels),(0,0,255),2)
            cv2.line(frame,(0,480-side_distance_pixels),(640,480-side_distance_pixels),(0,0,255),2)
            cv2.imshow('camera',frame)
            cv2.imshow('puck mask',mask)
            k = cv2.waitKey(5) & 0xFF
            if k == 27:
                break

        else:
            if object_found == True:
                print(impact_x)
                #Send defense position to another galileo
                write_to_file(impact_x, defense_y)
                write_to_serial(impact_x, defense_y)
            

main()
