#pragma once
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

//return a vector of images from the sourcePath directory
vector<cv::Mat> getDirectoryImages(char * sourcePath);