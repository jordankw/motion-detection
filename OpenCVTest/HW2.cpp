#include <stdio.h>
#include <opencv2\opencv.hpp>


using namespace cv;
RNG rng(12345);

int main(int argc, char** argv) {
	Mat board_rgb, board_g, board_bin, se_rgb, se_bin;
	board_rgb = imread("../../images/p1_board.JPG", 1);
	//board_rgb = imread($(ProjectDir) + "  ", 1);
	se_rgb = imread("../../images/frame.png", 1);

	int boxSize = 60;

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

	dilate(board_bin, board_dilated, se_inv);

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

	Mat contours = Mat::zeros(board_dilated.size(), CV_8UC3);
	Mat black = Mat::zeros(board_dilated.size(), CV_8UC3);
	Mat contours2 = Mat::zeros(board_dilated.size(), CV_8UC3);

	int maxX, maxY;

	for (int i = 0; i < spaces.size(); i++) {
		
		printf("contour %i size: %i\n", i, spaces[i].size() );

		if (spaces[i][0].x == 0 && spaces[i][0].y == 0) {
			printf("dfkdjfd\n");
			spaces.erase(spaces.begin() + i);
		}
	}

	/*
	for (int i = 0; i < spaces.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(contours, spaces, i, color, 2, 8, hierarchy, 0, Point());
		printf("size: %f\n", spaces[i].size());
	}
	*/
	/// Get the moments
	std::vector<Moments> mu(spaces.size());
	for (int i = 0; i < spaces.size(); i++)
	{
		mu[i] = moments(spaces[i], false);
	}

	///  Get the mass centers:
	std::vector<Point2f> mc(spaces.size());
	for (int i = 0; i < spaces.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}
	Vec2i recSize(boxSize, boxSize);

	Scalar color = Scalar(255, 0, 0);
	Scalar color2 = Scalar(10, 150, 70);
	for (int i = 0; i< spaces.size(); i++)
	{
		//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		//drawContours(contours, spaces, i, color, 2, 8, hierarchy, 0, Point());
		//circle(contours, mc[i], 4, color, -1, 8, 0);

		Vec2i recPos(mc[i].x, mc[i].y);

		rectangle(contours, recPos - recSize, recPos + recSize, color, -1, 9, 0);
		rectangle(contours2, recPos - recSize, recPos + recSize, color2, -1, 9, 0);
	}
	Mat colFlip;
	flip(contours2, colFlip, 1);


	Mat dest;
	addWeighted(colFlip, 0.9, contours, 0.9, 0.0, dest);

	/// Show in a window
	//namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	//imshow("Contours", contours);
	//imshow("Contours 2", colFlip);
	imshow("mix", dest);

	imshow("Display Image", board_bin);
	imshow("2", se_bin);
	imshow("Window 3", board_dilated);

	waitKey(0);
	//cin.get()
	return 0;


}