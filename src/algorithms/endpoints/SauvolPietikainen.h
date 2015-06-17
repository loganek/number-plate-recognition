/*
 * SauvolPietikainen.h
 *
 *  Created on: Jun 16, 2015
 *      Author: mkolny
 */

#ifndef SRC_ALGORITHMS_ENDPOINTS_SAUVOLPIETIKAINEN_H_
#define SRC_ALGORITHMS_ENDPOINTS_SAUVOLPIETIKAINEN_H_

#include "algorithms/EndpointAlgorithm.h"

class SauvolPietikainen : public EndpointAlgorithm
{
	std::vector<cv::Rect> internal_process() override;
public:
	SauvolPietikainen(){}
	virtual ~SauvolPietikainen() {}
	std::string get_name() const override { return "Sauvol Pietikainen"; }
	double S = 0.1;
	double K = 0.1;
};

#endif /* SRC_ALGORITHMS_ENDPOINTS_SAUVOLPIETIKAINEN_H_ */
