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
#include <vector>

class AlgorithmManager
{
	std::vector<std::shared_ptr<IAlgorithm>> algorithms;

public:
	virtual ~AlgorithmManager() {}
	AlgorithmManager(std::initializer_list<std::shared_ptr<IAlgorithm>> algorithms);

	void register_algorith(std::shared_ptr<IAlgorithm> algorithm);

	std::vector<std::string> get_algorithms() const;
	std::shared_ptr<IAlgorithm> get_algorithm(const std::string& name) const;
};

#endif /* SRC_ALGORITHMMANAGER_H_ */
