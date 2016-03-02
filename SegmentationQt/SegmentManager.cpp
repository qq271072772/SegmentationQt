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
	IplImage*  SegmentManager::DownSample(IplImage* src, int cnt){
		if (src == NULL)
			return NULL;
		IplImage* cur = src;
		for (int i = 0; i < cnt; i++){
			IplImage* down = cvCreateImage(CvSize(cur->width / 2, cur->height / 2), cur->depth, cur->nChannels);
			cvPyrDown(cur, down);
			if (cur != src)
				cvReleaseImage(&cur);
			cur = down;
		}
		return cur;
	}
	IplImage*  SegmentManager::UpSample(IplImage* src, int cnt){
		if (src == NULL)
			return NULL;
		IplImage* cur = src;
		for (int i = 0; i < cnt; i++){
			IplImage* up = cvCreateImage(CvSize(cur->width * 2, cur->height * 2), cur->depth, cur->nChannels);
			cvPyrUp(cur, up);
			if (cur != src)
				cvReleaseImage(&cur);
			cur = up;
		}
		return cur;
	}

	//Public

	bool SegmentManager::debugModeOn = true;
	SegmentManager* SegmentManager::instance = NULL;

	void SegmentManager::LoadSrcImage(char* filename){
		IplImage* ret = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
		if (ret != NULL){
			EnsureImg(srcImg);
			EnsureImg(srcCpImg);

			srcImg = ret;
			srcCpImg = cvCreateImage(CvSize(srcImg->width,srcImg->height),srcImg->depth,srcImg->nChannels);
			cvCopy(srcImg, srcCpImg);
			cvCvtColor(srcCpImg, srcCpImg, CV_BGR2RGB);
			divisionImg = cvCreateImage(CvSize(srcImg->width, srcImg->height), IPL_DEPTH_8U, 1);
			activeImgs.Add(divisionImg);

			activeImgs.Add(srcImg);
			activeImgs.Add(srcCpImg);
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
		cvCvtColor(src, grayImg, CV_BGR2GRAY);
		return grayImg;
	}
	IplImage* SegmentManager::GetGrabCut(){
		using namespace cv;
		SegmentViewer* segViewer = SegmentViewer::Instance();
		Mat mask(srcImg->height, srcImg->width, CV_8UC1), fgdModel, bgdModel;
		for (int i = 0; i < srcImg->height; i++){
			for (int j = 0; j < srcImg->width; j++){
				mask.at<uchar>(i, j) = cv::GC_PR_BGD;
			}
		}
		for (int i = 0; i < segViewer->FgdPixels().Count(); i++){
			QPoint point = segViewer->FgdPixels()[i];
			circle(mask, Point(point.x(), point.y()), segViewer->GCD_POINT_RADIUS, cv::GC_FGD, -1);
		}
		for (int i = 0; i < segViewer->BgdPixels().Count(); i++){
			QPoint point = segViewer->BgdPixels()[i];
			circle(mask, Point(point.x(), point.y()), segViewer->GCD_POINT_RADIUS, cv::GC_BGD,-1);
		}
		for (int i = 0; i < segViewer->PrFgdPixels().Count(); i++){
			QPoint point = segViewer->PrFgdPixels()[i];
			circle(mask, Point(point.x(), point.y()), segViewer->GCD_POINT_RADIUS, cv::GC_PR_FGD,-1);
		}
		for (int i = 0; i < segViewer->PrBgdPixels().Count(); i++){
			QPoint point = segViewer->PrBgdPixels()[i];
			circle(mask, Point(point.x(), point.y()), segViewer->GCD_POINT_RADIUS, cv::GC_PR_BGD,-1);
		}

		IplImage* downSrcImg = DownSample(srcImg, DOWN_SAMPLE_CNT);
		IplImage* downMaskImg = DownSample(&(IplImage(mask)), DOWN_SAMPLE_CNT);
		Mat downMaskMat = cvarrToMat(downMaskImg);

		grabCut(cvarrToMat(downSrcImg), downMaskMat, cv::Rect(), bgdModel, fgdModel, GCD_ITE_COUNT, cv::GC_INIT_WITH_MASK);

		for (int i = 0; i < downMaskImg->height; i++){
			for (int j = 0; j < downMaskImg->width; j++){
				uchar label = downMaskMat.at<uchar>(i, j);
				CV_IMAGE_ELEM(downMaskImg, uchar, i, j) = (label == cv::GC_FGD || label == cv::GC_PR_FGD) ? LABEL_TOP : LABEL_BOTTOM;
			}
		}

		EnsureImg(divisionImg);
		divisionImg=UpSample(downMaskImg, DOWN_SAMPLE_CNT);
		activeImgs.Add(divisionImg);


		cvReleaseImage(&downSrcImg);
		cvReleaseImage(&downMaskImg);

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