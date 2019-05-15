/*
 * Useful functions for intermediate results.
 *
 *
 *
 *
 */






#ifndef METRICS_H
#define METRICS_H



// for max/min
#include <algorithm>

// for abs
#include <cstdlib>

// todo: make namespace
#include "Segment.h"







std::uint8_t measure_intersection(Segment& s1,
                       Segment& s2,
					   std::uint16_t* ratio_1,
                       std::uint16_t* ratio_2);




#endif