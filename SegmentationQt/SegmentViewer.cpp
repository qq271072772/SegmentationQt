#include "SegmentViewer.h"

namespace IS{

	//Constructor
	SegmentViewer::SegmentViewer(QWidget *parent)
		: QMainWindow(parent)
	{
		ui.setupUi(this);
		ui.scrollview_gray->viewport()->installEventFilter(this);
		ui.scrollview_division->viewport()->installEventFilter(this);
	}
	SegmentViewer::~SegmentViewer()
	{

	}

	//Public

	const float SegmentViewer::SCALE_MIN = 0.2;
	const float SegmentViewer::SCALE_MAX = 2.0;
	const float SegmentViewer::WHEEL_SENSIBILITY = 0.0005;
	const float SegmentViewer::SCROLL_SENSIBILITY = 1;

	SegmentViewer* SegmentViewer::Instance(){
		if (instance == NULL){
			instance = new SegmentViewer();
		}
		return instance;
	}

	/*Register origin and new image*/
	void SegmentViewer::RegisterImage(int id, IplImage* cvImg){

		if (cvImg == NULL)
			return;

		if (images.find(id) != images.end())
			return;

		QImage qImg;
		switch (id)
		{
		case ID_SRC:
			qImg = QImage((uchar*)cvImg->imageData, cvImg->width, cvImg->height, cvImg->widthStep, QImage::Format_RGB888);
			ui.label_gray->setFixedHeight(qImg.height());
			ui.label_gray->setFixedWidth(qImg.width());
			ui.label_gray->setPixmap(QPixmap::fromImage(qImg));
			ui.pool_gray->resize(qImg.width(), qImg.height());
			images[id] = new ImageData(id, ui.label_gray, ui.pool_gray, ui.scrollview_gray);
			break;
		case ID_GRAY:
			qImg = QImage((uchar*)cvImg->imageData, cvImg->width, cvImg->height, cvImg->widthStep, QImage::Format_Grayscale8);
			ui.label_gray->setFixedHeight(qImg.height());
			ui.label_gray->setFixedWidth(qImg.width());
			ui.label_gray->setPixmap(QPixmap::fromImage(qImg));
			ui.pool_gray->resize(qImg.width(), qImg.height());
			images[id] = new ImageData(id, ui.label_gray, ui.pool_gray, ui.scrollview_gray);
			break;
		case ID_DIVISION:
			qImg = QImage((uchar*)cvImg->imageData, cvImg->width, cvImg->height, cvImg->widthStep, QImage::Format_Grayscale8);
			ui.label_division->setFixedHeight(qImg.height());
			ui.label_division->setFixedWidth(qImg.width());
			ui.label_division->setPixmap(QPixmap::fromImage(qImg));
			ui.pool_division->resize(qImg.width(), qImg.height());
			images[id] = new ImageData(id, ui.label_division, ui.pool_division, ui.scrollview_division);
			break;
		default:
			break;
		}
	}
	void SegmentViewer::ReleaseAll(){
		std::map<int, ImageData*>::iterator ite;
		for (ite = images.begin(); ite != images.end(); ite++){
			delete ite->second;
		}
		images.clear();
		delete this;
	}

	void SegmentViewer::DebugLog(float x){
		char msg[20];
		sprintf(msg, "%5.3f", x);
		DebugLog(msg);
	}
	void SegmentViewer::DebugLog(char* msg){
		ui.label_debug->setText(ui.label_debug->text() + "   " + msg);
	}

	//Private

	SegmentViewer* SegmentViewer::instance = NULL;

	void SegmentViewer::DealImageEvent(QObject *obj, QEvent* ev){
		switch (ev->type()){
		case QEvent::Wheel:
			{
				QWheelEvent* wev = static_cast<QWheelEvent*>(ev);
				float delta = wev->angleDelta().y()*WHEEL_SENSIBILITY;
				std::map<int, ImageData*>::iterator ite;
				for (ite = images.begin(); ite != images.end(); ite++){
						ScaleImage(ite->second->id, delta);
				}
			}
			break;
		case QEvent::MouseButtonPress:
			{
				QMouseEvent* mev = (QMouseEvent*)ev;
				mouseDown = true;
				lastMousePos.setX(mev->x());
				lastMousePos.setY(mev->y());
			}
			break;
		case QEvent::MouseButtonRelease:
			mouseDown = false;
			break;
		case QEvent::MouseMove:
			if (mouseDown){
				QMouseEvent* mev = (QMouseEvent*)ev;
				float deltaX = (mev->x() - lastMousePos.x())*SCROLL_SENSIBILITY;
				float deltaY = (mev->y() - lastMousePos.y())*SCROLL_SENSIBILITY;
				std::map<int, ImageData*>::iterator ite;
				for (ite = images.begin(); ite != images.end(); ite++){
						ScrollImage(ite->second->id, deltaX, deltaY);
				}
				lastMousePos.setX(mev->x());
				lastMousePos.setY(mev->y());
			}
			break;
		default:
			break;
		}

	}
	void SegmentViewer::ScaleImage(int id ,float delta){
		if (images.find(id) == images.end()){
			//DebugLog((char*)("Image id not exist(id:" + id + ')'));
			return;
		}
		ImageData* data = images[id];
		float newScale = data->scale + delta;
		if (newScale<SCALE_MIN || newScale>SCALE_MAX)
			return;
		data->scale = newScale;
		int newHeight = data->originHeight*newScale;
		int newWidth = data->originWidth*newScale;
		data->label->setFixedHeight(newHeight);
		data->label->setFixedWidth(newWidth);
		data->pool->resize(newWidth, newHeight);
	}
	void SegmentViewer::ScrollImage(int id, float deltaX, float deltaY){
		if (images.find(id) == images.end()){
			return;
		}
		ImageData* data = images[id];
		data->scroll->horizontalScrollBar()->setValue(data->scroll->horizontalScrollBar()->value() - deltaX);
		data->scroll->verticalScrollBar()->setValue(data->scroll->verticalScrollBar()->value() - deltaY);
	}

	//Protected

	bool SegmentViewer::eventFilter(QObject *obj, QEvent* ev){
		std::map<int, ImageData*>::iterator ite;
		for (ite = images.begin(); ite != images.end(); ite++){
			if (obj == ite->second->scroll->viewport()){
				DealImageEvent(obj, ev);
				return true;
			}
		}
		return false;
	}
}