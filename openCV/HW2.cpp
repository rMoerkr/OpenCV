#include "HW2.h"
void OnMouseAction(int event, int x, int y, int flags, void *ustc);
VideoCapture cap;
Mat videoFrame;
int xprev, yprev, xnow, ynow;
uint8_t downFlag;


int HW2() {
	clock_t cprev, cnow;

	cap.open("SkyFire.avi");
	char input = 0;
	int i = 0;
	int totalFrames = cap.get(CV_CAP_PROP_FRAME_COUNT);
	int fps = cap.get(cv::CAP_PROP_FPS);
	int num[3];

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
			imshow("frame", videoFrame);
		}
	}
	return 0;
}

void OnMouseAction(int event, int x, int y, int flags, void *ustc)
{
	switch (event)
	{
	case(CV_EVENT_MOUSEMOVE):
		xnow = x;
		ynow = y;
		break;
	case(CV_EVENT_LBUTTONDOWN):
		xprev = x;
		yprev = y;
		downFlag = 1;
		break;
	case(CV_EVENT_LBUTTONUP): {
		vector<Mat> channels;
		Mat ROI = (videoFrame(Rect(min(xprev, x), min(yprev, y), abs(x - xprev), abs(y - yprev))));
		split(ROI, channels);
		for (int i = 0; i < 3; i++) {
			Scalar sca = { 0, 0, 0, 0 };
			sca[i] = 255;
			Mat MaskImage(ROI.rows, ROI.cols, CV_8UC3, sca);
			Mat cROI(ROI.rows, ROI.cols, CV_8UC3, Scalar(0, 0, 0));
			ROI.copyTo(cROI, MaskImage);
			imshow("color" + i, cROI);
			Mat textFrame = videoFrame;
			putText(textFrame, to_string(countNonZero(channels[i])), Point(100, 50+50*i), 0, 1, sca);
			imshow("Numbers", textFrame);
		}
		downFlag = 0;
		imshow("ROI", ROI);
		break;
	}
	default:
		break;
	}
}