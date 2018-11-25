#include "MQLable.h"
#include <qevent.h>


MQLabel::MQLabel()
{
	this->iplLabel = nullptr;
}


MQLabel::MQLabel(const IplImage* _iplabel)
{
	this->iplLabel = _iplabel;
}

MQLabel::~MQLabel()
{

}
void MQLabel::closeEvent(QCloseEvent *event)
{
	if (iplLabel)
	{
		IplImage* tmp = const_cast<IplImage*>(iplLabel);
		cvReleaseImage(&tmp);
	}
	emit closeLabel();
}

void MQLabel::resizeEvent(QResizeEvent *event)
{
	//获得窗口新的大小
	QSize newSize = event->size();
	emit windowsizechanged(newSize);
}

