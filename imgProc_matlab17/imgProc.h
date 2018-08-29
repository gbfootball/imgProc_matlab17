#pragma once
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


class imgProc
{
public:
	imgProc();
	~imgProc();
	imgProc(const cv::String& filename);
	cv::Mat read(const cv::String& filename);
	cv::Mat bgr();
	cv::Mat rgb();
	cv::Mat rgb2gray(cv::Mat & rgb);
	cv::Mat rgb2bw(cv::Mat & rgb);
	cv::Mat rgb2hls(cv::Mat & rgb);
	cv::Mat rgb2hsv(cv::Mat & rgb);
	cv::Mat rgb2ycrcb(cv::Mat & rgb);

	//Mat imgdilate(Mat & img, Mat & kern);
	//Mat imgerode(Mat & img, Mat & kern);
	//Mat imgopen(Mat & img, Mat & kern);
	//Mat imgclose(Mat & img, Mat & kern);
	//Mat imgtophat(Mat & img, Mat & kern);
	//Mat imgbothat(Mat & img, Mat & kern);

private:
	cv::Mat img;
	int morph_se = cv::MORPH_ELLIPSE;
	cv::Size morph_se_size = cv::Size(5, 5);
};

