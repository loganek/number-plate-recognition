/*
 * SzczytyHistogramu.cpp
 *
 *  Created on: 27 maj 2015
 *      Author: loganek
 */

#include "SzczytyHistogramu.h"
#include "../utils.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>

std::string SzczytyHistogramu::get_name() const
{
	return "Szczyty histogramu";
}
#include <iostream>
std::string SzczytyHistogramu::process(const cv::Mat& mat)
{
	cv::cvtColor(mat, output, CV_BGR2GRAY);
	convert_if_need(output);
	cv::Mat mf;
	cv::GaussianBlur(output, mf, cv::Size(0, 0), 3);
	cv::addWeighted(output, 1.5, mf, -0.5, 0, mf);
	cv::equalizeHist(output, output);
	output = mf;

	std::vector<int> histogram(256, 0);
	for (int i = 0; i < output.cols; i++)
		for (int j = 0; j < output.rows; j++)
			histogram[output.at<uchar>(j, i)]++;

	histogram = smooth_histogram(histogram, 8);
	auto extremum = find_local_extremum(histogram, ExtremumType::MAXIMUM, 3);
	draw_plot(histogram, "histogram");

	int m1 = -1, m2 = -1;
	int m1i = -100, m2i = -100;
	for (auto e : extremum)
	{
		if (histogram[e] > m1 && std::abs(e-m2i) > 80)
		{
			m1 = histogram[e];
			m1i = e;
			if (m1 > m2)
			{
				std::swap(m1i, m2i);
				std::swap(m1, m2);
			}
		}
		else if (histogram[e] > m2)
		{
			m2 = histogram[e];
			m2i = e;
		}
	}

	cv::threshold(output, output, std::abs((m1i-m2i)/2), 255, 0);
	cv::Mat real_output(mat.rows, mat.cols, output.type(), cv::Scalar(255, 0, 0));
	auto ctr = output.rows / 6;
	std::vector<cv::Point2i> pts;
	for (int i = ctr; i < output.rows; i+=ctr)
	{
		auto ridges = find_ridges(output, i, true, 3, 127);

		for (auto r : ridges)
		{
			region_growing(output, real_output, cv::Point2i(r, i), output.at<uchar>(i, r)+30);
			pts.push_back(cv::Point2i(r, i));
		}
	}
	cv::Mat tmp(mat.rows, mat.cols, output.type(), cv::Scalar(255, 0, 0));
	std::vector<cv::Rect> rects;

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
	imshow("out", tmp);
	cv::cvtColor(real_output, output, CV_GRAY2BGR);

	return "empty string";
}


