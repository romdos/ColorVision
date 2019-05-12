/*
 *
 *
 *
 *
 */





#include "ImageProcess.h"




static int hue_zones[NUM_HUES] = {1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
5, 1, 1, 1, 1, 1};
static int dev_hue_zone[6] = { 2, 2, 2, 2, 2, 2 };
static int dev_mean_sat[16] =
{1, 1, 1, 1, 1, 2, 3, 4, 4, 4, 4, 4, 3, 2, 1, 0};
static  int dev_mean_sat1[16] =
{0, 1, 1, 1, 1, 1, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4};
static int imp_dev_mean_sat[16] =
{3, 3, 3, 3, 4, 5, 6, 6, 6, 6, 6, 5, 4, 4, 4, 4};
static int imp_dev_mean_sat1[16] =
{1, 2, 2, 3, 3, 3, 3, 4, 5, 5, 5, 6, 6, 7, 6, 6};

static int gray_zones[NUM_INTEN1] = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7};



static int dev_mean[8] = {5, 8, 8, 10, 10, 10, 12, 8};
static int imp_dev_mean[8] = {14, 16, 18, 18, 16, 16, 14, 12};





CImageProcess::CImageProcess()
{
    HorizontalVertical = HORIZONTAL;

    GGBorGGR = 1;

    VideoCameraIsLoaded = 1;

    Feature = 0;

    GreenFinding = 1;

    memset(SkyPixelsOfBoundaryPoints, (int) '\0', sizeof(int) * IMAGE_WIDTH);


    CurStrip = NULL;
	GrayBunches = NULL;//08.02.17
	ColorInt = NULL;

	IntegratedColorlessBackIntervals = NULL;
	IntegratedColorBunchesCharacteristics = NULL;

	MotionAnalysis = NULL;
	ColorDescrSect = NULL;
	ColorSection = NULL;
	SectionTraceLeft=NULL;
	SectionTraceRight=NULL;
	ColorOfBasicBunches=NULL;//last_cor07.12.16
	SkyGreenComponents=NULL;
	SectionStraightSegments=NULL;
	NumberOfConnectionsLeftRight=NULL;
	SectionNeighborsLeftRight=NULL;
	SectionNeighborsLeftRightIteration=NULL;
	NumberOfLRConnections=NULL;
	RightLeftConnections=NULL;
	StraightSections=NULL;
	VerticalComponent=NULL;
	InclinedComponent=NULL;
	RoadLikeSections=NULL;//last_cor14.03.18
	InclineLocal=NULL;//last_cor14.09.17
	InclineLocalFirst=NULL;//last_cor14.09.17
	InclineLocalLast=NULL;//last_cor14.09.17
	InclineCurve=NULL;
	BestVertComponentStart=NULL;
	BestVertComponentBeg=NULL;
	BestVertComponentEnd=NULL;
	BestVertNumPoints=NULL;
	GlobalDeviation=NULL;
	SkyVisualization=NULL;
	LeftBunchesInTheSky=NULL;
	LeftSkyComp=NULL;
	RightBunchesInTheSky=NULL;
	RightSkyComp=NULL;
	LeftGreenBoundary=NULL;
	LeftGreenBoundaryBunch=NULL;
	LeftGreenBoundaryVert=NULL;
	LeftGreenBoundarySection=NULL;
	LeftAdjacentNonGreenSectionMax=NULL;
	LeftAdjacentGreenSectionMax=NULL;//last_cor09.12.16
	RightGreenBoundary=NULL;
	RightGreenBoundaryBunch=NULL;
	RightGreenBoundarySection=NULL;
	RightAdjacentNonGreenSectionMax=NULL;
	RightAdjacentGreenSectionMax=NULL;//last_cor09.12.16
	LeftRightSectionNumberOfRightGreenBoundaryPoints=NULL;
	BoundarySections=NULL;
	LeftBounSecTrace=NULL;
	LengthofMotionAnalysisInterval = 0;
	LineVertTrace=NULL;//last_cor25.10.17
	VerticalContrastCurves=NULL;
	StripSignals=NULL;
	StripNewNumber=NULL;
	StripSignalsAdditional=NULL;
    StripNewNumberAdditional=NULL;//last_cor14.08.18
    VerticalContrastCurves=NULL;//last_cor3.06.18
    VerticalContrastCurvesClosedSignals=NULL;//last_cor3.06.18

	DimX = IMAGE_WIDTH;
	DimY = IMAGE_HEIGHT;


	SkyFinding = true;
	GreenFinding = true;

	Residual1 = 0;
	Residual = 0;
	NumStrips = STRIPSNUMBER;

	left_shift1[0] = 3;
	left_shift1[1] = 3;
	left_shift1[2] = 2;
	left_shift1[3] = 3;
	left_shift1[4] = 4;
	left_shift1[5] = 5;
	left_shift1[6] = 6;
	left_shift1[7] = 7;
	left_shift1[8] = 8;
	left_shift1[9] = 9;
	left_shift1[10] = 10;
	left_shift1[11] = 11;

	left_shift2[0] = 3;
	left_shift2[1] = 3;
	left_shift2[2] = 2;
	left_shift2[3] = 0;
	left_shift2[4] = 0;
	left_shift2[5] = 0;
	left_shift2[6] = 0;
	left_shift2[7] = 0;
	left_shift2[8] = 0;
	left_shift2[9] = 0;
	left_shift2[10] = 0;
	left_shift2[11] = 0;

	right_shift1[0] = 3;
	right_shift1[1] = 2;
	right_shift1[2] = 1;
	right_shift1[3] = 1;
	right_shift1[4] = 1;
	right_shift1[5] = 1;
	right_shift1[6] = 1;
	right_shift1[7] = 1;
	right_shift1[8] = 1;
	right_shift1[9] = 1;
	right_shift1[10] = 1;
	right_shift1[11] = 1;

	right_shift2[0] = 2;
	right_shift2[1] = 2;
	right_shift2[2] = 3;
	right_shift2[3] = 4;
	right_shift2[4] = 4;
	right_shift2[5] = 5;
	right_shift2[6] = 5;
	right_shift2[7] = 6;
	right_shift2[8] = 6;
	right_shift2[9] = 7;
	right_shift2[10] = 7;
	right_shift2[11] = 7;

	for (std::uint8_t i_count = 0; i_count < 238; i_count++) {
		correct_intence[i_count] = i_count >> 2;
	}
	correct_intence[238] = 237 >> 2;
	correct_intence[239] = 238 >> 2;
	correct_intence[240] = 238 >> 2;
	correct_intence[241] = 239 >> 2;
	correct_intence[242] = 239 >> 2;
	correct_intence[243] = 240 >> 2;
	correct_intence[244] = 240 >> 2;
	correct_intence[245] = 240 >> 2;
	correct_intence[246] = 241 >> 2;
	correct_intence[247] = 241 >> 2;
	correct_intence[248] = 241 >> 2;
	correct_intence[249] = 242 >> 2;
	correct_intence[250] = 242 >> 2;
	correct_intence[251] = 242 >> 2;
	correct_intence[252] = 243 >> 2;
	correct_intence[253] = 243 >> 2;
	correct_intence[254] = 243 >> 2;
	correct_intence[255] = 243 >> 2;

	DimDifference = NUM_INTEN1 >> 1;
	opponent_color_difference = new int[DimDifference];
	invert_color_difference1 = new int[DimDifference >> 2];
	invert_color_difference2 = new int[DimDifference >> 2];

	opponent_color_difference[0] = 0;
	opponent_color_difference[1] = 1;
	opponent_color_difference[2] = 2;
	invert_color_difference1[0] = 0;
	invert_color_difference2[0] = 0;
	invert_color_difference1[1] = 1;
	invert_color_difference2[1] = 1;
	opponent_color_difference[3] = 2;
	invert_color_difference1[2] = 2;
	invert_color_difference2[2] = 3;
	opponent_color_difference[4] = 3;
	opponent_color_difference[5] = 3;
	opponent_color_difference[6] = 3;
	invert_color_difference1[3] = 4;
	invert_color_difference2[3] = 6;
	opponent_color_difference[7] = 4;
	opponent_color_difference[8] = 4;
	opponent_color_difference[9] = 4;
	opponent_color_difference[10] = 4;
	invert_color_difference1[4] = 7;
	invert_color_difference2[4] = 10;
	opponent_color_difference[11] = 5;
	opponent_color_difference[12] = 5;
	opponent_color_difference[13] = 5;
	opponent_color_difference[14] = 5;
	opponent_color_difference[15] = 5;
	opponent_color_difference[16] = 5;
	invert_color_difference1[5] = 11;
	invert_color_difference2[5] = 16;
	opponent_color_difference[17] = 6;
	opponent_color_difference[18] = 6;
	opponent_color_difference[19] = 6;
	opponent_color_difference[20] = 6;
	opponent_color_difference[21] = 6;
	opponent_color_difference[22] = 6;
	invert_color_difference1[6] = 17;
	invert_color_difference2[6] = 22;
	opponent_color_difference[23] = 7;
	opponent_color_difference[24] = 7;
	opponent_color_difference[25] = 7;
	opponent_color_difference[26] = 7;
	opponent_color_difference[27] = 7;
	opponent_color_difference[28] = 7;
	opponent_color_difference[29] = 7;
	opponent_color_difference[30] = 7;
	opponent_color_difference[31] = 7;
	invert_color_difference1[7] = 23;
	invert_color_difference2[7] = 31;

	NumLevels = NUM_INTEN;

	if (SkyFinding)
	{
	    SkyVisualization = new int[DimX];
	}
	if (GreenFinding)
	{
        LeftGreenBoundary = new int[DimX];
        LeftGreenBoundaryBunch= new int[NumStrips];
        LeftGreenBoundaryVert= new int[NumStrips];
        LeftGreenBoundarySection= new int[NumStrips];
        LeftAdjacentNonGreenSectionMax= new int[NumStrips];
        LeftAdjacentGreenSectionMax= new int[NumStrips];//last_cor09.12.16
        RightGreenBoundary= new int[DimX];
        RightGreenBoundaryBunch= new int[NumStrips];
        RightGreenBoundarySection= new int[NumStrips];
        RightAdjacentNonGreenSectionMax= new int[NumStrips];
        RightAdjacentGreenSectionMax= new int[NumStrips];//last_cor12.10.17
	}
	VertFinding=true;
	if(VertFinding)
	{
        VerticalContrastCurves=new int [768];
        VerticalContrastCurvesClosedSignals=new int [768];
        StripSignals=new int [NumStrips];
        StripNewNumber=new int [NumStrips];
        StripSignalsAdditional=new int [NumStrips];
        StripNewNumberAdditional=new int [NumStrips];
	}

	InitialConstructions();
}




CImageProcess::~CImageProcess()
{
	delete[] opponent_color_difference;
	opponent_color_difference = NULL;

	delete[] invert_color_difference1;
	invert_color_difference1 = NULL;

	delete[] invert_color_difference2;
	invert_color_difference2 = NULL;

	delete[] CurStrip;
	delete[] GrayBunches;

	CurStrip = NULL;
	GrayBunches = NULL;


	if (VideoCameraIsLoaded)
	{
		if (MotionAnalysis != NULL)
		{
			delete MotionAnalysis;
			MotionAnalysis = NULL;
			delete[] IntegratedColorBunchesCharacteristics;
			IntegratedColorBunchesCharacteristics = NULL;
		}
	}

	delete[] IntegratedColorIntervals;
	IntegratedColorIntervals = NULL;

	delete[] ColorInt;
	ColorInt = NULL;

	if (ColorDescrSect != NULL)
	{
		for (int zero_sec = 0; zero_sec < NUM_SECT1; zero_sec++)
		{
			delete[] ColorDescrSect[zero_sec].location_of_section;
		}
		delete[] ColorDescrSect;
		ColorDescrSect = NULL;
	}

	if (ColorSection != NULL)
	{
		delete[] ColorSection;
		ColorSection = NULL;
	}
	if(SectionTraceLeft != NULL)
	{
	delete[] SectionTraceLeft;
	SectionTraceLeft = NULL;
	}
	if(SectionTraceRight!= NULL)
	{
	delete[] SectionTraceRight;
	SectionTraceRight = NULL;
	}
	if(SkyGreenComponents!=NULL)
	{
	delete[] SkyGreenComponents;//last_cor26.05.15
	SkyGreenComponents=NULL;
	}
	if(SectionStraightSegments!=NULL)
	{
	delete[] SectionStraightSegments;//last_cor20.11.14
	SectionStraightSegments=NULL;
	}
	if(NumberOfConnectionsLeftRight!=NULL)
	{
	delete[] NumberOfConnectionsLeftRight;
	NumberOfConnectionsLeftRight=NULL;
	}
	    if(SectionNeighborsLeftRight!=NULL)
	   {
		delete[] SectionNeighborsLeftRight;
		SectionNeighborsLeftRight=NULL;
	   }
	    if(SectionNeighborsLeftRightIteration!=NULL)
	    	{
	    delete[] SectionNeighborsLeftRightIteration;
	    SectionNeighborsLeftRightIteration=NULL;
	    	}
	    if(NumberOfLRConnections!=NULL)
	    	{
	    	delete[] NumberOfLRConnections;
	            NumberOfLRConnections=NULL;
	    	}
	        if(RightLeftConnections!=NULL)
	    	{
	         delete[] RightLeftConnections;
	         RightLeftConnections=NULL;
	    	}
	         if(VerticalContrastCurves!=NULL)
	         	{
	             delete[] VerticalContrastCurves;
	         	VerticalContrastCurves=NULL;
	         	}
	         	if(VerticalContrastCurvesClosedSignals!=NULL)
	         	{
	             delete[] VerticalContrastCurvesClosedSignals;
	         	VerticalContrastCurvesClosedSignals=NULL;
	         	}
if(StripSignals!=NULL)
{
delete[] StripSignals;
StripSignals=NULL;
}
if(StripNewNumber!=NULL)
{
 delete[] StripNewNumber;
StripNewNumber=NULL;
}
if(StripSignalsAdditional!=NULL)
{
  delete[] StripSignalsAdditional;
  StripSignalsAdditional=NULL;
}
if(StripNewNumberAdditional!=NULL)
{
delete[] StripNewNumberAdditional;
StripNewNumberAdditional=NULL;
}
if(RoadLikeSections!=NULL)
	{
	delete [] RoadLikeSections;
	RoadLikeSections=NULL;
	}
	        if(StraightSections!=NULL)
	        	{
	        	delete[] StraightSections;
	                StraightSections=NULL;
	        	}

	        if(VerticalComponent!=NULL)
	        	{
	        	delete [] VerticalComponent;
	                VerticalComponent=NULL;
	        	}

	        	if(InclinedComponent!=NULL)
	        	{
	        	delete [] InclinedComponent;
	                 InclinedComponent=NULL;
	        	}
	        	if(InclineLocal!=NULL)//last_cor14.08.17
	        		{
	        			delete [] InclineLocal;
	        			InclineLocal=NULL;
	        		}
	        		if(InclineLocalFirst!=NULL)//last_cor14.08.17
	        		{
	        			delete [] InclineLocalFirst;
	        			InclineLocalFirst=NULL;
	        		}
	        		if(InclineLocalLast!=NULL)//last_cor14.08.17
	        		{
	        			delete [] InclineLocalLast;
	        			InclineLocalLast=NULL;
	        		}
	        	if(InclineCurve!=NULL)
	        		{
	        		delete [] InclineCurve;
	        		}
	        	    InclineCurve=NULL;

	        	if(BestVertComponentStart!=NULL)
	        	{
	        	delete [] BestVertComponentStart;
	                BestVertComponentStart=NULL;
	        	}

	        	if(BestVertComponentBeg!=NULL)
	        	{
	        	delete [] BestVertComponentBeg;
	                BestVertComponentBeg=NULL;
	        	}

	        	if(BestVertComponentEnd!=NULL)
	        	{
	        	delete [] BestVertComponentEnd;
	                BestVertComponentEnd=NULL;
	        	}

	        	if(BestVertNumPoints!=NULL)
	        	{
	        	delete [] BestVertNumPoints;
	                BestVertNumPoints=NULL;
	        	}

	        	if(GlobalDeviation!=NULL)
	        	{
	        	delete [] GlobalDeviation;
	                GlobalDeviation=NULL;
	        	}
	        	if(ColorOfBasicBunches!=NULL)
	        			{//last_cor07.12.16
	        	delete [] ColorOfBasicBunches;
	        			}
	        	ColorOfBasicBunches=NULL;
	        	if(SkyFinding)
	        		{//scf
	        		if(SkyVisualization!=NULL)
	        	        {
	        		delete [] SkyVisualization;
	        	    SkyVisualization=NULL;
	        	       }
	        		}//scf
	        	//last_cor08.02.16
if(GreenFinding)
{//grf
	        	if(LeftGreenBoundary!=NULL)
	        			{
	        		delete [] LeftGreenBoundary;
	        		 LeftGreenBoundary=NULL;
	        			}
	        	if(LeftGreenBoundaryBunch!=NULL)
	        			{
	        		delete [] LeftGreenBoundaryBunch;
	        		LeftGreenBoundaryBunch=NULL;
	        			}
	        	if(LeftGreenBoundaryVert!=NULL)
	        			{
	        		delete [] LeftGreenBoundaryVert;
	        		LeftGreenBoundaryVert=NULL;//last_cor05.11.17
	        			}
	        		if(LeftGreenBoundarySection!=NULL)
	        			{
	        		delete [] LeftGreenBoundarySection;
	        		LeftGreenBoundarySection=NULL;
	        			}


	        		if(LeftAdjacentNonGreenSectionMax!=NULL)
	        			{
	        		delete [] LeftAdjacentNonGreenSectionMax;
	        		LeftAdjacentNonGreenSectionMax=NULL;
	        			}

	        		if(LeftAdjacentGreenSectionMax!=NULL)
	        				{
	        			delete [] LeftAdjacentGreenSectionMax;
	        			LeftAdjacentGreenSectionMax=NULL;//last_cor09.12.16
	        				}

	        	if(RightGreenBoundary!=NULL)
	        			{
	        		delete [] RightGreenBoundary;
	        		RightGreenBoundary=NULL;
	        			}
	        		if(RightGreenBoundaryBunch!=NULL)
	        			{
	        		delete [] RightGreenBoundaryBunch;
	        		RightGreenBoundaryBunch=NULL;
	        			}

        if (RightGreenBoundarySection != NULL)
        {
            delete [] RightGreenBoundarySection;
            RightGreenBoundarySection = NULL;
        }

        if (RightAdjacentNonGreenSectionMax != NULL)
        {
            delete[] RightAdjacentNonGreenSectionMax;
            RightAdjacentNonGreenSectionMax = NULL;
        }

        if (RightAdjacentGreenSectionMax != NULL)//last_cor09.12.16
        {
            delete[] RightAdjacentGreenSectionMax;
            RightAdjacentGreenSectionMax = NULL;//last_cor09.12.16
        }

    }//grf
    DeleteTemporal();
}



/*
 * @Description:
 *      Deletes dynamically allocated objects in some methods.
 */
void CImageProcess::DeleteTemporal()
{
	for (int i = 0; i < NumStrips; i++)
	{
		if (ColorInt[i].NumInterestingIntensities != 0)
		{
			delete[] ColorInt[i].ColorInform;
			ColorInt[i].ColorInform = NULL;
		}

		if (ColorInt[i].NumberOfColoredIntervals > 0)
		{ //cond
			if (VideoCameraIsLoaded)
			{
				delete[] ColorInt[i].bunches_occurred;
				ColorInt[i].bunches_occurred = NULL;

				delete[] ColorInt[i].bunch_connections;
				ColorInt[i].bunch_connections = NULL;

			}
			else
            {
				delete[] ColorInt[i].bunch_blocking;
				ColorInt[i].bunch_blocking = NULL;
				delete[] ColorInt[i].left_continuation;
				ColorInt[i].left_continuation = NULL;
				delete[] ColorInt[i].right_continuation;
				ColorInt[i].right_continuation = NULL;
				delete[] ColorInt[i].left_adjacent;
				ColorInt[i].left_adjacent = NULL;
				delete[] ColorInt[i].right_adjacent;
				ColorInt[i].right_adjacent = NULL;
				delete[] ColorInt[i].new_left_continuation;
				ColorInt[i].new_left_continuation=NULL;
				delete[] ColorInt[i].new_right_continuation;
				ColorInt[i].new_right_continuation=NULL;
				delete[] ColorInt[i].new_right_continuation;
				ColorInt[i].new_right_continuation=NULL;
				delete[] ColorInt[i].new_left_adjacent;
				ColorInt[i].new_left_adjacent=NULL;
				delete[] ColorInt[i].new_right_adjacent;
				ColorInt[i].new_right_adjacent=NULL;
				delete[] ColorInt[i].old_ordered_bunch_number;
				ColorInt[i].old_ordered_bunch_number=NULL;
				delete[] ColorInt[i].ordered_bunch_number;
				ColorInt[i].ordered_bunch_number=NULL;
				delete[] ColorInt[i].visible_bunches;
				ColorInt[i].visible_bunches = NULL;
				delete[] ColorInt[i].new_bunch_number;
				ColorInt[i].new_bunch_number = NULL;
				delete[] ColorInt[i].old_bunch_number;
				ColorInt[i].old_bunch_number = NULL;
				delete[] ColorInt[i].bunches_occurred;
				ColorInt[i].bunches_occurred = NULL;
				delete[] ColorInt[i].bunch_connections;
				ColorInt[i].bunch_connections = NULL;
			}

		} //cond

	} //i loop

	if (SkyFinding)
	{
		if(LeftSkyComp != NULL)
		{
            delete [] LeftSkyComp;
            LeftSkyComp = NULL;
		}
        if (RightSkyComp != NULL)
        {
            delete [] RightSkyComp;
            RightSkyComp = NULL;
        }
        if (LeftBunchesInTheSky != NULL)
        {
            delete [] LeftBunchesInTheSky;
            LeftBunchesInTheSky = NULL;
        }
        if (RightBunchesInTheSky != NULL)
        {
            delete [] RightBunchesInTheSky;
            RightBunchesInTheSky = NULL;
        }
	}

	if (GreenFinding)
	{
        if (LeftRightSectionNumberOfRightGreenBoundaryPoints != NULL)
        {
            delete[] LeftRightSectionNumberOfRightGreenBoundaryPoints;
            LeftRightSectionNumberOfRightGreenBoundaryPoints = NULL;
        }
	}
} //proc



// todo: remove this function to constructor
void CImageProcess::InitialConstructions()
{
	int Res_Width;
	int Cor_Width;
	int StripWidth1;
	int sec;
	int count_frames;
	StripWidth1 = 0;

	if (VideoCameraIsLoaded)
	{
		MotionAnalysis = new CMotion;
		MotionAnalysis->NumberofFrames = 2;
		LengthofMotionAnalysisInterval = MotionAnalysis->NumberofFrames;
	}

	CurStrip = new CStrip[NumStrips];
	GrayBunches = new CBunchGray[NumStrips];

	if (VideoCameraIsLoaded) {
		IntegratedColorIntervals =
				new TIntColored[LengthofMotionAnalysisInterval * NumStrips];
		IntegratedColorBunchesCharacteristics =
				new TIntColoredCharacteristics[LengthofMotionAnalysisInterval
						* NumStrips];
		MotionAnalysis->SeveralIntervalsMotion = new TMotionShifts[NumStrips];
	} else {
		IntegratedColorIntervals = new TIntColored[NumStrips];
	}
	ColorInt = new CColorIntervalSelect[NumStrips];
	ColorDescrSect = new ColorSectionDescr[NUM_SECT1];
	SectionTraceLeft = new int[MAX_COL_INT * NumStrips];
	SectionTraceRight = new int[MAX_COL_INT * NumStrips];
	SkyGreenComponents=new int[NUM_SECT1];//last_cor26.05.15
	ColorSection = new CColorSection(NumStrips, ColorDescrSect, CurStrip,
			ColorInt);

	if (HorizontalVertical == 0) {
		Dimension = DimX;
		StripWidth = DimY / NumStrips;
		Residual = DimY - StripWidth * NumStrips;
		StripLength = DimX;
		if (Residual > 0) {
			StripWidth1 = DimY / (NumStrips - 1);
			Residual1 = DimY - StripWidth1 * (NumStrips - 1);
			if (Residual1 > 0) {
				StripWidth = StripWidth1;
			} else {
				StripWidth = StripWidth1 - 1;
				Residual1 = DimY - StripWidth * (NumStrips - 1);
			}
		}

	} else {
		Dimension = DimY;
		StripWidth = DimX / NumStrips;
		StripLength = DimY;
		Residual = DimX - StripWidth * NumStrips;

		if (Residual > 0) {
			StripWidth1 = DimX / (NumStrips - 1);
			Residual1 = DimX - StripWidth1 * (NumStrips - 1);
			if (Residual1 > 0) {
				StripWidth = StripWidth1;
			} else {
				StripWidth = StripWidth1 - 1;
				Residual1 = DimX - StripWidth * (NumStrips - 1);
			}
		}

	}
	PressedLength = ((Dimension + 2) >> PRESSING); //think of the second summands

	//DimDifference=NUM_INTEN1>>1;
	Res_Width = (3 * DimX) % 4;
	if (Res_Width) {
		Cor_Width = 4 - Res_Width;
	} else {
		Cor_Width = 0;
	}
	for (int i = 0; i < NumStrips; i++)

	{
		CurStrip[i].NumLevels = NumLevels;
		CurStrip[i].NumbStr = NumStrips;
		CurStrip[i].BitPerP = BitsPerPix;
		CurStrip[i].Feature=Feature;
		CurStrip[i].num_strip = i;
		if (i < NumStrips - 1) {
			CurStrip[i].StripWidth = StripWidth;
			CurStrip[i].StripWidthPrev = StripWidth;
		} else {

			if (Residual1 > 0) {
				CurStrip[i].StripWidth = Residual1;
				CurStrip[i].StripWidthPrev = StripWidth;
			} else {
				CurStrip[i].StripWidth = StripWidth;
				CurStrip[i].StripWidthPrev = StripWidth;
			}

		}
		CurStrip[i].HorizontalVertical = HorizontalVertical;
		CurStrip[i].GGBorGGR = GGBorGGR;
		CurStrip[i].DimX = DimX;
		CurStrip[i].Cor_Width = Cor_Width;
		CurStrip[i].DimY = DimY;
		CurStrip[i].Dimension = Dimension;
		CurStrip[i].DimDifference = DimDifference;
		CurStrip[i].DimDifference1 = DimDifference / 2;
		CurStrip[i].DimDifference2 = DimDifference / 4;
		CurStrip[i].correct_int = correct_intence;
		CurStrip[i].PressedLength = PressedLength;
		CurStrip[i].StripLength = StripLength;

		CurStrip[i].valuable_intensity = new std::uint8_t[PressedLength];
		CurStrip[i].valuable_interval = new int[PressedLength];
		CurStrip[i].quantity_of_intensities = new int[PressedLength];
		CurStrip[i].intensities_occurred = new int[PressedLength
				* (NUM_INTEN / 4)];
		CurStrip[i].interval_occurred =
				new int[PressedLength * (NUM_INTEN / 4)];

		CurStrip[i].bright_consistency = new int[PressedLength];

		CurStrip[i].hist_fun = new int[NUM_INTEN];
		CurStrip[i].hist_sum = new int[NUM_INTEN];
		CurStrip[i].num_of_int = new int[NUM_INTEN];
		CurStrip[i].beg_point = new int[NUM_INTEN];
		CurStrip[i].end_point = new int[NUM_INTEN];
		CurStrip[i].thick_beg = new int[NUM_INTEN];
		CurStrip[i].thick_end = new int[NUM_INTEN];
		CurStrip[i].thick_stat = new int[NUM_INTEN];
		CurStrip[i].thick_stat_input = new int[NUM_INTEN];
		CurStrip[i].thick_break_beg = new int[NUM_INTEN];
		CurStrip[i].thick_break_end = new int[NUM_INTEN];
		CurStrip[i].thick_break_stat = new int[NUM_INTEN];
		CurStrip[i].thick_prev_beg = new int[NUM_INTEN];
		CurStrip[i].thick_prev_end = new int[NUM_INTEN];
		CurStrip[i].thick_prev_stat = new int[NUM_INTEN];
		CurStrip[i].thick_first = new int[NUM_INTEN];
		CurStrip[i].thick_last = new int[NUM_INTEN];
		CurStrip[i].jump_len = new int[NUM_INTEN];
		CurStrip[i].jump_end = new int[NUM_INTEN];
		CurStrip[i].import_beg = new int[NUM_INTEN];
		CurStrip[i].import_end = new int[NUM_INTEN];
		CurStrip[i].gray_min = new int[NUM_INTEN];
		CurStrip[i].gray_max = new int[NUM_INTEN];
		CurStrip[i].gray_mean = new float[NUM_INTEN];
		CurStrip[i].opponent1_min = new int[NUM_INTEN];
		CurStrip[i].opponent1_max = new int[NUM_INTEN];
		CurStrip[i].opponent1_mean = new float[NUM_INTEN];

		CurStrip[i].valuable_intensityg = new std::uint8_t[PressedLength];
		CurStrip[i].valuable_intervalg = new int[PressedLength];
		CurStrip[i].quantity_of_intensitiesg = new int[PressedLength];
		CurStrip[i].intensities_occurredg = new int[PressedLength
				* (NUM_INTEN1 / 4)];
		CurStrip[i].interval_occurredg = new int[PressedLength
				* (NUM_INTEN1 / 4)];

		CurStrip[i].bright_consistencyg = new int[PressedLength];

		CurStrip[i].hist_fung = new int[NUM_INTEN1];
		CurStrip[i].hist_sumg = new int[NUM_INTEN1];
		CurStrip[i].num_of_intg = new int[NUM_INTEN1];
		CurStrip[i].beg_pointg = new int[NUM_INTEN1];
		CurStrip[i].end_pointg = new int[NUM_INTEN1];
		CurStrip[i].thick_begg = new int[NUM_INTEN1];
		CurStrip[i].thick_endg = new int[NUM_INTEN1];
		CurStrip[i].thick_statg = new int[NUM_INTEN1];
		CurStrip[i].thick_stat_inputg = new int[NUM_INTEN1];
		CurStrip[i].thick_break_begg = new int[NUM_INTEN1];
		CurStrip[i].thick_break_endg = new int[NUM_INTEN1];
		CurStrip[i].thick_break_statg = new int[NUM_INTEN1];
		CurStrip[i].thick_prev_begg = new int[NUM_INTEN1];
		CurStrip[i].thick_prev_endg = new int[NUM_INTEN1];
		CurStrip[i].thick_prev_statg = new int[NUM_INTEN1];
		CurStrip[i].thick_firstg = new int[NUM_INTEN1];
		CurStrip[i].thick_lastg = new int[NUM_INTEN1];
		CurStrip[i].jump_leng = new int[NUM_INTEN1];
		CurStrip[i].jump_endg = new int[NUM_INTEN1];
		CurStrip[i].import_begg = new int[NUM_INTEN1];
		CurStrip[i].import_endg = new int[NUM_INTEN1];

		CurStrip[i].IntAllInform = new TIntCharact[NUM_INTEN];
		CurStrip[i].IntAllInformGray = new TIntCharactGray[NUM_INTEN1];

		CurStrip[i].opponent_color_difference = opponent_color_difference;
		CurStrip[i].invert_color_difference1 = invert_color_difference1;
		CurStrip[i].invert_color_difference2 = invert_color_difference2;

		GrayBunches[i].StripCur = &CurStrip[i];
		GrayBunches[i].HorizontalVertical = HorizontalVertical;
		GrayBunches[i].DimX = DimX;
		GrayBunches[i].DimY = DimY;
		GrayBunches[i].StripWidth = CurStrip[i].StripWidth;
		GrayBunches[i].StripWidthPrev = CurStrip[i].StripWidthPrev;
		GrayBunches[i].PressedLength = CurStrip[i].PressedLength;
		GrayBunches[i].NumberOfBackgroundGrains = 0;

		ColorInt[i].ColoredIntervalsStructure = &IntegratedColorIntervals[i];
		ColorInt[i].VideoCameraIsLoaded = VideoCameraIsLoaded;
		ColorInt[i].StripCurrent = &CurStrip[i];
		ColorInt[i].PressedLength = PressedLength;
		ColorInt[i].StripLength = StripLength;
		ColorInt[i].painted_strip_saturation = new int[3 * PressedLength];
		ColorInt[i].painted_numbers_saturation = new int[3 * PressedLength];
		ColorInt[i].saturation_consistency = new int[3 * PressedLength];
		ColorInt[i].painted_strip_colored = new int[PressedLength];
		ColorInt[i].painted_numbers_colored = new int[PressedLength];
		ColorInt[i].consistency_colored = new int[PressedLength];
		ColorInt[i].painted_strip_colored_long = new int[StripLength];
		ColorInt[i].painted_numbers_colored_long = new int[StripLength];
		ColorInt[i].consistency_colored_long = new int[StripLength];
		ColorInt[i].intensities_with_colored_int = new int[NUM_INTEN];

		ColorInt[i].OldNumbers = new int[NUM_INTEN];
		if (VideoCameraIsLoaded)
		{
			for (count_frames = 0; count_frames < LengthofMotionAnalysisInterval; count_frames++) {
				memset(
						IntegratedColorBunchesCharacteristics[i
								+ NumStrips * count_frames].shift_of_the_position,
						(int) '\0', sizeof(int) * (MAX_COL_INT));
				memset(
						IntegratedColorBunchesCharacteristics[i
								+ NumStrips * count_frames].length_of_trajectory,
						(int) '\0', sizeof(int) * (MAX_COL_INT));
				memset(
						IntegratedColorBunchesCharacteristics[i
								+ NumStrips * count_frames].right_length_of_trajectory,
						(int) '\0', sizeof(int) * (MAX_COL_INT));
			}
		}
	}

	ColorSection->section_fibre_first = new int[NUM_SECT1];
	ColorSection->section_fibre_last = new int[NUM_SECT1];
	ColorSection->section_left_end=new int[NUM_SECT1];
	ColorSection->section_right_end=new int[NUM_SECT1];
	ColorSection->section_out_connection = new int[NUM_SECT1];
	ColorSection->root_section = new int[NUM_SECT1];
	ColorSection->section_point_cross = new int[NUM_SECT1];
	ColorSection->section_mean_hue = new int[NUM_SECT1];
	ColorSection->section_mean_saturation = new int[NUM_SECT1];
	ColorSection->section_mean_gray = new int[NUM_SECT1];
	ColorSection->section_weight=new int[NUM_SECT1];//last_cor06.05.15
	ColorSection->section_fiber_max_length=new int[NUM_SECT1];//last_cor10.10.16
	ColorSection->section_upper_mean_length=new int[NUM_SECT1];//last_cor14.11.16
	ColorSection->section_lower_mean_length=new int[NUM_SECT1];//last_cor14.11.16
	ColorSection->section_upper_hues = new int[NumStrips * NUM_SECT1];
	ColorSection->section_lower_hues = new int[NumStrips * NUM_SECT1];
	ColorSection->section_upper_saturations = new int[NumStrips * NUM_SECT1];
	ColorSection->section_lower_saturations = new int[NumStrips * NUM_SECT1];
	ColorSection->section_upper_grays = new int[NumStrips * NUM_SECT1];
	ColorSection->section_lower_grays = new int[NumStrips * NUM_SECT1];
	ColorSection->left_section_start_number=new int [NumStrips];//last_cor15.04.15
	ColorSection->right_section_start_number=new int [NumStrips];//last_cor15.04.15
	ColorSection->left_section_last_number=new int [NumStrips];//last_cor15.04.15
	ColorSection->right_section_last_number=new int [NumStrips];//last_cor15.04.15
	ColorSection->sections_under=new int [8*NUM_SECT1];//last_cor14.04.16
	NumberOfConnectionsLeftRight=new int[(NUM_SECT1/2)];
	SectionStraightSegments=new int [NumStrips*NUM_SECT1];//last_cor20.11.14
		SectionNeighborsLeftRight=new int[(NUM_SECT1/2)*(NUM_SECT1/2)];
		SectionNeighborsLeftRightIteration=new int[(NUM_SECT1/2)*(NUM_SECT1/2)];
		    NumberOfLRConnections = new int[NUM_SECT1];
			RightLeftConnections = new int[NUM_SECT1*8];
		StraightSections=new int[NUM_SECT1];
		VerticalComponent=new int[NUM_SECT1];
		InclinedComponent=new int[NUM_SECT1];
		InclineCurve=new int[NUM_SECT1];
		InclineLocalFirst=new int[NUM_SECT1];//last_cor14.09.17
			InclineLocalLast=new int[NUM_SECT1];//last_cor14.09.17
			InclineLocal=new int[NUM_SECT1];//last_cor14.09.17
		RoadLikeSections=new int[NUM_SECT1];
		BestVertComponentStart=new int[NUM_SECT1];
			BestVertComponentBeg=new int[NUM_SECT1];
			BestVertComponentEnd=new int[NUM_SECT1];
			BestVertNumPoints=new int[NUM_SECT1];
			GlobalDeviation=new int[NUM_SECT1];

	ColorSection->HorizontalVertical = HorizontalVertical;
	ColorSection->DimX = DimX;
	ColorSection->DimY = DimY;
	ColorSection->StripWidth = StripWidth;//last_cor14.11.16
	ColorSection->NumStrips=NumStrips;//last_cor14.11.16


	for (int zero_sec = 0; zero_sec < NUM_SECT1; zero_sec++) {
		ColorDescrSect[zero_sec].location_of_section = new int[NumStrips];
	}
}



/*
 * @Description:
 *      Prepares all basic constructions (histograms, bunches etc.)
 *
 * @Parameters:
 *      @In:    frame -- image to be segmented,
 *              frame_count -- its number.
 * @Notes:
 */
void CImageProcess::segment(cv::Mat& frame,
                            size_t frame_count)
{
	RedNumberOfCurrentFrame = 0;

	MassiveGray = -1;//last_cor01.06.15
	MassiveGrayLow = -1;
	MassiveGrayTop = -1;
	MassiveGrayTopSection = -1;

	if (LengthofMotionAnalysisInterval != 0)
	{
		RedNumberOfCurrentFrame = frame_count % LengthofMotionAnalysisInterval;
	}

	MaximumNumberOfCoveringElements = 0;

	for (size_t i = 0; i < NumStrips; i++)
	{
		CurStrip[i].intensi = frame.data;
        // todo: avoid memset().
		memset(CurStrip[i].valuable_intensity, (char) '\0', PressedLength);
		memset(CurStrip[i].quantity_of_intensities, (int) '\0', sizeof(int) * (PressedLength));
		memset(CurStrip[i].bright_consistency, (int) '\0', sizeof(int) * (PressedLength));
		memset(CurStrip[i].valuable_intensityg, (char) '\0', PressedLength);
		memset(CurStrip[i].quantity_of_intensitiesg, (int) '\0', sizeof(int) * (PressedLength));
		memset(CurStrip[i].bright_consistencyg, (int) '\0', sizeof(int) * (PressedLength));

		memset(CurStrip[i].hist_fun, (int) '\0', sizeof(int) * NUM_INTEN);
		memset(CurStrip[i].hist_sum, (int) '\0', sizeof(int) * NUM_INTEN);
		memset(CurStrip[i].end_point, (int) '\0', sizeof(int) * NUM_INTEN);
		memset(CurStrip[i].jump_len, (int) '\0', sizeof(int) * NUM_INTEN);
		memset(CurStrip[i].jump_end, (int) '\0', sizeof(int) * NUM_INTEN);
		memset(CurStrip[i].thick_beg, (int) '\0', sizeof(int) * NUM_INTEN);
		memset(CurStrip[i].thick_end, (int) '\0', sizeof(int) * NUM_INTEN);
		memset(CurStrip[i].thick_break_end, (int) '\0', sizeof(int) * NUM_INTEN);
		memset(CurStrip[i].thick_prev_end, (int) '\0', sizeof(int) * NUM_INTEN);
		memset(CurStrip[i].thick_last, (int) '\0', sizeof(int) * NUM_INTEN);
		memset(CurStrip[i].num_of_int, (int) '\0', sizeof(int) * NUM_INTEN);

		memset(CurStrip[i].hist_fung, (int) '\0', sizeof(int) * NUM_INTEN1);
		memset(CurStrip[i].hist_sumg, (int) '\0', sizeof(int) * NUM_INTEN1);
		memset(CurStrip[i].end_pointg, (int) '\0', sizeof(int) * NUM_INTEN1);
		memset(CurStrip[i].jump_leng, (int) '\0', sizeof(int) * NUM_INTEN1);
		memset(CurStrip[i].jump_endg, (int) '\0', sizeof(int) * NUM_INTEN1);
		memset(CurStrip[i].thick_begg, (int) '\0', sizeof(int) * NUM_INTEN1);
		memset(CurStrip[i].thick_endg, (int) '\0', sizeof(int) * NUM_INTEN1);
		memset(CurStrip[i].thick_break_endg, (int) '\0', sizeof(int) * NUM_INTEN1);
		memset(CurStrip[i].thick_prev_endg, (int) '\0', sizeof(int) * NUM_INTEN1);
		memset(CurStrip[i].thick_lastg, (int) '\0', sizeof(int) * NUM_INTEN1);
		memset(CurStrip[i].num_of_intg, (int) '\0', sizeof(int) * NUM_INTEN1);
		memset(ColorInt[i].painted_strip_saturation, (int) '\0', sizeof(int) * (3 * PressedLength));
		memset(ColorInt[i].painted_strip_colored, (int) '\0', sizeof(int) * (PressedLength));
		memset(ColorInt[i].painted_strip_colored_long, (int) '\0', sizeof(int) * (StripLength));

		CurStrip[i].Loc_stat_geom_double(GGBorGGR);

        GrayBunches[i].find_bursts(2 * StripWidth, 5);

		if (VideoCameraIsLoaded)
		{
			ColorInt[i].ColoredIntervalsStructure = &IntegratedColorIntervals[i + RedNumberOfCurrentFrame * NumStrips];
			ColorInt[i].ColorBunchesCharacteristics = &IntegratedColorBunchesCharacteristics[i + RedNumberOfCurrentFrame * NumStrips];

			if (frame_count > 1)
			{
				memset(IntegratedColorBunchesCharacteristics[i + NumStrips * RedNumberOfCurrentFrame].length_of_trajectory,
						(int) '\0', sizeof(int) * (MAX_COL_INT));
				memset(IntegratedColorBunchesCharacteristics[i + NumStrips * RedNumberOfCurrentFrame].right_length_of_trajectory,
						(int) '\0', sizeof(int) * (MAX_COL_INT));
			}
		}
		else
        {
			ColorInt[i].ColoredIntervalsStructure = &IntegratedColorIntervals[i];
		}

		ColorInt[i].StripCurrent = &CurStrip[i];
		ColorInt[i].PressedLength = PressedLength;

		ColorInt[i].FindingStructureParametrs(CurStrip[i].num_of_int,
				ColorInt[i].intensities_with_colored_int,
				ColorInt[i].OldNumbers, &ColorInt[i].NumInterestingIntensities);

		int covering_number = ColorInt[i].NumberOfIntervalsInCovering;

		if (covering_number > MaximumNumberOfCoveringElements)
        {
            MaximumNumberOfCoveringElements = covering_number;
        }

        IntegratedColorBunchesCharacteristics[i + NumStrips * RedNumberOfCurrentFrame].num_of_bunches =
                ColorInt[i].NumberOfColoredIntervals;

		IntegratedColorBunchesCharacteristics[i + NumStrips * RedNumberOfCurrentFrame].num_of_refined_bunches =
                ColorInt[i].RefinedNumberOfBunches;

		IntegratedColorBunchesCharacteristics[i + NumStrips * RedNumberOfCurrentFrame].Max_number = ColorInt[i].MaximalNumber;

		IntegratedColorBunchesCharacteristics[i + NumStrips * RedNumberOfCurrentFrame].num_of_contrast_intervals = 0;
        //these characteristics should be refined in further investigations

        if (frame_count >= 1)
        {
            RedNumberOfPreviousFrame = (frame_count - 1) % LengthofMotionAnalysisInterval;
            ContrastBunchesMotion(i, ColorInt[i].bunches_occurred);
        }
	}

    int number_ordered;
    maximum_number_of_ordered_bunches = 0;

    for (int count_bunch_ord = 0; count_bunch_ord < NumStrips; count_bunch_ord++)
	{
	    int strip_disordering = ColorInt[count_bunch_ord].Disordering;

	    if (0 == strip_disordering)
        {
            number_ordered = ColorInt[count_bunch_ord].NumberOfIntervalsInCovering;
        }

        if (number_ordered > maximum_number_of_ordered_bunches)
        {
            maximum_number_of_ordered_bunches = number_ordered;
        }
	}

	ColorSection->SectionTracking(0, SectionTraceLeft);
	ColorSection->SectionTracking(1, SectionTraceRight);

    number_of_sections = ColorSection->Number_of_sections;
    number_of_section_left = ColorSection->Number_of_sections_left;
    number_of_section_right = ColorSection->Number_of_sections_right;

    if ((number_of_section_left > 0) && (number_of_section_right > 0))
    {
        memset(NumberOfConnectionsLeftRight,(int) '\0',sizeof(int)*(NUM_SECT1/2));
        memset(SectionNeighborsLeftRight,(int) '\0',sizeof(int)*(number_of_section_left*(NUM_SECT1/2)));
        memset(NumberOfLRConnections,(int) '\0',sizeof(int)*(number_of_sections));
        memset(RightLeftConnections,(int) '\0',sizeof(int)*(NUM_SECT1*8));
        memset(SectionStraightSegments,(int) '\0',sizeof(int)*(NumStrips*(NUM_SECT1)));//last_cor20.11.14
        memset(SectionNeighborsLeftRightIteration,(int) '\0',sizeof(int)*(number_of_section_left*(NUM_SECT1/2)));
        memset(StraightSections,(int) '\0',sizeof(int)*NUM_SECT1);
        memset(VerticalComponent,(int) '\0',sizeof(int)*NUM_SECT1);
        memset(InclinedComponent,(int) '\0',sizeof(int)*NUM_SECT1);
        memset(InclineCurve,(int) '\0',sizeof(int)*NUM_SECT1);
        memset(InclineLocal,(int) '\0',sizeof(int)*NUM_SECT1);//last_cor14.09.17
        memset(InclineLocalFirst,(int) '\0',sizeof(int)*NUM_SECT1);
        memset(InclineLocalLast,(int) '\0',sizeof(int)*NUM_SECT1);
        memset(RoadLikeSections,(int) '\0',sizeof(int)*NUM_SECT1);
        memset(BestVertComponentStart,(int) '\0',sizeof(int)*NUM_SECT1);
        memset(BestVertComponentBeg,(int) '\0',sizeof(int)*NUM_SECT1);
        memset(BestVertComponentEnd,(int) '\0',sizeof(int)*NUM_SECT1);

	    int right_connected_proc = FindingRightConectedSection();
	    int line_testing = StraightLineHighObjectsTesting();
	    int connections_found = ConnectedSections(SectionNeighborsLeftRight);

	    if (MaximumNumberOfCoveringElements > 0)
		{
		    ColorOfBasicBunches= new int [MaximumNumberOfCoveringElements*NumStrips];//last_cor12.12.16
			memset(ColorOfBasicBunches,(int) '\0',sizeof(int)*(MaximumNumberOfCoveringElements*NumStrips));
		}
    }

    detect_sky();
    detect_green();

    memset(VerticalContrastCurves, (int) '\0', sizeof(int)*(768));
    memset(VerticalLinesLength, (int) '\0', sizeof(int)*(32));
    memset(VertLineFirstStrip, (int) '\0', sizeof(int)*(32));
    memset(ConnectedVertLines, (int) '\0', sizeof(int)*(32));
    memset(ConnectedVertLinesRight, (int) '\0', sizeof(int)*(32));
    memset(ConnectedLeftBounVert, (int) '\0', sizeof(int)*(32));
    memset(RightBounVert, (int) '\0', sizeof(int)*(32));
    memset(FirstConnectedVertline, (int) '\0', sizeof(int)*(32));
    memset(LastConnectedVertline, (int) '\0', sizeof(int)*(32));
    memset(LeftClosestLine, (int) '\0', sizeof(int)*(32));
    memset(UpperClosestLine, (int) '\0', sizeof(int)*(32));
    memset(LowerClosestLine, (int) '\0', sizeof(int)*(32));
    memset(LeftClosestLineIntersecting, (int) '\0', sizeof(int)*(32));//last_cor26.03.18
    memset(RightClosestLine, (int) '\0', sizeof(int)*(32));
    memset(RightClosestLineIntersecting, (int) '\0', sizeof(int)*(32));//last_cor26.03.18
    memset(StripSignals, (int) '\0', sizeof(int)*(NumStrips));//last_cor30.10.17
    memset(StripNewNumber, (int) '\0', sizeof(int)*(NumStrips));
    memset(StripSignalsAdditional, (int) '\0', sizeof(int)*(NumStrips));
    memset(StripNewNumberAdditional, (int) '\0', sizeof(int)*(NumStrips));


    NumberOfVertLines = 0;
    NumberOfVertLinesCloseToSignals = 0;
    NumberOfVertLinesCloseToSignals1 = 0;

    if (maximum_number_of_ordered_bunches > 0)
    {
        memset(SignalNumber,(int) '\0',sizeof(int)*(256));
        memset(SignalNumberAdditional,(int) '\0',sizeof(int)*(256));



        int success_find = FindSignalZones();

        LineVertTrace = new int [maximum_number_of_ordered_bunches*NumStrips];

        memset(LineVertTrace,(int) '\0',sizeof(int)*(maximum_number_of_ordered_bunches*NumStrips));

        int vertical_success = VerticalLinesConstruct();

        int vert_signal_connected = VerticalLinesSignalsConnected();

        ConnectedVerticalLines();

        int intersec_vert = IntersectingVerticalLines();

        VerticalConnectedToBoundary();

        int breaking_vert = BreakingVerticalLines();
    }
}



/*
 *
 *
 *
 */
void CImageProcess::detect_green()
{
    UpperGreenBoundaryLeft = -1;
    UpperGreenBoundaryRight = -1;

    GreenMainLeft = -1;
    GreenMainLeftLb = -1;
    GreenMainLeftRb = -1;
    GreenMainRight = -1;
    GreenMainRightLb = -1;
    GreenMainRightRb = -1;

    memset(LeftGreenBoundary,(int) '\0',sizeof(int)*DimX);
    LeftGreenBoundaryBunch= new int[NumStrips];
    memset(LeftGreenBoundaryBunch,(int) '\0',sizeof(int)*NumStrips);
    memset(LeftGreenBoundaryVert,(int) '\0',sizeof(int)*NumStrips);//last_cor05.11.17
    memset(LeftGreenBoundarySection,(int) '\0',sizeof(int)*NumStrips);
    memset(LeftAdjacentNonGreenSectionMax,(int) '\0',sizeof(int)*NumStrips);
    memset(LeftAdjacentGreenSectionMax,(int) '\0',sizeof(int)*NumStrips);
    memset(RightGreenBoundary,(int) '\0',sizeof(int)*DimX);
    if (number_of_sections > 0)
    {
        LeftRightSectionNumberOfRightGreenBoundaryPoints = new int[number_of_sections];
        memset(LeftRightSectionNumberOfRightGreenBoundaryPoints,(int) '\0',sizeof(int)*number_of_sections);
    }
    memset(RightGreenBoundaryBunch,(int) '\0',sizeof(int)*NumStrips);
    memset(RightGreenBoundarySection,(int) '\0',sizeof(int)*NumStrips);
    memset(RightAdjacentNonGreenSectionMax,(int) '\0',sizeof(int)*NumStrips);
    memset(RightAdjacentGreenSectionMax,(int) '\0',sizeof(int)*NumStrips);//last_cor09.12.16

    ColorShapeSectionClassification();

    UpperGreenBoundaryLeft = 0;
    UpperGreenBoundaryRight = 0;

    MaximumGreenComp(1);
    MaximumGreenComp(0);

    VerticalPartsofGreenBoundary();
}




/*
 * @Description:
 *
 *
 */
void CImageProcess::detect_sky()
{
    int sky_hist[64];
    int sect_weight[64];
    int sky_hist_right[64];
    int sect_weight_right[64];

    SkyLeftHueZone = -1;
    SkyRightHueZone = -1;

    LowerSkyFiber = NumStrips+1;
    UpperSkyFiber = -1;

    if (!HorizontalVertical)
    {//horvert
        memset(SkyVisualization,(int) '\0',sizeof(int)*DimX);
        memset(SkyBoundaries,(int) '\0',sizeof(int)*1920);
        memset(SkyComponents,(int) '\0',sizeof(int)*32);
        memset(SkyGreenComponents,(int) '\0',sizeof(int)*NUM_SECT1);
        memset(sky_hist,(int) '\0',sizeof(int)*64);

        SkyIntencitiesDistributionFinding(sky_hist, sect_weight, 0);

        SkyMainLeft = -1;
        SkySaturatedLeft = -1;

        NumberLeftSkyComp = 0;
        NumberRightSkyComp = 0;

        int last_intensity_value = -1;
        int sky_saturated_left = -1;

        SkyMainLeft = MaximumSky(sky_hist,sect_weight,&last_intensity_value,&sky_saturated_left);

        SkySaturatedLeft = sky_saturated_left;

        if (SkyMainLeft >= 0)
        {
            NumberLeftSkyComp++;
            SkyGreenComponents[SkyMainLeft] = 1;
            int sky_main_left_hue = ColorSection->section_mean_hue[SkyMainLeft];//last_cor15.06.15
            SkyLeftHueZone = hue_zones[sky_main_left_hue];
        }

        memset(sky_hist_right,(int) '\0',sizeof(int)*64);

        SkyIntencitiesDistributionFinding(sky_hist_right,sect_weight_right,1);

        SkyMainRight = -1;
        SkySaturatedRight =-1;

        int last_intensity_value_right = -1;
        int sky_saturated_right=-1;

        SkyMainRight = MaximumSky(sky_hist_right,sect_weight_right,&last_intensity_value_right,
                                  &sky_saturated_right);

        SkySaturatedRight = sky_saturated_right;

        if (SkyMainRight >= 0)
        {//skmr
            SkyGreenComponents[SkyMainRight] = 1;
            NumberRightSkyComp++;
            int sky_main_right_hue = ColorSection->section_mean_hue[SkyMainRight];//last_cor15.06.15
            SkyRightHueZone = hue_zones[sky_main_right_hue];
        }//skmr

        if (SkyMainLeft >= 0)
        {//skml
            if (number_of_section_left > 0)
            {
                LeftSkyComp = new int[number_of_section_left];
                BelongsSkyTo(sky_hist,0,number_of_section_left);
            }
        }//skml

        if (SkyMainRight >= 0)
        {
            if (number_of_section_right > 0)
            {
                if (!RightSkyComp)
                {
                    RightSkyComp = new int[number_of_section_right];
                }
                BelongsSkyTo(sky_hist_right,number_of_section_left,number_of_sections);
            }
        }
    }//horvert
    else
    {
        memset(sky_hist,(int) '\0',sizeof(int)*64);
    }

    NumberOfSkyBoundaryPoints = 0;

    if ((NumberLeftSkyComp > 0) || (NumberRightSkyComp > 0))
    {
        if (MaximumNumberOfCoveringElements > 0)
        {
            if (NumberLeftSkyComp > 0)
            {
                LeftBunchesInTheSky= new int [MaximumNumberOfCoveringElements*NumStrips];
                memset(LeftBunchesInTheSky,(int) '\0',sizeof(int)*(MaximumNumberOfCoveringElements*NumStrips));
            }

            if (NumberRightSkyComp > 0)
            {
                RightBunchesInTheSky= new int [MaximumNumberOfCoveringElements*NumStrips];
                memset(RightBunchesInTheSky,(int) '\0',sizeof(int)*(MaximumNumberOfCoveringElements*NumStrips));
            }

            if ((NumberLeftSkyComp)||(NumberRightSkyComp))
            {
                LeftRightSkyAddition();

                if (NumberRightSkyComp > 0)
                {
                    LabelingSkyBunches(RightBunchesInTheSky,RightSkyComp,NumberRightSkyComp);
                }

                if(NumberLeftSkyComp > 0)
                {
                    LabelingSkyBunches(LeftBunchesInTheSky,LeftSkyComp,NumberLeftSkyComp);
                }

                if ((NumberLeftSkyComp>0)||(NumberRightSkyComp))
                {///correct this place in the original text!!!!
                    FindingBoundaryChains();
                    TranslationIntoBigDimension();
                }
            }
        }
    }

}



void CImageProcess::ContrastBunchesMotion(int num_strips, int* bunches_location) {
	int cycle_bunch;
	int number_of_color_bunches_previous;
	int strip_number;
	int blocking_previous;
	//int blocking_current;
	int bunch_beg;
	int bunch_end;
	int left_contrast_previous;
	int right_contrast_previous;
	int bunch_aver_hue;
	int bunch_lower_hue;
	int bunch_upper_hue;
	int bunch_aver_gray;
	int bunch_lower_gray;
	int bunch_upper_gray;
	int bunch_aver_saturation;
	int bunch_lower_saturation;
	int bunch_upper_saturation;
	int previous_strip_number;
	int suitability;
	int suitability_right;
	TIntColored* ColoredIntervalsStructure;

	suitability = -2;
	strip_number = num_strips;
	previous_strip_number = strip_number + NumStrips * RedNumberOfPreviousFrame;
	ColoredIntervalsStructure =
			&IntegratedColorIntervals[previous_strip_number];
	number_of_color_bunches_previous =
			IntegratedColorBunchesCharacteristics[previous_strip_number].num_of_bunches;

	for (cycle_bunch = 0; cycle_bunch < number_of_color_bunches_previous;
			cycle_bunch++) {
		blocking_previous =
				IntegratedColorBunchesCharacteristics[previous_strip_number].bunch_blocking[cycle_bunch];
		if (!blocking_previous) { //bp
			bunch_beg = ColoredIntervalsStructure->BegInterv[cycle_bunch];
			bunch_end = ColoredIntervalsStructure->EndInterv[cycle_bunch];
			bunch_aver_hue = -1;
			left_contrast_previous =
					IntegratedColorBunchesCharacteristics[previous_strip_number].left_continuation[cycle_bunch];
			right_contrast_previous =
					IntegratedColorBunchesCharacteristics[previous_strip_number].right_continuation[cycle_bunch];

			if (left_contrast_previous < 0) { //lc
				bunch_aver_hue =
						ColoredIntervalsStructure->AverageHue[cycle_bunch];
				bunch_lower_hue =
						ColoredIntervalsStructure->LowerHueValue[cycle_bunch];
				bunch_upper_hue =
						ColoredIntervalsStructure->UpperHueValue[cycle_bunch];
				bunch_aver_gray =
						ColoredIntervalsStructure->AverageGray[cycle_bunch];
				bunch_lower_gray =
						ColoredIntervalsStructure->LowerGrayscale[cycle_bunch];
				bunch_upper_gray =
						ColoredIntervalsStructure->UpperGrayscale[cycle_bunch];
				bunch_aver_saturation =
						ColoredIntervalsStructure->AverageSat[cycle_bunch];
				bunch_lower_saturation =
						ColoredIntervalsStructure->LowerSatValue[cycle_bunch];
				bunch_upper_saturation =
						ColoredIntervalsStructure->UpperSatValue[cycle_bunch];
				if (bunch_aver_saturation >= 2) {
					suitability = ExtensionOfLeftContrast(strip_number,
							cycle_bunch, bunch_beg, bunch_end, bunch_aver_hue,
							bunch_lower_hue, bunch_upper_hue, bunch_aver_gray,
							bunch_lower_gray, bunch_upper_gray,
							bunch_aver_saturation, bunch_lower_saturation,
							bunch_upper_saturation);
				}
			} //lc
			if (right_contrast_previous < 0) { //rc
				bunch_aver_hue =
						ColoredIntervalsStructure->AverageHue[cycle_bunch];
				bunch_lower_hue =
						ColoredIntervalsStructure->LowerHueValue[cycle_bunch];
				bunch_upper_hue =
						ColoredIntervalsStructure->UpperHueValue[cycle_bunch];
				bunch_aver_gray =
						ColoredIntervalsStructure->AverageGray[cycle_bunch];
				bunch_lower_gray =
						ColoredIntervalsStructure->LowerGrayscale[cycle_bunch];
				bunch_upper_gray =
						ColoredIntervalsStructure->UpperGrayscale[cycle_bunch];
				bunch_aver_saturation =
						ColoredIntervalsStructure->AverageSat[cycle_bunch];
				bunch_lower_saturation =
						ColoredIntervalsStructure->LowerSatValue[cycle_bunch];
				bunch_upper_saturation =
						ColoredIntervalsStructure->UpperSatValue[cycle_bunch];
				if (bunch_aver_saturation >= 2) {
					suitability_right = ExtensionOfRightContrast(strip_number,
							cycle_bunch, bunch_beg, bunch_end, bunch_aver_hue,
							bunch_lower_hue, bunch_upper_hue, bunch_aver_gray,
							bunch_lower_gray, bunch_upper_gray,
							bunch_aver_saturation, bunch_lower_saturation,
							bunch_upper_saturation);
				}
			} //rc
		} //bp
	}

}




int CImageProcess::ExtensionOfLeftContrast(int number_of_strip, int bunch_number,
		int bunch_beg, int bunch_end, int bunch_hue, int bunch_lower_hue,
		int bunch_upper_hue, int bunch_gray, int bunch_lower_gray,
		int bunch_upper_gray, int bunch_saturation, int bunch_lower_saturation,
		int bunch_upper_saturation) {
	int strip_number;
	int number_of_color_bunches_current;
	int paint_start;
	int paint_start_new;
	int paint_finish;
	int paint_finish_new;
	int cycle_limit;
	int paint_coun;
	int paint_step;
	int intens_cou;
	int point_location;
	int quant;
	int bunch_new;
	int bunch_left_adjacent;
	int bunch_new_left_adjacent;
	int bunch_left_adjacent_beg, bunch_left_adjacent_end;
	int bunch_new_left_adjacent_beg, bunch_new_left_adjacent_end;
	int bunch_new_beg, bunch_new_end;
	int blocking;
	int bunch_left_contrast;
	int bunch_right_contrast;
	int Intersection;
	int Intersection_adjacent;
	int indic_length;
	int ratio_length;
	int ratio_length1;
	int indic_length_adjacent;
	int ratio_length_adjacent;
	int ratio_length1_adjacent;
	int test_junction;
	int test_junction_adjacent;
	int previous_strip_number;
	int prior;
	int shift;
	int abs_shift;
	int previous_trajectory_length;
	int previous_total_shift;
	int current_strip_number;
	int paint_shift_plus;
	int paint_shift_minus;
	int parity;
	int* bunch_blocking;
	int* quantity;
	int* bunches_list;
	int* left_contrast;
	int* left_adjacent;
	int* right_contrast;
	TIntColored* ColoredIntervalsStructure1;
	TIntColored* ColoredIntervalsStructure;

	Intersection = -1;
	indic_length = -1;
	ratio_length = -1;
	ratio_length1 = -1;
	Intersection_adjacent = -1;
	indic_length_adjacent = -1;
	ratio_length_adjacent = -1;
	ratio_length1_adjacent = -1;
	prior = -1;
	strip_number = number_of_strip;
	previous_strip_number = strip_number + NumStrips * RedNumberOfPreviousFrame;
	current_strip_number = strip_number + NumStrips * RedNumberOfCurrentFrame;
	ColoredIntervalsStructure1 =
			ColorInt[strip_number].ColoredIntervalsStructure;
	ColoredIntervalsStructure =
			&IntegratedColorIntervals[previous_strip_number];

	number_of_color_bunches_current =
			ColorInt[strip_number].NumberOfColoredIntervals;
	quantity = ColorInt[strip_number].painted_numbers_colored;
	bunches_list = ColorInt[strip_number].bunches_occurred;
	bunch_blocking = ColorInt[strip_number].bunch_blocking;
	left_contrast = ColorInt[strip_number].left_continuation;
	left_adjacent = ColorInt[strip_number].left_adjacent;
	right_contrast = ColorInt[strip_number].right_continuation;
	bunch_left_adjacent =
			IntegratedColorBunchesCharacteristics[previous_strip_number].left_adjacent[bunch_number];
	bunch_left_adjacent = -bunch_left_adjacent;
	bunch_left_adjacent -= 1;
	bunch_left_adjacent_beg =
			ColoredIntervalsStructure->BegInterv[bunch_left_adjacent];
	bunch_left_adjacent_end =
			ColoredIntervalsStructure->EndInterv[bunch_left_adjacent];
	paint_start = bunch_beg / 4;
	paint_start_new = std::max(paint_start - 10, 0);

	paint_finish = bunch_end / 4;
	paint_finish_new = std::min(paint_finish, paint_start + 10);

	cycle_limit = (paint_finish_new - paint_start_new) / 2;
	paint_shift_plus = 2;
	paint_shift_minus = 0;
	for (paint_coun = 0; paint_coun <= cycle_limit; paint_coun++) {
		parity = paint_coun % 2;
		if (!(parity)) {
			if (paint_shift_minus >= 0) {
				paint_step = paint_start - paint_shift_minus;
			}
		} else {
			if (paint_shift_plus >= 0) {
				paint_step = paint_start + paint_shift_plus;
			}
		}
		if (((!parity) && (paint_shift_minus >= 0))
				|| ((parity) && (paint_shift_plus >= 0))) { //condition
			quant = *(quantity + paint_step);
			for (intens_cou = 0; intens_cou < quant; intens_cou++) { //cycle2
				point_location = intens_cou * PressedLength + paint_step;
				bunch_new = *(bunches_list + point_location);
				//bunch_indicator=*(bunch_investigated+bunch_new);
				blocking = *(bunch_blocking + bunch_new);
				if (!blocking) { //bl
					bunch_left_contrast = *(left_contrast + bunch_new);
					bunch_right_contrast = *(right_contrast + bunch_new);
					if (bunch_left_contrast < 0) { //lc
						bunch_new_left_adjacent =
								ColorInt[strip_number].left_adjacent[bunch_new];
						bunch_new_left_adjacent = -bunch_new_left_adjacent;
						bunch_new_left_adjacent -= 1;
						bunch_new_beg =
								ColoredIntervalsStructure1->BegInterv[bunch_new];
						bunch_new_end =
								ColoredIntervalsStructure1->EndInterv[bunch_new];
						bunch_new_left_adjacent_beg =
								ColoredIntervalsStructure1->BegInterv[bunch_new_left_adjacent];
						bunch_new_left_adjacent_end =
								ColoredIntervalsStructure1->EndInterv[bunch_new_left_adjacent];
						Intersection =
								ColorInt[strip_number].Intersection_measure_ratios(
										bunch_beg, bunch_end, bunch_new_beg,
										bunch_new_end, &indic_length,
										&ratio_length, &ratio_length1);
						Intersection_adjacent =
								ColorInt[strip_number].Intersection_measure_ratios(
										bunch_left_adjacent_beg,
										bunch_left_adjacent_end,
										bunch_new_left_adjacent_beg,
										bunch_new_left_adjacent_end,
										&indic_length_adjacent,
										&ratio_length_adjacent,
										&ratio_length1_adjacent);
						shift = bunch_new_beg - bunch_beg;
						abs_shift = abs(shift);
						test_junction = Test_Junction_Different_Strips(
								bunch_new, bunch_number, Intersection,
								indic_length, ratio_length, ratio_length1,
								strip_number, previous_strip_number);
						if ((test_junction > 0) && (test_junction <= 3)) {
							test_junction_adjacent =
									Test_Junction_Different_Strips(
											bunch_new_left_adjacent,
											bunch_left_adjacent,
											Intersection_adjacent,
											indic_length_adjacent,
											ratio_length_adjacent,
											ratio_length1_adjacent,
											strip_number,
											previous_strip_number);
							if (test_junction == 1) {
								if ((ratio_length >= 13)
										&& (ratio_length1 >= 13)) {
									if (bunch_right_contrast < 0) {
										prior = 0;
									} else {
										prior = 1;
									}
								} else { //erl11rl114
									if ((ratio_length >= 12)
											|| (ratio_length1 >= 12)) {
										if (bunch_right_contrast < 0) {
											prior = 2;
										} else {
											prior = 3;
										}
									} else { //e1010
										if ((ratio_length >= 10)
												|| (ratio_length1 >= 10)) { //||
											if (bunch_right_contrast < 0) {
												prior = 4;
											} else {
												prior = 5;
											}
										} //||
									} //e1010
								} //erl11rl114
							} else { //etj1
								if (test_junction == 2) { //tj2
									if (abs_shift <= 5) {
										if ((ratio_length >= 14)
												&& (ratio_length1 >= 14)) {
											if (bunch_right_contrast < 0) {
												prior = 4;
											} else {
												prior = 5;
											}
										} else { //e1414
											if ((ratio_length >= 10)
													&& (ratio_length1 >= 10)) { //rl10rl110
												if (bunch_right_contrast < 0) {
													prior = 5;
												} else {
													prior = 6;
												}
											} //rl10rl110
											else { //erl10rl110
												if ((ratio_length >= 14)
														|| (ratio_length1 >= 14)) {
													if (bunch_right_contrast
															< 0) {
														prior = 7;
													} else {
														prior = 8;
													}
												}
											} //erl10rl110
										} //e1414
									}
								} //tj2
							} //etj1
							if ((prior >= 0) && (prior <= 8)) {

								previous_trajectory_length =
										IntegratedColorBunchesCharacteristics[previous_strip_number].length_of_trajectory[bunch_number];
								if (previous_trajectory_length > 0) {
									previous_total_shift =
											IntegratedColorBunchesCharacteristics[previous_strip_number].total_shift[bunch_number];
								} else {
									previous_total_shift = 0;
								}
								IntegratedColorBunchesCharacteristics[current_strip_number].shift_of_the_position[bunch_new] =
										shift;
								IntegratedColorBunchesCharacteristics[current_strip_number].length_of_trajectory[bunch_new] =
										previous_trajectory_length + 1;
								IntegratedColorBunchesCharacteristics[current_strip_number].total_shift[bunch_new] =
										previous_total_shift + shift;
								IntegratedColorBunchesCharacteristics[current_strip_number].previous_bunch_number[bunch_new] =
										bunch_number;
								goto K;
							}
						}

					} //lc

					//bunch_new_occurrence=*(bunch_new_visibility+bunch_new);
				} //bl
				if (!(parity)) {
					if (paint_step - paint_shift_minus - 2 >= paint_start_new) {
						paint_shift_minus = paint_shift_minus + 2;
					} else {
						paint_shift_minus = -1;
					}
				} else {
					if (paint_step + paint_shift_plus + 2 <= paint_finish_new) {
						paint_shift_plus = paint_shift_plus + 2;
					} else {
						paint_shift_plus = -1;
					}
				}
			} //cycle2
		} //condition
	} //cycle1
	K: return (prior);
}
//=====================================================
int

CImageProcess::ExtensionOfRightContrast(int number_of_strip, int bunch_number,
		int bunch_beg, int bunch_end, int bunch_hue, int bunch_lower_hue,
		int bunch_upper_hue, int bunch_gray, int bunch_lower_gray,
		int bunch_upper_gray, int bunch_saturation, int bunch_lower_saturation,
		int bunch_upper_saturation) {
	int strip_number;
	int number_of_color_bunches_current;
	int paint_start;
	int paint_start_new;
	int paint_finish;
	int paint_finish_new;
	int cycle_limit;
	int paint_coun;
	int paint_step;
	int intens_cou;
	int point_location;
	int quant;
	int bunch_new;
	int bunch_right_adjacent;
	int bunch_new_right_adjacent;
	int bunch_right_adjacent_beg, bunch_right_adjacent_end;
	int bunch_new_right_adjacent_beg, bunch_new_right_adjacent_end;
	int bunch_new_beg, bunch_new_end;
	int blocking;
	int bunch_left_contrast;
	int bunch_right_contrast;
	int Intersection;
	int Intersection_adjacent;
	int indic_length;
	int ratio_length;
	int ratio_length1;
	int indic_length_adjacent;
	int ratio_length_adjacent;
	int ratio_length1_adjacent;
	int test_junction;
	int test_junction_adjacent;
	int previous_strip_number;
	int prior;
	int shift;
	int abs_shift;
	int previous_trajectory_length;
	int previous_total_shift;
	int current_strip_number;
	int paint_shift_plus;
	int paint_shift_minus;
	int parity;
	int* bunch_blocking;
	int* quantity;
	int* bunches_list;
	int* left_contrast;
	int* right_adjacent;
	int* right_contrast;
	TIntColored* ColoredIntervalsStructure1;
	TIntColored* ColoredIntervalsStructure;

	Intersection = -1;
	indic_length = -1;
	ratio_length = -1;
	ratio_length1 = -1;
	Intersection_adjacent = -1;
	indic_length_adjacent = -1;
	ratio_length_adjacent = -1;
	ratio_length1_adjacent = -1;
	prior = -1;
	strip_number = number_of_strip;
	previous_strip_number = strip_number + NumStrips * RedNumberOfPreviousFrame;
	current_strip_number = strip_number + NumStrips * RedNumberOfCurrentFrame;
	ColoredIntervalsStructure1 =
			ColorInt[strip_number].ColoredIntervalsStructure;
	ColoredIntervalsStructure =
			&IntegratedColorIntervals[previous_strip_number];

	number_of_color_bunches_current =
			ColorInt[strip_number].NumberOfColoredIntervals;
	quantity = ColorInt[strip_number].painted_numbers_colored;
	bunches_list = ColorInt[strip_number].bunches_occurred;
	bunch_blocking = ColorInt[strip_number].bunch_blocking;
	left_contrast = ColorInt[strip_number].left_continuation;
	right_adjacent = ColorInt[strip_number].right_adjacent;
	right_contrast = ColorInt[strip_number].right_continuation;
	bunch_right_adjacent =
			IntegratedColorBunchesCharacteristics[previous_strip_number].right_adjacent[bunch_number];
	bunch_right_adjacent = -bunch_right_adjacent;
	bunch_right_adjacent -= 1;
	bunch_right_adjacent_beg =
			ColoredIntervalsStructure->BegInterv[bunch_right_adjacent];
	bunch_right_adjacent_end =
			ColoredIntervalsStructure->EndInterv[bunch_right_adjacent];
	paint_start = bunch_beg / 4;
	paint_finish = bunch_end / 4;
	paint_start_new = std::max(paint_finish - 10, paint_start);
	paint_finish_new = std::min(paint_finish + 10, PressedLength / 2 - 1);

	cycle_limit = (paint_finish_new - paint_start_new) / 2;
	paint_shift_plus = 2;
	paint_shift_minus = 0;
	for (paint_coun = 0; paint_coun <= cycle_limit; paint_coun++) {
		parity = paint_coun % 2;
		if (!(parity)) {
			if (paint_shift_minus >= 0) {
				paint_step = paint_finish - paint_shift_minus;
			}
		} else {
			if (paint_shift_plus >= 0) {
				paint_step = paint_finish + paint_shift_plus;
			}
		}
		if (((!parity) && (paint_shift_minus >= 0))
				|| ((parity) && (paint_shift_plus >= 0))) { //condition
			quant = *(quantity + paint_step);
			for (intens_cou = 0; intens_cou < quant; intens_cou++) { //cycle2
				point_location = intens_cou * PressedLength + paint_step;
				bunch_new = *(bunches_list + point_location);
				//bunch_indicator=*(bunch_investigated+bunch_new);
				blocking = *(bunch_blocking + bunch_new);
				if (!blocking) { //bl
					bunch_right_contrast = *(right_contrast + bunch_new);
					bunch_left_contrast = *(left_contrast + bunch_new);
					if (bunch_right_contrast < 0) { //lc
						bunch_new_right_adjacent =
								ColorInt[strip_number].right_adjacent[bunch_new];
						bunch_new_right_adjacent = -bunch_new_right_adjacent;
						bunch_new_right_adjacent -= 1;
						bunch_new_beg =
								ColoredIntervalsStructure1->BegInterv[bunch_new];
						bunch_new_end =
								ColoredIntervalsStructure1->EndInterv[bunch_new];
						bunch_new_right_adjacent_beg =
								ColoredIntervalsStructure1->BegInterv[bunch_new_right_adjacent];
						bunch_new_right_adjacent_end =
								ColoredIntervalsStructure1->EndInterv[bunch_new_right_adjacent];
						Intersection =
								ColorInt[strip_number].Intersection_measure_ratios(
										bunch_beg, bunch_end, bunch_new_beg,
										bunch_new_end, &indic_length,
										&ratio_length, &ratio_length1);
						Intersection_adjacent =
								ColorInt[strip_number].Intersection_measure_ratios(
										bunch_right_adjacent_beg,
										bunch_right_adjacent_end,
										bunch_new_right_adjacent_beg,
										bunch_new_right_adjacent_end,
										&indic_length_adjacent,
										&ratio_length_adjacent,
										&ratio_length1_adjacent);
						shift = bunch_new_end - bunch_end;
						abs_shift = abs(shift);
						test_junction = Test_Junction_Different_Strips(
								bunch_new, bunch_number, Intersection,
								indic_length, ratio_length, ratio_length1,
								strip_number, previous_strip_number);
						if ((test_junction > 0) && (test_junction <= 3)) {
							test_junction_adjacent =
									Test_Junction_Different_Strips(
											bunch_new_right_adjacent,
											bunch_right_adjacent,
											Intersection_adjacent,
											indic_length_adjacent,
											ratio_length_adjacent,
											ratio_length1_adjacent,
											strip_number,
											previous_strip_number);
							if (test_junction == 1) {
								if ((ratio_length >= 13)
										&& (ratio_length1 >= 13)) {
									if (bunch_left_contrast < 0) {
										prior = 0;
									} else {
										prior = 1;
									}
								} else { //erl11rl114
									if ((ratio_length >= 12)
											|| (ratio_length1 >= 12)) {
										if (bunch_left_contrast < 0) {
											prior = 2;
										} else {
											prior = 3;
										}
									} else { //e1010
										if ((ratio_length >= 10)
												|| (ratio_length1 >= 10)) { //||
											if (bunch_right_contrast < 0) {
												prior = 4;
											} else {
												prior = 5;
											}
										} //||
									} //e1010
								} //erl11rl114
							} else { //etj1
								if (test_junction == 2) { //tj2
									if (abs_shift <= 5) {
										if ((ratio_length >= 14)
												&& (ratio_length1 >= 14)) {
											if (bunch_left_contrast < 0) {
												prior = 4;
											} else {
												prior = 5;
											}
										} else { //e1414
											if ((ratio_length >= 10)
													&& (ratio_length1 >= 10)) { //rl10rl110
												if (bunch_left_contrast < 0) {
													prior = 5;
												} else {
													prior = 6;
												}
											} //rl10rl110
											else { //erl10rl110
												if ((ratio_length >= 14)
														|| (ratio_length1 >= 14)) {
													if (bunch_left_contrast
															< 0) {
														prior = 7;
													} else {
														prior = 8;
													}
												}
											} //erl10rl110
										} //e1414
									}
								} //tj2
							} //etj1
							if ((prior >= 0) && (prior <= 8)) {

								previous_trajectory_length =
										IntegratedColorBunchesCharacteristics[previous_strip_number].right_length_of_trajectory[bunch_number];
								if (previous_trajectory_length > 0) {
									previous_total_shift =
											IntegratedColorBunchesCharacteristics[previous_strip_number].right_total_shift[bunch_number];
								} else {
									previous_total_shift = 0;
								}
								IntegratedColorBunchesCharacteristics[current_strip_number].right_shift_of_the_position[bunch_new] =
										shift;
//ColorInt[strip_number].ColorBunchesCharacteristics->shift_of_the_position[bunch_new]=shift;
								IntegratedColorBunchesCharacteristics[current_strip_number].right_length_of_trajectory[bunch_new] =
										previous_trajectory_length + 1;
//ColorInt[strip_number].ColorBunchesCharacteristics->length_of_trajectory[bunch_new]=previous_trajectory_length++;
								IntegratedColorBunchesCharacteristics[current_strip_number].right_total_shift[bunch_new] =
										previous_total_shift + shift;
								IntegratedColorBunchesCharacteristics[current_strip_number].right_previous_bunch_number[bunch_new] =
										bunch_number;

//ColorInt[strip_number].ColorBunchesCharacteristics->total_shift[bunch_new]=previous_total_shift+shift;
								goto K;
							}
						}

					} //lc

					//bunch_new_occurrence=*(bunch_new_visibility+bunch_new);
				} //bl
				if (!(parity)) {
					if (paint_step - paint_shift_minus - 2 >= paint_start_new) {
						paint_shift_minus = paint_shift_minus + 2;
					} else {
						paint_shift_minus = -1;
					}
				} else {
					if (paint_step + paint_shift_plus + 2 <= paint_finish_new) {
						paint_shift_plus = paint_shift_plus + 2;
					} else {
						paint_shift_plus = -1;
					}
				}
			} //cycle2
		} //condition
	} //cycle1
	K: return (prior);
}
//=====================================================
int
CImageProcess::Test_Junction_Different_Strips(int next_bunch,int previous_bunch,int Intersection_Measure,
int indic_length,int ratio_length,int ratio_length1,int strip_number,int previous_strip_number)
{
int bunch_mean_hue;
int corrected_bunch_mean_hue;
int neighbor_bunch_mean_hue;
int corrected_neighbor_bunch_mean_hue;
int neighbor_bunch_minimal_hue;
int neighbor_bunch_maximal_hue;
int bunch_mean_saturation;
int bunch_maximal_hue;
int bunch_minimal_hue;
int corrected_bunch_minimal_hue;
int corrected_bunch_maximal_hue;
int neighbor_bunch_mean_saturation;
int corrected_neighbor_bunch_minimal_hue;
int corrected_neighbor_bunch_maximal_hue;
int bunch_mean_gray;
int min_hue_zone;//last_cor17.04.15
int max_hue_zone;
int neighbor_bunch_mean_gray;
int prior;
int hue_difference;
int saturation_difference;
int gray_difference;
int beg_int;
int end_int;
int int_length;
int neighbor_beg_int;
int neighbor_end_int;
int neighbor_length;
int length_ratio;
int node_vicinity;
int node_vicinity_neighbor;
int bunch_occurrence;
int bunch_significance;
int neighbor_significance;
int Signif_ratio;
int bunch_density;
int neighbor_density;
int Density_ratio;
int possible_gray_variation;
int possible_sat_variation;
int possible_sat_variation1;
int impossible_sat_variation;
int impossible_sat_variation1;
int saturation_distance;
int saturation_distance1;
int max_satur_differ;
int gray_ntersection;
int indic_length_gray;
int ratio_length_gray;
int ratio_length1_gray;
int hue_bound;
int zone_hue;
int zone_hue_bunch;
int difference_hue_zone;
int zone_gray;
int zone_gray_bunch;
int difference_gray_zone;
int bunch_minimal_gray;
int bunch_maximal_gray;
int neighbor_minimum_gray;
int neighbor_maximum_gray;
int min_gray_dist;
int left_gray;
int right_gray;
int min_bunch_gray_dist;
int left_bunch_gray;
int right_bunch_gray;
int bunch_gray_length;
int neighbor_gray_length;
int wide_gray_gap;
int neighbor_bunch_maximal_saturation;
int neighbor_bunch_minimal_saturation;
int min_sat_dist;
int left_sat;
int right_sat;
int difference_hue_left;
int difference_hue_right;
int left_hue;
int right_hue;
int min_hue_dist;
int left_bunch_hue;
int right_bunch_hue;
int min_hue_bunch_dist;
int hue_close;
int saturation_close;
int grayscale_close;
int inclusion_relation;
int impossible_gray_variation;
int hue_dif;
int hue_dif1;
int maximum_zone_gray;
int Intersection_saturation;
int indic_length_satur, ratio_length_satur, ratio_length1_satur;
int bunch_minimal_saturation;
int bunch_maximal_saturation;
int left_sat_bunch;
int right_sat_bunch;
int poss_sat_variation;
int imp_sat_var;
int union_gray_left;
int union_gray_right;
int union_gray_length;
int abs_satur;
int abs_satur1;
int gray_ntersection1;
int gray_ntersection2;
int gray_ntersection3;
int indic_length_gray1;
int ratio_length_gray1;
int ratio_length1_gray1;
int indic_length_gray2;
int ratio_length_gray2;
int ratio_length1_gray2;
int indic_length_gray3;
int ratio_length_gray3;
int ratio_length1_gray3;
int Intersection_hue;
int minimum_zone_gray;
int indic_length_hue, ratio_length_hue, ratio_length1_hue;
int coinsidence1_hue;
int joint_hue_range;
int min_saturation;
int max_saturation;
int num_of_bunches;
TIntColored* ColoredIntervalsStructure1;
TIntColored* ColoredIntervalsStructure;

Intersection_saturation=-1;
Intersection_hue=-1;
indic_length_satur=-1;
ratio_length_satur=-1;
ratio_length1_satur=-1;
Intersection_Measure=-1;
inclusion_relation=0;
prior=0;
hue_close=0;
saturation_close=0;
grayscale_close=0;
wide_gray_gap=0;

ColoredIntervalsStructure1=ColorInt[strip_number].ColoredIntervalsStructure;
ColoredIntervalsStructure=&IntegratedColorIntervals[previous_strip_number];
num_of_bunches=ColoredIntervalsStructure->num_of_int;
if(previous_bunch>num_of_bunches)
{//error24.04.18
return(-1);
}
beg_int=ColoredIntervalsStructure->BegInterv[previous_bunch];
end_int=ColoredIntervalsStructure->EndInterv[previous_bunch];
int_length=end_int-beg_int+1;
neighbor_beg_int=ColoredIntervalsStructure1->BegInterv[next_bunch];
neighbor_end_int=ColoredIntervalsStructure1->EndInterv[next_bunch];
neighbor_length=neighbor_end_int-neighbor_beg_int+1;
if((neighbor_beg_int<=beg_int)&&(neighbor_end_int>=end_int))
{
inclusion_relation=1;
}
//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
	//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
bunch_significance=ColoredIntervalsStructure->Significance[previous_bunch];
neighbor_significance=ColoredIntervalsStructure1->Significance[next_bunch];
Signif_ratio=(16*bunch_significance)/(bunch_significance+neighbor_significance);

bunch_density=2*bunch_significance/int_length;
neighbor_density=2*neighbor_significance/neighbor_length;
Density_ratio=(16*bunch_density)/(bunch_density+neighbor_density);
length_ratio=(16*int_length)/(int_length+neighbor_length);

node_vicinity=0;
node_vicinity_neighbor=0;
bunch_occurrence=ColorInt[strip_number].visible_bunches[previous_bunch];
bunch_mean_hue=ColoredIntervalsStructure->AverageHue[previous_bunch];
if(bunch_mean_hue>=NUM_HUES)
{
bunch_mean_hue-=NUM_HUES;
}
bunch_minimal_hue=ColoredIntervalsStructure->LowerHueValue[previous_bunch];
if(bunch_minimal_hue>=NUM_HUES)
{
bunch_minimal_hue-=NUM_HUES;
}

bunch_maximal_hue=ColoredIntervalsStructure->UpperHueValue[previous_bunch];
if(bunch_maximal_hue>=NUM_HUES)
{
bunch_maximal_hue-=NUM_HUES;
}
zone_hue_bunch=hue_zones[bunch_mean_hue];
neighbor_bunch_mean_hue=ColoredIntervalsStructure1->AverageHue[next_bunch];
if(neighbor_bunch_mean_hue>=NUM_HUES)
{
neighbor_bunch_mean_hue-=NUM_HUES;
}
zone_hue=hue_zones[neighbor_bunch_mean_hue];
hue_bound=dev_hue_zone[zone_hue];
difference_hue_zone=abs(zone_hue_bunch-zone_hue);
		  difference_hue_zone=std::min(difference_hue_zone,5-difference_hue_zone);

hue_dif=abs(bunch_mean_hue-neighbor_bunch_mean_hue);
hue_difference=std::min(hue_dif,NUM_HUES-hue_dif);
min_hue_zone=std::min(zone_hue_bunch,zone_hue);//last_cor17.04.15
max_hue_zone=std::max(zone_hue_bunch,zone_hue);
if(hue_dif>hue_difference)
{
	if(bunch_mean_hue<neighbor_bunch_mean_hue)
	{
		corrected_bunch_mean_hue=bunch_mean_hue+NUM_HUES;
		corrected_neighbor_bunch_mean_hue=neighbor_bunch_mean_hue;
	}
	else
	{
		if(bunch_mean_hue>neighbor_bunch_mean_hue)
	    {
		corrected_bunch_mean_hue=bunch_mean_hue;
		corrected_neighbor_bunch_mean_hue=neighbor_bunch_mean_hue+NUM_HUES;
	    }
	}
}
else
{
	corrected_bunch_mean_hue=bunch_mean_hue;
	corrected_neighbor_bunch_mean_hue=neighbor_bunch_mean_hue;
}

hue_dif=abs(bunch_mean_hue-bunch_maximal_hue);
right_bunch_hue=std::min(hue_dif,NUM_HUES-hue_dif);
if(hue_dif>right_bunch_hue)
{
corrected_bunch_maximal_hue=bunch_maximal_hue+NUM_HUES;
     if(corrected_bunch_mean_hue<0)
	 {
	 corrected_bunch_mean_hue=bunch_mean_hue;
	 }

}
else
{
	corrected_bunch_maximal_hue=bunch_maximal_hue;
}

hue_dif=abs(bunch_mean_hue-bunch_minimal_hue);
left_bunch_hue=std::min(hue_dif,NUM_HUES-hue_dif);
min_hue_bunch_dist=std::min(left_bunch_hue,right_bunch_hue);
if(hue_dif>left_bunch_hue)
{
    if(corrected_bunch_mean_hue<0)
	{
corrected_bunch_mean_hue=bunch_mean_hue+NUM_HUES;
	}
}

neighbor_bunch_minimal_hue=ColoredIntervalsStructure1->LowerHueValue[next_bunch];
if(neighbor_bunch_minimal_hue>=NUM_HUES)
{
neighbor_bunch_minimal_hue-=NUM_HUES;
}
neighbor_bunch_maximal_hue=ColoredIntervalsStructure1->UpperHueValue[next_bunch];
if(neighbor_bunch_maximal_hue>=NUM_HUES)
{
neighbor_bunch_maximal_hue-=NUM_HUES;
}
hue_dif=abs(neighbor_bunch_maximal_hue-neighbor_bunch_mean_hue);
right_hue=std::min(hue_dif,NUM_HUES-hue_dif);
if(hue_dif>right_hue)
{
corrected_neighbor_bunch_maximal_hue=neighbor_bunch_maximal_hue+NUM_HUES;
if(corrected_neighbor_bunch_mean_hue<0)
{
corrected_neighbor_bunch_mean_hue=neighbor_bunch_mean_hue;
}
}
hue_dif=abs(neighbor_bunch_mean_hue-neighbor_bunch_minimal_hue);
left_hue=std::min(hue_dif,NUM_HUES-hue_dif);
if(hue_dif>left_hue)
{
	if(corrected_neighbor_bunch_mean_hue<0)
	{
corrected_neighbor_bunch_mean_hue=neighbor_bunch_mean_hue+NUM_HUES;
	}
}
min_hue_dist=std::min(left_hue,right_hue);
if(corrected_neighbor_bunch_mean_hue<0)
{
corrected_neighbor_bunch_mean_hue=neighbor_bunch_mean_hue;
}
if(corrected_bunch_mean_hue<0)
	{
corrected_bunch_mean_hue=bunch_mean_hue;
	}
hue_dif=abs(corrected_neighbor_bunch_mean_hue-corrected_bunch_mean_hue);
hue_dif1=std::min(hue_dif,NUM_HUES-hue_dif);
if(hue_dif1<hue_dif)
{
	if(corrected_neighbor_bunch_mean_hue>corrected_bunch_mean_hue)
	{
		corrected_bunch_mean_hue+=NUM_HUES;
	}
	else
	{
    corrected_neighbor_bunch_mean_hue+=NUM_HUES;
	}
}
if((neighbor_bunch_minimal_hue<=neighbor_bunch_mean_hue)&&(bunch_minimal_hue<=bunch_mean_hue))
{//cond0
corrected_neighbor_bunch_mean_hue=neighbor_bunch_mean_hue;
corrected_bunch_mean_hue=bunch_mean_hue;
}//cond0
else
{//econd0
if(neighbor_bunch_minimal_hue>neighbor_bunch_mean_hue)
{//cond1
hue_dif=neighbor_bunch_minimal_hue-neighbor_bunch_mean_hue;
	   if(hue_dif>(NUM_HUES-hue_dif))
	   {//cond2
corrected_neighbor_bunch_mean_hue=neighbor_bunch_mean_hue+NUM_HUES;
if(bunch_mean_hue<NUM_HUES/2)
{//last_cor21.05.15
	corrected_bunch_mean_hue=bunch_mean_hue+NUM_HUES;
}//last_cor21.05.15
	   }//cond2
	   else
	   {//econd2
       corrected_bunch_mean_hue=bunch_mean_hue;
	   }//econd2

}//cond1
//else
//{//econd1
	      if(bunch_minimal_hue>bunch_mean_hue)
	      {//last_cor21.05.15
hue_dif=bunch_minimal_hue-bunch_mean_hue;
    if(hue_dif>(NUM_HUES-hue_dif))
	{//cond3
corrected_bunch_mean_hue=bunch_mean_hue+NUM_HUES;
if(neighbor_bunch_mean_hue<NUM_HUES/2)
{//last_cor21.05.15
	corrected_neighbor_bunch_mean_hue=neighbor_bunch_mean_hue+NUM_HUES;
}//last_cor21.05.15
    }//cond3
	else
	{//econd3
corrected_bunch_mean_hue=bunch_mean_hue;
	}//econd3

		  }//last_cor21.05.15
		 /* if(bunch_minimal_hue>bunch_mean_hue)
	      {//last_cor21.05.15
hue_dif=bunch_minimal_hue-bunch_mean_hue;
    if(hue_dif>(NUM_HUES-hue_dif))
	{//cond3
corrected_bunch_mean_hue=bunch_mean_hue+NUM_HUES;
corrected_bunch_mean_hue=bunch_mean_hue;
    }//cond3
	else
	{//econd3
corrected_bunch_mean_hue=bunch_mean_hue;
	}//econd3
		  }//last_cor21.05.15*/
//}//econd1

}//econd0
left_bunch_hue=corrected_bunch_mean_hue-min_hue_bunch_dist;
right_bunch_hue=corrected_bunch_mean_hue+min_hue_bunch_dist;

left_hue=corrected_neighbor_bunch_mean_hue-min_hue_dist;
right_hue=corrected_neighbor_bunch_mean_hue+min_hue_dist;
difference_hue_left=abs(corrected_bunch_mean_hue-left_hue);
difference_hue_left=std::min(difference_hue_left,NUM_HUES-difference_hue_left);
difference_hue_right=abs(corrected_bunch_mean_hue-right_hue);
difference_hue_right=std::min(difference_hue_right,NUM_HUES-difference_hue_right);
Intersection_hue=ColorInt[strip_number].Intersection_measure_ratios(left_bunch_hue,right_bunch_hue,
left_hue,right_hue,&indic_length_hue,&ratio_length_hue,&ratio_length1_hue);
coinsidence1_hue=std::max(ratio_length_hue,ratio_length1_hue);
joint_hue_range=std::max(right_bunch_hue,right_hue)-std::min(left_bunch_hue,left_hue);
if((hue_difference<=1)||((hue_difference<=2)&&((coinsidence1_hue>=18)||(joint_hue_range<=6))))
{
	hue_close=1;
}
if(((bunch_mean_hue>=46)&&(bunch_mean_hue<48))||
((bunch_mean_hue>=0)&&(bunch_mean_hue<=2))||
((bunch_mean_hue>=30)&&(bunch_mean_hue<=34))||
((bunch_mean_hue>=12)&&(bunch_mean_hue<=20)))
{
node_vicinity=1;
}
if(((neighbor_bunch_mean_hue>=46)&&(neighbor_bunch_mean_hue<=48))||
((neighbor_bunch_mean_hue>=0)&&(neighbor_bunch_mean_hue<=2))||
((neighbor_bunch_mean_hue>=30)&&(neighbor_bunch_mean_hue<=34))||
((neighbor_bunch_mean_hue>=12)&&(neighbor_bunch_mean_hue<=20)))
{
node_vicinity_neighbor=1;
}
bunch_mean_saturation=ColoredIntervalsStructure->AverageSat[previous_bunch];
bunch_minimal_saturation=ColoredIntervalsStructure->LowerSatValue[previous_bunch];
bunch_maximal_saturation=ColoredIntervalsStructure->UpperSatValue[previous_bunch];
min_sat_dist=std::min(bunch_maximal_saturation-bunch_mean_saturation,bunch_mean_saturation-bunch_minimal_saturation);
left_sat_bunch=bunch_mean_saturation-min_sat_dist;
right_sat_bunch=min_sat_dist+bunch_mean_saturation;
neighbor_bunch_mean_saturation=
ColoredIntervalsStructure1->AverageSat[next_bunch];
max_saturation=std::max(bunch_mean_saturation,neighbor_bunch_mean_saturation);
min_saturation=std::min(bunch_mean_saturation,neighbor_bunch_mean_saturation);
//!?
neighbor_bunch_minimal_saturation=ColoredIntervalsStructure1->LowerSatValue[next_bunch];
neighbor_bunch_maximal_saturation=ColoredIntervalsStructure1->UpperSatValue[next_bunch];
min_sat_dist=std::min(neighbor_bunch_maximal_saturation-neighbor_bunch_mean_saturation,
				 neighbor_bunch_mean_saturation-neighbor_bunch_minimal_saturation);
left_sat=neighbor_bunch_mean_saturation-min_sat_dist;
right_sat=min_sat_dist+neighbor_bunch_mean_saturation;
abs_satur=abs(bunch_mean_saturation-left_sat);
abs_satur1=abs(bunch_mean_saturation-right_sat);
abs_satur=std::min(abs_satur,abs_satur1);
possible_sat_variation=dev_mean_sat[neighbor_bunch_mean_saturation];
possible_sat_variation1=dev_mean_sat1[neighbor_bunch_mean_saturation];
impossible_sat_variation=imp_dev_mean_sat[neighbor_bunch_mean_saturation];
impossible_sat_variation1=imp_dev_mean_sat1[neighbor_bunch_mean_saturation];
if(neighbor_bunch_mean_saturation>=bunch_mean_saturation)
	{//last_cor11.09.13
	saturation_distance=neighbor_bunch_mean_saturation-bunch_mean_saturation;
	saturation_distance1=0;
	poss_sat_variation=dev_mean_sat[bunch_mean_saturation];
	poss_sat_variation=std::max(possible_sat_variation1,poss_sat_variation);
	imp_sat_var=imp_dev_mean_sat[bunch_mean_saturation];
		imp_sat_var=std::min(impossible_sat_variation1,imp_sat_var);
	}
	else
	{
	saturation_distance1=bunch_mean_saturation-neighbor_bunch_mean_saturation;
	saturation_distance=0;
	poss_sat_variation=dev_mean_sat1[bunch_mean_saturation];
		poss_sat_variation=std::max(poss_sat_variation,possible_sat_variation);
	imp_sat_var=imp_dev_mean_sat1[bunch_mean_saturation];
		imp_sat_var=std::min(imp_sat_var,impossible_sat_variation);
	}
max_satur_differ=std::max(saturation_distance,saturation_distance1);
    Intersection_saturation=ColorInt[strip_number].Intersection_measure_ratios(left_sat,right_sat,left_sat_bunch,
	right_sat_bunch,&indic_length_satur,&ratio_length_satur,&ratio_length1_satur);
bunch_mean_gray=ColoredIntervalsStructure->AverageGray[previous_bunch];
bunch_minimal_gray=ColoredIntervalsStructure->LowerGrayscale[previous_bunch];
bunch_maximal_gray=ColoredIntervalsStructure->UpperGrayscale[previous_bunch];
min_bunch_gray_dist=std::min(bunch_maximal_gray-bunch_mean_gray,bunch_mean_gray-bunch_minimal_gray);
left_bunch_gray=bunch_mean_gray-min_bunch_gray_dist;
right_bunch_gray=min_bunch_gray_dist+bunch_mean_gray;
bunch_gray_length=2*min_bunch_gray_dist+1;
zone_gray_bunch=gray_zones[bunch_mean_gray];
neighbor_bunch_mean_gray=
ColoredIntervalsStructure1->AverageGray[next_bunch];
neighbor_minimum_gray=ColoredIntervalsStructure1->LowerGrayscale[next_bunch];
neighbor_maximum_gray=ColoredIntervalsStructure1->UpperGrayscale[next_bunch];
min_gray_dist=std::min(neighbor_maximum_gray-neighbor_bunch_mean_gray,neighbor_bunch_mean_gray-neighbor_minimum_gray);
left_gray=neighbor_bunch_mean_gray-min_gray_dist;
right_gray=min_gray_dist+neighbor_bunch_mean_gray;
union_gray_left=std::min(left_bunch_gray,left_gray);
union_gray_right=std::max(right_bunch_gray,right_gray);
union_gray_length=union_gray_right-union_gray_left;
zone_gray=gray_zones[neighbor_bunch_mean_gray];
neighbor_gray_length=2*min_gray_dist+1;
difference_gray_zone=abs(zone_gray_bunch-zone_gray);
possible_gray_variation=std::min(dev_mean[neighbor_bunch_mean_gray/8],dev_mean[bunch_mean_gray/8]);
impossible_gray_variation=std::min(imp_dev_mean[neighbor_bunch_mean_gray/8],
imp_dev_mean[bunch_mean_gray/8]);//last_cor5.07.13
maximum_zone_gray=std::max(zone_gray,zone_gray_bunch);
minimum_zone_gray=std::min(zone_gray,zone_gray_bunch);

gray_ntersection=ColorInt[strip_number].Intersection_measure_ratios(left_gray,right_gray,left_bunch_gray,
	right_bunch_gray,&indic_length_gray,&ratio_length_gray,&ratio_length1_gray);
gray_ntersection1=ColorInt[strip_number].Intersection_measure_ratios(left_gray,right_gray,left_bunch_gray,
	bunch_mean_gray,&indic_length_gray1,&ratio_length_gray1,&ratio_length1_gray1);
gray_ntersection2=ColorInt[strip_number].Intersection_measure_ratios(left_gray,right_gray,bunch_mean_gray,
	right_bunch_gray,&indic_length_gray2,&ratio_length_gray2,&ratio_length1_gray2);
if(gray_ntersection==3)
{//intersection is empty
	if(union_gray_length>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
	{
	if(((ratio_length_gray+1)>=bunch_gray_length)||((ratio_length_gray+1)>=neighbor_gray_length))
	{
	wide_gray_gap=1;
	}
    }
}

	//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
	//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller

/*if((bunch_mean_hue<4)&&(bunch_mean_hue>=0)&&(neighbor_bunch_mean_hue>39))
{
corrected_bunch_mean_hue=bunch_mean_hue+48;
}

if((bunch_mean_hue>39)&&(neighbor_bunch_mean_hue<4)&&
(neighbor_bunch_mean_hue>=0))
{
corrected_neighbor_bunch_mean_hue=neighbor_bunch_mean_hue+48;
  if(neighbor_bunch_minimal_hue<4)
  {
  corrected_neighbor_bunch_minimal_hue=neighbor_bunch_minimal_hue+48;
  corrected_neighbor_bunch_maximal_hue=neighbor_bunch_maximal_hue+48;
  }

}*/

/*hue_difference=
abs(corrected_bunch_mean_hue-corrected_neighbor_bunch_mean_hue);
hue_difference=std::min(hue_difference,NUM_HUES-hue_difference);
if(hue_difference<=1)
{
	hue_close=1;
}
          difference_hue_zone=abs(zone_hue_bunch-zone_hue);
		  difference_hue_zone=std::min(difference_hue_zone,5-difference_hue_zone);
		  hue_dif=corrected_neighbor_bunch_maximal_hue-corrected_neighbor_bunch_mean_hue;
hue_dif=std::min(hue_dif,NUM_HUES-hue_dif);
hue_dif1=corrected_neighbor_bunch_mean_hue-corrected_neighbor_bunch_minimal_hue;
hue_dif1=std::min(hue_dif1,NUM_HUES-hue_dif1);
min_hue_dist=
std::min(hue_dif,hue_dif1);
left_hue=corrected_neighbor_bunch_mean_hue-min_hue_dist;
right_hue=corrected_neighbor_bunch_mean_hue+min_hue_dist;
difference_hue_left=abs(corrected_bunch_mean_hue-left_hue);
difference_hue_left=std::min(difference_hue_left,NUM_HUES-difference_hue_left);
difference_hue_right=abs(corrected_bunch_mean_hue-right_hue);
difference_hue_right=std::min(difference_hue_right,NUM_HUES-difference_hue_right);*/
saturation_difference=
abs(bunch_mean_saturation-neighbor_bunch_mean_saturation);
gray_difference=abs(bunch_mean_gray-neighbor_bunch_mean_gray);

if((gray_difference>=impossible_gray_variation)||((wide_gray_gap)&&(difference_gray_zone>1)))
	{
		if(!grayscale_close)
		    {//last_cor18.05.15
		if(2*gray_difference>=3*impossible_gray_variation)
		{//last cor
			grayscale_close=-3;
		}
		else
		{//last_cor18.05.15
		grayscale_close=-2;
		}//last_cor18.05.15
		   }//last_cor18.05.15
	}
		else
		{//egray
			if(union_gray_length<possible_gray_variation)
			{
				grayscale_close=1;
			}
			else
			{
if((2*gray_difference<=possible_gray_variation)&&
   (((bunch_mean_gray>=left_gray)&&(bunch_mean_gray<=right_gray))||
   ((neighbor_bunch_mean_gray>=left_bunch_gray)&&(neighbor_bunch_mean_gray<=right_bunch_gray))))
		    {//gr2
		    grayscale_close=1;
			}//gr2
else
{
	if((gray_difference<=possible_gray_variation)&&
		((((bunch_mean_gray>=left_gray)&&(bunch_mean_gray<=right_gray))||
((neighbor_bunch_mean_gray>=left_bunch_gray)&&(neighbor_bunch_mean_gray<=right_bunch_gray)))))
	        {//gr3
				if((gray_difference<=(2*possible_gray_variation)/3))
				{
		    grayscale_close=2;
				}
				else
				{
				if((gray_ntersection<=1)&&(gray_difference<possible_gray_variation))
					{
						grayscale_close=2;
					}
					else
					{
				grayscale_close=3;
					}
				}
			}//gr3
	       else
	       {

			   if((maximum_zone_gray<=2)||((maximum_zone_gray<=3)&&(minimum_zone_gray<=2)))
			   {
				   if((gray_difference<=possible_gray_variation)&&(gray_ntersection<=1)&&
					   ((!gray_ntersection1)||(!gray_ntersection2)))
				   {
                    if((gray_difference<=(2*possible_gray_variation)/3))
				    {
		            grayscale_close=2;
				    }
				    else
				    {
				    grayscale_close=3;
				    }
				   }
				   else
				   {
	if((gray_difference<=possible_gray_variation)||
		(((bunch_mean_gray>=left_gray)&&(bunch_mean_gray<=right_gray))||
((neighbor_bunch_mean_gray>=left_bunch_gray)&&(neighbor_bunch_mean_gray<=right_bunch_gray))))
	{
		if((gray_difference<=(2*possible_gray_variation)/3))
				    {
		            if(wide_gray_gap)
					   {
						grayscale_close=3;
					   }
					   else
					   {
                   grayscale_close=2;
					   }
				    }
		else
		{
		grayscale_close=3;
		}
	}
				   }
			   }
			   else
			   {//e
				   if(gray_difference<=possible_gray_variation)
			       {
					   if(3*gray_difference<=2*possible_gray_variation)
					   {//!!!
						if(minimum_zone_gray>=5)
						   {//last_cor31.8.15
						   grayscale_close=1;
						   }
						   else
						   {
						grayscale_close=2;
						   }//last_cor31.8.15
					   }
					   else
					   {
 if((gray_difference<possible_gray_variation)&&((gray_ntersection<=1)||(gray_ntersection1<=1)||
	 (gray_ntersection2<=1)))
						   {//last_cor27.05.13
							   grayscale_close=2;
						   }
						   else
						   {
							   if(4*gray_difference<3*possible_gray_variation)
							   {//last_cor26.03.14
								   grayscale_close=2;
							   }//last_cor26.03.14
							   else
							   {
				               grayscale_close=3;
							   }
						   }
					   }
			       }
				   else
				   {//e1
				   if((bunch_occurrence>0)&&(Density_ratio>=6)&&(Density_ratio<=10))
				   {//bo
				   if(maximum_zone_gray>=3)
				   {//m=3
					   if((gray_ntersection==3)&&(wide_gray_gap))
					   {//gr_i=3
						   if(gray_difference>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
						   {
							   if((ratio_length_gray>(2*possible_gray_variation/3)))
							   {
                                grayscale_close=-2;
							   }
							   else
							   {
								   if((ratio_length_gray>(possible_gray_variation/2)))
							   {
                                grayscale_close=-1;
							   }
							   }
						   }

					   }//gr_i=3
				   }//m=3
				   else
				   {//em=3
				   if(maximum_zone_gray>=5)
				   {//mzg
					   if((gray_difference>possible_gray_variation)&&(gray_ntersection==3))
					   {
                       grayscale_close=-2;
					   }
					   else
					   {
	  if(((gray_ntersection==3)||((gray_ntersection>=2)&&((ratio_length1_gray<=3)||(ratio_length_gray<=3))))&&
							   (((bunch_mean_gray<left_gray)||(bunch_mean_gray>right_gray))&&
	  ((neighbor_bunch_mean_gray<left_bunch_gray)||(neighbor_bunch_mean_gray>right_bunch_gray))))
						   {
							   grayscale_close=-1;
						   }
					   }
				   }//mzg
				   }//em=3
				   }//bo
				   }//e1

			   }//e
	      }
}
}
		}//egray
if(grayscale_close==0)
{//gc0
	if(gray_difference<=possible_gray_variation)//?!
			       {
					   if(3*gray_difference<=2*possible_gray_variation)
					   {//!!!
						grayscale_close=2;
					   }
					   else
					   {
				   if(maximum_zone_gray<=1)
						   {
							   grayscale_close=2;
						   }
						   else
						   {
				   grayscale_close=3;
						   }
					   }
	                }//?!
	if((hue_close==1)&&(saturation_close==1)&&(gray_difference<impossible_gray_variation)&&
		(ratio_length>=13)&&(ratio_length1>=13))
	{
     grayscale_close=3;
	}
	else
	{//e1
if(gray_difference>=(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
{//gd>=
	if((bunch_mean_gray<left_gray)||(bunch_mean_gray>right_gray)||(ratio_length_gray<3)||
		(ratio_length1_gray<3))
	{
	grayscale_close=-2;
	}
	else
	{
		grayscale_close=-1;
	}

}//gd>=
else
{//e2
 if(gray_difference<(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/4))
 {//gd<
gray_ntersection3=ColorInt[strip_number].Intersection_measure_ratios(bunch_minimal_gray,
bunch_maximal_gray,neighbor_minimum_gray,neighbor_maximum_gray,&indic_length_gray3,
&ratio_length_gray3,&ratio_length1_gray3);
if(gray_ntersection3==3)
{
	ratio_length_gray3=0;
}
if((ratio_length_gray3>=10)&&(ratio_length1_gray3>=10))
	{
    if((ratio_length_gray>8)||(ratio_length1_gray>8))
		{//last_cor10.06.14
    grayscale_close=4;
		}
	}
 }//gd<
 else
  {//e3
	if(gray_difference>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
	{//last_cor03.06.13
		if((gray_ntersection>=2)&&((ratio_length1_gray<=2)||(ratio_length1_gray<=2)))
		{
			grayscale_close=-1;
		}
	}
  }//e3
}//e2
	}//e1
}//gc0
if(!grayscale_close)
{//last_cor23.05.14
	if(gray_difference>possible_gray_variation)
	{
		if(gray_ntersection==3)
		{
			if(((difference_gray_zone>=1)&&(wide_gray_gap))||(difference_gray_zone>=2))
			{
				grayscale_close=-1;
			}
		}

    }
}
if((bunch_mean_saturation==0)&&(neighbor_bunch_mean_saturation==0))
{//s==0
	saturation_close=1;
	if(maximum_zone_gray<=1)
			{//last_cor14.04.14
				if(grayscale_close>0)
				{
					grayscale_close=1;
				}
				if(!difference_hue_zone)
				{
					hue_close=1;
				}
			}
	else//last_cor27.05.15
	{
		if(minimum_zone_gray>=5)
		{
			if(maximum_zone_gray==7)
			{//last_cor04.06.15
if(gray_difference<=2)
{
	prior=1;
	goto L;
}
			}//last_cor04.06.15
			if(grayscale_close>0)
			{
			   if(!difference_hue_zone)
			   {
				   if(zone_hue==4)
				   {
					   if(hue_difference<=8)
					   {
						   if(grayscale_close<=2)
						   {
					   hue_close=1;
					   prior=1;
					   goto L;
						   }
						   else
						   {
							   hue_close=2;
					           prior=1;
					           goto L;
						   }
					   }
					   else
					   {
						   if(grayscale_close<=2)
						   {
					   hue_close=2;
					   prior=2;
					   goto L;
						   }
						   else
						   {
							   hue_close=3;
					           prior=3;
					           goto L;
						   }
					   }
				   }
			   }
			}
		}
	}//last_cor27.05.15
	saturation_close=1;
	if(grayscale_close<0)
	{//lacor
		if(grayscale_close<=-2)
		{
			if(2*gray_difference>3*impossible_gray_variation)
			{
			prior=5;
            goto L;
			}
			else
			{
			prior=6;
            goto L;
			}
		}
		else
		{
			if(gray_difference>impossible_gray_variation)
			{
			prior=6;
            goto L;
			}
			else
			{
			prior=7;
            goto L;
			}
		}
	}//lacor
	if(grayscale_close<0)
	{//lacor
		if(grayscale_close<=-2)
		{
			if(2*gray_difference>3*impossible_gray_variation)
			{
			prior=5;
            goto L;
			}
			else
			{
			prior=6;
            goto L;
			}
		}
		else
		{
			if(gray_difference>impossible_gray_variation)
			{
			prior=6;
            goto L;
			}
			else
			{
			prior=7;
            goto L;
			}
		}
	}//lacor
	if((grayscale_close==1)&&(gray_difference<=(possible_gray_variation/2)))
	{
	prior=1;
	goto L;
	}
           if((grayscale_close>0)&&(grayscale_close<=2)&&(difference_hue_zone<=1))
                   {//grayscale and hue_zones restrictions
	if(difference_hue_zone==0)
	{//the same zone

			if((grayscale_close==1)||(zone_gray<=2))
			{
	        prior=1;
			goto L;
			}
		    else
		    {
		if(zone_gray<=4)
		    {
			prior=2;
			goto L;
		    }
		else
		{
			prior=3;
			goto L;
		}

			}
	}//the same zone
	if(zone_gray<=2)
	{
		if(grayscale_close==1)
		{
	    prior=1;
		}
		else
		{
		prior=2;
		}
	goto L;
	}
	else
	   {//zgr>2
	if((grayscale_close>=1)&&(grayscale_close<=2)&&(zone_gray<=5))
	{//zg5
		if(!difference_hue_zone)
		{//dhz
			if(grayscale_close==1)
			{
			prior=1;
			}
			else
			{
	       prior=2;
			}
			goto L;
		}//dhz
		else
		{//!dhz
			if(hue_difference>=12)
			{
				prior=6;
			goto L;
			}
			else
			{//e>=12
				if(hue_difference>=8)
				{
					if((grayscale_close==0)||(grayscale_close>=2))
			        {
			        prior=7;
			        goto L;
			       }
				}
				else
				{//e>=8
					if(hue_difference<=4)
					{//h<=4
			if(grayscale_close==1)
			{
			prior=2;
			goto L;
			}
			else
			{//egs1
				if(grayscale_close==2)
				{
					prior=3;
			        goto L;
				}
			}//egs1
					}//h<=4
					else
					{//eh<=4
                      if(grayscale_close==1)
			          {
			          prior=3;
			          goto L;
			          }
					  else
					  {
						 if(grayscale_close==2)
			             {
			             prior=4;
			             goto L;
			             }
					   }
					}//eh<=4
				}//e>=8
			}//e>=12
		}//!dhz
	}//zg5
	else
	{
	if((grayscale_close<=2)&&(zone_gray<=4)&&(gray_difference<=possible_gray_variation))
	{
	prior=2;
	goto L;
	}
	}

	    }//zgr>2
                   }//grayscale and hue_zones restrictions

if(((difference_hue_zone>2)&&(grayscale_close<0))||(grayscale_close<-1))
{
prior=15;
goto L;
}
else
{
if((difference_hue_zone>2)&&(grayscale_close==0))
{
prior=16;
goto L;
}
if(grayscale_close<0)
{
prior=17;
goto L;
}
}
	   prior=18;
	   goto L;
}//both s=0
if((bunch_mean_saturation<=1)&&(neighbor_bunch_mean_saturation<=1))
{//s<=1
	saturation_close=1;
	if(maximum_zone_gray<=1)
			{//last_cor14.04.14
				if(grayscale_close>0)
				{
					grayscale_close=1;
				}
				if(!difference_hue_zone)
				{
					hue_close=1;
				}
			}
	else//last_cor27.05.15
	{
		if(minimum_zone_gray>=5)
		{
			if(maximum_zone_gray==7)
			{//last_cor04.06.15
if(gray_difference<=2)
{
	prior=1;
	goto L;
}
			}//last_cor04.06.15
			if(grayscale_close>0)
			{
			   if(!difference_hue_zone)
			   {
				   if(zone_hue==4)
				   {
					   if(hue_difference<=8)
					   {
						   if(grayscale_close<=2)
						   {
					   hue_close=1;
					   prior=1;
					   goto L;
						   }
						   else
						   {
							   hue_close=2;
					           prior=1;
					           goto L;
						   }
					   }
					   else
					   {
						   if(grayscale_close<=2)
						   {
					   hue_close=2;
					   prior=2;
					   goto L;
						   }
						   else
						   {
							   hue_close=3;
					           prior=3;
					           goto L;
						   }
					   }
				   }
			   }
			}
		}
	}//last_cor27.05.15
	if(grayscale_close<0)
	{//lacor
		if(grayscale_close<=-2)
		{
			if(2*gray_difference>3*impossible_gray_variation)
			{
			prior=5;
            goto L;
			}
			else
			{
			prior=6;
            goto L;
			}
		}
		else
		{
			if(gray_difference>impossible_gray_variation)
			{
			prior=6;
            goto L;
			}
			else
			{
			prior=7;
            goto L;
			}
		}
	}//lacor
	if((zone_gray<=1)||(zone_gray_bunch<=1))
	{//zg1g
		if((grayscale_close==1)&&(difference_hue_zone<=1))
		{
			prior=1;
	        goto L;
		}
		else
		{//last_cor
			if((grayscale_close>0)&&(grayscale_close<=2)&&(hue_close>0)&&(saturation_close>0))
			{
				prior=1;
	        goto L;
			}
		}
	}//zg1g
else
	{
if((zone_gray<=2)||(zone_gray_bunch<=2))//last_cor 21.04.13
	{//zg2
		if(!hue_close)
		{//last_cor18.03.15
			if(!difference_hue_zone)
			{
				if(zone_hue>2)
				{
					if(hue_difference<=3)
					{
						hue_close=1;
					}
				}

			}
		}//last_cor18.03.15
		if((grayscale_close==1)&&(difference_hue_zone<=1))
		{//gc1dhz1
			if((hue_close==1)&&(saturation_close==1))
			{//last_cor18.03.15
				prior=1;
				goto L;
			}
			else
			{//last_cor18.03.15
if(union_gray_length<possible_gray_variation)
			{//ugl
				if(hue_close>0)
				{//last_cor18.03.15
				prior=1;
				goto L;
				}
				else
				{
					if(!difference_hue_zone)
					{
			        prior=2;
	                 goto L;
					}

				 }

                }//last_cor18.03.15
			}//ugl
		}//gc1dhz1
	}//zg2
else
{//last_cor 10.06.13
	if(((zone_gray<=4)||(zone_gray_bunch<=4))&&(saturation_close==1))
	{//zg4
		if((gray_ntersection<=1)&&(max_saturation<=1)&&(grayscale_close==1)&&
   (gray_difference<=possible_gray_variation/2))
{//last_cor19.09.13
	if(hue_close>0)
	{//last_cor27.10.14
	prior=1;
goto L;
	}
}

		if(hue_close<=0)
		{//hc
if((difference_hue_zone<=1)&&(((zone_hue>2)&&(zone_hue<5))||
   ((zone_hue_bunch>2)&&(zone_hue_bunch<5))))
			{//dhz//last_cor11.04.14
				if(!difference_hue_zone)
				{//last_cor26.11.14
					if(grayscale_close==1)
					{
						if((hue_difference<=3)&&(!hue_close))
						{
							prior=1;
							hue_close=1;
							goto L;
						}
					}
				}
				if((hue_difference<=4)||((!difference_hue_zone)&&(hue_difference<=6)))
				{//last_cor11.4.14
				prior=2;
				hue_close=2;
				goto L;
				}
				else
				{//ehd
if((length_ratio<=2)||(length_ratio>=14))
		{//lr
					if(hue_difference<=8)
				    {
					prior=3;
				    hue_close=3;
				    goto L;
					}
					else
					{
					prior=4;
				    hue_close=-1;
				    goto L;
					}
		}//lr
				}//ehd
			}//dhz
		}//hc

	}//zg4
else
	{//last_cor09.06.14
		if((zone_gray>=5)&&(zone_gray_bunch>=5))
		{
			if((grayscale_close==1)&&(!gray_ntersection))
			{//last_cor26.11.14and29.12.14
               if(!hue_close)
			   {//hc0
				   if(difference_hue_zone<=1)
				   {//dhz1
					   if(!difference_hue_zone)
					   {
                       if(hue_difference<=5)
                       {
                       hue_close=1;
                       prior=1;
                       goto L;
                       }
                       else
                       {
	                       if(hue_difference<=8)
                           {
                           hue_close=2;
                           prior=1;
                           goto L;
	                       }
                       }
					   }
					   else
					   {//edhz0
                         if((zone_hue>2)||(zone_hue_bunch>2))
						 {//zh>2
							 if(hue_difference<=3)
                             {
                             hue_close=1;
                             prior=1;
                               goto L;
                             }
                             else
                            {//hd3
	                           if(hue_difference<=5)
                                {
                                hue_close=2;
                                prior=2;
                                 goto L;
	                            }
                            }//hd3
						 }//zh>2
					   }//edhz0
				   }//dhz1
			   }//dhz1
			}//last_cor26.11.14and29.12.14
			if((!bunch_mean_saturation)||(!neighbor_bunch_mean_saturation))
			{//bmsndms
				if((grayscale_close==1)&&(gray_difference<=possible_gray_variation/2)&&
(difference_hue_zone<=1)&&(gray_ntersection<=1))
				{//logcond
if(hue_close>0)
{
prior=1;
goto L;
}
else
{//hc>0
	if(!difference_hue_zone)
	{//last_cor29.12.14
		if(hue_difference<=6)
                       {
                       hue_close=1;
                       prior=1;
                       goto L;
                       }
                       else
                       {
	                       if(hue_difference<=8)
                           {
                           hue_close=2;
                           prior=1;
                           goto L;
	                       }
                       }
	}//last_cor29.12.14
	else
					   {//edhz0
                         if((zone_hue>2)||(zone_hue_bunch>2))
						 {//zh>2
							 if(hue_difference<=3)
                             {
                             hue_close=1;
                             prior=1;
                               goto L;
                             }
                             else
                            {//hd3
	                           if(hue_difference<=5)
                                {
                                hue_close=2;
                                prior=2;
                                 goto L;
	                            }
                            }//hd3
						 }//zh>2
					   }//edhz0//last_cor29.12.14

}//ehc>0
				}//logcond
			}//bmsndms
		}
	}//last_cor09.06.14
}//last_cor 10.06.13
	}
	if(zone_gray<=2)
	{//zg1
	if(!hue_close)
	{//hue indefinite
if((hue_difference<=4)||((hue_difference<=8)&&(difference_hue_zone==0)))
{
hue_close=2;
}
else
{//e0
    if((hue_difference<=6)&&(difference_hue_zone==1))
    {
	hue_close=3;
    }
	else
	{
		if((difference_hue_zone>1)||((hue_difference>=10)&&(difference_hue_zone==1)))
		{
			hue_close=-1;
		}
		else
		{
			if(difference_hue_zone>2)
			{
				hue_close=-2;
			}
		}
	}
}//e0
	}//hue indefinite
if((grayscale_close==1)&&(difference_hue_zone<=1))
		{
			if(hue_close>0)
			{
			prior=1;
	        goto L;
			}
			if(difference_hue_zone==0)
			{
				prior=2;
	        goto L;
			}
        }
if((grayscale_close>=1)&&(grayscale_close<=2)&&(hue_close==1))
	   {//gh
	 prior=1;
	 goto L;
       }//gh
       else
      {//e1
		  if((hue_close==1)&&(grayscale_close==3)&&(!saturation_difference))
		  {
			  prior=2;
			  goto L;
		  }
		  if((hue_close>1)&&(grayscale_close>=1)&&(grayscale_close<3))
		  {
		  prior=2;
		  goto L;
	      }
		  else
		  {
      if(((hue_close==1)&&(grayscale_close==3))||((hue_close==3)&&(grayscale_close==1)))
	  {
		  if(difference_gray_zone==0)
		  {
	  prior=2;
	  goto L;
		  }
		  else
		  {
			  prior=3;
			  goto L;
		  }
	     }
	  else
	  {
		  if((hue_close>=2)&&(grayscale_close>1))
		  {
		   if(grayscale_close<=2)
		   {
		prior=2;
		goto L;
		   }
		   else
		   {
			prior=3;
			goto L;
		   }
		  }
	  }
	  /*if((grayscale_close>0)&&(grayscale_close<=2)&&(difference_hue_zone<=1)&&(difference_gray_zone<=1)&&(hue_close>=0))
{
if((ratio_length1==16)&&(Signif_ratio<2)&&(Density_ratio<=2)&&(length_ratio<4))
	   {//incl1
		prior=2;
		//goto L;
	   }//incl1

}*/
		  }
      }//e1
if((grayscale_close==1)&&((!neighbor_bunch_mean_saturation)||
		  (!bunch_mean_saturation)))
	  {
		  if(difference_hue_zone<=1)
		  {
			  prior=1;
		     goto L;
		  }
		  else
		  {
			  prior=2;
		     goto L;
		  }
	  }
	   if(((hue_close<0)&&(grayscale_close<0))||(hue_close<-1)||(grayscale_close<-1))
{
prior=15;
goto L;
}
else
{
if(hue_close<0)
{
prior=16;
goto L;
}
if(grayscale_close<0)
{
prior=17;
goto L;
}
}
	   prior=18;
	   goto L;
	}//zg1
	else
	{//e2
		if((hue_close==1)&&(!saturation_difference))
		{
			if(grayscale_close==1)
			{
			prior=1;
	         goto L;
			}
			else
			{
				if((grayscale_close>0)&&(grayscale_close<=3))
			    {
					prior=2;
	               goto L;
				}
			}
		}
		if(!hue_close)
	{//hue indefinite
		if((hue_difference<=2)||((difference_hue_zone==0)&&(hue_difference<=3)))
{
hue_close=2;
}
else
{//e0
    if((hue_difference<=4)&&(difference_hue_zone<=1))
    {
	hue_close=3;
    }
	else
	{
		if((difference_hue_zone>=2)||((hue_difference>=8)&&(difference_hue_zone==1)))
		{
			hue_close=-1;
		}
		else
		{
			if((difference_hue_zone>2)||(hue_difference>12))
			{
				hue_close=-2;
			}
		}
	}
}//e0
		}//hue indefinite

if((grayscale_close>=1)&&(grayscale_close<=2))
	   {//gh!!!!
		   if(hue_close==1)
		   {
	 prior=1;
	 goto L;
		   }
		   else
		   {
			   if(hue_close>0)
			   {//last_cor27.10.14
				   if(grayscale_close==1)
				   {
					   prior=1;
	                   goto L;
				   }
				   else
				   {
					   prior=2;
	                   goto L;
				   }

			   }
		   }
       }//gh
       else
      {//e1
		  if((hue_close>1)&&(grayscale_close>=1)&&(grayscale_close<3))
		  {
		  prior=2;
		  goto L;
	      }
		  if(((hue_close==1)&&(grayscale_close==3))||((hue_close==3)&&(grayscale_close==1)))
		  {
		  prior=3;
		  goto L;
	      }
		   /*else
	      {
		  if((hue_close>2)&&(grayscale_close>1))
		  {

		  }
	      }*/
      }//e1
/*if(grayscale_close==1)
{
	if((ratio_length1==16)||(Signif_ratio<1)&&(Density_ratio<=2)&&(length_ratio<2))
 {
prior=4;
 }
}*/

if(((hue_close<0)&&(grayscale_close<0))||(hue_close<-1))
{
prior=15;
goto L;
}
else
{
if((hue_close<0)||(grayscale_close<-1))
{
prior=16;
goto L;
}
if(grayscale_close<0)
{
prior=17;
goto L;
}
}
prior=18;
	 goto L;
}//e2
}//s<=1
if((bunch_mean_saturation<=2)&&(neighbor_bunch_mean_saturation<=2))
{//s<=2//21.05.13
	if(maximum_zone_gray<=1)
			{//last_cor14.04.14
				if(grayscale_close>0)
				{
					grayscale_close=1;
				}
				if(!difference_hue_zone)
				{
					hue_close=1;
				}
			}
	if((saturation_difference==0)||((bunch_mean_saturation>=1)&&(neighbor_bunch_mean_saturation>=1)&&
		(saturation_difference<=1)))
	{
    saturation_close=1;
	}
	else
	{
		if(saturation_difference<=1)
	{
    saturation_close=2;
	}
		else
		{//last_cor28.10.13
			if(saturation_difference>=imp_sat_var)
			{
				if((zone_gray>=2)&&(zone_gray_bunch>=2))
				   {//last_cor06.04.15
				saturation_close=-1;
				   }
			}
			else
			{
			saturation_close=3;
			}
		}//last_cor28.10.13
	}
	if(hue_close==1)
	{//last_cor28.05.13
		if(saturation_close==1)
		{
			if((grayscale_close>=1)&&(grayscale_close<=2))
			{
            prior=1;
            goto L;
			}
			else
			{//last_cor03.06.13egc12
				if(grayscale_close>=0)
				{//last_cor03.07.13gc>=0
					if(gray_difference<possible_gray_variation)
					{
					prior=1;
					grayscale_close=2;
                    goto L;
					}
					else
					{//egc
if(gray_difference<(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
						   {
							   if(gray_difference<=(possible_gray_variation+1))
							   {
								 prior=2;
								 grayscale_close=3;
                                 goto L;
							   }
							   else
							   {
								 prior=3;
								 grayscale_close=4;
                                 goto L;
							   }

                           }
					}//egc
				}//gc>=0
			}//egc12

		}

	}
if((hue_close>0)&&(saturation_close>0))
	{//last_cor28.10.13
	if((grayscale_close>0)&&(hue_difference<=6)&&(difference_hue_zone<=1)&&(gray_difference<possible_gray_variation))
	{
		if(grayscale_close==2)
		{
         prior=3;
        goto L;
		}
		else
		{
		prior=4;
        goto L;
		}
	}
	}//last_cor28.10.13

if((saturation_close<0))
	{//last_cor28.10.13
if((difference_hue_zone>=1)&&(hue_difference>=4)&&((zone_gray>=3)||(zone_gray_bunch>=3)))
{
	if((grayscale_close>2)||(grayscale_close==0))
	{
		if(gray_ntersection==3)
		{
			prior=6;
            goto L;
		}
		else
		{
			if(gray_ntersection==2)
		{
			prior=7;
            goto L;
		}
		}
	}
	else
	{
		if(grayscale_close<0)
		{
			prior=5;
            goto L;
		}
	}
}
	}//last_cor28.10.13
	if(grayscale_close<0)
	{//lacor
		if(grayscale_close<=-2)
		{
			if(2*gray_difference>3*impossible_gray_variation)
			{
			prior=5;
            goto L;
			}
			else
			{
			prior=6;
            goto L;
			}
		}
		else
		{
			if(gray_difference>impossible_gray_variation)
			{
			prior=6;
            goto L;
			}
			else
			{
			prior=7;
            goto L;
			}
		}
	}//lacor
	if((grayscale_close>=1)&&(grayscale_close<=2))
	{//grcl1last_cor12.06.13
		if((zone_gray<=1)||(zone_gray_bunch<=1))//last_cor 8.04.13
	{//zg1g
		if((grayscale_close==1)&&(saturation_difference<=1))
		{//last_cor12.06.13
			if((hue_close>0)||(hue_difference<=4))
			{
            prior=1;
	        goto L;
			}
			else
			{
				if(difference_hue_zone<=1)
				{
					prior=2;
	               goto L;
				}
				else
				{
					if(difference_hue_zone<=2)
				{
					prior=3;
	               goto L;
				}
				}
			}
		}//last_cor12.06.13
		else
		{//last_cor
			if((grayscale_close>0)&&(grayscale_close<=2)&&(hue_close>0)&&(saturation_close>0))
			{
				if(saturation_difference<=1)
			{
			prior=1;
	        goto L;
			}
			else
			{
			prior=2;
	        goto L;
			}
			}
		}
	}//zg1g
	else
	{
if((zone_gray<=2)||(zone_gray_bunch<=2))//last_cor 21.04.13
	{//zg2
		if((grayscale_close==1)&&(difference_hue_zone<=1))
		{
if(union_gray_length<possible_gray_variation)
			{
			if(saturation_difference<=1)
			{
			prior=2;
	        goto L;
			}
			else
			{
			prior=3;
	        goto L;
			}
            }
             else
            {
			if(saturation_difference<=1)
			{
			prior=3;
	        goto L;
			}
			else
			{
			prior=4;
	        goto L;
			}
            }
		}
	}//zg2
else
{//last_cor 10.06.13
	if(((zone_gray<=4)||(zone_gray_bunch<=4))&&(saturation_close==1))
	{//zg4

		if(hue_close<=0)
		{//hc
			if((difference_hue_zone<=1)&&(((zone_hue>2)&&(zone_hue<5))||
   ((zone_hue_bunch>2)&&(zone_hue_bunch<5))))
			{//dhz//last_cor11.04.14
				if((hue_difference<=4)||((!difference_hue_zone)&&(hue_difference<=6)))
				{
				prior=2;
				hue_close=2;
				goto L;
				}
				else
				{//ehd
if((length_ratio<=2)||(length_ratio>=14))
		{//lr
					if(hue_difference<=8)
				    {
					prior=3;
				    hue_close=3;
				    goto L;
					}
					else
					{
					prior=4;
				    hue_close=-1;
				    goto L;
					}
		}//lr
				}//ehd
			}//dhz
		}//hc

	}//zg4
}//last_cor 10.06.13
	}

		if((hue_close==1)&&(grayscale_close==1)&&(saturation_close>0))
		{//last_cor28.11.14
		prior=1;
        goto L;
		}
		else
		{
			if((hue_close>=2)&&(hue_close<=3)&&(grayscale_close==1)&&(saturation_close>0))
			{//last_cor28.11.14
				prior=2;
                goto L;
			}
			else
			{
				if(difference_hue_zone==0)
				{//dhz0
					if(hue_difference<=4)
					{//dhz4
						if((saturation_close>0)&&(grayscale_close>0))
						{//last_cor28.11.14
					hue_close=2;//last_cor18.01.13
					prior=2;
                    goto L;
						}
					}//dhz4
					else
					{//edhz4
						if(hue_difference<=6)
					    {//dhz6
					    if((zone_gray>=6)&&(zone_gray_bunch>=6)&&(saturation_close==1)&&(grayscale_close==1)&&
   (gray_ntersection<=1)&&(3*gray_difference<=possible_gray_variation))
{//last_cor18.06.14
	hue_close=3;//last_cor18.01.13
prior=2;
goto L;
}
/*else
{
hue_close=3;//last_cor18.01.13
prior=3;
goto L;
}*/
					    }//dhz6
					}//edhz4
				}//dhz0
				/*else
				{//edhz0
					if(hue_difference<=7)
					{
						if(difference_hue_zone<=1)
						{
					hue_close=4;//last_cor18.01.13
					prior=3;
                    goto L;
						}
						else
						{
						hue_close=5;//last_cor18.01.13
						prior=4;
                        goto L;
						}
					}

				}//edhz0*/
			}//ep2
		}//ep1
	}
if((!gray_ntersection)&&(gray_difference<=2)&&(2*gray_difference<possible_gray_variation)&&
		(difference_hue_zone<=1)&&(zone_gray<=4)&&(zone_gray_bunch<=4))
	{//last_cor16.02.13
		if((hue_close>0)&&(saturation_close>0))
		{//last_cor11.03.1506.04.15
		prior=2;
        goto L;
		}
	}
if((grayscale_close>0)&&(hue_difference<=6)&&(difference_hue_zone<=1)&&(gray_difference<possible_gray_variation))
	{
		if((hue_close>0)&&(saturation_close>0))
		{//last_cor11.03.1506.04.15
		if(grayscale_close==2)
		{
         prior=3;
        goto L;
		}
		else
		{
		prior=4;
        goto L;
		}
		}
	}
if(grayscale_close<0)
	{
		if((hue_close<0)||((hue_close==0)&&(difference_hue_zone>=1)&&(hue_difference>=3)))
		{
			prior=5;
            goto L;
		}
		else
		{
		prior=6;
        goto L;
		}
	}
}//s<=2
if(saturation_difference>2*imp_sat_var)
	                     {//last_cor16.07.13
		                 saturation_close=-2;
	                     }
	                     else
	                     {
               if(saturation_difference>imp_sat_var)
	            {
		         saturation_close=-1;
                 }
	             else
	             {
		         if(saturation_difference<=poss_sat_variation)
		         {
			      if(saturation_difference<=std::max(poss_sat_variation/2,1))
					 {//last_cor17.09.13&01.09.14
			      saturation_close=1;
				     }
					 else
					 {
					 saturation_close=2;
					 }
		          }
	              }
	                        }//last_cor16.07.13
if(hue_close==1)
{//last_cor31.08.15
	if(grayscale_close==1)
{//last_cor31.08.15
if(saturation_close>0)
{
	prior=1;
	goto L;
}
else
{
	if(saturation_difference<imp_sat_var)
	{
		if(saturation_difference<=2)
		{
			if(minimum_zone_gray>=5)
			{
				saturation_close=2;
				prior=1;
				goto L;
			}
		}
	}
}
}//last_cor31.08.15
}//last_cor31.08.15
if((zone_gray<=3)&&(zone_gray_bunch<=3))
		{//last_cor11.04.14
			if(maximum_zone_gray<=1)
			{//last_cor14.04.14
				/*if(grayscale_close>0)
				{
					grayscale_close=1;
				}*/
				if(!difference_hue_zone)
				{
					hue_close=1;
				}
				else
				{//last_cor01.09.14
					if((grayscale_close==1)&&(saturation_close==1))
					{//gcsc1
if(difference_hue_zone<=1)
{//dhz1
	if(std::max(zone_hue_bunch,zone_hue)==4)
	{//std::max
		if(hue_difference<=5)
		{
			hue_close=1;
			prior=1;
			goto L;
		}
		else
		{//ehd
			if(hue_difference<=8)
		    {
			hue_close=2;
			prior=2;
			goto L;
			}
		}//ehd
	}//std::max
}//dhz1
					}//gcsc1

				}//last_cor01.09.14
				if(min_saturation>6)
				{//last_cor21.04.14
					if(minimum_zone_gray>0)
					{
						if(difference_hue_zone>=2)
						{
							hue_close=-1;
							//prior=-1;
							//goto L;
						}
					}

				}
			}//last_cor14.04.14
					   if((grayscale_close==1)&&(saturation_close==1))
		{//last_cor11.04.14
			if((zone_gray<=3)&&(zone_gray_bunch<=3))
			{
if(std::min(zone_gray,zone_gray_bunch)<=1)
{//last_cor28.11.14
	if(!difference_hue_zone)
	{
		if(max_saturation<=4)
		{
			if(hue_difference<=3)
						{
                        hue_close=1;
						prior=1;
						goto L;
						}
			else
			{
				if(hue_difference<=6)
						{
                        hue_close=2;
						prior=1;
						goto L;
						}
			}

		}
	}
}//last_cor28.11.14
				if(!hue_close)
				{
					if(((zone_hue_bunch==3)||(zone_hue==3))&&(difference_hue_zone<=1))
					{
						if(hue_difference<=3)
						{
                        hue_close=1;
						prior=1;
						goto L;
						}
						else
						{
							if((difference_hue_zone==1)&&((zone_hue_bunch==2)||(zone_hue==2)))
					{//last_cor18.02.15
						if((bunch_mean_hue<=5)||(neighbor_bunch_mean_hue<=5))
						{
                          if(hue_difference>=5)
						    {
                             hue_close=-1;
						     prior=6;
						     goto L;
						    }
						}
					}
					else
					{
							if(hue_difference<=5)
						    {
                             hue_close=2;
						     prior=2;
						     goto L;
						    }
					}
						}
					}
				}
			}

		}//last_cor11.04.14
}//last_cor11.04.14
if((zone_gray<=4)&&(zone_gray_bunch<=4))
		{//last_cor27.03.14
			if(hue_difference<=2)
{//last_cor09.04.14
	hue_close=1;
}
			if(max_saturation<=4)
			{//ms
			if(!hue_close)
			{//hc
if((zone_hue>1)&&(zone_hue<4)&&(zone_hue_bunch>1)&&(zone_hue_bunch<4)&&
   (std::min(neighbor_bunch_mean_hue,bunch_mean_hue)>5))
					{//zh
						if(hue_difference<=2)
						{
						hue_close=1;

						}
						else
						{//e
							if(hue_difference<=3)
						    {
                            hue_close=2;

							}
							else
							{
							if(hue_difference<=4)
						    {
                            hue_close=3;
							}

							}
						}//e
					}//zh
			}//hc
			}//ms
if(grayscale_close==1)
{//gc1
	if((saturation_close>=1)&&(saturation_close<=2))
	{
		if(hue_close==1)
		{
			prior=1;
			goto L;
		}
		else
		{
			if(hue_close==2)
		{
			prior=2;
			goto L;
		}
		}
	}
}//gc1
else
{//egc1
if(grayscale_close==2)
{//gc2
	if((saturation_close>=1)&&(saturation_close<=2))
	{
		if(hue_close==1)
		{
			prior=2;
			goto L;
		}
		else
		{
			if(hue_close==2)
		{
			prior=3;
			goto L;
		}
		}
	}
}//gc2
}//egc1
		}//last_cor27.03.14
if((zone_gray>=1)&&(zone_gray_bunch>=1))
{//last_cor07.03.14

	if((zone_gray>=2)||(zone_gray_bunch>=2))
	{//last_cor15.10.14
		if(min_saturation>=3)
		{
           if(difference_hue_zone>1)
		    {
            hue_close=-3;
		    }
		   else
		   {
			   if(difference_hue_zone==1)
			   {//dhz1
                  if(hue_difference>10)
			      {
					  hue_close=-2;
			      }
				  else
				  {//edhz10
					  if(hue_difference>6)
			          {
					  hue_close=-1;
			          }
					  else
					  {//last_cor12.02.15
						  if(((zone_hue_bunch>=1)&&(zone_hue_bunch<=2))
							  ||((zone_hue>=1)&&(zone_hue<=2)))
						  {
							if(min_saturation>=4)
							{
                              if(hue_difference>=4)
			                  {
					          hue_close=-1;
			                  }
						    }
						  }
					  }//last_cor12.02.15
				  }//edhz10
			   }//dhz1
		   }//edhz>1
		}//ms>=3
	}//last_cor15.10.14
	if(((zone_gray>=2)&&((neighbor_bunch_mean_saturation>5)))||
		((zone_gray_bunch>=2)&&(bunch_mean_saturation>5)))
	{//last_cor26.03.14
		if(difference_hue_zone>1)
		{
			hue_close=-3;
		}
		else
		{
			if((hue_difference>5)&&(difference_hue_zone==1))
			{
				hue_close=-2;
			}
		}
	}
}
if((!zone_gray)||(!zone_gray_bunch))
{//!zg
	if(grayscale_close==1)
	{//gc1
		if(!difference_hue_zone)
		{
			hue_close=1;
		}
		else
		{//e
			if((difference_hue_zone<=1)&&(hue_difference<=6))
			{//last_cor22.04.14
				hue_close=2;
		    }
			else
			{
				if((difference_hue_zone<=1)&&(hue_difference<=8))
		        {
			    hue_close=3;
		        }
			}
		}//e
    //prior=1;
		//goto L;
	}//zc1
	else
	{//egs
		if(grayscale_close==2)
	{//gc2
		if(!difference_hue_zone)
		{
			hue_close=1;
		}
		else
		{
			if((difference_hue_zone<=1)&&(hue_difference<=4))
			{
				hue_close=2;
		    }
			else
			{
				if((difference_hue_zone<=1)&&(hue_difference<=6))
		        {
			    hue_close=3;
		        }
			}
		}
		/*if((difference_hue_zone<=1)||(hue_difference<=3))
		{
    prior=1;
		}
		else
		{
			prior=2;
		}
		goto L;*/
	}//gc2
	}//egs
}//!zg
if((zone_gray<=1)||(zone_gray_bunch<=1))
	    {//zgb1-16.04.13
			if(std::max(zone_gray,zone_gray_bunch)>=2)
			{//last_cor27.10.14
				if(saturation_close<0)
				{//sc<0
					if(!hue_close)
					{//hc0
if(!difference_hue_zone)
		{
			if(hue_difference<=5)
			{
			hue_close=1;
			}
		}
		else
		{//edhz0
            if(hue_difference<=3)
		    {//last_cor27.10.14
			hue_close=2;
			}
		      else
		      {//edhz5
			      if(hue_difference<=5)
		          {//last_cor27.10.14
			       hue_close=3;
			       }
		      }//edhz5
		  }//edhz0
					}//hc0
					prior=5;
					goto L;
				}//sc<0
			}//last_cor27.10.14
			if((grayscale_close>=1)&&(grayscale_close<=2))
			{//gsc12
if((grayscale_close==1)&&(saturation_close==1))
{//last_cor29.09.14
	if(!hue_close)
	{//hc0
		if(!difference_hue_zone)
		{
			if(hue_difference<=5)
			{
			hue_close=1;
			prior=1;
			goto L;
			}
		}
		else
		{//edhz0
            if(hue_difference<=3)
		    {//last_cor27.10.14
			hue_close=2;
			prior=2;
			goto L;
			}
		      else
		      {//edhz5
			      if(hue_difference<=5)
		          {//last_cor27.10.14
			       hue_close=3;
			       prior=2;
			       goto L;
			       }
		      }//edhz5
		  }//edhz0
		}//hc0
	}//last_cor29.09.14
//}

				if(!saturation_close)
				{//last_cor12.06.13
	if(saturation_difference<=poss_sat_variation)
	 {//last_cor11.06.13
	saturation_close=1;
	}
	else
	{
		if(saturation_difference<imp_sat_var)
	    {
	saturation_close=2;
		}
		else
		{
			if((zone_gray>1)||(zone_gray_bunch>1))
			{//last_cor11.09.13
        saturation_close=-1;
			}
		}
	}
			}//last_cor12.06.13

				if(hue_close==1)
				{
					if((saturation_close>0)||((zone_gray<=1)&&(zone_gray_bunch<=1)))
				    {//last_cor11.09.13
					prior=1;
					goto L;
					}
				}
				else
				{//ehc1
					if(hue_close>0)
					{
						if(saturation_close>0)
						{//last_cor27.10.14
						prior=2;
					    goto L;
						}
					}
					else
					{//ehc0
		if(!difference_hue_zone)
		{
			if((hue_difference<=6)&&(saturation_close>0))
			{//last_cor27.10.14
			hue_close=2;
			prior=2;
			goto L;
			}
		}
		else
		{//edhz
			if((hue_difference<=8)&&(min_saturation<=2))
			{//last_cor03.08.13
				hue_close=2;
if((saturation_close>=1)&&(saturation_close<=2))
				{
					prior=2;
					goto L;
                }
		    }
			else
			{//ehc2
				if((difference_hue_zone<=1)&&((min_saturation<=2)))
		        {//dhz1
			    hue_close=3;

				if((saturation_close>=1)&&(saturation_close<=2)&&(grayscale_close==1))
				{//last_cor11.06.13
					prior=2;
					goto L;
				}//last_cor11.06.13
		        }//dhz1
			}//ehc2
		}//edhz
					}//ehc0
				}//ehc1
				if((saturation_close==1)&&(grayscale_close==1))
				{
					if(hue_close>0)
					{
					prior=2;
					goto L;
					}

				}
			} //gsc12
        }//zgb1-16.04.13
else
{
if((zone_gray<=3)||(zone_gray_bunch<=3))
	    {//zgb-18.04.13
if((saturation_difference<imp_sat_var)&&(saturation_difference<=2))
{
	if((saturation_difference<=poss_sat_variation)||(saturation_difference<=1))
	{//last_cor27.05.13and 25.03.14
	saturation_close=1;
	}
	else
	{
	saturation_close=2;
	}
if(saturation_close==1)
	{//last_cor25.03.14
		if((grayscale_close>=2)&&(gray_difference<=possible_gray_variation))
		{
        if(!hue_close)
		{//hc0
			if(hue_difference<=3)
			{//hd3
				if(!difference_hue_zone)
				{
                hue_close=1;
                prior=2;
			    goto L;
				}
				if(difference_hue_zone==1)
				{
					if((zone_hue>1)&&(zone_hue<4)&&(zone_hue_bunch>1)&&(zone_hue_bunch<4))
					{
						hue_close=2;
                        prior=2;
			             goto L;
					}
					else
					{
                hue_close=3;
                prior=3;
			    goto L;
					}
				}

			}//hd3
			else
			{//26.03.14
if((zone_hue>1)&&(zone_hue<4)&&(zone_hue_bunch>1)&&(zone_hue_bunch<4))
					{
if(hue_difference<=4)
			{//hd
            hue_close=2;
            }
                    }
			}
		}//hc0
		}
	}//last_cor25.03.14
if((grayscale_close==1)||(2*gray_difference<=possible_gray_variation))
	{//last_cor28.05.13
		             if((saturation_close>=1)&&(saturation_close<=2))
		              {//last_cor12.02.15
		if(hue_close==1)
		{
			prior=1;
			goto L;
		}
		else
		{//ehc1
			if(((zone_hue!=2)&&(zone_hue!=1))||(zone_hue_bunch!=2)&&(zone_hue_bunch!=1))
			{//last_cor15.07.13_17.09.13
			if(hue_difference<=3)
			{

				if(!hue_close)
				{
				prior=2;
				hue_close=2;
				goto L;
				}
			}

		}//last_cor15.07.13_17.09.13

		}//ehc1
		             }//last_cor12.02.15
	}//last_cor28.05.13
}
        }
}
if(saturation_difference>poss_sat_variation)
{//last cor
	    if((zone_gray>=2)&&(zone_gray_bunch>=2))
	    {//zgb
	if((saturation_difference>=imp_sat_var)&&(Intersection_saturation>=2)&&(int_length>=2*StripWidth))
	{//last_cor14.05.14
		saturation_close=-3;
		prior=6;
		goto L;
	}
	else
	{
		if(saturation_difference>(poss_sat_variation+(imp_sat_var-poss_sat_variation)/2)&&
(Intersection_saturation>=2)&&(int_length>=2*StripWidth))
	    {//last_cor14.05.14
		saturation_close=-2;
		}
	}
	    }//zgb
}//last cor
if(((saturation_distance1<=possible_sat_variation)&&(saturation_distance<=possible_sat_variation1))&&
		(((bunch_mean_saturation>=left_sat)&&(bunch_mean_saturation<=right_sat))||(abs_satur<=1)))
	 {//s
     saturation_close=1;
	 }//s
else
{//e=1
if((bunch_mean_saturation>=8)&&(neighbor_bunch_mean_saturation>=8)&&(zone_gray_bunch<=2)&&
		(zone_gray<=2))
	{
		saturation_close=2;
	}
	else
	{
    if(((saturation_distance1<=possible_sat_variation)&&(saturation_distance<=possible_sat_variation1))||
		((bunch_mean_saturation>=left_sat)&&(bunch_mean_saturation<=right_sat)&&
		(neighbor_bunch_mean_saturation>=left_sat_bunch)&&
		(neighbor_bunch_mean_saturation<=right_sat_bunch)))
     {//last_cor10.07.13
		 saturation_close=2;
     }
     else
    {
		if(((saturation_distance1<=possible_sat_variation+1)&&(saturation_distance<=possible_sat_variation1+1))&&
		((bunch_mean_saturation>=left_sat)&&(bunch_mean_saturation<=right_sat)
		&&(neighbor_bunch_mean_saturation>=left_sat_bunch)&&
		(neighbor_bunch_mean_saturation<=right_sat_bunch)))
     {//last_cor10.07.13
		 saturation_close=3;
     }
		else
		{
	if((saturation_distance1>=impossible_sat_variation)||(saturation_distance>=impossible_sat_variation1))
	{
if((Intersection_saturation>=2)&&(int_length>=2*StripWidth))
		{//last_cor14.05.14
	saturation_close=-1;
        }
	}
		}
    }
}
}//e=1

if(zone_gray<=1)
	{//zg1
		if((hue_close==1)&&(saturation_close==1)&&(grayscale_close>=0))
		{//except for grayscalelast_cor03.04.15
		if(zone_hue_bunch==3)
		{//green
			if(zone_gray_bunch<=2)
						 {//last_cor03.04.15
					 if(gray_ntersection<2)
					 {
							 if(zone_gray_bunch<=1)
							 {
								 if(grayscale_close>0)
								 {
							 prior=1;
								 }
								 else
								 {
									 prior=3;
								 }
							 }
							 else
							 {
								 if(grayscale_close>0)
								 {
						         prior=3;
								 }
							 }
					 }//last_cor03.04.15

						 goto L;
						 }
		}//green
		}//except for grayscale
		         if(!hue_close)
				 {//hue indefinite
if(((!difference_hue_zone)&&(hue_difference<=2))||((hue_difference<=3)&&(node_vicinity)))
				 {//closeness correction!!!!

					 hue_close=1;

					 if(!saturation_close)
					 {
					 if(((saturation_distance1<=possible_sat_variation+1)&&(saturation_distance<=possible_sat_variation1+1))||
		((bunch_mean_saturation>=left_sat)&&(bunch_mean_saturation<=right_sat)))
     {
		 saturation_close=2;
     }
					 }
					 if(!grayscale_close)
					 {
						 if(zone_gray_bunch<=2)
						 {
							 grayscale_close=3;
						 }
					 }
				 }//closeness correction

		if((((!difference_hue_zone)&&(hue_difference<=8))||((hue_difference<=6)&&(difference_hue_zone<=1))||
			((left_hue<=corrected_bunch_mean_hue)&&(corrected_bunch_mean_hue<=right_hue))))
		{//h
			hue_close=2;
		}//h
		else
{//e0
    if((hue_difference<=6)&&(difference_hue_zone==1))
    {
	hue_close=3;
    }
	else
	{
		if((difference_hue_zone>=2)||((hue_difference>=8)&&(difference_hue_zone==1)))
		{
			hue_close=-1;
		}
		else
		{
			if((difference_hue_zone>2)||(hue_difference>12))
			{
				hue_close=-2;
			}
		}
	}
}//e0

					 }//hue indefinite






if((grayscale_close>=1)&&(grayscale_close<=2)&&(saturation_close==1)&&(hue_close==1))
	   {//gh
	 prior=1;
	 goto L;
       }//gh
       else
      {//e1
		  if((hue_close>=1)&&(hue_close<=2)&&(grayscale_close>=1)&&(saturation_close>=1)&&(grayscale_close<3))
		  {
		  prior=2;
		  goto L;
	      }

		   else
		   {
if((((hue_close==1)&&(grayscale_close==3))||((hue_close==3)&&(grayscale_close==1)))&&(saturation_close>=1))
		  {
		  prior=3;
		  goto L;
	      }
  else
  {
if((hue_close>=2)&&(grayscale_close>1)&&(saturation_close>=1))
{
	if(grayscale_close==1)
	{
		prior=2;
		goto L;
	}
	/*else
	{
if((ratio_length1==16)||(Signif_ratio<=2)&&(Density_ratio<4)&&(length_ratio<=6))
 {//incl1
prior=2;
 }//incl1
	}*/
}
else
{
     if((hue_close>0)&&(hue_close<=2)&&(grayscale_close>=1)&&(grayscale_close<=2)&&(difference_gray_zone<=1))
	 {//last_cor20.05.15

		 /*if((ratio_length1==16)||(Signif_ratio<=2)&&(Density_ratio<3)&&(length_ratio<=6))
         {//incl1
         prior=2;
         }//incl1*/
		 if((saturation_close>=0)&&(grayscale_close==1)&&((!gray_ntersection)||(ratio_length1_gray==16)))
		 {
			 prior=2;
			 goto L;
		 }
	 }
		 else
		 {
			 if((hue_close==1)&&(grayscale_close>=1)&&(grayscale_close<=2)&&(difference_gray_zone<=1))
			 {
           if((ratio_length1>=14)&&((ratio_length>=14))&&(Density_ratio<10)&&(Density_ratio>6)
			   &&(!gray_ntersection))
		   {
		    prior=2;
			goto L;
		   }
			 }

		 }

}

   }
		  }
      }//e1


	 if((hue_close<-1)||((hue_close<0)&&(grayscale_close<0))||
		 ((saturation_close<0)&&(grayscale_close<0))||((hue_close<0)&&(saturation_close<0)))
{
prior=15;
goto L;
}
else
{
if((hue_close<0)||(saturation_close<0)||(grayscale_close<-1))
{
prior=16;
goto L;
}
if(grayscale_close<0)
{
prior=17;
goto L;
}
}
prior=18;
	 goto L;
	}//zg1
	else
	{//1
		if((zone_gray<=2)&&(difference_gray_zone<=1))
	    {//zg2
			if((hue_close==1)&&(saturation_close==1)&&((!grayscale_close)||(grayscale_close==3)))
		{//hc1sc1gc0
		if((zone_hue_bunch==3)&&(!difference_hue_zone))
		{//zhb3

							 if((gray_difference<=possible_gray_variation))
							 {
							 prior=1;
							 grayscale_close=2;
							 goto L;
							 }
							 else
							 {//e<=2
								 if(grayscale_close==3)
								 {
						         prior=2;
								 goto L;
								 }
							 }//e<=2

		}//zhb3
		}//hc1sc1gc0
			if(!hue_close)
				 {//hue indefinite
if(((!difference_hue_zone)&&(hue_difference<=4))||(hue_difference<=3)||
			((left_hue<=corrected_bunch_mean_hue)&&(corrected_bunch_mean_hue<=right_hue)))
         {//h2
			if(((zone_hue_bunch!=2)&&(zone_hue!=2))||((zone_hue_bunch==2)&&(zone_hue!=1))||
((zone_hue_bunch!=1)&&(zone_hue==2)))
		{//hue_cond_zone21_last_cor16.07.13
			hue_close=2;
		}
			 else
			 {
				 if((ratio_length_hue>7)&&(ratio_length1_hue>7)&&(hue_difference<=2))
				 {
                 hue_close=2;
				 }
			 }
		 }//h2
else
{//e9

if((hue_difference>=6)||((hue_difference>=5)&&(difference_hue_zone>0)))
       {
	   hue_close=-1;
	   }
        else//e11
		{
			if((difference_hue_zone>2)||(hue_difference>12))
			{
				hue_close=-2;
			}
		}//e11

}//e9

			}//hue indefinite

if(((!difference_hue_zone)&&(hue_difference<=2))||
   ((hue_difference<=2)&&(node_vicinity||node_vicinity_neighbor)))
				 {//characteristics correction
					 hue_close=1;
					 if(!saturation_close)
					 {
					 if(((saturation_distance1<=possible_sat_variation+1)&&(saturation_distance<=possible_sat_variation1+1))||
		((bunch_mean_saturation>=left_sat)&&(bunch_mean_saturation<=right_sat)))
     {
		 saturation_close=2;
     }
					 }
					 if(!grayscale_close)
					 {
						 if(zone_gray_bunch<=2)
						 {
							 grayscale_close=3;
						 }
						 else
						 {
if((hue_difference<=1)&&(max_satur_differ<=1)
   &&(gray_difference<(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/4)))
							 {
                             grayscale_close=2;
							 }
						 }
					 }

				 }//characteristics correction


if((grayscale_close>=1)&&(grayscale_close<=2)&&(saturation_close==1)&&(hue_close==1))
	   {//gh
	 prior=1;
	 goto L;
       }//gh
       else
      {//e2
if((hue_close>=1)&&(hue_close<=2)&&(grayscale_close>=1)&&(saturation_close>=1)&&(grayscale_close<3))
{
	 prior=2;
	 goto L;
}
else
{
	if((((hue_close==1)&&(grayscale_close==3))||((hue_close==3)&&(grayscale_close==1)))&&(saturation_close>=1))
		  {
		  prior=3;
		  goto L;
	      }
	else
	{
if((ratio_length1<13)&&(Signif_ratio<=2)&&((Density_ratio<6)||(length_ratio<6)))
	   {
       if((grayscale_close>0)&&(grayscale_close<=2)
		   &&(saturation_close>=0)&&(hue_close>0)&&(hue_close<2))
	 {
	 prior=4;
	 goto L;
	 }
       }
    }
}
     }//e2



       if((prior>0)&&(prior<=4))
	 {
       goto L;
	 }
	 if((hue_close<-1)||((hue_close<0)&&(grayscale_close<0))||
		 ((saturation_close<0)&&(grayscale_close<0))||((hue_close<0)&&(saturation_close<0)))
{
prior=15;
goto L;
}
else
{
if((hue_close<0)||(saturation_close<0)||(grayscale_close<-1))
{
prior=16;
goto L;
}
if(grayscale_close<0)
{
prior=17;
goto L;
}
}
prior=18;
	 goto L;
		}//zg2
		else
		{//zg>
		if(!hue_close)
				 {//hue indefinite
   if((hue_difference<=hue_bound))
   {//h3
	hue_close=2;
   }//h3
   else
   {

if(((!difference_hue_zone)&&(hue_difference<=4))||(hue_difference<=3))

{//!dhz
	if(zone_gray<=3)
		  {//10.04.13
               if((grayscale_close==1)&&(saturation_close==1))
			   {
				   hue_close=2;
			   }
               else
              {
				  if((grayscale_close==1)&&(saturation_close==2))
				  {
					  hue_close=3;
				  }
              }
	      }//10.04.13
	else
	{
	   if((Signif_ratio<=2)&&(length_ratio<=2)&&(Density_ratio<5))
	   {
	   hue_close=3;
	   }
	}

}//!dhz
      else
      {

	   if((hue_difference>=5)||((hue_difference>=4)&&(difference_hue_zone>0)))
       {//hddz
		   if(zone_gray>3)
		  {//9.04.13
	   hue_close=-1;
		   }
		   else
	      {//e1
		      if((difference_hue_zone==0))
		       {//dz0
		     if(hue_difference<=6)
            {
			   if((grayscale_close==1)&&(saturation_close==1))
			   {
				   hue_close=3;
			   }
		    }
			  }//dz0
			  else
			  {//e2
				  if(hue_difference>=6)
                  {
				  hue_close=-1;
				  }
			  }//e2

	      }//e1
	   }//hddz

else
{
if((difference_hue_zone>2)||(hue_difference>12))
{
hue_close=-2;
}
}
      }
   }
		         }//hue indefinite
		if(!saturation_close)
		{//saturation indefinite
			if((bunch_occurrence>0)&&(Density_ratio>=6)&&(Density_ratio<=10))
				   {//bo
if(Intersection_saturation==3)
					   {//s_i=3
  if((saturation_distance1>=possible_sat_variation)||(saturation_distance>=possible_sat_variation1))
						   {
							   if((ratio_length_satur>=(2*poss_sat_variation/3))||
								  (ratio_length_satur>=(imp_sat_var/3) ))
							   {
                                saturation_close=-2;
							   }
							   else
							   {
								   if((ratio_length_satur>=(poss_sat_variation/2)))
							   {
                                saturation_close=-1;
							   }
							   }
						   }

					   }//s_i=3
else
{
    if((Intersection_saturation>=1)&&((Signif_ratio>=15)||(Signif_ratio<=3)))
	{
		if((saturation_distance1>=possible_sat_variation)||(saturation_distance>=possible_sat_variation1))
		{
		if((gray_ntersection>=2)&&(ratio_length1_satur<=6)&&(ratio_length_satur<=6)&&(ratio_length1_gray<=6))
		{
        saturation_close=-1;
		}
		}
	}
}
			       }//bo
		}//saturation indefinite
if(grayscale_close<0)
{//last_cor22.07.13
	if(saturation_close<=0)
	{
		prior=5;
	 goto L;
	}
}
		if((grayscale_close>=1)&&(grayscale_close<=2)&&(saturation_close==1)&&(hue_close==1))
	   {//gh//last_cor050213
	 prior=1;
	 goto L;
       }//gh
       else
      {//e2
		  if((grayscale_close==1)&&(hue_close==1))
           {
			   if((saturation_close>0)&&(abs_satur<=2))
			   {
				   if(saturation_close==2)
				   {
				   prior=1;
				   goto L;
				   }
				   else
				   {
				   prior=2;
				   goto L;
				   }
			   }
			   else//last_cor050213
			   {
               if((saturation_close==2)&&(abs_satur>=3))
			   {
				   prior=1;
				   goto L;
			   }
			   }
           }
		}//e2

      if(((grayscale_close>=1)&&(grayscale_close<=2)&&(saturation_close==1)&&(hue_close==1))||
		  ((grayscale_close==1)&&(saturation_close>=1)&&(saturation_close<=2)&&(hue_close==1))||
		  ((grayscale_close==1)&&(saturation_close>=1)&&(saturation_close<=2)&&
		  (hue_close>=1)&&((hue_close<=2))))
	   {//gh
	 prior=1;
	 goto L;
       }//gh
       else
      {//e2
if(((hue_close==1)&&(grayscale_close>=1)&&(saturation_close>=1)&&
   (saturation_close<=2)&&(grayscale_close<3))||
   ((hue_close==1)&&(saturation_close==1)&&(grayscale_close<3)&&(grayscale_close>0))||
   ((hue_close>=1)&&(hue_close<=2)&&(grayscale_close>=1)&&(saturation_close==1)&&
   (grayscale_close<3))||((hue_close>=1)&&(hue_close<=2)&&(saturation_close>=1)&&
   (saturation_close<=2)&&(grayscale_close>=1)&&(grayscale_close<3)))
{
	 prior=2;
	 goto L;
}
else
{
	if(((hue_close==1)&&(grayscale_close>0)&&(grayscale_close<=3)&&(saturation_close>=1)&&
   (saturation_close<=2))||
  ((hue_close==3)&&(grayscale_close==1)&&(saturation_close>=1)))
		  {//last cor
			  if((hue_close==1)&&(saturation_close==1)&&(grayscale_close>0)&&
				  (grayscale_close<=2))
			  {///!!!
				  prior=2;
		          goto L;
			  }
			  else
			  {
		  prior=3;
		  goto L;
			  }
	      }
	else
	{
if((ratio_length1>13)&&(ratio_length>13)&&(Density_ratio<10)&&(Density_ratio>6))
	   {
       if((grayscale_close>0)&&(grayscale_close<=2)
		   &&(saturation_close>=0)&&(hue_close>0)&&(hue_close<2))
	 {
	 prior=4;
	 goto L;
	 }
       }
    }
}
if((hue_close==1)&&(saturation_close==1))
{//last cor
	if((grayscale_close>=3)&&(grayscale_close<=4))
	{
		prior=2;
	}
}
if(!grayscale_close)
{
	if((hue_close==1)&&(saturation_close==1))
	{
		if(difference_gray_zone<=1)
		{
		prior=4;
		goto L;
		}
	}
}//last cor
     }//e2
		if((prior>0)&&(prior<=4))
	 {
       goto L;
	 }
	 if((hue_close<-1)||((hue_close<0)&&(grayscale_close<0))||
		 ((saturation_close<0)&&(grayscale_close<0))||((hue_close<0)&&(saturation_close<0)))
{
prior=25;
goto L;
}
else
{
if((hue_close<0)||(saturation_close<0))
{
prior=26;
goto L;
}
if(grayscale_close<0)
{
	if((hue_close==1)&&(std::min(bunch_mean_saturation,neighbor_bunch_mean_saturation)>=3)&&
		(!saturation_close)&&(difference_gray_zone<=2))
	{
		if((Density_ratio>6)&&(Density_ratio<10))
		{
if(difference_gray_zone<=1)
           {
		   prior=3;
		   goto L;
   		   }
         else
        {
		prior=4;
		goto L;
		}
	}
	if((hue_close==1)&&(saturation_close==1)&&(difference_gray_zone<=1))
	{
		prior=3;
		goto L;
	}
        }
}
}
	 if((!hue_close)&&(!saturation_close))
	 {//lcor
if((Intersection_hue==3)&&(Intersection_saturation==3)&&(zone_gray>2)&&((zone_gray_bunch>2)))
		 {
			 prior=7;
		goto L;
		 }
	 }
	 if(grayscale_close<-1)
	 {//last cor
	 prior=6;
	 goto L;
	 }
prior=28;
	 goto L;
		}//zg>
	}//1

L:
	if((((prior%10)==0)||(prior%10>3))&&(saturation_close>=-1))
{//last_cor15.02.13prcond

	if((hue_close==1)&&((grayscale_close==1)||((grayscale_close==2)
		||(gray_difference<(possible_gray_variation-possible_gray_variation/4)))))
	{//hc1gs1
		if(min_saturation>=10)
		{//ms10
			if(saturation_difference<=(poss_sat_variation+1))
			 {
       prior=2;
			}
			else
			{
				prior=3;
			}
		}//ms10
		else
		{//e10
		 if(min_saturation>=6)
		 {//ms6
			 if(saturation_difference<=(poss_sat_variation+2))
			 {
				 prior=2;
			 }
			 else
			 {//e6
 if(saturation_difference<=(poss_sat_variation+(imp_sat_var-poss_sat_variation)/2))
 {
	 prior=3;
 }
			 }//e6
		 }//ms6
		 else
		 {
			 if(min_saturation>=4)
		 {//ms4
			 if(saturation_difference<=(poss_sat_variation+2))
			 {
				 prior=3;
			 }
			 else
			 {//e7
 if(saturation_difference<=(poss_sat_variation+(imp_sat_var-poss_sat_variation)/2))
 {
	 prior=4;
 }
			 }//e7
		 }//ms4
		 }
		}//e10
		if((((prior%10)>=5)&&((prior%10)<=7))&&(saturation_close>=-1))
		{//lastcor16.09.13
			if(min_saturation>=4)
		    {//ms4
				if(saturation_difference<=(poss_sat_variation+(imp_sat_var-poss_sat_variation)/2))
               {
	           prior=3;
               }
				else
				{//imp_error16.09.13
					if(!saturation_close)
					prior=4;
				}

			}//ms4
		}
	}//hc1gs1
if(((prior%10)>=5)&&((prior%10)<=7))
{
	if(!hue_close)
	{
if((difference_hue_zone>=2)||((hue_difference>=8)&&(difference_hue_zone>=1)))
{
hue_close=-1;
}
	}
}
}//last_cor15.02.13prcond
return(prior);
}

//=====================================================
int

CImageProcess::FindingRightConectedSection(void)
{
int count_sect;
int number_sect;
int num_sect_opposition;
int section_beg;
int section_end;
int section_bunch;
int bunch_old_number;
int count_fiber;
int right_adjacent;
int right_contrast;
int count_similar;
int number_of_bunches;
int section_bunch_next;
int section_trace_right;
int reduced_section_number;
int beg_int;
int end_int;
int next_beg_int;
int next_end_int;
int prior;
int number_of_connections;
int right_section_number;
int num_fiber_connections;

number_of_connections=0;
prior=1;

number_sect=ColorSection->Number_of_sections_left;
num_sect_opposition=ColorSection->Number_of_sections_right;
for(count_sect=0;count_sect<number_sect;count_sect++)
{//cs
	section_beg=ColorSection->section_fibre_first[count_sect];
    section_end=ColorSection->section_fibre_last[count_sect];
  for(count_fiber=section_beg;count_fiber<=section_end;count_fiber++)
  {
	  number_of_bunches=ColorInt[count_fiber].RefinedNumberOfBunches;
  section_bunch=ColorDescrSect[count_sect].location_of_section[count_fiber];
  if(section_bunch>0)
  {
 section_bunch-=1;
 if((section_bunch>=number_of_bunches)||(section_bunch<0))
 {
	 prior=-2;
	 return(prior);
 }
 bunch_old_number=ColorInt[count_fiber].old_bunch_number[section_bunch];
  }
  else
  {
	  prior=-2;
	  return(prior);
  }

reduced_section_number=-1;
  for(count_similar=0;count_similar<number_of_bunches;count_similar++)
  {
right_contrast=ColorInt[count_fiber].right_continuation[bunch_old_number];
right_adjacent=ColorInt[count_fiber].right_adjacent[bunch_old_number];
beg_int=ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[bunch_old_number];
end_int=ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[bunch_old_number];

if(right_contrast>0)
{//rc>0
	right_contrast-=1;
	right_adjacent-=1;
	right_contrast=right_contrast%10;
	if((right_contrast>=1)&&(right_contrast<=3))
	{//rc13
bunch_old_number=right_adjacent;
if(bunch_old_number<0)
  {
	  prior=-2;
	  return(prior);
  }
	}//rc13
	else
	{
	break;
	}
}
else
{
if(right_contrast<0)
{//rc<0
right_contrast=-right_contrast-1;
right_contrast=right_contrast%10;
section_trace_right=SectionTraceRight[count_fiber*MAX_COL_INT+bunch_old_number];
        if(section_trace_right>0)
		{//str>0
			if(section_trace_right>=1000)
			{
reduced_section_number=section_trace_right%1000;
			}
			else
			{
reduced_section_number=section_trace_right;
			}
			if(reduced_section_number>number_sect)
			{
				reduced_section_number-=1;
				right_section_number=reduced_section_number;
				reduced_section_number=reduced_section_number-number_sect;
			}
			else
			{
				break;
			}

if(reduced_section_number<num_sect_opposition)
{
NumberOfConnectionsLeftRight[count_sect]++;
SectionNeighborsLeftRight[count_sect*(NUM_SECT1/2)+reduced_section_number]++;
num_fiber_connections=
SectionNeighborsLeftRight[count_sect*(NUM_SECT1/2)+reduced_section_number];
if(num_fiber_connections==1)
{
number_of_connections=NumberOfLRConnections[count_sect];
     if(number_of_connections<8)
     {
RightLeftConnections[number_of_connections*NUM_SECT1 + count_sect]=right_section_number;
     NumberOfLRConnections[count_sect]++;
	 }
number_of_connections=NumberOfLRConnections[right_section_number];
       if(number_of_connections<8)
        {
RightLeftConnections[number_of_connections*NUM_SECT1 + right_section_number]=count_sect;
		NumberOfLRConnections[right_section_number]++;
        }
}

}
else
{
	prior=-2;
	return(prior);
}
			break;
		}//str>0
		else
		{
			break;
		}
}//rc<0
else
{
	if(!right_contrast)
	{//rc=0
	 if(end_int==(StripLength-1)||(!right_adjacent))
	  {//end
	section_trace_right=SectionTraceRight[count_fiber*MAX_COL_INT+bunch_old_number];
	if(section_trace_right>0)
		{//str>0
			if(section_trace_right>=1000)
			{
reduced_section_number=section_trace_right%1000;
			}
            else
			{
reduced_section_number=section_trace_right;
			}
			if(reduced_section_number>number_sect)
			{
				reduced_section_number-=1;
				right_section_number=reduced_section_number;
				reduced_section_number=reduced_section_number-number_sect;
			}
			else
			{
				break;
			}

if(reduced_section_number<num_sect_opposition)
{
NumberOfConnectionsLeftRight[count_sect]++;
SectionNeighborsLeftRight[count_sect*(NUM_SECT1/2)+reduced_section_number]++;
num_fiber_connections=
SectionNeighborsLeftRight[count_sect*(NUM_SECT1/2)+reduced_section_number];
if(num_fiber_connections==1)
{
number_of_connections=NumberOfLRConnections[count_sect];
     if(number_of_connections<8)
     {
RightLeftConnections[number_of_connections*NUM_SECT1 + count_sect]=right_section_number;
     NumberOfLRConnections[count_sect]++;
	 }
number_of_connections=NumberOfLRConnections[right_section_number];
       if(number_of_connections<8)
        {
RightLeftConnections[number_of_connections*NUM_SECT1 + right_section_number]=count_sect;
		NumberOfLRConnections[right_section_number]++;
        }
}
}
else
{
	prior=-2;
	return(prior);
}
		}//str>0
	break;
	 }//end
	 else
	 {
		 break;
	 }
	 break;
	}//rc=0
}
}
  }
  }
}//cs
return(prior);
}
//=====================================================
//=====================================================
/*int

CImageProcess::LabelCoordinatesFinding(void) {
	int coun_strip;
	int coun_bunch;
	//int Number_of_bunches;
	int Num_of_labels;
	int label_number;
	int left_neighbor;
	int left_contrast;
	int left_contrast_reduced;
	int right_neighbor;
	int right_contrast;
	int right_contrast_reduced;
	int beg_int;
	int end_int;
	int prior;
	int left_section_number;
	int right_section_number;
	int left_trace_number;
	int left_trace_number_reduced;
	int right_trace_number;
	int right_trace_number_reduced;
	int first_left_section_strip;
	int last_left_section_strip;
	int first_right_section_strip;
	int last_right_section_strip;
	int number_of_sections_left;
	int number_of_sections_right;
	int left_section_length;
	int right_section_length;

	prior = -1;
	Num_of_labels = -1;
	for (coun_strip = 0; coun_strip < NumStrips; coun_strip++) {
		//Number_of_bunches=ColorInt[coun_strip].NumberOfColoredIntervals;
		//for(coun_bunch=0;coun_bunch<Number_of_bunches;NumStrips++)
		//{
		Num_of_labels = ColorInt[coun_strip].NumberOfLabels;
		if (Num_of_labels > 0) {
			if (Num_of_labels == 1) {
				left_section_length = 0;
				right_section_length = 0;
				label_number = ColorInt[coun_strip].Label_bunch_num[0];
				beg_int =
						ColorInt[coun_strip].ColoredIntervalsStructure->BegInterv[label_number];
				end_int =
						ColorInt[coun_strip].ColoredIntervalsStructure->EndInterv[label_number];
				prior = beg_int + (end_int - beg_int) / 2;
				LabelLeftCoordinate=beg_int;
				LabelRightCoordinate=end_int;
				return prior;
				left_trace_number = SectionTraceLeft[coun_strip * MAX_COL_INT
						+ label_number];
				left_neighbor =
						ColorInt[coun_strip].left_adjacent[label_number];
				left_contrast =
						ColorInt[coun_strip].left_continuation[label_number];
				if (left_contrast < 0) {
					left_neighbor = -left_neighbor - 1;
					left_contrast_reduced = -left_contrast - 1;
					left_contrast_reduced = left_contrast_reduced % 10;
				}
				right_neighbor =
						ColorInt[coun_strip].right_adjacent[label_number];
				right_contrast =
						ColorInt[coun_strip].right_continuation[label_number];
				left_trace_number_reduced = left_trace_number % 1000;
				if (left_trace_number_reduced > 0) {
					left_trace_number_reduced -= 1;
				} else {
					goto K;
				}
				number_of_sections_left = ColorSection->Number_of_sections_left;
				number_of_sections_right =
						ColorSection->Number_of_sections_right;
				first_left_section_strip =
						ColorSection->section_fibre_first[left_trace_number_reduced];
				last_left_section_strip =
						ColorSection->section_fibre_last[left_trace_number_reduced];
				left_section_length = last_left_section_strip
						- first_left_section_strip + 1;
				right_trace_number = SectionTraceRight[coun_strip * MAX_COL_INT
						+ label_number];
				right_trace_number_reduced = right_trace_number % 1000;
				if (right_trace_number_reduced > 0) {
					right_trace_number_reduced -= 1;
				} else {
					goto K;
				}
				first_right_section_strip =
						ColorSection->section_fibre_first[right_trace_number_reduced];
				last_right_section_strip =
						ColorSection->section_fibre_last[right_trace_number_reduced];
				right_section_length = last_right_section_strip
						- first_right_section_strip + 1;
				if (right_contrast < 0) {
					right_neighbor = -right_neighbor - 1;
					right_contrast_reduced = -right_contrast - 1;
					right_contrast_reduced = right_contrast_reduced % 10;
				}
				goto K;
			}
		}
		K: ;
	}

	return (prior);
}*/
//=====================================================
//=====================================================
/*int

CImageProcess::LabelFiltering(void)
{
	int coun_strip;
	int coun_strip1;
	int coun_strip2;
	int Num_of_labels;
	int bunch_suitability;
	int central_bunch_number;
	int label_suitability;
	int left_neighbor;
	int left_contrast;
	int left_contrast_reduced;
	int right_neighbor;
	int right_contrast;
	int right_contrast_reduced;
	int beg_int;
	int end_int;
	int int_length;
	int beg_int_next;
	int end_int_next;
	int another_beg_int;
	int another_end_int;
	int another_length;
	int prior;
	//int left_section_number;
	//int right_section_number;
	int left_trace_number;
	int left_trace_number_reduced;
	int right_trace_number;
	int right_trace_number_reduced;
	int first_left_section_strip;
	int last_left_section_strip;
	int first_right_section_strip;
	int last_right_section_strip;
	int number_of_sections_left;
	int number_of_sections_right;
	int left_section_length;
	int right_section_length;
	int central_int_length;
	int next_central_int_length;
	int coun_label;
	//int coun_label1;
	int coun_label2;
	int another_bunch;
	int length_ratio;
	int length_ratio1;
	int left_shift;
	int right_shift;
	int up_down;
	int another_left_contrast;
	int another_right_contrast;
	int min_strip_number;
	int max_strip_number;
	int total_lebel_in_strips_number;
	int total_number_of_labels;
	int label_count;
	int label_count1;
	int label_height;
	int cand_bunch_number;
	int cand_strip_number;
	int cand_bunch_number_next;
	int cand_strip_number_next;
	int bunch_center;
	int next_bunch_center;
	//int length_ratio;
	int left_difference;
	int right_difference;
	int central_difference;
	int new_number;
	int Intersection;
	int indic_length;
	int ratio_length;
	int ratio_length1;
	int last_strip;
	int label_count3;
	int height_lab;
	int number_of_targets;
	int lab_num;
	int hue_central;
	int hue_zone_central;
	int given_central_hue_zone;
	int zone_diff;
	int min_zone;
	int min_zone_number;
	int sat_central;
	int gray_central;
	int center_coordinate;
	int new_lab_find;
	int max_inters;
	int max_inters_number;
	int penalt_label;
	int inter_sum;
	int cost_label;
	int current_cost;
	int mean_label_cost;
	int mean_inter_sum;
	int max_cost;
	int max_cost_number;
	int penalt_label1;
	int	inter_sum1;
	int	cost_label1;
	int label_height1;
	int cand_strip_number1;
	int central_bunch_number1;
	int beg_int1;
	int	end_int1;
	int	int_length1;
	int residual;
	int len_rat;
	int* candidate_strip_number;
	int* candidate_number;
	int* height_label;
	int* last_str_number;
	int* serial_label_number;
	int* new_label_quality;
	int* penalty_label;
	int* intersection_sum;
	int* label_blocked;
	int* label_cost;

	total_lebel_in_strips_number=0;
	Intersection=-1;
	indic_length=-1;
	ratio_length=-1;
	ratio_length1=-1;
	last_strip=-1;
	min_zone=100;
	min_zone_number=-1;
	max_inters=0;
	max_inters_number=-1;
	given_central_hue_zone=1;
	penalt_label=-1;
	center_coordinate=-1;
	inter_sum=-1;
	mean_label_cost=0;
	penalt_label1=-1;
	inter_sum1=-1;
	cost_label1=-1;
	label_height1=-1;
	cand_strip_number1=-1;
	central_bunch_number1=-1;
	beg_int1=-1;
	end_int1=-1;
	int_length1=-1;
	residual=-1;
	len_rat=-1;
	if(HorizontalVertical)
	{
	min_strip_number=DimY;
	}
	else
	{
	min_strip_number=DimX;
	}
	max_strip_number=-1;
	prior=-1;
	up_down=-1;
	Num_of_labels=-1;
	total_number_of_labels=0;
	for(coun_strip1=0;coun_strip1<NumStrips;coun_strip1++)
	     {//cycle_strips1
	 Num_of_labels=ColorInt[coun_strip1].NumberOfLabels;
	 if(Num_of_labels>0)
	 {//nl>0
		 total_lebel_in_strips_number+=Num_of_labels;
	if(coun_strip1<min_strip_number)
		 {
		 min_strip_number=coun_strip1;
		 }
		 max_strip_number=coun_strip1;

	 }//nl>0
	}//cycle_strips1
	if(!total_lebel_in_strips_number)
	 {
		 LabelCenterCoordinate=-1;
		 return(prior);
	 }
	if(total_lebel_in_strips_number>0)
	{//tln>0
		total_number_of_labels=total_lebel_in_strips_number;
		candidate_strip_number=new int [total_lebel_in_strips_number+1];
	    candidate_number=new int [total_lebel_in_strips_number+1];
		height_label=new int [total_lebel_in_strips_number+1];
		serial_label_number=new int [total_lebel_in_strips_number+1];
		new_label_quality=new int [total_lebel_in_strips_number+1];
		last_str_number=new int [total_lebel_in_strips_number+1];
		penalty_label=new int [total_lebel_in_strips_number+1];
		intersection_sum=new int [total_lebel_in_strips_number+1];
		label_blocked=new int [total_lebel_in_strips_number+1];
		label_cost=new int [total_lebel_in_strips_number+1];
		memset(height_label,(int) '\0',(sizeof(int))*(total_lebel_in_strips_number+1));
		memset(candidate_number,(int) '\0',(sizeof(int))*(total_lebel_in_strips_number+1));
		memset(penalty_label,(int) '\0',(sizeof(int))*(total_lebel_in_strips_number+1));
		memset(new_label_quality,(int) '\0',(sizeof(int))*(total_lebel_in_strips_number+1));
	    memset(last_str_number,(int) '\0',(sizeof(int))*(total_lebel_in_strips_number+1));
		memset(intersection_sum,(int) '\0',(sizeof(int))*(total_lebel_in_strips_number+1));
	memset(label_blocked,(int) '\0',(sizeof(int))*(total_lebel_in_strips_number+1));
	memset(label_cost,(int) '\0',(sizeof(int))*(total_lebel_in_strips_number+1));
		new_number=0;
	for(coun_strip2=min_strip_number;coun_strip2<=max_strip_number;coun_strip2++)
	     {//cycle_strips2
	Num_of_labels=ColorInt[coun_strip2].NumberOfLabels;
	 if(Num_of_labels>0)
	 {//nl>0
	 for(coun_label2=0;coun_label2<Num_of_labels;coun_label2++)
	     {//cycle_label1

	central_bunch_number=ColorInt[coun_strip2].Label_bunch_num[coun_label2];
	candidate_strip_number[new_number]=coun_strip2;
	candidate_number[new_number]=central_bunch_number+1;
	new_number++;
	     }//cycle_label1
	 }//nl>0
	     }//cycle_strips2
	last_strip=-1;
	cost_label=0;
	current_cost=0;
	for(label_count=0;label_count<total_lebel_in_strips_number;label_count++)
	  {//lab_c
			 cand_bunch_number=candidate_number[label_count];
			 if(cand_bunch_number>0)
			 {//cn>0
				 cand_bunch_number--;
	cand_strip_number=candidate_strip_number[label_count];
	last_strip=cand_strip_number;
	beg_int=ColorInt[cand_strip_number].ColoredIntervalsStructure->BegInterv[cand_bunch_number];
	end_int=ColorInt[cand_strip_number].ColoredIntervalsStructure->EndInterv[cand_bunch_number];
	bunch_center=beg_int+(end_int-beg_int)/2;
	central_int_length=end_int-beg_int+1;
	label_height=0;
	cost_label=ColorInt[cand_strip_number].center_bunch_suitability[cand_bunch_number];
	for(label_count1=label_count+1;label_count1<total_lebel_in_strips_number;label_count1++)
	     {//lab_c1
	 cand_bunch_number_next=candidate_number[label_count1];
	cand_bunch_number_next--;
	 cand_strip_number_next=candidate_strip_number[label_count1];
	beg_int_next=
	ColorInt[cand_strip_number_next].ColoredIntervalsStructure->BegInterv[cand_bunch_number_next];
	end_int_next=
	ColorInt[cand_strip_number_next].ColoredIntervalsStructure->EndInterv[cand_bunch_number_next];
	next_bunch_center=beg_int_next+(end_int_next-beg_int_next)/2;
	next_central_int_length=end_int_next-beg_int_next+1;
	length_ratio=(16*central_int_length)/(central_int_length+next_central_int_length);
	left_difference=beg_int-beg_int_next;
	right_difference=end_int-end_int_next;
	central_difference=bunch_center-next_bunch_center;
	Intersection=
	ColorInt[cand_strip_number_next].Intersection_measure_ratios(beg_int,end_int,beg_int_next,
	end_int_next,&indic_length,&ratio_length,&ratio_length1);
	if((Intersection>=0)&&(Intersection<=1))
	{//int1
	if((length_ratio>=6)&&(length_ratio<=10))
	{//length

		if(((abs(central_difference)<=StripWidth)||(abs(left_difference)<=StripWidth)
			||(abs(right_difference)<=StripWidth))||
			(!Intersection))
		{
	last_strip=cand_strip_number_next;
	label_height++;
	candidate_number[label_count1]=-10-label_count;
	current_cost=ColorInt[cand_strip_number_next].center_bunch_suitability[cand_bunch_number_next];
	cost_label+=current_cost;
	intersection_sum[label_count]+=length_ratio;
	if(length_ratio==8)
	{
		cost_label+=5;
	}
	else
	{
		if((length_ratio==7)||(length_ratio==9))
	    {
			cost_label+=2;
		}
	}
	total_number_of_labels--;
		}
	}//length
	}//int1
	     }//lab_c1

	height_label[label_count]=label_height+1;
	last_str_number[label_count]=last_strip;
	if(last_strip<=(NumStrips-3)&&(cand_strip_number>3))
	{
		if(2*central_int_length>3*StripWidth)
		{
			if(!label_height)
			{
				cost_label-=5;
			}
		}
		len_rat=central_int_length/StripWidth;
		if(len_rat>=1)
		{
	        if(label_height>=(2*len_rat-2))
			{
				cost_label+=(label_height*3);
			}
		}
	}
	label_cost[label_count]=cost_label;
			 }//cn>0

	  }//lab_c
	number_of_targets=0;
	for(label_count3=0;label_count3<total_lebel_in_strips_number;label_count3++)
	     {//lab_c3
			 if(candidate_number[label_count3]>0)
			 {
			 height_lab=height_label[label_count3];
	if(height_lab>0)
	{
		serial_label_number[number_of_targets]=label_count3;
		number_of_targets++;
	}
			 }

	     }//lab_c3
	if(number_of_targets>0)
	{//nt>0
		max_inters=0;
	max_inters_number=-1;
	for(int label_cou=0;label_cou<number_of_targets;label_cou++)
	     {//lab
		lab_num=serial_label_number[label_cou];
	cand_strip_number=candidate_strip_number[lab_num];
	central_bunch_number=candidate_number[lab_num];
	central_bunch_number--;
	beg_int=ColorInt[cand_strip_number].ColoredIntervalsStructure->BegInterv[central_bunch_number];
	end_int=ColorInt[cand_strip_number].ColoredIntervalsStructure->EndInterv[central_bunch_number];
	int_length=end_int-beg_int+1;
	hue_central=
	ColorInt[cand_strip_number].ColoredIntervalsStructure->AverageHue[central_bunch_number];
	hue_zone_central=hue_zones[hue_central];
	label_height=height_label[lab_num];
	zone_diff=abs(hue_zone_central-given_central_hue_zone);
	if(zone_diff>0)
	{
		penalty_label[label_cou]+=10;
		if(int_length>5*StripWidth)
	    {
			if(label_height==1)
			{
	label_blocked[lab_num]=1;
	//number_of_targets--;
	ColorInt[cand_strip_number].label_segments[central_bunch_number]=0;
			}
	    }
	}

	if(zone_diff<min_zone)
	{
	min_zone=zone_diff;
	min_zone_number=label_cou;
	}
	sat_central=
	ColorInt[cand_strip_number].ColoredIntervalsStructure->AverageSat[central_bunch_number];
	gray_central=
	ColorInt[cand_strip_number].ColoredIntervalsStructure->AverageGray[central_bunch_number];

	last_strip=last_str_number[lab_num];
	if(LabelCenterCoordinate>0)
	{//lc>0
		if((LabelLeftCoordinate>0)&&(LabelRightCoordinate>0))
		{//>>
	Intersection=
	ColorInt[cand_strip_number].Intersection_measure_ratios(beg_int,end_int,LabelLeftCoordinate,
	LabelRightCoordinate,&indic_length,&ratio_length,&ratio_length1);
	//}
	if(Intersection==3)
	{
		new_label_quality[label_cou]=-ratio_length;
	}
	else
	{//e
		if(!Intersection)
		{
			new_label_quality[label_cou]=ratio_length;
		}

		if(ratio_length>max_inters)
		{
	max_inters=ratio_length;
	max_inters_number=label_cou;
		}
	}//e
		}//>>
	}//lc>0
	     }//lab
	mean_label_cost=0;
	mean_inter_sum=0;
	NumberOflabels=number_of_targets;
	 if(number_of_targets==1)
	 {//nt=1
		 cost_label=label_cost[0];
	label_height=height_label[0];
	penalt_label=penalty_label[0];
	inter_sum=intersection_sum[0];

	if(label_height>1)
	{
		//residual=inter_sum%(label_height-1);
		mean_inter_sum=inter_sum/(label_height-1);


	}
		 if(LabelCenterCoordinate<0)
	     {//lc<0
			 if(label_height==1)
			 {
				 if((cost_label>12)&&(!min_zone)&&(!label_blocked[0]))
		         {//condh1
	              center_coordinate=beg_int+(end_int-beg_int)/2;
	NumberOflabels=1;
	LabelCenterCoordinate=center_coordinate;
	LabelLeftCoordinate=beg_int;
	LabelRightCoordinate=end_int;
	LabelCenterHue=hue_central;
	LabelHueZone=hue_zone_central;
	LabelCenterSaturation=sat_central;
	LabelFirstStrip=cand_strip_number;
	LabelLastStrip=last_strip;
	LabelCost=cost_label;
	prior=LabelCenterCoordinate;
	goto N;
				 }//condh1
				 else
				 {
					 center_coordinate=-1;
	NumberOflabels=0;
	LabelCenterCoordinate=center_coordinate;
	goto N;
				 }


			 }
			 else
			 {//ec
		if((cost_label>25)&&(!min_zone)&&(!label_blocked[0]))
		{//cond
			if(((mean_inter_sum>=7)&&(mean_inter_sum<=9))||(label_height==1))
			{//cond1
	center_coordinate=beg_int+(end_int-beg_int)/2;
	NumberOflabels=1;
	LabelCenterCoordinate=center_coordinate;
	LabelLeftCoordinate=beg_int;
	LabelRightCoordinate=end_int;
	LabelCenterHue=hue_central;
	LabelHueZone=hue_zone_central;
	LabelCenterSaturation=sat_central;
	LabelFirstStrip=cand_strip_number;
	LabelLastStrip=last_strip;
	LabelCost=cost_label;
	prior=LabelCenterCoordinate;
	goto N;
			}//cond1
		}//cond
			 }//ec
		 }//lc<0
		 else
		 {//ec>0
	if((cand_strip_number<(NumStrips-3))&&(last_strip>2)&&(beg_int>StripWidth)&&
	   (end_int<(Dimension-StripWidth)))
	{//loccon
		Intersection=
	ColorInt[cand_strip_number].Intersection_measure_ratios(beg_int,end_int,LabelLeftCoordinate,
	LabelRightCoordinate,&indic_length,&ratio_length,&ratio_length1);
		//if((Intersection>2)||((Intersection==3)&&(ratio_length>3*StripWidth)))
		if(Intersection>2)
		{
			NumberOflabels=0;
	LabelCenterCoordinate=-1;
	goto N;
		}
	Intersection=
	ColorInt[cand_strip_number].Intersection_measure_ratios(cand_strip_number,last_strip,
	LabelFirstStrip,LabelLastStrip,&indic_length,&ratio_length,&ratio_length1);
	if((Intersection==3)&&(ratio_length>2*StripWidth))
	{
		NumberOflabels=0;
	    LabelCenterCoordinate=-1;
	    goto N;
		if((mean_label_cost>15)&&(!min_zone)&&(!label_blocked[0]))
		{//cond
			if(((mean_inter_sum>=7)&&(mean_inter_sum<=9))||(label_height==1))
			{//cond1
	center_coordinate=beg_int+(end_int-beg_int)/2;
	NumberOflabels=1;
	LabelCenterCoordinate=center_coordinate;
	LabelLeftCoordinate=beg_int;
	LabelRightCoordinate=end_int;
	LabelCenterHue=hue_central;
	LabelHueZone=hue_zone_central;
	LabelCenterSaturation=sat_central;
	LabelFirstStrip=cand_strip_number;
	LabelLastStrip=last_strip;
	LabelCost=cost_label;
	prior=LabelCenterCoordinate;
	goto N;
			}//cond1
			else
			{
		NumberOflabels=0;
	    LabelCenterCoordinate=-1;
	    goto N;
			}
		}//cond
		else
		{
	    NumberOflabels=0;
	    LabelCenterCoordinate=-1;
	    goto N;
		}
	}

	}//loccon
		 }//ec>0
	 }//nt=1
	     if(number_of_targets>1)
		 {//nt>1
			 max_cost=0;
			 max_cost_number=-1;
	for(int label_cou1=0;label_cou1<number_of_targets;label_cou1++)
	     {//lab1
	lab_num=serial_label_number[label_cou1];
	new_lab_find=new_label_quality[label_cou1];
	if(new_lab_find>=0)
	{//nlf>=0
		penalt_label=penalty_label[label_cou1];
		inter_sum=intersection_sum[lab_num];
		cost_label=label_cost[lab_num];
	label_height=height_label[lab_num];

	mean_label_cost=cost_label;
		if(mean_label_cost>max_cost)
		{
	max_cost=mean_label_cost;
	max_cost_number=label_cou1;
		}
		cand_strip_number=candidate_strip_number[lab_num];
		central_bunch_number=candidate_number[lab_num];
		central_bunch_number--;
		beg_int=ColorInt[cand_strip_number].ColoredIntervalsStructure->BegInterv[central_bunch_number];
		end_int=ColorInt[cand_strip_number].ColoredIntervalsStructure->EndInterv[central_bunch_number];
		int_length=end_int-beg_int+1;
		hue_central=
		ColorInt[cand_strip_number].ColoredIntervalsStructure->AverageHue[central_bunch_number];
		sat_central=
		ColorInt[cand_strip_number].ColoredIntervalsStructure->AverageSat[central_bunch_number];
		gray_central=
		ColorInt[cand_strip_number].ColoredIntervalsStructure->AverageGray[central_bunch_number];
		label_height=height_label[lab_num];
		last_strip=last_str_number[lab_num];
		center_coordinate=beg_int+(end_int-beg_int)/2;
			//break;
	}//nlf>=0
	     }//lab1
	lab_num=serial_label_number[max_cost_number];

	penalt_label=penalty_label[lab_num];
			inter_sum=intersection_sum[lab_num];
			cost_label=label_cost[lab_num];
		    label_height=height_label[lab_num];
		    cand_strip_number=candidate_strip_number[lab_num];
		    central_bunch_number=candidate_number[lab_num];
		    beg_int=ColorInt[cand_strip_number].ColoredIntervalsStructure->BegInterv[central_bunch_number];
			end_int=ColorInt[cand_strip_number].ColoredIntervalsStructure->EndInterv[central_bunch_number];
			int_length=end_int-beg_int+1;
			hue_central=
				ColorInt[cand_strip_number].ColoredIntervalsStructure->AverageHue[central_bunch_number];
				hue_zone_central=hue_zones[hue_central];

				zone_diff=abs(hue_zone_central-given_central_hue_zone);
			if(LabelCenterCoordinate<0)
				     {//lc<0
						 if(label_height==1)
						 {//lh1
							 if((cost_label>12)&&(!zone_diff)&&(!label_blocked[max_cost_number]))
					         {//condh1
				              center_coordinate=beg_int+(end_int-beg_int)/2;
				//NumberOflabels=1;
				LabelCenterCoordinate=center_coordinate;
				LabelLeftCoordinate=beg_int;
				LabelRightCoordinate=end_int;
				LabelCenterHue=hue_central;
				LabelHueZone=hue_zone_central;
				LabelCenterSaturation=sat_central;
				LabelFirstStrip=cand_strip_number;
				LabelLastStrip=last_strip;
				LabelCost=cost_label;
				prior=LabelCenterCoordinate;
				goto N;
							 }//condh1
							 else
							 {
								 center_coordinate=-1;
				NumberOflabels=0;
				LabelCenterCoordinate=center_coordinate;
				goto N;
							 }


						 }//lh1
						 else
						 {//ec
					if((cost_label>25)&&(!zone_diff)&&(!label_blocked[max_cost_number]))
					{//cond
						//if(((mean_inter_sum>=7)&&(mean_inter_sum<=9))||(label_height==1))
						//{//cond1
				center_coordinate=beg_int+(end_int-beg_int)/2;
				NumberOflabels=1;
				LabelCenterCoordinate=center_coordinate;
				LabelLeftCoordinate=beg_int;
				LabelRightCoordinate=end_int;
				LabelCenterHue=hue_central;
				LabelHueZone=hue_zone_central;
				LabelCenterSaturation=sat_central;
				LabelFirstStrip=cand_strip_number;
				LabelLastStrip=last_strip;
				prior=LabelCenterCoordinate;
				goto N;
						//}//cond1
					}//cond
						 }//ec
					 }//lc<0
		 }//nt>1

	}//nt

	if(center_coordinate>0)
	{
		LabelCenterCoordinate=center_coordinate;
		LabelLeftCoordinate=beg_int;
		LabelRightCoordinate=end_int;
	}
	prior=center_coordinate;

	N:
	if(total_lebel_in_strips_number>0)
	{//tln>0
	delete[] candidate_strip_number;
	delete[] candidate_number;
	delete[] height_label;
	delete[] serial_label_number;
	delete[] new_label_quality;
	delete[] last_str_number;
	delete[] penalty_label;
	delete[] intersection_sum;
	delete[] label_blocked;
	delete[] label_cost;
	}
	return(prior);
	}//tnl>0
	else
	{
		return(prior);
	}
for(coun_strip=0;coun_strip<NumStrips;coun_strip++)
     {//cycle_strips
 Num_of_labels=ColorInt[coun_strip].NumberOfLabels;
 if(Num_of_labels>0)
 {//nl>0
for(coun_label=0;coun_label<Num_of_labels;coun_label++)
     {//cycle_label
		 left_section_length=0;
		 right_section_length=0;
central_bunch_number=ColorInt[coun_strip].Label_bunch_num[coun_label];

bunch_suitability=ColorInt[coun_strip].center_bunch_suitability[central_bunch_number];
label_suitability=ColorInt[coun_strip].label_segments[central_bunch_number];
if(label_suitability>0)
{//ls>0
beg_int=ColorInt[coun_strip].ColoredIntervalsStructure->BegInterv[central_bunch_number];
end_int=ColorInt[coun_strip].ColoredIntervalsStructure->EndInterv[central_bunch_number];
central_int_length=end_int-beg_int+1;
left_neighbor=ColorInt[coun_strip].left_adjacent[central_bunch_number];
left_contrast=ColorInt[coun_strip].left_continuation[central_bunch_number];
if(left_contrast<0)
{
left_neighbor=-left_neighbor-1;
left_contrast_reduced=-left_contrast-1;
left_contrast_reduced=left_contrast_reduced%10;
}
right_neighbor=ColorInt[coun_strip].right_adjacent[central_bunch_number];
right_contrast=ColorInt[coun_strip].right_continuation[central_bunch_number];
if(right_contrast<0)
{
right_neighbor=-right_neighbor-1;
right_contrast_reduced=-right_contrast-1;
right_contrast_reduced=right_contrast_reduced%10;
}
number_of_sections_left=ColorSection->Number_of_sections_left;
number_of_sections_right=ColorSection->Number_of_sections_right;
if(left_contrast<0)
{//lc<0
left_trace_number=SectionTraceLeft[coun_strip*MAX_COL_INT + central_bunch_number];
left_trace_number_reduced=left_trace_number%1000;
    if(left_trace_number_reduced>0)
    {//lt>0
	left_trace_number_reduced-=1;
first_left_section_strip=ColorSection->section_fibre_first[left_trace_number_reduced];
last_left_section_strip=ColorSection->section_fibre_last[left_trace_number_reduced];
left_section_length=last_left_section_strip-first_left_section_strip+1;
if(left_section_length==2)
{//lsl=2
another_left_contrast=-1;
another_right_contrast=-1;
	if(first_left_section_strip==coun_strip)
	{//fs=
another_bunch=
ColorSection->DescrSec[left_trace_number_reduced].location_of_section[last_left_section_strip];
up_down=0;
	}//fs=
	else
	{//efs
		another_bunch=
ColorSection->DescrSec[left_trace_number_reduced].location_of_section[first_left_section_strip];
		up_down=1;
	}//efs
if(another_bunch>=0)
{//ab>=0
	another_bunch-=1;
	if(!up_down)
	{//ud
another_bunch=ColorInt[last_left_section_strip].old_bunch_number[another_bunch];
another_beg_int=
ColorInt[last_left_section_strip].ColoredIntervalsStructure->BegInterv[another_bunch];
another_end_int=
ColorInt[last_left_section_strip].ColoredIntervalsStructure->EndInterv[another_bunch];
another_left_contrast=ColorInt[last_left_section_strip].left_continuation[another_bunch];
another_right_contrast=ColorInt[last_left_section_strip].right_continuation[another_bunch];
	}//ud
	else
	{//eud
another_bunch=ColorInt[first_left_section_strip].old_bunch_number[another_bunch];
another_beg_int=
ColorInt[first_left_section_strip].ColoredIntervalsStructure->BegInterv[another_bunch];
another_end_int=
ColorInt[first_left_section_strip].ColoredIntervalsStructure->EndInterv[another_bunch];
another_left_contrast=ColorInt[first_left_section_strip].left_continuation[another_bunch];
another_right_contrast=ColorInt[first_left_section_strip].right_continuation[another_bunch];
	}//eud
//}//ab>=0

another_length=another_end_int-another_beg_int+1;

length_ratio=(32*central_int_length)/(another_length+central_int_length);
if(another_length)
{
length_ratio1=(32*central_int_length)/(another_length);
}
else
{
	length_ratio1=0;
}
if(!up_down)
{//ud0
left_shift=another_beg_int-beg_int;
right_shift=another_end_int-end_int;
}//ud0
else
{//ead0
	if(another_end_int>0)
	{
	left_shift=beg_int-another_beg_int;
    right_shift=end_int-another_end_int;
	}
}//ead0
	if(right_contrast<0)
    {//rc<0
if(another_left_contrast<0)
{//alc<0
       if((length_ratio1<20)||(2*abs(left_shift)>StripWidth))
	   {
 ColorInt[coun_strip].label_segments[central_bunch_number]=0;
 goto K;
       }
}//alc<0
	}//rc<0
}//ab>=0
}//lsl=2

	}//lt>0
}//lc<0
right_trace_number=SectionTraceRight[coun_strip*MAX_COL_INT + central_bunch_number];
right_trace_number_reduced=right_trace_number%1000;
if(right_trace_number_reduced>0)
    {//rt>0
	right_trace_number_reduced-=1;
first_right_section_strip=ColorSection->section_fibre_first[right_trace_number_reduced];
last_right_section_strip=ColorSection->section_fibre_last[right_trace_number_reduced];
right_section_length=last_right_section_strip-first_right_section_strip+1;
    }//rt>0
	else
	{//ert
		if(left_trace_number==0)
		{//ltn0
			if(central_int_length>4*StripWidth)
			{
ColorInt[coun_strip].label_segments[central_bunch_number]=0;
 goto K;
			}
		}//ltn0

	}//ert
}//ls>0
K:
;

}//cycle_label
 }//nl>0


	 }//cycle_strips

prior=0;

return(prior);
}*/
//=====================================================
int

CImageProcess::StraightLineHighObjectsTesting(void)
{
	int count_sec;
	//int section_number;
	int number_of_sections;
	int number_of_sections_left;
	int number_of_sections_right;
	int prior;
	int first_strip;
	int end_strip;
	int section_length;
	int contrast_beginning;
	int section_trace;
	int reduced_section_number;
	int bunch_new_number;
	int bunch_old_number;
	int num_strip;
	int bunch_beg;
	int bunch_end;
	int shift_number;
	int left_contrast;
	int left_adjacent;
	int left_bunch_end;
	int right_contrast;
	int right_adjacent;
	int right_bunch_beg;
	int differ_ends;
	int* contrast_ends;
	int* left_right_contrast_ends;
	int* differences;

	differ_ends=0;
prior=0;
left_contrast=-1;
left_adjacent=-1;
left_bunch_end=-1;
right_contrast=-1;
right_adjacent=-1;
right_bunch_beg=-1;
contrast_ends=new int[NumStrips];
differences=new int[NumStrips];
left_right_contrast_ends=new int[NumStrips];
number_of_sections=ColorSection->Number_of_sections;
number_of_sections_left=ColorSection->Number_of_sections_left;
number_of_sections_right=ColorSection->Number_of_sections_right;
for(count_sec=0;count_sec<number_of_sections;count_sec++)
     {//cycle_sections
first_strip=ColorSection->section_fibre_first[count_sec];
bunch_new_number=ColorSection->DescrSec[count_sec].location_of_section[first_strip]-1;

bunch_old_number=ColorInt[first_strip].old_bunch_number[bunch_new_number];

if(count_sec<number_of_sections_left)
{
section_trace=SectionTraceLeft[first_strip*MAX_COL_INT+bunch_old_number];
}
else
{
section_trace=SectionTraceRight[first_strip*MAX_COL_INT+bunch_old_number];
}

			if(section_trace>=1000)
			{
reduced_section_number=section_trace%1000;
reduced_section_number-=1;
contrast_beginning=1;
			}
			else
			{
reduced_section_number=section_trace-1;
contrast_beginning=0;
			}
end_strip=ColorSection->section_fibre_last[count_sec];
section_length=end_strip-first_strip+1;
for(num_strip=first_strip;num_strip<=end_strip;num_strip++)
{//cycle_fibers
bunch_new_number=ColorSection->DescrSec[count_sec].location_of_section[num_strip]-1;
bunch_old_number=ColorInt[num_strip].old_bunch_number[bunch_new_number];
if(count_sec<number_of_sections_left)
{
left_contrast=ColorInt[num_strip].left_continuation[bunch_old_number];
left_adjacent=ColorInt[num_strip].left_adjacent[bunch_old_number];
    if(left_contrast<0)
    {
    left_adjacent=-left_adjacent-1;
    left_bunch_end=ColorInt[num_strip].ColoredIntervalsStructure->EndInterv[left_adjacent];
    }
    else
    {
    left_bunch_end=-1;
    }
}
else
{
right_contrast=ColorInt[num_strip].right_continuation[bunch_old_number];
right_adjacent=ColorInt[num_strip].right_adjacent[bunch_old_number];
if(right_contrast<0)
    {
    right_adjacent=-right_adjacent-1;
    right_bunch_beg=ColorInt[num_strip].ColoredIntervalsStructure->BegInterv[right_adjacent];
    }
    else
    {
    right_bunch_beg=-1;
    }
}
bunch_beg=ColorInt[num_strip].ColoredIntervalsStructure->BegInterv[bunch_old_number];
bunch_end=ColorInt[num_strip].ColoredIntervalsStructure->EndInterv[bunch_old_number];
shift_number=num_strip-first_strip;
if(count_sec<number_of_sections_left)
{
*(contrast_ends+shift_number)=bunch_beg;
*(left_right_contrast_ends+shift_number)=left_bunch_end;
}
else
{
*(contrast_ends+shift_number)=bunch_end;
*(left_right_contrast_ends+shift_number)=right_bunch_beg;
}
}//cycle_fibers
             if(section_length>=3)
			 {//sl
FindingLineSegments(section_length,count_sec,contrast_ends,left_right_contrast_ends,differences);
			 }//sl
             else
            			 {
            				 if(section_length==2)
            				 {//sl=2
            					 differ_ends=contrast_ends[1]-contrast_ends[0];
            					 //if(count_sec<number_of_sections_left)
            					 //{
            						 if((differ_ends>0)&&(differ_ends<=5*StripWidth))
            						 {
            							 InclineCurve[count_sec]=1;
            						 }

            						 if((differ_ends<0)&&(abs(differ_ends)<=5*StripWidth))
            						 {
            							 InclineCurve[count_sec]=-1;
            						 }
            				 }//sl=2
            			 }
     }//cycle_sections
delete[] contrast_ends;
delete[] left_right_contrast_ends;
delete[] differences;
return(prior);
}
//=====================================================
int

CImageProcess::ConnectedSections(int* initial_matrix)
{//beg
	int prior;
	int number_of_sections;
	int number_of_sections_left;
    int number_of_sections_right;
	int count_row;
	int count_column;
	int matrix_entry;
	//int matrix_entry1;
	int connected_right_sections;

prior=0;
connected_right_sections=-1;
number_of_sections=ColorSection->Number_of_sections;
number_of_sections_left=ColorSection->Number_of_sections_left;
number_of_sections_right=ColorSection->Number_of_sections_right;
for(count_row=0;count_row<number_of_sections_left;count_row++)
{//cycle row
	for(count_column=0;count_column<number_of_sections_right;count_column++)
    {//cycle column
matrix_entry=*(initial_matrix+count_row*(NUM_SECT1/2)+count_column);
//matrix_entry1=SectionNeighborsLeftRight[count_row*(NUM_SECT1/2)+count_column];
if(matrix_entry>0)
{//ment
	connected_right_sections=FindingConnectedLeftSections(count_row,count_column,matrix_entry);
}//ment
	}//cycle column

}//cycle row

return(prior);
}//end


//=====================================================
int

CImageProcess::FindingConnectedLeftSections(int coun_row,int coun_column,int mat_entry)
{//beg
	int prior;
	int count_row;
	int number_of_sections;
	int number_of_sections_left;
    int number_of_sections_right;
	int entry_mat;
	int section_connected;

	prior=0;
number_of_sections=ColorSection->Number_of_sections;
number_of_sections_left=ColorSection->Number_of_sections_left;
number_of_sections_right=ColorSection->Number_of_sections_right;
for(count_row=0;count_row<number_of_sections_left;count_row++)
{//cycle column
entry_mat=SectionNeighborsLeftRight[count_row*(NUM_SECT1/2)+coun_column];
if(entry_mat>0)
{
	section_connected=SectionNeighborsLeftRightIteration[coun_row*(NUM_SECT1/2)+count_row];
	if(!section_connected)
	{
	SectionNeighborsLeftRightIteration[coun_row*(NUM_SECT1/2)+count_row]=coun_column+1;
	}
}
}//cycle column
	return(prior);
}//end
//=====================================================
//=====================================================
int

CImageProcess::FindingLineSegments(int sect_length,int section_num,int* node_coordinates,
int* opp_nodes_coor,int* differ_array)
{
	int prior;
	int count_mem;
	int count_shift;
	int hist_arg;
	int abs_arg;
	int count_pos;
    int count_neg;
	int shift_divisor;
	int hist_pos[64];
	int hist_neg[64];
	int shift_pos[16];
	int shift_neg[16];
	int maximum_pos;
	int maximum_value_pos;
	int triple_maximum_pos;
	int triple_maximum_value_pos;
	int maximum_pos_red;
	int maximum_value_pos_red;
	int triple_maximum_pos_red;
	int triple_maximum_value_pos_red;
	int two_pix_neighborhood_pos_red;
	int one_pix_neighborhood_pos_red;
	int zero_two_pix_neighb_red;
	int maximum_neg_red;
	int maximum_value_neg_red;
	int triple_maximum_neg_red;
	int triple_maximum_value_neg_red;
	int two_pix_neighborhood_neg_red;
	int one_pix_neighborhood_neg_red;
	int zero_two_pix_neighb_neg_red;
	int maximum_neg;
	int maximum_value_neg;
	int triple_maximum_neg;
	int triple_maximum_value_neg;
	int two_pix_neighborhood_pos;
	int two_pix_neighborhood_neg;
	int one_pix_neighborhood_pos;
	int one_pix_neighborhood_neg;
	int pos_fraction;
	int residual;
	int hist_arg_div;
	int left_right_weight;
	int max_pos_neg_weight;
	int one_max_pos_neg_weight;
	int two_max_pos_neg_weight;
	int left_right_weight_red;
	int max_pos_neg_weight_red;
	int one_max_pos_neg_weight_red;
	int two_max_pos_neg_weight_red;
	int zero_two_pix_neighb;
	int zero_two_pix_neighb_neg;
	int dimen_links;
	int address_beginning;
	int node_coord;
	int* inclination_links;
	int* addresses_zero_nodes;
	int dimen_addresses;
	int node_coord1;
	int node_coord0;
	int total_num_pos;
	int total_num_neg;
	int center_m_pos;
	int center_m_neg;
	int dev_pos;
	int dev_neg;
    int left_number_pos;
    int right_number_pos;
	int left_number_neg;
    int right_number_neg;
	int small_angle_pos;
	int small_angle_neg;
	int big_angle_pos;
	int big_angle_neg;
	int scale[16];
	int paint[64];
	int hist_box_pos[16];
	int hist_box_neg[16];
	int dim_new_hist;
	int small_angle_weight;
	int max_pos_weight;
    int max_neg_weight;
	int small_angle_weight_red;
	int small_angle_weight_pos;
	int small_angle_weight_neg;
	int one_max_pos_weight;
	int one_max_neg_weight;
	int num_loc_max;
	int second_max_pos;
	int second_max_value;
	int beg_noticable;
	int end_noticable;
	int noticable_sum;
	int num_loc_max_neg;
	int second_max_pos_neg;
	int second_max_value_neg;
	int beg_noticable_neg;
	int end_noticable_neg;
	int noticable_sum_neg;
	int small_angle;
	int noticable_sum_weight;
    int noticable_sum_neg_weight;
	int noticable_width;
    int noticable_width_neg;
	int max_count_angle;
	int first_count_step;
	int vertical_weight;
	int one_max_pos_weight_red;
	int one_max_neg_weight_red;
	int one_max_pos_weight_red1;
	int one_max_neg_weight_red1;
	int vertical_comp;
	int shift_scale;
	int first_seg;
	int last_seg;
	int num_of_seg;
	int dev_red;
	int long_str_comp;
	int arg_max_num_seg;
	int max_num_seg;
	int mean_dev;
	int global_deviat;
	int global_deviat_plus;
	int global_deviat_abs;
	int global_deviat_max;
	int incline;
	int right_bound;
	int regular_fraction_of_segments;
	int vert_segm_count;
	int entrance_point;
	int max_last_segm;
	int pos_number;
    int neg_number;
	int vert_max_length;
	int incline_length;
	int hist_arg_pos_incl;
	int hist_arg_neg_incl;
	int pos_shift_curve;
	int total_pos_curve;
	int neg_shift_curve;
	int total_neg_curve;
	int total_shift_curve;
	int num_left_sections;
	int total_pos_ratio;
	int total_neg_ratio;
	int medium_step;
	int medium_step_pl;
	int medium_step_mn;//last_cor116.11.16
	int last_pos_shift;//last_cor24.07.17
	int last_neg_shift;
	int pos_bound;
	int neg_bound;
	int hist_arg1;
	int max_length_pos;
	int max_start_pos;
    int max_length_neg;
	int max_start_neg;
	int max_neg_weight_red;
	int max_pos_weight_red;//last_cor03.08.17
	int success_ord;//last_cor29.08.17
	int success_ord1;//last_cor31.08.17
	int number_of_chains_plus;//last_cor06.09.17
	int number_of_chains_minus;
	int longest_chain_number_plus;
	int longest_chain_number_minus;//last_cor06.09.17
	int max_finish_pos;
    int max_finish_neg;
    int incl_local;
	int incl_local1;
	int reliability;
    int reliability1;
	int max_start_pos_ratio;
	int max_start_neg_ratio;
	int max_finish_pos_ratio;
	int max_finish_neg_ratio;
	int first_strip;
	int last_strip;
    int first_strip_ratio;
	int last_strip_ratio;
	int last_ch_last_plus;
    int last_ch_last_minus;
	int continuation_appropriate_plus;
	int continuation_appropriate_minus;
	int zero_chain_prolong_last;
	int zero_chain_prolong_last_end;
	int zero_chain_prolong_last_end_ratio;
	int zero_chain_first;
    int zero_chain_first_ratio;
	int zero_chain_prolong_last_first_member;
    int optimal_chain_prolong_last;
	int optimal_chain_prolong_last_end;
	int optimal_chain_prolong_last_end_ratio;
	int optimal_chain_first;
    int optimal_chain_first_ratio;
	int optimal_chain_prolong_last_first_member;
	int zero_chain_prolong_last_pos;
	int zero_chain_prolong_last_end_pos;
	int zero_chain_prolong_last_end_ratio_pos;
	int zero_chain_first_pos;
    int zero_chain_first_ratio_pos;
	int zero_chain_prolong_last_first_member_pos;
	int optimal_chain_prolong_last_pos;
	int optimal_chain_prolong_last_end_pos;
	int optimal_chain_prolong_last_end_ratio_pos;
	int optimal_chain_first_pos;
    int optimal_chain_first_ratio_pos;
	int optimal_chain_prolong_last_first_member_pos;
	int check_max_minus;
	int check_max_plus;
	int first_continued;
	int maxim_length;
	int first_continued_minus;
	int maxim_length_minus;
	int case_number;
	int case_number_plus;
	int loop_begin;//last_cor20.12.17
	int next_plus;
	int next_minus;//last_cor20.12.17
	int incline_current;//last_cor21.12.17
    int incline_current_local;
    int incl_vert_length;
    int incl_vert_length_ratio;//last_cor21.12.17
	int optimal_length;//last_cor01.03.18
	int optimal_length_minus;//last_cor01.03.18
	int* local_maxim_points;
	int* local_maxim_points_neg;
	int* first_straight_segments_points;
	int* total_dev;
	int* first_seg_vert;
	int* last_seg_vert;
	int* total_dev_reduced;
	int* global_deviation_up_top;
	int* pos_deviation_max;
	int* neg_deviation_max;
	int* longest_positive_adjacent_interval;//last_cor25.07.17
	int* longest_negative_adjacent_interval;
	int* next_member_appropriate_plus;//last_cor31.08.17
	int* chains_appropriate_plus;
	int* chain_first_member_plus;
	int* next_member_appropriate_minus;
	int* chains_appropriate_minus;
	int* chain_first_member_minus;
	int chains_junction[32];
	int chains_junction_pos[32];
	int number_of_vertical_components;//last_cor20.12.17
	int* vertical_components;

//return(0);
	optimal_length_minus=-1;//last_cor01.03.18
	optimal_length=-1;//last_cor01.03.18
	incline_current=0;//last_cor21.12.17
    incline_current_local=0;
    incl_vert_length=-1;
    incl_vert_length_ratio=-1;//last_cor21.12.17
	number_of_vertical_components=0;
	case_number_plus=-1;
	case_number=-1;
	first_continued_minus=-1;
	maxim_length_minus=-1;
	first_continued=-1;
	maxim_length=-1;
	check_max_plus=-1;
	check_max_minus=-1;
	zero_chain_prolong_last_end_ratio_pos=-1;
	zero_chain_prolong_last_first_member_pos=-1;
	zero_chain_first_ratio_pos=-1;
	zero_chain_prolong_last_pos=-1;
	zero_chain_prolong_last_end_pos=-1;
	zero_chain_first_pos=-1;
	zero_chain_prolong_last_end_ratio=-1;
	zero_chain_prolong_last_first_member=-1;
	zero_chain_first_ratio=-1;
	zero_chain_prolong_last=-1;
	zero_chain_prolong_last_end=-1;
	zero_chain_first=-1;
	optimal_chain_prolong_last_end_ratio=-1;
	optimal_chain_prolong_last_first_member=-1;
	optimal_chain_first_ratio=-1;
	optimal_chain_prolong_last=-1;
	optimal_chain_prolong_last_end=-1;
	optimal_chain_first=-1;
	optimal_chain_prolong_last_end_ratio_pos=-1;
	optimal_chain_prolong_last_first_member_pos=-1;
	optimal_chain_first_ratio_pos=-1;
	optimal_chain_prolong_last_pos=-1;
	optimal_chain_prolong_last_end_pos=-1;
	optimal_chain_first_pos=-1;
	continuation_appropriate_plus=-1;
	continuation_appropriate_minus=-1;
	last_ch_last_plus=-1;
    last_ch_last_minus=-1;
	max_start_pos_ratio=-1;
	max_start_neg_ratio=-1;
	max_finish_pos_ratio=-1;
	max_finish_neg_ratio=-1;
	reliability=-1;
	reliability1=-1;
	incl_local=0;//last_cor11.09.17
	incl_local1=0;//last_cor12.09.17
	max_finish_pos=-1;
    max_finish_neg=-1;
	longest_chain_number_plus=-1;
	longest_chain_number_minus=-1;
	number_of_chains_plus=-1;//last_cor06.09.17
	number_of_chains_minus=-1;
	success_ord=-100;//last_cor29.08.17
	success_ord1=-100;//last_cor31.08.17
	max_length_pos=-1;
	max_start_pos=-1;
    max_length_neg=-1;
	max_start_neg=-1;//last_cor02.08.17
	hist_arg1=0;//last_cor25.07.17
	last_pos_shift=-1;//last_cor24.07.17
	last_neg_shift=-1;
	medium_step_pl=0;//last_cor116.11.16
	medium_step_mn=0;
	pos_shift_curve=0;
	total_pos_curve=0;
	neg_shift_curve=0;
	total_neg_curve=0;
	total_shift_curve=0;
	hist_arg_pos_incl=0;
	hist_arg_neg_incl=0;
	incline_length=-1;
	vert_max_length=-1;
	pos_number=0;
    neg_number=0;
	right_bound=-1;
	entrance_point=0;
	incline=-1;
	regular_fraction_of_segments=0;
	global_deviat=0;
	global_deviat_plus=0;
	global_deviat_abs=0;
	mean_dev=0;
	dev_red=-1;
	long_str_comp=-1;
	first_seg=-1;
	last_seg=-1;
	num_of_seg=-1;
	shift_scale=0;
	vertical_comp=0;
	max_neg_weight_red=0;
	max_pos_weight_red=0;
	one_max_pos_weight_red=0;
	one_max_neg_weight_red=0;
	one_max_pos_weight_red1=0;
	one_max_neg_weight_red1=0;
	vertical_weight=0;
	noticable_width=-1;
	noticable_width_neg=-1;
	noticable_sum_weight=0;
    noticable_sum_neg_weight=0;
	noticable_sum_neg=0;
	noticable_sum=0;
	beg_noticable_neg=-1;
	end_noticable_neg=-1;
	beg_noticable=-1;
	end_noticable=-1;
	small_angle=0;
	num_loc_max=0;
	second_max_pos=-1;
	second_max_value=0;
	num_loc_max_neg=0;
	second_max_pos_neg=-1;
	second_max_value_neg=0;
	one_max_pos_weight=0;
	one_max_neg_weight=0;
	small_angle_weight_pos=0;
	small_angle_weight_neg=0;
	max_pos_weight=0;
    max_neg_weight=0;
	small_angle_weight_red=0;
count_pos=0;
count_neg=0;
prior=-1;
hist_arg=-1;
abs_arg=-1;
shift_divisor=1;
maximum_pos=-1;
triple_maximum_pos=-1;
maximum_value_pos=-1;
triple_maximum_value_pos=0;
one_pix_neighborhood_pos=0;
two_pix_neighborhood_pos=0;
zero_two_pix_neighb=0;
maximum_pos_red=-1;
triple_maximum_pos_red=-1;
maximum_value_pos_red=0;
triple_maximum_value_pos_red=0;
one_pix_neighborhood_pos_red=0;
two_pix_neighborhood_pos_red=0;
zero_two_pix_neighb_red=0;
maximum_neg_red=-1;
maximum_value_neg_red=0;
triple_maximum_neg_red=-1;
triple_maximum_value_neg_red=0;
two_pix_neighborhood_neg_red=0;
one_pix_neighborhood_neg_red=0;
zero_two_pix_neighb_neg_red=0;
maximum_neg=-1;
triple_maximum_neg=-1;
maximum_value_neg=-1;
triple_maximum_value_neg=0;
one_pix_neighborhood_neg=0;
two_pix_neighborhood_neg=0;
zero_two_pix_neighb_neg=0;
left_right_weight=0;
max_pos_neg_weight=-1;
one_max_pos_neg_weight=0;
two_max_pos_neg_weight=0;
left_right_weight_red=0;
max_pos_neg_weight_red=0;
one_max_pos_neg_weight_red=0;
two_max_pos_neg_weight_red=0;
address_beginning=0;
center_m_pos=-1;
center_m_neg=-1;
dev_pos=-1;
dev_neg=-1;
total_num_pos=-1;
total_num_neg=-1;
left_number_pos=-1;
right_number_pos=-1;
small_angle_pos=0;
big_angle_pos=-1;
left_number_neg=-1;
right_number_neg=-1;
small_angle_neg=0;
small_angle_weight=0;
big_angle_neg=-1;
dim_new_hist=12;
pos_fraction=0;
max_count_angle=0;
first_count_step=0;
num_left_sections=ColorSection->Number_of_sections_left;
first_strip=ColorSection->section_fibre_first[section_num];
last_strip=ColorSection->section_fibre_last[section_num];
first_strip_ratio=(16*first_strip)/NumStrips;
last_strip_ratio=(16*last_strip)/NumStrips;
next_member_appropriate_plus= new int[sect_length];//last_cor31.08.17
chains_appropriate_plus= new int[sect_length];
chain_first_member_plus= new int[sect_length];
vertical_components= new int[sect_length];//last_cor20.12.17
memset(vertical_components,(int) '\0',sizeof(int)*(sect_length));
memset(next_member_appropriate_plus,(int) '\0',sizeof(int)*(sect_length));
memset(chains_appropriate_plus,(int) '\0',sizeof(int)*(sect_length));
memset(chain_first_member_plus,(int) '\0',sizeof(int)*(sect_length));
next_member_appropriate_minus= new int[sect_length];//last_cor31.08.17
chains_appropriate_minus= new int[sect_length];
chain_first_member_minus= new int[sect_length];
memset(next_member_appropriate_minus,(int) '\0',sizeof(int)*(sect_length));
memset(chains_appropriate_minus,(int) '\0',sizeof(int)*(sect_length));
memset(chain_first_member_minus,(int) '\0',sizeof(int)*(sect_length));
longest_positive_adjacent_interval=new int[sect_length];
memset(longest_positive_adjacent_interval,(int) '\0',(sizeof(int))*sect_length);
longest_negative_adjacent_interval=new int[sect_length];
memset(longest_negative_adjacent_interval,(int) '\0',(sizeof(int))*sect_length);
pos_deviation_max=new int[sect_length];
memset(pos_deviation_max,(int) '\0',(sizeof(int))*sect_length);
neg_deviation_max=new int[sect_length];
memset(neg_deviation_max,(int) '\0',(sizeof(int))*sect_length);
memset(chains_junction,(int) '\0',(sizeof(int))*32);
memset(chains_junction_pos,(int) '\0',(sizeof(int))*32);
dimen_links=(sect_length*(sect_length-1))/2;
dimen_addresses=sect_length-2;
inclination_links=new int[dimen_links];
addresses_zero_nodes=new int[dimen_addresses];
local_maxim_points=new int[dim_new_hist];
memset(local_maxim_points,(int) '\0',(sizeof(int))*dim_new_hist);
local_maxim_points_neg=new int[dim_new_hist];
memset(local_maxim_points_neg,(int) '\0',(sizeof(int))*dim_new_hist);
memset(hist_pos,(int) '\0',(sizeof(int))*64);
memset(hist_neg,(int) '\0',(sizeof(int))*64);
memset(shift_pos,(int) '\0',(sizeof(int))*16);
memset(shift_neg,(int) '\0',(sizeof(int))*16);
memset(scale,(int) '\0',(sizeof(int))*16);
memset(hist_box_pos,(int) '\0',(sizeof(int))*16);
memset(hist_box_neg,(int) '\0',(sizeof(int))*16);

scale[0]=0;
scale[1]=StripWidth/4;
scale[2]=StripWidth/2;
scale[3]=3*StripWidth/4;
scale[4]=std::min(StripWidth,63);
scale[5]=std::min(3*StripWidth/2,63);
scale[6]=std::min(2*StripWidth,63);
scale[7]=std::min(3*StripWidth,63);
scale[8]=std::min(4*StripWidth,63);
scale[9]=std::min(6*StripWidth,63);
scale[10]=std::min(8*StripWidth,63);
scale[11]=63;
//think of small images!
paint[0]=0;
for(int count_angle=0;count_angle<11;count_angle++)
    {//cycle angle_values
		first_count_step=scale[count_angle];
		if(first_count_step<63)
		{
for(int count_step=scale[count_angle]+1;count_step<=scale[count_angle+1];count_step++)
{
paint[count_step]=count_angle;
}
		}
		else
		{
		max_count_angle=count_angle;
		break;
		}
    }
if(section_num>=num_left_sections)
{//last_cor20.12.17
number_of_vertical_components=
	VerticalComponentsInBoundary(section_num,node_coordinates,vertical_components,-1);
}//last_cor20.12.17
else
{
number_of_vertical_components=
	VerticalComponentsInBoundary(section_num,node_coordinates,vertical_components,1);
}
//if(section_num>=num_left_sections)
//{//last_cor29.08.17
success_ord=LongestPlusMinusChain(section_num,sect_length,node_coordinates,
next_member_appropriate_plus,chains_appropriate_plus,chain_first_member_plus,
&number_of_chains_plus,1);
  if(number_of_chains_plus>1)
  {//ncp
longest_chain_number_plus=
ChainLongest(sect_length,number_of_chains_plus,node_coordinates,next_member_appropriate_plus,
chains_appropriate_plus,chain_first_member_plus,&last_ch_last_plus,chains_junction_pos,1);//last_cor06.09.17
check_max_plus=SequenceOfChainsLongest(sect_length,section_num,
number_of_chains_plus,node_coordinates,chains_junction_pos,chain_first_member_plus,chains_appropriate_plus,
&first_continued,&maxim_length);
if((first_continued>=0)&&(first_continued!=check_max_plus))
{//fc!
zero_chain_prolong_last_pos=chains_junction_pos[first_continued];
        if(zero_chain_prolong_last_pos>0)
	    {
	zero_chain_first_pos=*(chain_first_member_plus+first_continued);
zero_chain_prolong_last_first_member_pos=*(chain_first_member_plus+zero_chain_prolong_last_pos);
	if(zero_chain_prolong_last_first_member_pos>0)
	{
		zero_chain_prolong_last_first_member_pos-=1;
zero_chain_prolong_last_end_pos=
*(chains_appropriate_plus+zero_chain_prolong_last_first_member_pos);
	}

	if(zero_chain_first_pos>0)
	{
zero_chain_first_pos-=1;
zero_chain_first_ratio_pos=(16*(zero_chain_first_pos+first_strip))/NumStrips;
	}
	if(zero_chain_prolong_last_end_pos>0)
	{
zero_chain_prolong_last_end_ratio_pos=
(16*(zero_chain_prolong_last_end_pos+first_strip))/NumStrips;
	}
		}
}//fc!
if(check_max_plus>=0)
{//cmp>=
	optimal_chain_prolong_last_pos=chains_junction_pos[check_max_plus];
	optimal_chain_first_pos=*(chain_first_member_plus+check_max_plus);
optimal_chain_prolong_last_first_member_pos=
*(chain_first_member_plus+optimal_chain_prolong_last_pos);
	if(optimal_chain_prolong_last_first_member_pos>0)
	{
		optimal_chain_prolong_last_first_member_pos-=1;
optimal_chain_prolong_last_end_pos=
*(chains_appropriate_plus+optimal_chain_prolong_last_first_member_pos);
	}

	if(optimal_chain_first_pos>0)
	{
optimal_chain_first_pos-=1;
optimal_chain_first_ratio_pos=(16*(optimal_chain_first_pos+first_strip))/NumStrips;
	}
	if(optimal_chain_prolong_last_end_pos>0)
	{
optimal_chain_prolong_last_end_ratio_pos=
(16*(optimal_chain_prolong_last_end_pos+first_strip))/NumStrips;
optimal_length=optimal_chain_prolong_last_end_pos-optimal_chain_first_pos+1;
	}
}//cmp>=
if(longest_chain_number_plus>=0)
{//last_cor06.09.17
max_start_pos=*(chain_first_member_plus+longest_chain_number_plus);
  if(max_start_pos>0)
  {
max_start_pos--;
max_finish_pos=*(chains_appropriate_plus+max_start_pos);
max_length_pos=max_finish_pos-max_start_pos+1;
max_start_pos_ratio=(16*(max_start_pos+first_strip))/NumStrips;
max_finish_pos_ratio=(16*(max_finish_pos+first_strip))/NumStrips;
if(last_ch_last_plus!=(sect_length-1))
{
	continuation_appropriate_plus=*(next_member_appropriate_plus+last_ch_last_plus);
}
  }
}//last_cor06.09.17
if(optimal_chain_prolong_last_end_pos>=(sect_length-2))
{//last_cor01.03.18
if((optimal_chain_prolong_last_end_ratio_pos>=7)&&
   (optimal_chain_first_ratio_pos<optimal_chain_prolong_last_end_ratio_pos)&&
   (optimal_length>=5))
{//condpos
incl_local1=StraightApproximation(node_coordinates,paint,optimal_chain_first_pos,
optimal_chain_prolong_last_end_pos,&reliability1,0);
   if(reliability1>0)
   {
	   InclineCurve[section_num]=1;
    *(InclineLocal+section_num)=incl_local1;
	*(InclineLocalFirst+section_num)=optimal_chain_first_pos;
    *(InclineLocalLast+section_num)=optimal_chain_prolong_last_end_pos;
	if((optimal_length>=6)&&(optimal_chain_prolong_last_end_pos==(sect_length-1)))
	{
	*(InclinedComponent+section_num)=incl_local1;
	}
   }
}//condpos
}//last_cor01.03.18
  }//ncp
  else
  {//encp
if(number_of_chains_plus==1)
{
max_start_pos=*(chain_first_member_plus+0);
if(max_start_pos>0)
{
max_start_pos--;
max_finish_pos=*(chains_appropriate_plus+max_start_pos);
max_length_pos=max_finish_pos-max_start_pos+1;
max_start_pos_ratio=(16*(max_start_pos+first_strip))/NumStrips;
max_finish_pos_ratio=(16*(max_finish_pos+first_strip))/NumStrips;
//if(!number_of_chains_plus)
//{//last_cor29.11.1701.03.18!!!
if(next_member_appropriate_plus[0]==(max_start_pos+1))
{
	if(!node_coordinates[0])
	{
if((max_finish_pos>=(sect_length-2))&&(max_finish_pos_ratio>=7)&&
   (max_start_pos_ratio<max_finish_pos_ratio))
{
incl_local1=StraightApproximation(node_coordinates,paint,max_start_pos,max_finish_pos,
&reliability1,0);
if(reliability1>0)
 {
	 InclineCurve[section_num]=1;
	*(InclineLocal+section_num)=incl_local1;
	*(InclineLocalFirst+section_num)=max_start_pos;
    *(InclineLocalLast+section_num)=max_finish_pos;
	if((max_length_neg>=4)&&(max_finish_pos==(sect_length-1)))
	    {
        *(InclinedComponent+section_num)=incl_local1;
	    }
}
}
	}
}
//}//last_cor29.11.17
}
}
  }//encp
success_ord1=LongestPlusMinusChain(section_num,sect_length,node_coordinates,
next_member_appropriate_minus,chains_appropriate_minus,chain_first_member_minus,
&number_of_chains_minus,-1);
  if(number_of_chains_minus>1)
  {//ncm
longest_chain_number_minus=
ChainLongest(sect_length,number_of_chains_minus,node_coordinates,next_member_appropriate_minus,
chains_appropriate_minus,chain_first_member_minus,&last_ch_last_minus,chains_junction,-1);//last_cor06.09.17
check_max_minus=SequenceOfChainsLongest(sect_length,section_num,number_of_chains_minus,
node_coordinates,chains_junction,chain_first_member_minus,chains_appropriate_minus,
&first_continued_minus,&maxim_length_minus);
if((first_continued_minus>=0)&&(first_continued_minus!=check_max_minus))
{
    zero_chain_prolong_last=chains_junction[first_continued_minus];
	   if(zero_chain_prolong_last>0)
	    {
	zero_chain_first=*(chain_first_member_minus+first_continued_minus);
	zero_chain_prolong_last_first_member=*(chain_first_member_minus+zero_chain_prolong_last);
	if(zero_chain_prolong_last_first_member>0)
	{
		zero_chain_prolong_last_first_member-=1;
zero_chain_prolong_last_end=*(chains_appropriate_minus+zero_chain_prolong_last_first_member);
	}

	if(zero_chain_first>0)
	{
zero_chain_first-=1;
zero_chain_first_ratio=(16*(zero_chain_first+first_strip))/NumStrips;
	}
	if(zero_chain_prolong_last_end>0)
	{
zero_chain_prolong_last_end_ratio=(16*(zero_chain_prolong_last_end+first_strip))/NumStrips;
	}
	   }
}
if(check_max_minus>=0)
{
	optimal_chain_prolong_last=chains_junction[check_max_minus];
	optimal_chain_first=*(chain_first_member_minus+check_max_minus);
optimal_chain_prolong_last_first_member=*(chain_first_member_minus+optimal_chain_prolong_last);
	if(optimal_chain_prolong_last_first_member>0)
	{
		optimal_chain_prolong_last_first_member-=1;
optimal_chain_prolong_last_end=
*(chains_appropriate_minus+optimal_chain_prolong_last_first_member);
	}

	if(optimal_chain_first>0)
	{
optimal_chain_first-=1;
optimal_chain_first_ratio=(16*(optimal_chain_first+first_strip))/NumStrips;
	}
	if(optimal_chain_prolong_last_end>0)
	{
optimal_chain_prolong_last_end_ratio=
(16*(optimal_chain_prolong_last_end+first_strip))/NumStrips;
	}
}
if(longest_chain_number_minus>=0)
{//last_cor06.09.17
max_start_neg=*(chain_first_member_minus+longest_chain_number_minus);
  if(max_start_neg>0)
  {
max_start_neg--;
max_finish_neg=*(chains_appropriate_minus+max_start_neg);
max_length_neg=max_finish_neg-max_start_neg+1;
max_start_neg_ratio=(16*(max_start_neg+first_strip))/NumStrips;
max_finish_neg_ratio=(16*(max_finish_neg+first_strip))/NumStrips;
if(last_ch_last_minus!=(sect_length-1))
{
	continuation_appropriate_minus=*(next_member_appropriate_minus+last_ch_last_minus);
}
  }
}//last_cor06.09.17
if(optimal_chain_prolong_last_end>=(sect_length-2))
{//last_cor01.03.18
if((optimal_chain_prolong_last_end_ratio>=7)&&
   (optimal_chain_first_ratio<optimal_chain_prolong_last_end_ratio)&&
   (optimal_length_minus>=5))
{
incl_local=StraightApproximation(node_coordinates,paint,optimal_chain_first,
optimal_chain_prolong_last_end,&reliability,0);
   if(reliability>0)
   {
	   InclineCurve[section_num]=-1;
    *(InclineLocal+section_num)=incl_local;
	*(InclineLocalFirst+section_num)=optimal_chain_first;
    *(InclineLocalLast+section_num)=optimal_chain_prolong_last_end;
	if((optimal_length_minus>=6)&&(optimal_chain_prolong_last_end==(sect_length-1)))
	{
	*(InclinedComponent+section_num)=incl_local;
	}
   }
}
}//last_cor01.03.18
  }//ncm
  else
  {
	  if(number_of_chains_minus==1)
	  {
max_start_neg=*(chain_first_member_minus+0);
     if(max_start_neg>0)
     {
max_start_neg--;
max_finish_neg=*(chains_appropriate_minus+max_start_neg);
max_length_neg=max_finish_neg-max_start_neg+1;
max_start_neg_ratio=(16*(max_start_neg+first_strip))/NumStrips;
max_finish_neg_ratio=(16*(max_finish_neg+first_strip))/NumStrips;
if(!number_of_chains_plus)
{//last_cor29.11.17
if(next_member_appropriate_minus[0]==(max_start_neg+1))
{
	if(node_coordinates[0]==(DimX-1))
	{
if(max_finish_neg==(sect_length-1))
{
incl_local=StraightApproximation(node_coordinates,paint,max_start_neg,max_finish_neg,
&reliability,1);
InclineCurve[section_num]=-1;
if(reliability>0)
 {
	*(InclineLocal+section_num)=incl_local;
	*(InclineLocalFirst+section_num)=max_start_neg;
    *(InclineLocalLast+section_num)=max_finish_neg;
	if(max_length_neg>=3)
	    {
        *(InclinedComponent+section_num)=incl_local;
	    }
}
}
	}
}
}//last_cor29.11.17
	 }
	  }
  }
  if(((last_strip_ratio>=7)&&(((max_start_neg>=0)&&(max_finish_neg_ratio>=7)&&
	  (max_start_neg_ratio<=4))||
	  ((zero_chain_prolong_last_end_ratio>=7)&&(zero_chain_first_ratio<=4)&&
	  (zero_chain_first>=0)&&(number_of_chains_minus>1))||
	  ((check_max_minus>=0)&&(optimal_chain_prolong_last_end_ratio>=7)&&(optimal_chain_first_ratio<=4)&&
	  (optimal_chain_first>=0)&&(number_of_chains_minus>1))))
	  ||((max_start_neg>=0)&&(((max_start_neg<=1)&&(max_finish_neg>=(sect_length-2))&&
	  (max_length_neg>3))||((max_start_neg<=3)&&(max_finish_neg>=(sect_length-3))&&
	  (max_length_neg>5))))||
((check_max_minus>=0)&&(((optimal_chain_first<=1)&&(optimal_chain_prolong_last>=(sect_length-2))
 &&(maxim_length_minus>3))||((optimal_chain_first<=3)&&(optimal_chain_prolong_last>=(sect_length-3))
 &&(maxim_length_minus>5)))))
  {
	  if((number_of_chains_minus==1)||((max_start_neg<=1)&&(max_length_neg>=8)&&
		  (2*max_length_neg>sect_length)))
	  {//last_cor14.02.18
		  if(!*(InclineLocal+section_num))
		  {//last_cor02.03.18
incl_local=StraightApproximation(node_coordinates,paint,max_start_neg,max_finish_neg,
&reliability,1);
   if(reliability>0)
   {
	*(InclineLocal+section_num)=incl_local;
	*(InclineLocalFirst+section_num)=max_start_neg+1;
    *(InclineLocalLast+section_num)=max_finish_neg+1;
if(((max_start_neg>=0)&&(max_start_neg<=1)&&(max_finish_neg>=(sect_length-2))&&(max_length_neg>3))
||((max_start_neg>=0)&&(max_start_neg<=3)&&(max_finish_neg>=(sect_length-3))&&
	  (max_length_neg>5)))
	    {
        *(InclinedComponent+section_num)=incl_local;
	    }
case_number=1;
   }
		  }
	  }
	  else
	  {
		  if(check_max_minus>=0)
		  {
			  if((longest_chain_number_minus>=check_max_minus)&&
				  (longest_chain_number_minus<=optimal_chain_prolong_last))
			  {
				  if(!*(InclineLocal+section_num))
				  {
/*incl_local=StraightApproximation(node_coordinates,paint,optimal_chain_first,
optimal_chain_prolong_last_end,&reliability,1);*/
   if(reliability>0)
   {
    *(InclineLocal+section_num)=incl_local;
	*(InclineLocalFirst+section_num)=optimal_chain_first+1;
    *(InclineLocalLast+section_num)=optimal_chain_prolong_last_end+1;
	if((check_max_minus>=0)&&(((optimal_chain_first<=1)&&(optimal_chain_prolong_last>=(sect_length-2))
 &&(maxim_length_minus>3))||((optimal_chain_first<=3)&&(optimal_chain_prolong_last>=(sect_length-3))
 &&(maxim_length_minus>5))))
	    {
        *(InclinedComponent+section_num)=incl_local;
	    }
case_number=2;
   }
				  }
			  }
			  else
			  {
				  if(longest_chain_number_minus<check_max_minus)
				  {
if(max_length_neg>NumStrips/6)
{
	if(!*(InclineLocal+section_num))
	{
/*incl_local=StraightApproximation(node_coordinates,paint,max_start_neg,max_finish_neg,
&reliability,1);*/
if(reliability>0)
   {
	*(InclineLocal+section_num)=incl_local;
	*(InclineLocalFirst+section_num)=max_start_pos+1;
    *(InclineLocalLast+section_num)=max_finish_pos+1;
if(((max_start_neg>=0)&&(max_start_neg<=1)&&(max_finish_neg>=(sect_length-2))&&(max_length_neg>3))
||((max_start_neg>=0)&&(max_start_neg<=2)&&(max_finish_neg>=(sect_length-3))&&
	  (max_length_neg>5)))
	    {
        *(InclinedComponent+section_num)=incl_local;
	    }
   }
case_number=3;
	}
}
				  }
			  }
		  }
	  }
  }
  if(((last_strip_ratio>=7)&&(((max_start_pos>=0)&&(max_finish_pos_ratio>=7)&&
	  (max_start_pos_ratio<=4))||
	  ((zero_chain_prolong_last_end_ratio_pos>=7)&&(zero_chain_first_ratio_pos<=4)&&
	  (zero_chain_first_pos>=0)&&(number_of_chains_plus>1))))
	  ||((max_start_pos>=0)&&(max_start_pos<=1)&&(max_finish_pos>=(sect_length-2))&&
	  (max_length_pos>3)))
  {
	  if(!*(InclineLocal+section_num))
	  {
/*incl_local1=StraightApproximation(node_coordinates,paint,max_start_pos,max_finish_pos,
&reliability1,0);*/
if(reliability1>0)
{
	if(number_of_chains_plus==1)
	{
	*(InclineLocal+section_num)=incl_local1;
	*(InclineLocalFirst+section_num)=max_start_pos+1;
    *(InclineLocalLast+section_num)=max_finish_pos+1;
	  if(max_finish_pos==(sect_length-1))
	    {
        *(InclinedComponent+section_num)=incl_local1;
	    }
	}
	else
	{
      if(number_of_chains_plus>1)
	  {
*(InclineLocal+section_num)=incl_local1;
	*(InclineLocalFirst+section_num)=zero_chain_first_pos+1;
    *(InclineLocalLast+section_num)=zero_chain_prolong_last_end_pos+1;
	if(zero_chain_prolong_last_end_pos==(sect_length-1))
	    {
        *(InclinedComponent+section_num)=incl_local1;
	    }
	  }
	}
}
	  }
  }
  next_plus=next_member_appropriate_plus[0];
  next_minus=next_member_appropriate_minus[0];
  if(!next_plus)
  {//last_cor20.12.17
	  if(next_minus>0)
	  {
loop_begin=next_minus-1;
	  }
  }
  else
  {
	  if(next_minus>0)
	  {
loop_begin=std::min(next_minus,next_plus)-1;
	  }
	  else
	  {
loop_begin=next_plus-1;
	  }
  }
incline_current=*(InclinedComponent+section_num);
incline_current_local=*(InclineLocal+section_num);
if((maxim_length<0)&&(max_start_pos<0))
{//last_cor21.12.17
	if(number_of_chains_minus>1)
	{//nc>1
		if(maxim_length_minus>0)
		{//mlm>
		if((!incline_current)&&(!incline_current_local))
		{
incl_vert_length=std::min((maxim_length_minus+loop_begin+1),sect_length);
incl_vert_length_ratio=(16*incl_vert_length)/sect_length;
if(incl_vert_length_ratio>=12)
{
	if(optimal_chain_prolong_last_end>0)
	{
		if(!*(InclineLocal+section_num))
		{
incl_local=StraightApproximation(node_coordinates,paint,loop_begin,optimal_chain_prolong_last_end,
&reliability1,1);
if(reliability1>0)
{
*(InclineLocalFirst+section_num)=loop_begin;
*(InclineLocalLast+section_num)=optimal_chain_prolong_last_end;
*(InclineLocal+section_num)=incl_local;
*(InclinedComponent+section_num)=incl_local;
incline_current=*(InclinedComponent+section_num);
incline_current_local=*(InclineLocal+section_num);
}
		}
	}
}
		}
		}//mlm>
	}//nc>1
	else
	{
if(number_of_chains_minus==1)
{
	if(max_start_neg>=0)
		{//mlm>
		if((!incline_current)&&(!incline_current_local))
		{
incl_vert_length=std::min(((max_finish_neg-max_start_neg)+loop_begin+1),sect_length);
incl_vert_length_ratio=(16*incl_vert_length)/sect_length;
if(incl_vert_length_ratio>=12)
{
	if(max_finish_neg>0)
	{
		if(!*(InclineLocal+section_num))
		{
incl_local=
StraightApproximation(node_coordinates,paint,loop_begin,max_finish_neg,
&reliability1,1);
if(reliability1>0)
{
*(InclineLocalFirst+section_num)=loop_begin;
*(InclineLocalLast+section_num)=max_finish_neg;
*(InclineLocal+section_num)=incl_local;
*(InclinedComponent+section_num)=incl_local;
incline_current=*(InclinedComponent+section_num);
incline_current_local=*(InclineLocal+section_num);
}
		}
	}
}
		}
		}//mlm>
}
	}
}//last_cor21.12.17
if((maxim_length_minus<0)&&(max_start_neg<0))
{//last_cor21.12.17
	if(number_of_chains_plus>1)
	{//ncp>1
		if(maxim_length>0)
		{//ml
		if((!incline_current)&&(!incline_current_local))
		{
incl_vert_length=std::min((maxim_length+loop_begin+1),sect_length);
incl_vert_length_ratio=(16*incl_vert_length)/sect_length;
if(incl_vert_length_ratio>=12)
{
	if(optimal_chain_prolong_last_end_pos>0)
	{
		if(!*(InclineLocal+section_num))
		{
incl_local=StraightApproximation(node_coordinates,paint,loop_begin,
optimal_chain_prolong_last_end_pos,&reliability,0);
if(reliability>0)
{
*(InclineLocalFirst+section_num)=loop_begin;
*(InclineLocalLast+section_num)=optimal_chain_prolong_last_end_pos;
*(InclineLocal+section_num)=incl_local;
*(InclinedComponent+section_num)=incl_local;
incline_current=*(InclinedComponent+section_num);
incline_current_local=*(InclineLocal+section_num);
}
		}
	}
}
		}
		}//ml
	}//ncp>1
	else
	{
if(number_of_chains_plus==1)
{
	if(max_finish_pos>0)
		{//mlm>
		if((!incline_current)&&(!incline_current_local))
		{
incl_vert_length=std::min(((max_finish_pos-max_start_pos)+loop_begin+1),sect_length);
incl_vert_length_ratio=(16*incl_vert_length)/sect_length;
if(incl_vert_length_ratio>=12)
{
	if(max_finish_pos>0)
	{
		if(!*(InclineLocal+section_num))
		{
incl_local=
StraightApproximation(node_coordinates,paint,max_start_pos,max_finish_pos,
&reliability,0);
if(reliability>0)
{
*(InclineLocalFirst+section_num)=loop_begin;
*(InclineLocalLast+section_num)=max_finish_pos;
*(InclineLocal+section_num)=incl_local;
*(InclinedComponent+section_num)=incl_local;
incline_current=*(InclinedComponent+section_num);
incline_current_local=*(InclineLocal+section_num);
}
		}
	}
}
		}
		}//mlm>
}
	}
}//last_cor21.12.17
node_coord0=node_coordinates[0];
//for(count_mem=loop_begin;count_mem<sect_length-1;count_mem++)//last_cor20.12.17
for(count_mem=0;count_mem<sect_length-1;count_mem++)//last_cor20.12.17
    {//cycle members
		pos_bound=0;
	    neg_bound=0;
		node_coord1=*(node_coordinates+count_mem+1);
		node_coord=*(node_coordinates+count_mem);
			node_coord1-=node_coord;
			//node_coord1=node_coord1;
total_shift_curve+=node_coord1;
if(node_coord1>=0)
{//nc>=
	pos_shift_curve++;
	total_pos_curve+=node_coord1;
	     if(node_coord1<63)
	      {
	      node_coord1=paint[node_coord1];

	if(node_coord1<15)
	{
shift_pos[node_coord1]++;
last_pos_shift=count_mem;//last_cor24.07.17
	}
	else
	{
	shift_pos[15]++;

	}
	        }
		 else
		 {
			 node_coord1=paint[63];
			 last_pos_shift=count_mem;//last_cor24.07.17
			 if(node_coord1<15)
	         {
             shift_pos[node_coord1]++;
	         }
	         else
	         {
	          shift_pos[15]++;
	          }
		 }
}//nc>=
else
{
neg_shift_curve++;
total_neg_curve+=node_coord1;
node_coord1=abs(node_coord1);
if(node_coord1<63)
	{
node_coord1=paint[node_coord1];
if(node_coord1<15)
	{
shift_neg[node_coord1]++;
last_neg_shift=count_mem;//last_cor24.07.17
	}
   else
   {
shift_neg[15]++;
   }
     }
else
{
	node_coord1=paint[63];
	last_neg_shift=count_mem;//last_cor24.07.17
	if(node_coord1<15)
	{
shift_neg[node_coord1]++;
	}
   else
   {
shift_neg[15]++;
   }
}
}
node_coord=*(node_coordinates+count_mem);
    for(count_shift=count_mem;count_shift<sect_length-1;count_shift++)
        {//cycle shift
hist_arg=*(node_coordinates+count_shift+1)-node_coord;
hist_arg1=*(node_coordinates+count_shift+1)-*(node_coordinates+count_shift);
if(hist_arg1>0)
{
if(pos_bound>=0)
{//last_cor25.07.17
pos_bound++;
}
}
else
{
	if(pos_bound>0)
	{
longest_positive_adjacent_interval[count_mem]=pos_bound;
	}
	pos_bound=-1;
}
if(hist_arg1<0)
{
if(neg_bound>=0)
{//last_cor25.07.17
neg_bound++;
}
}
else
{
if(neg_bound>0)
	{
longest_negative_adjacent_interval[count_mem]=neg_bound;
	}
	neg_bound=-1;
}
if((address_beginning+(count_shift-count_mem))<dimen_links)
{
//*(inclination_links+address_beginning+(count_shift-count_mem))=hist_arg/(count_shift-count_mem+1);
*(inclination_links+address_beginning+(count_shift-count_mem))=hist_arg;
}
else
{
	prior--;
}

if(count_shift==count_mem)
{
differ_array[count_mem]=hist_arg;
hist_arg_div=hist_arg;
}
else
{
	shift_divisor=count_shift+1-count_mem;
	residual=abs(hist_arg)%shift_divisor;
	hist_arg_div=hist_arg/shift_divisor;
	if(2*residual>shift_divisor)
	{
		if(hist_arg>=0)
		{
		hist_arg_div++;
		}
		else
		{
			hist_arg_div--;
		}
	}
}
//hist_arg_div=hist_arg_div/2;
if(hist_arg>=0)
{//ha
	count_pos++;
	if(hist_arg_div<63)
	{
		hist_pos[hist_arg_div]++;
		hist_arg_pos_incl=paint[hist_arg_div];
		if(hist_arg_pos_incl>=2)
		{//last_cor24.07.17
pos_deviation_max[count_mem]++;
		}
	}
	else
	{
		hist_pos[63]++;
	}
}//ha
else
{//e1

	count_neg++;
	if(!hist_arg_div)
	{
		hist_neg[hist_arg_div]++;
	}
	else
	{
      abs_arg=-hist_arg_div;
        if(abs_arg<63)
	    {
		hist_neg[abs_arg]++;
		hist_arg_neg_incl=paint[abs_arg];
		if(hist_arg_neg_incl>=2)
		{//last_cor24.07.17
neg_deviation_max[count_mem]++;
		}
	    }
	    else
	    {
		hist_neg[63]++;
	    }
	}
}//e1
	}//cycle shift
if(pos_bound>0)
	{
longest_positive_adjacent_interval[count_mem]=pos_bound;
	}
if(neg_bound>0)
	{
longest_negative_adjacent_interval[count_mem]=neg_bound;
	}
	address_beginning+=(sect_length-1-count_mem);
    }//cycle members
max_length_pos=FindingMaxPosNegSegments(sect_length,longest_positive_adjacent_interval,&max_start_pos);
max_length_neg=FindingMaxPosNegSegments(sect_length,longest_negative_adjacent_interval,&max_start_neg);
	medium_step=total_shift_curve/(sect_length-1);
    medium_step_pl=total_pos_curve/(sect_length-1);
	medium_step_mn=total_neg_curve/(sect_length-1);
	if(total_pos_curve!=0)
	{
	total_pos_ratio=(16*total_shift_curve)/total_pos_curve;
	if((neg_shift_curve==1)&&(total_shift_curve>0)&&(medium_step_pl>StripWidth))
		{//last_cor116.11.16
        total_pos_ratio=14;
		}
	}
	else
	{
    total_pos_ratio=-1;
	}
    if(total_neg_curve!=0)
	{
	total_neg_ratio=(16*total_shift_curve)/total_neg_curve;
	if(total_neg_ratio<0)
	{//last_cor01.11.16//last_cor116.11.16
		if((pos_shift_curve==1)&&(total_shift_curve<0)&&(medium_step_mn<StripWidth))
		{
        total_neg_ratio=14;
		}
	}
	}
	else
	{
    total_neg_ratio=-1;
	}

//if(section_num<num_left_sections)
//{//s_left
	    if(total_neg_ratio>0)
		{//tnr>0
	if(!pos_shift_curve)
	{//!psh
		InclineCurve[section_num]=-1;
	}//!psh
	else
	{//e!psh
		if(total_neg_ratio>=15)
		{
			InclineCurve[section_num]=-2;
		}
		else
		{//e15
		    if(total_neg_ratio>=13)
		    {
			InclineCurve[section_num]=-3;
		    }
		    else
		   {//e13
             if(total_neg_ratio>=11)
		    {
			InclineCurve[section_num]=-4;
		    }
		   }//e13
		}//e15
	}//e!psh
		}//tnr>0
		if(total_pos_ratio>0)
		{//tpr>0
	if(!neg_shift_curve)
	{//!psh
		InclineCurve[section_num]=1;
	}//!psh
	else
	{//e!psh
		if(total_pos_ratio>=15)
		{
			InclineCurve[section_num]=2;
		}
		else
		{//e15
		    if(total_pos_ratio>=13)
		    {
			InclineCurve[section_num]=3;
		    }
		    else
		   {//e13
             if(total_pos_ratio>=11)
		    {
			InclineCurve[section_num]=4;
		    }
		   }//e13
		}//e15
	}//e!psh
		}//tpr>0
//}//s_left
/*else
{
	if(!neg_shift_curve)
	{//nsh0
		InclineCurve[section_num]=1;
	}
	else
	{
		if(total_pos_ratio>=15)
		{
			InclineCurve[section_num]=1;
		}
		else
		{
		if(total_pos_ratio>=13)
		{
			InclineCurve[section_num]=2;
		}
		else
		{
             if(total_pos_ratio>=11)
		     {
			  InclineCurve[section_num]=3;
		     }
		}
		}
	}
}*/
total_num_pos=CenterOfMass(hist_pos,64,&center_m_pos,&dev_pos,&left_number_pos,&right_number_pos,
&small_angle_pos,&big_angle_pos,paint,hist_box_pos,dim_new_hist);
total_num_neg=CenterOfMass(hist_neg,64,&center_m_neg,&dev_neg,&left_number_neg,&right_number_neg,
&small_angle_neg,&big_angle_neg,paint,hist_box_neg,dim_new_hist);
if(total_num_pos)
{
maximum_pos=MaximumTripleMaximum(hist_pos,hist_neg,64,&triple_maximum_pos,
&maximum_value_pos,&triple_maximum_value_pos,&one_pix_neighborhood_pos,&two_pix_neighborhood_pos,
&zero_two_pix_neighb);
maximum_pos_red=MaximumTripleMaximumReduced(hist_box_pos,dim_new_hist,&triple_maximum_pos_red,
&maximum_value_pos_red,&triple_maximum_value_pos_red,&one_pix_neighborhood_pos_red,
&two_pix_neighborhood_pos_red,&zero_two_pix_neighb_red,local_maxim_points,&num_loc_max,
&second_max_pos,&second_max_value,&beg_noticable,&end_noticable,&noticable_sum);
max_pos_weight=(32*maximum_value_pos)/(total_num_pos);
small_angle_weight_pos=(32*small_angle_pos)/(total_num_pos);
one_max_pos_weight=(32*one_pix_neighborhood_pos)/(total_num_pos+total_num_neg);
noticable_sum_weight=(32*noticable_sum)/(total_num_neg+total_num_pos);

   if(beg_noticable>=0)
   {
   noticable_width=end_noticable-beg_noticable+1;
   }
}
if(total_num_neg)
{
maximum_neg=MaximumTripleMaximum(hist_neg,hist_pos,64,&triple_maximum_neg,
&maximum_value_neg,&triple_maximum_value_neg,&one_pix_neighborhood_neg,&two_pix_neighborhood_neg,
&zero_two_pix_neighb_neg);
maximum_neg_red=MaximumTripleMaximumReduced(hist_box_neg,dim_new_hist,&triple_maximum_neg_red,
&maximum_value_neg_red,&triple_maximum_value_neg_red,&one_pix_neighborhood_neg_red,
&two_pix_neighborhood_neg_red,&zero_two_pix_neighb_neg_red,local_maxim_points,&num_loc_max_neg,
&second_max_pos_neg,&second_max_value_neg,&beg_noticable_neg,&end_noticable_neg,&noticable_sum_neg);
max_neg_weight=(32*(maximum_value_neg))/(total_num_neg);
small_angle_weight_neg=(32*small_angle_neg)/(total_num_neg);
one_max_neg_weight=(32*one_pix_neighborhood_neg)/(total_num_neg+total_num_pos);
noticable_sum_neg_weight=(32*noticable_sum_neg)/(total_num_neg+total_num_pos);

if(beg_noticable_neg>=0)
   {
   noticable_width_neg=end_noticable_neg-beg_noticable_neg+1;
   }
}
small_angle=small_angle_pos+small_angle_neg;
pos_fraction=(32*total_num_pos)/(total_num_pos+total_num_neg);
small_angle_weight=(32*small_angle)/(count_pos+count_neg);
left_right_weight=(16*count_pos)/(count_pos+count_neg);
if((count_pos>0)&&(count_neg>0))
{
max_pos_neg_weight=(32*(maximum_value_pos+maximum_value_neg))/(count_pos+count_neg);
vertical_weight=(32*zero_two_pix_neighb)/(count_pos+count_neg);
}
else
{
    if(count_pos>0)
    {
     max_pos_neg_weight=(32*(maximum_value_pos))/(count_pos);
	 vertical_weight=(32*zero_two_pix_neighb)/(count_pos);
     }
      else
       {
        max_pos_neg_weight=(32*maximum_value_neg)/(count_neg);
		vertical_weight=(32*zero_two_pix_neighb_neg)/(count_neg);
       }
}
/*if(count_pos)
{
max_pos_weight=(32*maximum_value_pos)/(count_pos);
small_angle_weight_pos=(32*small_angle_pos)/(count_pos);
one_max_pos_weight=(32*one_pix_neighborhood_pos)/(count_pos);
}
if(count_neg)
{
max_neg_weight=(32*(maximum_value_neg))/(count_neg);
small_angle_weight_neg=(32*small_angle_neg)/(count_neg);
one_max_neg_weight=(32*one_pix_neighborhood_neg)/(count_neg);
}*/
small_angle_weight_red=(32*(hist_box_neg[0]+hist_box_pos[0]))/(count_pos+count_neg);
one_max_pos_neg_weight=(32*(one_pix_neighborhood_pos+one_pix_neighborhood_neg))/(count_pos+count_neg);
two_max_pos_neg_weight=(32*(two_pix_neighborhood_pos+two_pix_neighborhood_neg))/(count_pos+count_neg);
left_right_weight_red=(16*(maximum_value_pos_red+maximum_value_neg_red))/(total_num_pos+total_num_neg);
max_pos_neg_weight_red=
(32*(maximum_value_pos_red+maximum_value_neg_red))/(total_num_pos+total_num_neg);
if(total_num_pos>0)
{//last_cor03.08.17
max_pos_weight_red=
(32*maximum_value_pos_red)/(total_num_pos);
}
if(total_num_neg>0)
{
max_neg_weight_red=
(32*maximum_value_neg_red)/(total_num_neg);
}
one_max_pos_neg_weight_red=
(32*(one_pix_neighborhood_pos_red+one_pix_neighborhood_neg_red))/(total_num_pos+total_num_neg);
one_max_neg_weight_red=
(32*one_pix_neighborhood_neg_red)/(total_num_pos+total_num_neg);
if(total_num_neg>0)
{//last_cor03.0817
one_max_neg_weight_red1=
(32*one_pix_neighborhood_neg_red)/(total_num_neg);
}
one_max_pos_weight_red=
(32*one_pix_neighborhood_pos_red)/(total_num_pos+total_num_neg);
if(total_num_pos>0)
{
one_max_pos_weight_red1=
(32*one_pix_neighborhood_pos_red)/(total_num_pos);
}
two_max_pos_neg_weight_red=
32*(two_pix_neighborhood_pos_red+two_pix_neighborhood_neg_red)/(total_num_pos+total_num_neg);

    if((vertical_weight>=30)||(small_angle_weight>=30))
    {
		*(VerticalComponent+section_num)=1;
	}
	else
	{
		if((vertical_weight>=26)||(small_angle_weight>=26))
       {
		*(VerticalComponent+section_num)=2;
	    }
		else
		{
			if((vertical_weight>=22)||(small_angle_weight>=22))
           {
		   *(VerticalComponent+section_num)=3;
	       }
			else
			{
				if((vertical_weight>=18)||(small_angle_weight>=18))
                {
		        *(VerticalComponent+section_num)=4;
	            }
				else
				{
					if((vertical_weight>=12)||(small_angle_weight>=12))
                    {
		            *(VerticalComponent+section_num)=5;
	                }
					else
					{
						if((vertical_weight>=6)||(small_angle_weight>=6))
                       {
		               *(VerticalComponent+section_num)=6;
	                   }
					}
				}
			}
		}
	}
	vertical_comp=*(VerticalComponent+section_num);
if((maximum_pos_red<=0)&&(maximum_neg_red<=0))
{//00
if((pos_fraction>=0)&&(pos_fraction<=10))
	{//ps010

if((maximum_pos<=1)&&(maximum_neg<=1))
{//firstcond1

		  if(one_max_pos_neg_weight>=30)
		  {
			  *(StraightSections+section_num)=2;//vertical line with very slow incline
			goto K;
		  }
		  else
		  {
			  if(one_max_pos_neg_weight>=28)
		     {
			      if((!noticable_sum_neg)&&(!noticable_sum))
			      {
			       *(StraightSections+section_num)=2;//vertical line with very slow incline
			      goto K;
			      }
		     }
			  else
			  {
				  if(!noticable_sum)
				  {
					  if((second_max_pos_neg>0)&&(second_max_value_neg>=3))
					  {
 if((noticable_sum_neg_weight>=10)&&((noticable_sum_neg_weight+one_max_pos_neg_weight)>=26))
						  {
  *(StraightSections+section_num)=121;//vertical line with sharp negative incline line
  if(!*(InclinedComponent+section_num))
  {
	*(InclinedComponent+section_num)=-second_max_pos_neg;
  }
			goto K;

						  }
					  }
				  }
			  }

		  }
}//firstcond1
else
{
         *(StraightSections+section_num)=21;//very slow negative incline
			goto K;
}

			if(small_angle_weight>=27)
	         {
             *(StraightSections+section_num)=22;//very slow negative incline
			goto K;
	         }
			else
			{
				if(small_angle_weight>=24)
	         {
             *(StraightSections+section_num)=23;//very slow negative incline with small deviations
			goto K;
	         }
				else
				{
					if(small_angle_weight>=16)
	                {
						if(!noticable_sum)
						{
							if((second_max_pos_neg>1)&&(second_max_value_neg>=3))
							{
								if((noticable_width_neg>3)&&(noticable_sum_neg_weight>=16))
								{
			*(StraightSections+section_num)=121;//vertical line with sharp negative incline line
			if(!*(InclinedComponent+section_num))
			{
	        *(InclinedComponent+section_num)=-second_max_pos_neg;
			}
			goto K;

								}

							}
						}

					}
				}
			}


		if((maximum_pos<=0)&&(maximum_neg<=0))
		{//mp0mn0
			if((max_pos_weight>18)&&(small_angle_weight>20)&&(one_max_pos_neg_weight_red>22))
             {
             *(StraightSections+section_num)=25;//straight as asubset
			goto K;
            }
if((max_pos_neg_weight>=10)&&(small_angle_weight>=26)&&(one_max_pos_neg_weight_red>20))
{
*(StraightSections+section_num)=26;//straight line as a subset
			goto K;
}
		}//mp0mn0
if(!pos_fraction)
		{//pf=0
if(!maximum_neg_red)
{
	if(one_max_pos_neg_weight_red>30)
	{
*(StraightSections+section_num)=27;//with deviation slow negative incline
	}
	else
	{
if(two_max_pos_neg_weight_red>30)
	{
*(StraightSections+section_num)=28;//with deviation slow negative incline
	}
	}
}
		}//pf=0
    }//ps010
else
{//e010
	if(pos_fraction>=21)
	{//pf21
if(pos_fraction>=28)
{//last_cor19.09.14neg=0
	if(!maximum_pos_red)
	{//mpr=0
		if(max_pos_neg_weight_red==32)
		{//mpnwr=0

			if((max_pos_neg_weight>=28)||(one_max_pos_neg_weight>=30))
			{
			//*(VerticalComponent+section_num)=1;
			*(StraightSections+section_num)=2;//vertical line
			goto K;
			}
			else
			{
			          if(one_max_pos_neg_weight>=26)
		               {//om26
				if(two_max_pos_neg_weight>=30)
				{
					if(second_max_pos<0)
					{
				//*(VerticalComponent+section_num)=2;
                 *(StraightSections+section_num)=21;//vertical line
			goto K;
					}
				}
			            }//om26

			}

		}//mpnwr=0
        else
		    {

					if((second_max_pos<0)||(!noticable_sum_weight))
					{
						if(!maximum_pos)
						{//mp0
							if(one_max_pos_weight_red>=30)
				             {
							*(StraightSections+section_num)=21;//vertical with deviations
							goto K;
							}
							else
							{
								if(one_max_pos_weight_red>=26)
				             {
							*(StraightSections+section_num)=22;//vertical with deviations
							goto K;
							}
							}
						}//mp0
						else
						{
							if(one_max_pos_weight_red>=30)
				             {
							*(StraightSections+section_num)=22;//vertical with deviations
							goto K;
							}
						}
					}

if(((one_max_pos_neg_weight>=12)&&(max_pos_neg_weight>=8))||((one_max_pos_neg_weight>=10)&&
   (small_angle_weight>=12)))
		               {//om14
						   if((noticable_width>=3)&&(second_max_pos>1))
						   {
		 if((noticable_sum_weight>=8)||(((one_max_pos_neg_weight+noticable_sum_weight)>=26)&&
			 (second_max_value>=3)))
							   {//ns
*(StraightSections+section_num)=131;
//vertical straight line as asubset and another line with positive incline
if(!*(InclinedComponent+section_num))
{
*(InclinedComponent+section_num)=second_max_pos;
}
goto K;

							   }
							   else
							   {
                               if(noticable_sum_weight>=2)
							   {
*(StraightSections+section_num)=132;
//vertical straight line as asubset and another line with positive incline
goto K;
							   }
							   }//ns
						   }
						   else
						   {
                        if((noticable_width>=2)&&(second_max_pos<0))
						 {
 *(StraightSections+section_num)=136;//straight as asubset and possible another line
			             goto K;
						 }
						   }
                       }//om14
		    }
	}//mpr=0
}//last_cor19.09.14neg=0
if(!maximum_pos_red)
	{//mpr1=0
		if((maximum_pos<=0)&&(maximum_neg<=0))
		{//mpmn0
          if(one_max_pos_neg_weight>=30)
		  {
			  *(StraightSections+section_num)=2;//vertical straight line
			goto K;
          }
		  else
		  {
          if(one_max_pos_neg_weight>=26)
		  {
			  *(StraightSections+section_num)=21;//vertical straight line with small deviations
			goto K;
          }
		  }
		}//mpmn0
if((maximum_pos<=1)&&(maximum_neg<=1))
{//firstcond2

		  if(one_max_pos_neg_weight>=30)
		  {//omw30
			  if(max_pos_neg_weight>=28)
			  {
			  *(StraightSections+section_num)=2;//straight line
			goto K;
			  }
			  else
			  {
*(StraightSections+section_num)=3;//straight line with positive incline
			goto K;
			  }
		  }
		  else
		  {
			  if(one_max_pos_neg_weight>=27)
		      {
*(StraightSections+section_num)=31;
//straight line with slight positive incline and with small deviations
			goto K;
			  }
			    else
			    {
if(one_max_pos_neg_weight>=22)
  {
	if((second_max_pos<0)&&(second_max_pos_neg<0))
	 {
*(StraightSections+section_num)=61;//vertical component without neg and pos inclined components
			goto K;
	 }
  }
		  else
		  {
			if(one_max_pos_neg_weight>=16)
		      {
				  if((second_max_pos<0)&&(second_max_pos_neg<0))
				  {
*(StraightSections+section_num)=62;//vertical component without neg and pos inclined components
			goto K;
				  }

			  }
		  }
                }
		  }//omw30
}//firstcond2

	if(small_angle_weight>=30)
	         {
         *(StraightSections+section_num)=31;//very slow positive incline
			goto K;
	         }
		     else
		    {//e30
			if(small_angle_weight>=27)
	         {
             *(StraightSections+section_num)=32;//very slow positive incline
			goto K;
	         }
			else
			{//e27
				if(small_angle_weight>=24)
	         {//sa24
				  if(triple_maximum_pos<=1)
                {
					if((!noticable_sum_neg_weight))
					{
    if(two_max_pos_neg_weight>=28)
	{
		if((noticable_width>=1)&&(noticable_width<=2)&&(one_max_pos_neg_weight_red>=28))

*(StraightSections+section_num)=130;//line with slow positive incline with possible small deviations
			goto K;
	}
				    }
					else
if((noticable_sum_neg_weight<=6))
					{
    if(two_max_pos_neg_weight>=24)
	{
		if((noticable_width>=1)&&(noticable_width<=3)&&(one_max_pos_neg_weight_red>=28))

*(StraightSections+section_num)=131;//line with slow positive incline with possible deviations
			goto K;
	}
				    }					{

					}
				 }
	         }//sa24
				else
				{//e24
          if(small_angle_weight>=18)
	         {//sa18
				 if((!noticable_sum_neg)&&(!noticable_sum))
				 {
          if(one_max_pos_neg_weight>=18)
		  {
			  *(StraightSections+section_num)=131;//line with slow positive incline with possible deviations
			goto K;
		  }
				 }
             }//sa18
				}//e24
			}//e27

		    }//e30
}//mpr1=0
		if((maximum_pos<=0)&&(maximum_neg<=0))
		{//mp0mn0
			if((max_pos_weight>18)&&(small_angle_weight>20)&&(one_max_pos_neg_weight_red>22))
             {
				 if(num_loc_max<=1)
				 {
             *(StraightSections+section_num)=35;//straight as asubset
			goto K;
				 }
				 else
				 {
					 if(num_loc_max>=2)
					 {
						 if((second_max_pos>1)&&(second_max_value)>=3)
						 {
		 *(StraightSections+section_num)=135;//straight as a subset and another line
			goto K;
						 }
					 }
					 else
					 {
						 if(noticable_width>=2)
						 {
 *(StraightSections+section_num)=136;//straight as asubset and possible another line
			             goto K;
						 }
					 }
				 }
            }
if((max_pos_neg_weight>=10)&&(small_angle_weight>=16)&&(one_max_pos_neg_weight_red>20))
{
	             if(num_loc_max<=1)
				 {
*(StraightSections+section_num)=36;//straight line as a subset
			goto K;
	             }
				 else
				 {
					 if(num_loc_max>=2)
					 {
						 if((second_max_pos>1)&&(second_max_value)>=3)
						 {
		 *(StraightSections+section_num)=136;//straight as asubset and another line
			goto K;
						 }
					 }
				 }

}
		}//mp0mn0
	}//pf21
	else
	{//epf21
		if(small_angle_weight>=30)
	    {
if((maximum_pos<=1)&&(maximum_neg<=1))
{//firstcond3
	if((maximum_pos<=0)&&(maximum_neg<=0))
	{
         if(one_max_pos_neg_weight>=30)
		  {
			  *(StraightSections+section_num)=1;//very straight
			goto K;
		  }
		 else
		 {
			 if(one_max_pos_neg_weight>=26)
		  {
			  *(StraightSections+section_num)=11;//very straight
			goto K;
		  }
		 }
	}

		  if(one_max_pos_neg_weight>=30)
		  {
			  *(StraightSections+section_num)=1;//very straight
			goto K;
		  }
		  else
		  {
            if(one_max_pos_neg_weight>=26)
		    {
				*(StraightSections+section_num)=11;//straight with small deviations
			goto K;
			}
		  }
}//firstcond3
else
{
         *(StraightSections+section_num)=11;//straight
			goto K;
}
	    }
		else
		{//e30
			if(small_angle_weight>=27)
	         {//saw27
				 if((!noticable_sum_neg)&&(!noticable_sum))
				 {
             *(StraightSections+section_num)=12;//almost straignt
			goto K;
				 }
				 else
				 {
				 *(StraightSections+section_num)=13;//almost straignt
			     goto K;
				 }
	         }//saw27
			else
			{//esaw27
				if(vertical_comp>0)
	         {//vc>0
				 if(one_max_pos_neg_weight_red>=30)
				 {//ompnwr30
				 if((second_max_pos<0)&&(second_max_pos<0))
				 {
                 *(StraightSections+section_num)=13;//straight line with deviations
			     goto K;
				 }
				 else
				 {
 if((noticable_sum_neg<=3)&&(noticable_sum<=3)&&(second_max_value<3)&&(second_max_value_neg<3))
	 {
		 *(StraightSections+section_num)=14;//straight line with deviations
			goto K;
	 }
				 }
				 }//ompnwr30
				 else
				 {//eompnwr30
				 if(one_max_pos_neg_weight_red>=26)
					 {//omwr26
	if((!noticable_sum_neg)&&(!noticable_sum))
	 {
     *(StraightSections+section_num)=14;//straight line with deviations
			goto K;
	 }
	else
	{
		if((!noticable_sum_neg)&&(second_max_value<3)&&(max_pos_neg_weight_red>18))
		{
			*(StraightSections+section_num)=15;//straight line with big deviations
			goto K;
		}
	}
					 }//omwr26
				 else
				 {//eomwr26
					 if(one_max_pos_neg_weight_red>=24)
					 {//omwr24
 if((!noticable_sum_neg)&&(!noticable_sum))
	 {
     *(StraightSections+section_num)=15;//straight line with deviations
			goto K;
	 }
	else
	{
		if((!noticable_sum_neg)&&(second_max_value<3)&&(max_pos_neg_weight_red>18))
		{
			*(StraightSections+section_num)=16;//straight line with big deviations
			goto K;
		}
	}
					 }//omwr24
					 else
					 {
 if(one_max_pos_neg_weight_red>=16)
					 {//omwr16
           if(noticable_sum_weight<=2)
		   {
   if(noticable_sum_neg_weight>=10)
		{
			if(second_max_value_neg>=3)
			{
				if(second_max_pos_neg>=1)
				{
			*(StraightSections+section_num)=110;//straight line with a negative inclined line
			if(!*(InclinedComponent+section_num))
			{
*(InclinedComponent+section_num)=-second_max_pos_neg;
			}
goto K;
				}
			}
		}
		   }
					 }//omwr16
					 }

				 }//eomwr26
				 }//eompnwr30

	         }//vc>0
			 else
			 {
if((maximum_pos<=0)&&(maximum_neg<=0))
		{//mp0mn0
if((max_pos_weight>18)&&(small_angle_weight>20)&&(one_max_pos_neg_weight_red>22))
{
*(StraightSections+section_num)=15;//straight as asubset
			goto K;
}
if((max_pos_neg_weight>=10)&&(small_angle_weight>=16)&&(one_max_pos_neg_weight_red>20))
{
*(StraightSections+section_num)=16;//straight line as a subset
			goto K;
}
		}//mp0mn0
			}
			}//esaw27
		}//e30
	}//epf21
}//e012
	if(!total_num_neg)
	{//tng0
		if(maximum_pos>0)
		{//mp>0
			if(one_max_pos_neg_weight>30)
			{
				*(StraightSections+section_num)=23;//slow negative incline
			goto K;
			}
			else
			{
               if(one_max_pos_neg_weight>25)
			   {
			   *(StraightSections+section_num)=24;//slow positive incline
			    goto K;
			   }
			}
		}//mp>0

	}//tng0
}//00
if((maximum_pos_red<=0)&&(maximum_neg_red>0))
{//0k
//if(small_angle_weight_pos>=30)
//{//smw30
	/*if((pos_fraction>=0)&&(pos_fraction<=10))
				  {
*(StraightSections+section_num)=25;//strong vertical straight component
			    goto K;
	              }
	else
	{
		if(pos_fraction>21)
		{
*(StraightSections+section_num)=35;//strong vertical straight component
			    goto K;
		}
		else
		{
*(StraightSections+section_num)=25;//strong vertical straight component
			    goto K;
	    }
	}*/
//}//smw30
//else
//{//smw30
	/*if(small_angle_weight_pos>=26)
    {
if((pos_fraction>=0)&&(pos_fraction<=10))
				  {
*(StraightSections+section_num)=16;//strong vertical straight component
			    goto K;
	              }
	else
	{
		if(pos_fraction>21)
		{
*(StraightSections+section_num)=36;//strong vertical straight component
			    goto K;
		}
		else
		{
*(StraightSections+section_num)=36;//strong vertical straight component
			    goto K;
	    }
	}

	}*/
	//else
	//{//smw26

				  if((pos_fraction>=0)&&(pos_fraction<=10))
				  {//pf010
					  if(maximum_neg_red>1)
					  {//mnr>1
						  if(!vertical_comp)
						  {//vc0
							  if(pos_fraction<=3)
							  {
								  if(one_max_neg_weight_red>=30)
								  {//omnw32
									  if(one_max_neg_weight>=28)
									  {
  *(StraightSections+section_num)=100;//a rather big negative inclined component
  if(!*(InclinedComponent+section_num))
  {
	*(InclinedComponent+section_num)=-maximum_neg_red;
	  goto K;
  }
									  }
								  }//omnw32
								  else
								  {
 if((one_max_neg_weight_red>=16)&&(one_max_neg_weight_red<=22))
 {
*(StraightSections+section_num)=105;//a negative inclined component
if(!*(InclinedComponent+section_num))
{
	*(InclinedComponent+section_num)=-maximum_neg_red;
}
 }
								  }
							  }
						  }//vc0
						  if(one_max_neg_weight_red>8)
						  {

							  if((small_angle_weight+noticable_sum_neg_weight)>=26)
							  {
								  if(vertical_comp>0)
								  {
  *(StraightSections+section_num)=111;//a rather big negative inclined component
  goto K;
								  }
							  }
						  }
					  }//mnr>1
					  else
					  {//mnr=1
						  if(!vertical_comp)
						  {//vc0
							  if(pos_fraction<=3)
							  {
								  if(one_max_neg_weight_red>=30)
								  {//omnw
									  if((one_max_neg_weight>=28)&&(maximum_neg>1))
									  {
  *(StraightSections+section_num)=100;//a  negative slow inclined component
  if(!*(InclinedComponent+section_num))
  {
  *(InclinedComponent+section_num)=-maximum_neg_red;
  }
  goto K;
									  }
									  else
									  {
*(StraightSections+section_num)=101;//a  negative slow inclined component
if(!*(InclinedComponent+section_num))
{
  *(InclinedComponent+section_num)=-maximum_neg_red;
}
  goto K;
									  }
								  }//omnw
							  }
						  }//vc0
		  if((noticable_width_neg>3)&&((small_angle_weight+noticable_sum_neg_weight)>=26))
						{
							if(!noticable_sum_weight)
							{
								if(vertical_comp>0)
								{
*(StraightSections+section_num)=111;//rather big almost vertical component and an inclined component
  goto K;
								}
							}
							else
							{
								if(vertical_comp>0)
								{
*(StraightSections+section_num)=112;//rather big almost vertical component, incline component+dev
  goto K;
								}
							}
					    }
					  }//mnr=1
		           }//pf010

	//}//esmw26
//}//esmw30
}//0k
if(maximum_pos_red>=1)
{//pos_red
	if(2*max_length_pos>sect_length)
		{//last_cor03.08.17
if(3*max_start_pos<sect_length)
{
	if((max_length_neg<sect_length/4)||(one_max_neg_weight_red<5))
	{
		if((one_max_pos_weight_red1>27)&&(max_pos_weight_red>22))
		{
			if(!*(InclinedComponent+section_num))
			{
		*(InclinedComponent+section_num)=maximum_pos_red;//last_cor03.08.17
			}
		}
	}
}
		}//last_cor03.08.17
	if((4*noticable_sum_neg<noticable_sum)&&(noticable_sum_neg_weight<5)&&
			(noticable_sum_weight>10))
		{
			if(!*(InclinedComponent+section_num))
			{
*(InclinedComponent+section_num)=maximum_pos_red;//last_cor29.10.14
			}
		}
	if((pos_fraction>28)||(!noticable_sum_neg))
	{//pos>
if((one_max_pos_neg_weight_red>=28)||((maximum_pos_red>=2)&&(one_max_pos_neg_weight_red>25)))
{
	if(small_angle_weight<2)
	{
*(StraightSections+section_num)=300;//noticable positive incline
if(!*(InclinedComponent+section_num))
{
*(InclinedComponent+section_num)= maximum_pos_red;//last_cor29.10.14
}
goto K;
	}
	else
	{
		if(small_angle_weight<5)
	   {
	*(StraightSections+section_num)=301;//noticable positive incline
	if(!*(InclinedComponent+section_num))
	{
	*(InclinedComponent+section_num)=maximum_pos_red;
	}
			goto K;
		}
		else
		{
*(StraightSections+section_num)=302;//noticable positive incline
if(!*(InclinedComponent+section_num))
{
*(InclinedComponent+section_num)=maximum_pos_red;
}
			goto K;
		}
	}
}
else
{
if(one_max_pos_neg_weight_red>25)
{
	*(StraightSections+section_num)=102;//noticable positive incline
			goto K;
}
}
if(two_max_pos_neg_weight_red>=28)
{
	*(StraightSections+section_num)=111;//noticable positive incline
			goto K;
}
else
{
if(two_max_pos_neg_weight_red>25)
{
	*(StraightSections+section_num)=112;//noticable possible incline
			goto K;
}
}
	}//pos>

}//pos_red
if(maximum_neg_red>=1)
{//ntg_red
	    if(2*max_length_neg>sect_length)
		{//last_cor03.08.17
if(3*max_start_neg<sect_length)
{
	if((max_length_pos<sect_length/4)||(one_max_pos_weight_red<5))
	{
		if((one_max_neg_weight_red1>27)&&(max_neg_weight_red>22))
		{
			if(!*(InclinedComponent+section_num))
			{
		*(InclinedComponent+section_num)=-maximum_neg_red;//last_cor03.08.17
			}
		}
	}
}
		}//last_cor03.08.17
	if((pos_fraction>=0)&&(pos_fraction<=10))
	{
		if((noticable_sum_neg>4*noticable_sum)&&(noticable_sum_neg_weight>10)&&
			(noticable_sum_weight<5))
		{
			if(!*(InclinedComponent+section_num))
			{
*(InclinedComponent+section_num)=-maximum_neg_red;//last_cor29.10.14
			}
		}
if(one_max_pos_neg_weight_red>=28)
{
	if(!small_angle_weight)
	{
		if(!pos_fraction)
		{
	*(StraightSections+section_num)=100;//noticable negative incline

	                       goto K;
		}
		else
		{
		if((!noticable_sum)&&(second_max_pos<0))
						   {
	*(StraightSections+section_num)=101;//noticable negative incline
	if(!*(InclinedComponent+section_num))
	{
	//*(InclinedComponent+section_num)=maximum_neg_red;
	}
	                       goto K;
		                   }
		}

	}

}
else
{
if(one_max_pos_neg_weight_red>25)
{
	if(small_angle_weight>=10)
	{
		if(maximum_pos<=0)
		{
	*(StraightSections+section_num)=212;//possible straight with noticable negative incline
			goto K;
		}
	}
}
}
if(two_max_pos_neg_weight_red>=28)
{
	*(StraightSections+section_num)=103;//noticable negative incline
			goto K;
}
else
{
if(two_max_pos_neg_weight_red>25)
{
	*(StraightSections+section_num)=104;//noticable negative incline
			goto K;
}
else
{
	if((one_max_pos_neg_weight_red>=18)&&(two_max_pos_neg_weight_red>22))
	{
		*(StraightSections+section_num)=105;//noticable negative incline
	}
}
}
	}
}//neg_red
//if(count_pos>count_neg)
//{//cp>cn
if(!total_num_pos)
{//tnp=0
 if((total_num_neg-small_angle_neg)<=(small_angle_neg/8))
 {//tnn-smn
if((triple_maximum_neg>=0)&&(triple_maximum_neg<=1))
  {
	   *(StraightSections+section_num)=11;//very slow negative incline
			goto K;
  }
else
{
	if((triple_maximum_neg>=1)&&(triple_maximum_neg<=3))
   {
	   *(StraightSections+section_num)=12;//slow negative incline
			goto K;
	}

}
 }//tnn-smn
}//tnp=0
if((total_num_neg-small_angle_neg)<=(small_angle_neg/8))
 {//spectnn-smn
	 if(total_num_pos>4*total_num_neg)
	 {
if((left_right_weight_red>=8)&&(max_pos_neg_weight_red>=16))
{
	if(one_max_pos_neg_weight_red>=24)
	{
		if(maximum_pos_red>1)
		{
*(StraightSections+section_num)=91;
goto K;
		}
		else
		{
			*(StraightSections+section_num)=1;
			goto K;
		}

	}
}
	 }
 }


	if(count_neg>4*count_pos)
	{//cn>>>cp
if(maximum_neg==triple_maximum_neg)
{//mp=tmp
           if(4*one_pix_neighborhood_neg>3*count_neg)
			{
				*(StraightSections+section_num)=31;
					goto K;
			}
}//mp=tmp
	}//cn>>>cp
	else
	{
if(two_max_pos_neg_weight>=30)
		  {
			  *(StraightSections+section_num)=41;
			goto K;
		  }
		  else
		  {//tmp
			  if(two_max_pos_neg_weight>=25)
		     {
			  *(StraightSections+section_num)=42;
			goto K;
		      }
			  else
			  {
				  if(two_max_pos_neg_weight>=23)
		     {
			  *(StraightSections+section_num)=43;
			goto K;
		      }
			  }

		  }//tmp
	}
//}//cn>=cn
	if((!maximum_pos_red)&&(!maximum_neg_red))
{//mr0
    if((pos_fraction>=0)&&(pos_fraction<=10))
	{//ps010
		/*if((!maximum_pos)&&(!maximum_neg))
		{
if(one_max_pos_neg_weight_red>=28)
{
}
		}*/
		if(one_max_pos_neg_weight_red>=28)
        {
       *(StraightSections+section_num)=111;//slow negative incline
	   goto K;
		}
		else
		{
			if(one_max_pos_neg_weight_red>25)
           {
	*(StraightSections+section_num)=132;//slow negative incline with oscillation
			goto K;
           }
			else
			{
				if(one_max_pos_neg_weight_red>=23)
           {
			   if(two_max_pos_neg_weight_red>=28)
			   {
	*(StraightSections+section_num)=133;//slow negative incline with oscillation
			goto K;
			   }
           }
			}
		}

    }//ps010
	else
	{//e1
		if(pos_fraction>=21)
		{//pf21
if(one_max_pos_neg_weight_red>=28)
        {
       *(StraightSections+section_num)=141;//slow positive incline
	   goto K;
		}
		else
		{
			if(one_max_pos_neg_weight_red>25)
           {
	*(StraightSections+section_num)=142;//very slow positive incline
			goto K;
           }
		}
		}//pf21
		else
		{//e2
			if(one_max_pos_neg_weight_red>=28)
        {
       *(StraightSections+section_num)=151;//slow oscillations incline
	   goto K;
		}
		else
		{//e3
			if(one_max_pos_neg_weight_red>25)
           {
	*(StraightSections+section_num)=152;//very slow oscillations incline
			goto K;
           }
		}//e3
		}//e2
	}//e1
}//mr0
	if((maximum_pos_red<=1)&&(maximum_neg_red<=1))
	{//mr1
if((pos_fraction>=0)&&(pos_fraction<=10))
	{//ps010
		if(one_max_pos_neg_weight_red>=28)
        {
       *(StraightSections+section_num)=161;//slow negative incline
	   goto K;
		}
		else
		{
			if(one_max_pos_neg_weight_red>25)
           {
	*(StraightSections+section_num)=162;//very slow negative incline
			goto K;
           }
		}

    }//ps010
	else
	{//e1
		if(pos_fraction>=21)
		{//pf21
if(one_max_pos_neg_weight_red>=28)
        {
       *(StraightSections+section_num)=171;//slow positive incline
	   goto K;
		}
		else
		{
			if(one_max_pos_neg_weight_red>25)
           {
	*(StraightSections+section_num)=172;//very slow positive incline
			goto K;
           }
		}
		}//pf21
		else
		{//e2
			if(one_max_pos_neg_weight_red>=28)
        {
       *(StraightSections+section_num)=181;//slow oscillations incline
	   goto K;
		}
		else
		{//e3
			if(one_max_pos_neg_weight_red>25)
           {
	*(StraightSections+section_num)=182;//very slow oscillations incline
			goto K;
           }
		}//e3
		}//e2
	}//e1

	}
K:
	if(vertical_comp>0)
	{//vc>0

		address_beginning=0;
		long_str_comp=-1;
		first_straight_segments_points=new int[sect_length];
memset(first_straight_segments_points,(int) '\0',(sizeof(int))*sect_length);
        total_dev=new int[sect_length];
	    first_seg_vert=new int[sect_length];
	    last_seg_vert=new int[sect_length];
		total_dev_reduced=new int[sect_length];
		global_deviation_up_top=new int[sect_length];
	arg_max_num_seg=-1;
	max_num_seg=0;
	vert_segm_count=0;
	max_last_segm=-1;
for(int count_point_sec=0;count_point_sec<sect_length-1;count_point_sec++)
        {//cycle_point_sec

			shift_scale=sect_length-1-count_point_sec;
			if(count_point_sec>max_last_segm)
			{//test
long_str_comp=Longest_Straight_Comp(inclination_links,count_point_sec,address_beginning,shift_scale,scale[1],
&first_seg,&last_seg,&num_of_seg,&dev_red,&global_deviat,&global_deviat_plus,node_coordinates);
/*long_str_comp=Longest_Straight_Comp(inclination_links,count_point_sec,address_beginning,shift_scale,1,
&first_seg,&last_seg,&num_of_seg,&dev_red);*/
if(num_of_seg>0)
{//nseg>0

	            if((first_seg-count_point_sec)<4)
				{//dist_fs_beg<

	mean_dev=dev_red/num_of_seg;
	    if(mean_dev<StripWidth)
	    {//md<
			if(last_seg>max_last_segm)
            {
				if(num_of_seg==(last_seg-count_point_sec))
				{
	         max_last_segm=last_seg;
			    }
            }
	if(num_of_seg>max_num_seg)
	{
		max_num_seg=num_of_seg;
        arg_max_num_seg=count_point_sec;
	}
	else
	{//e
		if(num_of_seg==max_num_seg)
	    {//ns=mns
			if(total_dev[arg_max_num_seg]>long_str_comp)
			{
			max_num_seg=num_of_seg;
            arg_max_num_seg=count_point_sec;
			}
			else
			{
                   if(total_dev_reduced[arg_max_num_seg]>dev_red)
			       {
			       max_num_seg=num_of_seg;
                   arg_max_num_seg=count_point_sec;
			       }

			}
		}//ns=mns
	}//e

*(first_straight_segments_points+count_point_sec)=num_of_seg+1;
*(total_dev+count_point_sec)=long_str_comp;
*(first_seg_vert+count_point_sec)=first_seg;
*(last_seg_vert+count_point_sec)=last_seg;
*(total_dev_reduced+count_point_sec)=dev_red;
*(global_deviation_up_top+count_point_sec)=global_deviat;
entrance_point=NumStrips*section_num;
if(!*(SectionStraightSegments+entrance_point+count_point_sec))
{
*(SectionStraightSegments+entrance_point+count_point_sec)=vert_segm_count+1;

  for(int count_vertic=first_seg;count_vertic<=last_seg;count_vertic++)
  {

	if(!*(SectionStraightSegments+entrance_point+count_vertic))
	{
	*(SectionStraightSegments+entrance_point+count_vertic)=vert_segm_count+1;
	}
   }
}
vert_segm_count++;
		}//md<
				}//dist_fs_beg<
/**(first_straight_segments_points+count_point_sec)=num_of_seg+1;
*(total_dev+count_point_sec)=long_str_comp;
*(first_seg_vert+count_point_sec)=first_seg;
*(last_seg_vert+count_point_sec)=last_seg;
*(total_dev_reduced+count_point_sec)=dev_red;
*(global_deviation_up_top+count_point_sec)=global_deviat;*/
}//nseg>0
			}//test
		address_beginning+=shift_scale;
        }//cycle_point_sec
if(max_num_seg>0)
{//mns>0
	global_deviat_max=*(global_deviation_up_top+arg_max_num_seg);
global_deviat_abs=abs(global_deviat_max);
regular_fraction_of_segments=(32*max_num_seg)/(sect_length-1);
*(BestVertComponentStart+section_num)=arg_max_num_seg+1;
*(BestVertComponentBeg+section_num)=*(first_seg_vert+arg_max_num_seg);
*(BestVertComponentEnd+section_num)=*(last_seg_vert+arg_max_num_seg);
*(BestVertNumPoints+section_num)=*(first_straight_segments_points+arg_max_num_seg);
*(GlobalDeviation+section_num)=global_deviat_max;
vert_max_length=*(BestVertComponentEnd+section_num)-*(BestVertComponentEnd+section_num)+1;
     if(regular_fraction_of_segments>=25)//4/5
     {
		 if(global_deviat_abs<StripWidth)
		 {
			 if(global_deviat_abs<StripWidth/2)
			 {
*(StraightSections+section_num)=2;
			 }
			 else
			 {
			    if(global_deviat_max>0)
			    {
*(StraightSections+section_num)=21;
			    }
			    else
			    {
				 if(global_deviat_max<0)
			     {
 *(StraightSections+section_num)=23;
			     }
			    }
			 }
		 }

     }
}//mns>0
if((*(InclinedComponent+section_num)||(arg_max_num_seg>=2)||(max_last_segm<sect_length-2))
   &&(regular_fraction_of_segments>0)&&(regular_fraction_of_segments<=27))
{//cond
	if(max_num_seg>0)
	{//seg_cond
incline=InclineRefinement(section_num,&right_bound);
      if(right_bound>0)
      {//rb>
incline_length=right_bound-incline;
                    if(incline_length>=2)
					{//il>
memset(hist_pos,(int) '\0',(sizeof(int))*64);
memset(hist_neg,(int) '\0',(sizeof(int))*64);
memset(hist_box_pos,(int) '\0',(sizeof(int))*16);
memset(hist_box_neg,(int) '\0',(sizeof(int))*16);
pos_number=InclineHistogram(node_coordinates,hist_pos,hist_neg,incline,right_bound,&neg_number);
if(pos_number>0)
{//pn>
total_num_pos=CenterOfMass(hist_pos,64,&center_m_pos,&dev_pos,&left_number_pos,&right_number_pos,
&small_angle_pos,&big_angle_pos,paint,hist_box_pos,dim_new_hist);
maximum_pos_red=MaximumTripleMaximumReduced(hist_box_pos,dim_new_hist,&triple_maximum_pos_red,
&maximum_value_pos_red,&triple_maximum_value_pos_red,&one_pix_neighborhood_pos_red,
&two_pix_neighborhood_pos_red,&zero_two_pix_neighb_red,local_maxim_points,&num_loc_max,
&second_max_pos,&second_max_value,&beg_noticable,&end_noticable,&noticable_sum);
max_pos_weight=(32*maximum_value_pos_red)/(pos_number);
small_angle_weight_pos=(32*small_angle_pos)/(pos_number);
one_max_pos_weight=(32*one_pix_neighborhood_pos_red)/(pos_number);
noticable_sum_weight=(32*noticable_sum)/(pos_number);
if(beg_noticable>=0)
   {
   noticable_width=end_noticable-beg_noticable+1;
   }

      if(!neg_number)
       {//!neg
	if(one_max_pos_weight>=30)
	{
		if(!*(InclinedComponent+section_num))
		{
*(InclinedComponent+section_num)=maximum_pos_red;
		}
	}
	else
	{
		if((noticable_width>0)&&(noticable_width<=2))
		{
			if(noticable_sum_weight>=25)
			{
				if(*(InclinedComponent+section_num))
				{
				*(InclinedComponent+section_num)=maximum_pos_red;
				}
			}
		}
	}

      }//!neg
}//pn>
if(neg_number>0)
{//nn>
total_num_neg=CenterOfMass(hist_neg,64,&center_m_neg,&dev_neg,&left_number_neg,&right_number_neg,
&small_angle_neg,&big_angle_neg,paint,hist_box_neg,dim_new_hist);
maximum_neg_red=MaximumTripleMaximumReduced(hist_box_neg,dim_new_hist,&triple_maximum_neg_red,
&maximum_value_neg_red,&triple_maximum_value_neg_red,&one_pix_neighborhood_neg_red,
&two_pix_neighborhood_neg_red,&zero_two_pix_neighb_neg_red,local_maxim_points,&num_loc_max_neg,
&second_max_pos_neg,&second_max_value_neg,&beg_noticable_neg,&end_noticable_neg,&noticable_sum_neg);
max_neg_weight=(32*(maximum_value_neg_red))/(total_num_neg);
small_angle_weight_neg=(32*small_angle_neg)/(total_num_neg);
one_max_neg_weight=(32*one_pix_neighborhood_neg_red)/(total_num_neg);
noticable_sum_neg_weight=(32*noticable_sum_neg)/(total_num_neg);

if(beg_noticable_neg>=0)
   {
   noticable_width_neg=end_noticable_neg-beg_noticable_neg+1;
   }
if(max_neg_weight>=16)
{
	if(one_max_neg_weight>=30)
	{
*(InclinedComponent+section_num)=-maximum_neg_red;
	}
}

}//nn>
					}//il>
         }//rb>
	}//seg_cond
	/*else
	{
	}*/

}//cond
delete[] first_straight_segments_points;
delete[] total_dev;
delete[] first_seg_vert;
delete[] last_seg_vert;
delete[] total_dev_reduced;
delete[] global_deviation_up_top;
	}//vc>0
	else
	{
/*if(*(InclinedComponent+section_num))
{
	incline=InclineRefinement(inclination_links,section_num);
}*/
		     if(sect_length>5)
		     {
		if(pos_fraction>=21)
		{

		;
			//InclinedLongest();
		}
			 }
	}
	delete[] inclination_links;
	delete[] addresses_zero_nodes;
	delete[] local_maxim_points;
	delete[] local_maxim_points_neg;
	delete[] pos_deviation_max;
	delete[] neg_deviation_max;
	delete[] longest_positive_adjacent_interval;
	delete[] longest_negative_adjacent_interval;
	delete [] next_member_appropriate_plus;
    delete [] chains_appropriate_plus;
    delete [] chain_first_member_plus;
	delete [] next_member_appropriate_minus;
    delete [] chains_appropriate_minus;
    delete [] chain_first_member_minus;
	delete [] vertical_components;


return(prior);
}//end
//=====================================================
int

CImageProcess::MaximumTripleMaximum(int* main_array,int* compl_array,int dimen,int* triple_max,
int* max_value,int* triple_max_value,int* one_pix_max_neighb,int* two_pix_max_neighb,
int* zero_two_pix)
{
	int prior;
	int max_arg;
	int max_val;
	int triple_max_arg;
	int triple_val;
	int current_value;
	int current_triple;
	int count_max;
	int one_neighborhood;
	int two_neighborhood;
	int zero_two_pix_val;

	prior=-1;
	max_val=-1;
	max_arg=-1;
	triple_val=-1;
	current_triple=-1;
	zero_two_pix_val=-1;
	two_neighborhood=-1;
	zero_two_pix_val=main_array[0]+main_array[1]+main_array[2]+compl_array[0]+compl_array[1];
	*zero_two_pix=zero_two_pix_val;
for(count_max=0;count_max<dimen;count_max++)
{//cycle members
current_value=*(main_array+count_max);
   if(current_value>max_val)
   {
	   max_val=current_value;
	   max_arg=count_max;
   }
   if((count_max>0)&&(count_max<dimen-1))
   {
current_triple=2*(*(main_array+count_max))+*(main_array+count_max-1)+*(main_array+count_max+1);

   }
   else
   {
	   if(count_max==0)
	   {
		   if(current_value>1)
		   {
current_triple=2*(*(main_array+count_max))+*(main_array+count_max+2)+*(main_array+count_max+1);
		   }
		   else
		   {
			current_triple=0;
		   }
	   }
	   else
	   {
		   if(count_max==(dimen-1))
	   {
current_triple=2*(*(main_array+count_max))+*(main_array+count_max-1)+*(main_array+count_max-2);
	   }
	   }
   }
   if(current_triple>triple_val)
   {
triple_val=current_triple;
triple_max_arg=count_max;
   }
}//cycle members
if((max_arg>1)&&(max_arg<(dimen-2)))
{
	if((triple_max_arg>1)&&(triple_max_arg<(dimen-2)))
	{
one_neighborhood=main_array[(triple_max_arg-1)]+ main_array[triple_val] + *(main_array+(triple_max_arg+1));
	}
	else
	{
one_neighborhood=main_array[(max_arg-1)]+ max_val + *(main_array+(max_arg+1));
	}
two_neighborhood = main_array[(max_arg-2)] + main_array[(max_arg-1)]+ max_val+
*(main_array+(max_arg+2)) + *(main_array+(max_arg+1));
}
else
{//earg
	if(max_arg==1)
	{
one_neighborhood=main_array[(max_arg-1)]+ max_val+ *(main_array+(max_arg+1));
two_neighborhood = main_array[(max_arg-1)]+ max_val+
*(main_array+(max_arg+2))+ *(main_array+(max_arg+1));
	}
	else
	{//earg1
		if(max_arg==0)
	    {
one_neighborhood=max_val+ *(main_array+(max_arg+1));
two_neighborhood =max_val+*(main_array+(max_arg+2))+ *(main_array+(max_arg+1));
	   }
		else
		{//earg0
			if(max_arg==(dimen-2))
	        {
one_neighborhood=main_array[(max_arg-1)]+ max_val+ *(main_array+(max_arg+1));
two_neighborhood = main_array[(max_arg-2)] + main_array[(max_arg-1)]+ max_val
+ *(main_array+(max_arg+1));
			}
			else
			{
				if(max_arg==(dimen-1))
	            {
one_neighborhood=main_array[(max_arg-1)]+ max_val;
two_neighborhood = main_array[(max_arg-2)] + main_array[(max_arg-1)]+ max_val;
				}
			}
		}//earg0
	}//earg1
}//earg
*triple_max=triple_max_arg;
*triple_max_value=triple_val;
*max_value=max_val;
*one_pix_max_neighb=one_neighborhood;
*two_pix_max_neighb=two_neighborhood;
prior=max_arg;
	return(prior);
}


//=====================================================
int

CImageProcess::CenterOfMass(int* main_array,int dimen,int* mass_center,
int* mass_deviation,int* left_number,int* right_number,
int* small_angle,int* big_angle,int* painting,int* new_array,int new_dim)
{
	int prior;
	int vertical_comp;
	int current_value;
	int total_sum;
	int indicator;
	int new_point;
	int infin_comp;
	int center_of_mass;
	int center_of_mass_new;
	int new_point_max;
	int new_point_min;

	vertical_comp=main_array[0]+main_array[1]+main_array[2];
	infin_comp=main_array[63];
    total_sum=0;
	indicator=0;
	prior=0;
	center_of_mass=0;
	new_point_max=-1;
	new_point_min=new_dim+1;
	center_of_mass_new=-1;
	for(int count_max=0;count_max<dimen;count_max++)
{//cycle members
current_value=*(main_array+count_max);
if(current_value)
{
new_point=*(painting+count_max);
if(new_point>new_point_max)
{
new_point_max=new_point;
}
if(new_point<new_point_min)
{
new_point_min=new_point;
}
*(new_array+new_point)+=current_value;
total_sum+=current_value;
center_of_mass+=(current_value*count_max);
}
//if(
}//cycle members
	prior=total_sum;
	if(total_sum)
	{
center_of_mass=center_of_mass/total_sum;
center_of_mass_new=*(painting+center_of_mass);
	}
	*mass_center=center_of_mass;
	*mass_deviation=0;
	 *left_number=new_point_min;
	 *right_number=new_point_max;
	 *small_angle=new_array[0];
	 *big_angle=new_array[new_point_max];
	return(prior);
}
//=====================================================
int

CImageProcess::MaximumTripleMaximumReduced(int* main_array,int dimen,int* triple_max,
int* max_value,int* triple_max_value,int* one_pix_max_neighb,int* two_pix_max_neighb,
int* zero_two_pix,int* loc_max_positions,int* num_loc_maxim,int* second_maxim_pos,
int* second_max_val,int* first_noticable,int* last_noticable,int* noticable_sum)
{
	int prior;
	int max_arg;
	int max_val;
	int triple_max_arg;
	int triple_val;
	int current_value;
	int current_triple;
	int count_max;
	int one_neighborhood;
	int two_neighborhood;
	int zero_two_pix_val;
	int number_of_loc_max;
	int second_max_value;
	int second_max_pos;
	int first_nonzero;
	int last_nonzero;
	int sum_noticable;
	int* local_maxim_points;

	sum_noticable=0;
	last_nonzero=-1;
	first_nonzero=-1;
	local_maxim_points=loc_max_positions;
	number_of_loc_max=0;
	second_max_pos=-1;
	second_max_value=0;
	prior=-1;
	max_val=-1;
	max_arg=-1;
	triple_val=-1;
	current_triple=-1;
	zero_two_pix_val=-1;
	two_neighborhood=-1;


	zero_two_pix_val=main_array[0]+main_array[1];
	*zero_two_pix=zero_two_pix_val;
for(count_max=0;count_max<dimen;count_max++)
{//cycle members
current_value=*(main_array+count_max);
if((current_value>1)&&(count_max>0))
{
	if(first_nonzero<0)
	{
first_nonzero=count_max;
	}
	last_nonzero=count_max;
	sum_noticable+=current_value;
}
   if(current_value>max_val)
   {
	   max_val=current_value;
	   max_arg=count_max;
   }
   if((count_max>0)&&(count_max<dimen-1))
   {
current_triple=2*(*(main_array+count_max))+*(main_array+count_max-1)+*(main_array+count_max+1);

   }
   else
   {
	   if(count_max==0)
	   {
		   if(current_value>1)
		   {
current_triple=2*(*(main_array+count_max))+*(main_array+count_max+2)+*(main_array+count_max+1);
		   }
		   else
		   {
			current_triple=0;
		   }
	   }
	   else
	   {
		   if(count_max==(dimen-1))
	   {
current_triple=2*(*(main_array+count_max))+*(main_array+count_max-1)+*(main_array+count_max-2);
	   }
	   }
   }
   if(current_triple>triple_val)
   {
triple_val=current_triple;
triple_max_arg=count_max;
   }
}//cycle members
if((max_arg>1)&&(max_arg<(dimen-2)))
{
one_neighborhood=main_array[(max_arg-1)]+ max_val+ *(main_array+(max_arg+1));
two_neighborhood = main_array[(max_arg-2)] + main_array[(max_arg-1)]+ max_val+
*(main_array+(max_arg+2)) + *(main_array+(max_arg+1));
}
else
{//earg
	if(max_arg==1)
	{
one_neighborhood=main_array[(max_arg-1)]+ max_val+ *(main_array+(max_arg+1));
two_neighborhood = main_array[(max_arg-1)]+ max_val+
*(main_array+(max_arg+2)) + *(main_array+(max_arg+1));
	}
	else
	{//earg1
		if(max_arg==0)
	    {
one_neighborhood=max_val+ *(main_array+(max_arg+1));
two_neighborhood =max_val+*(main_array+(max_arg+2))+ *(main_array+(max_arg+1));
	   }
		else
		{//earg0
			if(max_arg==(dimen-2))
	        {
one_neighborhood=main_array[(max_arg-1)]+ max_val+ *(main_array+(max_arg+1));
two_neighborhood = main_array[(max_arg-2)] + main_array[(max_arg-1)]+
max_val+ *(main_array+(max_arg+1));
			}
			else
			{
				if(max_arg==(dimen-1))
	            {
one_neighborhood=main_array[(max_arg-1)]+ max_val;
two_neighborhood = main_array[(max_arg-2)] + main_array[(max_arg-1)]+ max_val;
				}
			}
		}//earg0
	}//earg1
}//earg
if((main_array[0]>main_array[1])&&(main_array[0]>1))
{
local_maxim_points[0]=1;
		number_of_loc_max++;
}
for(int count_max1=1;count_max1<dimen-1;count_max1++)
{
current_value=main_array[(count_max1)];
	    if(current_value>1)
	     {
	if(((current_value>=main_array[(count_max1-1)])&&
		(current_value>main_array[(count_max1+1)]))||
		((current_value>main_array[(count_max1-1)])&&
		(current_value>=main_array[(count_max1+1)])))
	{
		local_maxim_points[count_max1]=1;
		number_of_loc_max++;
		if(count_max1!=max_arg)
		{
			if(current_value>=second_max_value)
			{
second_max_value=current_value;
second_max_pos=count_max1;
			}
		}
	}
		}
}


*triple_max=triple_max_arg;
*triple_max_value=triple_val;
*max_value=max_val;
*one_pix_max_neighb=one_neighborhood;
*two_pix_max_neighb=two_neighborhood;
*num_loc_maxim=number_of_loc_max;
*second_maxim_pos=second_max_pos;
*second_max_val=second_max_value;
*first_noticable=first_nonzero;
*last_noticable=last_nonzero;
*noticable_sum=sum_noticable;
prior=max_arg;

	return(prior);
}
//=====================================================
//=====================================================
int

CImageProcess::Longest_Straight_Comp(int* difference_components,int num_point,int start_point,
int last_point,int scale,int* first_segment,int* last_segment,int* number_of_segments,
int* dev_reduced,int* glob_dev,int* glob_dev_plus,int* end_coordinates)
{
	int prior;
	int array_max;
	int deviation;
	int abs_deviation;
	int deviation_reduced;
	int deviation_from_last;
	int abs_deviation_from_last;
	int abs_deviation_from_last_reduced;
	int number_of_points;
	int first_straight;
	int last_straignt;
	int end_point;
	int total_deviation_success;
	int total_deviation_success_reduced;
	int shift_in_point;
	int scale1;
	int global_dev;
	int global_dev_plus;
	int global_dev_minus;

deviation_from_last=100;
abs_deviation_from_last=100;
abs_deviation_from_last_reduced=0;
deviation=0;
abs_deviation=0;
global_dev_plus=0;
global_dev_minus=0;
prior=0;
number_of_points=0;
first_straight=-1;
last_straignt=-1;
shift_in_point=num_point+1;
total_deviation_success=0;
total_deviation_success_reduced=0;
end_point=start_point+last_point;
scale1=std::min(3,(3*scale)/2);
global_dev=0;
for(array_max=start_point;array_max<end_point;array_max++)
{//cycle members
	deviation=*(difference_components+array_max);
    abs_deviation=abs(deviation);
	deviation_reduced=abs_deviation/(array_max-start_point+1);
	if(first_straight<0)
	{
		if((deviation_reduced<=scale1)&&(abs_deviation>StripWidth))
		{
		deviation_reduced=abs_deviation;
		}
	}
	      if(last_straignt>0)
		  {
deviation_from_last=*(end_coordinates+shift_in_point)-*(end_coordinates+last_straignt);
abs_deviation_from_last=abs(deviation_from_last);
abs_deviation_from_last_reduced=abs_deviation_from_last/(shift_in_point-last_straignt);
if((abs_deviation_from_last_reduced<=scale1)&&(abs_deviation_from_last>StripWidth))
		{
		abs_deviation_from_last_reduced=abs_deviation_from_last;
		}
if((shift_in_point-last_straignt)>=4)
{
abs_deviation_from_last_reduced=100;
}
		  }
	//if((deviation_reduced<=scale1)&&((deviation<StripWidth/2)||(abs_deviation_from_last<=scale1)))
  if((deviation_reduced<=scale1)&&(abs_deviation_from_last_reduced<=scale1))
	{
		if(first_straight<0)
		{
			first_straight=shift_in_point;
			global_dev+=deviation;
			global_dev_plus+=abs_deviation;
			deviation_from_last=0;
			abs_deviation_from_last=0;
		}
        last_straignt=shift_in_point;
		number_of_points++;
		total_deviation_success+=abs_deviation;
		total_deviation_success_reduced+=deviation_reduced;
		global_dev+=deviation_from_last;
		global_dev_plus+=abs_deviation_from_last;
	}

shift_in_point++;
}//cycle members
if(last_straignt>0)
	{
prior=total_deviation_success_reduced;
global_dev_minus=global_dev - global_dev_plus;
	}
*first_segment=first_straight;
*last_segment=last_straignt;
*number_of_segments=number_of_points;
*dev_reduced=total_deviation_success;
*glob_dev=global_dev;
*glob_dev_plus=global_dev_plus;
return(prior);
}
//=====================================================
int

CImageProcess::InclineRefinement(int section_number,int* incl_right)
{
	int prior;
	int count_fiber;
	int section_fiber;
	int fiber_number;
	int start_point;
	//int first_point;
	int last_point;
	int first_strip;
    int last_strip;
	int section_length;
	int line_seg;
	int Left_Right;
	int num_sect_left;
	int contrast_end;
	int section_bunch;
	int bunch_old_number;
	int first_vert;
	int last_vert;
	int first_inclined;
	int last_inclined;
	int end_condition;
	int last_line_seg;
	int inclined_left;
	int inclined_right;

	prior=0;
first_vert=-1;
last_vert=-1;
first_inclined=-1;
last_inclined=-1;
last_line_seg=-1;
inclined_left=-1;
inclined_right=-1;

	first_strip=ColorSection->section_fibre_first[section_number];
    last_strip=ColorSection->section_fibre_last[section_number];
	num_sect_left=ColorSection->Number_of_sections_left;
	if(section_number>=num_sect_left)
	{
		Left_Right=0;
	}
	else
	{
    Left_Right=1;
	}

	start_point=NumStrips*section_number;
	section_length=last_strip-first_strip+1;
last_point=start_point+section_length;
for(count_fiber=start_point;count_fiber<last_point;count_fiber++)
{//cycle fibers
	fiber_number=count_fiber-start_point;
	section_fiber=fiber_number+first_strip;
section_bunch=ColorDescrSect[section_number].location_of_section[section_fiber];
if(section_bunch>0)
{
	section_bunch--;
}
bunch_old_number=ColorInt[section_fiber].old_bunch_number[section_bunch];
	if(Left_Right)
	{
contrast_end=ColorInt[section_fiber].ColoredIntervalsStructure->BegInterv[bunch_old_number];
       if(contrast_end<StripWidth)
	   {
end_condition=1;
	   }
	   else
	   {
		end_condition=0;
	   }
	}
	else
	{
contrast_end=ColorInt[section_fiber].ColoredIntervalsStructure->EndInterv[bunch_old_number];
if(contrast_end>StripLength-StripWidth)
	   {
end_condition=1;
	   }
	   else
	   {
		end_condition=0;
	   }
	}
line_seg=*(SectionStraightSegments+count_fiber);
 if(line_seg==1)
 {
	 if(first_inclined>=0)
	 {
		 break;
	 }
    if(first_vert<0)
    {
	first_vert=fiber_number;
    }
	last_vert=fiber_number;
	last_line_seg=line_seg;
 }
 else
 {
  if(!line_seg)
  {//ls=0
	 if(first_vert<0)
	 {
		     if(!end_condition)
		     {
		 if(first_inclined<0)
		 {
			 first_inclined=fiber_number;
		 }
			 }
last_inclined=fiber_number;
	 }
	 else
	 {//efv<0
		 if(last_line_seg==1)
		 {
			 if(!end_condition)
			 {
			 if(first_inclined<0)
		    {
			 first_inclined=fiber_number;
		    }
            last_inclined=fiber_number;
			 }

		 }
	 }//efv<0

  }//ls=0
  else
  {
  if(line_seg==2)
	 {
if(last_line_seg==1)
{
inclined_left=first_inclined-1;
inclined_right=last_inclined+1;
break;
}
	 }
  }
 }
}//cycle fibers

	if(first_inclined>=1)
	{
inclined_left=first_inclined-1;
	}
	else
	{
inclined_left=first_inclined;
	}
	   if(last_inclined==(section_length-1))
       {
       inclined_right=last_inclined;
	   }
	   else
	   {
       inclined_right=last_inclined+1;
	   }
prior=inclined_left;
*incl_right=inclined_right;
	return(prior);
}
//=====================================================
int

CImageProcess::InclineHistogram(int* main_array,int* hust_plus,int* hist_minus,
int first_incl,int last_incl,int* neg_count)
{
	int prior;
	int count_shift;
	int count_mem;
	int node_coord;
	int hist_arg;
	int hist_arg_div;
	int shift_divisor;
	int residual;
	int count_pos;
	int count_neg;
	int abs_arg;
	//int differen;
	//int differen_abs;

	prior=0;
	node_coord=0;
count_pos=0;
count_neg=0;
/*for(count_dif=first_incl;count_dif<last_incl;count_dif++)
{//cycle fibers
differen=*(main_array+count_dif+1)-*(main_array+count_dif);
if(differen>=0)
{
	if(differen<=63)
	{
		(*(hust_plus+differen))++;
	}
	else
	{
		(*(hust_plus+63))++;
	}
}
else
{
	differen_abs=abs(differen);
if(differen_abs<=63)
	{
		(*(hist_minus+differen_abs))++;
	}
	else
	{
		(*(hist_minus+63))++;
	}
}
}//cycle fibers*/
for(count_mem=first_incl;count_mem<last_incl;count_mem++)
    {//cycle members
	node_coord=*(main_array+count_mem);
    for(count_shift=count_mem;count_shift<last_incl;count_shift++)
        {//cycle shift
hist_arg=*(main_array+count_shift+1)-node_coord;
if(count_shift==count_mem)
{
hist_arg_div=hist_arg;
}
else
{
	shift_divisor=count_shift+1-count_mem;
	residual=abs(hist_arg)%shift_divisor;
	hist_arg_div=hist_arg/shift_divisor;
	if(2*residual>shift_divisor)
	{
		if(hist_arg>=0)
		{
		hist_arg_div++;
		}
		else
		{
			hist_arg_div--;
		}
	}
}
//hist_arg_div=hist_arg_div/2;
if(hist_arg>=0)
{//ha
	count_pos++;
	if(hist_arg_div<63)
	{
		hust_plus[hist_arg_div]++;
	}
	else
	{
		hust_plus[63]++;
	}
}//ha
else
{//e1

	count_neg++;
	if(!hist_arg_div)
	{
		hist_minus[hist_arg_div]++;
	}
	else
	{
      abs_arg=-hist_arg_div;
        if(abs_arg<63)
	    {
		hist_minus[abs_arg]++;
	    }
	    else
	    {
		hist_minus[63]++;
	    }
	}
}//e1
	}//cycle shift
	//address_beginning+=(sect_length-1-count_mem);
    }//cycle members
prior=count_pos;
*neg_count=count_neg;
	return(prior);
}
//=====================================================
int

CImageProcess::FindingMaxPosNegSegments(int sect_length,int* chain_array,int* max_start)
{
	int first_step_of_max_chain;
	int length_value;
	int max_length;
	int prior;

prior=-1;
first_step_of_max_chain=-1;
max_length=0;

for(int count_step=0;count_step<(sect_length-1);count_step++)
{//cycle over section levels
length_value=*(chain_array+count_step);
    if(length_value>max_length)
   {
	max_length=length_value;
    first_step_of_max_chain=count_step;
   }
}//cycle over section levels
prior=max_length;
*max_start=first_step_of_max_chain;
return(prior);
}
void

CImageProcess::SkyIntencitiesDistributionFinding(int* sky_gray,int* section_weight,
int left_right)
{
	int number_of_sect_left;
		int number_of_sect;
	    int mean_gray;
		int mean_gray_zone;
		int mean_hue;
		int mean_hue_zone;
		int mean_saturation;
		int max_gray;
		int min_gray;
		int weight_of_section;
		int previous_weight_of_section;
		int sky_value;
		int last_fiber;
		int first_fiber;
		int deviation_gray;
		int dev1;
		int dev2;
		int loop_begin;
		int loop_end;
	    int max_massive_gray;
		int max_massive_gray_low;
		int last_srip_ratio;
		int first_strip_ratio;
		int straight_comp;
		int min_left;
		int max_right;
		int shape_ratio;
		int invert_shape_ratio;
		int shape_ratio_mean;
		int invert_shape_ratio_mean;
		int width_length;
		int fiber_length;
		int massive_gray_top;
		int massive_gray_top_section;
		int last_srip_ratio1;
		int total_up_weight;
		int incline_sec;
		int incl_curve_sec;
		int max_section_length;
		int mean_section_length;
		int sect_geom;
		int mean_lower;
		int mean_upper;
		int upper_lower_ratio;

	mean_lower=0;
	mean_upper=0;
		incline_sec=0;
	incl_curve_sec=0;
		max_massive_gray=-1;
	max_massive_gray_low=-1;
	last_srip_ratio=-1;
	massive_gray_top=-1;
	total_up_weight=0;
	massive_gray_top_section=-1;
	number_of_sect_left=ColorSection->Number_of_sections_left;
	number_of_sect=ColorSection->Number_of_sections;
	if(!left_right)
	{
	loop_begin=0;
	loop_end=number_of_sect_left;
	}
	else
	{
	loop_begin=number_of_sect_left;
	loop_end=number_of_sect;
	}

	for(int count_sect=loop_begin;count_sect<loop_end;count_sect++)
	{//cycle over left sections
		sect_geom=SectionGeometry(count_sect,&mean_lower,&mean_upper);
		if(sect_geom>0)
		{
		upper_lower_ratio=(8*mean_upper)/mean_lower;
		}
		else
		{
	    upper_lower_ratio=-1;
		}
		last_fiber=ColorSection->section_fibre_last[count_sect];
		first_fiber=ColorSection->section_fibre_first[count_sect];
		last_srip_ratio=(16*last_fiber)/NumStrips;
		last_srip_ratio1=(16*(last_fiber+1))/NumStrips;
		first_strip_ratio=(16*first_fiber)/NumStrips;
		mean_gray=ColorSection->section_mean_gray[count_sect];
		mean_gray_zone=gray_zones[mean_gray];
		mean_hue=ColorSection->section_mean_hue[count_sect];
	    mean_hue_zone=hue_zones[mean_hue];
		mean_saturation=ColorSection->section_mean_saturation[count_sect];
		min_left=ColorSection->section_left_end[count_sect];
		max_right=ColorSection->section_right_end[count_sect];
		width_length=max_right-min_left+1;
		fiber_length=last_fiber-first_fiber;
		shape_ratio=width_length/(StripWidth*fiber_length);
		invert_shape_ratio=(StripWidth*fiber_length)/width_length;
		weight_of_section=ColorSection->section_weight[count_sect];
		max_section_length=ColorSection->section_fiber_max_length[count_sect];
		mean_section_length=weight_of_section/fiber_length;
		shape_ratio_mean=(16*mean_section_length)/(StripWidth*fiber_length);
		invert_shape_ratio_mean=
		(16*StripWidth*fiber_length)/(mean_section_length);
		if((last_srip_ratio<=12))
		{
		if(weight_of_section>500)
			 {
				 if(mean_gray>max_massive_gray)
				 {
					 max_massive_gray=mean_gray;

				 }
			 }
		}
		else
		{
			if(last_srip_ratio1>=15)
			{
				if(weight_of_section>250)
			 {
				 total_up_weight+=weight_of_section;
				 if(mean_gray>massive_gray_top)
				 {
					 massive_gray_top=mean_gray;
	              massive_gray_top_section=count_sect;

				 }
			 }
			}
		}
		if(first_strip_ratio<=4)
		{
	    if(weight_of_section>500)
			 {
				 if(mean_gray>max_massive_gray_low)
				 {
					 max_massive_gray_low=mean_gray;
				 }
			 }
		}
		straight_comp=*(StraightSections+count_sect);
		incline_sec=InclinedComponent[count_sect];
	    incl_curve_sec=InclineCurve[count_sect];
		if((straight_comp>0)&&(straight_comp<50))
		{//stcomp
			if((mean_hue_zone<3)||(mean_hue<=14))
			{//mhz
	             if(mean_gray_zone<=5)
	             {//mgz
		                    if(!left_right)
	                         {
		                      if(min_left>2*StripWidth)
		                      {
		                      goto K;
		                      }
	                         }
		                      else
		                     {
			                 if(max_right<(StripLength-2*StripWidth))
			                  {
		                      goto K;
			                  }
		       }

			      }//mgz
		}//mhz
		}//stcomp

		            if((last_fiber>(5*NumStrips/8))&&(first_fiber>NumStrips/3))
					{//upper_part

	    max_gray=ColorSection->DescrSec[count_sect].upper_gray;
		min_gray=ColorSection->DescrSec[count_sect].lower_gray;
		dev1=mean_gray-min_gray;
		dev2=max_gray-mean_gray;
		if((dev1>=0)&&(dev2>=0))
		{
	deviation_gray=std::min(dev1,dev2);
	max_gray=mean_gray+deviation_gray;
		}
		else
		{
			if(dev2<0)
			{
	max_gray=mean_gray;
			}

		}
	if(((last_srip_ratio>=14)&&((first_strip_ratio>=9)||(shape_ratio_mean>=8)||(upper_lower_ratio>=14)))
	   ||((first_strip_ratio>=10)&&(last_srip_ratio>=13))||
	   (((shape_ratio_mean>=8))&&(last_srip_ratio>=15)))
	{
	 if((mean_gray<NUM_INTEN1)&&(max_gray<NUM_INTEN1))
	 {//logcond
	    for(int count_intens=mean_gray;count_intens<=std::min(max_gray,mean_gray+5);count_intens++)
		{//loopbound
	     sky_value=sky_gray[count_intens];
			if(!sky_value)
			{
			sky_gray[count_intens]=count_sect+1;
	        section_weight[count_intens]=weight_of_section;
			}
			else
			{
	         previous_weight_of_section=section_weight[count_intens];
			 if(weight_of_section>previous_weight_of_section)
			 {
			 sky_gray[count_intens]=count_sect+1;
	         section_weight[count_intens]=weight_of_section;
			 }
			}
		}//loopbound
	 }
	 }//logcond
					}//upper_part
	K:
					;
	}//cycle over left sections

	MassiveGray = max_massive_gray;
	MassiveGrayLow = max_massive_gray_low;
	MassiveGrayTop = massive_gray_top;
	MassiveGrayTopSection = massive_gray_top_section;
}






int CImageProcess::MaximumSky(int* sk_gray,int* sec_weight,int* intensity_last,int* max_saturated)
{
	int sky_value;
		int sect_number;
		int sect_number_previous;
		int sky_weight;
		int sky_weight_maximum;
		int sky_maximum_number;
		int sky_max_intence;
		int first_maximum_intence;
		int sect_max_mean_gray;
		int new_sect_max_mean_gray;
		int new_sect_min_gray;
		int new_sect_max_gray;
		int last_sect_number;
		int last_intence;
		int mean_hue;
		int hue_zone_section;
		int mean_saturation;
		int last_section_fiber;
		int first_section_fiber;
		int first_section_ratio;
		int last_srip_ratio;
		int min_left;
		int max_right;
		int shape_ratio;
		int invert_shape_ratio;
		int straight_comp;
		int width_length;
		int fiber_length;
		int blue_saturated_maximum;
		int blue_sat_max_weight;
		int unsaturated_maximum;
		int unsaturatedmax_weight;
		int test_section;
		int number_of_left_sections;
		int blue_sat_end;
		int blue_sat_beg;
		int blue_sat_end_ratio;
		int blue_sat_length;
		int blue_sat_shape_ratio;
		int sky_max_end;
		int sky_max_beg;
		int sky_max_min_left;
		int sky_max_max_right;
		int blue_sat_min_left;
		int blue_sat_max_right;
		int green_separation;
		int specific_weight;//03.11.15


		specific_weight=-1;//03.11.15
		green_separation=-1;
		sky_max_min_left=-1;
		sky_max_max_right=-1;
		blue_sat_min_left=-1;
		blue_sat_max_right=-1;
		sky_max_end=-1;
		sky_max_beg=-1;
		blue_sat_end=-1;
		blue_sat_beg=-1;
	    blue_sat_end_ratio=-1;
		blue_sat_length=-1;
		blue_sat_shape_ratio=-1;
		test_section=-1;
		sky_maximum_number=-1;
	    sky_weight_maximum=0;
		sect_number_previous=-1;
		sky_max_intence=-1;
		first_maximum_intence=-1;
		sect_max_mean_gray=-1;
		new_sect_max_mean_gray=-1;
		last_sect_number=-1;
		last_intence=-1;
		blue_saturated_maximum=-1;
		blue_sat_max_weight=0;
		unsaturated_maximum=-1;
		unsaturatedmax_weight=0;
		number_of_left_sections=ColorSection->Number_of_sections_left;

		for(int count_intens=NUM_INTEN1-1;count_intens>=0;count_intens--)
		{//loop_sky_bound
	     sky_value=sk_gray[count_intens];
		 if((sky_value>0)&&(sky_value!=sect_number_previous))
		 {
             sect_number=sky_value-1;
	new_sect_max_mean_gray=ColorSection->section_mean_gray[sect_number];

	new_sect_min_gray=ColorSection->DescrSec[sect_number].lower_gray;
	new_sect_max_gray=ColorSection->DescrSec[sect_number].upper_gray;

	mean_hue=ColorSection->section_mean_hue[sect_number];
	mean_saturation=ColorSection->section_mean_saturation[sect_number];
	hue_zone_section=hue_zones[mean_hue];
	last_section_fiber=ColorSection->DescrSec[sect_number].base_last;
	first_section_fiber=ColorSection->DescrSec[sect_number].base_first;
	first_section_ratio=(16*first_section_fiber)/NumStrips;
	last_srip_ratio=(16*last_section_fiber)/NumStrips;
	min_left=ColorSection->section_left_end[sect_number];
	max_right=ColorSection->section_right_end[sect_number];
	width_length=max_right-min_left+1;
		fiber_length=last_section_fiber-first_section_fiber+1;

	shape_ratio=width_length/(StripWidth*fiber_length);
		invert_shape_ratio=(StripWidth*fiber_length)/width_length;
		straight_comp=*(StraightSections+sect_number);
	if((sect_max_mean_gray>0)&&(abs(sect_max_mean_gray-new_sect_max_mean_gray)>45)&&
	   (hue_zone_section!=4))
	{
		last_sect_number=sect_number;
		last_intence=count_intens;
		break;
	}
			 sect_number_previous=sky_value;
			 sky_weight=ColorSection->section_weight[sect_number];
			 specific_weight=sky_weight/fiber_length;
			      if(((last_srip_ratio>=14)&&(first_section_ratio>=6))||
					  ((last_srip_ratio>=13)&&(first_section_ratio>=10)))
			      {//lsr
					  if(hue_zone_section==3)
					  {//hzs3
						  if((mean_hue>=8)&&(mean_hue<=22))
						  {//mh816
							  if(mean_saturation>=4)
							  {
								     if(MassiveGrayLow>0)
					                 {
								  if(new_sect_max_mean_gray<=NUM_INTEN1/2)
								  {
						  if(new_sect_max_mean_gray<(2*MassiveGrayLow))
						  {
									  goto L;
						  }
								  }
								      }
							  }
						  }//mh816
						  if(MassiveGrayLow>0)
					      {
	if((new_sect_max_mean_gray<=NUM_INTEN1/3)&&(new_sect_max_mean_gray<MassiveGrayLow))
						  {
							  if(mean_saturation>=3)
							  {
							  goto L;
							  }
						  }
						  }
						  if((mean_hue>=8)&&(mean_hue<=14))
						  {//mh816

							  if(MassiveGray>0)
							  {//MG
	if((new_sect_max_mean_gray<MassiveGray/2)||((new_sect_max_mean_gray<=NUM_INTEN1/2)&&
	   (new_sect_max_mean_gray<MassiveGray)))
								 {//last_cor24.06.15
								 goto L;
								 }
							  }//MG
						  }//mh816
						  else
						  {
	if((mean_hue>=8)&&(mean_hue<=19))
						  {//mh819
							  if(mean_saturation>=2)
							  {
	                 if(MassiveGrayLow>0)
					  {
	if((new_sect_max_mean_gray<=NUM_INTEN1/3)&&(new_sect_max_mean_gray<MassiveGrayLow))
						  {
							  goto L;
						  }
	if((new_sect_max_mean_gray<MassiveGrayLow/3)||((new_sect_max_mean_gray<=(2*NUM_INTEN1)/5)&&
	   (new_sect_max_mean_gray<MassiveGrayLow/2)))
	{
		goto L;
	}
					 }
							  }
						  }//mh819
						  }
				    if(MassiveGray>0)
					{
		  if((new_sect_max_mean_gray<MassiveGray/2)&&(mean_saturation<=2))
							     {//last_cor24.06.15
								 goto L;
								 }
					}
					  }//hzs3
					  if(MassiveGray>0)
					  {
	if(((new_sect_max_mean_gray<(MassiveGray)/3)&&(hue_zone_section!=4))||
	 ((new_sect_max_mean_gray<=NUM_INTEN1/2)&&(hue_zone_section!=4)&&
									 (mean_saturation<=2)))
							     {
								 goto L;
								 }
					  }
					  if(MassiveGrayLow>0)
					  {
	if((new_sect_max_mean_gray<MassiveGrayLow/3)||((new_sect_max_mean_gray<=NUM_INTEN1/4)&&
	   (new_sect_max_mean_gray<MassiveGrayLow)))
							     {
								 goto L;
								 }
					  }
					  if(sky_maximum_number>=0)
					  {
						  if(sky_weight_maximum>1000)
						  {
							  if(sect_max_mean_gray>2*new_sect_max_mean_gray)
							  {
								  goto L;
							  }
						  }
					  }
					  if(mean_saturation>=2)
					  {
		if((hue_zone_section==4)||((hue_zone_section==3)&&(mean_hue>16)))
		{//hzs43
			if(((((new_sect_min_gray+new_sect_max_mean_gray)/2>30)||((last_srip_ratio>=15)&&(new_sect_max_mean_gray>40)))&&
				(hue_zone_section==4))||(((hue_zone_section==3)&&(new_sect_max_mean_gray>40))))
			{//last_cor31.10.15
						  if((sky_weight>blue_sat_max_weight)&&(sky_weight>100))
						  {

	                      blue_saturated_maximum=sect_number;
						  blue_sat_max_weight=sky_weight;
						  blue_sat_end=last_section_fiber;
						  blue_sat_beg=first_section_fiber;
	                      blue_sat_end_ratio=last_srip_ratio;
		                  blue_sat_length=blue_sat_end-blue_sat_beg+1;
		                  blue_sat_shape_ratio=shape_ratio;
						  blue_sat_min_left=min_left;
						  blue_sat_max_right=max_right;

						  }
			}
		}//hzs43
					  }
					  else
					  {
	                      if(sky_weight>unsaturatedmax_weight)
						  {
	                      unsaturated_maximum=sect_number;
		                  unsaturatedmax_weight=sky_weight;
						  }
					  }
				/*if((sky_weight<1000)&&(last_srip_ratio<=13))
				{//last-cor19.08.15
					goto L;
				}//last-cor19.08.15*/
					  if(((new_sect_min_gray+new_sect_max_mean_gray)/2<30)&&(new_sect_max_mean_gray<MassiveGrayLow))
					  {//last_cor31.10.15&02.11.15
	                  goto L;
					  }
					  if(!shape_ratio)
					  {//last_cor03.11.15
						  if(invert_shape_ratio>5)
						  {
	                if(specific_weight<StripLength/20)
					{

					goto L;
					}

						  }
					  }//last_cor03.11.15
					  if((specific_weight<StripLength/10)&&(last_srip_ratio<=14)&&(new_sect_max_mean_gray<40)&&(mean_saturation<=2))
					  {//last_cor03.11.15

					  goto L;
					  }
					  if((hue_zone_section>=1)&&(hue_zone_section<=3)&&((new_sect_min_gray+new_sect_max_mean_gray)/2<30)&&
						  (mean_saturation>=2))
					  {//last_cor31.10.15&02.11.15
						  goto L;
					  }
			 if((sky_weight>sky_weight_maximum)&&((sky_weight>300)||((sky_weight>200)&&
				 (last_srip_ratio>=15))))
			 {
				   if(((last_srip_ratio<=14)&&(new_sect_max_mean_gray<45))||(new_sect_max_mean_gray<40))
				    {//last_cor04.11.15
				 if(sect_number>=number_of_left_sections)
				 {
	test_section=TestSection(sect_number,1,min_left,max_right);
				 }
				 else
				 {
	test_section=TestSection(sect_number,0,min_left,max_right);
				 }
				   }//last_cor04.11.15
				 if(test_section>0)
				 {
					 goto L;
				 }
				 if(sky_maximum_number<0)
				 {
					 first_maximum_intence=count_intens;
				 }
	         sky_weight_maximum=sky_weight;
			 sky_maximum_number=sect_number;
			 sky_max_intence=count_intens;
			 sect_max_mean_gray=ColorSection->section_mean_gray[sky_maximum_number];
			 sky_max_end=last_section_fiber;
			 sky_max_beg=first_section_fiber;
			 sky_max_min_left=min_left;
			 sky_max_max_right=max_right;
			 }
				  }//lsr
		 }
	L:
		 ;
		}//loop_sky_bound
		if(sky_weight_maximum<100)
		{
			sky_maximum_number=-1;
		}
		if(blue_saturated_maximum>=0)
		{
			if(sky_maximum_number<0)
		    {
	if(blue_sat_end==(NumStrips-1))
	{
		if(blue_sat_shape_ratio>=1)
		{
	sky_maximum_number=blue_saturated_maximum;
		}
	}
		    }
			else
			{
				if(blue_sat_end==(NumStrips-1))
	            {
					if(((blue_sat_beg-sky_max_end)>1))
					{
	green_separation=GreenSeparation(sky_maximum_number,blue_saturated_maximum,
	sky_max_min_left,sky_max_max_right,blue_sat_min_left,blue_sat_max_right);
	if(green_separation>0)
	{
	sky_maximum_number=blue_saturated_maximum;
	}
					}

				}
			}
			if(blue_saturated_maximum!=sky_maximum_number)
			{
				*max_saturated=blue_saturated_maximum;
			}
			else
			{
				*max_saturated=-1;
			}
		}
		else
		{
			*max_saturated=-1;
		}

	return(sky_maximum_number);
	}
//=====================================================
void

CImageProcess::BelongsSkyTo(int* sky_gr,int first_section,int last_section)
{
	int mean_gray;
			int max_gray;
			int min_gray;
			int mean_gray_opt;
			int max_gray_opt;
			int min_gray_opt;
			int mean_gray_opt_left;
			int max_gray_opt_left;
			int min_gray_opt_left;
			int mean_gray_opt_right;
			int max_gray_opt_right;
			int min_gray_opt_right;
			int dev1;
			int dev2;
		    int deviation_gray;
			int weight_of_section;
			int weight_opt_section;
			int weight_opt_section_left;
			int weight_opt_section_right;
			int last_fiber;
			int first_fiber;
			int sky_inhabitant;
			int sky_value;
			//int inter_value;
			int mean_min;
			int max_min;
			int mean_max;
			int max_max;
			int length_opt;
			int length_opt_left;
			int length_opt_right;
			int length_new;
			int min_length;
			int max_length;
			int length_min;
			int length_max;
			int ratio_length;
			int ratio_length1;
			int ratio1_length;
			int ratio1_length1;
			int joint_length;
			int last_srip_ratio;
			int first_strip_ratio;
			int first_opt_fiber;
			int first_opt_fiber_left;
			int first_opt_fiber_right;
			int last_opt_fiber;
			int last_opt_fiber_left;
			int last_opt_fiber_right;
			int section_opt;
			int section_opt_left;
			int section_opt_right;
			int sect_gray_zone_opt;
			int sect_gray_zone_opt_left;
			int sect_gray_zone_opt_right;
		    int sect_gray_zone;
			int sect_hue_zone_opt;
			int sect_hue_zone_opt_left;
			int sect_hue_zone_opt_right;
			int sect_hue_zone;
			int mean_hue_opt;
			int mean_hue_opt_left;
			int mean_hue_opt_right;
			int mean_hue;
			int mean_saturation_opt;
			int mean_saturation_opt_left;
			int mean_saturation_opt_right;
			int mean_saturation;
			int max_min_fiber;
			int min_max_fiber;
			int last_fiber_difference;
			int inters_fiber_length;
			int fiber_length_opt;
			int fiber_length_opt_left;
			int fiber_length_opt_right;
			int fiber_length_oppos;
			int length_fiber_ratio;
			int length_fiber_ratio1;
			int gray_zone_opt;
			int hue_zone_opt;
			int gray_zone_opt_left;
			int hue_zone_opt_left;
			int gray_zone_opt_right;
			int hue_zone_opt_right;
			int hue_difference;
			int hue_dif;
			int hue_zone_difference;
			int saturation_difference;
			int gray_difference;
			int gray_zone_difference;
			int left_right_relation;
			int number_of_sections_left;
			int number_of_sections_right;
			int min_left;
			int max_right;
			int min_left_opt;
			int max_right_opt;
			int min_left_opt_left;
			int max_right_opt_left;
			int min_left_opt_right;
			int max_right_opt_right;
		    int weight_ratio;
			int fiber_length;
			int fiber_length_ratio;
			int shape_ratio;
			int invert_shape_ratio;
			int width_length;
			int pos_sat_dev;
			int Sky_saturated;
			int width_opt_length;
			int ratio_width;
			int ratio_width1;
			int inter_width_legth;
			int oppos_min_left;
			int oppos_max_right;
			int oppos_first_fiber;
			int oppos_last_fiber;
			int oppos_inters_width;
			int oppos_inters_fiber;
			int oppos_opt;
			int oppos_ratio_width;
		    int oppos_ratio_width1;
			int oppos_length_fiber_ratio;
			int	oppos_length_fiber_ratio1;
			int count_left;
			int count_right;
			int mean_gray_sat;
			int max_gray_sat;
			int min_gray_sat;
			int weight_sat_section;
			int last_fiber_sat;
			int first_fiber_sat;
			int gray_zone_sat;
			int hue_zone_sat;
			int mean_hue_sat;
			int mean_saturation_sat;
			int fiber_length_sat;
			int mean_hue_opp;
			int mean_gray_opp;
			int mean_sat_opp;
			int hue_difference_opp;
			int gray_difference_opp;
			int saturation_difference_opp;
			int locate;
			int sect_under;
			int sect_under_right;
			int indicative;
			int sky_left_index;
		    int sky_right_index;
			int sect_hue_zone_opp;
			int hue_zone_difference_opp;
			int last_fiber_beg;
			int last_fiber_end;
			int first_fiber_beg;
			int first_fiber_end;
			int last_fiber_length;
			int first_fiber_length;
			int last_bunch;
			int first_bunch;
			int first_last_ratio;
			int first_opt_bunch_left;
			int first_opt_bunch_left_beg;
	        int first_opt_bunch_left_end;
		    int first_opt_bunch_left_length;
			int first_last_ratio_left;
			int first_opt_bunch_right;
			int first_opt_bunch_right_beg;
	        int first_opt_bunch_right_end;
		    int first_opt_bunch_right_length;
			int first_last_ratio_right;
			int sect_incline;
		    int sect_incl_curve;
		    int sect_vert;
			int first_opt_bunch_beg;
		    int first_opt_bunch_end;
			int first_opt_bunch_length;
			int inters_bunch_length;
			int max_bunch_left;
		    int min_bunch_right;
			int oppos_length_bunch_ratio;
			int oppos_length_bunch_ratio1;

			max_bunch_left=-1;
		    min_bunch_right=-1;
			oppos_length_bunch_ratio=-1;
			oppos_length_bunch_ratio1=-1;
			inters_bunch_length=-1;
			first_opt_bunch_length=-1;
			first_opt_bunch_beg=-1;
		    first_opt_bunch_end=-1;
			first_last_ratio_right=-1;
			first_opt_bunch_right_length=-1;
			first_opt_bunch_right=-1;
			first_last_ratio_left=-1;
			first_opt_bunch_left_length=-1;
			first_opt_bunch_left=-1;
			last_fiber_length=-1;
			first_fiber_length=-1;
			hue_zone_difference_opp=-1;
			sect_hue_zone_opp=-1;
			oppos_ratio_width=-1;
			oppos_ratio_width1=-1;
			sky_left_index=-1;
		    sky_right_index=-1;
			indicative=-1;
			locate=-1;
		    sect_under=-1;
			sect_under_right=-1;
			fiber_length_opt_right=-1;
			hue_difference_opp=100;
			gray_difference_opp=100;
			saturation_difference_opp=100;
			hue_difference=100;
			gray_difference=100;
			saturation_difference=100;
			oppos_length_fiber_ratio=-1;
			oppos_length_fiber_ratio1=-1;
			mean_gray_opp=-1;
			mean_sat_opp=-1;
			mean_hue_opp=-1;
			ratio_length1=-1;
		ratio_length=-1;
		    last_fiber_sat=-1;
			first_fiber_sat=-1;
			mean_gray_sat=-1;
			max_gray_sat=-1;
			min_gray_sat=-1;
			weight_sat_section=-1;
			gray_zone_sat=-1;
			hue_zone_sat=-1;
			mean_hue_sat=-1;
			mean_saturation_sat=-1;
			fiber_length_sat=-1;
		count_left=0;
		count_right=0;
		fiber_length_oppos=-1;
		oppos_opt=-1;
		oppos_min_left=-1;
		oppos_max_right=-1;
		oppos_first_fiber=-1;
		oppos_last_fiber=-1;
		oppos_inters_width=-1;
		oppos_inters_fiber=-1;
		section_opt_left=-1;
			min_left_opt_left=-1;
			max_right_opt_left=-1;
		weight_opt_section_left=-1;
		mean_gray_opt_left=-1;
		max_gray_opt_left=-1;
		min_gray_opt_left=-1;
		length_opt_left=-1;
		last_opt_fiber_left=-1;
		first_opt_fiber_left=-1;
		mean_hue_opt_left=-1;
		mean_saturation_opt_left=-1;
		min_left_opt_right=-1;
		max_right_opt_right=-1;
		weight_opt_section_right=-1;
		mean_gray_opt_right=-1;
		max_gray_opt_right=-1;
		min_gray_opt_right=-1;
		length_opt_right=-1;
		last_opt_fiber_right=-1;
		first_opt_fiber_right=-1;
		mean_hue_opt_right=-1;
		mean_saturation_opt_right=-1;
		gray_zone_opt_left=-1;
			hue_zone_opt_left=-1;
			fiber_length_opt_left=-1;
			sect_gray_zone_opt_left=-1;
			sect_hue_zone_opt_left=-1;
			max_gray_opt=max_gray_opt_left;
		min_gray_opt=-1;
		length_opt=-1;
		last_opt_fiber=-1;
		first_opt_fiber=-1;
		mean_hue_opt=-1;
		mean_saturation_opt=-1;
		gray_zone_opt=-1;
		hue_zone_opt=-1;
		fiber_length_opt=-1;
		sect_gray_zone_opt=-1;
		sect_hue_zone_opt=-1;
			number_of_sections_left=ColorSection->Number_of_sections_left;
			number_of_sections_right=ColorSection->Number_of_sections_right;
			/*if(!first_section)
			{
			if(number_of_sections_left>0)
			{
			LeftSkyComp=new int[number_of_sections_left];
			}
			}
			else
			{
			    if(number_of_sections_right>0)
			     {
			     RightSkyComp=new int[number_of_sections_right];
			     }
			}*/
			if(SkyMainLeft>=0)
			{
			section_opt_left=SkyMainLeft;
			min_left_opt_left=ColorSection->section_left_end[SkyMainLeft];
			max_right_opt_left=ColorSection->section_right_end[SkyMainLeft];
		weight_opt_section_left=ColorSection->section_weight[SkyMainLeft];
		mean_gray_opt_left=ColorSection->section_mean_gray[SkyMainLeft];
		max_gray_opt_left=ColorSection->DescrSec[SkyMainLeft].upper_gray;
		min_gray_opt_left=ColorSection->DescrSec[SkyMainLeft].lower_gray;
		length_opt_left=max_gray_opt_left-mean_gray_opt_left+1;
		last_opt_fiber_left=ColorSection->DescrSec[SkyMainLeft].base_last;
		first_opt_fiber_left=ColorSection->DescrSec[SkyMainLeft].base_first;
		first_opt_bunch_left=
		ColorSection->DescrSec[SkyMainLeft].location_of_section[first_opt_fiber_left];
		if(first_opt_bunch_left>0)
		{
		first_opt_bunch_left--;
	    first_opt_bunch_left=
	ColorInt[first_opt_fiber_left].old_bunch_number[first_opt_bunch_left];
	    first_opt_bunch_left_beg=
	ColorInt[first_opt_fiber_left].ColoredIntervalsStructure->BegInterv[first_opt_bunch_left];
	    first_opt_bunch_left_end=
	ColorInt[first_opt_fiber_left].ColoredIntervalsStructure->EndInterv[first_opt_bunch_left];
		first_opt_bunch_left_length=first_opt_bunch_left_end-first_opt_bunch_left_beg+1;

		}
		mean_hue_opt_left=ColorSection->section_mean_hue[SkyMainLeft];
		mean_saturation_opt_left=ColorSection->section_mean_saturation[SkyMainLeft];
		gray_zone_opt_left=gray_zones[mean_gray_opt_left];
			hue_zone_opt_left=hue_zones[mean_hue_opt_left];
			fiber_length_opt_left=last_opt_fiber_left-first_opt_fiber_left+1;
			sect_gray_zone_opt_left=gray_zones[mean_gray_opt_left];
			sect_hue_zone_opt_left=hue_zones[mean_hue_opt_left];

			}
		section_opt_right=SkyMainRight;
		if(SkyMainRight>=0)
		{
		min_left_opt_right=ColorSection->section_left_end[SkyMainRight];
		max_right_opt_right=ColorSection->section_right_end[SkyMainRight];
		weight_opt_section_right=ColorSection->section_weight[SkyMainRight];
		mean_gray_opt_right=ColorSection->section_mean_gray[SkyMainRight];
		max_gray_opt_right=ColorSection->DescrSec[SkyMainRight].upper_gray;
		min_gray_opt_right=ColorSection->DescrSec[SkyMainRight].lower_gray;
		length_opt_right=max_gray_opt_right-mean_gray_opt_right+1;
		last_opt_fiber_right=ColorSection->DescrSec[SkyMainRight].base_last;
		first_opt_fiber_right=ColorSection->DescrSec[SkyMainRight].base_first;
		first_opt_bunch_right=
		ColorSection->DescrSec[SkyMainRight].location_of_section[first_opt_fiber_right];
		if(first_opt_bunch_right>0)
		{
		first_opt_bunch_right--;
	    first_opt_bunch_right=
	ColorInt[first_opt_fiber_right].old_bunch_number[first_opt_bunch_right];
	    first_opt_bunch_right_beg=
	ColorInt[first_opt_fiber_right].ColoredIntervalsStructure->BegInterv[first_opt_bunch_right];
	    first_opt_bunch_right_end=
	ColorInt[first_opt_fiber_right].ColoredIntervalsStructure->EndInterv[first_opt_bunch_right];
		first_opt_bunch_right_length=first_opt_bunch_right_end-first_opt_bunch_right_beg+1;

		}
		mean_hue_opt_right=ColorSection->section_mean_hue[SkyMainRight];
		mean_saturation_opt_right=ColorSection->section_mean_saturation[SkyMainRight];
			gray_zone_opt_right=gray_zones[mean_gray_opt_right];
			hue_zone_opt_right=hue_zones[mean_hue_opt_right];
			fiber_length_opt_right=last_opt_fiber_right-first_opt_fiber_right+1;
			sect_gray_zone_opt_right=gray_zones[mean_gray_opt_right];
			sect_hue_zone_opt_right=hue_zones[mean_hue_opt_right];

		}
		if(!first_section)
		{
			Sky_saturated=SkySaturatedLeft;
			if(SkyMainLeft>=0)
			{
			min_left_opt=min_left_opt_left;
			max_right_opt=max_right_opt_left;
		section_opt=section_opt_left;
		weight_opt_section=weight_opt_section_left;
		mean_gray_opt=mean_gray_opt_left;
		max_gray_opt=max_gray_opt_left;
		min_gray_opt=min_gray_opt_left;
		length_opt=length_opt_left;
		last_opt_fiber=last_opt_fiber_left;
		first_opt_fiber=first_opt_fiber_left;
		mean_hue_opt=mean_hue_opt_left;
		mean_saturation_opt=mean_saturation_opt_left;
		gray_zone_opt=gray_zone_opt_left;
		hue_zone_opt=hue_zone_opt_left;
		fiber_length_opt=fiber_length_opt_left;
		sect_gray_zone_opt=sect_gray_zone_opt_left;
		sect_hue_zone_opt=sect_hue_zone_opt_left;
		if(first_opt_bunch_left>=0)
		{
		first_opt_bunch_beg=first_opt_bunch_left_beg;
		first_opt_bunch_end=first_opt_bunch_left_end;
		first_opt_bunch_length=first_opt_bunch_end-first_opt_bunch_beg+1;
		}
			}
		if(SkyMainRight>=0)
		{
		fiber_length_oppos=fiber_length_opt_right;
		}
		     if(SkyMainLeft>=0)
			 {
				 LeftSkyComp[count_left]=SkyMainLeft;
				 count_left++;
				 if(first_opt_fiber<LowerSkyFiber)
		         {
			     LowerSkyFiber=first_opt_fiber;
		         }
		if(last_opt_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_opt_fiber;
		}
		if(mean_gray_opt>UpperMeanGray)
		{
	    UpperMeanGray=mean_gray_opt;
		}
		if(mean_gray_opt<LowerMeanGray)
		{
	    LowerMeanGray=mean_gray_opt;
		}
			 oppos_opt=SkyMainRight;
			 if(SkyMainRight>=0)
			 {
		     oppos_min_left=min_left_opt_right;
		     oppos_max_right=max_right_opt_right;
			 oppos_first_fiber=first_opt_fiber_right;
		     oppos_last_fiber=last_opt_fiber_right;
			 }
			 }
		}
		else
		{
			Sky_saturated=SkySaturatedRight;
			if(SkyMainRight>=0)
			{
			min_left_opt=min_left_opt_right;
		max_right_opt=max_right_opt_right;
		section_opt=section_opt_right;
		weight_opt_section=weight_opt_section_right;
		mean_gray_opt=mean_gray_opt_right;
		max_gray_opt=max_gray_opt_right;
		min_gray_opt=min_gray_opt_right;
		length_opt=length_opt_right;
		last_opt_fiber=last_opt_fiber_right;
		first_opt_fiber=first_opt_fiber_right;
		mean_hue_opt=mean_hue_opt_right;
		mean_saturation_opt=mean_saturation_opt_right;
		gray_zone_opt=gray_zone_opt_right;
		hue_zone_opt=hue_zone_opt_right;
		fiber_length_opt=fiber_length_opt_right;
		sect_gray_zone_opt=sect_gray_zone_opt_right;
		sect_hue_zone_opt=sect_hue_zone_opt_right;
		if(first_opt_bunch_right>=0)
		{
		first_opt_bunch_beg=first_opt_bunch_right_beg;
		first_opt_bunch_end=first_opt_bunch_right_end;
		first_opt_bunch_length=first_opt_bunch_end-first_opt_bunch_beg+1;
		}
			}
			if(SkyMainLeft>=0)
			{
		fiber_length_oppos=fiber_length_opt_left;
			}
		if(SkyMainRight>=0)
			 {
				 RightSkyComp[count_right]=SkyMainRight;
				 count_right++;
				 if(first_opt_fiber<LowerSkyFiber)
		         {
			     LowerSkyFiber=first_opt_fiber;
		         }
		if(last_opt_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_opt_fiber;
		}
		if(mean_gray_opt>UpperMeanGray)
		{
			UpperMeanGray=mean_gray_opt;
		}
		if(mean_gray_opt<LowerMeanGray)
		{
			LowerMeanGray=mean_gray_opt;
		}
			 oppos_opt=SkyMainLeft;
			 oppos_min_left=min_left_opt_left;
		     oppos_max_right=max_right_opt_left;
			 oppos_first_fiber=first_opt_fiber_left;
		     oppos_last_fiber=last_opt_fiber_left;
			 }
		}
		if(oppos_opt>=0)
		{
			mean_gray_opp=ColorSection->section_mean_gray[oppos_opt];
			mean_sat_opp=ColorSection->section_mean_saturation[oppos_opt];
			mean_hue_opp=ColorSection->section_mean_hue[oppos_opt];
			sect_hue_zone_opp=hue_zones[mean_hue_opp];
		}
		if(Sky_saturated>=0)
		{//ss>=0
			mean_hue_sat=ColorSection->section_mean_hue[Sky_saturated];
			hue_dif=abs(mean_hue_sat-mean_hue_opt);
		hue_difference=std::min(hue_dif,NUM_HUES-hue_dif);
			mean_saturation_sat=ColorSection->section_mean_saturation[Sky_saturated];
			saturation_difference=abs(mean_saturation_sat-mean_saturation_opt);
			mean_gray_sat=ColorSection->section_mean_gray[Sky_saturated];
		    gray_difference=abs(mean_gray_sat-mean_gray_opt);
			if(oppos_opt>=0)
		{
		hue_difference_opp=abs(mean_hue_sat-mean_hue_opp);
		gray_difference_opp=abs(mean_gray_sat-mean_gray_opp);
		saturation_difference_opp=abs(mean_saturation_sat-mean_sat_opp);
		}
		    last_fiber_sat=ColorSection->DescrSec[Sky_saturated].base_last;
			first_fiber_sat=ColorSection->DescrSec[Sky_saturated].base_first;
			max_gray_sat=ColorSection->DescrSec[Sky_saturated].upper_gray;
			min_gray_sat=ColorSection->DescrSec[Sky_saturated].lower_gray;
			weight_sat_section=ColorSection->section_weight[Sky_saturated];
			gray_zone_sat=gray_zones[mean_gray_sat];
			hue_zone_sat=hue_zones[mean_gray_sat];
			last_srip_ratio=(16*last_fiber_sat)/NumStrips;
			fiber_length_sat=last_fiber_sat-first_fiber_sat+1;
			max_min_fiber=std::max(first_opt_fiber,first_fiber_sat);
		    min_max_fiber=std::min(last_opt_fiber,last_fiber_sat);
			inters_fiber_length=min_max_fiber-max_min_fiber+1;
			if(inters_fiber_length>0)
			{
			length_fiber_ratio=(16*inters_fiber_length)/fiber_length_sat;
			length_fiber_ratio1=(16*inters_fiber_length)/fiber_length_opt;
			}
			else
			{
				length_fiber_ratio=-1;
				length_fiber_ratio1=-1;

			}
			if(oppos_opt>=0)
			       {//op>=0
		    max_min_fiber=std::max(oppos_first_fiber,first_fiber_sat);
		    min_max_fiber=std::min(oppos_last_fiber,last_fiber_sat);
			inters_fiber_length=min_max_fiber-max_min_fiber+1;
			if(inters_fiber_length>0)
			{
			oppos_length_fiber_ratio=(16*inters_fiber_length)/fiber_length_opt;
			oppos_length_fiber_ratio1=(16*inters_fiber_length)/fiber_length_oppos;
			}
			else
			{
				oppos_length_fiber_ratio=-1;
				oppos_length_fiber_ratio1=-1;

			}
			      }//op>=0
			mean_max=std::max(mean_gray_sat,mean_gray_opt);
		    max_min=std::min(max_gray_sat,max_gray_opt);
			mean_min=std::min(mean_gray_sat,mean_gray_opt);
		    max_max=std::max(max_gray_sat,max_gray_opt);
			length_new=max_gray_sat-mean_gray_sat+1;
			length_max=max_max-mean_min+1;

		joint_length=max_max-mean_min+1;
		ratio1_length=(16*length_new)/(joint_length);
				ratio1_length1=(16*length_opt)/(joint_length);
			if((max_min>=mean_max))
			{//m>m
				min_length=max_min-mean_max+1;
				length_min=std::min(length_new,length_opt);
				max_length=std::max(length_new,length_opt);

				ratio_length=(16*min_length)/(length_new);
				ratio_length1=(16*min_length)/(length_opt);
			}
			if(((length_fiber_ratio1>=8)&&(length_fiber_ratio>=8))||
		   ((length_fiber_ratio1>=14)&&(length_fiber_ratio>=6)&&(last_srip_ratio>=15))||
		   ((oppos_length_fiber_ratio1>=8)&&(oppos_length_fiber_ratio>=8))||
		   ((oppos_length_fiber_ratio1>=14)&&(oppos_length_fiber_ratio>=6)&&
		   (last_srip_ratio>=15)))
		{//cond1
			if(((ratio_length>0)&&(ratio_length1>0)&&(gray_difference<=13))||((hue_difference_opp<=2)&&
				(saturation_difference_opp<=2)&&(gray_difference_opp<=6)))
			{//cond2
				/*if((hue_zone_sat>=1)&&(hue_zone_sat<=2))
				{//very bad mistake!!!
					goto L;
				}*/
				if((hue_zone_sat>=3)&&(mean_hue_sat>=19))
				{//last_cor02.11.15
				*(SkyGreenComponents+Sky_saturated)=1;
				          if(!first_section)
			              {
							  LeftSkyComp[count_left]=Sky_saturated;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=Sky_saturated;
				              count_right++;
						  NumberRightSkyComp++;
						  }
		if(first_fiber_sat<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber_sat;
		}
		if(last_fiber_sat>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber_sat;
		}
		if(mean_gray_sat>UpperMeanGray)
		{
			UpperMeanGray=mean_gray_sat;
		}
		if(mean_gray_sat<LowerMeanGray)
		{
			LowerMeanGray=mean_gray_sat;
		}
			}//last_cor02.11.15
		}//cond2
			}//cond1
		}//ss>=0
			for(int count_sec=first_section;count_sec<last_section;count_sec++)
			{//loop_sky_finding
				first_last_ratio_left=-1;
				first_last_ratio_right=-1;
				if(count_sec!=section_opt)
				//if((count_sec!=section_opt)&&(count_sec!=Sky_saturated))
				{//cond!=
					if(*(SkyGreenComponents+count_sec)<0)
					{
						goto L;
					}
					left_right_relation=0;
		    last_fiber=ColorSection->section_fibre_last[count_sec];
			first_fiber=ColorSection->section_fibre_first[count_sec];
			last_bunch=ColorSection->DescrSec[count_sec].location_of_section[last_fiber];
			first_bunch=ColorSection->DescrSec[count_sec].location_of_section[first_fiber];
			if(last_bunch>0)
			{//lb>
				last_bunch--;
				last_bunch=ColorInt[last_fiber].old_bunch_number[last_bunch];
				last_fiber_beg=
					ColorInt[last_fiber].ColoredIntervalsStructure->BegInterv[last_bunch];
				last_fiber_end=
					ColorInt[last_fiber].ColoredIntervalsStructure->EndInterv[last_bunch];
				last_fiber_length=last_fiber_end-last_fiber_beg+1;
				if(first_opt_bunch_beg>=0)
				{
				max_bunch_left=std::min(last_fiber_end,first_opt_bunch_end);
		        min_bunch_right=std::max(last_fiber_beg,first_opt_bunch_beg);
			inters_bunch_length=max_bunch_left-min_bunch_right+1;
			if(inters_bunch_length>0)
			{
			oppos_length_bunch_ratio=(16*inters_bunch_length)/last_fiber_length;
			oppos_length_bunch_ratio1=(16*inters_bunch_length)/first_opt_bunch_length;
			}
				}
			}//lb>
			if(first_bunch>0)
			{//fb>
				first_bunch--;
				first_bunch=ColorInt[first_fiber].old_bunch_number[first_bunch];
				first_fiber_beg=
					ColorInt[first_fiber].ColoredIntervalsStructure->BegInterv[first_bunch];
				first_fiber_end=
					ColorInt[first_fiber].ColoredIntervalsStructure->EndInterv[first_bunch];
				first_fiber_length=first_fiber_end-first_fiber_beg+1;
				first_last_ratio=(16*last_fiber_length)/first_fiber_length;
				//first_last_ratio=(16*last_fiber_length)/(first_fiber_length+last_fiber_length);
				if(first_opt_bunch_left_length>0)
				{
	first_last_ratio_left=(16*first_fiber_length)/(first_opt_bunch_left_length+first_fiber_length);
				}
	if(first_opt_bunch_right_length>0)
				{
	first_last_ratio_right=(16*first_fiber_length)/(first_opt_bunch_right_length+first_fiber_length);
				}
			}//fb>
			min_left=ColorSection->section_left_end[count_sec];
		    max_right=ColorSection->section_right_end[count_sec];
			width_length=max_right-min_left+1;
			width_opt_length=max_right_opt-min_left_opt+1;
			inter_width_legth=std::min(max_right,max_right_opt)-std::max(min_left,min_left_opt);
			if(inter_width_legth>=0)
			{
			 ratio_width=(16*inter_width_legth)/width_length;
		     ratio_width1=(16*inter_width_legth)/width_opt_length;
			}
			else
			{
			ratio_width=0;
		    ratio_width1=0;
			}
			fiber_length=last_fiber-first_fiber+1;
			shape_ratio=width_length/(StripWidth*fiber_length);
			last_srip_ratio=(16*last_fiber)/NumStrips;
			first_strip_ratio=(16*first_fiber)/NumStrips;
			last_fiber_difference=abs(last_opt_fiber-last_fiber);
			invert_shape_ratio=(StripWidth*fiber_length)/width_length;
			mean_hue=ColorSection->section_mean_hue[count_sec];
		hue_dif=abs(mean_hue-mean_hue_opt);
		hue_difference=std::min(hue_dif,NUM_HUES-hue_dif);
		sect_hue_zone=hue_zones[mean_hue];
		hue_zone_difference=abs(sect_hue_zone-sect_hue_zone_opt);
		hue_zone_difference=std::min(hue_zone_difference,5-hue_zone_difference);
		mean_saturation=ColorSection->section_mean_saturation[count_sec];
		pos_sat_dev=std::max(mean_saturation,mean_saturation_opt)/2;
		saturation_difference=abs(mean_saturation-mean_saturation_opt);
		mean_gray=ColorSection->section_mean_gray[count_sec];
		gray_difference=abs(mean_gray-mean_gray_opt);
		sect_incline=*(InclinedComponent+count_sec);
		sect_incl_curve=*(InclineCurve+count_sec);
		sect_vert=*(VerticalComponent+count_sec);
		max_gray=ColorSection->DescrSec[count_sec].upper_gray;
		min_gray=ColorSection->DescrSec[count_sec].lower_gray;
		sect_gray_zone=gray_zones[mean_gray];
		gray_zone_difference=abs(sect_gray_zone-sect_gray_zone_opt);
		if(oppos_opt>=0)
		{
		hue_difference_opp=abs(mean_hue-mean_hue_opp);
		gray_difference_opp=abs(mean_gray-mean_gray_opp);
		saturation_difference_opp=abs(mean_saturation-mean_sat_opp);
		hue_zone_difference_opp=abs(sect_hue_zone-sect_hue_zone_opp);
		hue_zone_difference_opp=std::min(hue_zone_difference_opp,5-hue_zone_difference_opp);
		}
			max_min_fiber=std::max(first_opt_fiber,first_fiber);
		    min_max_fiber=std::min(last_opt_fiber,last_fiber);
			inters_fiber_length=min_max_fiber-max_min_fiber+1;
			if(inters_fiber_length>0)
			{
			length_fiber_ratio=(16*inters_fiber_length)/fiber_length;
			length_fiber_ratio1=(16*inters_fiber_length)/fiber_length_opt;
			}
			else
			{
				if((first_last_ratio<=5)&&(first_strip_ratio<=9)&&
	(last_fiber<first_opt_fiber)&&(last_fiber<oppos_first_fiber)&&(ratio_width>=8))
				{
					goto L;
				}
	if((first_last_ratio<=5)&&(first_strip_ratio<=12)&&((first_last_ratio_left>=0)&&
	(first_last_ratio_left<=4)||((first_last_ratio_right>=0)&&
	(first_last_ratio_right<=4)))&&
	(last_fiber<first_opt_fiber)&&(last_fiber<oppos_first_fiber)&&(ratio_width>=8))
				{
					goto L;
				}
				length_fiber_ratio=-1;
				length_fiber_ratio1=-1;
				if((last_fiber<(first_opt_fiber-2))&&(last_fiber<(oppos_first_fiber-2))&&
					(last_srip_ratio<=10))
				{//last_cor27.10.15
				goto L;
				}
	if((last_fiber<first_opt_fiber)&&(last_fiber<oppos_first_fiber)&&
					(first_strip_ratio<8))
				{//last_cor16.03.16
				goto L;
				}
			}
			       if(oppos_opt>=0)
			       {//op>=0
		width_opt_length=oppos_max_right-oppos_min_left+1;
		inter_width_legth=std::min(max_right,oppos_max_right)-std::max(min_left,oppos_min_left);
		if(inter_width_legth>=0)
			{
			 oppos_ratio_width=(16*inter_width_legth)/width_length;
		     oppos_ratio_width1=(16*inter_width_legth)/width_opt_length;
			}
			else
			{
			oppos_ratio_width=0;
		    oppos_ratio_width1=0;
			}
		    max_min_fiber=std::max(oppos_first_fiber,first_fiber);
		    min_max_fiber=std::min(oppos_last_fiber,last_fiber);
			inters_fiber_length=min_max_fiber-max_min_fiber+1;
			if(inters_fiber_length>0)
			{
			oppos_length_fiber_ratio=(16*inters_fiber_length)/fiber_length;
			oppos_length_fiber_ratio1=(16*inters_fiber_length)/fiber_length_oppos;
			}
			else
			{
				oppos_length_fiber_ratio=-1;
				oppos_length_fiber_ratio1=-1;

			}
			      }//op>=0
			weight_of_section=ColorSection->section_weight[count_sec];
		   weight_ratio=(32*weight_of_section)/(weight_of_section+weight_opt_section);

		   if(((min_left>max_right_opt_right)||(max_right<min_left_opt))&&(!ratio_width))
		   {
			   if((length_fiber_ratio1<3)&&(length_fiber_ratio<12))
			   {
			   if(weight_ratio<=4)
			   {
				   goto L;
			   }
			   }
		   }

			if(!first_section)
			{
				if(SkyMainRight>=0)
				{
		left_right_relation=
		SectionNeighborsLeftRight[count_sec*(NUM_SECT1/2)+SkyMainRight-number_of_sections_left];
				}
				else
				{
					left_right_relation=0;
				}

		if(left_right_relation>0)
		{
			fiber_length_ratio=(16*left_right_relation)/fiber_length;

		}
			}
			else
			{
				if(SkyMainLeft>=0)
				{
		left_right_relation=
		SectionNeighborsLeftRight[SkyMainLeft*(NUM_SECT1/2)+count_sec-number_of_sections_left];
				}
				else
				{
					left_right_relation=0;
				}
		if(left_right_relation>0)
		{
			fiber_length_ratio=(16*left_right_relation)/fiber_length;
		}
			}
		if(((last_srip_ratio>=12)||((first_fiber>LowerSkyFiber)&&(last_srip_ratio>=10)))||
			((last_fiber>first_opt_fiber)&&(length_fiber_ratio>=8))||
		   ((last_fiber>oppos_first_fiber)&&(oppos_length_fiber_ratio>=8))||
	((oppos_length_bunch_ratio>=10)&&(oppos_length_bunch_ratio1>=5)&&((last_fiber+1)==first_opt_fiber)&&
	 (gray_zone_difference<=1)))
			{//firstlastcond
		   if(((last_fiber>(5*NumStrips/8))||(((last_fiber+1)==first_opt_fiber)&&
			   (oppos_length_bunch_ratio>=10)))&&(first_fiber>NumStrips/3))
						{//upper_part

		if(sect_hue_zone==3)
		{//hzs3
							  if((mean_hue>=8)&&(mean_hue<=22))
							  {//mh822
								  if(mean_saturation>=4)
								  {
									     if(MassiveGrayLow>0)
						                 {
									  if(mean_gray<=NUM_INTEN1/2)
									  {
							  if(mean_gray<(2*MassiveGrayLow))
							  {
										  goto L;
							  }
									  }
									      }
								  }
							  }//mh816
		}//hzs3

		if(gray_zone_difference>=3)
		{
		goto L;
		}
		else
		{
			if(gray_zone_difference>=2)
		    {
		     if(inter_width_legth<0)
			 {
				 if(weight_ratio<=2)
				 {
				 goto L;
				 }
			 }
			}

		}
		if(weight_ratio<=2)
		{//last_cor30.09.15
			if(inter_width_legth<0)
			{
				if(abs(inter_width_legth)>=StripLength/8)
				{
					goto L;
				}
			}
		}
		if(!shape_ratio)
		{
			if(invert_shape_ratio>4)
			{
				goto L;
			}
		}
		if(!weight_ratio)
		{
			if(invert_shape_ratio>4)
			{
				goto L;
			}
			if((min_left-max_right_opt_right)>2*width_length)
			{
				goto L;
			}
			if((min_left_opt-max_right)>2*width_length)
			{
				goto L;
			}
		}
		if(left_right_relation>0)
		{
			if((saturation_difference_opp<=1)&&(!hue_zone_difference_opp))
			{//last_cor04.11.15

			if((fiber_length_ratio>=10)||(oppos_length_fiber_ratio>=10))
			{//last_cor30.10.15
		*(SkyGreenComponents+count_sec)=1;
		                  if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
		                   RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
		if(first_fiber<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber;
		}
		if(last_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber;
		}
		if(mean_gray>UpperMeanGray)
		{
			UpperMeanGray=mean_gray;
		}
		if(mean_gray<LowerMeanGray)
		{
			LowerMeanGray=mean_gray;
		}
					goto L;
			}
			else
			{
				if(shape_ratio>1)
				{
					if(((oppos_ratio_width>=12)||(oppos_ratio_width1>=12))||((ratio_width>=12)||(ratio_width1>=12)))
					{//last_cor30.10.15
						if(shape_ratio>=4)
						{

						   if((last_fiber>oppos_last_fiber))
						  {
							  if((!hue_zone_difference)&&(sect_gray_zone>=6))
							  {
								  *(SkyGreenComponents+count_sec)=1;
								  if(!first_section)
								  {
									  LeftSkyComp[count_left]=count_sec;
									  count_left++;
									  NumberLeftSkyComp++;
								  }
								  else
								  {
									  RightSkyComp[count_right]=count_sec;
									  count_right++;
									  NumberRightSkyComp++;
								  }
								  if(first_fiber<LowerSkyFiber)
								  {
									  LowerSkyFiber=first_fiber;
								  }
								  if(last_fiber>UpperSkyFiber)
								  {
									  UpperSkyFiber=last_fiber;
								  }
								  if(mean_gray>UpperMeanGray)
								  {
									  UpperMeanGray=mean_gray;
								  }
								  if(mean_gray<LowerMeanGray)
								  {
									  LowerMeanGray=mean_gray;
								  }
								  goto L;
							  }

						  }
						}
					}//last_cor30.10.15
				}//last_cor04.11.15
		if((oppos_length_fiber_ratio1>=14)&&(oppos_ratio_width1>=12))
		{
			if((hue_zone_difference<=1)&&(gray_zone_difference<=2))
			{
		*(SkyGreenComponents+count_sec)=1;
		                  if(!first_section)
			              {
						  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
		if(first_fiber<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber;
		}
		if(last_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber;
		}
		if(mean_gray>UpperMeanGray)
		{
			UpperMeanGray=mean_gray;
		}
		if(mean_gray<LowerMeanGray)
		{
			LowerMeanGray=mean_gray;
		}
					goto L;
			}
		}
				}
			}
		}
		if(((SkyRightHueZone==4)||((SkyRightHueZone==3)&&(mean_hue_opt_right>=17)))
		   &&((SkyLeftHueZone==4)||((SkyLeftHueZone==3)&&(mean_hue_opt_left>=17))))
		{//last_cor28.10.15
			if((oppos_length_fiber_ratio>=15)||(length_fiber_ratio>=15))
			{
				if(max_gray>LowerMeanGray)
				{
					    if((mean_saturation>=2)&&(gray_zone_difference<=1))
					    {

					if(sect_gray_zone>=5)
					{

					if((hue_difference<=4)||(hue_difference_opp<=4))
					{
	if(first_fiber_length>=StripLength/8)
	{
		if(mean_gray>=41)
		{
			if(!first_section)
			{
				LeftSkyComp[count_left]=count_sec;
				count_left++;
				NumberLeftSkyComp++;
			}
			else
			{
				RightSkyComp[count_right]=count_sec;
				count_right++;
				NumberRightSkyComp++;
			}
			*(SkyGreenComponents+count_sec)=1;
			if(first_fiber<LowerSkyFiber)
			{
				LowerSkyFiber=first_fiber;
			}
			if(last_fiber>UpperSkyFiber)
			{
				UpperSkyFiber=last_fiber;
			}
			if(mean_gray>UpperMeanGray)
			{
				UpperMeanGray=mean_gray;
			}
			if(mean_gray<LowerMeanGray)
			{
				LowerMeanGray=mean_gray;
			}
			goto L;

		}
	}

					}
					}
						}
				}

			}
			if((oppos_ratio_width>=14)||(ratio_width>=14))
			{
				if((oppos_length_fiber_ratio>=12)||(length_fiber_ratio>=12))
				{
					if(last_srip_ratio>=14)
					{
						if((!hue_zone_difference)&&(saturation_difference<=1))
						{
							if((sect_gray_zone>=5)||(gray_difference<=6))
							{
								if(!first_section)
								{
									LeftSkyComp[count_left]=count_sec;
									count_left++;
									NumberLeftSkyComp++;
								}
								else
								{
									RightSkyComp[count_right]=count_sec;
									count_right++;
									NumberRightSkyComp++;
								}
								*(SkyGreenComponents+count_sec)=1;
								if(first_fiber<LowerSkyFiber)
								{
									LowerSkyFiber=first_fiber;
								}
								if(last_fiber>UpperSkyFiber)
								{
									UpperSkyFiber=last_fiber;
								}
								if(mean_gray>UpperMeanGray)
								{
									UpperMeanGray=mean_gray;
								}
								if(mean_gray<LowerMeanGray)
								{
									LowerMeanGray=mean_gray;
								}
								goto L;
							}
						}
					}

				}
			}

			if(((!gray_zone_difference)||(sect_gray_zone>sect_gray_zone_opt))&&((!hue_zone_difference)||(hue_difference<=2))&&(saturation_difference<=1))
			{
				if((oppos_length_fiber_ratio>0)||(length_fiber_ratio>0))
				{

				   if((ratio_width>=13)||(oppos_ratio_width>=13))
				   {
					   if(hue_difference<=3)
					   {

					     if(gray_difference<=4)
					     {
							 if(!first_section)
							 {
								 LeftSkyComp[count_left]=count_sec;
								 count_left++;
								 NumberLeftSkyComp++;
							 }
							 else
							 {
								 RightSkyComp[count_right]=count_sec;
								 count_right++;
								 NumberRightSkyComp++;
							 }
							 *(SkyGreenComponents+count_sec)=1;
							 if(first_fiber<LowerSkyFiber)
							 {
								 LowerSkyFiber=first_fiber;
							 }
							 if(last_fiber>UpperSkyFiber)
							 {
								 UpperSkyFiber=last_fiber;
							 }
							 if(mean_gray>UpperMeanGray)
							 {
								 UpperMeanGray=mean_gray;
							 }
							 if(mean_gray<LowerMeanGray)
							 {
								 LowerMeanGray=mean_gray;
							 }
							 goto L;
					     }
					   }


				   }
				}

			}
			sky_left_index=-1;
		    sky_right_index=-1;
			if(sect_gray_zone>=5)
			{
		indicative=-1;
		locate=LocateOn(count_sec,&sect_under,&sect_under_right);

		if(locate==1)
		{
			sky_left_index=*(SkyGreenComponents+sect_under);
		 if(sect_under==SkyMainLeft)
		 {
			 indicative=1;
		 }
		}
		else
		{
			if(locate==3)
			{
		sky_right_index=*(SkyGreenComponents+sect_under_right);
		if(sect_under_right==SkyMainRight)
		 {
			 indicative=2;
		 }
			}
			else
			{
				if(locate==2)
			    {
		sky_left_index=*(SkyGreenComponents+sect_under);
		sky_right_index=*(SkyGreenComponents+sect_under_right);
					if(sect_under==SkyMainLeft)
		             {
			         indicative=3;
		             }
		            if(sect_under_right==SkyMainRight)
		            {
						if(indicative==3)
						{
			        indicative+=1;
						}
						else
						{
		                indicative=5;
						}
		            }
				}
			}
		}
			}
			else
			{
				locate=-1;
			}
			if((locate>0)&&(indicative<0))
			{
		if((sky_left_index==1)||(sky_right_index==1))
		{
			indicative=6;
		}
			}
			if((last_srip_ratio>=15)||((locate>0)&&(indicative>0)))
			{
		if(((ratio_width>=10)||(oppos_ratio_width>=10))||
		   (((oppos_length_fiber_ratio>=14)||(length_fiber_ratio>=14))&&
		   ((ratio_width>=8)||(oppos_ratio_width>=8))))
		{//rwlast_cor30.10.15&02.11.15
		        if((mean_gray>mean_gray_opt)||(!gray_zone_difference)||(mean_gray<=mean_gray_opt))
				{//mglast_cor30.10.15
					if(sect_gray_zone>=6)
					{
					if(!first_section)
										              {
														  LeftSkyComp[count_left]=count_sec;
											              count_left++;
													  NumberLeftSkyComp++;
													  }
													  else
													  {
														  RightSkyComp[count_right]=count_sec;
											              count_right++;
													  NumberRightSkyComp++;
													  }
												*(SkyGreenComponents+count_sec)=1;
												if(first_fiber<LowerSkyFiber)
									            {
										        LowerSkyFiber=first_fiber;
									            }
									            if(last_fiber>UpperSkyFiber)
									            {
										        UpperSkyFiber=last_fiber;
									            }
												if(mean_gray>UpperMeanGray)
												{
													UpperMeanGray=mean_gray;
												}
												if(mean_gray<LowerMeanGray)
												{
													LowerMeanGray=mean_gray;
												}
												goto L;
					}
				}//mg
		}//rw
			}
			if(last_fiber<=first_opt_fiber)
			{//lfff
			if((gray_zone_difference<=1)&&(sect_gray_zone>=6))
			{//gz
				if((!hue_zone_difference_opp)||(!hue_zone_difference)||(hue_difference<=4)||(hue_difference_opp<=4))
				{//hz
					indicative=-1;
					sect_under=-1;
					sect_under_right;
					locate=LocateUnder(count_sec,&sect_under,&sect_under_right,first_opt_fiber);
					if(locate==1)
					{//loc
						sky_left_index=*(SkyGreenComponents+sect_under);
						if(sect_under==SkyMainLeft)
						{
							indicative=1;
						}
					}//loc
					else
					{//eloc
						if(locate==3)
						{//lc3
							sky_right_index=*(SkyGreenComponents+sect_under_right);
							if(sect_under_right==SkyMainRight)
							{
								indicative=2;
							}
						}//lc3
						else
						{//elc3
							if(locate==2)
							{//lc2
								sky_left_index=*(SkyGreenComponents+sect_under);
								sky_right_index=*(SkyGreenComponents+sect_under_right);
								if(sect_under==SkyMainLeft)
								{
									indicative=3;
								}
								if(sect_under_right==SkyMainRight)
								{
									if(indicative==3)
									{
										indicative+=1;
									}
									else
									{
										indicative=5;
									}
								}
							}//elc2
						}//elc3
					}//eloc
				/*}
				else
				{
					locate=-1;
				}*/
					if((locate>0)&&(indicative<0))
					{
						if((sky_left_index==1)||(sky_right_index==1))
						{
							indicative=6;
						}
					}
					if((locate>0)&&(indicative>0))
					{
						if((ratio_width>=10)||(oppos_ratio_width>=10))
						{
							if(!first_section)
							{
								LeftSkyComp[count_left]=count_sec;
								count_left++;
								NumberLeftSkyComp++;
							}
							else
							{
								RightSkyComp[count_right]=count_sec;
								count_right++;
								NumberRightSkyComp++;
							}
							*(SkyGreenComponents+count_sec)=1;
							if(first_fiber<LowerSkyFiber)
							{
								LowerSkyFiber=first_fiber;
							}
							if(last_fiber>UpperSkyFiber)
							{
								UpperSkyFiber=last_fiber;
							}
							if(mean_gray>UpperMeanGray)
							{
								UpperMeanGray=mean_gray;
							}
							if(mean_gray<LowerMeanGray)
							{
								LowerMeanGray=mean_gray;
							}
							goto L;

						}
					}
				}//hz


			}//gz
			}//lfff
		}//last_cor28.10.15
		if((SkyRightHueZone==4)&&(SkyLeftHueZone==4))
		{//last_cor15.06.15
			if((hue_difference<=4)||(hue_difference_opp<=4))
				{//hd4
				if(saturation_difference<=2)
				  {//last_cor12.11.15
	if((hue_difference<=2)||(hue_difference_opp<=2))
	{
	   if((ratio_width>=14)||(oppos_ratio_width>=14))
	    {
			if((max_gray>LowerMeanGray)&&(mean_gray>=42))
			{
				if((mean_saturation>=2)&&(gray_zone_difference<=1))
				{

				if(!first_section)
				{
					LeftSkyComp[count_left]=count_sec;
					count_left++;
					NumberLeftSkyComp++;
				}
				else
				{
					RightSkyComp[count_right]=count_sec;
					count_right++;
					NumberRightSkyComp++;
				}
				*(SkyGreenComponents+count_sec)=1;
				if(first_fiber<LowerSkyFiber)
				{
					LowerSkyFiber=first_fiber;
				}
				if(last_fiber>UpperSkyFiber)
				{
					UpperSkyFiber=last_fiber;
				}
				if(mean_gray>UpperMeanGray)
				{
					UpperMeanGray=mean_gray;
				}
				if(mean_gray<LowerMeanGray)
				{
					LowerMeanGray=mean_gray;
				}
				goto L;
				}
			}
	    }
	}

					if((last_fiber>first_opt_fiber)||((shape_ratio>=3)&&(abs(last_fiber-first_opt_fiber)<=2)
						&&(last_srip_ratio>=13)))
					{
						if((ratio_width>=8)||(oppos_ratio_width>=8))
						{
							if(max_gray>mean_gray_opt)
							{
								if((mean_saturation>=2)&&(gray_zone_difference<=1))
								{
								if((sect_gray_zone>=5)&&(mean_gray>=46)&&(weight_ratio>=18)&&(last_fiber_length>=StripLength/4)&&
			(first_fiber_length>=StripLength/4))
								{
									if(!first_section)
									{
										LeftSkyComp[count_left]=count_sec;
										count_left++;
										NumberLeftSkyComp++;
									}
									else
									{
										RightSkyComp[count_right]=count_sec;
										count_right++;
										NumberRightSkyComp++;
									}
									*(SkyGreenComponents+count_sec)=1;
									if(first_fiber<LowerSkyFiber)
									{
										LowerSkyFiber=first_fiber;
									}
									if(last_fiber>UpperSkyFiber)
									{
										UpperSkyFiber=last_fiber;
									}
									if(mean_gray>UpperMeanGray)
									{
										UpperMeanGray=mean_gray;
									}
									if(mean_gray<LowerMeanGray)
									{
										LowerMeanGray=mean_gray;
									}
									goto L;
								}
								}
	indicative=-1;
					sect_under=-1;
					sect_under_right;
					locate=LocateUnder(count_sec,&sect_under,&sect_under_right,first_opt_fiber);
					if(locate==1)
					{//loc
						sky_left_index=*(SkyGreenComponents+sect_under);
						if(sect_under==SkyMainLeft)
						{
							indicative=1;
						}
					}//loc
					else
					{//eloc
						if(locate==3)
						{//lc3
							sky_right_index=*(SkyGreenComponents+sect_under_right);
							if(sect_under_right==SkyMainRight)
							{
								indicative=2;
							}
						}//lc3
						else
						{//elc3
							if(locate==2)
							{//lc2
								sky_left_index=*(SkyGreenComponents+sect_under);
								sky_right_index=*(SkyGreenComponents+sect_under_right);
								if(sect_under==SkyMainLeft)
								{
									indicative=3;
								}
								if(sect_under_right==SkyMainRight)
								{
									if(indicative==3)
									{
										indicative+=1;
									}
									else
									{
										indicative=5;
									}
								}
							}//elc2
						}//elc3
					}//eloc
				/*}
				else
				{
					locate=-1;
				}*/
					if((locate>0)&&(indicative<0))
					{
						if((sky_left_index==1)||(sky_right_index==1))
						{
							indicative=6;
						}
					}
							}
						}


					}
				}//last_cor12.11.15
					if(last_fiber>last_opt_fiber)
					{//last_cor27.10.15
						if((ratio_width>=10)||(oppos_ratio_width>=10))
								{//rwlast_cor30.10.15
							if((gray_zone_difference<=1)&&(sect_gray_zone>=6))
							{
								if(!first_section)
										              {
														  LeftSkyComp[count_left]=count_sec;
											              count_left++;
													  NumberLeftSkyComp++;
													  }
													  else
													  {
														  RightSkyComp[count_right]=count_sec;
											              count_right++;
													  NumberRightSkyComp++;
													  }
												*(SkyGreenComponents+count_sec)=1;
												if(first_fiber<LowerSkyFiber)
									            {
										        LowerSkyFiber=first_fiber;
									            }
									            if(last_fiber>UpperSkyFiber)
									            {
										        UpperSkyFiber=last_fiber;
									            }
												if(mean_gray>UpperMeanGray)
												{
													UpperMeanGray=mean_gray;
												}
												if(mean_gray<LowerMeanGray)
												{
													LowerMeanGray=mean_gray;
												}
												goto L;
							}

								}//rw

					}//last_cor27.10.15
					if(sect_gray_zone>=sect_gray_zone_opt)
					{//last_cor27.10.15
						if(((last_srip_ratio>=13)&&(first_strip_ratio>=8)&&(shape_ratio>=2))||
							((shape_ratio>=3)&&(last_srip_ratio>=12)&&(first_strip_ratio>=8)))
						{//lsr
							if(mean_gray>mean_gray_opt)
							{//mg
		if((last_fiber>=(first_opt_fiber-2))||(last_fiber>=(oppos_first_fiber-2)))
		//if(((length_fiber_ratio1>0)||(length_fiber_ratio>0))||((oppos_length_fiber_ratio1>0)||
		   //(oppos_length_fiber_ratio>0)))
		{//intercond1
			if((ratio_width>=14)||(oppos_ratio_width>=14))
			{//rw
				if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
					*(SkyGreenComponents+count_sec)=1;
					if(first_fiber<LowerSkyFiber)
		            {
			        LowerSkyFiber=first_fiber;
		            }
		            if(last_fiber>UpperSkyFiber)
		            {
			        UpperSkyFiber=last_fiber;
		            }
					if(mean_gray>UpperMeanGray)
					{
						UpperMeanGray=mean_gray;
					}
					if(mean_gray<LowerMeanGray)
					{
						LowerMeanGray=mean_gray;
					}
					goto L;
			}//rw

		}//intercond1

							}//mg
						}//lsr
					}//last_cor27.10.15
					if(last_srip_ratio>=14)
					{//lsr14
		if(((length_fiber_ratio1>=8)&&(length_fiber_ratio>=14))||((oppos_length_fiber_ratio1>=8)&&
		   (oppos_length_fiber_ratio>=14)))
						{//intercond
							if((saturation_difference<=2)||(saturation_difference_opp<=2))
							{//sd2sdo2
								if((gray_difference<=8)||(gray_difference_opp<=6))
								{//gd8gd06
		                  if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
					*(SkyGreenComponents+count_sec)=1;
					if(first_fiber<LowerSkyFiber)
		            {
			        LowerSkyFiber=first_fiber;
		            }
		            if(last_fiber>UpperSkyFiber)
		            {
			        UpperSkyFiber=last_fiber;
		            }
					if(mean_gray>UpperMeanGray)
					{
						UpperMeanGray=mean_gray;
					}
					if(mean_gray<LowerMeanGray)
					{
						LowerMeanGray=mean_gray;
					}
					goto L;
								}//gd8gd06
						 }//sd2sdo2


						}//intercond

					}//lsr14

		               if((last_srip_ratio>=13)&&(first_strip_ratio>=11))
					   {
						   if((length_fiber_ratio>=15)||(oppos_length_fiber_ratio>=15))
						   {
							       if(shape_ratio>=2)
								   {
							   if((saturation_difference<=2)||(saturation_difference_opp<=2))
							   {
								   if((gray_difference<=8)||(gray_difference_opp<=6))
								   {
									   if(!first_section)
			                            {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						                }
						                else
						                {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						                }
					*(SkyGreenComponents+count_sec)=1;
					if(first_fiber<LowerSkyFiber)
		            {
			        LowerSkyFiber=first_fiber;
		            }
		            if(last_fiber>UpperSkyFiber)
		            {
			        UpperSkyFiber=last_fiber;
		            }
					if(mean_gray>UpperMeanGray)
					{
						UpperMeanGray=mean_gray;
					}
					if(mean_gray<LowerMeanGray)
					{
						LowerMeanGray=mean_gray;
					}
					goto L;

								   }
							   }
								   }
						   }
					   }


				}//hd4
			if(mean_saturation>=1)
			{//ms1

				if(abs(sect_hue_zone-4)>1)
				{
					goto L;
				}
			}
			if(mean_saturation_opt>=2)
			{
				if(sect_gray_zone<sect_gray_zone_opt)
				{
		if(gray_zone_difference>=2)
		{
		goto L;
		}
		else
		{
		if(gray_zone_difference>=1)
		{
			if(sect_gray_zone<=3)
			{
		if(mean_saturation_opt>mean_saturation)
		{
			if(saturation_difference>2)
			{
				if(hue_zone_difference>=1)
				{
					if(hue_difference>=8)
					{
						if(gray_difference>=8)
						{
						goto L;
						}
					}
				}
			}
		}
			if(gray_difference>=12)
			{
		goto L;
			}
			}
		}
		}
				}
			}
		}
		if((hue_zone_opt>=3)&&(hue_zone_opt<=4))
		{//last_cor15.06.15
		    if((mean_hue_opt>=17)&&(gray_zone_opt>5))
			{
		if(mean_saturation>=2)
		{
			if(sect_hue_zone<=2)
			{
				if(sect_gray_zone<=5)
				{
				goto L;
				}
			}

		}
			}
		}//last_cor15.06.15
			if(mean_gray<min_gray_opt)
			{//last_cor27.05.15
			dev1=mean_gray-min_gray;
			dev2=max_gray-mean_gray;
			if((dev1>=0)&&(dev2>=0))
			{
		deviation_gray=std::min((mean_gray-min_gray),(max_gray-mean_gray));
		max_gray=mean_gray+deviation_gray;
			}
			else
			{
		max_gray=mean_gray;
			}
			}//last_cor27.05.15
			if(max_gray<mean_gray_opt/2)
			{
				goto L;
			}
			mean_max=std::max(mean_gray,mean_gray_opt);
		    max_min=std::min(max_gray,max_gray_opt);
			mean_min=std::min(mean_gray,mean_gray_opt);
		    max_max=std::max(max_gray,max_gray_opt);
			length_new=max_gray-mean_gray+1;
			length_max=max_max-mean_min+1;

		joint_length=max_max-mean_min+1;
		ratio1_length=(16*length_new)/(joint_length);
				ratio1_length1=(16*length_opt)/(joint_length);
			if((max_min>=mean_max))
			{//m>m
				min_length=max_min-mean_max+1;
				length_min=std::min(length_new,length_opt);
				max_length=std::max(length_new,length_opt);

				ratio_length=(16*min_length)/(length_new);
				ratio_length1=(16*min_length)/(length_opt);
				    if(weight_ratio>=4)
				     {//wr4
						 if((oppos_length_fiber_ratio1>3)||(length_fiber_ratio1>3))
				if((ratio_length1>=8)&&(ratio_length>=5)&&(gray_difference<=8))
				{//rl
					      if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
					*(SkyGreenComponents+count_sec)=1;
		if(first_fiber<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber;
		}
		if(last_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber;
		}
		if(mean_gray>UpperMeanGray)
		{
			UpperMeanGray=mean_gray;
		}
		if(mean_gray<LowerMeanGray)
		{
			LowerMeanGray=mean_gray;
		}
					goto L;
				}//rl
				else
				{//erl
		                if((ratio_length1>=4)&&(ratio_length>=4))
				        {
							if(!hue_zone_difference)
							{
								if(mean_saturation>=mean_saturation_opt)
								{
									if(last_fiber>last_opt_fiber)
									{
										if((ratio_width>=8)&&(ratio_width1>=8))
											if(shape_ratio>=3)
											{
							if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
		                                 *(SkyGreenComponents+count_sec)=1;
		 if(first_fiber<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber;
		}
		if(last_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber;
		}
		if(mean_gray>UpperMeanGray)
		{
			UpperMeanGray=mean_gray;
		}
		if(mean_gray<LowerMeanGray)
		{
			LowerMeanGray=mean_gray;
		}
					                      goto L;
											}
									}
								}
							}
						}
				}//erl
					}//wr4
					else
					{
					                if(last_srip_ratio>=14)
					                {
						if((shape_ratio>=3)||((shape_ratio>=2)&&(length_fiber_ratio>=12)&&
							(ratio_width>=8)))
						{
							if((mean_saturation<=1)&&(mean_saturation<=mean_saturation_opt))
							{
								if(sect_gray_zone>=gray_zone_opt)
				                {
									if(ratio_length>=9)
									{
									*(SkyGreenComponents+count_sec)=1;
						if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
		if(first_fiber<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber;
		}
		if(last_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber;
		}
		if(mean_gray>UpperMeanGray)
		{
			UpperMeanGray=mean_gray;
		}
		if(mean_gray<LowerMeanGray)
		{
			LowerMeanGray=mean_gray;
		}
					                      goto L;
									}
								}
							}
						}
						             }
					}
				if(sect_gray_zone>=gray_zone_opt)
				{
					if(sect_hue_zone==hue_zone_opt)
					{
						if(hue_difference<=4)
						{
		if(saturation_difference<=1)
		{
			if(shape_ratio>=8)
			{//last_cor01.10.15
		if(first_strip_ratio>=12)
		{
			if((ratio_width>=10)&&(ratio_width1>=12))
			{
				if(ratio1_length1>=10)
				{
		                  if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
					     *(SkyGreenComponents+count_sec)=1;
		if(first_fiber<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber;
		}
		if(last_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber;
		}
		if(mean_gray>UpperMeanGray)
		{
			UpperMeanGray=mean_gray;
		}
		if(mean_gray<LowerMeanGray)
		{
			LowerMeanGray=mean_gray;
		}
					goto L;
				}
			}
		}
			}//last_cor01.10.15
			if(ratio1_length1>=15)
			{
				          if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
				*(SkyGreenComponents+count_sec)=1;
		if(first_fiber<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber;
		}
		if(last_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber;
		}
		if(mean_gray>UpperMeanGray)
		{
			UpperMeanGray=mean_gray;
		}
		if(mean_gray<LowerMeanGray)
		{
			LowerMeanGray=mean_gray;
		}
					goto L;
			}
		}
		else
		{
			           if(hue_difference<=2)
						{
							if((mean_saturation_opt>=3)&&(mean_saturation_opt>=3))
							{
								if(saturation_difference<=pos_sat_dev)
								{
						if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
		*(SkyGreenComponents+count_sec)=1;
		if(first_fiber<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber;
		}
		if(last_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber;
		}
		if(mean_gray>UpperMeanGray)
		{
			UpperMeanGray=mean_gray;
		}
		if(mean_gray<LowerMeanGray)
		{
			LowerMeanGray=mean_gray;
		}
					goto L;
								}
							}
			            }
		}
						}
					}
				}

		            /*if((ratio1_length1>=5)&&(ratio1_length<=10))
				    {
					*(SkyGreenComponents+count_sec)=1;
					goto L;
					}*/

			}//m>m
			else
			{
		           if((ratio1_length1>=8)&&(ratio1_length>8)&&(ratio1_length1>=8))
				    {
					*(SkyGreenComponents+count_sec)=1;
					     if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
		if(first_fiber<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber;
		}
		if(last_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber;
		}
		if(mean_gray>UpperMeanGray)
		{
			UpperMeanGray=mean_gray;
		}
		if(mean_gray<LowerMeanGray)
		{
			LowerMeanGray=mean_gray;
		}
					goto L;
					}
				   if((sect_gray_zone_opt==7)&&(mean_saturation_opt<=1))
				   {
		                    if(sect_gray_zone>=6)
							{
								if(sect_hue_zone==4)
								{
									if(last_fiber_difference<=1)
									{
										if((length_fiber_ratio>=14)||(length_fiber_ratio>=14))
										{
			*(SkyGreenComponents+count_sec)=1;
			              if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
		if(first_fiber<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber;
		}
		if(last_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber;
		}
		if(mean_gray>UpperMeanGray)
		{
			UpperMeanGray=mean_gray;
		}
		if(mean_gray<LowerMeanGray)
		{
			LowerMeanGray=mean_gray;
		}
					goto L;
										}

									}
									else
									{
										if(length_fiber_ratio>=15)
										{
		if((length_fiber_ratio>=14)&&(length_fiber_ratio1>=10))
		{
			if((oppos_ratio_width>=12)||(ratio_width>=12))
			{
				if(shape_ratio>=2)
				{
		*(SkyGreenComponents+count_sec)=1;
		                  if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
		if(first_fiber<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber;
		}
		if(last_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber;
		}
		if(mean_gray>UpperMeanGray)
		{
			UpperMeanGray=mean_gray;
		}
		if(mean_gray<LowerMeanGray)
		{
			LowerMeanGray=mean_gray;
		}
					goto L;
		     	}
			}
		}
										}
									}
								}
							}
				   }
			}

		if(count_sec==Sky_saturated)
		{//cs=ss
		if(((length_fiber_ratio1>=8)&&(length_fiber_ratio>=8))||
		   ((length_fiber_ratio1>=14)&&(length_fiber_ratio>=6)&&(last_srip_ratio>=15))||
		   ((oppos_length_fiber_ratio1>=8)&&(oppos_length_fiber_ratio>=8))||
		   ((oppos_length_fiber_ratio1>=14)&&(oppos_length_fiber_ratio>=6)&&
		   (last_srip_ratio>=15)))
		{
			if((ratio_length>0)&&(ratio_length1>0))
			{
				if((gray_difference>=13)&&(!ratio_width)&&(weight_ratio<=12))
				{
					goto L;
				}
				*(SkyGreenComponents+count_sec)=1;
				          if(!first_section)
			              {
							  LeftSkyComp[count_left]=count_sec;
				              count_left++;
						  NumberLeftSkyComp++;
						  }
						  else
						  {
							  RightSkyComp[count_right]=count_sec;
				              count_right++;
						  NumberRightSkyComp++;
						  }
		if(first_fiber<LowerSkyFiber)
		{
			LowerSkyFiber=first_fiber;
		}
		if(last_fiber>UpperSkyFiber)
		{
			UpperSkyFiber=last_fiber;
		}
		if(mean_gray>UpperMeanGray)
		{
			UpperMeanGray=mean_gray;
		}
		if(mean_gray<LowerMeanGray)
		{
			LowerMeanGray=mean_gray;
		}
					goto L;
			}
		}
		}//cs=ss
			sky_inhabitant=0;
		     for(int count_intens=mean_gray;count_intens<=max_gray;count_intens++)
			  {//loopintens
		     sky_value=sky_gr[count_intens];
				   if(sky_value)
				   {
				    sky_inhabitant=sky_value;
					break;
				   }
				}//loopintens
						}//upper_part
			}//firstlastcond
		L:;
				}//cond!=
		    }//loop_sky_finding
		}
///===========================================================================
void

CImageProcess::LabelingSkyBunches(int* bunch_matrix,int* sky_components,int comp_number)
{
	int section_number;
	int last_fiber;
	int first_fiber;
	int bunch_number;
	int ord_bunch_number;
	int covering_number;
	int old_bunch_number;

	for(int count_comp=0;count_comp<comp_number;count_comp++)
	{//loop_ext
   section_number=*(sky_components+count_comp);
   last_fiber=ColorSection->DescrSec[section_number].base_last;
   first_fiber=ColorSection->DescrSec[section_number].base_first;
for(int count_fiber=first_fiber;count_fiber<=last_fiber;count_fiber++)
{//loop_int
	covering_number=ColorInt[count_fiber].NumberOfIntervalsInCovering;
bunch_number=ColorDescrSect[section_number].location_of_section[count_fiber];
  if(bunch_number>0)
  {
 bunch_number-=1;
 bunch_number=ColorInt[count_fiber].old_bunch_number[bunch_number];
 ord_bunch_number=ColorInt[count_fiber].ordered_bunch_number[bunch_number];
old_bunch_number=ColorInt[count_fiber].old_ordered_bunch_number[ ord_bunch_number];
 if((bunch_number>=0)&&(old_bunch_number==bunch_number))
 {
*(bunch_matrix+ord_bunch_number+count_fiber*MaximumNumberOfCoveringElements)=section_number+1;
 }
 else
 {
	 ;
 }
  }
}//loop_int

	}//loop_ext
}
//=========================================================
void

CImageProcess::LeftRightSkyAddition(void)
{
	int number_of_sections;
	int number_of_left_sections;
	int feature_sky;
	int feature_sky1;
	int number_of_connected;
	int next_connected;
	int number_of_connections;
	int first_fiber;
	int last_fiber;
	int first_fiber_next;
	int last_fiber_next;
	int length;
	int length_next;
	int length_ratio;
	int length_ratio1;
	int last_strip_ratio;
	int last_strip_ratio_next;

	last_strip_ratio=0;
    last_strip_ratio_next=0;
	number_of_connected=0;
	number_of_sections=ColorSection->Number_of_sections;
	number_of_left_sections=ColorSection->Number_of_sections_left;
	for(int count_section=0;count_section<number_of_sections;count_section++)
	{//loop_ext
		feature_sky=SkyGreenComponents[count_section];
		if(feature_sky==1)
		{
		last_fiber=ColorSection->section_fibre_last[count_section];
	    first_fiber=ColorSection->section_fibre_first[count_section];
		last_strip_ratio=(16*last_fiber)/NumStrips;
		length=last_fiber-first_fiber+1;
        number_of_connected=NumberOfLRConnections[count_section];
			if(number_of_connected>=1)
			{
for(int count_connected=0;count_connected<number_of_connected;count_connected++)
{//loop_int
        next_connected=RightLeftConnections[count_connected*NUM_SECT1 + count_section];
        last_fiber_next=ColorSection->section_fibre_last[next_connected];
	    first_fiber_next=ColorSection->section_fibre_first[next_connected];
		last_strip_ratio_next=(16*last_fiber_next)/NumStrips;
        length_next=last_fiber_next-first_fiber_next+1;
feature_sky1=SkyGreenComponents[next_connected];
if(!feature_sky1)
{
	if(count_section<number_of_left_sections)
	{
number_of_connections=
SectionNeighborsLeftRight[count_section*(NUM_SECT1/2)+(next_connected-number_of_left_sections)];
   }
	else
	{
number_of_connections=
SectionNeighborsLeftRight[next_connected*(NUM_SECT1/2)+(count_section-number_of_left_sections)];
	}
length_ratio=(16*number_of_connections)/length_next;
length_ratio1=(16*number_of_connections)/length;
if(length_ratio>=8)
{
SkyGreenComponents[next_connected]=1;
}

}
}//loop_int
			}
		}

	}//loop_ext
}
//=========================================================
void

CImageProcess::FindingBoundaryChains(void)
{
	int last_column;
	int bunch_involvement_left;
	int bunch_involvement_right;
	int bunch_old_number;
	int left_bunch_end;
	int right_bunch_end;

	bunch_involvement_left=-1;
	bunch_involvement_right=-1;

for(int count_fiber=LowerSkyFiber;count_fiber<=UpperSkyFiber;count_fiber++)
	{//loop_ext
last_column=ColorInt[count_fiber].NumberOfIntervalsInCovering;
for(int count_column=0;count_column<last_column;count_column++)
{//loop_int
	bunch_old_number=ColorInt[count_fiber].old_ordered_bunch_number[count_column];
left_bunch_end=ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[bunch_old_number];
right_bunch_end=ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[bunch_old_number];
if(NumberLeftSkyComp>0)
	{

		bunch_involvement_left=*(LeftBunchesInTheSky+count_column+
			count_fiber*MaximumNumberOfCoveringElements);


	}
if(NumberRightSkyComp>0)
	{
		bunch_involvement_right=*(RightBunchesInTheSky+count_column+
		count_fiber*MaximumNumberOfCoveringElements);
	}
if(bunch_involvement_left>0)
{
PaintingBoundaryChains(left_bunch_end,right_bunch_end,count_fiber,count_column);
}
else
{
	if(bunch_involvement_right>0)
	{
PaintingBoundaryChains(left_bunch_end,right_bunch_end,count_fiber,count_column);
	}
}
}//loop_int
    }//loop_ext
}


//=========================================================
void

CImageProcess::PaintingBoundaryChains(int left_end,int right_end,int fiber_num,int bunch_num)
{
	int index_point;
	int point_value;

	point_value=fiber_num*StripWidth + StripWidth/2;
    for(int count_pixel=left_end;count_pixel<=right_end;count_pixel++)
	{//loop

    index_point=SkyPixelsOfBoundaryPoints[count_pixel];
	if(!index_point)
	{
    SkyPixelsOfBoundaryPoints[count_pixel]=point_value;
	SkyVisualization[count_pixel]=bunch_num+1;
	}
	else
	{
		if(point_value<index_point)
		{
         SkyPixelsOfBoundaryPoints[count_pixel]=point_value;
		 SkyVisualization[count_pixel]=bunch_num+1;
		}
	}
    }//loop
	/*beg=mult_coef*left_end;
	end=mult_coef*right_end;
for(int count_pixel_l=beg;count_pixel_l<=end;count_pixel_l++)
{//loop1
	index_point1=SkyBoundaries[count_pixel_l];
	SkyBoundaries
}//loop1*/
//}

}
//=========================================================
void

CImageProcess::TranslationIntoBigDimension(void)
{
	int vert_dim;
	int hor_dim;
	int strip_width;
	int mult_coef;
	int index_point1;
	int fiber_number;
	int new_value;
	int count_pix;
	int upper_bound;
//return;
hor_dim=1920;
vert_dim=1080;
if(!HorizontalVertical)
{
	strip_width=vert_dim/NumStrips;
	mult_coef=hor_dim/DimX;
}
else
{
	strip_width=hor_dim/NumStrips;
}
upper_bound=hor_dim-2;
for(int count_pixel=0;count_pixel<upper_bound;count_pixel++)
{//loop_1
	count_pix=count_pixel/3;
index_point1=SkyPixelsOfBoundaryPoints[count_pix];
if(!index_point1)
{
new_value=0;
}
else
{
fiber_number=index_point1/StripWidth;
new_value=(NumStrips-fiber_number-1)*strip_width + strip_width/2;
}
     for(int count_r=0;count_r<mult_coef;count_r++)
     {//loop_int
     SkyBoundaries[count_pixel+count_r]=new_value;
     }//loop_int
}//loop_1

}
//=========================================================
int

CImageProcess::TestSection(int sect_candidate_number,int left_right_sec,
int min_left,int min_right)
{
	int prior;
		int number_of_connected;
		int next_connected;
		int last_fiber;
		int first_fiber;
		int last_fiber_next;
		int first_fiber_next;
		int last_strip_ratio_next;
		int first_strip_ratio_next;
		int number_of_left_sections;
		int number_of_connections;
		int length;
		int length_next;
		int length_ratio;
		int length_ratio1;
		//int weight_of_section;
		//int mean_gray;
		int num_connect_max;
		int maximal_coinc;
		int candidate_weight;
		int specific_weight;
		int incline_cand;
		int incline_next;
		int incline_difference;
		int vert_cand;
		int last_fiber_ratio;
		int first_fiber_ratio;
		int vert_next;
		int beg_first;
		int end_first;
		int length_first;
		int beg_last;
		int end_last;
		int length_last;
		int first_bunch;
		int last_bunch;
		int first_last_ratio;
		int bunch_over_last;
		int over_mean_hue;
		int over_mean_gray;
		int over_mean_satur;
		int over_beg;
		int over_end;
		int over_hue_zone;
		int bunch_over_last1;
		int over_mean_hue1;
		int over_mean_gray1;
		int over_mean_satur1;
		int over_beg1;
		int over_end1;
		int over_hue_zone1;
		int bunch_over_last2;
		int over_mean_hue2;
		int over_mean_gray2;
		int over_mean_satur2;
		int over_beg2;
		int over_end2;
		int over_hue_zone2;
		int beg_un;
		int end_un;
		//int last_fiber_ratio1;

		beg_first=-1;
		end_first=-1;
		length_first=-1;
		beg_last=-1;
		end_last=-1;
		length_last=-1;
		first_bunch=-1;
		last_bunch=-1;
		prior=-1;
		num_connect_max=0;
		maximal_coinc=-1;
		vert_next=0;
	number_of_left_sections=ColorSection->Number_of_sections_left;
	candidate_weight=ColorSection->section_weight[sect_candidate_number];
		number_of_connected=NumberOfLRConnections[sect_candidate_number];
	        last_fiber=ColorSection->section_fibre_last[sect_candidate_number];
			last_bunch=ColorSection->DescrSec[sect_candidate_number].location_of_section[last_fiber];

			first_fiber=ColorSection->section_fibre_first[sect_candidate_number];
			first_bunch=ColorSection->DescrSec[sect_candidate_number].location_of_section[first_fiber];
			if(first_bunch>0)
			{
            first_bunch-=1;
			beg_first=ColorInt[first_fiber].ColoredIntervalsStructure->BegInterv[first_bunch];
			end_first=ColorInt[first_fiber].ColoredIntervalsStructure->EndInterv[first_bunch];
			length_first=end_first-beg_first+1;
			}
			last_bunch=ColorSection->DescrSec[sect_candidate_number].location_of_section[last_fiber];
			if(last_bunch>0)
			{
				last_bunch-=1;
				beg_last=ColorInt[last_fiber].ColoredIntervalsStructure->BegInterv[last_bunch];
				end_last=ColorInt[last_fiber].ColoredIntervalsStructure->EndInterv[last_bunch];
				length_last=end_last-beg_last+1;
			}
			if((length_last>0)&&(length_first>0))
			{//lllf
			 first_last_ratio=(16*length_last)/(length_first);
			 if(last_fiber<(NumStrips-2))
			 {//lf
				 bunch_over_last=ColorInt[last_fiber+2].painted_strip_colored[((beg_last)/4)];
				 if(bunch_over_last>0)
				 {//bov>
                 bunch_over_last-=1;
over_beg=ColorInt[last_fiber+2].ColoredIntervalsStructure->BegInterv[bunch_over_last];
over_end=ColorInt[last_fiber+2].ColoredIntervalsStructure->EndInterv[bunch_over_last];
over_mean_hue=ColorInt[last_fiber+2].ColoredIntervalsStructure->AverageHue[bunch_over_last];
over_mean_gray=ColorInt[last_fiber+2].ColoredIntervalsStructure->AverageGray[bunch_over_last];
over_mean_satur=ColorInt[last_fiber+2].ColoredIntervalsStructure->AverageSat[bunch_over_last];
over_hue_zone=hue_zones[over_mean_hue];
bunch_over_last1=ColorInt[last_fiber+2].painted_strip_colored[((end_last)/4)];
if(bunch_over_last1>0)
{//bov1
	bunch_over_last1-=1;
	over_beg1=ColorInt[last_fiber+2].ColoredIntervalsStructure->BegInterv[bunch_over_last1];
	over_end1=ColorInt[last_fiber+2].ColoredIntervalsStructure->EndInterv[bunch_over_last1];
	over_mean_hue1=ColorInt[last_fiber+2].ColoredIntervalsStructure->AverageHue[bunch_over_last1];
	over_mean_gray1=ColorInt[last_fiber+2].ColoredIntervalsStructure->AverageGray[bunch_over_last1];
	over_mean_satur1=ColorInt[last_fiber+2].ColoredIntervalsStructure->AverageSat[bunch_over_last1];
	over_hue_zone1=hue_zones[over_mean_hue1];
if((over_beg<=beg_last)&&((over_end>=end_last)))
{//ob
	if(over_hue_zone==3)
	{//oh
if(over_mean_satur>=2)
{//os
	if(over_mean_gray<=40)
	{//4
		prior=1;
	goto L;
	}

}//os

	}//oh
	else
	{//oh
		if(over_hue_zone==4)
		{
			if(over_mean_satur>=2)
			{
				if(over_mean_gray<=20)
				{
					prior=1;
					goto L;
				}
			}
		}
	}//oh

}//ob
else
{//ob
	beg_un=std::min(over_beg,over_beg1);
	end_un=std::max(over_end,over_end1);
	if((beg_un<=beg_last)&&(end_un>=(end_last-StripWidth)))
	{
		if((((over_hue_zone==3)&&(over_mean_gray<=40))||((over_hue_zone==4)&&(over_mean_gray<=20)))&&
			(((over_hue_zone1==3)&&(over_mean_gray1<=40))||((over_hue_zone1==4)&&(over_mean_gray1<=20))))
		{
			if((over_mean_satur>=2)&&(over_mean_satur1>=2))
			{
				prior=1;
				goto L;

			}

		}

	}
}//eob

}//bov1

				 }//bov

			 }//lf

			}//lllf
			else
			{
            first_last_ratio=16;
			}

			last_fiber_ratio=(16*last_fiber)/NumStrips;
			first_fiber_ratio=(16*first_fiber)/NumStrips;
			length=last_fiber-first_fiber+1;
			specific_weight=candidate_weight/length;
			incline_cand=InclinedComponent[sect_candidate_number];
			vert_cand=VerticalComponent[sect_candidate_number];
	      if(number_of_connected>=1)
		  {//nc
	for(int count_connected=0;count_connected<number_of_connected;count_connected++)
	{//loop_opp
	 next_connected=RightLeftConnections[count_connected*NUM_SECT1 + sect_candidate_number];
	        last_fiber_next=ColorSection->section_fibre_last[next_connected];
		    first_fiber_next=ColorSection->section_fibre_first[next_connected];
			last_strip_ratio_next=(16*last_fiber_next)/NumStrips;
			first_strip_ratio_next=(16*first_fiber_next)/NumStrips;
			length_next=last_fiber_next-first_fiber_next+1;
			if(!left_right_sec)
		{
	number_of_connections=
	SectionNeighborsLeftRight[sect_candidate_number*(NUM_SECT1/2)+
	(next_connected-number_of_left_sections)];
	   }
		else
		{
	number_of_connections=
	SectionNeighborsLeftRight[next_connected*(NUM_SECT1/2)+
	(sect_candidate_number-number_of_left_sections)];
		}
			if(number_of_connections>num_connect_max)
			{
				num_connect_max=number_of_connections;
				maximal_coinc=next_connected;
			}

	length_ratio=(16*number_of_connections)/length_next;
	length_ratio1=(16*number_of_connections)/length;
	incline_next=InclinedComponent[next_connected];
	vert_next=VerticalComponent[next_connected];//03.11.15
	incline_difference=abs(incline_cand-incline_next);
	      if(length_ratio1>=6)
	      {
	 if(first_strip_ratio_next<=5)
	 {
		 prior=1;
		 break;
	 }
	 if((vert_cand>0)&&(vert_next>0))
	 {//last_cor03.11.15
if(specific_weight<StripLength/6)
{
	prior=1;
	break;
}

	 }
	 else
	 {
		 if((vert_cand>0)||(vert_next>0))
		 {//last_cor03.11.15
			 if(specific_weight<StripLength/6)
			 {
				 prior=1;
				 break;
			 }

		 }

	 }
	 if((incline_next!=0)&&(incline_cand!=0))
	 {
		 if(incline_difference<=5)
		 {
			 if(specific_weight<StripLength/4)
			 {
				 prior=1;
				 break;
			 }
		 }

	 }
	 if((((incline_next<0)||(vert_next>0))&&(((incline_cand>0)||(vert_cand>0))))||(((incline_next>0)||(vert_next>0))&&((incline_cand<0)||(vert_cand))))
	 {
		 if(first_last_ratio<10)
		 {
			 if(specific_weight<StripLength/4)
			 {
				 prior=1;
				 break;
			 }
		 }
	 }
	 //else
	 //{
	//if((incline_next<0)&&(incline_cand<0))
	 //{
		 //if(incline_difference<=4)
		 //{
			 //if(specific_weight<StripLength/5)
			 //{
				 //prior=1;
				// break;
			 //}
		 //}
	 //}
	 //}
		  }
	 }//loop_opp
		  }//nc
		  /*if(prior<0)
		  {
			  last_fiber_ratio1=(16*(last_fiber+1))/NumStrips;
			  if(last_fiber_ratio1<16)
			  {
	mean_gray=ColorSection->section_mean_gray[sect_candidate_number];
	if(mean_gray>2*MassiveGrayTop)
	{
		prior=1;
	}
			  }
		  }*/
		  L:;
		  if(prior==1)
		  {
	*(SkyGreenComponents+sect_candidate_number)=-1;
		  }
	return(prior);
	}


//=========================================================
int

CImageProcess::GreenSeparation(int sect_max_number,int sect_sat_number,
int min_left,int max_right,int min_left_sat,int max_right_sat)
{
	int prior;
	int fiber_first;
	int fiber_last;
	int tot_min_left;
    int tot_max_right;
	int first_bunch;
	int ordered_first_bunch;
	int point_scale;
	int bunch_hue;
	int bunch_sat;
	int bunch_gray;
	int bunch_visibility;
	int first_bunch_beg;
	int first_bunch_end;
	int bunch_hue_zone;
	int bunch_gray_zone;
	int number_of_ordered_bunches;
	int bunch_new;
	int bunch_new_beg;
	int bunch_new_end;

	bunch_new_beg=-1;
	bunch_new_end=-1;
	prior=-1;
	tot_min_left=std::min(min_left,min_left_sat);
	tot_max_right=std::max(max_right,max_right_sat);
	fiber_first=ColorSection->section_fibre_first[sect_sat_number]-1;
	fiber_last=ColorSection->section_fibre_last[sect_max_number]+1;
for(int count_fiber=fiber_first;count_fiber>=fiber_last;count_fiber--)
{//loop_out
	number_of_ordered_bunches=ColorInt[count_fiber].NumberOfIntervalsInCovering;
point_scale=tot_min_left/4;
first_bunch=ColorInt[count_fiber].painted_strip_colored[point_scale];
if(first_bunch>0)
{//fb

first_bunch--;
bunch_visibility=ColorInt[count_fiber].visible_bunches[first_bunch];
if(bunch_visibility>0)
{//bv
bunch_hue=ColorInt[count_fiber].ColoredIntervalsStructure->AverageHue[first_bunch];
bunch_sat=ColorInt[count_fiber].ColoredIntervalsStructure->AverageSat[first_bunch];
bunch_gray=ColorInt[count_fiber].ColoredIntervalsStructure->AverageGray[first_bunch];
bunch_hue_zone=hue_zones[bunch_hue];
bunch_gray_zone=gray_zones[bunch_gray];
first_bunch_beg=ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[first_bunch];
first_bunch_end=ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[first_bunch];
if((bunch_hue_zone==3)||(bunch_hue_zone==4))
{//bhz
	if(bunch_gray_zone<=3)
	{//bgz
		if(bunch_sat>=2)
		{//bs
if(first_bunch_end>=tot_max_right)
{
	prior=1;
	return(prior);
}
else
{//e
	ordered_first_bunch=ColorInt[count_fiber].ordered_bunch_number[first_bunch];
for(int count_bunch=ordered_first_bunch+1;count_bunch<number_of_ordered_bunches;count_bunch++)
    {//loop_inner
bunch_new=ColorInt[count_fiber].old_ordered_bunch_number[count_bunch];
bunch_hue=ColorInt[count_fiber].ColoredIntervalsStructure->AverageHue[bunch_new];
bunch_sat=ColorInt[count_fiber].ColoredIntervalsStructure->AverageSat[bunch_new];
bunch_gray=ColorInt[count_fiber].ColoredIntervalsStructure->AverageGray[bunch_new];
bunch_hue_zone=hue_zones[bunch_hue];
bunch_gray_zone=gray_zones[bunch_gray];
bunch_new_beg=ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[bunch_new];
bunch_new_end=ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[bunch_new];
if((bunch_hue_zone==3)||(bunch_hue_zone==4))
{//bhzi
	if(bunch_gray_zone<=3)
	{
		if(bunch_sat>=2)
		{
			if(bunch_new_end>=tot_max_right)
			{
			prior=1;
	        return(prior);
			}
		}
		else
		{
			break;
		}
	}
	else
	{
		break;
	}
}
else
{
	break;
}
	}//loop_inner


}//e
		}//bs
	}//bgz
}//bhz


}//bv

}//fb
}//loop_out
	return(prior);
}
//=====================================================
int

CImageProcess::LocateOn(int sect_number,int* sect_under_number,int* sect_under_number_r)
{
	int sect_first_fiber;
		int prior;
		int first_bunch;
		int first_beg;
		int first_end;
		int previous_fiber;
		int point_scale;
		int oppos_bunch;
		int left_section;
		int right_section;
		int bunch_visibility;
		int oppos_beg;
		int oppos_end;
		int Intersection;
		int indic_length;
		int ratio_length;
		int ratio_length1;
		int center_coor;

		prior=-1;
		*sect_under_number=-1;
		*sect_under_number_r=-1;
		previous_fiber=-1;
	sect_first_fiber=ColorSection->section_fibre_first[sect_number];

	if(sect_first_fiber>0)
	{//sff
	first_bunch=
	ColorSection->DescrSec[sect_number].location_of_section[sect_first_fiber];
	     if(first_bunch>0)
		 {//fb>
	     first_bunch--;
	first_bunch=ColorInt[sect_first_fiber].old_bunch_number[first_bunch];
	first_beg=
	ColorInt[sect_first_fiber].ColoredIntervalsStructure->BegInterv[first_bunch];
	first_end=
	ColorInt[sect_first_fiber].ColoredIntervalsStructure->EndInterv[first_bunch];
	center_coor=(first_beg+first_end)/2;
	previous_fiber=sect_first_fiber-1;
	point_scale=center_coor/4;
	oppos_bunch=ColorInt[previous_fiber].painted_strip_colored[point_scale];
	   if(oppos_bunch>0)
	    {//ob
	   oppos_bunch--;

	bunch_visibility=ColorInt[previous_fiber].visible_bunches[oppos_bunch];
	if(bunch_visibility>0)
	{//bv
	oppos_beg=ColorInt[previous_fiber].ColoredIntervalsStructure->BegInterv[oppos_bunch];
	oppos_end=ColorInt[previous_fiber].ColoredIntervalsStructure->EndInterv[oppos_bunch];
	Intersection=
	ColorInt[previous_fiber].Intersection_measure_ratios(first_beg,first_end,
	oppos_beg,oppos_end,&indic_length,&ratio_length,&ratio_length1);
	if(Intersection<=1)
	{//int
		    if((ratio_length>=13)||((ratio_length>=10)&&(ratio_length1>=12)))
		    {//rl>last_cor02.11.15
	left_section=SectionTraceLeft[previous_fiber*MAX_COL_INT + oppos_bunch];
	left_section=left_section%1000;
	    if(left_section>0)
	    {
		left_section-=1;
	*sect_under_number=left_section;
		prior=1;
	    }
	right_section=SectionTraceRight[previous_fiber*MAX_COL_INT + oppos_bunch];
	right_section=right_section%1000;
	              if(right_section>0)
	               {//rs
		right_section-=1;
		*sect_under_number_r=right_section;
		if(prior==1)
		{
			prior=2;
		}
		else
		{
			prior=3;
		}
				  }//rs
			}//rl>
	}//int
	}//bv
	   }//ob
		 }//fb>
	}//sff

	return(prior);
	}
//=========================================================
int

	CImageProcess::LocateUnder(int sect_number,int* sect_over_number,int* sect_over_number_r,int opt_fiber)
{

int sect_last_fiber;
int prior;
int first_bunch;
int first_beg;
int first_end;
int previous_fiber;
int point_scale;
int oppos_bunch;
int left_section;
int right_section;
int bunch_visibility;
int oppos_beg;
int oppos_end;
int Intersection;
int indic_length;
int ratio_length;
int ratio_length1;
int center_coor;
int opt_fiber_differ;
/*int oppos_bunch1;
int oppos_bunch2;

oppos_bunch=-1;
oppos_bunch1=-1;
oppos_bunch2=-1;*/
prior=-1;
*sect_over_number=-1;
*sect_over_number_r=-1;
previous_fiber=-1;
sect_last_fiber=ColorSection->section_fibre_last[sect_number];
opt_fiber_differ=opt_fiber-sect_last_fiber;
if(sect_last_fiber<(NumStrips-2))
{//sff
	first_bunch=
		ColorSection->DescrSec[sect_number].location_of_section[sect_last_fiber];
	if(first_bunch>0)
	{//fb>
		first_bunch--;
		first_bunch=ColorInt[sect_last_fiber].old_bunch_number[first_bunch];
		first_beg=
			ColorInt[sect_last_fiber].ColoredIntervalsStructure->BegInterv[first_bunch];
		first_end=
			ColorInt[sect_last_fiber].ColoredIntervalsStructure->EndInterv[first_bunch];
		center_coor=(first_beg+first_end)/2;
		if((opt_fiber_differ>=1)&&(opt_fiber_differ<=2))
		{
        previous_fiber=opt_fiber;
		}
		else
		{
		previous_fiber=sect_last_fiber+1;
		}
		point_scale=center_coor/4;
		oppos_bunch=ColorInt[previous_fiber].painted_strip_colored[point_scale];
		/*point_scale=first_beg/4;
		oppos_bunch1=ColorInt[previous_fiber].painted_strip_colored[point_scale];
		point_scale=first_end/4;
		oppos_bunch2=ColorInt[previous_fiber].painted_strip_colored[point_scale];*/
		if(oppos_bunch>0)
		{//ob
			oppos_bunch--;

			bunch_visibility=ColorInt[previous_fiber].visible_bunches[oppos_bunch];
			if(bunch_visibility>0)
			{//bv
				oppos_beg=ColorInt[previous_fiber].ColoredIntervalsStructure->BegInterv[oppos_bunch];
				oppos_end=ColorInt[previous_fiber].ColoredIntervalsStructure->EndInterv[oppos_bunch];
				Intersection=
					ColorInt[previous_fiber].Intersection_measure_ratios(first_beg,first_end,
					oppos_beg,oppos_end,&indic_length,&ratio_length,&ratio_length1);
				if(Intersection<=1)
				{//int
					if((ratio_length>=13)||((ratio_length>=10)&&(ratio_length1>=12))||(ratio_length1>=15))
					{//rl>last_cor02.11.15
						left_section=SectionTraceLeft[previous_fiber*MAX_COL_INT + oppos_bunch];
						left_section=left_section%1000;
						if(left_section>0)
						{
							left_section-=1;
							*sect_over_number=left_section;
							prior=1;
						}
						right_section=SectionTraceRight[previous_fiber*MAX_COL_INT + oppos_bunch];
						right_section=right_section%1000;
						if(right_section>0)
						{//rs
							right_section-=1;
							*sect_over_number_r=right_section;
							if(prior==1)
							{
								prior=2;
							}
							else
							{
								prior=3;
							}
						}//rs
					}//rl>
				}//int
			}//bv
		}//ob
	}//fb>
}//sff

return(prior);
	}
//=========================================================
TIntCharact::TIntCharact(void) {
	num_of_int = 0;
	MaxSize = MAX_INT_NUMBER;
	BegInt = new int[MaxSize];
	EndInt = new int[MaxSize];
	Signif = new int[MaxSize];
	ColorPeak = new int[MaxSize];
	PeakFraction = new int[MaxSize];
	ColorPeak1 = new int[MaxSize];
	PeakFraction1 = new int[MaxSize];
	FirstOutlet = new int[MaxSize];
	SecondOutlet = new int[MaxSize];
	FirstOutlet1 = new int[MaxSize];
	SecondOutlet1 = new int[MaxSize];
	MainOpponentRatiosBalance = new int[MaxSize];
	ZeroRatio = new int[MaxSize];
	PositiveFraction = new int[MaxSize];
	ZeroRatio1 = new int[MaxSize];
	NegativeFraction = new int[MaxSize];
	PositiveFraction2 = new int[MaxSize];
	NeighborPosFraction = new int[MaxSize];
	NeighborNegFraction = new int[MaxSize];
	CenterOfMass = new int[MaxSize];
	CenterOfMass1 = new int[MaxSize];
	LowerGrayScale = new int[MaxSize];
	UpperGrayScale = new int[MaxSize];
	GrayScaleMean = new int[MaxSize];
	LowerMRatio = new int[MaxSize];
	UpperMRatio = new int[MaxSize];
	MeanMRatio = new int[MaxSize];
}
//=====================================================
TIntCharact::~TIntCharact(void) {
	delete[] BegInt;
	delete[] EndInt;
	delete[] Signif;
	delete[] ColorPeak;
	delete[] PeakFraction;
	delete[] FirstOutlet;
	delete[] SecondOutlet;
	delete[] ColorPeak1;
	delete[] PeakFraction1;
	delete[] FirstOutlet1;
	delete[] SecondOutlet1;
	delete[] MainOpponentRatiosBalance;
	delete[] ZeroRatio;
	delete[] PositiveFraction;
	delete[] ZeroRatio1;
	delete[] NegativeFraction;
	delete[] PositiveFraction2;
	delete[] NeighborPosFraction;
	delete[] NeighborNegFraction;
	delete[] CenterOfMass;
	delete[] CenterOfMass1;
	delete[] LowerGrayScale;
	delete[] UpperGrayScale;
	delete[] GrayScaleMean;
	delete[] LowerMRatio;
	delete[] UpperMRatio;
	delete[] MeanMRatio;
}
//=====================================================
TIntCharactGray::TIntCharactGray(void) {
	num_of_int = 0;
	MaxSize = MAX_INT_NUMBER;
	BegInt = new int[MaxSize];
	EndInt = new int[MaxSize];
	Signif = new int[MaxSize];
}
//=====================================================
TIntCharactGray::~TIntCharactGray(void) {
	delete[] BegInt;
	delete[] EndInt;
	delete[] Signif;
}
//=====================================================
TIntColored::TIntColored(void) {
	BegInterv = new int[MAX_COL_INT];
	EndInterv = new int[MAX_COL_INT];
	GrainIntensity = new int[MAX_COL_INT];
	LowerIntensity = new int[MAX_COL_INT];
	UpperIntensity = new int[MAX_COL_INT];
	LowerGrayscale = new int[MAX_COL_INT];
	UpperGrayscale = new int[MAX_COL_INT];
	AverageGray = new int[MAX_COL_INT];
	LowerHueValue = new int[MAX_COL_INT];
	UpperHueValue = new int[MAX_COL_INT];
	AverageHue = new int[MAX_COL_INT];
	LowerSatValue = new int[MAX_COL_INT];
	UpperSatValue = new int[MAX_COL_INT];
	AverageSat = new int[MAX_COL_INT];
	Significance = new int[MAX_COL_INT];
	Cardinality = new int[MAX_COL_INT];
}
//=====================================================
TIntColored::~TIntColored(void) {
	delete[] BegInterv;
	delete[] EndInterv;
	delete[] GrainIntensity;
	delete[] LowerIntensity;
	delete[] UpperIntensity;
	delete[] LowerGrayscale;
	delete[] UpperGrayscale;
	delete[] AverageGray;
	delete[] LowerHueValue;
	delete[] UpperHueValue;
	delete[] AverageHue;
	delete[] LowerSatValue;
	delete[] UpperSatValue;
	delete[] AverageSat;
	delete[] Significance;
	delete[] Cardinality;
}
//=====================================================
TIntColorLessBack::TIntColorLessBack(void) {
	BegInterv = new int[MAX_COLLESS_INT];
	EndInterv = new int[MAX_COLLESS_INT];
	GrainIntensity = new int[MAX_COLLESS_INT];
	LowerIntensity = new int[MAX_COLLESS_INT];
	UpperIntensity = new int[MAX_COLLESS_INT];
	LowerGrayscale = new int[MAX_COLLESS_INT];
	UpperGrayscale = new int[MAX_COLLESS_INT];
	AverageGray = new int[MAX_COLLESS_INT];
	AverageHue = new int[MAX_COLLESS_INT];
	AverageSat = new int[MAX_COLLESS_INT];
	Significance = new int[MAX_COLLESS_INT];
	Cardinality = new int[MAX_COLLESS_INT];
}
//=====================================================
TIntColorLessBack::~TIntColorLessBack(void) {
	delete[] BegInterv;
	delete[] EndInterv;
	delete[] GrainIntensity;
	delete[] LowerIntensity;
	delete[] UpperIntensity;
	delete[] LowerGrayscale;
	delete[] UpperGrayscale;
	delete[] AverageGray;
	delete[] AverageHue;
	delete[] AverageSat;
	delete[] Significance;
	delete[] Cardinality;
}
//=====================================================
//=====================================================
TIntColoredCharacteristics::TIntColoredCharacteristics(void) {
	bunch_blocking = new int[MAX_COL_INT];
	left_continuation = new int[MAX_COL_INT];
	right_continuation = new int[MAX_COL_INT];
	left_adjacent = new int[MAX_COL_INT];
	right_adjacent = new int[MAX_COL_INT];
	new_left_continuation=new int[MAX_COL_INT];
	new_right_continuation=new int[MAX_COL_INT];
	new_left_adjacent=new int[MAX_COL_INT];
	new_right_adjacent=new int[MAX_COL_INT];
	ordered_bunch_number=new int[MAX_COL_INT];
	old_ordered_bunch_number=new int[MAX_COL_INT];
	//label_segments = new int[MAX_COL_INT];
	//center_bunch_suitability = new int[MAX_COL_INT];
	new_bunch_number = new int[MAX_COL_INT];
	old_bunch_number = new int[MAX_COL_INT];
	visible_bunches = new int[MAX_COL_INT];
	length_of_trajectory = new int[MAX_COL_INT];
	shift_of_the_position = new int[MAX_COL_INT];
	total_shift = new int[MAX_COL_INT];
	previous_bunch_number = new int[MAX_COL_INT];
	right_length_of_trajectory = new int[MAX_COL_INT];
	right_shift_of_the_position = new int[MAX_COL_INT];
	right_total_shift = new int[MAX_COL_INT];
	right_previous_bunch_number = new int[MAX_COL_INT];
}
//=====================================================
TIntColoredCharacteristics::~TIntColoredCharacteristics(void) {
	delete[] bunch_blocking;
	delete[] left_continuation;
	delete[] right_continuation;
	delete[] left_adjacent;
	delete[] right_adjacent;
	delete[] new_left_continuation;
	delete[] new_right_continuation;
	delete[] new_left_adjacent;
	delete[] new_right_adjacent;
	delete[] ordered_bunch_number;
	delete[] old_ordered_bunch_number;
	delete[] new_bunch_number;
	delete[] old_bunch_number;
	delete[] visible_bunches;
	delete[] length_of_trajectory;
	delete[] shift_of_the_position;
	delete[] total_shift;
	delete[] right_length_of_trajectory;
	delete[] right_shift_of_the_position;
	delete[] right_total_shift;
	delete[] previous_bunch_number;
	delete[] right_previous_bunch_number;
//delete[] bunch_connections;
}
//=====================================================
TMotionShifts::TMotionShifts(void) {
	SeveralIntervalsMovements = new int[4 * MAX_COL_INT];
}
//=====================================================
TMotionShifts::~TMotionShifts(void) {
	delete[] SeveralIntervalsMovements;
}
//=====================================================
CMotion::CMotion(void) {
	NumberofFrames = 4;
}
//=====================================================
CMotion::~CMotion(void) {
	delete[] SeveralIntervalsMotion;
}



/*
 * @Description:
 *      Draws all found road marking lines.
 * @Parameters:
 *      @In:    image -- image, where road marking will be drawn,
 *              markings -- list, where from markings are extracted and drawn.
 */
void CImageProcess::draw_markings(cv::Mat& image, std::vector<Marking>& markings)
{
    cv::Scalar color(250, 180, 30);     // color of line
    int thickness = 2;  // thickness of line

    for (auto& marking: markings)
    {
        for (auto& bunch: marking.bunches)
        {
            float y = (STRIPSNUMBER - bunch.stripNumber - 0.5) * StripWidth;

            cv::Point begin(bunch.beg, y);
            cv::Point end(bunch.end, y);

            cv::line(image, begin, end, color, thickness);
        }
    }
}