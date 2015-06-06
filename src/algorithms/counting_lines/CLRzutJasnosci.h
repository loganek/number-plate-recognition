/*
 * CLRzutJasnosci.h
 *
 *  Created on: Jun 6, 2015
 *      Author: mkolny
 */

#ifndef CLRZUTJASNOSCI_H_
#define CLRZUTJASNOSCI_H_

#include "algorithms/IAlgorithm.h"

class CLRzutJasnosci : public IAlgorithm
{
public:
	virtual ~CLRzutJasnosci() {}

	std::string get_name() const override;
	std::string process(const cv::Mat& mat) override;
};

#endif /* CLRZUTJASNOSCI_H_ */
