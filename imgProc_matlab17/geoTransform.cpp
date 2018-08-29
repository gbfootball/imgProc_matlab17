#include "geoTransform.h"

/**
 * @brief  ƽ�Ʋ��� ͼ���С����
 * @param  
 * @return 
 */
cv::Mat imgTranslation1(cv::Mat & srcImage, int xOffset, int yOffset)
{
	int nRows = srcImage.rows;
	int nCols = srcImage.cols;
	cv::Mat resultImage(srcImage.size(),
		srcImage.type());
	// ����ͼ��
	for (int i = 0; i < nRows; ++i) {
		for (int j = 0; j < nCols; ++j) {
			// ӳ��任
			int x = j - xOffset;
			int y = i - yOffset;
			// �߽��ж�
			if (x >= 0 && y >= 0 && x < nCols && y < nRows)
				resultImage.at<cv::Vec3b>(i, j) =
				srcImage.ptr<cv::Vec3b>(y)[x];
		}
	}
	return resultImage;
}
/**
 * @brief  ƽ�Ʋ��� ͼ���С�ı�
 * @param  
 * @return 
 */
cv::Mat imgTranslation2(cv::Mat & srcImage, int xOffset, int yOffset)
{
	// ����ƽ�Ƴߴ�
	int nRows = srcImage.rows + abs(yOffset);
	int nCols = srcImage.cols + abs(xOffset);
	cv::Mat resultImage(nRows, nCols,
		srcImage.type());
	for (int i = 0; i < nRows; ++i) {
		for (int j = 0; j < nCols; ++j) {
			// ӳ��任
			int x = j - xOffset;
			int y = i - yOffset;
			// �߽��ж�
			if (x >= 0 && y >= 0 && x < nCols && y < nRows)
				resultImage.at<cv::Vec3b>(i, j) =
				srcImage.ptr<cv::Vec3b>(y)[x];
		}
	}
	return resultImage;
}
/**
 * @brief  ���ڵȼ����ȡͼ������
 * @param  
 * @return 
 */
cv::Mat imgScale(cv::Mat &srcImage, float kx, float ky)
{
	// ��ȡ���ͼ��ֱ���
	int nRows = cvRound(srcImage.rows * kx);
	int nCols = cvRound(srcImage.cols * ky);
	cv::Mat resultImage(nRows, nCols, srcImage.type());
	for (int i = 0; i < nRows; ++i)
	{
		for (int j = 0; j < nCols; ++j)
		{
			// ����ˮƽ���Ӽ�������
			int x = static_cast<int>((i + 1) / kx + 0.5) - 1;
			// ���ݴ�ֱ���Ӽ�������
			int y = static_cast<int>((j + 1) / ky + 0.5) - 1;
			resultImage.at<cv::Vec3b>(i, j) =
				srcImage.at<cv::Vec3b>(x, y);
		}
	}
	return resultImage;
}
/**
 * @brief  ͼ����תһ���Ƕ�
 * @param  
 * @return 
 */
cv::Mat imgRotate(cv::Mat& src, int angle)
{
	cv::Point2f center(src.cols / 2, src.rows / 2);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1);
	cv::Rect bbox = cv::RotatedRect(center, src.size(), angle).boundingRect();

	rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

	cv::Mat dst;
	cv::warpAffine(src, dst, rot, bbox.size());
	return dst;
}

/**
 * @brief  3�����任
 * @param  
 * @return 
 */
cv::Mat imgAffine(cv::Mat& src)
{
	int nRows = src.rows;
	int nCols = src.cols;
	//�������任�Ķ�ά������
	//Դͼ���Ŀ��ͼ���Ӧӳ�������
	cv::Point2f srcPoint[3];
	cv::Point2f resPoint[3];
	srcPoint[0] = cv::Point2f(0, 0);
	srcPoint[1] = cv::Point2f(nCols - 1, 0);
	srcPoint[2] = cv::Point2f(0, nRows - 1);
	resPoint[0] = cv::Point2f(nCols * 0, nRows*0.33);
	resPoint[1] = cv::Point2f(nCols*0.85, nRows*0.25);
	resPoint[2] = cv::Point2f(nCols*0.15, nRows*0.7);
	// �������任����2X3
	cv::Mat warpMat(cv::Size(2, 3), CV_32F);
	cv::Mat dst;
	cv::Mat::zeros(nRows, nCols, src.type());
	// �������任���󣬼�����任��2*3����
	warpMat = cv::getAffineTransform(srcPoint, resPoint);
	// ���ݷ���������ͼ�����任
	cv::warpAffine(src, dst, warpMat, dst.size());
	return dst;
}
