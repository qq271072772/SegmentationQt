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

		modemenu.Add(ui.tools_GCD_catchON);
		ui.file_open->installEventFilter(this);
		ui.tools_GCD_catchON->installEventFilter(this);
		ui.tools_GCD_Clear->installEventFilter(this);
		ui.tools_GCD_Genrate->installEventFilter(this);
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
			oldScale = images[id]->scale;
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

			images.Add(id,new ImageData(id, ui.label_gray, ui.pool_gray, ui.scrollview_gray));
			break;
		case ID_GRAY:{
			qImg = QImage((uchar*)cvImg->imageData, cvImg->width, cvImg->height, cvImg->widthStep, QImage::Format_Grayscale8);

			ui.label_gray->setFixedSize(qImg.width(), qImg.height());
			ui.label_gray->setPixmap(QPixmap::fromImage(qImg));

			ui.pool_gray->resize(qImg.width(), qImg.height());

			images.Add(id, new ImageData(id, ui.label_gray, ui.pool_gray, ui.scrollview_gray));
		}
					 break;
		case ID_DIVISION:
			qImg = QImage((uchar*)cvImg->imageData, cvImg->width, cvImg->height, cvImg->widthStep, QImage::Format_Grayscale8);
			ui.label_division->setFixedSize(qImg.width(), qImg.height());
			ui.label_division->setPixmap(QPixmap::fromImage(qImg));
			ui.pool_division->resize(qImg.width(), qImg.height());
			images.Add(id, new ImageData(id, ui.label_division, ui.pool_division, ui.scrollview_division));
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
				ScaleImage(images[keys[i]]->id, delta);
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
					ScrollImage(images[keys[i]]->id, deltaX, deltaY);
				lastMousePos.setX(mev->x());
				lastMousePos.setY(mev->y());
			}
			break;
		default:
			break;
		}

	}
	void SegmentViewer::DealCatchEvent(int id, QEvent* ev){
		if (!images.ContainsKey(id))
			return;

		switch (Mode())
		{
		//case FEATURE_CATCH:
		//	if (ev->type() == QEvent::MouseButtonRelease)
		//		FeatureCatch(id, (QMouseEvent*)ev);
		//	break;
		case GCD_CATCH:
			switch (ev->type())
			{
			case  QEvent::MouseButtonPress:
				mouseDown = true;
				lastMouseButton = ((QMouseEvent*)ev)->button();
				GCD_Catch(id, (QMouseEvent*)ev, Mode() != GCD_CATCH);
				break;
			case QEvent::MouseMove:
				if (mouseDown)
					GCD_Catch(id, (QMouseEvent*)ev, Mode() != GCD_CATCH);
				break;
			case QEvent::MouseButtonRelease:
				mouseDown = false;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	void SegmentViewer::DealModeMenuEvent(QAction* obj, QEvent* ev){
		if (ev->type() == QEvent::ActionChanged){
			for (int i = 0; i < modemenu.Count(); i++){
				if (obj != modemenu[i])
					SetActionChecked(modemenu[i], false);
			}
		}
	}
	bool SegmentViewer::DealToolMenuEvent(QObject *obj, QEvent* ev){
		if (ev->type() == QEvent::ActionChanged){
			if (obj == ui.file_open){
				OpenFile();
				SetActionChecked(ui.file_open, false);
				return true;
			}
			if (obj == ui.tools_GCD_Clear){
				if (images.ContainsKey(ID_GRAY))
					ClearPaint(ID_GRAY);
				if (images.ContainsKey(ID_SRC))
					ClearPaint(ID_SRC);
				SetActionChecked(ui.tools_GCD_Clear, false);
				return true;
			}
			if (obj == ui.tools_GCD_Genrate){
				GenerateGrabCut();
				SetActionChecked(ui.tools_GCD_Genrate, false);
				return true;
			}
		}
		return false;
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
		data->label->setPixmap(data->pixmap.scaled(newWidth, newHeight));
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

	//This function is removed
	void SegmentViewer::FeatureCatch(int id, QMouseEvent* ev){
		//id = ID_GRAY;
		//ImageData* data = images[id];
		//QImage image = data->pixmap.toImage();
		//uchar bright = qGray(image.pixel(ev->x() / data->scale, ev->y() / data->scale));

		//SegmentManager* segMgr = SegmentManager::Instance();
		//IplImage* gray = segMgr->GrayImage();

		//if (ev->button() == Qt::LeftButton){
		//	IplImage* division = segMgr->GetThreeDivision(gray, bright, segMgr->BottomValue(), segMgr->TopTolerance(), segMgr->BottomTolerance());
		//	RegisterImage(ID_DIVISION, division);
		//	ui.tools_featureCatchOn->setChecked(false);
		//}
		//else if (ev->button() == Qt::RightButton){
		//	IplImage* division = segMgr->GetThreeDivision(gray, segMgr->TopValue(), bright, segMgr->TopTolerance(), segMgr->BottomTolerance());
		//	RegisterImage(ID_DIVISION, division);
		//	ui.tools_featureCatchOn->setChecked(false);
		//}
	}

	void SegmentViewer::GCD_Catch(int id, QMouseEvent* ev, bool isPr){
		ImageData* data = images[id];
		int realX = ev->x() /data->scale;
		int realY = ev->y()/data->scale;
		if (lastMouseButton == Qt::LeftButton){
			if (!isPr){
				DrawPoint(id, realX, realY, Qt::red);
				fgdPixels.Add(QPoint(realX, realY));
			}
			else{
				DrawPoint(id, realX, realY, Qt::darkRed);
				prFgdPixels.Add(QPoint(realX, realY));
			}
		}
		else if (lastMouseButton == Qt::RightButton){
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
		QImage image = data->pixmap.toImage(); 
		QPainter painter;
		painter.begin(&image);
		painter.setBrush(QBrush((Qt::GlobalColor)color));
		painter.setPen(QPen((Qt::GlobalColor)color));
		painter.drawEllipse(x, y, GCD_POINT_RADIUS * 2, GCD_POINT_RADIUS * 2);
		painter.end();
		data->pixmap = QPixmap::fromImage(image);
		data->label->setPixmap(data->pixmap.scaled(data->label->pixmap()->size()));
	}

	void SegmentViewer::OpenFile(){
		SegmentManager* segMgr = SegmentManager::Instance();
		QString filename = QFileDialog::getOpenFileName(this,
			tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
		if (!filename.isEmpty()){
			QByteArray ba = filename.toLatin1();
			char *c_filename = ba.data();
			segMgr->LoadSrcImage(c_filename);
			RegisterImage(SegmentViewer::ID_SRC, segMgr->SrcImage());
			RegisterImage(SegmentViewer::ID_DIVISION, segMgr->RetImage());
		}
	}
	void SegmentViewer::ClearPaint(int id){
		ImageData* data = images[id];
		data->pixmap = data->pixmap_origin;
		data->label->setPixmap(data->pixmap.scaled(data->label->pixmap()->size()));
		fgdPixels.Clear();
		bgdPixels.Clear();
		prFgdPixels.Clear();
		prBgdPixels.Clear();
	}
	void SegmentViewer::GenerateGrabCut(){
		if (fgdPixels.Count() == 0)
			return;
		SegmentManager* segMgr = SegmentManager::Instance();
		if (segMgr->SrcImage() == NULL)
			return;
		QMessageBox* msgBox=new QMessageBox(this);
		msgBox->setAttribute(Qt::WA_DeleteOnClose);
		msgBox->setWindowTitle(tr("Processing..."));
		msgBox->show();
		IplImage* division = segMgr->GetGrabCut(segMgr->SrcImage());
		RegisterImage(ID_DIVISION, division);
		segMgr->SaveDstImage(NULL);
		msgBox->close();
	}

	//Protected

	bool SegmentViewer::eventFilter(QObject *obj, QEvent* ev){
		switch (Mode())
		{
		case VIEW:{
			List<int> keys = images.Keys();
			for (int i = 0; i < keys.Count(); i++){
				if (obj == images[keys[i]]->scroll->viewport()){
					DealViewEvent(images[keys[i]]->id, ev);
					return true;
				}
			}
		}
			break;
		case GCD_CATCH:{
			List<int> keys = images.Keys();
			for (int i = 0; i < keys.Count(); i++){
				if (obj == images[keys[i]]->label){
					DealCatchEvent(images[keys[i]]->id, ev);
					return false;
				}
			}
		}
			break;
		default:
			break;
		}
		for (int i = 0; i < modemenu.Count(); i++)
			if (modemenu[i] == obj){
				DealModeMenuEvent((QAction*)obj, ev);
				return false;
			}
		if (DealToolMenuEvent(obj, ev))
			return true;
		return false;
	}
}