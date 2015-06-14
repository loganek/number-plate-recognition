/*
 * SzczytySrednia.h
 *
 *  Created on: 28 maj 2015
 *      Author: loganek
 */

#ifndef SRC_ALGORITHMS_SZCZYTY_HISTOGRAMU_SZCZYTYSREDNIA_H_
#define SRC_ALGORITHMS_SZCZYTY_HISTOGRAMU_SZCZYTYSREDNIA_H_

#include "SzczytyHistogramu.h"

class SzczytySrednia : public SzczytyHistogramu
{
	int get_threshold_value(int max_pos1, int max_pos2, const std::vector<int>& histogram) override;

public:
	SzczytySrednia(){}
	virtual ~SzczytySrednia() {}

	std::string get_name() const override { return "Szczyty Srednia"; }
};

#endif /* SRC_ALGORITHMS_SZCZYTY_HISTOGRAMU_SZCZYTYSREDNIA_H_ */
