#pragma once
#include <QWidget>
#include <opencv2/core/mat.hpp>
#include <string>
#include <opencv2/core/core.hpp>

class QImage;

class CVimg : public QObject
{
	Q_OBJECT

public:
	CVimg();
	~CVimg();

	//加载图片
	bool loadIPL(const char* filename);

	//显示	IPL 图片
	void showIpl(const char* windowname);

	// 转换格式
	QImage* IPLtoQImage(IplImage* iplimage);
	IplImage* QImagetoIPL(QImage& img);
	QImage MattoQImage(cv::Mat& mat);
	cv::Mat QImage2Mat(QImage& img);

	inline IplImage* getIPL() { return iplimage; };


private:
	IplImage* iplimage;
	
};

