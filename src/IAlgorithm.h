/*
 * IAlgorithm.h
 *
 *  Created on: Mar 2, 2015
 *      Author: loganek
 */

#ifndef SRC_IALGORITHM_H_
#define SRC_IALGORITHM_H_

#include <opencv2/core/core.hpp>

class IAlgorithm
{
public:
	virtual ~IAlgorithm() {}

	virtual std::string get_name() const = 0;
	virtual std::string process(const cv::Mat& mat) = 0;
	virtual cv::Mat get_debug_image() const;
};


#endif /* SRC_IALGORITHM_H_ */
