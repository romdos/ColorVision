/*
 *
 *
 *
 *
 */






#include "MarkingDetector.h"





Marking::Marking(){}
Marking::~Marking(){}






MarkingDetector::MarkingDetector()
{
    strips = nullptr;
}



MarkingDetector::~MarkingDetector()
{
}







/*
 *  @Description:
 *       Detects all marking lines in a picture based on found candidate bunches (bursts).
 *  @Parameters:
 *      @In:      markings -- list where detected markings will be stored,
 *                low_sky_boundary -- lower boundary of the sky
 *      @Out:
 *  @Return value:
 *      number of found road marking lines in the picture.
 */
void MarkingDetector::detect(std::vector<Marking>& markings,
                             std::uint8_t low_sky_boundary)
{
    if (low_sky_boundary < STRIPSNUMBER)
    {
        for (size_t origin_strip = 0; origin_strip < low_sky_boundary; origin_strip++)
        {
            for (auto& seed: strips[origin_strip].bursts)
            {
                if (seed.sectionCrossed)
                    continue;

                std::uint8_t strip_number = seed.stripNumber;

                GrayBunch next_bunch = seed;
                Marking	marking;

                bool bunch_is_found = true;
                while (bunch_is_found)
                {
                    ++strip_number;
                    if (strip_number == STRIPSNUMBER)
                        break;

                    for (auto& candidate: strips[strip_number].bursts)
                    {
                        if (candidate.sectionCrossed && (candidate.length() < 2))
                            continue;

                        Segment s1(next_bunch.beg, next_bunch.end);
                        Segment s2(candidate.beg, candidate.end);

                        std::uint16_t r1, r2; // useless, only for measure_intersection
                        // todo: adjust thresholds
                        bool bunches_intersected = measure_intersection(s1, s2, &r1, &r2) <= 1;
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

                if (marking.bunches.size() > 2)
                    markings.push_back(marking);
            }
        }
    }
}