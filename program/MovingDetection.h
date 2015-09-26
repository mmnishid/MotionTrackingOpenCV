/*
Michael Nishida
CS 410VC
3/14/14
Project 2
MovingDetection.h
*/


//Opencv Library Declarations
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"

//Basic C++ library Declarations
#include <iostream>
#include <string>
#include<vector>

using namespace cv;
using namespace std;


class MovingDetection{

	private:

		VideoCapture cap;
		Mat background, result;

	public:

		//Constructor
		MovingDetection();

		//user interface
		void menu();

		//read in and play a video file
		int readVideo(String address);
		void playVideo();

		//moving tracking functions
		void getBackground(String address);
		void filterMovingObjects(String address1, String address2);
		Mat filterTotalBackground(Mat frame);

		//save the average static background image
		void saveBackgroundImage(String address);

		//Read User Input Functions
		String readString(String message);
		double readDouble(String message);
		int readInt(String message);
		int readOddInt(String message);
};