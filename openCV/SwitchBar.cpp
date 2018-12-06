#include "SwitchBar.h"

Mat img;
void switch_trigger(int state) {
	float times = (state+1) / 100.0;
	cout << times << endl;
	Mat out;
	resize(img, out, Size(img.cols*times, img.rows*times), times, times, 1);
	imshow("pic", out);
}
int SwitchBar() {
	int switch_value=100;
	img = imread("pic.jpg", 1);
	namedWindow("SwitchBar");
	namedWindow("pic");
	imshow("pic", img);
	cvCreateTrackbar("Times", "SwitchBar", &switch_value, 200, switch_trigger);
	cv::waitKey(0);
	destroyAllWindows();
	return 0;
}
