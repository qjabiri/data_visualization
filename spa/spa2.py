from doctest import TestResults
from re import A
import cv2
import numpy as np
import time

def Isolate(img):
    # use selectROI to physically select area (temporarily, also will try black dot method)
    r = cv2.selectROI("select the area", img)
    cropped_image = img[int(r[1]):int(r[1]+r[3]),int(r[0]):int(r[0]+r[2])]
    cropped = (r[1],r[3],r[0],r[2])
    h,w = img.shape[:2]
    return cropped_image,cropped
    
     
def Compare(img1, img2, h, w):
    #Use normalize function to compare images pixel by pixel
    errorL2 = cv2.norm(img1, img2, cv2.NORM_L2)
    #compute their percent similatiry
    similarity = 1- errorL2/(h*w)
    return similarity

def His(img1, img2):
      #using the compare histogram to see if the moniter is on

    hsv_img1 = cv2.cvtColor(img1, cv2.COLOR_BGR2HSV) 
    hsv_img2 = cv2.cvtColor(img2, cv2.COLOR_BGR2HSV)
    h_bins = 50  
    s_bins = 50 
    histSize = [h_bins, s_bins]
    h_ranges = [0, 180]
    s_ranges = [0, 256]
    ranges = h_ranges + s_ranges
    channels = [0, 1]
    
    hist_img1 = cv2.calcHist([hsv_img1], channels, None, histSize, ranges, accumulate=False)
    cv2.normalize(hist_img1, hist_img1, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX)
    hist_img2 = cv2.calcHist([hsv_img2], channels, None, histSize, ranges, accumulate=False)  
    cv2.normalize(hist_img2, hist_img2, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX)
    

    compare_method = cv2.HISTCMP_CORREL

    img1_img2 = cv2.compareHist(hist_img1, hist_img2, compare_method)
    return img1_img2

def On(img1, img2, h, w):
    #using the compare histogram to see if the moniter is on

    hsv_img1 = cv2.cvtColor(img1, cv2.COLOR_BGR2HSV)
    hsv_img2 = cv2.cvtColor(img2, cv2.COLOR_BGR2HSV)
    h_bins = 50
    s_bins = 50
    histSize = [h_bins, s_bins]
    h_ranges = [0, 180]
    s_ranges = [0, 256]
    ranges = h_ranges + s_ranges
    channels = [0, 1]
    
    hist_img1 = cv2.calcHist([hsv_img1], channels, None, histSize, ranges, accumulate=False)
    cv2.normalize(hist_img1, hist_img1, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX)
    hist_img2 = cv2.calcHist([hsv_img2], channels, None, histSize, ranges, accumulate=False)
    cv2.normalize(hist_img2, hist_img2, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX)
    

    compare_method = cv2.HISTCMP_CORREL

    img1_img2 = cv2.compareHist(hist_img1, hist_img2, compare_method)
    sim = Compare(img1, img2, h, w)
    
    if img1_img2 <.98 or sim < .97:
        return True
    else:
        return False
    
def Capture(cam, img_counter):
#captures one or multiple images with the press of th b e space bar
    #cam = cv2.VideoCapture(1)
    #cv2.namedWindow("test")
    #dWidth = cam.get(cv2.CAP_PROP_FRAME_WIDTH) #get the width of frames of the video
    #dHeight = cam.get(cv2.CAP_PROP_FRAME_HEIGHT) #get the height of frames of the video

    #print ("Resolution of the video : " + str(dWidth) + " x " + str(dHeight))
    while True:
        ret, frame = cam.read()
        if not ret:
            print("failed to grab frame")
            break
        cv2.imshow("test", frame)

       
        img = "opencv_frame_{}.png".format(img_counter)
        cv2.imwrite(img, frame)
        img_counter += 1
        return img, img_counter, #dWidth, dHeight
    cam.release()
    cv2.destroyAllWindows()
    
def Flashing(img1, img2, h, w):
    sim = Compare(img1, img2, h, w)
    his = His(img1,img2)
    return sim, his
        
