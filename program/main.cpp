/*
Michael Nishida
CS 410VC
3/14/14
Project 2
main.cpp
*/


//opencv library declarations
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

//
#include <iostream>
#include "MovingDetection.h"

using namespace cv;
using namespace std;



int main(int argc, const char** argv)
{
	//create hybrid image class
	MovingDetection video = MovingDetection();
	video.menu();

	/*
	//
	String input1 = "mv2_003.avi";
	String input2 = "mv2_004.avi";
	String input3 = "mv2_005.avi";
	String file = input2;
	video.getBackground(file);
	video.filterMovingObjects(file,"MyVideo.avi");
	*/
	

	return 0;
}
