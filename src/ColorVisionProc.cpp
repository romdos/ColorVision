/*
 * ColorVisionProc.cpp
 *
 *  Created on: 03.10.2011
 *      Author: Konstantin Kiy
 */






#include <iostream>
#include "ColorVision.h"




#define RED_COEF    0.3  // 0.299
#define GREEN_COEF  0.59  // 0.587
#define BLUE_COEF   0.11  // 0.114




static int CoorHuPoints[16] = { 7, 23, 39, 55, 71, 87, 103, 119, 135, 151, 167,
		183, 199, 215, 231, 247 };




CColorVisionApp::CColorVisionApp()
{
    m_GrayScaleOpened = false;
    m_ImageSegmented = false;
    m_Video_ImageSegmented = false;
    m_ImageIsInverted = false;
    m_StripColorRepresentation = false;
    m_StripGrayRepresentation = false;
    m_ColorBunchRepresentation = false;
    m_ColorSectionsRepresentation = false;
    m_VideoCameraSequenceProcessing = false;

    pBuffer = NULL;

    NumberOfBunch = 0;
    VideoImageProcessedNumber = 0;
    NumberOfColorSection = -1;

    ColorImageProcess = NULL;
    marking_detector = new MarkingDetector;


    ImageRepresentationType = 0;
    VideoImageRepresentationType = 0;
}



CColorVisionApp::~CColorVisionApp()
{
    if (ColorImageProcess != NULL)
    {
        ColorImageProcess->DeleteTemporal();

        delete ColorImageProcess;
    }

    if (marking_detector != NULL)
    {
        delete marking_detector;
    }
}



void CColorVisionApp::OnVideoCameraprocess(cv::Mat* frame, std::vector<Marking>& markings,
        long ImageSizex, long ImageSizey,
			 Input_Data* InputData, int FrameNumber, int TotalNumberOfFrames, int* BoundaryReduced)
{
    frame_ptr = frame;
	 dimX = ImageSizex;
	 dimY = ImageSizey;

	 pBuffer = frame->data;
	 VideoImageProcessedNumber = FrameNumber;
	 Imbuffer =  frame->data;

	 if (VideoImageProcessedNumber == 0)
	 {
	     OnSegmentInitialize(frame->data, ImageSizex, ImageSizey, InputData,TotalNumberOfFrames,BoundaryReduced);
	 }
	 else
	 {
	    ColorImageProcess->DeleteTemporal();
	 }

	 ColorImageProcess->Im = Imbuffer;
	 ColorImageProcess->SegmentImage(marking_detector, markings, VideoImageProcessedNumber);

	 m_Video_ImageSegmented = true;
}




void CColorVisionApp::OnSegmentInitialize(std::uint8_t* ImageData, long ImageSizex,
			long ImageSizey, Input_Data* InputData,int TotalNumberOfFrames,int* ReducedBoundary)
{
    Imbuffer = ImageData;

	if (ColorImageProcess == NULL)
	{
        ColorImageProcess = new CImageProcess(ImageData, ImageSizex,ImageSizey, InputData->NumberOfStrips,TotalNumberOfFrames);
        ColorImageProcess->BitsPerPix=24;
        ColorImageProcess->HorizontalVertical = InputData->HorizontalVertical;
        ColorImageProcess->GGBorGGR = InputData->PermuteRatios;
        ColorImageProcess->SkyFinding = InputData->SkyFinding;
        ColorImageProcess->VideoCameraIsLoaded=InputData->m_VideoCameraIsLoaded;
        ColorImageProcess->Feature = top_bottom_origin;
        ColorImageProcess->SkyPixelsOfBoundaryPoints=ReducedBoundary;
        ColorImageProcess->GreenFinding = InputData->GreenFinding;
        ColorImageProcess->InitialConstructions();
		}
	else
    {
		delete ColorImageProcess;

		ColorImageProcess = NULL;

		ColorImageProcess = new CImageProcess(ImageData, ImageSizex,ImageSizey, InputData->NumberOfStrips,TotalNumberOfFrames);
		                ColorImageProcess->BitsPerPix=24;
						ColorImageProcess->HorizontalVertical = InputData->HorizontalVertical;
						ColorImageProcess->GGBorGGR = InputData->PermuteRatios;
						ColorImageProcess->Feature = top_bottom_origin;
						ColorImageProcess->SkyFinding = InputData->SkyFinding;
											ColorImageProcess->VideoCameraIsLoaded=InputData->m_VideoCameraIsLoaded;
											ColorImageProcess->Feature = top_bottom_origin;
											ColorImageProcess->SkyPixelsOfBoundaryPoints=ReducedBoundary;
											ColorImageProcess->GreenFinding = InputData->GreenFinding;
	    ColorImageProcess->InitialConstructions();
    }
}




void CColorVisionApp::CalculateGrayscaleVideo(char* ImageData)
{
		unsigned char gray_intense;
		unsigned char inten_red, inten_green, inten_blue;
		long int entry_point;
		long int word_size;
		long int Coun;
		long int coor1, coor2, coor3;
		unsigned char *m_ImageBits;
		unsigned char *Im;
		long int Image_Size;
		long int shift;
				int DimenX_bytes;//image row size in pixels
				int Res_Width;
				int Cor_Width;
				int count_horiz;
				int count_vert;

		Im = (unsigned char *) ImageData;
		//dimX = ImageSizex;
					//dimY = ImageSizey;
		if (Im != NULL)
		{
			DimenX_bytes=aligned_width;
			//Image_Size = (3 * dimX) * dimY;
			//pBuffer = new unsigned char[Image_Size];
			//word_size = (Image_Size / 3);
		}
		else
		{
			return;
		}

		m_ImageBits = pBuffer;


					entry_point=0;
					for(count_vert=0;count_vert<dimY;count_vert++)
						{//vert cycle
						shift=((long)DimenX_bytes)*((long)count_vert);
						for(count_horiz=0;count_horiz<dimX;count_horiz++)
							{//horiz cycle
							entry_point=(long)(3*count_horiz)+shift;
							coor1 = entry_point;
							inten_blue = *(Im + entry_point++);
							coor2 = entry_point;
							inten_green = *(Im + entry_point++);
							coor3 = entry_point;
							inten_red = *(Im + entry_point);
							gray_intense = (unsigned char) (BLUE_COEF * (float) inten_blue
											+ GREEN_COEF * (float) inten_green
											+ RED_COEF * (float) inten_red);
									 				*(m_ImageBits + coor1) = gray_intense;
									 				*(m_ImageBits + coor2) = gray_intense;
									 				*(m_ImageBits + coor3) = gray_intense;
									 				//coor3 = entry_point;
									 		}//horiz cycle
						 }//vert cycle
	}






/*
 * @Description:
 *      Writes to pbuffer new color pixels corresponding to found objects.
 */
void CColorVisionApp::DirectArrayDraw(int Feature)
{
    int OrPointx, OrPointy;
    int EndPointx, EndPointy;
    int OrPointRx, OrPointRy;
    int EndPointRx, EndPointRy;
    int NumStrips;
    int HorizVert;
    int StripWidth1;
    int StripWidthPrev1;
    int WMWidth;
    int SideHue;
    int NumSideHues;
    int SideTriangle;
    int Beg_bunch;
    int End_bunch;
    int Scoor1;
    int Scoor2;
    int Scoor3;
    int NScoor1;
    int NScoor2;
    int NScoor3;
    int RScoor1;
    int RScoor2;
    int RScoor3;
    int bunch_average_hue;
    int bunch_average_sat;
    int bunch_average_gray;
    int old_col_number;
    int old_col_num;
    int bunch_number1;
    int bunch_occurrence;
    int max_number_of_covering_bunches;
    int ordered_bunch_number;
    int bunch_feature;
    int left_green_boun_bunch;
    int right_green_boun_bunch;
    CStrip* CurrentStrip;

    OrPointRx = 0;
    OrPointRy = 0;;
    EndPointRx = 0;
    EndPointRy = 0;
    RScoor1 = 0;
    RScoor2 = 0;
    RScoor3 = 0;
    int left_sky_index=-1;
    int right_sky_index=-1;

    max_number_of_covering_bunches = ColorImageProcess->MaximumNumberOfCoveringElements;

    if (((m_ImageSegmented) || (m_Video_ImageSegmented)) && (pBuffer != NULL))
    {
        NumStrips = ColorImageProcess->NumStrips;
        HorizVert = ColorImageProcess->HorizontalVertical;
        CurrentStrip = ColorImageProcess->CurStrip;

        WMWidth = dimX;

        for (int coun_strip1 = 0; coun_strip1 < NumStrips; coun_strip1++) //draw bunches
        {//str_count
            old_col_number = ColorImageProcess->ColorInt[coun_strip1].NumberOfColoredIntervals;
            bunch_number1 = ColorImageProcess->ColorInt[coun_strip1].RefinedNumberOfBunches;

            StripWidth1 = CurrentStrip[coun_strip1].StripWidth;
            StripWidthPrev1 = CurrentStrip[coun_strip1].StripWidthPrev;

            left_green_boun_bunch = ColorImageProcess->LeftGreenBoundaryBunch[coun_strip1];
            right_green_boun_bunch = ColorImageProcess->RightGreenBoundaryBunch[coun_strip1];
            if(left_green_boun_bunch > 0)
            {
                left_green_boun_bunch--;
            }
            else
            {
                left_green_boun_bunch = -1;
            }
            if (right_green_boun_bunch > 0)
            {
                right_green_boun_bunch--;
            }
            else
            {
                right_green_boun_bunch = -1;
            }

            for (int coun_bunch2 = 0; coun_bunch2 < bunch_number1; coun_bunch2++) //draw color bunches
            {
                old_col_num = ColorImageProcess->ColorInt[coun_strip1].old_bunch_number[coun_bunch2];
                bunch_feature = -1;

                if (old_col_num < old_col_number)
                {
                    bunch_average_hue =
                            ColorImageProcess->ColorInt[coun_strip1].ColoredIntervalsStructure->AverageHue[old_col_num];
                    bunch_average_sat =
                            ColorImageProcess->ColorInt[coun_strip1].ColoredIntervalsStructure->AverageSat[old_col_num];
                    bunch_average_gray =
                            ColorImageProcess->ColorInt[coun_strip1].ColoredIntervalsStructure->AverageGray[old_col_num];

                    Beg_bunch =	ColorImageProcess->ColorInt[coun_strip1].ColoredIntervalsStructure->BegInterv[old_col_num];
                    End_bunch =	ColorImageProcess->ColorInt[coun_strip1].ColoredIntervalsStructure->EndInterv[old_col_num];

                    bunch_occurrence = ColorImageProcess->ColorInt[coun_strip1].visible_bunches[old_col_num];

                    NumSideHues = (NUM_HUES / 3);
                    if (bunch_average_hue >= NUM_HUES)
                    {
                        bunch_average_hue -= NUM_HUES;
                    }

                    SideTriangle = bunch_average_hue / NumSideHues;
                    SideHue = bunch_average_hue - NumSideHues * SideTriangle;

                    CalculateBrush(SideTriangle, SideHue, bunch_average_sat,
                            bunch_average_gray, &Scoor1, &Scoor2, &Scoor3,
                            &NScoor1, &NScoor2, &NScoor3);

                    if (HorizVert)
                    {//vert
                        OrPointy = Beg_bunch;
                        if (bunch_occurrence > 0)
                        {
                            ordered_bunch_number = ColorImageProcess->ColorInt[coun_strip1].ordered_bunch_number[old_col_num];

                            left_sky_index = ColorImageProcess->LeftBunchesInTheSky[ordered_bunch_number+
                                                         coun_strip1*max_number_of_covering_bunches];
                            right_sky_index = ColorImageProcess->RightBunchesInTheSky[ordered_bunch_number+
                                                                                      coun_strip1*max_number_of_covering_bunches];
                            // set color of sky bucnhes
                            if ((left_sky_index > 0) || (right_sky_index > 0))
                            {
                                bunch_feature = 1;
                                NScoor1 = 10;
                                NScoor2 = 10;
                                NScoor3 = 10;
                            }

                            OrPointx = (StripWidthPrev1 * coun_strip1 + StripWidth1 / 2) - 1;
                        }
                        else
                        {
                            OrPointx = (StripWidthPrev1 * coun_strip1 + StripWidth1 / 2) +  std::min(3,StripWidth1 / 2);
                        }

                        EndPointx = OrPointx;
                        OrPointx -= 1;
                        EndPointx -= 1;
                        EndPointy = End_bunch;
                        VerticalLine(OrPointx, OrPointy, EndPointx, EndPointy,
                                NScoor1, NScoor2, NScoor3);
                        OrPointx += 1;
                        EndPointx += 1;
                        VerticalLine(OrPointx, OrPointy, EndPointx, EndPointy,
                                NScoor1, NScoor2, NScoor3);
                        OrPointx += 1;
                        EndPointx += 1;
                        VerticalLine(OrPointx, OrPointy, EndPointx, EndPointy,
                                NScoor1, NScoor2, NScoor3);

                    }//vet
                    else
                    {//horiz
                        OrPointx = Beg_bunch;
                        if (bunch_occurrence > 0)
                        {
                            left_sky_index = ColorImageProcess->SectionTraceLeft[coun_strip1*MAX_COL_INT+old_col_num];
                            left_sky_index %= 1000;
                            left_sky_index -= 1;
                            right_sky_index = ColorImageProcess->SectionTraceRight[coun_strip1*MAX_COL_INT+old_col_num];
                            right_sky_index %= 1000;
                            right_sky_index -= 1;

                            if ((left_sky_index >= 0) || (right_sky_index >= 0))
                            {//righleft_sky
                                if (left_sky_index >= 0)
                                {//lsi
                                    if (ColorImageProcess->SkyGreenComponents[left_sky_index] == 1)
                                    {
                                        bunch_feature = 1;
                                    }
                                }//lsi
                                if (right_sky_index > 0)
                                {//rsi
                                    if(ColorImageProcess->SkyGreenComponents[right_sky_index]==1)
                                    {
                                        bunch_feature = 1;
                                    }
                                }//rsi
                                NScoor1=10;
                                NScoor2=10;
                                NScoor3=10;
                            }//leftright_sky
                            OrPointy = (StripWidthPrev1 * (NumStrips-1-coun_strip1)
                                                                    + StripWidth1 / 2) - 1;

                        }
                        else
                        {
                            OrPointy = (StripWidthPrev1 * (NumStrips-1-coun_strip1)
                                                                    + StripWidth1 / 2) + std::min(3,StripWidth1 / 2);

                        }

                        EndPointx = End_bunch;
                        EndPointy = OrPointy;
                        OrPointy -= 1;
                        EndPointy -= 1;

                        if (bunch_feature == 1)
                        {//only sky

                            HorizontalLine(OrPointx, OrPointy, EndPointx, EndPointy,
                                NScoor1, NScoor2, NScoor3);

                            OrPointy += 1;
                            EndPointy += 1;

                            HorizontalLine(OrPointx, OrPointy, EndPointx, EndPointy,
                                NScoor1, NScoor2, NScoor3);

                            OrPointy += 1;
                            EndPointy += 1;

                            HorizontalLine(OrPointx, OrPointy, EndPointx, EndPointy,
                                NScoor1, NScoor2, NScoor3);
                        }//only sky

                        if((left_green_boun_bunch>=0) || (right_green_boun_bunch>=0))
                        {//lgb||rgb
                                bunch_feature=2;
                                RScoor1=250;
                                if((left_green_boun_bunch==old_col_num)&&(left_green_boun_bunch>=0))
                                {//ldb=
                                    OrPointRx = End_bunch;
                                    OrPointRy = (StripWidthPrev1 * (NumStrips-1-coun_strip1)+ StripWidth1 / 2) - 1;

                                    EndPointRx = OrPointRx;
                                    EndPointRy = OrPointRy + std::min(5,StripWidth1/2);

                                    if((OrPointRx<WMWidth)&&(OrPointRx>0))
                                     {
                                      OrPointRx+=1;
                                      EndPointRx+=1;
        VerticalLine(OrPointRx, OrPointRy, EndPointRx, EndPointRy,RScoor1, RScoor2, RScoor3);
        OrPointRx -= 1;
        EndPointRx -= 1;
        VerticalLine(OrPointRx, OrPointRy, EndPointRx, EndPointRy,RScoor1, RScoor2, RScoor3);
        OrPointRx -= 1;
        EndPointRx -= 1;
        VerticalLine(OrPointRx, OrPointRy, EndPointRx, EndPointRy,RScoor1, RScoor2, RScoor3);
                                     }
                                }//ldb=
                                if((right_green_boun_bunch==old_col_num)&&(right_green_boun_bunch>=0))
                                                             {//rgb=
                                    OrPointRx = Beg_bunch;
                                    OrPointRy = (StripWidthPrev1 * (NumStrips-1-coun_strip1)+ StripWidth1 / 2) - 1;
                                    //OrPointRy = WMHeight - (StripWidthPrev1*coun_strip1 + StripWidth1/2)-1;
                                    EndPointRx=OrPointRx;
                                    EndPointRy = OrPointRy + std::min(5,StripWidth1/2);//!
                                        if((OrPointRx<WMWidth)&&(OrPointRx>0))
                                        {
                                            OrPointRx-=1;
                                            EndPointRx-=1;
                                        }
             VerticalLine(OrPointRx, OrPointRy, EndPointRx, EndPointRy,RScoor1, RScoor2, RScoor3);
             OrPointRx += 1;
             EndPointRx += 1;
           VerticalLine(OrPointRx, OrPointRy, EndPointRx, EndPointRy,RScoor1, RScoor2, RScoor3);
            OrPointRx += 1;
            EndPointRx += 1;
            VerticalLine(OrPointRx, OrPointRy, EndPointRx, EndPointRy,RScoor1, RScoor2, RScoor3);
                                                             }//rgb=


                             }//lgb||rgb
                    }//horiz
                }
            }
        }
    }
}




void CColorVisionApp::CalculateBrush(int side_triangle, int side_hue, int BunAverSat,
			int BunAverGr, int* scoor1, int* scoor2, int* scoor3, int* nscoor1,
			int* nscoor2, int* nscoor3)
	{
		int Coor1;
		int Coor2;
		int Scoor1;
		int Scoor2;
		int Scoor3;
		int NScoor1;
		int NScoor2;
		int NScoor3;
		int GrayLev;
		int NewGrayLev;
		int Imax;

		if (side_triangle == 0)
		{
			Coor1 = 255 - CoorHuPoints[side_hue];
			Coor2 = CoorHuPoints[side_hue];
			Scoor1 = 84 + (BunAverSat * (Coor1 - 84)) / 15;
			Scoor2 = 84 + (BunAverSat * (Coor2 - 84)) / 15;
			Scoor3 = (84 * (15 - BunAverSat)) / 15;
			Imax = std::max(Scoor1, std::max(Scoor2, Scoor3));
			Scoor1 = (Scoor1 * 255) / Imax;
			Scoor2 = (Scoor2 * 255) / Imax;
			Scoor3 = (Scoor3 * 255) / Imax;
			GrayLev = int(
					0.3 * ((float) Scoor1) + 0.59 * ((float) Scoor2)
							+ 0.11 * ((float) Scoor3));
			GrayLev = GrayLev / 4;
			if (GrayLev > BunAverGr)
			{

				NScoor1 = (Scoor1 * BunAverGr) / GrayLev;
				NScoor2 = (Scoor2 * BunAverGr) / GrayLev;
				NScoor3 = (Scoor3 * BunAverGr) / GrayLev;
				NewGrayLev = int(
						0.3 * ((float) NScoor1) + 0.59 * ((float) NScoor2)
								+ 0.11 * ((float) NScoor3));
				NewGrayLev = NewGrayLev / 4;
			}
			else
			{
				NScoor1 = Scoor1;
				NScoor2 = Scoor2;
				NScoor3 = Scoor3;
			}
		}
		if (side_triangle == 1)
		{
			Coor1 = 255 - CoorHuPoints[side_hue];
			Coor2 = CoorHuPoints[side_hue];
			Scoor1 = (84 * (15 - BunAverSat)) / 15;
			Scoor2 = 84 + (BunAverSat * (Coor1 - 84)) / 15;
			Scoor3 = 84 + (BunAverSat * (Coor2 - 84)) / 15;
			Imax = std::max(Scoor1, std::max(Scoor2, Scoor3));
			Scoor1 = (Scoor1 * 255) / Imax;
			Scoor2 = (Scoor2 * 255) / Imax;
			Scoor3 = (Scoor3 * 255) / Imax;

			GrayLev = int(
					0.3 * ((float) Scoor1) + 0.59 * ((float) Scoor2)
							+ 0.11 * ((float) Scoor3));
			GrayLev = GrayLev / 4;
			if (GrayLev > BunAverGr)
			{
				NScoor1 = (Scoor1 * BunAverGr) / GrayLev;
				NScoor2 = (Scoor2 * BunAverGr) / GrayLev;
				NScoor3 = (Scoor3 * BunAverGr) / GrayLev;
				NewGrayLev = int(
						0.3 * ((float) NScoor1) + 0.59 * ((float) NScoor2)
								+ 0.11 * ((float) NScoor3));
				NewGrayLev = NewGrayLev / 4;
			}
			else
			{
				NScoor1 = Scoor1;
				NScoor2 = Scoor2;
				NScoor3 = Scoor3;
			}

		}
		if (side_triangle == 2)
		{
			Coor1 = CoorHuPoints[side_hue];
			Coor2 = 255 - CoorHuPoints[side_hue];
			Scoor1 = 84 + (BunAverSat * (Coor1 - 84)) / 15;
			Scoor2 = (84 * (15 - BunAverSat)) / 15;
			Scoor3 = 84 + (BunAverSat * (Coor2 - 84)) / 15;
			Imax = std::max(Scoor1, std::max(Scoor2, Scoor3));
			Scoor1 = (Scoor1 * 255) / Imax;
			Scoor2 = (Scoor2 * 255) / Imax;
			Scoor3 = (Scoor3 * 255) / Imax;

			GrayLev = int(
					0.3 * ((float) Scoor1) + 0.59 * ((float) Scoor2)
							+ 0.11 * ((float) Scoor3));
			GrayLev = GrayLev / 4;
			if (GrayLev > BunAverGr)
			{
				NScoor1 = (Scoor1 * BunAverGr) / GrayLev;
				NScoor2 = (Scoor2 * BunAverGr) / GrayLev;
				NScoor3 = (Scoor3 * BunAverGr) / GrayLev;
				NewGrayLev = int(
						0.3 * ((float) NScoor1) + 0.59 * ((float) NScoor2)
								+ 0.11 * ((float) NScoor3));
				NewGrayLev = NewGrayLev / 4;
			}
			else
			{
				NScoor1 = Scoor1;
				NScoor2 = Scoor2;
				NScoor3 = Scoor3;
			}

		}
		*scoor1 = Scoor1;
		*scoor2 = Scoor2;
		*scoor3 = Scoor3;
		*nscoor1 = NScoor1;
		*nscoor2 = NScoor2;
		*nscoor3 = NScoor3;
	}



void CColorVisionApp::HorizontalLine(int OrPointx, int OrPointy,
        int EndPointx, int EndPointy, int ACoor1, int ACoor2, int ACoor3)
{
    long int coor2, coor3;

    std::uint16_t length = EndPointx - OrPointx + 1;

    int DimenX_bytes = aligned_width;

    long int Array_length = ((long)DimenX_bytes)*((long)dimY);
    long int entry_point = (DimenX_bytes) * (long) OrPointy + (long) (3 * OrPointx);

    for (int Coun = 0; Coun < length; Coun++)
    {
        if (entry_point < (Array_length-1))
        {
			*(pBuffer + entry_point++) = ACoor3;
			coor2 = entry_point++;

			if (coor2 < (Array_length-1))
			{
			    *(pBuffer + coor2) = ACoor2;
			    coor3 = entry_point++;
                if (coor3 < (Array_length-1))
                {
                    pBuffer[coor3] = ACoor1;
                }
			}
        }
    }
}



void CColorVisionApp::VerticalLine(int OrPointx, int OrPointy, int EndPointx,
			int EndPointy, int ACoor1, int ACoor2, int ACoor3)
{
		if (OrPointy<=0)
			return;
		unsigned char *m_ImageBits;
		long int entry_point;
		long int coor2, coor3;
		int loop_limit;
		int DimenX_bytes;//image row size in pixels
		long int Array_length;

		m_ImageBits = pBuffer;

		loop_limit = EndPointy - OrPointy + 1;

				 DimenX_bytes=aligned_width;
				 Array_length=((long)DimenX_bytes)*((long)dimY);
		entry_point = ((long)DimenX_bytes) * (long) OrPointy + (long) (3 * OrPointx);

		for (int Coun = 0; Coun < loop_limit; Coun++)
		{//long_cycle
			if(entry_point<(Array_length-1))
			{//first_cond
			*(m_ImageBits + entry_point) = ACoor3;
			coor2 = entry_point + 1;
			   if(coor2<(Array_length-1))
					{//second_cond
			        *(m_ImageBits + coor2) = ACoor2;

			coor3 = entry_point + 2;
			               if(coor3<(Array_length-1))
							{//third_cond
			                *(m_ImageBits + coor3) = ACoor1;
							}//third_cond
					}//second_cond
			}//first_cond
			entry_point += 3 * dimX;
		}//long_cycle

	}




void CColorVisionApp::draw_bunch(const GrayBunch& bunch)
{
    std::int16_t begin = bunch.beg;
    std::int16_t end   = bunch.end;

    float stripWidth = ColorImageProcess->StripWidth;
    float y = (STRIPSNUMBER - bunch.stripNumber-1 + 0.5) * stripWidth;

    cv::line(*frame_ptr, cv::Point(begin, y), cv::Point(end, y), cv::Scalar(250, 180, 30), 2);
}



void CColorVisionApp::draw_markings(std::vector<Marking>& markings)
{
    for (auto& marking: markings)
    {
        for (auto& bunch: marking.bunches)
        {
            draw_bunch(bunch);
        }
    }
}