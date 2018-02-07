#include <stdio.h>
#include "opencv2\opencv.hpp"


using namespace cv;

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage: DisplayImage\n");

		return -1;

	}

	Mat image;
	image = imread(argv[1], 1);

	Mat boardimage = imread("C:/Users/Jordan/Documents/Drive/CMPR 575/HW2/input/Part_1/p1_board.JPG", 1);

	if (!image.data) {
		printf("No image data\n");
		return -1;



	}

	namedWindow("Display Image", WINDOW_AUTOSIZE);

	imshow("Display Image", boardimage);

	waitKey(0);

	return 0;


}