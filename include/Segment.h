/*
 * Horizontal or vertical segment.
 *
 */

#ifndef COLORVISION2_SEGMENT_H
#define COLORVISION2_SEGMENT_H


#include <cstdint>


class Segment
{
public:
	Segment(std::int16_t begin,
            std::int16_t end);
	~Segment();

    std::int16_t beg, end;

    std::uint16_t length() {return end - beg + 1;}
};

#endif
