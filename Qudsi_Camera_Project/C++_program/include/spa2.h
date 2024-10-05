#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>

namespace spa2
{
    std::tuple<cv::Mat, cv::Rect2d> Isolate(cv::Mat img) {
        // use selectROI to physically select area (temporarily, also will try black dot method)    
        cv::Rect2d cropRect = cv::selectROI("select the area", img); 
        cv::Mat cropped_img = img(cropRect); 
        return {cropped_img, cropRect}; 
    }

    double Compare(cv::Mat img1, cv::Mat img2, double h, double w) {
        // Use normalize function to compare images pixel by pixel
        double errorL2 = cv::norm(img1, img2, cv::NORM_L2); 
        // compute their percent similatiry
        double similarity = 1 - errorL2 /(h*w);
        return similarity; 
    }

    double His(cv::Mat img1, cv::Mat img2) {
        // using the compare histogram to see if the moniter is on
        cv::Mat hsv_img1, hsv_img2; 
        cv::cvtColor(img1, hsv_img1, cv::COLOR_BGR2HSV);
        cv::cvtColor(img2, hsv_img2, cv::COLOR_BGR2HSV);
    
        int h_bins = 50; 
        int s_bins = 50; 
        int histSize[] = {h_bins, s_bins};  
        float h_ranges[] = {0, 180}; 
        float s_ranges[] = {0, 256}; 
        const float* ranges[] = { h_ranges, s_ranges };
        int channels[] = {0, 1};
        bool uniform = true, accumulate = false; 

        cv::MatND hist_img1, hist_img2; 
        cv::calcHist(&hsv_img1, 1, channels, cv::Mat(), hist_img1, 2, histSize, ranges, uniform, accumulate);
        cv::normalize(hist_img1, hist_img1, 0, 1, cv::NORM_MINMAX); 
        cv::calcHist(&hsv_img2, 1, channels, cv::Mat(), hist_img2, 2, histSize, ranges, uniform, accumulate);
        cv::normalize(hist_img2, hist_img2, 0, 1, cv::NORM_MINMAX); 

        int compare_method = cv::HISTCMP_CORREL; 
        double img1_img2 = cv::compareHist(hist_img1, hist_img2, compare_method);

        return img1_img2;
    }

    bool On(cv::Mat img1, cv::Mat img2, double h, double w) {
        cv::Mat hsv_img1, hsv_img2; 
        cv::cvtColor(img1, hsv_img1, cv::COLOR_BGR2HSV);
        cv::cvtColor(img2, hsv_img2, cv::COLOR_BGR2HSV);
        
        int h_bins = 50; 
        int s_bins = 50; 
        int histSize[] = {h_bins, s_bins};  
        float h_ranges[] = {0, 180}; 
        float s_ranges[] = {0, 256}; 
        const float* ranges[] = { h_ranges, s_ranges };
        int channels[] = {0, 1};
        bool uniform = true, accumulate = false; 

        cv::Mat hist_img1, hist_img2; 
        cv::calcHist(&hsv_img1, 1, channels, cv::Mat(), hist_img1, 2, histSize, ranges, uniform, accumulate);
        cv::normalize(hist_img1, hist_img1, 0, 1, cv::NORM_MINMAX); 
        cv::calcHist(&hsv_img2, 1, channels, cv::Mat(), hist_img2, 2, histSize, ranges, uniform, accumulate);
        cv::normalize(hist_img2, hist_img2, 0, 1, cv::NORM_MINMAX); 

        int compare_method = cv::HISTCMP_CORREL; 
        double img1_img2 = cv::compareHist(hist_img1, hist_img2, compare_method);

        if(img1_img2 < .5) {
            std::cout << "Turned On" << std::endl; 
            return true; 
        }
        else{
            std::cout << "not on" << std::endl; 
            return false; 
        }
    }

    std::tuple<std::string, int> Capture(cv::VideoCapture cam, int img_counter) {
        while (true)
        {
            cv::Mat frame; 
            bool ret = cam.read(frame); 
            if(!ret) {
                std::cout << "failed to grab frame" << std::endl; 
                break; 
            };
            cv::imshow("test", frame); 

            std::string img = "opencv_frame_" + std::to_string(img_counter) + ".png"; 
            cv::imwrite(img, frame);
            img_counter += 1;
            return {img, img_counter};  
        }
    }

    std::array<double, 2> Flashing(cv::Mat img1, cv::Mat img2, double h, double w) {
        double sim = Compare(img1, img2, h, w); 
        double his = His(img1, img2); 
        return {sim, his}; 
    }


} // namespace spa2

