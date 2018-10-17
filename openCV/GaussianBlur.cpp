#include "GaussianBlur.h"

int GaussianBlur() {
	Mat src_img = imread("pic.jpg"), Gaussian_img1, Gaussian_img2;
	if (src_img.empty()) return -1;
	GaussianBlur(src_img, Gaussian_img1, Size(11, 11), 10, 10);
	GaussianBlur(src_img, Gaussian_img2, Size(5, 5), 10, 10);
	imshow("Sor", src_img);
	imshow("1", Gaussian_img1);
	imshow("2", Gaussian_img2);
	cv::waitKey(0);
	return 0;
}