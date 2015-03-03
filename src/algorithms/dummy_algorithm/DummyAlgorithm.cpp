/*
 * DummyAlgorithm.cpp
 *
 *  Created on: Mar 3, 2015
 *      Author: loganek
 */

#include "DummyAlgorithm.h"

std::string DummyAlgorithm::get_name() const
{
	return "Dummy Algorithm";
}

std::string DummyAlgorithm::process(const cv::Mat& mat)
{
	return "empty string";
}

cv::Mat DummyAlgorithm::get_debug_image() const
{
	return cv::Mat();
}
