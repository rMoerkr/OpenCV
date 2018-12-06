#include "HW2.h"
void OnMouseAction(int event, int x, int y, int flags, void *ustc);
Mat videoFrame, ROI, bgrval;
int xprev, yprev, xnow, ynow;
uint8_t downFlag, downFlag2;
int value[3];

void ValSum(int k, int state) {
	Scalar sca = { 0, 0, 0, 0 };
	sca[k] = 255;
	int level = (int)(255 / state) + 1;
	int val;
	vector<int> vec(level);//������F�l�ʵ�����
	for (int i = 0; i < ROI.rows; i++) {//��HW1��ͬ���yӋ�ɫ�Ĕ�Ŀ
		uchar* data = ROI.ptr<uchar>(i);
		for (int j = 0; j < ROI.cols; j++) {
			val = data[j * 3 + k];
			vec[(int)(val / state)]++;
		}
	}
	rectangle(bgrval, cvPoint(0, (200 * k) + 1), cvPoint(850, 199 + 200 * k), cvScalar(255, 255, 255), CV_FILLED, 8, 0);//���
	for (int lev = 0; lev < level; lev++) {
		float pct = (vec[lev] * 1.0 / (ROI.rows*ROI.cols));
		rectangle(bgrval, cvPoint((1 + lev) * 40, (200 * (k + 1) - (int)(pct * 175))), cvPoint((2 + lev) * 40, 200 * (k + 1)), sca, 1, 8, 0);
		if (pct >= 0.01) {//��ע�ٷֱ�
			String tex;
			tex = to_string(pct * 100);
			tex = tex.substr(0, 4);
			putText(bgrval, tex + "%", Point((1 + lev) * 40, (200 * (k + 1) - (int)(pct * 175) - 5)), 0, 0.45, sca);
		}
		putText(bgrval, to_string(lev), Point((1 + lev) * 40 + 10, 199 + 200 * k), 0, 0.5, sca);//��ע���e
	}
	cv::line(bgrval, CvPoint(0, 200), CvPoint(850, 200), Scalar(0, 0, 0));
	cv::line(bgrval, CvPoint(0, 400), CvPoint(850, 400), Scalar(0, 0, 0));//���Q
}


void Bswitch(int state) {
	if (state < 13) {
		value[0] = 13;
	}
	else {
		ValSum(0, state);
		imshow("bgrval", bgrval);
	}
}
void Gswitch(int state) {
	if (state < 13) {
		value[1] = 13;
	}
	else {
		ValSum(1, state);
		imshow("bgrval", bgrval);
	}
}
void Rswitch(int state) {
	if (state < 13) {
		value[2] = 13;
	}
	else {
		ValSum(2, state);
		imshow("bgrval", bgrval);
	}
}

int HW2() {
	downFlag2 = 0;
	value[0] = 255;
	value[1] = 255;
	value[2] = 255;
	clock_t cprev, cnow;
	VideoCapture cap;
	VideoWriter w_cap;
	int rec = cap.open("SkyFire.avi");

	char input = 0;
	int fra = 0;
	int totalFrames = cap.get(CV_CAP_PROP_FRAME_COUNT);
	int fps = cap.get(cv::CAP_PROP_FPS);
	int flag1 = 0;

	cvNamedWindow("frame");
	setMouseCallback("frame", OnMouseAction);
	cprev = clock();
	bgrval = Mat(600, 850, CV_8UC3, Scalar(255, 255, 255));//�D���ʼ��

	while (1) {
		cnow = clock();

		if (cnow - cprev >= 1000 / fps) {//�����Ҫ�M����һ��

			cprev = cnow;
			if (!cap.read(videoFrame)) continue;

			if (downFlag) {//��Ⱦ�t��
				rectangle(videoFrame, Point(xprev, yprev), Point(xnow, ynow), Scalar(0, 0, 255), 1);
			}

			imshow("frame", videoFrame);
			if (++fra % totalFrames == 0) cap.set(CV_CAP_PROP_POS_FRAMES, 0);//���^�_ʼѭ�h
			input = cv::waitKey(1);
			if (input == '1') {
				flag1 = 1;
			}
			if (downFlag2) {//�_�����I�E��

				//����ROI
				if (downFlag) {//������I�б������^������������
					ROI = (videoFrame(Rect(min(xprev, xnow), min(yprev, ynow), abs(xnow - xprev), abs(ynow - yprev))));
					w_cap = VideoWriter("SkyFire-ROI.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(ROI.cols, ROI.rows), true);
				}
				downFlag = 0;

				cvCreateTrackbar("B", "ROI", &value[0], 255, Bswitch);
				cvCreateTrackbar("G", "ROI", &value[1], 255, Gswitch);
				cvCreateTrackbar("R", "ROI", &value[2], 255, Rswitch);

				//Ӌ��ƽ��ֵ�Kݔ��Ӱ��
				Mat avg(ROI.rows, ROI.cols, CV_8UC3, Scalar(255, 255, 255));
				int franow = fra - 1;
				for (int i = 0; i < ROI.rows; i++) {
					uchar* data = ROI.ptr<uchar>(i);
					uchar* datan = avg.ptr<uchar>(i);
					for (int j = 0; j < ROI.cols; j++) {
						int valAvg = (data[j * 3] + data[j * 3 + 1] + data[j * 3 + 2]) / 3;
						for (int k = 0; k < 3; k++) { datan[j * 3 + k] = valAvg; }
					}
					imshow("avange", avg);
				}
				if (fra == franow) franow = -1;
				if (franow > 0) w_cap.write(avg);

				//�ָ�D�񼰽yӋ
				if (flag1) {
					vector<Mat> channels;
					split(ROI, channels);
					Mat textFrame;
					videoFrame.copyTo(textFrame);
					for (int i = 0; i < 3; i++) {
						Scalar sca = { 0, 0, 0, 0 };
						sca[i] = 255;
						Mat MaskImage(ROI.rows, ROI.cols, CV_8UC3, sca);//�u������
						Mat cROI(ROI.rows, ROI.cols, CV_8UC3, Scalar(0, 0, 0));//���Ɇ�ɫ�D��
						ROI.copyTo(cROI, MaskImage);
						imshow("color" + i, cROI);
						putText(textFrame, to_string(countNonZero(channels[i])), Point(100, 50 + 50 * i), 0, 0.7, sca);//�yӋ���N�ɫ�Ĕ���
						imshow("Numbers", textFrame);
					}
				}

				imshow("ROI", ROI);
				for (int i = 0; i < 3; i++) {//ˢ�����Ĕ�ֵ�ց�
					ValSum(i, value[i]);
					imshow("bgrval", bgrval);
				}
				
			}
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
		downFlag2 = 0;//�ɂ�downflag���|�lѭ�h�е��¼�
		break;
	case(CV_EVENT_LBUTTONUP): {
		downFlag2 = 1;
		break;
	}
	default:
		break;
	}
}
