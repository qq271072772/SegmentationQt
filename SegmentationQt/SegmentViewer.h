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
		static const int ID_GRAY = 1;
		static const int ID_DIVISION = 2;

		static const int GCD_POINT_RADIUS = 3;
		static const int GCD_POINT_THICKNESS = 0;

		static const float SCALE_MIN;
		static const float SCALE_MAX;
		static const float WHEEL_SENSIBILITY;
		static const float SCROLL_SENSIBILITY;

		static SegmentViewer* Instance();

		SegmentViewer(QWidget *parent = 0);
		~SegmentViewer();

		void ReleaseAll();

		void RegisterImage(int id, IplImage* cvImg);

		List<QPoint> FgdPixels(){
			return fgdPixels;
		}
		List<QPoint> BgdPixels(){
			return bgdPixels;
		}
		List<QPoint> PrFgdPixels(){
			return prFgdPixels;
		}
		List<QPoint> PrBgdPixels(){
			return prBgdPixels;
		}

	private:

		enum ViwerMode
		{
			VIEW,FEATURE_CATCH , GCD_CATCH, GCD_PR_CATCH
		};

		static SegmentViewer* instance;

		Ui::SegmentationQtClass ui;

		Dictionary<int, ImageData*> images;

		List<QAction*> modemenu;

		List<QPoint> fgdPixels, bgdPixels, prFgdPixels, prBgdPixels;

		bool mouseDown = false;
		QPoint lastMousePos;
		int lastMouseButton;

		ViwerMode Mode(){
			if (ui.tools_featureCatchOn->isChecked())
				return FEATURE_CATCH;
			if (ui.tools_GCD_catchON->isChecked())
				return GCD_CATCH;
			if (ui.tools_GCD_PR_catchON->isChecked())
				return GCD_PR_CATCH;
			return VIEW;
		}

		void DealViewEvent(int id, QEvent* ev);
		void DealCatchEvent(int id,QEvent* ev);
		void DealModeMenuEvent(QAction* obj, QEvent* ev);
		bool DealToolMenuEvent(QObject* obj, QEvent* ev);

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
