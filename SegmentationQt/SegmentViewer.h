#ifndef SEGMENTATIONQT_H
#define SEGMENTATIONQT_H

#include <QtWidgets/QMainWindow>
#include "ui_SegmentViewer.h"
#include <opencv2/core/core.hpp> 
#include <vector>

namespace IS{
	class SegmentViewer : public QMainWindow
	{
		Q_OBJECT

	public:

		static const int ID_GRAY = 0;
		static const int ID_DIVISION = 1;

		static SegmentViewer* Instance();

		SegmentViewer(QWidget *parent = 0);
		~SegmentViewer();

		void ReleaseAll();

		void ShowImage(int id, IplImage* cvImg);

	private:

		static SegmentViewer* instance;

		Ui::SegmentationQtClass ui;

		std::vector<QImage*> activeImgs;
	};
}

#endif // SEGMENTATIONQT_H
