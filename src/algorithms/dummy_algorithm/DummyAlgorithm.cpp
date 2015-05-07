/*
 * DummyAlgorithm.cpp
 *
 *  Created on: Mar 3, 2015
 *      Author: loganek
 */

#include "DummyAlgorithm.h"
#include "sigc++lambda-hack.h"

#include <opencv2/imgproc/imgproc.hpp>

DummyAlgorithm::DummyAlgorithm()
{
	scale.set_range(0, 255);
	scale.set_value(100);
	box.pack_start(scale);
	scale.show();
	canny_threshold1.set_range(0, 255);
	canny_threshold1.set_value(100);
	box.pack_start(canny_threshold1);
	canny_threshold1.show();
	canny_threshold2.set_range(0, 255);
	canny_threshold2.set_value(100);
	box.pack_start(canny_threshold2);
	canny_threshold2.show();

	scale.signal_change_value().connect([this](Gtk::ScrollType,double){processing_handler(); return true;});
	canny_threshold1.signal_change_value().connect([this](Gtk::ScrollType,double){processing_handler(); return true;});
	canny_threshold2.signal_change_value().connect([this](Gtk::ScrollType,double){processing_handler(); return true;});
}

std::string DummyAlgorithm::get_name() const
{
	return "Test Algorithm";
}

std::string DummyAlgorithm::process(const cv::Mat& mat)
{
	cv::cvtColor(mat, output, CV_BGR2GRAY);

	cv::threshold(output, output, scale.get_value(), 255, 0);

	cv::cvtColor(output, output, CV_GRAY2BGR);

	return "empty string";
}


Gtk::Widget* DummyAlgorithm::get_tuner_widget()
{
	return &box;
}
