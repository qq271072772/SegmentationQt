#include"SegmentManager.h"

namespace IS {

	//Constructor
	SegmentManager::SegmentManager(){
		grayImg = NULL;
		divisionImg = NULL;
		activeImgs.Clear();
	}
	SegmentManager::~SegmentManager(){

	}

	//Private

	//Public

	bool SegmentManager::debugModeOn = true;
	SegmentManager* SegmentManager::instance = NULL;

	void SegmentManager::LoadSrcImage(char* filename){
		IplImage* ret = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
		if (ret != NULL){
			EnsureImg(srcImg);
			cvCvtColor(ret, ret, CV_BGR2RGB);
			activeImgs.Add(ret);
			srcImg = ret;
			ConvertToGrayImage(srcImg);
		}
	}
	void SegmentManager::SaveDstImage(char* filename){
		cvSaveImage("output.jpg", divisionImg);
	}

	IplImage* SegmentManager::ConvertToGrayImage(IplImage* src) {
		if (src == NULL)
			return NULL;
		EnsureImg(grayImg);
		grayImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		activeImgs.Add(grayImg);
		cvCvtColor(src, grayImg, CV_RGB2GRAY);
		divisionImg = grayImg;
		return grayImg;
	}
	IplImage* SegmentManager::GetGrabCut(IplImage* src){
		using namespace cv;
		SegmentViewer* segViewer = SegmentViewer::Instance();
		Mat mask(src->height, src->width, CV_8UC1), fgdModel, bgdModel;
		for (int i = 0; i < src->height; i++){
			for (int j = 0; j < src->width; j++){
				mask.at<uchar>(i, j) = cv::GC_PR_BGD;
			}
		}
		for (int i = 0; i < segViewer->FgdPixels().Count(); i++){
			QPoint point = segViewer->FgdPixels()[i];
			circle(mask, Point(point.x(), point.y()), segViewer->GCD_POINT_RADIUS, cv::GC_FGD);
		}
		for (int i = 0; i < segViewer->BgdPixels().Count(); i++){
			QPoint point = segViewer->BgdPixels()[i];
			circle(mask, Point(point.x(), point.y()), segViewer->GCD_POINT_RADIUS, cv::GC_BGD);
		}
		for (int i = 0; i < segViewer->PrFgdPixels().Count(); i++){
			QPoint point = segViewer->PrFgdPixels()[i];
			circle(mask, Point(point.x(), point.y()), segViewer->GCD_POINT_RADIUS, cv::GC_PR_FGD);
		}
		for (int i = 0; i < segViewer->PrBgdPixels().Count(); i++){
			QPoint point = segViewer->PrBgdPixels()[i];
			circle(mask, Point(point.x(), point.y()), segViewer->GCD_POINT_RADIUS, cv::GC_PR_BGD);
		}
		grabCut(cv::cvarrToMat(src), mask, cv::Rect(), bgdModel, fgdModel, GCD_ITE_COUNT, cv::GC_INIT_WITH_MASK);

		EnsureImg(divisionImg);
		divisionImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		activeImgs.Add(divisionImg);

		for (int i = 0; i < src->height; i++){
			for (int j = 0; j < src->width; j++){
				uchar label = mask.at<uchar>(i, j);
				CV_IMAGE_ELEM(divisionImg, uchar, i, j) = (label == cv::GC_FGD || label == cv::GC_PR_FGD) ? LABEL_TOP : LABEL_BOTTOM;
			}
		}
		return divisionImg;
	}

	SegmentManager* SegmentManager::Instance() {
		if (instance == NULL) {
			instance = new SegmentManager();
		}
		return instance;
	}

	void SegmentManager::ReleaseAll() {
		for (int i = 0; i < activeImgs.Count();i++) {
			if (activeImgs[i] != NULL) {
				IplImage* tmp = activeImgs[i];
				cvReleaseImage(&tmp);
			}
		}
		delete this;
	}
}