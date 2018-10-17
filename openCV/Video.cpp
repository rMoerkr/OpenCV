#include "Video.h"

int video() {
	VideoCapture cap;
	Mat videoFrame;
	char input=0;
	cap.open(0);
	while (1) {
		if (!cap.read(videoFrame)) break;
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