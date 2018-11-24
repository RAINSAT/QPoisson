#pragma once
#include <opencv2/highgui.hpp>

class CMat
{
public:
	CMat();
	CMat(const cv::Mat& _mat);
	CMat(const CMat& another) = delete;
	CMat& operator=(const CMat& another) = delete;
	~CMat();
public:
	bool setCMat(const cv::Mat& _mat);
	cv::Mat Poission();
private:
	cv::Mat* mat;
};

