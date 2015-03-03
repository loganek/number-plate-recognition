/*
 * AlgorithmManager.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: loganek
 */

#include "AlgorithmManager.h"

AlgorithmManager::AlgorithmManager(std::initializer_list<std::shared_ptr<IAlgorithm>> algorithms)
{
	for (auto a : algorithms)
		register_algorith(a);
}

void AlgorithmManager::register_algorith(std::shared_ptr<IAlgorithm> algorithm)
{
	algorithms.push_back(algorithm);
}

std::vector<std::string> AlgorithmManager::get_algorithms() const
{
	std::vector<std::string> names;

	for (auto a : algorithms)
	{
		names.push_back(a->get_name());
	}

	return names;
}

std::shared_ptr<IAlgorithm> AlgorithmManager::get_algorithm(const std::string& name) const
{
	for (auto a : algorithms)
	{
		if (a->get_name() == name)
			return a;
	}

	return std::shared_ptr<IAlgorithm>();
}
