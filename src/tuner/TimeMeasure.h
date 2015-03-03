/*
 * TimeMeasure.h
 *
 *  Created on: Mar 3, 2015
 *      Author: loganek
 */

#ifndef SRC_TIMEMEASURE_H_
#define SRC_TIMEMEASURE_H_

#include <chrono>

#define MEASURE_TIME(unit, ...)						\
  [&] {									\
    using namespace std::chrono;					\
    auto start = high_resolution_clock::now ();				\
    __VA_ARGS__;							\
    auto time = high_resolution_clock::now () - start;			\
    return duration_cast<unit> (time).count ();				\
  } ();


#endif /* SRC_TIMEMEASURE_H_ */
