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

	//����ͼƬ
	bool loadIPL(const char* filename);

	//��ʾ	IPL ͼƬ
	void showIpl(const char* windowname);

	// ת����ʽ
	QImage* IPLtoQImage(IplImage* iplimage);
	IplImage* QImagetoIPL(QImage& img);
	QImage MattoQImage(cv::Mat& mat);
	cv::Mat QImage2Mat(QImage& img);

	inline IplImage* getIPL() { return iplimage; };


private:
	IplImage* iplimage;
	
};

