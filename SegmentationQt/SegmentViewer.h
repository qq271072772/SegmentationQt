#ifndef SEGMENTATIONQT_H
#define SEGMENTATIONQT_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qscrollbar.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qevent.h>
#include <qdebug.h>
#include <qpainter.h>
#include "ui_SegmentViewer.h"
#include <opencv2/core/core.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include "Utility.h"
#include "SegmentManager.h"

using namespace Utility;

namespace IS{

	struct ImageData{
		int id;
		QLabel* label;
		QPixmap pixmap; 
		QPixmap pixmap_origin;
		QWidget* pool;
		QScrollArea* scroll;
		float scale = 1;
		int originWidth;
		int originHeight;

		ImageData(int i, QLabel* l, QWidget* p, QScrollArea* s){
			id = i;
			label = l;
			pixmap = QPixmap::fromImage(l->pixmap()->toImage());
			pixmap_origin = pixmap;
			pool = p;
			scroll = s;
			if (l->pixmap() != NULL){
				originWidth = l->pixmap()->width();
				originHeight = l->pixmap()->height();
			}
		}

		ImageData(){

		}
	};

	class SegmentViewer : public QMainWindow
	{
		Q_OBJECT

	public:

		static const int ID_SRC = 0;
		static const int ID_DST = 1;

		static const float SCALE_MIN;
		static const float SCALE_MAX;
		static const float WHEEL_SENSIBILITY;
		static const float SCROLL_SENSIBILITY;

		static SegmentViewer* Instance();

		SegmentViewer(QWidget *parent = 0);
		~SegmentViewer();

		void ReleaseAll();

		void RegisterImage(int id, IplImage* cvImg);

	private:

		char m_filename[_MAX_PATH];

		int GC_ITE_CNT = 10;
		int GC_DOWN_SAMPLE_CNT = 2;
		int GC_POINT_RADIUS = 8;

		enum ViwerMode
		{
			VIEW,GCD_CATCH
		};

		static SegmentViewer* m_instance;
		SegmentManager* m_segMgr;

		Ui::SegmentationQtClass m_ui;

		Dictionary<int, ImageData*> m_images;

		List<QAction*> m_modemenu;
		List<QAction*> m_resolution;

		bool m_mouseDown = false;
		QPoint m_lastMousePos;
		int m_lastMouseButton;

		ViwerMode Mode(){
			if (m_ui.tools_GCD_catchON->isChecked())
				return GCD_CATCH;
			return VIEW;
		}

		void DealViewEvent(int id, QEvent* ev);
		void DealCatchEvent(int id,QEvent* ev);
		void DealModeMenuEvent(QAction* obj, QEvent* ev);
		bool DealToolMenuEvent(QObject* obj, QEvent* ev);
		void DealResolutionMenuEvent(QObject* obj, QEvent* ev);

		void FeatureCatch(int id, QMouseEvent* ev);
		void GCD_Catch(int id, QMouseEvent* ev, bool isPr);

		void ScaleImage(int id, float delta);
		void ScrollImage(int id, float deltaX, float deltaY);

		void DrawPoint(int id,int x,int y,int color);

		void OpenFile();
		void ClearPaint(int id);
		void GenerateGrabCut();

		void SetActionChecked(QAction* obj,bool flag){
			obj->removeEventFilter(this);
			obj->setChecked(flag);
			obj->installEventFilter(this);
		}


	protected:
		bool eventFilter(QObject *obj, QEvent* ev);
	};
}

#endif // SEGMENTATIONQT_H
