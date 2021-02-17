#pragma once
#include <QtWidgets/QMainWindow>
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <vector>
#include "ATM_Resource.h"
#include "ATM_MouseEvent.h"
#include "ATM_GiState.h"
#include "ATM_ThreadMatch.h"

enum ThisWinState
{
	Normal,
	Minimize,
	NoTop,
	Top,
	TopShow,
};

class AutomaticTrackingMap
{
	//������Դ
	ATM_Resource RES;
	//ԭ�񴰿���Ϣ
	ATM_GiState GIS;
	//���߳�ƥ��
	ATM_ThreadMatch TMS;

	cv::Mat MainMat;
	cv::Mat MainMatTmp;

	int thisStateMode = 0;

public:
	AutomaticTrackingMap();
	~AutomaticTrackingMap();
public:

	//���λ��
	ATM_MouseEvent MET;

	//�Զ����������
	HWND thisHandle;
	//�Ƿ������Զ�׷��
	bool isAutoMode = false;

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

	//ԭ���Ƿ��������б�־



	//Qt
	QImage MainImg;

	void Init(HWND mapWindowsHandle);
	void Exit();

	void FrontEndUpdata();
	void BackEndUpdata();

	void Mat2QImage();

public:
	Mat getViewMap();

	void getGiState();
	void setThisState_Normal();
	void setThisState_Minimize();
	void setThisState_TopShow();

	void getThisOffset();

	void getGiHandle();
	void getThisHandle();
	void getThisHandle(HWND _thisHandle);

	void setWindowsPos();
	void setWindowsPos(HWND _thisHandle);

	void setMouseDownPos(int x,int y);
	void setMouseUpPos(int x, int y);
	void setMouseMovePos(int x, int y);

	void setAutoMode();
	bool getAutoMode();
private:
	int getThisState();
	

	
	void setThisState();
	void setThreadMatchMat();
};

typedef AutomaticTrackingMap ATmap;

