/*
 * Canny.h
 *
 *  Created on: Jun 18, 2015
 *      Author: mkolny
 */

#ifndef SRC_ALGORITHMS_ENDPOINTS_CANNY_H_
#define SRC_ALGORITHMS_ENDPOINTS_CANNY_H_

#include "../EndpointAlgorithm.h"

class Canny : public EndpointAlgorithm
{
protected:
	std::vector<cv::Rect> internal_process() override;

public:
	virtual ~Canny() {}

	std::string get_name() const override { return "Canny"; }
	int BS = 3;
	int MINTH=70;
	int HIGHTH=200;
	int SOB=3;
};


#endif /* SRC_ALGORITHMS_ENDPOINTS_CANNY_H_ */
