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
	//����
	static CVimg* getInstance()
	{
		if (mImg == nullptr)
		{
			mImg = new CVimg();
			return mImg;
		}
		else
			return mImg;
	}

	//����ͼƬ
	bool loadIPL(const char* filename);

	//��ʾ	IPL ͼƬ
	void showIpl(const char* windowname);

	// ת����ʽ
	QImage* IPLtoQImage(IplImage* iplimage);
	IplImage* QImagetoIPL(QImage& img);
	QImage MattoQImage(cv::Mat& mat);
	cv::Mat QImage2Mat(QImage& img);

	void releaseIpl() { iplimage = nullptr; };

	inline IplImage* getIPL() { return iplimage; };

protected:
	CVimg();
	~CVimg();

	static CVimg* mImg;


private:
	IplImage* iplimage;
	QImage* qimage;
	
};

