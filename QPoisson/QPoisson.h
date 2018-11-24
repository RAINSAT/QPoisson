#pragma once

#include <QtWidgets/QWidget>
#include "CVimg.h"
#include "MQLable.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QMenuBar;
class QAction;
class QLabel;
class QImage;
QT_END_NAMESPACE

#define QStr(x) QString::fromLocal8Bit(x) 

class QPoisson : public QWidget
{
	Q_OBJECT

public:
	QPoisson(QWidget *parent = Q_NULLPTR);
	~QPoisson();

	void setGuiControl();
protected:
	void resizeEvent(QResizeEvent *event);

private slots:
	void on_open_triggered();
	void on_clicked_diy();
	void on_clicked_poisson();

private:
	void loadImg(QString&);
	void resizeImg();
private:
	QMenuBar* m_menuBar;
	QMenu* m_menu[2];
	QAction* m_menuAct[2];
	QAction* m_editAct[2];
	QLabel* m_pLabelOld;
	QImage* m_coreImg;	//ά������ͼƬ
	//QImage* m_coreNew; //������ͼƬ
	QString m_picName; //ά����ͼƬ��
	CVimg* cvImg; //cv
};
