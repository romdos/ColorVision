#include "ColorVision.h"
#include "ColorSection.h"
#include "ColorIntervalSelect.h"

#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>

using namespace std;

#define HORIZONT1 0
//#define LENGTH 16
#define TRUE 1

static  int hue_zones[NUM_HUES] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
		5, 1, 1, 1, 1, 1};
		static  int dev_hue_zone[6] = {2, 2, 2, 2, 2, 2};
		static  int dev_mean_sat[16] = {1, 1, 1, 1, 1, 2, 3, 4, 4, 4, 4, 4, 3, 2, 1, 0};
		static  int dev_mean_sat1[16] = {0, 1, 1, 1, 1, 1, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4};
		static  int imp_dev_mean_sat[16] = {3, 3, 3, 4, 5, 5, 6, 6, 6, 6, 6, 5, 4, 4, 4, 4};
		static  int imp_dev_mean_sat1[16] = {1, 2, 2, 3, 3, 3, 3, 4, 5, 5, 5, 6, 6, 7, 6, 6};

		static  int gray_zones[NUM_INTEN1] = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7};//last_cor05.06.15
		//0. 0-6, 1. 7-12, 2. 13-21, 3. 22-30, 4. 31-39, 5. 40-49, 6. 50-57, 7. 58-63

		//static  int dev_mean[8] = {5, 8, 8, 10, 10, 10, 12, 8};
		static  int dev_mean[8] = {5, 8, 8, 8, 8, 10, 12, 8};

		//static  int imp_dev_mean[8] = {14, 16, 18, 18, 16, 16, 14, 12};
		static  int imp_dev_mean[8] = {12, 12, 12, 12, 12, 12, 12, 12};//last_cor03.12.14
		//=====================================================

		ColorSectionDescr::ColorSectionDescr(void) {
			length_of_section = 0;
		}
		//----------------------------------------------------------------------------
		ColorSectionDescr::~ColorSectionDescr(void) {
			//delete[] location_of_section;
		}

		//----------------------------------------------------------------------------

		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		CColorSection::CColorSection(int NumStrips, ColorSectionDescr* CurrentSecDes, CStrip* CurrentStrip,
				CColorIntervalSelect* ColorInterv) {
			Num_Strips = NumStrips;
			DescrSec = CurrentSecDes;
			StripCurrent = CurrentStrip;
			ColorInterval = ColorInterv;
			//DimX = StripCurrent[0].DimX;
			//DimY = StripCurrent[0].DimY;
			//HorizontalVertical = StripCurrent[0].HorizontalVertical;
			//StripWidth = StripCurrent[0].StripWidth;
			NumMaxSection = NUM_SECT1;
			Number_of_sections = 0;
			//section_fibre_first=new int[NumMaxSection];
			//section_fibre_last=new int[NumMaxSection];
			//section_out_connection=new int[NumMaxSection];
			//SectionVisibility=new int[NumMaxSection];
			//root_section=new int[NumMaxSection];
			//section_point_cross=new int[NumMaxSection];

		}

		CColorSection::~CColorSection() {
			delete[] section_fibre_first;
			delete[] section_fibre_last;
			delete[] section_left_end;
			delete[] section_right_end;
			delete[] section_out_connection;
			delete[] root_section;
			delete[] section_point_cross;
			delete[] section_mean_hue;
			delete[] section_mean_saturation;
			delete[] section_mean_gray;
			delete[] section_weight;
			delete[] section_fiber_max_length;
			delete[] section_upper_mean_length;//last_cor14.11.16
			delete[] section_lower_mean_length;//last_cor14.11.16
			//delete[] trace_of_section;
			delete[] section_upper_hues;
			delete[] section_lower_hues;
			delete[] section_upper_saturations;
			delete[] section_lower_saturations;
			delete[] section_upper_grays;
			delete[] section_lower_grays;
			delete[] left_section_start_number;//01.04.15
			delete[] right_section_start_number;//01.04.15
			delete[] left_section_last_number;//01.04.15
			delete[] right_section_last_number;//01.04.15
			delete[] sections_under;//14.04.16

			//delete[] SectionVisibility;

		}

		//----------------------------------------------------------------------------
		void

		CColorSection::SectionTracking(int left_right_boundary,int* section_trace)
		{

		int previous_strip,start_strip,nex_fibre;
		int mem_strip,mem_beg_grain;
		int new_mem_beg_grain;
		int new_beginning;
		int member;
		int next_value;
		int new_next_value;
		//int next_grain_inten;
		int gay,count_of_displacement;
		int counting_of_fibres,coun_strips,run_prev;
		int trace,new_trace;
		//int run_sect;
		//int consistent_fibres,grain_long;
		//int section_consistency;
		//int* trace_of_section;
		int zero_sec;
		int NumberOfBunches;
		int num_strp;
		int bunch_signif;
		int beg_int;
		int end_int;
		int Strwidth;
		int first_signif;
		TIntColored* ColoredIntervalsStructure;




		/*DimX=ColorInterval[0].StripCurrent->DimX;
		DimY=ColorInterval[0].StripCurrent->DimY;
		HorizontalVertical=ColorInterval[0].StripCurrent->HorizontalVertical;*/
		FirstGrain=-1;
		dev_mean[0]=5;
		  dev_mean[1]=8;
		  dev_mean[2]=8;//last_cor03.12.14
		  dev_mean[3]=8;
		  dev_mean[4]=8;
		  dev_mean[5]=10;
		  dev_mean[6]=12;
		  dev_mean[7]=8;
		  LeftRightBoundary=left_right_boundary;
		  StripLength=ColorInterval[0].StripLength;
		  StripWidth=ColorInterval[0].StripWidth;//last_cor15.05.14

		if(!LeftRightBoundary)
		{
		memset(section_point_cross,(int) '\0',sizeof(int)*NUM_SECT1);
		memset(section_out_connection,(int) '\0',sizeof(int)*NUM_SECT1);
		memset(left_section_start_number,(int) '\0',sizeof(int)*Num_Strips);//15.4.15
		memset(section_upper_mean_length,(int) '\0',sizeof(int)*NUM_SECT1);//last_cor16.11.16
		memset(section_lower_mean_length,(int) '\0',sizeof(int)*NUM_SECT1);
		memset(right_section_start_number,(int) '\0',sizeof(int)*Num_Strips);
		memset(left_section_last_number,(int) '\0',sizeof(int)*Num_Strips);
		memset(right_section_last_number,(int) '\0',sizeof(int)*Num_Strips);
		memset(sections_under,(int) '\0',sizeof(int)*(8*NUM_SECT1));
		}

		MaximalNumberOfBunches=0;
		for(num_strp=0;num_strp<Num_Strips;num_strp++)
		{
		 NumberOfBunches=ColorInterval[num_strp].NumberOfColoredIntervals;
		 		if(NumberOfBunches>MaximalNumberOfBunches)
				{
				MaximalNumberOfBunches=NumberOfBunches;
				}
		}
		if(MaximalNumberOfBunches==0)
		{
		return;
		}
		trace_of_section=section_trace;
		if(!LeftRightBoundary)
		{
			trace_of_section1=trace_of_section;
		}
		memset(trace_of_section,(int) '\0',sizeof(int)*(Num_Strips)*MAX_COL_INT);
		if(!LeftRightBoundary)
		{
		for(zero_sec=0;zero_sec<NUM_SECT1;zero_sec++)
						{
						//DescrSec[zero_sec].location_of_section=new int[Num_Strips];
		memset(DescrSec[zero_sec].location_of_section,(int) '\0',sizeof(int)*Num_Strips);
						}
		}
			start_strip=0;
			gay=0;
			next_value=-1;
			new_beginning=0;

			if(!LeftRightBoundary)
			{
				Number_of_sections=0;
				Number_of_sections_left=0;
			}
			else
			{
			Number_of_sections_right=0;
			}

			while(TRUE)
			  {
			     if(!LeftRightBoundary)
				 {
			  if(Number_of_sections_left>=NUM_SECT1/2)
			  {
			  goto wrap;
			  }
			     }
				 else
				 {
					 if(Number_of_sections_right>=NUM_SECT1/2)
			         {
			          goto wrap;
			         }
				 }
			  mem_strip=Num_Strips+1;
			FirstHueZone=0;
			FirstHue=-1;
			FirstSat=0;
			FirstGray=-1;
			FirstGrayZone=-1;
			FirstDensity=-1;
				 for(coun_strips=start_strip;coun_strips<Num_Strips-1;coun_strips++)
				  {//cs=strp
			if(Section_start(coun_strips,&trace_of_section[coun_strips*MAX_COL_INT],
					 &new_beginning,LeftRightBoundary))
					 {//if
					 mem_strip=coun_strips;
					 mem_beg_grain=new_beginning;
					 FirstGrain=mem_beg_grain;
					 FirstStrip=mem_strip;
			 FirstHue=ColorInterval[mem_strip].ColoredIntervalsStructure->AverageHue[mem_beg_grain];
			FirstHueZone=hue_zones[FirstHue];
			FirstSat=ColorInterval[mem_strip].ColoredIntervalsStructure->AverageSat[mem_beg_grain];
			FirstGray=ColorInterval[mem_strip].ColoredIntervalsStructure->AverageGray[mem_beg_grain];
			FirstGrayZone=gray_zones[FirstGray];
			FirstBeg=ColorInterval[FirstStrip].ColoredIntervalsStructure->BegInterv[FirstGrain];//lc27.11.15
			FirstEnd=ColorInterval[FirstStrip].ColoredIntervalsStructure->EndInterv[FirstGrain];//lc27.11.15
			FirstLength=FirstEnd-FirstBeg+1;
			first_signif=ColorInterval[FirstStrip].ColoredIntervalsStructure->Significance[FirstGrain];
			FirstDensity=(2*first_signif)/FirstLength;
			TotalShift=0;
			if((mem_strip<(2*Num_Strips)/3)||(FirstDensity>StripWidth/2))
			//if(mem_strip<(2*Num_Strips)/3)
			 {//last_cor19.02.18
			break;
			 }
			else
			{//last_cor27.03.17
			start_strip=mem_strip;
			goto conti;
			}//last_cor27.03.17

					 }//if
				  }//cs=strp
			if(mem_strip>Num_Strips)
			 {
			 goto wrap;
			 }

			        nex_fibre=mem_strip+1;
					  previous_strip=mem_strip;
					  counting_of_fibres=1;
				count_of_displacement=0;
			   for(run_prev=0;run_prev<Num_Strips-1;run_prev++)
				 {

			     for(member=0;member<Num_Strips;member++)
					{
					next_value=MaximalNumberOfBunches+1;
					ColoredIntervalsStructure=ColorInterval[previous_strip].ColoredIntervalsStructure;
					Strwidth=ColorInterval[previous_strip].StripWidth;
			bunch_signif=ColoredIntervalsStructure->Significance[new_beginning];
							beg_int=ColoredIntervalsStructure->BegInterv[new_beginning];
							end_int=ColoredIntervalsStructure->EndInterv[new_beginning];
							bunch_signif=(2*bunch_signif)/(end_int-beg_int+1);
			gay=Continuation_of_section(new_beginning,&next_value,previous_strip,nex_fibre,counting_of_fibres);

			         if(gay>0)
						 {//gay>0
						 count_of_displacement=0;
			trace=trace_of_section[nex_fibre*MAX_COL_INT+next_value];
			//trace=abs(trace);
							 if(trace>0)
							 {//tr>0
							 new_trace=trace-1;
							 section_point_cross[Number_of_sections]=nex_fibre;
							 section_out_connection[Number_of_sections]=trace;
						 if(root_section[new_trace])
					{
					root_section[Number_of_sections]=root_section[new_trace];
					}
					 else
					 {
					 root_section[Number_of_sections]=trace;
					 }
					 counting_of_fibres++;
			new_next_value=ColorInterval[nex_fibre].new_bunch_number[next_value];
					 Record_of_section(nex_fibre,
							  &trace_of_section[nex_fibre*MAX_COL_INT],
							  new_next_value,next_value,Number_of_sections);
			new_mem_beg_grain=ColorInterval[mem_strip].new_bunch_number[mem_beg_grain];
							 End_of_process(mem_strip,new_mem_beg_grain,Number_of_sections,
							 nex_fibre,counting_of_fibres);

							 start_strip=mem_strip;
							 Number_of_sections++;
							 if(!LeftRightBoundary)
				             {
				              Number_of_sections_left++;
				             }
				             else
				             {
				              Number_of_sections_right++;
				             }
			*(trace_of_section+mem_strip*MAX_COL_INT+mem_beg_grain)=
			Number_of_sections;
							 goto conti;
							 }//tr>0
							 counting_of_fibres++;
			new_next_value=ColorInterval[nex_fibre].new_bunch_number[next_value];
							 Record_of_section(nex_fibre,
							 &trace_of_section[nex_fibre*MAX_COL_INT],
							  new_next_value,next_value,Number_of_sections);
						 if((nex_fibre+1)>=Num_Strips)
									  {//nf>=
			new_mem_beg_grain=ColorInterval[mem_strip].new_bunch_number[mem_beg_grain];
			End_of_process(mem_strip,new_mem_beg_grain,Number_of_sections,
							 nex_fibre,counting_of_fibres);
			gay=0;
			              if(mem_strip>=1)
						  {
			gay=Continuation_of_section(mem_beg_grain,&next_value,mem_strip,mem_strip-1,counting_of_fibres);
						  }
						  else
						  {
							  if((bunch_signif)>(Strwidth))
							  {
			                  gay=-1;
							  }
						  }
										  Number_of_sections++;
							 if(!LeftRightBoundary)
				             {
				              Number_of_sections_left++;
				             }
				             else
				             {
				              Number_of_sections_right++;
				             }
							 if(gay<0)
							 {
			*(trace_of_section+mem_strip*MAX_COL_INT+mem_beg_grain)=
			1000+Number_of_sections;
							 }
							 else
							 {
							 *(trace_of_section+mem_strip*MAX_COL_INT+mem_beg_grain)=
			Number_of_sections;
							 }

										  start_strip=mem_strip;
										goto conti;
									  }//nf>=
								 previous_strip=nex_fibre;
								 nex_fibre++;
								 new_beginning=next_value;
							 break;
						 }//g>0
						 else
							{
			ColoredIntervalsStructure=ColorInterval[previous_strip].ColoredIntervalsStructure;
							bunch_signif=ColoredIntervalsStructure->Significance[new_beginning];
							beg_int=ColoredIntervalsStructure->BegInterv[new_beginning];
							end_int=ColoredIntervalsStructure->EndInterv[new_beginning];
							bunch_signif=(2*bunch_signif)/(end_int-beg_int+1);
			if((++nex_fibre>=Num_Strips)||(++count_of_displacement>HORIZONT1)||
			(bunch_signif)<(Strwidth/2))
								{
								 if(counting_of_fibres>=2)
								  {//cf
			 new_mem_beg_grain=ColorInterval[mem_strip].new_bunch_number[mem_beg_grain];
							 End_of_process(mem_strip,new_mem_beg_grain,Number_of_sections,
							 previous_strip,counting_of_fibres);

									if(gay<0)
									{//g1<0
			*(trace_of_section+previous_strip*MAX_COL_INT+new_beginning)=
			1000+(Number_of_sections+1);
									}
							 if(!LeftRightBoundary)
				             {
				              Number_of_sections_left++;
				             }
				             else
				             {
				              Number_of_sections_right++;
				             }
							 gay=0;
			              if(mem_strip>=1)
						  {
			gay=Continuation_of_section(mem_beg_grain,&next_value,mem_strip,mem_strip-1,counting_of_fibres);
						  }
						  else
						  {
							  if((bunch_signif)>(Strwidth))
							  {
			                  gay=-1;
							  }
						  }
						  Number_of_sections++;
							 if(gay<0)
							 {
			*(trace_of_section+mem_strip*MAX_COL_INT+mem_beg_grain)=
			1000+Number_of_sections;
							 }
							 else
							 {
							 *(trace_of_section+mem_strip*MAX_COL_INT+mem_beg_grain)=
			Number_of_sections;
							 }
								  }
								 start_strip=mem_strip;
								 goto conti;
								}

							}
					}
				 }
			conti:
			;
			  }
			wrap:;

			/*section_mean_hue=new int[Number_of_sections];
			section_mean_saturation=new int[Number_of_sections];
			section_mean_gray=new int[Number_of_sections];*/
			  if(LeftRightBoundary)
			  {
			MeanCharacteristics();
			FindingLowerAdjacentSections();
			  }
			//delete[] trace_of_section;


			}
		//----------------------------------------------------------------------------
		int

		CColorSection::Section_start(int strip_num,int *status_show,int *new_beg,int left_right_bound)
		{
			int foll;
			int success_of_seaching;
			int fibre_limit;
			int blocking;
			int left_cont;
			int right_cont;
			int direct_cont;
			int beg;
			int end;
			int length;
			int reference_size;
			int left_right;
			bool end_condition;
			//TIntColored* ColoredIntervalsStructure;

			//ColoredIntervalsStructure=ColorInterval[strip_num].ColoredIntervalsStructure;
			success_of_seaching=0;
			left_right=left_right_bound;

			if(HorizontalVertical)
				{
				reference_size=DimY;
				}
				else
				{
				reference_size=DimX;
				}

			fibre_limit=ColorInterval[strip_num].NumberOfColoredIntervals;

				 if(fibre_limit==0)
				 {
				 return(success_of_seaching);
				 }
			  for(foll=0;foll<fibre_limit;foll++)
			  {//follloop
			  		blocking = ColorInterval[strip_num].bunch_blocking[foll];
			  		if (!blocking)
			  		{//bllast_cor19.02.18
			  			left_cont = ColorInterval[strip_num].left_continuation[foll];
			  			right_cont = ColorInterval[strip_num].right_continuation[foll];
			  			beg = ColorInterval[strip_num].ColoredIntervalsStructure->BegInterv[foll];
			  			end = ColorInterval[strip_num].ColoredIntervalsStructure->EndInterv[foll];
			  			length = end - beg + 1;
			  			if (!left_right)
			  			{
			  				direct_cont = left_cont;
			  				if ((beg == 0) || ((beg <= 10) && (!direct_cont)))
			  				{
			  					end_condition = 1;
			  				}
			  				else
			  				{
			  					end_condition = 0;
			  				}
			  			}
			  			else
			  			{
			  				direct_cont = right_cont;
			  				if ((end == (StripLength - 1)) || ((end >= (StripLength - 10)) && (!direct_cont)))
			  				{
			  					end_condition = 1;
			  				}
			  				else
			  				{
			  					end_condition = 0;
			  				}
			  			}

			  			if (!*(status_show + foll))
			  			{
			  				*(status_show + foll) = -1;
			  				if ((blocking == 0) && ((direct_cont<0) || (end_condition)))
			  				{

			  					*new_beg = foll;
			  					return(success_of_seaching = 1);
			  				}
			  				else
			  				{

			  					*(status_show + foll) = -2;
			  				}
			  				/*if((blocking==0)&&((right_cont<0)||(beg==reference_size))
			  				{

			  				*new_beg=foll;
			  				return(success_of_seaching=1);
			  				}
			  				else
			  				{

			  				*(status_show+foll)=-2;
			  				}*/
			  			}
			  		}//bl
			  	}//follloop
			return(success_of_seaching);
			}
		//----------------------------------------------------------------------------
		int

		CColorSection::Continuation_of_section(int prev_corn,int *prolongation_grain,
		int previous_sec,int continuation_sec,int fiber_number)
		{
			int jump_sec;
				int paint_coun;
				int success_of_process = 0;
				int quant;
				int* quantity;
				int* bunches_list;
				int* bunch_blocking;
				int* bunch_visibility;
				int* bunch_new_visibility;
				int* bunch_investigated;
				int* geometric_proximity;
				int* geometric_proximity1;
				int paint_start;
				int paint_finish;
				int PressedLength;
				int cycle_limit;
				int paint_step;
				int intens_cou;
				int point_location;
				int blocking;
				int bunch_new;
				int beg_int;
				int end_int;
				int length_int;
				int beg_int_new;
				int end_int_new;
				int length_int_new;
				int bunch_mean_hue;
				int previous_bunch_mean_hue;
				int bunch_new_mean_hue;
				//int bunch_mean_hue_corrected;
				//int bunch_new_mean_hue_corrected;
				int left_bunch_contrast;
				int right_bunch_contrast;
				int left_bunch_new_contrast;
				int right_bunch_new_contrast;
				int vertex;
				int vertex1;
				int bunch_mean_sat;
				int bunch_new_mean_sat;
				int previous_bunch_mean_sat;
				int bunch_mean_gray;
				int previous_bunch_mean_gray;
				int bunch_new_mean_gray;
				int number_bunches_continuations;
				int bunch_indicator;
				int hue_difference;
				int sat_difference;
				int gray_difference;
				int possible_deviation;
				int coinsidence;
				int coinsidence1;
				int shift_to_left;
				int shift_to_right;
				int coinsidence_adj;
				int coinsidence1_adj;
				int shift_to_left_adj;
				int shift_to_right_adj;
				int signed_shift_to_left;
				int signed_shift_to_right;
				int maximal_coincidence;
				int closest_bunch;
				int second_maximal_coincidence;
				int second_closest_bunch;
				int maximal_coincidence1;
				int closest_bunch1;
				int closest_in_hue_bunch;
				int min_hue_difference;
				int min_sat_difference;
				int hue_closeness;
				int distance_proximity;
				int distance_proximity01;
				int distance_proximity1;
				int distance_proximity11;
				//int difference;
				int closest_bunch_hue_dif;
				int closest_bunch_hue_dif1;
				//int bunch_opt_mean_sat;
				//int bunch_opt_mean_hue;
				//int bunch_opt_mean_gray;
				int bunch_occurrence;
				int bunch_new_occurrence;
				int occurrence_ratio;
				int signif_bunch;
				int signif_bunch_new;
				int bunch_significance_ratio;
				int small_interval;
				int Strwidth;
				int density_bunch;
				int density_bunch_new;
				int density_ratio;
				int Intersection;
				int Intersection_adj;
				int Intersection1;
				int Intersection2;
				int indic_length;
				int ratio_length, ratio_length1;
				int indic_length_adj;
				int ratio_length_adj, ratio_length1_adj;
				int indic1_length;
				int ratio1_length, ratio1_length1;
				int coins, coins1, shift1_to_left, shift1_to_right;
				int coinsen;
				int coinsen2;
				int shift2_to_left;
				int shift2_to_right;
				int indic2_length;
				int ratio2_length;
				int ratio2_length1;
				int hue_close, gray_close, sat_close;
				int hue_close_adj, gray_close_adj, sat_close_adj;
				int test_junction;
				int test_junction_adj;
				int shift_direct;
				int signed_shift_direct;
				int shift_opposite;//last_cor02.03.15
				int new_bunch_contrast;
				int new_bunch_opposite_contrast;
				int bunch_contrast;
				int bunch_opposite_contrast;
				//int next_bunch_opposite_contrast;
				//int next_bunch_opposite_contrast1;
				int bunch_adjacent;
				int bunch_left_adjacent;
				int bunch_opposite_adjacent;
				int bunch_right_adjacent;
				int new_bunch_left_adjacent;
				int new_bunch_right_adjacent;
				int new_bunch_adjacent;
				int new_bunch_opposite_adjacent;
				int hue_zone_difference;
				int count_total;
				int closest_total_left;
				int closest_total_right;
				int min_total_dist_left;
				int min_total_dist_right;
				int count_TJ_1;
				int count_TJ_2;
				int closest_TJ_1_left;
				int closest_TJ_1_left_density_ratio;
				int closest_TJ_1_right;
				int closest_TJ_1_right_density_ratio;
				int min_TJ1_dist_left;
				int min_TJ1_dist_right;
				int closest_TJ_2_left;
				int closest_TJ_2_left_density_ratio;
				int closest_TJ_2_right;
				int closest_TJ_2_right_density_ratio;
				int min_TJ2_dist_left;
				int min_TJ2_dist_right;
				int extended_beg_int;
				int extended_end_int;
				int extended_new_beg_int;
				int extended_new_end_int;
				int adjacent_beg_int;
				int adjacent_end_int;
				int adjacent_new_beg_int;
				int adjacent_new_end_int;
				int gap_extention;
				int new_gap_extention;
				int length_int1;
				int length_int_new1;
				//int opt_TJ1_left;
				//int opt_TJ1_right;
				//int opt_TJ2_left;
				//int opt_TJ2_right;
				int Intersection_optimal1_left;
				int Intersection_optimal2_left;
				int Intersection_optimal1_right;
				int Intersection_optimal2_right;
				int hue_closeness1;
				bool end_condition;
				int big_interval;
				int big_interval_new;
				int total_TJ_right;
				int total_TJ_left;
				int covering_beg;
				int covering_end;
				int total_left_beg;
				int total_left_end;
				int total_right_beg;
				int total_right_end;
				int covering_interval_length;
				int closest_left_intersection;
				int closest_right_intersection;
				int total_lengthTJ1;
				int total_lengthTJ2;
				int second_closest_bunch_hue_dif;
				int second_covering_beg;
				int second_covering_end;
				int previous_maximal_coincidence;
				int previous_closest_bunch;
				int previous_closest_bunch_hue_dif;
				int previous_covering_beg;
				int previous_covering_end;
				int contrast_interval_total_length;
				int covering_interval_length_main;
				int opposite_end_condition;
				int previous_bunch;
				int previous_bunch_beg;
				int previous_bunch_end;
				int previous_fiber;
				int number_of_fibers;
				int cont_grain;
				int leave;
				int leave_first;
				int abs_sat_dif;
				int abs_sat_dif1;
				//int first_fiber;
				//int first_hue;
				int previous_gray_zone;
				int bunch_gray_zone;
				int hue_new;
				int hue_zone_new;
				int sat_new;
				int gray_new;
				int gray_zone_new;
				int max_gray_zone;
				int bunch_mean_zone;
				int small_bunch;
				int small_new;
				int fiber_shift;
				int mean_shift;
				int close_node;
				int close_contrast;
				int height_of_prolongation;//last_cor27.07.15
				int color_coincidence;
				int size_coincidence;
				int length_first_last_ratio;
				int first_strip_ratio;
				int last_strip_ratio;
				int bunch_ordered_number;
				int blocking_prev;
				int pos_gr_var;
				int imp_gr_var;
				int gray_differ;
				int bunch_mean_hue_zone;//last_cor03.02.17
				int strip_length;
				int difference_hue;
				int closest_left_color_coins;//last_cor17.04.17
				int closest_left_color_intersection1;
				int closest_left_density_ratio;
				int closest_left_occurrence_ratio;
				int closest_right_color_coins;
				int closest_right_color_intersection1;
				int closest_right_density_ratio;
				int closest_right_occurrence_ratio;//last_cor17.04.17
				int length_ratio;
				int cont_length_ratio;
				int cont_beg;
				int cont_end;
				int cont_length;
				int hue_cont;
				int difference_hue_cont;//last_cor21.02.18
				int hue_zone_cont;
				int gray_cont;
				int gray_zone_cont;
				int max_gray_zone_cont;
				int sat_cont;

			/*if((previous_sec>(2*Num_Strips)/3)&&(FirstDensity<StripWidth))
			 {
			 success_of_process=0;
				return(success_of_process);
			 }*/
			TIntColored* ColoredIntervalsStructure;
			TIntColored* ColoredIntervalsStructure1;
			difference_hue_cont=-100;//last_cor21.02.18
				hue_zone_cont = -1;
				gray_cont = -1;
				gray_zone_cont = -1;
				max_gray_zone_cont = -1;
				sat_cont = -1;
				closest_left_color_coins = -1;
				closest_left_color_intersection1 = -1;
				closest_left_density_ratio = -1;
				closest_left_occurrence_ratio = -1;
				closest_right_color_coins = -1;
				closest_right_color_intersection1 = -1;
				closest_right_density_ratio = -1;
				closest_right_occurrence_ratio = -1;
				difference_hue = -1;
				bunch_mean_hue_zone = -1;
				strip_length = 0;
				big_interval_new = 0;
				fiber_shift = 0;
				mean_shift = 0;
				success_of_process = 0;//last_cor270213
				previous_gray_zone = -1;
				bunch_gray_zone = -1;
				max_gray_zone = -1;
				gray_new = -1;
				gray_zone_new = -1;
				previous_bunch = -1;
				previous_bunch_beg = -1;
				previous_bunch_end = -1;
				leave = 0;
				shift_opposite = 0;//last_cor0203.15
			Strwidth=ColorInterval[previous_sec].StripWidth;
			//StripLength=ColorInterval[previous_sec].StripLength;

			number_bunches_continuations=
			ColorInterval[continuation_sec].NumberOfColoredIntervals;
			height_of_prolongation=(16*continuation_sec)/Num_Strips;
			if(number_bunches_continuations==0)
			{
			return(success_of_process);
			}
			cont_grain=-1;
			number_of_fibers=fiber_number;
			Intersection_optimal1_left=-1;
			Intersection_optimal2_left=-1;
			Intersection_optimal1_right=-1;
			Intersection_optimal2_right=-1;
			//opt_TJ1_left=-1;
			//opt_TJ1_right=-1;
			hue_close=-2;
			gray_close=-2;
			sat_close=-2;
			test_junction_adj=-2;
			Intersection_adj=-2;
			indic_length_adj=-2;
			ratio_length_adj=-2;
			ratio_length1_adj=-2;
			hue_close_adj=-2;
			gray_close_adj=-2;
			sat_close_adj=-2;
			coinsidence_adj=-2;
			coinsidence1_adj=-2;
			shift_to_left_adj=-2;
			shift_to_right_adj=-2;
			count_total=0;
			min_total_dist_left=1000;
			min_total_dist_right=1000;
			closest_total_left=-1;
			closest_total_right=-1;
			count_TJ_1=0;
			count_TJ_2=0;
			closest_TJ_1_left=-1;
			closest_TJ_1_right=-1;
			min_TJ1_dist_left=1000;
			min_TJ1_dist_right=1000;
			total_lengthTJ1=0;

			closest_TJ_2_left=-1;
			closest_TJ_2_right=-1;
			min_TJ2_dist_left=1000;
			min_TJ2_dist_right=1000;
			total_lengthTJ2=0;
			small_interval=0;
			shift_to_left=-1;
			shift_to_right=-1;
			indic_length=-1;
			Intersection=-2;
			closest_bunch_hue_dif=-1;
			closest_bunch_hue_dif1=-1;
			big_interval=0;
			gap_extention=-1;
			new_gap_extention=-1;
			contrast_interval_total_length=0;
			previous_bunch=-1;
			previous_fiber=-1;
			hue_new=-1;
			hue_zone_new=-1;
			sat_new=-1;
			 max_gray_zone=100;
			bunch_mean_zone=100;
			mean_shift=TotalShift/(continuation_sec-FirstStrip);
			first_strip_ratio=(16*FirstStrip)/(Num_Strips+1);
			last_strip_ratio=(16*continuation_sec)/(Num_Strips+1);
				jump_sec=continuation_sec-previous_sec;
				if(jump_sec>0)
				{//js
					if (number_of_fibers>=2)
					{//>=2
			previous_fiber=previous_sec-1;//!!!if jumps are not available
						if(number_of_fibers==2)
						{
			previous_bunch=FirstGrain;
						}
						else
						{

			previous_bunch=DescrSec[Number_of_sections].location_of_section[previous_fiber];
			if(previous_bunch>0)
			{
			previous_bunch-=1;
			previous_bunch=ColorInterval[previous_fiber].old_bunch_number[previous_bunch];
			ColorInterval[previous_fiber].ColoredIntervalsStructure;
			previous_bunch_beg=
			ColorInterval[previous_fiber].ColoredIntervalsStructure->BegInterv[previous_bunch];
			previous_bunch_end=
			ColorInterval[previous_fiber].ColoredIntervalsStructure->EndInterv[previous_bunch];
			}
						}

					}//>=2
				}//js

			maximal_coincidence=0;
			second_maximal_coincidence=0;
			closest_bunch_hue_dif=-1;
			closest_bunch=-1;
			second_closest_bunch=-1;
			second_closest_bunch_hue_dif=-1;
			maximal_coincidence1=-1;
			closest_bunch1=-1;
			closest_in_hue_bunch=-1;
			min_hue_difference=48;
			min_sat_difference=16;
			covering_interval_length=0;
			covering_beg=-1;
			covering_end=-1;
			opposite_end_condition=-1;
			second_covering_beg=-1;
				  second_covering_end=-1;

			PressedLength=ColorInterval[previous_sec].PressedLength;
			bunches_list=ColorInterval[continuation_sec].bunches_occurred;
			quantity=ColorInterval[continuation_sec].painted_numbers_colored;
			bunch_blocking=ColorInterval[continuation_sec].bunch_blocking;
			bunch_new_visibility=ColorInterval[continuation_sec].visible_bunches;
			bunch_visibility=ColorInterval[previous_sec].visible_bunches;
			ColoredIntervalsStructure=ColorInterval[previous_sec].ColoredIntervalsStructure;
			ColoredIntervalsStructure1=
			ColorInterval[continuation_sec].ColoredIntervalsStructure;
			beg_int=ColoredIntervalsStructure->BegInterv[prev_corn];
			end_int=ColoredIntervalsStructure->EndInterv[prev_corn];
			blocking_prev=bunch_blocking[prev_corn];
			length_int=end_int-beg_int+1;
			strip_length=previous_sec-FirstStrip+1;//last_cor03.02.17
			if(length_int<3*StripWidth)
			{
				small_bunch=1;
			}
			else
			{
				if(length_int<6*StripWidth)
				{//last_cor09.02.16
					small_bunch=2;
				}
				else
				{
				small_bunch=0;
				}//last_cor09.02.16
			}
			signif_bunch=ColoredIntervalsStructure->Significance[prev_corn];
			density_bunch=(2*signif_bunch)/length_int;
			bunch_mean_hue=ColoredIntervalsStructure->AverageHue[prev_corn];
			bunch_mean_hue_zone=hue_zones[bunch_mean_hue];
			if(bunch_mean_hue>=NUM_HUES)
			{
				bunch_mean_hue-=NUM_HUES;
			}
			bunch_mean_sat=ColoredIntervalsStructure->AverageSat[prev_corn];
			bunch_mean_gray=ColoredIntervalsStructure->AverageGray[prev_corn];
			bunch_mean_zone=gray_zones[bunch_mean_gray];
			bunch_occurrence=*(bunch_visibility+prev_corn);
			bunch_ordered_number=ColorInterval[previous_sec].old_ordered_bunch_number[prev_corn];
			pos_gr_var=dev_mean[FirstGray/8];
			imp_gr_var=imp_dev_mean[FirstGray/8];
			if(!bunch_occurrence)
			{
				success_of_process=0;
				return(success_of_process);
			}
			/*if(((!bunch_ordered_number)&&(beg_int>=3*StripWidth)))
			{
				success_of_process=0;
				return(success_of_process);
			}*/
			/*if(blocking_prev)
			{
				success_of_process=0;
				return(success_of_process);
			}*/
			left_bunch_contrast=ColorInterval[previous_sec].left_continuation[prev_corn];
			left_bunch_contrast=left_bunch_contrast%10;
			right_bunch_contrast=ColorInterval[previous_sec].right_continuation[prev_corn];
			right_bunch_contrast=right_bunch_contrast%10;
			bunch_left_adjacent=ColorInterval[previous_sec].left_adjacent[prev_corn];
			bunch_right_adjacent=ColorInterval[previous_sec].right_adjacent[prev_corn];

			if(!LeftRightBoundary)
			{
			bunch_contrast=left_bunch_contrast;
			bunch_opposite_contrast=right_bunch_contrast;
			bunch_adjacent=bunch_left_adjacent;
			bunch_opposite_adjacent=bunch_right_adjacent;
			if(end_int==(StripLength-1))
			{
			opposite_end_condition=0;
			}
			}
			else
			{
			bunch_contrast=right_bunch_contrast;
			bunch_opposite_contrast=left_bunch_contrast;
			bunch_adjacent=bunch_right_adjacent;
			bunch_opposite_adjacent=bunch_left_adjacent;
			if(!beg_int)
			{
			opposite_end_condition=0;
			}
			}

			extended_beg_int=beg_int;
			extended_end_int=end_int;
			if(bunch_contrast>0)
			{
				bunch_contrast-=1;
			}
			else
			{
			if(bunch_contrast<0)
			{
				bunch_contrast+=1;
			}
			}
			if(bunch_opposite_contrast>0)
			{
				bunch_opposite_contrast-=1;
			}
			else
			{
			if(bunch_opposite_contrast<0)
			{
				bunch_opposite_contrast+=1;
			}
			}
			if((bunch_opposite_contrast>0)&&(bunch_opposite_contrast<=2))
			{//boc

			    if(bunch_opposite_adjacent>0)
			    {//ba
				bunch_opposite_adjacent-=1;
				adjacent_beg_int=ColoredIntervalsStructure->BegInterv[bunch_opposite_adjacent];
			    adjacent_end_int=ColoredIntervalsStructure->EndInterv[bunch_opposite_adjacent];
				   if(!LeftRightBoundary)
			       {
					   if(adjacent_beg_int>end_int+1)
					   {
						   gap_extention=adjacent_beg_int-end_int;
					   }
				   }
				   else
				   {
					   if(adjacent_end_int+1<beg_int)
					   {
						   gap_extention=beg_int-adjacent_end_int;
					   }
				   }
				   if((gap_extention<=4)||((bunch_opposite_contrast==1)&&(gap_extention<=StripWidth)))
				   {
				extended_beg_int=std::min(adjacent_beg_int,beg_int);
			    extended_end_int=std::max(adjacent_end_int,end_int);
				   }
			    }//ba

			}//boc
			length_int1=extended_end_int-extended_beg_int+1;
			//if((length_int<=2*Strwidth)&&((bunch_opposite_contrast<0)))
			length_first_last_ratio=(16*length_int1)/(length_int1+FirstLength);
			if(length_int1<=2*Strwidth)
			{
			small_interval=1;
			}
			else
			{
				if(length_int1>((StripLength-1)/8))
				{
			    big_interval=1;
				}
			}
			possible_deviation=dev_mean[bunch_mean_gray/8];
			bunch_investigated=new int[number_bunches_continuations];
			geometric_proximity=new int[number_bunches_continuations];
			geometric_proximity1=new int[number_bunches_continuations];
			memset(bunch_investigated,(int) '\0',sizeof(int)*(number_bunches_continuations));

			vertex=0;
			if((bunch_mean_hue>=45)||(bunch_mean_hue<=2))
			{
			vertex=1;
			}
					if((bunch_mean_hue>=3)&&(bunch_mean_hue<=7))
					 {
					 vertex=2;
					 }

			if((bunch_mean_hue>=8)&&(bunch_mean_hue<=20))
			{
			vertex=3;
			}
				 if((bunch_mean_hue>=21)&&(bunch_mean_hue<=27))
				  {
				  vertex=4;
				  }

						 if((bunch_mean_hue>=28)&&(bunch_mean_hue<=33))
						  {
						  vertex=5;
						  }
							 if((bunch_mean_hue>=34)&&(bunch_mean_hue<=44))
							 {
							 vertex=6;
							 }

			paint_start=beg_int/4;
			paint_finish=end_int/4;
			if(!LeftRightBoundary)
			   {

			if(small_interval)
			{
				paint_start=std::max(0,(paint_start-2));
				paint_finish=std::min(paint_finish+2,(StripLength/4)-2);
			}
			else
			{
				paint_start=std::max(0,(paint_start-1));
				paint_finish=std::min(paint_finish+1,(StripLength/4)-2);
			}
			cycle_limit=(paint_finish-paint_start)/2;
			//cycle_limit=std::min(cycle_limit,Strwidth/2);
			   }
			   else
			   {
				     if(small_interval)
			        {
				    paint_start=std::max(0,(paint_start-2));
				    paint_finish=std::min(paint_finish+2,(StripLength/4)-2);
			        }
					 else
					 {
				      //paint_start=std::max(beg_int/4,paint_finish-Strwidth);
						 paint_start=beg_int/4;
					  paint_finish=std::min(paint_finish+1,(StripLength/4)-2);
					 }
				 cycle_limit=(paint_finish-paint_start)/2;
			   }
			close_node;
			close_contrast;
			for(paint_coun=0;paint_coun<=cycle_limit;paint_coun++)
				 {//c1
					 close_node=0;
			         close_contrast=0;
				 paint_step=paint_start+2*paint_coun;
				 quant=*(quantity+paint_step);
				  for(intens_cou=0;intens_cou<quant;intens_cou++)
					 {//c2
					 point_location=intens_cou*PressedLength+paint_step;
					 bunch_new=*(bunches_list+point_location);
					 if((bunch_new>=number_bunches_continuations)||(bunch_new<0))
					 {
						 goto L;
					 }
					 bunch_indicator=*(bunch_investigated+bunch_new);
					 if(bunch_indicator)
					 {
					 goto L;
					 }
					 blocking=*(bunch_blocking+bunch_new);
						 if(blocking)
						 {
			             *(bunch_investigated+bunch_new)=-1;
						 goto L;
						 }


					 bunch_new_occurrence=*(bunch_new_visibility+bunch_new);
					 if((!bunch_new_occurrence)&&(bunch_occurrence>0)&&((!small_bunch)||((small_bunch==2)&&
						 (bunch_occurrence>=10))))
					 {//last_cor14.12.16
					 *(bunch_investigated+bunch_new)=-1;
			         goto L;
					 }
					 if((bunch_new_occurrence)&&(bunch_occurrence))
					 {//last_cor17.02.16
					 occurrence_ratio=(16*bunch_new_occurrence)/(bunch_new_occurrence+bunch_occurrence);
					 }
					 else
					 {
						 occurrence_ratio=-1;
					 }

					 Intersection2=-1;
					 left_bunch_new_contrast=ColorInterval[continuation_sec].left_continuation[bunch_new];
					 left_bunch_new_contrast=left_bunch_new_contrast%10;
			right_bunch_new_contrast=ColorInterval[continuation_sec].right_continuation[bunch_new];
			right_bunch_new_contrast=right_bunch_new_contrast%10;
			new_bunch_left_adjacent=ColorInterval[continuation_sec].left_adjacent[bunch_new];
			new_bunch_right_adjacent=ColorInterval[continuation_sec].right_adjacent[bunch_new];
			beg_int_new=ColoredIntervalsStructure1->BegInterv[bunch_new];
			end_int_new=ColoredIntervalsStructure1->EndInterv[bunch_new];
			hue_new=ColoredIntervalsStructure1->AverageHue[bunch_new];
			difference_hue=abs(hue_new-bunch_mean_hue);//last_cor03.02.17
			hue_zone_new=hue_zones[hue_new];
			gray_new=ColoredIntervalsStructure1->AverageGray[bunch_new];
			gray_zone_new=gray_zones[gray_new];
			max_gray_zone=std::max(gray_zone_new,bunch_mean_zone);
			sat_new=ColoredIntervalsStructure1->AverageSat[bunch_new];
			gray_differ=abs(gray_new-FirstGray);
			if((FirstHueZone==2)&&(hue_zone_new==3))
			{//last_cor04.05.17
				if((sat_new>=3)&&(gray_zone_new>1)&&(gray_zone_new<=5))
				{
					if(((hue_new>10)&&(FirstHue<=6))||((hue_new>11)))
					{
			*(bunch_investigated+bunch_new)=-1;
							  goto L;
					}
				}
			}//last_cor04.05.17
			if((FirstHueZone==3)&&(hue_zone_new==3))
			{//last_cor04.05.17
				if((sat_new>=3)&&(gray_zone_new>1)&&(gray_zone_new<=5))
				{
					if((hue_new>=11)&&(FirstHue<=9)&&(FirstSat<=2))
					{
			*(bunch_investigated+bunch_new)=-1;
							  goto L;
					}
				}
			}//last_cor04.05.17
			if((FirstSat<=1)&&(bunch_mean_sat<=1)&&(sat_new>=2)&&(FirstHueZone==bunch_mean_hue_zone)&&
			   (FirstHue<=19)&&(max_gray_zone>=2)&&(continuation_sec>=(3*NumStrips/4))&&(strip_length>=4)
			   &&(hue_new>=22)&&(bunch_mean_hue<=19))
			{//last_cor03.02.17
			*(bunch_investigated+bunch_new)=-1;
							  goto L;
			}
			if((FirstSat<=1)&&(sat_new>=3)&&(bunch_mean_hue_zone==3))
			{//last_cor17.04.17
			*(bunch_investigated+bunch_new)=-1;
							  goto L;
			}
			if((gray_zone_new>=3)&&(hue_zone_new==3))
				{//last_cor23.08.16
					if(FirstSat<=1)
					{
						if((sat_new>=4)&&(bunch_mean_sat<=2))
						{
			            *(bunch_investigated+bunch_new)=-1;
							  goto L;
						}
					}
				}//last_cor23.08.16
			if((FirstSat<=2)&&(sat_new<=2))
			{//last_cor28.03.16

				if((gray_zone_new>=6)&&(FirstGrayZone<=2))
				{
					*(bunch_investigated+bunch_new)=-1;
							  goto L;
				}
				if(FirstStrip<=Num_Strips/8)
				{
					if(continuation_sec<=Num_Strips/8)
					{
						if(gray_differ>2*pos_gr_var)
						{
						*(bunch_investigated+bunch_new)=-1;
							  goto L;
						}
					}
				}
			}//last_cor28.03.16
			if((FirstSat>=2)&&(sat_new<=1))
			{//last_cor30.03.16
				if((gray_zone_new>=3)&&(FirstGrayZone<=1))
				{
					*(bunch_investigated+bunch_new)=-1;
							  goto L;
				}
			}//last_cor30.03.16
			if(((sat_new>2)&&(FirstSat>=2))||((sat_new>=2)&&(FirstSat>2)))//onlyub16.05.17
						{//onlyublast_cor16.05.17
			if((((FirstHueZone==4)&&(hue_zone_new==3))||((FirstHueZone==3)&&(hue_zone_new==4)))
								&&(abs(hue_new-FirstHue)>=2))//onlyub16.05.17
							{//last_cor08.03.17
								if((FirstGrayZone>=1)&&(FirstGrayZone<=2)&&(gray_zone_new>=1)&&(gray_zone_new<=2))
								{
									*(bunch_investigated+bunch_new)=-1;
										  goto L;
								}
							}//last_cor08.03.17
						}//onlyublast_cor16.05.17
			if((sat_new>2)&&(FirstSat>2))//onlyub16.05.17
			{
				if(((FirstHue==22)&&(hue_new<=18))||((FirstHue<=18)&&(hue_new==22)))
				{//last_cor08.03.17
					if((FirstGrayZone>=1)&&(FirstGrayZone<=2)&&(gray_zone_new>=1)&&(gray_zone_new<=2))
					{
						*(bunch_investigated+bunch_new)=-1;
							  goto L;
					}
				}//last_cor08.03.17

				    if(((FirstHueZone==1)||(hue_zone_new==1))&&(abs(hue_new-FirstHue)>2))
				    {
				if((hue_zone_new!=FirstHueZone)&&(gray_zone_new>=3))
				{
			    *(bunch_investigated+bunch_new)=-1;
							  goto L;
				}
					}
			}
			length_int_new=end_int_new-beg_int_new+1;
			if(length_int>2*StripWidth)
			{//last_cor24.01.17
				if((bunch_occurrence>10)&&(!bunch_new_occurrence))
				{
				*(bunch_investigated+bunch_new)=-1;
							  goto L;
				}

			}
			if(length_int_new<3*StripWidth)
			{
				small_new=1;
			}
			else
			{//last_cor09.02.16
				if(length_int_new<6*StripWidth)
				{
			small_new=2;
				}
				else
				{
				small_new=0;
				}
			}
			if(length_int_new>(StripLength-1)/8)
				{
			    big_interval_new=1;
				}
			else
			{
			big_interval_new=0;
			}
			signif_bunch_new=ColoredIntervalsStructure1->Significance[bunch_new];
			  density_bunch_new=(2*signif_bunch_new)/length_int_new;
			  density_ratio=(16*density_bunch_new)/(density_bunch_new+density_bunch);
			 bunch_significance_ratio=(16*signif_bunch_new)/(signif_bunch+signif_bunch_new);
			if((bunch_occurrence>12)&&(bunch_new_occurrence<5)&&(density_ratio<6))
				{//last_cor24.03.17
				*(bunch_investigated+bunch_new)=-1;
							  goto L;
				}
			if(!LeftRightBoundary)
			{
			new_bunch_contrast=left_bunch_new_contrast;
			new_bunch_opposite_contrast=right_bunch_new_contrast;
			new_bunch_adjacent=new_bunch_left_adjacent;
			new_bunch_opposite_adjacent=new_bunch_right_adjacent;
			    if(beg_int_new>0)
			    {
				end_condition=1;
			    }
				else
				{
			     end_condition=0;
				}
			}
			else
			{//e_left_sect
			new_bunch_contrast=right_bunch_new_contrast;
			new_bunch_opposite_contrast=left_bunch_new_contrast;
			new_bunch_adjacent=new_bunch_right_adjacent;
			new_bunch_opposite_adjacent=new_bunch_left_adjacent;
			if(end_int_new==(StripLength-1))//error!14.05.14
			    {
				end_condition=0;
			    }
				else
				{
			     end_condition=1;
				}
			}//e_left_sect/error!14.05.14

			if(new_bunch_contrast<0)
			{
				new_bunch_contrast+=1;
			}
			else
			{//enbc
				if(new_bunch_contrast>0)
			{
				new_bunch_contrast-=1;
			}
			}//enbc
			if(new_bunch_opposite_contrast<0)
			{
				new_bunch_opposite_contrast+=1;
			}
			else
			{//enoc
				if(new_bunch_opposite_contrast>0)
			{
				new_bunch_opposite_contrast-=1;
			}
			}//enoc

			color_coincidence=0;
			size_coincidence=0;


			        extended_new_beg_int=beg_int_new;
			        extended_new_end_int=end_int_new;

			if((new_bunch_opposite_contrast>0)&&(new_bunch_opposite_contrast<=2))
			{//boclast_cor23.04.15
			     if(new_bunch_opposite_adjacent>0)
			     {//bna
			     new_bunch_opposite_adjacent-=1;
				 adjacent_new_beg_int=ColoredIntervalsStructure1->BegInterv[new_bunch_opposite_adjacent];
				 adjacent_new_end_int=ColoredIntervalsStructure1->EndInterv[new_bunch_opposite_adjacent];
				 if(!LeftRightBoundary)
			       {
					   if(adjacent_new_beg_int>end_int_new+1)
					   {
						   new_gap_extention=adjacent_new_beg_int-end_int_new;
					   }
				   }
				   else
				   {
					   if(adjacent_new_end_int+1<beg_int_new)
					   {
						   new_gap_extention=beg_int_new-adjacent_new_end_int;
				       }
				   }
				   if((new_gap_extention<=4)||((new_gap_extention<=StripWidth)
					   &&(new_bunch_opposite_contrast==1)))
				   {
				extended_new_beg_int=std::min(adjacent_new_beg_int,beg_int_new);
			    extended_new_end_int=std::max(adjacent_new_end_int,end_int_new);
				   }

				}//bna

			}//boc
			length_int_new1=extended_new_end_int-extended_new_beg_int+1;
			Intersection=Measure1_of_proximity(extended_beg_int,extended_end_int,extended_new_beg_int,
			extended_new_end_int,&coinsidence,&coinsidence1,&shift_to_left,&shift_to_right,&indic_length,
			&ratio_length,&ratio_length1);
			if((occurrence_ratio<=0)&&(bunch_occurrence>0)&&(small_bunch!=1))
			{//last_cor17.02.16
				if(density_ratio<8)
				{
			if(ratio_length<=4)
			{
				*(bunch_investigated+bunch_new)=-1;
							  goto L;
			}

				}
			}
			if(density_ratio<=5)
			{//last_cor14.03.16
				if((occurrence_ratio<5)&&(bunch_significance_ratio<5)&&((!small_bunch)||(!small_new)))
				{
				*(bunch_investigated+bunch_new)=-1;
							  goto L;
				}
				if((!sat_new)&&(FirstSat>=2)&&(Intersection>0))
				{
			if((abs(FirstGrayZone-gray_zone_new)>1)||(abs(FirstGray-gray_new)>14))
			{
			*(bunch_investigated+bunch_new)=-1;
							  goto L;
			}
				}
			}//last_cor14.03.16
			else
			{//last_cor31.08.16
				if((density_ratio>=10)&&(occurrence_ratio>=11)&&((!small_bunch)||(!small_new)))
				{
					*(bunch_investigated+bunch_new)=-1;
							  goto L;
				}
			}//last_cor31.08.16
			if((!small_bunch)||(!small_new))
			{//last_cor29.08.15
				if((density_ratio<5)||(density_ratio>=11))
				{//last_cor25.11.15and14.01.16
					*(bunch_investigated+bunch_new)=-1;
							  goto L;
				}//last_cor25.11.15
			if(((ratio_length<=4)||(ratio_length1<=4))&&((density_ratio<=6)||(density_ratio>=10)))
			{//last_cor24.07.15
				*(bunch_investigated+bunch_new)=-1;
							  goto L;
			}
			}
			if(new_bunch_opposite_contrast<=0)
			{
				if((indic_length>=3)&&((!ratio_length)||(Intersection==3)))
				{
					*(bunch_investigated+bunch_new)=-1;
							  goto L;
				}
			}
			if(Intersection==3)
			{
				ratio_length=0;
				ratio_length1=0;
				*(bunch_investigated+bunch_new)=-1;
							  goto L;
			}
			Intersection1=Measure1_of_proximity(beg_int,end_int,beg_int_new,
			end_int_new,&coins,&coins1,&shift1_to_left,&shift1_to_right,&indic1_length,
			&ratio1_length,&ratio1_length1);

			if(((ratio1_length>=12)&&(ratio1_length1>=12))||((ratio1_length>=14)&&(ratio1_length1>=10))||
			((ratio1_length>=10)&&(ratio1_length1>=14))||((ratio_length>=12)&&(ratio_length1>=12))||
			((ratio_length>=14)&&(ratio_length1>=10))||((ratio_length>=10)&&(ratio_length1>=14)))
			{//last_cor29.08.15
				size_coincidence=1;
			}//last_cor29.08.15
			else
			{//last_cor08.02.16
				if(((ratio1_length>=13)&&(ratio1_length1>=8))||((ratio1_length>=8)&&(ratio1_length1>=13))||
					((ratio_length>=13)&&(ratio_length1>=8))||((ratio_length>=8)&&(ratio_length1>=13)))
				{
			size_coincidence=2;
				}
			}//last_cor08.02.16


			if(Intersection1==3)
			{
				ratio1_length=0;
				ratio_length1=0;

			}
			           if(extended_new_beg_int>=extended_beg_int)
			            {
						signed_shift_to_left=shift_to_left;
						}
					   else
					   {
						signed_shift_to_left=-shift_to_left;
					   }
					   if(extended_new_end_int>=extended_end_int)
			            {
						signed_shift_to_right=shift_to_right;
						}
					   else
					   {
						signed_shift_to_right=-shift_to_right;
					   }
					          if(!LeftRightBoundary)
			                  {
			                  shift_direct=shift_to_left;
							  shift_opposite=shift_to_right;
			                  signed_shift_direct=signed_shift_to_left;
							    /*if(signed_shift_direct<0)
							    {
			if(((ratio_length<8)&&(ratio_length1<=5))||((ratio_length<5)&&(ratio_length1<=8)))
			{
				*(bunch_investigated+bunch_new)=-1;
							  goto L;
			}
							    }*/
							  }
							  else
							  {
							  shift_direct=shift_to_right;
			                  shift_opposite=shift_to_left;
			                  signed_shift_direct=signed_shift_to_right;
							  /*if(signed_shift_direct>0)
							    {
			if(((ratio_length<8)&&(ratio_length1<=5))||((ratio_length<5)&&(ratio_length1<=8)))
			{
				*(bunch_investigated+bunch_new)=-1;
							  goto L;
			}
							    }*/
							  }
				 if((bunch_mean_hue_zone==3)&&(ratio_length<=4)&&(bunch_mean_sat<=2)&&(sat_new>=3)
					 &&(big_interval)&&(max_gray_zone>=2)&&(count_TJ_1>0))
				 {//last_cor03.02.17
				 *(bunch_investigated+bunch_new)=-1;
							  goto L;
				 }//last_cor03.02.17
			  if(!size_coincidence)
			{//last_cor29.08.15
			if(((ratio_length>=13)&&(ratio_length1>=13))||((ratio_length>=15)&&(ratio_length1>=10))||
			((ratio_length>=10)&&(ratio_length1>=15))||((ratio_length>=15)&&(shift_direct<=StripWidth)))
			{//last_cor29.08.15
				size_coincidence=1;
			}//last_cor29.08.15
			}//last_cor29.08.15
							  if(shift_direct<2*StripWidth)
							  {
							  close_node=1;
							  }
			test_junction=Test_Junction_Different_Strips_Same_Image(bunch_new,prev_corn,Intersection,indic_length,ratio_length,
			ratio_length1,continuation_sec,previous_sec,&hue_close,&sat_close,&gray_close);
			test_junction=test_junction%10;
			if(((hue_close==1)&&(sat_close==1)&&(gray_close==1))||
			((test_junction==1)&&(sat_close==1)&&(gray_close==1)&&(!hue_close)))
			{
			color_coincidence=1;
			}
			if(signed_shift_direct<0)
			{//last_cor15.02.17
				if((ratio_length<6)&&(shift_direct>length_int/2)&&(shift_direct>6*StripWidth))
				{
			*(bunch_investigated+bunch_new)=test_junction;
							  goto L;
				}
			}

				if(previous_bunch_beg>=0)
				{
			Intersection2=Measure1_of_proximity(previous_bunch_beg,previous_bunch_end,extended_new_beg_int,
			extended_new_end_int,&coinsen,&coinsen2,&shift2_to_left,&shift2_to_right,&indic2_length,
			&ratio2_length,&ratio2_length1);
			if((big_interval_new==1)&&(big_interval==1))
			{
				if(((Intersection1>1)&&(!color_coincidence))&&((Intersection2==3)||((Intersection2==2)&&((ratio2_length<=2)||(ratio2_length1<=2)))))
							{//last_cor13.04.17
						*(bunch_investigated+bunch_new)=test_junction;
										  goto L;
							}
			}
				}
			if((!small_bunch)&&(!small_new))
			{//last_cor18.06.15&09.02.16
				if(length_first_last_ratio<=4)
			{//last_cor27.11.15
				if(first_strip_ratio<=6)
				{
					if(last_strip_ratio>=9)
					{
						if(ratio_length1<=6)
						{
			*(bunch_investigated+bunch_new)=test_junction;
							  goto L;
						}

					}
				}
			}//last_cor27.11.15
				if((ratio1_length<=6)&&(ratio1_length1<=6)&&(shift_direct>5*StripWidth))
				{
			*(bunch_investigated+bunch_new)=test_junction;
							  goto L;
				}
				if((!color_coincidence)&&(sat_new<=3))
				{//last_cor26.11.15
			if(((ratio_length<=9)&&(ratio_length1<=4))||((ratio_length1<=9)&&(ratio_length<=4)))
			{
				*(bunch_investigated+bunch_new)=test_junction;
							  goto L;
			}

				}//last_cor26.11.15
			}//last_cor18.06.15
			if(!HorizontalVertical)
			{//last_cor27.07.15
				        if(height_of_prolongation>=12)
				         {
			        if((new_bunch_opposite_contrast<0)&&(bunch_opposite_contrast<0))
					{
				if(ratio1_length1<=6)
				{
					if(((!color_coincidence)&&(sat_new<=3))
						||((!size_coincidence)&&(!small_bunch)&&(!small_new)))
					{//last_cor01.10.15&09.02.16
			     *(bunch_investigated+bunch_new)=test_junction;
				 goto L;
					}//last_cor01.10.15
				}
					}
						}
			}//last_cor27.07.15

			if((((hue_close<=0)&&(test_junction>1))||(test_junction>2))&&(new_bunch_opposite_contrast<0)&&(bunch_opposite_contrast<=0)&&
			   ((ratio_length<=5)||(ratio_length1<=5)))
			{
				*(bunch_investigated+bunch_new)=-1;
							  goto L;
			}
			if(test_junction==2)
			{//last_cor02.04.14
				if(shift_direct<=3)
				{
					if(((ratio_length1>=14)&&(ratio_length>=6))||((ratio_length1>=6)&&(ratio_length>=14)))
					{
						test_junction=1;
					}
				}

			}//last_cor02.04.14
			else
			{
			if(test_junction==3)
			{
				if((sat_close==1)&&(gray_close==1)&&(hue_close>0))
				{
			if((ratio_length1>=15)&&(ratio_length>=15))
			{
				if((density_ratio>=7)&&(density_ratio<=9))
				{
			     test_junction=2;
				}
			}
				}
			}
			}
			if((test_junction>4)&&(test_junction<=8))
			{
			*(bunch_investigated+bunch_new)=test_junction;
							     goto L;
			}
			if((big_interval_new)||(big_interval))
			{
			if(shift_direct>=StripWidth)
			{//last_cor28.05.15
				if((ratio_length<=2)||(ratio_length1<=2))
				{
			*(bunch_investigated+bunch_new)=test_junction;
							     goto L;
				}
			}//last_cor28.05.15
			}
			if(shift_direct>=12*StripWidth)
			{//last_cor28.04.15
				if((ratio_length<=8)||(ratio_length1<=8))
				{
					if(((!color_coincidence)&&(sat_new<=3))&&(!size_coincidence))
					{//last_cor22.08.16
			*(bunch_investigated+bunch_new)=test_junction;
							     goto L;
					}
				}
			}//last_cor28.04.15
			//if((shift_direct>4*StripWidth)&&((ratio_length<=8)||(ratio_length1<=8))&&(test_junction!=1))
			if((shift_direct>6*StripWidth)&&((ratio_length<=8)&&(ratio_length1<=8)))
			{//last_cor09.06.14and 23.10.1411.12.14
				//if(test_junction>1)
				//{//last_cor02.03.15
				*(bunch_investigated+bunch_new)=test_junction;
							     goto L;
				//}
			}//last_cor09.06.14
			if(new_bunch_opposite_contrast<0)
			{//nboc
			new_bunch_opposite_adjacent=-new_bunch_opposite_adjacent-1;
			adjacent_new_beg_int=ColoredIntervalsStructure1->BegInterv[new_bunch_opposite_adjacent];
			adjacent_new_end_int=ColoredIntervalsStructure1->EndInterv[new_bunch_opposite_adjacent];
			Intersection_adj=Measure1_of_proximity(extended_beg_int,extended_end_int,adjacent_new_beg_int,
			adjacent_new_end_int,&coinsidence_adj,&coinsidence1_adj,&shift_to_left_adj,&shift_to_right_adj,
			&indic_length_adj,&ratio_length_adj,&ratio_length1_adj);
				if((ratio_length<=5)&&((!small_bunch)||(!small_new)))
				{//last_cor29.05.15
			test_junction_adj=
			Test_Junction_Different_Strips_Same_Image(new_bunch_opposite_adjacent,prev_corn,Intersection_adj,
			indic_length_adj,ratio_length_adj,ratio_length1_adj,continuation_sec,previous_sec,&hue_close_adj,
			&sat_close_adj,&gray_close_adj);
			test_junction_adj=test_junction_adj%10;
			if((test_junction_adj>4)&&(test_junction_adj<8))
			{
				if((ratio_length_adj>=4)&&(shift_direct>2*StripWidth)&&(ratio_length1<13)&&(&hue_close_adj<=0))
				{//test this and use the length of the first member!!!
			*(bunch_investigated+bunch_new)=test_junction;
							     goto L;
				}
				if((big_interval)&&(small_new))
				{//last_cor17.03.15
					if(ratio_length<4)
					{
						*(bunch_investigated+bunch_new)=test_junction;
							     goto L;
					}

				}
			}
				}
			}//nboc
			else//last_cor17.03.15
			{
			             if(new_bunch_opposite_contrast<=0)
						 {//last_cor17.03.15
							 if((shift_direct>length_int_new1)&&(ratio_length<8))
							 {
								 *(bunch_investigated+bunch_new)=test_junction;
							     goto L;
							 }
						 }
			}//last_cor17.03.15
			if(test_junction==3)
			{
				if((max_gray_zone<=1)&&(density_ratio>=7)&&(density_ratio<=9))
				{
			if((gray_close==1)&&(sat_close>0))
			{
				if((ratio_length>=13)&&(ratio_length1>=13))
				{
					if(shift_direct<=4)
					{
			    test_junction=2;
					}
				}
			}
				}
			if((ratio_length>=14)&&(ratio_length1>5)&&(density_ratio>6)&&(density_ratio<10)
			   &&(Intersection<=1)&&(shift_direct<=2))
			    {//last_cor22.09.14
				   if((hue_close==1)&&(sat_close==1)&&(gray_close>0)&&(gray_close<=2))
				   {
					   test_junction=2;
				   }
			    }
			}

								 if((Intersection>=0)&&(Intersection<3))
								 {//int
			count_total++;
			if(signed_shift_direct>=0)
			{//sd0
				if(shift_direct<min_total_dist_right)
				{//last_cor17.04.17
			if((min_total_dist_right<4*StripWidth)&&(closest_right_color_coins==1)&&
			(!color_coincidence)&&(occurrence_ratio<closest_right_occurrence_ratio)&&
			(ratio_length1<closest_right_color_intersection1))
					{
			        closest_total_right=closest_total_right;
					}
			else
			{
					closest_total_right=bunch_new;
			       min_total_dist_right=shift_direct;
				   total_TJ_right=test_junction;
				   total_right_beg=extended_new_beg_int;
			       total_right_end=extended_new_end_int;
				  closest_right_intersection=ratio_length;
				  closest_right_color_coins=color_coincidence;
				   closest_right_color_intersection1=ratio1_length;
				   closest_right_density_ratio=density_ratio;
				   closest_right_occurrence_ratio=occurrence_ratio;
			}
				}
			}//sd0
			else
			{//sde0
			if(shift_direct<min_total_dist_left)
				{//last_cor17.04.17
					if((min_total_dist_left<4*StripWidth)&&(closest_left_color_coins==1)&&
			(!color_coincidence)&&(occurrence_ratio<=closest_left_occurrence_ratio)&&
			(ratio1_length<closest_left_color_intersection1))
					{
			        closest_total_left=closest_total_left;
					}
					else
					{
					closest_total_left=bunch_new;
			       min_total_dist_left=shift_direct;
				   total_TJ_left=test_junction;
				   total_left_beg=extended_new_beg_int;
			       total_left_end=extended_new_end_int;
				   closest_left_intersection=ratio_length;
				   closest_left_color_coins=color_coincidence;
				   closest_left_color_intersection1=ratio1_length;
				   closest_left_density_ratio=density_ratio;
				   closest_left_occurrence_ratio=occurrence_ratio;
					}
				}
			}//sde0
								 }//int
			if((count_TJ_1>0)&&(test_junction>1)&&(hue_close>2)&&(density_ratio<7))
			{
			*(bunch_investigated+bunch_new)=test_junction;
			 goto L;
			}

								 if((((bunch_new_occurrence>0)&&(density_ratio>5)&&(density_ratio<14))||
									 (bunch_new_occurrence>=8)||((density_ratio>6)&&(density_ratio<14)))||
									 ((test_junction==1)&&(hue_close==1)&&(sat_close==1)&&
									 (gray_close==1)&&(ratio1_length>=10)&&(ratio1_length1>=10)
									 &&(density_ratio>=5)&&(density_ratio<=9)))
								 {//last_cor04.12.14
									 if(test_junction==1)
									 {

									 count_TJ_1++;
			                         total_lengthTJ1+=ratio1_length;
									 }
									 if(test_junction==2)
									 {
									 count_TJ_2++;
			                         total_lengthTJ2+=ratio1_length;
									 }
									 if((test_junction>=5)&&(test_junction<=7)&&(ratio1_length>0))
									 {
									 contrast_interval_total_length+=ratio1_length;
									 }
								  if((ratio1_length>maximal_coincidence)&&(ratio1_length>0))
				  {
					  previous_maximal_coincidence=maximal_coincidence;
					  previous_closest_bunch=closest_bunch;
					  previous_closest_bunch_hue_dif=closest_bunch_hue_dif;
					  previous_covering_beg=covering_beg;
					  previous_covering_end=covering_end;
				  maximal_coincidence=ratio1_length;
				  closest_bunch=bunch_new;
				  closest_bunch_hue_dif=test_junction;
				  covering_beg=extended_new_beg_int;
				  covering_end=extended_new_end_int;
			if((previous_maximal_coincidence>second_maximal_coincidence)&&(previous_closest_bunch>=0))
				  {
				  second_maximal_coincidence=previous_maximal_coincidence;
				  second_closest_bunch=previous_closest_bunch;
				  second_closest_bunch_hue_dif=previous_closest_bunch_hue_dif;
				  second_covering_beg=previous_covering_beg;
				  second_covering_end=previous_covering_end;
				  }
				  }
								  else
								  {
									  if(ratio1_length>second_maximal_coincidence)
				  {
				  second_maximal_coincidence=ratio1_length;
				  second_closest_bunch=bunch_new;
				  second_closest_bunch_hue_dif=test_junction;
				  second_covering_beg=extended_new_beg_int;
				  second_covering_end=extended_new_end_int;
				  }
								  }
								 if(ratio_length1>maximal_coincidence1)
								 {
								 maximal_coincidence1=ratio_length1;
								 closest_bunch1=bunch_new;
								 closest_bunch_hue_dif1=test_junction;
								 }
								 }

								 if((density_ratio<4)&&(test_junction!=1))
								 {
								 *(bunch_investigated+bunch_new)=test_junction;
							     goto L;
								 }
			 if(((bunch_contrast<0)||(!bunch_contrast))&&((new_bunch_contrast<0)||(!new_bunch_contrast)))
							    {
									if(test_junction>1)
									{
								  if((shift_direct>4*Strwidth)&&(shift_direct>length_int/4)&&
									  ((ratio_length<8)||(ratio_length1<8))&&(shift_opposite>2*Strwidth))
								 {
								 *(bunch_investigated+bunch_new)=test_junction;
							     goto L;
								 }
									}
							    }
						 if(signed_shift_direct<0)
						  {
							  if((bunch_contrast<0)&&(bunch_opposite_contrast<0))
				             {
								 if(shift_direct>length_int)
								 {
									 if(((!color_coincidence)&&(sat_new<=3))&&(!size_coincidence))
									 {//last_cor22.08.16
								 *(bunch_investigated+bunch_new)=test_junction;
							     goto L;
									 }
								 }

							 }


						  }
			             if(signed_shift_direct>0)
						  {
							  if((bunch_contrast<0)&&(bunch_opposite_contrast<=0))
				             {
								 if(shift_direct>length_int)
								 {
								 *(bunch_investigated+bunch_new)=test_junction;
							     goto L;
								 }
								 else
								 {//last_cor17.03.15
									 if((shift_direct>(length_int/2))&&(!small_interval))
									 {
				 if((ratio1_length<6)&&(ratio1_length1<6))
				 {
			*(bunch_investigated+bunch_new)=test_junction;
							     goto L;
				 }
									 }
								 }//last_cor17.03.15

							 }
			if((bunch_contrast>=0)&&(bunch_opposite_contrast<=0)&&(big_interval))
				{
			if(shift_direct>length_int1/2)
								 {
								 *(bunch_investigated+bunch_new)=test_junction;
							     goto L;
								 }
			    }

						  }
						 if(signed_shift_direct<0)
						  {
							  if((new_bunch_contrast<0)&&(new_bunch_opposite_contrast<0))
				             {
								 if(shift_direct>length_int)
								 {
									 if((!small_bunch)&&(!small_new)&&(!color_coincidence)&&(sat_new<=3))
									 {
								 *(bunch_investigated+bunch_new)=test_junction;
							     goto L;
									 }
								 }

							 }
			if((bunch_contrast>=0)&&(bunch_opposite_contrast<=0)&&(big_interval))
				{
			//if(shift_direct>std::max(length_int1/2,length_int_new1/2))
					if(test_junction>=2)
					{
			if(shift_direct>length_int1/2)
								 {
								 *(bunch_investigated+bunch_new)=test_junction;
							     goto L;
								 }
					}
			    }

						  }
			if(!LeftRightBoundary)
			                  {
			                    if(signed_shift_direct<0)
							    {
			if((((ratio_length<3)&&(ratio_length1<3))||((ratio_length<3)&&(ratio_length1<3)))&&
			   (Intersection1==3))
			{
				*(bunch_investigated+bunch_new)=test_junction;
							  goto L;
			}
							    }
							  }
							  else
							  {
							    if(signed_shift_direct>0)
							    {
			if((((ratio_length<3)&&(ratio_length1<3))||((ratio_length<3)&&(ratio_length1<3)))&&
			   (Intersection1==3))
			{
				*(bunch_investigated+bunch_new)=test_junction;
							  goto L;
			}
							    }
							  }
				 if((Intersection==3)||((Intersection>1)&&(Intersection1==3)&&((ratio_length1<3)||(ratio_length<3))))
				  {
				 *(bunch_investigated+bunch_new)=test_junction;
							  goto L;
				  }
				       if(new_bunch_opposite_contrast<=0)
				         {//last_cor19.03.15
				 if(ratio_length<=2)
					 {
						 if((!small_new)||(!small_bunch))
						 {
				 *(bunch_investigated+bunch_new)=test_junction;
							  goto L;
						 }
				     }
				 if(big_interval)
				 {//last_cor19.03.15
			         if((ratio1_length<5)&&(!close_node)&&((!color_coincidence)&&(sat_new<=3)))
					 {//last_cor30.03.16
			*(bunch_investigated+bunch_new)=test_junction;
							  goto L;
				     }
				 }//last_cor19.03.15

					   }//last_cor19.03.15

					   if(bunch_opposite_contrast<=0)
					   {//last_cor19.03.15
						   if(big_interval_new)
				           {//last_cor19.03.15
			               if(ratio_length1<5)
					       {
			               *(bunch_investigated+bunch_new)=test_junction;
							  goto L;
						   }

				         if(shift_direct>length_int)
						 {
							 if((!small_bunch)&&(!small_new))
							 {
								 if((!color_coincidence)&&(sat_new<=3))
								 {//last_cor22.08.16
							 *(bunch_investigated+bunch_new)=test_junction;
							  goto L;
							     }
							 }
						 }
						   }//last_cor19.03.15
					   }//last_cor19.03.15


				 if((ratio_length1<=2)&&(bunch_opposite_contrast<0))
					 {
						 if((!small_new)||(!small_bunch))
						 {
				 *(bunch_investigated+bunch_new)=test_junction;
							  goto L;
						 }
				  }
			if(((ratio_length<5)&&(ratio_length1<8)||((ratio_length<8)&&(ratio_length1<5)))
			   &&(bunch_opposite_contrast<0)&&(new_bunch_opposite_contrast<0))
					 {
						 if((!small_new)||(!small_bunch))
						 {
				 *(bunch_investigated+bunch_new)=test_junction;
							  goto L;
						 }
				  }
			/*if((new_bunch_contrast<0)&&(bunch_contrast<0)&&(new_bunch_opposite_contrast<0)&&
					(bunch_opposite_contrast<0))
				{
					if((shift_direct>Strwidth/2)&&((ratio1_length>=12)&&(ratio1_length1<5))||((ratio1_length<5)&&(ratio1_length1>=12)))
					{
						*(bunch_investigated+bunch_new)=test_junction;
							  goto L;
					}
				}*/
			if((shift_direct>length_int_new)||(shift_direct>length_int))
					{
						if(test_junction!=1)
						{//last_cor16.12.14
						*(bunch_investigated+bunch_new)=test_junction;
							  goto L;
						}
					}
					  if((new_bunch_contrast>1)&&(new_bunch_contrast<=3))
			         {//last_cor14.11.14
						 if((((beg_int_new>StripWidth)||(end_int_new<(StripLength-StripWidth)))
							 &&(test_junction!=1))||((ratio_length<10)&&(ratio1_length<10)))
						 {//last_cor29.08.15
							 if((!small_new)||(!small_bunch))
						 {
							 if((!size_coincidence)||((!color_coincidence)&&(sat_new<=3)))
							 {
				     *(bunch_investigated+bunch_new)=test_junction;
									goto L;
							 }
							 }
						 }
			        }
					  else
					  {
						  if(new_bunch_contrast==4)
						  {
			if(((ratio_length<10)&&(ratio1_length<10)&&(ratio1_length1<10))||(occurrence_ratio<4)||(bunch_contrast>0))
					{//last_cor02.02.17
						if((!small_new)||(!small_bunch))
						 {//last_cor16.12.14
			if((!color_coincidence)&&(sat_new<=2))
							 {
				     *(bunch_investigated+bunch_new)=test_junction;
									goto L;
							 }
						}
					}
						  }
					  }
			if(!new_bunch_contrast)
			{
				if(end_condition)
				{
					if(((ratio_length<10)&&(ratio_length1<10))||(occurrence_ratio<4))
					{
						if((!small_new)||(!small_bunch))
						 {
			*(bunch_investigated+bunch_new)=test_junction;
									goto L;
						}
					}
				}
				else
				{
					if(((ratio_length<10)&&(ratio_length1<10))||((occurrence_ratio<4)&&(bunch_contrast>0)))
					{//last_cor14.11.14
						if((!small_new)||(!small_bunch))
						 {
			*(bunch_investigated+bunch_new)=test_junction;
									goto L;
						}
					}
				}
			}
						//if(!blocking)
						  //{//bl
						  vertex1=0;
					 bunch_new_mean_hue=ColoredIntervalsStructure1->AverageHue[bunch_new];
					 if(bunch_new_mean_hue>=NUM_HUES)
			         {
				      bunch_new_mean_hue-=NUM_HUES;
			         }
					 if((bunch_new_mean_hue>=45)||(bunch_new_mean_hue<=2))
						{
						vertex1=1;
						}
						if((bunch_new_mean_hue>=3)&&(bunch_new_mean_hue<=7))
						{
						vertex1=2;
						}
							if((bunch_new_mean_hue>=8)&&(bunch_new_mean_hue<=20))
								{
								vertex1=3;
								}
								if((bunch_new_mean_hue>=21)&&(bunch_new_mean_hue<=27))
								{
								vertex1=4;
								}
								 if((bunch_new_mean_hue>=28)&&(bunch_new_mean_hue<=33))
								  {
								  vertex1=5;
								  }
								  if((bunch_new_mean_hue>=35)&&(bunch_new_mean_hue<=44))
								  {
								  vertex1=6;
								  }
			hue_difference=
			abs(bunch_mean_hue-bunch_new_mean_hue);
			hue_difference=std::min(hue_difference,NUM_HUES-hue_difference);

			hue_zone_difference=abs(vertex-vertex1);
			hue_zone_difference=std::min(hue_zone_difference,7-hue_zone_difference);

			  bunch_new_mean_sat=ColoredIntervalsStructure1->AverageSat[bunch_new];
			  bunch_new_mean_gray=ColoredIntervalsStructure1->AverageGray[bunch_new];


			  sat_difference=abs(bunch_new_mean_sat-bunch_mean_sat);
			  gray_difference=abs(bunch_new_mean_gray-bunch_mean_gray);

			  /*if((new_bunch_contrast>=0)&&(beg_int_new>0))
			  {
				  *(bunch_investigated+bunch_new)=-1;
									goto L;
			  }*/
			 /* if(hue_zone_difference<=1)
			  {//hzd
			if((hue_difference<=1)&&((sat_difference<=1)))
			{
			*(bunch_investigated+bunch_new)=5;
			}
				else
				{//e1
				if(((hue_difference<=1)&&(sat_difference<=3))||
				((hue_difference<=2)&&(sat_difference<=2)))
				{
				*(bunch_investigated+bunch_new)=4;
				}
				  else
				  {//e2
					if((hue_difference<=3)&&((sat_difference<=5)))
					{
					*(bunch_investigated+bunch_new)=3;
					}
					else
					{
			      *(bunch_investigated+bunch_new)=2;
					}
				  }//e2
			     }//e1
			  }//hzd*/


			/*Intersection1=ColorInterval[previous_sec].Intersection_measure_ratios(extended_beg_int,
			extended_end_int,extended_new_beg_int,extended_new_end_int,&indic_length,&ratio_length,
			&ratio_length1);*/
			/*test_junction=Test_Junction_Different_Strips_Same_Image(bunch_new,prev_corn,Intersection,indic_length,ratio_length,
			ratio_length1,continuation_sec,previous_sec,&hue_close,&sat_close,&gray_close);
			test_junction=test_junction%10;
			if(test_junction==3)
			{   if(ratio_length>=14)
			    {
				   if((hue_close==1)&&(sat_close==1)&&(gray_close>0)&&(gray_close<=3))
				   {
					   test_junction=2;
				   }
			    }
			}*/
			*(bunch_investigated+bunch_new)=test_junction;
			if(test_junction==3)
			{//tj3
				if((hue_close==1)&&(sat_close==1)&&(bunch_new_mean_sat>=3)&&(gray_close>0)&&(gray_close<=2))
				{//c==1last_cor27.09.13
					if((new_bunch_opposite_contrast<0)&&(bunch_opposite_contrast<0))
					{//oc
						if(small_interval)
						{
							if((ratio_length>=8)&&(ratio_length1>=8))
						   {
			               test_junction=2;
						   }
						}
						else
						{
						   if((ratio_length>=12)&&(ratio_length1>=12))
						   {
			               test_junction=2;
						   }
						}

					}

				}
			}


			if(test_junction==1)
			{//tj1
			   if((hue_close==1)&&(sat_close==1)&&(gray_close==1))
			   {//hcscgc


			         *(bunch_investigated+bunch_new)=11;
				        //success_of_process=1;
				       //*prolongation_grain=bunch_new;
					    //goto M;




			   }//hcscgc

			/*count_TJ_1++;
			total_lengthTJ1+=ratio_length;*/
			//if((Intersection>=0)&&(Intersection<3))
			//{
			if(signed_shift_direct>=0)
			{//sd
				if(density_ratio>=2)
					{//dr2
				if(shift_direct<min_TJ1_dist_right)
				{//shd
					if(closest_TJ_1_right<0)
					{//last_cor16.12.14
						closest_TJ_1_right=bunch_new;
			       min_TJ1_dist_right=shift_direct;
				   Intersection_optimal1_right=Intersection;
				   closest_TJ_1_right_density_ratio=density_ratio;
				  // opt_TJ1_right=test_junction;
					}
					else
					{

					if(density_ratio>=4)
					{
					closest_TJ_1_right=bunch_new;
			       min_TJ1_dist_right=shift_direct;
				   Intersection_optimal1_right=Intersection;
				   closest_TJ_1_right_density_ratio=density_ratio;
				  // opt_TJ1_right=test_junction;
					}
					}

				}//shd
				}//dr2
			}//sd
			else
			{//sde
				if(density_ratio>=2)
					{//dr216.12.14
			if(shift_direct<min_TJ1_dist_left)
				{//shdr
					if(closest_TJ_1_left<0)
					{
					closest_TJ_1_left=bunch_new;
			       min_TJ1_dist_left=shift_direct;
				   Intersection_optimal1_left=Intersection;
				   closest_TJ_1_left_density_ratio=density_ratio;
					}
					else
					{
					if(density_ratio>=4)
					{
					closest_TJ_1_left=bunch_new;
			       min_TJ1_dist_left=shift_direct;
				   Intersection_optimal1_left=Intersection;
				   closest_TJ_1_left_density_ratio=density_ratio;
				   //opt_TJ1_left=test_junction;
					}
					}
				}//shdr
				}//dr2
			}//sde
			//}
			}//tj1
			else
			{
			   /*if((test_junction==2)||((test_junction==3)&&
				   (ratio_length>=12)&&(ratio_length1>=12)&&(density_ratio>4)&&(density_ratio<11)&&
				   (new_bunch_opposite_contrast<0)&&(bunch_opposite_contrast<0)))*/
			   if(test_junction==2)
			    {//tj2


					/*count_TJ_2++;
					total_lengthTJ2+=ratio_length;*/
			if(signed_shift_direct>=0)
			{//sd
				if(shift_direct<min_TJ2_dist_right)
				{

					if(density_ratio>=4)
					{
					closest_TJ_2_right=bunch_new;
			       min_TJ2_dist_right=shift_direct;
				   Intersection_optimal2_right=Intersection;
				   closest_TJ_2_right_density_ratio=density_ratio;
					}
				}
			}//sd
			else
			{//sde
			if(shift_direct<min_TJ2_dist_left)
				{
					if(density_ratio>=4)
					{
					closest_TJ_2_left=bunch_new;
			       min_TJ2_dist_left=shift_direct;
				   Intersection_optimal2_left=Intersection;
				   closest_TJ_2_left_density_ratio=density_ratio;
					}
				}
			}//sde
			    }//tj2
			}

			/*if((small_interval)&&(length_int<length_int_new)&&(coinsidence1<14))
			{
			*(bunch_investigated+bunch_new)=1;
			  goto L;
			}
			if((coinsidence1<8))
			{
			*(bunch_investigated+bunch_new)=1;
			  goto L;
			}*/
			/*if((closest_bunch>=0)||(closest_in_hue_bunch>=0)||(closest_bunch1>=0))
			{//feat3

			}//feat3
			else
			{//ef3
				if(small_interval)
				{
				if(shift_direct>(3*Strwidth))
			     {
			     *(bunch_investigated+bunch_new)=1;
			     goto L;
			     }
				}
			}//ef3*/
			/*if(shift_to_right>Strwidth/2)
			{
			*(bunch_investigated+bunch_new)=1;
			  goto L;
			}*/
			//if(!ratio_length1)
			//{
			//*(geometric_proximity+bunch_new)=coinsidence;
			//}
			//else
			//{
			*(geometric_proximity+bunch_new)=ratio_length;
			//}
			*(geometric_proximity1+bunch_new)=ratio_length1;
			if(hue_difference<min_hue_difference)
			{
			min_hue_difference=hue_difference;
			min_sat_difference=sat_difference;
			closest_in_hue_bunch=bunch_new;
			}
			else
			{
			  if(hue_difference==min_hue_difference)
			  {
				 if(sat_difference<min_sat_difference)
				  {
				  closest_in_hue_bunch=bunch_new;
			     min_sat_difference=sat_difference;
				  }
			  }
			}
				  /*if(coinsidence>maximal_coincidence)
				  {
				  maximal_coincidence=coinsidence;
				  closest_bunch=bunch_new;
				  closest_bunch_hue_dif=hue_difference;
				  }
								 if(coinsidence1>maximal_coincidence1)
								 {
								 maximal_coincidence1=coinsidence1;
								 closest_bunch1=bunch_new;
								 closest_bunch_hue_dif1=hue_difference;
								 }*/

						  //}//bl
						  L:;
					 }//c2
				 }//c1
				                if((closest_TJ_1_left>=0)&&(closest_TJ_1_right<0))
								 {//cond1
								 hue_closeness=*(bunch_investigated+closest_TJ_1_left);
								distance_proximity=*(geometric_proximity+closest_TJ_1_left);
				                distance_proximity1=*(geometric_proximity1+closest_TJ_1_left);
			left_bunch_new_contrast=ColorInterval[continuation_sec].left_continuation[closest_TJ_1_left];
			right_bunch_new_contrast=ColorInterval[continuation_sec].right_continuation[closest_TJ_1_left];
			                 if(!LeftRightBoundary)
			                  {
			                   new_bunch_contrast=left_bunch_new_contrast;
			                  new_bunch_opposite_contrast=right_bunch_new_contrast;
			                  }
			                   else
			                  {
			                  new_bunch_contrast=right_bunch_new_contrast;
			                  new_bunch_opposite_contrast=left_bunch_new_contrast;
			                  }
			new_bunch_contrast=new_bunch_contrast%10;
			new_bunch_opposite_contrast=new_bunch_opposite_contrast%10;
			if((distance_proximity1==0)&&(distance_proximity==0))
			{
			if((closest_TJ_2_left<0)&&(closest_TJ_2_right<0))
			 {
				 		 success_of_process=0;
						         goto M;
			 }
			}
			/*if((distance_proximity1<8)&&(distance_proximity<8))
			{
			if(big_interval)
			{
			 if((closest_TJ_2_left<0)&&(closest_TJ_2_right<0))
			 {
				 if((maximal_coincidence>=30)&&(closest_bunch_hue_dif1>=1)&&(closest_bunch_hue_dif1<=2))
				 {
					 success_of_process=0;
						         goto M;
				 }

			 }
			}
			}*/
			if((distance_proximity1>0)||((Intersection_optimal1_left>=0)&&(Intersection_optimal1_left<3)))
			//for left end
								{//cond2
								 success_of_process=1;
						         *prolongation_grain=closest_TJ_1_left;
								 cont_grain=closest_TJ_1_left;
								 fiber_shift=min_TJ1_dist_left;
						         goto M;
								}//cond2
								else
								{
									if(small_interval&&(new_bunch_opposite_contrast>0)&&
										(new_bunch_opposite_contrast<=3))
									{
										if((count_TJ_1==1)&&(!count_TJ_2))
										{
			                            success_of_process=1;
						               *prolongation_grain=closest_TJ_1_left;
									   cont_grain=closest_TJ_1_left;
									   fiber_shift=min_TJ1_dist_left;
						               goto M;
										}
									}
								}
								 }//cond1
								else
								{//oe1
			                    if((closest_TJ_1_left<0)&&(closest_TJ_1_right>=0))
								 {//cond3
								 hue_closeness=*(bunch_investigated+closest_TJ_1_right);
				                distance_proximity=*(geometric_proximity+closest_TJ_1_right);
								distance_proximity1=*(geometric_proximity1+closest_TJ_1_right);
			left_bunch_new_contrast=ColorInterval[continuation_sec].left_continuation[closest_TJ_1_right];
			right_bunch_new_contrast=ColorInterval[continuation_sec].right_continuation[closest_TJ_1_right];
			if(!LeftRightBoundary)
			                  {
			                   new_bunch_contrast=left_bunch_new_contrast;
			                  new_bunch_opposite_contrast=right_bunch_new_contrast;
			                  }
			                   else
			                  {
			                  new_bunch_contrast=right_bunch_new_contrast;
			                  new_bunch_opposite_contrast=left_bunch_new_contrast;
			                  }
			new_bunch_contrast=new_bunch_contrast%10;
			new_bunch_opposite_contrast=new_bunch_opposite_contrast%10;
			if((distance_proximity1==0)&&(distance_proximity==0))
			{
			if((closest_TJ_2_left<0)&&(closest_TJ_2_right<0))
			 {
				 		 success_of_process=0;
						         goto M;
			 }
			}
			/*if((distance_proximity1<8)&&(distance_proximity<8))
			{
			if(big_interval)
			{
			 if((closest_TJ_2_left<0)&&(closest_TJ_2_right<0))
			 {
				 if((maximal_coincidence1>=30)&&(closest_bunch_hue_dif1>=1)&&(closest_bunch_hue_dif1<=2))
				 {
					 success_of_process=0;
						         goto M;
				 }
			 }
			}
			}*/
			                      success_of_process=1;
						         *prolongation_grain=closest_TJ_1_right;
								 cont_grain=closest_TJ_1_right;
								 fiber_shift=min_TJ1_dist_right;
						         goto M;
								 }//cond3
								else
								{//oe2
			                     if((closest_TJ_1_left>=0)&&(closest_TJ_1_right>=0))
								  {//cond4
			left_bunch_new_contrast=ColorInterval[continuation_sec].right_continuation[closest_TJ_1_left];
			right_bunch_new_contrast=ColorInterval[continuation_sec].right_continuation[closest_TJ_1_right];
			if(!LeftRightBoundary)
			                  {
			                   new_bunch_contrast=left_bunch_new_contrast;
			                  new_bunch_opposite_contrast=right_bunch_new_contrast;
			                  }
			                   else
			                  {
			                  new_bunch_contrast=right_bunch_new_contrast;
			                  new_bunch_opposite_contrast=left_bunch_new_contrast;
			                  }
			new_bunch_contrast=new_bunch_contrast%10;
			new_bunch_opposite_contrast=new_bunch_opposite_contrast%10;
								  hue_closeness=*(bunch_investigated+closest_TJ_1_left);
			                      hue_closeness1=*(bunch_investigated+closest_TJ_1_right);
								  distance_proximity=*(geometric_proximity1+closest_TJ_1_left);
								  distance_proximity01=*(geometric_proximity1+closest_TJ_1_right);
								  distance_proximity1=*(geometric_proximity+closest_TJ_1_left);
								  distance_proximity11=*(geometric_proximity+closest_TJ_1_right);
								  if(hue_closeness%10==1)
								  {//last_cor29.05.15
				  if(min_TJ1_dist_left<min_TJ1_dist_right)
				  {
					  if((min_TJ1_dist_right-min_TJ1_dist_left)>StripWidth)
					  {
			success_of_process=1;
			*prolongation_grain=closest_TJ_1_left;
			cont_grain=closest_TJ_1_left;
			fiber_shift=min_TJ1_dist_left;
			  goto M;
					  }
				  }

								  }//last_cor29.05.15
								  if(hue_closeness1%10==1)
								  {//last_cor29.05.15
				  if(min_TJ1_dist_right<min_TJ1_dist_left)
				  {
					  if((min_TJ1_dist_left-min_TJ1_dist_right)>StripWidth)
					  {
			success_of_process=1;
			*prolongation_grain=closest_TJ_1_right;
			cont_grain=closest_TJ_1_right;
			fiber_shift=min_TJ1_dist_right;
			  goto M;
					  }
				  }

								  }//last_cor29.05.15
					 if((min_TJ1_dist_left<=StripWidth)&&(min_TJ1_dist_right>=3*StripWidth))
					 {//last_cor04.12.14
						 if(hue_closeness%10<=hue_closeness1%10)
						 {
			if(((distance_proximity>=10)&&(distance_proximity1>=14))||
			   ((distance_proximity>=14)&&(distance_proximity1>=10)))
			{
			success_of_process=1;
			*prolongation_grain=closest_TJ_1_left;
			cont_grain=closest_TJ_1_left;
			fiber_shift=min_TJ1_dist_left;
				        goto M;
			}
						 }
					 }//last_cor04.12.14
					 if((min_TJ1_dist_right<=StripWidth)&&(min_TJ1_dist_left>=3*StripWidth))
					 {//last_cor04.12.14
						 if(hue_closeness1%10<=hue_closeness%10)
						 {
			if(((distance_proximity01>=10)&&(distance_proximity11>=14))||
			   ((distance_proximity01>=14)&&(distance_proximity11>=10)))
			{
			success_of_process=1;
			*prolongation_grain=closest_TJ_1_right;
			cont_grain=closest_TJ_1_right;
			fiber_shift=min_TJ1_dist_right;
				        goto M;
			}
						 }
					 }//last_cor04.12.14
			if((distance_proximity1)&&(distance_proximity01))
			{//cond5
				    //if(new_bunch_contrast<0)
					//{
				if(distance_proximity11>distance_proximity1)
				{
				success_of_process=1;
				*prolongation_grain=closest_TJ_1_right;
				cont_grain=closest_TJ_1_right;
				fiber_shift=min_TJ1_dist_right;
				goto M;
				}
				else
				{
					//}

					//if(new_bunch_opposite_contrast<0)
					//{
			           if(distance_proximity11<distance_proximity1)
				       {
				        success_of_process=1;
				        *prolongation_grain=closest_TJ_1_left;
						cont_grain=closest_TJ_1_left;
						fiber_shift=min_TJ1_dist_left;
				        goto M;
				        }
					   else
					   {
						   if(min_TJ1_dist_right<min_TJ1_dist_left)
						   {
							   success_of_process=1;
				              *prolongation_grain=closest_TJ_1_right;
							  cont_grain=closest_TJ_1_right;
							  fiber_shift=min_TJ1_dist_right;
				               goto M;
						   }
						   else
						   {
						success_of_process=1;
				        *prolongation_grain=closest_TJ_1_left;
						cont_grain=closest_TJ_1_left;
			            fiber_shift=min_TJ1_dist_left;
				        goto M;
						   }
					   }
				}
					//}

			}//cond5
								  }//cond4
								}//oe2
								}//oe1
			if((closest_TJ_2_left>=0)&&(closest_TJ_2_right<0))
								 {//2cond1
								hue_closeness=*(bunch_investigated+closest_TJ_2_left);
				                distance_proximity=*(geometric_proximity1+closest_TJ_2_left);
								if(distance_proximity>0)//for left end
								{//cond2
								 success_of_process=1;
						         *prolongation_grain=closest_TJ_2_left;
								 cont_grain=closest_TJ_2_left;
								 fiber_shift=min_TJ1_dist_left;
						         goto M;
								}//cond2
			                      }//2cond1
			                     else
								{//2oe1
			                    if((closest_TJ_2_left<0)&&(closest_TJ_2_right>=0))
								 {//cond3
								 hue_closeness=*(bunch_investigated+closest_TJ_2_right);
				                distance_proximity=*(geometric_proximity1+closest_TJ_2_right);
			                      success_of_process=1;
						         *prolongation_grain=closest_TJ_2_right;
								 cont_grain=closest_TJ_2_right;
								 fiber_shift=closest_TJ_2_right;
						         goto M;
								 }//cond3
								else
								{
									if((closest_TJ_2_left>=0)&&(closest_TJ_2_right>=0))
								    {//cond4
										distance_proximity1=*(geometric_proximity+closest_TJ_2_left);
								  distance_proximity11=*(geometric_proximity+closest_TJ_2_right);
			  if(distance_proximity11>distance_proximity1)
				{
					if(min_TJ2_dist_right<min_TJ1_dist_right)
					{
				success_of_process=1;
				*prolongation_grain=closest_TJ_2_right;
				cont_grain=closest_TJ_2_right;
				fiber_shift=min_TJ2_dist_right;
				goto M;
					}
				}
				else
				{

			           if(distance_proximity11<distance_proximity1)
				       {
						   if(min_TJ2_dist_left<min_TJ1_dist_left)
					       {
				        success_of_process=1;
				        *prolongation_grain=closest_TJ_2_left;
						cont_grain=closest_TJ_2_left;
						fiber_shift=min_TJ2_dist_left;
				        goto M;
						   }
				        }
					   else
					   {
						   if(min_TJ2_dist_right<min_TJ2_dist_left)
						   {
							   if(min_TJ2_dist_right<min_TJ1_dist_right)
					          {
							   success_of_process=1;
				              *prolongation_grain=closest_TJ_2_right;
							  cont_grain=closest_TJ_2_right;
							  fiber_shift=min_TJ2_dist_right;
				               goto M;
							   }
						   }
						   else
						   {
							   if(min_TJ2_dist_left<min_TJ1_dist_left)
					           {
						success_of_process=1;
				        *prolongation_grain=closest_TJ_2_left;
						cont_grain=closest_TJ_2_left;
						fiber_shift=min_TJ2_dist_left;
							   }
				        goto M;
						   }
					   }
				}
									}
								}
			                      }//2oe1




			/*if((closest_bunch<0)&&(closest_in_hue_bunch<0)&&(closest_bunch1<0))
				 {
					 goto M;
			     }*/
				 /*if((closest_bunch>=0)&&(closest_in_hue_bunch>=0))
				 {//2
				 hue_closeness=*(bunch_investigated+closest_in_hue_bunch);
				 distance_proximity=*(geometric_proximity+closest_in_hue_bunch);
			    bunch_opt_mean_sat=ColoredIntervalsStructure1->AverageSat[closest_in_hue_bunch];
			bunch_opt_mean_hue=ColoredIntervalsStructure1->AverageHue[closest_in_hue_bunch];
			bunch_opt_mean_gray=
			ColoredIntervalsStructure1->AverageGray[closest_in_hue_bunch];
			hue_difference=abs(bunch_opt_mean_hue-bunch_mean_hue);
			gray_difference=abs(bunch_opt_mean_gray-bunch_mean_gray);
				 if(((hue_closeness>=4)&&
				 (distance_proximity==maximal_coincidence))||((hue_closeness>=5)&&(((3*distance_proximity>
				 2*maximal_coincidence)&&(distance_proximity>=14))||
				 ((jump_sec==1)&&(StripLength>8*length_int)&&
				 (StripLength>8*length_int_new)&&
				 (distance_proximity>=8)))))
				 {//cond1

						 if(((bunch_opt_mean_sat<=2)||(bunch_mean_sat<=2))&&(gray_difference>possible_deviation))
						 {
						 goto M;
						 }
						 success_of_process=1;
						 *prolongation_grain=closest_in_hue_bunch;
						 goto M;
				 }//cond1
				 else
				 {//econd1
				 distance_proximity=*(geometric_proximity+closest_bunch);
				 hue_closeness=*(bunch_investigated+closest_bunch);
			bunch_opt_mean_sat=ColoredIntervalsStructure1->AverageSat[closest_bunch];
			bunch_opt_mean_hue=ColoredIntervalsStructure1->AverageHue[closest_bunch];
			bunch_opt_mean_gray=
			ColoredIntervalsStructure1->AverageGray[closest_bunch];
			hue_difference=abs(bunch_opt_mean_hue-bunch_mean_hue);
			gray_difference=abs(bunch_opt_mean_gray-bunch_mean_gray);
				 if((jump_sec==1)&&(hue_closeness>=4)&&((distance_proximity>=14)&&
				 (closest_bunch_hue_dif<=1))||((distance_proximity>=24)&&
				 (gray_difference<possible_deviation)))
				 {//cond2

			        if(((bunch_opt_mean_sat<=2)||(bunch_mean_sat<=2))&&(gray_difference>possible_deviation))
						 {
						 goto M;
						 }
				 success_of_process=1;
				 *prolongation_grain=closest_bunch;
				 goto M;
				 }//cond2
				 }//econd1

				 }//2*/
					  /*if((closest_bunch1>=0)&&(closest_in_hue_bunch>=0))
						{//cond3
						distance_proximity1=*(geometric_proximity1+closest_in_hue_bunch);
						distance_proximity=*(geometric_proximity+closest_in_hue_bunch);
						hue_closeness=*(bunch_investigated+closest_in_hue_bunch);
			bunch_opt_mean_sat=ColoredIntervalsStructure1->AverageSat[closest_in_hue_bunch];
			bunch_opt_mean_hue=ColoredIntervalsStructure1->AverageHue[closest_in_hue_bunch];
			bunch_opt_mean_gray=
			ColoredIntervalsStructure1->AverageGray[closest_in_hue_bunch];
			hue_difference=abs(bunch_opt_mean_hue-bunch_mean_hue);
			gray_difference=abs(bunch_opt_mean_gray-bunch_mean_gray);
						if((((hue_closeness>=4)&&
				 (distance_proximity1==maximal_coincidence1)&&
				 (maximal_coincidence1>=24))||((hue_closeness>=5)&&(((3*distance_proximity1>
				 2*maximal_coincidence1)&&(distance_proximity1>=24))||
				 ((jump_sec==1)&&(StripLength>8*length_int)&&
				 (StripLength>8*length_int_new)&&(distance_proximity1>=12)
				 )))))
				 {//cond4

			           if(((bunch_new_mean_sat<=2)||(bunch_mean_sat<=2))&&(gray_difference>possible_deviation))
						 {
						 goto M;
						 }
				 success_of_process=1;
				 *prolongation_grain=closest_in_hue_bunch;
				 //goto M;
				 }//cond4
						}//cond3
						else
						{//econd3
						distance_proximity1=*(geometric_proximity1+closest_bunch1);
						distance_proximity=*(geometric_proximity+closest_bunch1);
						hue_closeness=*(bunch_investigated+closest_bunch1);
			bunch_opt_mean_sat=ColoredIntervalsStructure1->AverageSat[closest_bunch1];
			bunch_opt_mean_hue=ColoredIntervalsStructure1->AverageHue[closest_bunch1];
			bunch_opt_mean_gray=
			ColoredIntervalsStructure1->AverageGray[closest_bunch1];
			hue_difference=abs(bunch_opt_mean_hue-bunch_mean_hue);
			gray_difference=abs(bunch_opt_mean_gray-bunch_mean_gray);
			  if((jump_sec==1)&&(hue_closeness>=4)&&(distance_proximity1>=16)&&(closest_bunch_hue_dif1<=1)
			  &&(gray_difference<=possible_deviation))
				 {//cond5
					 if(((bunch_new_mean_sat<=2)||(bunch_mean_sat<=2))&&(gray_difference>possible_deviation))
						 {
						 goto M;
						 }
			    success_of_process=1;
				 *prolongation_grain=closest_in_hue_bunch;
				 //goto M;
				 }//cond5
						}//econd3*/
				 M:;
				 if((previous_bunch>=0)&&(success_of_process==1))
				 {//pb
			    distance_proximity=*(geometric_proximity+cont_grain);
				distance_proximity1=*(geometric_proximity1+cont_grain);
				 hue_closeness=*(bunch_investigated+cont_grain);
				 cont_beg = ColoredIntervalsStructure1->BegInterv[cont_grain];//last_cor21.02.18
				 		cont_end = ColoredIntervalsStructure1->EndInterv[cont_grain];
				 		cont_length = cont_end - cont_beg + 1;
				 		cont_length_ratio = (16 * cont_length) / (cont_length + length_int);
				 		hue_cont = ColoredIntervalsStructure1->AverageHue[cont_grain];
				 		difference_hue_cont = abs(hue_cont - bunch_mean_hue);//last_cor21.02.18
				 		hue_zone_cont = hue_zones[hue_cont];
				 		gray_cont = ColoredIntervalsStructure1->AverageGray[cont_grain];
				 		gray_zone_cont = gray_zones[gray_cont];
				 		max_gray_zone_cont = std::max(gray_zone_cont, bunch_mean_zone);
				 		sat_cont = ColoredIntervalsStructure1->AverageSat[cont_grain];
				 		if (small_bunch)
				 		{//last_cor21.02.18
				 			if ((cont_length_ratio >= 7) && (cont_length_ratio <= 9))
				 			{
				 				if (max_gray_zone_cont >= 6)
				 				{
				 					if ((distance_proximity >= 8) && (distance_proximity1 >= 8))
				 					{
				 						leave = 1;
				 					}
				 				}
				 			}
				 		}//last_cor21.02.18
				 if(((((distance_proximity>=14)&&(distance_proximity1>=8))||
					 ((distance_proximity1>=14)&&(distance_proximity>=8))||
					 ((distance_proximity>=10)&&(distance_proximity1>=10)))&&(hue_closeness%10==1))||
					 ((hue_closeness%10==2)&&(distance_proximity>=13)&&(distance_proximity1>=13))||
					 ((hue_closeness==11)&&(distance_proximity>=6)&&(distance_proximity1>=6)))
				 {//last_cor19.01.16
					 leave=1;
				 }
				 else
				 {//edp_14dp1_14
					 if(cont_grain==closest_TJ_1_right)
					 {//cg==clTR
						 if(min_TJ1_dist_right<StripWidth)
						 {//std::min<
							 if((hue_closeness==1)&&((distance_proximity>=14)||(distance_proximity1>=14)))
							 {
								 leave=1;
							 }
						 }//std::min<
						 else
						 {//emin<
							 if(closest_TJ_1_left>=0)
							 {//cT>=
				distance_proximity=*(geometric_proximity+closest_TJ_1_left);
				distance_proximity1=*(geometric_proximity1+closest_TJ_1_left);
				 hue_closeness=*(bunch_investigated+closest_TJ_1_left);
			 if((min_TJ1_dist_left<StripWidth)&&(min_TJ1_dist_left<min_TJ1_dist_right))
						 {//std::min<STmin
							 if((hue_closeness==1)&&((distance_proximity>=14)||(distance_proximity1>=14)))
							 {
			                 cont_grain=closest_TJ_1_left;
							 *prolongation_grain=cont_grain;
							 fiber_shift=min_TJ2_dist_left;
								 leave=1;
							 }
						 }//std::min<STmin
							 }//cT>=
						 }//emin<
					 }//cg==clTR
					 else
					 {//ecg==clTR
			if(cont_grain==closest_TJ_1_left)
					   {//cg==clTL
						 if(min_TJ1_dist_left<StripWidth)
						 {
							 if((hue_closeness==1)&&((distance_proximity>=14)||(distance_proximity1>=14)))
							 {
								 leave=1;
							 }
						 }
						 else
						 {//emin
							 if(closest_TJ_1_right>=0)
							 {//cltr>=
			 distance_proximity=*(geometric_proximity+closest_TJ_1_right);
				distance_proximity1=*(geometric_proximity1+closest_TJ_1_right);
				hue_closeness=*(bunch_investigated+closest_TJ_1_right);
			 if((min_TJ1_dist_right<StripWidth)&&(min_TJ1_dist_right<min_TJ1_dist_left))
						 {//drStdr
				if((hue_closeness==1)&&((distance_proximity>=14)||(distance_proximity1>=14)))
				 {
			     cont_grain=closest_TJ_1_right;
				 *prolongation_grain=cont_grain;
				 fiber_shift=min_TJ2_dist_right;
				 leave=1;
				 }
						 }//drStdr
							 }//cltr>=
						 }//emin

					    }//cg==clTL
					 }//ecg==clTR
				 }//edp_14dp1_14
				 if(leave!=1)
				 {//l!=1
			leave=ColorJumpTesting(previous_fiber,previous_bunch,continuation_sec,cont_grain,0,mean_shift);
			previous_bunch_mean_hue=
			ColorInterval[previous_fiber].ColoredIntervalsStructure->AverageHue[previous_bunch];
			previous_bunch_mean_sat=
			ColorInterval[previous_fiber].ColoredIntervalsStructure->AverageSat[previous_bunch];
			previous_bunch_mean_gray=
			ColorInterval[previous_fiber].ColoredIntervalsStructure->AverageGray[previous_bunch];
			previous_gray_zone=gray_zones[previous_bunch_mean_gray];
			bunch_new_mean_sat=ColoredIntervalsStructure1->AverageSat[cont_grain];
			bunch_new_mean_hue=ColoredIntervalsStructure1->AverageHue[cont_grain];
			bunch_new_mean_gray=ColoredIntervalsStructure1->AverageGray[cont_grain];
			bunch_gray_zone=gray_zones[bunch_new_mean_gray];
			abs_sat_dif=abs(previous_bunch_mean_sat-bunch_mean_sat);
			abs_sat_dif1=abs(bunch_mean_sat-bunch_new_mean_sat);
			if((bunch_gray_zone>=2)&&(previous_gray_zone>=2))
			{//bgz
			      if(abs_sat_dif)
			      {
			if((abs_sat_dif1>3*abs_sat_dif)&&(bunch_mean_sat>=2))
			{
				leave=-2;
			}
			      }

			}//bgz
			//}

				 }//l!=1
				 }//pb
				 if(leave<=-2)
				 {//last_cor12.02.15
					 success_of_process=0;
				 }
				 if((FirstGrain>=0)&&(success_of_process==1)&&(previous_bunch>=0)&&(leave<1))
				 {
			leave_first=ColorJumpTesting(FirstStrip,FirstGrain,continuation_sec,cont_grain,1,mean_shift);
			if(leave_first<=-2)
				 {//last_cor02.03.15
					 success_of_process=0;
				 }
			previous_bunch_mean_hue=
			ColorInterval[FirstStrip].ColoredIntervalsStructure->AverageHue[FirstGrain];
			previous_bunch_mean_sat=
			ColorInterval[FirstStrip].ColoredIntervalsStructure->AverageSat[FirstGrain];
			previous_bunch_mean_gray=
			ColorInterval[FirstStrip].ColoredIntervalsStructure->AverageGray[FirstGrain];
			bunch_new_mean_sat=ColoredIntervalsStructure1->AverageSat[cont_grain];
			bunch_new_mean_hue=ColoredIntervalsStructure1->AverageHue[cont_grain];
			bunch_new_mean_gray=ColoredIntervalsStructure1->AverageGray[cont_grain];
				 }

				 if(!success_of_process)
				 {//s=0
					 if(closest_bunch>=0)
					 {//cb
			covering_interval_length_main=0;
			covering_interval_length=0;
			if((closest_bunch_hue_dif>=5)&&(closest_bunch_hue_dif<=7))
			{//cond57
				covering_interval_length_main=maximal_coincidence;
					 if(second_closest_bunch>=0)
					 {
			if((second_closest_bunch_hue_dif>=5)&&(second_closest_bunch_hue_dif<=8))
			{
				covering_interval_length=covering_interval_length_main+second_maximal_coincidence;
			}
					 }
					 if(closest_total_left>=0)
					 {//cll
						 if((closest_total_left!=closest_bunch)&&(closest_total_left!=second_closest_bunch))
						 {
							 hue_closeness=*(bunch_investigated+closest_total_left);
							 if((hue_closeness>=5)&&(hue_closeness<=8))
							 {
								 covering_interval_length+=closest_left_intersection;
							 }
						 }
						 if(closest_total_right>=0)
						 {//con
							 hue_closeness1=*(bunch_investigated+closest_total_right);
							 if((hue_closeness1>=5)&&(hue_closeness1<=8))
							 {
			 if((closest_total_right!=closest_bunch)&&(closest_total_right!=closest_total_left)&&
				 (closest_total_right!=second_closest_bunch))
			 {
			covering_interval_length+=closest_right_intersection;
			 }
							 }
						 }//con
					 }//cll
			}//cond57
					 if((!count_TJ_1)&&(!count_TJ_2)||
						 ((total_lengthTJ1<=2)&&(total_lengthTJ2<=2)))
					 {
						 //if(covering_interval_length_main>=8)
						   //{
					if((bunch_opposite_contrast<=0)||(!opposite_end_condition))
					 {//bo
						 //if((covering_interval_length>=12)||(contrast_interval_total_length>=12))
						 if(contrast_interval_total_length>=15)
						 {
						 success_of_process=-1;
						 }
			              else
						 {//ee
			                   if((contrast_interval_total_length>=11)||
			((contrast_interval_total_length>=9)&&(covering_interval_length_main>=9)))
						       {
						        success_of_process=-2;
						       }
						 }//ee
					 }//bo
						 else
						 {
			               if(covering_interval_length_main==16)
						   {
							   success_of_process=-1;
						   }
						   else
						   {//el
			                if(covering_interval_length_main>=10)
						     {
							   if((contrast_interval_total_length>=14)||(covering_interval_length>=16))
							   {
							   success_of_process=-1;
							   }
						     }
						   }//el
						 }
					 }



					 }//cb
					 else
					 {
						 if((!count_TJ_1)&&(!count_TJ_2)||
						 ((total_lengthTJ1<=2)&&(total_lengthTJ2<=2)))
					     {
							 success_of_process=-1;
						 }
					 }
			if((closest_total_left>=0)&&(closest_total_right<0))
			{
			hue_closeness=*(bunch_investigated+closest_total_left);

			}
			else
			{//ec
				if((closest_total_left<0)&&(closest_total_right>=0))
			    {
			    hue_closeness=*(bunch_investigated+closest_total_right);
				}
				else
				{//e
					if((closest_total_right>=0)&&(closest_total_left>=0))
			        {
						hue_closeness=*(bunch_investigated+closest_total_left);
			            hue_closeness1=*(bunch_investigated+closest_total_right);
					}
				}//e
			}//ec
				 }//s=0
			if(success_of_process>0)
			{
			TotalShift+=fiber_shift;
			}
			delete[] geometric_proximity;
			delete[] geometric_proximity1;
			delete[] bunch_investigated;
			return(success_of_process);
			}

		//----------------------------------------------------------------------------
		void CColorSection::Record_of_section(int current_fibre, int *trace, int current_grain, int old_current_grain,
				int number_of_section) {
			DescrSec[number_of_section].location_of_section[current_fibre] = current_grain + 1;
			//*(trace + old_current_grain) = number_of_section + 1;
			if(!trace[old_current_grain])
			{
			*(trace+old_current_grain)=number_of_section+1;
			}
		}
		//----------------------------------------------------------------------------
		void CColorSection::End_of_process(int first_strip, int first_grain, int section_numb, int last_strip, int count) {
			int coun_sect;

			DescrSec[section_numb].location_of_section[first_strip] = first_grain + 1;
			DescrSec[section_numb].length_of_section = count;
			DescrSec[section_numb].base_first = first_strip;
			DescrSec[section_numb].base_last = last_strip;
			section_fibre_first[section_numb] = first_strip;
			section_fibre_last[section_numb] = last_strip;

			return;
		}
		//----------------------------------------------------------------------------
		int

		CColorSection::Measure1_of_proximity(int first_left,int first_right,
				int second_left,int second_right,int* proximity1,int* proximity2,int* left_shift,int* right_shift,
				int* ind_length,int* inter_length_ratio,int* inter_length_ratio1)
				{
				int left_end,right_end,length1,length2;
				int minimum_of_length,max_length;
				int union_beg,union_end;
				int shift_left,shift_right;
				int inter_length;
				int prior;
				int length_ratio,length_ratio1;

				prior=-1;
				left_end=std::max(first_left,second_left);
				right_end=std::min(first_right,second_right);
				inter_length=right_end-left_end+1;
				length1=first_right-first_left+1;
				length2=second_right-second_left+1;
				*ind_length=0;

					if(length1>=2*length2)
					{
					 *ind_length=3;
					}
								else
							  {
									 if(length1>=length2)
									  {
									  *ind_length=2;
									  }
										  else
											{
											  if(2*length1>=length2)
												{
												*ind_length=1;
												}
											}
							 }
				minimum_of_length=std::min(length1,length2);
				max_length=std::max(length1,length2);
				union_beg=std::min(first_left,second_left);
				union_end=std::max(first_right,second_right);
				shift_left=left_end-union_beg;
				shift_right=union_end-right_end;
				if(left_end>right_end+1)
				  {
				  *proximity1=left_end-right_end;
				  *proximity2=0;
				  *left_shift=shift_left;
				  *right_shift=shift_right;
				*inter_length_ratio=left_end-right_end;
				*inter_length_ratio1=0;
				  prior=3;
				  return(prior);
				  }
				else
				 {//e3
				*proximity1=((right_end-left_end+1)<<5)/max_length;
				*proximity2=((right_end-left_end+1)<<5)/minimum_of_length;
				length_ratio=(16*inter_length)/length1;
				length_ratio1=(16*inter_length)/length2;
				*inter_length_ratio=length_ratio;
				*inter_length_ratio1=length_ratio1;

				  if(((inter_length)*2)>(max_length))
				  {
				  prior=0;
				  }
				  else
				  {//e1
				  if(((inter_length)*2)>(minimum_of_length))
				  {
				  prior=1;
				  }
					else
					{//e2
					 prior=2;
					}//e2
				  }//e1
				}//e3
				  *left_shift=shift_left;
				  *right_shift=shift_right;


						return(prior);

				}
		//----------------------------------------------------------------------------
		void

		CColorSection::MeanCharacteristics(void)
		{
			int coun;
			int first_fibre;
			int last_fibre;
			int section_length;
			int fibre_coun;
			int first_member;
			int old_num_first;
			int old_num_current;
			int current_member;
			int aver_hue;
			int lower_hue;
			int upper_hue;
			int aver_hue1;
			int lower_hue1;
			int upper_hue1;
			float mean_hue;
			float mean_saturation;
			float mean_gray;
			float current_mean_hue;
			float current_mean_saturation;
			float current_mean_gray;
			int Mean_hue;
			int Mean_gray;
			int Mean_sat;
			int aver_sat;
			int aver_sat1;
			int lower_sat;
			int upper_sat;
			int lower_sat1;
			int upper_sat1;
			int aver_gray;
			int aver_gray1;
			int lower_gray;
			int lower_gray1;
			int upper_gray;
			int upper_gray1;
			int total_significance;
			int current_significance;
			int fiber_weight;//last_cor06.05.15
			int section_total_weight;
			int fiber_beg;
			int fiber_end;//last_cor06.05.15
			int min_gray_dist;
			int min_left;
			int max_right;
			int section_fiber_length;
			int section_fiber_m_length;
			int mean_lower;//last_cor14.11.16
			int mean_upper;
			int sect_geom;
			int number_saturated;
			int section_sat_weight;
			int sat_weight_ratio;
			//int count_section;
			int* fibre_hues;
			int* hue_weights;
			int* lower_hues;
			int* upper_hues;
			int* aver_sats;
			int* lower_sats;
			int* upper_sats;
			int* aver_grays;
			int* lower_grays;
			int* upper_grays;
			TIntColored* ColoredIntervalsStructure;


			section_fiber_length=0;
			section_fiber_m_length=0;
			mean_lower=0;
			mean_upper=0;
			sect_geom=-1;
			fibre_hues=new int [Num_Strips];
			hue_weights=new int [Num_Strips];
			lower_hues=new int [Num_Strips];
			upper_hues=new int [Num_Strips];
			aver_sats=new int [Num_Strips];
			lower_sats=new int [Num_Strips];
			upper_sats=new int [Num_Strips];
			aver_grays=new int [Num_Strips];
			lower_grays=new int [Num_Strips];
			upper_grays=new int [Num_Strips];


			/*section_upper_hues=new int [Num_Strips*Number_of_sections];
			section_lower_hues=new int [Num_Strips*Number_of_sections];
			section_upper_saturations=new int [Num_Strips*Number_of_sections];
			section_lower_saturations=new int [Num_Strips*Number_of_sections];
			section_upper_grays=new int [Num_Strips*Number_of_sections];
			section_lower_grays=new int [Num_Strips*Number_of_sections];*/


			for(coun=0;coun<Number_of_sections;coun++)
			{
					section_fiber_m_length=0;
					max_right=0;
					section_total_weight=0;
					section_sat_weight=0;
			         number_saturated=0;

				first_fibre=*(section_fibre_first+coun);
				last_fibre=*(section_fibre_last+coun);
				//section_length=last_fibre-first_fibre+1;
				if(coun<Number_of_sections_left)
				{
					*(left_section_start_number+first_fibre)+=1;
					*(left_section_last_number+last_fibre)+=1;
				}
				else
				{
				*(right_section_start_number+first_fibre)+=1;
				*(right_section_last_number+last_fibre)+=1;
				}

				first_member=DescrSec[coun].location_of_section[first_fibre]-1;
				section_length=DescrSec[coun].length_of_section;
				old_num_first=ColorInterval[first_fibre].old_bunch_number[first_member];
			ColoredIntervalsStructure=ColorInterval[first_fibre].ColoredIntervalsStructure;
			total_significance=ColoredIntervalsStructure->Significance[old_num_first];
			fiber_beg=ColoredIntervalsStructure->BegInterv[old_num_first];
			fiber_end=ColoredIntervalsStructure->EndInterv[old_num_first];
			section_fiber_length=fiber_end-fiber_beg+1;
			if(section_fiber_length>section_fiber_m_length)
			{
			section_fiber_m_length=section_fiber_length;
			}
			min_left=fiber_beg;
			fiber_weight=section_fiber_length;
			section_total_weight+=fiber_weight;

			aver_hue= ColoredIntervalsStructure->AverageHue[old_num_first];
			lower_hue=ColoredIntervalsStructure->LowerHueValue[old_num_first];
			upper_hue=ColoredIntervalsStructure->UpperHueValue[old_num_first];

			if(aver_hue>=NUM_HUES)
			{
			aver_hue-=NUM_HUES;
			}
			if(lower_hue>=NUM_HUES)
			{
			lower_hue-=NUM_HUES;
			}
			if(upper_hue>=NUM_HUES)
			{
			upper_hue-=NUM_HUES;
			}
			*(fibre_hues+first_fibre)=aver_hue;
			*(hue_weights+first_fibre)=total_significance;
			*(lower_hues+first_fibre)=lower_hue;
			*(upper_hues+first_fibre)=upper_hue;
			mean_hue=(float)aver_hue;
			aver_sat=ColoredIntervalsStructure->AverageSat[old_num_first];
			if(aver_sat>=3)
			{
				number_saturated++;
				section_sat_weight+=fiber_weight;
			}
			lower_sat=ColoredIntervalsStructure->LowerSatValue[old_num_first];
			upper_sat=ColoredIntervalsStructure->UpperSatValue[old_num_first];
			mean_saturation=(float)aver_sat;
			*(aver_sats+first_fibre)=aver_sat;
			*(lower_sats+first_fibre)=lower_sat;
			*(upper_sats+first_fibre)=upper_sat;
			aver_gray=ColoredIntervalsStructure->AverageGray[old_num_first];
			lower_gray=ColoredIntervalsStructure->LowerGrayscale[old_num_first];
			upper_gray=ColoredIntervalsStructure->UpperGrayscale[old_num_first];
			*(aver_grays+first_fibre)=aver_gray;
			*(lower_grays+first_fibre)=lower_gray;
			*(upper_grays+first_fibre)=upper_gray;
			mean_gray=(float) aver_gray;


			  for(fibre_coun=first_fibre+1;fibre_coun<=last_fibre;fibre_coun++)
				  {
				  current_member=DescrSec[coun].location_of_section[fibre_coun];
				  if(current_member)
				  {

			  current_member--;
			  old_num_current=ColorInterval[fibre_coun].old_bunch_number[current_member];

			  ColoredIntervalsStructure=ColorInterval[fibre_coun].ColoredIntervalsStructure;
			  current_significance=
			  ColoredIntervalsStructure->Significance[old_num_current];
			fiber_beg=ColoredIntervalsStructure->BegInterv[old_num_current];
			fiber_end=ColoredIntervalsStructure->EndInterv[old_num_current];
			section_fiber_length=fiber_end-fiber_beg+1;
			if(section_fiber_length>section_fiber_m_length)
			{
			section_fiber_m_length=section_fiber_length;
			}
			if(fiber_beg<min_left)
			{
				min_left=fiber_beg;
			}
			if(fiber_end>max_right)
			{
			max_right=fiber_end;
			}
			fiber_weight=section_fiber_length;
			section_total_weight+=fiber_weight;
			  aver_hue1=ColoredIntervalsStructure->AverageHue[old_num_current];
			 lower_hue1=ColoredIntervalsStructure->LowerHueValue[old_num_current];
			 upper_hue1=ColoredIntervalsStructure->UpperHueValue[old_num_current];
			 if(aver_hue1>=NUM_HUES)
			{
			aver_hue1-=NUM_HUES;
			}
			if(lower_hue1>=NUM_HUES)
			{
			lower_hue1-=NUM_HUES;
			}
			if(upper_hue1>=NUM_HUES)
			{
			upper_hue1-=NUM_HUES;
			}
			  *(fibre_hues+fibre_coun)=aver_hue1;
			  *(hue_weights+fibre_coun)=current_significance;
			  *(lower_hues+fibre_coun)=lower_hue1;
			  *(upper_hues+fibre_coun)=upper_hue1;

				if((aver_hue<NUM_HUES)&&(aver_hue>38)&&(aver_hue1<6))
				{
				aver_hue1+=NUM_HUES;
				}

				if((aver_hue1<NUM_HUES)&&(aver_hue1>38)&&(aver_hue<6))
				{
				mean_hue=mean_hue+(float)NUM_HUES;
				}
			  current_mean_hue= (float)aver_hue1;
			  aver_sat1=ColoredIntervalsStructure->AverageSat[old_num_current];
			  if(aver_sat1>=3)
			{
				number_saturated++;
				section_sat_weight+=fiber_weight;
			}
			  lower_sat1=ColoredIntervalsStructure->LowerSatValue[old_num_current];
			  upper_sat1=ColoredIntervalsStructure->UpperSatValue[old_num_current];
			  current_mean_saturation=(float)aver_sat1;
			  *(aver_sats+fibre_coun)=aver_sat1;
			  *(lower_sats+fibre_coun)=lower_sat1;
			  *(upper_sats+fibre_coun)=upper_sat1;
			  aver_gray1=ColoredIntervalsStructure->AverageGray[old_num_current];
			  lower_gray1=ColoredIntervalsStructure->LowerGrayscale[old_num_current];
			  upper_gray1=ColoredIntervalsStructure->UpperGrayscale[old_num_current];
			  if((aver_gray1>=lower_gray1)&&(aver_gray1<=upper_gray1))
			  {
				  min_gray_dist=std::min((aver_gray1-lower_gray1),(upper_gray1-aver_gray1));
			     lower_gray1=aver_gray1-min_gray_dist;
			      upper_gray1=aver_gray1+min_gray_dist;
			  }
			  else
			  {
				  if(aver_gray1<lower_gray1)
				  {
					  aver_gray1=lower_gray1;
				  }
				  else
				  {
					  if(aver_gray1>upper_gray1)
				  {
					  aver_gray1=upper_gray1;
				  }
				  }
			  }
			*(aver_grays+fibre_coun)=aver_gray1;
			*(lower_grays+fibre_coun)=lower_gray1;
			*(upper_grays+fibre_coun)=upper_gray1;
			  current_mean_gray=(float) aver_gray1;
			  mean_hue=((((float)total_significance)*mean_hue)+
			  (((float)current_significance)*current_mean_hue))/((float)(total_significance+
			  current_significance));
			  aver_hue=(int)mean_hue;
						  if(aver_hue>=NUM_HUES)
							{
							 mean_hue=mean_hue-(float)NUM_HUES;
							}
			  mean_saturation=((((float)total_significance)*mean_saturation)+
			(((float)current_significance)*current_mean_saturation))/
			((float)(total_significance+current_significance));

			  mean_gray=((((float)total_significance)*mean_gray)+
			(((float)current_significance)*current_mean_gray))/((float)(total_significance+
			  current_significance));
			  total_significance+=current_significance;
				  }
				  }
				  sat_weight_ratio=(16*section_sat_weight)/section_total_weight;

			Mean_hue=(int)mean_hue;

				if((mean_hue-Mean_hue)>0.5)
				{
				Mean_hue+=1;
				}

				if(Mean_hue>=NUM_HUES)
				{
			    Mean_hue-=NUM_HUES;
				}
			*(section_mean_hue+coun)=Mean_hue;
			DescrSec[coun].average_hue=Mean_hue;
			Mean_sat=(int)mean_saturation;
				if((mean_saturation-Mean_sat)>0.5)
				{
				Mean_sat+=1;
				}
				else
				{
					if(Mean_sat==2)
					{
			     if((mean_saturation-Mean_sat)>0.4)
				  {
					  if((number_saturated>=section_length/2)&&(sat_weight_ratio>=7))
					  {
					  Mean_sat+=1;
					  }
				  }
					}
				}
			*(section_mean_saturation+coun)=Mean_sat;
			DescrSec[coun].mean_sat=Mean_sat;
			Mean_gray=(int)mean_gray;
				if((mean_gray-Mean_gray)>0.5)
				{
				Mean_gray+=1;
				}
				*(section_weight+coun)=section_total_weight;
			*(section_mean_gray+coun)=Mean_gray;
			DescrSec[coun].mean_gray=Mean_gray;
			ThreshodRecord(upper_hues,lower_hues,upper_sats,lower_sats,upper_grays,
			lower_grays,coun);
			*(section_left_end+coun)=min_left;
			*(section_right_end+coun)=max_right;
			*(section_fiber_max_length+coun)=section_fiber_m_length;
			sect_geom=SectionGeometry(coun,&mean_lower,&mean_upper);
			if(sect_geom>0)
				{
			section_lower_mean_length[coun]=mean_lower;
			section_upper_mean_length[coun]=mean_upper;
			    }
				}



			delete[] fibre_hues;
			delete[] hue_weights;
			delete[] lower_hues;
			delete[] upper_hues;
			delete[] aver_sats;
			delete[] lower_sats;
			delete[] upper_sats;
			delete[] aver_grays;
			delete[] lower_grays;
			delete[] upper_grays;
			}
		//----------------------------------------------------------------------------
		void
		CColorSection::ThreshodRecord(int* upp_hues,int* low_hues, int* upp_sats,
		int* low_sats,int* upp_grays,int* low_grays,int section_number)
		{
			int coun_str;
			int point_start;
			int fibre_first;
			int fibre_last;
			int Max_hue;
			int Min_hue;
			int Max_gray;
			int Min_gray;
			int Max_sat;
			int Min_sat;
			int current_up_hue;
			int current_min_hue;
			int current_up_sat;
			int current_min_sat;
			int current_up_gray;
			int current_min_gray;
			int Mean_Min_gray;//last_cor06.05.15
			int Mean_Max_gray;
			int current_mean_gray;

			Max_hue=-1;
			Min_hue=1000;
			Max_sat=-1;
			Min_sat=1000;
			Max_gray=-1;
			Min_gray=1000;
			Mean_Min_gray=0;
			Mean_Max_gray=0;

			point_start=section_number*Num_Strips;
			fibre_first=*(section_fibre_first+section_number);
			fibre_last=*(section_fibre_last+section_number);
			current_mean_gray=DescrSec[section_number].mean_gray;
			for(coun_str=fibre_first;coun_str<=fibre_last;coun_str++)
				{
					current_up_hue=*(upp_hues+coun_str);
					if(current_up_hue>Max_hue)
					{
			        Max_hue=current_up_hue;
					}
					current_min_hue=*(low_hues+coun_str);
					if(current_min_hue<Min_hue)
					{
			        Min_hue=current_min_hue;
					}
				*(section_upper_hues+point_start+coun_str)=current_up_hue;
				*(section_lower_hues+point_start+coun_str)=current_min_hue;
			current_up_sat=*(upp_sats+coun_str);
					if(current_up_sat>Max_sat)
					{
			        Max_sat=current_up_sat;
					}
					current_min_sat=*(low_sats+coun_str);
					if(current_min_sat<Min_sat)
					{
			        Min_sat=current_min_sat;
					}
				*(section_upper_saturations+point_start+coun_str)=current_up_sat;
				*(section_lower_saturations+point_start+coun_str)=current_min_sat;
				current_up_gray=*(upp_grays+coun_str);
				current_min_gray=*(low_grays+coun_str);
				Mean_Max_gray+=current_up_gray;
					if(current_up_gray>Max_gray)
					{
			        Max_gray=current_up_gray;
					}

					Mean_Min_gray+=current_min_gray;
					if(current_min_gray<Min_gray)
					{
			        Min_gray=current_min_gray;
					}
				*(section_upper_grays+point_start+coun_str)=current_up_gray;
				*(section_lower_grays+point_start+coun_str)=current_min_gray;

				}
			Mean_Max_gray=Mean_Max_gray/(fibre_last-fibre_first+1);
			Mean_Min_gray=Mean_Min_gray/(fibre_last-fibre_first+1);
			DescrSec[section_number].lower_gray=Min_gray;
			DescrSec[section_number].lower_hue=Min_hue;
			DescrSec[section_number].lower_sat=Min_sat;
			DescrSec[section_number].upper_gray=Max_gray;
			DescrSec[section_number].upper_hue=Max_hue;
			DescrSec[section_number].upper_sat=Max_sat;

			}
		//----------------------------------------------------------------------------
		int
		CColorSection::Test_Junction_Different_Strips_Same_Image(int next_bunch,int previous_bunch,
		int Intersection_Measure,int indic_length,int ratio_length,int ratio_length1,int strip_number,
		int previous_strip_number,int *hue_close_r,int *sat_close_r,int* gray_close_r)
		{
			int bunch_mean_hue;
				int corrected_bunch_mean_hue;
				int neighbor_bunch_mean_hue;
				int corrected_neighbor_bunch_mean_hue;
				int neighbor_bunch_minimal_hue;
				int neighbor_bunch_maximal_hue;
				int bunch_mean_saturation;
				int neighbor_bunch_mean_saturation;
				int bunch_maximal_hue;
				int bunch_minimal_hue;
				int corrected_bunch_minimal_hue;
				int corrected_bunch_maximal_hue;
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
				int Intersection_hue;
				int coinsidence_hue;
				int coinsidence1_hue;
				int shift_to_left_hue, shift_to_right_hue;
				int indic_length_hue, ratio_length_hue, ratio_length1_hue;
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
				int minimum_zone_gray;
				int hue_dif1;
				int joint_hue_range;
				int min_saturation;
				int max_saturation;
				int gray_hole;//last_cor24.12.15
				int len_ratio;//last_cor20.02.18
			TIntColored* ColoredIntervalsStructure1;
			TIntColored* ColoredIntervalsStructure;

			Intersection_saturation = -1;

				gray_hole = -1;//last_cor24.12.15
				indic_length_satur = -1;
				ratio_length_satur = -1;
				ratio_length1_satur = -1;
				Intersection_hue = -1;
				inclusion_relation = 0;
				prior = 0;
				hue_close = 0;
				saturation_close = 0;
				grayscale_close = 0;
				wide_gray_gap = 0;
				Intersection_hue = -1;
				corrected_bunch_mean_hue = -1;
				corrected_neighbor_bunch_mean_hue = -1;
				corrected_bunch_minimal_hue = -1;
				corrected_bunch_maximal_hue = -1;
				corrected_neighbor_bunch_minimal_hue = -1;
				corrected_neighbor_bunch_maximal_hue = -1;
				if ((next_bunch<0) || (previous_bunch<0))
					{
						prior = 5;
						goto L;
					}
					ColoredIntervalsStructure1 = ColorInterval[strip_number].ColoredIntervalsStructure;
					ColoredIntervalsStructure = ColorInterval[previous_strip_number].ColoredIntervalsStructure;

					beg_int = ColoredIntervalsStructure->BegInterv[previous_bunch];
					end_int = ColoredIntervalsStructure->EndInterv[previous_bunch];
					int_length = end_int - beg_int + 1;
					neighbor_beg_int = ColoredIntervalsStructure1->BegInterv[next_bunch];
					neighbor_end_int = ColoredIntervalsStructure1->EndInterv[next_bunch];
					neighbor_length = neighbor_end_int - neighbor_beg_int + 1;
					len_ratio = (16 * int_length) / (int_length + neighbor_length);
					if ((neighbor_beg_int <= beg_int) && (neighbor_end_int >= end_int))
					{
						inclusion_relation = 1;
					}
					//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
					//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
					bunch_significance = ColoredIntervalsStructure->Significance[previous_bunch];
					neighbor_significance = ColoredIntervalsStructure1->Significance[next_bunch];
					Signif_ratio = (16 * bunch_significance) / (bunch_significance + neighbor_significance);

					bunch_density = 2 * bunch_significance / int_length;
					neighbor_density = 2 * neighbor_significance / neighbor_length;
					Density_ratio = (16 * bunch_density) / (bunch_density + neighbor_density);
					length_ratio = (16 * int_length) / (int_length + neighbor_length);

					node_vicinity = 0;
					node_vicinity_neighbor = 0;
					bunch_occurrence = ColorInterval[strip_number].visible_bunches[previous_bunch];
					bunch_mean_hue = ColoredIntervalsStructure->AverageHue[previous_bunch];
					if (bunch_mean_hue >= NUM_HUES)
					{
						bunch_mean_hue -= NUM_HUES;
					}
					bunch_minimal_hue = ColoredIntervalsStructure->LowerHueValue[previous_bunch];
					if (bunch_minimal_hue >= NUM_HUES)
					{
						bunch_minimal_hue -= NUM_HUES;
					}

					bunch_maximal_hue = ColoredIntervalsStructure->UpperHueValue[previous_bunch];
					if (bunch_maximal_hue >= NUM_HUES)
					{
						bunch_maximal_hue -= NUM_HUES;
					}
					zone_hue_bunch = hue_zones[bunch_mean_hue];
					neighbor_bunch_mean_hue = ColoredIntervalsStructure1->AverageHue[next_bunch];
					if (neighbor_bunch_mean_hue >= NUM_HUES)
					{
						neighbor_bunch_mean_hue -= NUM_HUES;
					}
					zone_hue = hue_zones[neighbor_bunch_mean_hue];
					hue_bound = dev_hue_zone[zone_hue];
					difference_hue_zone = abs(zone_hue_bunch - zone_hue);
					difference_hue_zone = std::min(difference_hue_zone, 5 - difference_hue_zone);

					hue_dif = abs(bunch_mean_hue - neighbor_bunch_mean_hue);
					hue_difference = std::min(hue_dif, NUM_HUES - hue_dif);
					min_hue_zone = std::min(zone_hue_bunch, zone_hue);//last_cor17.04.15
					max_hue_zone = std::max(zone_hue_bunch, zone_hue);
					if (hue_dif>hue_difference)
					{
						if (bunch_mean_hue<neighbor_bunch_mean_hue)
						{
							corrected_bunch_mean_hue = bunch_mean_hue + NUM_HUES;
							corrected_neighbor_bunch_mean_hue = neighbor_bunch_mean_hue;
						}
						else
						{
							if (bunch_mean_hue>neighbor_bunch_mean_hue)
							{
								corrected_bunch_mean_hue = bunch_mean_hue;
								corrected_neighbor_bunch_mean_hue = neighbor_bunch_mean_hue + NUM_HUES;
							}
						}
					}
					else
					{
						corrected_bunch_mean_hue = bunch_mean_hue;
						corrected_neighbor_bunch_mean_hue = neighbor_bunch_mean_hue;
					}

					hue_dif = abs(bunch_mean_hue - bunch_maximal_hue);
					right_bunch_hue = std::min(hue_dif, NUM_HUES - hue_dif);
					if (hue_dif>right_bunch_hue)
					{
						corrected_bunch_maximal_hue = bunch_maximal_hue + NUM_HUES;
						if (corrected_bunch_mean_hue<0)
						{
							corrected_bunch_mean_hue = bunch_mean_hue;
						}

					}
					else
					{
						corrected_bunch_maximal_hue = bunch_maximal_hue;
					}

					hue_dif = abs(bunch_mean_hue - bunch_minimal_hue);
					left_bunch_hue = std::min(hue_dif, NUM_HUES - hue_dif);
					min_hue_bunch_dist = std::min(left_bunch_hue, right_bunch_hue);
					if (hue_dif>left_bunch_hue)
					{
						if (corrected_bunch_mean_hue<0)
						{
							corrected_bunch_mean_hue = bunch_mean_hue + NUM_HUES;
						}
					}

					neighbor_bunch_minimal_hue = ColoredIntervalsStructure1->LowerHueValue[next_bunch];
					if (neighbor_bunch_minimal_hue >= NUM_HUES)
					{
						neighbor_bunch_minimal_hue -= NUM_HUES;
					}
					neighbor_bunch_maximal_hue = ColoredIntervalsStructure1->UpperHueValue[next_bunch];
					if (neighbor_bunch_maximal_hue >= NUM_HUES)
					{
						neighbor_bunch_maximal_hue -= NUM_HUES;
					}
					hue_dif = abs(neighbor_bunch_maximal_hue - neighbor_bunch_mean_hue);
					right_hue = std::min(hue_dif, NUM_HUES - hue_dif);
					if (hue_dif>right_hue)
					{
						corrected_neighbor_bunch_maximal_hue = neighbor_bunch_maximal_hue + NUM_HUES;
						if (corrected_neighbor_bunch_mean_hue<0)
						{
							corrected_neighbor_bunch_mean_hue = neighbor_bunch_mean_hue;
						}
					}
					hue_dif = abs(neighbor_bunch_mean_hue - neighbor_bunch_minimal_hue);
					left_hue = std::min(hue_dif, NUM_HUES - hue_dif);
					if (hue_dif>left_hue)
					{
						if (corrected_neighbor_bunch_mean_hue<0)
						{
							corrected_neighbor_bunch_mean_hue = neighbor_bunch_mean_hue + NUM_HUES;
						}
					}
					min_hue_dist = std::min(left_hue, right_hue);
					if (corrected_neighbor_bunch_mean_hue<0)
					{
						corrected_neighbor_bunch_mean_hue = neighbor_bunch_mean_hue;
					}
					if (corrected_bunch_mean_hue<0)
					{
						corrected_bunch_mean_hue = bunch_mean_hue;
					}
					hue_dif = abs(corrected_neighbor_bunch_mean_hue - corrected_bunch_mean_hue);
					hue_dif1 = std::min(hue_dif, NUM_HUES - hue_dif);
					if (hue_dif1<hue_dif)
					{
						if (corrected_neighbor_bunch_mean_hue>corrected_bunch_mean_hue)
						{
							corrected_bunch_mean_hue += NUM_HUES;
						}
						else
						{
							corrected_neighbor_bunch_mean_hue += NUM_HUES;
						}
					}
					if ((neighbor_bunch_minimal_hue <= neighbor_bunch_mean_hue) && (bunch_minimal_hue <= bunch_mean_hue))
					{//cond0
						corrected_neighbor_bunch_mean_hue = neighbor_bunch_mean_hue;
						corrected_bunch_mean_hue = bunch_mean_hue;
					}//cond0
					else
					{//econd0
						if (neighbor_bunch_minimal_hue>neighbor_bunch_mean_hue)
						{//cond1
							hue_dif = neighbor_bunch_minimal_hue - neighbor_bunch_mean_hue;
							if (hue_dif>(NUM_HUES - hue_dif))
							{//cond2
								corrected_neighbor_bunch_mean_hue = neighbor_bunch_mean_hue + NUM_HUES;
								if (bunch_mean_hue<NUM_HUES / 2)
								{//last_cor21.05.15
									corrected_bunch_mean_hue = bunch_mean_hue + NUM_HUES;
								}//last_cor21.05.15
							}//cond2
							else
							{//econd2
								corrected_bunch_mean_hue = bunch_mean_hue;
							}//econd2

						}//cond1
						 //else
						 //{//econd1
						if (bunch_minimal_hue>bunch_mean_hue)
						{//last_cor21.05.15
							hue_dif = bunch_minimal_hue - bunch_mean_hue;
							if (hue_dif>(NUM_HUES - hue_dif))
							{//cond3
								corrected_bunch_mean_hue = bunch_mean_hue + NUM_HUES;
								if (neighbor_bunch_mean_hue<NUM_HUES / 2)
								{//last_cor21.05.15
									corrected_neighbor_bunch_mean_hue = neighbor_bunch_mean_hue + NUM_HUES;
								}//last_cor21.05.15
							}//cond3
							else
							{//econd3
								corrected_bunch_mean_hue = bunch_mean_hue;
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
					left_bunch_hue = corrected_bunch_mean_hue - min_hue_bunch_dist;
					right_bunch_hue = corrected_bunch_mean_hue + min_hue_bunch_dist;
					left_hue = corrected_neighbor_bunch_mean_hue - min_hue_dist;
					right_hue = corrected_neighbor_bunch_mean_hue + min_hue_dist;
					difference_hue_left = abs(corrected_bunch_mean_hue - left_hue);
					difference_hue_left = std::min(difference_hue_left, NUM_HUES - difference_hue_left);
					difference_hue_right = abs(corrected_bunch_mean_hue - right_hue);
					difference_hue_right = std::min(difference_hue_right, NUM_HUES - difference_hue_right);
					Intersection_hue = Measure1_of_proximity(left_bunch_hue, right_bunch_hue, left_hue, right_hue, &coinsidence_hue,
						&coinsidence1_hue, &shift_to_left_hue, &shift_to_right_hue, &indic_length_hue, &ratio_length_hue,
						&ratio_length1_hue);
					if (Intersection_hue == 3)
					{
						coinsidence_hue = 0;
						//ratio_length_hue=0;
						//ratio_length1_hue=0;
					}
					joint_hue_range = std::max(right_bunch_hue, right_hue) - std::min(left_bunch_hue, left_hue);
					joint_hue_range = std::min(joint_hue_range, NUM_HUES - joint_hue_range);
					if ((hue_difference <= 1) || ((hue_difference <= 2) && ((coinsidence1_hue >= 18) || (joint_hue_range <= 6))))
					{//last_cor07.07.13
						hue_close = 1;
					}
					else
					{//ehd1

						if (((zone_hue_bunch != 2) && (zone_hue != 2)) || ((zone_hue_bunch == 2) && (zone_hue != 1)) ||
							((zone_hue_bunch != 1) && (zone_hue == 2)))
						{//hue_cond_zone21_last_cor16.07.13
							if ((hue_difference <= 2) && ((coinsidence1_hue >= 18) || (joint_hue_range <= 4)))
							{
								hue_close = 1;
							}
						}//hue_cond_zone21
					}//ehd1
					if (((bunch_mean_hue >= 46) && (bunch_mean_hue<48)) ||
						((bunch_mean_hue >= 0) && (bunch_mean_hue <= 2)) ||
						((bunch_mean_hue >= 30) && (bunch_mean_hue <= 34)) ||
						((bunch_mean_hue >= 12) && (bunch_mean_hue <= 20)))
					{
						node_vicinity = 1;
					}
					if (((neighbor_bunch_mean_hue >= 46) && (neighbor_bunch_mean_hue <= 48)) ||
						((neighbor_bunch_mean_hue >= 0) && (neighbor_bunch_mean_hue <= 2)) ||
						((neighbor_bunch_mean_hue >= 30) && (neighbor_bunch_mean_hue <= 34)) ||
						((neighbor_bunch_mean_hue >= 12) && (neighbor_bunch_mean_hue <= 20)))
					{
						node_vicinity_neighbor = 1;
					}
					//saturation
					bunch_mean_saturation = ColoredIntervalsStructure->AverageSat[previous_bunch];
					bunch_minimal_saturation = ColoredIntervalsStructure->LowerSatValue[previous_bunch];
					if (bunch_mean_saturation<bunch_minimal_saturation)
					{//last_cor14.12.15
						bunch_minimal_saturation = bunch_mean_saturation;
						ColoredIntervalsStructure->LowerSatValue[previous_bunch] = bunch_mean_saturation;
					}//last_cor14.12.15
					bunch_maximal_saturation = ColoredIntervalsStructure->UpperSatValue[previous_bunch];
					if (bunch_mean_saturation>bunch_maximal_saturation)
					{//last_cor14.12.15
						bunch_maximal_saturation = bunch_mean_saturation;
						ColoredIntervalsStructure->UpperSatValue[previous_bunch] = bunch_mean_saturation;
					}//last_cor14.12.15
					min_sat_dist = std::min(bunch_maximal_saturation - bunch_mean_saturation, bunch_mean_saturation - bunch_minimal_saturation);
					left_sat_bunch = bunch_mean_saturation - min_sat_dist;
					right_sat_bunch = min_sat_dist + bunch_mean_saturation;
					neighbor_bunch_mean_saturation =
						ColoredIntervalsStructure1->AverageSat[next_bunch];
					max_saturation = std::max(bunch_mean_saturation, neighbor_bunch_mean_saturation);
					min_saturation = std::min(bunch_mean_saturation, neighbor_bunch_mean_saturation);
					//!?
					neighbor_bunch_minimal_saturation = ColoredIntervalsStructure1->LowerSatValue[next_bunch];
					if (neighbor_bunch_mean_saturation<neighbor_bunch_minimal_saturation)
					{//last_cor14.12.15
						neighbor_bunch_minimal_saturation = neighbor_bunch_mean_saturation;
						ColoredIntervalsStructure1->LowerSatValue[next_bunch] = neighbor_bunch_mean_saturation;
					}//last_cor14.12.15
					neighbor_bunch_maximal_saturation = ColoredIntervalsStructure1->UpperSatValue[next_bunch];
					if (neighbor_bunch_mean_saturation>neighbor_bunch_maximal_saturation)
					{//last_cor14.12.15
						neighbor_bunch_maximal_saturation = neighbor_bunch_mean_saturation;
						ColoredIntervalsStructure1->UpperSatValue[next_bunch] = neighbor_bunch_mean_saturation;
					}//last_cor14.12.15
					min_sat_dist = std::min(neighbor_bunch_maximal_saturation - neighbor_bunch_mean_saturation,
						neighbor_bunch_mean_saturation - neighbor_bunch_minimal_saturation);
					left_sat = neighbor_bunch_mean_saturation - min_sat_dist;
					right_sat = min_sat_dist + neighbor_bunch_mean_saturation;
					abs_satur = abs(bunch_mean_saturation - left_sat);
					abs_satur1 = abs(bunch_mean_saturation - right_sat);
					abs_satur = std::min(abs_satur, abs_satur1);
					possible_sat_variation = dev_mean_sat[neighbor_bunch_mean_saturation];
					possible_sat_variation1 = dev_mean_sat1[neighbor_bunch_mean_saturation];
					impossible_sat_variation = imp_dev_mean_sat[neighbor_bunch_mean_saturation];
					impossible_sat_variation1 = imp_dev_mean_sat1[neighbor_bunch_mean_saturation];
					if (neighbor_bunch_mean_saturation >= bunch_mean_saturation)
					{//last_cor11.09.13
						saturation_distance = neighbor_bunch_mean_saturation - bunch_mean_saturation;
						saturation_distance1 = 0;
						poss_sat_variation = dev_mean_sat[bunch_mean_saturation];
						poss_sat_variation = std::max(possible_sat_variation1, poss_sat_variation);
						imp_sat_var = imp_dev_mean_sat[bunch_mean_saturation];
						imp_sat_var = std::max(impossible_sat_variation1, imp_sat_var);//last_cor14.12.15
					}
					else
					{
						saturation_distance1 = bunch_mean_saturation - neighbor_bunch_mean_saturation;
						saturation_distance = 0;
						poss_sat_variation = dev_mean_sat1[bunch_mean_saturation];
						poss_sat_variation = std::max(poss_sat_variation, possible_sat_variation);
						imp_sat_var = imp_dev_mean_sat1[bunch_mean_saturation];
						imp_sat_var = std::max(imp_sat_var, impossible_sat_variation);//last_cor08.12.15
					}
					max_satur_differ = std::max(saturation_distance, saturation_distance1);
					saturation_difference =
						abs(bunch_mean_saturation - neighbor_bunch_mean_saturation);
					Intersection_saturation = ColorInterval[strip_number].Intersection_measure_ratios(left_sat, right_sat, left_sat_bunch,
						right_sat_bunch, &indic_length_satur, &ratio_length_satur, &ratio_length1_satur);
					if (Intersection_saturation == 3)
					{//last_cor08.12.15
						if (ratio_length_satur>2 * imp_sat_var)
						{
							saturation_close = -2;
						}
						else
						{
							if (ratio_length_satur>imp_sat_var)
							{
								saturation_close = -1;
							}
						}
					}//last_cor08.12.15
					if (!saturation_close)
					{//last_cor08.12.15
						if (saturation_difference>imp_sat_var)
						{
							saturation_close = -2;
						}
					}//last_cor08.12.15
					if (Intersection_saturation == 3)
					{
						ratio_length_satur = 0;//???
					}
					if (saturation_difference <= 1)
					{//last_cor16.12.15
						saturation_close = 1;
					}//last_cor16.12.15
					bunch_mean_gray = ColoredIntervalsStructure->AverageGray[previous_bunch];
					bunch_minimal_gray = ColoredIntervalsStructure->LowerGrayscale[previous_bunch];
					bunch_maximal_gray = ColoredIntervalsStructure->UpperGrayscale[previous_bunch];
					min_bunch_gray_dist = std::min(bunch_maximal_gray - bunch_mean_gray, bunch_mean_gray - bunch_minimal_gray);
					left_bunch_gray = bunch_mean_gray - min_bunch_gray_dist;
					right_bunch_gray = min_bunch_gray_dist + bunch_mean_gray;
					bunch_gray_length = 2 * min_bunch_gray_dist + 1;
					zone_gray_bunch = gray_zones[bunch_mean_gray];
					neighbor_bunch_mean_gray =
						ColoredIntervalsStructure1->AverageGray[next_bunch];
					neighbor_minimum_gray = ColoredIntervalsStructure1->LowerGrayscale[next_bunch];
					neighbor_maximum_gray = ColoredIntervalsStructure1->UpperGrayscale[next_bunch];
					min_gray_dist = std::min(neighbor_maximum_gray - neighbor_bunch_mean_gray, neighbor_bunch_mean_gray - neighbor_minimum_gray);
					left_gray = neighbor_bunch_mean_gray - min_gray_dist;
					right_gray = min_gray_dist + neighbor_bunch_mean_gray;
					union_gray_left = std::min(left_bunch_gray, left_gray);
					union_gray_right = std::max(right_bunch_gray, right_gray);
					union_gray_length = union_gray_right - union_gray_left + 1;
					zone_gray = gray_zones[neighbor_bunch_mean_gray];
					neighbor_gray_length = 2 * min_gray_dist + 1;
					difference_gray_zone = abs(zone_gray_bunch - zone_gray);
					possible_gray_variation = std::min(dev_mean[neighbor_bunch_mean_gray / 8], dev_mean[bunch_mean_gray / 8]);
					impossible_gray_variation = std::min(imp_dev_mean[neighbor_bunch_mean_gray / 8],
						imp_dev_mean[bunch_mean_gray / 8]);//last_cor5.07.13
					maximum_zone_gray = std::max(zone_gray, zone_gray_bunch);
					minimum_zone_gray = std::min(zone_gray, zone_gray_bunch);
					gray_difference = abs(bunch_mean_gray - neighbor_bunch_mean_gray);
					if (Intersection_hue == 3)
					{//last_cor10.03.15
						if ((min_saturation >= 1) && ((max_saturation >= 2) || ((max_saturation >= 1) && (minimum_zone_gray >= 3))))
						{//last_cor11.03.15
							if (minimum_zone_gray >= 2)
							{
								if ((ratio_length_hue>2) && (hue_difference >= 6) && (difference_hue_zone >= 1))
								{
									if (minimum_zone_gray<6)
									{
										hue_close = -1;
									}
								}

							}
							else
							{//last_cor16.03.15
								if (max_saturation >= 6)
								{
									if (difference_hue_zone >= 1)
									{
										if ((ratio_length_hue>3) && (hue_difference >= 6))
										{
											hue_close = -1;
										}
									}
								}
							}//last_cor16.03.15
						}
					}//last_cor10.03.15

					 /*if(union_gray_length<=possible_gray_variation)
					 {//last_cor19.06.14
					 gray_ntersection=0;
					 gray_ntersection1=0;
					 gray_ntersection2=0;
					 }
					 else
					 {*/
					gray_ntersection = ColorInterval[strip_number].Intersection_measure_ratios(left_gray, right_gray, left_bunch_gray,
						right_bunch_gray, &indic_length_gray, &ratio_length_gray, &ratio_length1_gray);
					gray_ntersection1 = ColorInterval[strip_number].Intersection_measure_ratios(left_gray, right_gray, left_bunch_gray,
						bunch_mean_gray, &indic_length_gray1, &ratio_length_gray1, &ratio_length1_gray1);
					gray_ntersection2 = ColorInterval[strip_number].Intersection_measure_ratios(left_gray, right_gray, bunch_mean_gray,
						right_bunch_gray, &indic_length_gray2, &ratio_length_gray2, &ratio_length1_gray2);
					//}
					if (gray_ntersection == 3)
					{//intersection is empty
						gray_hole = ratio_length_gray;//last_cor24.12.15
						if (union_gray_length>(possible_gray_variation + (impossible_gray_variation - possible_gray_variation) / 2))
						{

							if (((ratio_length_gray + 1) >= bunch_gray_length) || ((ratio_length_gray + 1) >= neighbor_gray_length))
							{
								wide_gray_gap = 1;
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
					hue_difference=std::min(hue_difference,NUM_HUES-hue_difference);*/

					/* difference_hue_zone=abs(zone_hue_bunch-zone_hue);
					difference_hue_zone=std::min(difference_hue_zone,5-difference_hue_zone);*/
					/*hue_dif=corrected_neighbor_bunch_maximal_hue-corrected_neighbor_bunch_mean_hue;
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


					if ((maximum_zone_gray <= 1) && (gray_difference <= possible_gray_variation))
					{//last_cor30.07.15
						if (!grayscale_close)
						{
							grayscale_close = 1;
						}
					}
					if ((gray_difference >= impossible_gray_variation) || ((wide_gray_gap) && (difference_gray_zone>1)))
					{
						if (!grayscale_close)
						{//last_cor18.05.15
							if (2 * gray_difference >= 3 * impossible_gray_variation)
							{//last cor
								grayscale_close = -3;
							}
							else
							{//last_cor18.05.15
								grayscale_close = -2;
							}//last_cor18.05.15
						}//last_cor18.05.15
					}
					else
					{//egray
						if (union_gray_length<possible_gray_variation)
						{
							grayscale_close = 1;
						}
						else
						{
							if ((2 * gray_difference <= possible_gray_variation) &&
								(((bunch_mean_gray >= left_gray) && (bunch_mean_gray <= right_gray)) ||
								((neighbor_bunch_mean_gray >= left_bunch_gray) && (neighbor_bunch_mean_gray <= right_bunch_gray))))
							{//gr2
								grayscale_close = 1;
							}//gr2
							else
							{
								if ((gray_difference <= possible_gray_variation) &&
									((((bunch_mean_gray >= left_gray) && (bunch_mean_gray <= right_gray)) ||
									((neighbor_bunch_mean_gray >= left_bunch_gray) && (neighbor_bunch_mean_gray <= right_bunch_gray)))))
								{//gr3
									if ((gray_difference <= (2 * possible_gray_variation) / 3) ||
										((gray_ntersection >= 0) && (gray_ntersection <= 1) && (gray_difference<possible_gray_variation)))
									{//last_cor21.12
										if ((3 * gray_difference<2 * possible_gray_variation) && (gray_ntersection <= 1) &&
											((ratio_length_gray >= 14) || (ratio_length1_gray >= 14)))
										{//last_cor25.12.15
											grayscale_close = 1;
										}
										else
										{
											grayscale_close = 2;
										}
									}
									else
									{
										if ((gray_ntersection <= 1) || (union_gray_length <= possible_gray_variation))
										{
											grayscale_close = 2;
										}
										else
										{
											grayscale_close = 3;
										}
									}
								}//gr3
								else
								{

									if ((maximum_zone_gray <= 2) || ((maximum_zone_gray <= 3) && (minimum_zone_gray <= 2)))
									{
										if ((gray_difference <= possible_gray_variation) && (gray_ntersection <= 1) &&
											((!gray_ntersection1) || (!gray_ntersection2)))
										{
											if ((gray_difference <= (2 * possible_gray_variation) / 3))
											{//gd<
												if (wide_gray_gap)
												{
													grayscale_close = 3;
												}
												else
												{
													//if(gray_ntersection<2)
													//{//last_cor19.06.14
													grayscale_close = 2;
													//}
												}
											}//gd<
											else
											{
												grayscale_close = 3;
											}
										}
										else
										{
											if ((gray_difference <= possible_gray_variation) ||
												(((bunch_mean_gray >= left_gray) && (bunch_mean_gray <= right_gray)) ||
												((neighbor_bunch_mean_gray >= left_bunch_gray) && (neighbor_bunch_mean_gray <= right_bunch_gray))))
											{
												if ((gray_difference <= (2 * possible_gray_variation) / 3))
												{//last_cor14.12.15
													if (wide_gray_gap)
													{
														grayscale_close = 2;
													}
													else
													{
														if (gray_ntersection<2)
														{//last_cor19.06.14
															grayscale_close = 1;
														}
													}
												}//last_cor14.12.15
												else
												{
													if (gray_difference <= possible_gray_variation)
													{//last_cor14.12.15
														grayscale_close = 3;
													}//last_cor14.12.15
												}
											}
										}
									}
									else
									{//e
										if (gray_difference <= possible_gray_variation)
										{//gdpgv
											if (2 * gray_difference <= possible_gray_variation)
											{//last_cor11.04.14
												grayscale_close = 1;
											}//last_cor11.04.14
											else
											{//egc1
												if (3 * gray_difference <= 2 * possible_gray_variation)
												{//!!!
													if (minimum_zone_gray >= 5)
													{//last_cor31.8.15
														grayscale_close = 1;
													}
													else
													{
														grayscale_close = 2;
													}//last_cor31.8.15
												}
												else
												{//e2
													if ((gray_difference<possible_gray_variation) && ((gray_ntersection <= 1) || (gray_ntersection1 <= 1) ||
														(gray_ntersection2 <= 1)))
													{//last_cor27.05.13
														grayscale_close = 2;
													}
													else
													{//e1
														if (4 * gray_difference<3 * possible_gray_variation)
														{//last_cor26.03.14
															grayscale_close = 2;
														}//last_cor26.03.14
														else
														{
															if (minimum_zone_gray >= 6)
															{//last_cor01.09.15
																grayscale_close = 2;
															}
															else
															{
																if (zone_hue == 3)
																{//last_cor25.11.15
																	if (min_saturation >= 6)
																	{
																		if (minimum_zone_gray >= 3)
																		{
																			grayscale_close = 2;
																		}
																	}
																}//last_cor25.11.15
																else
																{
																	grayscale_close = 3;
																}
															}
														}
													}//e1
												}//e2
											}//egc1
										}//gdpgv
										else
										{//e1
											if ((bunch_occurrence>0) && (Density_ratio >= 6) && (Density_ratio <= 10))
											{//bo
												if (maximum_zone_gray >= 3)
												{//m=3
													if ((gray_ntersection == 3) && (wide_gray_gap))
													{//gr_i=3
														if (gray_difference>(possible_gray_variation + (impossible_gray_variation - possible_gray_variation) / 2))
														{
															if ((ratio_length_gray>(2 * possible_gray_variation / 3)))
															{
																grayscale_close = -2;
															}
															else
															{
																if ((ratio_length_gray>(possible_gray_variation / 2)))
																{
																	grayscale_close = -1;
																}
															}
														}

													}//gr_i=3
												}//m=3
												else
												{//em=3
													if (maximum_zone_gray >= 5)
													{//mzg
														if ((gray_difference>possible_gray_variation) && (gray_ntersection == 3))
														{
															grayscale_close = -2;
														}
														else
														{
															if (((gray_ntersection == 3) || ((gray_ntersection >= 2) && ((ratio_length1_gray <= 3) || (ratio_length_gray <= 3)))) &&
																(((bunch_mean_gray<left_gray) || (bunch_mean_gray>right_gray)) &&
																((neighbor_bunch_mean_gray<left_bunch_gray) || (neighbor_bunch_mean_gray>right_bunch_gray))))
															{
																grayscale_close = -1;
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
					if (grayscale_close == 0)
					{//gc0
						if (gray_difference <= possible_gray_variation)//?!
						{
							if (2 * (gray_difference - 1) <= possible_gray_variation)
							{//last_cor11.04.14
								grayscale_close = 1;
							}
							else
							{//e2g
								if (3 * gray_difference <= 2 * possible_gray_variation)
								{//!!!

									if ((gray_ntersection <= 2) || (union_gray_length <= possible_gray_variation))
									{//last_cor20.06.14
										grayscale_close = 2;
									}
								}
								else
								{
									if (minimum_zone_gray <= 1)
									{
										grayscale_close = 2;
									}
									else
									{
										grayscale_close = 3;
									}
								}//egc
							}//e2g
						}//?!
						if ((hue_close == 1) && (saturation_close == 1) && (gray_difference<impossible_gray_variation) &&
							(ratio_length >= 13) && (ratio_length1 >= 13))
						{
							if ((!difference_gray_zone) || ((zone_gray >= 1) && (difference_gray_zone <= 1)))
							{//last_cor14.12.15
								grayscale_close = 3;
							}//last_cor14.12.15
						}
						else
						{//e1
							if (gray_difference>(possible_gray_variation + (impossible_gray_variation - possible_gray_variation) / 2))
							{//gd>=last_cor22.12.15
								if ((bunch_mean_gray<left_gray) || (bunch_mean_gray>right_gray) || (ratio_length_gray<3) ||
									(ratio_length1_gray<3))
								{
									if (gray_ntersection == 3)
									{//last_cor26.11.15
										grayscale_close = -2;
									}//last_cor26.11.15
								}
								else
								{
									if (gray_ntersection == 3)
									{//last_cor26.11.15
										grayscale_close = -1;
									}//last_cor26.11.15
								}

							}//gd>=
							else
							{//e2
								if (gray_difference <= (possible_gray_variation + (impossible_gray_variation - possible_gray_variation) / 3))
								{//gd<
									gray_ntersection3 = ColorInterval[strip_number].Intersection_measure_ratios(bunch_minimal_gray,
										bunch_maximal_gray, neighbor_minimum_gray, neighbor_maximum_gray, &indic_length_gray3,
										&ratio_length_gray3, &ratio_length1_gray3);

									if ((Intersection_Measure <= 1) && (std::min(ratio_length, ratio_length1) >= 10))
									{
										if (gray_ntersection3 <= 1)
										{
											grayscale_close = 2;
										}
									}

									if (gray_ntersection3 == 3)
									{
										ratio_length_gray3 = 0;
									}
									if ((ratio_length_gray3 >= 10) && (ratio_length1_gray3 >= 10))
									{
										if ((ratio_length_gray>8) || (ratio_length1_gray>8))
										{//last_cor10.06.14
											grayscale_close = 3;
										}
									}
									if (gray_difference>possible_gray_variation)
									{
										if (!grayscale_close)
										{
											grayscale_close = 4;
										}
									}
								}//gd<
								else
								{//e3
									if (gray_difference>(possible_gray_variation + (impossible_gray_variation - possible_gray_variation) / 2))
									{//last_cor03.06.13
										if ((gray_ntersection >= 2) && ((ratio_length1_gray <= 2) || (ratio_length1_gray <= 2)))
										{
											grayscale_close = -1;
										}
									}
								}//e3
							}//e2
						}//e1
					}//gc0
					if (!grayscale_close)
					{//last_cor23.05.14
						if (gray_difference>possible_gray_variation)
						{
							if (gray_ntersection == 3)
							{
								if (((difference_gray_zone >= 1) && (wide_gray_gap)) || (difference_gray_zone >= 2))
								{
									grayscale_close = -1;
								}
							}

						}
					}
					if ((hue_close<0) && (saturation_close<0) && (grayscale_close != 0))
					{//last_cor08.12.15
						prior = 5;
						goto L;
					}
					if ((hue_close == 1) && (saturation_close == 1) && (grayscale_close == 1))
					{
						prior = 1;
						goto L;
					}
					if ((bunch_mean_saturation == 0) && (neighbor_bunch_mean_saturation == 0))
					{//s==0
						saturation_close = 1;
						if ((!Intersection_Measure) && (ratio_length >= 10) && (ratio_length1 >= 10))
						{//last_cor15.02.18
							if ((grayscale_close >= 1) && (grayscale_close <= 2))
							{
								if (difference_hue_zone <= 1)
								{
									hue_close = 1;
									prior = 1;
									goto L;
								}
								else
								{
									hue_close = 0;
									prior = 2;
									goto L;
								}
							}
						}
						if (maximum_zone_gray <= 1)
						{//last_cor14.04.14
							if (grayscale_close>0)
							{
								grayscale_close = 1;
							}
							if (difference_hue_zone <= 1)
							{
								hue_close = 1;
							}
						}
						else//last_cor27.05.15
						{
							if (minimum_zone_gray >= 3)
							{//last_cor08.06.15
								if ((ratio_length >= 6) && (ratio_length1 >= 13))
								{//last_cor25.12.15
									if (grayscale_close == 1)
									{
										if (!hue_close)
										{
											if (difference_hue_zone <= 1)
											{
												hue_close = 1;
											}
											else
											{

												hue_close = 2;

											}
										}

										prior = 1;
										goto L;
									}
									else
									{//last_cor18.07.16
										if ((!grayscale_close) || (grayscale_close>0))
										{
											if (gray_difference <= possible_gray_variation)
											{
												prior = 2;
												grayscale_close = 2;
												goto L;
											}
										}
									}//last_cor18.07.16
								}//last_cor25.12.15
								if (((ratio_length >= 15) && (ratio_length1 >= 10))
									|| ((ratio_length >= 10) && (ratio_length1 >= 15)))
									if ((grayscale_close == 1) || ((grayscale_close == 2) && (gray_ntersection<2)))
									{
										if (difference_hue_zone <= 1)
										{
											if (!hue_close)
											{
												if (hue_difference <= 8)
												{
													hue_close = 1;
												}
												else
												{
													hue_close = 2;
												}
											}
										}
										prior = 1;
										goto L;
									}
							}//last_cor08.06.15
							if (minimum_zone_gray >= 5)
							{
								if (((gray_ntersection >= 0) && (gray_ntersection <= 1)) ||
									(gray_difference <= possible_gray_variation / 2))
								{//last_cor30.09.15and29.04.16
									if (grayscale_close == 1)
									{
										if (gray_difference <= possible_gray_variation / 2)
										{//last_cor29.04.16

											if (difference_hue_zone <= 1)
											{
												hue_close = 1;
											}
											else
											{
												hue_close = 0;
											}
											prior = 1;
											goto L;
										}
									}
								}//last_cor30.09.15
								else
								{//last_cor27.04.16
									if (!difference_hue_zone)
									{
										hue_close = 1;
									}
									else
									{//e1
										if (difference_hue_zone <= 1)
										{
											hue_close = 2;
										}
										else
										{
											hue_close = 0;
										}
									}//e1
									if (grayscale_close == 1)
									{//gc=1
										if (hue_close == 1)
										{
											prior = 1;
											goto L;
										}
										else
										{
											prior = 2;
											goto L;
										}

									}//gc=1
									else
									{//e2
										if (gray_difference <= possible_gray_variation)
										{
											grayscale_close = 2;
											if (hue_close == 1)
											{
												prior = 2;
												goto L;
											}
											else
											{
												if (hue_close >= 0)
												{
													prior = 3;
													goto L;
												}
											}
										}
									}//e2
								}//last_cor27.04.16

								if (maximum_zone_gray == 7)
								{//last_cor04.06.15
									if (gray_difference <= 2)
									{
										if (!difference_hue_zone)
										{
											hue_close = 1;
										}
										else
										{
											hue_close = 0;
										}
										prior = 1;
										goto L;
									}
								}//last_cor04.06.15
								if (grayscale_close>0)
								{
									if (!difference_hue_zone)
									{
										if (zone_hue == 4)
										{
											if (hue_difference <= 8)
											{
												if (grayscale_close <= 2)
												{
													hue_close = 1;
													prior = 1;
													goto L;
												}
												else
												{
													hue_close = 2;
													prior = 1;
													goto L;
												}
											}
											else
											{
												if (grayscale_close <= 2)
												{
													hue_close = 2;
													prior = 2;
													goto L;
												}
												else
												{
													hue_close = 3;
													prior = 3;
													goto L;
												}
											}
										}
									}
								}
							}
						}//last_cor27.05.15
						if (grayscale_close>0)
						{//last_cor27.10.17
							if (maximum_zone_gray <= 4)
							{
								if (grayscale_close == 1)
								{
									if (!difference_hue_zone)
									{
										hue_close = 1;
										prior = 1;
										goto L;
									}
								}
								if (grayscale_close == 2)
								{
									if (!Intersection_Measure)
									{
										if (!difference_hue_zone)
										{
											hue_close = 1;
											prior = 1;
											goto L;
										}
										else
										{
											if ((difference_hue_zone <= 1) && (std::max(zone_hue_bunch, zone_hue) == 4))
											{
												hue_close = 2;
												prior = 2;
												goto L;
											}
										}
									}
								}
							}
						}//last_cor27.10.17
						if (grayscale_close<0)
						{//lacor
							if (grayscale_close <= -2)
							{
								if (2 * gray_difference>3 * impossible_gray_variation)
								{
									prior = 5;
									goto L;
								}
								else
								{
									prior = 6;
									goto L;
								}
							}
							else
							{
								if (gray_difference>impossible_gray_variation)
								{
									prior = 6;
									goto L;
								}
								else
								{
									prior = 7;
									goto L;
								}
							}
						}//lacor
						if ((grayscale_close == 1) && (gray_difference <= (possible_gray_variation / 2)))
						{//gsc
							if ((gray_ntersection <= 1) && ((ratio_length1_gray >= 8) || (ratio_length_gray >= 8)))
							{//last_cor19.07.16
								prior = 1;
								goto L;
							}//last_cor19.07.16
							if (!difference_hue_zone)
							{//last_cor27.11.14
								hue_close = 1;
								prior = 1;
								goto L;
							}
							else
							{//e!dhz
								if (difference_hue_zone <= 1)
								{//dhz1
									if (std::max(zone_hue_bunch, zone_hue) >= 2)
									{//std::max
										if (hue_difference <= 4)
										{
											hue_close = 1;
											prior = 1;
											goto L;
										}
										else
										{//e
											if (hue_difference <= 8)
											{
												hue_close = 2;
												prior = 1;
												goto L;
											}
										}//e
									}//std::max

								}//dhz1//last_cor27.11.14
								else
								{//last_cor28.03.16
									if (((ratio_length >= 13) && (ratio_length1 >= 9))
										|| ((ratio_length >= 9) && (ratio_length1 >= 13)))
									{
										hue_close = 0;
										prior = 1;
										goto L;
									}
								}//last_cor28.03.16
							}//e!dhz
						}//gsc

						if ((grayscale_close>0) && (grayscale_close <= 2) && (difference_hue_zone <= 1))
						{//grayscale and hue_zones restrictions
							if (difference_hue_zone == 0)
							{//the same zone

								if ((grayscale_close == 1) || (zone_gray <= 2))
								{
									prior = 1;
									goto L;
								}
								else
								{
									if (zone_gray <= 4)
									{
										prior = 2;
										goto L;
									}
									else
									{
										prior = 3;
										goto L;
									}

								}
							}//the same zone
							if (zone_gray <= 2)
							{
								if (grayscale_close == 1)
								{
									prior = 1;
								}
								else
								{
									prior = 2;
								}
								goto L;
							}
							else
							{//zgr>2
								if ((grayscale_close >= 1) && (grayscale_close <= 2) && (zone_gray <= 5))
								{//zg5
									if (!difference_hue_zone)
									{//dhz
										if (grayscale_close == 1)
										{
											prior = 1;
										}
										else
										{
											prior = 2;
										}
										goto L;
									}//dhz
									else
									{//!dhz
										if (hue_difference >= 12)
										{
											prior = 6;
											goto L;
										}
										else
										{//e>=12
											if (hue_difference >= 8)
											{
												if ((grayscale_close == 0) || (grayscale_close >= 2))
												{
													prior = 7;
													goto L;
												}
											}
											else
											{//e>=8
												if (hue_difference <= 4)
												{//h<=4
													if (grayscale_close == 1)
													{
														prior = 2;
														goto L;
													}
													else
													{//egs1
														if (grayscale_close == 2)
														{
															prior = 3;
															goto L;
														}
													}//egs1
												}//h<=4
												else
												{//eh<=4
													if (grayscale_close == 1)
													{
														prior = 3;
														goto L;
													}
													else
													{
														if (grayscale_close == 2)
														{
															prior = 4;
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
									if ((grayscale_close <= 2) && (zone_gray <= 4) && (gray_difference <= possible_gray_variation))
									{
										prior = 2;
										goto L;
									}
								}

							}//zgr>2
						}//grayscale and hue_zones restrictions

						if (((difference_hue_zone>2) && (grayscale_close<0)) || (grayscale_close<-1))
						{
							prior = 15;
							goto L;
						}
						else
						{
							if ((difference_hue_zone>2) && (grayscale_close == 0))
							{
								prior = 16;
								goto L;
							}
							if (grayscale_close<0)
							{
								prior = 17;
								goto L;
							}
						}
						prior = 18;
						goto L;
					}//both s=0
					if ((bunch_mean_saturation <= 1) && (neighbor_bunch_mean_saturation <= 1))
					{//s<=1
						saturation_close = 1;
						if (!saturation_difference)
						{//last_cor21.08.17
							if (grayscale_close == 1)
							{
								if (((ratio_length_gray >= 13) && (ratio_length1_gray >= 12)) ||
									((ratio_length_gray >= 8) && (ratio_length1_gray >= 8) && (gray_difference <= possible_gray_variation / 2)) ||
									((ratio_length_gray >= 15) && (gray_difference <= possible_gray_variation / 2)) ||
									((ratio_length1_gray >= 15) && (gray_difference <= possible_gray_variation / 2)))
								{
									if ((ratio_length >= 12) && (ratio_length1 >= 12))
									{
										if (!difference_hue_zone)
										{
											if (!hue_close)
											{
												hue_close = 1;
											}
											else
											{
												if (hue_close<0)
												{
													hue_close = 3;
												}
											}
										}
										else
										{
											hue_close = 0;
										}

										saturation_close = 1;
										prior = 1;
										goto L;
									}
								}
							}
						}//last_cor21.08.17
						if (grayscale_close == 1)
						{//last_cor20.02.18

							if ((!Intersection_Measure) && (ratio_length >= 8) && (ratio_length1 >= 8) && (len_ratio >= 7) && (len_ratio <= 9))
							{
								if (minimum_zone_gray >= 6)
								{
									if (!difference_hue_zone)
									{
										hue_close = 1;
										prior = 1;
										goto L;
									}
									else
									{
										if (difference_hue_zone == 1)
										{
											hue_close = 2;
											prior = 1;
											goto L;
										}
									}
								}
							}
						}//last_cor20.02.18
						if (hue_close>0)
						{//last_cor18.10.17

							if (grayscale_close>0)
							{
								if ((grayscale_close >= 1) && (grayscale_close <= 2))
								{
									prior = 1;
									goto L;
								}
								if ((!Intersection_Measure) && (ratio_length >= 12) && (ratio_length1 >= 12))
								{
									if (gray_difference<possible_gray_variation)
									{
										grayscale_close = 2;
										prior = 1;
										goto L;
									}

								}
							}
							else
							{
								if (!grayscale_close)
								{
									if ((!Intersection_Measure) && (ratio_length >= 12) && (ratio_length1 >= 12) && (ratio_length1>ratio_length))
									{
										if (gray_difference <= possible_gray_variation)
										{
											grayscale_close = 3;
											prior = 2;
											goto L;
										}

									}
								}
							}
						}//last_cor18.10.17
						if (hue_close <= 0)
						{//last_cor16..11.16
							if ((min_saturation == 1) && (minimum_zone_gray >= 2))
							{//ms=1
								if ((zone_hue == 3) && (zone_hue_bunch == 3))
								{
									if (((bunch_mean_hue >= 18) && (hue_difference >= 6)) ||
										((neighbor_bunch_mean_hue >= 18) && (hue_difference >= 6)))
									{
										hue_close = -2;
										if ((gray_difference>possible_gray_variation) && (grayscale_close <= 0))
										{
											grayscale_close = -2;
										}
										if (grayscale_close>0)
										{
											prior = 6;
											goto L;
										}
										else
										{
											prior = 5;
											goto L;
										}
									}
								}
							}//ms=1
						}//last_cor16..11.16
						if (maximum_zone_gray <= 1)
						{//last_cor14.04.14
							if (grayscale_close>0)
							{
								grayscale_close = 1;
							}
							if (!difference_hue_zone)
							{
								hue_close = 1;
							}
						}
						else//last_cor27.05.15
						{
							if (!min_saturation)
							{//last_cor08.06.15
								if (minimum_zone_gray >= 3)
								{//last_cor08.06.15
									if (((ratio_length >= 15) && (ratio_length1 >= 10))
										|| ((ratio_length >= 10) && (ratio_length1 >= 15)))
										if ((grayscale_close == 1) || ((grayscale_close == 2) && (gray_ntersection<2)))
										{
											if (!difference_hue_zone)
											{
												if (hue_difference<8)
												{
													hue_close = 1;
												}
												else
												{
													hue_close = 2;
												}
											}
											prior = 1;
											goto L;
										}
								}//last_cor08.06.15
							}//last_cor08.06.15
							if (minimum_zone_gray >= 5)
							{
								if (maximum_zone_gray == 7)
								{//last_cor04.06.15
									if (gray_difference <= 2)
									{
										prior = 1;
										goto L;
									}
									if (grayscale_close == 1)
									{
										if ((min_hue_zone >= 3) && (max_hue_zone == 4))
										{//mhz)
											if ((ratio_length1 >= 12) && (ratio_length >= 14))
											{
												if (hue_difference <= 6)
												{
													hue_close = 1;
													prior = 1;
													goto L;
												}
												else
												{
													if (hue_difference <= 8)
													{
														hue_close = 2;
														prior = 1;
														goto L;
													}
													else
													{
														if (hue_difference <= 10)
														{
															hue_close = 3;
															prior = 2;
															goto L;
														}
													}
												}
											}
										}//mhz
									}
								}//last_cor04.06.15
								if (grayscale_close>0)
								{
									if (!difference_hue_zone)
									{
										if (zone_hue == 4)
										{
											if (hue_difference <= 8)
											{
												if (grayscale_close <= 2)
												{
													hue_close = 1;
													prior = 1;
													goto L;
												}
												else
												{
													hue_close = 2;
													prior = 1;
													goto L;
												}
											}
											else
											{
												if (grayscale_close <= 2)
												{
													hue_close = 2;
													prior = 2;
													goto L;
												}
												else
												{
													hue_close = 3;
													prior = 3;
													goto L;
												}
											}
										}
									}
								}
							}
						}//last_cor27.05.15
						if (grayscale_close<0)
						{//lacor
							if (grayscale_close <= -2)
							{
								if (2 * gray_difference>3 * impossible_gray_variation)
								{
									prior = 5;
									goto L;
								}
								else
								{
									prior = 6;
									goto L;
								}
							}
							else
							{
								if (gray_difference>impossible_gray_variation)
								{
									prior = 6;
									goto L;
								}
								else
								{
									prior = 7;
									goto L;
								}
							}
						}//lacor
						if ((minimum_zone_gray >= 3) && (maximum_zone_gray <= 4) && (grayscale_close == 1))
						{//last_cor17.03.17
							if ((difference_hue_zone <= 1))
							{
								if (!hue_close)
								{
									if (hue_difference <= 6)
									{
										hue_close = 1;
									}
									else
									{
										hue_close = 2;
									}
								}
								if (gray_difference <= possible_gray_variation / 2)
								{
									prior = 1;
								}
								else
								{
									prior = 2;
								}
								goto L;
							}
						}//last_cor17.03.17
						if ((zone_gray <= 1) || (zone_gray_bunch <= 1))
						{//zg1g
							if ((grayscale_close == 1) && (difference_hue_zone <= 1))
							{
								prior = 1;
								goto L;
							}
							else
							{//last_cor
								if ((grayscale_close>0) && (grayscale_close <= 2) && (hue_close>0) && (saturation_close>0))
								{
									prior = 1;
									goto L;
								}
							}
						}//zg1g
						else
						{
							if ((zone_gray <= 2) || (zone_gray_bunch <= 2))//last_cor 21.04.13
							{//zg2
								if (!hue_close)
								{//last_cor18.03.15
									if (!difference_hue_zone)
									{
										if (zone_hue>2)
										{
											if (hue_difference <= 3)
											{
												hue_close = 1;
											}
										}

									}
								}//last_cor18.03.15
								if ((grayscale_close == 1) && (difference_hue_zone <= 1))
								{//gc1dhz1
									if ((hue_close == 1) && (saturation_close == 1))
									{//last_cor18.03.15
										prior = 1;
										goto L;
									}
									else
									{//last_cor18.03.15
										if (union_gray_length<possible_gray_variation)
										{//ugl
											if (hue_close>0)
											{//last_cor18.03.15
												prior = 1;
												goto L;
											}
											else
											{
												if (!difference_hue_zone)
												{
													prior = 2;
													goto L;
												}

											}

										}//last_cor18.03.15
									}//ugl
								}//gc1dhz1
							}//zg2
							else
							{//last_cor 10.06.13
								if (((zone_gray <= 4) || (zone_gray_bunch <= 4)) && (saturation_close == 1))
								{//zg4
									if ((gray_ntersection <= 1) && (max_saturation <= 1) && (grayscale_close == 1) &&
										(gray_difference <= possible_gray_variation / 2))
									{//last_cor19.09.13
										if (hue_close>0)
										{//last_cor27.10.14
											prior = 1;
											goto L;
										}



										if ((difference_hue_zone <= 1) && (((zone_hue>2) && (zone_hue<5)) ||
											((zone_hue_bunch>2) && (zone_hue_bunch<5))))
										{//dhz//last_cor11.04.14
											if ((hue_difference <= 4) || ((!difference_hue_zone) && (hue_difference <= 6)))
											{
												prior = 2;
												hue_close = 2;
												goto L;
											}
											else
											{//ehd
												if ((length_ratio <= 2) || (length_ratio >= 14))
												{//lr
													if (hue_difference <= 8)
													{
														prior = 3;
														hue_close = 3;
														goto L;
													}
													else
													{
														prior = 4;
														hue_close = -1;
														goto L;
													}
												}//lr
											}//ehd
										}//dhz
									}//hc

								}//zg4
								else
								{//last_cor09.06.14
									if (minimum_zone_gray >= 6)
									{//last_cor21.04.15
										if (grayscale_close == 1)
										{
											if (hue_difference <= 6)
											{
												hue_close = 1;
												prior = 1;
												goto L;
											}
										}
									}//last_cor21.04.15
									else
									{//last_cor21.04.15
										if ((zone_gray >= 5) && (zone_gray_bunch >= 5))
										{//zg5zgb5
											if ((grayscale_close == 1) && (!gray_ntersection))
											{//last_cor26.11.14and29.12.14
												if (!hue_close)
												{//hc0
													if (difference_hue_zone <= 1)
													{//dhz1
														if (!difference_hue_zone)
														{
															if (hue_difference <= 5)
															{
																hue_close = 1;
																prior = 1;
																goto L;
															}
															else
															{
																if (hue_difference <= 8)
																{
																	hue_close = 2;
																	prior = 1;
																	goto L;
																}
															}
														}
														else
														{//edhz0
															if ((zone_hue>2) || (zone_hue_bunch>2))
															{//zh>2
																if (hue_difference <= 3)
																{
																	hue_close = 1;
																	prior = 1;
																	goto L;
																}
																else
																{//hd3
																	if (hue_difference <= 5)
																	{
																		hue_close = 2;
																		prior = 2;
																		goto L;
																	}
																}//hd3
															}//zh>2
														}//edhz0
													}//dhz1
												}//dhz1
											}//last_cor26.11.14and29.12.14
											if ((!bunch_mean_saturation) || (!neighbor_bunch_mean_saturation))
											{//bmsndms
												if ((grayscale_close == 1) && (gray_difference <= possible_gray_variation / 2) &&
													(difference_hue_zone <= 1) && (gray_ntersection <= 1))
												{//logcond
													if (hue_close>0)
													{
														prior = 1;
														goto L;
													}
													else
													{//hc>0
														if (!difference_hue_zone)
														{//last_cor29.12.14
															if (hue_difference <= 6)
															{
																hue_close = 1;
																prior = 1;
																goto L;
															}
															else
															{
																if (hue_difference <= 8)
																{
																	hue_close = 2;
																	prior = 1;
																	goto L;
																}
															}
														}//last_cor29.12.14
														else
														{//edhz0
															if ((zone_hue>2) || (zone_hue_bunch>2))
															{//zh>2
																if (hue_difference <= 3)
																{
																	hue_close = 1;
																	prior = 1;
																	goto L;
																}
																else
																{//hd3
																	if (hue_difference <= 5)
																	{
																		hue_close = 2;
																		prior = 2;
																		goto L;
																	}
																}//hd3
															}//zh>2
														}//edhz0//last_cor29.12.14

													}//ehc>0
												}//logcond
											}//bmsndms
										}//zg5zgb5
									}//last_cor21.04.15
								}//last_cor09.06.14
							}//last_cor 10.06.13
						}
						if (zone_gray <= 2)
						{//zg1
							if (!hue_close)
							{//hue indefinite

								if ((hue_difference <= 4) || ((hue_difference <= 8) && (difference_hue_zone == 0)))
								{
									hue_close = 2;
								}
								else
								{//e0
									if ((hue_difference <= 6) && (difference_hue_zone == 1))
									{
										hue_close = 3;
									}
									else
									{
										if ((difference_hue_zone>1) || ((hue_difference >= 10) && (difference_hue_zone == 1)))
										{
											hue_close = -1;
										}
										else
										{
											if (difference_hue_zone>2)
											{
												hue_close = -2;

											}
										}
									}
								}//e0
							}//hue indefinite
							if ((grayscale_close == 1) && (difference_hue_zone <= 1))
							{
								if (hue_close>0)
								{
									prior = 1;
									goto L;
								}
								if (difference_hue_zone == 0)
								{
									prior = 2;
									goto L;
								}
							}
							if ((grayscale_close >= 1) && (grayscale_close <= 2) && (hue_close == 1))
							{//gh
								prior = 1;
								goto L;
							}//gh
							else
							{//e1
								if ((hue_close == 1) && (grayscale_close == 3) && (!saturation_difference))
								{
									prior = 2;
									goto L;
								}
								if ((hue_close>1) && (grayscale_close >= 1) && (grayscale_close<3))
								{
									prior = 2;
									goto L;
								}
								else
								{
									if (((hue_close == 1) && (grayscale_close == 3)) || ((hue_close == 3) && (grayscale_close == 1)))
									{
										if (difference_gray_zone == 0)
										{
											prior = 2;
											goto L;
										}
										else
										{
											prior = 3;
											goto L;
										}
									}
									else
									{
										if ((hue_close >= 2) && (grayscale_close>1))
										{
											if (grayscale_close <= 2)
											{
												prior = 2;
												goto L;
											}
											else
											{
												prior = 3;
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
							if ((grayscale_close == 1) && ((!neighbor_bunch_mean_saturation) ||
								(!bunch_mean_saturation)))
							{
								if (difference_hue_zone <= 1)
								{
									prior = 1;
									goto L;
								}
								else
								{
									prior = 2;
									goto L;
								}
							}
							if (((hue_close<0) && (grayscale_close<0)) || (hue_close<-1) || (grayscale_close<-1))
							{
								prior = 15;
								goto L;
							}
							else
							{
								if (hue_close<0)
								{
									prior = 16;
									goto L;
								}
								if (grayscale_close<0)
								{
									prior = 17;
									goto L;
								}
							}
							prior = 18;
							goto L;
						}//zg1
						else
						{//e2
							if ((hue_close == 1) && (!saturation_difference))
							{
								if ((grayscale_close >= 1) && (grayscale_close <= 2))
								{//last_cor29.05.15
									prior = 1;
									goto L;
								}
								else
								{
									if (grayscale_close>0)
									{//last_cor29.05.15
										prior = 2;
										goto L;
									}
									else
									{
									}
								}
							}
							if (!hue_close)
							{//hue indefinite
								if ((hue_difference <= 2) || ((difference_hue_zone == 0) && (hue_difference <= 3)))
								{
									hue_close = 2;
								}
								else
								{//e0
									if ((hue_difference <= 4) && (difference_hue_zone <= 1))
									{
										hue_close = 3;
									}
									else
									{
										if ((difference_hue_zone >= 2) || ((hue_difference >= 8) && (difference_hue_zone == 1)))
										{
											hue_close = -1;
										}
										else
										{
											if ((difference_hue_zone>2) || (hue_difference>12))
											{
												hue_close = -2;
											}
										}
									}
								}//e0
							}//hue indefinite

							if ((grayscale_close >= 1) && (grayscale_close <= 2))
							{//gh!!!!
								if (hue_close == 1)
								{
									prior = 1;
									goto L;
								}
								else
								{
									if (hue_close>0)
									{//last_cor27.10.14
										if (grayscale_close == 1)
										{
											prior = 1;
											goto L;
										}
										else
										{
											prior = 2;
											goto L;
										}

									}
								}
							}//gh
							else
							{//e1
								if ((hue_close>1) && (grayscale_close >= 1) && (grayscale_close<3))
								{
									prior = 2;
									goto L;
								}
								if ((hue_close == 2) && (grayscale_close == 3) && (gray_difference <= possible_gray_variation) &&
									(max_satur_differ == 0))
								{
									prior = 2;
									goto L;
								}
								if (((hue_close == 1) && (grayscale_close == 3)) || ((hue_close == 3) && (grayscale_close == 1)))
								{
									prior = 3;
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

							if (((hue_close<0) && (grayscale_close<0)) || (hue_close<-1))
							{
								prior = 15;
								goto L;
							}
							else
							{
								if ((hue_close<0) || (grayscale_close<-1))
								{
									prior = 16;
									goto L;
								}
								if (grayscale_close<0)
								{
									prior = 17;
									goto L;
								}
							}
							prior = 18;
							goto L;
						}//e2
					}//s<=1
					if ((bunch_mean_saturation <= 2) && (neighbor_bunch_mean_saturation <= 2))
					{//s<=2//16.07.13
						if ((min_hue_zone == 3) && (max_hue_zone == 3))
						{//mhzmaxhz
							if ((max_saturation == 2) && (!min_saturation))
							{//last_cor17.11.17
								if ((ratio_length <= 8) && (ratio_length1 <= 8))
								{
									saturation_close = -1;
									prior = 5;
									goto L;
								}
							}//last_cor17.11.17
							if (min_saturation == 2)
							{//ms2
								if (minimum_zone_gray >= 2)
								{//mzg
									if (((neighbor_bunch_mean_hue >= 21) && (bunch_mean_hue <= 17)) ||
										((neighbor_bunch_mean_hue <= 17) && (bunch_mean_hue >= 21)))
									{//huecond
										hue_close = -1;
										if ((grayscale_close <= 0))
										{
											prior = 5;
											goto L;
										}
										else
										{
											prior = 6;
											goto L;
										}
									}//huecond
								}//mzg
							}//ms2
						}//mhzmaxhz
						if (maximum_zone_gray <= 1)
						{//last_cor14.04.14
							if (grayscale_close>0)
							{
								grayscale_close = 1;
							}
							if (!difference_hue_zone)
							{
								hue_close = 1;
							}
						}
						if ((saturation_difference == 0) || ((bunch_mean_saturation >= 1) && (neighbor_bunch_mean_saturation >= 1)))
						{//last_cor28.05.15
							saturation_close = 1;
						}
						else
						{///condlog
							if (saturation_difference <= 1)
							{
								saturation_close = 1;
							}

							{//last_cor28.10.13
								if (saturation_difference >= imp_sat_var)
								{
									if ((zone_gray >= 2) && (zone_gray_bunch >= 2))
									{//last_cor06.04.15
										if (minimum_zone_gray <= 5)
										{
											saturation_close = -1;
										}
									}

								}
								else
								{
									saturation_close = 3;
								}
							}//last_cor28.10.13
						}///condlog
						if (minimum_zone_gray >= 6)
						{//last_cor31.08.15

							if ((grayscale_close >= 1) && (grayscale_close <= 2))
							{
								if (grayscale_close == 1)
								{//last_cor18.09.15
									if (!difference_hue_zone)
									{//dhz
										if ((min_hue_zone >= 3) && (max_hue_zone == 4))
										{//mhz
											if (hue_difference <= 4)
											{
												hue_close = 1;
											}
											else
											{
												if (hue_difference <= 7)
												{
													hue_close = 2;
												}
											}
											if (saturation_difference <= 1)
											{
												saturation_close = 1;
												if ((hue_close >= 1) && (hue_close <= 2))
												{
													prior = 1;
													goto L;
												}
											}
											else
											{
												saturation_close = 2;
												if (hue_close == 1)
												{
													prior = 1;
													goto L;
												}
											}
										}//mhz
									}//dhz
								}//last_cor18.09.15
								if (saturation_close == 1)
								{
									if ((min_hue_zone >= 3) && (max_hue_zone == 4))
									{
										if (hue_difference <= 4)
										{
											hue_close = 1;
											prior = 1;
											goto L;
										}
										else
										{
											if (hue_difference <= 8)
											{
												hue_close = 2;
												prior = 1;
												goto L;
											}
										}
									}
								}
							}
						}//last_cor31.08.15
						if ((zone_gray >= 3) && (zone_gray_bunch >= 3))
						{//last_cor29.10.14
							if ((difference_hue_zone >= 2) || ((difference_hue_zone >= 1) && (hue_difference>7)))
							{
								hue_close = -1;
							}
						}
						if (saturation_close == 1)
						{//last_cor26.03.14
							if (hue_close == 1)
							{//last_cor18.06.15
								if (grayscale_close <= 0)
								{
									if (minimum_zone_gray >= 6)
									{
										if (!difference_hue_zone)
										{
											if ((zone_hue >= 3) && (zone_hue <= 4))
											{
												if ((ratio_length >= 10) && (ratio_length1 >= 10))
												{
													if (gray_difference <=
														(possible_gray_variation + (impossible_gray_variation - possible_gray_variation) / 2))
													{
														grayscale_close = 2;
														prior = 1;
														goto L;
													}
												}
											}
										}
									}
								}
							}//last_cor18.06.15
							if ((zone_gray <= 4) && (zone_gray_bunch <= 4))
							{//last_cor26.03.14
								if (max_saturation <= 5)
								{//ms
									if (!hue_close)
									{//hc
										if ((zone_hue>1) && (zone_hue<4) && (zone_hue_bunch>1) && (zone_hue_bunch<4))
										{//zh
											if (hue_difference <= 3)
											{//last_cor04.12.14
												if (!difference_hue_zone)
												{
													hue_close = 1;
												}
												else
												{
													if (min_saturation == 2)
													{
														hue_close = 2;
													}
													else
													{
														hue_close = 1;
													}

												}

											}
											else
											{//e
												if (hue_difference <= 4)
												{
													if (!difference_hue_zone)
													{//last_cor04.12.14
														hue_close = 2;
													}
													else
													{
														if (min_saturation == 2)
														{
															hue_close = 3;
														}
														else
														{
															hue_close = 2;
														}
													}
												}

											}//e
										}//zh
									}//hc
								}//ms

							}//last_cor26.03.14
							if ((zone_gray >= 4) && (zone_gray_bunch >= 4))
							{//zg3
								if (((gray_difference<possible_gray_variation / 3) || (gray_difference <= 3)) && (gray_ntersection <= 1))
								{
									if (hue_difference <= 5)
									{
										if (!difference_hue_zone)
										{
											hue_close = 1;
											prior = 1;
											goto L;
										}
										else
										{
											if (difference_hue_zone <= 1)
											{//last_cor23.10.14
												if ((max_saturation <= 1) || (hue_difference <= 3))
												{
													hue_close = 2;
													prior = 2;
													goto L;
												}
												else
												{//last_cor24.10.14
													if (grayscale_close == 1)
													{
														if ((hue_difference <= 5) && (zone_hue >= 3) && (zone_hue_bunch >= 3))
														{
															hue_close = 3;
															prior = 3;
															goto L;
														}
													}
												}
											}
										}
									}
								}
							}//zg3
						}//last_cor24.03.14
						if (hue_close == 1)
						{//last_cor28.05.13
							if (saturation_close == 1)
							{
								if ((grayscale_close >= 1) && (grayscale_close <= 2))
								{
									prior = 1;
									goto L;
								}
								else
								{//last_cor03.06.13egc12
									if (grayscale_close >= 0)
									{//last_cor03.07.13gc>=0
										if (gray_difference<possible_gray_variation)
										{
											prior = 1;
											grayscale_close = 2;
											goto L;
										}
										else
										{//egc
											if (gray_difference<(possible_gray_variation + (impossible_gray_variation - possible_gray_variation) / 2))
											{
												if (gray_difference <= (possible_gray_variation + 1))
												{
													prior = 2;
													grayscale_close = 3;
													goto L;
												}
												else
												{
													prior = 3;
													grayscale_close = 4;
													goto L;
												}

											}
										}//egc
									}//gc>=0
								}//egc12

							}

						}
						if (grayscale_close<0)
						{//lacor
							if (grayscale_close <= -2)
							{
								if (2 * gray_difference>3 * impossible_gray_variation)
								{
									prior = 5;
									goto L;
								}
								else
								{
									prior = 6;
									goto L;
								}
							}
							else
							{
								if (gray_difference>impossible_gray_variation)
								{
									prior = 6;
									goto L;
								}
								else
								{
									prior = 7;
									goto L;
								}
							}
						}//lacor
						if ((saturation_difference == 0) || ((bunch_mean_saturation >= 1) && (neighbor_bunch_mean_saturation >= 1) &&
							(saturation_difference <= 1)))
						{
							saturation_close = 1;
						}
						else
						{
							if (saturation_difference <= 1)
							{
								saturation_close = 2;
							}
							else
							{
								if (saturation_difference <= 2)
								{//last_cor29.10.14
									if (!saturation_close)
									{
										saturation_close = 3;
									}
								}
							}
						}

						if ((grayscale_close >= 1) && (grayscale_close <= 2))
						{//grcl1
							if ((!zone_gray) || (!zone_gray_bunch))
							{//last_cor13.06.13
								if (!difference_hue_zone)
								{
									hue_close = 1;
									prior = 1;
									goto L;
								}
								else
								{
									if (difference_hue_zone <= 1)
									{
										hue_close = 2;
										prior = 2;
										goto L;
									}
									else
									{
										prior = 3;
										goto L;
									}
								}
							}//last_cor13.06.13
							if ((zone_gray <= 1) || (zone_gray_bunch <= 1))//last_cor 8.04.13
							{//zg1g
								if ((grayscale_close == 1) && (saturation_difference <= 1))
								{//last_cor12.06.13
									if ((hue_close>0) || (hue_difference <= 4))
									{
										prior = 1;
										goto L;
									}
									else
									{
										if (difference_hue_zone <= 1)
										{
											prior = 2;
											goto L;
										}
										else
										{
											if (difference_hue_zone <= 2)
											{
												prior = 3;
												goto L;
											}
										}
									}
								}//last_cor12.06.13
								else
								{//last_cor
									if ((grayscale_close>0) && (grayscale_close <= 2) && (hue_close>0) && (saturation_close>0))
									{
										if (saturation_difference <= 1)
										{
											prior = 1;
											goto L;
										}
										else
										{
											prior = 2;
											goto L;
										}
									}
								}
							}//zg1g
							else
							{
								if ((zone_gray <= 2) || (zone_gray_bunch <= 2))//last_cor 21.04.13
								{//zg2
									if ((grayscale_close == 1) && (difference_hue_zone <= 1))
									{
										if (union_gray_length<possible_gray_variation)
										{
											if (saturation_difference <= 1)
											{
												prior = 2;
												goto L;
											}
											else
											{
												prior = 3;
												goto L;
											}
										}
										else
										{
											if (saturation_difference <= 1)
											{
												prior = 3;
												goto L;
											}
											else
											{
												prior = 4;
												goto L;
											}
										}
									}
								}//zg2
								else
								{//last_cor 10.06.13
									if (((zone_gray <= 4) || (zone_gray_bunch <= 4)) && (saturation_close == 1))
									{//zg4

										if (hue_close <= 0)
										{//hc
											if ((difference_hue_zone <= 1) && (((zone_hue>2) && (zone_hue<5)) ||
												((zone_hue_bunch>2) && (zone_hue_bunch<5))))
											{//dhz//last_cor11.04.14
												if (!difference_hue_zone)
												{//last_cor26.11.14
													if (grayscale_close == 1)
													{
														if ((hue_difference <= 3) && (!hue_close))
														{
															prior = 1;
															hue_close = 1;
															goto L;
														}
													}
												}
												if ((hue_difference <= 4) || ((!difference_hue_zone) && (hue_difference <= 6)))
												{//last_cor11.4.14
													prior = 2;
													hue_close = 2;
													goto L;
												}
												else
												{//ehd
													if ((length_ratio <= 2) || (length_ratio >= 14))
													{//lr
														if (hue_difference <= 8)
														{
															prior = 3;
															hue_close = 3;
															goto L;
														}
														else
														{
															prior = 4;
															hue_close = -1;
															goto L;
														}
													}//lr
												}//ehd
											}//dhz
										}//hc

									}//zg4
								}//last_cor 10.06.13
							}
							if ((hue_close == 1) && (grayscale_close == 1) && (saturation_close>0))
							{//last_cor28.11.14
								prior = 1;
								goto L;
							}
							else
							{
								if ((hue_close >= 2) && (hue_close <= 3) && (grayscale_close == 1) && (saturation_close>0))
								{//last_cor28.11.14
									prior = 2;
									goto L;
								}
								else
								{
									if (difference_hue_zone == 0)
									{//dhz0
										if (hue_difference <= 4)
										{//hd4
											if ((saturation_close>0) && (grayscale_close>0))
											{//last_cor28.11.14
												if ((saturation_close == 1) && (grayscale_close == 1))
												{//last_cor29.04.15
													if (hue_difference <= 3)
													{
														prior = 1;
														hue_close = 1;
														goto L;
													}
													else
													{
														prior = 2;
														hue_close = 2;
														goto L;
													}
												}
												else
												{
													hue_close = 2;//last_cor18.01.13
													prior = 2;
													goto L;
												}//last_cor29.04.15
											}

										}//hd4
										else
										{//ehd4
											if (hue_difference <= 6)
											{//hd6
												if ((zone_gray >= 6) && (zone_gray_bunch >= 6) && (saturation_close == 1) && (grayscale_close == 1) &&
													(gray_ntersection <= 1) && (3 * gray_difference <= possible_gray_variation))
												{//last_cor18.06.14
													hue_close = 3;//last_cor18.01.13
													prior = 2;
													goto L;
												}
												/*else
												{
												hue_close=3;//last_cor18.01.1306.04.15
												prior=3;
												goto L;
												}*/
											}//hd6
										}//ehd4
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
						if ((!gray_ntersection) && (gray_difference <= 2) && (2 * gray_difference<possible_gray_variation) &&
							(difference_hue_zone <= 1) && (zone_gray <= 4) && (zone_gray_bunch <= 4))
						{//last_cor16.02.13and29.10.14
							if ((hue_close>0) && (saturation_close>0))
							{
								prior = 2;
								goto L;
							}
						}
						if ((hue_close>0) && (saturation_close>0))
						{//last_cor28.10.13
							if ((grayscale_close>0) && (hue_difference <= 6) && (difference_hue_zone <= 1) && (gray_difference<possible_gray_variation))
							{
								if (grayscale_close == 2)
								{
									prior = 3;
									goto L;
								}
								else
								{
									prior = 4;
									goto L;
								}
							}
						}//last_cor28.10.13
						if ((saturation_close<0))
						{//last_cor28.10.13
							if ((difference_hue_zone >= 1) && (hue_difference >= 4) && ((zone_gray >= 3) || (zone_gray_bunch >= 3)))
							{
								if ((grayscale_close>2) || (grayscale_close == 0))
								{
									if (gray_ntersection == 3)
									{
										prior = 6;
										goto L;
									}
									else
									{
										if (gray_ntersection == 2)
										{
											prior = 7;
											goto L;
										}
									}
								}
								else
								{
									if (grayscale_close<0)
									{
										prior = 5;
										goto L;
									}
								}
							}
						}//last_cor28.10.13
						if (grayscale_close<0)
						{
							if ((hue_close<0) || ((hue_close == 0) && (difference_hue_zone >= 1) && (hue_difference >= 3)))
							{
								prior = 5;
								goto L;
							}
							else
							{
								prior = 6;
								goto L;
							}
						}
						if ((saturation_close<0) && (Density_ratio>10))
						{//last_cor28.11.14
							if ((hue_close<0) || (grayscale_close<0))
							{
								prior = 5;
								goto L;
							}
							else
							{
								prior = 6;
								goto L;
							}
						}
					}//s<=2
					if (!saturation_close)
					{//last_cor26.11.15-sc0
						if (saturation_difference <= poss_sat_variation)
						{//last_cor17.08.15
							saturation_close = 1;
						}//last_cor17.08.15
						else
						{//esdpv
							if ((min_hue_zone == 3) && (max_hue_zone == 3))
							{//mzhmhz3
								if (min_saturation == 3)
								{
									if (saturation_difference >= imp_sat_var)
									{
										saturation_close = -1;
									}
								}
								else
								{//last_cor26.09.16
									if ((min_saturation <= 2) && (max_saturation >= 4))
									{//msms24
										if (((bunch_mean_saturation >= 4) && (zone_gray_bunch >= 1) && (zone_gray >= 4)) ||
											((neighbor_bunch_mean_saturation >= 4) && (zone_gray_bunch >= 4) && (zone_gray >= 1)))
										{//bmsnbms
											saturation_close = -1;
											if (!grayscale_close)
											{
												if (gray_difference>possible_gray_variation)
												{
													grayscale_close = -1;
												}
											}
											if (!hue_close)
											{
												if ((hue_difference <= 3) && (!difference_hue_zone))
												{
													hue_close = 1;
												}
											}
											if ((grayscale_close <= 0) || (hue_close <= 0))
											{
												prior = 5;
												goto L;
											}
											else
											{
												prior = 6;
												goto L;
											}
										}//bmsnbms
									}//msms24
								}//last_cor26.09.16
							}//mzhmhz3
							if ((min_saturation == 1) && (saturation_difference >= 2))
							{//last_cor17.12.15
								if ((bunch_mean_saturation == 1) && (zone_gray_bunch >= 3))
								{
									if ((neighbor_bunch_mean_saturation >= 3) && (zone_gray >= 1))
									{
										saturation_close = -1;
									}
								}
								if ((neighbor_bunch_mean_saturation == 1) && (zone_gray >= 3))
								{
									if ((bunch_mean_saturation >= 3) && (zone_gray_bunch >= 1))
									{
										saturation_close = -1;
									}
								}
							}//last_cor17.12.15


							if (saturation_difference<imp_sat_var)
							{//last_cor14.01.12sd<isv
								if (saturation_difference <= 2)
								{//last_cor15.01.16sd<=2
									if (max_saturation>4)
									{
										saturation_close = 2;
									}
								}//last_cor15.01.16sd<=2
								else//
								{//last_cor14.01.12esd<=2
									if (min_saturation >= 6)
									{
										saturation_close = 2;
									}

								}//last_cor14.01.12esd<=2
							}//last_cor14.01.12sd<isv
						}//esdpv
					}//last_cor26.11.15-sc0
					if (((bunch_mean_hue == 22) && (neighbor_bunch_mean_hue <= 18))
						|| (bunch_mean_hue <= 18) && (neighbor_bunch_mean_hue == 22))
					{//last_cor08.03.17
						if ((minimum_zone_gray >= 1) && (maximum_zone_gray <= 2) && (min_saturation >= 4))
						{

							if (hue_close>0)
							{
								if (hue_difference <= 4)
								{
									hue_close = -2;
								}
								else
								{
									hue_close = -1;
								}
							}
							if ((grayscale_close>0) || (saturation_close>0))
							{
								prior = 5;
								goto L;
							}
							else
							{
								prior = 5;
								goto L;
							}
						}
					}//last_cor08.03.17
					if ((zone_hue == 3) && (zone_hue_bunch == 3))
					{//last_cor15.09.17
						if ((min_saturation <= 2) && (max_saturation >= 3))
						{
							if (ratio_length <= 4)
							{
								if ((gray_difference >= (possible_gray_variation - 2)) && ((ratio_length1_gray<8) ||
									(ratio_length_gray<8)))
								{
									if (grayscale_close>0)
									{
										grayscale_close = -1;
									}
								}
							}
						}
					}//last_cor15.09.17
					if (((zone_hue == 4) && (zone_hue_bunch == 3)) || ((zone_hue == 3) && (zone_hue_bunch == 4)))
					{//last_cor08.03.17
						if ((minimum_zone_gray >= 1) && (maximum_zone_gray <= 2) && (min_saturation >= 4))
						{
							if (hue_difference >= 2)
							{
								if (hue_close>0)
								{
									hue_close = 0;
								}
								if ((grayscale_close>0) || (saturation_close>0))
								{
									prior = 5;
									goto L;
								}
								else
								{
									prior = 5;
									goto L;
								}
							}
						}
					}//last_cor08.03.17
					if ((zone_hue == 3) && (zone_hue_bunch == 3))
					{//zhzhb=3last_cor15.11.16
						if (min_saturation >= 3)
						{//last_cor05.04.17
							if ((ratio_length >= 12) && (ratio_length1 >= 12))
							{
								if (saturation_close == 1)
								{
									if (!hue_close)
									{
										if ((bunch_mean_hue >= 13) && (bunch_mean_hue <= 16) &&
											(neighbor_bunch_mean_hue >= 13) && (neighbor_bunch_mean_hue <= 16))
										{
											hue_close = 1;
											if (grayscale_close>0)
											{
												prior = 1;
												goto L;
											}
										}
									}

								}
							}
						}//last_cor05.04.17
						if (min_saturation >= 4)
						{//last_cor30.03.17
							if (!Intersection_Measure)
							{
								if (saturation_close == 1)
								{
									if ((bunch_mean_hue >= 11) && (bunch_mean_hue <= 18) &&
										(neighbor_bunch_mean_hue >= 11) && (neighbor_bunch_mean_hue <= 18))
									{
										if (!hue_close)
										{
											if (hue_difference <= 3)
											{
												hue_close = 1;
											}
											else
											{
												if (hue_difference <= 4)
												{
													hue_close = 2;
												}
											}
										}
										if (grayscale_close>0)
										{
											if ((hue_close == 1) && (saturation_close == 1))
											{
												prior = 1;
												goto L;
											}
										}
										else
										{
											if (!grayscale_close)
											{
												if ((gray_difference<impossible_gray_variation) && (gray_ntersection >= 0)
													&& (gray_ntersection <= 1))
												{
													grayscale_close = 2;
												}
											}
										}
									}
								}
							}
						}//last_cor30.03.17
						if ((bunch_mean_saturation == 2) && (neighbor_bunch_mean_saturation >= 3))
						{//last_cor03.03.17
							if ((zone_gray_bunch >= 3) && (zone_gray <= 2))
							{
								if ((gray_ntersection >= 2) && (gray_difference >= (possible_gray_variation - 1)))
								{
									grayscale_close = -2;
									if ((hue_close>0) || (saturation_close>0))
									{
										prior = 6;
										goto L;
									}
									else
									{
										prior = 5;
										goto L;
									}
								}
							}
						}//last_cor03.03.17
						if ((bunch_mean_saturation >= 3) && (neighbor_bunch_mean_saturation == 2))
						{//last_cor03.03.17
							if ((zone_gray_bunch <= 2) && (zone_gray >= 3))
							{
								if ((gray_ntersection >= 2) && (gray_difference >= (possible_gray_variation - 1)))
								{
									grayscale_close = -2;
									if ((hue_close>0) || (saturation_close>0))
									{
										prior = 6;
										goto L;
									}
									else
									{
										prior = 5;
										goto L;
									}
								}
							}
						}//last_cor03.03.17
						if (!hue_close)
						{//hc0
							if (Intersection_hue == 3)
							{
								if (((neighbor_bunch_mean_hue <= 19) && (bunch_mean_hue == 22)) ||
									((neighbor_bunch_mean_hue == 22) && (bunch_mean_hue <= 19)))
								{
									hue_close = -1;
									if ((saturation_close>0) || (grayscale_close>0))
									{
										prior = 6;
										goto L;
									}
									else
									{
										prior = 5;
										goto L;
									}
								}
							}
						}//hc0
					}//zhzhb=3last_cor15.11.16
					if ((saturation_close == 1) && (hue_close == 1) && (grayscale_close >= 1) && (grayscale_close <= 2))
					{//last-cor21.12.15
						prior = 1;
						goto L;
					}
					if ((zone_hue == 3) && (zone_hue_bunch == 3))
					{//zhzhb=3
						if ((min_saturation <= 2) && (max_saturation >= 4))
						{//last_cor15.02.17
							saturation_close = -2;
							if ((hue_close>0) || (saturation_close>0))
							{
								prior = 6;
								goto L;
							}
							else
							{
								prior = 5;
								goto L;
							}

						}//last_cor15.02.17
						if (Intersection_hue == 3)
						{
							if ((neighbor_bunch_mean_hue <= 19) && (bunch_mean_hue == 22))
							{
							}
						}
						if ((neighbor_bunch_mean_hue <= 16) && (bunch_mean_hue >= 20) && (zone_gray_bunch >= 2))
						{
							if (neighbor_bunch_mean_saturation >= 3)
							{
								hue_close = -1;
								if ((saturation_close>0) || (grayscale_close>0))
								{
									prior = 6;
									goto L;
								}
								else
								{
									prior = 5;
									goto L;
								}
							}
						}
						if (((bunch_mean_hue <= 16) && (neighbor_bunch_mean_hue >= 20) && (maximum_zone_gray >= 2)) ||
							((bunch_mean_hue <= 16) && (neighbor_bunch_mean_hue >= 21) && (maximum_zone_gray >= 1)) ||
							((bunch_mean_hue >= 20) && (neighbor_bunch_mean_hue <= 16) && (maximum_zone_gray >= 2)) ||
							((bunch_mean_hue >= 21) && (neighbor_bunch_mean_hue <= 16) && (maximum_zone_gray >= 1)))
						{//last_cor20.09.16
							if (min_saturation >= 3)
							{//ms
								hue_close = -1;
								if ((saturation_close>0) || (grayscale_close>0))
								{
									prior = 6;
									goto L;
								}
								else
								{
									prior = 5;
									goto L;
								}
							}//ms
						}//last_cor20.09.16
					}//zhzhb=3

					if (min_saturation >= 5)
					{//last_cor26.11.15
						if ((hue_close == 1) && (saturation_close == 1))
						{//hc1sc1
							if (!grayscale_close)
							{//gc0
								if (gray_difference <= possible_gray_variation)
								{
									grayscale_close = 2;
									prior = 1;
									goto L;
								}
								else
								{//egdp
									if ((gray_difference <= impossible_gray_variation) && (gray_ntersection<3))
									{
										if (zone_hue == 3)
										{
											if ((!difference_gray_zone) || ((zone_gray >= 1) && (difference_gray_zone <= 1)))
											{//last_cor14.12.15
												grayscale_close = 3;
												prior = 2;
												goto L;
											}//last_cor14.12.15
										}
									}
								}//egdp
							}//gc0
							else
							{
								if ((grayscale_close >= 0) && grayscale_close <= 2)
								{//last_cor14.12.15
									prior = 1;
									goto L;
								}//last_cor14.12.15
							}
						}
					}//last_cor26.11.15
					 //}//last_cor28.05.15
					if (hue_close == 1)
					{//last_cor17.06.15
						if (grayscale_close == 1)
						{//last_cor02.11.15
							if (minimum_zone_gray >= 6)
								if (saturation_close>0)
								{
									saturation_close = 1;
									prior = 1;
									goto L;
								}
								else
								{
									if (saturation_difference <= 2)
									{
										saturation_close = 1;
										prior = 1;
										goto L;
									}
								}


						}//last_cor02.11.15
						if (zone_hue == 3)
						{
							if (((ratio_length >= 10) && (ratio_length1 >= 10)) || ((ratio_length >= 14) && (ratio_length1 >= 8)) ||
								((ratio_length >= 8) && (ratio_length1 >= 14)))
							{
								if (saturation_close == 1)
								{//last_cor22.12.15
									if ((grayscale_close>0) && (grayscale_close <= 2))
									{
										prior = 1;
										goto L;
									}
									else
									{//egc02
										if (min_saturation >= 6)
										{//ms7
											if (grayscale_close>0)
											{
												prior = 1;
												goto L;
											}
											else
											{//egc>0
												if (!grayscale_close)
												{//gc0
													if (gray_difference <= possible_gray_variation)
													{
														grayscale_close = 2;
														prior = 1;
														goto L;
													}
													else
													{//egdpg
														if (gray_difference <= (possible_gray_variation + (impossible_gray_variation - possible_gray_variation) / 2))
														{
															if (minimum_zone_gray >= 2)
															{
																grayscale_close = 3;
																prior = 2;
																goto L;
															}
														}

													}//egdpg
												}//gc0
												else
												{
													if (gray_hole <= 2)
													{//last_cor24.12.15
														if (gray_difference <= possible_gray_variation)
														{
															grayscale_close = 2;
															prior = 1;
															goto L;
														}
														else
														{//egdpg
															if (gray_difference <= (possible_gray_variation + (impossible_gray_variation - possible_gray_variation) / 2))
															{
																if (minimum_zone_gray >= 2)
																{
																	grayscale_close = 3;
																	prior = 2;
																	goto L;
																}
															}
															else
															{
																if (gray_difference<impossible_gray_variation)
																{
																	if (minimum_zone_gray >= 2)
																	{
																		grayscale_close = 4;
																		prior = 2;
																		goto L;
																	}
																}
															}

														}//egdpg

													}//last_cor24.12.15
												}
											}//egc>0
										}//ms7
									}//egc02
								}//last_cor22.12.15
								else
								{//last_cor18.01.16
									if (saturation_close == 2)
									{
										if (grayscale_close == 1)
										{
											prior = 1;
											goto L;
										}
										else
										{
											if (gray_difference <= possible_gray_variation)
											{
												grayscale_close = 2;
												prior = 2;
												goto L;
											}
											else
											{
												if (grayscale_close>0)
												{
													grayscale_close = 2;
													prior = 2;
													goto L;
												}
											}
										}
									}
								}//last_cor18.01.16
								if (min_saturation >= 2)
								{
									if (!saturation_close)
									{
										if (saturation_difference <= 2)
										{
											saturation_close = 1;
										}
										if (min_saturation >= 6)
										{//last_cor30.07.15
											if (maximum_zone_gray <= 1)
											{
												saturation_close = 1;
											}
										}

									}
									if (saturation_close == 1)
									{
										if ((grayscale_close>0) && (grayscale_close <= 2))
										{
											prior = 1;
											goto L;
										}
									}
								}
							}
						}
						if (grayscale_close == 1)
						{//last_cor31.08.15
							if (saturation_close>0)
							{
								prior = 1;
								goto L;
							}
							else
							{
								if (saturation_difference<imp_sat_var)
								{
									if (saturation_difference <= 2)
									{
										if (minimum_zone_gray >= 5)
										{
											saturation_close = 2;
											prior = 1;
											goto L;
										}
									}
								}
							}
						}//last_cor31.08.15
					}//last_cor17.06.15
					if ((hue_close == 1) && (saturation_close == 1))
					{//last_cor27.07.15
						if (grayscale_close == 3)
						{
							if (gray_difference<possible_gray_variation)
							{
								grayscale_close = 2;
								prior = 1;
								goto L;
							}
						}

					}//last_cor27.07.15
					if ((!difference_hue_zone) && (hue_difference <= 4))
					{//last_cor29.04.15
						if (minimum_zone_gray >= 5)
							if (!hue_close)
							{//last_cor28.05.15
								if (hue_difference <= 3)
								{
									hue_close = 1;
								}
								else
								{
									hue_close = 2;
								}
							}//last_cor28.05.15
						if (!saturation_close)
						{//last_cor21.12.15
							if (saturation_difference == 2)
							{
								if ((saturation_difference>poss_sat_variation) && (saturation_difference<imp_sat_var))
								{
									saturation_close = 3;
									if ((grayscale_close>1) && (hue_close>0))
									{
										prior = 3;
										goto L;
									}
								}
							}
						}//last_cor21.12.15
					}//last_cor29.04.15
					if (!saturation_close)
					{//last_cor21.12.15
						if (saturation_difference <= poss_sat_variation)
						{
							if (min_saturation >= 6)
							{//last_cor21.04.15
								if (saturation_difference <= 2)
								{
									saturation_close = 1;
								}
							}//last_cor21.04.15
						}
					}//last_cor21.12.15

					if ((zone_gray <= 3) && (zone_gray_bunch <= 3))
					{//last_cor11.04.14
						if (!hue_close)
						{//last_cor19.03.15
							if (min_saturation >= 10)
							{
								if (((zone_hue_bunch == 2) && (zone_hue == 3)) || ((zone_hue_bunch == 3) && (zone_hue == 2)))
								{
									if (hue_difference >= 3)
									{
										hue_close = -1;
									}
								}
							}
							if (saturation_close == 1)
							{
								if (((zone_hue_bunch == 3) && (zone_hue == 4)) || ((zone_hue_bunch == 4) && (zone_hue == 3)))
								{
									if ((hue_difference <= 3) && (max_saturation <= 3))
									{
										hue_close = 1;
										if ((grayscale_close >= 1) && (grayscale_close <= 2))
										{
											prior = 1;
											goto L;
										}
										else
										{
											if (grayscale_close>0)
											{
												prior = 2;
												goto L;
											}
										}

									}
								}
							}
						}
						if (maximum_zone_gray <= 1)
						{//last_cor14.04.14
						 /*if(grayscale_close>0)
						 {
						 grayscale_close=1;
						 }*/
							if (!difference_hue_zone)
							{
								hue_close = 1;
							}
							else
							{//last_cor01.09.14
								if ((grayscale_close == 1) && ((saturation_close == 1) || ((max_saturation <= 4) &&
									(saturation_difference <= 2))))
								{//gcsc1last_cor23.10.14
									if (saturation_close <= 0)
									{
										saturation_close = 2;
									}
									if (difference_hue_zone <= 1)
									{//dhz1
										if (std::max(zone_hue_bunch, zone_hue) == 4)
										{//std::max
											if (hue_difference <= 5)
											{
												hue_close = 1;
												prior = 1;
												goto L;
											}
											else
											{//ehd
												if (hue_difference <= 8)
												{
													if (max_saturation <= 3)
													{//last_cor16.03.15
														hue_close = 2;
														prior = 2;
														goto L;
													}//last_cor16.03.15
												}
											}//ehd
										}//std::max
									}//dhz1
								}//gcsc1

							}//last_cor01.09.14
							if (min_saturation>6)
							{//last_cor21.04.14
								if (minimum_zone_gray>0)
								{
									if (difference_hue_zone >= 2)
									{
										hue_close = -1;
										//prior=-1;
										//goto L;
									}
								}

							}
						}//last_cor14.04.14
						if ((grayscale_close == 1) && (saturation_close == 1))
						{//last_cor11.04.14
							if ((zone_gray <= 3) && (zone_gray_bunch <= 3))
							{
								if (std::min(zone_gray, zone_gray_bunch) <= 1)
								{//last_cor28.11.14
									if (!difference_hue_zone)
									{
										if (max_saturation <= 4)
										{
											if (hue_difference <= 3)
											{
												hue_close = 1;
												prior = 1;
												goto L;
											}
											else
											{
												if (hue_difference <= 6)
												{
													hue_close = 2;
													prior = 1;
													goto L;
												}
											}

										}
									}
								}//last_cor28.11.14
								if (!hue_close)
								{
									if (((zone_hue_bunch == 3) || (zone_hue == 3)) && (difference_hue_zone <= 1))
									{
										if (hue_difference <= 3)
										{
											hue_close = 1;
											prior = 1;
											goto L;
										}
										else
										{
											if ((difference_hue_zone == 1) && ((zone_hue_bunch == 2) || (zone_hue == 2)))
											{//last_cor18.02.15
												if ((bunch_mean_hue <= 5) || (neighbor_bunch_mean_hue <= 5))
												{
													if (hue_difference >= 5)
													{
														hue_close = -1;
														prior = 6;
														goto L;
													}
												}
											}
											else
											{
												if (hue_difference <= 5)
												{
													hue_close = 2;
													prior = 2;
													goto L;
												}
											}
										}
									}
								}
							}

						}//last_cor11.04.14
					}//last_cor11.04.14
					if ((zone_gray <= 4) && (zone_gray_bunch <= 4))
					{//last_cor27.03.14
						if (hue_difference <= 2)
						{//last_cor09.04.14
							hue_close = 1;
						}
						if (max_saturation <= 4)
						{//ms
							if (!hue_close)
							{//hc
								if ((zone_hue>1) && (zone_hue<4) && (zone_hue_bunch>1) && (zone_hue_bunch<4) &&
									(std::min(neighbor_bunch_mean_hue, bunch_mean_hue)>5))
								{//zh
									if (hue_difference <= 2)
									{
										hue_close = 1;

									}
									else
									{//e
										if (hue_difference <= 3)
										{
											hue_close = 2;

										}
										else
										{
											if (hue_difference <= 4)
											{
												hue_close = 3;
											}

										}
									}//e
								}//zh
							}//hc
						}//ms
						if (grayscale_close == 1)
						{//gc1
							if ((saturation_close >= 1) && (saturation_close <= 2))
							{
								if (hue_close == 1)
								{
									prior = 1;
									goto L;
								}
								else
								{
									if (hue_close == 2)
									{
										prior = 2;
										goto L;
									}
								}
							}
						}//gc1
						else
						{//egc1
							if (grayscale_close == 2)
							{//gc2
								if ((saturation_close >= 1) && (saturation_close <= 2))
								{
									if (hue_close == 1)
									{
										prior = 2;
										goto L;
									}
									else
									{
										if (hue_close == 2)
										{
											prior = 3;
											goto L;
										}
									}
								}
							}//gc2
						}//egc1
					}//last_cor27.03.14
					if ((zone_gray >= 1) && (zone_gray_bunch >= 1))
					{//last_cor07.03.14

						if ((zone_gray >= 2) || (zone_gray_bunch >= 2))
						{//last_cor15.10.14
							if (min_saturation >= 3)
							{
								if (difference_hue_zone>1)
								{
									hue_close = -3;
								}
								else
								{
									if (difference_hue_zone == 1)
									{//dhz1
										if (hue_difference>10)
										{
											hue_close = -2;
										}
										else
										{//edhz10
											if (hue_difference>6)
											{
												hue_close = -1;
											}
											else
											{//last_cor12.02.15
												if (((zone_hue_bunch >= 1) && (zone_hue_bunch <= 2))
													|| ((zone_hue >= 1) && (zone_hue <= 2)))
												{
													if (min_saturation >= 4)
													{
														if (hue_difference >= 4)
														{
															hue_close = -1;
														}
													}
												}
											}//last_cor12.02.15
										}//edhz10
									}//dhz1
								}//edhz>1
							}//ms>=3
						}//last_cor15.10.14
						if (((zone_gray >= 2) && ((neighbor_bunch_mean_saturation>5))) ||
							((zone_gray_bunch >= 2) && (bunch_mean_saturation>5)))
						{//last_cor26.03.14
							if (difference_hue_zone>1)
							{
								hue_close = -3;
							}
							else
							{
								if ((hue_difference>5) && (difference_hue_zone == 1))
								{
									hue_close = -2;
								}
							}
						}
					}
					if ((!zone_gray) || (!zone_gray_bunch))
					{//!zg
						if (grayscale_close == 1)
						{//gc1
							if (!difference_hue_zone)
							{
								if (!hue_close)
								{//last_cor16.03.15
									hue_close = 1;
								}//last_cor16.03.15
							}
							else
							{//e
								if ((difference_hue_zone <= 1) && (hue_difference <= 6))
								{//last_cor22.04.14
									if (!hue_close)
									{//last_cor16.03.15
										hue_close = 2;
									}//last_cor16.03.15
								}
								else
								{
									if ((difference_hue_zone <= 1) && (hue_difference <= 8))
									{
										if (!hue_close)
										{//last_cor16.03.15
											hue_close = 3;
										}//last_cor16.03.15
									}
								}
							}//e
							 //prior=1;
							 //goto L;
						}//zc1
						else
						{//egs
							if (grayscale_close == 2)
							{//gc2
								if (!difference_hue_zone)
								{
									if (!hue_close)
									{//last_cor16.03.15
										hue_close = 1;
									}//last_cor16.03.15
								}
								else
								{
									if ((difference_hue_zone <= 1) && (hue_difference <= 4))
									{
										if (!hue_close)
										{//last_cor16.03.15
											hue_close = 2;
										}//last_cor16.03.15
									}
									else
									{
										if ((difference_hue_zone <= 1) && (hue_difference <= 6))
										{
											if (!hue_close)
											{//last_cor16.03.15
												hue_close = 3;
											}//last_cor16.03.15
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
					if ((zone_gray <= 1) || (zone_gray_bunch <= 1))
					{//zgb1-16.04.13
						if (std::max(zone_gray, zone_gray_bunch) >= 2)
						{//last_cor27.10.14
							if (saturation_close<0)
							{//sc<0
								if (!hue_close)
								{//hc0
									if (!difference_hue_zone)
									{
										if (hue_difference <= 5)
										{
											hue_close = 1;
										}
									}
									else
									{//edhz0
										if (hue_difference <= 3)
										{//last_cor27.10.14
											hue_close = 2;
										}
										else
										{//edhz5
											if (hue_difference <= 5)
											{//last_cor27.10.14
												hue_close = 3;
											}
										}//edhz5
									}//edhz0
								}//hc0
								prior = 5;
								goto L;
							}//sc<0
						}//last_cor27.10.14
						if (!saturation_close)//last_cor26.09.13
						{//last_cor11.06.131
							if (saturation_difference <= poss_sat_variation)
							{//last_cor11.06.13
								saturation_close = 1;
							}
							else
							{
								if (saturation_difference<imp_sat_var)
								{//last_cor11.06.13
									saturation_close = 2;
								}
							}
						}//last_cor26.09.13
						if ((grayscale_close >= 1) && (grayscale_close <= 2))
						{//gsc12

							if (hue_close == 1)
							{
								if (saturation_close>0)
								{
									prior = 1;
									goto L;
								}
							}
							else
							{//ehc1
								if (hue_close>0)
								{
									if (saturation_close>0)
									{
										prior = 2;
										goto L;
									}
								}
								else
								{//ehc0
									if (!difference_hue_zone)
									{
										if (!hue_close)
										{//last_cor16.03.15
											hue_close = 1;
										}//last_cor16.03.15
									}
									else
									{//edhz
										if ((hue_difference <= 8) && (min_saturation <= 2))
										{//last_cor03.08.13
											if (!hue_close)
											{//last_cor16.03.15
												hue_close = 2;
											}
										}
										else
										{//ehc2
											if (difference_hue_zone <= 1)
											{//dhz1last_cor26.09.13
												if ((zone_hue != 1) && (zone_hue_bunch != 1) && (zone_hue != 2) && (zone_hue_bunch != 2))
												{
													if (!hue_close)
													{//last_cor16.03.15
														hue_close = 3;
													}
												}
												else
												{
													if ((zone_gray_bunch >= 1) && (zone_gray >= 1) && (min_saturation >= 4))
													{//last_cor26.09.13
														if (((zone_hue == 1) && (zone_hue_bunch == 2)) || ((zone_hue == 2) && (zone_hue_bunch == 1)))
														{
															if (hue_difference >= 3)
															{
																hue_close = -1;
															}
															else
															{
																if (hue_difference >= 5)
																{
																	hue_close = -2;
																}
															}
														}
													}//last_cor26.09.13
												}
												if (!saturation_close)
												{//last_cor11.06.131
													if (saturation_difference <= poss_sat_variation)
													{//last_cor11.06.13
														saturation_close = 1;
													}
													else
													{
														if (saturation_difference<imp_sat_var)
														{//last_cor11.06.13
															saturation_close = 2;
														}
													}
												}//last_cor26.09.13
												if ((max_saturation <= 2) && (saturation_close == 1) && (grayscale_close == 1))
												{//last_cor11.06.13
													prior = 2;
													goto L;
												}//last_cor11.06.13
											}//dhz1
										}//ehc2
									}//edhz
								}//ehc0
							}//ehc1
						} //gsc12
					}//zgb1-16.04.13
					else
					{
						if ((zone_gray <= 3) || (zone_gray_bunch <= 3))
						{//zgb-18.04.13
							if ((zone_gray <= 2) && (zone_gray_bunch <= 2))
							{//last_cor08.09.14
								if (min_saturation >= 2)
								{
									if ((length_ratio >= 7) && (length_ratio <= 10) && (Density_ratio >= 7) && (Density_ratio <= 9))
									{
										if ((grayscale_close == 1) && (hue_close == 1))
										{
											if (!saturation_close)
											{//last_cor21.12.15
												if ((saturation_difference<impossible_sat_variation) && (saturation_difference>possible_sat_variation))
												{
													prior = 2;
													saturation_close = 2;
													goto L;
												}
											}//last_cor21.12.15
										}
									}
								}
							}
							if ((saturation_difference<imp_sat_var) && (saturation_difference <= 2))
							{
								if (!saturation_close)
								{//last_cor28.05.15
									saturation_close = 2;
								}//last_cor28.05.15
								if (hue_close == 1)
								{//last_cor26.03.14
									if ((saturation_close >= 1) && (saturation_close <= 2))
									{
										if ((grayscale_close >= 2) && (gray_difference <= possible_gray_variation))
										{
											prior = 2;
											goto L;
										}

									}

								}//last_cor26.03.14
								if (saturation_close == 1)
								{//last_cor25.03.14
									if ((grayscale_close >= 2) && (gray_difference <= possible_gray_variation))
									{
										if (!hue_close)
										{//hc0
											if (hue_difference <= 3)
											{//hd3
												if (!difference_hue_zone)
												{
													hue_close = 1;
													prior = 2;
													goto L;
												}
												if (difference_hue_zone == 1)
												{
													if ((zone_hue>1) && (zone_hue<4) && (zone_hue_bunch>1) && (zone_hue_bunch<4))
													{
														hue_close = 2;
														prior = 2;
														goto L;
													}
													else
													{
														hue_close = 3;
														prior = 3;
														goto L;
													}
												}

											}//hd3
											else
											{//26.03.14
												if ((zone_hue>1) && (zone_hue<4) && (zone_hue_bunch>1) && (zone_hue_bunch<4))
												{
													if (hue_difference <= 4)
													{//hd
														hue_close = 2;
													}
												}
											}
										}//hc0
									}
								}//last_cor25.03.14
								if ((grayscale_close == 1) || (2 * gray_difference <= possible_gray_variation))
								{//last_cor28.05.13
									if ((saturation_close >= 1) && (saturation_close <= 2))
									{//last_cor12.02.15
										if (hue_close == 1)
										{
											prior = 1;
											goto L;
										}
										else
										{//ehc1
											if (((zone_hue != 2) && (zone_hue != 1)) || (zone_hue_bunch != 2) && (zone_hue_bunch != 1))
											{//last_cor15.07.13_17.09.13
												if (hue_difference <= 3)
												{

													if (!hue_close)
													{
														prior = 2;
														hue_close = 2;
														goto L;
													}
												}

											}//last_cor15.07.13_17.09.13

										}//ehc1
									}//last_cor12.02.15
								}//last_cor28.05.13
							}//esd
						}//zgb-18.04.13
					}//ezb1
					if ((saturation_difference>poss_sat_variation) && (!saturation_close))
					{//last cor21.12.15
						if ((zone_gray >= 2) && (zone_gray_bunch >= 2))
						{//zgb
							if (!saturation_close)
							{//last_cor21.12.15
								if ((saturation_difference >= imp_sat_var) && (Intersection_saturation >= 2) && (int_length >= 2 * StripWidth))
								{//last_cor14.05.14
									saturation_close = -3;
									prior = 6;
									goto L;
								}
								else
								{
									if (saturation_difference>(poss_sat_variation + (imp_sat_var - poss_sat_variation) / 2) &&
										(Intersection_saturation >= 2) && (int_length >= 2 * StripWidth))
									{//last_cor14.05.14
										saturation_close = -2;
									}
								}
							}//last_cor21.12.15
						}//zgb
					}//last cor21.12.15
					if (!saturation_close)
					{//last_cor21.12.15
						if (((saturation_distance1 <= possible_sat_variation) && (saturation_distance <= possible_sat_variation1)) &&
							(((bunch_mean_saturation >= left_sat) && (bunch_mean_saturation <= right_sat)) || (abs_satur <= 1)))
						{//s
							saturation_close = 1;
						}//s
						else
						{//e=1
							if ((bunch_mean_saturation >= 8) && (neighbor_bunch_mean_saturation >= 8) && (zone_gray_bunch <= 2) &&
								(zone_gray <= 2))
							{
								saturation_close = 2;
							}
							else
							{
								if (((saturation_distance1 <= possible_sat_variation) && (saturation_distance <= possible_sat_variation1)) ||
									((bunch_mean_saturation >= left_sat) && (bunch_mean_saturation <= right_sat) &&
									(neighbor_bunch_mean_saturation >= left_sat_bunch) &&
										(neighbor_bunch_mean_saturation <= right_sat_bunch)))
								{//last_cor10.07.13
									saturation_close = 2;
								}
								else
								{
									if (((saturation_distance1 <= possible_sat_variation + 1) && (saturation_distance <= possible_sat_variation1 + 1)) &&
										((bunch_mean_saturation >= left_sat) && (bunch_mean_saturation <= right_sat)
											&& (neighbor_bunch_mean_saturation >= left_sat_bunch) &&
											(neighbor_bunch_mean_saturation <= right_sat_bunch)))
									{//last_cor10.07.13
										saturation_close = 3;
									}
									else
									{
										if ((saturation_distance1 >= impossible_sat_variation) || (saturation_distance >= impossible_sat_variation1))
										{
											if ((Intersection_saturation >= 2) && (int_length >= 2 * StripWidth))
											{//last_cor14.05.14
												saturation_close = -1;
											}
										}
									}
								}
							}
						}//e=1
					}//last_cor21.12.15
					if (zone_gray <= 1)
					{//zg1
						if ((hue_close == 1) && (saturation_close == 1) && (grayscale_close >= 0))
						{//except for grayscalelast_cor03.04.15
							if (zone_hue_bunch == 3)
							{//green
								if (zone_gray_bunch <= 2)
								{//last_cor03.04.15
									if (gray_ntersection<2)
									{
										if (zone_gray_bunch <= 1)
										{
											if (grayscale_close>0)
											{
												prior = 1;
											}
											else
											{
												prior = 3;
											}
										}
										else
										{
											if (grayscale_close>0)
											{
												prior = 3;
											}
										}
									}//last_cor03.04.15

									goto L;
								}
							}//green
						}//except for grayscale
						if (!hue_close)
						{//hue indefinite
							if ((!difference_hue_zone) && ((hue_difference <= 2) || (hue_difference <= 3) && (node_vicinity)))
							{//closeness correction
								if ((zone_hue != 2) || (zone_hue != 2))
								{//last_cor15.07.13
									hue_close = 1;
								}

								if (!saturation_close)
								{
									if (((saturation_distance1 <= possible_sat_variation + 1) && (saturation_distance <= possible_sat_variation1 + 1)) ||
										((bunch_mean_saturation >= left_sat) && (bunch_mean_saturation <= right_sat)))
									{
										saturation_close = 2;
									}
								}
								if (!grayscale_close)
								{
									if (zone_gray_bunch <= 2)
									{
										grayscale_close = 3;
									}
								}
							}//closeness correction


							if ((((!difference_hue_zone) && (hue_difference <= 8)) || ((hue_difference <= 6) && (difference_hue_zone <= 1)) ||
								((left_hue <= corrected_bunch_mean_hue) && (corrected_bunch_mean_hue <= right_hue))))
							{//h
								hue_close = 2;
							}//h
							else
							{//e0
								if ((hue_difference <= 6) && (difference_hue_zone == 1))
								{
									hue_close = 3;
								}
								else
								{
									if ((difference_hue_zone >= 2) || ((hue_difference >= 8) && (difference_hue_zone == 1)))
									{
										hue_close = -1;
									}
									else
									{
										if ((difference_hue_zone>2) || (hue_difference>12))
										{
											hue_close = -2;
										}
									}
								}
							}//e0
							if (zone_gray == 1)
							{//last_cor12.03.13
								if (min_saturation>5)
								{
									if (zone_hue_bunch == 1)
									{
										if ((hue_difference >= 5) && (difference_hue_zone == 1))
										{
											hue_close = -2;
										}
									}
								}
							}
						}//hue indefinite


						if (((zone_hue == 2) && (zone_hue_bunch == 1)) || (zone_hue_bunch == 2) && (zone_hue == 1))
						{//last_cor17.09.13
							if (!hue_close)
							{
								if ((zone_gray >= 3) && (zone_gray_bunch >= 3))
								{//zgb
									if (min_saturation>4)
									{
										if (hue_difference >= 3)
										{
											if (hue_difference >= 4)
											{
												hue_close = -2;
											}
											else
											{
												hue_close = -1;
											}
										}
									}
								}//zgb

							}
						}



						if ((grayscale_close >= 1) && (grayscale_close <= 2) && (saturation_close>0) && (saturation_close <= 2)
							&& (hue_close>0) && (hue_close <= 2))
						{//gh!!!!!
							if ((hue_close == 1) && (saturation_close == 1))
							{
								prior = 1;
								goto L;
							}
							if ((grayscale_close == 1) && (saturation_close == 1))
							{
								prior = 1;
								goto L;
							}
							if ((hue_close == 1) && (grayscale_close == 1))
							{
								prior = 1;
								goto L;
							}
						}//gh
						else
						{//e1
							if ((hue_close >= 1) && (hue_close <= 2) && (grayscale_close >= 1) && (saturation_close >= 1) && (grayscale_close<3))
							{
								prior = 2;
							}

							else
							{
								if ((((hue_close == 1) && (grayscale_close == 3)) || ((hue_close == 3) && (grayscale_close == 1))) && (saturation_close >= 1))
								{
									prior = 3;
									goto L;
								}
								else
								{
									if ((hue_close >= 2) && (grayscale_close>1) && (saturation_close >= 1))
									{
										if (grayscale_close == 1)
										{
											prior = 2;
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
										if ((hue_close>0) && (hue_close <= 2) && (grayscale_close >= 1) && (grayscale_close <= 2) && (difference_gray_zone <= 1))
										{

											/*if((ratio_length1==16)||(Signif_ratio<=2)&&(Density_ratio<3)&&(length_ratio<=6))
											{//incl1
											prior=2;
											}//incl1*/
											if ((saturation_close >= 0) && (grayscale_close == 1) && ((!gray_ntersection) || (ratio_length1_gray == 16)))
											{
												prior = 2;
												goto L;
											}
										}
										else
										{
											if ((hue_close == 1) && (grayscale_close >= 1) && (grayscale_close <= 2) && (difference_gray_zone <= 1))
											{
												if ((ratio_length1 >= 14) && ((ratio_length >= 14)) && (Density_ratio<10) && (Density_ratio>6)
													&& (!gray_ntersection))
												{
													prior = 2;
													goto L;
												}
											}

										}

									}

								}
							}
						}//e1


						if ((hue_close<-1) || ((hue_close<0) && (grayscale_close<0)) ||
							((saturation_close<0) && (grayscale_close<0)) || ((hue_close<0) && (saturation_close<0)))
						{
							prior = 15;
							goto L;
						}
						else
						{
							if ((hue_close<0) || (saturation_close<0) || (grayscale_close<-1))
							{
								prior = 16;
								goto L;
							}
							if (grayscale_close<0)
							{
								prior = 17;
								goto L;
							}
						}
						prior = 18;
						goto L;
					}//zg1
					else
					{//1
						if ((zone_gray <= 2) && (difference_gray_zone <= 1))
						{//zg2
							if ((hue_close == 1) && (saturation_close == 1) && ((!grayscale_close) || (grayscale_close == 3)))
							{//hc1sc1gc0
								if ((zone_hue_bunch == 3) && (!difference_hue_zone))
								{//zhb3

									if ((gray_difference <= possible_gray_variation))
									{
										prior = 1;
										grayscale_close = 2;
										goto L;
									}
									else
									{//e<=2
										if (grayscale_close == 3)
										{
											prior = 2;
											goto L;
										}

									}//e<=2
								}//zhb3
							}//hc1sc1gc0
							if (!hue_close)
							{//hue indefinite

								if (((!difference_hue_zone) && (hue_difference <= 4)) || (hue_difference <= 3) ||
									((left_hue <= corrected_bunch_mean_hue) && (corrected_bunch_mean_hue <= right_hue)))
								{//h2
									if (((zone_hue_bunch != 2) && (zone_hue != 2)) || ((zone_hue_bunch == 2) && (zone_hue != 1)) && (zone_hue != 3) ||
										((zone_hue_bunch != 1) && (zone_hue_bunch != 3) && (zone_hue == 2)))
									{//hue_cond_zone21_last_cor16.07.13last_cor24.02.15
										hue_close = 2;
									}
									else
									{
										if ((ratio_length_hue>7) && (ratio_length1_hue>7) && (hue_difference <= 2))
										{
											hue_close = 2;
										}
									}
								}//h2
								else
								{//e9

									if ((hue_difference >= 6) || ((hue_difference >= 5) && (difference_hue_zone>0)))
									{
										hue_close = -1;
									}
									else//e11
									{
										if ((difference_hue_zone>2) || (hue_difference>12))
										{
											hue_close = -2;
										}
									}//e11

								}//e9
								if (zone_gray >= 1)
								{//last_cor12.03.13
									if (min_saturation>5)
									{
										if (zone_hue_bunch == 1)
										{
											if ((hue_difference >= 5) && (difference_hue_zone == 1))
											{
												hue_close = -2;
											}
										}
									}
								}

							}//hue indefinite

							if ((!difference_hue_zone) && ((hue_difference <= 2) || (hue_difference <= 2) && ((node_vicinity) || (node_vicinity_neighbor))))
							{//characteristics correction
								hue_close = 1;
								if (!saturation_close)
								{
									if (((saturation_distance1 <= possible_sat_variation + 1) && (saturation_distance <= possible_sat_variation1 + 1)) ||
										((bunch_mean_saturation >= left_sat) && (bunch_mean_saturation <= right_sat)))
									{
										saturation_close = 2;
									}
								}
								if (!grayscale_close)
								{
									if (zone_gray_bunch <= 2)
									{
										grayscale_close = 3;
									}
									else
									{
										if ((hue_difference <= 1) && (max_satur_differ <= 1)
											&& (gray_difference<(possible_gray_variation + (impossible_gray_variation - possible_gray_variation) / 4)))
										{
											grayscale_close = 2;
										}
									}
								}

							}//characteristics correction


							if (((grayscale_close >= 1) && (grayscale_close <= 2) && (saturation_close == 1) && (hue_close == 1)) ||
								((grayscale_close == 1) && (saturation_close >= 1) && (saturation_close <= 2) && (hue_close == 1)) ||
								((grayscale_close == 1) && (saturation_close >= 1) && (saturation_close <= 2) &&
								(hue_close >= 1) && ((hue_close <= 2))))
							{//gh
								prior = 1;
								goto L;
							}//gh
							else
							{//e2
								if (((hue_close == 1) && (grayscale_close >= 1) && (saturation_close >= 1) &&
									(saturation_close <= 2) && (grayscale_close<3)) ||
									((hue_close == 1) && (saturation_close == 1) && (grayscale_close<3) && (grayscale_close>0)) ||
									((hue_close >= 1) && (hue_close <= 2) && (grayscale_close >= 1) && (saturation_close == 1) &&
									(grayscale_close<3)) || ((hue_close >= 1) && (hue_close <= 2) && (saturation_close >= 1) &&
										(saturation_close <= 2) && (grayscale_close >= 1) && (grayscale_close<3)))
								{
									prior = 2;
									goto L;
								}
								else
								{
									if (((hue_close == 1) && (grayscale_close>0) && (grayscale_close <= 3) && (saturation_close >= 1) &&
										(saturation_close <= 2)) ||
										((hue_close == 3) && (grayscale_close == 1) && (saturation_close >= 1)))
									{//last cor
										if ((hue_close == 1) && (saturation_close == 1) && (grayscale_close>0) &&
											(grayscale_close <= 2))
										{///!!!
											prior = 2;
											goto L;
										}
										else
										{
											prior = 3;
											goto L;
										}
									}
									else
									{
										if ((ratio_length1<13) && (Signif_ratio <= 2) && (Density_ratio<6) || (length_ratio<6))
										{
											if ((grayscale_close>0) && (grayscale_close <= 2) && (saturation_close >= 0) && (hue_close>0) && (hue_close<2))
											{
												prior = 4;

											}
										}
									}
								}
							}//e2



							if ((prior>0) && (prior <= 4))
							{
								goto L;
							}
							if ((hue_close<-1) || ((hue_close<0) && (grayscale_close<0)) ||
								((saturation_close<0) && (grayscale_close<0)) || ((hue_close<0) && (saturation_close<0)))
							{
								prior = 15;
								goto L;
							}
							else
							{
								if ((hue_close<0) || (saturation_close<0) || (grayscale_close<-1))
								{
									prior = 16;
									goto L;
								}
								if (grayscale_close<0)
								{
									prior = 17;
									goto L;
								}
							}
							if ((hue_close != 1) || (saturation_close != 1))
							{//last_cor08.09.14
								prior = 18;
								goto L;
							}
						}//zg2
						else
						{//zg>
							if (!hue_close)
							{//hue indefinite


								if ((hue_difference <= hue_bound))
								{//h3
									hue_close = 2;
								}//h3
								else
								{

									if (((!difference_hue_zone) && (hue_difference <= 4)) || (hue_difference <= 3))

									{//!dhz
										if (zone_gray <= 2)
										{//10.04.13
											if ((grayscale_close == 1) && (saturation_close == 1))
											{
												hue_close = 2;
											}
											else
											{
												if ((grayscale_close == 1) && (saturation_close == 2))
												{
													hue_close = 3;
												}
											}
										}//10.04.13
										else
										{
											if ((Signif_ratio <= 2) && (length_ratio <= 2) && (Density_ratio<5))
											{
												hue_close = 3;
											}
										}

									}//!dhz
									else
									{

										if ((hue_difference >= 5) || ((hue_difference >= 4) && (difference_hue_zone>0)))
										{//hddz
											if (zone_gray>3)
											{//9.04.13
												hue_close = -1;
											}
											else
											{//e1
												if ((difference_hue_zone == 0))
												{//dz0
													if (hue_difference <= 6)
													{
														if ((grayscale_close == 1) && (saturation_close == 1))
														{
															hue_close = 3;
														}
													}
												}//dz0
												else
												{//e2
													if (hue_difference >= 6)
													{
														hue_close = -1;
													}
												}//e2

											}//e1
										}//hddz

										else
										{
											if ((difference_hue_zone>2) || (hue_difference>12))
											{
												hue_close = -2;
											}
										}
									}
								}
								if (zone_gray >= 1)
								{//last_cor12.03.13
									if (min_saturation>5)
									{
										if (zone_hue_bunch == 1)
										{
											if ((hue_difference >= 5) && (difference_hue_zone == 1))
											{
												hue_close = -2;
											}
										}
									}
								}
							}//hue indefinite

							if (!saturation_close)
							{//saturation indefinite
								if ((bunch_occurrence>0) && (Density_ratio >= 6) && (Density_ratio <= 10))
								{//bo
									if (Intersection_saturation == 3)
									{//s_i=3
										if ((saturation_distance1 >= possible_sat_variation) || (saturation_distance >= possible_sat_variation1))
										{
											if ((ratio_length_satur >= (2 * poss_sat_variation / 3)) ||
												(ratio_length_satur >= (imp_sat_var / 3)))
											{
												saturation_close = -2;
											}
											else
											{
												if ((ratio_length_satur >= (poss_sat_variation / 2)))
												{
													saturation_close = -1;
												}
											}
										}

									}//s_i=3
									else
									{
										if ((Intersection_saturation >= 1) && ((Signif_ratio >= 15) || (Signif_ratio <= 3)))
										{
											if ((saturation_distance1 >= possible_sat_variation) || (saturation_distance >= possible_sat_variation1))
											{
												if ((gray_ntersection >= 2) && (ratio_length1_satur <= 6) && (ratio_length_satur <= 6) && (ratio_length1_gray <= 6))
												{
													saturation_close = -1;
												}
											}
										}
									}
								}//bo
							}//saturation indefinite
							if (grayscale_close<0)
							{//last_cor22.07.13
								if (saturation_close <= 0)
								{
									prior = 5;
									goto L;
								}
							}
							if ((grayscale_close >= 1) && (grayscale_close <= 2) && (saturation_close == 1) && (hue_close == 1))
							{//gh//last_cor050213
								prior = 1;
								goto L;
							}//gh
							else
							{//e2
								if ((grayscale_close == 1) && (hue_close == 1))
								{
									if ((saturation_close>0) && (abs_satur <= 2))
									{
										if (saturation_close == 2)
										{
											prior = 1;
											goto L;
										}
										else
										{
											prior = 2;
											goto L;
										}
									}
									else//last_cor050213
									{
										if ((saturation_close == 2) && (abs_satur >= 3))
										{
											prior = 1;
											goto L;
										}
									}
								}
							}//e2

							if (((grayscale_close >= 1) && (grayscale_close <= 2) && (saturation_close == 1) && (hue_close == 1)) ||
								((grayscale_close == 1) && (saturation_close >= 1) && (saturation_close <= 2) && (hue_close == 1)) ||
								((grayscale_close == 1) && (saturation_close >= 1) && (saturation_close <= 2) &&
								(hue_close >= 1) && ((hue_close <= 2))))
							{//gh
								prior = 1;
								goto L;
							}//gh
							else
							{//e2
								if (((hue_close == 1) && (grayscale_close >= 1) && (saturation_close >= 1) &&
									(saturation_close <= 2) && (grayscale_close<3)) ||
									((hue_close == 1) && (saturation_close == 1) && (grayscale_close<3) && (grayscale_close>0)) ||
									((hue_close >= 1) && (hue_close <= 2) && (grayscale_close >= 1) && (saturation_close == 1) &&
									(grayscale_close<3)) || ((hue_close >= 1) && (hue_close <= 2) && (saturation_close >= 1) &&
										(saturation_close <= 2) && (grayscale_close >= 1) && (grayscale_close<3)))
								{
									prior = 2;
									goto L;
								}
								else
								{
									if (((hue_close == 1) && (grayscale_close>0) && (grayscale_close <= 3) && (saturation_close >= 1) &&
										(saturation_close <= 2)) ||
										((hue_close == 3) && (grayscale_close == 1) && (saturation_close >= 1)))
									{
										if ((hue_close == 1) && (saturation_close == 1) && (grayscale_close>0) && (grayscale_close <= 2))
										{///!!!
											prior = 2;
											goto L;
										}
										else
										{
											prior = 3;
											goto L;
										}
									}
									else
									{
										if ((ratio_length1>13) && (ratio_length>13) && (Density_ratio<10) || (Density_ratio>6))
										{
											if ((grayscale_close>0) && (grayscale_close <= 2) && (saturation_close >= 0) && (hue_close>0) && (hue_close <= 3))
											{
												prior = 4;
												goto L;

											}
										}
									}
								}
								if ((hue_close == 1) && (saturation_close == 1))
								{
									if ((grayscale_close >= 3) && (grayscale_close <= 4))
									{
										prior = 2;
										goto L;
									}
								}
								if (!grayscale_close)
								{
									if ((hue_close == 1) && (saturation_close == 1))
									{
										if (difference_gray_zone <= 1)
										{
											if ((gray_ntersection <= 1) || (union_gray_length <= possible_gray_variation))
											{//last_cor19.06.14
												prior = 3;
												goto L;
											}
											else
											{
												prior = 4;
												goto L;
											}
										}
									}
								}
							}//e2
							if ((prior>0) && (prior <= 4))
							{
								goto L;
							}
							if ((hue_close<-1) || ((hue_close<0) && (grayscale_close<0)) ||
								((saturation_close<0) && (grayscale_close<0)) || ((hue_close<0) && (saturation_close<0)))
							{
								prior = 25;
								goto L;
							}
							else
							{
								if ((hue_close<0) || (saturation_close<0))
								{
									prior = 26;
									goto L;
								}
								if (grayscale_close<0)
								{
									if ((hue_close == 1) && (std::min(bunch_mean_saturation, neighbor_bunch_mean_saturation) >= 3) &&
										(!saturation_close) && (difference_gray_zone <= 2))
									{
										if ((Density_ratio>6) && (Density_ratio<10))
										{
											if (difference_gray_zone <= 1)
											{
												prior = 3;
												goto L;
											}
											else
											{
												prior = 4;
												goto L;
											}
										}
										if ((hue_close == 1) && (saturation_close == 1) && (difference_gray_zone <= 1))
										{
											prior = 3;
											goto L;
										}
									}
								}
							}
							if ((!hue_close) && (!saturation_close))
							{//lcor
								if ((Intersection_hue == 3) && (Intersection_saturation == 3) && (zone_gray>2) && ((zone_gray_bunch>2)))
								{
									prior = 7;
									goto L;
								}
							}
							if (grayscale_close<-1)
							{//last cor
								prior = 6;
								goto L;
							}
							prior = 28;
							goto L;
						}//zg>
					}//1

				L:
					if ((((prior % 10) == 0) || (prior % 10>3)) && (saturation_close >= -1))
					{//last_cor15.02.13prcond

						if ((hue_close == 1) && ((grayscale_close == 1) || ((grayscale_close == 2)
							|| (gray_difference<(possible_gray_variation - possible_gray_variation / 4)))))
						{//hc1gs1
							if (min_saturation >= 10)
							{//ms10
								if (saturation_difference <= (poss_sat_variation + 1))
								{
									prior = 2;
								}
								else
								{
									prior = 3;
								}
							}//ms10
							else
							{//e10
								if (min_saturation >= 6)
								{//ms6
									if (saturation_difference <= (poss_sat_variation + 2))
									{
										if (gray_ntersection<2)
										{//last_cor19.06.14
											prior = 2;
										}
									}
									else
									{//e6
										if (saturation_difference <= (poss_sat_variation + (imp_sat_var - poss_sat_variation) / 2))
										{
											prior = 3;
										}
									}//e6
								}//ms6
								else
								{
									if (min_saturation >= 4)
									{//ms4
										if (saturation_difference <= (poss_sat_variation + 2))
										{
											prior = 3;
										}
										else
										{//e7
											if (saturation_difference <= (poss_sat_variation + (imp_sat_var - poss_sat_variation) / 2))
											{
												prior = 4;
											}
										}//e7
									}//ms4
								}
							}//e10
							if ((((prior % 10) >= 5) && ((prior % 10) <= 7)) && (saturation_close >= -1))
							{//lastcor16.09.13
								if (min_saturation >= 4)
								{//ms4
									if (saturation_difference <= (poss_sat_variation + (imp_sat_var - poss_sat_variation) / 2))
									{
										prior = 3;
									}
									else
									{//imp_error16.09.13
										if (!saturation_close)
											prior = 4;
									}

								}//ms4
							}
						}//hc1gs1
						if (((prior % 10) >= 5) && ((prior % 10) <= 7))
						{
							if (!hue_close)
							{
								if ((difference_hue_zone >= 2) || ((hue_difference >= 8) && (difference_hue_zone >= 1)))
								{
									hue_close = -1;
								}
							}
						}
					}//last_cor15.02.13prcond
					*hue_close_r = hue_close;
					*sat_close_r = saturation_close;
					*gray_close_r = grayscale_close;
					return(prior);
				}
		//-------------------------------------------------------------------------------------------
		int

		CColorSection::ColorJumpTesting(int first_fiber,int first_bunch,int second_fiber,int second_bunch,
		int place,int m_shift)
		{//beg
			int prior;
				int test_junction;
				int previous_sec;
				int continuation_sec;
				int hue_close;
				int sat_close;
				int gray_close;
				int bunch_new;
				int prev_corn;
				int prev_prev_corn;
				int Intersection;
				int indic_length;
				int ratio_length;
				int ratio_length1;
				int beg_int;
				int end_int;
				int new_beg_int;
				int new_end_int;
				//int beg_int_middle;
				//int end_int_middle;
				int coinsidence;
				int coinsidence1;
				int shift_to_left;
				int shift_to_right;
				int boundary_shift;
				int bunch_contrast;//last_cor18.09.14
				int left_contrast;
				int right_contrast;
				int length_int;
				int signif_bunch;
				int density_bunch;
				int length_int_new;
				int signif_bunch_new;
				int density_bunch_new;
				int density_ratio;
				int bunch_significance_ratio;
				int num_sec;
				int first_strip_section;
				int sect_shift;
				int bound_shift;
				int end_shift;
				int color_coincidence;
				int small_bunch;
				int small_new;
				int max_strip;
				int min_strip;

				TIntColored* ColoredIntervalsStructure;
				TIntColored* ColoredIntervalsStructure1;
				//TIntColored* ColoredIntervalsStructure2;

				//beg_int_middle=-1;
				//end_int_middle=-1;
				color_coincidence = 0;
				bound_shift = 0;
				end_shift = 0;
				prev_prev_corn = -1;
				first_strip_section = -1;
				sect_shift = -1;
				left_contrast = 0;
				right_contrast = 0;
				prior = -1;
				Intersection = -1;
				indic_length = -1;
				ratio_length = -1;
				ratio_length1 = -1;
				coinsidence = -1;
				previous_sec = first_fiber;
				continuation_sec = second_fiber;
				bunch_new = second_bunch;
				prev_corn = first_bunch;
				num_sec = Number_of_sections;
				first_strip_section = FirstStrip;
				sect_shift = continuation_sec - first_strip_section;
				ColoredIntervalsStructure = ColorInterval[previous_sec].ColoredIntervalsStructure;
				ColoredIntervalsStructure1 = ColorInterval[continuation_sec].ColoredIntervalsStructure;
				//ColoredIntervalsStructure2=ColorInterval[(continuation_sec-1)].ColoredIntervalsStructure;
				beg_int = ColoredIntervalsStructure->BegInterv[prev_corn];
				end_int = ColoredIntervalsStructure->EndInterv[prev_corn];
				length_int = end_int - beg_int + 1;
				small_bunch = length_int / StripWidth;
				signif_bunch = ColoredIntervalsStructure->Significance[prev_corn];
				density_bunch = (2 * signif_bunch) / length_int;
				new_beg_int = ColoredIntervalsStructure1->BegInterv[bunch_new];
				new_end_int = ColoredIntervalsStructure1->EndInterv[bunch_new];
				length_int_new = new_end_int - new_beg_int + 1;
				small_new = length_int_new / StripWidth;
				max_strip = std::max(small_bunch, small_new);
				min_strip = std::min(small_bunch, small_new);
				signif_bunch_new = ColoredIntervalsStructure1->Significance[bunch_new];
				density_bunch_new = (2 * signif_bunch_new) / length_int_new;
				density_ratio = (16 * density_bunch_new) / (density_bunch_new + density_bunch);
				bunch_significance_ratio = (16 * signif_bunch_new) / (signif_bunch + signif_bunch_new);
				Intersection = Measure1_of_proximity(beg_int, end_int, new_beg_int, new_end_int, &coinsidence,
					&coinsidence1, &shift_to_left, &shift_to_right, &indic_length, &ratio_length, &ratio_length1);
				test_junction =
					Test_Junction_Different_Strips_Same_Image(bunch_new, prev_corn, Intersection, indic_length,
						ratio_length, ratio_length1, continuation_sec, previous_sec, &hue_close, &sat_close, &gray_close);
				test_junction = test_junction % 10;
				if ((hue_close == 1) && (sat_close == 1) && (gray_close == 1))
				{//last_cor09.02.16
					color_coincidence = 1;
				}//last_cor09.02.16
				if ((test_junction >= 5) && (test_junction <= 6))
				{//last_cor02.03.15
					if ((hue_close<0) || (sat_close<0) || (gray_close<0))
					{
						prior = -2;
						return(prior);
					}
				}//last_cor02.03.15
				if ((continuation_sec - previous_sec) == 2)
				{//last_cor18.09.14

					left_contrast = ColorInterval[continuation_sec].left_continuation[bunch_new];
					right_contrast = ColorInterval[continuation_sec].right_continuation[bunch_new];
					if (!LeftRightBoundary)
					{//last_cor12.02.15
						bunch_contrast = right_contrast;
						bound_shift = shift_to_left;
						end_shift = shift_to_right;
					}
					else
					{
						bunch_contrast = left_contrast;
						bound_shift = shift_to_right;
						end_shift = shift_to_left;
					}
					if ((test_junction >= 5) && (test_junction <= 6) && (bound_shift>4 * m_shift))
					{
						if ((hue_close<0) || (sat_close<0))
						{
							prior = -3;
							return(prior);
						}
					}
					if (bunch_contrast<0)
					{//bc<0
						if ((!color_coincidence) || (max_strip >= 8))
						{//last_cor09.02.16
							if ((ratio_length <= 1) || (ratio_length1 <= 1))
							{
								if (std::min(length_int, length_int_new)>4 * StripWidth)
								{
									prior = -2;
									return(prior);
								}
							}
						}
						if (bunch_contrast == -6)
						{
							if ((!color_coincidence) || (max_strip >= 8))
							{//last_cor09.02.16
								if ((density_ratio<6) && (ratio_length<3))
								{//last_cor02.03.15
									prior = -2;
									return(prior);
								}
							}
						}

					}//bc<0
					if (sect_shift>3)
					{//ssh3
						if (previous_sec >= 1)
						{
							prev_prev_corn = DescrSec[num_sec].location_of_section[(previous_sec - 1)];
							if (prev_prev_corn>0)
							{
								prev_prev_corn--;
							}
							else
							{
								prior = -1;
								return(prior);
							}
							prev_prev_corn = ColorInterval[(previous_sec - 1)].old_bunch_number[prev_prev_corn];
						}

					}//ssh3
				}//last_cor18.09.14

				if (!LeftRightBoundary)
				{
					boundary_shift = shift_to_left;
				}
				else
				{
					boundary_shift = shift_to_right;
				}

				if ((test_junction >= 1) && (test_junction <= 2))
				{
					prior = 1;
					return(prior);
				}
				else
				{
					if ((test_junction >= 2) && (test_junction <= 4))
					{
						prior = 0;
					}
					else
					{
						if ((test_junction >= 5) && (test_junction <= 7) && (((hue_close <= 0) && (sat_close<0)) ||
							((gray_close <= 0) && (sat_close <= 0))))
						{
							prior = -2;
						}
					}
					if ((hue_close>0) && (sat_close>0) && (gray_close>0))
					{//last_cor26.11.15
						prior = 2;
					}//last_cor26.11.15
				}
				return(prior);
			}//end

		//---------------------------------------------------------------------------
		int

		CColorSection::FindingLowerAdjacentSections(void)
		{
			int prior;
				int section_total_weight;
				int first_fibre;
				int last_fibre;
				int first_member;
				int section_length;
				int old_num_first;
				int fiber_beg;
				int fiber_end;
				int disordering;
				int point_scale;
				int point_scale1;
				int oppos_bunch;
				int oppos_bunch1;
				int oppos_beg;
				int oppos_end;
				int Intersection;
				int indic_length;
				int ratio_length;
				int ratio_length1;
				int number_of_covering_bunches;
				int ordered_oppos_number;
				int ordered_oppos_number1;
				int first_in_ordering_old_number;
				int succees_index;

				TIntColored* ColoredIntervalsStructure;
				TIntColored* ColoredIntervalsStructure1;

				prior = -1;
				succees_index = 0;

				for (int coun = 0; coun<Number_of_sections; coun++)
				{//loop
					section_total_weight = 0;
					first_fibre = *(section_fibre_first + coun);
					last_fibre = *(section_fibre_last + coun);
					if (first_fibre >= 3)
					{//ff>=3
						first_member = DescrSec[coun].location_of_section[first_fibre] - 1;
						section_length = DescrSec[coun].length_of_section;
						old_num_first = ColorInterval[first_fibre].old_bunch_number[first_member];
						ColoredIntervalsStructure = ColorInterval[first_fibre].ColoredIntervalsStructure;
						ColoredIntervalsStructure1 = ColorInterval[first_fibre - 1].ColoredIntervalsStructure;
						fiber_beg = ColoredIntervalsStructure->BegInterv[old_num_first];
						fiber_end = ColoredIntervalsStructure->EndInterv[old_num_first];
						point_scale = fiber_beg / 4;
						point_scale1 = fiber_end / 4;
						for (int num_strp = first_fibre - 1; num_strp >= 0; num_strp--)
						{//strip_loop
							disordering = ColorInterval[num_strp].Disordering;
							if (!disordering)
							{//diso0
								ColoredIntervalsStructure1 = ColorInterval[num_strp].ColoredIntervalsStructure;
								number_of_covering_bunches = ColorInterval[num_strp].NumberOfIntervalsInCovering;
								first_in_ordering_old_number = ColorInterval[num_strp].old_ordered_bunch_number[0];
								oppos_bunch = ColorInterval[num_strp].painted_strip_colored[point_scale];

								oppos_bunch1 = ColorInterval[num_strp].painted_strip_colored[point_scale1];

								if (oppos_bunch>0)
								{//ob
									oppos_bunch--;
									ordered_oppos_number = ColorInterval[num_strp].ordered_bunch_number[oppos_bunch];
									if ((ordered_oppos_number>0) || ((!ordered_oppos_number)
										&& (oppos_bunch == first_in_ordering_old_number)))
									{//oon>
										oppos_beg = ColoredIntervalsStructure1->BegInterv[oppos_bunch];
										oppos_end = ColoredIntervalsStructure1->EndInterv[oppos_bunch];
										Intersection =
											ColorInterval[num_strp].Intersection_measure_ratios(fiber_beg, fiber_end,
												oppos_beg, oppos_end, &indic_length, &ratio_length, &ratio_length1);
										if (oppos_bunch1>0)
										{//ob1
											oppos_bunch1--;
											ordered_oppos_number1 = ColorInterval[num_strp].ordered_bunch_number[oppos_bunch1];
											if ((ordered_oppos_number1>0) || ((!ordered_oppos_number1)
												&& (oppos_bunch1 == first_in_ordering_old_number)))
											{//oon1>
												succees_index =
													WritingLowerAdjacentSections(num_strp, ordered_oppos_number, ordered_oppos_number1, coun);
												if (succees_index >= 1)
												{
													break;
												}
												/*if(Intersection<=1)
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
												}
												}//rl>last_cor02.11.15
												}//int*/
											}//oon1
										}//ob1
									}//oon>
								}//ob
							}//diso0
						}//strip_loop
						 //return(prior);
					}//ff>=3
				}//loop
				return(prior);
			}
		//-------------------------------------------------------------------------
		int

		CColorSection::WritingLowerAdjacentSections(int strip_number,int left_bunch,int right_bunch,
		int num_sect)
		{
			int prior;
				int old_bunch_number;
				int section_number;
				int section_oppos_number;
				int section_count;

				prior = 0;
				section_count = 0;
				for (int count = left_bunch; count <= right_bunch; count++)
				{//loop_bunch
					old_bunch_number = ColorInterval[strip_number].old_ordered_bunch_number[count];
					if (num_sect<Number_of_sections_left)
					{
						section_number = trace_of_section1[strip_number*MAX_COL_INT + old_bunch_number];
						section_oppos_number = trace_of_section[strip_number*MAX_COL_INT + old_bunch_number];
					}
					else
					{
						section_number = trace_of_section[strip_number*MAX_COL_INT + old_bunch_number];
						section_oppos_number = trace_of_section1[strip_number*MAX_COL_INT + old_bunch_number];
					}


					section_number = section_number % 1000;
					if (section_number>0)
					{//sn>0
						section_number -= 1;
						if (section_count<7)
						{
							sections_under[(num_sect * 8) + section_count] = section_number + 1;
							section_count++;
						}
						else
						{
							sections_under[(num_sect * 8) + 7] = section_count;
							prior = 2;
							return(prior);
						}

						prior = 1;
					}//sn>0
				}//loop_bunch
				if (prior == 1)
				{
					sections_under[(num_sect * 8) + 7] = section_count;
				}
				return(prior);
			}
		//================================================================================
		int

		CColorSection::SectionGeometry(int section_num,int* meam_l,int* mean_u)
		{
			int prior;
				int last_strip_ratio;
				int left_right;
				int last_fiber;
				int section_length;
				int first_fiber;
				int count_point;
				int current_fiber_beg;
				int current_fiber_end;
				int current_bunch;
				int last_left_section;
				int section_weight_sec;
				int first_boun;
				int last_boun;
				int mid_fiber;
				int upper_sum;
				int lower_sum;
				int upper_mean;
				int lower_mean;
				int upper_count;
				int lower_count;
				int current_fiber_length;
				//int* incline_points;
				//int* oppos_incline_points;
				//int* seq_points;
				//int* oppos_seq_points;
				TIntColored* ColoredIntervalsStructure;


				prior = -1;
				upper_sum = 0;
				lower_sum = 0;
				upper_count = 0;
				lower_count = 0;
				upper_mean = -1;
				lower_mean = -1;
				last_left_section = Number_of_sections_left;
				if (section_num<last_left_section)
				{
					left_right = 0;
				}
				else
				{
					left_right = 1;
				}
				last_fiber = section_fibre_last[section_num];
				first_fiber = section_fibre_first[section_num];
				mid_fiber = (last_fiber - first_fiber) / 2 + first_fiber;
				section_length = last_fiber - first_fiber + 1;
				last_strip_ratio = (16 * last_fiber) / (NumStrips - 1);
				section_weight_sec = section_weight[section_num];
				if (section_length >= 4)
				{
					/*incline_points=new int[section_length];
					oppos_incline_points=new int[section_length];
					seq_points=new int[section_length];
					oppos_seq_points=new int[section_length];*/
					count_point = 0;
					/*memset(incline_points,(int) '\0',sizeof(int)*(section_length));
					memset(oppos_incline_points,(int) '\0',sizeof(int)*(section_length));*/
					first_boun = -1;
					last_boun = -1;
					for (int count_fiber = first_fiber; count_fiber <= last_fiber; count_fiber++)
					{//loop_fiber
						current_bunch = DescrSec[section_num].location_of_section[count_fiber];
						ColoredIntervalsStructure = ColorInterval[count_fiber].ColoredIntervalsStructure;
						if (current_bunch>0)
						{//cb>
							current_bunch--;
							current_bunch = ColorInterval[count_fiber].old_bunch_number[current_bunch];
							current_fiber_beg = ColoredIntervalsStructure->BegInterv[current_bunch];
							current_fiber_end = ColoredIntervalsStructure->EndInterv[current_bunch];
							current_fiber_length = current_fiber_end - current_fiber_beg + 1;
							if (count_fiber <= mid_fiber)
							{
								lower_sum += current_fiber_length;
								lower_count++;
							}
							else
							{
								upper_sum += current_fiber_length;
								upper_count++;
							}
							/*if(!left_right)
							{
							seq_points[count_point]=current_fiber_beg;
							oppos_seq_points[count_point]=current_fiber_end;
							}
							else
							{
							seq_points[count_point]=current_fiber_end;
							oppos_seq_points[count_point]=current_fiber_beg;
							}*/
						}//cb
						else
						{
							return(-2);
						}
						count_point++;
					}//loop_fiber
					if (lower_count>0)
					{
						lower_mean = lower_sum / lower_count;
					}
					if (upper_count>0)
					{
						upper_mean = upper_sum / upper_count;
					}
					*meam_l = lower_mean;
					*mean_u = upper_mean;
					prior = 1;
					/*delete [] incline_points;
					delete [] oppos_incline_points;
					delete [] seq_points;
					delete [] oppos_seq_points;*/
				}
				return(prior);
			}
