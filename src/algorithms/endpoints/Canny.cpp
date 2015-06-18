/*
 * Canny.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: mkolny
 */

#include "Canny.h"

#include <opencv2/imgproc/imgproc.hpp>

std::vector<cv::Rect> Canny::internal_process()
{
	cv::Mat cannymat;
	cv::blur(output, cannymat, cv::Size(BS,BS) );

	cv::Canny( cannymat, cannymat, 60, 130, 3 );
	show_wnd("windcany", cannymat);
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours( cannymat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
	cv::imshow("canny", cannymat);
	std::vector<cv::Rect> rects( contours.size() );
	std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{
		cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true );
		rects[i] = cv::boundingRect( cv::Mat(contours_poly[i]) );
	}
	cv::RNG rng(12345);
	 cv::Mat drawing = cv::Mat::zeros( cannymat.size(), CV_8UC3 );
	  for( int i = 0; i< contours.size(); i++ )
	     {
	       cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	       cv::drawContours( drawing, contours_poly, i, color, 1, 8,std::vector<cv::Vec4i>(), 0, cv::Point() );
	     }
	  cv::imshow("xxx", drawing);
	return rects;
}


