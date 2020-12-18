#include<iostream>
#include<Windows.h>
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include<opencv2/xfeatures2d.hpp>
#include<opencv2/xfeatures2d/nonfree.hpp>

using namespace cv;
using namespace std;

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
		AutoTest AT;
	private:
		//˽�б���
		bool isInit = false;
		bool isRun = false;

		HWND handle;

		bool giIsRunningFlag = false;
		bool giIsDisplayFlag = false;
		bool giIsZoomedFlag = false;

		Size autoMapSize=Size(250,200);
		Mat autoMapMat;
		String autoMapWindowsName = "Genshin Impact Auto Map";
	public:
		//����ຯ��
		bool init();
		bool run();
		bool exit();
	public:
		//ʵ���ຯ��

	private:
		//����ʵ�ֺ���
		void giIsRunning();
		void giIsDisplay();
		void giIsZoomed();
		void giCheckWindows();
		void mapShow();

	};

	typedef GenshinImpact_AutoMap giAM;
}


