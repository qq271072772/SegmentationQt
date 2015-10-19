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
		ui.scrollview_gray->horizontalScrollBar()->setEnabled(false);
		ui.scrollview_gray->verticalScrollBar()->setEnabled(false);
		ui.scrollview_division->horizontalScrollBar()->setEnabled(false);
		ui.scrollview_division->verticalScrollBar()->setEnabled(false);
		ui.label_gray->installEventFilter(this);

		toolmenu.Add(ui.tools_featureCatchOn);
		toolmenu.Add(ui.tools_GCD_catchON);
		toolmenu.Add(ui.tools_GCD_PR_catchON);
		ui.tools_featureCatchOn->installEventFilter(this);
		ui.tools_GCD_catchON->installEventFilter(this);
		ui.tools_GCD_PR_catchON->installEventFilter(this);
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

			ui.label_gray->setFixedSize(qImg.width(), qImg.height());
			ui.label_gray->setPixmap(QPixmap::fromImage(qImg));


			ui.pool_gray->resize(qImg.width(), qImg.height());

			images[id] = new ImageData(id, ui.label_gray, ui.pool_gray, ui.scrollview_gray);
			break;
		case ID_GRAY:{
			qImg = QImage((uchar*)cvImg->imageData, cvImg->width, cvImg->height, cvImg->widthStep, QImage::Format_Grayscale8);

			ui.label_gray->setFixedSize(qImg.width(), qImg.height());
			ui.label_gray->setPixmap(QPixmap::fromImage(qImg));

			ui.pool_gray->resize(qImg.width(), qImg.height());

			images[id] = new ImageData(id, ui.label_gray, ui.pool_gray, ui.scrollview_gray);
		}
					 break;
		case ID_DIVISION:
			qImg = QImage((uchar*)cvImg->imageData, cvImg->width, cvImg->height, cvImg->widthStep, QImage::Format_Grayscale8);
			ui.label_division->setFixedSize(qImg.width(), qImg.height());
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
		List<int> keys = images.Keys();
		for (int i = 0; i < keys.Count(); i++){
			delete images[keys[i]];
		}
		images.Clear();
		delete this;
	}

	//Private

	SegmentViewer* SegmentViewer::instance = NULL;

	void SegmentViewer::DealViewEvent(int id, QEvent* ev){
		switch (ev->type()){
		case QEvent::Wheel:
		{
			QWheelEvent* wev = static_cast<QWheelEvent*>(ev);
			float delta = wev->angleDelta().y()*WHEEL_SENSIBILITY;
			List<int> keys = images.Keys();
			for (int i = 0; i < keys.Count(); i++)
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
				List<int> keys = images.Keys();
				for (int i = 0; i < keys.Count(); i++)
					ScrollImage(images.GetCasted(keys[i])->id, deltaX, deltaY);
				lastMousePos.setX(mev->x());
				lastMousePos.setY(mev->y());
			}
			break;
		default:
			break;
		}

	}
	void SegmentViewer::DealClickEvent(int id, QMouseEvent* ev){
		if (!images.ContainsKey(id))
			return;

		if (ui.tools_featureCatchOn->isChecked())
			FeatureCatch(id, ev);
		if (ui.tools_GCD_catchON->isChecked())
			GCD_Catch(id, ev, false);
		if (ui.tools_GCD_PR_catchON->isChecked())
			GCD_Catch(id, ev, true);
	}
	void SegmentViewer::DealToolMenuEvent(QAction* obj, QEvent* ev){
		if (ev->type() == QEvent::ActionChanged){
			for (int i = 0; i < toolmenu.Count(); i++){
				toolmenu.Get(i)->removeEventFilter(this);
				if (obj != toolmenu[i])
					toolmenu.Get(i)->setChecked(false);
				toolmenu.Get(i)->installEventFilter(this);
			}
		}
	}

	void SegmentViewer::ScaleImage(int id, float delta){
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
		data->label->setFixedSize(newWidth, newHeight);
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

	void SegmentViewer::FeatureCatch(int id, QMouseEvent* ev){
		id = ID_GRAY;
		ImageData* data = images[id];
		QImage image = data->label->pixmap()->toImage();
		uchar bright = qGray(image.pixel(ev->x() / data->scale, ev->y() / data->scale));

		SegmentManager* segMgr = SegmentManager::Instance();
		IplImage* gray = segMgr->GrayImage();

		if (ev->button() == Qt::LeftButton){
			IplImage* division = segMgr->GetThreeDivision(gray, bright, segMgr->BottomValue(), segMgr->TopTolerance(), segMgr->BottomTolerance());
			RegisterImage(ID_DIVISION, division);
			ui.tools_featureCatchOn->setChecked(false);
		}
		else if (ev->button() == Qt::RightButton){
			IplImage* division = segMgr->GetThreeDivision(gray, segMgr->TopValue(), bright, segMgr->TopTolerance(), segMgr->BottomTolerance());
			RegisterImage(ID_DIVISION, division);
			ui.tools_featureCatchOn->setChecked(false);
		}
	}
	void SegmentViewer::GCD_Catch(int id, QMouseEvent* ev, bool isPr){
		ImageData* data = images[id];
		int realX = ev->x() / data->scale;
		int realY = ev->y() / data->scale;
		if (ev->button() == Qt::LeftButton){
			if (!isPr){
				DrawPoint(id, realX, realY, Qt::red);
				fgdPixels.Add(QPoint(realX, realY));
			}
			else{
				DrawPoint(id, realX, realY, Qt::darkRed);
				prFgdPixels.Add(QPoint(realX, realY));
			}
		}
		else if (ev->button() == Qt::RightButton){
			if (!isPr){
				DrawPoint(id, realX, realY, Qt::blue);
				bgdPixels.Add(QPoint(realX, realY));
			}
			else{
				DrawPoint(id, realX, realY, Qt::darkBlue);
				prBgdPixels.Add(QPoint(realX, realY));
			}
		}
	}

	void SegmentViewer::DrawPoint(int id, int x,int y,int color){
		ImageData* data = images[id];
		QImage image = data->label->pixmap()->toImage();
		QPainter painter;
		painter.begin(&image);
		painter.setBrush(QBrush((Qt::GlobalColor)color));
		painter.setPen(QPen((Qt::GlobalColor)color));
		painter.drawEllipse(x, y, GCD_POINT_RADIUS * 2, GCD_POINT_RADIUS * 2);
		painter.end();
		data->label->setPixmap(QPixmap::fromImage(image));
	}
	void SegmentViewer::ResetPaint(int id){

	}

	//Protected

	bool SegmentViewer::eventFilter(QObject *obj, QEvent* ev){
		List<int> keys = images.Keys();
		for (int i = 0; i < keys.Count(); i++){
			if (obj == images.GetCasted(keys[i])->scroll->viewport()){
				DealViewEvent(images.GetCasted(keys[i])->id, ev);
				return true;
			}
			if (obj == images.GetCasted(keys[i])->label && ev->type() == QEvent::MouseButtonRelease){
				DealClickEvent(images.GetCasted(keys[i])->id, (QMouseEvent*)ev);
				return true;
			}
		}
		for (int i = 0; i < toolmenu.Count();i++)
			if (toolmenu[i] == obj){
				DealToolMenuEvent((QAction*)obj, ev);
				return false;
			}
		return false;
	}
}