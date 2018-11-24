#include "MQLable.h"



MQLabel::MQLabel()
{

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
	IplImage* tmp = const_cast<IplImage*>(iplLabel);
	cvReleaseImage(&tmp);

	int a = 0;
}

