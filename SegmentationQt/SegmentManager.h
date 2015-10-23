#include<iostream>
#include<math.h>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Utility.h"
#include "SegmentViewer.h"

using namespace std;

namespace IS {
	class SegmentManager {
		private:

			const int LABEL_TOP = 255;
			const int LABEL_INTERMEDIATE = 128;
			const int LABEL_BOTTOM = 0;
			const int MAX_COLOR = 256;
			const int MAX_TOLERANCE = 100;

			const int GCD_ITE_COUNT = 5;

			int brightness[256];

			
			IplImage *srcImg=NULL,*divisionImg = NULL, *grayImg = NULL;
			int topValue, bottomValue, topToleranceValue, bottomToleranceValue;

			static SegmentManager* instance;

			List<IplImage*> activeImgs;

			void EnsureImg(IplImage* img){
				if (img != NULL){
					activeImgs.Remove(img);
					cvReleaseImage(&img);
				}
			}

		public:

			const char* SRC_WIN = "Source";
			const char* GRAY_WIN = "Gray";
			const char* DIVISION_WIN = "Division";

			static bool debugModeOn;

			SegmentManager();
			~SegmentManager();

			static SegmentManager* Instance();

			IplImage* GrayImage() {
				return grayImg;
			}
			IplImage* SrcImage(){
				return srcImg;
			}
			IplImage* RetImage(){
				return divisionImg;
			}
			int TopValue() {
				return topValue;
			}
			int BottomValue() {
				return bottomValue;
			}
			int TopTolerance() {
				return topToleranceValue;
			}
			int BottomTolerance() {
				return bottomToleranceValue;
			}

			void LoadSrcImage(char * filename);

			IplImage* ConvertToGrayImage(IplImage* src);

			//tstep = threshold step , find two peak point of gray scale
			//bad result,removed
			IplImage* GetThreeDivisionAuto(IplImage* src, int tstep = 1);

			//get result by adjustment feature and tolerance
			//removed
			IplImage* GetThreeDivision(IplImage* src, int featureTop, int featureBottom, int toleranceTop, int toleranceBottom);

			//get grab cut result
			IplImage* GetGrabCut(IplImage* src);

			void ReleaseAll();
	};
}