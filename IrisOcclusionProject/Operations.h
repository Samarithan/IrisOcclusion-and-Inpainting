#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

#include <assert.h>
#include <stdio.h>
#include <algorithm>

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<math.h>
#include<vector>
#include<map>
#include <string>
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <direct.h>
#include <utility>

#pragma comment(lib, "User32.lib")
#pragma warning (disable:4996)

using namespace std;

class Operations
{
public:
	Operations();

	cv::Mat loadImage(string sourcePath);

	void showImage(cv::Mat image,string windowName);
	
	cv::Mat createDamagedIris(cv::Mat irisImage, int deletedDimension, char position);

	string getTypeOfImage(cv::Mat image);

	cv::Mat createIrisMask(cv::Mat irisContour, int deletedDimension, char position);

	cv :: Mat getPatch(const cv :: Mat & mat, cv :: Point point, int RADIUS);

	cv::Mat readYmlFile(string path,string name);

	vector<cv::Mat> getDirectoryImages(char * sourcePath);


	~Operations();
};
#endif 
