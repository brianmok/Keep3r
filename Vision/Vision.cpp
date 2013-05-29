// Original tracking code written by Kyle Hounslow 2013, from http://www.youtube.com/watch?v=bSeFrPrqZ2A

#include <sstream>
#include <string>
#include <iostream>
#include <highgui.h>
#include <cv.h>
#include <time.h>
#include <sys/time.h>

using namespace cv;

//initial min and max HSV filter values.
//these will be changed using trackbars
//int H_MIN = 0;
//int H_MAX = 256;
//int S_MIN = 0;
//int S_MAX = 256;
//int V_MIN = 0;
//int V_MAX = 256;

#define THRESH_MAX 256
/*
Optimal for Purple Ball with all lights on
int H_MIN = 88;
int H_MAX = 138;
int S_MIN = 31;
int S_MAX = 102;
int V_MIN = 220;
int V_MAX = 255;
*/

int H_MIN = 88;
int H_MAX = 138;
int S_MIN = 31;
int S_MAX = 102;
int V_MIN = 191;
int V_MAX = 255;

//default capture width and height

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
const float FRAME_WIDTH_M = 6.4;
const float FRAME_HEIGHT_M = 4.8;
const float TOTAL_LENGTH = 6;
float metersPerPixel = FRAME_WIDTH_M / FRAME_WIDTH;

const int BALL_DIA = 10;

//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS= 50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 10 * 10;
const int MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH / 1.5;

VideoCapture capture; /* Video capture object to acquire webcam feed */
Mat cameraFeed; /* Matrix to store each frame of the webcam feed */
Mat HSVimage; /* Matrix storage for HSV image */
Mat threshimage; /*matrix storage for binary threshold image */

//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";


//x and y values for the location of the object
int x = 0, y = 0;
int frame_num = 0;
float y_f = 0;

struct timeval start, prev_frame_time;



/*--------------------------- Funtion Prototypes ---------------------------*/
void initVision();
void createTrackbars();
void on_trackbar(int, void*);

float processFrame(bool &a_kickInProgess);
void morphOps(Mat &thresh);
bool trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed);
void drawObject(int x, int y, Mat &frame);
void analysis();
void showFrames();

string intToString(int number);
string floatToString(float number);



void initVision() {
	createTrackbars(); // Create slider bars for HSV filtering
    capture.open(0); // Open capture object at location zero (default location for webcam)
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH); // Set the width of capture frame
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT); // Set the height and width of capture frame
	
    //start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
    gettimeofday(&start, NULL);
    prev_frame_time = start;
}



void on_trackbar(int, void*) {
    //This function gets called whenever a trackbar position is changed
}



/* Original
void createTrackbars() {
	//create window for trackbars
    
    
    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
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
*/

void createTrackbars()
{
	//create window for trackbars
    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf(TrackbarName, "H_MIN (%d)", H_MIN);
	sprintf(TrackbarName, "H_MAX (%d)", H_MAX);
	sprintf(TrackbarName, "S_MIN (%d)", S_MIN);
	sprintf(TrackbarName, "S_MAX (%d)", S_MAX);
	sprintf(TrackbarName, "V_MIN (%d)", V_MIN);
	sprintf(TrackbarName, "V_MAX (%d)", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, THRESH_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, THRESH_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, THRESH_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, THRESH_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, THRESH_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, THRESH_MAX, on_trackbar );
}


int x_prev = 0;
int y_prev = 0;
float vx_prev = 0;
float vy_prev = 0;
float ax_m = 0;
float ay_m = 0;

float dt = 0;
bool kickInProgress = false;


float processFrame(bool &a_kickInProgess) {
    
    frame_num++;
    
    capture.read(cameraFeed); /* Store image to matrix */
    cvtColor(cameraFeed, HSVimage, COLOR_BGR2HSV); /* Convert frame from BGR to HSV colorspace */
    inRange(HSVimage, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshimage); /* Filter HSV image between values and store filtered image to threshold matrix */
    morphOps(threshimage); /* Perform morphological operations on thresholded image to eliminate noise and emphasize the filtered object(s) */
    
    //pass in thresholded frame to our object tracking function
    //this function will return the x and y coordinates of the filtered object
    if (trackFilteredObject(x, y, threshimage, cameraFeed)) {
        analysis();
    } else {
        kickInProgress = false;
    }
    showFrames();
    a_kickInProgess = kickInProgress;
    return y_f;
}

void morphOps(Mat &thresh) {
	// Create structuring element that will be used to "dilate" and "erode" image.
	// The element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
    // Dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));
    
	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);
	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);
}

bool trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed) {
	Mat temp;
	threshold.copyTo(temp);
    
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
    
	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	
    //use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
        int numObjects = (int) hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if (numObjects < MAX_NUM_OBJECTS) {
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
        
				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
                if (area > MIN_OBJECT_AREA && area < MAX_OBJECT_AREA && area > refArea) {
                    x = moment.m10/area;
					y = moment.m01/area;
					objectFound = true;
                } else objectFound = false;
			}
            
			//let user know you found an object
			if(objectFound) {
				putText(cameraFeed, "Tracking Object", Point(0,50), 2, 1, Scalar(0, 255, 0), 2);
				drawObject(x, y, cameraFeed); /* draw object location on screen */
            } else
                putText(cameraFeed, "No objects of correct size", Point(0, 50), 2, 1, Scalar(255, 0, 0), 2);
		} else
            putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
    }
    return objectFound;
}

void analysis() {
    struct timeval frame_time;
    gettimeofday(&frame_time, NULL);
    
    dt = (frame_time.tv_sec + .000001 * frame_time.tv_usec) - (prev_frame_time.tv_sec + .000001 * prev_frame_time.tv_usec);
    
    float filter = kickInProgress ? .5 : 0;
    
    float vx = filter * vx_prev + (1 - filter) * (x - x_prev) / dt;
    float vy = filter * vy_prev + (1 - filter) * (y - y_prev) / dt;
    
    float y_m = y * metersPerPixel;
    float vy_m = vy * metersPerPixel;
    
    float x_m = x * metersPerPixel;
    float vx_m = vx * metersPerPixel;
    
    float t = x_m / - vx_m;
    y_f = y_m + vy_m * t;
    
    // fast enough to potentially be a kick
    if (-vx > 20)  {
        if (!kickInProgress)
            kickInProgress = true;
        printf("%d: x=(%d,%d)\t v=(%.2f,%.2f)\t y,vy=%.02f,%.02f\t yf=%.02fm @ %.02fs\n", frame_num, x, y, vx, vy, y_m, vy_m, y_f, t);
    } else
        kickInProgress = false;
   
    prev_frame_time = frame_time;
    x_prev = x;
    y_prev = y;
    vx_prev = vx;
    vy_prev = vy;
}

void drawObject(int x, int y, Mat &frame) {
	// Use some of the openCV drawing functions to draw crosshairs on your tracked image
    circle(frame, Point(x, y), BALL_DIA, Scalar(0, 255, 0), 2);
	line(frame, Point(x, y - BALL_DIA / 2), Point(x, y - BALL_DIA / 2 * 3), Scalar(0, 255, 0), 2);
	line(frame, Point(x, y + BALL_DIA / 2), Point(x, y + BALL_DIA / 2 * 3), Scalar(0, 255, 0), 2);
	line(frame, Point(x - BALL_DIA / 2, y), Point(x - BALL_DIA / 2 * 3, y), Scalar(0, 255, 0), 2);
	line(frame, Point(x + BALL_DIA / 2, y), Point(x + BALL_DIA / 2 * 3, y), Scalar(0, 255, 0), 2);
    line(frame, Point(x, y), Point(x + vx_prev, y + vy_prev), Scalar(0, 255, 0), 2);
    putText(frame, "Position " + intToString(x) + "," + intToString(y), Point(x, y + 40), 1, 1, Scalar(0, 255, 0), 2);
    putText(frame, "Velocity " + floatToString(vx_prev) + "," + floatToString(vy_prev), Point(x, y + 60), 1, 1, Scalar(0, 255, 0), 2);
    putText(frame, "Time Interval " + floatToString(dt), Point(x, y + 80), 1, 1, Scalar(0, 255, 0), 2);
}

void showFrames() {
    if (kickInProgress) {
        circle(cameraFeed, Point(13, y_f / metersPerPixel), BALL_DIA, Scalar(0, 0, 255), 2);
    }
    imshow(windowName,cameraFeed);
    imshow(windowName1,HSVimage);
    imshow(windowName2,threshimage);
    //waitKey(10); /* Delay 10ms so that screen can refresh or images will not appear */
}

string intToString(int number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}

string floatToString(float number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}
