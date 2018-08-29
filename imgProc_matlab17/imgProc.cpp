#include "imgProc.h"

imgProc::imgProc()
{
}

imgProc::imgProc(const cv::String & filename)
{
	img = cv::imread(filename);
}

cv::Mat imgProc::read(const cv::String & filename)
{
	img = cv::imread(filename);
	return img;
}

cv::Mat imgProc::bgr()
{
	cv::Mat img_bgr;
	if (img.channels() == 3) {
		img_bgr = img;
	}
	else if (img.channels() == 1) {
		cv::cvtColor(img, img_bgr, CV_GRAY2BGR);
	}
	return img_bgr;
}

cv::Mat imgProc::rgb()
{
	cv::Mat img_rgb;
	if (img.channels() == 3) {
		cv::cvtColor(img, img_rgb, CV_BGR2RGB);
	}
	else if (img.channels() == 1) {
		cv::cvtColor(img, img_rgb, CV_GRAY2RGB);
	}
	return img_rgb;
}

cv::Mat imgProc::rgb2gray(cv::Mat & rgb)
{
	cv::Mat img_gray;
	if (rgb.channels() == 3) {
		cv::cvtColor(rgb, img_gray, CV_RGB2GRAY);
	}
	else if (rgb.channels() == 1) {
		img_gray = rgb;
	}
	return img_gray;
}

cv::Mat imgProc::rgb2bw(cv::Mat & rgb)
{
	cv::Mat img_bw;
	cv::threshold(rgb2gray(rgb), img_bw, 0, 255, CV_THRESH_OTSU);
	return img_bw;
}

cv::Mat imgProc::rgb2hls(cv::Mat & rgb)
{
	cv::Mat img_hls;
	if (rgb.channels() == 3) {
		cv::cvtColor(rgb, img_hls, CV_RGB2HLS);
	}
	return img_hls;
}

cv::Mat imgProc::rgb2hsv(cv::Mat & rgb)
{
	cv::Mat img_hsv;
	if (rgb.channels() == 3) {
		cv::cvtColor(rgb, img_hsv, CV_RGB2HSV);
	}
	return img_hsv;
}

cv::Mat imgProc::rgb2ycrcb(cv::Mat & rgb)
{
	cv::Mat img_ycrcb;
	if (rgb.channels() == 3) {
		cv::cvtColor(rgb, img_ycrcb, CV_RGB2YCrCb);
	}
	return img_ycrcb;
}

imgProc::~imgProc()
{
}


