# OpenCV program to detect face in real time
# import libraries of python OpenCV
# where its functionality resides
import cv2
import requests
import time


nodemcu_address = "http://192.168.148.201"
led_on=False

# load the required trained XML classifiers
# https://github.com/Itseez/opencv/blob/master/
# data/haarcascades/haarcascade_frontalface_default.xml
# Trained XML classifiers describes some features of some
# object we want to detect a cascade function is trained
# from a lot of positive(faces) and negative(non-faces)
# images.
face_cascade = cv2.CascadeClassifier(r'C:\Users\Deby Bright G\Desktop\IIT\haarcascade_frontalface_default.xml')



# capture frames from a camera
cap = cv2.VideoCapture(0) #0 initiates the laptop camera. for Raspberry pi web cam, value will be -1 as it does not have inbuilt camera

# loop runs if capturing has been initialized.
while 1:

    # reads frames from a camera
    ret, img = cap.read()

    # convert to gray scale of each frames
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)#cv2

    # Detects faces of different sizes in the input image
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    for (x,y,w,h) in faces:
        # To draw a rectangle in a face
        cv2.rectangle(img,(x,y),(x+w,y+h),(255,255,0),2)#in the rectangle, if image is found, start point, stop point and colour RGB point

    if len(faces)>0 and not led_on:
        requests.get(f'{nodemcu_address}//LED1=ON')
        print("/LED1=ON")
        led_on = True
    elif len(faces)==0 and led_on:
        requests.get(f'{nodemcu_address}//LED1=OFF')
        print("/LED1=OFF")
        led_on=False

       

    # Display an image in a window
    cv2.imshow('img',img)

    # Wait for Esc key to stop
    k = cv2.waitKey(30) & 0xff #waitkey(30) means it has to wait to make the frames into video again. 0xff means esc. combining the function and 0xff, we will get the decimal number.
    if k == 27:#you are supposed to get k=27 if u press on esc. 
        break

# Close the window
cap.release()

# De-allocate any associated memory usage
cv2.destroyAllWindows()#closes the window completely
