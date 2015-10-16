#ifndef SEGMENTATIONQT_H
#define SEGMENTATIONQT_H

#include <QtWidgets/QMainWindow>
#include <qevent.h>
#include <QtWidgets/qscrollbar.h>
#include <qdebug.h>
#include "ui_SegmentViewer.h"
#include <opencv2/core/core.hpp> 
#include <vector>
#include "Utility.h"

namespace IS{

	struct ImageData{
		int id;
		QLabel* label;
		QWidget* pool;
		QScrollArea* scroll;
		float scale = 1;
		int originWidth;
		int originHeight;

		ImageData(int i,QLabel* l, QWidget* p,QScrollArea* s){
			id = i;
			label = l;
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

		static SegmentViewer* instance;

		Ui::SegmentationQtClass ui;

		Dictionary<int, ImageData*> images;

		void DealViewEvent(QObject *obj, QEvent* ev);
		int DealClickEvent(int id,QMouseEvent* ev);

		void ScaleImage(int id, float delta);
		void ScrollImage(int id, float deltaX, float deltaY);

		bool mouseDown = false;
		QPoint lastMousePos;

	protected:
		bool eventFilter(QObject *obj, QEvent* ev);
	};
}

#endif // SEGMENTATIONQT_H
