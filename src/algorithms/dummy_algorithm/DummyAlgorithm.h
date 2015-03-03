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
	Gtk::Box box;
	Gtk::Scale scale;

public:
	DummyAlgorithm();
	virtual ~DummyAlgorithm() {}

	std::string get_name() const override;
	std::string process(const cv::Mat& mat) override;

	Gtk::Widget* get_tuner_widget() override;
};


#endif /* DUMMYALGORITHM_H_ */
