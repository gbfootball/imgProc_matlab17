#include "geoTransform.h"

/**
 * @brief  平移操作 图像大小不变
 * @param  
 * @return 
 */
cv::Mat imgTranslation1(cv::Mat & srcImage, int xOffset, int yOffset)
{
	int nRows = srcImage.rows;
	int nCols = srcImage.cols;
	cv::Mat resultImage(srcImage.size(),
		srcImage.type());
	// 遍历图像
	for (int i = 0; i < nRows; ++i) {
		for (int j = 0; j < nCols; ++j) {
			// 映射变换
			int x = j - xOffset;
			int y = i - yOffset;
			// 边界判断
			if (x >= 0 && y >= 0 && x < nCols && y < nRows)
				resultImage.at<cv::Vec3b>(i, j) =
				srcImage.ptr<cv::Vec3b>(y)[x];
		}
	}
	return resultImage;
}
/**
 * @brief  平移操作 图像大小改变
 * @param  
 * @return 
 */
cv::Mat imgTranslation2(cv::Mat & srcImage, int xOffset, int yOffset)
{
	// 设置平移尺寸
	int nRows = srcImage.rows + abs(yOffset);
	int nCols = srcImage.cols + abs(xOffset);
	cv::Mat resultImage(nRows, nCols,
		srcImage.type());
	for (int i = 0; i < nRows; ++i) {
		for (int j = 0; j < nCols; ++j) {
			// 映射变换
			int x = j - xOffset;
			int y = i - yOffset;
			// 边界判断
			if (x >= 0 && y >= 0 && x < nCols && y < nRows)
				resultImage.at<cv::Vec3b>(i, j) =
				srcImage.ptr<cv::Vec3b>(y)[x];
		}
	}
	return resultImage;
}
/**
 * @brief  基于等间隔提取图像缩放
 * @param  
 * @return 
 */
cv::Mat imgScale(cv::Mat &srcImage, float kx, float ky)
{
	// 获取输出图像分辨率
	int nRows = cvRound(srcImage.rows * kx);
	int nCols = cvRound(srcImage.cols * ky);
	cv::Mat resultImage(nRows, nCols, srcImage.type());
	for (int i = 0; i < nRows; ++i)
	{
		for (int j = 0; j < nCols; ++j)
		{
			// 根据水平因子计算坐标
			int x = static_cast<int>((i + 1) / kx + 0.5) - 1;
			// 根据垂直因子计算坐标
			int y = static_cast<int>((j + 1) / ky + 0.5) - 1;
			resultImage.at<cv::Vec3b>(i, j) =
				srcImage.at<cv::Vec3b>(x, y);
		}
	}
	return resultImage;
}
/**
 * @brief  图像旋转一定角度
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
 * @brief  3点仿射变换
 * @param  
 * @return 
 */
cv::Mat imgAffine(cv::Mat& src)
{
	int nRows = src.rows;
	int nCols = src.cols;
	//定义仿射变换的二维点数组
	//源图像和目标图像对应映射的三点
	cv::Point2f srcPoint[3];
	cv::Point2f resPoint[3];
	srcPoint[0] = cv::Point2f(0, 0);
	srcPoint[1] = cv::Point2f(nCols - 1, 0);
	srcPoint[2] = cv::Point2f(0, nRows - 1);
	resPoint[0] = cv::Point2f(nCols * 0, nRows*0.33);
	resPoint[1] = cv::Point2f(nCols*0.85, nRows*0.25);
	resPoint[2] = cv::Point2f(nCols*0.15, nRows*0.7);
	// 定义仿射变换矩阵2X3
	cv::Mat warpMat(cv::Size(2, 3), CV_32F);
	cv::Mat dst;
	cv::Mat::zeros(nRows, nCols, src.type());
	// 计算仿射变换矩阵，即仿射变换的2*3数组
	warpMat = cv::getAffineTransform(srcPoint, resPoint);
	// 根据仿射矩阵计算图像仿射变换
	cv::warpAffine(src, dst, warpMat, dst.size());
	return dst;
}
