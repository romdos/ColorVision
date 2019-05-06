/*
 *  Visualizes all elements.
 *
 *
 */




#ifndef COLORVISION_H
#define COLORVISION_H





// to visualize
#include <opencv2/highgui/highgui.hpp>



#include "ImageProcess.h"
#include "MarkingDetector.h"




#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480



struct Input_Data
{
	bool m_ImageIsLoaded;
	bool m_VideoCameraIsLoaded;
	bool m_ParamHaveBeenChanged;
	bool m_NetworkDirectX;
	bool HorizontalVertical;
	bool PermuteRatios;
	bool SkyFinding;
	bool GreenFinding;
	int VideoInputLimit;
	int NumberOfStrips;
	int CameraNumber;
};



class CColorVisionApp
{
public:
	CColorVisionApp();
	~CColorVisionApp();

	bool m_GrayScaleOpened;
	bool m_ImageSegmented;
	bool m_Video_ImageSegmented;
	bool m_ImageIsInverted;
	bool m_StripColorRepresentation;
	bool m_StripGrayRepresentation;
	bool m_ColorBunchRepresentation;
	bool m_ColorSectionsRepresentation;
	bool m_VideoCameraInput;
	bool m_VideoCameraSequenceProcessing;

	int dimX, dimY;

	int aligned_width;
	int top_bottom_origin;
	int VideoImageProcessedNumber;
	int NumberOfBunch;
	int NumberOfColorSection;
	int ImageRepresentationType;
	int VideoImageRepresentationType;

    MarkingDetector* marking_detector;
	CImageProcess* ColorImageProcess;

	std::uint8_t* pBuffer;//pointer to the generated grayscale image
    std::uint8_t* Imbuffer;//pointer to the color image

    cv::Mat* frame_ptr;

	Input_Data *InputDataCP;

public:

	void CalculateGrayscaleVideo(char* ImageData);

	void  DirectArrayDraw(int Feature);

	void CalculateBrush(int side_triangle,int side_hue,int BunAverSat,int BunAverGr,
	                    int* scoor1,int* scoor2,int* scoor3,int* nscoor1,int* nscoor2,int* nscoor3);

	void HorizontalLine(int OrPointx,int OrPointy,int EndPointx,int EndPointy,int ACoor1,int ACoor2,int ACoor3);

	void VerticalLine(int OrPointx,int OrPointy,int EndPointx,int EndPointy,int ACoor1,int ACoor2,int ACoor3);

    void OnSegmentInitialize(std::uint8_t* ImageData, long ImageSizex,long ImageSizey, Input_Data* InputData,
		            int TotalNumberOfFrames,int* ReducedBoundary);

    void OnVideoCameraprocess(cv::Mat* frame, std::vector<Marking>& markings,
            long ImageSizex,long ImageSizey,Input_Data* InputData,
		int FrameNumber,int TotalNumberOfFrames,int* BoundaryReduced);

    void draw_bunch(const GrayBunch& bunch);

    void draw_markings(std::vector<Marking>& markings);
};

#endif
