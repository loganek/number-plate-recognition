/*
 * AlgorithmManager.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: loganek
 */

#include "AlgorithmManager.h"

std::vector<std::string> AlgorithmManager::get_algorithms() const
{
	std::vector<std::string> names;

	for (auto a : algorithms)
	{
		names.push_back(a->get_name());
	}

	return names;
}
