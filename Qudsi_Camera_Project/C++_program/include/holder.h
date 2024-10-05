#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void holder() {
    cv::VideoCapture cap(1); 
    cap.set(3, 640); 
    cap.set(4, 420);

    int count = 0; 
    int count2 = 0; 

    //  import cascade file for facial recognition
    cv::CascadeClassifier whiteCascade("/Users/qudsi/Desktop/OpenCV-Python-Series-master/src/cascades/data/cascade_white.xml");
    cv::CascadeClassifier blackCascade("/Users/qudsi/Desktop/OpenCV-Python-Series-master/src/cascades/data/cascade_black.xml");

    while (true)
    {
        cv::Mat img;
        cap.read(img); 
        cv::Mat imgGray;
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

        // Getting corners around the white noise
        std::vector<cv::Rect> whites;
        whiteCascade.detectMultiScale(imgGray, whites, 1.3, 5); // 1.3 = scale factor, 5 = minimum neighbor
        //  drawing bounding box around white noise
        for ( int i = 0; i < whites.size(); i++ )
        {
            cv::Rect whiteRect = whites[i]; 
            cv::rectangle(img, whiteRect, cv::Scalar(0, 255, 0), 3);
            count2++; 
            if(count2 > 5) {
                std::cout << "flashing/white noise" << std::endl; 
                return; 
            }
        }

        // std::vector<cv::Rect> blacks;
        // blackCascade.detectMultiScale(imgGray, blacks, 20, 5); // 20 = scale factor, 5 = minimum neighbor
        // //  drawing bounding box around white noise
        // for ( int i = 0; i < blacks.size(); i++ )
        // {
        //     cv::Rect blackRect = blacks[i]; 
        //     cv::rectangle(img, blackRect, cv::Scalar(255, 0, 0), 3);
        //     count++; 
        //     if(count > 5) {
        //         std::cout << "flashing/white noise" << std::endl; 
        //         return; 
        //     }
        // }
        cv::imshow("image", img); 
        if((cv::waitKey(10) & 0xFF) == 'q') {
            break; 
        }
    }
    cap.release(); 
    cv::destroyWindow("image"); 
}