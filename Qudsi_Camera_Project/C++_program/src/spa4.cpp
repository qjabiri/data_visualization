#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "spa2.h"


std::string get_moniter_status(double his, double sim) {
    if(sim > .9 && sim < .97 && his < .93 && his > .8) {
        std::cout << "Working Fine" << std::endl; 
        return "W"; 
    }
     if(sim > .2 && his < .9) {
        std::cout << "Turning on and off" << std::endl; 
        return "T"; 
    }
    std::cout << "Working Fine" << std::endl; 
    return "W";
}

std::string start_observation() {
    std::cout << "how long in seconds" << std::endl; 
    int x; 
    std::cin >> x; 
    x *= 60; 
    cv::VideoCapture cam = cv::VideoCapture(0); 
    cv::namedWindow("test"); 
    cv::Mat firstFrame; 
    cam.read(firstFrame); 
    cam.read(firstFrame); 
    cam.read(firstFrame); 

    auto [frame, cropRect] = spa2::Isolate(firstFrame); 
    bool on = false;
    double sim = 0;
    double his = 0;
    double count = 0;
    double count2 = 0;
    double h = frame.size[0], w = frame.size[1]; 
    cv::CascadeClassifier whiteCascade("C:/Users/qaljabir/Desktop/cascade_white.xml");

    for (size_t I = 0; I < x; I++)
    {
        cv::Mat frame0 = frame; 
        cam.read(frame); 
        cv::imshow("frame", frame); 
        frame = frame(cropRect); 
        if(!on) {
            count ++;
            if(count == (x - 1)) {
                return "not on"  
            }
            
            if (spa2::On(frame0, frame, h, w) && I > 10){
                on = true
            }
        }
        else if(I > 2) {
            sim = ((sim + spa2::Compare(frame0, frame, h, w))/2);
            his = ((his + spa2::His(frame0, frame))/2);  
        }
        cv::Mat frame;
        cv::Mat imgGray;
        cv::cvtColor(frame, imgGray, cv::COLOR_BGR2GRAY);
        std::vector<cv::Rect> whites;
        whiteCascade.detectMultiScale(imgGray, whites, 1.3, 5); // 1.3 = scale factor, 5 = minimum neighbor
        for ( int i = 0; i < whites.size(); i++ )
        {
            cv::Rect whiteRect = whites[i]; 
            cv::rectangle(frame, whiteRect, cv::Scalar(0, 255, 0), 3);
            count2 ++;
            if (count2>(I/2)){
                return "flashing/ wite noise"
            }
        }
    }

    std::string status = get_moniter_status(his, sim); 
    std::cout << status << std::endl; 
    return status; 
}


int main() {
    while(true) {
        std::cout << "Select S to start observation or E to end program" << std::endl; 
        char i; 
        std::cin >> i; 
        if(i == 'S') {
            start_observation(); 
        }
        else if(i == 'E') {
            break; 
        }
        else{
            std::cout << "error, select S to start or E to end program" << std::endl; 
        }
    }
}