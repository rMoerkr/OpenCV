#include "HW2.h"
void OnMouseAction(int event, int x, int y, int flags, void *ustc);
Mat videoFrame,ROI;
int xprev, yprev, xnow, ynow;
uint8_t downFlag;
void Bswitch(int state) {
	for (int i = 0; i < ROI.rows; i++) {
		uchar* data = ROI.ptr<uchar>(i);
		for (int j = 0; j < ROI.cols; j++) {
			data[j * 3 + 0] = data[j * 3 + 0] * (state / 255);
		}
	}
	imshow("ROI", ROI);
}
void Gswitch(int state) {
	for (int i = 0; i < ROI.rows; i++) {
		uchar* data = ROI.ptr<uchar>(i);
		for (int j = 0; j < ROI.cols; j++) {
			data[j * 3 + 1] = data[j * 3 + 0] * (state / 255);
		}
	}
	imshow("ROI", ROI);
}
void Rswitch(int state) {
	for (int i = 0; i < ROI.rows; i++) {
		uchar* data = ROI.ptr<uchar>(i);
		for (int j = 0; j < ROI.cols; j++) {
			data[j * 3 + 2] = data[j * 3 + 0] * (state / 255);
		}
	}
	imshow("ROI", ROI);
}
int HW2() {
	clock_t cprev, cnow;
	VideoCapture cap;
	cap.open("SkyFire.avi");

	char input = 0;
	int i = 0;
	int totalFrames = cap.get(CV_CAP_PROP_FRAME_COUNT);
	int fps = cap.get(cv::CAP_PROP_FPS);
	int valueB = 255; int valueG = 255; int valueR = 255;

	cvNamedWindow("frame");
	setMouseCallback("frame", OnMouseAction);
	cprev = clock();

	while (1) {
		cnow = clock();
		if (downFlag) {
			rectangle(videoFrame, Point(xprev, yprev), Point(xnow, ynow), Scalar(0, 0, 255), 1);
			imshow("frame", videoFrame);
		}
		if (cnow - cprev >= 1000 / fps) {
			if (!cap.read(videoFrame)) continue;
			if (++i % totalFrames == 0) cap.set(CV_CAP_PROP_POS_FRAMES, 0);
			input = cv::waitKey(1);
			cprev = cnow;
			cvCreateTrackbar("B", "ROI", &valueB, 255, Bswitch);
			cvCreateTrackbar("G", "ROI", &valueG, 255, Gswitch);
			cvCreateTrackbar("R", "ROI", &valueR, 255, Rswitch);
			imshow("frame", videoFrame);
		}
	}
	return 0;
}

void OnMouseAction(int event, int x, int y, int flags, void *ustc)
{
	switch (event) {
	case(CV_EVENT_MOUSEMOVE): {
		xnow = x;
		ynow = y;
		break;
	}
	case(CV_EVENT_LBUTTONDOWN):
		xprev = x;
		yprev = y;
		downFlag = 1;
		break;
	case(CV_EVENT_LBUTTONUP): {
		vector<Mat> channels;
		ROI = (videoFrame(Rect(min(xprev, x), min(yprev, y), abs(x - xprev), abs(y - yprev))));
		imshow("ROI", ROI);
		split(ROI, channels);
		for (int i = 0; i < 3; i++) {
			Scalar sca = { 0, 0, 0, 0 };
			sca[i] = 255;
			Mat MaskImage(ROI.rows, ROI.cols, CV_8UC3, sca);
			Mat cROI(ROI.rows, ROI.cols, CV_8UC3, Scalar(0, 0, 0));
			ROI.copyTo(cROI, MaskImage);
			imshow("color" + i, cROI);
			Mat textFrame = videoFrame;
			putText(textFrame, to_string(countNonZero(channels[i])), Point(100, 50 + 50 * i), 0, 1, sca);
			imshow("Numbers", textFrame);
		}
		downFlag = 0;
		break;
	}
	default:
		break;
	}
}