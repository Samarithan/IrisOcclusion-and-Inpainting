#pragma once
#include "Feature.h"

class Lbp16 : public Feature
{
public:
	Lbp16();
	vector<double>computeHistogram(cv::Mat lbpSubPatch);
	void getInfo();
	~Lbp16();
};

