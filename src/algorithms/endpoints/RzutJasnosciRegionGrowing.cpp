/*
 * RzutJasnosciRegionGrowing.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: mkolny
 */

#include "RzutJasnosciRegionGrowing.h"
#include "../utils.h"

#include <opencv2/imgproc/imgproc.hpp>

static void get_brightnesses(const cv::Mat& mat, std::vector<int>& b_h, std::vector<int>& b_v)
{
	for (int i = 0; i < mat.cols; i++)
	{
		b_h.push_back(0);
			for (int j = 0; j < mat.rows; j++)
				b_h.back() += mat.at<uchar>(j, i);
	}

		for (int i = 0; i < mat.rows; i++)
		{
			b_v.push_back(0);
			for (int j = 0; j < mat.cols; j++)
				b_v.back() += mat.at<uchar>(i, j);
		}
}

static std::vector<int> get_two_greatest_values(std::vector<int> histogram, int epsilon)
{
	std::vector<int> greatest_extrema;

	auto me = std::max_element(histogram.begin(), histogram.end());
	auto pos = me - histogram.begin();
	greatest_extrema.push_back(pos);
	histogram.erase(me);

	while (true)
	{
		if (histogram.size() == 0)
			break;
		me = std::max_element(histogram.begin(), histogram.end());
		auto p2 = me - histogram.begin();
		if (std::abs(p2-pos) < epsilon)
			histogram.erase(me);
		else
		{
			greatest_extrema.push_back(p2);
			break;
		}
	}
	std::sort(greatest_extrema.begin(), greatest_extrema.end());
	return greatest_extrema;
}

std::vector<cv::Rect> RzutJasnosciRegionGrowing::internal_process()
{
	std::vector<cv::Rect> boxes;

	// sharpness
	cv::Mat mf;
	cv::GaussianBlur(output, mf, cv::Size(0, 0), 3);
	cv::addWeighted(output, 1.5, mf, -0.5, 0, mf);
	output = mf;
	show_wnd("after preproc", output);
	rotate_if_needed(output);
	auto mean = cv::mean(output).val[0];

	std::vector<int> brightness_h, brightness_v;
	get_brightnesses(output, brightness_h, brightness_v);

	brightness_h = smooth_histogram(brightness_h, 1);
	brightness_v = smooth_histogram(brightness_v, output.rows/20);

	auto maximums_v = find_local_extremum(brightness_v, ExtremumType::MINIMUM, 1);

	auto ge = get_two_greatest_values(brightness_v, output.rows / 4);

	if (ge.size() > 1)
	{
		if (ge[0] < output.rows/4)
		for (int i = 0; i < ge[0]-output.rows/20; i++)
			for (int j = 0; j < output.cols; j++)
				output.at<uchar>(i, j) = 255;

		auto tmp = output.rows * 3 /4;
		if (ge[1] > tmp)
			for (int i = tmp+output.rows/10; i < output.rows; i++)
				for (int j = 0; j < output.cols; j++)
					output.at<uchar>(i, j) = 255;

	}

	auto maximums = find_local_extremum(brightness_h, ExtremumType::MINIMUM, 1);

	draw_plot(brightness_h, "h");
	draw_plot(brightness_v, "v", false);
	cv::Mat real_output(output.rows, output.cols, output.type(), cv::Scalar(255, 0, 0));
		std::vector<cv::Rect> rects;
		std::vector<cv::Point2i> pts;

		for (auto m : maximums)
		{
			for (int i = 5; i < output.rows; i++)
			{
				if ( output.at<uchar>(i, m) < A)
				{
					region_growing(output, real_output, cv::Point2i(m, i), output.at<uchar>(i, m) + mean*B, false);
					pts.push_back(cv::Point2i(m, i));
				}
			}
		}

		cv::Mat tmp(output.rows, output.cols, output.type(), cv::Scalar(255, 0, 0));
		for (auto p : pts)
		{
			cv::Rect rect(p.x, p.y, 0, 0);
			region_bounding_box(real_output, tmp, p, 150, rect);
			if ((double)rect.height/rect.width > 1.2)
			{
				rect.width += 2; rect.height += 2;
				rect.x -= 2; rect.y -= 2;
				boxes.push_back(rect);
			}
		}

		for (auto r : rects)
			cv::rectangle(real_output, r.tl(), r.br(), cv::Scalar(100));

	//	cv::imshow("without borders", tmp);
	//	cv::imshow("ro", real_output);

	return boxes;
}


