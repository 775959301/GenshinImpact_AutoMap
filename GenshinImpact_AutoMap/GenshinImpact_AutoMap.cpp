#include "GenshinImpact_AutoMap.h"

GenshinImpact_AutoMap::GenshinImpact_AutoMap(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	//�����ޱ߿�
	//setWindowFlags(Qt::CustomizeWindowHint);
	setWindowFlags(Qt::FramelessWindowHint);
	
}

void GenshinImpact_AutoMap::doubleClickExit()
{

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

void GenshinImpact_AutoMap::mouseMoveEvent(QMouseEvent * e)
{
	qDebug() << "mouse move "<<e->x()<<","<< e->y();
}

void GenshinImpact_AutoMap::EventClickMove()
{
}
