#include"SegmentManager.h"

namespace IS {

	//Constructor
	SegmentManager::SegmentManager(){
		grayImg = NULL;
		divisionImg = NULL;
		activeImgs.Clear();
		activeWins.Clear();
	}
	SegmentManager::~SegmentManager(){

	}

	//Private
	void SegmentManager::OnMouseClickedOnGray(int event, int x, int y, int flags, void* param) {
		SegmentManager* segMgr = SegmentManager::Instance();
		IplImage* gray = segMgr->GrayImage();
		uchar bright = CV_IMAGE_ELEM(gray, uchar, y, x);
		if (event == CV_EVENT_LBUTTONDOWN) {
			IplImage* division = segMgr->GetThreeDivision(gray, bright, segMgr->BottomValue(), segMgr->TopTolerance(), segMgr->BottomTolerance());
			cvShowImage((char*)(segMgr->DIVISION_WIN), division);
		}
		if (event == CV_EVENT_RBUTTONDOWN) {
			IplImage* division = segMgr->GetThreeDivision(gray, segMgr->TopValue(), bright, segMgr->TopTolerance(), segMgr->BottomTolerance());
			cvShowImage((char*)(segMgr->DIVISION_WIN), division);
		}
	}
	void SegmentManager::OnToleranceChanged(int value) {
		SegmentManager* segMgr = SegmentManager::Instance();
		IplImage* gray = segMgr->GrayImage();
		IplImage* division = segMgr->GetThreeDivision(gray, segMgr->TopValue(), segMgr->BottomValue(), segMgr->TopTolerance(), segMgr->BottomTolerance());
		cvShowImage((char*)(segMgr->DIVISION_WIN), division);
	}

	//Public

	bool SegmentManager::debugModeOn = false;
	SegmentManager* SegmentManager::instance = NULL;

	IplImage* SegmentManager::LoadImage(char* filename) {
		IplImage* ret = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
		if (ret != NULL){
			cvCvtColor(ret, ret, CV_BGR2RGB);
			activeImgs.Add(ret);
			srcImg = ret;
		}
		return ret;
	}
	void SegmentManager::ReleaseImage(IplImage* img) {
		if (img == NULL || &img==NULL)
			return;
		cvReleaseImage(&img);
	}
	IplImage* SegmentManager::ConvertToGrayImage(IplImage* src) {
		if (src == NULL)
			return NULL;
		if (grayImg == NULL) {
			grayImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
			activeImgs.Add(grayImg);
		}
		cvCvtColor(src, grayImg, CV_RGB2GRAY);
		return grayImg;
	}
	IplImage* SegmentManager::GetThreeDivisionAuto(IplImage* src, int tstep) {
		for (int i = 0; i < src->height; i++)
			for (int j = 0; j < src->width; j++) {
				uchar bright = CV_IMAGE_ELEM(src, uchar, i, j);
				brightness[bright]++;
			}
		int hill[256];
		int peak1 = -1, peak2 = -1, peakVal1 = 0, peakVal2 = 0;
		for (int i = 0; i < MAX_COLOR ; i++) {
			hill[i] = 0;
			for (int j = i - tstep + 1; j <= i + tstep - 1; j++) {
				if (j >= 0 && j < MAX_COLOR)
					hill[i] += brightness[j];
			}
			if (hill[i] > peakVal1) {
				peakVal1 = hill[i];
				peak1 = i;
			}
		}
		for (int i = 0; i < MAX_COLOR;i++)
			if (hill[i]>peakVal2 && i != peak1) {
				peakVal2 = hill[i];
				peak2 = i;
			}

		if (peak1 < 0 || peak2 < 0)
			return NULL;
		if (peak2 < peak1) {
			int tmp = peak1;
			peak1 = peak2;
			peak2 = tmp;
		}

		IplImage* ret = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		activeImgs.Add(ret);

		cvCopy(src, ret, NULL);
		for (int i = 0; i < ret->height; i++)
			for (int j = 0; j < ret->width; j++) {
				uchar bright = CV_IMAGE_ELEM(src, uchar, i, j);
				if (bright < peak1)
					CV_IMAGE_ELEM(ret, uchar, i, j) = LABEL_BOTTOM;
				else if (bright > peak2)
					CV_IMAGE_ELEM(ret, uchar, i, j) = LABEL_TOP;
				else
					CV_IMAGE_ELEM(ret, uchar, i, j) = LABEL_INTERMEDIATE;
			}

		return ret;
	}
	IplImage* SegmentManager::GetThreeDivision(IplImage* src, int featureTop,int featureBottom,int toleranceTop,int toleranceBottom) {

		if (src == NULL)
			return NULL;

		topValue = featureTop;
		bottomValue = featureBottom;
		topToleranceValue = toleranceTop;
		bottomToleranceValue = toleranceBottom;

		if (divisionImg == NULL) {
			divisionImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
			activeImgs.Add(divisionImg);
		}

		cvCopy(src, divisionImg, NULL);
		for (int i = 0; i < divisionImg->height; i++)
			for (int j = 0; j < divisionImg->width; j++) {
				uchar bright = CV_IMAGE_ELEM(src, uchar, i, j);
				int topDelta = abs(bright - featureTop);
				int bottomDelta = abs(bright - featureBottom);
				if (topDelta <= toleranceTop && bottomDelta <= toleranceBottom){
					CV_IMAGE_ELEM(divisionImg, uchar, i, j) = topDelta <= bottomDelta ? LABEL_TOP: LABEL_BOTTOM;
				}
				else if (topDelta <= toleranceTop)
					CV_IMAGE_ELEM(divisionImg, uchar, i, j) = LABEL_TOP;
				else if (bottomDelta <= toleranceBottom)
					CV_IMAGE_ELEM(divisionImg, uchar, i, j) = LABEL_BOTTOM;
				else
					CV_IMAGE_ELEM(divisionImg, uchar, i, j) = LABEL_INTERMEDIATE;
			}

		return divisionImg;
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
			//mask.at<uchar>(point.y(), point.x()) = cv::GC_FGD;
		}
		for (int i = 0; i < segViewer->BgdPixels().Count(); i++){
			QPoint point = segViewer->BgdPixels()[i];
			circle(mask, Point(point.x(), point.y()), segViewer->GCD_POINT_RADIUS, cv::GC_BGD);
			//mask.at<uchar>(point.y(), point.x()) = cv::GC_BGD;
		}
		for (int i = 0; i < segViewer->PrFgdPixels().Count(); i++){
			QPoint point = segViewer->PrFgdPixels()[i];
			circle(mask, Point(point.x(), point.y()), segViewer->GCD_POINT_RADIUS, cv::GC_PR_FGD);
			//mask.at<uchar>(point.y(), point.x()) = cv::GC_PR_FGD;
		}
		for (int i = 0; i < segViewer->PrBgdPixels().Count(); i++){
			QPoint point = segViewer->PrBgdPixels()[i];
			circle(mask, Point(point.x(), point.y()), segViewer->GCD_POINT_RADIUS, cv::GC_PR_BGD);
			//mask.at<uchar>(point.y(), point.x()) = cv::GC_PR_BGD;
		}
		grabCut(cv::cvarrToMat(src), mask, cv::Rect(), bgdModel, fgdModel, GCD_ITE_COUNT, cv::GC_INIT_WITH_MASK);
		if (divisionImg == NULL) {
			divisionImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
			activeImgs.Add(divisionImg);
		}
		for (int i = 0; i < src->height; i++){
			for (int j = 0; j < src->width; j++){
				uchar label = mask.at<uchar>(i, j);
				CV_IMAGE_ELEM(divisionImg, uchar, i, j) = (label == cv::GC_FGD || label == cv::GC_PR_FGD) ? LABEL_TOP : LABEL_BOTTOM;
			}
		}
		return divisionImg;
	}

	void SegmentManager::ShowImageWin(char* name, IplImage* img) {
		if (img == NULL)
			return;
		cvNamedWindow(name, CV_WINDOW_AUTOSIZE);
		cvShowImage(name, img);
		activeWins.Add(name);
	}
	void SegmentManager::RegisterGrayWinEvent(char* name) {
		cvCreateTrackbar("Top Tol:", name, &topToleranceValue, 100,OnToleranceChanged);
		cvCreateTrackbar("Bot Tol:", name, &bottomToleranceValue, 100, OnToleranceChanged);
		cvSetMouseCallback(name, OnMouseClickedOnGray, NULL);
	}
	void SegmentManager::DestoryWin(char* name) {
		cvDestroyWindow(name);
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
		for (int i = 0; i < activeWins.Count(); i++){
			cvDestroyWindow(activeWins[i]);
		}
		delete this;
	}
}