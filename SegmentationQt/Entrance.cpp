#include"SegmentManager.h"
#include "SegmentViewer.h"
#include <QtWidgets/QApplication>

int main(int argCnt, char** args) {

	using namespace IS;

	if (argCnt != 2 && !SegmentManager::debugModeOn)
		return 0;

	SegmentManager* segMgr = SegmentManager::Instance();
	IplImage* src = segMgr->LoadImage(SegmentManager::debugModeOn ? "E:\\Project\\SegmentationQt\\Assets\\2.jpeg" : args[1]);
	IplImage* gray = segMgr->ConvertToGrayImage(src);
	IplImage* division = segMgr->GetThreeDivision(gray, 50, 100, 30, 20);
	//segMgr->ShowImageWin((char*)(segMgr->SRC_WIN), src);
	//segMgr->ShowImageWin((char*)(segMgr->GRAY_WIN), gray);
	//segMgr->ShowImageWin((char*)(segMgr->DIVISION_WIN), division);
	//segMgr->RegisterGrayWinEvent((char*)(segMgr->GRAY_WIN));
	//segMgr->WaitKey(0);
	//segMgr->ReleaseAll();

	QApplication app(argCnt, args);

	SegmentViewer* segViewer = SegmentViewer::Instance();

	segViewer->RegisterImage(SegmentViewer::ID_GRAY, gray);
	segViewer->RegisterImage(SegmentViewer::ID_DIVISION, division);

	segViewer->show();

	app.exec();

	segViewer->ReleaseAll();
	segMgr->ReleaseAll();
}