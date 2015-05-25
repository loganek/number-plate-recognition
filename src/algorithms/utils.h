/*
 * utils.h
 *
 *  Created on: 25 maj 2015
 *      Author: loganek
 */

#ifndef SRC_ALGORITHMS_UTILS_H_
#define SRC_ALGORITHMS_UTILS_H_

#include <opencv2/core/mat.hpp>

enum class ExtremumType
{
	MINIMUM,
	MAXIMUM
};

void region_growing(const cv::Mat& input, cv::Mat& output, const cv::Point2i& seed, int relative_threshold);
void region_bounding_box(const cv::Mat& input, cv::Mat& output, const cv::Point2i& seed, int threshold, cv::Rect& bounding);
void draw_plot(const std::vector<int>& v, const std::string& name, bool horizontal = true, cv::Scalar fg_color = cv::Scalar(0), cv::Scalar bg_color = cv::Scalar(255));
std::vector<int> find_local_extremum(const std::vector<int>& input, ExtremumType ex_type, int epsilon=1);
std::vector<int> smooth_histogram(const std::vector<int>& input, std::size_t epsilon = 1);

#endif /* SRC_ALGORITHMS_UTILS_H_ */
