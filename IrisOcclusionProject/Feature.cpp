#include "Feature.h"

Feature::Feature()
{
}

map<pair<int, int>, vector<double>> Feature :: computeAllHistograms(cv::Mat lbpMask, cv::Mat availableMatrix,int RADIUS)
{	
	Operations * operationObj = new Operations();

	map<pair<int, int>, vector<double>>histogramSet;
	int nr = 0;
	for (int i = 1; i < lbpMask.rows - 1; i += 3)
		for (int j = 1; j < lbpMask.cols - 1; j += 3)
		{			
			if (availableMatrix.at<float>(i, j) == 1)
			{
				nr++;
				pair<int, int>pct(i, j);
				cv::Mat patch = operationObj -> getPatch(lbpMask, cv::Point(i, j), RADIUS);
				histogramSet[pct] = computeHistogram(patch);
			}
		}

	return histogramSet;
}

cv::Mat Feature :: computeLBPMask(cv::Mat img)
{
	cv::Mat LBPMask(img.rows - 2, img.cols - 2, CV_8UC1);

	int line[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
	int col[8] = { -1,-1,-1, 0, 1, 1,  1,  0 };

	cvtColor(img, img, CV_BGR2GRAY);
	img.convertTo(img, CV_32FC1);

	int i, j;

	for (i = 1; i < img.rows - 1; i++)
		for (j = 1; j < img.cols - 1; j++)
		{
			string pattern;
			int ic, jc;
			for (int k = 0; k < 8; k++)
			{
				ic = i + line[k];
				jc = j + col[k];

				if (img.at<float>(i, j) < img.at<float>(ic, jc))
					pattern.push_back('0');
				else pattern.push_back('1');
			}
			int value = stoi(pattern, nullptr, 2);
			LBPMask.at<uchar>(i - 1, j - 1) = value;
		}
	return LBPMask;
}


Feature::~Feature()
{
}

