
#include <Windows.h>
#include<opencv2/opencv.hpp>//OpenCV header to use VideoCapture class//
#include<iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
//Our Current Code is Detecting if Images are still or not
// Noise Checking in Images
// Objects Detection in Images
bool CheckIfImageHasNoise(Mat myImage,Mat myImage2)
{

	double NoiseRatio=PSNR(myImage, myImage2);
	int R = myImage2.at<cv::Vec3b>(10, 29)[0];
	int G = myImage2.at<cv::Vec3b>(10, 29)[1];
	int B = myImage2.at<cv::Vec3b>(10, 29)[2];
	if ((R==G) && (G==B))
	{
		return true;
	}
	else
	{
		return false;
	}

}

Point2i center(vector<Point2i> contour)
{
	Moments m = moments(contour);

	return Point2i(m.m10 / m.m00, m.m01 / m.m00);
}


vector<Point2i> getCenters(vector<vector<Point>> contours)
{
	
	vector<Point2i> result(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		result[i] = center(contours[i]);
	}

	return result;
}



bool ChecIfImagesAreSame(const Mat & a, const Mat & b)
{
	if ((a.rows != b.rows) || (a.cols != b.cols))
		return false;
	Scalar s = sum(a - b);
	return (s[0] == 0) && (s[1] == 0) && (s[2] == 0);
}
bool CheckIfImageHasblack(const Mat & a)
{

	//	// Now we are detecting Contours in the Image this will detect all No of Objects so that After that we can Create another Algo to Check if there is any COmputer screen in the given Picture.
	Mat OutputImg;
	cv::inRange(a, cv::Scalar(0, 0, 0, 0), cv::Scalar(180, 255, 30, 0), OutputImg);
	imwrite("BlackImage.jpeg", OutputImg);
	
	erode(OutputImg, OutputImg, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(OutputImg, OutputImg, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	
	dilate(OutputImg, OutputImg, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(OutputImg, OutputImg, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	imshow("Thresholded Image", OutputImg); 
	
	imshow("Original", a);
	imwrite("Thresholded.jpeg", OutputImg);
	imwrite("Original.jpeg", a);
	std::vector<std::vector<cv::Point> > contours;
	findContours(OutputImg, contours, RETR_LIST, CHAIN_APPROX_NONE);

	cv::Mat contourImage(a.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Scalar colors[3];
	colors[0] = cv::Scalar(255, 0, 0);
	colors[1] = cv::Scalar(0, 255, 0);
	colors[2] = cv::Scalar(0, 0, 255);
	for (size_t idx = 0; idx < contours.size(); idx++) {
		cv::drawContours(contourImage, contours, idx, colors[idx % 3]);
		imwrite("idx"+to_string(idx)+".jpeg", contourImage);
	}
	
	imwrite("contourImage.jpeg", contourImage);

	return 0;
}


int main() {
	Mat webCamImage= cv::Mat(2, 2, CV_8UC3, Scalar(0, 0, 255));
	VideoCapture cap(0);
	if (!cap.isOpened()) { 

		cout << "No video stream detected" << endl;
		system("pause");
		return-1;
	}
	int count = 0;
	system("cls");
	cout << "Press Any Key to Start Observation" << endl;
	cin.get();
	while (true) {
		cap >> webCamImage;
		if (webCamImage.empty()) {

			cout << "Monitor Screen is OFF..." << endl;
			break;
		}
		//imshow("Current Image", webCamImage);
		char Waiting = (char)waitKey(25);
		if (Waiting == 27) {
			break;
		}
		
		char b = (char)waitKey(25);
		if (b == 27) { 
			break;
		}
		if (count > 1)
		{
			break;
		}
		string ImageName = "WebCamImage" + to_string(count)+".jpeg";
		imwrite(ImageName, webCamImage);
		count++;
	}
	cap.release();
	system("cls");
	//*** Since we are running COunt to 2 places so We know will Check PSNR of these saved Images
	
	Mat ReadImageOne = imread("WebCamImage0.jpeg" , 1);
	Mat ReadImageTwo = imread("WebCamImage1.jpeg", 1);
	cout << "Press Any KeyWord to Start getting Status.." << endl;
	cin.get();
	if (CheckIfImageHasNoise(ReadImageOne, ReadImageTwo))
	{
		//namedWindow("Image Detected", WINDOW_AUTOSIZE);
		//imshow("Current Image", webCamImage);   
		//waitKey(25);
		cout << "Image has White Noise in this" << endl;
		cin.get();
	}
	if (ChecIfImagesAreSame(ReadImageOne, ReadImageTwo))
	{
		cout << "Images are Same It Means Video is in Still Position" << endl;
		cin.get();
	}
	if (CheckIfImageHasblack(ReadImageOne))
	{
		cout << "Image Have Black Screen so May be Computer Screen is Turned Off" << endl;
		cin.get();
	}
	

	

	
	return 0;
}
