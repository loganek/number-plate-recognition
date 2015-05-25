/*
 * RzutJasnosci.h
 *
 *  Created on: 21 maj 2015
 *      Author: loganek
 */

#ifndef SRC_ALGORITHMS_RZUT_JASNOSCI_RZUTJASNOSCI_H_
#define SRC_ALGORITHMS_RZUT_JASNOSCI_RZUTJASNOSCI_H_

#include "algorithms/IAlgorithm.h"

class RzutJasnosci : public IAlgorithm
{
public:
	RzutJasnosci();
	virtual ~RzutJasnosci() {}

	std::string get_name() const override;
	std::string process(const cv::Mat& mat) override;
};

#endif /* SRC_ALGORITHMS_RZUT_JASNOSCI_RZUTJASNOSCI_H_ */
