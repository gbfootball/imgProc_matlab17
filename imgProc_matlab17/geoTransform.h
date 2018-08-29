#pragma once
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

cv::Mat imgTranslation1(cv::Mat & srcImage, int xOffset, int yOffset);
cv::Mat imgTranslation2(cv::Mat & srcImage, int xOffset, int yOffset);
cv::Mat imgScale(cv::Mat &srcImage, float kx, float ky);
cv::Mat imgRotate(cv::Mat& src, int angle);
cv::Mat imgAffine(cv::Mat& src);