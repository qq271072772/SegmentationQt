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
#include <QtWidgets/QMenu>
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
    QAction *tools_featureCatchOn;
    QAction *tools_GCD_catchON;
    QAction *tools_GCD_PR_catchON;
    QAction *tools_GCD_Genrate;
    QAction *tools_GCD_Clear;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *layout_images;
    QScrollArea *scrollview_gray;
    QWidget *pool_gray;
    QLabel *label_gray;
    QScrollArea *scrollview_division;
    QWidget *pool_division;
    QLabel *label_division;
    QMenuBar *menuBar;
    QMenu *menu_file;
    QMenu *menuTools;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SegmentationQtClass)
    {
        if (SegmentationQtClass->objectName().isEmpty())
            SegmentationQtClass->setObjectName(QStringLiteral("SegmentationQtClass"));
        SegmentationQtClass->resize(1371, 801);
        SegmentationQtClass->setTabShape(QTabWidget::Rounded);
        tools_featureCatchOn = new QAction(SegmentationQtClass);
        tools_featureCatchOn->setObjectName(QStringLiteral("tools_featureCatchOn"));
        tools_featureCatchOn->setCheckable(true);
        tools_GCD_catchON = new QAction(SegmentationQtClass);
        tools_GCD_catchON->setObjectName(QStringLiteral("tools_GCD_catchON"));
        tools_GCD_catchON->setCheckable(true);
        tools_GCD_PR_catchON = new QAction(SegmentationQtClass);
        tools_GCD_PR_catchON->setObjectName(QStringLiteral("tools_GCD_PR_catchON"));
        tools_GCD_PR_catchON->setCheckable(true);
        tools_GCD_Genrate = new QAction(SegmentationQtClass);
        tools_GCD_Genrate->setObjectName(QStringLiteral("tools_GCD_Genrate"));
        tools_GCD_Genrate->setCheckable(true);
        tools_GCD_Clear = new QAction(SegmentationQtClass);
        tools_GCD_Clear->setObjectName(QStringLiteral("tools_GCD_Clear"));
        tools_GCD_Clear->setCheckable(true);
        centralWidget = new QWidget(SegmentationQtClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        layout_images = new QHBoxLayout();
        layout_images->setSpacing(6);
        layout_images->setObjectName(QStringLiteral("layout_images"));
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
        pool_gray->setGeometry(QRect(13, 113, 640, 480));
        label_gray = new QLabel(pool_gray);
        label_gray->setObjectName(QStringLiteral("label_gray"));
        label_gray->setGeometry(QRect(0, 0, 640, 480));
        label_gray->setScaledContents(false);
        label_gray->setAlignment(Qt::AlignCenter);
        scrollview_gray->setWidget(pool_gray);

        layout_images->addWidget(scrollview_gray);

        scrollview_division = new QScrollArea(centralWidget);
        scrollview_division->setObjectName(QStringLiteral("scrollview_division"));
        scrollview_division->setMaximumSize(QSize(65536, 65536));
        scrollview_division->setWidgetResizable(false);
        scrollview_division->setAlignment(Qt::AlignCenter);
        pool_division = new QWidget();
        pool_division->setObjectName(QStringLiteral("pool_division"));
        pool_division->setGeometry(QRect(13, 113, 640, 480));
        label_division = new QLabel(pool_division);
        label_division->setObjectName(QStringLiteral("label_division"));
        label_division->setGeometry(QRect(0, 0, 640, 480));
        label_division->setScaledContents(false);
        label_division->setAlignment(Qt::AlignCenter);
        scrollview_division->setWidget(pool_division);

        layout_images->addWidget(scrollview_division);


        verticalLayout->addLayout(layout_images);

        SegmentationQtClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SegmentationQtClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1371, 27));
        menu_file = new QMenu(menuBar);
        menu_file->setObjectName(QStringLiteral("menu_file"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QStringLiteral("menuTools"));
        SegmentationQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SegmentationQtClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SegmentationQtClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SegmentationQtClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SegmentationQtClass->setStatusBar(statusBar);

        menuBar->addAction(menu_file->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuTools->addAction(tools_featureCatchOn);
        menuTools->addAction(tools_GCD_catchON);
        menuTools->addAction(tools_GCD_PR_catchON);
        menuTools->addSeparator();
        menuTools->addAction(tools_GCD_Clear);
        menuTools->addAction(tools_GCD_Genrate);

        retranslateUi(SegmentationQtClass);

        QMetaObject::connectSlotsByName(SegmentationQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *SegmentationQtClass)
    {
        SegmentationQtClass->setWindowTitle(QApplication::translate("SegmentationQtClass", "SegmentViewer", 0));
        tools_featureCatchOn->setText(QApplication::translate("SegmentationQtClass", "FeatureCatchON", 0));
        tools_GCD_catchON->setText(QApplication::translate("SegmentationQtClass", "GCD_CatchON", 0));
        tools_GCD_PR_catchON->setText(QApplication::translate("SegmentationQtClass", "GCD_PR_CatchON", 0));
        tools_GCD_Genrate->setText(QApplication::translate("SegmentationQtClass", "GCD_Genrate", 0));
        tools_GCD_Clear->setText(QApplication::translate("SegmentationQtClass", "GCD_Clear", 0));
        label_gray->setText(QApplication::translate("SegmentationQtClass", "Label_Gray", 0));
        label_division->setText(QApplication::translate("SegmentationQtClass", "Label_Division", 0));
        menu_file->setTitle(QApplication::translate("SegmentationQtClass", "File", 0));
        menuTools->setTitle(QApplication::translate("SegmentationQtClass", "Tools", 0));
    } // retranslateUi

};

namespace Ui {
    class SegmentationQtClass: public Ui_SegmentationQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGMENTVIEWER_H
