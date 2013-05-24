////// Example showing how to read and write images
////#include <opencv2/opencv.hpp>
////#include <opencv2/highgui/highgui.hpp>
////#include <opencv/cvaux.hpp>
////
////int main(int argc, char** argv)
////{
////  IplImage * pInpImg = 0;
////  
////  // Load an image from file - change this based on your image name
////  pInpImg = cvLoadImage("cell1.jpg", CV_LOAD_IMAGE_UNCHANGED);
////  if(!pInpImg)
////  {
////    fprintf(stderr, "failed to load input image\n");
////    return -1;
////  }
////  
////  // Write the image to a file with a different name,
////  // using a different image format -- .png instead of .jpg
////  if( !cvSaveImage("my_image_copy.png", pInpImg) )
////  {
////    fprintf(stderr, "failed to write image file\n");
////  }
////  
////  // Remember to free image memory after using it!
////  cvReleaseImage(&pInpImg);
////  
////  return 0;
////}
///*
///////////////////////////////////////////////////////
//// A Simple Camera Capture Framework
//// This program will connect to a camera then
//// show the frames in a window
///////////////////////////////////////////////////////
//#include <stdio.h>
//#include <cv.h>
//#include <highgui.h>
//#include <time.h>
//
//int main()
//{
//  IplImage *frame = NULL; //Preparing frame cvPointer
//  int key;
//  time_t start, end;
//  double fps, seconds;
//  int frames = 0;
//  
//  //Allocates and initializes cvCapture structure
//  // for reading a video stream from the camera.
//  CvCapture *input_camera = cvCaptureFromCAM(-1);
//  
//  //Grabs and returns a frame from camera
//  frame = cvQueryFrame(input_camera);
//  
//  //Creates window for displaying the frames
//  //Flag is reset (0) --> change window size
//  // manually
//  cvNamedWindow("Capturing Image ...", 0);
//  
//  //Change to the appropriate size. In GTK, the
//  // inappropriate size will return a segmentation
//  // fault. I don't know why ...
//  //Gets the appropriate size using cvGetCaptureProperty
//  // with CV_CAP_PROP_FRAME_HEIGHT and CV_CAP_PROP_FRAME_WIDTH
//  // as property_id
//  cvResizeWindow("Capturing Image ...",
//                 (int) cvGetCaptureProperty(input_camera, CV_CAP_PROP_FRAME_HEIGHT),
//                 (int) cvGetCaptureProperty(input_camera, CV_CAP_PROP_FRAME_WIDTH));
//  
//  time(&start);
//  
//  while(frame != NULL)
//  {
//    
//    //Shows a frame
//    
////    char frameHeader[30];
////    sprintf(frameHeader, "Image stream at %.2f FPS", fps);
//    
//    cvShowImage("Capturing image...", frame);
//    
//    //Checks if ESC is pressed and gives a delay
//    // so that the frame can be displayed properly
//    key = cvWaitKey(10);
//    if(key == 27)
//      break;
//    //Grabs and returns the next frame
//    frame = cvQueryFrame(input_camera);
//    
//    time(&end);
//    frames++;
//    seconds = difftime(end, start);
//    fps = frames / seconds;
//    printf("%.2f FPS\n", fps);
//  }
//  
//  //Release cvCapture structure
//  cvReleaseCapture(&input_camera);
//  
//  //Destroy the window 
//  cvDestroyWindow("Capturing Image ...");
//  
//  return 0;
//}*/
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////#include "stdafx.h"
///*
//#include <cv.h>
//#include <highgui.h>
//
////This function threshold the HSV image and create a binary image
//IplImage* GetThresholdedImage(IplImage* imgHSV){
//  IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
//  cvInRangeS(imgHSV, cvScalar(150,100,0), cvScalar(180,256,256), imgThresh);
//  return imgThresh;
//}
//
//int main(){
//  CvCapture* capture =0;
//  
//  capture = cvCaptureFromCAM(-1);
//  if(!capture){
//    printf("Capture failure\n");
//    return -1;
//  }
//  
//  IplImage* frame=0;
//  
//  cvNamedWindow("Video");
//  cvNamedWindow("Ball");
//  
//  
//  //iterate through each frames of the video
//  while(true){
//    
//    frame = cvQueryFrame(capture);
//    if(!frame) break;
//    
//    frame=cvCloneImage(frame);
//    cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel
//    
//    IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
//    cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color forcv::Mat from BGR to HSV
//    IplImage* imgThresh = GetThresholdedImage(imgHSV);
//    
//    cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
//    
//    cvShowImage("Ball", imgThresh);
//    cvShowImage("Video", frame);
//    
//    //Clean up used images
//    cvReleaseImage(&imgHSV);
//    cvReleaseImage(&imgThresh);
//    cvReleaseImage(&frame);
//    
//    //Wait 50mS
//    int c = cvWaitKey(10);
//    //If 'ESC' is pressed, break the loop
//    if((char)c==27 ) break;
//  }
//  
//  cvDestroyAllWindows() ;
//  cvReleaseCapture(&capture);
//  
//  return 0;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//
////objectTrackingTutorial.cpp
//
////Written by  Kyle Hounslow 2013
//
////Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
////, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
////and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
////The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
////THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
////FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
////LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
////IN THE SOFTWARE.
//
//#include <sstream>
//#include <string>
//#include <iostream>
//#include <cv.h>
//#include <highgui.h>
//
////using namespace cv;
////initial min and max HSV filter values.
////these will be changed using trackbars
//int H_MIN = 0;
//int H_MAX = 256;
//int S_MIN = 0;
//int S_MAX = 256;
//int V_MIN = 0;
//int V_MAX = 256;
////default capture width and height
//const int FRAME_WIDTH = 640;
//const int FRAME_HEIGHT = 480;
////max number of objects to be detected in frame
//const int MAX_NUM_OBJECTS=50;
////minimum and maximum object area
//const int MIN_OBJECT_AREA = 20*20;
//const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
////names that will appear at the top of each window
//const cv::string windowName = "Original Image";
//const cv::string windowName1 = "HSV Image";
//const cv::string windowName2 = "Thresholded Image";
//const cv::string windowName3 = "After Morphological Operations";
//const cv::string trackbarWindowName = "Trackbars";
//void on_trackbar( int, void* )
//{//This function gets called whenever a
//	// trackbar position is changed
//  
//  
//  
//  
//  
//}
//cv::string intToString(int number){
//  
//  
//	std::stringstream ss;
//	ss << number;
//	return ss.str();
//}
//void createTrackbars(){
//	//create window for trackbars
//  
//  
//  cvNamedWindow(trackbarWindowName,0);
//	//create memory to store trackbar name on window
//	char TrackbarName[50];
//	sprintf( TrackbarName, "H_MIN", H_MIN);
//	sprintf( TrackbarName, "H_MAX", H_MAX);
//	sprintf( TrackbarName, "S_MIN", S_MIN);
//	sprintf( TrackbarName, "S_MAX", S_MAX);
//	sprintf( TrackbarName, "V_MIN", V_MIN);
//	sprintf( TrackbarName, "V_MAX", V_MAX);
//	//create trackbars and insert them into window
//	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
//	//the max value the trackbar can move (eg. H_HIGH),
//	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
//	//                                  ---->    ---->     ---->
//  cv::createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
//  cv::createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
//  cv::createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
//  cv::createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
//  cv::createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
//  cv::createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
//  
//  
//}
//void drawObject(int x, int y,cv::Mat &frame){
//  
//	//use some of the openCV drawing functions to draw crosshairs
//	//on your tracked image!
//	cv::circle(frame,cvPoint(x,y),20,cvScalar(0,255,0),2);
//	cv::line(frame,cvPoint(x,y-5),cvPoint(x,y-25),cvScalar(0,255,0),2);
//	cv::line(frame,cvPoint(x,y+5),cvPoint(x,y+25),cvScalar(0,255,0),2);
//	cv::line(frame,cvPoint(x-5,y),cvPoint(x-25,y),cvScalar(0,255,0),2);
//	cv::line(frame,cvPoint(x+5,y),cvPoint(x+25,y),cvScalar(0,255,0),2);
//  
//	cv::putText(frame,intToString(x)+","+intToString(y),cvPoint(x,y+30),1,1,cvScalar(0,255,0),2);
//  
//}
//void morphOps(cv::Mat &thresh){
//  
//	//create structuring element that will be used to "dilate" and "erode" image.
//	//the element chosen here is a 3px by 3px rectangle
//  
//	cv::Mat erodeElement = getStructuringElement( cv::MORPH_RECT,cvSize(3,3));
//  //dilate with larger element so make sure object is nicely visible
//	cv::Mat dilateElement = getStructuringElement( cv::MORPH_RECT,cvSize(8,8));
//  
//	erode(thresh,thresh,erodeElement);
//	erode(thresh,thresh,erodeElement);
//  
//  
//	dilate(thresh,thresh,dilateElement);
//	dilate(thresh,thresh,dilateElement);
//	
//  
//  
//}
//void trackFilteredObject(int &x, int &y, cv::Mat threshold, cv::Mat &cameraFeed){
//  
//	cv::Mat temp;
//	threshold.copyTo(temp);
//	//these two vectors needed for output of findContours
//  cv::Vector< vector<cvPoint> > contours;
//	vector<Vec4i> hierarchy;
//	//find contours of filtered image using openCV findContours function
//	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
//	//use moments method to find our filtered object
//	double refArea = 0;
//	bool objectFound = false;
//	if (hierarchy.size() > 0) {
//		int numObjects = hierarchy.size();
//    //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
//    if(numObjects<MAX_NUM_OBJECTS){
//			for (int index = 0; index >= 0; index = hierarchy[index][0]) {
//        
//				cv::Moments moment = moments((cv::cv::Mat)contours[index]);
//				double area = moment.m00;
//        
//				//if the area is less than 20 px by 20px then it is probably just noise
//				//if the area is the same as the 3/2 of the image size, probably just a bad filter
//				//we only want the object with the largest area so we safe a reference area each
//				//iteration and compare it to the area in the next iteration.
//        if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
//					x = moment.m10/area;
//					y = moment.m01/area;
//					objectFound = true;
//          
//				}else objectFound = false;
//        
//        
//			}
//			//let user know you found an object
//			if(objectFound ==true){
//				putText(cameraFeed,"Tracking Object",cvPoint(0,50),2,1,cvScalar(0,255,0),2);
//				//draw object location on screen
//				drawObject(x,y,cameraFeed);}
//      
//		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",cvPoint(0,50),1,2,cvScalar(0,0,255),2);
//	}
//}
//int main(int argc, char* argv[])
//{
//	//some boolean variables for different functionality within this
//	//program
//  bool trackObjects = false;
//  bool useMorphOps = false;
//	//cv::Matrix to store each frame of the webcam feed
//	cv::Mat cameraFeed;
//	//cv::Matrix storage for HSV image
//	cv::Mat HSV;
//	//cv::Matrix storage for binary threshold image
//	cv::Mat threshold;
//	//x and y values for the location of the object
//	int x=0, y=0;
//	//create slider bars for HSV filtering
//	createTrackbars();
//	//video capture object to acquire webcam feed
//	VideoCapture capture;
//	//open capture object at location zero (default location for webcam)
//	capture.open(0);
//	//set height and width of capture frame
//	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
//	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
//	//start an infinite loop where webcam feed is copied to cameraFeed cv::Matrix
//	//all of our operations will be performed within this loop
//	while(1){
//		//store image to cv::Matrix
//		capture.read(cameraFeed);
//		//convert frame from BGR to HSV colorspace
//		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
//		//filter HSV image between values and store filtered image to
//		//threshold cv::Matrix
//		inRange(HSV,cvScalar(H_MIN,S_MIN,V_MIN),cvScalar(H_MAX,S_MAX,V_MAX),threshold);
//		//perform morphological operations on thresholded image to eliminate noise
//		//and emphasize the filtered object(s)
//		if(useMorphOps)
//      morphOps(threshold);
//		//pass in thresholded frame to our object tracking function
//		//this function will return the x and y coordinates of the
//		//filtered object
//		if(trackObjects)
//			trackFilteredObject(x,y,threshold,cameraFeed);
//    
//		//show frames
//		imshow(windowName2,threshold);
//		imshow(windowName,cameraFeed);
//		imshow(windowName1,HSV);
//		
//    
//		//delay 30ms so that screen can refresh.
//		//image will not appear without this waitKey() command
//		waitKey(30);
//	}
//  
//  
//  
//  
//  
//  
//	return 0;
//}
//
