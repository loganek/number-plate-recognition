/*
 * EndpointAlgorithm.cpp
 *
 *  Created on: Jun 14, 2015
 *      Author: mkolny
 */

#include "EndpointAlgorithm.h"
#include "utils.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <tesseract/baseapi.h>

#define MIN_H_W_RATIO  0.9
#define MIN_CHARACTER_HEIGHT_DIVISOR 4

static double average_bounding_box_height(const std::vector<cv::Rect>& rects, int imgheight)
{
	double sum = 0.0;
	imgheight = imgheight/2;
	int x = 0;
	for (auto r : rects)
	{
		if (r.height > imgheight)
		{
			sum += r.height;
			x++;
		}
	}
	return sum / x;
}

static bool in_range(double v, double mid, double epsilon)
{
	return v >= mid-epsilon && v <= mid+epsilon;
}

static bool verify_bounding_box(const cv::Rect& rect, const cv::Mat& image)
{
	return rect.height > image.rows/MIN_CHARACTER_HEIGHT_DIVISOR && (double)rect.height/rect.width > MIN_H_W_RATIO;
}

static void sort_bounding_boxes(std::vector<cv::Rect>& rects)
{
	std::sort(rects.begin(), rects.end(), [](const cv::Rect& r1, const cv::Rect& r2){
		return r1.x < r2.x;
	});
}

void check_rectangle(std::vector<cv::Rect>& rects)
{
	for (int i = 0; i < rects.size(); i++)
	{
		for (int j = 0; j < rects.size(); j++)
		{
			if (i == j)
				continue;
			auto pi = rects[i].x+rects[i].width;
			auto pj = rects[j].x+rects[j].width;
			if (rects[i].x >= rects[j].x-7 && rects[i].x <= pj &&
					pi >= rects[i].x && pi <= pj+10)
			{
				rects.erase(rects.begin() + i);
				i--;
				break;
			}
		}
	}
}

std::string EndpointAlgorithm::process(const cv::Mat& mat)
{
	size_epsilon = 0.2;
	cv::cvtColor(mat, output, CV_BGR2GRAY);

	if (cv::mean(output).val[0] < 50)
		cv::equalizeHist(output, output);

	//convert_if_need(output);
	show_wnd("pre pre processing", output);

	auto bounding_rectangles = internal_process();
//	LOG_D("from process: " << bounding_rectangles.size());
	for (std::size_t i = 0; i < bounding_rectangles.size(); i++)
		if (!verify_bounding_box(bounding_rectangles[i], output))
		{
			bounding_rectangles.erase(bounding_rectangles.begin()+i); i--;
		}
//	LOG_D("after 1st verify: " << bounding_rectangles.size());
	double avg = average_bounding_box_height(bounding_rectangles, output.rows);

	for (std::size_t i = 0; i < bounding_rectangles.size(); i++)
			if (!in_range(bounding_rectangles[i].height, avg, avg*size_epsilon))
			{
				bounding_rectangles.erase(bounding_rectangles.begin()+i); i--;
			}
//	LOG_D("after 2nd verify: " << bounding_rectangles.size());
	check_rectangle(bounding_rectangles);
//	LOG_D("after 3rd verify: " << bounding_rectangles.size());
	tesseract::TessBaseAPI tess;
	tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
	tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
	sort_bounding_boxes(bounding_rectangles);
	std::string out;
	for (auto rect : bounding_rectangles)
	{
		cv::Mat cropped = output(pump_rectangle(rect, 2, output)).clone();
		tess.SetImage((uchar*)cropped.data, cropped.cols, cropped.rows, 1, cropped.cols);
		std::string tmp(tess.GetUTF8Text());
		for (auto t : tmp)
		{
			auto x = ::toupper(t);
			if ((x >= 'A' && x <='Z') || (x >= '0' && x <= '9'))
				out += x;
		}
	}

#ifdef DEBUG_MODE
	// draw bounding rectangles
	for (auto rect : bounding_rectangles)
	{
		rect = pump_rectangle(rect, 2, output);
		cv::rectangle(output, rect.tl(), rect.br(), cv::Scalar(0));
	}
#endif

	cv::cvtColor(output, output, CV_GRAY2BGR);

	segment_count = bounding_rectangles.size();
	return out;
}
