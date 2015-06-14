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
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <functional>

class IAlgorithm
{
protected:
	cv::Mat output;
	std::function<void()> processing_handler;

public:
	virtual ~IAlgorithm() {}

	virtual int get_segment_count() const { return 0;}
	virtual std::string get_name() const = 0;
	virtual std::string process(const cv::Mat& mat) = 0;
	virtual cv::Mat get_output_image() const { return output; }

	virtual Gtk::Widget* get_tuner_widget() { return nullptr; }

	void set_processing_handler(std::function<void()> proc_handler) { processing_handler = proc_handler; }
};
#define DEBUG_MODE
#ifdef DEBUG_MODE
#define LOG_D(x) do {std::cout << x << std::endl;} while (0)
inline void show_wnd(const std::string& name, const cv::Mat& mat)
{
	cv::imshow(name, mat);
}

#else
#define LOG_D(X)
inline void show_wnd(const std::string& name, const cv::Mat& mat)
{
}
#endif

#endif /* SRC_IALGORITHM_H_ */
