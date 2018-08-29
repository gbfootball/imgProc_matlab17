#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QMainWindow>
#include "ui_imgProc_matlab17.h"
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include "time.h"
#include <QFileDialog>
#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include "geoTransform.h"
#include "imgProc.h"
#include "svm.h"
#include "getPicFeature.h"
#pragma comment(lib, "getPicFeature.lib")

using namespace std;

class imgProc_matlab17 : public QMainWindow
{
	Q_OBJECT

public:
	imgProc_matlab17(QWidget *parent = Q_NULLPTR);
	QImage mat2qimg(const cv::Mat& mat);
	void displayMat(cv::Mat mat, QLabel *label);

	bool processPics(QStringList picsPath);
	bool initSvmParam(svm_parameter &param);
	bool initSvmProb(QStringList allDataList, svm_problem *prob, svm_node *node);
	QVector<double> predictSvm(svm_model *model, svm_node *xnode);

private slots:
	void on_action_open_triggered();
	void on_action_refresh_triggered();
	void on_action_close_triggered();
	void on_action_exit_triggered();

	void on_action_gray_triggered();
	void on_action_bw_triggered();
	void on_action_hls_triggered();
	void on_action_hsv_triggered();
	void on_action_ycrcb_triggered();

	void on_action_dilate_triggered();
	void on_action_erode_triggered();
	void on_action_morphopen_triggered();
	void on_action_morphclose_triggered();
	void on_action_tophat_triggered();
	void on_action_bothat_triggered();
	void on_action_grad_triggered();

	void on_action_trans1_triggered();
	void on_action_trans2_triggered();
	void on_action_scale_triggered();
	void on_action_rotate_triggered();
	void on_action_affine_triggered();
	void on_action_left90_triggered();
	void on_action_Hflip_triggered();
	void on_action_Vflip_triggered();
	void on_action_HVflip_triggered();

	void on_action_trainsvm_triggered();
	void on_action_selectimg_triggered();
	void on_action_predictsvm_triggered();

	void on_action_dec_train_triggered();
	void on_action_dec_select_triggered();

private:
	Ui::imgProc_matlab17Class ui;

	imgProc imgproc;

	cv::Mat img;
	//Mat img_gray;
	//Mat img_bw;
	int morph_se = cv::MORPH_ELLIPSE;
	cv::Size morph_se_size = cv::Size(5, 5);

	bool haveTrainedSvm = false;    //是否进行了训练
	int pSize;//选择了几幅图片
	QVector<double> feature;    //保存处理图片得到的所有特征数据，可考虑构造为二维
	int train_size, feature_size;//样本数及特征维数
	svm_problem prob;	//svm训练数据
	svm_parameter param;//svm参数
	svm_node* node;		//存放单个样本数据
	svm_node* xnode;    //存放预测数据
	svm_model* model;   //svm模型
};
