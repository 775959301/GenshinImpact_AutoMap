#pragma once
#include <QWidget>
#include <QUdpSocket>


class ATM_SendSocket
{
	QString _AvatarData;
	QString _statusCode; //状态信息
	QUdpSocket *_autoMapUdpSocket; //套接字
	QHostAddress _clientIp=QHostAddress::LocalHost; //客户端ip
	quint16 _clientPort = 6666; //客户端port
public:
	ATM_SendSocket();
	~ATM_SendSocket();
	void setPort(quint16 port);
	void AutoMapUdpSocketSend(int x,int y,double orientationAngle);
	//void SocketSend(QString sendStr, QHostAddress targetIp, quint16 targetPort); //发送数据,可以向指定目标发送，或者广播

};

