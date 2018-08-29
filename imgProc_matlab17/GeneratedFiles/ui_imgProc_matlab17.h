/********************************************************************************
** Form generated from reading UI file 'imgProc_matlab17.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMGPROC_MATLAB17_H
#define UI_IMGPROC_MATLAB17_H

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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_imgProc_matlab17Class
{
public:
    QAction *action_new;
    QAction *action_open;
    QAction *action_save;
    QAction *action_close;
    QAction *action_exit;
    QAction *action_gray;
    QAction *action_bw;
    QAction *action_refresh;
    QAction *action_trans1;
    QAction *action_trans2;
    QAction *action_scale;
    QAction *action_rotate;
    QAction *action_left90;
    QAction *action_Hflip;
    QAction *action_Vflip;
    QAction *action_HVflip;
    QAction *action_hls;
    QAction *action_hsv;
    QAction *action_ycrcb;
    QAction *action_affine;
    QAction *action_dilate;
    QAction *action_erode;
    QAction *action_morphopen;
    QAction *action_morphclose;
    QAction *action_tophat;
    QAction *action_bothat;
    QAction *action_grad;
    QAction *action_trainsvm;
    QAction *action_predictsvm;
    QAction *action_selectimg;
    QAction *action_dec_train;
    QAction *action_dec_select;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_img;
    QLabel *label_img_after;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QMenu *menuSVM;
    QMenu *menu_dec;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *imgProc_matlab17Class)
    {
        if (imgProc_matlab17Class->objectName().isEmpty())
            imgProc_matlab17Class->setObjectName(QStringLiteral("imgProc_matlab17Class"));
        imgProc_matlab17Class->resize(600, 400);
        action_new = new QAction(imgProc_matlab17Class);
        action_new->setObjectName(QStringLiteral("action_new"));
        action_open = new QAction(imgProc_matlab17Class);
        action_open->setObjectName(QStringLiteral("action_open"));
        action_save = new QAction(imgProc_matlab17Class);
        action_save->setObjectName(QStringLiteral("action_save"));
        action_close = new QAction(imgProc_matlab17Class);
        action_close->setObjectName(QStringLiteral("action_close"));
        action_exit = new QAction(imgProc_matlab17Class);
        action_exit->setObjectName(QStringLiteral("action_exit"));
        action_gray = new QAction(imgProc_matlab17Class);
        action_gray->setObjectName(QStringLiteral("action_gray"));
        action_bw = new QAction(imgProc_matlab17Class);
        action_bw->setObjectName(QStringLiteral("action_bw"));
        action_refresh = new QAction(imgProc_matlab17Class);
        action_refresh->setObjectName(QStringLiteral("action_refresh"));
        action_trans1 = new QAction(imgProc_matlab17Class);
        action_trans1->setObjectName(QStringLiteral("action_trans1"));
        action_trans2 = new QAction(imgProc_matlab17Class);
        action_trans2->setObjectName(QStringLiteral("action_trans2"));
        action_scale = new QAction(imgProc_matlab17Class);
        action_scale->setObjectName(QStringLiteral("action_scale"));
        action_rotate = new QAction(imgProc_matlab17Class);
        action_rotate->setObjectName(QStringLiteral("action_rotate"));
        action_left90 = new QAction(imgProc_matlab17Class);
        action_left90->setObjectName(QStringLiteral("action_left90"));
        action_Hflip = new QAction(imgProc_matlab17Class);
        action_Hflip->setObjectName(QStringLiteral("action_Hflip"));
        action_Vflip = new QAction(imgProc_matlab17Class);
        action_Vflip->setObjectName(QStringLiteral("action_Vflip"));
        action_HVflip = new QAction(imgProc_matlab17Class);
        action_HVflip->setObjectName(QStringLiteral("action_HVflip"));
        action_hls = new QAction(imgProc_matlab17Class);
        action_hls->setObjectName(QStringLiteral("action_hls"));
        action_hsv = new QAction(imgProc_matlab17Class);
        action_hsv->setObjectName(QStringLiteral("action_hsv"));
        action_ycrcb = new QAction(imgProc_matlab17Class);
        action_ycrcb->setObjectName(QStringLiteral("action_ycrcb"));
        action_affine = new QAction(imgProc_matlab17Class);
        action_affine->setObjectName(QStringLiteral("action_affine"));
        action_dilate = new QAction(imgProc_matlab17Class);
        action_dilate->setObjectName(QStringLiteral("action_dilate"));
        action_erode = new QAction(imgProc_matlab17Class);
        action_erode->setObjectName(QStringLiteral("action_erode"));
        action_morphopen = new QAction(imgProc_matlab17Class);
        action_morphopen->setObjectName(QStringLiteral("action_morphopen"));
        action_morphclose = new QAction(imgProc_matlab17Class);
        action_morphclose->setObjectName(QStringLiteral("action_morphclose"));
        action_tophat = new QAction(imgProc_matlab17Class);
        action_tophat->setObjectName(QStringLiteral("action_tophat"));
        action_bothat = new QAction(imgProc_matlab17Class);
        action_bothat->setObjectName(QStringLiteral("action_bothat"));
        action_grad = new QAction(imgProc_matlab17Class);
        action_grad->setObjectName(QStringLiteral("action_grad"));
        action_trainsvm = new QAction(imgProc_matlab17Class);
        action_trainsvm->setObjectName(QStringLiteral("action_trainsvm"));
        action_predictsvm = new QAction(imgProc_matlab17Class);
        action_predictsvm->setObjectName(QStringLiteral("action_predictsvm"));
        action_selectimg = new QAction(imgProc_matlab17Class);
        action_selectimg->setObjectName(QStringLiteral("action_selectimg"));
        action_dec_train = new QAction(imgProc_matlab17Class);
        action_dec_train->setObjectName(QStringLiteral("action_dec_train"));
        action_dec_select = new QAction(imgProc_matlab17Class);
        action_dec_select->setObjectName(QStringLiteral("action_dec_select"));
        centralWidget = new QWidget(imgProc_matlab17Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setStyleSheet(QStringLiteral("background-color: rgb(197, 197, 197);"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_img = new QLabel(centralWidget);
        label_img->setObjectName(QStringLiteral("label_img"));
        label_img->setSizeIncrement(QSize(50, 50));

        horizontalLayout->addWidget(label_img);

        label_img_after = new QLabel(centralWidget);
        label_img_after->setObjectName(QStringLiteral("label_img_after"));
        label_img_after->setSizeIncrement(QSize(50, 50));

        horizontalLayout->addWidget(label_img_after);


        horizontalLayout_2->addLayout(horizontalLayout);

        imgProc_matlab17Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(imgProc_matlab17Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QStringLiteral("menu_4"));
        menuSVM = new QMenu(menuBar);
        menuSVM->setObjectName(QStringLiteral("menuSVM"));
        menu_dec = new QMenu(menuBar);
        menu_dec->setObjectName(QStringLiteral("menu_dec"));
        imgProc_matlab17Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(imgProc_matlab17Class);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        imgProc_matlab17Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(imgProc_matlab17Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        imgProc_matlab17Class->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menuSVM->menuAction());
        menuBar->addAction(menu_dec->menuAction());
        menu->addAction(action_new);
        menu->addAction(action_open);
        menu->addAction(action_save);
        menu->addAction(action_refresh);
        menu->addAction(action_close);
        menu->addAction(action_exit);
        menu_2->addAction(action_gray);
        menu_2->addAction(action_bw);
        menu_2->addAction(action_hls);
        menu_2->addAction(action_hsv);
        menu_2->addAction(action_ycrcb);
        menu_3->addAction(action_trans1);
        menu_3->addAction(action_trans2);
        menu_3->addAction(action_scale);
        menu_3->addAction(action_rotate);
        menu_3->addAction(action_affine);
        menu_3->addAction(action_left90);
        menu_3->addAction(action_Hflip);
        menu_3->addAction(action_Vflip);
        menu_3->addAction(action_HVflip);
        menu_4->addAction(action_dilate);
        menu_4->addAction(action_erode);
        menu_4->addAction(action_morphopen);
        menu_4->addAction(action_morphclose);
        menu_4->addAction(action_tophat);
        menu_4->addAction(action_bothat);
        menu_4->addAction(action_grad);
        menuSVM->addAction(action_trainsvm);
        menuSVM->addAction(action_selectimg);
        menuSVM->addAction(action_predictsvm);
        menu_dec->addAction(action_dec_train);
        menu_dec->addAction(action_dec_select);

        retranslateUi(imgProc_matlab17Class);

        QMetaObject::connectSlotsByName(imgProc_matlab17Class);
    } // setupUi

    void retranslateUi(QMainWindow *imgProc_matlab17Class)
    {
        imgProc_matlab17Class->setWindowTitle(QApplication::translate("imgProc_matlab17Class", "\345\260\217\351\272\246\347\251\227\350\204\270\347\211\271\345\276\201\346\217\220\345\217\226\345\217\212\350\207\252\345\212\250\345\210\206\347\261\273", Q_NULLPTR));
        action_new->setText(QApplication::translate("imgProc_matlab17Class", "\346\226\260\345\273\272", Q_NULLPTR));
        action_open->setText(QApplication::translate("imgProc_matlab17Class", "\346\211\223\345\274\200", Q_NULLPTR));
        action_save->setText(QApplication::translate("imgProc_matlab17Class", "\344\277\235\345\255\230", Q_NULLPTR));
        action_close->setText(QApplication::translate("imgProc_matlab17Class", "\345\205\263\351\227\255", Q_NULLPTR));
        action_exit->setText(QApplication::translate("imgProc_matlab17Class", "\351\200\200\345\207\272", Q_NULLPTR));
        action_gray->setText(QApplication::translate("imgProc_matlab17Class", "\347\201\260\345\272\246\345\214\226", Q_NULLPTR));
        action_bw->setText(QApplication::translate("imgProc_matlab17Class", "\344\272\214\345\200\274\345\214\226", Q_NULLPTR));
        action_refresh->setText(QApplication::translate("imgProc_matlab17Class", "\345\210\267\346\226\260", Q_NULLPTR));
        action_trans1->setText(QApplication::translate("imgProc_matlab17Class", "\345\271\263\347\247\273-\345\210\207\351\231\244", Q_NULLPTR));
        action_trans2->setText(QApplication::translate("imgProc_matlab17Class", "\345\271\263\347\247\273-\346\211\251\345\261\225", Q_NULLPTR));
        action_scale->setText(QApplication::translate("imgProc_matlab17Class", "\346\224\276\347\274\251", Q_NULLPTR));
        action_rotate->setText(QApplication::translate("imgProc_matlab17Class", "\346\227\213\350\275\254", Q_NULLPTR));
        action_left90->setText(QApplication::translate("imgProc_matlab17Class", "\345\267\246\346\227\21390", Q_NULLPTR));
        action_Hflip->setText(QApplication::translate("imgProc_matlab17Class", "\346\260\264\345\271\263\347\277\273\350\275\254", Q_NULLPTR));
        action_Vflip->setText(QApplication::translate("imgProc_matlab17Class", "\345\236\202\347\233\264\347\277\273\350\275\254", Q_NULLPTR));
        action_HVflip->setText(QApplication::translate("imgProc_matlab17Class", "\346\260\264\345\236\202\347\277\273\350\275\254", Q_NULLPTR));
        action_hls->setText(QApplication::translate("imgProc_matlab17Class", "HLS", Q_NULLPTR));
        action_hsv->setText(QApplication::translate("imgProc_matlab17Class", "HSV", Q_NULLPTR));
        action_ycrcb->setText(QApplication::translate("imgProc_matlab17Class", "YCrCb", Q_NULLPTR));
        action_affine->setText(QApplication::translate("imgProc_matlab17Class", "\347\202\271\344\273\277\345\260\204", Q_NULLPTR));
        action_dilate->setText(QApplication::translate("imgProc_matlab17Class", "\350\206\250\350\203\200", Q_NULLPTR));
        action_erode->setText(QApplication::translate("imgProc_matlab17Class", "\350\205\220\350\232\200", Q_NULLPTR));
        action_morphopen->setText(QApplication::translate("imgProc_matlab17Class", "\345\274\200", Q_NULLPTR));
        action_morphclose->setText(QApplication::translate("imgProc_matlab17Class", "\351\227\255", Q_NULLPTR));
        action_tophat->setText(QApplication::translate("imgProc_matlab17Class", "\351\241\266\345\270\275", Q_NULLPTR));
        action_bothat->setText(QApplication::translate("imgProc_matlab17Class", "\345\272\225\345\270\275", Q_NULLPTR));
        action_grad->setText(QApplication::translate("imgProc_matlab17Class", "\346\242\257\345\272\246", Q_NULLPTR));
        action_trainsvm->setText(QApplication::translate("imgProc_matlab17Class", "\350\256\255\347\273\203", Q_NULLPTR));
        action_predictsvm->setText(QApplication::translate("imgProc_matlab17Class", "\351\242\204\346\265\213", Q_NULLPTR));
        action_selectimg->setText(QApplication::translate("imgProc_matlab17Class", "\351\200\211\346\213\251\345\233\276\347\211\207", Q_NULLPTR));
        action_dec_train->setText(QApplication::translate("imgProc_matlab17Class", "\350\256\255\347\273\203", Q_NULLPTR));
        action_dec_select->setText(QApplication::translate("imgProc_matlab17Class", "\351\200\211\346\213\251\345\233\276\347\211\207", Q_NULLPTR));
        label_img->setText(QString());
        label_img_after->setText(QString());
        menu->setTitle(QApplication::translate("imgProc_matlab17Class", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("imgProc_matlab17Class", "\350\275\254\346\215\242", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("imgProc_matlab17Class", "\345\207\240\344\275\225\345\217\230\346\215\242", Q_NULLPTR));
        menu_4->setTitle(QApplication::translate("imgProc_matlab17Class", "\345\275\242\346\200\201\345\255\246", Q_NULLPTR));
        menuSVM->setTitle(QApplication::translate("imgProc_matlab17Class", "SVM\345\210\206\347\261\273", Q_NULLPTR));
        menu_dec->setTitle(QApplication::translate("imgProc_matlab17Class", "\346\243\200\346\265\213", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class imgProc_matlab17Class: public Ui_imgProc_matlab17Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMGPROC_MATLAB17_H
