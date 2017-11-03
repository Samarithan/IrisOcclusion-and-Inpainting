#include "stdafx.h"
#include "Operations.h"

using namespace std;

Operations::Operations()
{
}

cv::Mat Operations :: loadImage(string sourcePath)
{
	cv::Mat image;
	image = cv::imread(sourcePath, 1);
	image.convertTo(image, CV_32FC1);
	return image;
}

cv::Mat Operations :: readYmlFile(string path,string name)
{
	cv::Mat imageFile;

	cv::FileStorage ss;
	ss.open(path, cv::FileStorage::READ);
	ss[name] >> imageFile;
	ss.release();

	return imageFile;
}

void Operations::showImage(cv::Mat image, string windowName)
{
	cv :: namedWindow(windowName);
	cv :: imshow(windowName, image);
	cv :: waitKey(0);
	cv :: destroyWindow(windowName);
}

cv::Mat Operations::createDamagedIris(cv::Mat irisImage,int deletedDimension,char position)
{
	int i, j;
	cv::Mat damagedIris = irisImage.clone();
	cout << getTypeOfImage(irisImage);
	//cv::cvtColor(damagedIris, damagedIris, CV_BGR2GRAY);

	if (position == 'U')
	{
		for (i = 0; i<deletedDimension; i++)
			for (j = 0; j < irisImage.cols; j++)
			{
				damagedIris.at<cv::Vec3f>(i,j) = (0,0,0);
			}
	}
	else if (position == 'D')
	{
		for (i = damagedIris.rows - deletedDimension; i<damagedIris.rows; i++)
			for (j = 0; j < damagedIris.cols; j++)
			{
				damagedIris.at<cv::Vec3f>(i, j) = (0, 0, 0);
			}
	}

	return damagedIris;
}

string Operations::getTypeOfImage(cv::Mat image)
{
		string r;
		int type = image.type();

		uchar depth = type & CV_MAT_DEPTH_MASK;
		uchar chans = 1 + (type >> CV_CN_SHIFT);

		switch (depth) {
		case CV_8U:  r = "8U"; break;
		case CV_8S:  r = "8S"; break;
		case CV_16U: r = "16U"; break;
		case CV_16S: r = "16S"; break;
		case CV_32S: r = "32S"; break;
		case CV_32F: r = "32F"; break;
		case CV_64F: r = "64F"; break;
		default:     r = "User"; break;
		}

		r += "C";
		r += (chans + '0');

		return r;
}

cv::Mat Operations :: getPatch(const cv::Mat & mat, cv::Point point, int RADIUS)
{
	return mat(cv :: Range(point.y - RADIUS, point.y + RADIUS + 1), cv ::Range(point.x - RADIUS, point.x + RADIUS + 1));
}


cv::Mat Operations:: createIrisMask(cv::Mat irisContour, int deletedDimension,char position)
{
	int i, j;
	cv::Mat maskImage = irisContour.clone();

	if (position == 'U')
	{
		for (i = deletedDimension;i<maskImage.rows; i++)
			for (j = 0; j < maskImage.cols; j++)
			{
				maskImage.at<float>(i, j) = 1;
			}
	}
	else if (position == 'D')
		{
		for (i=maskImage.rows - deletedDimension;i<maskImage.rows;i++)
			for (j = 0; j < maskImage.cols; j++)
			{
				maskImage.at<float>(i, j) = (255, 255, 255);
			}
		}

	return maskImage;
}

vector<cv::Mat> Operations ::  getDirectoryImages(char * sourcePath)
{
	vector<cv::Mat> listOfImages;
	vector<string> files;
	WIN32_FIND_DATA data;
	LARGE_INTEGER filesize;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	string pathStr = sourcePath;
	wstring pathWstring(pathStr.begin(), pathStr.end());
	LPCTSTR pathOfFiles = pathWstring.c_str();
	hFind = FindFirstFile(pathOfFiles, &data);

	cv::Mat currentImage;

	if (hFind == INVALID_HANDLE_VALUE)
	{
		cout << "first file error" << dwError << endl;
		exit(0);
	}

	while (FindNextFile(hFind, &data) != 0)
	{
		wstring fileName(data.cFileName);
		size_t size = wcslen(data.cFileName);

		char * buffer = (char *)calloc(2 * size + 2, 1);
		wcstombs(buffer, data.cFileName, 2 * size + 2);

		if (strstr(buffer, "bmp") == NULL &&  strstr(buffer, "jpg") == NULL)
			continue;

		char * fullPath = (char *)calloc(strlen(sourcePath) + 20, 1);
		strcpy(fullPath, sourcePath);
		fullPath[strlen(fullPath) - 1] = 0;
		strcat(fullPath, buffer);

		cv::Mat currentMat = cv::imread(fullPath, 1);
		listOfImages.push_back(currentMat);
	}

	FindClose(hFind);
	return listOfImages;
	
}

Operations::~Operations()
{
}
