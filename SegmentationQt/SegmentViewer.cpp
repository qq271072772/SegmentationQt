#include "SegmentViewer.h"

namespace IS{

	//Constructor
	SegmentViewer::SegmentViewer(QWidget *parent)
		: QMainWindow(parent)
	{
		m_ui.setupUi(this);
		m_ui.scrollview_gray->viewport()->installEventFilter(this);
		m_ui.scrollview_division->viewport()->installEventFilter(this);
		m_ui.label_gray->installEventFilter(this);

		m_modemenu.Add(m_ui.tools_GCD_catchON);
		m_ui.file_open->installEventFilter(this);
		m_ui.tools_GCD_catchON->installEventFilter(this);
		m_ui.tools_GCD_Clear->installEventFilter(this);
		m_ui.tools_GCD_Genrate->installEventFilter(this);

		m_ui.action_res_1_x->installEventFilter(this);
		m_ui.action_res_1_2_x->installEventFilter(this);
		m_ui.action_res_1_4_x->installEventFilter(this);
		m_ui.action_res_1_8_x->installEventFilter(this);
		m_resolution.Add(m_ui.action_res_1_x);
		m_resolution.Add(m_ui.action_res_1_2_x);
		m_resolution.Add(m_ui.action_res_1_4_x);
		m_resolution.Add(m_ui.action_res_1_8_x);

		m_segMgr = SegmentManager::Instance();
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
		if (m_instance == NULL){
			m_instance = new SegmentViewer();
		}
		return m_instance;
	}

	/*Register origin and new image*/
	void SegmentViewer::RegisterImage(int id, IplImage* cvImg){

		if (cvImg == NULL)
			return;

		float oldScale = 1;
		if (m_images.ContainsKey(id)){
			oldScale = m_images[id]->scale;
			delete m_images[id];
			m_images.Remove(id);
		}

		IplImage* cp = ImageHelper::CreateCopy(cvImg);
		if (cvImg->nChannels == 3)
			cvCvtColor(cp, cp, CV_BGR2RGB);

		QImage::Format fomat = cvImg->nChannels == 1 ? QImage::Format_Grayscale8 : QImage::Format_RGB888;
		QImage qImg = QImage((uchar*)cp->imageData, cp->width, cp->height, cp->widthStep, fomat);
		switch (id)
		{
		case ID_SRC:
			m_ui.label_gray->setFixedSize(qImg.width(), qImg.height());
			m_ui.label_gray->setPixmap(QPixmap::fromImage(qImg));
			m_ui.pool_gray->resize(qImg.width(), qImg.height());
			m_images.Add(id,new ImageData(id, m_ui.label_gray, m_ui.pool_gray, m_ui.scrollview_gray));
			break;
		case ID_DST:
			m_ui.label_division->setFixedSize(qImg.width(), qImg.height());
			m_ui.label_division->setPixmap(QPixmap::fromImage(qImg));
			m_ui.pool_division->resize(qImg.width(), qImg.height());
			m_images.Add(id, new ImageData(id, m_ui.label_division, m_ui.pool_division, m_ui.scrollview_division));
			break;
		default:
			break;
		}
		if (oldScale != 1)
			ScaleImage(id, oldScale - 1);
		ImageHelper::ReleaseImage(&cp);
	}
	void SegmentViewer::ReleaseAll(){
		List<int> keys = m_images.Keys();
		for (int i = 0; i < keys.Count(); i++){
			delete m_images[keys[i]];
		}
		m_images.Clear();
		delete this;
	}

	//Private

	SegmentViewer* SegmentViewer::m_instance = NULL;

	void SegmentViewer::DealViewEvent(int id, QEvent* ev){
		switch (ev->type()){
		case QEvent::Wheel:
		{
			QWheelEvent* wev = static_cast<QWheelEvent*>(ev);
			float delta = wev->angleDelta().y()*WHEEL_SENSIBILITY;
			List<int> keys = m_images.Keys();
			for (int i = 0; i < keys.Count(); i++)
				ScaleImage(m_images[keys[i]]->id, delta);
		}
		break;
		case QEvent::MouseButtonPress:
		{
			QMouseEvent* mev = (QMouseEvent*)ev;
			m_mouseDown = true;
			m_lastMousePos.setX(mev->x());
			m_lastMousePos.setY(mev->y());
		}
		break;
		case QEvent::MouseButtonRelease:
			m_mouseDown = false;
			break;
		case QEvent::MouseMove:
			if (m_mouseDown){
				QMouseEvent* mev = (QMouseEvent*)ev;
				float deltaX = (mev->x() - m_lastMousePos.x())*SCROLL_SENSIBILITY;
				float deltaY = (mev->y() - m_lastMousePos.y())*SCROLL_SENSIBILITY;
				List<int> keys = m_images.Keys();
				for (int i = 0; i < keys.Count(); i++)
					ScrollImage(m_images[keys[i]]->id, deltaX, deltaY);
				m_lastMousePos.setX(mev->x());
				m_lastMousePos.setY(mev->y());
			}
			break;
		default:
			break;
		}

	}
	void SegmentViewer::DealCatchEvent(int id, QEvent* ev){
		if (!m_images.ContainsKey(id))
			return;

		switch (Mode())
		{
		case GCD_CATCH:
			switch (ev->type())
			{
			case  QEvent::MouseButtonPress:
				m_mouseDown = true;
				m_lastMouseButton = ((QMouseEvent*)ev)->button();
				GCD_Catch(id, (QMouseEvent*)ev, Mode() != GCD_CATCH);
				break;
			case QEvent::MouseMove:
				if (m_mouseDown)
					GCD_Catch(id, (QMouseEvent*)ev, Mode() != GCD_CATCH);
				break;
			case QEvent::MouseButtonRelease:
				m_mouseDown = false;
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
			for (int i = 0; i < m_modemenu.Count(); i++){
				if (obj != m_modemenu[i])
					SetActionChecked(m_modemenu[i], false);
			}
		}
	}
	bool SegmentViewer::DealToolMenuEvent(QObject *obj, QEvent* ev){
		if (ev->type() == QEvent::ActionChanged){
			if (obj == m_ui.file_open){
				OpenFile();
				SetActionChecked(m_ui.file_open, false);
				return true;
			}
			if (obj == m_ui.tools_GCD_Clear){
				if (m_images.ContainsKey(ID_SRC))
					ClearPaint(ID_SRC);
				SetActionChecked(m_ui.tools_GCD_Clear, false);
				return true;
			}
			if (obj == m_ui.tools_GCD_Genrate){
				GenerateGrabCut();
				SetActionChecked(m_ui.tools_GCD_Genrate, false);
				return true;
			}
		}
		return false;
	}
	void SegmentViewer::DealResolutionMenuEvent(QObject *obj, QEvent* ev){
		if (ev->type() == QEvent::ActionChanged){
			for (int i = 0; i < m_resolution.Count(); i++){
				if (obj == m_resolution[i])
					GC_DOWN_SAMPLE_CNT = i ;
				SetActionChecked(m_resolution[i], obj == m_resolution[i]);
			}
		}
	}

	void SegmentViewer::ScaleImage(int id, float delta){
		if (!m_images.ContainsKey(id)){
			//DebugLog((char*)("Image id not exist(id:" + id + ')'));
			return;
		}
		ImageData* data = m_images[id];
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
		if (!m_images.ContainsKey(id)){
			return;
		}
		ImageData* data = m_images[id];
		data->scroll->horizontalScrollBar()->setValue(data->scroll->horizontalScrollBar()->value() - deltaX);
		data->scroll->verticalScrollBar()->setValue(data->scroll->verticalScrollBar()->value() - deltaY);
	}

	void SegmentViewer::GCD_Catch(int id, QMouseEvent* ev, bool isPr){
		ImageData* data = m_images[id];
		int realX = ev->x() /data->scale;
		int realY = ev->y()/data->scale;
		if (m_lastMouseButton == Qt::LeftButton){
			DrawPoint(id, realX, realY, isPr ? Qt::darkRed : Qt::red);
			m_segMgr->DrawMaskPoint(Vector2(realX, realY), GC_POINT_RADIUS, isPr ? cv::GC_PR_FGD : cv::GC_FGD);
		}
		else if (m_lastMouseButton == Qt::RightButton){
			DrawPoint(id, realX, realY, isPr ? Qt::darkBlue : Qt::blue);
			m_segMgr->DrawMaskPoint(Vector2(realX, realY), GC_POINT_RADIUS, isPr ? cv::GC_PR_BGD : cv::GC_BGD);
		}
	}

	void SegmentViewer::DrawPoint(int id, int x,int y,int color){
		ImageData* data = m_images[id];
		QImage image = data->pixmap.toImage(); 
		QPainter painter;
		painter.begin(&image);
		painter.setBrush(QBrush((Qt::GlobalColor)color));
		painter.setPen(QPen((Qt::GlobalColor)color));
		painter.drawEllipse(x, y, GC_POINT_RADIUS * 2, GC_POINT_RADIUS * 2);
		painter.end();
		data->pixmap = QPixmap::fromImage(image);
		data->label->setPixmap(data->pixmap.scaled(data->label->pixmap()->size()));
	}

	void SegmentViewer::OpenFile(){
		QString filename = QFileDialog::getOpenFileName(this,
			tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
		if (!filename.isEmpty()){
			QByteArray ba = filename.toLatin1();
			char *c_filename = ba.data();
			strcpy_s(m_filename, c_filename);
			m_segMgr->LoadSrcImage(c_filename);
			RegisterImage(SegmentViewer::ID_SRC, m_segMgr->SrcImage());
			RegisterImage(SegmentViewer::ID_DST, m_segMgr->DstImage());
			ClearPaint(ID_SRC);
			for (int i = 0; i < m_modemenu.Count(); i++){
				SetActionChecked(m_modemenu[i], false);
			}
		}
	}
	void SegmentViewer::ClearPaint(int id){
		ImageData* data = m_images[id];
		data->pixmap = data->pixmap_origin;
		data->label->setPixmap(data->pixmap.scaled(data->label->pixmap()->size()));
		m_segMgr->ClearMask();
	}
	void SegmentViewer::GenerateGrabCut(){
		if (m_segMgr->SrcImage() == NULL)
			return;
		QMessageBox* msgBox=new QMessageBox(this);
		msgBox->setAttribute(Qt::WA_DeleteOnClose);
		msgBox->setWindowTitle(tr("Processing..."));
		msgBox->show();
		m_segMgr->GenerateGrabCut(GC_ITE_CNT, GC_DOWN_SAMPLE_CNT);
		m_segMgr->SaveDstImage(m_filename);
		RegisterImage(ID_DST, m_segMgr->DstImage());
		msgBox->close();
	}

	//Protected

	bool SegmentViewer::eventFilter(QObject *obj, QEvent* ev){
		switch (Mode())
		{
		case VIEW:{
			List<int> keys = m_images.Keys();
			for (int i = 0; i < keys.Count(); i++){
				if (obj == m_images[keys[i]]->scroll->viewport()){
					DealViewEvent(m_images[keys[i]]->id, ev);
					return true;
				}
			}
		}
			break;
		case GCD_CATCH:{
			List<int> keys = m_images.Keys();
			for (int i = 0; i < keys.Count(); i++){
				if (obj == m_images[keys[i]]->label){
					DealCatchEvent(m_images[keys[i]]->id, ev);
					return false;
				}
			}
		}
			break;
		default:
			break;
		}
		for (int i = 0; i < m_modemenu.Count(); i++)
			if (m_modemenu[i] == obj){
				DealModeMenuEvent((QAction*)obj, ev);
				return false;
			}
		for (int i = 0; i < m_resolution.Count(); i++)
			if (m_resolution[i] == obj){
				DealResolutionMenuEvent((QAction*)obj, ev);
				return true;
			}
		if (DealToolMenuEvent(obj, ev))
			return true;
		return false;
	}
}