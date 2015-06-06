/*
 * CLRzutJasnosci.cpp
 *
 *  Created on: Jun 6, 2015
 *      Author: mkolny
 */

#include "CLRzutJasnosci.h"
#include "../utils.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

std::string CLRzutJasnosci::get_name() const
{
	return "CL Rzut jasnosci";
}

static bool check_distance(const std::vector<int>& greatest_extrema, int distance, int pos)
{
	for (auto ge : greatest_extrema)
	{
		if (std::abs(pos - ge) < distance)
			return false;
	}
	return true;
}

static std::vector<int> find_n_greatest_extrema(const std::vector<int>& histogram, int distance, const std::vector<int>& extremum, int n)
{
	std::vector<int> greatest_extrema;

	for (int j = 0; j < n; j++)
	{
		int max = -1, max_i = -1;
		for (std::size_t i = 0; i < extremum.size(); i++)
		{
			if (histogram[extremum[i]] > max &&
					check_distance(greatest_extrema, distance, extremum[i]))
			{
				max_i = extremum[i];
				max = histogram[extremum[i]];
			}
		}
		greatest_extrema.push_back(max_i);
	}
	return greatest_extrema;
}

std::string CLRzutJasnosci::process(const cv::Mat& mat)
{
	cv::cvtColor(mat, output, CV_BGR2GRAY);
	cv::equalizeHist(output, output);

	std::vector<int> brightness_v;

	for (int i = 0; i < output.rows; i++)
	{
		brightness_v.push_back(0);
		for (int j = 0; j < output.cols; j++)
			brightness_v.back() += output.at<uchar>(i, j);
	}

	brightness_v = smooth_histogram(brightness_v, output.rows/30);
	draw_plot(brightness_v, "v", false);
	auto max = find_local_extremum(brightness_v, ExtremumType::MAXIMUM, 1);
	auto tgmax = find_n_greatest_extrema(brightness_v, output.rows / 3, max, 3);

	cv::cvtColor(output, output, CV_GRAY2BGR);

	if (std::find(tgmax.begin(), tgmax.end(), -1) != tgmax.end())
	{
		return "1";
	}
	else
	{
		std::sort(tgmax.begin(), tgmax.end());
		if (brightness_v[tgmax[1]] > brightness_v[tgmax[0]] && brightness_v[tgmax[1]] > brightness_v[tgmax[2]])
			return "2";
		else
			return "1";
	}
}
