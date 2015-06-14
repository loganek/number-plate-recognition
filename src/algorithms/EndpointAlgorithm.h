/*
 * EndpointAlgorithm.h
 *
 *  Created on: Jun 14, 2015
 *      Author: mkolny
 */

#ifndef SRC_ALGORITHMS_ENDPOINTALGORITHM_H_
#define SRC_ALGORITHMS_ENDPOINTALGORITHM_H_

#include "IAlgorithm.h"

class EndpointAlgorithm : public IAlgorithm
{
protected:
	virtual std::vector<cv::Rect> internal_process() = 0;
	double size_epsilon;
	int segment_count = 0;
public:
	virtual ~EndpointAlgorithm() {}

	std::string process(const cv::Mat& mat) override;
	int get_segment_count() const override { return segment_count;}
};


#endif /* SRC_ALGORITHMS_ENDPOINTALGORITHM_H_ */
