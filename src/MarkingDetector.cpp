/*
 *
 *
 *
 *
 */






#include <cmath>

#include "MarkingDetector.h"





Marking::Marking()
{
    isValid = false;
}



Marking::~Marking(){}




MarkingDetector::MarkingDetector()
{
    strips = nullptr;
}



MarkingDetector::~MarkingDetector()
{}




/***********************************************************************************
 *  @Description:
 *       Finds two marking lines in a picture based on found candidate bunches (bursts)
 *  @Return:
 *      Number of found markings.
 **********************************************************************************/
uint8 MarkingDetector::find(uint8 low_sky_boundary)
{
    if (low_sky_boundary >= STRIPS_NUM)
    {
        return 0U;
    }

    uint8 markingNum = 0U;
    uint8 originStrip;

    for (originStrip = 0U; originStrip < low_sky_boundary; ++originStrip)
    {
        if (markingNum == 2U)
            break;
        for (int i = 0; i < strips[originStrip].bursts.size(); i++)
        {
            if (markingNum == 2U)
                break;

            GrayBunch next_bunch = strips[originStrip].bursts[i];
            if (next_bunch.sectionCrossed)
                continue;

            uint8 stripNum = next_bunch.stripNumber;

            Marking	marking;
            bool bunch_is_found = true;

            while (bunch_is_found)
            {
                if ((++stripNum) >= low_sky_boundary)
                    break;

                for (auto& candidate: strips[stripNum].bursts)
                {
                    if (candidate.sectionCrossed && (candidate.length() < 2))
                        continue;

                    Segment s1(next_bunch.beg, next_bunch.end);
                    Segment s2(candidate.beg, candidate.end);
                    uint16 r1;
                    uint16 r2;

                    bool bunchesIntersect = measure_intersection(s1, s2, &r1, &r2) <= 2;
                    bool theirIntensClose = std::abs(candidate.intens - next_bunch.intens) <= 3.0f;

                    if (bunchesIntersect && theirIntensClose)
                    {
                        bunch_is_found           = true;
                        candidate.sectionCrossed = true;

                        next_bunch = candidate;

                        marking.bunches.push_back(candidate);

                        break;
                    }
                    bunch_is_found = false;
                }
            }

            bool straightLeft = marking.left_curvature() < 16.0f;
            bool straightRight = marking.right_curvature() < 16.0f;
            if (marking.length() > 5U && straightLeft && straightRight)
            {
                marking.isValid = true;
                markings[markingNum++] = marking;
            }
        }
    }

    return markingNum;
}



/*
 * @Description:
 *      Sum left deviations.
 * @Return value:
 *
 */
float Marking::left_curvature()
{
    uint8 length = bunches.size();

    float strip_width = IMHEIGHT / STRIPS_NUM;
    if (length > 2)
    {
        float error = 0.0f;
        for (uint8 i = 1; i < length; ++i)
        {
            error += std::abs(bunches[i].beg - bunches[i-1].beg);
        }
        return error /(length-1.0f);
    }
    return 0.0f;
}



/*
 * @Description:
 *      Sums 1+cos(xi), where xi -- angle between (xi-1, xi) and (xi, xi+1) vectors.
 * @Return value:
 *
 */
float Marking::right_curvature()
{
    uint8 length = bunches.size();

    float strip_width = IMHEIGHT / STRIPS_NUM;
    if (length > 2)
    {
        float error = 0.0f;
        for (uint8 i = 1; i < length; ++i)
        {
            error += std::abs(bunches[i].end - bunches[i-1].end);
        }
        return error /(length-1.0f);
    }
    return 0.0f;
}
