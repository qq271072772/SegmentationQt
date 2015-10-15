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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SegmentationQtClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollview_gray;
    QWidget *pool_gray;
    QLabel *label_gray;
    QScrollArea *scrollview_division;
    QWidget *pool_division;
    QLabel *label_division;
    QLabel *label_debug;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SegmentationQtClass)
    {
        if (SegmentationQtClass->objectName().isEmpty())
            SegmentationQtClass->setObjectName(QStringLiteral("SegmentationQtClass"));
        SegmentationQtClass->resize(1371, 801);
        centralWidget = new QWidget(SegmentationQtClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        scrollview_gray = new QScrollArea(centralWidget);
        scrollview_gray->setObjectName(QStringLiteral("scrollview_gray"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollview_gray->sizePolicy().hasHeightForWidth());
        scrollview_gray->setSizePolicy(sizePolicy);
        scrollview_gray->setMaximumSize(QSize(65536, 65536));
        scrollview_gray->setFocusPolicy(Qt::WheelFocus);
        scrollview_gray->setFrameShape(QFrame::StyledPanel);
        scrollview_gray->setFrameShadow(QFrame::Sunken);
        scrollview_gray->setLineWidth(1);
        scrollview_gray->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollview_gray->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollview_gray->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        scrollview_gray->setWidgetResizable(false);
        scrollview_gray->setAlignment(Qt::AlignCenter);
        pool_gray = new QWidget();
        pool_gray->setObjectName(QStringLiteral("pool_gray"));
        pool_gray->setGeometry(QRect(15, 111, 640, 480));
        label_gray = new QLabel(pool_gray);
        label_gray->setObjectName(QStringLiteral("label_gray"));
        label_gray->setGeometry(QRect(0, 0, 640, 480));
        label_gray->setScaledContents(true);
        label_gray->setAlignment(Qt::AlignCenter);
        scrollview_gray->setWidget(pool_gray);
        label_gray->raise();

        horizontalLayout->addWidget(scrollview_gray);

        scrollview_division = new QScrollArea(centralWidget);
        scrollview_division->setObjectName(QStringLiteral("scrollview_division"));
        scrollview_division->setMaximumSize(QSize(65536, 65536));
        scrollview_division->setWidgetResizable(false);
        scrollview_division->setAlignment(Qt::AlignCenter);
        pool_division = new QWidget();
        pool_division->setObjectName(QStringLiteral("pool_division"));
        pool_division->setGeometry(QRect(15, 111, 640, 480));
        label_division = new QLabel(pool_division);
        label_division->setObjectName(QStringLiteral("label_division"));
        label_division->setGeometry(QRect(0, 0, 640, 480));
        label_division->setScaledContents(true);
        label_division->setAlignment(Qt::AlignCenter);
        scrollview_division->setWidget(pool_division);
        scrollview_gray->raise();
        scrollview_gray->raise();
        scrollview_gray->raise();
        label_division->raise();

        horizontalLayout->addWidget(scrollview_division);


        verticalLayout->addLayout(horizontalLayout);

        label_debug = new QLabel(centralWidget);
        label_debug->setObjectName(QStringLiteral("label_debug"));
        label_debug->setWordWrap(true);

        verticalLayout->addWidget(label_debug);

        SegmentationQtClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SegmentationQtClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1371, 24));
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
        label_division->setText(QApplication::translate("SegmentationQtClass", "Label_Division", 0));
        label_debug->setText(QApplication::translate("SegmentationQtClass", "label_debug", 0));
    } // retranslateUi

};

namespace Ui {
    class SegmentationQtClass: public Ui_SegmentationQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGMENTVIEWER_H
