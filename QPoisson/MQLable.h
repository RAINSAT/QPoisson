#pragma once
#include <QtWidgets/QLabel>
#include <opencv2/imgproc.hpp>

class MQLabel :public QLabel
{
	Q_OBJECT
public:
	MQLabel();
	~MQLabel();

	MQLabel(const IplImage* _iplabel);

protected:
	void closeEvent(QCloseEvent *event);
	void resizeEvent(QResizeEvent *event);

signals:
	void windowsizechanged(QSize size);
	void closeLabel();

private:
	const IplImage* iplLabel;
};

