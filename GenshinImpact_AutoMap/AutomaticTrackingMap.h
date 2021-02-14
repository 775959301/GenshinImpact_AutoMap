#pragma once
#include <QtWidgets/QMainWindow>
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <vector>
#include "ATM_Resource.h"

class AutomaticTrackingMap
{
	//������Դ
	ATM_Resource RES;

	cv::Mat MainMat;
	cv::Mat MainMatTmp;
public:
	AutomaticTrackingMap();
	~AutomaticTrackingMap();
public:
	//ԭ����Ϸ���ھ��
	HWND giHandle;
	//�Զ����������
	HWND thisHandle;
	//ԭ�񴰿�����
	RECT giRect;
	//ԭ�񴰿����򱣴�Ա���
	RECT giRectTmp;

	Point offGiMinMap = Point(288, 82);

	QImage MainImg;

	void Mat2QImage();

public:
	void getGiHandle();
	void getThisHandle();
	void getThisHandle(HWND _thisHandle);

	void setWindowsPos();
	void setWindowsPos(HWND _thisHandle);
};

typedef AutomaticTrackingMap ATmap;

