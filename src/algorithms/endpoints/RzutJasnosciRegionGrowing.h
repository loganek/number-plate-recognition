/*
 * RzutJasnosciRegionGrowing.h
 *
 *  Created on: Jun 17, 2015
 *      Author: mkolny
 */

#ifndef SRC_ALGORITHMS_ENDPOINTS_RZUTJASNOSCIREGIONGROWING_H_
#define SRC_ALGORITHMS_ENDPOINTS_RZUTJASNOSCIREGIONGROWING_H_

#include "../EndpointAlgorithm.h"

class RzutJasnosciRegionGrowing : public EndpointAlgorithm
{
protected:
	std::vector<cv::Rect> internal_process() override;

public:
	virtual ~RzutJasnosciRegionGrowing() {}

	std::string get_name() const override { return "Rzut jasnosci - Region Growing"; }

	int A = 50;
	double B = 0.1;
};



#endif /* SRC_ALGORITHMS_ENDPOINTS_RZUTJASNOSCIREGIONGROWING_H_ */
