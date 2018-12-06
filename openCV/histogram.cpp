#include "histogram.h"

Mat  imgSrc, imgDst;
Mat hist[3];
vector <cv::Mat> channels;
Mat histImage, showhistImage;
void onTrackbarCallback(int pos, void*)
{
	imgDst = imgSrc.clone();
	showhistImage = histImage.clone();
	line(showhistImage, Point(pos, 0), Point(pos, showhistImage.size().height), Scalar(0, 255, 0), 3);
	threshold(imgDst, imgDst, pos, 255, THRESH_BINARY /*CV_THRESH_BINARY*/);
	Mat imgOtsu = imgSrc.clone();
	double otsuThres = threshold(imgOtsu, imgOtsu, pos, 255, THRESH_BINARY | THRESH_OTSU);
	line(showhistImage, Point((int)otsuThres, 0), Point((int)otsuThres, showhistImage.size().height), Scalar(255, 0, 0), 4);
	imshow("NTUT", imgDst);
	imshow("Histogram", showhistImage);
	waitKey(0);
}
void onHistogram(Mat imgSrc)
{
	/// Establish the number of bins
	int histSize = 256;
	// Set the ranges ( for B,G,R) )
	float range_R[] = { 0, 256 };
	const float* histRange = { range_R /*&range_R[0]*/ };
	for (int c = 0; c < 3; c++) {
		calcHist(&channels[c], 1, 0, Mat(), hist[c], 1, &histSize, &histRange, true, false);
	}

	// Draw the histograms
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	histImage = Mat(hist_h/*rows,Y*/, hist_w/*cols, X*/, CV_8UC3, Scalar(0, 0, 0));
	for (int c = 0; c < 3; c++) {
		Scalar sca = { 0, 0, 0, 0 };
		sca[c] = 255;
		normalize(hist[c], hist[c], 0, histImage.rows, NORM_MINMAX, -1, Mat());
		for (int i = 1; i < histSize; i++)
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(hist[c].at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(hist[c].at<float>(i))),
				sca, 2, 8, 0);
	}

	namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	int TrackbarSlider = 0;
	int TrackbarStopValue = hist_w;
	//createTrackbar("TrackBar", "Histogram", &TrackbarSlider, TrackbarStopValue, onTrackbarCallback);
	imshow("Histogram", histImage);
}
int histogram()
{
	Mat imgSrc = imread("lenna.png");
	if (!imgSrc.data) return -1;
	namedWindow("NTUT", CV_WINDOW_AUTOSIZE);
	imshow("NTUT", imgSrc);
	split(imgSrc, channels);
	/// Call the function to initialize
	onHistogram(imgSrc);
	//onTrackbarCallback(0, 0);
	waitKey(0);
	return 0;
}