/********************************************************************************
** Form generated from reading UI file 'SegmentViewer.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGMENTVIEWER_H
#define UI_SEGMENTVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SegmentationQtClass
{
public:
    QWidget *centralWidget;
    QScrollArea *scrollview_images;
    QWidget *scrollAreaWidgetContents;
    QLabel *label_gray;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SegmentationQtClass)
    {
        if (SegmentationQtClass->objectName().isEmpty())
            SegmentationQtClass->setObjectName(QStringLiteral("SegmentationQtClass"));
        SegmentationQtClass->resize(903, 542);
        centralWidget = new QWidget(SegmentationQtClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        scrollview_images = new QScrollArea(centralWidget);
        scrollview_images->setObjectName(QStringLiteral("scrollview_images"));
        scrollview_images->setGeometry(QRect(10, 8, 881, 461));
        scrollview_images->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollview_images->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollview_images->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 856, 436));
        label_gray = new QLabel(scrollAreaWidgetContents);
        label_gray->setObjectName(QStringLiteral("label_gray"));
        label_gray->setGeometry(QRect(240, 30, 451, 451));
        label_gray->setScaledContents(true);
        label_gray->setAlignment(Qt::AlignCenter);
        scrollview_images->setWidget(scrollAreaWidgetContents);
        SegmentationQtClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SegmentationQtClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 903, 27));
        SegmentationQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SegmentationQtClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SegmentationQtClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SegmentationQtClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SegmentationQtClass->setStatusBar(statusBar);

        retranslateUi(SegmentationQtClass);

        QMetaObject::connectSlotsByName(SegmentationQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *SegmentationQtClass)
    {
        SegmentationQtClass->setWindowTitle(QApplication::translate("SegmentationQtClass", "SegmentationQt", 0));
        label_gray->setText(QApplication::translate("SegmentationQtClass", "Label_Gray", 0));
    } // retranslateUi

};

namespace Ui {
    class SegmentationQtClass: public Ui_SegmentationQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGMENTVIEWER_H
