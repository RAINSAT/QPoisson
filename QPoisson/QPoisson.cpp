#include "QPoisson.h"
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QDir>
#include <QLabel>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QImage>

//CV
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>

#include <stdio.h>
#include <stdlib.h>

#include "CMat.h"

QPoisson::QPoisson(QWidget *parent) : QWidget(parent)
{
	//qDebug() << QDir::currentPath();
	setGuiControl();
	this->cvImg = nullptr; //初始化
	this->m_coreImg = new QImage();
}

QPoisson::~QPoisson()
{
	delete m_coreImg; 
	m_coreImg = nullptr;
}

void QPoisson::setGuiControl()
{
	//设置窗口大小
	setMinimumSize(640,480);
	//设置窗口Icon
	setWindowIcon(QIcon(QString("../x64/Debug/images/ico.jpg")));
	//菜单栏
	m_menuBar = new QMenuBar(this); 
	//工具栏
	m_toolBar = new QToolBar(this);
	m_toolBar->setGeometry(0, 25, width(), 25);
	m_toolBar->addAction("test");
	m_toolBar->addAction("test1");
	m_toolBar->addAction("test2");
	m_toolBar->addAction("test3");
	m_toolBar->addAction("test4");
	m_toolBar->addAction("test5");
	m_toolBar->addAction("test6");
	//状态栏
	m_statusBar = new QStatusBar(this);
	m_statusBar->setGeometry(0, height() - 25, width(), 25);
	m_statusBar->showMessage("this is a statusBar");
	m_statusLabel = new QLabel(m_statusBar);
	m_statusBar->addPermanentWidget(m_statusLabel,0);
	m_statusLabel->setText("this is a label");
	//菜单
	m_menu[0] = new QMenu(QString::fromLocal8Bit("菜单"), m_menuBar);
	m_menu[1] = new QMenu(QStr("编辑"), m_menuBar);
	//动作
	m_menuAct[0] = new QAction(QIcon("../x64/Debug/images/fileimage.png"), QStr("打开"), this);
	m_menuAct[0]->setShortcuts(QKeySequence::Open);
	m_menuAct[1] = new QAction(QStr("保存"),this);
	m_menuAct[1]->setShortcuts(QKeySequence::Save);
	m_editAct[0] = new QAction(QStr("中值滤波"), this);
	m_editAct[1] = new QAction(QStr("灰度化"), this);
	//添加动作
	m_menu[0]->addAction(m_menuAct[0]);
	m_menu[0]->addAction(m_menuAct[1]);
	m_menu[1]->addAction(m_editAct[0]);
	m_menu[1]->addAction(m_editAct[1]);
	//添加菜单
	m_menuBar->addMenu(m_menu[0]);
	m_menuBar->addMenu(m_menu[1]);
	m_menuBar->setGeometry(0, 0, width(), 25);
	//添加 Label
	m_pLabelOld = new QLabel(this);
	m_pLabelOld->setGeometry(0, 50, width(), height() - 75);
	//关联槽函数
	connect(m_menuAct[0], &QAction::triggered, this, &QPoisson::on_open_triggered);
	connect(m_editAct[1], &QAction::triggered, this, &QPoisson::on_clicked_gray);
	connect(m_editAct[0], &QAction::triggered, this, &QPoisson::on_clicked_poisson);
}

//窗口大小改变事件
void QPoisson::resizeEvent(QResizeEvent * event)
{
	//Label自适应
	m_pLabelOld->setGeometry(0, 50, width(), height() - 75);
	//图片自适应窗口
	resizeImg();
	//菜单栏工具栏状态栏自适应
	m_menuBar->setGeometry(0, 0, width(), 25);
	m_toolBar->setGeometry(0, 25, width(), 25);
	m_statusBar->setGeometry(0, height() - 25, width(), 25);
}

void QPoisson::loadImg(QString& _imgname)
{
	if (!_imgname.isEmpty())
	{
		//加载图片
		bool flag = m_coreImg->load(_imgname);
		if (flag)
		{
			resizeImg();
			setWindowTitle(_imgname);
		}
		else
			qDebug() << QStr("图片加载失败！");
	}
}

void QPoisson::resizeImg()
{
	if (m_coreImg->isNull())
	{
		m_pLabelOld->setPixmap(QPixmap(QString("../x64/Debug/images/bkg.jpg")).scaled(width(), height()));
	}
	else
	{
		//获取图片分辨率
		QRect pRect = m_coreImg->rect();
		if (pRect.width() > pRect.height())
			m_pLabelOld->setPixmap(QPixmap::fromImage(*m_coreImg).scaledToWidth(m_pLabelOld->width()));
		else
			m_pLabelOld->setPixmap(QPixmap::fromImage(*m_coreImg).scaledToHeight(m_pLabelOld->height()));
	}
}

//处理打开动作
void QPoisson::on_open_triggered()
{ 
	//打开图片并加载
	m_picName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开图片"), "./", "Images (*.png *.xpm *.jpg)");
	if (!m_picName.isEmpty())
	{
		loadImg(m_picName);
	} 
}

//处理 灰度化 图片
void QPoisson::on_clicked_gray()
{
	if (this->m_picName.isEmpty())
		return;
	//获得 CVimg 中的实例句柄
	CVimg cvImg;
	IplImage* iplcopy = nullptr;
	if (cvImg.getIPL() == nullptr)
	{
		//加载当前所维护的图片
		bool success = cvImg.loadIPL(m_picName.toLocal8Bit().data());
		if (success)
		{
			//获得 CVimg 中 IPLimag 的资源
			iplcopy = cvImg.getIPL();
		}
	}
	/////////////////////////////////////////////////
	//将IPLimage 转成 Mat
	cv::Mat mat = cv::cvarrToMat(iplcopy,true); //拷贝一份资源
	// 将 Mat 转成灰度图像
	cvtColor(mat, mat, CV_BGR2GRAY);
	//将 Mat 转成 QImage
	QImage newImg = cvImg.MattoQImage(mat);
	//将转换完成的 QImage 显示在 QLabel 中
	MQLabel *m_pLabelNew = new MQLabel;
	//解决内存泄漏
	connect(m_pLabelNew, &MQLabel::closeLabel, [=]() {delete m_pLabelNew; });
	m_pLabelNew->show();
	m_pLabelNew->setMinimumSize(640, 480);
	//这个地方有点好笑
	m_pLabelNew->setPixmap(QPixmap::fromImage(newImg).scaled(m_pLabelNew->size()));
	connect(m_pLabelNew, &MQLabel::windowsizechanged, this, [=]()
	{
		m_pLabelNew->setPixmap(QPixmap::fromImage(newImg).scaled(m_pLabelNew->size()));
	});
}

//处理 泊松 
void QPoisson::on_clicked_poisson()
{
	if (this->m_picName.isEmpty())
		return;
	///////////////////////////////////////////////////////////////
	//获得 CVimg 中的实例句柄
	CVimg cvImg;
	IplImage* ipl;
	//如果为空，即未加载任何的图片资源
	if (cvImg.getIPL() == nullptr)
	{
		//加载当前所维护的图片
		bool success = cvImg.loadIPL(m_picName.toLocal8Bit().data());
		if (success)
		{
			//加载了资源后，取出资源的指针
			ipl = cvImg.getIPL();
		}
	}
	//拷贝
	cv::Mat m = cv::cvarrToMat(ipl, true);
	int nwidth = ipl->width;
	int nheight = ipl->height;

	CMat cmat;
	cmat.setCMat(m);
	cv::Mat cm = cmat.Poission();
	//转换图像
	QImage img = cvImg.MattoQImage(cm);
	//将转换完成的 QImage 显示在 QLabel 中
	MQLabel *m_pLabelNew= new MQLabel;
	//解决内存泄漏
	connect(m_pLabelNew, &MQLabel::closeLabel, [=]() {delete m_pLabelNew; });
	m_pLabelNew->show();
	m_pLabelNew->setMinimumSize(640, 480);
	m_pLabelNew->setPixmap(QPixmap::fromImage(img).scaled(m_pLabelNew->size()));
	//触发resize 事件
	connect(m_pLabelNew, &MQLabel::windowsizechanged, this, [=]()
	{
		m_pLabelNew->setPixmap(QPixmap::fromImage(img).scaled(m_pLabelNew->size()));
	});
}


 


