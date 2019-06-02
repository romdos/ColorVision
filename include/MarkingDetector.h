/*
 * Finds road marking
 *
 *
 *
 */





#ifndef COLORVISION_START_MARKINGDETECTOR_H
#define COLORVISION_START_MARKINGDETECTOR_H




#include "BunchGray.h"



/* Marking line */
class Marking
{
public:
    Marking();
    ~Marking();
    std::vector<GrayBunch> bunches;       // chain of bunches which is candidate for road marking
public:
    std::uint8_t length() {return bunches.size();}
    float left_curvature();
    float right_curvature();

};




class MarkingDetector
{
public:
    MarkingDetector();
    ~MarkingDetector();

    CBunchGray* strips;

public:

    void find(std::vector<Marking>& markings,
                std::uint8_t low_sky_boundary);

};


#endif //COLORVISION_START_MARKINGDETECTOR_H
