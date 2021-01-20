#include "GenshinImpact_AutoMap.h"

giam::GenshinImpact_AutoMap::GenshinImpact_AutoMap()
{
	//init();
	cvtColor(matMap, matMatchMap, CV_RGB2GRAY);
	giMatch = giAMM(matMatchMap);

}

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
	thisHandle = FindWindowA(NULL, autoMapWindowsName.c_str());
	//��ʼ����ͼ
	imshow(autoMapWindowsName, autoMapMat);
	//��ȡԭ�񴰿�״̬
	giCheckWindows();
	//���ȫ����������������ʼλ��
	if (giIsFullScreenFlag)offGiMinMap = Point(288, 82);
	//���ô���λ��
	SetWindowPos(thisHandle, HWND_TOPMOST, giRect.left + offGiMinMap.x, giRect.top + offGiMinMap.y, autoMapSize.width, autoMapSize.height, SWP_NOMOVE);
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
		//��ͼͼ�����ֵȸ���
		mapUpdata();

		customProcess();
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
	static Mat minMap;

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
	minMapRect = Rect(minMapPoint, reMapSize);

	resize(matMap(minMapRect), minMap, autoMapSize);
	//minMap = matMap(Rect(minMapPoint, reMapSize));

	return minMap;
}

//�ж�RECT�Ƿ����
bool giam::GenshinImpact_AutoMap::isEqual(RECT & r1, RECT & r2)
{
	if (r1.bottom != r2.bottom || r1.left != r2.left || r1.right != r2.right || r1.top != r2.top)
	{
		return false;
	}
	else
	{
		return true;
	}
	return false;
}

//�ж�RECT�Ƿ����Point
bool giam::GenshinImpact_AutoMap::isContains(RECT & r, Point & p)
{
	if (p.x<r.left || p.x>r.right||p.y<r.top||p.y>r.bottom)
	{
		return false;
	}
	else
	{
		return true;
	}

	return false;
}

//�ж�Rect�Ƿ����Point
bool giam::GenshinImpact_AutoMap::isContains(Rect & r, Point & p)
{
	if (p.x<r.x || p.x>(r.x+r.width) || p.y<r.y || p.y>(r.y+r.height))
	{
		return false;
	}
	else
	{
		return true;
	}
	return false;
}

//ԭ���Ƿ�����
void giam::GenshinImpact_AutoMap::giIsRunning()
{
	//���Ի�ȡԭ����
	giHandle = FindWindowA(NULL, "ԭ��");
	if (giHandle != NULL)
	{
		giIsRunningFlag = true;
		return;
	}
	giIsRunningFlag= false;
}

//ԭ���Ƿ�ɼ�
void giam::GenshinImpact_AutoMap::giIsDisplay()
{
	if (giHandle != NULL && giIsRunningFlag)
	{
		giIsDisplayFlag = !IsIconic(giHandle);
		return;
	}
	giIsDisplayFlag = false;
}

//ԭ���Ƿ����
void giam::GenshinImpact_AutoMap::giIsZoomed()
{
	if (giHandle != NULL&& giIsDisplayFlag)
	{
		giGetScreen();
		giScreenROI();
		giGetPaimon();

		giIsZoomedFlag = IsZoomed(giHandle);
		//��ȡԭ�񴰿�����
		GetWindowRect(giHandle, &giRect);
		return;
	}
	giIsZoomedFlag = false;
}

//ԭ���Ƿ�ȫ��
void giam::GenshinImpact_AutoMap::giIsFullScreen()
{
	if (giHandle != NULL&& giIsDisplayFlag)
	{
		giIsPaimonVisible();

		static RECT rcDesk;
		GetWindowRect(GetDesktopWindow(), &rcDesk);
		if (giRect.left <= rcDesk.left&& giRect.top <= rcDesk.top&& giRect.right >= rcDesk.right&& giRect.bottom >= rcDesk.bottom)
		{
			giIsFullScreenFlag = true;
			return;
		}
	}
	giIsZoomedFlag = false;
}

void giam::GenshinImpact_AutoMap::giIsPaimonVisible()
{
	static Mat tmp;
	static Mat matPaimon;
	if (giIsFullScreenFlag)
	{
		matPaimon = matPaimon1;
	}
	else
	{
		matPaimon = matPaimon2;
	}
	cv::matchTemplate(matPaimon, giFramePaimon, tmp, cv::TM_CCOEFF_NORMED);

	double minVal, maxVal;
	cv::Point minLoc, maxLoc;
	//Ѱ�����ƥ��λ��
	cv::minMaxLoc(tmp, &minVal, &maxVal, &minLoc, &maxLoc);
	if (minVal + maxVal < 0.82)
	{
		if (giIsPaimonVisibleFlag)giFlag.isUpHUD = true;
		giIsPaimonVisibleFlag = false;

	}
	else
	{
		if (!giIsPaimonVisibleFlag)giFlag.isUpHUD = true;
		giIsPaimonVisibleFlag = true;
		giGetMap();
	}
}

//���ԭ�񴰿�״̬
void giam::GenshinImpact_AutoMap::giCheckWindows()
{
	giIsRunning();
	giIsDisplay();
	giIsZoomed();
	giIsFullScreen();
}

//��ȡԭ����
void giam::GenshinImpact_AutoMap::giGetScreen()
{
	
	static HBITMAP	hBmp;
	RECT rc;
	BITMAP bmp;

	DeleteObject(hBmp);

	if (giHandle == NULL)return;

	//��ȡĿ�����Ĵ��ڴ�СRECT
	GetWindowRect(giHandle, &rc);

	//��ȡĿ������DC
	HDC hScreen = GetDC(giHandle);
	HDC hCompDC = CreateCompatibleDC(hScreen);

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
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));
	
	GetBitmapBits(hBmp, bmp.bmHeight*bmp.bmWidth*nChannels, giFrame.data);
	
}

void giam::GenshinImpact_AutoMap::giScreenROI()
{
	if (giIsFullScreenFlag)
	{
		giFrameROI = giFrame(Rect(0, 0, 285, 240)); 
		//giFrame(Rect(0, 0, 285, 240)).copyTo(giFrameRect);
		cvtColor(giFrameROI, giFrameRect, CV_RGB2GRAY);
	}
	else
	{
		giFrameROI = giFrame(Rect(0, 0, 250, 210));
		//giFrame(Rect(0, 0, 250, 210)).copyTo(giFrameRect);
		cvtColor(giFrameROI, giFrameRect, CV_RGB2GRAY);
	}
}

void giam::GenshinImpact_AutoMap::giGetPaimon()
{
	//(18,8,48,55) 1366 768
	if (giIsFullScreenFlag)
	{
		giFrameRect(Rect(26, 12, 68, 77)).copyTo(giFramePaimon);
		//giFrameRect(Rect(0, 0, 94, 89)).copyTo(giFramePaimon);
	}
	else
	{
		giFrameRect(Rect(23, 11, 59, 67)).copyTo(giFramePaimon);
		//giFrameRect(Rect(0, 0, 82, 78)).copyTo(giFramePaimon);
	}
}

void giam::GenshinImpact_AutoMap::giGetMap()
{
	if (giIsFullScreenFlag)
	{
		giFrameRect(Rect(62, 19, 212, 212)).copyTo(giFrameMap);
	}
	else
	{
		giFrameRect(Rect(54, 17, 185, 185)).copyTo(giFrameMap);
	}
}

bool giam::GenshinImpact_AutoMap::thisIsIconic()
{
	thisIsIconicFlag = IsIconic(thisHandle);
	return thisIsIconicFlag;
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
	}
	if (giIsDisplayFlag)
	{
		giHUD.displayFlagColor = Scalar(0, 255, 0);
	}
	else
	{
		giHUD.displayFlagColor = Scalar(0, 0, 255);
	}
	if (giIsPaimonVisibleFlag)
	{
		giHUD.paimonFlagColor = Scalar(0, 255, 0);
	}
	else
	{
		giHUD.paimonFlagColor = Scalar(128, 128, 128);
	}
	if (giFlag.isAutoMove)
	{
		giHUD.autoMoveFlagColor = Scalar(0, 255, 0);
	}
	else
	{
		giHUD.autoMoveFlagColor = Scalar(128, 128, 128);
	}
}

//����HUD
void giam::GenshinImpact_AutoMap::addHUD(Mat img)
{
	Mat tmp;

	//���Ʊ�����
	Mat backRect(20, autoMapSize.width, CV_8UC4, Scalar(200, 200,200, 255));
	tmp = img(Rect(0,0, autoMapSize.width, 20));
	addWeighted(tmp, 0.6, backRect, 0.4, 0, tmp);
	
	//����Сͼ��
	Mat backgound;
	tmp = img(giTab.pngARect);//Rect(30, 0, giTab.pngA.cols, giTab.pngA.rows)
	tmp.copyTo(backgound);
	giTab.pngA.copyTo(tmp, giTab.pngAMask);
	if (!giFlag.isShow[0])
	{
		//Сͼ�������
		addWeighted(tmp, 0.5, backgound, 0.5, 0, tmp);
	}
	else
	{
		addWeighted(tmp, 0.7, backgound, 0.2, 0, tmp);
	}

	tmp = img(giTab.pngBRect);//Rect(30, 0, giTab.pngA.cols, giTab.pngA.rows)
	tmp.copyTo(backgound);
	giTab.pngB.copyTo(tmp, giTab.pngBMask);

	if (!giFlag.isShow[1])
	{
		//Сͼ�������
		addWeighted(tmp, 0.5, backgound, 0.5, 0, tmp);
	}
	else
	{
		addWeighted(tmp, 0.7, backgound, 0.2, 0, tmp);
	}

	tmp.release();
	tmp = img(Rect(autoMapSize.width - giTab.sysIcon1.cols , 0, giTab.sysIcon1.cols, giTab.sysIcon1.rows));
	tmp.copyTo(backgound);
	giTab.sysIcon1.copyTo(tmp, giTab.sysIcon1Mask);
	addWeighted(tmp, 0.5, backgound, 0.5, 0, tmp);

	tmp.release();



	//Բ����ʾԭ��״̬
	circle(img, Point(6, 10), 4, giHUD.paimonFlagColor, -1);
	circle(img, Point(16, 10), 4, giHUD.autoMoveFlagColor, -1);

	//putText(img, giHUD.runState, Point(24, 12), FONT_HERSHEY_COMPLEX_SMALL, 0.4, giHUD.runTextColor, 1);

	putText(img, to_string((int)(1.0/FRL.runningTime)), Point(10, 40), FONT_HERSHEY_COMPLEX_SMALL, 1, giHUD.runTextColor, 1);

}

//���ñ���Ƿ���ʾ
void giam::GenshinImpact_AutoMap::setFLAG()
{
	// mouse click change giFlag.isShow[] state

}

//�ڵ�ͼ�ϻ��Ʊ��
void giam::GenshinImpact_AutoMap::addFLAG(Mat img)
{
	//����FlagΪ��
	if (giFlag.isUpdata||giFlag.isGetMap)
	{
		for (int i = 0; i < min(giFlag.max, OBJ.num); i++)
		{
			//��ʾFlagΪ��
			if (giFlag.isShow[i])
			{
				for (int j = 0; j < OBJ.o[i].size(); j++)
				{

					Point p = Point(OBJ.o[i].at(j).x, OBJ.o[i].at(j).y);
					//Ŀ�����С��ͼ��ʾ������
					if (isContains(minMapRect, p))
					{
						Mat r;
						int x = (int)((p.x - minMapRect.x) / giMEF.scale);
						int y = (int)((p.y - minMapRect.y) / giMEF.scale);
						//��x��y���½�Ҫ���㹻�Ŀռ������ͼ��
						if (x + giTab.lis[i].cols > autoMapSize.width || y + giTab.lis[i].rows > autoMapSize.height)
						{
							;
						}
						else
						{
							r = img(Rect(x, y, giTab.lis[i].cols, giTab.lis[i].rows));
							giTab.lis[i].copyTo(r, giTab.lisMask[i]);
						}

					}

				}
			}
		}
		
		giFlag.isUpdata = false;
	}

}

//������
void giam::GenshinImpact_AutoMap::customProcess()
{
	_count++;
	//giTab.HBitmap2Mat(giTab.aa, giTab.png);
	//IplImage img=*giTab.hBitmap2Ipl(giTab.aa);
	//Mat iasd=Mat(img,0);
	
	static int k = 1;


	if (giIsPaimonVisibleFlag&&giFlag.isAutoMove)
	{
		if (k == 1)
		{
			//imwrite("output.png", giFrame);
			k = 0;
		}
		static Point tmp;
		giMatch.init();
		giMatch.setObject(giFrameMap);
		giMatch.test();
		zerosMinMap = giMatch.getLocation();
		//cout << zerosMinMap << endl;
		if (zerosMinMap != tmp)
		{
			tmp = zerosMinMap;
			giFlag.isGetMap = true;
		}
	}
}

//��ͼ����״̬����
void giam::GenshinImpact_AutoMap::mapUpdata()
{
	//������
	 static Mat tmpMap;

	//����ԭ�񴰿�״̬
	giCheckWindows();

	//��ȡԭ�񴰿ڽ���
	//giGetScreen();
	//giScreenROI();
	//giGetPaimon();
	//��ȡ��ͼ
	if (giFlag.isGetMap|| giFlag.isUpHUD)
	{
		getMinMap().copyTo(tmpMap);
		giFlag.isGetMap = false;
		giFlag.isUpHUD = true;
	}

	//������ʾ���
	setFLAG();
	addFLAG(tmpMap);

	//������ʾHUD
	setHUD();
	if (giFlag.isUpHUD)
	{
		addHUD(tmpMap);

		//���ӹ��õĻ��渳����ʾ����
		tmpMap.copyTo(autoMapMat);
		giFlag.isUpHUD = false;
		tmpMap.release();
	}

}

//��ͼ��ʾˢ��
void giam::GenshinImpact_AutoMap::mapShow()
{
	//������������ڲ����ڣ�ֹͣ���в��˳�
	if (!IsWindow(thisHandle))
	{
		isRun = false;
		return;
	}



	//������ڲ�������С��״̬���Ի�����и���
	if (!thisIsIconic())
	{
		imshow(autoMapWindowsName, autoMapMat);
		//���ƽ��������������ԭ�񴰿����ƶ�����������֮�ƶ�
		if (offGiMinMap != offGiMinMapTmp|| (!isEqual(giRect, giRectTmp)))
		{
			//���ڸ���ԭ��ƽ��
			//ƽ�ƴ��ڡ�giRect�����ԭ���Ѿ��˳�����ʹ�ùر�ǰ��λ��
			SetWindowPos(thisHandle, HWND_TOPMOST, giRect.left + offGiMinMap.x, giRect.top + offGiMinMap.y, 0, 0, SWP_NOSIZE);
			offGiMinMapTmp = offGiMinMap;
			giRectTmp = giRect;
		}
	}

	//ԭ���Ƿ�����
	if (giIsRunningFlag)
	{
		//�Ƿ��Զ�׷��
		switch (giFlag.isAutoMove)
		{
			case true:
			{
				//�Ƿ��ڿɼ�״̬
				switch (giIsDisplayFlag)
				{
					case true:
					{
						//������������������������С�����ָ�������
						if (giIsPaimonVisibleFlag && thisIsIconicFlag)
						{
							ShowWindow(thisHandle, SW_RESTORE);
							SetForegroundWindow(giHandle);
						}
						//����������������������������С������С��������
						if ((!giIsPaimonVisibleFlag) && (!thisIsIconicFlag))
						{
							ShowWindow(thisHandle, SW_MINIMIZE);
						}
						break;
					}
					case false:
					{
						//��������������С������С��������
						if (!thisIsIconicFlag)
						{
							ShowWindow(thisHandle, SW_MINIMIZE);
						}
						break;
					}
				}
				break;
			}
			case false:
			{
				//�Ƿ��ڿɼ�״̬
				switch (giIsDisplayFlag)
				{
					case true:
					{
						if (thisIsIconicFlag)
						{
							ShowWindow(thisHandle, SW_RESTORE);
							SetForegroundWindow(giHandle);
						}
						break;
					}
					case false:
					{
						if (!thisIsIconicFlag)
						{
							//���ڲ��ɼ�״̬����������������С��
							ShowWindow(thisHandle, SW_MINIMIZE);
						}
						break;
					}
				}

				break;
			}
		}
	}

	//�ȴ���ʾ����
	FRL.Wait();

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
		gi.giMEF.x1 = x;
		gi.giMEF.y1 = y;
		gi.giMEF.p1 = gi.offGiMinMap;//Point(gi.offGiMinMap.x - x, gi.offGiMinMap.y - y);
		break;
	}
	case EVENT_LBUTTONUP: 
	{
		for (int i = 0; i < gi.OBJ.num; i++)
		{
			Rect& r = gi.giTab.lisRect[i];

			if (x<r.x || x>(r.x + r.width) || y<r.y || y>(r.y + r.height))
			{

			}
			else
			{
				gi.giFlag.isShow[i] = !gi.giFlag.isShow[i];
				gi.giFlag.isUpdata = true;
				gi.giFlag.isGetMap = true;
			}
		}
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
		if (x > gi.autoMapSize.width - gi.giTab.sysIcon1.cols&&y <= gi.giTab.sysIcon1.rows)
		{
			gi.isRun = false;
		}
		else
		{
			gi.giFlag.isAutoMove = !gi.giFlag.isAutoMove;
			gi.giFlag.isUpdata = true;

		}
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
			if (gi.giMEF.scale < 6)
			{
				gi.giMEF.scale *= 1.2;
			}
		}
		else if (gi.giMEF.value > 0)
		{
			if (gi.giMEF.scale > 0.5)
			{
				gi.giMEF.scale /= 1.2;
			}
		}
		gi.giFlag.isGetMap = true;
		gi.giFlag.isUpdata = true;
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
		if (x > gi.autoMapSize.width - gi.giTab.sysIcon1.cols&&x < gi.autoMapSize.width && y < gi.giTab.sysIcon1.rows&&y>0)
		{
			//gi.giMEF.dx = x - gi.giMEF.x1;
			//gi.giMEF.dy = y - gi.giMEF.y1;
			//gi.offGiMinMap = gi.giMEF.p1 + Point(gi.giMEF.dx, gi.giMEF.dy);
		}
		else
		{
			gi.giMEF.dx = x - gi.giMEF.x0;
			gi.giMEF.dy = y - gi.giMEF.y0;
			gi.zerosMinMap = gi.giMEF.p0 - Point((int)(gi.giMEF.dx*gi.giMEF.scale), (int)(gi.giMEF.dy*gi.giMEF.scale));
			gi.giFlag.isUpdata = true;
			gi.giFlag.isGetMap = true;
		}

		break;
	}
	case EVENT_FLAG_RBUTTON:
	{
		break;
	}
	case EVENT_FLAG_MBUTTON:
	{
		//gi.giMEF.dx = x - gi.giMEF.x1;
		//gi.giMEF.dy = y - gi.giMEF.y1;
		gi.offGiMinMap = gi.giMEF.p1 + Point(x - gi.giMEF.x1, y - gi.giMEF.y1);

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
