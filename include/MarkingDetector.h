/*
 * Finds road marking
 *
 *
 *
 */





#ifndef MARKINGDETECTOR_H
#define MARKINGDETECTOR_H




#include "BunchGray.h"



class Marking
{
public:
    Marking();
    ~Marking();

    // detected or not
    bool isValid;
    // bunches composing marking
    std::vector<GrayBunch> bunches;

    std::uint8_t length() {return bunches.size();}
    float left_curvature();
    float right_curvature();
};



class MarkingDetector
{
public:
    MarkingDetector();
    ~MarkingDetector();


    Marking markings[2U];


    CBunchGray* strips;

    uint8 find(uint8 low_sky_boundary);

};


#endif
