#pragma once
#include <QWidget>
#include <QUdpSocket>


class ATM_SendSocket
{
	QString _AvatarData;
	QString _statusCode; //״̬��Ϣ
	QUdpSocket *_autoMapUdpSocket; //�׽���
	QHostAddress _clientIp=QHostAddress::LocalHost; //�ͻ���ip
	quint16 _clientPort = 6666; //�ͻ���port
public:
	ATM_SendSocket();
	~ATM_SendSocket();
	void setPort(quint16 port);
	void AutoMapUdpSocketSend(int x,int y,double orientationAngle);
	//void SocketSend(QString sendStr, QHostAddress targetIp, quint16 targetPort); //��������,������ָ��Ŀ�귢�ͣ����߹㲥

};
