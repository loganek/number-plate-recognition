/*
 * SzczytyHistogramu.cpp
 *
 *  Created on: 27 maj 2015
 *      Author: loganek
 */

#include "SzczytyHistogramu.h"
#include "SzczytySrednia.h"
#include "SzczytyMinimum.h"

#include "../utils.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>

#include <iostream>

int SzczytySrednia::get_threshold_value(int max_pos1, int max_pos2, const std::vector<int>& histogram)
{
	return std::min(max_pos1, max_pos2) + std::abs((max_pos1-max_pos2)/2);
}

int SzczytyMinimum::get_threshold_value(int max_pos1, int max_pos2, const std::vector<int>& histogram)
{
	int i1, i2;
	if (max_pos1 < max_pos2) { i1 = max_pos1; i2 = max_pos2; }
	else { i2 = max_pos1; i1 = max_pos2; }

	int min_value = 256;
	int min_pos = 0;

	for (int i = i1; i < i2; i++)
		if (histogram[i] < min_value)
		{
			min_value = histogram[i];
			min_pos = i;
		}
	return min_pos;
}

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
	int threshold_value = get_threshold_value(m1i, m2i, histogram);
	std::cout << "Threshold value: " << threshold_value << std::endl;
	cv::threshold(output, output, threshold_value, 255, 0);
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
	int max_y = 0, min_y = output.rows;
	int sum_heights = 0;
	for (auto p : pts)
	{
		cv::Rect rect(p.x, p.y, 0, 0);
		region_bounding_box(real_output, tmp, p, 150, rect);
		if (rect.height <= output.rows/4)
			continue;
		if ((double)rect.height/rect.width > 1.2)
		{
			rect.width += 2; rect.height += 2;
			rect.x -= 2; rect.y -= 2;
			rects.push_back(rect);
			if (rect.y < min_y) min_y = rect.y;
			if (rect.y > max_y) max_y = rect.y;
			sum_heights += rect.height;
		}
	}

	if (rects.empty())
		std::cout << "Whoops! No characters found";
	else
	{
		bool one_liner = static_cast<std::size_t>(max_y - min_y) < sum_heights/rects.size();

		std::cout << "lines cout: " << (one_liner ? 1 : 2) << std::endl;

		for (auto r : rects)
			cv::rectangle(real_output, r.tl(), r.br(), cv::Scalar(100));
		imshow("out", tmp);
	}

	cv::cvtColor(real_output, output, CV_GRAY2BGR);

	return "empty string";
}


