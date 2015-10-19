#include "SegmentViewer.h"
#include "SegmentManager.h"

namespace IS{

	//Constructor
	SegmentViewer::SegmentViewer(QWidget *parent)
		: QMainWindow(parent)
	{
		ui.setupUi(this);
		ui.scrollview_gray->viewport()->installEventFilter(this);
		ui.scrollview_division->viewport()->installEventFilter(this);
		ui.label_gray->installEventFilter(this);
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

		float oldScale = 1;
		if (images.ContainsKey(id)){
			oldScale = images.GetCasted(id)->scale;
			delete images[id];
			images.Remove(id);
		}

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
		if (oldScale != 1)
			ScaleImage(id, oldScale - 1);
	}
	void SegmentViewer::ReleaseAll(){
		vector<int> keys = images.Keys();
		for (int i = 0; i < keys.size(); i++){
			delete images[keys[i]];
		}
		images.Clear();
		delete this;
	}

	//Private

	SegmentViewer* SegmentViewer::instance = NULL;

	void SegmentViewer::DealViewEvent(QObject *obj, QEvent* ev){
		switch (ev->type()){
		case QEvent::Wheel:
			{
				QWheelEvent* wev = static_cast<QWheelEvent*>(ev);
				float delta = wev->angleDelta().y()*WHEEL_SENSIBILITY;
				vector<int> keys = images.Keys();
				for (int i = 0; i < keys.size(); i++)
					ScaleImage(images.GetCasted(keys[i])->id, delta);
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
				vector<int> keys = images.Keys();
				for (int i = 0; i < keys.size(); i++)
					ScrollImage(images.GetCasted(keys[i])->id, deltaX, deltaY);
				lastMousePos.setX(mev->x());
				lastMousePos.setY(mev->y());
			}
			break;
		default:
			break;
		}

	}
	int SegmentViewer::DealClickEvent(int id,QMouseEvent* ev){
		if (!images.ContainsKey(id) || id != ID_GRAY)
			return -1;
		ImageData* data = images[id];
		QImage image = data->label->pixmap()->toImage();
		uchar bright = qGray(image.pixel(ev->x() / data->scale, ev->y() / data->scale));

		SegmentManager* segMgr = SegmentManager::Instance();
		IplImage* gray = segMgr->GrayImage();

		if (ev->button() == Qt::LeftButton){
			IplImage* division = segMgr->GetThreeDivision(gray, bright, segMgr->BottomValue(), segMgr->TopTolerance(), segMgr->BottomTolerance());
			RegisterImage(ID_DIVISION, division);
			return 0;
		}
		else if (ev->button() == Qt::RightButton){
			IplImage* division = segMgr->GetThreeDivision(gray, segMgr->TopValue(), bright, segMgr->TopTolerance(), segMgr->BottomTolerance());
			RegisterImage(ID_DIVISION, division);
			return 0;
		}
		return -1;
	}

	void SegmentViewer::ScaleImage(int id ,float delta){
		if (!images.ContainsKey(id)){
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
		if (!images.ContainsKey(id)){
			return;
		}
		ImageData* data = images[id];
		data->scroll->horizontalScrollBar()->setValue(data->scroll->horizontalScrollBar()->value() - deltaX);
		data->scroll->verticalScrollBar()->setValue(data->scroll->verticalScrollBar()->value() - deltaY);
	}

	//Protected

	bool SegmentViewer::eventFilter(QObject *obj, QEvent* ev){
		vector<int> keys = images.Keys();
		for (int i = 0; i < keys.size(); i++){
			if (obj == images.GetCasted(keys[i])->scroll->viewport()){
				DealViewEvent(obj, ev);
				return true;
			}
			if (obj == images.GetCasted(keys[i])->label && ev->type() == QEvent::MouseButtonRelease){
				if (DealClickEvent(images.GetCasted(keys[i])->id, (QMouseEvent*)ev) >= 0)
					return false;
			}
		}
		return false;
	}
}