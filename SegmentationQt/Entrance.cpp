#include"SegmentManager.h"
#include "SegmentViewer.h"
#include <QtWidgets/QApplication>

int main(int argCnt, char** args) {
	using namespace IS;

	QApplication app(argCnt, args);

	SegmentViewer* segViewer = SegmentViewer::Instance();
	SegmentManager* segMgr = SegmentManager::Instance();

	segViewer->show();

	app.exec();

	segViewer->ReleaseAll();
	segMgr->ReleaseAll();
}