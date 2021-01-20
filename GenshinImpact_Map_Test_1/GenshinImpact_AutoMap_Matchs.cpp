#include "GenshinImpact_AutoMap_Matchs.h"

GenshinImpact_AutoMap_Matchs::GenshinImpact_AutoMap_Matchs()
{

}

GenshinImpact_AutoMap_Matchs::~GenshinImpact_AutoMap_Matchs()
{
}

GenshinImpact_AutoMap_Matchs::GenshinImpact_AutoMap_Matchs(Mat _target)
{
	setTarget(_target);


	//orb = ORB::create(10000, 1.6, 8, 31, 0, 2, ORB::HARRIS_SCORE, 31, 20);
	//ͨ��ORB�㷨�������ͼ���е������㣬��������ԵĶ�ֵ������
	//orb->detectAndCompute(target, Mat(), keyPoints_test, descriptors_test, false);

#if 0

	namedWindow("2", 256);
	imshow("2", descriptors_scene);
#endif
}

void GenshinImpact_AutoMap_Matchs::init()
{
	if (isInit) return;

	detector = cv::xfeatures2d::SURF::create(minHessian);

	detector->detectAndCompute(target, noArray(), keypoints_scene, descriptors_scene);

	isInit = true;
}

void GenshinImpact_AutoMap_Matchs::setMode(int flag)
{
	mode = flag;
}

void GenshinImpact_AutoMap_Matchs::setObject(Mat img)
{
	object = img;
	isObjectExist = true;
}

void GenshinImpact_AutoMap_Matchs::setTarget(Mat img)
{
	target = img;
	isTargetExist = true;
}

void GenshinImpact_AutoMap_Matchs::getKeyPoints()
{
	getObjectKeyPoints();
	getTargetKeyPoints();
}

void GenshinImpact_AutoMap_Matchs::onMatch()
{
}

Point GenshinImpact_AutoMap_Matchs::getLocation()
{
	return p;
}

void GenshinImpact_AutoMap_Matchs::testSURF()
{
	//surf
	Mat img_scene = target;
	Mat img_object = object;

	if (isCout)
	{
		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		cout << "SURF Start time:" << t << "s" << endl;
		t = (double)cv::getTickCount();
	}

	detector->detectAndCompute(img_object, noArray(), keypoints_object, descriptors_object);


	//-- Step 2: Matching descriptor vectors with a FLANN based matcher
	// Since SURF is a floating-point descriptor NORM_L2 is used
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	std::vector< std::vector<DMatch> > knn_matches;
	matcher->knnMatch(descriptors_object, descriptors_scene, knn_matches, 2);



	//-- Filter matches using the Lowe's ratio test
	//const float ratio_thresh = 0.8f;
	std::vector<DMatch> good_matches;
	for (size_t i = 0; i < knn_matches.size(); i++)
	{
		if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
		{
			good_matches.push_back(knn_matches[i][0]);

		}
	}
	int k = (int)good_matches.size();
	static double x = 0;
	static double y = 0;
	for (size_t i = 0; i < k; i++)
	{
		//size/2-obj+sce
		if (i == 0) 
		{
			x = 0;
			y = 0;
		}
		x = x + (92.5 - keypoints_object[good_matches[i].queryIdx].pt.x + keypoints_scene[good_matches[i].trainIdx].pt.x) / k;
		y = y + (92.5 - keypoints_object[good_matches[i].queryIdx].pt.y + keypoints_scene[good_matches[i].trainIdx].pt.y) / k;
	}
	if (isCout)
	{
		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		cout << "SURF End time:" << t << "s" << endl;
		t = (double)cv::getTickCount();
	}
	p = Point((int)x, (int)y);
	//-- Draw matches

	//Mat img_matches;
	//drawMatches(img_object, keypoints_object, target, keypoints_scene, good_matches, img_matches, Scalar::all(-1),
	//	Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	//namedWindow("1", WINDOW_FREERATIO);
	//imshow("1", img_matches);
}

void GenshinImpact_AutoMap_Matchs::test()
{
	testSURF();
	//testORB();
}

void GenshinImpact_AutoMap_Matchs::testORB()
{
	//ORB ��̫�У�ƥ�䲻��
	Mat tem = object;
	//flip(tem, tem, 0);
	//resize(tem, tem, Size(), 1.5, 1.5);
	Mat test = target;

	if (isCout)
	{
		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		cout << "ORB Start time:" << t << "s" << endl;
		t = (double)cv::getTickCount();
	}


	//FAST_SCORE

	orb->detectAndCompute(tem, Mat(), keyPoints_tem, descriptors_tem, false);
	if (isCout)
	{
		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		cout << "ORB detectAndCompute time:" << t << "s" << endl;
		t = (double)cv::getTickCount();
	}

	//����ƥ����ͨ��ʹ�ú��ʵ����ƶȶ����Ƚ�������������ִ�еġ�
	//��������������ƥ����
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::MatcherType::BRUTEFORCE);
	//����MatcherType��ƥ�������ͣ�����ʹ��MatcherType::BRUTEFORCE������ƥ���㷨��

	vector<DMatch> matches;
	//ͨ��������ƥ������������ͼ��������ӽ���ƥ�䣬Ҳ���ǽ�����ͼ���еĶ�Ӧ���������ƥ�䣻�������һ��DMatch�ṹ����������ÿһ��DMatch�ṹ�����һ���Ӧ���������Ϣ��
	matcher->match(descriptors_tem, descriptors_test, matches, Mat());


	if (isCout)
	{
		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		cout << "ORB BRUTEFORCE time:" << t << "s" << endl;
		t = (double)cv::getTickCount();
	}

	float maxdist = 0;
	for (int i = 0; i < matches.size(); i++)
	{
		//Ѱ��ƥ�����������ƥ���������ĵ�ԣ�Ҳ����ƥ�������Զ�ĵ�ԣ���ȡ��������ֵ
		maxdist = max(maxdist, matches[i].distance);
	}

	vector<DMatch> good_matches;
	for (int j = 0; j < matches.size(); j++)
	{
		//���ƥ����������У�ĳ����Ե�ƥ�����С��ĳ����ֵ��������������ֵ����һ��С��1��ϵ�������������Ϊ�Ǹ߶�ƥ����������
		if (matches[j].distance < 0.7 * maxdist)
		{
			good_matches.push_back(matches[j]);
		}
	}

	if (isCout)
	{
		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		cout << "ORB End time:" << t << "s" << endl;
		t = (double)cv::getTickCount();
	}

	//������ͼ��֮��ĸ߶�ƥ��Ķ�Ӧ������ʹ�����߻��Ƴ��������һ��������ͼ��ƴ�������ٽ������ߵ�ͼ��
	//Scalar::all(-1)��ѡ�������ɫ
	Mat result;
	//drawMatches(tem, keyPoints_tem, test, keyPoints_test, good_matches, result, Scalar::all(-1), Scalar::all(-1));
	drawMatches(tem, keyPoints_tem, test, keyPoints_test, good_matches, result, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	namedWindow("1", WINDOW_FREERATIO);
	imshow("1", result);
}

bool GenshinImpact_AutoMap_Matchs::keySave()
{
	//descriptors_scene keypoints_scene
	return false;
}

bool GenshinImpact_AutoMap_Matchs::keyLoad()
{
	return false;
}

void GenshinImpact_AutoMap_Matchs::setCout(bool _isCout)
{
	isCout = _isCout;
}

void GenshinImpact_AutoMap_Matchs::getObjectKeyPoints()
{

}

void GenshinImpact_AutoMap_Matchs::getTargetKeyPoints()
{
	if (!isTargetExist)return;
	switch (mode)
	{
	case 0:
	{
		detector = cv::xfeatures2d::SURF::create(minHessian);
		//detector->detectAndCompute(target, noArray(), keypoints_target, descriptors_target);

		break;
	}
	default:
		break;
	}
}
