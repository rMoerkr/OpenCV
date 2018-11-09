#include "SharpnessScore.h"

int SharpnessScore(String name)
{
	Mat imageSource = imread(name);
	Mat imageGrey;

	cvtColor(imageSource, imageGrey, CV_RGB2GRAY);
	Mat imageSobel;
	Sobel(imageGrey, imageSobel, CV_16U, 1, 1);

	//图像的平均灰度
	double meanValue = 0.0;
	meanValue = mean(imageSobel)[0];

	//double to string
	stringstream meanValueStream;
	string meanValueString;
	meanValueStream << meanValue;
	meanValueStream >> meanValueString;
	meanValueString = "Articulation(Sobel Method): " + meanValueString;
	putText(imageSource, meanValueString, Point(20, 50), CV_FONT_HERSHEY_COMPLEX, 0.8, Scalar(255, 255, 25), 2);
	imshow(name, imageSource);
	waitKey(1);
	cout << name<< " "<<meanValue << endl;
	return meanValue;
}