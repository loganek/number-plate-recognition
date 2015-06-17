/*
 * SauvolPietikainen.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: mkolny
 */

#include "SauvolPietikainen.h"
#include "../utils.h"

#define RIDGES_DISTANCE_DIVISOR 6
#define MIN_RIDGE_WIDTH 3

std::vector<uchar> get_values_from_range(const cv::Mat& mat, int r, int c, int range)
{
	std::vector<uchar> values;

	for(int i=std::max(0, r-range); i< std::min(mat.rows, r+range); i++)
	{
		for(int j=std::max(0, c-range); j<std::min(mat.cols, c+range); j++)
		{
			values.push_back(mat.at<uchar>(i,j));
		}
	}

	return values;
}

static uchar calculate_sp_value(const cv::Mat& mat, int i, int j, int range, int k)
{
	auto vals = get_values_from_range(mat, i, j, range);
	auto sum = std::accumulate(vals.begin(), vals.end(), 0);

	double mean = sum / vals.size();
	double accum = 0.0;
	std::for_each (std::begin(vals), std::end(vals), [&](const double d) {
	    accum += (d - mean) * (d - mean);
	});

	double stdev = std::sqrt(accum / (vals.size()-1));
	auto R = 128.0;

	return mean * (1 + k * (stdev/R-1));
}

static std::vector<cv::Point2i> find_init_points(const cv::Mat& input)
{
	cv::Mat binary_output(input.rows, input.cols, input.type(), cv::Scalar(255));

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

#include <opencv2/imgproc/imgproc.hpp>
std::vector<cv::Rect> SauvolPietikainen::internal_process()
{
	std::vector<cv::Rect> bounding_rectangles;
	double range = output.cols * S*0.5;
	for(int i=0; i< output.rows; i++)
	{
		for(int j=0; j<output.cols; j++)
		{
			auto tmp = output.at<uchar>(i,j);
			output.at<uchar>(i,j) =
					calculate_sp_value(output, i, j, std::max(1, (int)range), K) < tmp ? 255 : 0;
		}
	}

	auto init_points = find_init_points(output);
	cv::Mat tmp(output.rows, output.cols, output.type(), cv::Scalar(255, 0, 0));

	for (auto p : init_points)
	{
		cv::Rect rect(p.x, p.y, 0, 0);
		region_bounding_box(output, tmp, p, 127, rect);
		bounding_rectangles.push_back(rect);
	}

	return bounding_rectangles;
}
