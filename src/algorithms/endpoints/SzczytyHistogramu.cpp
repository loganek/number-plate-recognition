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

#define MIN_DISTANCE_BETWEEN_EXTREMA 80
#define RIDGES_DISTANCE_DIVISOR 6
#define MIN_RIDGE_WIDTH 3



int SzczytySrednia::get_threshold_value(int max_pos1, int max_pos2, const std::vector<int>& histogram)
{
	return std::min(max_pos1, max_pos2) + std::abs((max_pos1-max_pos2)/2);
}

int SzczytyMinimum::get_threshold_value(int max_pos1, int max_pos2, const std::vector<int>& histogram)
{
	if (max_pos1 > max_pos2)
		std::swap(max_pos1, max_pos2);

	int min_value = 256;
	int min_pos = 0;

	/*int start1 = (max_pos2 - max_pos1)/2 +max_pos1;
	int j = 0;

	for (int i = start1; i < max_pos2; i++)
	{
		if (histogram[i] < min_value)
		{
			min_value = histogram[i];
			min_pos = i;
		}

		if (histogram[start1-j] < min_value)
		{
			min_value = histogram[start1-j];
			min_pos = start1-j;
		}
		j--;
	}*/

	for (int i = max_pos1; i < max_pos2; i++)
	{
		if (histogram[i] < min_value)
		{
			min_value = histogram[i];
			min_pos = i;
		}
	}

	return min_pos;
}

static std::pair<int, int> find_two_greatest_extrema(const std::vector<int>& histogram)
{
	int m1 = -1, m2 = -1;
	int m1i = 0, m2i = 0;
	auto extrema = find_local_extremum(histogram, ExtremumType::MAXIMUM, 1);
	for (auto e : extrema)
	{
		if (histogram[e] > m1 && std::abs(e-m2i) > MIN_DISTANCE_BETWEEN_EXTREMA)
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

	return std::make_pair(m1i, m2i);
}

static std::vector<cv::Point2i> find_init_points(const cv::Mat& input, cv::Mat& binary_output)
{
	auto inc_value = input.rows / RIDGES_DISTANCE_DIVISOR;
	std::vector<cv::Point2i> ridge_points;
	for (int i = inc_value; i < input.rows; i += inc_value)
	{
		for (auto r : find_ridges(input, i, true, MIN_RIDGE_WIDTH, 127))
		{
			region_growing(input, binary_output, cv::Point2i(r, i), input.at<uchar>(i, r)+30);
			ridge_points.push_back(cv::Point2i(r, i));
		}
	}

	return ridge_points;
}

static int get_lines_count(const std::vector<cv::Rect>& bounding_rectangles, const cv::Mat& image)
{
	if (bounding_rectangles.empty())
		return -1;

	int max_y = 0, min_y = image.rows;
	int sum_heights = 0;
	for (auto rect : bounding_rectangles)
	{
		if (rect.y < min_y) min_y = rect.y;
		if (rect.y > max_y) max_y = rect.y;
		sum_heights += rect.height;
	}

	return static_cast<std::size_t>(max_y - min_y) < sum_heights/bounding_rectangles.size() ? 1 : 2;
}

std::vector<cv::Rect> SzczytyHistogramu::internal_process()
{
	cv::Mat mf;
	cv::GaussianBlur(output, mf, cv::Size(0, 0), 3);
	cv::addWeighted(output, 1.5, mf, -0.5, 0, mf);
	output = mf;
	show_wnd("after preproc", output);
	auto histogram = smooth_histogram(build_histogram(output), 28);
#ifdef DEBUG_MODE
	draw_plot(histogram, "histogram", true, true);
#endif

	auto greatest_extrema = find_two_greatest_extrema(histogram);
	int threshold_value = get_threshold_value(greatest_extrema.first, greatest_extrema.second, histogram);
	LOG_D("Threshold value: " << threshold_value);
	cv::threshold(output, output, threshold_value, 255, 0);
	show_wnd("szczyty after threshold", output);

	cv::Mat binary_output(output.rows, output.cols, output.type(), cv::Scalar(255));

	cv::Mat tmp(output.rows, output.cols, output.type(), cv::Scalar(255, 0, 0));
	std::vector<cv::Rect> bounding_rectangles;

	auto init_points = find_init_points(output, binary_output);
	output = binary_output;
	for (auto p : init_points)
	{
		cv::Rect rect(p.x, p.y, 0, 0);
		region_bounding_box(output, tmp, p, 127, rect);

		bounding_rectangles.push_back(rect);
	}

	if (bounding_rectangles.empty())
		LOG_D("Whoops! No characters found");
	else
	{
		LOG_D("lines cout: " << get_lines_count(bounding_rectangles, output));
	}

	return bounding_rectangles;
}
