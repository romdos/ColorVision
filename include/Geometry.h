/*
 * Segments intersection measure and other useful functions.
 *
 *
 *
 *
 */






#ifndef GEOMETRY_H
#define GEOMETRY_H



// Min and Max functions are contained here
#include <algorithm>

// for abs
#include <cstdlib>



/*
 * Geometrized histogram operates with segments.
 * So, it is reasonably to make corresponding class.
 *
 */
#include "Segment.h"







std::uint8_t measure_intersection(Segment& s1,
                       Segment& s2,
					   std::uint16_t* ratio_1,
                       std::uint16_t* ratio_2);




#endif