#!/usr/bin/env python
import spa2
import cv2

WhiteCascade = cv2.CascadeClassifier(r"C:\Users\qaljabir\cascade_white.xml")

def get_moniter_status(his, sim):
    
    if sim >.9 and sim < .97 and his <.93 and his > .8:
        print ("Working Fine, video is playing")
        return 'W'
    #if sim > .98 and his > .98:
     #   print("not on2")
      #  return ('O')
    #if sim > .8 and his > .9:
     #   print ("White Noise")
      #  return 'F'
    if sim > .2 and his < .9:
        print("turning on and off")
        return ('T')
    print ("Working Fine, video is playing")
    return 'W'

def start_observation():
    x = input("how long in secods")
    x = int(x)
    x = x*60
    cam = cv2.VideoCapture(0)
    cv2.namedWindow("test", cv2.WINDOW_AUTOSIZE)
    ret , frame = cam.read()
    ret , frame = cam.read()
    ret , frame = cam.read()
    
    frame, cropp = spa2.Isolate(frame)
    framein = frame

    h = frame.shape[0]
    w  = frame.shape[1]
    count = 0
    count2 = 0
    on = False
    sim = 0
    his = 0


    for I in range(x):
        frame0 = frame
        ret , frame = cam.read()
        cv2.imshow("test", frame)
        frame = (frame)[int(cropp[0]):int(cropp[0]+cropp[1]),int(cropp[2]):int(cropp[2]+cropp[3])]
        if on != True:
            count +=1
            if count == x - 1:
                print("not on")
                return "not on"
            if spa2.On(frame0, frame, h ,w) == True and I > 10:
                on = True 
        elif I > 2:
            sim = (sim + spa2.Compare(frame0, frame, h, w)) / 2
            his = (his + spa2.His(frame0,frame)) / 2
            
        imgGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # Getting corners around the white noise
        white = WhiteCascade.detectMultiScale(imgGray, 1.3, 5)  # 1.3 = scale factor, 5 = minimum neighbor
        # drawing bounding box around white noise
        for (x, y, w, h) in white:
            img = cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 3)
            count2+=1
            if count2 > (I/2):
                print("White Noise / Static")
                return 'flahsing/white noise'
     
    print(his)
    print(sim)
    status = get_moniter_status(his,sim)
    print(status)
    return status
        
       
       # i = input ("Select G to get monitor status or E to end program")
       # if i == 'G':
       #     status = get_moniter_status(cam, h, w, cropp)
       #     print (status)
       # elif i == 'E':
       #     cam.release()
       #     cv2.destroyAllWindows()
       #     return "Observation Ended"
       # else:
        #    print ("error, select G to get moniter status or E to end program")
       # return True

if __name__=="__main__":  
    while True:
        i = input ("Select S to start observation or E to end program")
        if i == 'S':
            start_observation()
        elif i == 'E':
            break
        else:
            print("error, select S to start or E to end program")
        
        