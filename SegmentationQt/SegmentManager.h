#include<iostream>
#include<vector>
#include<math.h>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>

using namespace std;

namespace IS {
	class SegmentManager {
		private:

			const int LABEL_TOP = 0;
			const int LABEL_INTERMEDIATE = 128;
			const int LABEL_BOTTOM = 255;
			const int MAX_COLOR = 256;
			const int MAX_TOLERANCE = 100;
			int brightness[256];

			
			IplImage* divisionImg = NULL, *grayImg = NULL;
			int topValue, bottomValue, topToleranceValue, bottomToleranceValue;

			static SegmentManager* instance;

			vector<IplImage*> activeImgs;
			vector<char*> activeWins;

			static void OnMouseClickedOnGray(int event, int x, int y, int flags, void* param);
			static void OnToleranceChanged(int value);

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

			IplImage* LoadImage(char* filename);
			void ReleaseImage(IplImage* img);
			void WaitKey(int delay){
				cvWaitKey(delay);
			}

			IplImage* ConvertToGrayImage(IplImage* src);

			//tstep = threshold step , find two peak point of gray scale
			//bad result
			IplImage* GetThreeDivisionAuto(IplImage* src, int tstep = 1);

			//get result by adjustment feature and tolerance
			IplImage* GetThreeDivision(IplImage* src, int featureTop, int featureBottom, int toleranceTop, int toleranceBottom);

			void ShowImageWin(char* name,IplImage* img);
			void RegisterGrayWinEvent(char* name);
			void DestoryWin(char* name);

			void ReleaseAll();
	};
}