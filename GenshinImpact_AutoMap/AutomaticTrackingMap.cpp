#include "AutomaticTrackingMap.h"

AutomaticTrackingMap::AutomaticTrackingMap()
{
}

AutomaticTrackingMap::~AutomaticTrackingMap()
{
	Exit();
}

void AutomaticTrackingMap::Init(HWND mapWindowsHandle)
{
	/*
	����״̬��ʼ��
	*/
	MET.zerosMinMap = zerosMinMap;
	getGiHandle();
	getThisHandle(mapWindowsHandle);


	/*
	������ݸ���
	*/
	BackEndUpdata();


	
	/**/
}

void AutomaticTrackingMap::Exit()
{
}

//ǰ����ʾ
void AutomaticTrackingMap::FrontEndUpdata()
{
	/*
	���ò���
	*/

	setThisState();
	setThreadMatchMat();




	/*
	��ʾ����
	*/

	//��ȡ��ʾ�����ͼ
	MainMat = getViewMap();
	//�����Ʒͼ��
	//��ӵ�ǰλ��ͼ��

	/*
	��ʾ�������
	*/

	//ͼƬ�����Qt��ʾ
	Mat2QImage();
}

//���״̬����
void AutomaticTrackingMap::BackEndUpdata()
{
	/*
	���ݴ�����
	*/

	if (isAutoMode)
	{
		//���̼߳�����
		TMS.CheckThread();
		TMS.GetMatchResults();
		//���̳߳�ʼ��
		TMS.cThreadSurfMapInit(RES.GIMAP);
		TMS.cThreadSurfMapMatch();

		if (TMS.tIsEndSurfMapInit)
		{
			zerosMinMap = TMS.pos;
		}
	}

	/*
	��ȡ����
	*/

	//��ȡԭ�񴰿�״̬
	getGiState();

	//��ȡ�������ԭ��λ��
	getThisOffset();
	getThisState();

	/*
	״̬ת�Ʋ���
	*/

	//�жϵ�ǰ������״̬
	thisStateMode = getThisState();


}

void AutomaticTrackingMap::Mat2QImage()
{
	std::vector<Mat> mv0;
	std::vector<Mat> mv1;

	//ͨ������
	split(MainMat, mv0);
	split(RES.MAINMASK, mv1);
	mv0.push_back(mv1[0]);
	merge(mv0, MainMat);
	MainImg = QImage((uchar*)(MainMat.data), MainMat.cols, MainMat.rows, MainMat.cols*(MainMat.channels()), QImage::Format_ARGB32);
}

Mat AutomaticTrackingMap::getViewMap()
{
	static Mat minMap;

	Point minMapPoint = Point(0, 0);

	Size reMapSize = autoMapSize;
	reMapSize.width = (int)(reMapSize.width * MET.scale);
	reMapSize.height = (int)(reMapSize.height * MET.scale);

	Size R = reMapSize / 2;

	Point LT = zerosMinMap - Point(R);
	Point RB = zerosMinMap + Point(R);

	minMapPoint = LT;

	if (LT.x < 0)
	{
		minMapPoint.x = 0;
	}
	if (LT.y < 0)
	{
		minMapPoint.y = 0;
	}
	if (RB.x > mapSize.width)
	{
		minMapPoint.x = mapSize.width - reMapSize.width;
	}
	if (RB.y > mapSize.height)
	{
		minMapPoint.y = mapSize.height - reMapSize.height;
	}
	minMapRect = Rect(minMapPoint, reMapSize);

	resize(RES.GIMAP(minMapRect), minMap, autoMapSize);
	//minMap = matMap(Rect(minMapPoint, reMapSize));

	return minMap;

}

void AutomaticTrackingMap::getGiState()
{
	GIS.getGiState();
	if (GIS.giRectMode > 0)
	{
		GIS.getGiFrame();
	}
}

void AutomaticTrackingMap::setThisState_Normal()
{
	//���ô���λ��
	//setWindowsPos();
	SetWindowPos(thisHandle, HWND_TOP, GIS.giRect.left + offGiMinMap.x, GIS.giRect.top + offGiMinMap.y, 0, 0, SWP_NOSIZE);
}

void AutomaticTrackingMap::setThisState_Minimize()
{
	//��С����ʾ����
	ShowWindow(thisHandle, SW_MINIMIZE);
	//����ԭ�񴰿�Ϊǰ̨
	SetForegroundWindow(GIS.giHandle);/* ��ԭ�񴰿ڵĲ��� */

}

void AutomaticTrackingMap::setThisState_Top()
{
	//���ô���λ��
	setWindowsPos();
	//��ԭ��ʾ����
	ShowWindow(thisHandle, SW_SHOW);
}

void AutomaticTrackingMap::setThisState_TopShow()
{
	//���ô���λ��
	setWindowsPos();
	//��ԭ��ʾ����
	ShowWindow(thisHandle, SW_SHOW);
	//����ԭ�񴰿�Ϊǰ̨
	SetForegroundWindow(GIS.giHandle);/* ��ԭ�񴰿ڵĲ��� */
}

void AutomaticTrackingMap::getThisOffset()
{
	static Point offGiMinMapTmp;
	if (GIS.giRectMode > 0)
	{
		offGiMinMapTmp = GIS.getOffset();
		if (offGiMinMap != offGiMinMapTmp)
		{
			offGiMinMap = offGiMinMapTmp;
		}
	}
}

void AutomaticTrackingMap::getGiHandle()
{
	GIS.getHandle();
}

void AutomaticTrackingMap::getThisHandle()
{
	thisHandle = FindWindowA(NULL, "GenshinImpact_AutoMap");
}

void AutomaticTrackingMap::getThisHandle(HWND _thisHandle)
{
	thisHandle = _thisHandle;
}

void AutomaticTrackingMap::setWindowsPos()
{
	SetWindowPos(thisHandle, HWND_TOPMOST, GIS.giRect.left + offGiMinMap.x, GIS.giRect.top + offGiMinMap.y, 0, 0, SWP_NOSIZE);
}

void AutomaticTrackingMap::setWindowsPos(HWND _thisHandle)
{
	SetWindowPos(_thisHandle, HWND_TOPMOST, GIS.giRect.left + offGiMinMap.x, GIS.giRect.top + offGiMinMap.y, 0, 0, SWP_NOSIZE);
}

void AutomaticTrackingMap::setMouseDownPos(int x, int y)
{
	MET.setMouseDownPos(x, y);
}

void AutomaticTrackingMap::setMouseUpPos(int x, int y)
{
	MET.setMouseUpPos(x, y);
	MET.normalizationZerosMinMap(Rect(0, 0, mapSize.width, mapSize.width));
	zerosMinMap = MET.zerosMinMap;
}

void AutomaticTrackingMap::setMouseMovePos(int x, int y)
{
	MET.setMouseMovePos(x, y);
	MET.normalizationZerosMinMap(Rect(0,0,mapSize.width,mapSize.width));
	zerosMinMap = MET.zerosMinMap;
	
}

void AutomaticTrackingMap::setAutoMode()
{
	isAutoMode = !isAutoMode;
}

bool AutomaticTrackingMap::getAutoMode()
{
	return isAutoMode;
}

int AutomaticTrackingMap::getThisState()
{
	//����״̬���Ա����Ƿ���������״̬���ã���ֹ��������ԭ�񴰿ڣ���꽹���޷�ת�ơ�
	thisStateModeNext = thisStateMode;
	//����ԭ��״̬�ж���һ֡ʱ������״̬
	if (GIS.isRunning)
	{
		if (isAutoMode)
		{
			if (GIS.isPaimonVisible)
			{
				thisStateModeNext = ThisWinState::TopShow;
			}
			else
			{
				thisStateModeNext = ThisWinState::Minimize;
			}
		}
		else
		{
			thisStateModeNext = ThisWinState::Top;
		}
	}
	else
	{
		thisStateModeNext = ThisWinState::Normal;
	}
	//���״̬����һ֡��ͬ��������״̬����������
	if (isFristChangeThisState||thisStateModeNext != thisStateMode)
	{
		thisStateMode = thisStateModeNext;
		isFristChangeThisState = true;
	}
	else
	{
		thisStateModeNext = ThisWinState::Wait;
	}

	return thisStateMode;
}

void AutomaticTrackingMap::setThisState()
{
	//if (thisStateModeNext == ThisWinState::Wait)return;
	if (isFristChangeThisState != true)
	{
		return;
	}
	else
	{
		isFristChangeThisState = false;
	}
	switch (thisStateMode)
	{
		case ThisWinState::Normal:
		{
			setThisState_Normal();
			break;
		}
		case ThisWinState::Minimize:
		{
			setThisState_Minimize();
			break;
		}
		case ThisWinState::Top:
		{
			//���ñ����ö���ǰ����״̬
			setThisState_Top();
			break;
		}
		case ThisWinState::TopShow:
		{
			//���ñ����ö���ǰ����״̬
			setThisState_TopShow();
			break;
		}
		default:
		{
			setThisState_Normal();
			break;
		}
	}
}

void AutomaticTrackingMap::setThreadMatchMat()
{
	if (GIS.isRunning)
	{
		Mat matRGB2GRAY;
		cvtColor(GIS.giFrameMap, matRGB2GRAY, CV_RGB2GRAY);
		TMS.getObjMinMap(matRGB2GRAY);
		cvtColor(GIS.giFramePaimon, matRGB2GRAY, CV_RGB2GRAY);
		TMS.getObjPaimon(matRGB2GRAY);
		cvtColor(GIS.giFrameUID, matRGB2GRAY, CV_RGB2GRAY);
		TMS.getObjUID(matRGB2GRAY);
	}
	else
	{
		TMS.isExistObjMinMap = false;
		TMS.isExistObjPaimon = false;
		TMS.isExistObjUID = false;
	}
}
