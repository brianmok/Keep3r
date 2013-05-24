//objectTrackingTutorial.cpp

//Original tracking code written by Kyle Hounslow 2013, from http://www.youtube.com/watch?v=bSeFrPrqZ2A

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.



#include <sstream>
#include <string>
#include <iostream>
#include <highgui.h>
#include <cv.h>
#include <time.h>
#include <sys/time.h>

#define ACCELERATION 0

using namespace cv;
//initial min and max HSV filter values.
//these will be changed using trackbars
//int H_MIN = 0;
//int H_MAX = 256;
//int S_MIN = 0;
//int S_MAX = 256;
//int V_MIN = 0;
//int V_MAX = 256;

int H_MIN = 2;
int H_MAX = 10;
int S_MIN = 121;
int S_MAX = 173;
int V_MIN = 135;
int V_MAX = 255;

//default capture width and height
const int FRAME_WIDTH = 320;
const int FRAME_HEIGHT = 640;

const float FRAME_WIDTH_M = 1.115;
const float FRAME_HEIGHT_M = 2.23;
const float TOTAL_LENGTH = 3.68;

float metersPerPixel = FRAME_WIDTH_M / FRAME_WIDTH;

//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

int x_prev = 0;
int y_prev = 0;
float vx_prev = 0;
float vy_prev = 0;
float ax_m = 0;
float ay_m = 0;

struct timeval start, prev_frame_time;
float dt = 0;
bool kickInProgress = false;

void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed
}

string intToString(int number)
{
  
	std::stringstream ss;
	ss << number;
	return ss.str();
}

string floatToString(float number)
{
  
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void createTrackbars()
{
	//create window for trackbars
  
  
  namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN (%d)", H_MIN);
	sprintf( TrackbarName, "H_MAX (%d)", H_MAX);
	sprintf( TrackbarName, "S_MIN (%d)", S_MIN);
	sprintf( TrackbarName, "S_MAX (%d)", S_MAX);
	sprintf( TrackbarName, "V_MIN (%d)", V_MIN);
	sprintf( TrackbarName, "V_MAX (%d)", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
  createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
  createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
  createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
  createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
  createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
  createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
  
  
}

void drawObject(int x, int y,Mat &frame)
{
  
	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!
	circle(frame,Point(x,y),20,Scalar(0,255,0),2);
	line(frame,Point(x,y-5),Point(x,y-25),Scalar(0,255,0),2);
	line(frame,Point(x,y+5),Point(x,y+25),Scalar(0,255,0),2);
	line(frame,Point(x-5,y),Point(x-25,y),Scalar(0,255,0),2);
	line(frame,Point(x+5,y),Point(x+25,y),Scalar(0,255,0),2);
  
  line(frame, Point(x,y), Point(x+vx_prev,y+vy_prev), Scalar(0,255,0), 2);
  
	putText(frame,intToString(x)+","+intToString(y) + " -- " + floatToString(vx_prev)+","+floatToString(vy_prev) + "--" + floatToString(dt),
          Point(x,y+30),1,1,Scalar(0,255,0),2);
  
}
void morphOps(Mat &thresh)
{
  
	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle
  
	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
  //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));
  
	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);
  
	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
  
}

bool trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed)
{
  
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0)
  {
		int numObjects = (int) hierarchy.size();
    //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
    if(numObjects<MAX_NUM_OBJECTS)
    {
			for (int index = 0; index >= 0; index = hierarchy[index][0])
      {
        
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
        
				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
        if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea)
        {
					x = moment.m10/area;
					y = moment.m01/area;
					objectFound = true;
          
				}
        else objectFound = false;
        
        
			}
			//let user know you found an object
			if(objectFound ==true)
      {
				putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
				//draw object location on screen
				drawObject(x,y,cameraFeed);
      }
      else
        putText(cameraFeed,"No objects of correct size",Point(0,50),2,1,Scalar(255,0,0),2);
      
		}
    else
      putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
  
  return objectFound;
}

int main(int argc, char* argv[])
{
	//some boolean variables for different functionality within this
	//program
  bool trackObjects = true;
  bool useMorphOps = true;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
	Mat threshold;
	//x and y values for the location of the object
	int x=0, y=0;
	//create slider bars for HSV filtering
	createTrackbars();
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
  
  gettimeofday(&start, NULL);
  prev_frame_time = start;
  int frame_num = 0;
  
	while(1){
    frame_num++;
		//store image to matrix
		capture.read(cameraFeed);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		//filter HSV image between values and store filtered image to
		//threshold matrix
		inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
		//perform morphological operations on thresholded image to eliminate noise
		//and emphasize the filtered object(s)
		if(useMorphOps)
      morphOps(threshold);
		
        
        
        //pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object
		
        if(trackObjects)
    {
      
			if (trackFilteredObject(x,y,threshold,cameraFeed))
      {
        
        struct timeval frame_time;
        gettimeofday(&frame_time, NULL);
        
        dt = (frame_time.tv_sec + .000001*frame_time.tv_usec) -
             (prev_frame_time.tv_sec + .000001*prev_frame_time.tv_usec);
        
        prev_frame_time = frame_time;
        
        float filter = kickInProgress ? .5 : 0;
        
        float vx = filter*vx_prev + (1 - filter)*(x - x_prev)/dt;
        float vy = filter*vy_prev + (1 - filter)*(y - y_prev)/dt;
        
        float y_m = (FRAME_HEIGHT-y)*metersPerPixel;
        float vy_m = vy*metersPerPixel;
        
        //        float vxfilter = kickInProgress ? .5 : 0;
        //        vx_m = vxfilter*vx_m + (1-vxfilter)*vx*metersPerPixel;
        float x_m = x*metersPerPixel;
        float vx_m = vx*metersPerPixel;
        
        float t = 0;
        float x_f = 0;
          
        // not used for now
        if (ACCELERATION)
        {
          float afilter = kickInProgress ? .5 : 0;
          ax_m = afilter*ax_m + (1-afilter)* (vx - vx_prev)/dt * metersPerPixel;
          ay_m = afilter*ay_m + (1-afilter)* (vy - vy_prev)/dt * metersPerPixel;
          
          float a = .5*ay_m;
          float b = vy_m;
          float c = y_m - TOTAL_LENGTH;
          float delta = sqrt(b*b - 4*a*c);
          
          float t1 = (-b+delta)/(2*a);
          float t2 = (-b-delta)/(2*a);
          
          if (t1 > 0)
            t = t1;
          else if (t2 > 0)
            t = t2;
          else
            t = 0;
          
          x_f = x_m + vx_m*t + .5*ax_m*t*t;
        }
          
          
        else
        {
          t = (TOTAL_LENGTH - y_m) / -vy_m;
          x_f = x_m + vx_m*t;
        }
        
        if (-vy > 20) // fast enough to potentially be a kick
        {
          if (!kickInProgress)
            kickInProgress = true;
          
          printf("%d: x=(%d,%d)\t v=(%.2f,%.2f)\t y,vy=%.02f,%.02f\t xf=%.02fm @ %.02fs\n", frame_num, x,y, vx,vy, y_m,vy_m, x_f, t);
        }
        else
          kickInProgress = false;
        
        x_prev = x;
        y_prev = y;
        vx_prev = vx;
        vy_prev = vy;
      }
    }
    
		//show frames
		imshow(windowName2,threshold);
		imshow(windowName,cameraFeed);
		imshow(windowName1,HSV);
		
    
		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);
	}
  
  
	return 0;
}
