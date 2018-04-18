#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"
#include <iostream>
using namespace cv;
using namespace std;

vector<vector<Point> > contours;

Mat erosionElement;
Mat frame, binMask, colorMask;
Ptr<BackgroundSubtractor> pMOG2;

void TrainMaps(string inputVideo);

int main() {
	TrainMaps("../MU_Video.mp4");
}

void TrainMaps(string inputVideo) {
	VideoCapture trainingVideo = VideoCapture(inputVideo);
	if (!trainingVideo.isOpened())
		return;
	//VideoWriter vid_out = VideoWriter(
	//							"../out.avi",
	//							CV_FOURCC('D', 'I', 'V', 'X'),
	//							vid_in.get(CV_CAP_PROP_FPS),
	//							Size(vid_in.get(CV_CAP_PROP_FRAME_WIDTH),
	//								 vid_in.get(CV_CAP_PROP_FRAME_HEIGHT)));

	pMOG2 = createBackgroundSubtractorMOG2(1500, 10, true);
	erosionElement = getStructuringElement(MORPH_RECT, Size(2, 2));

	while (1) {
		trainingVideo >> frame;
		if (frame.empty())
			break;

		// Determine moving elements in the scene, fill in outlines, and filter noise
		pMOG2->apply(frame, binMask);
		dilate(binMask, binMask, erosionElement);
		erode(binMask, binMask, erosionElement);
		erode(binMask, binMask, erosionElement);
		erode(binMask, binMask, erosionElement);

		// Create color mask of original image
		colorMask = binMask;
		frame.copyTo(colorMask, binMask);

		// Find all moving objects in the scene, and filter out small ones
		findContours(binMask, contours, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		for (int i = contours.size() - 1; i >= 0; i--) {
			if (contourArea(contours[i]) < 20)
				contours.erase(contours.begin() + i);
		}
		drawContours(frame, contours, -1, Scalar(0, 255, 0), 2);

		// TODO - train location map
		// TODO - train velocity map
		// TODO - train size map
		// TODO - train color map?

		//vid_out.write(frame);
		imshow("VidFrame", colorMask);
		if (waitKey(25) == 27)
			break;
	}

	trainingVideo.release();
	//vid_out.release();

	destroyAllWindows();
	return;
}