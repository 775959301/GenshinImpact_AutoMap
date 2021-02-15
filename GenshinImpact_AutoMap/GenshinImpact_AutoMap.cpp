#include "GenshinImpact_AutoMap.h"

GenshinImpact_AutoMap::GenshinImpact_AutoMap(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	//ˢ�¶�ʱ��
	mapMessageLoopTimer = new QTimer(this);
	mapMessageLoopTimer->start(16);//1000/30=33.3
	//mapMessageLoopTimer->setSingleShot(true);
	connect(mapMessageLoopTimer, SIGNAL(timeout()), this, SLOT(runMap()));
	connect(this, &GenshinImpact_AutoMap::mapUpdataFrontEnd, this, &GenshinImpact_AutoMap::updataFrontEnd);
	connect(this, &GenshinImpact_AutoMap::mapUpdataBackEnd, this, &GenshinImpact_AutoMap::updataBackEnd);

	//�������ָ��
	myCursor = new QCursor(QPixmap(":/CurMain/resource/cur3.cur"), 2, 2);
	this->setCursor(*myCursor);

	//���ð�͸���ޱ߿򴰿�
	//setWindowFlags(Qt::CustomizeWindowHint);
	this->setWindowOpacity(0.9);
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.MainView->setAttribute(Qt::WA_TranslucentBackground);

	mapInit();
}

void GenshinImpact_AutoMap::mapInit()
{
	map.Init((HWND)this->winId());
}

void GenshinImpact_AutoMap::mouseMoveEvent(QMouseEvent * event)
{
	if (map.MET.bLCD)
	{
		qDebug() << "mouse move " << event->x() << "," << event->y();
		map.setMouseMovePos(event->x(), event->y());
	}
}

void GenshinImpact_AutoMap::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton /*&& (event->buttons() & Qt::LeftButton)*/)
	{
		// �������
		map.MET.bLCD = true;
		map.setMouseDownPos(event->x(), event->y());
		qDebug() << "mouse move setMouseDownPos" << event->x() << "," << event->y();
	}
}

void GenshinImpact_AutoMap::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton /*&& (event->buttons() & Qt::LeftButton)*/)
	{
		// �������
		map.MET.bLCD = false;
		map.setMouseUpPos(event->x(), event->y());
		qDebug() << "mouse move setMouseUpPos" << event->x() << "," << event->y();
	}
}

void GenshinImpact_AutoMap::mouseDoubleClickEvent(QMouseEvent * event)
{

}

void GenshinImpact_AutoMap::wheelEvent(QWheelEvent * event)
{

}

void GenshinImpact_AutoMap::paintEvent(QPaintEvent * event)
{
	//���û���Ϊ��ͼ
	QPainter painter(this);
	painter.drawImage(0, 0, map.MainImg);
}

void GenshinImpact_AutoMap::runMap()
{
	static bool isUpdataEnd = true;
	//�����Ƿ��ѽ���
	if (isUpdataEnd)
	{
		isUpdataEnd = false;
		emit this->mapUpdataFrontEnd();
		emit this->mapUpdataBackEnd();
		isUpdataEnd = true;
		//������һ֡
		mapMessageLoopTimer->start(16);
	}
	else
	{
		//������һ֡
		mapMessageLoopTimer->start(16);
	}


}

void GenshinImpact_AutoMap::updataFrontEnd()
{
	//static int count = 0;
	//qDebug() << "Updata Front-End " << count++ << " ";
	
	//���»���ͼ��
	map.FrontEndUpdata();
	//���Ƶ�����
	update();
}

void GenshinImpact_AutoMap::updataBackEnd()
{
	//static int count = 0;
	//qDebug() << "Updata Back-End " << count++ << " ";
	map.BackEndUpdata();
}

