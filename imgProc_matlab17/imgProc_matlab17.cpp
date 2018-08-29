#include "imgProc_matlab17.h"

imgProc_matlab17::imgProc_matlab17(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	if (!getPicFeatureInitialize()) {
		qDebug() << "matlab���ʼ��ʧ��!";
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
	mwArray feaTmp(1, feature_size, mxDOUBLE_CLASS);//���ÿ⺯���õ�����ʱ����
	double dataTmp;//��ʱ���ݵĵ�����ʱ����
	pSize = picsPath.size();

	//������ͼƬѭ�������ȡ��������
	for (int i = 0; i < pSize; ++i) {
		getPicFeature(1, feaTmp, picsPath.at(i).toLocal8Bit().data());
		for (int j = 1; j <= feature_size; ++j) { //����δ���õ�����ʱ���ݱ��浽һ���ط�
			dataTmp = feaTmp.Get(1, j);
			feature.push_back(dataTmp);
		}
	}
	return true;
}

bool imgProc_matlab17::initSvmParam(svm_parameter &param)
{
	param.svm_type = C_SVC;   //����ͨ�Ķ������
	param.kernel_type = RBF;  //������˺���
	param.degree = 3;
	param.gamma = 0.0625;	//��RBFһ���gamma��C��������
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
	train_size = allDataList.size() - 1;//ȥ��ת����ĩβһ����
	qDebug() << "train_size:" << train_size;

	QStringList lineList = allDataList.at(1).split(" ");
	feature_size = lineList.size() - 2;//ȥ����ͷlabelһ���Լ�ת����ĩβһ����
	qDebug() << "feature_size:" << feature_size;
	QStringList tmp = lineList.at(2).split(":");

	prob->l = train_size;	//ѵ��������
	prob->y = new double[train_size];	//����������ǩ
	prob->x = new svm_node*[train_size];	//ָ���������ݾ���
	node = new svm_node[train_size*(1 + feature_size)];	//��ŵ�����������
														//���ո�ʽ���������ݴ���
	for (int i = 0; i < train_size; ++i) {
		lineList = allDataList.at(i).split(" ");   //��ȡ��i+1������
		for (int j = 0; j < feature_size; ++j) {
			tmp = lineList.at(j + 1).split(":");
			node[(feature_size + 1)*i + j].index = tmp.at(0).toInt();     //���ݱ�ǩ��ָʾ�ڼ���

			node[(feature_size + 1)*i + j].value = tmp.at(1).toDouble();	//���ݵ�ֵ
		}
		node[(feature_size + 1)*i + feature_size].index = -1;	//�궨һ���������ݽ�β
		prob->x[i] = &node[(feature_size + 1)*i];	//ָ����������
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
			xnode[j].index = j + 1;	//���ݱ�ǩ��ָʾ�ڼ���
			xnode[j].value = feature.at(j + (i*feature_size));	//���ݵ�ֵ
		}
		xnode[feature_size].index = -1;	//�궨һ���������ݽ�β
		tmp = svm_predict(model, xnode);
		result.push_back(tmp);
	}
	return result;
}

void imgProc_matlab17::on_action_open_triggered()
{
	QString picPath = QFileDialog::getOpenFileName(this, tr("ѡ��ͼƬ"), "./",
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
* @brief  ��̬ѧ����
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
 * @brief  ���α任
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
	cv::transpose(imgproc.rgb(), img_left90);//����90��
	displayMat(img_left90, ui.label_img_after);
}

void imgProc_matlab17::on_action_Hflip_triggered()
{
	cv::Mat img_Hflip;
	cv::flip(imgproc.rgb(), img_Hflip, 1);//ˮƽ��ת
	displayMat(img_Hflip, ui.label_img_after);
}

void imgProc_matlab17::on_action_Vflip_triggered()
{
	cv::Mat img_Vflip;
	cv::flip(imgproc.rgb(), img_Vflip, 0);//��ֱ��ת
	displayMat(img_Vflip, ui.label_img_after);
}

void imgProc_matlab17::on_action_HVflip_triggered()
{
	cv::Mat img_HVflip;
	cv::flip(imgproc.rgb(), img_HVflip, -1);//��ֱ��ˮƽ��ת
	displayMat(img_HVflip, ui.label_img_after);
}

void imgProc_matlab17::on_action_trainsvm_triggered()
{
	//ѡ��ѵ������
	QString dataPath = QFileDialog::getOpenFileName(this, tr("ѡ��ѵ������"), "./",
		tr("TXT (*.txt);;ALL (*.*)"));
	if (dataPath.isEmpty()) return;
	QFile file(dataPath);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("error"),
			tr("�޷�д���ļ� %1��/n %2")
			.arg(dataPath).arg(file.errorString()));
		return;
	}
	QTextStream in(&file);//������д��
	QStringList allDataList = in.readAll().split("\n");//����������list

	initSvmProb(allDataList, &prob, node);
	//qDebug() << node[2].value;
	initSvmParam(param);

	model = svm_train(&prob, &param);	//ѵ���õ�ģ��
	//haveTrainedSvm = true;
	qDebug() << "end train svm!";
}

void imgProc_matlab17::on_action_selectimg_triggered()
{
	//ѡ��Ԥ��ͼƬ��
	QStringList picsPath = QFileDialog::getOpenFileNames(this, tr("ѡ��ͼƬ"), "./",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if (picsPath.isEmpty()) return;
	feature.clear();//ÿ�����²���������ϴ�����
	QImage pic;
	pic.load(*picsPath.begin());
	QSize picSize = QSize(ui.label_img->width(), ui.label_img->height());
	ui.label_img->setPixmap(QPixmap::fromImage(pic.scaled(picSize, Qt::KeepAspectRatio)));

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));//���ָ���Ϊ�ȴ�״̬
	processPics(picsPath);
	qDebug() << "get feature data ok";
	QApplication::restoreOverrideCursor();//���ָ��ָ�ԭ����״̬
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
	//QMessageBox::warning(this, tr("���"),
	//	tr("ѡ���ͼƬ����Ϊ�� %1")
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
		QMessageBox::warning(this, tr("���"),tr("ѡ���ͼƬ����Ϊ��ˮ��"));
		break;
	case 2:
		QMessageBox::warning(this, tr("���"), tr("ѡ���ͼƬ����Ϊũ��2018"));
		break;
	case 3:
		QMessageBox::warning(this, tr("���"), tr("ѡ���ͼƬ����Ϊ��ũ585"));
		break;
	case 4:
		QMessageBox::warning(this, tr("���"), tr("ѡ���ͼƬ����Ϊũ��2011"));
		break;
	case 5:
		QMessageBox::warning(this, tr("���"), tr("ѡ���ͼƬ����Ϊ֣��9023"));
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
	int hogSize = 6804;//64*64Ϊ1764��64*128Ϊ3780��64*224Ϊ6804

	//string positivePath = "wheat pics\\pos\\";
	//string negativePath = "wheat pics\\neg\\";
	//string suffix = ".jpg";// ͼƬ��׺ 

	//int positiveSampleCount = 50;
	//int negativeSampleCount = 100;
	//int totalSampleCount = positiveSampleCount + negativeSampleCount;

	////std::cout << "/******************************/" << std::endl;
	////cout << "��������: " << totalSampleCount << endl;
	////cout << "��������: " << positiveSampleCount << endl;
	////cout << "��������: " << negativeSampleCount << endl;

	//cv::Mat sampleFeaturesMat = cv::Mat::zeros(totalSampleCount, hogSize, CV_32FC1);
	//cv::Mat sampleLabelMat = cv::Mat::zeros(totalSampleCount, 1, CV_32SC1);

	//// ������ʱ
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

	//	hog.compute(img, featureVec, cv::Size(8, 8));//����HOG����  
	//	int featureVecSize = (int)featureVec.size();

	//	for (int j = 0; j < featureVecSize; j++) {
	//		sampleFeaturesMat.at<float>(i + positiveSampleCount, j) = featureVec[j];
	//	}
	//	sampleLabelMat.at<int>(i + positiveSampleCount) = 0;
	//}

	////std::cout << "/******************************/" << std::endl;
	////cout << "ѵ��SVM..." << endl;

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
	////std::cout << "ѵ����ϣ���ʱ��" << duration << "s" << std::endl;

	//// accuracy
	//float cnt = 0;
	//int rowsize = sampleLabelMat.rows;
	//for (int i = 0; i < rowsize; ++i) {
	//	cv::Mat samp = sampleFeaturesMat.row(i);
	//	float res = svm->predict(samp);
	//	cnt += std::abs(res - sampleLabelMat.at<int>(i)) <= FLT_EPSILON ? 1 : 0;
	//}
	////std::cout << "׼ȷ�ʣ�" << cnt / rowsize * 100 << "%" << std::endl;
	////cv::Mat SVmat = svm->getSupportVectors();
	////std::cout << "֧������������" << SVmat.rows << std::endl;

	//// ��XML�ļ���ȡѵ���õ�SVMģ��  
	////cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::load("hog-svm-model.xml");
	////if (svm->empty()) {
	////	std::cout << "load svm detector failed!!!" << std::endl;
	////	return;
	////}

	////HOG�����ӵ�ά��
	//int DescriptorDim;
	//DescriptorDim = svm->getVarCount();//����������ά������HOG�����ӵ�ά��
	//cv::Mat svecsmat = svm->getSupportVectors();//��ȡsvecsmat��Ԫ������Ϊfloat

	//int svdim = svm->getVarCount();
	//int numofsv = svecsmat.rows;

	////alphamat��svindex�����ʼ��������getDecisionFunction()�����ᱨ��  
	//cv::Mat alphamat = cv::Mat::zeros(numofsv, svdim, CV_32F);
	//cv::Mat svindex = cv::Mat::zeros(1, numofsv, CV_64F);
	//double rho = svm->getDecisionFunction(0, alphamat, svindex);
	//alphamat.convertTo(alphamat, CV_32F);//��alphamatԪ�ص�������������ת��CV_32F

	//cv::Mat Result;
	//Result = -1 * alphamat * svecsmat;

	//std::vector<float> vec;
	//for (int i = 0; i < svdim; ++i) {
	//	vec.push_back(Result.at<float>(0, i));
	//}
	//vec.push_back((float)rho);

	//// ����hog�������
	//std::ofstream fout("hog-detector.txt");
	//for (int i = 0; i < vec.size(); ++i) {
	//	fout << vec[i] << std::endl;
	//}
	////std::cout << "����hog������ϣ�" << std::endl;
	////std::cout << "/******************************/" << std::endl;
	//QMessageBox::warning(this, tr("��ʾ"),
	//	tr("���ģ��ѵ����ϣ�"));

	// ���ļ���ȡhog�������
	//ѡ��ѵ������
	//QString dataPath = QFileDialog::getOpenFileName(this, tr("ѡ��ѵ������"), "./",
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
	/*std::cout << "hog�����Ӽ�����ϣ�" << std::endl;
	std::cout << "ʶ����..." << std::endl;*/
	QMessageBox::warning(this, tr("��ʾ"),
		tr("���ģ�ͼ�����ϣ�"));
}

void imgProc_matlab17::on_action_dec_select_triggered()
{
	QString picPath = QFileDialog::getOpenFileName(this, tr("ѡ��ͼƬ"), "./",
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
	int hogSize = 6804;//64*64Ϊ1764��64*128Ϊ3780��64*224Ϊ6804

	// ��ȡhog�������
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
	//std::cout << "ʶ����..." << std::endl;

	//cv::Mat src;
	//src = cv::imread("D:\\pic\\wheat_pic\\wheat_stand_mid.jpg");
	//src = cv::imread("D:\\pic\\muti\\wheat\\wheat_132.jpg");
	//src = cv::imread("D:\\pic\\wheat_pic\\wheat1_ps_low.jpg");
	//src = cv::imread("C:\\Users\\Administrator\\Desktop\\timg.jpg");

	vector<cv::Rect> found, found_filtered;//���ο�����
	hog.detectMultiScale(src, found, 0, cv::Size(8, 8), cv::Size(32, 32), 1.5, 2);//��ͼ����ж�߶ȼ��
	std::cout << "detect done!" << std::endl;

	//cout << "���θ�����" << found.size() << endl;
	//�ҳ�����û��Ƕ�׵ľ��ο�r,������found_filtered��,�����Ƕ�׵Ļ�,��ȡ���������Ǹ����ο����found_filtered��
	for (int i = 0; i < found.size(); i++) {
		cv::Rect r = found[i];
		int j = 0;
		for (; j < found.size(); j++)
			if (j != i && (r & found[j]) == r)
				break;
		if (j == found.size())
			found_filtered.push_back(r);
	}
	//cout << "���εĸ�����" << found_filtered.size() << endl;

	//�����ο���Ϊhog�����ľ��ο��ʵ�������Ҫ��΢��Щ,����������Ҫ��һЩ����
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
	cv::waitKey();//ע�⣺imshow֮��һ��Ҫ��waitKey�������޷���ʾͼ��
}
