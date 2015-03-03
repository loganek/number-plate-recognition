/*
 * DummyAlgorithm.h
 *
 *  Created on: Mar 3, 2015
 *      Author: loganek
 */

#ifndef DUMMYALGORITHM_H_
#define DUMMYALGORITHM_H_

#include "algorithms/IAlgorithm.h"

class DummyAlgorithm : public IAlgorithm
{
public:
	virtual ~DummyAlgorithm() {}

	std::string get_name() const override;
	std::string process(const cv::Mat& mat) override;
};


#endif /* DUMMYALGORITHM_H_ */
