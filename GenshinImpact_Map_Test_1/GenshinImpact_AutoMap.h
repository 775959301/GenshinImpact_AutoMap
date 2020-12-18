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
	bool t = true;
	bool f = false;
private:
	//˽�б���
public:
	//����ຯ��
	void on() { state = true; };
	void off() { state = false; };
	bool _is() { return true; };
	bool _isn() { return false; };
	bool s() { return state; };
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
		bool giIsRunning();
		bool giIsDisplay();
		bool giIsZoomed();
	private:
		//����ʵ�ֺ���
		void mapShow();

	};

	typedef GenshinImpact_AutoMap giAM;
}


