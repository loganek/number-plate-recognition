/*
 * AlgorithmManager.h
 *
 *  Created on: Mar 2, 2015
 *      Author: loganek
 */

#ifndef SRC_ALGORITHMMANAGER_H_
#define SRC_ALGORITHMMANAGER_H_

#include "IAlgorithm.h"

#include <set>
#include <memory>

class AlgorithmManager
{
	std::set<std::shared_ptr<IAlgorithm>> algorithms;

public:
	virtual ~AlgorithmManager() {}

	std::vector<std::string> get_algorithms() const;
	std::shared_ptr<IAlgorithm> get_algorithm(const std::string& name) const;
};

#endif /* SRC_ALGORITHMMANAGER_H_ */
