/*
Michael Nishida
CS 410VC
3/14/14
Project 2
MovingDetection.cpp
*/


#include "MovingDetection.h"


	/*
	Default Constructor
	inputs: none
	outputs: none
	notes: creates the MovingDetection object
	*/
	MovingDetection::MovingDetection(){
		background = 0;
		result = 0;
		cap = 0;
	}

	/*
	readVideo
	inputs: String representing the address of a video file
	outputs: none
	description: Loads the video file at the addres string if the video 
		could not be readed print out an error before exiting
	*/
	int MovingDetection::readVideo(String address){

		//load the image from the address param as either an RGB or greyscale image
		VideoCapture dst(address);

		//Check to see if the image loaded correctly if not return
		if(!dst.isOpened() )
		{
			cout << "Error: Video cannot be loaded" << endl;
			return -1;
		}

		//saves loaded video to one of the class variables
		cap = dst;
		return 1;
	}
	

	/*
	playVideo
	inputs: none
	outputs: none
	description: plays the video file loaded by the readVideo function
	*/
	void MovingDetection::playVideo(){
		
		Mat frame;

		//get the frames per seconds of the video
		double fps = cap.get(CV_CAP_PROP_FPS); 
		cout << "Frame per seconds : " << fps << endl;

		//
		namedWindow("MyVideo",CV_WINDOW_AUTOSIZE);
		while(1){
			// read a new frame from video
			bool bSuccess = cap.read(frame);
			//if not success, break loop
			if (!bSuccess) 
			{
				cout << "Cannot read the frame from video file" << endl;
				break;
			}

			//show the frame
			imshow("MyVideo", frame); 
			//wait for 'esc' key press for 1 ms. If 'esc' key is pressed, break loop
			if(waitKey(1) == 27) 
			{
				cout << "esc key is pressed by user" << endl;
				break; 
			}
		}
		destroyAllWindows();
	}


	/*
	getBackground
	inputs: String representing the address of a video file
	outputs: none
	description:get the static background of a video file
	*/
	void MovingDetection::getBackground(String address){

		//load the image from the address param as either an RGB or greyscale image
		VideoCapture capSrc(address);

		//Check to see if the image loaded correctly if not return
		if(!capSrc.isOpened() )
		{
			cout << "Error: Video cannot be loaded" << endl;
			return;
		}

		//
		Mat frame;
		Mat dstAvg;
		double alpha = .01;
		double beta;

		//
		
		namedWindow("img",CV_WINDOW_AUTOSIZE); 
		namedWindow("avg",CV_WINDOW_AUTOSIZE);
		

		//
		capSrc.read(dstAvg);
		while(1){
			//read a new frame from video and if not success, break loop
			bool bSuccess = capSrc.read(frame);
			if(!bSuccess)
			{
                cout << "Cannot read the frame from video file" << endl;
				break;
			}

			//
			if(alpha < 0.001){
				alpha = 0.001;
			}
			beta = 1 - alpha;
			addWeighted(frame, alpha, dstAvg, beta, 0.0, dstAvg);
			alpha = alpha / 1.0001;
			background = dstAvg;

			//display background image for testing purposes
			
			imshow("img",frame);
			imshow("avg",dstAvg);
			if(waitKey(1) == 27)
			{
				cout << "esc key is pressed by user" << endl;
				break; 
			}
			
		}

		//
		saveBackgroundImage("background.jpg");
		capSrc.release();
		destroyAllWindows();
		//waitKey(0);
	}


	/*
	filterMovingObjects
	inputs: A String representing the address of the video file, 
	outputs: none
	description: 
	*/
	void MovingDetection::filterMovingObjects(String address1, String address2){

		//
		Mat info;
		Mat frame;
		Mat back;
		Mat back2;

		//
		Mat fore;
		Mat fore2;
		Mat dst;
		
		//
		vector<vector<Point>> contours;

		Mat img = imread("background.jpg", CV_LOAD_IMAGE_UNCHANGED);
		if(img.empty()){
			getBackground(address1);
		}

		//load video from string address
		VideoCapture capSrc(address1);
		//get the frame rate of the video
		double fps = capSrc.get(CV_CAP_PROP_FPS);
		//load the first video frame
		capSrc.read(info);
		//get the width of frames of the video
		double dWidth = info.cols;
		//get the height of frames of the video
		double dHeight = info.rows;
		
		//Create the frame size
		Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

		//initialize the VideoWriter object
		VideoWriter oVideoWriter(address2, CV_FOURCC('P','I','M','1'), fps, frameSize, true);  

		//exit the program if VideoWriter is not initialized successfully
		if(!oVideoWriter.isOpened()){
			cout << "ERROR: Failed to write the video" << endl;
			return;
		}

		//background subtractor for the frames
		BackgroundSubtractorMOG2 bg = BackgroundSubtractorMOG2();
		bg.set("history", 50);
		bg.set("nmixtures", 3);
		bg.set("backgroundRatio", 0.7);
		bg.set("detectShadows", false);
		
		//background subtractor for the filterTotalBackground results
		BackgroundSubtractorMOG2 bg2 = BackgroundSubtractorMOG2();
		bg2.set("history", 50);
		bg2.set("nmixtures", 3);
		bg2.set("backgroundRatio", 0.7);
		bg2.set("detectShadows", false);
		
		//
		namedWindow("Frame");
		namedWindow("Fore");
		namedWindow("Fore2");
		namedWindow("Comb");
		namedWindow("Background");
		namedWindow("Background2");

		//
		namedWindow("TotalFilter");
		imshow("TotalFilter", img);
 
		//traverse through the video file
		while(1){

			//read in the next frame in the video
			bool bSuccess = capSrc.read(frame);
			//if not success, break loop
			if (!bSuccess) 
			{
				cout << "Cannot read the frame from video file" << endl;
				break;
			}

			//find the moving objects in results of the filterTotalBackground function
			Mat total = filterTotalBackground(frame);
			bg.operator()(total,fore);
			bg.getBackgroundImage(back);
			imshow("Fore", fore);
			
			//find the moving objects in the frame and erode the image
			bg2.operator()(frame,fore2);
			bg2.getBackgroundImage(back2);
			erode(fore2, fore2, Mat());
			imshow("Fore2", fore2);

			//combine the two images and blur  and threshold the results
			dst = fore & fore2;			
			blur(dst,dst,Size(10,10));
			blur(dst,dst,Size(10,10));
			blur(dst,dst,Size(10,10));
			threshold(dst,dst,20,255,THRESH_BINARY);
			imshow("Comb", dst);

			//find all the object contours in the image
			findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			
			//Approximate contours to polygons and get bounding rects
			vector<vector<Point> > contours_poly( contours.size() );
			vector<Rect> boundRect( contours.size() );
			for( int i = 0; i < contours.size(); i++ ){ 
				approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
				boundRect[i] = boundingRect( Mat(contours_poly[i]) );
			}

			// Draw polygonal contour and bonding rects
			for( int i = 0; i< contours.size(); i++ ){
				Scalar color = Scalar(0, 0, 255);
				drawContours( frame, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
				rectangle( frame, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
			}

			//writer the frame into the file
			oVideoWriter.write(frame); 

			//show frames
			imshow("Frame", frame);
			imshow("Background", back);
			imshow("Background2", back2);

			if(waitKey(1) >= 0){ 
				break;
			}
		}

		//
		capSrc.release();
		destroyAllWindows();
		//waitKey(0);
	}


	/*
	filterTotalBackground
	inputs: a video frame
	outputs: the thersholded absolute difference between the frame and its 
		static background
	description: finds the foreground objects of a frame image using a static 
		background image
	*/
	Mat MovingDetection::filterTotalBackground(Mat frame){

		Mat diff;
		Mat result;

		//if background class variable is empty read in the background image file
		if(background.empty()){
			background = imread("background.jpg", CV_LOAD_IMAGE_UNCHANGED);
		}

		//take the difference between the frame image and the background and 
		//threshold the result before returning
		absdiff(frame, background, diff);
		threshold(diff, result, 60, 255, CV_THRESH_BINARY);
		return result;
	}


	/*
	saveBackgroundImage
	inputs: a string representing the target address
	outputs: none
	description: saves the image stored in the class variable background to the address the user passes.
	*/
	void MovingDetection::saveBackgroundImage(String address){

		bool bSuccess = imwrite(address, background);
		if( !bSuccess ){
			cout << "Error: Failed to save the image" << endl;
		}
	}
	

	/*
	Menu
	inputs: none
	outputs: none
	description: creates a user interface
	*/
	void MovingDetection::menu(){

		//
		String file;
		bool isNewVideo = true;
		
		//
		bool finished = true;
		while(finished){

			//prints out the menu options the user can choose from
			cout << "Select one of the options below\n";
			cout << "1. Read in new video\n";
			cout << "2. Get the Static Background of the Video\n";
			cout << "3. Detect Moving Objects\n";
			cout << "4. End Program\n";

			//prompts the user for an option number then carries out a command based
			//off of the option number provided
			int option = readInt("Enter Option Number: ");
			if(option == 1){
				file = readString("enter video address: ");
				isNewVideo = true;
			}else if(option == 2){
				if(!file.empty()){
					getBackground(file);
					isNewVideo = false;
				}else{
					cout << "read in a video file";
				}
			}else if(option == 3){
				if(!isNewVideo){
					String address = readString("enter detection video address: ");
					filterMovingObjects(file, address);
				}else{
					cout << "get static background";
				}
			}else{
				finished = false;
			}
		}
	}



	/*
	readString
	inputs: String representing a message to be displayed
	outputs: a string representing that the user passes
	description: prompts the user using the message from the input string
		to enter a string
	*/
	String MovingDetection::readString(String message){
		String input;
		cout << message;
		cin >> input;
		cout << "\n";
		return input;
	}


	/*
	readDouble
	inputs: String representing a message to be displayed
	outputs: a double representing the number the user passes
	description: prompts the user using the message from the input string
		to enter a number, when the user passes a string representing a number
		the function checks to see if the string actually represents a double
		before returning it as a double variable if true. if false the function
		will repeat and prompt the user for a new number.
	*/
	double MovingDetection::readDouble(String message){
		char numbers[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};
		int conti = 1;
		String input;

		//A while loop that keeps looping until user enters a double number
		while(conti == 1){
			conti = 0;
			input = readString(message);

			//
			for(int i = 0; i < input.size(); ++i){
				int isNum = 0;
				int isDeci = 0;
				for(int j = 0; j < 11; ++j){
					if(input[i] == numbers[j]){
						isNum = 1;
						if(input[i] == numbers[10]){
							++isDeci;
							if(input.size() == 1){
							isNum = 0;
							}
						}
					}
				}

				//
				if(isNum == 0 || isDeci > 1){
					conti = 1;
				}
			}

			//
			if(conti == 1){
				cout << "input was not a number\n";
			}
		}

		//
		double value = atof(input.c_str());
		return value;
	}


	/*
	readInt
	inputs: String representing a message to be displayed
	outputs: an int representing the number the user passes
	description: this function uses the readDouble method to prompt the user
		to enter an integer and return the integer the user passes
	*/
	int MovingDetection::readInt(String message){
		int result = readDouble(message);
		return result;
	}


	/*
	readOddInt
	inputs: String representing a message to be displayed
	outputs: an odd int representing the number the user passes
	description: this function uses the readInt method to prompt the user
		to enter an odd integer and return the odd integer the user passes
	*/
	int MovingDetection::readOddInt(String message){

		//
		int result = 1;
		int finish = 1;
		while(finish == 1){
			result = readInt(message);
			if(result % 2 == 1){
				finish = 0;
			}else{
				cout << "input was not an odd number\n";
			}
		}
		return result; 
	}