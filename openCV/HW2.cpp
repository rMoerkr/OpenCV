#include "HW2.h"
void OnMouseAction(int event, int x, int y, int flags, void *ustc);
Mat videoFrame;
int xprev, yprev, xnow, ynow;
uint8_t downFlag;
int HW2() {
	clock_t cprev, cnow;
	VideoCapture cap;

	cap.open("SkyFire.avi");
	char input = 0;
	int i = 0;
	int totalFrames = cap.get(CV_CAP_PROP_FRAME_COUNT);
	int fps = cap.get(cv::CAP_PROP_FPS);

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
	if (event == CV_EVENT_MOUSEMOVE)
	{
		xnow = x;
		ynow = y;
	}
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		xprev = x;
		yprev = y;
		downFlag = 1;
	}
	if (event == CV_EVENT_LBUTTONUP) {
		vector<Mat> channels;
		Mat ROI = (videoFrame(Rect(min(xprev,xnow),	min(yprev,ynow), abs(xnow-xprev), abs(ynow-xprev))));
		split(ROI, channels);
		downFlag = 0;
		imshow("ROI", ROI);
		for (int i = 0; i < channels.size(); ++i) {
			Mat singleChannelImage = channels.at(i);
			namedWindow("color"+i);
			imshow("color"+i, singleChannelImage);
		}
	}
}