#pragma once
#include <QtWidgets/QLabel>
#include <opencv2/imgproc.hpp>

class MQLabel :public QLabel
{
public:
	MQLabel();
	~MQLabel();

	MQLabel(const IplImage* _iplabel);

protected:
	void closeEvent(QCloseEvent *event);

private:
	const IplImage* iplLabel;

};

