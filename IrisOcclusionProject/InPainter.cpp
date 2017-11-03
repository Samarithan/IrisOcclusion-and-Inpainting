#include "InPainter.h"


InPainter :: InPainter(int RADIUS, int BORDER_RADIUS, float alfa, float beta, float omega, float errorInterv,Feature * object)
{
	this->RADIUS = RADIUS;
	this->BORDER_RADIUS = BORDER_RADIUS;
	this->alfa = alfa;
	this->beta = beta;
	this->omega = omega;
	this->errorInterval = errorInterv;
	featureUsed = object;
	operationObj = new Operations();
}

void InPainter ::initialise(cv :: Mat& image,cv:: Mat& mask,cv:: Mat& confidenceMat,cv:: Mat &grayImage,cv:: Mat &priorityMat,cv:: Mat &iris_contour)
{
	image.convertTo(image, CV_32F);
	image /= 255.0f;

	copyMakeBorder(image, image, RADIUS, RADIUS, RADIUS, RADIUS, cv:: BORDER_CONSTANT,cv:: Scalar_<float>(0));
	copyMakeBorder(iris_contour, iris_contour, RADIUS, RADIUS, RADIUS, RADIUS, cv:: BORDER_CONSTANT, 1);

	cvtColor(image, grayImage, CV_BGR2GRAY);

	mask.convertTo(confidenceMat, CV_32F);
	confidenceMat /= 255.0f;

	copyMakeBorder(mask, mask, RADIUS, RADIUS, RADIUS, RADIUS,cv:: BORDER_CONSTANT, 255);

	copyMakeBorder(confidenceMat, confidenceMat, RADIUS, RADIUS, RADIUS, RADIUS, cv::BORDER_CONSTANT, 0.0001f);

	cv::Mat priority(confidenceMat.size(), CV_32FC1);
	priorityMat = priority;
}

cv::Mat  InPainter::computeLBPMaskAndAvailablePointsMatrix(cv::Mat img, cv::Mat &availablePoint, cv::Mat iris_contour, cv::Mat mask)
{
	cv::Mat LBPMask(img.rows, img.cols, CV_8UC1);

	int line[8] = { -1,0,1,1,1,0,-1,-1 };
	int col[8] = { -1,-1,-1,0,1,1,1,0 };

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

			if (i - RADIUS >= 0 && j - RADIUS >= 0 && i + RADIUS + 1 < img.rows && j + RADIUS + 1 < img.cols)
			{
				cv::Point stg_sus(i - RADIUS, j - RADIUS);
				cv::Point stg_jos(i + RADIUS + 1, j - RADIUS);
				cv::Point dr_sus(i - RADIUS, j + RADIUS + 1);
				cv::Point dr_jos(i + RADIUS + 1, j + RADIUS + 1);


				if (iris_contour.at<float>(stg_sus.x, stg_sus.y) == 0 && iris_contour.at<float>(stg_jos.x, stg_jos.y) == 0 &&
					iris_contour.at<float>(dr_sus.x, dr_sus.y) == 0 && iris_contour.at<float>(dr_jos.x, dr_jos.y) == 0 &&
					mask.at<float>(stg_sus.x, stg_sus.y) == 1 && mask.at<float>(stg_jos.x, stg_jos.y) == 1 &&
					mask.at<float>(dr_sus.x, dr_sus.y) == 1 && mask.at<float>(dr_jos.x, dr_jos.y) == 1)
						availablePoint.at<float>(i, j) = 1;
				
			}
		}

	return LBPMask;
}

int InPainter::mod(int a, int b)
{
	return ((a % b) + b) % b;
}


void InPainter :: getContours(const cv::Mat &mat, vector<vector<cv::Point>>&contours, vector<cv::Vec4i>&hierarchy)
{
	findContours(mat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
}

void InPainter :: getDerivatives(cv::Mat &mat, cv::Mat &dx,cv:: Mat& dy)
{
	Sobel(mat, dx, -1, 1, 0, -1);
	Sobel(mat, dy, -1, 0, 1, -1);
}

float InPainter :: computeConfidence(cv:: Mat & confidencePatch)
{
	return sum(confidencePatch)[0] / (float)confidencePatch.total();
}

cv:: Point2f InPainter::getNormal(vector<cv::Point> contour, const cv::Point& point, int BORDER_RADIUS)
{
	int length = (int)contour.size();

	cv::Mat X(cv::Size(2, 2 * BORDER_RADIUS + 1), CV_32F);
	cv::Mat Y(cv::Size(1, 2 * BORDER_RADIUS + 1), CV_32F);

	float* Xrow;
	float* Yrow;

	int count = 0;
	int countXequal = 0;

	int pointIndex = (int)(find(contour.begin(), contour.end(), point) - contour.begin());

	if (length == 1)
	{
		return cv::Point2f(1.0f, 0.0f);
	}

	else
		if (length < 2 * BORDER_RADIUS + 1)
		{
			cv::Point pct = contour[(pointIndex + 1) % length] - contour[pointIndex];
			return cv::Point2f(pct.y, -pct.x) / cv::norm(pct);
		}

	int i = mod((pointIndex - BORDER_RADIUS), length);

	while (count < 2 * BORDER_RADIUS + 1)
	{
		Xrow = X.ptr<float>(count);
		Xrow[0] = contour[i].x;
		Xrow[1] = 1.0f;

		Yrow = Y.ptr<float>(count);
		Yrow[0] = contour[i].y;

		if (Xrow[0] == contour[pointIndex].x)
		{
			countXequal++;
		}

		i = mod(i + 1, length);
		count++;
	}

	if (countXequal == count)
	{
		return cv::Point2f(1.0f, 0.0f);
	}

	cv::Mat sol;
	cv::solve(X, Y, sol, cv::DECOMP_SVD);

	float slope = sol.ptr<float>(0)[0];
	cv::Point2f normal(-slope, 1);

	return normal / cv::norm(normal);
}

void InPainter :: computePriorities(cv :: Mat &priorityMat, cv:: Mat &confidenceMat,cv::Mat grayMat, vector<vector<cv::Point>>contours, float alfa, float omega, int RADIUS, int BORDER_RADIUS)
{
	cv::Mat dx, dy, magnitudeMat;
	cv::Mat confidencePatch;
	cv::Mat magnitudePatch;
	cv::Mat  dxPatch, dyPatch;
	cv::Point2f normal;
	cv::Point maxPoint;
	cv::Point2f gradient;
	float confidence;

	getDerivatives(grayMat, dx, dy);
	magnitude(dx, dy, magnitudeMat);
	//calculeaza sqrt(dx^2+dy^2)

	cv::Mat maskedMagnitudeMat(magnitudeMat.size(), magnitudeMat.type(), cv::Scalar_<float>(0));
	//showMat(magnitudeMat, "magnitudemat masked");

	magnitudeMat.copyTo(maskedMagnitudeMat, (confidenceMat != 0.0f));
	//showMat(maskedMagnitudeMat, "magnitude mat masked");

	erode(maskedMagnitudeMat, maskedMagnitudeMat,cv:: Mat());
	//erodam matricea pentru a elimina mici zgomote
	//testat ->> docs.opencv.org/3.0-beta/doc/py_tutorials/py_imgproc/py_morphological_ops/py_morphological_ops.html
	//showMat(maskedMagnitudeMat,"magnitudemat masked eroded");

	cv::Point point;

	int i, j;
	for (i = 0; i < contours.size(); i++)
	{
		vector<cv::Point>contour = contours[i];

		for (j = 0; j < contour.size(); j++)
		{
			point = contour[j];
			confidencePatch = operationObj->getPatch(confidenceMat, point, RADIUS);

			confidence = computeConfidence(confidencePatch);
			//C(p),unde la noi p = point 

			confidence = float(confidence *(1 - omega)) + omega;
			confidence = confidence * alfa;

			//gradientul maxim 
			magnitudePatch = operationObj->getPatch(maskedMagnitudeMat, point, RADIUS);
			minMaxLoc(magnitudePatch, NULL, NULL, NULL, &maxPoint);

			dxPatch = operationObj->getPatch(dx, point, RADIUS);
			dyPatch = operationObj->getPatch(dy, point, RADIUS);

			gradient = cv::Point2f(-dyPatch.ptr<float>(maxPoint.y)[maxPoint.x],
				dxPatch.ptr<float>(maxPoint.y)[maxPoint.x]);

			//calculez D(p)
			normal = getNormal(contour, point, BORDER_RADIUS);

			//actualizez priorityMat
			//priorityMat.ptr<float>(point.y)[point.x] = abs((double)confidence * gradient.dot(normal));
			//P(p) = alfa*Rc(p) + beta * D(p)
			priorityMat.ptr<float>(point.y)[point.x] = (float)abs((float)gradient.dot(normal))*alfa + confidence;

		}
	}
}

void InPainter :: transferPatch(cv::Point bestMatch,cv:: Point bestPriority, cv::Mat &mat, const cv::Mat &maskMat, int RADIUS)
{	
	operationObj->getPatch(mat, bestMatch, RADIUS).copyTo(operationObj->getPatch(mat, bestPriority, RADIUS), operationObj ->getPatch(maskMat, bestPriority, RADIUS));
}

double InPainter::computeDistance(vector<double>A, vector<double>B)
{
	double distance = 0;

	for (int i = 0; i < A.size(); i++)
	{
		distance = distance + pow((A[i] - B[i]), 2);

	}

	distance = sqrt(distance);

	return distance;
}

cv::Point InPainter::getBestPoint(vector<double>patchHistogram, map< pair<int, int>, vector<double>>histogramSet)
{
	cv::Point bestMatch(0, 0);
	double minimumDistance = 1000000;

	map<pair<int, int>, vector<double>>::iterator it;

	for (it = histogramSet.begin(); it != histogramSet.end(); it++)
	{
		double distance = computeDistance(it->second, patchHistogram);
		if (distance < minimumDistance)
		{
			minimumDistance = distance;
			bestMatch.x = it->first.second;
			bestMatch.y = it->first.first;
		}
	}

	return bestMatch;
}

cv::Mat InPainter::doInPaint(cv::Mat image, cv::Mat mask,cv::Mat irisContour)
{	
	cv :: Mat lbpMask;
	map<pair<int, int>, vector<double>>histogramSet;

	int initialDim = image.cols;
	cv :: Mat confidenceMat, grayImage, priorityMat, erodedMask;

	cv :: Point maxPriorityPoint;
	cv :: Point bestMatchedPoint;

	cv :: Mat maxPriorityPatch, maxPriorityPatchConfidence, templateMask, result;
	double confidence;

	initialise(image, mask, confidenceMat, grayImage, priorityMat, irisContour);
	cv :: Mat availablePointMatrix(image.rows, image.cols, CV_32F, 0.0);
	lbpMask = computeLBPMaskAndAvailablePointsMatrix(image,availablePointMatrix,irisContour,mask);

	operationObj->showImage(availablePointMatrix, "available Points");

	erode(availablePointMatrix, availablePointMatrix, cv :: Mat(), cv :: Point(-1, -1), 2);
	histogramSet = featureUsed -> computeAllHistograms(lbpMask, availablePointMatrix,RADIUS);
	cout << histogramSet.size();
	
	vector<vector<cv :: Point>>contours;
	vector<cv :: Vec4i>hierarchy;

	const size_t area = mask.total();
	erode(mask, erodedMask,cv ::  Mat(), cv :: Point(-1, -1), RADIUS);

	cout << "incep bucla" << endl;
	while (countNonZero(mask) != area)	//cat timp mai sunt in mask elemente de zero(care trebuie completate) 
	{
		//cout << countNonZero(mask) << "  din " << area<<endl;
		//pun o valoare negativa in matricea de prioritati astfel incat sa nu se poata selecta partea bordata

		priorityMat.setTo(-0.1f);

		//Stept 1a) generam conturul
		getContours((mask == 0), contours, hierarchy);

		//Step 1b)
		//calculam prioritatile pentru punctele de pe contur

		computePriorities(priorityMat, confidenceMat, grayImage, contours, alfa, omega, RADIUS, BORDER_RADIUS);

		//Step 2 a)
		//luam patch-ul cu prioritate maxima din imagine si din confidenceMat

		minMaxLoc(priorityMat, NULL, NULL, NULL, &maxPriorityPoint);
		maxPriorityPatch = operationObj->getPatch(image, maxPriorityPoint, RADIUS);
		
		maxPriorityPatchConfidence = operationObj->getPatch(confidenceMat, maxPriorityPoint, RADIUS);
		//(maxPriorityPatchConfidence este alb negru

		cv::Mat aux = maxPriorityPatchConfidence != 0.0f;
		aux.convertTo(aux, CV_32F);
		aux /= 255.0f;
		
		cv::Mat lbpPatch = operationObj->getPatch(lbpMask, maxPriorityPoint, RADIUS);
		cv::Mat patchMask = operationObj->getPatch(mask, maxPriorityPoint, RADIUS);

		lbpPatch.setTo(0, patchMask == 0);

		vector<double>patchHistogram = featureUsed->computeHistogram(lbpPatch);
		
		bestMatchedPoint = getBestPoint(patchHistogram, histogramSet);

		transferPatch(bestMatchedPoint, maxPriorityPoint, image, (mask == 0), RADIUS);
		transferPatch(bestMatchedPoint, maxPriorityPoint, grayImage, (mask == 0), RADIUS);

		//fac update-uri
		confidence = computeConfidence(maxPriorityPatchConfidence);

		//pun valoarea de incredere tuturor punctelor din pattch care se lfa in regiunea target
		maxPriorityPatchConfidence.setTo(confidence, (maxPriorityPatchConfidence == 0.0f));
		mask = (confidenceMat != 0.0f);
	}
	
	return image;
}

InPainter::~InPainter()
{
}
