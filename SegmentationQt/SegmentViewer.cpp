#include "SegmentViewer.h"

namespace IS{

	//Constructor
	SegmentViewer::SegmentViewer(QWidget *parent)
		: QMainWindow(parent)
	{
		ui.setupUi(this);
	}
	SegmentViewer::~SegmentViewer()
	{

	}

	//Public

	SegmentViewer* SegmentViewer::Instance(){
		if (instance == NULL){
			instance = new SegmentViewer();
		}
		return instance;
	}

	void SegmentViewer::ShowImage(int id, IplImage* cvImg){

		if (cvImg == NULL)
			return;

		QImage* qImg = new QImage((uchar*)cvImg->imageData, cvImg->width, cvImg->height, cvImg->widthStep, QImage::Format_Grayscale8);
		activeImgs.push_back(qImg);
		switch (id)
		{
		case ID_GRAY:{
			ui.label_gray->setFixedHeight(qImg->height());
			ui.label_gray->setFixedWidth(qImg->width());
			ui.label_gray->setPixmap(QPixmap::fromImage(*qImg));
		}
					 break;
		case ID_DIVISION:
			break;
		default:
			break;
		}
	}
	void SegmentViewer::ReleaseAll(){
		std::vector<QImage*>::iterator imgI;
		for (imgI = activeImgs.begin(); imgI != activeImgs.end(); imgI++) {
			if (*imgI != NULL) {
				delete *imgI;
			}
		}
		delete this;
	}

	//Private

	SegmentViewer* SegmentViewer::instance = NULL;
}