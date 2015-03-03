/*
 * DummyAlgorithm.cpp
 *
 *  Created on: Mar 3, 2015
 *      Author: loganek
 */

#include "DummyAlgorithm.h"

#include <opencv2/imgproc/imgproc.hpp>

std::string DummyAlgorithm::get_name() const
{
	return "Dummy Algorithm";
}

std::string DummyAlgorithm::process(const cv::Mat& mat)
{
	cv::cvtColor(mat, output, CV_BGR2GRAY);

	cv::threshold(output, output, 100, 255, 0);

	cv::cvtColor(output, output, CV_GRAY2BGR);

	return "empty string";
}
