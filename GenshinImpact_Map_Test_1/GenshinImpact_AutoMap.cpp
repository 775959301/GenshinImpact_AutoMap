#include "GenshinImpact_AutoMap.h"

giam::GenshinImpact_AutoMap::GenshinImpact_AutoMap(){}

giam::GenshinImpact_AutoMap::~GenshinImpact_AutoMap(){}

//��ʼ��
bool giam::GenshinImpact_AutoMap::init()
{
	//AT.on();
	
	isInit = false;
	isRun = true;
	//��ʼ������
	autoMapMat = Mat(autoMapSize, CV_8UC4, Scalar(200, 200, 200, 255));
	//��������
	namedWindow(autoMapWindowsName);
	//���ûص�
	setMouseCallback(autoMapWindowsName, on_MouseHandle, (void*)this);
	//��ȡ���������
	thisHandle = FindWindow(NULL, autoMapWindowsName.c_str());
	//��ʼ����ͼ
	imshow(autoMapWindowsName, autoMapMat);
	//���ô���λ��
	SetWindowPos(thisHandle, HWND_TOPMOST, giRect.left + 250, giRect.top + 100, autoMapSize.width, autoMapSize.height, 0);
	//���ô���Ϊ�ޱ߿�
	SetWindowLong(thisHandle, GWL_STYLE, GetWindowLong(thisHandle, GWL_EXSTYLE | WS_EX_TOPMOST)); //�ı䴰�ڷ��
	ShowWindow(thisHandle, SW_SHOW);

	return false;
}

//����
bool giam::GenshinImpact_AutoMap::run()
{
	//���п��
	if (isInit) { init(); }
	//����ѭ��
	while (isRun)
	{
		//customProcess();
		//��ͼͼ�����ֵȸ���
		mapUpdata();
		//��ʾ
		mapShow();
	}
	//�˳�����
	exit();
	return false;
}

//�˳�
bool giam::GenshinImpact_AutoMap::exit()
{
	return false;
}

//�Ӵ��ͼ�н�ȡ��ʾ����
Mat giam::GenshinImpact_AutoMap::getMinMap()
{
	Mat minMap;

	Point minMapPoint = Point(0, 0);

	Size reMapSize = autoMapSize;
	reMapSize.width = (int)(reMapSize.width * giMEF.scale);
	reMapSize.height = (int)(reMapSize.height * giMEF.scale);

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
		minMapPoint.x = mapSize.width- reMapSize.width;
	}
	if (RB.y > mapSize.height)
	{
		minMapPoint.y = mapSize.height - reMapSize.height;
	}

	resize(mapMat(Rect(minMapPoint, reMapSize)), minMap, autoMapSize);
	//minMap = mapMat(Rect(minMapPoint, reMapSize));

	return minMap;
}

//ԭ���Ƿ�����
void giam::GenshinImpact_AutoMap::giIsRunning()
{
	if (AT)
	{
		//AutoTest Work Is On
		giIsRunningFlag = AT;
		return;
	}
	else
	{
		//
		giHandle = FindWindow(NULL, "ԭ��");
		if (giHandle != NULL)
		{
			giIsRunningFlag= true;
			return;
		}
	}
	giIsRunningFlag= false;
}

//ԭ���Ƿ�ɼ�
void giam::GenshinImpact_AutoMap::giIsDisplay()
{
	if (AT)
	{
		//AutoTest Work Is On
		giIsDisplayFlag = AT;
		return;
	}
	else
	{
		//
		if (giHandle != NULL)
		{
			giIsDisplayFlag= !IsIconic(giHandle);
			return;
		}
	}
	giIsDisplayFlag= false;
}

//ԭ���Ƿ����
void giam::GenshinImpact_AutoMap::giIsZoomed()
{
	if (AT)
	{
		//AutoTest Work Is On
		giIsZoomedFlag = !AT;
		return;
	}
	else
	{
		//
		if (giHandle != NULL)
		{
			//WindowFromPoint();
			giIsZoomedFlag=IsZoomed(giHandle);
			GetWindowRect(giHandle, &giRect);
			return;
		}
	}
	giIsZoomedFlag = false;
	return;
}

//���ԭ�񴰿�״̬
void giam::GenshinImpact_AutoMap::giCheckWindows()
{
	giIsRunning();
	giIsDisplay();
	giIsZoomed();
}

//��ȡԭ����
void giam::GenshinImpact_AutoMap::giGetScreen()
{
	HBITMAP	hBmp;
	RECT rc;
	BITMAP bmp;
	//��ȡĿ�����Ĵ��ڴ�СRECT
	GetWindowRect(giHandle, &rc);
	//��ȡĿ������DC
	HDC hScreen = GetDC(giHandle);
	HDC	hCompDC = CreateCompatibleDC(hScreen);
	//��ȡĿ�����Ŀ�Ⱥ͸߶�
	int	nWidth = rc.right - rc.left;
	int	nHeight = rc.bottom - rc.top;
	//����Bitmap����
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//�õ�λͼ
	SelectObject(hCompDC, hBmp); //��д��ȫ��
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);
	//�ͷŶ���
	DeleteDC(hScreen);
	DeleteDC(hCompDC);
	//����ת��
	GetObject(hBmp, sizeof(BITMAP), &bmp);
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
	//mat����
	giFrame.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));
	GetBitmapBits(hBmp, bmp.bmHeight*bmp.bmWidth*nChannels, giFrame.data);
}

//����HUD
void giam::GenshinImpact_AutoMap::setHUD()
{
	if (giIsRunningFlag)
	{
		giHUD.runState = "Genshin Impact Is Running";
		giHUD.runTextColor = Scalar(0, 255, 0);

	}
	else
	{
		giHUD.runState = "Genshin Impact Not Run";
		giHUD.runTextColor = Scalar(0, 0, 255);
	};
	if (giIsDisplayFlag)
	{
		giHUD.displayFlagColor = Scalar(0, 255, 0);
	}
	else
	{
		giHUD.displayFlagColor = Scalar(0, 0, 255);
	}

}

//����HUD
void giam::GenshinImpact_AutoMap::addHUD(Mat img)
{
	Mat blueRect(20, autoMapSize.width, CV_8UC4, Scalar(200, 200,200, 255));
	Mat tmp = img(Rect(0,0, autoMapSize.width, 20));
	addWeighted(tmp, 0.5, blueRect, 0.5, 0, tmp);

	//Mat star;
	//img(Rect(autoMapSize.width / 2 - 5, autoMapSize.height / 2 - 5, 11, 11)).copyTo(star);
	//tmp = img(Rect(autoMapSize.width / 2 - 5, autoMapSize.height / 2 - 5, 11, 11));
	//circle(star, Point(5, 5), 4, giHUD.minStarColor, 2, LINE_AA);
	//addWeighted(tmp, 0.5, star, 0.5, 0, tmp);

	Mat backgound;
	tmp = img(Rect(30, 0, giTab.pngA.cols, giTab.pngA.rows));
	tmp.copyTo(backgound);
	giTab.pngA.copyTo(tmp, giTab.pngAMask);

	if (!giFlag.isShow[0])
	{
		addWeighted(tmp, 0.5, backgound, 0.5, 0, tmp);

	}
	

	circle(img, Point(10, 10), 4, giHUD.displayFlagColor, -1);

	circle(img, Point(20, 10), 4, giHUD.runTextColor, -1);

	//putText(img, giHUD.runState, Point(24, 12), FONT_HERSHEY_COMPLEX_SMALL, 0.4, giHUD.runTextColor, 1);

	//putText(img, to_string(giMEF.scale), Point(30, 14), FONT_HERSHEY_COMPLEX_SMALL, 0.4, giHUD.runTextColor, 1);

}

//���ñ���Ƿ���ʾ
void giam::GenshinImpact_AutoMap::setFLAG()
{
	// mouse click change giFlag.isShow[] state

	/*Test*/
	//giFlag.isShow[0] = true;
}

//�ڵ�ͼ�ϻ��Ʊ��
void giam::GenshinImpact_AutoMap::addFLAG(Mat img)
{
	for (int i = 0; i < giFlag.max; i++)
	{
		if (giFlag.isShow[i])
		{
			for (int j = 0; j < giFlag.numFlag[i]; j++)
			{
				
			}
		}
	}
}

//������
void giam::GenshinImpact_AutoMap::customProcess()
{
	switch ((rand()%3))
	{
	case 0:
		zerosMinMap.x = zerosMinMap.x + 30;
		break;
	case 1:
		break;
	case 2:
		zerosMinMap.x = zerosMinMap.x - 30;
	default:
		break;
	}

	switch ((rand() % 3))
	{
	case 0:
		zerosMinMap.y = zerosMinMap.y + 30;
		break;
	case 1:
		break;
	case 2:
		zerosMinMap.y = zerosMinMap.y - 30;
	default:
		break;
	}
}

//��ͼ����״̬����
void giam::GenshinImpact_AutoMap::mapUpdata()
{
	Mat tmpMap;


	giCheckWindows();

	giGetScreen();

	//
	getMinMap().copyTo(tmpMap);
	setHUD();
	addHUD(tmpMap);

	setFLAG();
	addFLAG(tmpMap);

	autoMapMat = tmpMap;
	//tmpMap
}

//��ͼ��ʾˢ��
void giam::GenshinImpact_AutoMap::mapShow()
{
	if (IsWindow(thisHandle))
	{
		if (!IsIconic(thisHandle))
		{
			imshow(autoMapWindowsName, autoMapMat);
		}
		if (giIsRunningFlag)
		{
			if (giIsDisplayFlag)
			{
				if (IsIconic(thisHandle))
				{
					ShowWindow(thisHandle, SW_RESTORE);
					//ShowWindow(thisHandle, SW_MAXIMIZE);
				}
				SetWindowPos(thisHandle, HWND_TOPMOST, giRect.left + 250, giRect.top + 100, 0, 0, SWP_NOSIZE);
			}
			else
			{
				ShowWindow(thisHandle, SW_MINIMIZE);
			}
		}
		
		FRL.Wait();
	}
	else
	{
		isRun = false;
	}
}

//���ص�
void giam::GenshinImpact_AutoMap::on_MouseHandle(int event, int x, int y, int flags, void * parm)
{
	GenshinImpact_AutoMap& gi = *(giam::GenshinImpact_AutoMap*) parm;

	gi.giMEF.value = flags;

	switch (event)	
	{
	case EVENT_MOUSEMOVE: 
	{
		break;
	}
	case EVENT_LBUTTONDOWN: 
	{
		gi.giMEF.x0 = x;
		gi.giMEF.y0 = y;
		gi.giMEF.p0 = gi.zerosMinMap;
		break;
	}
	case EVENT_RBUTTONDOWN:
	{
		break;
	}
	case EVENT_MBUTTONDOWN: 
	{
		break;
	}
	case EVENT_LBUTTONUP: 
	{
		break;
	}
	case EVENT_RBUTTONUP: 
	{
		break;
	}
	case EVENT_MBUTTONUP: 
	{
		break;
	}
	case EVENT_LBUTTONDBLCLK: 
	{
		break;
	}
	case EVENT_RBUTTONDBLCLK: 
	{
		break;
	}
	case EVENT_MBUTTONDBLCLK:
	{
		break;
	}
	case EVENT_MOUSEWHEEL:
	{
		gi.giMEF.value = getMouseWheelDelta(flags);

			if (gi.giMEF.value < 0)
			{
				if(gi.giMEF.scale < 6)
				{
					gi.giMEF.scale *= 1.2;
				}
			}
			else if (gi.giMEF.value > 0)
			{
				if (gi.giMEF.scale >0.5)
				{
					gi.giMEF.scale /= 1.2;
				}
			}

		break;
	}
	case EVENT_MOUSEHWHEEL:
	{
		break;
	}
	default:
		break;
	}

	switch (flags)
	{
	case EVENT_FLAG_LBUTTON:
	{
		gi.giMEF.dx = x - gi.giMEF.x0;
		gi.giMEF.dy = y - gi.giMEF.y0;

		gi.zerosMinMap = gi.giMEF.p0 - Point((int)(gi.giMEF.dx*gi.giMEF.scale), (int)(gi.giMEF.dy*gi.giMEF.scale));
		break;
	}
	case EVENT_FLAG_RBUTTON:
	{
		break;
	}
	case EVENT_FLAG_MBUTTON:
	{
		break;
	}
	case EVENT_FLAG_CTRLKEY:
	{
		break;
	}
	case EVENT_FLAG_SHIFTKEY:
	{
		break;
	}
	case EVENT_FLAG_ALTKEY:
	{
		break;
	}
	default:
		break;
	}
}
