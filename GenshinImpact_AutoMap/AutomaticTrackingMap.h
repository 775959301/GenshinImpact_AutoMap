#pragma once
#include <QtWidgets/QMainWindow>
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <vector>
#include "ATM_Resource.h"
#include "ATM_MouseEvent.h"


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

	ATM_MouseEvent MET;


	//ԭ����Ϸ���ھ��
	HWND giHandle;
	//�Զ����������
	HWND thisHandle;
	//ԭ�񴰿�����
	RECT giRect;
	//ԭ�񴰿����򱣴�Ա���
	RECT giRectTmp;


	//��������С
	Size autoMapSize = Size(250, 200);
	//������ͼ��С
	Size mapSize = Size(RES.GIMAP.cols, RES.GIMAP.rows);
	//������ͼ�������ģ����ԭ��
	Point mapWorldCenter = Point(1416, 3306);

	//�������������Դ��ͼλ��
	Point zerosMinMap = Point(1466, 3272);
	//���������ԭ�񴰿�λ��
	Point offGiMinMap = Point(250, 100);
	//
	Rect minMapRect;

	QImage MainImg;



	void FrontEndUpdata();
	void BackEndUpdata();

	void Mat2QImage();

public:
	Mat getViewMap();


	void getGiHandle();
	void getThisHandle();
	void getThisHandle(HWND _thisHandle);

	void setWindowsPos();
	void setWindowsPos(HWND _thisHandle);

	void setMouseDownPos(int x,int y);
	void setMouseUpPos(int x, int y);
	void setMouseMovePos(int x, int y);
};

typedef AutomaticTrackingMap ATmap;

