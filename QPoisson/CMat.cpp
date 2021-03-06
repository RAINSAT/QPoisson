#include "CMat.h"
#include <opencv/cv.h>
#include <assert.h>

CMat::CMat()
{
	
}

CMat::CMat(const cv::Mat& _mat)
{
	//�õ���Դ
	this->mat = new cv::Mat(_mat);
}

CMat::~CMat()
{
	if (this->mat)
	{
		delete mat;
		mat = nullptr;
	}
}

bool CMat::setCMat(const cv::Mat& _mat)
{
	this->mat = new cv::Mat(_mat);
	assert(mat != &_mat);
	return true;
}

#include <opencv2/imgproc.hpp>
cv::Mat CMat::Poission()
{
	//here do some work
	cv::medianBlur(*mat, *mat, 3);
	return *mat;
}