#include "Video.h"
Mat videoFrame;
Mat thrFrame;
void switch_action(int state) {
	threshold(videoFrame, thrFrame, state, 255, THRESH_BINARY);
	imshow("thrFrame", thrFrame);
}
int Video() {
	int switch_value = 100;
	VideoCapture cap;
	char input=0;
	cap.open(0);
	while (1) {
		if (!cap.read(videoFrame)) break;
		Mat thrFrame;
		cvCreateTrackbar("Door", "frame", &switch_value, 200, switch_action);

		imshow("frame", videoFrame);
		input = cv::waitKey(1);
		if (input == 's') {
			imwrite("photo.jpg", videoFrame);
		}
		else if (input == 'q') {
			break;
		}
	}
	system("pause");
	destroyAllWindows();
	return 0;
}