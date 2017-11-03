#include "stdafx.h"
#include "LbpUniform.h"


void  LbpUniform::getInfo()
{
	cout << "We are using LBP Uniform feature";
}

LbpUniform::LbpUniform()
{
	int k = -1;
	for (int i = 0; i <= 255; i++)
	{
		int sw = 0;
		for (int j = 0; j < 58; j++)//daca i este uniform
		{
			if (i == uniformLBPValues[j])
			{
				sw = 1;
				break;
			}
		}

		if (sw == 1)//daca i este uniform
		{
			k++;
			uniformMap[i] = k;
		}
		else
			uniformMap[i] = 58;
	}
}


vector<double> LbpUniform :: computeHistogram(cv::Mat lbpSubPatch)
{	
	vector<double>histogram;


	//calculez histograma propriu-zisa
	for (int i = 0; i < 59; i++)
		histogram.push_back(0);

	for (int i = 0; i<lbpSubPatch.rows; i++)
		for (int j = 0; j < lbpSubPatch.cols; j++)
		{
			int value = lbpSubPatch.at<uchar>(i, j);
			histogram[uniformMap[value]] ++;
		}


	for (int i = 0; i < histogram.size(); i++)
	{
		histogram[i] = histogram[i] / float(lbpSubPatch.cols * lbpSubPatch.rows) * 1.0;
	}


	return histogram;

}


LbpUniform::~LbpUniform()
{
}
