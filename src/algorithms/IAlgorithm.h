/*
 * IAlgorithm.h
 *
 *  Created on: Mar 2, 2015
 *      Author: loganek
 */

#ifndef SRC_IALGORITHM_H_
#define SRC_IALGORITHM_H_

#include <gtkmm.h>
#include <opencv2/core/core.hpp>

#include <functional>

class IAlgorithm
{
protected:
	cv::Mat output;
	std::function<void()> processing_handler;

public:
	virtual ~IAlgorithm() {}

	virtual std::string get_name() const = 0;
	virtual std::string process(const cv::Mat& mat) = 0;
	virtual cv::Mat get_output_image() const { return output; }

	virtual Gtk::Widget* get_tuner_widget() { return nullptr; }

	void set_processing_handler(std::function<void()> proc_handler) { processing_handler = proc_handler; }
};


#endif /* SRC_IALGORITHM_H_ */
