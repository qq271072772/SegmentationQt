#include"SegmentManager.h"
#include "SegmentViewer.h"
#include <QtWidgets/QApplication>

int main(int argCnt, char** args) {

	using namespace IS;

	if (argCnt != 2 && !SegmentManager::debugModeOn)
		return 0;

	SegmentManager* segMgr = SegmentManager::Instance();
	IplImage* src = segMgr->LoadImage(SegmentManager::debugModeOn ? "E:\\Project\\SegmentationQt\\Assets\\1.png" : args[1]);
	IplImage* gray = segMgr->ConvertToGrayImage(src);
	IplImage* division = segMgr->GetThreeDivision(gray, 50, 100, 30, 20);

	QApplication app(argCnt, args);

	SegmentViewer* segViewer = SegmentViewer::Instance();

	segViewer->RegisterImage(SegmentViewer::ID_GRAY, gray);
	segViewer->RegisterImage(SegmentViewer::ID_DIVISION, division);

	segViewer->show();

	app.exec();

	segViewer->ReleaseAll();
	segMgr->ReleaseAll();
}