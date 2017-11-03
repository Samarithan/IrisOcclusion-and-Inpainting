#include "TestController.h"

#define irisContourPath "D:\\Facultate\\Image Proc\\Project\\iris_contours.yml"
#define irisContourName "iris_contour_mask"

#define irisImagePath "D:\\Facultate\\Image Proc\\Project\\0.jpg"
#define irisCutImage "D:\\Facultate\\Image Proc\\Project\\irisCut.jpg"

#define damagedIrisMask "D:\\Facultate\\Image Proc\\Project\\maskDamaged.jpg"
#define damagedMaskName "irisDamagedMask"

int main(int argc, char * argv[])
{	
	Operations * op = new Operations();

	cv::Mat damagedIris = op->loadImage(irisCutImage);
	cv::Mat irisContour = op-> readYmlFile(irisContourPath,irisContourName);
	cv::Mat mask = op->createIrisMask(irisContour,30,'U');

	op->showImage(mask,"masca");
	op->showImage(damagedIris, "damaged");
	op->showImage(irisContour, "Iris Contour");

	Feature * featureObj = new Lbp16();
	InPainter * inPaintObj = new InPainter(20, 30, float(0.1), float(0.2), float(0.5),float(0.004),featureObj);

	cv::Mat result = inPaintObj->doInPaint(damagedIris, mask,irisContour);

	op->showImage(result, "the result image");

	return 0;
}
