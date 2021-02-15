#include "GenshinImpact_AutoMap.h"

GenshinImpact_AutoMap::GenshinImpact_AutoMap(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	mapMessageLoopTimer = new QTimer(this);
	mapMessageLoopTimer->start(16);//1000/30=33.3
	//mapMessageLoopTimer->setSingleShot(true);
	connect(mapMessageLoopTimer, SIGNAL(timeout()), this, SLOT(runMap()));
	connect(this, &GenshinImpact_AutoMap::mapUpdataFrontEnd, this, &GenshinImpact_AutoMap::updataFrontEnd);
	connect(this, &GenshinImpact_AutoMap::mapUpdataBackEnd, this, &GenshinImpact_AutoMap::updataBackEnd);


	//�����ޱ߿�
	//setWindowFlags(Qt::CustomizeWindowHint);
	setWindowFlags(Qt::FramelessWindowHint);
	myCursor = new QCursor(QPixmap(":/CurMain/resource/cur3.cur"), 2,2);
	this->setCursor(*myCursor);
	//setWindowOpacity(0.7);

	setWindowFlag(Qt::FramelessWindowHint); /* ע�⣺���������������͸����Ч������Windowsϵͳ�б�������, ����ϵͳ�ɺ��ԡ�*/
	setAttribute(Qt::WA_TranslucentBackground);

	this->setAttribute(Qt::WA_TranslucentBackground, true);

	map.Mat2QImage();
	//ui.MainView->setPixmap(QPixmap::fromImage(map.MainImg));

	ui.MainView->setAttribute(Qt::WA_TranslucentBackground);

	//label->setAttribute(Qt::WA_TranslucentBackground);

	//label->setStyleSheet("background:transparent");

	//map.getGiHandle();
	//map.getThisHandle((HWND)this->winId());
	//map.setWindowsPos();

	//connect(this, &GenshinImpact_AutoMap::mapUpdata, this, &GenshinImpact_AutoMap::runMap);
	//emit this->mapUpdata();
	qDebug() << "Init finish";

}

//bool GenshinImpact_AutoMap::eventFilter(QObject * watched, QEvent * event)
//{
//	switch (event->type())
//	{
//		case QEvent::MouseButtonPress:
//		{
//			if (watched == ui.MainView)
//			{
//
//			}
//			break;
//		}
//	}
//
//	return QWidget::eventFilter(watched, event);
//}

void GenshinImpact_AutoMap::mapinit()
{
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

	//QPainter painter(this);
	///* 0x20Ϊ͸������ɫ�����Զ�������Ϊ0x0��0xff */
	//painter.fillRect(this->rect(), QColor(0, 0, 0, 0x20));

	//QPainter p(this);
	//p.setCompositionMode(QPainter::CompositionMode_Clear);
	//p.fillRect(10, 10, 300, 300, Qt::SolidPattern);
}

void GenshinImpact_AutoMap::EventClickMove()
{
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

