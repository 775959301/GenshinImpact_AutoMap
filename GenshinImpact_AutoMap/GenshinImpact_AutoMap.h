#pragma once

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include "ui_GenshinImpact_AutoMap.h"
#include "AutomaticTrackingMap.h"

////
#include <QDebug>

class GenshinImpact_AutoMap : public QMainWindow
{
    Q_OBJECT

public:
    GenshinImpact_AutoMap(QWidget *parent = Q_NULLPTR);

private:
    Ui::GenshinImpact_AutoMapClass ui;

private:
	//�Զ�׷�ٵ�ͼ
	ATmap map;

	//void eventMouseMove();
	//void updataMap();

protected:
	//bool eventFilter(QObject * watched, QEvent * event)override;
	void mouseMoveEvent(QMouseEvent *e);

private:
	//timer = new QTimer(this);
	//connect(timer, SIGNAL(timeout()), this, SLOT(DrawLine()));

private slots:
	void doubleClickExit();
	void EventClickMove();
};
