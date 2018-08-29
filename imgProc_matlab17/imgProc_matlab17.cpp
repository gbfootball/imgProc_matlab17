#include "imgProc_matlab17.h"

imgProc_matlab17::imgProc_matlab17(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	if (!getPicFeatureInitialize()) {
		qDebug() << "matlab库初始化失败!";
	}
}
QImage imgProc_matlab17::mat2qimg(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1) {
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		//image.setNumColors(256);
		for (int i = 0; i < 256; i++) {
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++) {
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3) {
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		//return image.rgbSwapped();
		return image;
	}
	else if (mat.type() == CV_8UC4) {
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else {
		return QImage();
	}
}

void imgProc_matlab17::displayMat(cv::Mat mat, QLabel *label)
{
	QImage qimg = mat2qimg(mat);
	QSize picSize = QSize(label->width(), label->height());
	label->setPixmap(QPixmap::fromImage(qimg.scaled(picSize, Qt::KeepAspectRatio)));
}

bool imgProc_matlab17::processPics(QStringList picsPath)
{
	mwArray feaTmp(1, feature_size, mxDOUBLE_CLASS);//调用库函数用到的临时数据
	double dataTmp;//临时数据的单个临时数据
	pSize = picsPath.size();

	//对输入图片循环处理获取特征数据
	for (int i = 0; i < pSize; ++i) {
		getPicFeature(1, feaTmp, picsPath.at(i).toLocal8Bit().data());
		for (int j = 1; j <= feature_size; ++j) { //将多次处理得到的临时数据保存到一个地方
			dataTmp = feaTmp.Get(1, j);
			feature.push_back(dataTmp);
		}
	}
	return true;
}

bool imgProc_matlab17::initSvmParam(svm_parameter &param)
{
	param.svm_type = C_SVC;   //即普通的二类分类
	param.kernel_type = RBF;  //径向基核函数
	param.degree = 3;
	param.gamma = 0.0625;	//用RBF一般调gamma和C两个参数
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 1000;
	param.C = 32;
	param.eps = 1e-5;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	return true;
}

bool imgProc_matlab17::initSvmProb(QStringList allDataList, svm_problem *prob, svm_node *node)
{
	train_size = allDataList.size() - 1;//去除转换后末尾一个空
	qDebug() << "train_size:" << train_size;

	QStringList lineList = allDataList.at(1).split(" ");
	feature_size = lineList.size() - 2;//去除开头label一个以及转换后末尾一个空
	qDebug() << "feature_size:" << feature_size;
	QStringList tmp = lineList.at(2).split(":");

	prob->l = train_size;	//训练样本数
	prob->y = new double[train_size];	//样本所属标签
	prob->x = new svm_node*[train_size];	//指向样本数据矩阵
	node = new svm_node[train_size*(1 + feature_size)];	//存放单个样本数据
														//按照格式将样本数据存入
	for (int i = 0; i < train_size; ++i) {
		lineList = allDataList.at(i).split(" ");   //读取第i+1行数据
		for (int j = 0; j < feature_size; ++j) {
			tmp = lineList.at(j + 1).split(":");
			node[(feature_size + 1)*i + j].index = tmp.at(0).toInt();     //数据标签，指示第几个

			node[(feature_size + 1)*i + j].value = tmp.at(1).toDouble();	//数据的值
		}
		node[(feature_size + 1)*i + feature_size].index = -1;	//标定一个样本数据结尾
		prob->x[i] = &node[(feature_size + 1)*i];	//指向样本数据
		prob->y[i] = lineList.at(0).toDouble();
	}
	return true;
}

QVector<double> imgProc_matlab17::predictSvm(svm_model *model, svm_node *xnode)
{
	xnode = new svm_node[1 + feature_size];
	QVector<double> result;
	double tmp;

	for (int i = 0; i < pSize; ++i) {
		for (int j = 0; j < feature_size; ++j) {
			xnode[j].index = j + 1;	//数据标签，指示第几个
			xnode[j].value = feature.at(j + (i*feature_size));	//数据的值
		}
		xnode[feature_size].index = -1;	//标定一个样本数据结尾
		tmp = svm_predict(model, xnode);
		result.push_back(tmp);
	}
	return result;
}

void imgProc_matlab17::on_action_open_triggered()
{
	QString picPath = QFileDialog::getOpenFileName(this, tr("选择图片"), "./",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if (picPath.isEmpty()) return;
	
	imgproc.read(picPath.toLocal8Bit().data());
	displayMat(imgproc.rgb(), ui.label_img);
	
	//cv::Mat_<cv::Vec3b>::iterator it = m.begin<cv::Vec3b>();
	//int a = m.at<Vec3b>(114, 25)[1];
	//cout << a << endl;

}

void imgProc_matlab17::on_action_refresh_triggered()
{
	displayMat(imgproc.rgb(), ui.label_img);
}

void imgProc_matlab17::on_action_close_triggered()
{
	imgproc.rgb().release();
	ui.label_img->clear();
	ui.label_img_after->clear();
}

void imgProc_matlab17::on_action_exit_triggered()
{
	qApp->quit();
}

void imgProc_matlab17::on_action_gray_triggered()
{
	displayMat(imgproc.rgb2gray(imgproc.rgb()), ui.label_img_after);
}

void imgProc_matlab17::on_action_bw_triggered()
{
	displayMat(imgproc.rgb2bw(imgproc.rgb()), ui.label_img_after);
}

void imgProc_matlab17::on_action_hls_triggered()
{

	displayMat(imgproc.rgb2hls(imgproc.rgb()), ui.label_img_after);
}

void imgProc_matlab17::on_action_hsv_triggered()
{
	displayMat(imgproc.rgb2hsv(imgproc.rgb()), ui.label_img_after);
}

void imgProc_matlab17::on_action_ycrcb_triggered()
{
	displayMat(imgproc.rgb2ycrcb(imgproc.rgb()), ui.label_img_after);
}

/**
* @brief  形态学膨胀
* @param
* @return
*/
void imgProc_matlab17::on_action_dilate_triggered()
{
	cv::Mat kern = cv::getStructuringElement(morph_se, morph_se_size);
	cv::Mat img_dilate;
	cv::dilate(imgproc.rgb(), img_dilate, kern);
	displayMat(img_dilate, ui.label_img_after);
}

void imgProc_matlab17::on_action_erode_triggered()
{
	cv::Mat kern = cv::getStructuringElement(morph_se, morph_se_size);
	cv::Mat img_erode;
	cv::erode(imgproc.rgb(), img_erode, kern);
	displayMat(img_erode, ui.label_img_after);
}

void imgProc_matlab17::on_action_morphopen_triggered()
{
	cv::Mat kern = cv::getStructuringElement(morph_se, morph_se_size);
	cv::Mat img_open;
	cv::morphologyEx(imgproc.rgb(), img_open, cv::MORPH_OPEN, kern);
	displayMat(img_open, ui.label_img_after);
}

void imgProc_matlab17::on_action_morphclose_triggered()
{
	cv::Mat kern = cv::getStructuringElement(morph_se, morph_se_size);
	cv::Mat img_close;
	cv::morphologyEx(imgproc.rgb(), img_close, cv::MORPH_CLOSE, kern);
	displayMat(img_close, ui.label_img_after);
}

void imgProc_matlab17::on_action_tophat_triggered()
{
	cv::Mat kern = cv::getStructuringElement(morph_se, morph_se_size);
	cv::Mat img_tophat;
	cv::morphologyEx(imgproc.rgb(), img_tophat, cv::MORPH_TOPHAT, kern);
	displayMat(img_tophat, ui.label_img_after);
}

void imgProc_matlab17::on_action_bothat_triggered()
{
	cv::Mat kern = cv::getStructuringElement(morph_se, morph_se_size);
	cv::Mat img_bothat;
	cv::morphologyEx(imgproc.rgb(), img_bothat, cv::MORPH_BLACKHAT, kern);
	displayMat(img_bothat, ui.label_img_after);
}

void imgProc_matlab17::on_action_grad_triggered()
{
	cv::Mat kern = cv::getStructuringElement(morph_se, morph_se_size);
	cv::Mat img_bothat;
	cv::morphologyEx(imgproc.rgb(), img_bothat, cv::MORPH_GRADIENT, kern);
	displayMat(img_bothat, ui.label_img_after);
}

/**
 * @brief  几何变换
 * @param  
 * @return 
 */
void imgProc_matlab17::on_action_trans1_triggered()
{
	cv::Mat img_translation;
	img_translation = imgTranslation1(imgproc.rgb(), 100, 100);
	displayMat(img_translation, ui.label_img_after);
}

void imgProc_matlab17::on_action_trans2_triggered()
{
	cv::Mat img_translation;
	img_translation = imgTranslation2(imgproc.rgb(), 100, 100);
	displayMat(img_translation, ui.label_img_after);
}

void imgProc_matlab17::on_action_scale_triggered()
{
	cv::Mat img_scale;
	img_scale = imgScale(imgproc.rgb(), 0.2, 0.3);
	displayMat(img_scale, ui.label_img_after);
}

void imgProc_matlab17::on_action_rotate_triggered()
{
	cv::Mat img_rorate;
	img_rorate = imgRotate(imgproc.rgb(), 30);
	displayMat(img_rorate, ui.label_img_after);
}

void imgProc_matlab17::on_action_affine_triggered()
{
	cv::Mat img_affine;
	img_affine = imgAffine(imgproc.rgb());
	displayMat(img_affine, ui.label_img_after);
}

void imgProc_matlab17::on_action_left90_triggered()
{
	cv::Mat img_left90;
	cv::transpose(imgproc.rgb(), img_left90);//左旋90度
	displayMat(img_left90, ui.label_img_after);
}

void imgProc_matlab17::on_action_Hflip_triggered()
{
	cv::Mat img_Hflip;
	cv::flip(imgproc.rgb(), img_Hflip, 1);//水平翻转
	displayMat(img_Hflip, ui.label_img_after);
}

void imgProc_matlab17::on_action_Vflip_triggered()
{
	cv::Mat img_Vflip;
	cv::flip(imgproc.rgb(), img_Vflip, 0);//垂直翻转
	displayMat(img_Vflip, ui.label_img_after);
}

void imgProc_matlab17::on_action_HVflip_triggered()
{
	cv::Mat img_HVflip;
	cv::flip(imgproc.rgb(), img_HVflip, -1);//垂直和水平翻转
	displayMat(img_HVflip, ui.label_img_after);
}

void imgProc_matlab17::on_action_trainsvm_triggered()
{
	//选择训练数据
	QString dataPath = QFileDialog::getOpenFileName(this, tr("选择训练数据"), "./",
		tr("TXT (*.txt);;ALL (*.*)"));
	if (dataPath.isEmpty()) return;
	QFile file(dataPath);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("error"),
			tr("无法写入文件 %1：/n %2")
			.arg(dataPath).arg(file.errorString()));
		return;
	}
	QTextStream in(&file);//创建读写流
	QStringList allDataList = in.readAll().split("\n");//读入数据至list

	initSvmProb(allDataList, &prob, node);
	//qDebug() << node[2].value;
	initSvmParam(param);

	model = svm_train(&prob, &param);	//训练得到模型
	//haveTrainedSvm = true;
	qDebug() << "end train svm!";
}

void imgProc_matlab17::on_action_selectimg_triggered()
{
	//选择预测图片集
	QStringList picsPath = QFileDialog::getOpenFileNames(this, tr("选择图片"), "./",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if (picsPath.isEmpty()) return;
	feature.clear();//每次重新操作需清除上次数据
	QImage pic;
	pic.load(*picsPath.begin());
	QSize picSize = QSize(ui.label_img->width(), ui.label_img->height());
	ui.label_img->setPixmap(QPixmap::fromImage(pic.scaled(picSize, Qt::KeepAspectRatio)));

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));//鼠标指针变为等待状态
	processPics(picsPath);
	qDebug() << "get feature data ok";
	QApplication::restoreOverrideCursor();//鼠标指针恢复原来的状态
}

void imgProc_matlab17::on_action_predictsvm_triggered()
{
	//QVector<double> result = predictSvm(model, xnode);
	//QString result_str;
	//for (auto val : result)
	//{
	//	qDebug() << "result is " << val;
	//	result_str.push_back(QString::number(val));
	//}
	//QMessageBox::warning(this, tr("结果"),
	//	tr("选择的图片分类为： %1")
	//	.arg(result_str));


	QVector<double> result = predictSvm(model, xnode);
	int result_int;
	for (auto val : result)
	{
		qDebug() << "result is " << val;
		result_int = val;
	}
	switch (result_int)
	{
	case 1:
		QMessageBox::warning(this, tr("结果"),tr("选择的图片分类为望水白"));
		break;
	case 2:
		QMessageBox::warning(this, tr("结果"), tr("选择的图片分类为农大2018"));
		break;
	case 3:
		QMessageBox::warning(this, tr("结果"), tr("选择的图片分类为西农585"));
		break;
	case 4:
		QMessageBox::warning(this, tr("结果"), tr("选择的图片分类为农大2011"));
		break;
	case 5:
		QMessageBox::warning(this, tr("结果"), tr("选择的图片分类为郑麦9023"));
		break;
	default:
		break;
	}
}

void imgProc_matlab17::on_action_dec_train_triggered()
{
	cv::Size winSize(64, 224);
	cv::Size blockSize(16, 16);
	cv::Size blockStride(8, 8);
	cv::Size cellSize(8, 8);
	int hogSize = 6804;//64*64为1764，64*128为3780，64*224为6804

	//string positivePath = "wheat pics\\pos\\";
	//string negativePath = "wheat pics\\neg\\";
	//string suffix = ".jpg";// 图片后缀 

	//int positiveSampleCount = 50;
	//int negativeSampleCount = 100;
	//int totalSampleCount = positiveSampleCount + negativeSampleCount;

	////std::cout << "/******************************/" << std::endl;
	////cout << "总样本数: " << totalSampleCount << endl;
	////cout << "正样本数: " << positiveSampleCount << endl;
	////cout << "负样本数: " << negativeSampleCount << endl;

	//cv::Mat sampleFeaturesMat = cv::Mat::zeros(totalSampleCount, hogSize, CV_32FC1);
	//cv::Mat sampleLabelMat = cv::Mat::zeros(totalSampleCount, 1, CV_32SC1);

	//// 计算用时
	//clock_t start, finish;
	//double duration;
	//start = clock();

	//for (int i = 0; i < positiveSampleCount; i++) {
	//	stringstream path;
	//	path << positivePath << i + 1 << suffix;
	//	cv::Mat img = cv::imread(path.str());
	//	if (img.data == NULL) {
	//		cout << "positive image sample load error: " << i << " " << path.str() << endl;
	//		system("pause");
	//		continue;
	//	}

	//	cv::HOGDescriptor hog(winSize, blockSize, blockStride, cellSize, 9);
	//	vector<float> featureVec;

	//	hog.compute(img, featureVec, cv::Size(8, 8));
	//	int featureVecSize = (int)featureVec.size();

	//	for (int j = 0; j < featureVecSize; j++) {
	//		sampleFeaturesMat.at<float>(i, j) = featureVec[j];
	//	}
	//	sampleLabelMat.at<int>(i) = 1;
	//}

	//for (int i = 0; i < negativeSampleCount; i++) {
	//	stringstream path;
	//	path << negativePath << i + 1 << suffix;
	//	cv::Mat img = cv::imread(path.str());
	//	if (img.data == NULL) {
	//		cout << "negative image sample load error: " << path.str() << endl;
	//		continue;
	//	}

	//	cv::HOGDescriptor hog(winSize, blockSize, blockStride, cellSize, 9);
	//	vector<float> featureVec;

	//	hog.compute(img, featureVec, cv::Size(8, 8));//计算HOG特征  
	//	int featureVecSize = (int)featureVec.size();

	//	for (int j = 0; j < featureVecSize; j++) {
	//		sampleFeaturesMat.at<float>(i + positiveSampleCount, j) = featureVec[j];
	//	}
	//	sampleLabelMat.at<int>(i + positiveSampleCount) = 0;
	//}

	////std::cout << "/******************************/" << std::endl;
	////cout << "训练SVM..." << endl;

	//// initial SVM
	//cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
	//svm->setType(cv::ml::SVM::Types::C_SVC);
	//svm->setKernel(cv::ml::SVM::KernelTypes::LINEAR);
	//svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 1000, FLT_EPSILON));
	//svm->setDegree(0.2);
	//svm->setGamma(0.5);
	//svm->setC(32);

	//// train operation
	//svm->train(sampleFeaturesMat, cv::ml::SampleTypes::ROW_SAMPLE, sampleLabelMat);
	//svm->save("hog-svm-model.xml");
	//finish = clock();
	//duration = (double)(finish - start) / CLOCKS_PER_SEC;
	////std::cout << "训练完毕，用时：" << duration << "s" << std::endl;

	//// accuracy
	//float cnt = 0;
	//int rowsize = sampleLabelMat.rows;
	//for (int i = 0; i < rowsize; ++i) {
	//	cv::Mat samp = sampleFeaturesMat.row(i);
	//	float res = svm->predict(samp);
	//	cnt += std::abs(res - sampleLabelMat.at<int>(i)) <= FLT_EPSILON ? 1 : 0;
	//}
	////std::cout << "准确率：" << cnt / rowsize * 100 << "%" << std::endl;
	////cv::Mat SVmat = svm->getSupportVectors();
	////std::cout << "支持向量个数：" << SVmat.rows << std::endl;

	//// 从XML文件读取训练好的SVM模型  
	////cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::load("hog-svm-model.xml");
	////if (svm->empty()) {
	////	std::cout << "load svm detector failed!!!" << std::endl;
	////	return;
	////}

	////HOG描述子的维数
	//int DescriptorDim;
	//DescriptorDim = svm->getVarCount();//特征向量的维数，即HOG描述子的维数
	//cv::Mat svecsmat = svm->getSupportVectors();//获取svecsmat，元素类型为float

	//int svdim = svm->getVarCount();
	//int numofsv = svecsmat.rows;

	////alphamat和svindex必须初始化，否则getDecisionFunction()函数会报错  
	//cv::Mat alphamat = cv::Mat::zeros(numofsv, svdim, CV_32F);
	//cv::Mat svindex = cv::Mat::zeros(1, numofsv, CV_64F);
	//double rho = svm->getDecisionFunction(0, alphamat, svindex);
	//alphamat.convertTo(alphamat, CV_32F);//将alphamat元素的数据类型重新转成CV_32F

	//cv::Mat Result;
	//Result = -1 * alphamat * svecsmat;

	//std::vector<float> vec;
	//for (int i = 0; i < svdim; ++i) {
	//	vec.push_back(Result.at<float>(0, i));
	//}
	//vec.push_back((float)rho);

	//// 保存hog分类参数
	//std::ofstream fout("hog-detector.txt");
	//for (int i = 0; i < vec.size(); ++i) {
	//	fout << vec[i] << std::endl;
	//}
	////std::cout << "保存hog参数完毕！" << std::endl;
	////std::cout << "/******************************/" << std::endl;
	//QMessageBox::warning(this, tr("提示"),
	//	tr("检测模型训练完毕！"));

	// 从文件读取hog分类参数
	//选择训练数据
	//QString dataPath = QFileDialog::getOpenFileName(this, tr("选择训练数据"), "./",
	//	tr("TXT (*.txt);;ALL (*.*)"));
	//if (dataPath.isEmpty()) return;
	//ifstream fin(dataPath.toLocal8Bit());
	ifstream fin("hog-detector.txt");
	vector<float> vec;
	float tmp = 0;
	while (fin >> tmp) {
		vec.push_back(tmp);
	}
	fin.close();

	cv::HOGDescriptor hog(winSize, blockSize, blockStride, cellSize, 9);
	hog.setSVMDetector(vec);
	/*std::cout << "hog描述子加载完毕！" << std::endl;
	std::cout << "识别中..." << std::endl;*/
	QMessageBox::warning(this, tr("提示"),
		tr("检测模型加载完毕！"));
}

void imgProc_matlab17::on_action_dec_select_triggered()
{
	QString picPath = QFileDialog::getOpenFileName(this, tr("选择图片"), "./",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if (picPath.isEmpty()) return;

	imgproc.read(picPath.toLocal8Bit().data());
	displayMat(imgproc.rgb(), ui.label_img);
	cv::Mat src = imgproc.bgr().clone();
	std::cout << "read pics done!" << std::endl;
	//cv::Mat src;
	//string pathp = picPath.toLocal8Bit().data();
	//std::cout << pathp << std::endl;
	//src = cv::imread(pathp);

	cv::Size winSize(64, 224);
	cv::Size blockSize(16, 16);
	cv::Size blockStride(8, 8);
	cv::Size cellSize(8, 8);
	int hogSize = 6804;//64*64为1764，64*128为3780，64*224为6804

	// 读取hog分类参数
	ifstream fin("hog-detector.txt");
	vector<float> vec;
	float tmp = 0;
	while (fin >> tmp) {
		vec.push_back(tmp);
	}
	fin.close();

	cv::HOGDescriptor hog(winSize, blockSize, blockStride, cellSize, 9);
	hog.setSVMDetector(vec);
	std::cout << "hog descriptor load done!" << std::endl;
	//std::cout << "识别中..." << std::endl;

	//cv::Mat src;
	//src = cv::imread("D:\\pic\\wheat_pic\\wheat_stand_mid.jpg");
	//src = cv::imread("D:\\pic\\muti\\wheat\\wheat_132.jpg");
	//src = cv::imread("D:\\pic\\wheat_pic\\wheat1_ps_low.jpg");
	//src = cv::imread("C:\\Users\\Administrator\\Desktop\\timg.jpg");

	vector<cv::Rect> found, found_filtered;//矩形框数组
	hog.detectMultiScale(src, found, 0, cv::Size(8, 8), cv::Size(32, 32), 1.5, 2);//对图像进行多尺度检测
	std::cout << "detect done!" << std::endl;

	//cout << "矩形个数：" << found.size() << endl;
	//找出所有没有嵌套的矩形框r,并放入found_filtered中,如果有嵌套的话,则取外面最大的那个矩形框放入found_filtered中
	for (int i = 0; i < found.size(); i++) {
		cv::Rect r = found[i];
		int j = 0;
		for (; j < found.size(); j++)
			if (j != i && (r & found[j]) == r)
				break;
		if (j == found.size())
			found_filtered.push_back(r);
	}
	//cout << "矩形的个数：" << found_filtered.size() << endl;

	//画矩形框，因为hog检测出的矩形框比实际人体框要稍微大些,所以这里需要做一些调整
	for (int i = 0; i < found_filtered.size(); i++) {
		cv::Rect r = found_filtered[i];
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8);
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8);
		rectangle(src, r.tl(), r.br(), cv::Scalar(0, 255, 255), 2);
	}
	cv::namedWindow("detector", cv::WINDOW_NORMAL);
	cv::imshow("detector", src);
	//displayMat(src, ui.label_img_after);
	//imwrite("ImgProcessed.jpg",src);
	cv::waitKey();//注意：imshow之后一定要加waitKey，否则无法显示图像
}
