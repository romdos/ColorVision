/*
 *
 *
 *
 *
 */






#include <cmath>
#include <iostream>
#include "MarkingDetector.h"





Marking::Marking(){}
Marking::~Marking(){}






MarkingDetector::MarkingDetector()
{
    strips = nullptr;
}



MarkingDetector::~MarkingDetector()
{}




/*
 *  @Description:
 *       Finds all marking lines in a picture based on found candidate bunches (bursts)
 *          and pushes them to the list <markings>.
 *  @Parameters:
 *      @In:    low_sky_boundary -- lower boundary of the sky,
 *      @Out:   markings -- list where detected markings will be stored,
 */
void MarkingDetector::find(std::vector<Marking>& markings,
                           std::uint8_t low_sky_boundary)
{
    if (low_sky_boundary < STRIPS_NUMBER)
    {
        for (size_t origin_strip = 0; origin_strip < low_sky_boundary; ++origin_strip)
        {
            for (auto& seed: strips[origin_strip].bursts)
            {
                if (seed.sectionCrossed)
                    continue;

                std::uint8_t strip_number = seed.stripNumber;
                GrayBunch next_bunch = seed;
                Marking	marking;

                bool bunch_is_found = true;
                std::uint8_t length = 0;
                while (bunch_is_found && (length < 6))
                {
                    ++strip_number;
                    if (strip_number >= low_sky_boundary)
                        break;

                    for (auto& candidate: strips[strip_number].bursts)
                    {
                        if (candidate.sectionCrossed && (candidate.length() < 2))
                            continue;

                        Segment s1(next_bunch.beg, next_bunch.end);
                        Segment s2(candidate.beg, candidate.end);

                        std::uint16_t r1, r2; // useless, only for measure_intersection
                        // todo: adjust thresholds
                        bool bunches_intersected = measure_intersection(s1, s2, &r1, &r2) <= 2;
                        bool similar = std::abs(candidate.intens - next_bunch.intens) <= 5;

                        if (bunches_intersected && similar)
                        {
                            marking.bunches.push_back(candidate);
                            candidate.sectionCrossed = true;
                            next_bunch = candidate;
                            bunch_is_found = true;
                            break;
                        }
                        bunch_is_found = false;
                    }
                }
                // Criteria
                bool long_enough = marking.length() > 4;
                //bool straight_from_left = marking.left_curvature() < 2;
                //bool straight_from_right = marking.right_curvature() < 2;

                if (long_enough)// && straight_from_left && straight_from_right)
                    markings.push_back(marking);
            }
        }
    }
}



/*
 * @Description:
 *      Sum left deviations.
 * @Return value:
 *
 */
float Marking::left_curvature()
{
    std::uint8_t length = bunches.size();

    float strip_width = IMAGE_HEIGHT / STRIPS_NUMBER;
    if (length > 2)
    {
        float error = 0.0;
        for (size_t i = 1; i < length-1; ++i)
        {
            float x1 = bunches[i-1].beg;
            float x2 = bunches[i].beg;
            float x3 = bunches[i+1].beg;

            float y1 = (i-1) * strip_width + 0.5;
            float y2 = (i) * strip_width + 0.5;
            float y3 = (i+1) * strip_width + 0.5;

            float scalar_product = (x1 - x2) * (x3 - x2) + (y3 - y2) * (y2 - y1);
            float length_1 = std::sqrt((x1 - x2)*(x1 - x2) + (y2 - y1)*(y2 - y1));
            float length_2 = std::sqrt((x3 - x2)*(x3 - x2) + (y3 - y2)*(y3 - y2));

            float cos_angle = scalar_product / (length_1 * length_2);

            error += (1 + cos_angle);
        }
        return error;
    }
    return 0;
}



/*
 * @Description:
 *      Sums 1+cos(xi), where xi -- angle between (xi-1, xi) and (xi, xi+1) vectors.
 * @Return value:
 *
 */
float Marking::right_curvature()
{
    std::uint8_t length = bunches.size();

    float strip_width = IMAGE_HEIGHT / STRIPS_NUMBER;
    if (length > 2)
    {
        float error = 0.0;
        for (size_t i = 1; i < length-1; ++i)
        {
            float x1 = bunches[i-1].end;
            float x2 = bunches[i].end;
            float x3 = bunches[i+1].end;

            float y1 = (i-1) * strip_width + 0.5;
            float y2 = (i) * strip_width + 0.5;
            float y3 = (i+1) * strip_width + 0.5;

            float scalar_product = (x1 - x2) * (x3 - x2) + (y3 - y2) * (y2 - y1);
            float length_1 = std::sqrt((x1 - x2)*(x1 - x2) + (y2 - y1)*(y2 - y1));
            float length_2 = std::sqrt((x3 - x2)*(x3 - x2) + (y3 - y2)*(y3 - y2));

            float cos_angle = scalar_product / (length_1 * length_2);

            error += (1 + cos_angle);
        }
        return error;
    }
    return 0;
}
