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
	this->cvImg = nullptr; //��ʼ��
	this->m_coreImg = new QImage();
}

QPoisson::~QPoisson()
{
	delete m_coreImg; 
	m_coreImg = nullptr;
}

void QPoisson::setGuiControl()
{
	//���ô��ڴ�С
	setMinimumSize(640,480);
	//���ô���Icon
	setWindowIcon(QIcon(QString("../x64/Debug/images/ico.jpg")));
	//�˵���
	m_menuBar = new QMenuBar(this); 
	//������
	m_toolBar = new QToolBar(this);
	m_toolBar->setGeometry(0, 25, width(), 25);
	m_toolBar->addAction("test");
	m_toolBar->addAction("test1");
	m_toolBar->addAction("test2");
	m_toolBar->addAction("test3");
	m_toolBar->addAction("test4");
	m_toolBar->addAction("test5");
	m_toolBar->addAction("test6");
	//״̬��
	m_statusBar = new QStatusBar(this);
	m_statusBar->setGeometry(0, height() - 25, width(), 25);
	m_statusBar->showMessage("this is a statusBar");
	m_statusLabel = new QLabel(m_statusBar);
	m_statusBar->addPermanentWidget(m_statusLabel,0);
	m_statusLabel->setText("this is a label");
	//�˵�
	m_menu[0] = new QMenu(QString::fromLocal8Bit("�˵�"), m_menuBar);
	m_menu[1] = new QMenu(QStr("�༭"), m_menuBar);
	//����
	m_menuAct[0] = new QAction(QIcon("../x64/Debug/images/fileimage.png"), QStr("��"), this);
	m_menuAct[0]->setShortcuts(QKeySequence::Open);
	m_menuAct[1] = new QAction(QStr("����"),this);
	m_menuAct[1]->setShortcuts(QKeySequence::Save);
	m_editAct[0] = new QAction(QStr("��ֵ�˲�"), this);
	m_editAct[1] = new QAction(QStr("�ҶȻ�"), this);
	//��Ӷ���
	m_menu[0]->addAction(m_menuAct[0]);
	m_menu[0]->addAction(m_menuAct[1]);
	m_menu[1]->addAction(m_editAct[0]);
	m_menu[1]->addAction(m_editAct[1]);
	//��Ӳ˵�
	m_menuBar->addMenu(m_menu[0]);
	m_menuBar->addMenu(m_menu[1]);
	m_menuBar->setGeometry(0, 0, width(), 25);
	//��� Label
	m_pLabelOld = new QLabel(this);
	m_pLabelOld->setGeometry(0, 50, width(), height() - 75);
	//�����ۺ���
	connect(m_menuAct[0], &QAction::triggered, this, &QPoisson::on_open_triggered);
	connect(m_editAct[1], &QAction::triggered, this, &QPoisson::on_clicked_gray);
	connect(m_editAct[0], &QAction::triggered, this, &QPoisson::on_clicked_poisson);
}

//���ڴ�С�ı��¼�
void QPoisson::resizeEvent(QResizeEvent * event)
{
	//Label����Ӧ
	m_pLabelOld->setGeometry(0, 50, width(), height() - 75);
	//ͼƬ����Ӧ����
	resizeImg();
	//�˵���������״̬������Ӧ
	m_menuBar->setGeometry(0, 0, width(), 25);
	m_toolBar->setGeometry(0, 25, width(), 25);
	m_statusBar->setGeometry(0, height() - 25, width(), 25);
}

void QPoisson::loadImg(QString& _imgname)
{
	if (!_imgname.isEmpty())
	{
		//����ͼƬ
		bool flag = m_coreImg->load(_imgname);
		if (flag)
		{
			resizeImg();
			setWindowTitle(_imgname);
		}
		else
			qDebug() << QStr("ͼƬ����ʧ�ܣ�");
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
		//��ȡͼƬ�ֱ���
		QRect pRect = m_coreImg->rect();
		if (pRect.width() > pRect.height())
			m_pLabelOld->setPixmap(QPixmap::fromImage(*m_coreImg).scaledToWidth(m_pLabelOld->width()));
		else
			m_pLabelOld->setPixmap(QPixmap::fromImage(*m_coreImg).scaledToHeight(m_pLabelOld->height()));
	}
}

//����򿪶���
void QPoisson::on_open_triggered()
{ 
	//��ͼƬ������
	m_picName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("��ͼƬ"), "./", "Images (*.png *.xpm *.jpg)");
	if (!m_picName.isEmpty())
	{
		loadImg(m_picName);
	} 
}

//���� �ҶȻ� ͼƬ
void QPoisson::on_clicked_gray()
{
	if (this->m_picName.isEmpty())
		return;
	//��� CVimg �е�ʵ�����
	CVimg cvImg;
	IplImage* iplcopy = nullptr;
	if (cvImg.getIPL() == nullptr)
	{
		//���ص�ǰ��ά����ͼƬ
		bool success = cvImg.loadIPL(m_picName.toLocal8Bit().data());
		if (success)
		{
			//��� CVimg �� IPLimag ����Դ
			iplcopy = cvImg.getIPL();
		}
	}
	/////////////////////////////////////////////////
	//��IPLimage ת�� Mat
	cv::Mat mat = cv::cvarrToMat(iplcopy,true); //����һ����Դ
	// �� Mat ת�ɻҶ�ͼ��
	cvtColor(mat, mat, CV_BGR2GRAY);
	//�� Mat ת�� QImage
	QImage newImg = cvImg.MattoQImage(mat);
	//��ת����ɵ� QImage ��ʾ�� QLabel ��
	MQLabel *m_pLabelNew = new MQLabel;
	//����ڴ�й©
	connect(m_pLabelNew, &MQLabel::closeLabel, [=]() {delete m_pLabelNew; });
	m_pLabelNew->show();
	m_pLabelNew->setMinimumSize(640, 480);
	//����ط��е��Ц
	m_pLabelNew->setPixmap(QPixmap::fromImage(newImg).scaled(m_pLabelNew->size()));
	connect(m_pLabelNew, &MQLabel::windowsizechanged, this, [=]()
	{
		m_pLabelNew->setPixmap(QPixmap::fromImage(newImg).scaled(m_pLabelNew->size()));
	});
}

//���� ���� 
void QPoisson::on_clicked_poisson()
{
	if (this->m_picName.isEmpty())
		return;
	///////////////////////////////////////////////////////////////
	//��� CVimg �е�ʵ�����
	CVimg cvImg;
	IplImage* ipl;
	//���Ϊ�գ���δ�����κε�ͼƬ��Դ
	if (cvImg.getIPL() == nullptr)
	{
		//���ص�ǰ��ά����ͼƬ
		bool success = cvImg.loadIPL(m_picName.toLocal8Bit().data());
		if (success)
		{
			//��������Դ��ȡ����Դ��ָ��
			ipl = cvImg.getIPL();
		}
	}
	//����
	cv::Mat m = cv::cvarrToMat(ipl, true);
	int nwidth = ipl->width;
	int nheight = ipl->height;

	CMat cmat;
	cmat.setCMat(m);
	cv::Mat cm = cmat.Poission();
	//ת��ͼ��
	QImage img = cvImg.MattoQImage(cm);
	//��ת����ɵ� QImage ��ʾ�� QLabel ��
	MQLabel *m_pLabelNew= new MQLabel;
	//����ڴ�й©
	connect(m_pLabelNew, &MQLabel::closeLabel, [=]() {delete m_pLabelNew; });
	m_pLabelNew->show();
	m_pLabelNew->setMinimumSize(640, 480);
	m_pLabelNew->setPixmap(QPixmap::fromImage(img).scaled(m_pLabelNew->size()));
	//����resize �¼�
	connect(m_pLabelNew, &MQLabel::windowsizechanged, this, [=]()
	{
		m_pLabelNew->setPixmap(QPixmap::fromImage(img).scaled(m_pLabelNew->size()));
	});
}


 


