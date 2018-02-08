#include <stdio.h>
#include <opencv2\opencv.hpp>


using namespace cv;
RNG rng(12345);

int main(int argc, char** argv) {
	Mat board_rgb, board_g, board_bin, se_rgb, se_bin;
	board_rgb = imread("C:/Users/jordan/Documents/CV_Homework2/images/p1_board.JPG", 1);
	se_rgb = imread("C:/Users/jordan/Documents/CV_Homework2/images/frame.png", 1);

	cvtColor(board_rgb, board_g, cv::COLOR_BGR2GRAY);
	threshold(board_g, board_bin, 0.85 * 255, 255, THRESH_BINARY);

	{
		Mat se_g;
		cvtColor(se_rgb, se_g, cv::COLOR_BGR2GRAY);
		threshold(se_g, se_bin, 0.85 * 255, 255, THRESH_BINARY);
	}
	
	Mat board_dilated, board_inv, se_inv;
	bitwise_not(se_bin, se_inv);
	bitwise_not(board_bin, board_inv);

	dilate(board_inv, board_dilated, se_inv);

	//namedWindow("Display Image", WINDOW_AUTOSIZE);
	/*
	
	SimpleBlobDetector det;
	std::vector<KeyPoint> keypoints;
	det.detect(board_dilated, keypoints);
	Mat detected;
	drawKeypoints(board_dilated, keypoints, detected, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	imshow("blobs", detected);
	*/

	
	std::vector<std::vector<Point>> spaces;
	std::vector<Vec4i> hierarchy;

	findContours(board_dilated, spaces, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	Mat contours = Mat::ones(board_dilated.size(), CV_8UC3);
	Mat black = Mat::zeros(board_dilated.size(), CV_8UC3);


	for (int i = 0; i < spaces.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(contours, spaces, i, color, 2, 8, hierarchy, 0, Point());
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", contours);

	imshow("Display Image", board_bin);
	imshow("2", se_bin);
	imshow("Window 3", board_dilated);

	waitKey(0);
	//cin.get()
	return 0;


}