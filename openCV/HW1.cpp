#include "HW1.h"

void picout(Mat input, String formname) {
	Mat output;
	resize(input, output, Size(500, 400), 0, 0, 0);
	imshow(formname, output);
}

int HW1() {
	cout << "import level:" << endl;
	int level;
	cin >> level;
	Mat pic(4, 5, CV_8UC3, Scalar(0, 0, 0));
	Mat picPlus(4, 5, CV_8UC3, Scalar(0, 0, 0));
	Mat picMinus(4, 5, CV_8UC3, Scalar(0, 0, 0));
	Mat picLevel(4, 5, CV_8UC3, Scalar(0, 0, 0));
	Mat picLevel2(4, 5, CV_8UC3, Scalar(0, 0, 0));
	vector<int[3]> vec(level);//儲存出現頻率的向量
	int sum[3] = { 0,0,0 };
	int BGR[3][4][5] = {
		{ {17,44,32,127,240},	{22,231,21,156,124},	{32,35,78,21,194},		{127,22,33,54,45} } ,
		{ {14,145,132,25,40},	{212,221,121,54,14},	{132,235,178,19,14} ,	{122,122,133,54,47} } ,
		{ {247,67,32,187,240 }, {122,41,21, 16, 154},	{52,35,79,21,93},		{27,22,35,154,75} }
	};
	for (int i = 0; i < pic.rows; i++) {
		for (int j = 0; j < pic.cols; j++) {
			for (int k = 0; k < 3; k++) {//顔色空間
				pic.at<cv::Vec3b>(i, j)[k] = BGR[k][i][j];
				picPlus.at<cv::Vec3b>(i, j)[k] = min(BGR[k][i][j] + 32, 255);
				picMinus.at<cv::Vec3b>(i, j)[k] = max(BGR[k][i][j] - 20, 0);
				picLevel.at<cv::Vec3b>(i, j)[k] = ((int)(BGR[k][i][j] / (255 / level))) * (255 / level);
				vec[(int)((BGR[k][i][j] / (255 / level)))][k]++;
				sum[k]++;
			}
		}
	}
	for (int i = 0; i < pic.rows; i++) {
		for (int j = 0; j < pic.cols; j++) {
			for (int k = 0; k < 3; k++) {//顔色空間
				picLevel2.at<cv::Vec3b>(i, j)[k] =
					((vec[((int)picLevel.at<cv::Vec3b>(i, j)[k] / (255 / level))][k]) * 255 / sum[k]);
			}
		}
	}
	for (int i = 0; i < level; i++) {
		cout << "level " << i << endl;
		cout << "    B" << vec[i][0] << endl;
		cout << "    G" << vec[i][1] << endl;
		cout << "    R" << vec[i][2] << endl;
	}
	picout(pic, "pic");
	picout(picPlus, "picPlus");
	picout(picMinus, "picMinus");
	picout(picLevel, "picLevel");
	picout(picLevel2, "picLevel2");
	cv::waitKey(0);
	return 0;
}