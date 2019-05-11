/*
 * Finds large bunches based only on intensity. It is not supposed to be used for road marking
 * finding because it finds only large objects (e.g. road).
 *
 *
 *
 *
 *
*/




#include "BunchGray.h"



#define YES 1
#define NO 0



#define FREE 0
#define OCCUP 1



GrayBunch::GrayBunch(std::int16_t begin,
                     std::int16_t ending,
                     std::uint8_t stripNum,
			         float meanIntensity)
{
 	beg = begin;
	end = ending;
	stripNumber = stripNum;
	intens = meanIntensity;
	sectionCrossed = false;
}



GrayBunch::~GrayBunch(){}



CBunchGray::CBunchGray()
{
	GapWidth = 2;
	
	BackgroundClusters = NULL;
	IntervalIntensity = NULL;
	IntervalOldNumber = NULL;
	BackgroundGrains = NULL;

	TotalNumInt = 0;
	MaxNumInt = 0;
}



CBunchGray::~CBunchGray()
{ 
	if (BackgroundClusters != NULL)
	{
		delete[] BackgroundClusters;
	}

	if (IntervalIntensity != NULL)
	{
		delete[] IntervalIntensity;
	}

	if (IntervalOldNumber != NULL)
	{
		delete[] IntervalOldNumber;
	}

	if (BackgroundGrains != NULL)
	{
		delete[] BackgroundGrains;
	}

	if (NumberOfBackgroundGrains != 0)
	{
		delete[] ClusterGrains;

		delete[] BegIntervOfClusters;
		delete[] EndIntervOfClusters;

		delete[] GrainIntensityIntervalOfClusters;
		
		delete[] LowerIntensityOfClusters;
		delete[] UpperIntensityOfClusters;
		
		delete[] AverageIntensityOfClusters;
		delete[] SignificanceOfClusters;
		delete[] CardinalityOfClusters;
	}
}



// Finds total number of intervals and MaxNumInt for current strip
bool CBunchGray::IntervalInteraction()
{   
	// todo: make total number of intervals member of class Strip
	for (std::uint8_t i = 0; i < NUM_INTEN1; i++)
	{
        std::uint8_t intervalsNumber = StripCur->num_of_intg[i];
 
		TotalNumInt += intervalsNumber;

		if (intervalsNumber > MaxNumInt)
		{
			MaxNumInt = intervalsNumber;
		}
	}

	if (MaxNumInt != 0)
	{
		NumberOfBackgroundGrains = 0;

		int* valuable_intervalb  = new int[PressedLength]();
		int* signif_of_intervalb = new int[PressedLength];

		int* LeftLowerInterval = new int[TotalNumInt];
		int* LeftUpperInterval = new int[TotalNumInt];
		
		int* RightLowerInterval = new int[TotalNumInt];
		int* RightUpperInterval = new int[TotalNumInt];
		
		int* ClusterNumber      = new int[TotalNumInt]();
 		int* IntervalProperties = new int[TotalNumInt];  // intervals proximity: connected or isolated
		int* IntervalWeight     = new int[TotalNumInt]();
		int* IntervalVisibility = new int[TotalNumInt]();
		
		int* AdjacentLinked = new int[TotalNumInt]();

		// shows how many points in the interval
		int* LocalSignifSum = new int[TotalNumInt]();

		int* LocalOptimalBackground = new int[TotalNumInt]();
		
		BackgroundClusters   = new int[TotalNumInt]();
		BackgroundGrains     = new int[TotalNumInt];
		BackgrGrainOldNumber = new int[TotalNumInt];
		
		IntervalIntensity = new int[TotalNumInt];
		IntervalOldNumber = new int[TotalNumInt]; 

		int* IntervalNewNumber = new int[NUM_INTEN1 * MaxNumInt];
		
 		/* Makes new table numeration: IntervalNewNumber, IntervalIntensity, IntervalOldNumber */
		// todo: call one time for optimizations purpose
		IntervalNumeration(IntervalNewNumber, IntervalIntensity, IntervalOldNumber);
		
		//Sets weights(IntervWeight) for most visible intervals according to valuable_intensitiesg
		Room_testing(IntervalWeight, IntervalNewNumber);
		
		// Finds limits of intersecting intervals forward
		LimitIntervCalcFor(RightLowerInterval, RightUpperInterval, IntervalNewNumber);
		
		// Finds limits of intersecting intervals backward
		LimitIntervCalcBack(LeftLowerInterval, LeftUpperInterval, IntervalNewNumber);
        
		// Fills IntervalProperties with degree of proximity -2, -1, 0, 1, 2
 		ConnectedOrIsolated(IntervalProperties, IntervalNewNumber);

		/* Fills array ClusterNumber with interval numbers*/
		// todo: next function is not completed! IntervalProperties not initialized! 
		IsolatedClustersLeft(ClusterNumber, IntervalProperties, IntervalNewNumber,    
							 RightLowerInterval, RightUpperInterval, 
							IntervalWeight, IntervalIntensity, IntervalOldNumber);

		LocalBackgroundFinding(IntervalWeight, LeftLowerInterval, LeftUpperInterval,
						       RightLowerInterval, RightUpperInterval,IntervalIntensity,IntervalOldNumber,
							   IntervalProperties, IntervalNewNumber,LocalSignifSum, 0);
        
		LocalBackgroundFinding(IntervalWeight, LeftLowerInterval, LeftUpperInterval,
								RightLowerInterval,RightUpperInterval, IntervalIntensity, IntervalOldNumber,
								IntervalProperties, IntervalNewNumber, LocalSignifSum, 1);

		LinkAdjacent(AdjacentLinked, IntervalNewNumber, LeftLowerInterval,LeftUpperInterval,
						RightLowerInterval, RightUpperInterval, IntervalWeight, IntervalIntensity, IntervalOldNumber,
						IntervalProperties, LocalSignifSum);

		FindingLocalBackgroundGrains(AdjacentLinked, 
									LeftLowerInterval, LeftUpperInterval,
									RightLowerInterval, RightUpperInterval,
									IntervalIntensity, IntervalOldNumber, IntervalNewNumber, 
									LocalSignifSum,
									LocalOptimalBackground,
									valuable_intervalb,
									signif_of_intervalb);

		ArrangeIntervals(valuable_intervalb, 
						IntervalVisibility, 
					    LocalSignifSum,
						LocalOptimalBackground, 
						IntervalIntensity,IntervalOldNumber, LeftLowerInterval,
						LeftUpperInterval, RightLowerInterval, RightUpperInterval, 
						AdjacentLinked, IntervalNewNumber);


		delete[] LeftLowerInterval;
		delete[] LeftUpperInterval;
		delete[] RightLowerInterval;
		delete[] RightUpperInterval;
		delete[] ClusterNumber;
		delete[] IntervalWeight;
 
		delete[] IntervalNewNumber;
		delete[] IntervalProperties;
		delete[] AdjacentLinked;
		delete[] LocalSignifSum;
		delete[] valuable_intervalb;
		delete[] signif_of_intervalb;
		delete[] IntervalVisibility;
		delete[] LocalOptimalBackground;
	}
	else
	{
		return false;
	}
 
	delete[] BackgrGrainOldNumber;

	return true;
}



//**************************************************************************************
// For all intensities i and for all intervals corresponding this intensity finds boundary intervals corresponding
// neighbour intensity i+1 intersecting this interval. Number of left interval is written to lower_int, number of right
// interval is written to upper_int. If intervals not intersecting, -1 is written to both arrays.
//**************************************************************************************
void CBunchGray::LimitIntervCalcFor(int* lower_int, int* upper_int, int* int_new_num)
{ 
	int num_int1 = StripCur->num_of_intg[NUM_INTEN1-1];
	
	if (num_int1 > 0)
    { 
		NegativeInitialization(lower_int, upper_int,
			                  NUM_INTEN1-1, num_int1, 0, int_new_num);
    }

	for (size_t inu = 0; inu < (NUM_INTEN1 - 1); inu++)
	{
		num_int1 = StripCur->num_of_intg[inu];
	    if (num_int1 != 0)
		{ 
      		int num_int2 = StripCur->num_of_intg[inu + 1];
		    if (num_int2 != 0)
			{
				FindBoundaryIntervals(lower_int, upper_int, inu, inu + 1,
									   num_int1, num_int2, int_new_num); 
			}
			else
			{
				NegativeInitialization(lower_int, upper_int, inu, num_int1, 0, int_new_num);
			} 
		}
	} 
}



//**********************************************************************
// Finds boundary points (left and right) connecting intersecting intervals corresponding to two intesity values: 
// n_fibre_1 and n_fibre_2
//**************************************************************************
void CBunchGray::FindBoundaryIntervals(int* lower_int, int* upper_int, 
									   int n_fibre1, int n_fibre2,
                                       int n_int1, int n_int2,
									   int* int_new_num)
{ 
 	int int_start = 0;

	//!eliminate singular cases to reduce the computations
	for (size_t int_f = 0; int_f < n_int1; int_f++)
	{
		int new_number = *(int_new_num + n_fibre1 * MaxNumInt + int_f);

		int num_prev = -1;
		int bot_int_num = -1;
		int top_int_num = -1;

		int beg1 = StripCur->IntAllInformGray[n_fibre1].BegInt[int_f];
		int end1 = StripCur->IntAllInformGray[n_fibre1].EndInt[int_f];

		int beg2 = StripCur->IntAllInformGray[n_fibre2].BegInt[n_int2 - 1];
		int end2 = StripCur->IntAllInformGray[n_fibre2].EndInt[n_int2 - 1];
		
		if (end2 < beg1)
		{
			NegativeInitialization(lower_int, upper_int, n_fibre1, n_int1, int_f, int_new_num);
			return; 
		}

		beg2 = StripCur->IntAllInformGray[n_fibre2].BegInt[0];
		end2 = StripCur->IntAllInformGray[n_fibre2].EndInt[0];
		if (beg2 > end1)
		{
 			*(lower_int + new_number) = bot_int_num;
			*(upper_int + new_number) = top_int_num;
			continue;
		} 

		for (int int_s = int_start; int_s < n_int2; int_s++)
		{
			beg2 = StripCur->IntAllInformGray[n_fibre2].BegInt[int_s];
			end2 = StripCur->IntAllInformGray[n_fibre2].EndInt[int_s];
			if (beg2 > end1) // not intersected
			{
				if (num_prev >= 0)
				{
					top_int_num = num_prev;
				} 
				break;
			}
		
			if (((beg2 >= beg1) && (beg2 <= end1)) || ((beg1 >= beg2) && (beg1 <= end2)))
			{
				if (num_prev < 0)
				{
					bot_int_num = int_s; 
				}
				top_int_num = int_s;
				num_prev = int_s;
			}
		}
		*(lower_int + new_number) = bot_int_num;
		*(upper_int + new_number) = top_int_num;

		if (top_int_num > 0)
		{
			int_start = top_int_num;
		} 
    } 
}
 


//**************************************************************************************
// Fills lower_int and upper_int with boundary points of intersecting intervals corresponding 
// to intensities i and i-1 (backward)
//**************************************************************************************
void CBunchGray::LimitIntervCalcBack(int* lower_int, int* upper_int, int* int_new_num)
{  
	int num_int1 = StripCur->num_of_intg[0];
	
	if (num_int1 > 0)
	{
		NegativeInitialization(lower_int, upper_int, 0, num_int1, 0, int_new_num);
	}
	
	for (int inu = NUM_INTEN1-1; inu > 0; inu--)
	{
		num_int1 = StripCur->num_of_intg[inu];
	    if (num_int1 != 0)
		{ 
			int num_int2 = StripCur->num_of_intg[inu-1];
		    if (num_int2 != 0)
			{
				FindBoundaryIntervals(lower_int, upper_int, inu, inu-1, num_int1, num_int2, int_new_num); 
			}
			else
			{
				NegativeInitialization(lower_int, upper_int, inu, num_int1, 0, int_new_num);
			} 
		}
	} 
}



//*********************************************************************************************************
// Sets lower_int and upper_int with -1 for intervals starting from n_start and for current intensity n_fibre  
//*********************************************************************************************************
void CBunchGray::NegativeInitialization(int* lower_int, int* upper_int,
										int intensity, int num_of_intervals,
									    int n_start, int* int_new_num)
{ 
	for (int k = n_start; k < num_of_intervals; k++)
	{
		int new_num = *(int_new_num + intensity * MaxNumInt + k);
		
		*(lower_int + new_num) = -1;
		*(upper_int + new_num) = -1;
	} 
}




// Sets weights (IntervWeight) for most visible intervals 
void CBunchGray::Room_testing(int* IntervWeight, int* intervals_table_numeration)
{  
	for (std::uint8_t space_go = 0; space_go < PressedLength; space_go++)
	{
        std::uint8_t corn_presense = StripCur->valuable_intensityg[space_go];
		if (corn_presense != 0)
		{
			int interval_num = StripCur->valuable_intervalg[space_go]; // most visible interval 
			int new_interval_num = intervals_table_numeration[(corn_presense - 1) * MaxNumInt + interval_num];
			IntervWeight[new_interval_num]++;
		} 
	} 
}



/*
 * @Description:
 *      Fills arrays.
 * @Parameters:
 *      IntervalNewNumber -- two dimensional matrix numeration of intervals. E.g.
 *          0 1 2 * - (intensity 0: 3 intervals)
 *          3 4 5 6 - (intensity 1 has max intervals -- 4)
 *          7 8 * * - (intensity 2: 2 intervals)
 *      IntervalIntensity --
 *      IntervalOldNumber -- intervals numbers, if we numerate them starting from intensity = 0. E.g.
 */
void CBunchGray::IntervalNumeration(int* table_numeration, 
									int* IntervalIntensity, 
									int* linear_numeration)
{ 
	int linear_index = 0;

	for (std::uint8_t intensity = 0; intensity < NUM_INTEN1; intensity++)
	{
		int num_of_intervals = StripCur->num_of_intg[intensity];
		if (num_of_intervals != 0)
		{
			for (std::uint8_t interval_number = 0; interval_number < num_of_intervals; interval_number++)
			{ 
				*(table_numeration + MaxNumInt * intensity + interval_number) = linear_index;
				*(IntervalIntensity + linear_index) = intensity;
				*(linear_numeration + linear_index) = interval_number;
				
				linear_index++;
			}
		}
	}
}



//***********************************************************************
// Fills con_isol array with degree of proximity -2, -1, 0, 1, 2 
//***********************************************************************
void CBunchGray::ConnectedOrIsolated(int* con_isol, int* int_new_num)
{  
 	int NewIntNum;
 
	for (int inu = 0; inu < NUM_INTEN1; inu++)
	{
		int NumInt = StripCur->num_of_intg[inu];
		if (NumInt != 0)
		{
			if (NumInt == 1)
			{
			    NewIntNum = *(int_new_num + MaxNumInt * inu);
                *(con_isol + NewIntNum) = 2;
			    //must be refined
			    continue;
			}
			else
			{
				NewIntNum = *(int_new_num + MaxNumInt * inu + NumInt - 1);
				*(con_isol + NewIntNum) = 2;
			}
	   
			for (int inu_int = 0; inu_int < NumInt-1; inu_int++)
			{
				int beg1 = StripCur->IntAllInformGray[inu].BegInt[inu_int];
				int end1 = StripCur->IntAllInformGray[inu].EndInt[inu_int];
				int signif1 = StripCur->IntAllInformGray[inu].Signif[inu_int];
		
				int beg2 = StripCur->IntAllInformGray[inu].BegInt[inu_int + 1];
				int end2 = StripCur->IntAllInformGray[inu].EndInt[inu_int + 1];
				int signif2 = StripCur->IntAllInformGray[inu].Signif[inu_int + 1];
		
				int gap_length = beg2 - end1 + 1;

				int length1 = end1 - beg1 + 1;
				int length2 = end2 - beg2 + 1;


				int min_length = std::min(length1, length2);
				int max_length = std::max(length1, length2);

				signif1 = (4 * signif1) / length1;
				signif2 = (4 * signif2) / length2;

				int signif_ratio = (16 * signif2) / (signif2 + signif1);

				NewIntNum = *(int_new_num + MaxNumInt*inu + inu_int);
		
				if (max_length > StripWidthPrev)
				{
					if ((min_length + gap_length) > max_length)
					{
						if (gap_length > min_length)
						{
							*(con_isol + NewIntNum) = 1;
						}
						else
						{
							if ((((3*gap_length) < min_length) && (4 * gap_length < max_length)) ||
								((gap_length < StripWidthPrev) && (gap_length < min_length) && (4*gap_length < max_length)))
							{
								if (std::abs(signif_ratio - 8) <= 1)
								{
									*(con_isol + NewIntNum) = 0;
								}
							}
							else
							{
								*(con_isol + NewIntNum) = -1;
							}
						}
					}
					else
					{ 
						*(con_isol + NewIntNum) = ((4 * (gap_length + min_length) > max_length) && (gap_length > 2 * min_length)) ? 1 : -1;
 
						if (((3*gap_length < min_length) && (4 * gap_length < max_length))||
							((gap_length<StripWidthPrev) && (gap_length < min_length) && ((4 * gap_length) < max_length)))
						{
							if (std::abs(signif_ratio - 8) <= 1)
							{
								*(con_isol + NewIntNum) = 0;
							}
						}
					}
				}
				else
				{  
					*(con_isol + NewIntNum) = (gap_length > (max_length + min_length)) ? 1 : -2; 
				}  
			} 
		} 
	}
}
 


/* @Description:
 *
 *
 * */
void CBunchGray::LinkAdjacent(int* adjac_link,
                              int* int_new_num,
                              int* left_lower, int* left_upper,
							  int* right_lower, int* right_upper,
							  int* inter_weight, int* inter_intens, int* inter_num,
							  int* con_isol, int* extended_signif)
{
	int test_inten2, test_inten3, test_inten4;
	int beg2, end2, beg3,end3;
 	int new_num,new_num1;
 	int dense1;
 	int ext_dense1;
	int ext_signif2;
	int ext_dense2;
	int length1;
	int signif2;
	int signif3;
	int dense2,dense3,dense4;
	int length2,length3;
	int low_left1,up_left1;
	int low_right1,up_right1;
	int low_left2,up_left2;
	int low_right2,up_right2;
 	int number_of_successful_steps;
	int comp_density1,comp_density2,comp_density3,comp_density4;
	int index_of_step;
	int previous_intensity;
	int next_intensity;

	for (std::uint8_t inter_go = 0; inter_go < TotalNumInt; inter_go++)
	{
		int test_inten1 = *(inter_weight+inter_go);
		int was_linked1 = *(adjac_link+inter_go);
		
		if ((test_inten1 > 0) && (!was_linked1))
		{
			int total_signif = 0;
			int interval_intensity = *(inter_intens + inter_go);
			int inter_number = *(inter_num + inter_go);
			int number_of_int = StripCur->num_of_intg[interval_intensity];
			int num_step = number_of_int - 1 - inter_number;
			
			if (num_step > 0)
			{
				int beg1 = StripCur->IntAllInformGray[interval_intensity].BegInt[inter_number];
				int end1 = StripCur->IntAllInformGray[interval_intensity].EndInt[inter_number];
				int signif1 = StripCur->IntAllInformGray[interval_intensity].Signif[inter_number];
				
				int ext_signif1 = *(extended_signif + inter_go);
				length1 = end1 - beg1 + 1;
				dense1 = (4*signif1) / length1;
				ext_dense1 = (4*ext_signif1)/length1;

				total_signif += ext_signif1;
				number_of_successful_steps = 0;
				index_of_step = inter_go;

				for (size_t inter_step = 1; inter_step <= num_step; inter_step++)
				{
					new_num = *(int_new_num+MaxNumInt*interval_intensity+inter_number+inter_step);
					test_inten2 = *(inter_weight+new_num);
 
					if ((test_inten2 > 0) || (test_inten2 < 0))
					{
						signif2 = StripCur->IntAllInformGray[interval_intensity].Signif[inter_number+inter_step];
						beg2 = StripCur->IntAllInformGray[interval_intensity].BegInt[inter_number+inter_step];
						end2 = StripCur->IntAllInformGray[interval_intensity].EndInt[inter_number+inter_step];

						length2 = end2-beg2+1;
						dense2 = (4*signif2) / length2;
						ext_signif2 = *(extended_signif+new_num);
						ext_dense2=(4*ext_signif2)/length2;
						comp_density1=(16*dense2)/(dense1+dense2);
						comp_density2=(16*ext_dense2)/(ext_dense1+ext_dense2);
						if(((comp_density2 >= 6) && (comp_density2 <= 10))||
							(comp_density1 >= 6) && (comp_density1 <= 10))
						{
							low_left1=*(left_lower+index_of_step);
							up_left1=*(left_upper+index_of_step);
							low_right1=*(right_lower+index_of_step);
							up_right1=*(right_upper+index_of_step);
							low_left2=*(left_lower+new_num);
							up_left2=*(left_upper+new_num);
							low_right2=*(right_lower+new_num);
							up_right2=*(right_upper+new_num);
							if((up_left1==low_left2)&&(up_left1>=0))
							{
								previous_intensity=interval_intensity-1;
								new_num1=*(int_new_num+MaxNumInt*previous_intensity+up_left1);
								test_inten3=*(inter_weight+new_num1);
								beg3=StripCur->IntAllInformGray[previous_intensity].BegInt[up_left1];
								end3=StripCur->IntAllInformGray[previous_intensity].EndInt[up_left1];
								signif3=StripCur->IntAllInformGray[previous_intensity].Signif[up_left1];
								length3=end3-beg3+1;
								dense3=(4*signif3)/length3;
							}
							else
							{
								previous_intensity=-1;
								new_num1=-1;
								dense3=0;
							}
							
							if ((up_right1 == low_right2) && (up_right1>=0))
				            {
								next_intensity=interval_intensity+1;
								new_num1=*(int_new_num+MaxNumInt*next_intensity+up_right1);
								test_inten4=*(inter_weight+new_num1);
								beg3=StripCur->IntAllInformGray[next_intensity].BegInt[up_right1];
								end3=StripCur->IntAllInformGray[next_intensity].EndInt[up_right1];
								signif3=StripCur->IntAllInformGray[next_intensity].Signif[up_right1];
								length3 = end3-beg3+1;
								dense4 = (4*signif3)/length3; 
				            }
							else
							{
								next_intensity=-1;
								dense4=0;
							}
							
							if ((previous_intensity < 0) && (next_intensity < 0))
							{
								if (number_of_successful_steps != 0)
								{
							  //*(adjac_link+inter_go)=number_of_successful_steps;
								  *(extended_signif + inter_go)=total_signif;
									ChainAssignment(adjac_link,inter_go+1,number_of_successful_steps);
								}
								break;
							}
							
							dense4 += dense3;
							comp_density3 = (16 * dense4) / (dense4 + dense1);
							comp_density4 = (16 * dense4) / (dense4 + dense2);
							if ((comp_density3 >= 5) && (comp_density4 >= 5))
							{
								number_of_successful_steps++;
								total_signif+=ext_signif2;
							}
						}
						else
						{
							if (number_of_successful_steps != 0)
							{
								//*(adjac_link+inter_go)=number_of_successful_steps;
								*(extended_signif+inter_go)=total_signif;
								ChainAssignment(adjac_link,inter_go+1,number_of_successful_steps);
							}
							break;
						}
					}
					else
					{
						if (number_of_successful_steps != 0)
						{
							  //*(adjac_link+inter_go)=number_of_successful_steps;
							*(extended_signif+inter_go)=total_signif;
							ChainAssignment(adjac_link,inter_go+1,number_of_successful_steps);
						}
						break;
					}
					
					dense1=dense2;
					index_of_step=new_num;
					ext_dense1=ext_dense2;
				}
			}
		}
	}
}



//*************************************************************************************
// @Description: To each interval sets number of cluster. 
//				 These numbers are located in cluster_number array.
//*************************************************************************************
void CBunchGray::IsolatedClustersLeft(int* cluster_number, 
									  int* int_properties, 
									  int* int_new_num,     
 									  int* right_lower, int* right_upper,    
									  int* inter_weight,    
									  int* inter_intens, int* inter_num)
{  
	int NumChainCluster = 0;

	// todo: add checker for TotalNumInt (>0?)
	int* ClusterList = new int[TotalNumInt];

	for (size_t intens = 0; intens < (NUM_INTEN1-1); intens++)
	{
        int num_of_intervals = StripCur->num_of_intg[intens];
		for (size_t interval_number = 0; interval_number < num_of_intervals; interval_number++)
		{
			int inten = intens;
			int num_interv = interval_number;

			int first_new_number = *(int_new_num + inten * MaxNumInt + num_interv);
 			
			if ((cluster_number[first_new_number] != 0) || 
				(int_properties[first_new_number] == 0))
				continue;  
			
			int prev_prop_index = 3; 
			
			if (num_interv != 0)
			{
				int PrevNewNumber = *(int_new_num + inten * MaxNumInt + num_interv - 1);
				prev_prop_index = *(int_properties + PrevNewNumber);
			}

			if (prev_prop_index == 0) continue;  // look next interval  
				
			int first_interval_weight = *(inter_weight + first_new_number);
			int NumberOfMembers = 0;

			while (true)
			{
				int new_number = *(int_new_num + inten * MaxNumInt + num_interv);
 				int continuation_num = -1;
					
				ClusterizeRight(inten, num_interv, new_number, int_properties, int_new_num, cluster_number,
								right_lower, right_upper, &continuation_num);
                if (continuation_num < 0)
				{
					if (NumberOfMembers == 0)
					{
						if (first_interval_weight > 0)
						{
							NumChainCluster++;
							*(cluster_number + first_new_number) = NumChainCluster;
						}
					}
					else
					{
						// todo: ChainProcessing is empty
						ChainProcessing(intens, interval_number, NumberOfMembers, ClusterList);
						NumChainCluster++;
						ClusterMembersAssignment(NumChainCluster, NumberOfMembers, intens, 
												first_new_number, ClusterList, cluster_number, int_new_num);
					}
					break;
				}

				NumberOfMembers += 1;
				*(ClusterList + NumberOfMembers) = continuation_num;
				
				inten += 1;
				if (inten >= (NUM_INTEN1 - 1))
				{		
					// todo: ChainProcessing is empty
					ChainProcessing(intens, interval_number, NumberOfMembers, ClusterList);
					NumChainCluster++;
					// Transports elements from ClusterList to cluster_number
					ClusterMembersAssignment(NumChainCluster, NumberOfMembers, intens, 
											first_new_number, ClusterList, cluster_number, int_new_num);
					break;
				} 
				num_interv = continuation_num; 
			} 
		} 
	}
   delete [] ClusterList;
}



//*************************************************************************************
// @Description: Looks at inten+1 interval whether it includes new_number interval? 
//               If Yes, sets continuation_num to lower_bound, else to -1.
//*************************************************************************************
void CBunchGray::ClusterizeRight(int inten, int num_interv, int new_number, 
							     int* int_properties, int* int_new_num, int* cluster_number,
								 int* right_lower, int* right_upper,   int* continuation_num)
{ 
	/* Not used */
	int greater; 
 	int comparable1, comparable2;
	int intersect_measure;

	*continuation_num = -1;
	int prev_prop_index = 3;

	int lower_bound = *(right_lower + new_number);
	int upper_bound = *(right_upper + new_number);
	
	if ((lower_bound >= 0) && (lower_bound == upper_bound))
	{ 
		int beg1 = StripCur->IntAllInformGray[inten].BegInt[num_interv];
		int end1 = StripCur->IntAllInformGray[inten].EndInt[num_interv];
		int beg2 = StripCur->IntAllInformGray[inten+1].BegInt[lower_bound];
		int end2 = StripCur->IntAllInformGray[inten+1].EndInt[lower_bound];
		
		/* Not used */
		int prior = Intersection_measure(beg1, end1, beg2, end2, &greater, &comparable1, &comparable2,
										&intersect_measure);

		int new_num    = *(int_new_num + (inten + 1) * MaxNumInt + lower_bound);
		int num_clust  = *(cluster_number + new_num);
		int prop_index = *(int_properties + new_num);
			
		if (lower_bound != 0)
		{
			int PrevNewNumber = *(int_new_num + (inten + 1) * MaxNumInt + lower_bound - 1);
			prev_prop_index = *(int_properties + PrevNewNumber);
		}

		if ((num_clust == 0) && (prop_index >= 1) && (prev_prop_index >= 1))
		{
			*continuation_num = lower_bound;
		}  
	}
}




// Compares two bunches: [beg_first; end_first] and [beg_second; end_second]. Writes the degree of superiority of the 
// first bunch length in comparison two the second one (to *greater). 
int CBunchGray::Intersection_measure(int beg_first, int end_first,
									 int beg_second, int end_second,
									 int* greater,
									 int* inter_compar1,int* inter_compar2, 
									 int* intersection_length)
{
 	int prior; 
 	int compar1,compar2;

	int length_first = end_first - beg_first + 1;
	int length_second = end_second - beg_second + 1;
	int min_len = std::min(length_first, length_second);
	int max_len = std::max(length_first, length_second);

	int inter_beg = std::max(beg_first, beg_second);
	int inter_end = std::min(end_first, end_second);

	int inter_length = inter_end - inter_beg + 1;  // todo: rename. It is intersection_length
 
	*greater = 0;
	*inter_compar1 = 0;
	*inter_compar2 = 0;

	if (length_first >= (2 * length_second))
	{
		*greater = 3;
	}
	else
	{
		if (length_first >= length_second)
		{
			*greater = 2;
		}
		else
		{
			if ((2*length_first) >= length_second)
			{
				*greater = 1;
			}
		}
	}
	if (inter_beg > inter_end)
	{
		prior = 3;
	}
	else
	{
		compar1 = (16*inter_length) / (inter_length + length_first);
		compar2 = (16*inter_length) / (inter_length + length_second);
		*inter_compar1 = compar1;
		*inter_compar2 = compar2;
		if ((2 * inter_length) > max_len)
		{
			prior = 0;
		}
		else
		{ 
			prior = ((2 * inter_length) > min_len) ? 1 : 2;
		}
	} 

	*intersection_length = (prior < 3) ? inter_length : 0;
  
	return(prior);
}



 // While empty  
void CBunchGray::ChainProcessing(int inten,int num_int, int num_steps, int* ListOfIntervals)
{
	// todo: fill ListOfIntervals. Else it is random and used after.
	return;
}

 

//********************************************************************************
// @Description: Fills array cluster_numbers with num_clust at positions starting 
//				 from start_inten up to number_of_members.
//********************************************************************************
void CBunchGray::ClusterMembersAssignment(int num_clust, int number_of_members, int start_inten,  
										  int start_new_nuw,
										  int* ListOfIntervals,
										  int* cluster_numbers,
										  int* new_num)
{  
	*(cluster_numbers + start_new_nuw) = num_clust;
	for (size_t coun = 1; coun <= number_of_members; coun++)
	{
		int num_intensity = start_inten + coun;
		int num_interval = *(ListOfIntervals + coun);
		int new_number = *(new_num + num_intensity * MaxNumInt + num_interval);
		*(cluster_numbers + new_number) = num_clust;
	}
}



//****************************************************************************************************************
// @Description: Finds local_signif_sum
//****************************************************************************************************************
void CBunchGray::LocalBackgroundFinding(int* interval_weigtht, 
										int* lower_left, int* upper_left,
									    int* lower_right, int* upper_right,
										int* int_intensity,
										int* int_old_number,
										int* interval_properties,
										int* new_num, 
										int* local_signif_sum,
										int key)
{
 	int next_intensity;
  	int new_number;
   	
	for (size_t coun_int = 0; coun_int < TotalNumInt; coun_int++)
	{
		int weight_interv = *(interval_weigtht + coun_int);
		if (key == 0)
		{
			if (weight_interv <= 0)
			{
				continue;
			}
		}
		else
		{
			if (weight_interv >= 0)
			{
				continue;
			}
		}
		int fit_best_left = -2;
		int fit_best_right = -2;	 
		
		int greater_adjacent_left = -2;
		int greater_adjacent_right = -2;
 		
		int added_signif = 0;

		int lower_left_interval = *(lower_left + coun_int);
		int upper_left_interval = *(upper_left + coun_int);
		int lower_right_interval = *(lower_right + coun_int);
		int upper_right_interval = *(upper_right + coun_int);
		
		int interv_intensity = *(int_intensity + coun_int);
		int interv_number = *(int_old_number + coun_int);	   
     
		int interval_signif0 = StripCur->IntAllInformGray[interv_intensity].Signif[interv_number];
		int extended_signif = interval_signif0;
		
		int beg0 = StripCur->IntAllInformGray[interv_intensity].BegInt[interv_number];
		int end0 = StripCur->IntAllInformGray[interv_intensity].EndInt[interv_number];
		
		int length0 = end0 - beg0 + 1;
		int dense0 = (4 * interval_signif0) / length0;
		
		int previous_intensity = -1;
		
		if (interv_intensity != 0)
		{
			previous_intensity = interv_intensity - 1;
			if (lower_left_interval >= 0)
			{
				LocalGrowing(beg0, end0, dense0, previous_intensity, lower_left_interval, upper_left_interval,
								&fit_best_left, &greater_adjacent_left, &added_signif);
				extended_signif += added_signif;
				if (greater_adjacent_left >= 0)
				{
					new_number = *(new_num + (previous_intensity) * MaxNumInt + greater_adjacent_left);
					weight_interv = *(interval_weigtht + new_number);
				    if (weight_interv == 0)
					{
						*(interval_weigtht + new_number)=-1;
					}
				}
			}
		} 
		int next_intensity = -1;
		if (interv_intensity != NUM_INTEN1-1)
		{
			next_intensity = interv_intensity + 1;
			if (lower_right_interval >= 0)
			{
				LocalGrowing(beg0, end0, dense0, next_intensity, lower_right_interval, upper_right_interval,
							&fit_best_right, &greater_adjacent_right, &added_signif);
				extended_signif += added_signif;
				if (key == 0)
				{
					if (greater_adjacent_right >= 0)
					{
						new_number = *(new_num + (next_intensity)*MaxNumInt + greater_adjacent_right);
						weight_interv = *(interval_weigtht + new_number);
						if (weight_interv == 0)
						{
							*(interval_weigtht + new_number) = -1;
						}
					}
				}
			}
		} 
		local_signif_sum[coun_int] = extended_signif;
	} 
}
 


//********************************************************************************************
// @Description: Finds best_fit (interval) and additional_signif between 
//				 lower_bound and upper_bound intervals
//********************************************************************************************
void CBunchGray::LocalGrowing(int beg1, int end1,
						      int dense1, int fibre,
							  int lower_bound, int upper_bound,
						      int* best_fit,
						      int* greater_adjacent,
						      int* additional_signif)
{
	int beg2, end2;
	int interval_signif2, length2, dense2;
	int comp1, comp2, greater, prior, inter_length;
	int comp_density;
  
	*best_fit = -1;
	*greater_adjacent = -1;
	
	int added_signif = 0;
	
	if (lower_bound == upper_bound)
	{
		beg2 = StripCur->IntAllInformGray[fibre].BegInt[lower_bound];
		end2 = StripCur->IntAllInformGray[fibre].EndInt[lower_bound];
		
		prior = Intersection_measure(beg1, end1, beg2, end2, &greater, &comp1, &comp2, &inter_length);
      
		length2 = end2 - beg2 + 1;
		interval_signif2 = StripCur->IntAllInformGray[fibre].Signif[lower_bound];
		dense2 = (4*interval_signif2) / length2;
		comp_density = (16 * dense2) / (dense1 + dense2);
		if ((comp_density >= 5) && (comp_density <= 11))
		{
			if ((prior <= 1) && (comp1 >= 5) && (comp2 >= 5))
			{
				*best_fit = lower_bound;
				if ((comp_density >= 6) && (comp_density <= 9))
				{
					added_signif = (interval_signif2 * inter_length) / length2;
					*additional_signif = added_signif;
				}
				if ((comp1 > 6) && (comp2 > comp1) && ((comp_density >= 6) && (comp_density <= 10)))
				{
					*greater_adjacent = lower_bound;
				}
			}
		}
		return;
	}
	
	int fit_num = -1;
	int fitness_number = 0;
	for (int coun_shift = lower_bound; coun_shift <= upper_bound; coun_shift++)
	{ 
		beg2 = StripCur->IntAllInformGray[fibre].BegInt[coun_shift];
		end2 = StripCur->IntAllInformGray[fibre].EndInt[coun_shift];

		prior = Intersection_measure(beg1, end1, beg2, end2, 
									 &greater,&comp1,&comp2,&inter_length);
		length2 = end2 - beg2 + 1;
		interval_signif2 = StripCur->IntAllInformGray[fibre].Signif[coun_shift];
		dense2 = (4 * interval_signif2) / length2;
		comp_density = (16 * dense2) / (dense1 + dense2);
   
		if ((comp_density >= 5) && (comp_density <= 10) && (comp2 >= 6))
		{
			added_signif += (interval_signif2 * inter_length) / length2;

			if (comp1 > fitness_number)
			{
				fitness_number = comp1;
				fit_num = coun_shift;
			} 
		}
	}
   *best_fit = fit_num;
   *additional_signif = added_signif;
}



void CBunchGray::ChainAssignment(int* record_list, int first_member, int chain_length)
{ 
	int start_int = first_member;
	int label_ind = -start_int;
	int end_int = first_member + chain_length - 1;

	record_list[first_member-1] = end_int;
	
	for (int coun_chain = start_int; coun_chain <= end_int; coun_chain++)
	{
		record_list[coun_chain] = label_ind;
	}
}



void CBunchGray::FindingLocalBackgroundGrains(int* adjac_link,
                                              int* lower_left, int* upper_left,
											  int* lower_right, int* upper_right,
											  int* int_intensity,
											  int* int_old_number,
											  int* new_num,
											  int* local_signif_sum,
											  int* local_background,
											  int* valuable_interv,
											  int* value_of_place)
{
 	int new_upper_left, new_upper_right;
	int adjacent_intensity;

	int found_greater = -1;
	int found_greater1 = -1;

	for (std::uint16_t coun_int = 0; coun_int < TotalNumInt; coun_int++)
	{
		int signif_sum = local_signif_sum[coun_int];
		int adjacent_linked = adjac_link[coun_int];

		if ((adjacent_linked >= 0) && (signif_sum > 0))
		{
			int interv_intensity = int_intensity[coun_int];
			int interv_number    = int_old_number[coun_int];

			std::int16_t beg0 = StripCur->IntAllInformGray[interv_intensity].BegInt[interv_number];
            std::int16_t end0 = StripCur->IntAllInformGray[interv_intensity].EndInt[interv_number];

            int new_lower_left = lower_left[coun_int];
			int new_lower_right = lower_right[coun_int];
			
			if (adjacent_linked > 0)
			{
				int oldSegment = int_old_number[adjacent_linked];

				end0 = StripCur->IntAllInformGray[interv_intensity].EndInt[oldSegment];

				new_upper_left = upper_left[adjacent_linked];
				new_upper_right = upper_right[adjacent_linked];
			}
			else
			{
				new_upper_left = upper_left[coun_int];
				new_upper_right = upper_right[coun_int];
			}

			adjacent_intensity = interv_intensity - 1;

			if ((new_lower_left >= 0) && (adjacent_intensity >= 0))
			{
				LocalComparison(beg0, end0, signif_sum, adjacent_intensity, new_lower_left,
								new_upper_left,&found_greater,new_num,local_signif_sum);
			}

			adjacent_intensity = interv_intensity + 1;
			
			if ((new_lower_right >= 0) && (adjacent_intensity < NUM_INTEN1))
			{
				LocalComparison(beg0, end0, signif_sum,adjacent_intensity, new_lower_right,
								new_upper_right, &found_greater1, new_num, local_signif_sum);
			}
			
			if ((found_greater >= 0) || (found_greater1 >= 0))
			{
				local_background[coun_int] = 1;
			}
			else
			{
			    std::uint16_t length = end0 - beg0 + 1;
				std::uint16_t density = (4 * signif_sum) / length;

				Strip_value_painting_secondary(
						beg0, end0,
						coun_int, density,
				        valuable_interv,
				        value_of_place);

				BackgroundGrains[coun_int] = NumberOfBackgroundGrains;
				BackgrGrainOldNumber[NumberOfBackgroundGrains] = coun_int;

				NumberOfBackgroundGrains++;
			}
		}
	}
}
	


// Finds the most different bunch from {lower_bound ... upper_bound} for bunch [beg1; end1] based on its significance
// and significance from exten_signif
void CBunchGray::LocalComparison(int beg1, int end1, int signif, 
								 int fibre,
							     int lower_bound, int upper_bound,
								 int* is_greater,
								 int* new_num,
								 int* exten_signif)
{
	int beg2, end2;
	int interval_signif2;
	int comp1,comp2, greater, prior, inter_length;
	int max_signif;
	int new_number;
 
	*is_greater = -1;
	
	if (lower_bound == upper_bound)
	{
		beg2 = StripCur->IntAllInformGray[fibre].BegInt[lower_bound];
		end2 = StripCur->IntAllInformGray[fibre].EndInt[lower_bound];
      
		prior = Intersection_measure(beg1, end1, beg2, end2, &greater, &comp1, &comp2, &inter_length);
      
		interval_signif2 = StripCur->IntAllInformGray[fibre].Signif[lower_bound];
		
		new_number = *(new_num + fibre*MaxNumInt + lower_bound);
 		
		max_signif = std::max(exten_signif[new_number], interval_signif2);
       
		if ((prior <= 1) && (comp1 >= 5) && (comp2 >= 5))
		{
			if (max_signif > signif)
			{
				*is_greater = lower_bound;
			}
		} 
		return;
	}
	  
	for (int coun_shift = lower_bound; coun_shift <= upper_bound; coun_shift++)
	{
		beg2 = StripCur->IntAllInformGray[fibre].BegInt[coun_shift];
		end2 = StripCur->IntAllInformGray[fibre].EndInt[coun_shift];
		
		prior = Intersection_measure(beg1,end1,beg2,end2,&greater,&comp1,&comp2,&inter_length);
		
		interval_signif2 = StripCur->IntAllInformGray[fibre].Signif[coun_shift];
		new_number = *(new_num + fibre*MaxNumInt + coun_shift);
 		max_signif = std::max(exten_signif[new_number], interval_signif2);
		if ((prior <= 1) && (comp2 >= 6))
		{
			if (max_signif > signif)
			{
 				*is_greater = lower_bound;
				return;
			}
		}
	}
}



/*
 * @Description:
 *
 * @Parameters:
 *		segments -- numbers of most significant segments,
 *		significances -- corresponding significances.
 * @Notes:
 * 		todo: rename function
 * */
void CBunchGray::Strip_value_painting_secondary(int begin, int end,
                                                int segmentNum,
                                                int signif,
												int* segments,
												int* significances)
{
	for (int point = (begin >> PRESSING); point <= (end >> PRESSING); point++)
	{
		if ((segments[point] == 0) || (significances[point] < signif))
		{
			segments[point] = segmentNum + 1;
			significances[point] = signif;
		}
	} 
}
 


void CBunchGray::ArrangeIntervals(int* valuable_interval, 
								  int* visibility, 
								  int* local_signif_sum,
								  int* local_background,
								  int* interv_intensity,
								  int* previous_number,
								  int* lower_left, int* upper_left,
								  int* lower_right, int* upper_right,
								  int* adjac_link, int* new_number)
{ 
	int num_extent = 4;

	NumberOfClusters = 0;

	if (NumberOfBackgroundGrains == 0)
	{
		return;
	}

	ClusterGrains = new int[NumberOfBackgroundGrains];
	
	BegIntervOfClusters = new int[NumberOfBackgroundGrains];
	EndIntervOfClusters = new int[NumberOfBackgroundGrains];
	
	GrainIntensityIntervalOfClusters = new int[NumberOfBackgroundGrains];
	
	LowerIntensityOfClusters = new int[NumberOfBackgroundGrains];
	UpperIntensityOfClusters = new int[NumberOfBackgroundGrains];
	
	AverageIntensityOfClusters = new int[NumberOfBackgroundGrains];
	
	SignificanceOfClusters = new int[NumberOfBackgroundGrains];
	CardinalityOfClusters = new int[NumberOfBackgroundGrains];
	
	for (std::uint16_t space_go = 0; space_go < PressedLength; space_go++)
	{
		int corn_presense = valuable_interval[space_go];
		if (corn_presense > 0)
		{
 			visibility[corn_presense - 1]++;
		} 
	}

	int MaxVisibility = 0;
	int number_of_intervals[8] = {0};

	for (std::uint16_t interv_go = 0; interv_go < TotalNumInt; interv_go++)
	{
		int signif_sum = local_signif_sum[interv_go];
		int background_index = local_background[interv_go];
        
		if ((signif_sum > 0) && (background_index == 0))
		{
			int interv_visibility = visibility[interv_go];
		
			if (interv_visibility > MaxVisibility)
			{
				MaxVisibility = interv_visibility;
			}
		}
	}

	int* interval_zone = new int[8 * NumberOfBackgroundGrains];

	for (std::uint16_t count = 0; count < NumberOfBackgroundGrains; count++)
	{
		int total_number = BackgrGrainOldNumber[count];
		int interv_visibility = visibility[total_number];

		int zone_number = (interv_visibility * 8) / (MaxVisibility + 1);
		int inter_num = zone_number * NumberOfBackgroundGrains + number_of_intervals[zone_number];
		
		number_of_intervals[zone_number]++;
		
		interval_zone[inter_num] = total_number;
	}
	
	int contin_number = -1;
	int first_cont_interval = -1;
	int last_cont_interval = -1;
	int joined_interv = -1;
	int added_weighed_intensity = 0;
	int first_cont_interv1 = -1;
	int last_cont_interv1 = -1;

	for (std::uint8_t coun_zone = 0; coun_zone < 8; coun_zone++)
	{
		int inter_num = (7 - coun_zone) * NumberOfBackgroundGrains;
		int bound_int = number_of_intervals[7-coun_zone];
		
		for (std::uint8_t cou_int = 0; cou_int < bound_int; cou_int++)
		{
			int total_number = *(interval_zone + inter_num + cou_int);
			int extended_signif = 0;

			int clust_num = BackgroundClusters[total_number];
           
			if (clust_num == 0)
	        {
				int additional_signif = 0;
				int additional_signif1 = 0;

				int signif_sum = *(local_signif_sum + total_number);
				int adjacent_linked = *(adjac_link + total_number);
				int grain_intensity = *(interv_intensity + total_number);
				int grain_previous_number = *(previous_number + total_number);
				int interval_signif0 = StripCur->IntAllInformGray[grain_intensity].Signif[grain_previous_number];
 				
				extended_signif += signif_sum;

				int grain_beg  = StripCur->IntAllInformGray[grain_intensity].BegInt[grain_previous_number];
				int grain_end0 = StripCur->IntAllInformGray[grain_intensity].EndInt[grain_previous_number];
				
				int dense0 = (4 * interval_signif0) / (grain_end0 - grain_beg+1);
				int grain_lower_left = *(lower_left + total_number);
				int grain_lower_right = *(lower_right + total_number);
				
				int grain_upper_left = *(upper_left + total_number);
				int grain_upper_right = *(upper_right + total_number);
				int grain_end = StripCur->IntAllInformGray[grain_intensity].EndInt[grain_previous_number];
				
				if (adjacent_linked > 0)
				{
					int interv_number1 = *(previous_number + adjacent_linked);
					int grain_end = StripCur->IntAllInformGray[grain_intensity].EndInt[interv_number1];
					int grain_upper_left = *(upper_left+adjacent_linked);
					int grain_upper_right = *(upper_right+adjacent_linked);
					
					for (int count_interv = total_number; count_interv <= adjacent_linked; count_interv++)
					{
						*(BackgroundClusters + count_interv) = NumberOfClusters + 1;
					}
				}
				else
				{
					*(BackgroundClusters + total_number) = NumberOfClusters + 1; 
				}
		
				int failure_counter_left = 0;
				int failure_counter_right = 0;
		
				int contin_intensity = -1;
				int contin_intensity1=-1;
				
				ClusterGrains[NumberOfClusters] = total_number;
				BegIntervOfClusters[NumberOfClusters] = grain_beg;
				EndIntervOfClusters[NumberOfClusters] = grain_end;
		
				int total_joined_interv = 1;
				long weighed_intensity = ((long)(grain_intensity))*((long)signif_sum);
 
				for (size_t count_cont = 1; count_cont <= num_extent; count_cont++)
				{
					int contin_result = 0;

					if (grain_lower_left != -1)
					{
						contin_result = GrainsGrowing(total_number,grain_intensity,grain_previous_number,
						grain_lower_left,grain_upper_left,grain_beg,grain_end,dense0,signif_sum,-count_cont,
						adjacent_linked,new_number,local_signif_sum,&additional_signif, &first_cont_interval,
							&last_cont_interval, &joined_interv, &added_weighed_intensity);

						if (contin_result == 1)
						{
							contin_intensity = grain_intensity - count_cont;
							contin_number = *(new_number + contin_intensity * MaxNumInt + first_cont_interval);
							int number_of_intersect_int = last_cont_interval - first_cont_interval+1;
							total_joined_interv += joined_interv;
							weighed_intensity += (long)added_weighed_intensity;
							int lower_cand = *(lower_left + contin_number);
							if (lower_cand == -1)
							{
								int lower_shift = FindingIntervalsWithAdjacent(contin_number + number_of_intersect_int,
																			   contin_number, 1,
																				lower_left);
								if (lower_shift != -1)
								{
									grain_lower_left = *(lower_left + lower_shift);
								}
								else
								{
									grain_lower_left = -1;
								}
							}
							else
							{
								grain_lower_left = lower_cand;
							}
							contin_number = *(new_number+contin_intensity*MaxNumInt+last_cont_interval);
							int upper_cand = *(upper_left + contin_number);
							if (upper_cand == -1)
							{
								int upper_shift = FindingIntervalsWithAdjacent(contin_number - number_of_intersect_int, 
																		       contin_number, -1,
																			   upper_left);
 
								grain_upper_left = (upper_shift != -1) ? *(upper_left + upper_shift) : -1;
 
							}
							else
							{
								grain_upper_left = upper_cand;
							}
							extended_signif += additional_signif; 
						}
						else
						{
							grain_lower_left = -1;
							grain_upper_left = -1;
							failure_counter_left++;
	//we can continue here the process if failure_counter_left is sufficiently small
						}
					}
					else
					{
						additional_signif=0;
					}
					int contin_result1 = 0;

					if (grain_lower_right != -1)
					{
						contin_result1 = GrainsGrowing(total_number,grain_intensity,grain_previous_number,
										grain_lower_right,grain_upper_right,grain_beg,grain_end,dense0,signif_sum,count_cont,
										adjacent_linked,new_number,local_signif_sum,&additional_signif1, &first_cont_interv1,
										&last_cont_interv1,&joined_interv,&added_weighed_intensity);
						if (contin_result1 == 1)
						{
							contin_intensity1 = grain_intensity+count_cont;
							int contin_number1 = *(new_number + contin_intensity1*MaxNumInt + first_cont_interv1);
							int number_of_intersect_int = last_cont_interv1-first_cont_interv1+1;

							total_joined_interv += joined_interv;
							weighed_intensity += (long)added_weighed_intensity;
							
							int lower_cand = *(lower_right + contin_number1); 
							if (lower_cand == -1)
							{
								int lower_shift = FindingIntervalsWithAdjacent(contin_number1+number_of_intersect_int,contin_number1,1,
																			lower_right);
 
								grain_lower_right = (lower_shift != -1) ? *(lower_right + lower_shift) : -1; 
							}
							else
							{
								grain_lower_right = lower_cand;
							}
							contin_number1 = *(new_number + contin_intensity1 * MaxNumInt + last_cont_interv1);

							int upper_cand = *(upper_right + contin_number1);

							if (upper_cand == -1)
							{
								int upper_shift = FindingIntervalsWithAdjacent(contin_number1 - number_of_intersect_int,
																		       contin_number1,-1,upper_right);
 
								grain_upper_right = (upper_shift != -1) ? *(upper_right + upper_shift) : -1;
 
							}
							else
							{
								grain_upper_right = upper_cand;
							}

							extended_signif += additional_signif1;
						}
						else
						{
							grain_lower_right = -1;
							failure_counter_right++;
				
							if ((failure_counter_left >= 1) && (failure_counter_right >= 1))
							{
								break;
							}
						}
					}
					else
					{
						additional_signif = 0;
					} 
				}

				LowerIntensityOfClusters[NumberOfClusters] = (contin_intensity != -1) ? contin_intensity: grain_intensity;
				UpperIntensityOfClusters[NumberOfClusters] = (contin_intensity1 != -1) ? contin_intensity1 : grain_intensity;

				SignificanceOfClusters[NumberOfClusters] = extended_signif; 
				CardinalityOfClusters[NumberOfClusters] = total_joined_interv;

				AverageIntensityOfClusters[NumberOfClusters] = (int)(weighed_intensity / (long)extended_signif);
				NumberOfClusters++;
			}
		} 
	}

    delete[] interval_zone;
}
 


int CBunchGray::GrainsGrowing(int num_grain, int gr_intensity, int gr_prev_number,
							  int adjacent_lower_interv, int adjacent_upper_interv,
							  int beg_grain, int end_grain,
							  int grain_dens0,
							  int signif,
							  int direct,
							  int adj_link,
							  int* new_num,
							  int* local_signif_sum,
							  int* added_signif,
							  int* starting_cont_interv,
							  int* finishing_cont_interv,
							  int* joined_intervals,
							  int* added_w_intens)
{
	int beg2, end2;
	int interval_signif2;
	int comp1,comp2,greater,prior,inter_length;
	int extended_signif;
 	int dens1;
	int comp_density1;
 	int number_of_intervals;
	int total_joined_length,joined_length;
  
	int first_interval = -1;
	int last_interval = -1;
	int extension_result = 0;
	int number_of_joined_intervals = 0;
	int number_of_dense_int = 0;
	int total_signif = 0;
	int dense_signif = 0;
	int joined_signif = 0;
	int max_signif = 0;
	int added_intens = 0;
	int comp_total_length = 0;
	int dense_total = 0;
	int grain_length = end_grain - beg_grain + 1;
	int new_intensity = gr_intensity + direct;
	int shift = std::abs(direct);
	int new_number = *(new_num+new_intensity*MaxNumInt+adjacent_lower_interv);

	if (adjacent_lower_interv == adjacent_upper_interv)
	{
		beg2 = StripCur->IntAllInformGray[new_intensity].BegInt[adjacent_lower_interv];
		
		if (*(BackgroundClusters + new_number))
		{
			return extension_result;
		}
	  
		end2 = StripCur->IntAllInformGray[new_intensity].EndInt[adjacent_lower_interv];
		prior = Intersection_measure(beg_grain,end_grain,beg2,end2,&greater,&comp1,&comp2,&inter_length);
		interval_signif2 = StripCur->IntAllInformGray[new_intensity].Signif[adjacent_lower_interv];
		joined_length = end2-beg2+1;
		dens1 = (4 * interval_signif2) / joined_length;
		extended_signif = *(local_signif_sum + new_number);
		max_signif = std::max(extended_signif, interval_signif2);
		comp_density1 = (16 * dens1) / (dens1 + grain_dens0);
		
		if ((prior <= 1) && (signif > max_signif))
		{
			if (comp2 >= 6)
			{
				if(((comp_density1>=6)&&(comp_density1<=10))||((shift<=2)&&(comp_density1>=3)&&
					(comp_density1<=11)))
				{
              
					number_of_dense_int++;
					number_of_joined_intervals++;
					joined_signif+=interval_signif2;
					added_intens=new_intensity*interval_signif2;
					*(BackgroundClusters+new_number) = NumberOfClusters + 1;
					BegIntervOfClusters[NumberOfClusters] = std::min(BegIntervOfClusters[NumberOfClusters],beg2);
					EndIntervOfClusters[NumberOfClusters] = std::max(EndIntervOfClusters[NumberOfClusters],end2);
				}
				else
				{
			  if(((shift<=2)&&(comp_density1>=2)&&(comp_density1<=10)&&(comp2>=7))||
				  ((comp2>=7)&&(shift==1)&&(comp_density1>=2)&&(comp_density1<=10))
				  ||
				  ((comp2>=7)&&(comp1>=7)&&(shift<=3)&&(comp_density1>=2)&&(comp_density1<=10)))

			  {
			  number_of_joined_intervals++;
			  *(BackgroundClusters+new_number) = NumberOfClusters + 1;
			  joined_signif+=interval_signif2;
			  
			  }
		  }
			 }
			 else
			{
				if((shift==1)&&(comp1<=2)&&(comp2>=5)&&(comp_density1>=6))
				{
			  *(BackgroundClusters+new_number) = NumberOfClusters + 1;
			  number_of_dense_int++;
			  number_of_joined_intervals++;
			  joined_signif+=interval_signif2;
			  added_intens=new_intensity*interval_signif2;
				BegIntervOfClusters[NumberOfClusters] = std::min(BegIntervOfClusters[NumberOfClusters],beg2);
				EndIntervOfClusters[NumberOfClusters] = std::max(EndIntervOfClusters[NumberOfClusters],end2);
				}
			}
	  }
	  *added_signif=joined_signif;
	  comp_total_length=(16*joined_length)/(joined_length+grain_length);
	    if(number_of_dense_int==1)
	     {
			 extension_result=1;
			 first_interval=adjacent_lower_interv;
			 last_interval=adjacent_lower_interv;
			 
		 }
		else
		{
			if(number_of_joined_intervals==1)
			{
				if(comp_total_length>=6)
		  {
		  extension_result=1;
		  first_interval=adjacent_lower_interv;
		  last_interval=adjacent_lower_interv;
		  }
		  else
		  {
				extension_result=2;
				first_interval=adjacent_lower_interv;
		        last_interval=adjacent_lower_interv;
		  }
				
			}
		}
	 *starting_cont_interv=first_interval;
	 *finishing_cont_interv=last_interval;
	 *joined_intervals=number_of_joined_intervals;
	 *added_w_intens=added_intens;
	  return(extension_result);
     }
	else
	{
		number_of_intervals=adjacent_upper_interv-adjacent_lower_interv+1;
		total_joined_length=0;
		
		
		for(int coun_int=adjacent_lower_interv;coun_int<=adjacent_upper_interv;coun_int++)
         {
			 new_number=*(new_num+new_intensity*MaxNumInt+coun_int);
			 if(!*(BackgroundClusters+new_number))
			 {
beg2=StripCur->IntAllInformGray[new_intensity].BegInt[coun_int];
end2=StripCur->IntAllInformGray[new_intensity].EndInt[coun_int];
         interval_signif2=
			 StripCur->IntAllInformGray[new_intensity].Signif[coun_int];
		 if(interval_signif2>max_signif)
		 {
		 max_signif=interval_signif2;
		 }
         prior=
Intersection_measure(beg_grain,end_grain,beg2,end2,&greater,&comp1,&comp2,&inter_length);
		 joined_length=end2-beg2+1;
dens1=(4*interval_signif2)/joined_length;
comp_density1=(16*dens1)/(dens1+grain_dens0);
if((prior<=1)&&(signif>interval_signif2))
	  {
	   
			if(comp2>=6)
			{
		  if(((comp_density1>=5)&&(comp_density1<=10))||((shift<=2)&&(comp_density1>=3)&&
			  (comp_density1<=11)))
		  {
		  number_of_dense_int++;
		  number_of_joined_intervals++;
	      *(BackgroundClusters+new_number) = NumberOfClusters + 1;
		  dense_signif+=interval_signif2;
          total_joined_length+=joined_length;
		  added_intens+=new_intensity*interval_signif2;
			BegIntervOfClusters[NumberOfClusters] = std::min(BegIntervOfClusters[NumberOfClusters],beg2);
			EndIntervOfClusters[NumberOfClusters] = std::max(EndIntervOfClusters[NumberOfClusters],end2);
		    if(first_interval==-1)
			{
            first_interval=coun_int;
			}
			last_interval=coun_int;
		  }
		  else
		  {
 if(((comp2>=7)&&(shift<=2)&&(comp_density1>=2)&&(comp_density1<=10))||
	 ((shift==1)&&(comp2>=7)&&(comp_density1>=2)&&(comp_density1<=10))
	 ||((comp2>=7)&&(comp1>=7)&&(shift<=3)&&(comp_density1>=2)&&(comp_density1<=10)))
			  {
			  *(BackgroundClusters+new_number) = NumberOfClusters + 1;
			  number_of_joined_intervals++;
			  joined_signif+=interval_signif2;
			  total_joined_length+=joined_length;
			  if(first_interval==-1)
			   {
               first_interval=coun_int;
			   }
			   last_interval=coun_int;
			  }
		  }
		  
			}
			else
			{
				if((shift==1)&&(comp1<=2)&&(comp2>=5)&&(comp_density1>=6)&&(comp_density1<=10))
				{
			  *(BackgroundClusters+new_number) = NumberOfClusters + 1;
			  number_of_dense_int++;
			  number_of_joined_intervals++;
			  dense_signif+=interval_signif2;
			  total_joined_length+=joined_length;
			  added_intens+=new_intensity*interval_signif2;
				BegIntervOfClusters[NumberOfClusters] = std::min(BegIntervOfClusters[NumberOfClusters],beg2);
				EndIntervOfClusters[NumberOfClusters] = std::max(EndIntervOfClusters[NumberOfClusters],end2);
			     if(first_interval==-1)
			     {
                 first_interval=coun_int;
			     }
				 last_interval=coun_int;
				}
			}
			
		
	  }
		 total_signif+=interval_signif2;
			 }
		 }
		 if(total_joined_length)
		 {
dense_total=(4*total_signif)/total_joined_length;
comp_total_length=(16*total_joined_length)/(total_joined_length+grain_length);
		 }
*added_signif=joined_signif+dense_signif;
      if(dense_signif)
	  {
		  extension_result=1;
	  }
	  else
	  {
		  if(comp_total_length>=6)
		  {
		  extension_result=1;
		  }
		  else
		  {
           extension_result=2;
		  }
		}
		*starting_cont_interv = first_interval;
		*finishing_cont_interv = last_interval;
		*joined_intervals = number_of_joined_intervals;
		*added_w_intens = added_intens;
		return(extension_result);
	}
}
 


int CBunchGray::FindingIntervalsWithAdjacent(int last_member, 
											 int first_member, 
											 int direc, 
											 int* boundary_int)
{ 
	int new_member = -1;
	int param = first_member + direc;
	
	while (param != last_member)
	{
		if (boundary_int[param] != -1)
		{
			new_member = param;
			break;
		}
		param += direc;
	}
	return new_member;
}



/*
 *  @Description:
 *       Constructs burst bunches from given segments of a strip and adds them to bursts list.
 *  @Parameters:
 *      @In:    max_length -- upper boundary of bunch length,
 *              depth -- shows how deep we go forward along intensities.
 *  @Return value:
 *              -1 -- if no bunch was found,
 *              0 -- otherwise.
 *  @Notes:
 *  	todo: make convenient and efficient indexation -> (intens, interv) (e.g. B-tree).
*/
std::int8_t CBunchGray::find_bursts(std::uint16_t max_length,
                                    std::uint8_t depth)
{
    // todo: analyse performance of clear()
    bursts.clear();

    /* Find maximal intensity with non-empty set of segments */
    TIntCharactGray* segments = StripCur->IntAllInformGray;

    std::uint8_t start_intens = NUM_INTEN1 - 1;
    std::uint8_t segments_num = segments[start_intens].num_of_int;

    while ((start_intens > 0) && (segments_num == 0))
    {
        --start_intens;
        segments_num = segments[start_intens].num_of_int;
    }

    // Dark min intensity is not appropriate for obvious reasons
    if (start_intens == 0)
        return -1;


    // Array (or matrix) showing was a segment (intens, number) already picked or not
    std::vector<bool> picked_segments(NUM_INTEN1 * MAX_INT_NUMBER, false);

    // todo: not only start_intens can contain necessary seeds
    for (size_t segment = 0; segment < segments_num; segment++)
    {
        bool segment_picked = picked_segments[start_intens * MAX_INT_NUMBER + segment];

        if (segment_picked)
            continue;

        picked_segments[start_intens * MAX_INT_NUMBER + segment] = true;

        std::int16_t beg = segments[start_intens].BegInt[segment];
        std::int16_t end = segments[start_intens].EndInt[segment];

        Segment origin_segment(beg, end);

        if (origin_segment.length() > max_length)
            continue;

        /* Grow a bunch going from start_intens to left */
        std::uint8_t picked_segments_num = 1;
        std::uint8_t next_intens = start_intens - 1;
        std::uint8_t intens_differ = start_intens - next_intens;
        float mean_intens = start_intens;

        while (intens_differ < depth)
        {
            std::uint8_t next_segments_num = segments[next_intens].num_of_int;

            for (size_t next_segment = 0; next_segment < next_segments_num; next_segment++)
            {
                if (picked_segments[next_intens * MAX_INT_NUMBER + next_segment])
                    continue;

                std::int16_t candidate_beg = segments[next_intens].BegInt[next_segment];
                std::int16_t candidate_end = segments[next_intens].EndInt[next_segment];

                Segment segment_candidate(beg, end);
				std::uint16_t r1, r2; // useless, only for measure_intersection
				bool intersected = measure_intersection(origin_segment, segment_candidate, &r1, &r2) <= 1;
                if (intersected)
                {
                    candidate_beg = std::min(beg, candidate_beg);
                    candidate_end = std::max(end, candidate_end);
                    // if new length is not big
                    if ((candidate_end - candidate_beg + 1) < max_length)
                    {
                        picked_segments[next_intens * MAX_INT_NUMBER + next_segment] = true;
                        beg = candidate_beg;
                        end = candidate_end;
                        mean_intens += next_intens;
                        ++picked_segments_num;
                    }
                }
            }
            next_intens--;
            intens_differ = start_intens - next_intens;
        }
        mean_intens /= picked_segments_num;
        GrayBunch bunch(beg, end, StripCur->num_strip, mean_intens);
        // todo: analyse copy
        bursts.push_back(bunch);
    }
    return 0;
}
