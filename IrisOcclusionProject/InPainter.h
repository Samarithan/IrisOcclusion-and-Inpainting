#pragma once
#include "Feature.h"

class InPainter
{
private:
	int RADIUS, BORDER_RADIUS;
	float alfa, beta, omega, errorInterval;
	cv::Mat image, mask;
	cv::Mat grayImage;
	cv::Mat confidenceMat;
	cv::Mat priorityMat;
	cv::Point maxPriorityPoint;
	cv::Point bestMatchedPoint;
	Feature * featureUsed;
	cv::Mat maxPriorityPatch;
	cv::Mat maxPriorityPatchConfidence;
	cv::Mat templateMask;
	cv::Mat result;
	cv::Mat erodedMask;
	cv::Mat irisContour;
	Operations *operationObj;

	double confidence;

public:
	InPainter(int RADIUS, int BORDER_RADIUS, float alfa, float beta, float omega, float errorInterv,Feature * object);

	void initialise(cv :: Mat& image, cv :: Mat& mask,cv ::  Mat& confidenceMat,cv:: Mat &grayImage,cv :: Mat &priorityMat,cv:: Mat &iris_contour);
	
	cv::Mat  InPainter::computeLBPMaskAndAvailablePointsMatrix(cv::Mat img, cv::Mat &availablePoint, cv::Mat iris_contour, cv::Mat mask);

	int mod(int a, int b);

	void transferPatch(cv::Point bestMatch, cv::Point bestPriority, cv::Mat &mat, const cv::Mat &maskMat, int RADIUS);

	void getDerivatives(cv::Mat &mat, cv::Mat &dx, cv::Mat& dy);

	float computeConfidence(cv::Mat & confidencePatch);

	cv::Point2f getNormal(vector<cv::Point> contour, const cv::Point& point, int BORDER_RADIUS);

	void computePriorities(cv::Mat &priorityMat, cv::Mat &confidenceMat, cv::Mat grayMat, vector<vector<cv::Point>>contours, float alfa, float omega, int RADIUS, int BORDER_RADIUS);

	void getContours(const cv :: Mat &mat, vector<vector<cv :: Point>>&contours, vector<cv :: Vec4i>&hierarchy);
	
	cv::Point getBestPoint(vector<double>patchHistogram, map< pair<int, int>, vector<double>>histogramSet);

	double computeDistance(vector<double>A, vector<double>B);

	cv::Mat doInPaint(cv::Mat image, cv::Mat mask,cv::Mat irisContour);

	~InPainter();
};

