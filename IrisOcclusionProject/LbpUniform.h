#pragma once
#include "Feature.h"

class LbpUniform : public Feature
{	
private:
	int uniformLBPValues[59] = { 0,1,2,3,4,6,7,8,12,14,15,16,24,28,30,31,32,48,56,60,62,63,64,96,112,120,124,126,127,128,129,131,135,143,159,191,192,193,195,199,207,223,224,225,227,231,239,240,241,243,247,248,249,251,252,253,254,255 };
	map <int, int>uniformMap;

public:

	LbpUniform();
	void getInfo();
	vector<double>computeHistogram(cv::Mat lbpSubPatch);

	~LbpUniform();
};

