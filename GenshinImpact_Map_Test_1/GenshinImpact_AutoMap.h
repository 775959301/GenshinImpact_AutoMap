#include<iostream>
#include<Windows.h>
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include<opencv2/xfeatures2d.hpp>
#include<opencv2/xfeatures2d/nonfree.hpp>

#include "GenshinImpact_AutoMap_Objects.h"

using namespace cv;
using namespace std;

//�Զ�����
class AutoTest
{
public:
	AutoTest() :AutoTest(false) {};
	~AutoTest() {};
public:
	AutoTest(bool flag) :state(flag) {};
	bool operator==(const AutoTest& AT) { return AT.state; };
	operator bool() { return state; };
public:
	//��������
	bool state=false;
private:
	//˽�б���
public:
	//����ຯ��
	void on() { state = true; };
	void off() { state = false; };

};

//֡������
class FrameRateLock
{
public:
	FrameRateLock():FrameRateLock(30){};
	~FrameRateLock() {};
public:
	FrameRateLock(int f) :_fps(f){};
public:
	//��������
	double runningTime=0;
	double absClock0 = (double)getTickCount();
	double absClock1 = (double)getTickCount();
private:
	//˽�б���
	int _fps = 30;
	double _s = 0.0323f;
public:
	//����ຯ��
	void fps(int f) { _fps = f; _s = 1.0 / _fps - 0.001; };
	int fps() { return _fps; };
	void Wait()
	{
		absClock1 = (double)getTickCount();
		runningTime = (absClock1 - absClock0) / getTickFrequency();
		if (runningTime < _s)
		{
			if (runningTime >= 0)
			{
				waitKey((int)((_s - runningTime) * 1000) + 1);
			}
			else
			{
				waitKey((int)(_s * 1000));
			}
		}
		absClock0 = absClock1;
	};

};

class MessBox
{

};

class Popup
{
public:
	Popup():Popup(""){};
	~Popup() {};
public:
	Popup(std::string text) :Popup("", text) {};
	Popup(std::string title,std::string text):Popup(title,text,false){};
	Popup(std::string title, std::string text,bool isReturn) {};
	

};

namespace giam
{
	class GenshinImpact_AutoMap
	{
	public:
		//��������������
		GenshinImpact_AutoMap();
		~GenshinImpact_AutoMap();
	public:
		//��������

		//������
		//AutoTest AT;

		//ͼ�����֡������
		FrameRateLock FRL;

	private:
		//˽�б���

		//�Ƿ��ʼ��
		bool isInit = true;
		//�Ƿ�����
		bool isRun = false;
		//ԭ����Ϸ���ھ��
		HWND giHandle;
		//�Զ����������
		HWND thisHandle;
		//ԭ�񴰿�����
		RECT giRect;
		//ԭ�񴰿����򱣴�Ա���
		RECT giRectTmp;

		//ԭ���Ƿ��������б�־
		bool giIsRunningFlag = false;
		//ԭ���Ƿ�ɼ���־
		bool giIsDisplayFlag = false;
		//ԭ���Ƿ���󻯱�־
		bool giIsZoomedFlag = false;

		//HUD����
		struct HUD
		{
			
			String runState = "";
			Scalar runTextColor=Scalar(255,0,0);
			Scalar displayFlagColor = Scalar(255, 0, 0);
			Scalar minBackgroundColo = Scalar(255, 255, 0, 0);
			Scalar minStarColor = Scalar(255, 255, 0, 128);

		}giHUD;

		//���ص����ݲ���
		struct MEF
		{
			int x0 = 0;
			int y0 = 0;
			int dx = 0;
			int dy = 0;
			int x1 = 0;
			int y1 = 0;
			Point p0 = Point(0, 0);
			Point p1 = Point(0, 0);
			double value = 0.0;
			double scale = 1.0;

			bool bLCD = false;
			bool bLCU = false;
		}giMEF;

		//��ͼ��Ƕ�Ӧͼ��
		struct TAB
		{
			//string pngA = "f_t_c_1_1.png";
			Mat pngA = imread("C:/Users/GengG/source/repos/GenshinImpact_AutoMap/GenshinImpact_Map_Test_1/f_t_c_1_1.png", IMREAD_UNCHANGED);
			Mat pngAMask = imread("C:/Users/GengG/source/repos/GenshinImpact_AutoMap/GenshinImpact_Map_Test_1/f_t_c_1_1_mask.bmp", IMREAD_UNCHANGED);
			Mat pngB = imread("C:/Users/GengG/source/repos/GenshinImpact_AutoMap/GenshinImpact_Map_Test_1/f_t_c_1_1.png", IMREAD_UNCHANGED);
			Mat pngC = imread("C:/Users/GengG/source/repos/GenshinImpact_AutoMap/GenshinImpact_Map_Test_1/f_t_c_1_1.png", IMREAD_UNCHANGED);
			Mat sysIcon1 = imread("C:/Users/GengG/source/repos/GenshinImpact_AutoMap/GenshinImpact_Map_Test_1/sysIcon_1.png", IMREAD_UNCHANGED);
			Mat sysIcon1Mask = imread("C:/Users/GengG/source/repos/GenshinImpact_AutoMap/GenshinImpact_Map_Test_1/sysIcon_1_mask.bmp", IMREAD_UNCHANGED);
			Mat sysIcon2 = imread("C:/Users/GengG/source/repos/GenshinImpact_AutoMap/GenshinImpact_Map_Test_1/sysIcon_2.png", IMREAD_UNCHANGED);
			Mat sysIcon2Mask = imread("C:/Users/GengG/source/repos/GenshinImpact_AutoMap/GenshinImpact_Map_Test_1/sysIcon_2_mask.bmp", IMREAD_UNCHANGED);
			Rect pngARect = Rect(30, 0, pngA.cols, pngA.rows);
		}giTab;

		//��ͼ�����ؼ�¼
		struct FLAG
		{
			bool isUpdata = true;
			int max;
			bool *isShow;
			int numFlag[3] = {73,10,19};//[66,131,80];

			FLAG() :FLAG(3) {};
			FLAG(int n) 
			{
				max = n;
				isShow = new bool[n] { false };

			}
			~FLAG() { delete isShow; };

		}giFlag;

		struct Location2d
		{
			int x;
			int y;
			Location2d() :Location2d(0, 0) {}
			Location2d(int _x, int _y) :x(_x), y(_y) {}
		};

		struct OBJECT
		{
			string name;
			unsigned char klass;
			int orderNum;
			Location2d p;
			OBJECT(){}
			OBJECT(string _name, unsigned char _klass, int _orderNum, int _x, int _y) :OBJECT(_name, _klass, _orderNum, Location2d(_x, _y)) {}
			OBJECT(string _name, unsigned char _klass, int _orderNum, Location2d _p) :name(_name), klass(_klass), orderNum(_orderNum), p(_p) {}
		};
		struct OBJECTS :OBJECT
		{

		};

		struct FLAG_ICON_T_1
		{

		};

		struct obj
		{
			giAMO obj1;
			obj()
			{
				obj1.initCSD();
			}
		}OBJ;

		//������ͼԴ����
		//Mat mapMatSource = imread("Map.png", IMREAD_UNCHANGED);
		//������ͼ Ӧ��
		Mat mapMat = imread("C:/Users/GengG/source/repos/GenshinImpact_AutoMap/GenshinImpact_Map_Test_1/Map.png", IMREAD_UNCHANGED);
		//��������ʾ����
		Mat autoMapMat;
		//���������ڱ���
		String autoMapWindowsName = "Genshin Impact Auto Map";
		//��������С
		Size autoMapSize = Size(250, 200);
		//������ͼ��С
		Size mapSize= Size(mapMat.cols, mapMat.rows);
		//�������������Դ��ͼλ��
		Point zerosMinMap = Point(1466,3272);
		//���������ԭ�񴰿�λ��
		Point offGiMinMap = Point(250, 100);
		//���������ԭ�񴰿�λ�öԱ���
		Point offGiMinMapTmp = Point(250, 100);
		//
		Rect minMapRect;
		//ԭ����Ϸ���ڽ�ͼ
		Mat giFrame;

	public:
		//����ຯ��

		//��ʼ��
		bool init();
		//����
		bool run();
		//�˳�
		bool exit();
	public:
		//ʵ���ຯ��

		//�Ӵ��ͼ�н�ȡ��ʾ����
		Mat getMinMap();
		//�ж�RECT�Ƿ����
		bool isEqual(RECT &r1, RECT &r2);
		bool isContains(RECT &r, Point &p);
		bool isContains(Rect &r, Point &p);

	private:
		//����ʵ�ֺ���

		//ԭ���Ƿ�����
		void giIsRunning();
		//ԭ���Ƿ�ɼ�
		void giIsDisplay();
		//ԭ���Ƿ����
		void giIsZoomed();
		//���ԭ�񴰿�״̬
		void giCheckWindows();
		//��ȡԭ����
		void giGetScreen();

		//����HUD
		void setHUD();
		//����HUD
		void addHUD(Mat img);
		//���ñ���Ƿ���ʾ
		void setFLAG();
		//�ڵ�ͼ�ϻ��Ʊ��
		void addFLAG(Mat img);

		//������
		void customProcess();

		//��ͼ����״̬����
		void mapUpdata();
		//��ͼ��ʾˢ��
		void mapShow();

		//���ص�
		static void on_MouseHandle(int event, int x, int y, int flags, void *parm);
	};

	typedef GenshinImpact_AutoMap giAM;
}


