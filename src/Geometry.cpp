/*
 * Useful functions for some intermediate calculations.
 */






#include "Geometry.h"



/*  
*  @Description:
*	   Analyses intersection between 2 segments: s1 and s2.
*  @Parameters:
*      intersection_ratio_1 = intersection / first_interval_length;
*      intersection_ratio_2 = intersection / second_interval_length;
*	   if intervals are not intersecting, intersection_ratio_1 == distance between intervals
*	   and intersection_ratio_2 == 0.
*  @Return value: 
*	   0 - if intersection length is more than a half of a big interval, 
*	   1 - if intersection length is more than a half of a small interval, 
*	   2 - if intersection length is less than a half of a small interval, 
*	   3 - if intervals are not intersecting.                                                       
*/
std::uint8_t measure_intersection(Segment& s1, Segment& s2,
                                  std::uint16_t* Ratio1,
                                  std::uint16_t* Ratio2)
{
	std::int16_t begIntersection = std::max(s1.beg, s2.beg);
    std::int16_t endIntersection = std::min(s1.end, s2.end);
	
	if (begIntersection > endIntersection) // not intersecting
	{
		*Ratio1 = begIntersection - endIntersection;
		*Ratio2 = 0;
		return 3;
	}
	else
	{
        std::int16_t intersection = endIntersection - begIntersection + 1;

		std::uint16_t firstLen = s1.end - s1.beg + 1;
        std::uint16_t secondLen = s2.end - s2.beg + 1;

        std::uint16_t minLen = std::min(firstLen, secondLen);
        std::uint16_t maxLen = std::max(firstLen, secondLen);
		//  16 is for zero avoiding from division 
		*Ratio1 = (16 * intersection) / firstLen;
		*Ratio2 = (16 * intersection) / secondLen;

		if ((2 * intersection) > maxLen) // is it possible?
		{
			return 0;
		}
		else
		{
			if ((2 * intersection) > minLen)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
}



/*
 * @Description:
 * 		Checks whether two segments are intersected or not.
 * @Parameters:
 *      s1 - first segment,
 *      s2 - second segment.
 * @Return value:
 *      0 -- segments s1 and s2 are not intersected,
 *      1 -- otherwise.
 * @Note:
 *      todo: make measure of intersection.
 */
bool intersected(Segment& s1, Segment& s2)
{
	bool result = (s2.beg >= s1.beg && s2.beg <= s1.end) || (s2.end >= s1.beg && s2.beg <= s1.beg);
	return result;
}