#include "CVimg.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <QImage>
#include <QDebug>

// Iplimage
CVimg::CVimg()
{
	iplimage = nullptr;
}

CVimg::~CVimg()
{
	if (iplimage != nullptr)
	{
		cvReleaseImage(&iplimage);
		iplimage = nullptr;
	}
}
//加载图片
bool CVimg::loadIPL(const char* filename)
{
	if (filename != nullptr)
	{
		if (iplimage == nullptr)
		{
			iplimage = cvLoadImage(filename);
			std::cout << "图片加载成功" << std::endl;
			return true;
		}
		else
		{
			cvReleaseImage(&iplimage);
			iplimage = cvLoadImage(filename);
			return true;
		}
		
	}
	return false;
}
void CVimg::showIpl(const char* windowname)
{
	if (iplimage != nullptr)
	{
		cv::namedWindow(windowname, cv::WINDOW_AUTOSIZE);
		cvShowImage(windowname, iplimage);
		cvWaitKey(0);
		cvReleaseImage(&iplimage);
		cvDestroyWindow(windowname);
	}
	else
		std::cout << "错误：" << __LINE__ << __TIME__ << std::endl;
}

//将 iplimage 转到 QImage
QImage* CVimg::IPLtoQImage(IplImage* iplimage)
{
	if (iplimage != nullptr)
	{
		//由IPL转换成MAT
		// Mat(int rows, int cols, int type, void* data, size_t step=AUTO_STEP);
		/*int rows = iplimage->height;
		int cols = iplimage->width;
		int type = CV_8UC1;
		void* data = (void*)iplimage->imageData;

		cv::Mat* mat = new cv::Mat(rows, cols, type, data);*/

		QImage* image;
		int channles = iplimage->nChannels;
		uchar* imgData = (uchar*)iplimage->imageData;

		switch (channles)
		{
		case 1:
			image = new QImage(imgData, iplimage->width, iplimage->height, QImage::Format_Indexed8);
			break;
		case 3:
			image = new QImage(imgData, iplimage->width, iplimage->height, QImage::Format_RGB888);
			break;
		case 4:
			image = new QImage(imgData, iplimage->width, iplimage->height, QImage::Format_ARGB32);
			break;
		default:
			std::cout << "error:no valid image!";
			break;
		}
		
		return image;
		
	}
	return nullptr;
}
// QImage to IPL
IplImage* CVimg::QImagetoIPL(QImage& img)
{
	int width = img.width();
	int height = img.height();

	CvSize size;
	size.height = height;
	size.width = width;

	IplImage* IplImageBuffer = cvCreateImage(size, IPL_DEPTH_8U, 3);
	for(int y = 0;y <height;++y)
	{
		for(int x = 0;x < width;++x)
		{
			QRgb rgb = img.pixel(x, y);
			cvSet2D(IplImageBuffer, y, x, CV_RGB(qRed(rgb), qGreen(rgb), qBlue(rgb)));
		}
	}
	return IplImageBuffer;
}
// Mat -> QImage
QImage CVimg::MattoQImage(cv::Mat& mat)
{
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		qDebug() << "CV_8UC4";
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}

cv::Mat CVimg::QImage2Mat(QImage& image)
{
	cv::Mat mat;
	qDebug() << image.format();
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, CV_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	}
	return mat;
}