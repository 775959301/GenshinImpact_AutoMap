#pragma once
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>

class QtClassMyExitButton : public QPushButton
{
	Q_OBJECT

public:
	QtClassMyExitButton(QWidget  *parent);
	~QtClassMyExitButton();

protected:
	void mouseDoubleClickEvent(QMouseEvent *ev); /*˫���¼���Ӧ����*/

signals:
	void mouseDoubleClickExitExe();
};
