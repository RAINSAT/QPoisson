#include "CMat.h"
#include <opencv/cv.h>
#include <assert.h>

CMat::CMat()
{
	
}

CMat::CMat(const cv::Mat& _mat)
{
	//ÄÃµ½×ÊÔ´
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
	assert(mat == &_mat);
	return true;
}

cv::Mat CMat::Poission()
{

}