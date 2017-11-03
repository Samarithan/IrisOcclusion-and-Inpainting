#include "stdafx.h"
#include "Lbp16.h"


Lbp16::Lbp16()
{

}


void  Lbp16::getInfo()
{
	cout << "We are using LBP16 feature";
}

vector<double> Lbp16:: computeHistogram(cv::Mat lbpSubPatch)
{	
	vector<double>histogram;
	

	for (int i = 0; i < 16; i++)
		histogram.push_back(0);

	for (int i = 0; i<lbpSubPatch.rows; i++)
		for (int j = 0; j < lbpSubPatch.cols; j++)
		{
			int value = lbpSubPatch.at<uchar>(i, j);
			int interval = value / 16;
			histogram[interval] ++;
		}
	
	for (int i = 0; i < histogram.size(); i++)
	{
		histogram[i] = histogram[i] / float(lbpSubPatch.cols * lbpSubPatch.rows);
	}
	return histogram;
}


Lbp16::~Lbp16()
{
}
