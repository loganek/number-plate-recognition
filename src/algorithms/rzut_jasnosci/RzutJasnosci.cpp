/*
 * RzutJasnosci.cpp
 *
 *  Created on: 21 maj 2015
 *      Author: loganek
 */

#include "RzutJasnosci.h"
#include "../utils.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <algorithm>
#include <cassert>

RzutJasnosci::RzutJasnosci()
{
}

std::string RzutJasnosci::get_name() const
{
	return "Rzut jasności";
}

std::string RzutJasnosci::process(const cv::Mat& mat)
{
	cv::cvtColor(mat, output, CV_BGR2GRAY);
	convert_if_need(output);
	cv::Mat mf;
	cv::GaussianBlur(output, mf, cv::Size(0, 0), 3);
	cv::addWeighted(output, 1.5, mf, -0.5, 0, mf);
	output = mf;

	cv::equalizeHist(output, output);

	cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size( 2, 2 ));
	cv::morphologyEx( output, output, cv::MORPH_CLOSE, element );

	std::vector<int> brightness_h, brightness_v;
	for (int i = 0; i < output.cols; i++)
	{
		brightness_h.push_back(0);
		for (int j = 0; j < output.rows; j++)
			brightness_h.back() += output.at<uchar>(j, i);
	}

	for (int i = 0; i < output.rows; i++)
	{
		brightness_v.push_back(0);
		for (int j = 0; j < output.cols; j++)
			brightness_v.back() += output.at<uchar>(i, j);
	}

	brightness_h = smooth_histogram(brightness_h, 1);
	brightness_v = smooth_histogram(brightness_v, output.rows/20);

	draw_plot(brightness_h, "h");
	draw_plot(brightness_v, "v", false);
	cv::Mat real_output(mat.rows, mat.cols, output.type(), cv::Scalar(255, 0, 0));
	std::vector<cv::Rect> rects;
	std::vector<cv::Point2i> pts;
	auto maximums = find_local_extremum(brightness_h, ExtremumType::MINIMUM, 1);

	for (auto m : maximums)
	{
		for (int i = 5; i < output.rows; i++)
		{
			if ( output.at<uchar>(i, m) < 40)
			{
				region_growing(output, real_output, cv::Point2i(m, i), output.at<uchar>(i, m)+30);
				pts.push_back(cv::Point2i(m, i));
			}
		}
	}

	cv::Mat tmp(mat.rows, mat.cols, output.type(), cv::Scalar(255, 0, 0));
	for (auto p : pts)
	{
		cv::Rect rect(p.x, p.y, 0, 0);
		region_bounding_box(real_output, tmp, p, 150, rect);
		if ((double)rect.height/rect.width > 1.2)
		{
			rect.width += 2; rect.height += 2;
			rect.x -= 2; rect.y -= 2;
			rects.push_back(rect);
		}
	}

	for (auto r : rects)
		cv::rectangle(real_output, r.tl(), r.br(), cv::Scalar(100));

	cv::imshow("without borders", tmp);
	cv::imshow("ro", real_output);

	cv::cvtColor(output, output, CV_GRAY2BGR);

	return "empty string";
}
