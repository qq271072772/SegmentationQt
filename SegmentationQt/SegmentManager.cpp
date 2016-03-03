#include"SegmentManager.h"

namespace IS {

	//Constructor
	SegmentManager::SegmentManager(){
		m_dstImg = NULL;
		m_activeImgs.Clear();
		m_maskInited = false;
	}
	SegmentManager::~SegmentManager(){

	}

	//Private
	IplImage*  SegmentManager::DownSample(IplImage* src, int cnt){
		if (src == NULL)
			return NULL;
		IplImage* cur = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
		cvCopy(src, cur);
		for (int i = 0; i < cnt; i++){
			IplImage* down = cvCreateImage(CvSize(cur->width / 2, cur->height / 2), cur->depth, cur->nChannels);
			cvPyrDown(cur, down);
			cvReleaseImage(&cur);
			cur = down;
		}
		return cur;
	}
	IplImage*  SegmentManager::UpSample(IplImage* src, int cnt){
		if (src == NULL)
			return NULL;
		IplImage* cur = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
		cvCopy(src, cur);
		for (int i = 0; i < cnt; i++){
			IplImage* up = cvCreateImage(CvSize(cur->width * 2, cur->height * 2), cur->depth, cur->nChannels);
			cvPyrUp(cur, up);
			cvReleaseImage(&cur);
			cur = up;
		}
		return cur;
	}
	void SegmentManager::DrawMask(IplImage* src, IplImage* mask){
		if (src == NULL || mask==NULL)
			return;
		if (mask->width != src->width || mask->height != src->height)
			return;
		for (int i = 0; i < src->height; i++)
			for (int j = 0; j < src->width; j++){
				uchar label = ImageHelper::SampleElem(mask, j, i);
				RGB value = ImageHelper::SampleElemRGB(src, j, i);
				ImageHelper::SetElemRGB(src, j, i, label <128 ? value : RGB(255, 255, 255));
			}
	}

	//Public

	SegmentManager* SegmentManager::m_instance = NULL;

	void SegmentManager::LoadSrcImage(char* filename){
		IplImage* ret = cvLoadImage(filename, CV_LOAD_IMAGE_COLOR);
		if (ret != NULL){
			m_srcImg = EnsureImg(m_srcImg);
			m_srcCpImg = EnsureImg(m_srcCpImg);

			m_srcImg = ret;
			m_srcCpImg = cvCreateImage(CvSize(m_srcImg->width,m_srcImg->height),m_srcImg->depth,m_srcImg->nChannels);
			cvCopy(m_srcImg, m_srcCpImg);
			cvCvtColor(m_srcCpImg, m_srcCpImg, CV_BGR2RGB);

			m_activeImgs.Add(m_srcImg);
			m_activeImgs.Add(m_srcCpImg);

			ConvertToGrayImage(m_srcImg);

			ClearMask();
		}
	}
	void SegmentManager::SaveDstImage(char* filename){
		if (m_dstImg == NULL)
			return;
		IplImage* output = ImageHelper::CreateCopy(m_srcImg);
		DrawMask(output, m_dstImg);
		cvSaveImage("output.jpg", output);
		ImageHelper::ReleaseImage(&output);
		cvSaveImage("grabcut.jpg", m_dstImg);
	}

	IplImage* SegmentManager::ConvertToGrayImage(IplImage* src) {
		if (src == NULL)
			return NULL;
		m_dstImg = EnsureImg(m_dstImg);

		m_dstImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		m_activeImgs.Add(m_dstImg);
		cvCvtColor(src, m_dstImg, CV_BGR2GRAY);
		return m_dstImg;
	}
	void SegmentManager::GenerateGrabCut(int iteCnt, int downSampleCnt){
		if (!m_maskInited)
			return;
		using namespace cv;
		Mat fgdModel, bgdModel;

		IplImage* downSrcImg = DownSample(m_srcImg, downSampleCnt);
		IplImage* downMaskImg = DownSample(&(IplImage(m_mask)), downSampleCnt);
		Mat downMaskMat = cvarrToMat(downMaskImg);

		grabCut(cvarrToMat(downSrcImg), downMaskMat, cv::Rect(), bgdModel, fgdModel, iteCnt, cv::GC_INIT_WITH_MASK);

		for (int i = 0; i < downMaskImg->height; i++){
			for (int j = 0; j < downMaskImg->width; j++){
				uchar label = downMaskMat.at<uchar>(i, j);
				CV_IMAGE_ELEM(downMaskImg, uchar, i, j) = (label == cv::GC_FGD || label == cv::GC_PR_FGD) ? LABEL_TOP : LABEL_BOTTOM;
			}
		}

		m_dstImg = EnsureImg(m_dstImg);
		m_dstImg = UpSample(downMaskImg, downSampleCnt);
		m_activeImgs.Add(m_dstImg);


		cvReleaseImage(&downSrcImg);
		cvReleaseImage(&downMaskImg);
	}
	void SegmentManager::DrawMaskPoint(Vector2 p, int radius, cv::GrabCutClasses type){
		circle(m_mask, cv::Point(p.X(), p.Y()), radius, type, -1);
		m_maskInited = true;
	}
	void SegmentManager::ClearMask(){
		if (m_srcImg == NULL)
			return;
		m_mask = cv::Mat(m_srcImg->height, m_srcImg->width, CV_8UC1);
		for (int i = 0; i < m_srcImg->height; i++){
			for (int j = 0; j < m_srcImg->width; j++){
				m_mask.at<uchar>(i, j) = cv::GC_PR_BGD;
			}
		}
		m_maskInited = false;
	}

	SegmentManager* SegmentManager::Instance() {
		if (m_instance == NULL) {
			m_instance = new SegmentManager();
		}
		return m_instance;
	}

	void SegmentManager::ReleaseAll() {
		for (int i = 0; i < m_activeImgs.Count();i++) {
			if (m_activeImgs[i] != NULL) {
				IplImage* tmp = m_activeImgs[i];
				cvReleaseImage(&tmp);
			}
		}
		delete this;
	}
}