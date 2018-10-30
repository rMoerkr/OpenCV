#include "HW2.h"
void OnMouseAction(int event, int x, int y, int flags, void *ustc);
Mat videoFrame, ROI, bgrval;
int xprev, yprev, xnow, ynow;
uint8_t downFlag;
int valueB = 255; int valueG = 255; int valueR = 255;

void ValSum(int k, int state) {
	Scalar sca = { 0, 0, 0, 0 };
	sca[k] = 255;
	int level = (int)(255 / state) +1;
	int val;
	vector<int> vec(level);//儲存出現頻率的向量
	for (int i = 0; i < ROI.rows; i++) {
		uchar* data = ROI.ptr<uchar>(i);
		for (int j = 0; j < ROI.cols; j++) {
			val = data[j * 3 + k];
			cout << val << endl;
			vec[(int)(val / state)]++;
		}
	}
	rectangle(bgrval, cvPoint(0, (200 * k) + 1), cvPoint(850, 199 + 200 * k), cvScalar(255, 255, 255), CV_FILLED, 8, 0);
	for (int lev = 0; lev < level; lev++) {
		float pct = (vec[lev] * 1.0 / (ROI.rows*ROI.cols));
		rectangle(bgrval, cvPoint((1 + lev) * 40, (200 * (k + 1) - (int)(pct * 175))), cvPoint((2 + lev) * 40, 200 * (k + 1)), sca, 1, 8, 0);
		if (pct >= 0.01) {
			String tex;
			tex = to_string(pct * 100);
			tex = tex.substr(0, 4);
			putText(bgrval, tex + "%", Point((1 + lev) * 40, (200 * (k + 1) - (int)(pct * 175)-5)), 0, 0.45, sca);
		}
		putText(bgrval, to_string(lev), Point((1 + lev) * 40 + 10, 199 + 200 * k), 0, 0.5, sca);
	}
	cv::line(bgrval, CvPoint(0, 200), CvPoint(850, 200), Scalar(0, 0, 0));
	cv::line(bgrval, CvPoint(0, 400), CvPoint(850, 400), Scalar(0, 0, 0));
}

void Valinti() {
	bgrval = Mat(600, 850, CV_8UC3, Scalar(255, 255, 255));
	cv::line(bgrval, CvPoint(0, 200), CvPoint(850, 200), Scalar(0, 0, 0));
	cv::line(bgrval, CvPoint(0, 400), CvPoint(850, 400), Scalar(0, 0, 0));
}

void Bswitch(int state) {
	if (state < 13) {
		valueB = 13;
	}
	else {
		ValSum(0, state);
		imshow("bgrval", bgrval);
	}
}

void Gswitch(int state) {
	if (state < 13) {
		valueG = 13;
	}
	else {
		ValSum(1, state);
		imshow("bgrval", bgrval);
	}
}

void Rswitch(int state) {
	if (state < 13) {
		valueR = 13;
	}
	else {
		ValSum(2, state);
		imshow("bgrval", bgrval);
	}
}

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
	Valinti();

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
/*
for (int i = 0; i < ROI.rows; i++) {
uchar* data = ROI.ptr<uchar>(i);
for (int j = 0; j < ROI.cols; j++) {
data[j * 3 + 0] = data[j * 3 + 0] * (state / 255);
}
}
imshow("ROI", ROI);
*/