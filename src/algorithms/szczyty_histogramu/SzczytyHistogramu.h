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
	virtual int get_threshold_value(int max_pos1, int max_pos2, const std::vector<int>& histogram) = 0;
public:
	SzczytyHistogramu(){}
	virtual ~SzczytyHistogramu() {}

	std::string process(const cv::Mat& mat) override;
};

#endif /* SRC_ALGORITHMS_SZCZYTY_HISTOGRAMU_SZCZYTYHISTOGRAMU_H_ */
