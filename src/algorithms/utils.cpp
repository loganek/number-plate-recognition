/*
 * utils.cpp
 *
 *  Created on: 25 maj 2015
 *      Author: loganek
 */

#include "utils.h"

#include <opencv2/hal/defs.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define REGION_GROWING_RECURSIVE(OP_Y, OP_X) \
		if (relative_threshold > input.at<uchar>(seed.y OP_Y, seed.x OP_X) && output.at<uchar>(seed.y OP_Y, seed.x OP_X) != 0) \
		region_growing(input, output, cv::Point2i(seed.x OP_X, seed.y OP_Y), relative_threshold);

void region_growing(const cv::Mat& input, cv::Mat& output, const cv::Point2i& seed, int relative_threshold)
{
	if (seed.y < 2 || seed.x < 2 || seed.y >= output.rows-2 || seed.x >= output.cols-2)
		return;

	output.at<uchar>(seed.y, seed.x) = 0;
	REGION_GROWING_RECURSIVE(+0, -1)
	REGION_GROWING_RECURSIVE(+0, +1)
	REGION_GROWING_RECURSIVE(-1, +0)
	REGION_GROWING_RECURSIVE(+1, +0)
}

#define REGION_BOUNDING_BOX_RECURSIVE(OP_Y, OP_X) \
		if (input.at<uchar>(seed.y OP_Y, seed.x OP_X) < threshold && output.at<uchar>(seed.y OP_Y, seed.x OP_X) != 0) \
		region_bounding_box(input, output, cv::Point2i(seed.x OP_X, seed.y OP_Y), threshold, bounding);

void region_bounding_box(const cv::Mat& input, cv::Mat& output, const cv::Point2i& seed, int threshold, cv::Rect& bounding)
{
	if (seed.y < 2 || seed.x < 2 || seed.y >= input.rows-2 || seed.x >= input.cols-2)
		return;

	if (bounding.x > seed.x)
	{
		bounding.width += bounding.x - seed.x;
		bounding.x = seed.x;
	}
	if (bounding.y > seed.y)
	{
		bounding.height += bounding.y - seed.y;
		bounding.y = seed.y;
	}
	if (bounding.x + bounding.width < seed.x)
		bounding.width = seed.x - bounding.x;
	if (bounding.y + bounding.height < seed.y)
		bounding.height = seed.y - bounding.y;

	output.at<uchar>(seed.y, seed.x) = 0;
	REGION_BOUNDING_BOX_RECURSIVE(+0, -1)
	REGION_BOUNDING_BOX_RECURSIVE(+0, +1)
	REGION_BOUNDING_BOX_RECURSIVE(-1, +0)
	REGION_BOUNDING_BOX_RECURSIVE(+1, +0)
}

void draw_plot(const std::vector<int>& v, const std::string& name, bool horizontal, cv::Scalar fg_color, cv::Scalar bg_color)
{
	double hist_h = 400.0;
	cv::Mat histImage( horizontal ? hist_h : v.size(), horizontal ? v.size() : hist_h, CV_8UC1, bg_color);

	double sc = hist_h / *std::max_element(v.begin(), v.end());

	for(std::size_t i = 1; i < v.size(); i++)
	{
		if (horizontal)
			cv::line(histImage, cv::Point(i-1, v[i-1]*sc), cv::Point(i, v[i]*sc), fg_color);
		else
			cv::line(histImage, cv::Point(v[i-1]*sc, i-1), cv::Point(v[i]*sc, i), fg_color);
	}

	cv::imshow(name, histImage);
}

static bool is_extremum(const std::vector<int>& input, std::size_t pos, ExtremumType ex_type, int epsilon)
{
	auto cmp = ex_type == ExtremumType::MINIMUM ?
			[](int x, int y) {return x>y;} :
			[](int x, int y) {return x<y;};

	auto start = std::max<int>(pos-epsilon, 0);
	auto prev = input[start];
	for (auto i = start+1; i <= pos; i++)
	{
		if (cmp(input[i], prev))
			return false;
		prev = input[i];
	}
	prev = input[pos];
	for (auto i = std::min(pos+1, input.size()-1); i <= std::min(pos+epsilon, input.size()-1); i++)
	{
		if (cmp(prev, input[i]))
			return false;
		prev = input[i];
	}
	return true;
}

std::vector<int> find_local_extremum(const std::vector<int>& input, ExtremumType ex_type, int epsilon)
{
	std::vector<int> extms;

	for (std::size_t i = 0; i < input.size(); i++)
	{
		if (is_extremum(input, i, ex_type, epsilon))
			extms.push_back(i);
	}

	return extms;
}

std::vector<int> smooth_histogram(const std::vector<int>& input, std::size_t epsilon)
{
	std::vector<int> output;
	output.reserve(input.size());

	for (std::size_t i = 0; i < epsilon; i++)
		output.push_back(std::accumulate(input.begin(), input.begin()+epsilon+i, 0)/(epsilon+1+i));

	for (std::size_t i = epsilon; i < input.size()-epsilon; i++)
		output.push_back(std::accumulate(input.begin()+i-epsilon, input.begin()+i+epsilon, 0)/(epsilon*2+1));

	for (std::size_t i = input.size() - epsilon; i < input.size(); i++)
		output.push_back(std::accumulate(input.begin()+i-epsilon, input.end(), 0)/(epsilon+1+input.size()-i));

	return output;
}

// todo another method should be used
void convert_if_need(cv::Mat& mat)
{
	std::vector<int> bh;
	for (int i = 0; i < mat.cols; i++)
	{
		bh.push_back(0);
		for (int j = 0; j < mat.rows; j++)
			bh.back() += mat.at<uchar>(j, i);
	}
	bh = smooth_histogram(bh, 3);
	auto m = find_local_extremum(bh, ExtremumType::MAXIMUM, 4);
	int maxes = 0;
	for (std::size_t i = 0; i < m.size(); i++)
		maxes += bh[m[i]]; maxes = maxes / m.size();
	m = find_local_extremum(bh, ExtremumType::MINIMUM, 4);
	int mins = 0;
	for (std::size_t  i = 0; i < m.size(); i++)
		mins += bh[m[i]];
	mins = mins / m.size();

	if ((double)mins / maxes < 0.5)
		mat = cv::Scalar::all(255) - mat;
}
