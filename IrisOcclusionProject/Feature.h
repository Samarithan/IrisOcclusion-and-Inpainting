#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "Operations.h"

#include<vector>
#include<map>

using namespace std;


class Feature
{
	private:

public:

	Feature();
	virtual void getInfo() = 0;
	virtual vector<double>computeHistogram(cv::Mat lbpSubPatch) = 0;

	map<pair<int, int>, vector<double>>computeAllHistograms(cv::Mat lbpMask, cv::Mat availableMatrix,int RADIUS);

	cv::Mat computeLBPMask(cv::Mat img);

	~Feature();
};



