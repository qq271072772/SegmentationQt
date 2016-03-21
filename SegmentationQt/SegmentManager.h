#ifndef SEGMENT_MANAGER
#define SEGMENT_MANAGER

#include<iostream>
#include<math.h>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Utility.h"
#include "IOHelper.h"
#include "ImageHelper.h"
#include "EdgePicker.h"

using namespace std;
using namespace Utility;

namespace IS {
	class SegmentManager {
		private:

			const int LABEL_TOP = 255;
			const int LABEL_INTERMEDIATE = 128;
			const int LABEL_BOTTOM = 0;
			const int MAX_COLOR = 256;

			int brightness[256];
			
			IplImage *m_srcImg = NULL, *m_dstImg = NULL;
			IplImage *m_grabcutImg = NULL;
			cv::Mat m_mask;
			bool m_maskInited;

			static SegmentManager* m_instance;

			List<IplImage*> m_activeImgs;

			IplImage* EnsureImg(IplImage* img){
				if (img != NULL){
					m_activeImgs.Remove(img);
					cvReleaseImage(&img);
					return NULL;
				}
				return img;
			}
			IplImage* DownSample(IplImage* src, int cnt = 1);
			IplImage* UpSample(IplImage* src, int cnt = 1);
			void RefineGrabCut(IplImage* src);

		public:

			SegmentManager();
			~SegmentManager();

			static SegmentManager* Instance();

			IplImage* SrcImage(){
				return m_srcImg;
			}
			IplImage* DstImage(){
				return m_dstImg;
			}

			void DrawMaskPoint(Vector2 p, int radius, cv::GrabCutClasses type);
			void ClearMask();

			void LoadSrcImage(char * filename);
			void SaveDstImage(char* filename);

			IplImage* ConvertToGrayImage(IplImage* src);
			void GenerateGrabCut(int iteCnt, int downSampleCnt);

			void ReleaseAll();
	};
}

#endif