import cv2
import numpy as np

#Ohjelma seuraa punaista ympyraa ja piirtaa sen keskelle ympyran.


cap = cv2.VideoCapture(0)
kernel_dilation = np.ones((8,8),np.uint8)
kernel_erosion = np.ones((3,3),np.uint8)
min_area = 20*20
max_area = 200*200

while True:
    #Take each frame
    ret,frame = cap.read()

    #Smooth image
    #frame = cv2.GaussianBlur(frame,(5,5),0)

    #Convert BGR to HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    #Define range of red color in HSV
    lower_blue = np.array([170,50,50])
    upper_blue = np.array([190,255,255])

    #Threshold the HSV image to get only blue colors
    mask = cv2.inRange(hsv, lower_blue, upper_blue)

    #Remove noise
    #blur = cv2.GaussianBlur(mask,(5,5),0)
    mask = cv2.erode(mask,kernel_erosion,iterations = 1)
    mask = cv2.dilate(mask,kernel_dilation,iterations = 1)

    #Find contours
    contours, hierarchy = cv2.findContours(mask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    object_found = False
    
    #Use moments method to find our filtered object
    if hierarchy != None:
        num_objects = len(hierarchy)
        if num_objects < 50:
            for i in range(len(hierarchy)):
                area = cv2.contourArea(contours[i])
                if area > min_area and area < max_area and area > previous_area:
                    moments = cv2.moments(contours[i])
                    x = int(moments['m10'] / area)
                    y = int(moments['m01'] / area)
                    object_found = True
                previous_area = area

        if object_found == True:
            cv2.putText(frame,"Tracking Object",(100,100), cv2.FONT_HERSHEY_SIMPLEX, 2, 255)
            cv2.circle(frame,(x,y), 63, (0,0,255), -1)

        else:
            cv2.putText(frame,"Too much noise",(100,100), cv2.FONT_HERSHEY_SIMPLEX, 2, 255)



    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(frame,frame, mask= mask)
    
    #Show results
    cv2.imshow('camera',frame)
    cv2.imshow('mask',mask)
    cv2.imshow('res',res)
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

# When everything done, release the capture
kamera.release()
cv2.destroyAllWindows()
