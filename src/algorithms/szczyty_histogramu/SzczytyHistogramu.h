/*
 * SzczytyHistogramu.h
 *
 *  Created on: 27 maj 2015
 *      Author: loganek
 */

#ifndef SRC_ALGORITHMS_SZCZYTY_HISTOGRAMU_SZCZYTYHISTOGRAMU_H_
#define SRC_ALGORITHMS_SZCZYTY_HISTOGRAMU_SZCZYTYHISTOGRAMU_H_

#include "algorithms/IAlgorithm.h"

class SzczytyHistogramu : public IAlgorithm
{
public:
	SzczytyHistogramu(){}
	virtual ~SzczytyHistogramu() {}

	std::string get_name() const override;
	std::string process(const cv::Mat& mat) override;
};

#endif /* SRC_ALGORITHMS_SZCZYTY_HISTOGRAMU_SZCZYTYHISTOGRAMU_H_ */
