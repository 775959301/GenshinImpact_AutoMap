#pragma once
#include<iostream>
#include<Windows.h>
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include<opencv2/xfeatures2d.hpp>
#include<opencv2/xfeatures2d/nonfree.hpp>

using namespace cv;
using namespace std;

class GenshinImpact_AutoMap_Matchs
{
public:
	GenshinImpact_AutoMap_Matchs();
	~GenshinImpact_AutoMap_Matchs();
public:
	GenshinImpact_AutoMap_Matchs(Mat _target);

public:
	void init();
	void setMode(int flag);

	//ƥ����󣬰���Ҫƥ��ĵ�ͼ��
	void setObject(Mat img);
	//ƥ��Ŀ�꣬�ڸ�ͼ����ƥ��
	void setTarget(Mat img);
	void getKeyPoints();
	void onMatch();
	Point getLocation();

	void testSURF();
	void test();
	void test2();
	void testORB();
	bool keySave();
	bool keyLoad();

	void setCout(bool _isCout);

private:
	bool isInit = false;
	bool isCout = false;
	bool isObjectExist = false;
	bool isTargetExist = false;
	Mat object;
	Mat target;
	//0:SURF
	int mode = 0;
	Point p;
	double t = 0;
	//int minHessian = 200;
	//float ratio_thresh = 0.7f;
	//Ptr<cv::xfeatures2d::SURF> detector;
	//std::vector<KeyPoint> keypoints_object;
	//std::vector<KeyPoint> keypoints_target;
	//Mat descriptors_object;
	//Mat descriptors_target;

	//vector<Point2d> off_obj;
	//vector<Point2d> off_sce;
	int minHessian = 400;
	float ratio_thresh = 0.66f;
	Ptr<cv::xfeatures2d::SURF> detector;
	std::vector<KeyPoint> keypoints_object, keypoints_scene;
	Mat descriptors_object, descriptors_scene;
	Ptr<ORB> orb;
	vector<KeyPoint> keyPoints_tem, keyPoints_test;
	Mat descriptors_tem, descriptors_test;
private:
	void getObjectKeyPoints();
	void getTargetKeyPoints();
};

typedef GenshinImpact_AutoMap_Matchs giAMM;
