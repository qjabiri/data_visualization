from re import A
import cv2
import numpy as np
import time

def Isolate(img):
    # use selectROI to physically select area (temporarily, also will try black dot method)
    imgr = cv2.imread(img)
    r = cv2.selectROI("select the area", imgr)
    cropped_image = imgr[int(r[1]):int(r[1]+r[3]),int(r[0]):int(r[0]+r[2])]
    cropped = (r[1],r[3],r[0],r[2])
    h,w = imgr.shape[:2]
    return cropped_image,cropped, h, w
    
    
def Compare(img1, img2, h, w):
    #Use normalize function to compare images pixel by pixel
    errorL2 = cv2.norm(img1, img2, cv2.NORM_L2)
    #compute their percent similatiry
    similarity = 1- errorL2/(h*w)
    return similarity


def On(img1, img2):
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
    print(img1_img2)
    
    if img1_img2 <.9:
        print ("Turned On")
        return True
    else:
        return False 
    
def Capture(img_counter):
#captures one or multiple images with the press of the space bar
    cam = cv2.VideoCapture(1)
    cv2.namedWindow("test")
    dWidth = cam.get(cv2.CAP_PROP_FRAME_WIDTH) #get the width of frames of the video
    dHeight = cam.get(cv2.CAP_PROP_FRAME_HEIGHT) #get the height of frames of the video

    print ("Resolution of the video : " + str(dWidth) + " x " + str(dHeight))
    ret, frame = cam.read()
    while True:
        ret, frame = cam.read()
        if not ret:
            print("failed to grab frame")
            break
        cv2.imshow("test", frame)

        k = cv2.waitKey(1)
        if k%256 == 27:
            # ESC pressed
            print("Escape hit, closing...")
            break
        elif k%256 == 32:
            # SPACE pressed
            img = "opencv_frame_{}.png".format(img_counter)
            cv2.imwrite(img, frame)
            img_counter += 1
            return img, img_counter, dWidth, dHeight
    cam.release()
    cv2.destroyAllWindows()
    
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
    


if __name__=="__main__":
    img_counter = 1
    img1, img_counter, w, h = Capture(img_counter)
    img2, img_counter, w, h = Capture(img_counter)
    img1, cropp, h, w = Isolate(img1)
    img2 = cv2.imread(img2)[int(cropp[0]):int(cropp[0]+cropp[1]),int(cropp[2]):int(cropp[2]+cropp[3])]
    #sim = Compare(img1, img2, h, w)
    #print(sim)
    
    h1 = img1.shape[0]
    w1  = img1.shape[1]
    
    c1 = Compare(img1, img2, h, w)
    print(c1)
    c2 = Compare(img1, img2, h1, w1)
    print(c2)
    h = His(img1, img2)
    print(h)
    
    #ifon1 = False
    #while ifon1 == False:
    #    ifon = On(img1,img2)
    #    if ifon == False:
    #        again = input("not on, press  Y to take picture again or click enter to end")
    #        if again != 'Y':
    #            break 
    #        else:
    #            img1, img_counter = Capture(img_counter)
    #            img2, img_counter = Capture(img_counter)
    #    if ifon == True:
    #        print ("Display On")
    #        break
     
    