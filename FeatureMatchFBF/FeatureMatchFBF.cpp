#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

int main(int argc, char** argv)
{
	std::string video = argv[1];
	Ptr<SURF> detector = SURF::create();
	int minHessian = 400;
	detector->setHessianThreshold(minHessian);

	std::vector<KeyPoint> keypoints_1, keypoints_2;
	Mat descriptors_1, descriptors_2;
	Mat frame1, frame2;
	Mat frame_keypoints;
	VideoCapture cap(video);
	FlannBasedMatcher matcher;
	std::vector<DMatch> matches;
	std::vector<DMatch> good_matches;
	double max_dist = 0;
	double min_dist = 100;

	for (int i = 0; i < 60; i++)
	{
		cap >> frame1;
		cap >> frame2;

		detector->detectAndCompute(frame1, Mat(), keypoints_1, descriptors_1);
		detector->detectAndCompute(frame2, Mat(), keypoints_2, descriptors_2);
		matcher.match(descriptors_1, descriptors_2, matches);

		for (int j = 0; j < descriptors_1.rows; j++)
		{
			double dist = matches[j].distance;
			if (dist < min_dist)
				min_dist = dist;
			if (dist > max_dist)
				max_dist = dist;
		}

		for (int j = 0; j < descriptors_1.rows; j++)
		{
			if (matches[j].distance <= max(2 * min_dist, 0.02))
				good_matches.push_back(matches[j]);
		}

		drawKeypoints(frame2, keypoints_2, frame_keypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

		imshow("POI", frame_keypoints);

		if (waitKey(1) == 27)
			break;
	}
	return 0;
}

