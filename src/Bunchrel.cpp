/*
 *
 *
 *
 *
 *
 *
 *
 */




#include "ImageProcess.h"




#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>



using namespace std;






static  int dev_mean[8] = {5, 8, 8, 8, 8, 10, 12, 8};//last_cor11.12.17
static  int imp_dev_mean[8] = {12, 14, 16, 16, 14, 14, 14, 12};//last_cor27.01.16
static  int dev_mean_sat[16] = {1, 1, 1, 1, 1, 2, 3, 4, 4, 4, 4, 4, 3, 2, 1, 0};
static  int dev_mean_sat1[16] = {0, 1, 1, 1, 1, 1, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4};
static  int imp_dev_mean_sat[16] = {3, 3, 3, 3, 4, 5, 6, 7, 7, 6, 6, 4, 4, 4, 4, 4};//14.12.15
static  int imp_dev_mean_sat1[16] = {1, 2, 2, 3, 3, 3, 3, 4, 5, 5, 5, 6, 6, 7, 7, 6};//14.12.15

static  int hue_zones[NUM_HUES] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
5, 1, 1, 1, 1, 1};

static  int dev_hue_zone[6] = {2, 2, 2, 2, 2, 2};

static int sat_max_deviation[16] = {1, 1, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 3, 3, 1,
0};

static int sat_min_deviation[16] = {0, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4, 4, 4, 3,
3};

static  int uppermax_hue_dif[NUM_HUES] = {2, 2, 1, 1, 2, 2, 1, 1, 2, 5, 6, 6, 6,
6, 5, 4, 3, 3, 3, 2, 2, 2, 1, 1, 3, 3, 4, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3,
2, 3, 4, 5, 4, 4, 3, 3};

static  int lowermax_hue_dif[NUM_HUES] = {5, 3, 2, 1, 2, 2, 3, 3, 1, 1, 2, 2, 3,
4, 5, 5, 5, 6, 6, 3, 3, 2, 2, 2, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 4, 4, 3, 3, 3,
3, 3, 2, 2, 3, 3, 4, 4};
//last_cor04.02.14
static  int gray_zones[NUM_INTEN1] = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2,
2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7};
//0. 0-6, 1. 7-12, 2. 13-21, 3. 22-30, 4. 31-39, 5. 40-49, 6. 50-57, 7. 58-63
static int up_gray_val_dev[8] = {2, 3, 5, 8, 6, 5, 0, 0};

static int down_gray_val_dev[8] = {0, 2, 3, 6, 8, 6, 5, 0};

static int inside_gray_val_dev[8] = {3, 4, 6, 6, 6, 8, 8, 0};

static  int uppermax_hc_dif[NUM_HUES] = {6, 6, 6, 6, 6, 6, 7, 7, 12, 12, 12, 11,
 10, 9, 8, 8, 8, 8, 7, 7, 7, 8, 8, 9, 10, 10, 10, 9, 9, 9, 9, 9, 9, 8, 7, 7, 7,
7, 7, 8, 8, 9, 9, 9, 9, 9, 8, 7};

static  int lowermax_hc_dif[NUM_HUES] = {10, 10, 9, 10, 10, 9, 8, 7, 7, 6, 7, 7,
 7, 7, 8, 9, 10, 11, 12, 12, 12, 11, 10, 10, 9, 9, 8, 8, 8, 8, 8, 9, 9, 10, 10,
 10, 10, 9, 9, 8, 7, 7, 7, 7, 7, 7, 8, 8};
static int color_lower_bound[8] = {0, 39, 0, 3, 5, 15, 20, 31};
static int color_upper_bound[8] = {47, 6, 6, 10, 25, 31, 39, 44};
static int color_mean[8] = {-1,0,3, 6, 15, 24, 31, 37};


//=====================================================
void

CColorIntervalSelect::BunchClassification()
{
	int coun_bunch;
	int beg_int;
	int end_int;
	int signif;
	int coun_place;
	//int Max_number;
	int Max_loc_number;
	//int number_of_bunches;
	//int* locally_visible_bunches;
	int bunch_num;
	int coun1_bun;
	int coun_bun;
	int coun_bunch1;
	int count_bunch;
	int count_bu;
	int bunch_occurrence;
	int left_neighbor;
	int right_neighbor;
	int bunch_occurrence1;
	int test_junction;
	int coun_place1;
	int* encountered_bunches;
	int* explored_bunches;
	int* intersecting_bunches;
	int* intersecting_type;
	int number_intersect;
	//int number_encountered;
	int num_bunch;
	int test_bunch;
	int Number_Connections;
	int block_index;
	int block_index1;
	int b_neighbor;
	int empty_places;
	int point_beg;
	int point_end;
	int Intersection;
	int indic_length;
	int ratio_length;
	int ratio_length1;
	int neighbor_beg_int;
	int neighbor_end_int;
	int right_neighbor_beg_int;
	int right_neighbor_end_int;
	int paint_start;
	int paint_finish;
	int left_bunch_domin;
	int right_bunch_domin;
	int neighbor_point_beg;
	int neighbor_point_end;
	int right_neighbor_point_beg;
	int right_neighbor_point_end;
	int neighbor_end_dominant;
	int neighbor_beg_dominant;
	int right_neighbor_end_dominant;
	int right_neighbor_beg_dominant;
	int bunch_significance;
	int bunch_density;
	int bunch_new_significance;
	int neighbor_bunch_significance;
	int neighbor_bunch_density;
	int right_bunch_new_significance;
	int right_neighbor_bunch_density;
	int density_ratio;
	int density_sum;
	int small_hole;
	int right_small_hole;
	int left_domin;
	int right_domin;
	int difference_neighbors;
	int gap_between_neighbors;
	int first_right_occurred;
	//int right_hole_size;
	int right_gap_length;
	int first_left_occurred;
	//int right_hole_size;
	int left_gap_length;
	//int bunch_encoun;
	//bool FindLabels;
	/*int first_lower_hue;
	int first_upper_hue;
	int first_mean_hue;
	int second_lower_hue;
	int second_upper_hue;
	int second_mean_hue;
	int third_lower_hue;
	int third_upper_hue;
	int third_mean_hue;*/
	/*int ColorLabel1;
	int ColorLabel2;
	int ColorLabel3;*/
	int label_value;
	int outlet_length_ratio;
	int bunch_int_length;
	int union_length;
	int union_beg;
	int union_end;
	int outlet_length_ratio1;
	int outlet_length_ratio2;
	int outlet_length_ratio3;
	int outlet_length;
	int outlet_length1;
	int neighbor_length;
	int bunch_saturation;
	int neighbor_saturation;
	int center_int;
	int central_neighbor;
	int bunch_suitability;
	int new_left_domin_point;
	int new_left_domin;
	int hue_close;
	int saturation_close;
	int gray_close;
	int right_differ;
	int left_differ;
	int count_bunch3;
	int left_bunch_contrast,right_bunch_contrast,bunch_left_adjacent,bunch_right_adjacent;
	int sign_intersect;
	int count_labels;
	int new_right_adj;
	int	new_right_contr;
	int new_left_adj;
	int	new_left_contr;
	int left_neighbor_hue;
	int left_neighbor_gray;
	int left_neighbor_sat;
	int left_hue_zone;
	int bunch_gray;
	int bunch_sat;
	int Number_of_Merging;
	int count_improve;
	int test_jun;
	int test_jun_left;
	int left_neighbor_end;
	int right_neighbor_beg;
	int length_l_r;
	int leng_rat;
	int orderning_result;
	int linear_ordering_result;
	int last_bunch_num;
	int smallest_coordinate;
	int greatest_coordinate;
	int bunch_hue;//13.01.15
	int bunch_hue_zone;//13.01.15
	//int occurred_dimension;

	bunch_hue=-1;
	bunch_hue_zone=-1;
	orderning_result=-1;
	linear_ordering_result=-1;
	last_bunch_num=-1;
	smallest_coordinate=-1;
	greatest_coordinate=-1;
	NumberOfIntervalsInCovering=0;
	Disordering=0;
	if(!NumberOfColoredIntervals)
	{//22.10.13
	return;
	}

	if (VideoCameraIsLoaded)
	{
		ColorBunchesCharacteristics->num_of_bunches=NumberOfColoredIntervals;
		visible_bunches=ColorBunchesCharacteristics->visible_bunches;
		bunch_blocking=ColorBunchesCharacteristics->bunch_blocking;
		left_continuation=ColorBunchesCharacteristics->left_continuation;
		right_continuation=ColorBunchesCharacteristics->right_continuation;
	    left_adjacent=ColorBunchesCharacteristics->left_adjacent;
		right_adjacent=ColorBunchesCharacteristics->right_adjacent;
		new_left_continuation=ColorBunchesCharacteristics->new_left_continuation;
			new_right_continuation=ColorBunchesCharacteristics->new_right_continuation;
		    new_left_adjacent=ColorBunchesCharacteristics->new_left_adjacent;
			new_right_adjacent=ColorBunchesCharacteristics->new_right_adjacent;
			ordered_bunch_number=ColorBunchesCharacteristics->ordered_bunch_number;
			//memset(ordered_bunch_number, 0, sizeof(int) * NumberOfColoredIntervals); // ������
			old_ordered_bunch_number=ColorBunchesCharacteristics->old_ordered_bunch_number;
		//label_segments=ColorBunchesCharacteristics->label_segments;
		//center_bunch_suitability=ColorBunchesCharacteristics->center_bunch_suitability;
		new_bunch_number=ColorBunchesCharacteristics->new_bunch_number;
	    old_bunch_number=ColorBunchesCharacteristics->old_bunch_number;
	}
	else
	{
	visible_bunches=new int[NumberOfColoredIntervals];
	bunch_blocking=new int[NumberOfColoredIntervals];
	left_continuation=new int[NumberOfColoredIntervals];
	right_continuation=new int[NumberOfColoredIntervals];
	left_adjacent=new int[NumberOfColoredIntervals];
	right_adjacent=new int[NumberOfColoredIntervals];
	new_left_continuation=new int[NumberOfColoredIntervals];
	new_right_continuation=new int[NumberOfColoredIntervals];
	new_left_adjacent=new int[NumberOfColoredIntervals];
	new_right_adjacent=new int[NumberOfColoredIntervals];
	ordered_bunch_number=new int[NumberOfColoredIntervals];
	old_ordered_bunch_number=new int[NumberOfColoredIntervals];
	//label_segments=new int[NumberOfColoredIntervals];
	//center_bunch_suitability=new int[NumberOfColoredIntervals];
	new_bunch_number=new int[NumberOfColoredIntervals];
	old_bunch_number=new int[NumberOfColoredIntervals];
	}

	explored_bunches=new int[NumberOfColoredIntervals];
	Number_Connections=NumberOfColoredIntervals*NumberOfColoredIntervals;
	encountered_bunches=new int[NumberOfColoredIntervals];
	intersecting_bunches=new int[NumberOfColoredIntervals];
	intersecting_type=new int[NumberOfColoredIntervals];
	bunch_connections=new int[Number_Connections];
	//locally_visible_bunches=new int[NumberOfColoredIntervals];

	length_l_r=-1;
	leng_rat=-1;
	left_neighbor_end=-1;
	right_neighbor_beg=-1;
	new_left_domin_point=-1;
	new_left_domin=-1;
	indic_length=-1;
	ratio_length=-1;
	ratio_length1=-1;
	bunch_suitability=-1;
	test_jun_left=0;
	Intersection=3;
	test_junction=0;
	Max_number=0;
	empty_places=0;
	Max_loc_number=0;
	memset(bunch_connections,(int) '\0',sizeof(int)*(Number_Connections));
	memset(painted_strip_colored,(int) '\0',sizeof(int)*(PressedLength));
	memset(painted_numbers_colored,(int) '\0',sizeof(int)*(PressedLength));
	memset(painted_strip_colored_long,(int) '\0',sizeof(int)*(StripLength));
	memset(painted_numbers_colored_long,(int) '\0',sizeof(int)*(StripLength));

	memset(visible_bunches,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	memset(bunch_blocking,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	memset(left_continuation,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	memset(right_continuation,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	memset(left_adjacent,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	memset(right_adjacent,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	memset(new_left_continuation,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	memset(new_right_continuation,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	memset(new_left_adjacent,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	memset(new_right_adjacent,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	memset(ordered_bunch_number, 0, sizeof(int) * NumberOfColoredIntervals); // ������
	//memset(label_segments,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	//memset(center_bunch_suitability,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	memset(explored_bunches,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
	/*memset(locally_visible_bunches,(int) '\0',
	sizeof(int)*(NumberOfColoredIntervals));*/
	strip_number=StripCurrent->num_strip;//last_cor22.05.15
	NumStrip=StripCurrent->NumbStr;
	for(coun_bunch=0;coun_bunch<NumberOfColoredIntervals;coun_bunch++)
	{
	beg_int=ColoredIntervalsStructure->BegInterv[coun_bunch];
	end_int=ColoredIntervalsStructure->EndInterv[coun_bunch];
	signif=ColoredIntervalsStructure->Significance[coun_bunch];
	signif=(signif<<1)/(end_int-beg_int+1);
	Strip_value_painting_bunch(coun_bunch,beg_int/4,end_int/4,signif,
	painted_strip_colored,painted_numbers_colored,consistency_colored,PressedLength);
	}
		for(coun_place=0;coun_place<PressedLength/2;coun_place++)
		{
		bunch_num=*(painted_strip_colored+coun_place);
		if((bunch_num>0)&&(bunch_num<NumberOfColoredIntervals))
		{
		(*(visible_bunches+bunch_num-1))++;
		//number_of_bunches=*(painted_numbers_colored+coun_place);
			/*if(number_of_bunches>Max_number)
			{
			Max_number=number_of_bunches;
			}*/
		 }
		else
		{
	    empty_places++;
		}
		}

		for(coun1_bun=0;coun1_bun<NumberOfColoredIntervals;coun1_bun++)
			{
			bunch_occurrence=*(visible_bunches+coun1_bun);
			beg_int=ColoredIntervalsStructure->BegInterv[coun1_bun];
			end_int=ColoredIntervalsStructure->EndInterv[coun1_bun];
			point_beg=beg_int/4;
			point_end=end_int/4;
			bunch_occurrence=(16*bunch_occurrence)/(point_end-point_beg+1);
			*(visible_bunches+coun1_bun)=bunch_occurrence;
			}
		for(coun_bun=0;coun_bun<NumberOfColoredIntervals;coun_bun++)
			{//loop
			test_junction=0;
			outlet_length_ratio=-1;
			block_index=*(bunch_blocking+coun_bun);
			bunch_occurrence=*(visible_bunches+coun_bun);
			if(!block_index)
			  {//block and bo rather small
			beg_int=ColoredIntervalsStructure->BegInterv[coun_bun];
			end_int=ColoredIntervalsStructure->EndInterv[coun_bun];
			bunch_significance=ColoredIntervalsStructure->Significance[coun_bun];
			bunch_int_length=end_int-beg_int+1;
			bunch_density=(2*bunch_significance)/bunch_int_length;

			bunch_saturation=ColoredIntervalsStructure->AverageSat[coun_bun];
			point_beg=beg_int/4;
			point_end=end_int/4;
			 center_int=(point_beg+point_end)/2;
			 central_neighbor=*(painted_strip_colored+center_int);
			 if((central_neighbor>0)&&(central_neighbor!=coun_bun+1))
			 {//central_neighbor
				 central_neighbor-=1;
				 b_neighbor=*(bunch_blocking+central_neighbor);
					if(b_neighbor>0)
					{
					central_neighbor=b_neighbor-1;
					}
					block_index1=*(bunch_blocking+central_neighbor);
					if((!block_index1)&&(central_neighbor!=coun_bun))
					{//bi1
		neighbor_beg_int=ColoredIntervalsStructure->BegInterv[central_neighbor];
		neighbor_end_int=ColoredIntervalsStructure->EndInterv[central_neighbor];
		neighbor_length=neighbor_end_int-neighbor_beg_int+1;
		neighbor_saturation=ColoredIntervalsStructure->AverageSat[central_neighbor];
		bunch_occurrence1=*(visible_bunches+central_neighbor);
		union_beg=std::min(beg_int,neighbor_beg_int);
		union_end=std::max(end_int,neighbor_end_int);
			union_length=union_end-union_beg+1;
			outlet_length=union_length-neighbor_length;
			outlet_length_ratio=(16*bunch_int_length)/(bunch_int_length+union_length);
			outlet_length_ratio1=(16*outlet_length)/(outlet_length+union_length);
			outlet_length_ratio2=(16*neighbor_length)/(neighbor_length+union_length);//last_cor06.03.13
		Intersection=Intersection_measure_ratios(neighbor_beg_int,neighbor_end_int,beg_int,
			end_int,&indic_length,&ratio_length,&ratio_length1);
		if(Intersection==3)
		{
			ratio_length=0;
		}
		//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
			//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
		neighbor_bunch_significance=ColoredIntervalsStructure->Significance[central_neighbor];
		neighbor_bunch_density=(2*neighbor_bunch_significance)/neighbor_length;
		density_sum=neighbor_bunch_density+bunch_density;
		density_ratio=(16*bunch_density)/(bunch_density+neighbor_bunch_density);

			test_junction=Test_Junction_Corrected(coun_bun,central_neighbor,bunch_occurrence,
		Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
			test_jun=test_junction%10;
			if(test_jun==1)
			{//tj1last_cor09.09.14
				if(ratio_length==16)
				{//rl16
					if((density_ratio>=7)&&(density_ratio<=9))
					{//dr79
						if(ratio_length1<=8)
						{//rl1_8
					 block_index=coun_bun+1;
					*(bunch_blocking+central_neighbor)=block_index;
					if(hue_close==1)
								{//last_cor15.02.17
								Merging(central_neighbor,coun_bun);
								}
								else
								{
								SimpleMerging(coun_bun,central_neighbor);
								}
								goto L;//last_cor15.02.17
						}//rl1_8
					else
					{//e8
						if((bunch_occurrence1==16)&&(!bunch_occurrence))
						{//bo160
			  left_neighbor=*(painted_strip_colored+point_beg);
			  if((left_neighbor>=0)&&(left_neighbor!=coun_bun+1))
			 {//left_neighbor
				 //left_neighbor-=1;
				 left_neighbor_end=ColoredIntervalsStructure->EndInterv[left_neighbor];

			  right_neighbor=*(painted_strip_colored+point_end);
			  if((right_neighbor>0)&&(right_neighbor!=coun_bun+1))
			 {//right_neighbor
				 //right_neighbor-=1;
				 right_neighbor_beg=ColoredIntervalsStructure->BegInterv[right_neighbor];

		 if((left_neighbor==central_neighbor)&&(right_neighbor==central_neighbor))
		  {
			  block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					Merging(central_neighbor,coun_bun);
					goto L;
		  }
		 else
		 {
			 length_l_r=right_neighbor_beg-left_neighbor_end+1;
			      if(length_l_r>0)
			       {//errorlast_cor15.06.15
			 leng_rat=(16*length_l_r)/(length_l_r+neighbor_length);
			 if(leng_rat>=12)
			 {
		            block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					Merging(central_neighbor,coun_bun);
					goto L;
			 }
				  }//errorlast_cor15.06.15
		 }
			  }//right_neighbor
			  }//left_neighbor
						}//bo160
					}//e8
					}//dr79
				}//rl16
			}//tj1
			if((test_jun>=1)&&(test_jun<=2)&&(ratio_length1==16)&&(outlet_length_ratio1<=3))
			{
				block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					if(hue_close==1)
								{//last_cor15.02.17
								Merging(central_neighbor,coun_bun);
								}
								else
								{
									SimpleMerging(central_neighbor,coun_bun);
								}
								goto L;//last_cor15.02.17
			}
			else
			{
				if((test_jun>=1)&&((test_jun<=2))&&(ratio_length1==16)&&(outlet_length_ratio1<=8)&&(density_ratio<=10))
			{
				block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					Merging(central_neighbor,coun_bun);
					goto L;
			}
			}
			if((test_jun>0)&&(test_jun<=2))
							 {//tj021012
				                         if(!bunch_occurrence)
										 {//last_cor29.01.16
											 if((ratio_length1>=8)&&(outlet_length_ratio1<=4))
											 {
												 if(density_ratio<=6)
												 {
				             block_index=central_neighbor+1;
							*(bunch_blocking+coun_bun)=block_index;
							SimpleMerging(central_neighbor,coun_bun);
							goto L;
												 }
											 }
										 }
		if((density_ratio<=2)&&(ratio_length1==16))
		{
			if(ratio_length<4)
			{
			        block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					SimpleMerging(central_neighbor,coun_bun);
					goto L;
			}
			else
			{
		    block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					Merging(central_neighbor,coun_bun);
					goto L;
			}
		}
								 if(test_jun==1)
								 {//tj111

									 if(Intersection==2)
									 {//I2
		if((density_ratio>=6)&&(density_sum>(StripWidth+StripWidth/2))&&(outlet_length_ratio1<=3))
									 {//c2 last_cor17.10.13
										 if((bunch_occurrence1>=12)&&(bunch_occurrence<=2))
										 {
					block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					Merging(central_neighbor,coun_bun);
					goto L;
										 }
									 }//c2
										 /*if((!ratio_length)&&(!ratio_length1))
										 {

										 }*/
		 if((density_ratio>=4)&&(density_sum>(StripWidth+StripWidth/4))&&(outlet_length_ratio1<=2))
									 {//c1 last_cor
					block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					Merging(central_neighbor,coun_bun);
					goto L;
									 }//c1
		 else
		 {//last_cor
		if((test_jun==1)&&(density_ratio>=7)
		   &&(density_sum>(StripWidth+StripWidth/4))&&(outlet_length_ratio1<=4))
		{
		block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					Merging(central_neighbor,coun_bun);
					goto L;
		}
		 }//last_cor
									 }//I2
									 else
									 {//e2
		if((Intersection<=1)&&((ratio_length==16)&&(outlet_length_ratio2>=11))
		   ||((ratio_length1==16)&&(outlet_length_ratio1<=5)))
										 {//c2
											 if(ratio_length1==16)
											 {//c3
												 if((density_sum>(StripWidth+StripWidth/4))&&(density_ratio<=8))
									  {//c4
					block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					Merging(central_neighbor,coun_bun);
					goto L;
									 }//c4
											 }//c3
											 else
											 {//e3
		if((density_ratio<=5)&&(density_sum>StripWidth))
									 {//c5
					block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					Merging(central_neighbor,coun_bun);
					goto L;
									 }//c5
											 }//e3
										 }//c2
									 }//e2
								 }//tj111
			 if((bunch_int_length<=4)&&(outlet_length_ratio<=2))
			{//c6
				if(density_ratio<=4)
				{//c7
					block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					Merging(central_neighbor,coun_bun);
					goto L;
				}//c7
			}//c6
								 if(((ratio_length>=14)&&(ratio_length1>=14))||(((ratio_length1==16))&&(density_ratio<5)))
		                          {//c8

		 if(((bunch_occurrence1>=bunch_occurrence)||
			 ((bunch_occurrence1>0)&&(indic_length>=2)))&&(neighbor_bunch_density>bunch_density)&&
			 (density_sum>(StripWidth+StripWidth/4)))
								 {//c9
								 block_index=central_neighbor+1;
								 *(bunch_blocking+coun_bun)=block_index;
								 Merging(central_neighbor,coun_bun);
								 goto L;
								 }//c9
								  }//c8
							 }//tj0211
			else
			{//e4
		if(test_jun==3)
		{//c10
			if((bunch_int_length<=4)&&(outlet_length_ratio<=2))
			{//c11
				if(density_ratio<=4)
				{//c12
					block_index=central_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					Merging(central_neighbor,coun_bun);
					goto L;
				}//c12
			}//c11
		}//c10
			}//e4


					}//bi1
			 }//central_neighbor
			 right_neighbor_beg=-1;
			 left_neighbor_end=-1;
			  if(point_beg!=point_end)
			  {
			  left_neighbor=*(painted_strip_colored+point_beg);
			  right_neighbor=*(painted_strip_colored+point_end);
			  }
			  else
			  {//small intervals
			  if(end_int+4<StripLength)
			  {
			  left_neighbor=*(painted_strip_colored+point_beg);
			  right_neighbor=*(painted_strip_colored+point_end+1);
			  }
			  else
			  {
		      right_neighbor=*(painted_strip_colored+point_end);
			  if(point_beg>=1)
			  {
			  left_neighbor=*(painted_strip_colored+point_beg-1);
			  }
			  else
			  {
			  left_neighbor=*(painted_strip_colored+point_beg);
			  }
			  }

			  }//small intervals
			  if(left_neighbor==coun_bun+1)
			  {
				  if(point_beg>=1)
			  {
			  left_neighbor=*(painted_strip_colored+point_beg-1);
			  }
			  }
			  if(right_neighbor==coun_bun+1)
			  {
				  if(end_int+4<StripLength)
			  {
			   right_neighbor=*(painted_strip_colored+point_end+1);
			  }
			  }
			  if(left_neighbor>=0)
			  {//last_cor15.09.14!last_cor29.10.14
				  left_neighbor-=1;
			  }
			  if(right_neighbor>=0)
			 {//last_cor15.09.14!last_cor29.10.14
				 right_neighbor-=1;
		      }

					if((left_neighbor!=coun_bun)&&(left_neighbor>=0))
					{//left neighbor exists//last_cor15.09.14
					//left_neighbor-=1;
					b_neighbor=*(bunch_blocking+left_neighbor);
					if(b_neighbor>0)
					{
					left_neighbor=b_neighbor-1;
					}
					block_index1=*(bunch_blocking+left_neighbor);
					bunch_occurrence1=*(visible_bunches+left_neighbor);
						  //if(bunch_occurrence1>=bunch_occurrence)
							//{//bo
								if((!block_index1)&&(left_neighbor!=coun_bun))
								{//bi

		neighbor_beg_int=ColoredIntervalsStructure->BegInterv[left_neighbor];
		neighbor_end_int=ColoredIntervalsStructure->EndInterv[left_neighbor];
		if(right_neighbor>=0)
			 {//last_cor15.09.14
				 //right_neighbor-=1;
				 right_neighbor_beg=ColoredIntervalsStructure->BegInterv[right_neighbor];
		      }

		left_neighbor_end=neighbor_end_int;
		neighbor_length=neighbor_end_int-neighbor_beg_int+1;
		left_differ=beg_int-neighbor_end_int;
		neighbor_saturation=ColoredIntervalsStructure->AverageSat[left_neighbor];
		bunch_occurrence1=*(visible_bunches+left_neighbor);
		union_beg=std::min(beg_int,neighbor_beg_int);
			union_end=std::max(end_int,neighbor_end_int);
			union_length=union_end-union_beg+1;
			outlet_length=union_length-neighbor_length;
			outlet_length_ratio=(16*bunch_int_length)/(bunch_int_length+union_length);
			outlet_length_ratio1=(16*outlet_length)/(outlet_length+union_length);
			outlet_length_ratio2=(16*neighbor_length)/(neighbor_length+union_length);
		    outlet_length_ratio3=(16*union_length)/(neighbor_length+union_length);
		Intersection=Intersection_measure_ratios(neighbor_beg_int,neighbor_end_int,beg_int,
			end_int,&indic_length,&ratio_length,&ratio_length1);
		test_junction=Test_Junction_Corrected(left_neighbor,coun_bun,bunch_occurrence1,
		Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
		test_jun=test_junction%10;
		test_jun_left=test_jun;
		neighbor_bunch_significance=ColoredIntervalsStructure->Significance[left_neighbor];
		neighbor_bunch_density=(2*neighbor_bunch_significance)/(neighbor_end_int-neighbor_beg_int+1);
		density_ratio=(16*bunch_density)/(bunch_density+neighbor_bunch_density);
		     if((test_jun>=1)&&(test_jun<=3))
		      {//tj13last_cor16.09.14
		if(test_jun==1)
		{//last_cor09.09.14
			if(bunch_int_length<StripWidth)
			{
				if(right_neighbor_beg>=end_int)
				{
					block_index=left_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					 Merging(left_neighbor,coun_bun);
					 goto L;
				}
			}
			else
			{//last_cor09.04.15and13.01.16
				if((hue_close==1)&&(saturation_close>=1)&&(saturation_close<=2)&&(gray_close>=1)&&
							(gray_close<=2))
						{
						if((outlet_length_ratio1<4)&&(density_ratio<7))
						{
							block_index=left_neighbor+1;
							*(bunch_blocking+coun_bun)=block_index;
							 SimpleMerging(left_neighbor,coun_bun);
							 goto L;
						}
						}
			}//last_cor09.04.15
		}//last_cor09.09.14
		if(end_int>=(StripLength-StripWidth/2))
		{//last_cor15.09.14
			if((outlet_length_ratio<=2)&&(outlet_length_ratio2>=6)&&(bunch_int_length<2*StripWidth))
			{
				if((test_jun>=1)&&(test_jun<=3))
				{
					block_index=left_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					 Merging(left_neighbor,coun_bun);
					 goto L;
				}
			}
		}
		if(((Intersection>=0)&&(Intersection<=2))||((Intersection==3)&&(ratio_length<=2)))
		{
		if(!outlet_length_ratio2)
		{//io0

		if(((test_jun>=1)&&(test_jun<=2))||((test_jun==3)&&(ratio_length>=15)))
				{
					if((bunch_occurrence>bunch_occurrence1)&&(density_ratio>=7))
					{
				block_index=coun_bun+1;
								 *(bunch_blocking+left_neighbor)=block_index;
								 SimpleMerging(coun_bun,left_neighbor);
								 goto L;
					}
				}
		}//io0
		else
		{//eol0
		if(outlet_length_ratio2<=2)
		{//ol2


				if((test_jun>=1)&&(test_jun<=2))
				{
					if((bunch_occurrence>bunch_occurrence1)&&(density_ratio>=7))
					{
				block_index=coun_bun+1;
								 *(bunch_blocking+left_neighbor)=block_index;
								 SimpleMerging(coun_bun,left_neighbor);
								 goto L;
					}
				}
				else//last_cor17.06.14
				{//last_cor12.01.16!!!!error
					if((hue_close==1)&&(saturation_close>=1)&&(saturation_close<=2)&&(gray_close==1))
					{//last_cor12.01.16!!!!error
								if((bunch_occurrence>bunch_occurrence1)&&(density_ratio>=7))
								{
							block_index=coun_bun+1;
											 *(bunch_blocking+left_neighbor)=block_index;
											 SimpleMerging(coun_bun,left_neighbor);
											 goto L;
								}
					}

				}

		}//ol2
		}//eol0
		}
		if(Intersection==3)
		{
			ratio_length=0;
		}
			//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
			//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller

		   if(((Intersection<=1)&&(indic_length>=2))||
		((Intersection==0)&&(indic_length>=1)&&(ratio_length>=12)&&(ratio_length1>=12)&&((ratio_length>=15)||(ratio_length1>=15))))
		   {//big intersection
		if((test_jun>0)&&(test_jun<=2))
							 {
		 if(((bunch_occurrence1>=bunch_occurrence))||
			 ((bunch_occurrence1>0)&&(indic_length>=2)&&(ratio_length1>=12)&&(density_ratio<=10)&&(density_ratio>=6)))
								 {
								 block_index=left_neighbor+1;
								 *(bunch_blocking+coun_bun)=block_index;
								 Merging(left_neighbor,coun_bun);
								 goto L;
								 }
							 }
							 else
							 {//not so close

		if(((test_jun>0)&&(test_jun<=3))
		   &&((ratio_length1>=15)||((ratio_length<=1)&&(ratio_length1>=11))||
		   (((ratio_length>=15)&&(ratio_length1>=12))||((ratio_length>=12)&&(ratio_length1>=15)))))
								 {
									 if((bunch_occurrence1>bunch_occurrence)&&(bunch_occurrence<4))
									 {
								 block_index=left_neighbor+1;
								 *(bunch_blocking+coun_bun)=block_index;
								 Merging(left_neighbor,coun_bun);
								 goto L;
									 }
								 }
		                         else
		                         {//not too big intersection but small outlet and very close colors
		  if(((test_jun>0)&&(test_jun<=2))
			  &&((outlet_length_ratio<=6)||(outlet_length_ratio1<=2))&&(indic_length>=2))
		  {
			  if((bunch_occurrence1>bunch_occurrence)&&(density_ratio<=9))
					{
		  block_index=left_neighbor+1;
		 *(bunch_blocking+coun_bun)=block_index;
		 Merging(left_neighbor,coun_bun);
		 goto L;
			        }
		  }
		                          }//not too big intersection but small outlet and very close colors
		                     }//not so close

		   }//big intersection

		  else
		   {//small intersection and small outlet
		if((Intersection==2)||((Intersection==3)&&(outlet_length_ratio<=2)&&(left_differ<4))||
		   ((Intersection==3)&&(outlet_length_ratio<=5)&&(left_differ<=1)))
		{//I=2
			if(test_junction==1)
		{//tj1
		if((density_ratio>=6)&&(density_ratio<=10)&&(bunch_occurrence1>=14)&&
		   (bunch_occurrence1>2*bunch_occurrence))
					{//last_cor31.10.13
				block_index=left_neighbor+1;
				*(bunch_blocking+coun_bun)=block_index;
				Merging(left_neighbor,coun_bun);
		goto L;
					}
		}//tj1
				if(test_junction==1)
				{//tj
					if((bunch_occurrence1>=8)&&(bunch_occurrence>=5)&&(neighbor_saturation>=2)&&(bunch_saturation>=2))
					{
					if((density_ratio>=6)&&(density_ratio<=9)&&(neighbor_bunch_density>(StripWidth+StripWidth/4)))
					{
				block_index=left_neighbor+1;
				*(bunch_blocking+coun_bun)=block_index;
				Merging(left_neighbor,coun_bun);
		goto L;
					}
					}
					if(((outlet_length_ratio<=6)||(outlet_length_ratio1<=2))&&(indic_length>=2))
			{//out
				  if((test_jun>0)&&(test_jun<4))
			      {
				if(!outlet_length_ratio)
			    {
		        block_index=left_neighbor+1;
				*(bunch_blocking+coun_bun)=block_index;
				Merging(left_neighbor,coun_bun);
				goto L;
				}
				  }
		if((neighbor_saturation>=2)&&(bunch_saturation>=2)&&(density_ratio>=7)&&(density_ratio<=9)&&
					(neighbor_bunch_density>(StripWidth+StripWidth/4)))
				   {
				  if(((test_jun)>0)&&((test_jun)<=1))
				  {
				block_index=left_neighbor+1;
				*(bunch_blocking+coun_bun)=block_index;
				Merging(left_neighbor,coun_bun);
				goto L;
				  }
				   }
		else
		{//e10
			if((test_jun==1)&&(density_ratio>=4)&&(density_ratio<=9)&&(outlet_length_ratio<=2))
			{
		     block_index=left_neighbor+1;
			*(bunch_blocking+coun_bun)=block_index;
			Merging(left_neighbor,coun_bun);
			goto L;
			}
		}//e10
			}//out
			   }//tj

		}//I=2
		   }//small intersection
		  }//tj13last_cor16.09.14
								}//bi
							//}//bo
					}//left neighbor exists
				 if(!block_index)
				 {//left neighbor ether does not exist or does not block the current bunch

					if((right_neighbor!=coun_bun)&&(right_neighbor>=0))
					{//right neighbor exists//last_cor15.09.14
							//right_neighbor-=1;
							b_neighbor=*(bunch_blocking+right_neighbor);
					if(b_neighbor>0)
					{
					right_neighbor=b_neighbor-1;
					}
					block_index1=*(bunch_blocking+right_neighbor);
							   if((right_neighbor!=left_neighbor)&&(!block_index1)&&(right_neighbor!=coun_bun))
							   {//rn is new and ln failed
					bunch_occurrence1=*(visible_bunches+right_neighbor);
						  //if(bunch_occurrence1>=bunch_occurrence)
							//{//bo cond for rn
		neighbor_beg_int=ColoredIntervalsStructure->BegInterv[right_neighbor];
		neighbor_end_int=ColoredIntervalsStructure->EndInterv[right_neighbor];
		neighbor_bunch_significance=ColoredIntervalsStructure->Significance[right_neighbor];
		neighbor_bunch_density=(2*neighbor_bunch_significance)/(neighbor_end_int-neighbor_beg_int+1);
		density_ratio=(16*bunch_density)/(bunch_density+neighbor_bunch_density);

		right_differ=neighbor_beg_int-end_int;
		neighbor_saturation=ColoredIntervalsStructure->AverageSat[right_neighbor];
		neighbor_length=neighbor_end_int-neighbor_beg_int+1;
		union_beg=std::min(beg_int,neighbor_beg_int);
			union_end=std::max(end_int,neighbor_end_int);
			union_length=union_end-union_beg+1;
			outlet_length=union_length-neighbor_length;
			outlet_length_ratio=(16*bunch_int_length)/(bunch_int_length+union_length);
			outlet_length_ratio1=(16*outlet_length)/(outlet_length+union_length);
			outlet_length_ratio2=(16*neighbor_length)/(neighbor_length+union_length);
		    outlet_length_ratio3=(16*union_length)/(neighbor_length+union_length);

		Intersection=Intersection_measure_ratios(neighbor_beg_int,neighbor_end_int,beg_int,
			end_int,&indic_length,&ratio_length,&ratio_length1);
		/*if((Intersection==3)&&(density_ratio<=5))
		{
			goto L;
		}*/
		test_junction=Test_Junction_Corrected(right_neighbor,coun_bun,bunch_occurrence1,
		Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
		test_jun=test_junction%10;
		     if((test_jun>=1)&&(test_jun<=3))
		      {//last_cor16.09.14
		if(test_jun==1)
		{//last_cor09.09.14
			if(bunch_int_length<StripWidth)
			{//last_cor09.04.15
				if((left_neighbor_end<beg_int)&&(left_neighbor_end>=0))
				{
					block_index=right_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					 Merging(right_neighbor,coun_bun);
					 goto L;
				}
			}
			else
			{//last_cor09.04.15
				if((hue_close==1)&&(saturation_close==1)&&(gray_close==1))
				{
				if((outlet_length_ratio1<4)&&(density_ratio<7))
				{
					block_index=right_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					 Merging(right_neighbor,coun_bun);
					 goto L;
				}
				}
			}//last_cor09.04.15
		}//last_cor09.09.14
		if(beg_int<=StripWidth/2)
		{//last_cor12.09.14
			if((outlet_length_ratio<=2)&&(outlet_length_ratio2>=6)&&(bunch_int_length<2*StripWidth))
			{
				if((test_jun>=1)&&(test_jun<=3))
				{
					block_index=right_neighbor+1;
					*(bunch_blocking+coun_bun)=block_index;
					 Merging(right_neighbor,coun_bun);
					 goto L;
				}
			}
		}
		if(((Intersection>=0)&&(Intersection<=2))||((Intersection==3)&&(ratio_length<=2)))
		{//intcond
		if(!outlet_length_ratio2)
		{//io0

		if(((test_jun>=1)&&(test_jun<=2))||((test_jun==3)&&(ratio_length==15)))
				{
				block_index=coun_bun+1;
								 *(bunch_blocking+right_neighbor)=block_index;
								 SimpleMerging(coun_bun,right_neighbor);
								 goto L;
				}
		}//io0
		else
		{//eol0
		if((outlet_length_ratio2<=2)&&(density_ratio>=8)&&(ratio_length>=8))
		{//ol2last_cor25.03.14//error!22.04.14error09.06.14


				if((test_jun>=1)&&(test_jun<=2))
				{
				block_index=coun_bun+1;
								 *(bunch_blocking+right_neighbor)=block_index;
								 SimpleMerging(coun_bun,right_neighbor);
								 goto L;
				}

		}//ol2
		else
		{
			if((outlet_length_ratio<=2)&&(density_ratio<=8)&&(ratio_length1>=8))
		    {//ol2inlast_cor25.03.14//error!22.04.14error09.06.14


				if((test_jun>=1)&&(test_jun<=2))
				{
				                 block_index=right_neighbor+1;
								 *(bunch_blocking+coun_bun)=block_index;
								 SimpleMerging(right_neighbor,coun_bun);
								 goto L;
				}

		}//ol2in
		}
		}//eol0
		}//intcond
		if(Intersection==3)
		{
		ratio_length=0;
		}
			//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
			//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
		if(Intersection<=1)
		{//short
			if((test_jun>0)&&(test_jun<=2))
							 {//test_jun1
			if(bunch_occurrence1>=12)
			{//bo1
				if((test_jun_left>=5)&&(test_jun_left<=6))
				{//tjl
					if((bunch_occurrence<=2)&&(abs(left_differ)<=3))
					{
		                 if(((outlet_length_ratio1<=2)&&(right_differ<4))||
		                  ((outlet_length_ratio1<=5)&&(right_differ<=1)))
		                  {
		                   block_index=right_neighbor+1;
				           *(bunch_blocking+coun_bun)=block_index;
				           SimpleMerging(right_neighbor,coun_bun);
		                  }
					}
				}//tjl
			}//bo1
			                }//test_jun1
		}//short

		if(((Intersection<=1)&&(indic_length>=2))||
		   ((Intersection==0)&&(indic_length>=1)&&(ratio_length>=12)&&(ratio_length1>=12)&&((ratio_length>=15)||(ratio_length1>=15))))
		   {//big intersection
		                     if((test_jun>0)&&(test_jun<=2))
							 {
		 if(((bunch_occurrence1>=bunch_occurrence))||
			 ((bunch_occurrence1>0)&&(indic_length>=2)&&((ratio_length1>=12)||
			 (outlet_length_ratio1<=2))
			 &&(density_ratio<=10)&&(density_ratio>=6)))
								 {
								 block_index=right_neighbor+1;
								 *(bunch_blocking+coun_bun)=block_index;
								 Merging(right_neighbor,coun_bun);
								 goto L;
								 }
							 }
							 else
							 {//not so close

							  if((((test_junction>0)&&(test_junction<=4))||((test_junction>10)&&(test_junction<15)))
		   &&((ratio_length1>=15)||((ratio_length<=1)&&(ratio_length1>=11))||
		   (((ratio_length>=15)&&(ratio_length1>=12))||((ratio_length>=12)&&(ratio_length1>=15)))))
								 {//small embedded
									 if((bunch_occurrence1>bunch_occurrence)&&(bunch_occurrence<4))
									 {
								 block_index=right_neighbor+1;
								 *(bunch_blocking+coun_bun)=block_index;
								 Merging(right_neighbor,coun_bun);
								 goto L;
									 }
								 }
							  else
		                         {//not too big intersection but small outlet and very close colors
		if(((test_jun>0)&&(test_jun<=2))&&((outlet_length_ratio<=6)
		   ||(outlet_length_ratio1<=2))&&(indic_length>=2))
		  {
		  block_index=right_neighbor+1;
		 *(bunch_blocking+coun_bun)=block_index;
		 Merging(right_neighbor,coun_bun);
		 goto L;
		  }
		                          }//not too big intersection but small outlet and very close colors
							 }//not so close
		    }//big intersection
		else
		   {//small intersection
		if((Intersection==2)||((Intersection==3)&&(outlet_length_ratio<=2)&&(right_differ<4))||
		   ((Intersection==3)&&(outlet_length_ratio<=5)&&(right_differ<=1)))
		{//last_cor15.10.13
		if(test_junction==1)
		{//tj1
		if((density_ratio>=6)&&(density_ratio<=10)&&(bunch_occurrence1>=14)&&
		   (bunch_occurrence1>2*bunch_occurrence))
					{//last_cor31.10.13
				block_index=right_neighbor+1;
				*(bunch_blocking+coun_bun)=block_index;
				Merging(right_neighbor,coun_bun);
		goto L;
					}
		}//tj1

			if(test_jun==1)
				{//tj01
		if((bunch_occurrence1>=8)&&(bunch_occurrence>=5)&&(neighbor_saturation>=2)&&(bunch_saturation>=2))
					{
					if((density_ratio>=4)&&(density_ratio<=11)&&(neighbor_bunch_density>(StripWidth+StripWidth/4)))
					{
				block_index=right_neighbor+1;
				*(bunch_blocking+coun_bun)=block_index;
				Merging(right_neighbor,coun_bun);
		goto L;
					}
					}
			   }//tj01

			if(((outlet_length_ratio<=6)||(outlet_length_ratio1<=2))&&(indic_length>=2))
			{//out
				 if((test_jun>0)&&(test_jun<4))
			    {
			if(!outlet_length_ratio)
			{
				block_index=right_neighbor+1;
				*(bunch_blocking+coun_bun)=block_index;
				Merging(right_neighbor,coun_bun);
				goto L;
			}
				 }
		if((neighbor_saturation>=2)&&(bunch_saturation>=2)&&(density_ratio>=7)&&(density_ratio<=9)&&
					(neighbor_bunch_density>(StripWidth+StripWidth/2)))
				   {
				if((test_jun>0)&&(test_jun<=1))
				{
				block_index=right_neighbor+1;
				*(bunch_blocking+coun_bun)=block_index;
				Merging(right_neighbor,coun_bun);
				goto L;
				}
				   }
			}//out

		}//last_cor15.10.13
		   }//small intersection
							//}//bo cond for rn
							}//tj13last_cor16.09.14
							    }//rn is new and ln failed
					}//right neighbor exists
				 }//left neighbor has not been found
			  }//block and bo rather small
		L:
			  ;
			}//loop
			/*bunches_occurred=new int[Max_number*PressedLength];

			occurred_dimension=Max_number*PressedLength;
		memset(bunches_occurred,(int) '\0',sizeof(int)*(occurred_dimension));*/
		   //memset(painted_strip_colored,(int) '\0',sizeof(int)*(PressedLength));
		//memset(painted_numbers_colored,(int) '\0',sizeof(int)*(PressedLength));
		memset(encountered_bunches,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));
		//memset(visible_bunches,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));

		Cleaning(0);
		      test_jun=-1;
		      for(count_bunch=0;count_bunch<NumberOfColoredIntervals;count_bunch++)
		      	      {//continuation and contrast
		      			  test_bunch=*(bunch_blocking+count_bunch);
		      			  small_hole=0;
		      			  right_small_hole=0;
		      			  left_bunch_domin=-1;
		      			  right_bunch_domin=-1;
		      		 if (!test_bunch)
		      		 {//not blocked
		      			  bunch_occurrence=*(visible_bunches+count_bunch);
		      			  beg_int=ColoredIntervalsStructure->BegInterv[count_bunch];
		                    end_int=ColoredIntervalsStructure->EndInterv[count_bunch];
		      			  bunch_significance=ColoredIntervalsStructure->Significance[count_bunch];
		                    bunch_density=(2*bunch_significance)/(end_int-beg_int+1);

		      			  point_beg=beg_int/4;
		      	          point_end=end_int/4;


		      	  	     if(point_beg>=1)
		      		     {
		      		     left_neighbor=*(painted_strip_colored+point_beg-1);
		      			 left_bunch_domin=*(painted_strip_colored+point_beg);
		      			 left_domin=left_bunch_domin;
		      			 }
		      		     else
		      		     {
		      	         left_neighbor=*(painted_strip_colored+point_beg);
		      			 left_bunch_domin=0;//left boundary of the array of visible bunches
		      			 left_domin=-1;
		      		     }
		      		                         if(point_end<StripLength-1/4)
		      	                             {
		      	                             right_neighbor=*(painted_strip_colored+point_end+1);
		      								 right_bunch_domin=*(painted_strip_colored+point_end);
		      								 right_domin=right_bunch_domin;
		      								 }
		      	                             else
		      	                             {
		                                       right_neighbor=*(painted_strip_colored+point_end);
		      								 right_bunch_domin=0;//right boundary of the array of visible bunches
		      								 right_domin=-1;

		      	                             }
		      first_left_occurred=-1;
		      left_gap_length=-1;
		      if(left_neighbor==0)
		      	  {//left hole processing
		      		  if(left_bunch_domin>0)
		      		  {
		      			  paint_finish=0;
		      			  paint_start=point_beg-1;
		      			  first_left_occurred=Left_hole_finding(paint_start,paint_finish,&left_gap_length);
		      			  if((left_gap_length<=3)&&(first_left_occurred>=0))
		      			  {
		      				  left_neighbor=first_left_occurred;
		      			  }
		      		  }
		      //Bunch_continuation(-1,count_bunch,bunch_occurrence,beg_int,end_int,paint_start,paint_finish,left_continuation);
		      	  }//right hole processing

		      			 left_bunch_domin=left_bunch_domin-1;
		      			 left_neighbor=left_neighbor-1;
		      			 if((left_neighbor>=0)&&(left_neighbor!=count_bunch))
		      			 {//ln
		      neighbor_beg_int=ColoredIntervalsStructure->BegInterv[left_neighbor];
		      neighbor_end_int=ColoredIntervalsStructure->EndInterv[left_neighbor];
		      neighbor_point_beg=neighbor_beg_int/4;
		      neighbor_point_end=neighbor_end_int/4;
		      bunch_new_significance=ColoredIntervalsStructure->Significance[left_neighbor];
		      neighbor_bunch_density=
		      (2*bunch_new_significance)/(neighbor_end_int-neighbor_beg_int+1);
		      density_ratio=(16*bunch_density)/(neighbor_bunch_density+bunch_density);
		      if(neighbor_end_int<beg_int)
		            {//there is a small hole
		      		  small_hole=beg_int-neighbor_end_int;
		      		  //change left_neighbor if the hole is large enough!!!
		      		  if(small_hole>=4)
		      		  {
		      			  new_left_domin_point=neighbor_end_int+(beg_int-neighbor_end_int)/2;
		      			  new_left_domin=*(painted_strip_colored_long+new_left_domin_point);
		      			  if(new_left_domin>0)
		      			  {
		      				  new_left_domin-=1;
		      			  }
		      		  }

		      	  }//there is a small hole
		          if(left_bunch_domin<0)
		      	{//left bunch dominator
		      		if(neighbor_point_end<point_end)
		      		{
		      left_bunch_domin=*(painted_strip_colored+neighbor_point_end+1);
		      		}
		      	}//left bunch dominator
		      	else
		      	{//elb
		      		if(small_hole>0)
		      		  {//smh
		      			  if((left_bunch_domin!=left_neighbor)&&(left_bunch_domin!=count_bunch)&&
		      				  (left_bunch_domin>=0))
		      			  {
		                    left_neighbor=left_bunch_domin;
		      neighbor_beg_int=ColoredIntervalsStructure->BegInterv[left_neighbor];
		      neighbor_end_int=ColoredIntervalsStructure->EndInterv[left_neighbor];
		      neighbor_point_beg=neighbor_beg_int/4;
		      neighbor_point_end=neighbor_end_int/4;
		      bunch_new_significance=ColoredIntervalsStructure->Significance[left_neighbor];
		      neighbor_bunch_density=
		      (2*bunch_new_significance)/(neighbor_end_int-neighbor_beg_int+1);
		      density_ratio=(16*bunch_density)/(neighbor_bunch_density+bunch_density);
		      			  }
		      		  }//smh
		      	}//elb


		      	         if((left_neighbor==left_bunch_domin)||(left_bunch_domin==count_bunch))
		      	          {//one of the two bunches is visible at the left end
		      if(neighbor_point_end<point_end)
		      {
		      	neighbor_end_dominant=*(painted_strip_colored+neighbor_point_end+1);
		      }
		      else
		      {
		      neighbor_end_dominant=0;
		      }
		      neighbor_end_dominant=neighbor_end_dominant-1;

		      			 if((left_bunch_domin==count_bunch)||(neighbor_end_dominant==count_bunch)||
		      				 ((density_ratio>=6)&&(bunch_occurrence>0)&&(neighbor_end_dominant>0)))
		      		 {//intersection and alternating dominance
		      Intersection=Intersection_measure_ratios(neighbor_beg_int,neighbor_end_int,beg_int,
		      	end_int,&indic_length,&ratio_length,&ratio_length1);
		      if(Intersection==3)
		      {
		      	ratio_length=0;
		      }
		      	//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
		      	//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
		      test_junction=Test_Junction_Corrected(count_bunch,left_neighbor,bunch_occurrence,
		      Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
		        if((density_ratio>=3)&&(density_ratio<=12))
		        {//density restriction
		               if(((test_junction%10)>0)&&((test_junction%10)<=4))
		                {
		      			 *(left_continuation+count_bunch)=test_junction+1;
		      			 *(left_adjacent+count_bunch)=left_neighbor+1;
		                }
		      		 else
		      		 {
		      				 if((test_junction%10)==8)
		      				 {
		                       *(left_adjacent+count_bunch)=left_neighbor+1;
		      				 }
		      		 }
		        }//density restriction
		        else
		        {
		      	  //if((density_ratio>11)||(density_ratio<4))
		      	  //{
		      		  if(((test_junction%10)>=1)&&((test_junction%10)<=2))
		      		  {//last_cor
		      			  *(left_continuation+count_bunch)=test_junction+1;
		      			 *(left_adjacent+count_bunch)=left_neighbor+1;
		      		  }

		      	  //}
		        }
		        if(((test_junction%10)>4)&&((test_junction%10)<=7))
		        {//last_cor12.06.13

		      *(left_continuation+count_bunch)=-(test_junction+1);
		      *(left_adjacent+count_bunch)=-(left_neighbor+1);
		        }
		       /* else
		        {
		      	   if(((test_junction%10)>=4)&&(small_hole>=3)&&(density_ratio>=10))
		      	   {
		      		   *(left_continuation+count_bunch)=-38;
		      *(left_adjacent+count_bunch)=-(left_neighbor+1);
		      	   }
		        }*/
		      		 }//intersection and alternating dominance
		      	          }//one of the two bunches is visible at the left end

		      			 }//ln
		      			 else
		      			 {
		      	  if(left_neighbor==-1)
		      	  {//hole processing
		      			  paint_finish=beg_int/4;
		      			  paint_start=std::max((paint_finish-2),0);
		      //Bunch_continuation(-1,count_bunch,bunch_occurrence,beg_int,end_int,paint_start,paint_finish,left_continuation);
		      	  }
		      			 }

		      //right cjntinuation
		      first_right_occurred=-1;
		      right_gap_length=-1;
		      if(right_neighbor==0)
		      	  {//right hole processing
		      		  if(right_bunch_domin>0)
		      		  {
		      			  paint_finish=(StripLength-1)/4;
		      			  paint_start=point_end+1;
		      			  first_right_occurred=Right_hole_finding(paint_start,paint_finish,&right_gap_length);
		      			  if((right_gap_length<=3)&&(first_right_occurred>=0))
		      			  {
		      				  right_neighbor=first_right_occurred;
		      			  }
		      		  }
		      //Bunch_continuation(-1,count_bunch,bunch_occurrence,beg_int,end_int,paint_start,paint_finish,left_continuation);
		      	  }//right hole processing
		      			 right_bunch_domin=right_bunch_domin-1;
		      			 right_neighbor=right_neighbor-1;
		      			 if((right_neighbor>=0)&&(right_neighbor!=count_bunch))
		      			 {//rln
		      right_neighbor_beg_int=ColoredIntervalsStructure->BegInterv[right_neighbor];
		      right_neighbor_end_int=ColoredIntervalsStructure->EndInterv[right_neighbor];
		      right_neighbor_point_beg=right_neighbor_beg_int/4;
		      right_neighbor_point_end=right_neighbor_end_int/4;
		      right_bunch_new_significance=ColoredIntervalsStructure->Significance[right_neighbor];
		      right_neighbor_bunch_density=
		      (2*right_bunch_new_significance)/(right_neighbor_end_int-right_neighbor_beg_int+1);
		      density_ratio=(16*bunch_density)/(right_neighbor_bunch_density+bunch_density);
		      if(right_neighbor_beg_int>end_int)
		            {//there is a small hole
		      		  right_small_hole=right_neighbor_beg_int-end_int;
		      	  }//there is a small hole
		          if(right_bunch_domin<0)
		      	{//right bunch dominator
		      		if(right_neighbor_point_beg>point_beg)
		      		{
		      right_bunch_domin=*(painted_strip_colored+right_neighbor_point_beg-1);
		      		}
		      	}//right bunch dominator
		      	else
		      	{//erb
		      if(right_small_hole>0)
		      		  {//smh
		      			  if((right_bunch_domin!=right_neighbor)&&(right_bunch_domin!=count_bunch)&&
		      				  (right_bunch_domin>=0))
		      			  {
		                    right_neighbor=right_bunch_domin;
		      right_neighbor_beg_int=ColoredIntervalsStructure->BegInterv[right_neighbor];
		      right_neighbor_end_int=ColoredIntervalsStructure->EndInterv[right_neighbor];
		      right_neighbor_point_beg=right_neighbor_beg_int/4;
		      right_neighbor_point_end=right_neighbor_end_int/4;
		      right_bunch_new_significance=ColoredIntervalsStructure->Significance[right_neighbor];
		      right_neighbor_bunch_density=
		      (2*right_bunch_new_significance)/(right_neighbor_end_int-right_neighbor_beg_int+1);
		      density_ratio=(16*bunch_density)/(right_neighbor_bunch_density+bunch_density);
		      			  }
		      		  }//smh

		      	}//erb

		      	         if((right_neighbor==right_bunch_domin)||(right_bunch_domin==count_bunch))
		      	          {//one of the two bunches is visible at the right end
		      if(right_neighbor_point_beg>point_beg)
		      {
		      	right_neighbor_beg_dominant=*(painted_strip_colored+right_neighbor_point_beg-1);
		      }
		      else
		      {
		      right_neighbor_beg_dominant=0;
		      }
		      right_neighbor_beg_dominant=right_neighbor_beg_dominant-1;

		      			 if((right_bunch_domin==count_bunch)||(right_neighbor_beg_dominant==count_bunch)||
		      				 ((density_ratio>=6)&&(bunch_occurrence>0)&&(right_neighbor_beg_dominant>=0)))
		      		 {//intersection and alternating dominance
		      Intersection=Intersection_measure_ratios(right_neighbor_beg_int,right_neighbor_end_int,beg_int,
		      	end_int,&indic_length,&ratio_length,&ratio_length1);
		      if(Intersection==3)
		      {
		      ratio_length=0;
		      }
		      	//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
		      	//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
		      test_junction=Test_Junction_Corrected(count_bunch,right_neighbor,bunch_occurrence,
		      Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
		        if((density_ratio>=3)&&(density_ratio<=12))
		        {//density restriction
		      if(((test_junction%10)>0)&&((test_junction%10)<=4))
		       {//04
		        *(right_continuation+count_bunch)=test_junction+1;
		        *(right_adjacent+count_bunch)=right_neighbor+1;
		      }//04
		      else
		      		 {

		      				 if((test_junction%10)==8)
		      				 {
		                       *(right_adjacent+count_bunch)=right_neighbor+1;
		      				 }

		      		 }

		        }//density restriction
		        else
		        {//important error!16.09.13
		      		  if(((test_junction%10)>=1)&&((test_junction%10)<=2))
		      		  {//last_cor
		      			  *(right_continuation+count_bunch)=test_junction+1;
		      			 *(right_adjacent+count_bunch)=right_neighbor+1;//!!!!last_cor04.02.14
		      		  }
		        }
		        if(((test_junction%10)>4)&&((test_junction%10)<=7))
		        {
		      *(right_continuation+count_bunch)=-(test_junction+1);
		      *(right_adjacent+count_bunch)=-(right_neighbor+1);
		       }
		       /* else
		        {
		      	   if(((test_junction%10)>=4)&&(right_small_hole>=3)&&(density_ratio>=10))
		      	   {
		      		   *(right_continuation+count_bunch)=-38;
		      *(right_adjacent+count_bunch)=-(right_neighbor+1);
		      	   }
		        }*/
		      		 }//intersection and alternating dominance
		      	          }//one of the two bunches is visible at the right end

		      			 }//rln

		      			 if((right_neighbor>=0)&&(left_neighbor>=0))
		      	       {//between close bunches
		      if((left_domin>0)&&(right_domin>0))
		      {
		      	difference_neighbors=right_neighbor_point_beg-neighbor_point_end;
		               if(difference_neighbors<=1)
		      		 {
		      			 gap_between_neighbors=right_neighbor_beg_int-neighbor_end_int;
		      		 }
		      }
		      			 }
		      		 }//not blocked
		                }//continuation and contrast

		      for(count_improve=0;count_improve<7;count_improve++)
		      {//merging cycleslast_cor08.04.15
		      Number_of_Merging=0;
		      		 for(count_bunch3=0;count_bunch3<NumberOfColoredIntervals;count_bunch3++)
		      	      {//prolongation small contrasts and very close contrasts
		      			  test_bunch=*(bunch_blocking+count_bunch3);
		      			  bunch_occurrence=*(visible_bunches+count_bunch3);
		      		 if ((!test_bunch)&&(bunch_occurrence>0))
		      		 {//not blocked
		       left_neighbor_hue=-1;
		      left_neighbor_gray=-1;
		      left_neighbor_sat=-1;
		      left_hue_zone=-1;
		      bunch_gray=-1;
		      bunch_sat=-1;
		      beg_int=ColoredIntervalsStructure->BegInterv[count_bunch3];
		      end_int=ColoredIntervalsStructure->EndInterv[count_bunch3];
		      left_bunch_contrast=left_continuation[count_bunch3];
		      left_bunch_contrast=left_bunch_contrast%10;
		      bunch_left_adjacent=left_adjacent[count_bunch3];
		      if((bunch_left_adjacent<0)&&(left_bunch_contrast>0))
		      {//error23.03.18
		    	  bunch_left_adjacent=-bunch_left_adjacent;
		    	  left_adjacent[count_bunch3]=bunch_left_adjacent;
		      }//error23.03.18
		      right_bunch_contrast=right_continuation[count_bunch3];
		      right_bunch_contrast=right_bunch_contrast%10;
		      bunch_right_adjacent=right_adjacent[count_bunch3];
		      if(((left_bunch_contrast<0)&&(right_bunch_contrast<0))||
		         ((!left_bunch_contrast)&&(right_bunch_contrast<0))||
		         ((left_bunch_contrast<0)&&(!right_bunch_contrast))||
		         ((!left_bunch_contrast)&&(!right_bunch_contrast)))
		      {//last_cor24.04.15
		      	goto K;
		      }
		      if(left_bunch_contrast>0)
		      {//last_cor22.04.15
		      left_bunch_contrast-=1;
		      bunch_left_adjacent-=1;
		      }
		      else
		      {
		      	if(left_bunch_contrast<0)
		      	{
		      	left_bunch_contrast+=1;
		      	bunch_left_adjacent=-bunch_left_adjacent-1;
		          }
		      }//last_cor22.04.15
		      if(right_bunch_contrast>0)
		      	{//rb>0
		      		right_bunch_contrast-=1;
		      		bunch_right_adjacent-=1;
		          }
		      else
		      {//last_cor22.04.15
		      if(right_bunch_contrast<0)
		      	{
		      	right_bunch_contrast+=1;
		      	bunch_right_adjacent=-bunch_right_adjacent-1;
		      }
		      }//last_cor22.04.15
		      block_index=0;
		      	/*beg_int=ColoredIntervalsStructure->BegInterv[count_bunch3];
		      	end_int=ColoredIntervalsStructure->EndInterv[count_bunch3];*/
		      	bunch_significance=ColoredIntervalsStructure->Significance[count_bunch3];
		      	bunch_int_length=end_int-beg_int+1;
		      	bunch_density=(2*bunch_significance)/bunch_int_length;
		      	bunch_gray=ColoredIntervalsStructure->AverageGray[count_bunch3];
		         bunch_sat=ColoredIntervalsStructure->AverageSat[count_bunch3];
		         bunch_hue=ColoredIntervalsStructure->AverageHue[count_bunch3];//13.01.16
		         bunch_hue_zone=hue_zones[bunch_hue];//13.01.16

		      if(left_bunch_contrast<0)
		      {//lc<0

		      	if(right_bunch_contrast>0)
		      	{//rb>0
		      	    neighbor_beg_int=ColoredIntervalsStructure->BegInterv[bunch_right_adjacent];
		      		neighbor_end_int=ColoredIntervalsStructure->EndInterv[bunch_right_adjacent];
		      		neighbor_length=neighbor_end_int-neighbor_beg_int+1;
		          union_beg=std::min(beg_int,neighbor_beg_int);
		      	union_end=std::max(end_int,neighbor_end_int);
		      	union_length=union_end-union_beg+1;
		      	outlet_length=union_length-neighbor_length;
		          outlet_length1=union_length-bunch_int_length;
		      	outlet_length_ratio=(16*bunch_int_length)/(bunch_int_length+union_length);
		      	outlet_length_ratio1=(16*outlet_length)/(outlet_length+union_length);
		          outlet_length_ratio2=(16*neighbor_length)/(neighbor_length+union_length);
		          outlet_length_ratio3=(16*neighbor_length)/(neighbor_length+union_length);
		      	neighbor_bunch_significance=ColoredIntervalsStructure->Significance[bunch_right_adjacent];
		      neighbor_bunch_density=(2*neighbor_bunch_significance)/neighbor_length;
		      density_sum=neighbor_bunch_density+bunch_density;
		      density_ratio=(16*bunch_density)/(bunch_density+neighbor_bunch_density);
		      Intersection=Intersection_measure_ratios(neighbor_beg_int,neighbor_end_int,beg_int,
		      end_int,&indic_length,&ratio_length,&ratio_length1);
		      test_junction=Test_Junction_Corrected(count_bunch3,bunch_right_adjacent,bunch_occurrence,
		      Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
		      test_junction=test_junction%10;
		      test_junction=std::min(test_junction,right_bunch_contrast);
		      //!!!!last_cor04.02.14.
		      	  if((test_junction>=1)&&(test_junction<=2))
		      	  {//12
		      if((test_junction==1)||((density_ratio>=6)&&(density_ratio<=10))||(bunch_sat<=2))
		      {//rbc1
		                        if(((hue_close>=1)&&(hue_close<=2))||(saturation_close==1)
		      					  ||(gray_close==1))
		      				  {//last_cor23.12.12cond
		      sign_intersect=0;
		      if(Intersection==3)
		                   {//int3
		      				 if(bunch_hue_zone==3)
		      				 {//last_cor15.01.15
		      					 if(bunch_sat>=2)
		      					 {//bs2
		      				         if(ratio_length>6)
		      				          {//last_cor14.01.16
		      					      sign_intersect=1;
		      				          }
		      				     }//bs2
		      					 else
		      					 {//ebs2
		      						 if(ratio_length>4)
		      				         {//last_cor15.01.16
		      					     sign_intersect=1;
		      						 }//last_cor15.01.16

		      				     }//ebs2
		      				 }//last_cor15.01.15
		      				 else
		      				 {
		      					     if(ratio_length>4)
		      				         {//last_cor15.01.16
		      					     sign_intersect=1;
		      						 }//last_cor15.01.16
		      				 }//last_cor15.01.16

		      	         }//int3
		      if(!sign_intersect)
		      {//si
		      	if((bunch_hue_zone==3)&&(bunch_sat>=2))
		         {//13.01.16
		      if(bunch_significance>=neighbor_bunch_significance)
		      			{
		      		    block_index=count_bunch3+1;
		                 *(bunch_blocking+bunch_right_adjacent)=block_index;
		      		   Merging(count_bunch3,bunch_right_adjacent);
		      		   Number_of_Merging++;
		      		   new_right_adj=right_adjacent[bunch_right_adjacent];
		      		   new_right_contr=right_continuation[bunch_right_adjacent];
		      if(new_right_contr>0)
		      	{//rb>0
		      		new_right_contr-=1;
		      		new_right_adj-=1;
		      if(new_right_adj>=0)
		      {
		       if(left_continuation[new_right_adj]>0)
		       {
		      left_adjacent[new_right_adj]=left_adjacent[bunch_right_adjacent];
		       }
		       else
		       {
		      if(left_continuation[new_right_adj]<0)
		       {
		      left_adjacent[new_right_adj]=-left_adjacent[bunch_right_adjacent];
		       }
		       }
		      }
		          }
		      else
		      {//last_cor22.04.15
		      if(new_right_contr<0)
		      	{
		      	new_right_contr+=1;
		      	new_right_adj=-new_right_adj-1;
		      	   if(new_right_adj>=0)
		             {
		             //left_continuation[new_right_adj]=left_continuation[bunch_right_adjacent];
		      		   if(left_continuation[new_right_adj]>0)
		      		   {
		             left_adjacent[new_right_adj]=block_index;
		      		   }
		      		   else
		      		   {
		      			   if(left_continuation[new_right_adj]<0)
		      			   {
		      left_adjacent[new_right_adj]=-block_index;
		      			   }
		      		   }
		      	   }
		          }
		      }
		      /*else
		      {
		      	if(left_continuation[new_right_adj]>0)
		      	{
		      		if()
		      left_adjacent[new_right_adj]=left_adjacent[bunch_right_adjacent];
		      	}
		      	else
		      	{
		      		if(left_continuation[new_right_adj]<0)
		      	   {
		      left_adjacent[new_right_adj]=-left_adjacent[bunch_right_adjacent];
		      	   }
		      	}
		      }*/

		      right_adjacent[count_bunch3]=right_adjacent[bunch_right_adjacent];
		      right_continuation[count_bunch3]=right_continuation[bunch_right_adjacent];
		      goto K;
		      			}
		      			else
		      			{
		                 block_index=bunch_right_adjacent+1;
		                 *(bunch_blocking+count_bunch3)=block_index;
		      			Merging(bunch_right_adjacent,count_bunch3);
		      			Number_of_Merging++;
		      left_continuation[bunch_right_adjacent]=left_continuation[count_bunch3];
		      left_adjacent[bunch_right_adjacent]=left_adjacent[count_bunch3];
		      if(right_continuation[bunch_left_adjacent]<0)
		      {
		      right_adjacent[bunch_left_adjacent]=-(block_index);
		      }
		      else
		      {
		      if(right_continuation[bunch_left_adjacent]>0)
		      {
		      right_adjacent[bunch_left_adjacent]=block_index;
		      }
		      }
		      goto K;
		      			}
		         }//13.01.16
		              if((Intersection<=1)||(ratio_length>=6)||(ratio_length1>=6)||(outlet_length_ratio<=4)||
		      			(outlet_length_ratio1<=4)||((Intersection>=2)&&
		      			(right_bunch_contrast==1)&&(density_ratio>=7)&&(density_ratio<=9))||
		      			((outlet_length_ratio2<=4)&&(right_bunch_contrast==1))||
		      			((outlet_length_ratio3<=4)&&(right_bunch_contrast==1)))
		      		{//Int<=1
		      			    new_right_adj=right_adjacent[bunch_right_adjacent];
		      				new_right_contr=right_continuation[bunch_right_adjacent];

		      		//last_cor14.01.13
		      		if(neighbor_bunch_significance>bunch_significance)
		      		{//nb>b
		      			block_index=bunch_right_adjacent+1;
		              *(bunch_blocking+count_bunch3)=block_index;
		      			if(neighbor_bunch_significance>2*bunch_significance)
		      			{
		      		SimpleMerging(bunch_right_adjacent,count_bunch3);
		      		Number_of_Merging++;
		      			}
		      			else
		      			{
		      			Merging(bunch_right_adjacent,count_bunch3);
		      			Number_of_Merging++;
		      			}
		              left_adjacent[bunch_right_adjacent]=left_adjacent[count_bunch3];
		      		left_continuation[bunch_right_adjacent]=left_continuation[count_bunch3];
		      		if(right_continuation[bunch_left_adjacent]>=0)
		      		{
		              right_continuation[bunch_left_adjacent]=left_continuation[count_bunch3];
		      		}
		              right_adjacent[bunch_left_adjacent]=-block_index;

		      		}//nb>b
		      		else
		      		{//nb<=b
		      		block_index=count_bunch3+1;
		              *(bunch_blocking+bunch_right_adjacent)=block_index;
		      			if(bunch_significance>2*neighbor_bunch_significance)
		      			{
		      		SimpleMerging(count_bunch3,bunch_right_adjacent);
		      		Number_of_Merging++;
		      			}
		      			else
		      			{
		      			Merging(count_bunch3,bunch_right_adjacent);
		      			Number_of_Merging++;
		      			}
		      if(new_right_contr>0)
		      		{//nra>
		      			new_right_contr-=1;
		      			new_right_adj-=1;
		      			if(new_right_adj>=0)
		      			{
		              left_adjacent[new_right_adj]=count_bunch3+1;
		      		left_continuation[new_right_adj]=new_right_contr+1;
		      			}
		      		}//nra>
		      		else
		      		{//era
		      			if(new_right_contr<0)
		      		    {//nra<
		      				//new_right_contr+=1;
		      	new_right_adj=-new_right_adj-1;
		      	if(new_right_adj>=0)
		      	{
		      		left_adjacent[new_right_adj]=-count_bunch3-1;
		      		left_continuation[new_right_adj]=new_right_contr;
		      	}
		      			}//nra<
		      		}//era

		              right_adjacent[count_bunch3]=right_adjacent[bunch_right_adjacent];
		      		right_continuation[count_bunch3]=right_continuation[bunch_right_adjacent];
		      		}//nb<=b

		      		goto K;
		      		}//Int<=1
		      }//si
		      				  }//last_cor23.12.12cond
		      }//rbc1
		      	if((outlet_length_ratio<=2)||((bunch_int_length<StripWidth)&&((outlet_length_ratio<=6)
		      		||(outlet_length_ratio1<=6)))||
		      		(outlet_length_ratio1<=2))
		      	{//o
		      		sign_intersect=0;
		      		if(bunch_right_adjacent>=0)
		      		{
		      			if(Intersection==3)
		                   {
		      				 if(ratio_length>4)
		      				 {
		      					 sign_intersect=1;
		      				 }
		      	         }
		      			if(!sign_intersect)
		      			{
		      		block_index=bunch_right_adjacent+1;
		              *(bunch_blocking+count_bunch3)=block_index;
		      		SimpleMerging(bunch_right_adjacent,count_bunch3);
		      		Number_of_Merging++;
		      		if(right_continuation[bunch_left_adjacent]>=0)
		      		{
		      			right_continuation[bunch_left_adjacent]=left_continuation[count_bunch3];
		      		}
		      		right_adjacent[bunch_left_adjacent]=-block_index;
		      		left_adjacent[bunch_right_adjacent]=left_adjacent[count_bunch3];
		      		left_continuation[bunch_right_adjacent]=left_continuation[count_bunch3];
		      		goto K;
		      			}
		      		}
		      	}//o
		      	  }//12

		      	  }//rb>0
		      }//lc<0
		      else
		      {//elc last_cor24.12
		      	//if((!left_bunch_contrast)&&(beg_int<=4))
		      	if((!left_bunch_contrast)||(left_bunch_contrast>=1))
		      	{//lc0bi0last_cor17.04.15!!!
		      		block_index=0;
		      if(right_bunch_contrast>0)
		      	{//rb>0
		      	    neighbor_beg_int=ColoredIntervalsStructure->BegInterv[bunch_right_adjacent];
		      		neighbor_end_int=ColoredIntervalsStructure->EndInterv[bunch_right_adjacent];
		      		neighbor_length=neighbor_end_int-neighbor_beg_int+1;
		              union_beg=std::min(beg_int,neighbor_beg_int);
		      	union_end=std::max(end_int,neighbor_end_int);
		      	union_length=union_end-union_beg+1;
		      	outlet_length=union_length-neighbor_length;
		      	outlet_length1=union_length-bunch_int_length;
		      	outlet_length_ratio=(16*bunch_int_length)/(bunch_int_length+union_length);
		      	outlet_length_ratio1=(16*outlet_length)/(outlet_length+union_length);
		          outlet_length_ratio2=(16*neighbor_length)/(neighbor_length+union_length);
		          outlet_length_ratio3=(16*neighbor_length)/(neighbor_length+union_length);
		      	neighbor_bunch_significance=ColoredIntervalsStructure->Significance[bunch_right_adjacent];
		      neighbor_bunch_density=(2*neighbor_bunch_significance)/neighbor_length;
		      density_sum=neighbor_bunch_density+bunch_density;
		      density_ratio=(16*bunch_density)/(bunch_density+neighbor_bunch_density);//error!!!11.03.15
		      Intersection=Intersection_measure_ratios(neighbor_beg_int,neighbor_end_int,beg_int,
		      end_int,&indic_length,&ratio_length,&ratio_length1);
		      test_junction=Test_Junction_Corrected(count_bunch3,bunch_right_adjacent,bunch_occurrence,
		      Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
		      test_junction=test_junction%10;
		      if((test_junction>=1)&&(test_junction<=2))
		      {//last_cor08.04.15
		      right_bunch_contrast=std::min(test_junction,right_bunch_contrast);//error11.03.15
		      }
		      else
		      {
		      	right_bunch_contrast=test_junction;
		      }

		      	  if((right_bunch_contrast>=1)&&(right_bunch_contrast<=2))
		      	  {//12
		      if((right_bunch_contrast==1)||((density_ratio>=6)&&(density_ratio<=10)))
		      {//rc1last_cor20.04.15&&||24.04.15
		      if(((hue_close>=1)&&(hue_close<=2))||(saturation_close==1)
		      					  ||(gray_close==1))
		      				  {//last_cor18.01.13cond

		      sign_intersect=0;
		      if(Intersection==3)
		                   {//int3
		      	 if(bunch_sat<=2)
		      	 {//bs<=2
		      		 if((gray_close>=1)&&(gray_close<=2)&&(hue_close>0))
		      		 {//gc1hc>=
		      if(ratio_length<=StripWidth)
		      {//rl<=
		      if((!left_bunch_contrast)&&(beg_int<=StripWidth))//18.01.15add the case of zero number!!!
		      {//!lc&bg<=
		      if(bunch_significance>=neighbor_bunch_significance)
		      			{
		      		    block_index=count_bunch3+1;
		                 *(bunch_blocking+bunch_right_adjacent)=block_index;
		      		   Merging(count_bunch3,bunch_right_adjacent);
		      		   Number_of_Merging++;
		      		   new_right_adj=right_adjacent[bunch_right_adjacent];
		      		   new_right_contr=right_continuation[bunch_right_adjacent];

		      if(new_right_contr>0)
		      	{//rb>0
		      		new_right_adj-=1;
		      		if(new_right_adj>=0)
		      		{
		      if(left_continuation[new_right_adj]>0)
		      			{
		      			left_adjacent[new_right_adj]=(count_bunch3+1);
		      			}
		      			else
		      			{
		      left_adjacent[new_right_adj]=-(count_bunch3+1);
		      			}
		      		}
		          }
		      else
		      {//last_cor22.04.15
		      if(new_right_contr<0)
		      	{
		      	new_right_adj=-new_right_adj-1;
		      	if(new_right_adj>=0)
		      		{
		      			if(left_continuation[new_right_adj]>0)
		      			{
		      			left_adjacent[new_right_adj]=(count_bunch3+1);
		      			}
		      			else
		      			{
		      left_adjacent[new_right_adj]=-(count_bunch3+1);
		      			}
		      	   }
		          }
		      }
		      right_adjacent[count_bunch3]=right_adjacent[bunch_right_adjacent];
		      right_continuation[count_bunch3]=right_continuation[bunch_right_adjacent];
		      goto K;
		      			}
		      			else
		      			{
		                 block_index=bunch_right_adjacent+1;
		                 *(bunch_blocking+count_bunch3)=block_index;
		      			Merging(bunch_right_adjacent,count_bunch3);
		      			Number_of_Merging++;
		      left_continuation[bunch_right_adjacent]=left_continuation[count_bunch3];
		      left_adjacent[bunch_right_adjacent]=left_adjacent[count_bunch3];
		      goto K;
		      			}
		      }//!lc&bg<=
		      }//rl<=
		      		 }//gc1hc
		      	 }//bs<=2
		      				 if(bunch_hue_zone==3)
		      				 {//last_cor15.01.15
		      					 if(bunch_sat>=2)
		      					 {//bs2
		      				         if(ratio_length>6)
		      				          {//last_cor14.01.16
		      					      sign_intersect=1;
		      				          }
		      				     }//bs2
		      					 else
		      					 {//ebs2
		      						 if(ratio_length>4)
		      				         {//last_cor15.01.16
		      					     sign_intersect=1;
		      						 }//last_cor15.01.16

		      				     }//ebs2
		      				 }//last_cor15.01.15
		      				 else
		      				 {
		      					     if(ratio_length>4)
		      				         {//last_cor15.01.16
		      					     sign_intersect=1;
		      						 }//last_cor15.01.16
		      				 }//last_cor15.01.16

		      	         }//int3
		      if(!sign_intersect)
		      {//si
		      	if((bunch_hue_zone==3)&&(bunch_sat>=2))
		         {//last_cor15.01.16
		      	   if((!left_bunch_contrast)&&(beg_int<=4))//18.01.15add the case of zero number!!!
		      	   {//!lc&bg<=
		      if(bunch_significance>=neighbor_bunch_significance)
		      			{
		      		    block_index=count_bunch3+1;
		                 *(bunch_blocking+bunch_right_adjacent)=block_index;
		      		   Merging(count_bunch3,bunch_right_adjacent);
		      		   Number_of_Merging++;
		      		   new_right_adj=right_adjacent[bunch_right_adjacent];
		      		   new_right_contr=right_continuation[bunch_right_adjacent];

		      if(new_right_contr>0)
		      	{//rb>0
		      		new_right_adj-=1;
		      		if(new_right_adj>=0)
		      		{
		      if(left_continuation[new_right_adj]>0)
		      			{
		      			left_adjacent[new_right_adj]=(count_bunch3+1);
		      			}
		      			else
		      			{
		      left_adjacent[new_right_adj]=-(count_bunch3+1);
		      			}
		      		}
		          }
		      else
		      {//last_cor22.04.15
		      if(new_right_contr<0)
		      	{
		      	new_right_adj=-new_right_adj-1;
		      	if(new_right_adj>=0)
		      		{
		      			if(left_continuation[new_right_adj]>0)
		      			{
		      			left_adjacent[new_right_adj]=(count_bunch3+1);
		      			}
		      			else
		      			{
		      left_adjacent[new_right_adj]=-(count_bunch3+1);
		      			}
		      	   }
		          }
		      }
		      /*if((new_right_adj>=0)&&(new_right_contr))
		       {
		      left_adjacent[new_right_adj]=left_adjacent[bunch_right_adjacent];
		      }*/
		      right_adjacent[count_bunch3]=right_adjacent[bunch_right_adjacent];
		      right_continuation[count_bunch3]=right_continuation[bunch_right_adjacent];
		      goto K;
		      			}
		      			else
		      			{
		                 block_index=bunch_right_adjacent+1;
		                 *(bunch_blocking+count_bunch3)=block_index;
		      			Merging(bunch_right_adjacent,count_bunch3);
		      			Number_of_Merging++;
		      left_continuation[bunch_right_adjacent]=left_continuation[count_bunch3];
		      left_adjacent[bunch_right_adjacent]=left_adjacent[count_bunch3];
		      goto K;
		      			}
		      	   }
		         }//last_cor15.01.16
		      if((Intersection<=1)||(ratio_length>=6)||(ratio_length1>=6)||(outlet_length_ratio<=4)||
		      			(outlet_length_ratio1<=4)||((Intersection>=2)&&
		      			(right_bunch_contrast==1)&&(density_ratio>=7)&&(density_ratio<=9))||
		      			((outlet_length_ratio2<=4)&&(right_bunch_contrast==1))||
		      			((outlet_length_ratio3<=4)&&(right_bunch_contrast==1)))

		      		{//Int<=1
		      			new_right_adj=right_adjacent[bunch_right_adjacent];
		      			new_right_contr=right_continuation[bunch_right_adjacent];
		      			//last_cor16.01.13
		      			if((neighbor_bunch_significance>bunch_significance)||((neighbor_beg_int<=beg_int)&&
		      						(neighbor_end_int>=end_int)))
		      					{//nb>b//last_cor07.03.18
		      						block_index=bunch_right_adjacent+1;
		      			        *(bunch_blocking+count_bunch3)=block_index;
		      						if(neighbor_bunch_significance>2*bunch_significance)
		      						{
		      					SimpleMerging(bunch_right_adjacent,count_bunch3);
		      					Number_of_Merging++;
		      						}
		      						else
		      						{
		      						Merging(bunch_right_adjacent,count_bunch3);
		      						Number_of_Merging++;
		      						}
		      						if((neighbor_beg_int<=beg_int)&&(neighbor_end_int>=end_int))
		      						{
		      							goto K;
		      						}
		      			        left_adjacent[bunch_right_adjacent]=left_adjacent[count_bunch3];
		      					left_continuation[bunch_right_adjacent]=left_continuation[count_bunch3];

		      					if(left_bunch_contrast>0)
		      					{//last_cor27.04.15
		      			        right_adjacent[bunch_left_adjacent]=bunch_right_adjacent+1;
		      					//correction of an important errpor!27.04.15
		      					right_continuation[bunch_left_adjacent]=left_continuation[count_bunch3];//l_c24.04.15
		      					}//last_cor16.04.15
		      					}//nb>b
		      		else
		      		{//nb<=b
		      		block_index=count_bunch3+1;
		              *(bunch_blocking+bunch_right_adjacent)=block_index;
		      			if(bunch_significance>2*neighbor_bunch_significance)
		      			{
		      		SimpleMerging(count_bunch3,bunch_right_adjacent);
		      		Number_of_Merging++;
		      			}
		      			else
		      			{
		      			Merging(count_bunch3,bunch_right_adjacent);
		      			Number_of_Merging++;
		      			}
		      			if((neighbor_beg_int>=beg_int)&&(neighbor_end_int<=end_int))
		      			{
		      				goto K;
		      			}
		      if(new_right_contr>0)
		      		{//nra>
		      			new_right_contr-=1;
		      			new_right_adj-=1;
		      			if(new_right_adj>=0)
		      			{
		              left_adjacent[new_right_adj]=count_bunch3+1;
		      		left_continuation[new_right_adj]=new_right_contr+1;
		      			}
		      		}//nra>
		      		else
		      		{//era
		      			if(new_right_contr<0)
		      		    {//nra<
		      				//new_right_contr+=1;
		      	new_right_adj=-new_right_adj-1;
		      	if(new_right_adj>=0)
		      	{
		      		left_adjacent[new_right_adj]=-count_bunch3-1;
		              left_continuation[new_right_adj]=new_right_contr;
		      	}
		      			}//nra<
		      			/*else
		      			{//last_cor24.04.15error!
		      //left_continuation[new_right_adj]=0;
		      			}*/
		      		}//era

		      right_adjacent[count_bunch3]=right_adjacent[bunch_right_adjacent];
		      right_continuation[count_bunch3]=right_continuation[bunch_right_adjacent];
		      		}//nb<=b

		      			goto K;
		      		}//Int<=1
		      }//si
		            }//cond
		      }//rc1
		           if(!block_index)
		      	 {//bo0
		      	/*if((outlet_length_ratio<=2)||((bunch_int_length<StripWidth)&&((outlet_length_ratio<=6)
		      		||(outlet_length_ratio1<=6)))||
		      		(outlet_length_ratio1<=2))*/
		      if((outlet_length_ratio<=2)||((bunch_int_length<StripWidth)&&((outlet_length_ratio<=6)
		      		||(outlet_length_ratio1<=6))))
		      	{//olast_cor24.04.15
		      		sign_intersect=0;


		      		if(bunch_right_adjacent>=0)
		      		{//bra>=0
		      			if(Intersection==3)
		                   {
		      				 if(ratio_length>4)
		      				 {
		      					 sign_intersect=1;
		      				 }
		      	         }
		      			if(!sign_intersect)
		      			{//si0
		      		block_index=bunch_right_adjacent+1;
		              *(bunch_blocking+count_bunch3)=block_index;
		      		SimpleMerging(bunch_right_adjacent,count_bunch3);
		      		Number_of_Merging++;
		      		//lascor
		      		if(!left_bunch_contrast)
		      		{//last_cor20.04.15
		      		left_adjacent[bunch_right_adjacent]=0;
		      		left_continuation[bunch_right_adjacent]=0;
		      		}
		      		else
		      		{
		      			if(left_bunch_contrast>0)
		      			{
		              left_adjacent[bunch_right_adjacent]=left_adjacent[count_bunch3];
		      		left_continuation[bunch_right_adjacent]=left_continuation[count_bunch3];
		      			}
		      		}//last_cor20.04.15

		      			}////si0
		      		}//bra>=0
		      	}//o
		      	 }//bo0
		      	  }//12
		      goto K;
		      	  }//rb>0

		      	}//lc0bi0

		      //}//elc
		      block_index=*(bunch_blocking+count_bunch3);
		      if(!block_index)
		      {//bi!0
		      	if(right_bunch_contrast<0)
		      	{//rb1<0
		      if(left_bunch_contrast>0)
		      {//lc1>0last_cor20.04.15
		      	    neighbor_beg_int=ColoredIntervalsStructure->BegInterv[bunch_left_adjacent];
		      		neighbor_end_int=ColoredIntervalsStructure->EndInterv[bunch_left_adjacent];
		      		neighbor_length=neighbor_end_int-neighbor_beg_int+1;
		          union_beg=std::min(beg_int,neighbor_beg_int);
		      	union_end=std::max(end_int,neighbor_end_int);
		      	union_length=union_end-union_beg+1;
		      	outlet_length=union_length-neighbor_length;
		      	outlet_length_ratio=(16*bunch_int_length)/(bunch_int_length+union_length);
		      	outlet_length_ratio1=(16*outlet_length)/(outlet_length+union_length);
		          outlet_length1=union_length-bunch_int_length;
		          outlet_length_ratio2=(16*neighbor_length)/(neighbor_length+union_length);
		          outlet_length_ratio3=(16*neighbor_length)/(neighbor_length+union_length);
		      	neighbor_bunch_significance=ColoredIntervalsStructure->Significance[bunch_left_adjacent];
		      neighbor_bunch_density=(2*neighbor_bunch_significance)/neighbor_length;
		      density_sum=neighbor_bunch_density+bunch_density;
		      density_ratio=(16*bunch_density)/(bunch_density+neighbor_bunch_density);
		      Intersection=Intersection_measure_ratios(neighbor_beg_int,neighbor_end_int,beg_int,
		      end_int,&indic_length,&ratio_length,&ratio_length1);
		      test_junction=Test_Junction_Corrected(count_bunch3,bunch_left_adjacent,bunch_occurrence,
		      Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
		      test_junction=test_junction%10;

		      if((left_bunch_contrast>=1)&&(left_bunch_contrast<=2))
		      	  {//121

		      if((left_bunch_contrast==1)||((density_ratio>=6)&&(density_ratio<=10)))
		      {//lbc1last_cor20.04.15&&||

		                        if(((hue_close>=1)&&(hue_close<=2))||(saturation_close==1)
		      					  ||(gray_close==1))
		      				  {//last_cor21.01.13cond
		      sign_intersect=0;
		      if(Intersection==3)
		                   {
		      				 if(ratio_length>4)
		      				 {
		      					 sign_intersect=1;
		      				 }
		      	         }
		      if(!sign_intersect)
		      {//si
		              if((Intersection<=1)||(ratio_length>=6)||(ratio_length1>=6)||(outlet_length_ratio<=4)||
		      			(outlet_length_ratio1<=4)||((Intersection>=2)&&
		      			(left_bunch_contrast==1)&&(density_ratio>=7)&&(density_ratio<=9))||
		      			((outlet_length_ratio2<=4)&&(left_bunch_contrast==1))||
		      			((outlet_length_ratio3<=4)&&(left_bunch_contrast==1)))
		      		{//Int<=1
		      			new_left_adj=left_adjacent[bunch_left_adjacent];
		      				new_left_contr=left_continuation[bunch_left_adjacent];
		      		//last_cor14.01.13
		      		if(neighbor_bunch_significance>bunch_significance)
		      		{//nb>b
		      			block_index=bunch_left_adjacent+1;
		              *(bunch_blocking+count_bunch3)=block_index;
		      			if(neighbor_bunch_significance>2*bunch_significance)
		      			{
		      		SimpleMerging(bunch_left_adjacent,count_bunch3);
		      		Number_of_Merging++;
		      			}
		      			else
		      			{
		      			Merging(bunch_left_adjacent,count_bunch3);
		      			Number_of_Merging++;
		      			}
		      			if(left_continuation[bunch_right_adjacent]>=0)
		      		{
		               left_continuation[bunch_right_adjacent]=right_continuation[count_bunch3];

		      		}
		              right_adjacent[bunch_left_adjacent]=right_adjacent[count_bunch3];
		      		right_continuation[bunch_left_adjacent]=right_continuation[count_bunch3];
		      		left_adjacent[bunch_right_adjacent]=-block_index;
		              }//nb>b
		      		else
		      		{//nb<=b
		      		block_index=count_bunch3+1;
		              *(bunch_blocking+bunch_left_adjacent)=block_index;
		      			if(bunch_significance>2*neighbor_bunch_significance)
		      			{
		      		SimpleMerging(count_bunch3,bunch_left_adjacent);
		      		Number_of_Merging++;
		      			}
		      			else
		      			{
		      			Merging(count_bunch3,bunch_left_adjacent);
		      			Number_of_Merging++;
		      			}
		      if(new_left_contr>0)
		      		{//nra>
		      			new_left_contr-=1;
		      			new_left_adj-=1;
		      			if(new_left_adj>=0)
		      			{
		              right_adjacent[new_left_adj]=count_bunch3+1;
		      		right_continuation[new_left_adj]=new_left_contr+1;
		      			}
		      		}//nra>
		      		else
		      		{//era
		      			if(new_left_contr<0)
		      		    {//nra<
		      				//new_left_contr+=1;
		      	new_left_adj=-new_left_adj-1;
		      	if(new_left_adj>=0)
		      	{
		      		right_adjacent[new_left_adj]=-count_bunch3-1;
		              right_continuation[new_left_adj]=new_left_contr;
		      	}
		      			}//nra<
		      		}//era
		      			 left_adjacent[count_bunch3]=left_adjacent[bunch_left_adjacent];
		      			 left_continuation[count_bunch3]=left_continuation[bunch_left_adjacent];
		      		}//nb<=b

		      		goto K;
		      		}//Int<=1
		      }//si
		      				  }//last_cor21.01.13cond
		      }//lbc1

		      	if((outlet_length_ratio<=2)||(outlet_length_ratio1<=2))
		      	{//o1
		      		sign_intersect=0;


		      		if(bunch_left_adjacent>=0)
		      		{
		      			if(Intersection==3)
		                   {
		      				 if(ratio_length>=4)
		      				 {
		      					 sign_intersect=1;
		      				 }
		      	         }
		      			if(!sign_intersect)
		      			{
		      		block_index=bunch_left_adjacent+1;
		              *(bunch_blocking+count_bunch3)=block_index;
		      		SimpleMerging(bunch_left_adjacent,count_bunch3);
		      		Number_of_Merging++;
		      		//lascor
		      		//left_adjacent[bunch_left_adjacent]=-left_adjacent[count_bunch3];
		      		if(left_continuation[bunch_right_adjacent]>=0)
		      		{
		               left_continuation[bunch_right_adjacent]=right_continuation[count_bunch3];
		      		}

		      		left_adjacent[bunch_right_adjacent]=-block_index;

		      		right_adjacent[bunch_left_adjacent]=right_adjacent[count_bunch3];
		      		right_continuation[bunch_left_adjacent]=right_continuation[count_bunch3];
		      			}
		      		}
		      	}//o1
		      	  }//121
		      }//lc1>0
		      	}//rb1<0
		      }//bi!0
		      }//elc
		      		 }//not blocked
		      K:
		      		 ;
		      		  }//prolongation small contrasts
		      		  if(!Number_of_Merging)
		      		  {
		      			 break;
		      		  }
		      	}//merging cycles


		///!!!!
			//memset(bunches_occurred,(int) '\0',sizeof(int)*(Max_number*PressedLength));
		Cleaning(1);
		orderning_result=CompleteOrdering(new_left_adjacent,new_left_continuation,
		new_right_adjacent,new_right_continuation,&last_bunch_num,&smallest_coordinate,
		&greatest_coordinate);
		linear_ordering_result=LinearOrdering(new_left_adjacent,new_left_continuation,
		new_right_adjacent,new_right_continuation,orderning_result,last_bunch_num);
		if(linear_ordering_result<0)
		{
			Disordering=1;
		}
		smallest_coordinate=-1;
		greatest_coordinate=-1;
		    /*if(FindLabels)
			{//l
				NumberOfLabels=0;


		first_lower_hue=color_lower_bound[ColorLabel1];
		first_upper_hue=color_upper_bound[ColorLabel1];
		first_mean_hue=color_mean[ColorLabel1];
		second_lower_hue=color_lower_bound[ColorLabel2];
		second_upper_hue=color_upper_bound[ColorLabel2];
		second_mean_hue=color_mean[ColorLabel2];
		third_lower_hue=color_lower_bound[ColorLabel3];
		third_upper_hue=color_upper_bound[ColorLabel3];
		third_mean_hue=color_mean[ColorLabel3];
		for(count_labels=0;count_labels<15;count_labels++)
		{//cycle labels
			Label_bunch_num[count_labels]=-1;
		}//cycle labels

		for(count_bu=0;count_bu<NumberOfColoredIntervals;count_bu++)
		{//cycle bunch
			    test_bunch=*(bunch_blocking+count_bu);

				 if (!test_bunch)
				 {//tb
		 bunch_occurrence=*(visible_bunches+count_bu);
		 label_value=-1;
		 if(bunch_occurrence>0)
		 {//bo
		label_value=Finding_labels(first_lower_hue,first_upper_hue,first_mean_hue,second_lower_hue,second_upper_hue,second_mean_hue,
		third_lower_hue,third_upper_hue,third_mean_hue,count_bu,&bunch_suitability);
		*(center_bunch_suitability+count_bu)=bunch_suitability+1;
		if(label_value>0)
		 {//lv
		*(label_segments+count_bu)=label_value;
		if(NumberOfLabels<16)
		Label_bunch_num[NumberOfLabels]=count_bu;
		NumberOfLabels++;
		//printf("\n $%d",StripCurrent->num_strip);
		 }//lv
		 }//bo

				 }//tb
		}//cycle bunch
			}//l*/
			//!!!!
			//memset(bunches_occurred,(int) '\0',sizeof(int)*(Max_number*PressedLength));
			Cleaning(2);

			RefinedNumberOfBunches=0;
			for(num_bunch=0;num_bunch<NumberOfColoredIntervals;num_bunch++)
			  {
			  test_bunch=*(bunch_blocking+num_bunch);
				 if (!test_bunch)
				 {
				 *(new_bunch_number+num_bunch)=RefinedNumberOfBunches;
				 *(old_bunch_number+RefinedNumberOfBunches)=num_bunch;
				 RefinedNumberOfBunches++;
				 }
				 else
				 {
				 *(new_bunch_number+num_bunch)=-1;
				 }
			  }

		// delete[] locally_visible_bunches;
		 delete[] encountered_bunches;
		 delete[] explored_bunches;
		 delete[] intersecting_bunches;
		 delete[] intersecting_type;
		}
//-----------------------------------------------------------------------------
void

CColorIntervalSelect::Strip_value_painting_bunch(int bunch, int beg_int,
		int end_int, int bunch_signif, int* bunch_strip, int* number_of_bunch,
		int* bunch_significance,int array_length)
{
	int paint_coun;
	int current_inhabitant;
	int paint_start, paint_finish;
	int val;

	paint_start = beg_int;
	paint_finish = end_int;
	for (paint_coun = paint_start; paint_coun <= paint_finish; paint_coun++)
	{
		current_inhabitant = *(bunch_strip + paint_coun);
		(*(number_of_bunch + paint_coun))++;
		if(!current_inhabitant)
		{
			*(bunch_strip+paint_coun)=bunch+1;
			*(bunch_significance+paint_coun)=bunch_signif;
		}
		else
		{
			val=*(bunch_significance+paint_coun);

			if(val<bunch_signif)
			{
				*(bunch_strip+paint_coun)=bunch+1;
				*(bunch_significance+paint_coun)=bunch_signif;

			}
		}

	}

}
//-----------------------------------------------------------------------------
int

CColorIntervalSelect::Test_Junction(int bunch, int neighbor_bunch,
		int occurrence)
{
	int bunch_mean_hue;
	int corrected_bunch_mean_hue;
	int neighbor_bunch_mean_hue;
	int corrected_neighbor_bunch_mean_hue;
	int neighbor_bunch_minimal_hue;
	int neighbor_bunch_maximal_hue;
	int bunch_mean_saturation;
	int neighbor_bunch_mean_saturation;
	int neighbor_bunch_minimal_saturation;
	int neighbor_bunch_maximal_saturation;
	int corrected_neighbor_bunch_minimal_hue;
	int corrected_neighbor_bunch_maximal_hue;
	int bunch_mean_gray;
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
	int Intersection;
	int indic_length;
	int inten_length;
	int node_vicinity;
	int node_vicinity_neighbor;
	int bunch_occurrence;
	int inclusion_relation;
	int bunch_significance;
	int neighbor_significance;
	int Signif_ratio;

	inclusion_relation=0;
	prior=0;
	node_vicinity=0;
	node_vicinity_neighbor=0;
	bunch_occurrence=occurrence;
	bunch_mean_hue=ColoredIntervalsStructure->AverageHue[bunch];
	neighbor_bunch_mean_hue=ColoredIntervalsStructure->AverageHue[neighbor_bunch];
	bunch_significance=ColoredIntervalsStructure->Significance[bunch];
	neighbor_significance=ColoredIntervalsStructure->Significance[neighbor_bunch];
	Signif_ratio=(16*bunch_significance)/(bunch_significance+neighbor_significance);
	neighbor_bunch_minimal_hue=
	ColoredIntervalsStructure->LowerHueValue[neighbor_bunch];
	neighbor_bunch_maximal_hue=
	ColoredIntervalsStructure->UpperHueValue[neighbor_bunch];
	corrected_bunch_mean_hue=bunch_mean_hue;
	corrected_neighbor_bunch_mean_hue=neighbor_bunch_mean_hue;
	corrected_neighbor_bunch_minimal_hue=neighbor_bunch_minimal_hue;
	corrected_neighbor_bunch_maximal_hue=neighbor_bunch_maximal_hue;
	if(((bunch_mean_hue>=44)&&(bunch_mean_hue<48))||
	((bunch_mean_hue>=0)&&(bunch_mean_hue<4))||
	((bunch_mean_hue>=28)&&(bunch_mean_hue<36))||
	((bunch_mean_hue>=10)&&(bunch_mean_hue<22)))
	{
	node_vicinity=1;
	}
	if(((neighbor_bunch_mean_hue>=44)&&(neighbor_bunch_mean_hue<52))||
	((neighbor_bunch_mean_hue>=0)&&(neighbor_bunch_mean_hue<4))||
	((neighbor_bunch_mean_hue>=28)&&(neighbor_bunch_mean_hue<36))||
	((neighbor_bunch_mean_hue>=10)&&(neighbor_bunch_mean_hue<22)))
	{
	node_vicinity_neighbor=1;
	}
	bunch_mean_saturation=ColoredIntervalsStructure->AverageSat[bunch];
	neighbor_bunch_mean_saturation=
	ColoredIntervalsStructure->AverageSat[neighbor_bunch];
	neighbor_bunch_minimal_saturation=
	ColoredIntervalsStructure->LowerSatValue[neighbor_bunch];
	neighbor_bunch_maximal_saturation=
	ColoredIntervalsStructure->UpperSatValue[neighbor_bunch];
	bunch_mean_gray=ColoredIntervalsStructure->AverageGray[bunch];
	neighbor_bunch_mean_gray=
	ColoredIntervalsStructure->AverageGray[neighbor_bunch];
	if(((bunch_mean_hue<8)&&(neighbor_bunch_mean_hue>8)
	&&(neighbor_bunch_mean_hue<32))||
	((bunch_mean_hue>8)&&(bunch_mean_hue<32)&&(neighbor_bunch_mean_hue<8)))
	{
	 return(prior);
	}
	if((bunch_mean_hue<4)&&(bunch_mean_hue>=0)&&(neighbor_bunch_mean_hue>39))
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

	}

	hue_difference=abs(corrected_bunch_mean_hue-corrected_neighbor_bunch_mean_hue);
	saturation_difference=
	abs(bunch_mean_saturation-neighbor_bunch_mean_saturation);
	gray_difference=abs(bunch_mean_gray-neighbor_bunch_mean_gray);
	if(hue_difference<=4)
	{
	beg_int=ColoredIntervalsStructure->BegInterv[bunch];
	end_int=ColoredIntervalsStructure->EndInterv[bunch];
	int_length=end_int-beg_int+1;
	neighbor_beg_int=ColoredIntervalsStructure->BegInterv[neighbor_bunch];
	neighbor_end_int=ColoredIntervalsStructure->EndInterv[neighbor_bunch];
	neighbor_length=neighbor_end_int-neighbor_beg_int+1;
	length_ratio=(16*int_length)/(int_length+neighbor_length);
	if((neighbor_beg_int<=beg_int)&&(neighbor_end_int>=end_int))
	{
	inclusion_relation=1;
	}
	Intersection=Intersection_measure(neighbor_beg_int,neighbor_end_int,beg_int,
	end_int,&indic_length);
	     if(gray_difference<=dev_mean[bunch_mean_gray/8])
		 {
		if((Intersection<=1)&&(indic_length>=2)&&(hue_difference<=1)&&
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation))&&
		((saturation_difference<=3)||
		((bunch_mean_saturation>8)&&(neighbor_bunch_mean_saturation>8)&&
		(saturation_difference<=4))))
		{
		 prior=1;
		 return(prior);
		}
		 }
		  if((node_vicinity)&&(node_vicinity_neighbor))
		  {
		  if((Intersection<=1)&&(indic_length>=2)&&(hue_difference<=3)&&
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation))&&
		((saturation_difference<=3)||
		((bunch_mean_saturation>8)&&(neighbor_bunch_mean_saturation>8)&&
		(saturation_difference<=4)))&&
		(gray_difference<=NUM_INTEN/4))
		{
		 prior=1;
		 return(prior);
		}
		  }
		inten_length=(NUM_INTEN/8)+4;
		if((bunch_mean_hue<5)&&(bunch_mean_hue>=2)&&
		(neighbor_bunch_mean_hue<5)&&(neighbor_bunch_mean_hue>=2))
		 {
		  if((Intersection<=1)&&(indic_length>=2)&&((hue_difference<=1)&&
		((bunch_mean_hue<=neighbor_bunch_maximal_hue)&&
		(bunch_mean_hue>=neighbor_bunch_minimal_hue)))&&
		((saturation_difference<=3)||
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation)))&&
		(gray_difference<=inten_length))
		{
		 prior=1;
		 return(prior);
		}
			if((Intersection<=1)&&(indic_length>=2)&&((hue_difference<=2)&&
		((bunch_mean_hue<=neighbor_bunch_maximal_hue)&&
		(bunch_mean_hue>=neighbor_bunch_minimal_hue)))&&
		((saturation_difference<=3)&&
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation)))&&
		(gray_difference<=inten_length))
		{
		 prior=1;
		 return(prior);
		}
		 }
			 if((bunch_mean_hue<8)&&(bunch_mean_hue>4)&&
		(neighbor_bunch_mean_hue<8)&&(neighbor_bunch_mean_hue>4))
			 {
		if((Intersection<=1)&&(indic_length>=2)&&((hue_difference<=1)&&
		((bunch_mean_hue<=neighbor_bunch_maximal_hue)&&
		(bunch_mean_hue>=neighbor_bunch_minimal_hue)))&&
		((saturation_difference<=3)||
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation)))&&
		(gray_difference<=inten_length))
		{
		 prior=1;
		 return(prior);
		}
			  if((Intersection<=1)&&(indic_length>=2)&&((hue_difference<=2)&&
		((bunch_mean_hue<=neighbor_bunch_maximal_hue)&&
		(bunch_mean_hue>=neighbor_bunch_minimal_hue)))&&
		((saturation_difference<=3)&&
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation)))&&
		(gray_difference<=inten_length))
		{
		 prior=1;
		 return(prior);
				}
			  }
				  if((bunch_mean_hue<24)&&(bunch_mean_hue>=8)&&
		(neighbor_bunch_mean_hue<24)&&(neighbor_bunch_mean_hue>=8))
				  {
		if((Intersection<=1)&&(indic_length>=2)&&((hue_difference<=1)&&
		((bunch_mean_hue<=neighbor_bunch_maximal_hue)&&
		(bunch_mean_hue>=neighbor_bunch_minimal_hue)))&&
		((saturation_difference<=3)||
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation)))&&
		(gray_difference<=inten_length))
		{
		 prior=1;
		 return(prior);
		}
			  if((Intersection<=1)&&(indic_length>=2)&&((hue_difference<=2)&&
		((bunch_mean_hue<=neighbor_bunch_maximal_hue)&&
		(bunch_mean_hue>=neighbor_bunch_minimal_hue)))&&
		((saturation_difference<=3)&&
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation)))&&
		(gray_difference<=inten_length))
			 {
		 prior=1;
		 return(prior);
			 }
				 }
		  if((bunch_mean_hue<39)&&(bunch_mean_hue>=24)&&
		(neighbor_bunch_mean_hue<39)&&(neighbor_bunch_mean_hue>=24))
		  {
	if((Intersection<=1)&&(indic_length>=2)&&((hue_difference<=2)&&
		((bunch_mean_hue<=neighbor_bunch_maximal_hue)&&
		(bunch_mean_hue>=neighbor_bunch_minimal_hue)))&&
		((saturation_difference<=3)||
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation)))&&
		(gray_difference<=inten_length))
		{
		 prior=1;
		 return(prior);
		}
		  if((Intersection<=1)&&(indic_length>=2)&&((hue_difference<=3)&&
		((bunch_mean_hue<=neighbor_bunch_maximal_hue+1)&&
		(bunch_mean_hue>=neighbor_bunch_minimal_hue-1)))&&
		((saturation_difference<=3)||
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation)))&&
		(gray_difference<=inten_length))
		{
		 prior=1;
		 return(prior);
		}
		  }
	  if((bunch_mean_hue<46)&&(bunch_mean_hue>=39)&&
		(neighbor_bunch_mean_hue<46)&&(neighbor_bunch_mean_hue>=39))
		  {
		  if((Intersection<=1)&&(indic_length>=2)&&((hue_difference<=2)&&
		((bunch_mean_hue<=neighbor_bunch_maximal_hue)&&
		(bunch_mean_hue>=neighbor_bunch_minimal_hue)))&&
		((saturation_difference<=3)||
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation)))&&
		(gray_difference<=inten_length))
		{
		 prior=1;
		 return(prior);
		}
		  }
	 if((corrected_bunch_mean_hue<50)&&(corrected_bunch_mean_hue>45)&&
	(corrected_neighbor_bunch_mean_hue<50)&&(corrected_neighbor_bunch_mean_hue>45))
			 {
			if((Intersection<=1)&&(indic_length>=2)&&((hue_difference<=1)&&
		((corrected_bunch_mean_hue<=corrected_neighbor_bunch_maximal_hue+1)&&
		(corrected_bunch_mean_hue>=corrected_neighbor_bunch_minimal_hue-1)))&&
		((saturation_difference<=3)||
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation)))&&
		(gray_difference<=inten_length))
		{
		 prior=1;
		 return(prior);
		}
		if((Intersection<=1)&&(indic_length>=2)&&((hue_difference<=2)&&
		((corrected_bunch_mean_hue<=corrected_neighbor_bunch_maximal_hue)&&
		(corrected_bunch_mean_hue>=corrected_neighbor_bunch_minimal_hue)))&&
		((saturation_difference<=3)||
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation)))&&
		(gray_difference<=inten_length))
		{
		 prior=1;
		 return(prior);
		}
		if((Intersection==2)&&((hue_difference<=1)&&(indic_length>=3)&&
		((corrected_bunch_mean_hue<=corrected_neighbor_bunch_maximal_hue)&&
		(corrected_bunch_mean_hue>=corrected_neighbor_bunch_minimal_hue)))&&
		((saturation_difference<=3)&&
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation)))&&
		(gray_difference<=inten_length))
		{
		 prior=1;
		 return(prior);
		}

			 }
						 if(bunch_occurrence>=4)
						 {
	 if((Intersection<=2)&&(indic_length>=1)&&(!hue_difference)&&
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation))&&
		((saturation_difference<=3)||
		((bunch_mean_saturation>8)&&(neighbor_bunch_mean_saturation>8)&&
		(saturation_difference<=4)))&&
		(gray_difference<=NUM_INTEN/4))
		{
		 prior=2;
		 return(prior);
		}
		if((node_vicinity)&&(node_vicinity_neighbor)&&
		(bunch_mean_saturation>=3)&&(neighbor_bunch_mean_saturation>=3))
		  {
		  if(((Intersection<=2)&&(indic_length>=1)||(Intersection<=1))
		  &&(hue_difference<=3)&&
		((bunch_mean_saturation<=neighbor_bunch_maximal_saturation)
		&&(bunch_mean_saturation>=neighbor_bunch_minimal_saturation))&&
		((saturation_difference<=3)||
		((bunch_mean_saturation>8)&&(neighbor_bunch_mean_saturation>8)&&
		(saturation_difference<=4)))&&
		(gray_difference<=NUM_INTEN/4))
		{
		 prior=2;
		 return(prior);
		}
		if((Intersection<=1)&&(hue_difference<=3)&&
		((saturation_difference<=3)||
		((bunch_mean_saturation>5)&&(neighbor_bunch_mean_saturation>5)&&
		(saturation_difference<=4))))
		{
		 prior=3;
		 return(prior);
		}
	 if((Intersection<=2)&&(hue_difference<=4)&&(indic_length>=2)&&
		(saturation_difference<=2)&&(bunch_mean_saturation>4)&&
		(neighbor_bunch_mean_saturation>4)&&
		(corrected_bunch_mean_hue<=corrected_neighbor_bunch_maximal_hue)&&
		(corrected_bunch_mean_hue>=corrected_neighbor_bunch_minimal_hue))
		{
		 prior=4;
		 return(prior);
		}
	     }
						 }
	}
	else
	{
	return(prior);
	}
	return(prior);
}

//-----------------------------------------------------------------------
void CColorIntervalSelect::Bunch_painting(int bunch_number, int beg_int,
		int end_int, int bunch_consist, int * valuable_bunch,
		int* quantity_of_bunches, int* bunch_consistency)
{
	int paint_coun;
	int current_inhabitant;
	int paint_start, paint_finish;
	int val;
	int point_location;
	int quant;

	paint_start = beg_int / 4;
	paint_finish = end_int / 4;
	for (paint_coun = paint_start; paint_coun <= paint_finish; paint_coun++)
	{
		current_inhabitant = *(valuable_bunch + paint_coun);
		quant = *(quantity_of_bunches + paint_coun);
		point_location = quant * PressedLength + paint_coun;

		*(bunches_occurred + point_location) = bunch_number;
		if (!current_inhabitant)
		{
			*(valuable_bunch + paint_coun) = bunch_number + 1;
			*(bunch_consistency + paint_coun) = bunch_consist;
		}
		else
		{
			val = *(bunch_consistency + paint_coun);
			if (val < bunch_consist)
			{
				*(valuable_bunch + paint_coun) = bunch_number + 1;
				*(bunch_consistency + paint_coun) = bunch_consist;

			}
		}
		(*(quantity_of_bunches + paint_coun))++;}

	}
	//-----------------------------------------------------------------------
void CColorIntervalSelect::Bunch_relation(int bunch, int* visible_bunches,
		int* intersecting_bunches, int* number_of_intersect, int* inters_type)
{
	int paint_coun;
	int paint_start;
	int paint_finish;
	int paint_length;
	int quant;
	int intens_cou;
	int point_location;
	int bunch_new;
	int test_junction;
	int test_interaction;
	int bunch_occurrence;
	int bunch_occurrence1;
	int blocking;
	int bunch_considered;
	int shift;
	int beg_int;
	int end_int;
	int bunch_significance;
	int bunch_density;
	int bunch_mean_hue;
	int bunch_minimal_hue;
	int bunch_maximal_hue;
	int bunch_mean_saturation;
	int bunch_lower_saturation;
	int bunch_upper_saturation;
	int bunch_mean_gray;
	int corrected_bunch_mean_hue;
	int bunch_hue_zone;
	int neighbor_beg_int;
	int neighbor_end_int;
	int bunch_new_significance;
	int neighbor_bunch_density;
	int neighbor_bunch_mean_hue;
	int neighbor_bunch_minimal_hue;
	int neighbor_bunch_maximal_hue;
	int neighbor_bunch_mean_gray;
	int neighbor_bunch_mean_saturation;
	int corrected_neighbor_bunch_mean_hue;
	int neighbor_bunch_hue_zone;
	int Intersection;
	int indic_length;
	int coinsidence;
	int coinsidence1;
	int prior;
	int difference_hue_zone;
	int hue_difference;
	int difference;
	int sat_difference;
	int upper_deviation;
	int lower_deviation;
	int admissible_sat_dif;
	int hue_upper_deviation;
	int hue_lower_deviation;
//int new_corrected_bunch_mean_hue;
//int new_corrected_neighbor_bunch_mean_hue;
	int admissible_hue_dif;
	int dif_col_signif;
	int dif1_col_signif;
	int close_col_signif;
	int close_shade_signif;
	int close_shade1_signif;
	int bunch_gray_zone;
	int bunch_new_gray_zone;
	int gray_difference;
	int gray_zone_difference;
	int admis_gray_dif;
	int count_intersect;
//int covered_part;
	int density_ratio;
	int close_color_dominate;
	int close_color_dominated;
	int different_color_dominate;
	int different_color_dominated;
	int different1_color_dominate;
	int different1_color_dominated;
	int close_shade_dominate;
	int close_shade_dominated;
	int close_shade1_dominate;
	int close_shade1_dominated;
	int neighbore_share;
	int bound_hue;
	int hue_dif_different;
	int unique_colored;
	int unique_ratio;
	/*int corrected_bunch_minimal_hue;
	 int corrected_bunch_maximal_hue; */
	int close_color_dominant;
	int close_shade_dominant;
	int close_color_dominant_signif;
	int close_shade_dominant_signif;
	int neighbor_length;
//int indic_length;
	int ratio_length;
	int ratio_length1;
	int distance_between_bunches;
	int inclusion_relation;
	int left_covered_boundary;
	int right_covered_boundary;

	indic_length = -1;
	ratio_length = -1;
	ratio_length1 = -1;
	distance_between_bunches = 0;
	Intersection = 3;
	inclusion_relation = 0;
	test_junction = 0;
	unique_colored = 0;
	bound_hue = 0;
	close_color_dominate = 0;
	close_color_dominated = 0;
	close_color_dominant_signif = 0;
	close_color_dominant = -1;
	different_color_dominate = 0;
	different_color_dominated = 0;
	different1_color_dominate = 0;
	different1_color_dominated = 0;
	close_color_dominate = 0;
	close_color_dominated = 0;
	close_shade_dominate = 0;
	close_shade_dominated = 0;
	close_shade_dominant_signif = 0;
	close_shade_dominant = -1;
	close_shade1_dominate = 0;
	close_shade1_dominated = 0;
	dif_col_signif = 0;
	dif1_col_signif = 0;
	close_col_signif = 0;
	close_shade_signif = 0;
	close_shade1_signif = 0;
//covered_part=0;
	shift = NumberOfColoredIntervals * bunch;
	bunch_occurrence = *(visible_bunches + bunch);
	beg_int = ColoredIntervalsStructure->BegInterv[bunch];
	end_int = ColoredIntervalsStructure->EndInterv[bunch];
	left_covered_boundary = beg_int - 1;
	right_covered_boundary = end_int + 1;
	bunch_significance = ColoredIntervalsStructure->Significance[bunch];
	bunch_density = (2 * bunch_significance) / (end_int - beg_int + 1);
	bunch_mean_hue = ColoredIntervalsStructure->AverageHue[bunch];
	bunch_minimal_hue = ColoredIntervalsStructure->LowerHueValue[bunch];
	bunch_maximal_hue = ColoredIntervalsStructure->UpperHueValue[bunch];
	/*if(bunch_minimal_hue<=bunch_maximal_hue)
	 {
	 corrected_bunch_minimal_hue=bunch_minimal_hue;
	 corrected_bunch_maximal_hue=bunch_maximal_hue;
	 }
	 else
	 {
	 corrected_bunch_minimal_hue=bunch_minimal_hue;
	 corrected_bunch_maximal_hue=bunch_maximal_hue+NUM_HUES;
	 }
	 if((corrected_bunch_maximal_hue-corrected_bunch_minimal_hue)<=2)
	 {
	 corrected_bunch_minimal_hue=std::max(0,bunch_minimal_hue-1);
	 corrected_bunch_maximal_hue=(bunch_maximal_hue+1);
	 }*/

	if (bunch_mean_hue >= NUM_HUES)
	{
		corrected_bunch_mean_hue = bunch_mean_hue - NUM_HUES;
	}
	else
	{
		corrected_bunch_mean_hue = bunch_mean_hue;
	}
	bunch_hue_zone = hue_zones[corrected_bunch_mean_hue];
	bunch_mean_saturation = ColoredIntervalsStructure->AverageSat[bunch];
	bunch_lower_saturation = ColoredIntervalsStructure->LowerSatValue[bunch];
	bunch_upper_saturation = ColoredIntervalsStructure->UpperSatValue[bunch];
	bunch_mean_gray = ColoredIntervalsStructure->AverageGray[bunch];
	bunch_gray_zone = gray_zones[bunch_mean_gray];

	paint_start = beg_int / 4;
	paint_finish = end_int / 4;
	paint_length = paint_finish - paint_start + 1;

	for (paint_coun = paint_start; paint_coun <= paint_finish; paint_coun++)
	{
		quant = *(painted_numbers_colored + paint_coun);
		if (quant > 1)
		{
			//covered_part+=1;
			for (intens_cou = 0; intens_cou < quant; intens_cou++)
			{
				point_location = intens_cou * PressedLength + paint_coun;
				bunch_new = *(bunches_occurred + point_location);
				bunch_occurrence1 = *(visible_bunches + bunch_new);
				bunch_new_significance =
						ColoredIntervalsStructure->Significance[bunch_new];
				blocking = *(bunch_blocking + bunch_new);
				bunch_considered = *(bunch_connections + shift + bunch_new);

				if ((!bunch_considered) && (!blocking) && (bunch_new != bunch))
				{
					neighbor_beg_int =
							ColoredIntervalsStructure->BegInterv[bunch_new];
					neighbor_end_int =
							ColoredIntervalsStructure->EndInterv[bunch_new];
					neighbor_length = neighbor_end_int - neighbor_beg_int + 1;
					if (neighbor_beg_int <= beg_int)
					{
						if (neighbor_end_int >= end_int)
						{
							inclusion_relation = 1;
//covers
						}
						else
						{
							inclusion_relation = -1;
//locates to the left
						}
					}
					else
					{
						if (neighbor_end_int <= end_int)
						{
							inclusion_relation = 2;
//belongs
						}
						else
						{
							inclusion_relation = 3;
//locates to the right
						}
					}
					neighbor_bunch_density = (2 * bunch_new_significance)
							/ (neighbor_end_int - neighbor_beg_int + 1);

					density_ratio = (16 * bunch_density)
							/ (neighbor_bunch_density + bunch_density);
					if (density_ratio <= 8)
					{
						if (inclusion_relation == 1)
						{
							left_covered_boundary = end_int;
							right_covered_boundary = beg_int;
						}
						else
						{
							if (inclusion_relation == -1)
							{
								left_covered_boundary = std::max(
										left_covered_boundary,
										neighbor_end_int);
							}
							else
							{
								if (inclusion_relation == 3)
								{
									right_covered_boundary = std::min(
											right_covered_boundary,
											neighbor_beg_int);
								}
							}
						}
					}

					Intersection = Intersection_measure_ratios(beg_int, end_int,
							neighbor_beg_int, neighbor_end_int, &indic_length,
							&ratio_length, &ratio_length1);
					//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
					//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
					//test_junction=Test_Junction(bunch_new,bunch,16);
					if (ratio_length1 == 0)
					{
						distance_between_bunches = ratio_length;
					}
					/*test_junction = Test_Junction_Corrected(bunch_new, bunch,
							bunch_occurrence, Intersection, indic_length,
							ratio_length, ratio_length1);*/
					neighbor_bunch_mean_hue =
							ColoredIntervalsStructure->AverageHue[bunch_new];
					neighbor_bunch_minimal_hue =
							ColoredIntervalsStructure->LowerHueValue[bunch_new];
					neighbor_bunch_maximal_hue =
							ColoredIntervalsStructure->UpperHueValue[bunch_new];
					neighbor_bunch_mean_saturation =
							ColoredIntervalsStructure->AverageSat[bunch_new];
					neighbor_bunch_mean_gray =
							ColoredIntervalsStructure->AverageGray[bunch_new];
					bunch_new_gray_zone = gray_zones[neighbor_bunch_mean_gray];
					if (neighbor_bunch_mean_hue >= NUM_HUES)
					{
						corrected_neighbor_bunch_mean_hue =
								neighbor_bunch_mean_hue - NUM_HUES;
					}
					else
					{
						corrected_neighbor_bunch_mean_hue =
								neighbor_bunch_mean_hue;
					}
					neighbor_bunch_hue_zone =
							hue_zones[corrected_neighbor_bunch_mean_hue];
					difference_hue_zone = abs(
							bunch_hue_zone - neighbor_bunch_hue_zone);
					difference_hue_zone = std::min(difference_hue_zone,
							5 - difference_hue_zone);

					difference = abs(neighbor_bunch_mean_hue - bunch_mean_hue);
					hue_difference = NUM_HUES - difference;
					hue_difference = std::min(difference, hue_difference);
					sat_difference = abs(
							bunch_mean_saturation
									- neighbor_bunch_mean_saturation);
					upper_deviation = bunch_upper_saturation
							- bunch_mean_saturation + 1;
					lower_deviation = bunch_mean_saturation
							- bunch_lower_saturation + 1;
					if (bunch_mean_saturation >= neighbor_bunch_mean_saturation)
					{
						admissible_sat_dif =
								sat_min_deviation[bunch_mean_saturation];
						admissible_sat_dif = std::min(admissible_sat_dif,
								lower_deviation);
					}
					else
					{
						admissible_sat_dif =
								sat_max_deviation[bunch_mean_saturation];
						admissible_sat_dif = std::min(admissible_sat_dif,
								upper_deviation);
					}
					difference = abs(bunch_maximal_hue - bunch_mean_hue);
					hue_upper_deviation = NUM_HUES - difference;
					hue_upper_deviation = std::min(difference, hue_upper_deviation);
					hue_upper_deviation = std::max(hue_upper_deviation, 2);
					difference = abs(bunch_minimal_hue - bunch_mean_hue);
					hue_lower_deviation = NUM_HUES - difference;
					hue_lower_deviation = std::min(hue_lower_deviation, difference);
					hue_lower_deviation = std::max(hue_lower_deviation, 2);
					if (corrected_bunch_mean_hue
							> corrected_neighbor_bunch_mean_hue)
					{
						difference = corrected_bunch_mean_hue
								- corrected_neighbor_bunch_mean_hue;
						if (difference > NUM_HUES / 2)
						{
							admissible_hue_dif =
									uppermax_hue_dif[corrected_bunch_mean_hue];
							admissible_hue_dif = std::min(admissible_hue_dif,
									hue_upper_deviation + 1);
							hue_dif_different =
									uppermax_hc_dif[corrected_bunch_mean_hue];
						}
						else
						{
							admissible_hue_dif =
									lowermax_hue_dif[corrected_bunch_mean_hue];
							admissible_hue_dif = std::min(admissible_hue_dif,
									hue_lower_deviation + 1);
							hue_dif_different =
									lowermax_hc_dif[corrected_bunch_mean_hue];
						}
					}
					else
					{
						difference = corrected_neighbor_bunch_mean_hue
								- corrected_bunch_mean_hue;
						if (difference > NUM_HUES / 2)
						{
							admissible_hue_dif =
									lowermax_hue_dif[corrected_bunch_mean_hue];
							admissible_hue_dif = std::min(admissible_hue_dif,
									hue_lower_deviation + 1);
							hue_dif_different =
									lowermax_hc_dif[corrected_bunch_mean_hue];
						}
						else
						{
							admissible_hue_dif =
									uppermax_hue_dif[corrected_bunch_mean_hue];
							admissible_hue_dif = std::min(admissible_hue_dif,
									hue_upper_deviation + 1);
							hue_dif_different =
									uppermax_hc_dif[corrected_bunch_mean_hue];
						}
					}
					/* if(new_corrected_bunch_mean_hue<=new_corrected_neighbor_bunch_mean_hue)
					 {
					 admissible_hue_dif=uppermax_hue_dif[corrected_bunch_mean_hue];
					 admissible_hue_dif=std::min(admissible_hue_dif,hue_upper_deviation+1);
					 }
					 else
					 {
					 admissible_hue_dif=lowermax_hue_dif[corrected_bunch_mean_hue];
					 admissible_hue_dif=std::min(admissible_hue_dif,hue_lower_deviation+1);
					 }*/
					gray_difference = abs(
							bunch_mean_gray - neighbor_bunch_mean_gray);
					gray_zone_difference = abs(
							bunch_new_gray_zone - bunch_gray_zone);
					if (gray_zone_difference == 1)
					{
						if (bunch_new_gray_zone >= bunch_gray_zone)
						{
							admis_gray_dif = up_gray_val_dev[bunch_gray_zone];
						}
						else
						{
							admis_gray_dif = down_gray_val_dev[bunch_gray_zone];
						}
					}
					else
					{
						if (!gray_zone_difference)
						{
							admis_gray_dif =
									inside_gray_val_dev[bunch_gray_zone];
						}
						else
						{
							admis_gray_dif = 0;
						}
					}

					prior = 0;
					Intersection = Measure1_of_proximity(beg_int, end_int,
							neighbor_beg_int, neighbor_end_int, &coinsidence,
							&coinsidence1, &indic_length);

					if ((test_junction > 0) && (test_junction < 10)
							&& (ratio_length1 == 16))
					{
						if ((bunch_occurrence1 < 4)
								&& (bunch_occurrence >= bunch_occurrence1)
								&& (bunch_new_significance < bunch_significance))
						{
							*(bunch_blocking + bunch_new) = bunch + 1;
							*(bunch_connections
									+ NumberOfColoredIntervals * bunch
									+ bunch_new) = test_junction + 1;
						}
					}
					blocking = *(bunch_blocking + bunch_new);
					if (!blocking)
					{

						if (difference_hue_zone > 1)
						{
							test_interaction = 8;
						}
						else
						{
							if ((difference_hue_zone == 1)
									&& (hue_difference >= hue_dif_different))
							{
								test_interaction = 7;
							}
							else
							{
								test_interaction = Test_Interaction(
										difference_hue_zone, hue_difference,
										admissible_hue_dif, sat_difference,
										admissible_sat_dif,
										bunch_mean_saturation,
										neighbor_bunch_mean_saturation);

							}
						}

						*(bunch_connections + NumberOfColoredIntervals * bunch
								+ bunch_new) = -(test_interaction + 1);

						if (((coinsidence >= 4) && (Intersection >= 2))
								|| ((coinsidence >= 8) && (Intersection >= 1))
								|| (coinsidence >= 12))
						{

							count_intersect = *number_of_intersect;
							*(intersecting_bunches + count_intersect) =
									bunch_new;
							*number_of_intersect += 1;
							if (test_interaction >= 7)
							{
								if (indic_length < 2)
								{
									neighbore_share = ((coinsidence1
											* bunch_new_significance)
											/ neighbor_length);
									dif_col_signif += neighbore_share;
									if (((density_ratio < 10)
											|| ((density_ratio < 11)
													&& (Intersection < 3)))
											&& ((coinsidence >= 14)
													|| ((coinsidence >= 12)
															&& (Intersection < 3)))
											&& (neighbor_bunch_density
													>= StripWidth / 2)
											&& (bunch_significance
													< bunch_new_significance))
									{
										different_color_dominated++;
									}
								}
								else
								{
									neighbore_share = ((coinsidence
											* bunch_new_significance) / 16);
									dif_col_signif += neighbore_share;
									if (((density_ratio > 6)
											|| ((density_ratio > 5)
													&& (Intersection < 3)))
											&& ((coinsidence >= 14)
													|| ((coinsidence >= 12)
															&& (Intersection < 3)))
											&& (bunch_density >= StripWidth / 2)
											&& (bunch_significance
													> bunch_new_significance))
									{
										different_color_dominate++;
									}
								}
								if ((indic_length >= 2) && (coinsidence >= 12)
										&& (bunch_density
												> neighbor_bunch_density))
								{
									*(inters_type + count_intersect) = 12;
								}
								else
								{
									if ((indic_length < 2)
											&& (coinsidence >= 12)
											&& (bunch_density
													< neighbor_bunch_density))
									{
										*(inters_type + count_intersect) = 13;
									}
									else
									{
										*(inters_type + count_intersect) = 14;
									}
								}
								goto L;
							}
							if (test_interaction == 6)
							{
								if (indic_length < 2)
								{
									neighbore_share = ((coinsidence1
											* bunch_new_significance)
											/ neighbor_length);
									dif1_col_signif += neighbore_share;
									if (((density_ratio < 10)
											|| ((density_ratio < 11)
													&& (Intersection < 3)))
											&& ((coinsidence >= 14)
													|| ((coinsidence >= 12)
															&& (Intersection < 3)))
											&& (neighbor_bunch_density
													>= StripWidth / 2)
											&& (bunch_significance
													< bunch_new_significance))
									{
										different1_color_dominated++;
									}
								}
								else
								{
									neighbore_share = ((coinsidence
											* bunch_new_significance) / 16);
									dif1_col_signif += neighbore_share;
									if (((density_ratio > 6)
											|| ((density_ratio > 5)
													&& (Intersection < 3)))
											&& ((coinsidence >= 14)
													|| ((coinsidence >= 12)
															&& (Intersection < 3)))
											&& (bunch_density >= StripWidth / 2)
											&& (bunch_significance
													> bunch_new_significance))
									{
										different1_color_dominate++;
									}
								}
								if ((indic_length >= 2) && (coinsidence >= 12)
										&& (bunch_density
												> neighbor_bunch_density))
								{
									*(inters_type + count_intersect) = 9;
								}
								else
								{
									if ((indic_length < 2)
											&& (coinsidence >= 12)
											&& (bunch_density
													< neighbor_bunch_density))
									{
										*(inters_type + count_intersect) = 10;
									}
									else
									{
										*(inters_type + count_intersect) = 11;
									}
								}
								goto L;
							}

							if (test_interaction <= 2)
							{
								if (gray_difference <= admis_gray_dif)
								{
									if (indic_length < 2)
									{
										neighbore_share = (coinsidence1
												* bunch_new_significance)
												/ neighbor_length;
										close_col_signif += neighbore_share;
										if ((Intersection >= 8)
												&& ((neighbor_bunch_density
														>= StripWidth / 2)
														|| (bunch_density
																>= StripWidth
																		/ 2)))
										{
											Merging(bunch, bunch_new);
										}
										if ((Intersection < 8)
												&& (coinsidence < 12)
												&& (density_ratio < 12)
												&& (density_ratio >= 4)
												&& ((neighbor_bunch_density
														>= StripWidth / 2)
														|| (bunch_density
																>= StripWidth
																		/ 2)))
										{
											Extension(bunch, bunch_new);
										}
										if (((density_ratio < 10)
												|| ((density_ratio < 11)
														&& (Intersection < 3)))
												&& ((coinsidence >= 14)
														|| ((coinsidence >= 12)
																&& (Intersection
																		< 3)))
												&& (neighbor_bunch_density
														>= StripWidth / 2)
												&& (bunch_significance
														< bunch_new_significance))
										{
											close_color_dominated++;
											if (bunch_new_significance
													> close_color_dominant_signif)
											{
												close_color_dominant =
														bunch_new;
												close_color_dominant_signif =
														bunch_new_significance;
											}
										}
									}
									else
									{
										neighbore_share = (coinsidence
												* bunch_new_significance) / 16;
										close_col_signif += neighbore_share;
										if (((density_ratio > 6)
												|| ((density_ratio > 5)
														&& (Intersection < 3)))
												&& ((coinsidence >= 14)
														|| ((coinsidence >= 12)
																&& (Intersection
																		< 3)))
												&& (bunch_density
														>= StripWidth / 2)
												&& (bunch_significance
														> bunch_new_significance))
										{
											close_color_dominate++;
										}
									}
									if ((indic_length >= 2)
											&& (coinsidence >= 12)
											&& (bunch_density
													> neighbor_bunch_density))
									{
										*(inters_type + count_intersect) = 0;
									}
									else
									{
										if ((indic_length < 2)
												&& (coinsidence >= 12)
												&& (bunch_density
														< neighbor_bunch_density))
										{
											*(inters_type + count_intersect) =
													1;
										}
										else
										{
											*(inters_type + count_intersect) =
													2;
										}
									}
								}
								else
								{
									if (indic_length < 2)
									{
										neighbore_share = (coinsidence1
												* bunch_new_significance)
												/ neighbor_length;
										close_shade_signif += neighbore_share;
										if (((density_ratio < 10)
												|| ((density_ratio < 11)
														&& (Intersection < 3)))
												&& ((coinsidence >= 14)
														|| ((coinsidence >= 12)
																&& (Intersection
																		< 3)))
												&& (neighbor_bunch_density
														>= StripWidth / 2)
												&& (bunch_significance
														< bunch_new_significance))
										{
											close_shade_dominated++;
											if (bunch_new_significance
													> close_shade_dominant_signif)
											{
												close_shade_dominant =
														bunch_new;
												close_shade_dominant_signif =
														bunch_new_significance;
											}
										}
									}
									else
									{
										neighbore_share = (coinsidence
												* bunch_new_significance) / 16;
										close_shade_signif += neighbore_share;
										if (((density_ratio > 6)
												|| ((density_ratio > 5)
														&& (Intersection < 3)))
												&& ((coinsidence >= 14)
														|| ((coinsidence >= 12)
																&& (Intersection
																		< 3)))
												&& (bunch_density
														>= StripWidth / 2)
												&& (bunch_significance
														> bunch_new_significance))
										{
											close_shade_dominate++;
										}
									}
									if ((indic_length >= 2)
											&& (coinsidence >= 12)
											&& (bunch_density
													> neighbor_bunch_density))
									{
										*(inters_type + count_intersect) = 3;
									}
									else
									{
										if ((indic_length < 2)
												&& (coinsidence >= 12)
												&& (bunch_density
														< neighbor_bunch_density))
										{
											*(inters_type + count_intersect) =
													4;
										}
										else
										{
											*(inters_type + count_intersect) =
													5;
										}
									}
								}
								goto L;
							}

							if (test_interaction != 4)
							{
								if (indic_length < 2)
								{
									neighbore_share = (coinsidence1
											* bunch_new_significance)
											/ neighbor_length;
									close_shade_signif += neighbore_share;
									/*if(((density_ratio<10)||((density_ratio<11)&&(Intersection<3)))&&
									 ((coinsidence>=14)||((coinsidence>=12)&&(Intersection<3)))&&
									 (neighbor_bunch_density>=StripWidth/2)&&
									 (bunch_significance<bunch_new_significance)&&
									 (((corrected_bunch_minimal_hue<=neighbor_bunch_mean_hue)&&
									 (neighbor_bunch_mean_hue<=corrected_bunch_maximal_hue))
									 ||((corrected_bunch_minimal_hue<=neighbor_bunch_mean_hue+NUM_HUES)&&
									 ((neighbor_bunch_mean_hue+NUM_HUES)<=corrected_bunch_maximal_hue))))*/
									if (((density_ratio < 10)
											|| ((density_ratio < 11)
													&& (Intersection < 3)))
											&& ((coinsidence >= 14)
													|| ((coinsidence >= 12)
															&& (Intersection < 3)))
											&& (neighbor_bunch_density
													>= StripWidth / 2)
											&& (bunch_significance
													< bunch_new_significance))
									{
										close_shade_dominated++;
										if (bunch_new_significance
												> close_shade_dominant_signif)
										{
											close_shade_dominant = bunch_new;
											close_shade_dominant_signif =
													bunch_new_significance;
										}
									}
								}
								else
								{
									neighbore_share = (coinsidence
											* bunch_new_significance) / 16;
									close_shade_signif += neighbore_share;
									if (((density_ratio > 6)
											|| ((density_ratio > 5)
													&& (Intersection < 3)))
											&& ((coinsidence >= 14)
													|| ((coinsidence >= 12)
															&& (Intersection < 3)))
											&& (bunch_density >= StripWidth / 2)
											&& (bunch_significance
													> bunch_new_significance))
									{
										close_shade_dominate++;
									}
								}

								if ((indic_length >= 2) && (coinsidence >= 12)
										&& (bunch_density
												> neighbor_bunch_density))
								{
									*(inters_type + count_intersect) = 3;
								}
								else
								{
									if ((indic_length < 2)
											&& (coinsidence >= 12)
											&& (bunch_density
													< neighbor_bunch_density))
									{
										*(inters_type + count_intersect) = 4;
									}
									else
									{
										*(inters_type + count_intersect) = 5;
									}
								}
							}
							else
							{
								if (indic_length < 2)
								{
									neighbore_share = (coinsidence1
											* bunch_new_significance)
											/ neighbor_length;
									close_shade1_signif += neighbore_share;
									if (((density_ratio < 10)
											|| ((density_ratio < 11)
													&& (Intersection < 3)))
											&& ((coinsidence >= 14)
													|| ((coinsidence >= 12)
															&& (Intersection < 3)))
											&& (neighbor_bunch_density
													>= StripWidth / 2)
											&& (bunch_significance
													< bunch_new_significance))
									{
										close_shade1_dominated++;
									}
								}
								else
								{
									neighbore_share = (coinsidence
											* bunch_new_significance) / 16;
									close_shade1_signif += neighbore_share;
									if (((density_ratio > 6)
											|| ((density_ratio > 5)
													&& (Intersection < 3)))
											&& ((coinsidence >= 14)
													|| ((coinsidence >= 12)
															&& (Intersection < 3)))
											&& (bunch_density >= StripWidth / 2)
											&& (bunch_significance
													> bunch_new_significance))
									{
										close_shade1_dominate++;
									}
								}
								if ((indic_length >= 2) && (coinsidence >= 12)
										&& (bunch_density
												> neighbor_bunch_density))
								{
									*(inters_type + count_intersect) = 6;
								}
								else
								{
									if ((indic_length < 2)
											&& (coinsidence >= 12)
											&& (bunch_density
													< neighbor_bunch_density))
									{
										*(inters_type + count_intersect) = 7;
									}
									else
									{
										*(inters_type + count_intersect) = 8;
									}
								}

							}
							L: ;
							//if()
						}
						else
						{
							if (test_interaction <= 2)
							{
								;
							}
						}
					}
				}
			}

		}
		else
		{
			unique_colored++;
		}

	}

	unique_ratio = 16 * unique_colored / paint_length;
	if (bunch_hue_zone == 2)
	{
		if (unique_ratio >= 14)
		{
			if ((bunch_mean_saturation <= 2) && (bunch_mean_gray <= 36)
					&& (bunch_density < (StripWidth / 2)))
			{
				//*(bunch_blocking+bunch)=NumberOfColoredIntervals+1;
				goto M;
			}
		}
	}
	if (bunch_hue_zone == 4)
	{

		if (((close_col_signif + close_shade_signif + close_shade1_signif)
				< bunch_significance / 2) && (bunch_density < (StripWidth / 3))
				&& (bunch_mean_saturation <= 2) && (bunch_mean_gray <= 30))
		{
			//*(bunch_blocking+bunch)=NumberOfColoredIntervals+1;
			goto M;
		}
		if (((close_col_signif + close_shade_signif + close_shade1_signif)
				< bunch_significance / 2)
				&& (bunch_density / 2 < (StripWidth / 3))
				&& (bunch_mean_saturation <= 5) && (bunch_mean_gray <= 7))
		{
			//*(bunch_blocking+bunch)=NumberOfColoredIntervals+1;
			goto M;
		}
		if (((close_col_signif + close_shade_signif + close_shade1_signif)
				< bunch_significance / 2)
				&& (bunch_density / 2 < (StripWidth / 2))
				&& (bunch_mean_saturation <= 5) && (bunch_mean_gray <= 4)
				&& (unique_ratio <= 8))
		{
			//*(bunch_blocking+bunch)=NumberOfColoredIntervals+1;//block!!!
			goto M;
		}
		if (((close_col_signif + close_shade_signif) > 4 * bunch_significance)
				&& (bunch_density < (StripWidth / 4))
				&& (bunch_mean_saturation <= 2) && (bunch_mean_gray <= 50)
				&& (unique_ratio <= 8))
		{
			//*(bunch_blocking+bunch)=NumberOfColoredIntervals+1;
			goto M;
		}

		if (unique_ratio >= 14)
		{
			if ((bunch_mean_saturation <= 2) && (bunch_mean_gray <= 30)
					&& (bunch_density < (StripWidth / 2)))
			{
				// *(bunch_blocking+bunch)=NumberOfColoredIntervals+1;
				goto M;
			}
			if ((bunch_mean_saturation <= 7) && (bunch_mean_gray <= 9)
					&& (bunch_density < (StripWidth / 3)))
			{
				// *(bunch_blocking+bunch)=NumberOfColoredIntervals+1;
				goto M;
			}
		}
		if ((bunch_mean_saturation <= 2) && (bunch_mean_gray <= 30)
				&& (bunch_density < (StripWidth / 4)))
		{
			//*(bunch_blocking+bunch)=NumberOfColoredIntervals+1;
			goto M;
		}
		if (close_color_dominated)
		{
			if ((bunch_density < (StripWidth / 4))
					&& (close_shade_signif > 2 * bunch_significance))
			{
				//*(bunch_blocking+bunch)=NumberOfColoredIntervals+2;
				goto M;
			}
		}
		else
		{
			if (close_shade_dominated)
			{
				if ((bunch_density < (StripWidth / 4))
						&& (bunch_mean_saturation <= 3)
						&& (bunch_mean_gray <= 30)
						&& (close_shade_signif > 2 * bunch_significance))
				{
					//*(bunch_blocking+bunch)=NumberOfColoredIntervals+2;
					goto M;
				}
			}
		}
	}
	if (bunch_hue_zone == 3)
	{
		if (close_color_dominated)
		{
			if ((bunch_density < (StripWidth / 4))
					&& (close_col_signif > 2 * bunch_significance))
			{
				// *(bunch_blocking+bunch)=NumberOfColoredIntervals+close_color_dominant;
				goto M;
			}
		}
		else
		{
			if (close_shade_dominated)
			{
				if ((bunch_density < (StripWidth / 4))
						&& (bunch_mean_saturation <= 3)
						&& (bunch_mean_gray <= 30)
						&& (close_shade_signif > 2 * bunch_significance))
				{
					//*(bunch_blocking+bunch)=NumberOfColoredIntervals+2;
					goto M;
				}
			}
		}
	}
	M: ;
	/*dif_col_signif=dif_col_signif/2;
	 close_col_signif=close_col_signif/2;
	 close_shade_signif=close_shade_signif/2;
	 close_shade1_signif=close_shade1_signif/2;*/
}
//-----------------------------------------------------------------------------
int

CColorIntervalSelect::Find_Better_Representative(int intens, int num_int,
		int* total_int_new, int* interval_collection, int start_point,
		int* interv_num, int* interval_intens)
{
	int prior;
	int beg_int;
	int end_int;
	int length_gener;
	int beg_int_member;
	int end_int_member;
	int length_member;
	int beg_int_bunch;
	int end_int_bunch;
	int length_bunch;
	int coun_bunch;
	int end_point;
	int bucn_member;
	int number_int;
	int num_intensit;
	int new_number_of_intensity;
	int new_number_of_new_intensity;
	int Mean_hue_bunch;
	int Mean_sat_bunch;
	int Hue_gener;
	int Sat_gener;
	int Hue_member;
	int Sat_member;
	int maximum_length;
	int maximal_int;
	int max_member_ratio;
	int intersection;
	int indic_length;
	int length_ratio;
	int grain_significance;
	int member_significance;
	int signif_ratio;
	int mean_grayscale;
	int mean_grayscale_mem;
	int possible_variation;
	int mean_gr_distance;

	prior = 0;
//return(prior);
//indic_length=-1;

	new_number_of_intensity = intensities_with_colored_int[intens];
	Mean_hue_bunch =
			ColoredIntervalsStructure->AverageHue[NumberOfColoredIntervals - 1];
	Mean_sat_bunch =
			ColoredIntervalsStructure->AverageSat[NumberOfColoredIntervals - 1];
	Hue_gener = ColorInform[new_number_of_intensity].TotCMHue[num_int];
	Sat_gener = ColorInform[new_number_of_intensity].TotCMSat[num_int];
	grain_significance = StripCurrent->IntAllInform[intens].Signif[num_int];
	beg_int = StripCurrent->IntAllInform[intens].BegInt[num_int];
	end_int = StripCurrent->IntAllInform[intens].EndInt[num_int];
	mean_grayscale = StripCurrent->IntAllInform[intens].GrayScaleMean[num_int];
	possible_variation = dev_mean[mean_grayscale / 8];
	length_gener = end_int - beg_int + 1;
	beg_int_bunch =
			ColoredIntervalsStructure->BegInterv[NumberOfColoredIntervals - 1];
	end_int_bunch =
			ColoredIntervalsStructure->EndInterv[NumberOfColoredIntervals - 1];
	length_bunch = end_int_bunch - beg_int_bunch + 1;
	end_point = start_point
			+ ColoredIntervalsStructure->Cardinality[NumberOfColoredIntervals
					- 1];
	maximum_length = length_gener;
	for (coun_bunch = start_point + 1; coun_bunch < end_point; coun_bunch++)
	{
		bucn_member = *(interval_collection + coun_bunch);
		number_int = *(interv_num + bucn_member);
		num_intensit = *(interval_intens + bucn_member);

		beg_int_member =
				StripCurrent->IntAllInform[num_intensit].BegInt[number_int];
		end_int_member =
				StripCurrent->IntAllInform[num_intensit].EndInt[number_int];
		member_significance =
				StripCurrent->IntAllInform[num_intensit].Signif[number_int];
		mean_grayscale_mem =
				StripCurrent->IntAllInform[num_intensit].GrayScaleMean[number_int];
		mean_gr_distance = abs(mean_grayscale_mem - mean_grayscale);
		length_member = end_int_member - beg_int_member + 1;
		intersection = Intersection_measure(beg_int, end_int, beg_int_member,
				end_int_member, &indic_length);

		length_ratio = (16 * length_member) / (length_member + length_gener);
		signif_ratio = (16 * member_significance)
				/ (member_significance + grain_significance);
		if ((length_ratio > 8) && (signif_ratio >= 6))
		{
			new_number_of_new_intensity =
					intensities_with_colored_int[num_intensit];
			Hue_member =
					ColorInform[new_number_of_new_intensity].TotCMHue[number_int];
			Sat_member =
					ColorInform[new_number_of_new_intensity].TotCMSat[number_int];

			if ((mean_gr_distance < possible_variation / 2)
					&& ((abs(Hue_member - Hue_gener) <= 1)
							|| (Hue_member == Mean_hue_bunch))
					&& ((abs(Sat_member - Sat_gener) <= 2)
							|| (abs(Sat_member - Mean_sat_bunch) <= 2)))
			{

				if (length_member > maximum_length)
				{
					maximal_int = bucn_member;
					maximum_length = length_member;
					max_member_ratio = (16 * maximum_length) / length_bunch;
					prior = max_member_ratio;
					*total_int_new = maximal_int;
				}
			}
		}
	}
	return (prior);
}
//-----------------------------------------------------------------------------
void

CColorIntervalSelect::AnalysisColorLessBack(int* inver_num,
		int* last_interval_continuation_investigated, int* visibility,
		int* interv_num, int* interval_intens, int* members,
		int* number_members)
{
	int count_lev;
	int num_intens;
	int NumInt;
	int count_interval;
	int begin_int;
	int end_int;
	int signif;
	int total_number;
	int coun_place;
	int strip_width;
	int intens;
	int int_intens;
//int interval_num;
	int last_investigated;
	int NumberOfGrains;
	int success;
	int* involved_intervals;
	int NumTotInt;
	int MaxLength;
	int interv_length;
	int NumberOfIntervalsInvolved;
	int* InvolvedCollection;
	int* interval_zone;
	int coun_zone;
	int inter_num;
	int count;
	int zone_number;
	int number_of_intervals[8];
	int bound_int;
	int current_inhabitant1;
	int co_interval;
	int new_number_of_intensity1;
	int cou_int;
	int first_second_ratio;
	int start_point_of_members;
	int* painted_numbers_colorless;
	int* painted_strip_colorless;

	return;
	success = 0;
	painted_strip_colorless = new int[PressedLength];
	painted_numbers_colorless = new int[PressedLength];
	strip_width = StripCurrent->StripWidth;
	NumberOfGrains = 0;
	NumberOfIntervals1 = NumberOfIntervals;
	MaxLength = 0;
	NumberOfIntervalsInvolved = 0;
	start_point_of_members = 0;

	memset(painted_strip_colorless, (int) '\0', sizeof(int) * (PressedLength));
	memset(number_of_intervals, (int) '\0', sizeof(int) * (8));

	for (count_lev = 0; count_lev < NumInterestingIntensities; count_lev++)
	{
		num_intens = OldNumbers[count_lev];
		NumInt = StripCurrent->num_of_int[num_intens];

		if (num_intens > 0)
		{
			for (count_interval = 0; count_interval < NumInt; count_interval++)
			{
				first_second_ratio =
						ColorInform[count_lev].UpperSatValue[count_interval];
				if ((first_second_ratio != -1)
						&& (ColorInform[count_lev].TypeOfColor[count_interval]
								< 0))
//	 (((ColorInform[count_lev].TypeOfColor[count_interval]<0)||
//(ColorInform[count_lev].TypeOfColor[count_interval]>=100))))
				{
					*(interval_intens + NumberOfIntervals1) = num_intens;
					*(interv_num + NumberOfIntervals1) = count_interval;
					*(inver_num + NUM_INTEN * count_interval + num_intens) =
							NumberOfIntervals1;
					NumberOfIntervals1++;
					begin_int =
							StripCurrent->IntAllInform[num_intens].BegInt[count_interval];
					end_int =
							StripCurrent->IntAllInform[num_intens].EndInt[count_interval];
					signif =
							StripCurrent->IntAllInform[num_intens].Signif[count_interval];
					signif = (signif << 1) / (end_int - begin_int + 1);
					if (signif > strip_width / 4)
					{
						Strip_value_painting_saturation(num_intens, begin_int,
								end_int, count_interval, signif,
								painted_strip_colorless,
								painted_numbers_colorless, consistency_colored);
					}
				}
			}
		}
	}
	VisibilityFinding(visibility, interv_num, interval_intens, inver_num,
			NumberOfIntervals, NumberOfIntervals1, painted_strip_colorless,
			painted_numbers_colorless);
	NumTotInt = NumberOfIntervals1 - NumberOfIntervals;
	involved_intervals = new int[NumTotInt];
	InvolvedCollection = new int[NumTotInt];
	memset(involved_intervals, (int) '\0', sizeof(int) * (NumTotInt));
	for (coun_place = 0; coun_place < PressedLength; coun_place++)
	{
		intens = *(painted_strip_colorless + coun_place);
		if (intens)
		{
			intens -= 1;
			int_intens = *(painted_numbers_colorless + coun_place);
			total_number = *(inver_num + NUM_INTEN * int_intens + intens);
			if (!*(involved_intervals + total_number - NumberOfIntervals))
			{
				*(InvolvedCollection + NumberOfIntervalsInvolved) =
						total_number;
				begin_int =
						StripCurrent->IntAllInform[intens].BegInt[int_intens];
				end_int = StripCurrent->IntAllInform[intens].EndInt[int_intens];
				interv_length = end_int - begin_int + 1;
				if (interv_length > MaxLength)
				{
					MaxLength = interv_length;
				}
				*(involved_intervals + total_number - NumberOfIntervals) =
						interv_length;
				NumberOfIntervalsInvolved++;
			}
		}

	}
	interval_zone = new int[8 * NumberOfIntervalsInvolved];for
(	count=0;count<NumberOfIntervalsInvolved;count++)
	{
		total_number=*(InvolvedCollection+count);
		interv_length=*(involved_intervals+total_number-NumberOfIntervals);
		zone_number=(interv_length*8)/(MaxLength+1);
		inter_num=zone_number*NumberOfIntervalsInvolved+
		number_of_intervals[zone_number];
		number_of_intervals[zone_number]++;
		*(interval_zone+inter_num)=total_number;
	}
	for (coun_zone = 0; coun_zone < 8; coun_zone++)
	{
		inter_num = (7 - coun_zone) * NumberOfIntervalsInvolved;
		bound_int = number_of_intervals[7 - coun_zone];
		for (cou_int = 0; cou_int < bound_int; cou_int++)
		{
			total_number = *(interval_zone + inter_num + cou_int);
			current_inhabitant1 = *(interval_intens + total_number);
			co_interval = *(interv_num + total_number);
			new_number_of_intensity1 =
					intensities_with_colored_int[current_inhabitant1];
			if ((ColorInform[new_number_of_intensity1].NumberOfColorBunch[co_interval]
					< 0)
					&& ((ColorInform[new_number_of_intensity1].NumberOfColorBunch[co_interval]
							+ MAX_COL_INT) > 0))
			{
				if (ColorInform[new_number_of_intensity1].TypeOfColor[co_interval]
						< 0)
				{
					memset(last_interval_continuation_investigated, (int) '\0',
							sizeof(int) * (NUM_INTEN));
					success = ColorLessGrouping(current_inhabitant1,
							co_interval, inver_num,
							last_interval_continuation_investigated, members,
							&start_point_of_members, visibility);
					if (success)
					{
						ColorInform[new_number_of_intensity1].NumberOfColorBunch[co_interval] =
								-NumberOfColorlessIntervals - MAX_COL_INT;
						if (NumberOfColorlessIntervals == 1)
						{
							*(number_members + 0) = 0;
						}
						else
						{
							*(number_members + NumberOfColorlessIntervals - 1) =
									*(number_members
											+ NumberOfColorlessIntervals - 2)
											+ ColorLessIntervalsStructure->Cardinality[NumberOfColorlessIntervals
													- 2];
						}
					}
				}
			}
		}
	}
	delete[] painted_strip_colorless;
	delete[] painted_numbers_colorless;
	delete[] involved_intervals;
	delete[] InvolvedCollection;
	delete[] interval_zone;
}
//-----------------------------------------------------------------------------
int

CColorIntervalSelect::ColorLessGrouping(int intensity, int interval_number,
		int* inver_num, int* continuation_investigated, int* members,
		int* start_poin, int* visibility)
{
	int intensity_red;
	int beg_int;
	int end_int;
	int fin_beg_int;
	int fin_end_int;
	int grain_significance;
	int density;
	int minimum_grayscale;
	int maximum_grayscale;
	int mean_grayscale;
	int new_number_of_intensity;
	int number_of_extentions;
	int Mean_sat_cl_bunch;
	int Mean_hue_cl_bunch;
	int prior;
	int paint_start;
	int paint_finish;
	int cycle_limit;
	int* quantity;
	int* intensity_list;
	int* interv_occurred;
	int paint_coun;
	int intens_cou;
	int paint_step;
	int quant;
	int point_location;
	int intensity_new;
	int interval_new;
	int intensity_red_new;
	int new_number_of_new_intensity;
	int beg_int_new;
	int end_int_new;
	int coinsidence;
	int coinsidence1;
	int intersection1;
	int intersection2;
	int indic_length;
	int indic_length1;
	int new_minimum_grayscale;
	int new_maximum_grayscale;
	int new_mean_grayscale;
	int new_impact_of_significance;
	int intensity_interval_left;
	int intensity_interval_right;
	int total_number;
	int total_number_new;
	int last_investigated_interval;
	float Bunch_gray;
	float Bunch_sat;
	int Mean_gray;
	int Mean_sat_bunch;
	int Mean_sat_new;
	int first_second_ratio;
	int Mean_hue_new;
	int new_dens;
	int visible_index;
	int sat_dif;

	prior = 0;
	number_of_extentions = 0;
	if ((intensity > 60) && (intensity < 71))
	{
		intensity_red = intensity - 39;
	}
	else
	{
		if ((intensity > 70) && (intensity < 81))
		{
			intensity_red = intensity - 49;
		}
		else
		{
			if ((intensity > 80) && (intensity < 95))
			{
				intensity_red = intensity - 49;
			}
			else
			{
				if ((intensity > 94) && (intensity <= 108))
				{
					intensity_red = intensity - 63;
				}
				else
				{
					if ((intensity > 108) && (intensity <= 123))
					{
						intensity_red = intensity - 63;
					}
					else
					{
						if ((intensity > 123) && (intensity <= 127))
						{
							intensity_red = 31;
						}
						else
						{
							intensity_red = intensity;
						}
					}
				}
			}
		}
	}
	/*if((intensity>60)&&(intensity<68))
	 {
	 intensity_red=intensity-34;
	 }
	 else
	 {
	 if(intensity>67)
	 {
	 intensity_red=intensity-42;
	 }
	 else
	 {
	 intensity_red=intensity;
	 }
	 }*/

	beg_int = StripCurrent->IntAllInform[intensity].BegInt[interval_number];
	end_int = StripCurrent->IntAllInform[intensity].EndInt[interval_number];
	fin_beg_int = beg_int;
	fin_end_int = end_int;
	intensity_interval_left = intensity_red;
	intensity_interval_right = intensity_red;
	grain_significance =
			StripCurrent->IntAllInform[intensity].Signif[interval_number];
	density = grain_significance / (end_int - beg_int + 1);
	minimum_grayscale =
			StripCurrent->IntAllInform[intensity].LowerGrayScale[interval_number];
	maximum_grayscale =
			StripCurrent->IntAllInform[intensity].UpperGrayScale[interval_number];
	mean_grayscale =
			StripCurrent->IntAllInform[intensity].GrayScaleMean[interval_number];
	Mean_gray = mean_grayscale;
	new_number_of_intensity = intensities_with_colored_int[intensity];
	Mean_sat_cl_bunch =
			ColorInform[new_number_of_intensity].TotCMSat[interval_number];
	Mean_hue_cl_bunch =
			ColorInform[new_number_of_intensity].TotCMHue[interval_number];
	if ((Mean_sat_cl_bunch > 2)
			|| ((Mean_sat_cl_bunch == 2) && (mean_grayscale > 40)
					&& (mean_grayscale < 50)))
	{
		return (prior);
	}
	Mean_sat_bunch = Mean_sat_cl_bunch;
	total_number = *(inver_num + NUM_INTEN * interval_number + intensity);
	*(members + *start_poin) = total_number;
	(*start_poin)++;
	paint_start = beg_int >> PRESSING;
	paint_finish = end_int >> PRESSING;
	cycle_limit = (paint_finish - paint_start) / 2;
	quantity = StripCurrent->quantity_of_intensities;
	intensity_list = StripCurrent->intensities_occurred;
	interv_occurred = StripCurrent->interval_occurred;

	for (paint_coun = 0; paint_coun <= cycle_limit; paint_coun++)
	{
		paint_step = paint_start + 2 * paint_coun;
		quant = *(quantity + paint_step);
		for (intens_cou = 0; intens_cou < quant; intens_cou++)
		{
			point_location = intens_cou * PressedLength + paint_step;
			intensity_new = *(intensity_list + point_location);
			if (intensity_new == 0)
			{
				goto L;
			}
			interval_new = *(interv_occurred + point_location);

			last_investigated_interval = *(continuation_investigated
					+ intensity_new) - 1;
			new_number_of_new_intensity =
					intensities_with_colored_int[intensity_new];
			first_second_ratio =
					ColorInform[new_number_of_new_intensity].UpperSatValue[interval_new];
			if ((interval_new == interval_number)
					&& (intensity_new == intensity) || (first_second_ratio < 0))
			{
				goto L;
			}
			if (interval_new > last_investigated_interval)
			{
				*(continuation_investigated + intensity_new) = interval_new + 1;
				if ((ColorInform[new_number_of_new_intensity].NumberOfColorBunch[interval_new]
						>= 0)
						|| (ColorInform[new_number_of_new_intensity].TypeOfColor[interval_new]
								>= 0)
						|| ((ColorInform[new_number_of_new_intensity].NumberOfColorBunch[interval_new]
								+ MAX_COL_INT) <= 0))
				{
					goto L;
				}
				if ((intensity_new > 60) && (intensity_new < 68))
				{
					intensity_red_new = intensity_new - 34;
				}
				else
				{
					if (intensity_new > 67)
					{
						intensity_red_new = intensity_new - 42;
					}
					else
					{
						intensity_red_new = intensity_new;
					}
				}
				if (((intensity_red_new > 31) && (intensity_red < 31)
						&& ((intensity_red_new - intensity_red) > 2))
						|| ((intensity_red_new < 31) && (intensity_red > 31)
								&& (intensity_red - intensity_red_new) > 2))
				{
					goto L;
				}
				beg_int_new =
						StripCurrent->IntAllInform[intensity_new].BegInt[interval_new];
				end_int_new =
						StripCurrent->IntAllInform[intensity_new].EndInt[interval_new];

				intersection1 = Intersection_measure(beg_int, end_int,
						beg_int_new, end_int_new, &indic_length);
				intersection2 = Measure1_of_proximity(beg_int, end_int,
						beg_int_new, end_int_new, &coinsidence, &coinsidence1,
						&indic_length1);

//if((intersection2>11)||((coinsidence>11)&&(indic_length>=2))
				if (((intersection1 == 1) && (indic_length >= 2))
						|| ((intersection1 == 0) && (indic_length >= 1)))
				{
					new_minimum_grayscale =
							StripCurrent->IntAllInform[intensity_new].LowerGrayScale[interval_new];
					new_maximum_grayscale =
							StripCurrent->IntAllInform[intensity_new].UpperGrayScale[interval_new];
					new_mean_grayscale =
							StripCurrent->IntAllInform[intensity_new].GrayScaleMean[interval_new];
					new_impact_of_significance =
							StripCurrent->IntAllInform[intensity_new].Signif[interval_new];
					new_dens = new_impact_of_significance
							/ (end_int_new - beg_int_new + 1);
					total_number_new = *(inver_num + NUM_INTEN * interval_new
							+ intensity_new);
					visible_index = *(visibility + total_number_new);

					if ((visible_index > 0) && (intersection2 < 12)
							&& (coinsidence < 12))
					{
						goto L;
					}
					if ((new_dens < 2) || (density > 4 * new_dens))
					{
						goto L;
					}

					Mean_sat_new =
							ColorInform[new_number_of_new_intensity].TotCMSat[interval_new];
					sat_dif = abs(Mean_sat_cl_bunch - Mean_sat_new);
					Mean_hue_new =
							ColorInform[new_number_of_new_intensity].TotCMHue[interval_new];
					if (sat_dif > 1)
					{
						goto L;
					}
					if ((intersection2 < 3)
							&& ((coinsidence < 14) || (density > 3 * new_dens)
									|| (sat_dif > 0)))
					{
						goto L;
					}
					if ((intersection2 < 4)
							&& ((density > 4 * new_dens) || (coinsidence < 12)
									|| (sat_dif > 0)))
					{
						goto L;
					}
					if ((Mean_sat_cl_bunch > 0) && (Mean_sat_new > 0)
							&& (abs(Mean_hue_new - Mean_hue_cl_bunch) >= 16))
					{
						goto L;
					}
					Bunch_gray = ((float) Mean_gray * (float) grain_significance
							+ (float) new_mean_grayscale
									* (float) new_impact_of_significance)
							/ (float) (grain_significance
									+ new_impact_of_significance);
					Mean_gray = (int) Bunch_gray;
					if ((Bunch_gray - Mean_gray) > 0.5)
					{
						Mean_gray += 1;
					}
					Bunch_sat = (float) (Mean_sat_bunch * grain_significance
							+ Mean_sat_new * new_impact_of_significance)
							/ (float) (grain_significance
									+ new_impact_of_significance);
					Mean_sat_bunch = (int) Bunch_sat;
					if ((Bunch_sat - Mean_sat_bunch) > 0.5)
					{
						Mean_sat_bunch += 1;
					}
					number_of_extentions++;

					fin_beg_int = std::min(fin_beg_int, beg_int_new);
					fin_end_int = std::max(fin_end_int, end_int_new);
					minimum_grayscale = std::min(minimum_grayscale,
							new_minimum_grayscale);
					maximum_grayscale = std::max(maximum_grayscale,
							new_maximum_grayscale);
					intensity_interval_left = std::min(intensity_interval_left,
							intensity_red_new);
					intensity_interval_right = std::max(intensity_interval_right,
							intensity_red_new);
					grain_significance += new_impact_of_significance;

					*(members + *start_poin) = total_number_new;
					(*start_poin)++;
					ColorInform[new_number_of_new_intensity].NumberOfColorBunch[interval_new] =
							-NumberOfColorlessIntervals - MAX_COL_INT;
				}
			}
			L: ;
		}

	}
	prior = 1;
	ColorLessIntervalsStructure->BegInterv[NumberOfColorlessIntervals] =
			fin_beg_int;
	ColorLessIntervalsStructure->EndInterv[NumberOfColorlessIntervals] =
			fin_end_int;
	ColorLessIntervalsStructure->GrainIntensity[NumberOfColorlessIntervals] =
			intensity;
	ColorLessIntervalsStructure->LowerIntensity[NumberOfColorlessIntervals] =
			intensity_interval_left;
	ColorLessIntervalsStructure->UpperIntensity[NumberOfColorlessIntervals] =
			intensity_interval_right;
	ColorLessIntervalsStructure->AverageSat[NumberOfColorlessIntervals] =
			Mean_sat_bunch;
	ColorLessIntervalsStructure->LowerGrayscale[NumberOfColorlessIntervals] =
			minimum_grayscale;
	ColorLessIntervalsStructure->UpperGrayscale[NumberOfColorlessIntervals] =
			maximum_grayscale;
	ColorLessIntervalsStructure->AverageGray[NumberOfColorlessIntervals] =
			Mean_gray;
	ColorLessIntervalsStructure->Significance[NumberOfColorlessIntervals] =
			grain_significance;
	ColorLessIntervalsStructure->Cardinality[NumberOfColorlessIntervals] =
			number_of_extentions + 1;
//ColorInform[new_number_of_intensity].NumberOfColorBunch[interval_number]=
//	 NumberOfColorlessIntervals+MAX_COL_INT;
	NumberOfColorlessIntervals++;

	return (prior);
}
//-----------------------------------------------------------------------------
int

CColorIntervalSelect::Test_Interaction(int difference_hue_zone,
		int hue_difference, int admissible_hue_dif, int sat_difference,
		int admissible_sat_dif, int bunch_mean_saturation,
		int neighbor_bunch_mean_saturation)
{
	int color_closeness;
//int log_gray_dif;

	color_closeness = -1;

	if (difference_hue_zone != 0)
	{
		if ((hue_difference <= 1) && (sat_difference <= admissible_sat_dif))
		{
			color_closeness = 0;
		}
		else
		{
			if (hue_difference <= 3)
			{
				color_closeness = 5;
			}
			else
			{
				color_closeness = 6;
			}
		}
	}
	else
	{

		if ((hue_difference <= 1)
				&& ((sat_difference <= admissible_sat_dif)
						|| ((bunch_mean_saturation >= 4)
								&& (neighbor_bunch_mean_saturation >= 4)
								&& (sat_difference <= admissible_sat_dif + 1))))
		{
			color_closeness = 0;
		}
		else
		{
			if ((hue_difference <= admissible_hue_dif)
					&& ((sat_difference <= admissible_sat_dif)
							|| ((bunch_mean_saturation >= 4)
									&& (neighbor_bunch_mean_saturation >= 4)
									&& (sat_difference <= admissible_sat_dif + 1))))
			{
				color_closeness = 1;
			}
			else
			{
				if ((hue_difference <= admissible_hue_dif)
						&& (bunch_mean_saturation >= 4)
						&& (neighbor_bunch_mean_saturation >= 4))
				{
					color_closeness = 2;
				}
				else
				{
					if (hue_difference <= 4)
					{
						color_closeness = 3;
					}
					else
					{
						color_closeness = 4;
					}
				}
			}
		}
	}
//log_gray_dif=(difference*16)/bunch_mean_gray;

	return (color_closeness);
}
//----------------------------------------------------------------------------
int

CColorIntervalSelect::Measure1_of_proximity(int first_left, int first_right,
		int second_left, int second_right, int *proximity1, int *proximity2,
		int* greater)
{
	int prior;
	int left_end, right_end, length1, length2;
	int minimum_of_length, max_length;
	int inter_length;

	left_end = std::max(first_left, second_left);
	right_end = std::min(first_right, second_right);
	length1 = first_right - first_left + 1;
	length2 = second_right - second_left + 1;
	*greater = 0;
	if (length1 >= 2 * length2)
	{
		*greater = 3;
	}
	else
	{
		if (length1 >= length2)
		{
			*greater = 2;
		}
		else
		{
			if (2 * length1 >= length2)
			{
				*greater = 1;
			}
		}
	}
	minimum_of_length = std::min(length1, length2);
	max_length = std::max(length1, length2);
	if (left_end > right_end)
	{
		inter_length = left_end - right_end + 1;
		prior = 0;
		*proximity1 = 0;
		*proximity2 = ((inter_length + 1) << 4)
				/ (inter_length + 1 + minimum_of_length);
		return (prior);
	}
	inter_length = right_end - left_end + 1;
	prior = ((inter_length + 1) << 4) / max_length;
	*proximity1 = ((inter_length + 1) << 4) / minimum_of_length;
	*proximity2 = inter_length;
	return (prior);

}
//----------------------------------------------------------------------------
void

CColorIntervalSelect::Merging(int bunch,int bunch_new)
{
int bunch_significance;
int new_bunch_significance;
int signif_ratio;
int bunch_new_significance;
int bunch_occurence;
int bunch_new_occurence;
int bunch_beg;
int bunch_end;
int bunch_new_beg;
int bunch_new_end;
int new_beg;
int new_end;
int bunch_density;
int new_bunch_density;
int bunch_new_density;
int int_length;
int bunch_new_length;
int Density_ratio;
int length_ratio;
int indic_length;
int ratio_length;
int ratio_length1;
int Intersection;
int paint_beg;
int paint_end;
int union_beg;
int union_end;
int union_length;
int outlet_length_ratio;
int outlet_length_ratio1;
int outlet_length;
int bunch_hue;
int corrected_bunch_hue;
int bunch_new_hue;
int corrected_bunch_new_hue;
int correction_hue;
int bunch_saturation;
int bunch_new_saturation;
int Mean_hue_bunch;
int lower_bunch_hue_bound;
int upper_bunch_hue_bound;
int lower_bunch_new_hue_bound;
int upper_bunch_new_hue_bound;
float Bunch_hue;
float Bunch_sat;
float Bunch_gray;
int Mean_gray_bunch;
int Mean_sat_bunch;
int bunch_zone;
int bunch_new_zone;
int zone_difference;
int hue_difference;
int hue_difference1;
int bunch_gray;
int bunch_new_gray;
int gray_difference;
int bunch_zone_gray;
int bunch_new_zone_gray;
int gray_zone_difference;
int lower_bunch_gray_bound;
int upper_bunch_gray_bound;
int new_bunch_occurence;


correction_hue=0;

bunch_significance=ColoredIntervalsStructure->Significance[bunch];
bunch_new_significance=ColoredIntervalsStructure->Significance[bunch_new];
signif_ratio=(16*bunch_new_significance)/(bunch_new_significance+bunch_significance);
bunch_hue=ColoredIntervalsStructure->AverageHue[bunch];
if(bunch_hue>=NUM_HUES)
{
	bunch_hue=bunch_hue-NUM_HUES;
	ColoredIntervalsStructure->AverageHue[bunch]=bunch_hue;
}
lower_bunch_hue_bound=ColoredIntervalsStructure->LowerHueValue[bunch];
upper_bunch_hue_bound=ColoredIntervalsStructure->UpperHueValue[bunch];
bunch_zone=hue_zones[bunch_hue];
bunch_gray=ColoredIntervalsStructure->AverageGray[bunch];
Bunch_gray=(float)bunch_gray;
lower_bunch_gray_bound=ColoredIntervalsStructure->LowerGrayscale[bunch];
upper_bunch_gray_bound=ColoredIntervalsStructure->UpperGrayscale[bunch];
bunch_new_hue=ColoredIntervalsStructure->AverageHue[bunch_new];
if(bunch_new_hue>=NUM_HUES)
{
	bunch_new_hue=bunch_new_hue-NUM_HUES;
	ColoredIntervalsStructure->AverageHue[bunch_new]=bunch_new_hue;
}
bunch_new_zone=hue_zones[bunch_new_hue];
zone_difference=abs(bunch_new_zone-bunch_zone);
zone_difference=std::min(zone_difference,5-zone_difference);
hue_difference1=abs(bunch_hue-bunch_new_hue);
hue_difference=std::min(hue_difference1,NUM_HUES-hue_difference1);
if((hue_difference1>NUM_HUES/2)&&(hue_difference<10))
{
	correction_hue=1;
	if(bunch_hue>bunch_new_hue)
	{
corrected_bunch_hue=bunch_hue;
corrected_bunch_new_hue=bunch_new_hue+NUM_HUES;
	}
	else
	{
    corrected_bunch_hue=bunch_hue+NUM_HUES;
	corrected_bunch_new_hue=bunch_new_hue;
	}
}
else
{
	corrected_bunch_hue=bunch_hue;
	corrected_bunch_new_hue=bunch_new_hue;
}
Bunch_hue=(float)corrected_bunch_hue;

lower_bunch_new_hue_bound=ColoredIntervalsStructure->LowerHueValue[bunch_new];
upper_bunch_new_hue_bound=ColoredIntervalsStructure->UpperHueValue[bunch_new];
bunch_saturation=ColoredIntervalsStructure->AverageSat[bunch];
Bunch_sat=(float)bunch_saturation;
bunch_new_saturation=ColoredIntervalsStructure->AverageSat[bunch_new];
bunch_new_gray=ColoredIntervalsStructure->AverageGray[bunch_new];
gray_difference=abs(bunch_new_gray-bunch_gray);
bunch_zone_gray=gray_zones[bunch_gray];
bunch_new_zone_gray=gray_zones[bunch_new_gray];
gray_zone_difference=abs(bunch_new_zone_gray-bunch_zone_gray);

bunch_occurence=*(visible_bunches+bunch);
bunch_new_occurence=*(visible_bunches+bunch_new);
bunch_beg=ColoredIntervalsStructure->BegInterv[bunch];
bunch_end=ColoredIntervalsStructure->EndInterv[bunch];
int_length=bunch_end-bunch_beg+1;
bunch_new_beg=ColoredIntervalsStructure->BegInterv[bunch_new];
bunch_new_end=ColoredIntervalsStructure->EndInterv[bunch_new];
bunch_new_length=bunch_new_end-bunch_new_beg+1;
union_beg=std::min(bunch_beg,bunch_new_beg);
	union_end=std::max(bunch_end,bunch_new_end);
	union_length=union_end-union_beg+1;
	outlet_length=union_length-int_length;
	outlet_length_ratio=(16*bunch_new_length)/(bunch_new_length+union_length);
    outlet_length_ratio1=(16*outlet_length)/(outlet_length+union_length);

Intersection=Intersection_measure_ratios(bunch_beg,bunch_end,bunch_new_beg,bunch_new_end,
&indic_length,&ratio_length,&ratio_length1);
if(Intersection==3)
{
	ratio_length=0;
}
	//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
	//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
bunch_density=2*bunch_significance/int_length;
bunch_new_density=2*bunch_new_significance/bunch_new_length;
Density_ratio=(16*bunch_new_density)/(bunch_density+bunch_new_density);
length_ratio=(16*bunch_new_length)/(int_length+bunch_new_length);
new_beg=std::min(bunch_beg,bunch_new_beg);
new_end=std::max(bunch_end,bunch_new_end);
  if((signif_ratio>3)&&(length_ratio>3))
  {//sr
if((zone_difference<=1)&&(hue_difference<=6))
{//zd&hd
Bunch_hue=(Bunch_hue*(float)bunch_significance+
	(float)(corrected_bunch_new_hue*bunch_new_significance))/
	(float)(bunch_significance+bunch_new_significance);

	Mean_hue_bunch=(int)Bunch_hue;
	                    if((Bunch_hue-Mean_hue_bunch)>0.5)
	                      {
	                      Mean_hue_bunch+=1;
	                      }
						if(Mean_hue_bunch>=NUM_HUES)
	{
		Mean_hue_bunch=Mean_hue_bunch-NUM_HUES;
	}


						Bunch_sat=(float)(Bunch_sat*(float)bunch_significance+
	(float)(bunch_new_saturation*bunch_new_significance))/
	(float)(bunch_significance+bunch_new_significance);
	Mean_sat_bunch=(int)Bunch_sat;
	     if(Bunch_sat<2)
	     {
if((Bunch_sat-Mean_sat_bunch)>0.6)
	{
	Mean_sat_bunch+=1;
	}
	     }
	     else
	     {
	    	 if((bunch_new_zone==3)&&(bunch_zone==3))
	    	 			 {//last_cor14.02.17
	    	 if(((bunch_new_saturation>=3)&&(bunch_saturation>=2))
	    	    ||((bunch_saturation>=3)&&(bunch_new_saturation>=2)))
	    	 {
	if((bunch_new_zone_gray>=1)&&(bunch_new_zone_gray<=4)&&
			(bunch_zone_gray>=1)&&(bunch_zone_gray<=4)&&(gray_difference<=6))
		{
			if((Bunch_sat-Mean_sat_bunch)>0.4)
		    {
		    Mean_sat_bunch+=1;
		    }
		}
		else
		{//last_cor03.05.17
	if((Bunch_sat-Mean_sat_bunch)>0.5)
		    {
		    Mean_sat_bunch+=1;
		    }
		}
}
			 }//last_cor14.02.17
			 else
			 {//last_cor14.02.17
	if((Bunch_sat-Mean_sat_bunch)>0.46)
	{
	Mean_sat_bunch+=1;
	}
			 }//last_cor14.02.17
		 }
		 if(bunch_hue!=Mean_hue_bunch)
						{//bh!
							if(lower_bunch_hue_bound>=NUM_HUES)
							{
								lower_bunch_hue_bound=lower_bunch_hue_bound-NUM_HUES;
							}
							if(upper_bunch_hue_bound>=NUM_HUES)
							{
								upper_bunch_hue_bound=upper_bunch_hue_bound-NUM_HUES;
							}
							if(Mean_hue_bunch>upper_bunch_hue_bound)
							{
								if(abs(Mean_hue_bunch-upper_bunch_hue_bound)<(NUM_HUES/2))
								{
								upper_bunch_hue_bound=Mean_hue_bunch;
								}
							}
							else
							{
								if(Mean_hue_bunch<upper_bunch_hue_bound)
							   {
								   if(abs(Mean_hue_bunch-upper_bunch_hue_bound)>(NUM_HUES/2))
								   {
								   upper_bunch_hue_bound=Mean_hue_bunch;
								   }
							   }
							}
                           if(Mean_hue_bunch<lower_bunch_hue_bound)
							{
								if(abs(Mean_hue_bunch-lower_bunch_hue_bound)<(NUM_HUES/2))
								{
								lower_bunch_hue_bound=Mean_hue_bunch;
								}
							}
                            else
							{
								if(Mean_hue_bunch>lower_bunch_hue_bound)
							{
								if(abs(Mean_hue_bunch-lower_bunch_hue_bound)>(NUM_HUES/2))
								{
								lower_bunch_hue_bound=Mean_hue_bunch;
								}
							}
						    }
ColoredIntervalsStructure->AverageHue[bunch]=Mean_hue_bunch;
ColoredIntervalsStructure->LowerHueValue[bunch]=lower_bunch_hue_bound;
ColoredIntervalsStructure->UpperHueValue[bunch]=upper_bunch_hue_bound;
						}//bh!

		 ColoredIntervalsStructure->AverageSat[bunch]=Mean_sat_bunch;
}////zd&hd
if((gray_zone_difference<=1)&&(gray_difference<=imp_dev_mean[bunch_zone_gray]))
{//grd//last_cor02.02.16
	Bunch_gray=(Bunch_gray*(float)bunch_significance+
	(float)(bunch_new_gray*bunch_new_significance))/
	(float)(bunch_significance+bunch_new_significance);
	Mean_gray_bunch=(int)Bunch_gray;
	if((Bunch_gray-Mean_gray_bunch)>0.5)
	{
	Mean_gray_bunch+=1;
	}
	if(bunch_gray!=Mean_gray_bunch)
						{

							if(Mean_gray_bunch>upper_bunch_gray_bound)
							{
								upper_bunch_gray_bound=Mean_gray_bunch;
							}
							if(Mean_gray_bunch<lower_bunch_gray_bound)
							{
								lower_bunch_gray_bound=Mean_gray_bunch;
							}
ColoredIntervalsStructure->AverageGray[bunch]=Mean_gray_bunch;
ColoredIntervalsStructure->LowerGrayscale[bunch]=lower_bunch_gray_bound;
ColoredIntervalsStructure->UpperGrayscale[bunch]=upper_bunch_gray_bound;
						}

}//grd
  }//sr
/*if(((!bunch_new_beg)&&(bunch_new_length<=4)&&(Density_ratio>=4))||((signif_ratio>=4)&&(Density_ratio>=4))
   ||(bunch_new_occurence>0)||(Density_ratio>=6)||((Intersection==2)&&(Density_ratio>=5))||((Density_ratio>=4)&&
   ((ratio_length1>=14)||((ratio_length>=15)&&(ratio_length1>=12))||((ratio_length>=12)&&(ratio_length1>=15))||
   (outlet_length_ratio1<=2))))
{//last_cor23.12.12*/
ColoredIntervalsStructure->BegInterv[bunch]=new_beg;
ColoredIntervalsStructure->EndInterv[bunch]=new_end;
ColoredIntervalsStructure->Significance[bunch]+=bunch_new_significance;
new_bunch_significance=ColoredIntervalsStructure->Significance[bunch];
new_bunch_density=(2*new_bunch_significance)/(new_end-new_beg+1);
new_bunch_occurence=(bunch_occurence*int_length+bunch_new_occurence*bunch_new_length)/union_length;
*(visible_bunches+bunch)=std::min(16,new_bunch_occurence);
if((bunch_beg!=new_beg)||(bunch_end!=new_end))
{
paint_beg=new_beg/4;
paint_end=(bunch_beg/4);
   if(paint_beg<paint_end)
   {
paint_end-=1;
Strip_value_correction_bunch(bunch,paint_beg,paint_end,new_bunch_density);
    }
paint_beg=bunch_end/4;
paint_end=(new_end/4);
   if(paint_beg<paint_end)
   {
paint_beg+=1;
Strip_value_correction_bunch(bunch,paint_beg,paint_end,new_bunch_density);
    }
}
/*}
else
{

	//if((ratio_length1>=14)||((ratio_length1>=12)&&(ratio_length<=1))
	//	||((ratio_length>=15)&&(ratio_length1>=12))||((ratio_length>=12)&&(ratio_length1>=15)))
	if((ratio_length1>=14)||((signif_ratio>=4)&&(Density_ratio>=4))
		||((ratio_length1>=12)&&(ratio_length<=1)))
	{//last_cor23.12.12
	ColoredIntervalsStructure->Significance[bunch]+=bunch_new_significance;
	}
}*/

return;
}
//----------------------------------------------------------------------------
void

CColorIntervalSelect::Extension(int bunch, int bunch_new)
{

	return;
}

//-----------------------------------------------------------------------------
int

CColorIntervalSelect::Test_Junction_Corrected(int bunch,int neighbor_bunch,int occurrence,
int Intersection,int indic_length,int ratio_length,int ratio_length1,
int *hue_close_r,int *sat_close_r,int* gray_close_r)
{
int bunch_mean_hue;
int corrected_bunch_mean_hue;
int neighbor_bunch_mean_hue;
int corrected_neighbor_bunch_mean_hue;
int neighbor_bunch_minimal_hue;
int neighbor_bunch_maximal_hue;
int bunch_mean_saturation;
int neighbor_bunch_mean_saturation;
int corrected_neighbor_bunch_minimal_hue;
int corrected_neighbor_bunch_maximal_hue;
int bunch_mean_gray;
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
int minimum_zone_gray;
int joint_hue_range;
int bunch_minimal_hue;
int bunch_maximal_hue;
int left_bunch_hue,right_bunch_hue;
int corrected_bunch_maximal_hue;
int min_hue_bunch_dist;
int Intersection_hue;
int coinsidence_hue;
int coinsidence1_hue;
int shift_to_left_hue;
int shift_to_right_hue;
int indic_length_hue;
int ratio_length_hue;
int ratio_length1_hue;
int corrected_bunch_minimal_hue;
int min_saturation;
int max_saturation;
int gray_hole;
int min_mean_gray;
int min_hue_zone;//last_cor17.04.15
int max_hue_zone;
int min_zone_gray_bunch;//last_cor27.04.15
int max_zone_gray_bunch;
int bunch_gray_zone_dif;
int min_zone_gray_neighbor;//last_cor27.04.15
int max_zone_gray_neighbor;
int neighbor_gray_zone_dif;
int bunch_hue_range;//last_cor28.04.15
int neighbor_hue_range;
int max_hue;//last_cor27.01.16
int min_hue;

min_mean_gray=-1;
Intersection_saturation=-1;
indic_length_satur=-1;
ratio_length_satur=-1;
ratio_length1_satur=-1;
Intersection_hue=-1;
inclusion_relation=0;
prior=0;
hue_close=0;
saturation_close=0;
grayscale_close=0;
wide_gray_gap=0;
Intersection_saturation=-1;
gray_hole=-1;

indic_length_satur=-1;
ratio_length_satur=-1;
ratio_length1_satur=-1;

inclusion_relation=0;
prior=0;
hue_close=0;
saturation_close=0;
grayscale_close=0;
wide_gray_gap=0;
Intersection_hue=-1;
corrected_bunch_mean_hue=-1;
corrected_neighbor_bunch_mean_hue=-1;
corrected_bunch_minimal_hue=-1;
corrected_bunch_maximal_hue=-1;
corrected_neighbor_bunch_minimal_hue=-1;
corrected_neighbor_bunch_maximal_hue=-1;
if(neighbor_bunch<0)
{//!!!error43imagelast_cor04.17
	neighbor_bunch=-neighbor_bunch;
}

beg_int=ColoredIntervalsStructure->BegInterv[bunch];
end_int=ColoredIntervalsStructure->EndInterv[bunch];
int_length=end_int-beg_int+1;
neighbor_beg_int=ColoredIntervalsStructure->BegInterv[neighbor_bunch];
neighbor_end_int=ColoredIntervalsStructure->EndInterv[neighbor_bunch];
neighbor_length=neighbor_end_int-neighbor_beg_int+1;
if((neighbor_beg_int<=beg_int)&&(neighbor_end_int>=end_int))
{
inclusion_relation=1;
}

/*Intersection=Intersection_measure_ratios(neighbor_beg_int,neighbor_end_int,beg_int,
	end_int,&indic_length,&ratio_length,&ratio_length1);
	//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
	//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
if(Intersection>1)
{
	goto L;
}*/
bunch_significance=ColoredIntervalsStructure->Significance[bunch];
neighbor_significance=ColoredIntervalsStructure->Significance[neighbor_bunch];
Signif_ratio=(16*bunch_significance)/(bunch_significance+neighbor_significance);
/*if(Signif_ratio>12)
{
	goto L;
}*/
bunch_density=2*bunch_significance/int_length;
neighbor_density=2*neighbor_significance/neighbor_length;
Density_ratio=(16*bunch_density)/(bunch_density+neighbor_density);
length_ratio=(16*int_length)/(int_length+neighbor_length);
/*if(Density_ratio>12)
{
	goto L;
}
if((Density_ratio>8)&&(length_ratio>10))
{
	goto L;
}*/
node_vicinity=0;
node_vicinity_neighbor=0;
bunch_occurrence=occurrence;
bunch_mean_hue=ColoredIntervalsStructure->AverageHue[bunch];
if(bunch_mean_hue>=NUM_HUES)
{
bunch_mean_hue-=NUM_HUES;
}
bunch_minimal_hue=ColoredIntervalsStructure->LowerHueValue[bunch];
if(bunch_minimal_hue>=NUM_HUES)
{
bunch_minimal_hue-=NUM_HUES;
}
if((bunch_minimal_hue<NUM_HUES/2)&&(bunch_minimal_hue>bunch_mean_hue))
{//last_cor21.05.15
	bunch_minimal_hue=bunch_mean_hue;
ColoredIntervalsStructure->LowerHueValue[bunch]=bunch_mean_hue;
}//last_cor21.05.15
bunch_maximal_hue=ColoredIntervalsStructure->UpperHueValue[bunch];
if(bunch_maximal_hue>=NUM_HUES)
{
bunch_maximal_hue-=NUM_HUES;
}
zone_hue_bunch=hue_zones[bunch_mean_hue];
neighbor_bunch_mean_hue=ColoredIntervalsStructure->AverageHue[neighbor_bunch];
if(neighbor_bunch_mean_hue>=NUM_HUES)
{
neighbor_bunch_mean_hue-=NUM_HUES;
}
zone_hue=hue_zones[neighbor_bunch_mean_hue];
max_hue=std::max(bunch_mean_hue,neighbor_bunch_mean_hue);
min_hue=std::min(bunch_mean_hue,neighbor_bunch_mean_hue);
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
neighbor_bunch_minimal_hue=ColoredIntervalsStructure->LowerHueValue[neighbor_bunch];
if(neighbor_bunch_minimal_hue>=NUM_HUES)
{
neighbor_bunch_minimal_hue-=NUM_HUES;
}
if((neighbor_bunch_minimal_hue<NUM_HUES/2)&&
   (neighbor_bunch_minimal_hue>neighbor_bunch_mean_hue))
{//last_cor21.05.15
	neighbor_bunch_minimal_hue=neighbor_bunch_mean_hue;
ColoredIntervalsStructure->LowerHueValue[neighbor_bunch]=neighbor_bunch_mean_hue;
}//last_cor21.05.15
neighbor_bunch_maximal_hue=ColoredIntervalsStructure->UpperHueValue[neighbor_bunch];
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
corrected_bunch_mean_hue=bunch_mean_hue;
corrected_neighbor_bunch_mean_hue=neighbor_bunch_mean_hue;
corrected_neighbor_bunch_minimal_hue=neighbor_bunch_minimal_hue;
corrected_neighbor_bunch_maximal_hue=neighbor_bunch_maximal_hue;
Intersection_hue=Measure2_of_proximity(left_bunch_hue,right_bunch_hue,left_hue,right_hue,&coinsidence_hue,
&coinsidence1_hue,&shift_to_left_hue,&shift_to_right_hue,&indic_length_hue,&ratio_length_hue,
&ratio_length1_hue);
if(Intersection_hue==3)
{
	coinsidence_hue=0;
}
bunch_hue_range=right_bunch_hue-left_bunch_hue;
neighbor_hue_range=right_hue-left_hue;

joint_hue_range=std::max(right_bunch_hue,right_hue)-std::min(left_bunch_hue,left_hue)+1;
if((hue_difference<=1)||((hue_difference<=2)&&((coinsidence1_hue>=18)
   ||(joint_hue_range<=6)&&(coinsidence1_hue>0))))
{
	hue_close=1;
}
else
{
	if(difference_hue_zone>=2)
	{
     hue_close=-2;
	}
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
bunch_mean_saturation=ColoredIntervalsStructure->AverageSat[bunch];
bunch_minimal_saturation=ColoredIntervalsStructure->LowerSatValue[bunch];
bunch_maximal_saturation=ColoredIntervalsStructure->UpperSatValue[bunch];
min_sat_dist=std::min(bunch_maximal_saturation-bunch_mean_saturation,bunch_mean_saturation-bunch_minimal_saturation);
left_sat_bunch=bunch_mean_saturation-min_sat_dist;
right_sat_bunch=min_sat_dist+bunch_mean_saturation;
neighbor_bunch_mean_saturation=
ColoredIntervalsStructure->AverageSat[neighbor_bunch];
saturation_difference=abs(neighbor_bunch_mean_saturation-bunch_mean_saturation);
min_saturation=std::min(bunch_mean_saturation,neighbor_bunch_mean_saturation);
//!?
max_saturation=std::max(bunch_mean_saturation,neighbor_bunch_mean_saturation);
neighbor_bunch_minimal_saturation=ColoredIntervalsStructure->LowerSatValue[neighbor_bunch];
neighbor_bunch_maximal_saturation=ColoredIntervalsStructure->UpperSatValue[neighbor_bunch];
min_sat_dist=std::min(neighbor_bunch_maximal_saturation-neighbor_bunch_mean_saturation,
				 neighbor_bunch_mean_saturation-neighbor_bunch_minimal_saturation);
left_sat=neighbor_bunch_mean_saturation-min_sat_dist;
right_sat=min_sat_dist+neighbor_bunch_mean_saturation;
abs_satur=abs(bunch_mean_saturation-left_sat);
abs_satur1=abs(bunch_mean_saturation-right_sat);
abs_satur=std::min(abs_satur,abs_satur1);
possible_sat_variation=dev_mean_sat[neighbor_bunch_mean_saturation];//last_cor05.07.13
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
		imp_sat_var=std::max(impossible_sat_variation1,imp_sat_var);//last_cor08.12.15
	}
	else
	{
	saturation_distance1=bunch_mean_saturation-neighbor_bunch_mean_saturation;
	saturation_distance=0;
	poss_sat_variation=dev_mean_sat1[bunch_mean_saturation];
		poss_sat_variation=std::max(poss_sat_variation,possible_sat_variation);
	imp_sat_var=imp_dev_mean_sat1[bunch_mean_saturation];
		imp_sat_var=std::max(imp_sat_var,impossible_sat_variation);//last_cor14.12.15
	}
max_satur_differ=std::max(saturation_distance,saturation_distance1);

    Intersection_saturation=Intersection_measure_ratios(left_sat,right_sat,left_sat_bunch,
	right_sat_bunch,&indic_length_satur,&ratio_length_satur,&ratio_length1_satur);
	if(Intersection_saturation==3)
	{//last_cor08.12.15
		if(ratio_length_satur>2*imp_sat_var)
		{
			saturation_close=-2;
		}
		else
		{
		  if(ratio_length_satur>imp_sat_var)
		  {
          saturation_close=-1;
		  }
		}
	}//last_cor08.12.15
	if(!saturation_close)
	{//last_cor08.12.15
		if(saturation_difference>2*imp_sat_var)
		{
			saturation_close=-2;
		}
	}//last_cor08.12.15
	if(Intersection_saturation==3)
	{
		ratio_length_satur=0;//???
	}
	if(saturation_difference<=1)
	{//last_cor16.12.15
		saturation_close=1;
	}//last_cor16.12.15
bunch_mean_gray=ColoredIntervalsStructure->AverageGray[bunch];
bunch_minimal_gray=ColoredIntervalsStructure->LowerGrayscale[bunch];
bunch_maximal_gray=ColoredIntervalsStructure->UpperGrayscale[bunch];
if(bunch_mean_gray<bunch_minimal_gray)
{
	if(abs((bunch_minimal_gray-bunch_mean_gray)==1))
	{
		bunch_mean_gray=bunch_minimal_gray;
        ColoredIntervalsStructure->AverageGray[bunch]=bunch_mean_gray;
	}
}
if(bunch_maximal_gray<bunch_mean_gray)
{
bunch_mean_gray=bunch_maximal_gray;
}
if(bunch_minimal_gray>bunch_mean_gray)
{
	bunch_mean_gray=bunch_minimal_gray;
}
min_bunch_gray_dist=std::min(bunch_maximal_gray-bunch_mean_gray,bunch_mean_gray-bunch_minimal_gray);
left_bunch_gray=bunch_mean_gray-min_bunch_gray_dist;
right_bunch_gray=min_bunch_gray_dist+bunch_mean_gray;
min_zone_gray_bunch=gray_zones[left_bunch_gray];
max_zone_gray_bunch=gray_zones[right_bunch_gray];
bunch_gray_zone_dif=max_zone_gray_bunch-min_zone_gray_bunch;
bunch_gray_length=2*min_bunch_gray_dist+1;
zone_gray_bunch=gray_zones[bunch_mean_gray];
neighbor_bunch_mean_gray=
ColoredIntervalsStructure->AverageGray[neighbor_bunch];
min_mean_gray=std::min(bunch_mean_gray,neighbor_bunch_mean_gray);
neighbor_minimum_gray=ColoredIntervalsStructure->LowerGrayscale[neighbor_bunch];
neighbor_maximum_gray=ColoredIntervalsStructure->UpperGrayscale[neighbor_bunch];
/*if(neighbor_bunch_mean_gray<neighbor_minimum_gray)
{//error
	if(abs((neighbor_minimum_gray-neighbor_bunch_mean_gray)==1))
	{
		neighbor_bunch_mean_gray=neighbor_minimum_gray;
        ColoredIntervalsStructure->AverageGray[neighbor_bunch]=neighbor_bunch_mean_gray;
	}
}*/
if(neighbor_maximum_gray<neighbor_bunch_mean_gray)
{//last_cor28.07.15
neighbor_maximum_gray=neighbor_bunch_mean_gray;
ColoredIntervalsStructure->UpperGrayscale[neighbor_bunch]=neighbor_bunch_mean_gray;
}
if(neighbor_minimum_gray>neighbor_bunch_mean_gray)
{//last_cor28.07.15
neighbor_minimum_gray=neighbor_bunch_mean_gray;
ColoredIntervalsStructure->LowerGrayscale[neighbor_bunch]=neighbor_bunch_mean_gray;
}
min_gray_dist=std::min(neighbor_maximum_gray-neighbor_bunch_mean_gray,neighbor_bunch_mean_gray-neighbor_minimum_gray);
left_gray=neighbor_bunch_mean_gray-min_gray_dist;
right_gray=min_gray_dist+neighbor_bunch_mean_gray;
min_zone_gray_neighbor=gray_zones[left_gray];
max_zone_gray_neighbor=gray_zones[right_gray];
neighbor_gray_zone_dif=max_zone_gray_neighbor-min_zone_gray_neighbor;
union_gray_left=std::min(left_bunch_gray,left_gray);
union_gray_right=std::max(right_bunch_gray,right_gray);
union_gray_length=union_gray_right-union_gray_left;//last_cor11.03.15
zone_gray=gray_zones[neighbor_bunch_mean_gray];
neighbor_gray_length=2*min_gray_dist+1;
difference_gray_zone=abs(zone_gray_bunch-zone_gray);
possible_gray_variation=std::min(dev_mean[neighbor_bunch_mean_gray/8],dev_mean[bunch_mean_gray/8]);
/*if(min_saturation>=3)
{
possible_gray_variation=possible_gray_variation+2;
}*/
impossible_gray_variation=std::min(imp_dev_mean[neighbor_bunch_mean_gray/8],
imp_dev_mean[bunch_mean_gray/8]);//last_cor5.07.13
maximum_zone_gray=std::max(zone_gray,zone_gray_bunch);
minimum_zone_gray=std::min(zone_gray,zone_gray_bunch);
gray_difference=abs(bunch_mean_gray-neighbor_bunch_mean_gray);
gray_ntersection=Intersection_measure_ratios(left_gray,right_gray,left_bunch_gray,
	right_bunch_gray,&indic_length_gray,&ratio_length_gray,&ratio_length1_gray);
gray_ntersection1=Intersection_measure_ratios(left_gray,right_gray,left_bunch_gray,
	bunch_mean_gray,&indic_length_gray1,&ratio_length_gray1,&ratio_length1_gray1);
gray_ntersection2=Intersection_measure_ratios(left_gray,right_gray,bunch_mean_gray,
	right_bunch_gray,&indic_length_gray2,&ratio_length_gray2,&ratio_length1_gray2);
//}
/*if((union_gray_length>
 (possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))&&
 (gray_ntersection>=2)&&(gray_ntersection!=3))
	   {//last_cor20.04.15
		   if(std::min(ratio_length_gray,ratio_length1_gray)<4)
		   {
			   if(difference_gray_zone<=1)
		    {
				     if(minimum_zone_gray>=2)
		             {
				if(gray_difference>=possible_gray_variation)
				{
                   grayscale_close=-2;
				}
				else
				{
					if(gray_difference>possible_gray_variation/2)
				     {
						 grayscale_close=-1;
					 }
				}
					 }
			}
		   }
	   }//last_cor14.04.15*/
if(gray_ntersection==3)
{//intersection is empty
	gray_hole=std::max(left_bunch_gray,left_gray) - std::min(right_gray,right_bunch_gray);
	     if(union_gray_length<=possible_gray_variation/3)
	     {//last_cor29.04.15
		   grayscale_close=1;
	     }
	     else
	     {//last_cor29.04.15
	if(union_gray_length>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
	{
	if(((ratio_length_gray+1)>=bunch_gray_length)||((ratio_length_gray+1)>=neighbor_gray_length))
	{
	wide_gray_gap=1;
	}
    }
	if(ratio_length_gray>=2)
	{//rlg2
		if(ratio_length_gray>=4)
				{//last_cor15.11.17
		grayscale_close=-3;
				}
		if(difference_gray_zone>=2)
		{
			grayscale_close=-3;
		}
		else
		{//e
			if(difference_gray_zone<=1)
		    {
				if(minimum_zone_gray<=1)
				{//last_cor27.04.16mzg<=1
					if(maximum_zone_gray>=2)
					{//mzg>=2
			if(union_gray_length>impossible_gray_variation)
			{//ugl
if((gray_difference>possible_gray_variation)&&(gray_hole>=2))
{
	if(!grayscale_close)
	{
	grayscale_close=-2;
	}
}
			 }//ugl
					}//mzg>=2
    if(maximum_zone_gray>=1)
     {//mzg>
if(!minimum_zone_gray)
{//mzg0
	if((gray_difference>possible_gray_variation)&&(gray_hole>=possible_gray_variation))
     {
	    if(!grayscale_close)
	     {
	     grayscale_close=-2;
	     }
     }
 }//mzg0
      }//mzg>

				}//last_cor27.04.16mzg<=1
				     if(minimum_zone_gray>=2)
		             {
if(union_gray_length>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
{//last_cor14.04.15&07.05.1501.12.15
				if(gray_difference>possible_gray_variation)
				{
					if(zone_hue!=3)
					{//last_cor01.12.15
	if(!grayscale_close)
	{
                   grayscale_close=-1;
	}
					}
				}
				/*else
				{
					if(gray_difference>possible_gray_variation/2)
				     {
						 grayscale_close=-1;
					 }
				}*/
}//last_cor14.04.15&07.05.15
					 }
			}
		}//e
	}//rlg2
		 }//last_cor29.04.15
}//intersection is empty
if(length_ratio>10)
{//lr10last_cor27.04.15
	if(Density_ratio>8)
	{//dr>8
if((bunch_gray_length<=possible_gray_variation)&&
   (neighbor_gray_length>impossible_gray_variation))
{//b<posN>imp
		if((bunch_gray_zone_dif<=1)&&(neighbor_gray_zone_dif>=bunch_gray_zone_dif))
		{//bgzd1
if((2*bunch_gray_length<neighbor_gray_length)||(neighbor_gray_zone_dif>=2))
{
	if(2*gray_difference>3*possible_gray_variation)
	{//last_cor30.07.15error!
		if(!grayscale_close)
		{
			if(difference_gray_zone>=2)
		    {
			grayscale_close=-3;
		    }
			else
			{
				if(gray_difference>impossible_gray_variation)
				{
					grayscale_close=-2;
				}
				/*else
				{
	grayscale_close=-1;
				}*/
			}
		}
	}
}
	     }//bgzd1
}//b<posN>imp
	}//dr>8
}//lr10last_cor27.04.15
if(length_ratio<7)
{//nlr10last_cor27.04.15
	if(Density_ratio<8)
	{//dr>8
if((bunch_gray_length>impossible_gray_variation)&&
   (neighbor_gray_length<=possible_gray_variation))
{//b<posN>imp
		if((neighbor_gray_zone_dif<=1)&&(neighbor_gray_zone_dif<=bunch_gray_zone_dif))
		{//bgzd1
if((bunch_gray_length>2*neighbor_gray_length)||(bunch_gray_zone_dif>=2))
{
	if(3*gray_difference>2*possible_gray_variation)
	{
		if(!grayscale_close)
		{
			if(difference_gray_zone>=2)
		    {
			grayscale_close=-3;
		    }
			else
			{
	if(gray_difference>impossible_gray_variation)
				{
					grayscale_close=-2;
				}
				else
				{//last_cor27.01.16
if(gray_difference>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
{
	grayscale_close=-1;
}
				}
			}
		}
	}
}
	     }//bgzd1
}//b<posN>imp
	}//dr>8
}//nlr10last_cor27.04.15
if(Intersection_hue==3)
{//last_cor10.03.15
	if(min_hue_zone==3)
	{
		if((min_hue<16)&&(max_hue>=20))
		{
			if(max_saturation>=4)
			{
				if(grayscale_close<=0)
				{
					if((maximum_zone_gray>=3)&&(difference_gray_zone>=1))
					{
					hue_close=-1;
					if((max_hue_zone>3)||(saturation_close<=0)||(grayscale_close<0))
					{
					prior=5;
					}
					else
					{
                    prior=6;
					}
					goto L;
					}
				}
			}
		}
	}
	if(((min_saturation>=1)&&(max_saturation>=2))||((max_saturation>=1)&&(minimum_zone_gray>=3)))
	{//last_cor11.03.1528.04.15
		if(minimum_zone_gray>=2)
		{//mzg

			if(ratio_length_hue>3)
			{//rlhlast_cor20.04.15
				if(max_saturation>5)
				{//ms5
				        if(hue_difference>=6)
						{
				        hue_close=-1;
						}
						else
						{
							if(hue_difference>=4)
				            {
if((!bunch_hue_range)||(!neighbor_hue_range)||
   (4*bunch_hue_range<=joint_hue_range)||(4*neighbor_hue_range<=joint_hue_range))
{
hue_close=-1;
}
							}
						}
				}//ms5
				else
				{//ems
					if(saturation_difference>=1)
								{//last_cor28.04.15
									if(minimum_zone_gray>=6)
									{
										if(ratio_length_hue>=5)
			                           {
                       if(hue_difference>=6)
		               {
						   if(min_saturation>=2)
						   {//last_cor11.09.15
		               hue_close=-1;
						   }
		               }
										}
									}
								}//last_cor28.04.15
         if((gray_ntersection==3)&&(2*gray_difference>3*possible_gray_variation))
		 {//last_cor11.09.15
          if(hue_difference>=7)
				{
			if(min_saturation>=2)
						   {//last_cor11.09.15
				hue_close=-1;
			}
				}
		 }
		 else
		 {
if(minimum_zone_gray>=3)
{//mzg3
	if(hue_difference>=10)
				{
					if(min_saturation>1)
					{//last_cor08.06.15and25.11.15
				hue_close=-1;
					}
				}
	else
	{//last_cor24.04.15
if(node_vicinity_neighbor!=node_vicinity)
{//rlhlast_cor24.04.15
	     if(ratio_length_hue>=3)
		 {
	if(hue_difference>=7)
	{
		if(min_saturation>=1)
		{
			if(minimum_zone_gray<6)
			{
		hue_close=-1;
			}
		}
	}
		 }
}
	}//last_cor24.04.15
}//mzg3
		 }
				}//ems
				//}

			}//rlhlast_cor20.04.15
			if(ratio_length_hue>=2)
			{
				if((joint_hue_range>=9)&&(hue_difference>=6)&&(max_saturation>5))
				{
					if(min_saturation>=1)
					{
					hue_close=-1;
					}
				}


			}
                   if(minimum_zone_gray>=3)
			       {//last_cor17.04.15
				if((min_hue_zone==2)&&(max_hue_zone==3))
				{
                   if(ratio_length_hue>2)
			       {//last_cor17.04.15
					   if(min_saturation>2)
					   {
				if(hue_difference>=3)
				{
				hue_close=-1;
				}
					   }
					   else
					   {
                      if(min_saturation>1)
					   {
				           if(hue_difference>=4)
				          {
				           hue_close=-1;
				          }
					   }
					   }
			      }
				}
			      }//last_cor17.04.15
		}//mzg
		else
		{//last_cor16.03.15

if(gray_difference>possible_gray_variation/2)
	{//grcondlast_cor28.04.15
			if(min_saturation>=6)
			{//ms6
	if(ratio_length_hue>2)
	{//last_cor28.04.15
		if(hue_difference>=6)
		{
	hue_close=-1;
		}
	}//last_cor28.04.15

			}//ms6
			else
			{//last_cor28.04.15

				if(min_saturation>=4)
			    {//ms4
if(ratio_length_hue>3)
{//last_cor28.04.15
if((!bunch_hue_range)||(!neighbor_hue_range)||
   (4*bunch_hue_range<=joint_hue_range)||(4*neighbor_hue_range<=joint_hue_range))
{
	if(hue_difference>=4)
	{
hue_close=-1;
	}
}
}
				}//ms4

			}//last_cor28.04.15
}//last_cor28.04.15
		}//last_cor16.03.15
	}//grcondlast_cor28.04.15
}//last_cor10.03.15

/*if(union_gray_length<=possible_gray_variation)
{//last_cor19.06.14
	gray_ntersection=0;
	gray_ntersection1=0;
    gray_ntersection2=0;
}
else
{*/


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

}

hue_difference=
abs(corrected_bunch_mean_hue-corrected_neighbor_bunch_mean_hue);
hue_difference=std::min(hue_difference,NUM_HUES-hue_difference);
left_hue=corrected_neighbor_bunch_mean_hue-min_hue_dist;
right_hue=corrected_neighbor_bunch_mean_hue+min_hue_dist;
left_bunch_hue=corrected_bunch_mean_hue-min_hue_bunch_dist;
right_bunch_hue=corrected_bunch_mean_hue+min_hue_bunch_dist;
joint_hue_range=std::max(right_bunch_hue,right_hue)-std::min(left_bunch_hue,left_hue);
if((hue_difference<=1)||((hue_difference<=2)&&(joint_hue_range<=6)))
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
//gray_difference=abs(bunch_mean_gray-neighbor_bunch_mean_gray);

                             if((gray_ntersection==3)&&(!grayscale_close))
							 {//last_cor27.04.15
						 if((zone_gray>=3)||(zone_gray_bunch>=3))
						 {//zg3
								 if(3*gray_difference>2*impossible_gray_variation)
								 {//last_cor03.09.14and03.03.15
									 if(gray_difference>=impossible_gray_variation)
									 {
									 grayscale_close=-2;
									 }
									 else
									 {
	 if(gray_difference>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
									     {
										 grayscale_close=-1;
										 }
									 }
								 }//last_cor03.09.14and03.03.15
								 if(gray_hole>(min_mean_gray/4))
								 {//last_cor03.03.15
									 if(!grayscale_close)
									 {
                                 grayscale_close=-1;
									 }
								 }//last_cor03.03.15
								 else
								 {//last_cor04.03.15
				                  if(difference_gray_zone>=2)
				                  {
                                     if(!grayscale_close)
                                     {
                                     grayscale_close=-2;
                                     }
								 }
								  else
								  {//edgz2
              if((difference_gray_zone==1)||(gray_difference>=min_mean_gray/4))
				{//dgz1last_cor04.03.15
					if((gray_hole>=(min_mean_gray/5))&&(gray_difference>=min_mean_gray/3))
					{//last_cor04.03.15
                                 if(!grayscale_close)
									 {
	 if(gray_difference>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
										 {
                                 grayscale_close=-1;
										 }
									 }
					}
					else
					{//last_cor04.03.15
if(union_gray_length>=
   possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/4)
					{
						if(gray_hole>=1)
						{
if((gray_difference>=min_mean_gray/4)&&
   (gray_difference>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2)))
{
	                                 if(!grayscale_close)
									 {
                                 grayscale_close=-1;
									 }
}

						}
					}
					}//last_cor04.03.15
				}//dgz1last_cor04.03.15
								  }//edgz2
								 }//last_cor04.03.15
						 }//zg3
						 else
						 {//last_cor13.06.14
if((zone_gray>=2)&&(zone_gray_bunch>=2))
						      {//zg2last_cor21.04.15
if((3*gray_difference>2*possible_gray_variation)&&(difference_gray_zone>=1)&&(ratio_length_gray>=2))
								 {//3gd2pv
									 if(gray_difference>=impossible_gray_variation)
									 {
									 grayscale_close=-2;
									 }
									 else
									 {
			 if(union_gray_length>=impossible_gray_variation)
										 {//last_cor20.06.14
							 grayscale_close=-2;
										 }
			 else
			 {//last_cor10.03.15
if((difference_gray_zone==1)&&(gray_difference>=min_mean_gray/3))
				{//dgz1last_cor10.03.15
					if((gray_hole>=1)&&(gray_difference>=min_mean_gray/2))
					{//last_cor04.03.15
                                 if(!grayscale_close)
									 {
                                 grayscale_close=-1;
									 }
					}
                }//dgz1last_cor10.03.15
			 }//last_cor10.03.15
									 }
								 }//3gd2pv
						      }//zg2
						 }
							 }
if(!grayscale_close)
{//last_cor28.04.15
	if((union_gray_length>impossible_gray_variation)&&
		(gray_difference<impossible_gray_variation))
						{//last_cor28.04.15
if(difference_gray_zone>=2)
{
	if(gray_difference>impossible_gray_variation)
	{
		grayscale_close=-1;
	}
}
						}//last_cor28.04.15

}//last_cor28.04.15
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
			if(!grayscale_close)
					{//last_cor14.04.15

			if(union_gray_length<=possible_gray_variation)
			{//last_cor11.03.15
				if(3*gray_difference<=2*possible_gray_variation)
				{//last_cor11.03.15

				grayscale_close=1;

				}
				else
				{
					if(gray_difference<=possible_gray_variation)
					{
						grayscale_close=2;
					}
				}
			}//last_cor11.03.15
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
	//if((gray_difference<=possible_gray_variation)&&(bunch_mean_gray>=left_gray)&&(bunch_mean_gray<=right_gray))
		    {//gr3
				if(3*gray_difference<=(2*possible_gray_variation))
				{//last_cor05.11.14
		    grayscale_close=1;
				}
				else
				{
if((((gray_ntersection<=1)&&(union_gray_length<=2*possible_gray_variation))
   ||(union_gray_length<=possible_gray_variation))&&
   (gray_difference<=possible_gray_variation))
					{
						grayscale_close=2;
					}
					else
					{
if((gray_ntersection==1)&&(gray_difference<=possible_gray_variation)
   &&(union_gray_length<=2*possible_gray_variation))
{
grayscale_close=2;
}
else
{
	if((!difference_gray_zone)&&(gray_ntersection<3)&&
			(ratio_length_gray>=5)&&(ratio_length1_gray>=5))
		{//last_cor05.03.18
	grayscale_close=3;
		}
}
					}
				}
			}//gr3
	       else
	       {
			   if(maximum_zone_gray<=1)
			   {//last_cor05.11.14
				   if(3*gray_difference<=(2*possible_gray_variation))
				   {
                   grayscale_close=1;
				   }
				   else
				   {
                   if(gray_difference<=possible_gray_variation)
				   {
                   grayscale_close=2;
				   }
				   }
			   }
			   if((maximum_zone_gray<=2)||((maximum_zone_gray<=3)&&(minimum_zone_gray<=2)))
			   {//mz
				   if((gray_difference<=possible_gray_variation)&&(gray_ntersection<=1)&&
					   ((!gray_ntersection1)||(!gray_ntersection2)))
				   {//last_cor
		   if(3*gray_difference<=(2*possible_gray_variation))
				    {//gd<last_cor05.11.14
					   if(wide_gray_gap)
					   {//last_cor04.09.13
						grayscale_close=3;
					   }
					   else
					   {//last_cor04.09.13
                   grayscale_close=2;
					   }
					   }//gd<
				   }//last_cor
				   else
				   {//e2
					   if(3*gray_difference<=(2*possible_gray_variation))
				       {//last_cor05.11.14
					   grayscale_close=2;
					   }
					   else
					   {
				   if((gray_difference<=possible_gray_variation)&&(gray_ntersection<=1)&&
					   ((!gray_ntersection1)||(!gray_ntersection2)))
				   {//1
                   if((gray_difference<=(2*possible_gray_variation)/3))
				    {
		            grayscale_close=2;
				    }
				    else
				    {
				    grayscale_close=3;
				    }
				   }//1
					   }
				   /*else
				   {
	if((gray_difference<=possible_gray_variation)||((bunch_mean_gray>=left_gray)&&(bunch_mean_gray<=right_gray)))
	{
		grayscale_close=3;
	}
				   }*/
				   }//e2
			   }//mzlast_cor
			   else
			   {//e
				   if(gray_difference<=possible_gray_variation)
			       {//gdpv
					   if(2*gray_difference<=possible_gray_variation)
					   {//last_cor11.04.14
                      grayscale_close=1;
					   }//last_cor11.04.14
					   else
					   {//egc1
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
					   {//e!!!
 if((gray_difference<possible_gray_variation)&&(gray_ntersection<=1))
						   {//last_cor27.05.13
							   grayscale_close=2;
						   }
						   else
						   {//e1
							   if(4*gray_difference<3*possible_gray_variation)
							   {//last_cor26.03.14
								   if(!grayscale_close)
								   {//last_cor034.03.15
								   grayscale_close=2;
								   }
							   }//last_cor26.03.14
							   else
							   {//e
if((gray_ntersection!=3)||
(union_gray_length<(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2)))
								   {//last_cor02.04.14
	 if(gray_difference<=3*(possible_gray_variation/4))
	 {//last_cor11.06.15
      grayscale_close=2;
	 }
	 else
	 {
		 if((ratio_length1_gray>=8)&&(ratio_length_gray>=8))
		 		 {//last_cor05.03.18
		 	 grayscale_close=3;
		 		 }
	 }//last_cor11.06.15
								   }
							   }//e
						   }//e1
					   }//e!!!
					   }//egc1
			       }//gdpv
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
}//last_cor14.04.15
		}//egray
if(grayscale_close==0)
{
	if(gray_difference<=possible_gray_variation)//?!
			       {
					   if(2*gray_difference<=possible_gray_variation)
					   {
						   grayscale_close=1;
					   }
					   else
					   {//e2g
					   if(3*gray_difference<=2*possible_gray_variation)
					   {//!!!
   if((gray_ntersection<=2)||(union_gray_length<=possible_gray_variation))
						   {//last_cor20.06.14
						grayscale_close=2;
						   }
					   }
					   else
					   {//egc
						   if(minimum_zone_gray<=1)
						   {
 if(((gray_ntersection<=2)&&((ratio_length1_gray>=8)||(ratio_length_gray>=8)))
	 ||(union_gray_length<=possible_gray_variation))
						      {//last_cor20.06.14
							   grayscale_close=2;
							   }
						   }//last_cor07.05.15
						   else
						   {//last_cor19.05.15
					if(minimum_zone_gray>=5)
					{
						if((zone_hue==4)&&(!difference_hue_zone))
						{
				   grayscale_close=2;
						}
					}

						   }//last_cor19.05.15
					   }//egc
					   }//e2g
	                }//?!
if(gray_difference>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
{
	if((bunch_mean_gray<left_gray)||(bunch_mean_gray>right_gray)||((ratio_length_gray<3)&&
		(ratio_length1_gray<3)))
	{
		grayscale_close=-2;
	}
	else
	{
		grayscale_close=-1;
	}

}
else
{
if(gray_difference<(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/4))
{
gray_ntersection3=Intersection_measure_ratios(bunch_minimal_gray,bunch_maximal_gray,
neighbor_minimum_gray,neighbor_maximum_gray,&indic_length_gray3,&ratio_length_gray3,
&ratio_length1_gray3);
if(gray_ntersection3==3)
{
	ratio_length_gray3=0;
}
if(gray_difference<possible_gray_variation)
{
	if(gray_ntersection3==1)
	{
		grayscale_close=2;
	}
}
    if((ratio_length_gray3>=10)&&(ratio_length1_gray3>=10))
	{
		if((ratio_length_gray>8)||(ratio_length1_gray>8))
		{//last_cor10.06.14
    grayscale_close=4;
		}
	}
}
else
{
	if(gray_difference>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
	{//last_cor03.06.13
		if((gray_ntersection>=2)&&((ratio_length1_gray<=2)||(ratio_length_gray<=2)))
		{//last_cor09.06.14
			grayscale_close=-1;
		}
	}
	else
	{//last_cor09.06.14
if(gray_difference>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
		{//last_cor27.01.16
if((gray_ntersection==3)&&(ratio_length_gray<=3)&&(ratio_length1_gray<=3))
{
grayscale_close=-2;
}
		}//last_cor27.01.16
	}
}
}

}
if(!grayscale_close)
{//last_cor23.05.14

	if(gray_difference>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
	{//g>p
		if((gray_ntersection==3)||
			((gray_ntersection==2)&&(ratio_length_gray<4)&&(ratio_length1_gray<4)))
		{//last_cor17.06.14
			if(((difference_gray_zone>=1)&&(wide_gray_gap))||(difference_gray_zone>=2))
			{
				grayscale_close=-1;
			}
		}

    }
	  else//last_cor17.06.14
	   {
        /*if(2*gray_difference>possible_gray_variation)
	    {//2g>p
if((gray_ntersection==3)||
			((gray_ntersection==2)&&(ratio_length_gray<4)&&(ratio_length1_gray<4)))
{
	if(maximum_zone_gray>=2)
	{
		grayscale_close=-5;
	}
	else//last_cor17.0614
	{
if((gray_ntersection==3)&&(ratio_length_gray>0)&&(4*gray_difference>3*possible_gray_variation))
{
	grayscale_close=-5;
}
	}//last_cor17.0614
}
	   }//2g>p*/
    }//g>p
}//last_cor23.05.14
if(!grayscale_close)
{//last_cor22.01.15
	if(!difference_gray_zone)
	{
		if(gray_difference<=possible_gray_variation)
		{
			if((gray_ntersection<3)&&((ratio_length_gray>=8)||(ratio_length1_gray>=8)))
			{
			grayscale_close=2;
			}
		}
	}
}
if((hue_close<0)&&(saturation_close<0)&&(grayscale_close!=0))
{//last_cor08.12.15
	prior=5;
	goto L;
}
if((hue_close<0)&&(grayscale_close<0))
{//last_cor08.12.15
	prior=5;
	goto L;
}
if((bunch_mean_saturation==0)&&(neighbor_bunch_mean_saturation==0))
{//s==0
	saturation_close=1;
	if(!grayscale_close)
		{//last_cor26.02.18
			if((ratio_length1_gray<=4)||(ratio_length_gray<=4))
			{
				if(gray_difference>=(possible_gray_variation+
					(impossible_gray_variation-possible_gray_variation)/3))
				{
					grayscale_close=-2;
					prior=5;
					goto L;
				}
			}
		}
		if(grayscale_close==1)
		{//last_cor13.02.18
		if(gray_difference<=possible_gray_variation/3)
		{//last_cor13.02.18
	if(!difference_hue_zone)
				{
					hue_close=1;
					prior=1;
					goto L;
				}
	else
	{
		if(difference_hue_zone<=1)
		{
		hue_close=2;
					prior=1;
					goto L;
		}
		else
		{
			hue_close=0;
					prior=2;
					goto L;
		}
	}
		}
		}//last_cor13.02.18
		if((grayscale_close>=1)&&(grayscale_close<=2))
			{//gc>=
				if(gray_difference<=(possible_gray_variation/2+1))
				{//last_cor18.10.17
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
							hue_close=2;
							prior=1;
							goto L;
						}
					}
					else
					{
						if(difference_hue_zone<=1)
						{
							if (hue_difference <= 3)
								{//last_cor24.11.17
									hue_close = 3;
									prior = 2;
									goto L;
								}//last_cor24.11.17
								else
								{
									if (hue_difference >= 10)
									{//last_cor24.11.17
										hue_close = -1;
										prior = 6;
										goto L;
									}
								}
						}
					}
				}
			}//gc>=
	if(hue_close>0)
	{
		if(!grayscale_close)
			{
				if(gray_difference<possible_gray_variation)
				{
					grayscale_close=2;
					prior=2;
					goto L;
				}
				if((gray_difference<=possible_gray_variation)&&
					((gray_ntersection<3)||(ratio_length_gray<=1)))
				{//last_cor18.10.17
					prior=2;
					grayscale_close=2;
					goto L;
				}
			}
	}
	if(grayscale_close==1)
		{//last_cor24.08.15
			if(minimum_zone_gray>=7)
			{
				if((min_hue_zone>=3)&&(max_hue_zone<=4))
				{
					if(hue_difference<=6)
					{
						hue_close=1;
						prior=1;
						goto L;
					}
					else
					{
						if(hue_difference<=9)
					    {
						hue_close=2;
						prior=2;
						goto L;
					    }
						else
						{
							if(hue_difference<=12)
							{//last_cor29.08.15
						    hue_close=3;
						    prior=2;
						    goto L;
							}
						}
					}
				}
			}
		}//last_cor24.08.15
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
if((gray_ntersection>=0)&&(gray_ntersection<=1))
			{//last_cor30.09.15
				if(grayscale_close==1)
				{
					if(gray_difference<=2)
                     {

					 if(!difference_hue_zone)
					 {
                     hue_close=1;
					 }
					 else
					 {
						 hue_close=0;
					 }
					 prior=1;
	                 goto L;
                     }
				}
			}//last_cor30.09.15
			if(maximum_zone_gray==7)
			{//last_cor04.06.15
if(gray_difference<=2)
{
	if(!difference_hue_zone)
					 {
                     hue_close=1;
					 }
					 else
					 {
					 hue_close=0;
					 }
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
	if((grayscale_close==1)&&(gray_difference<=(possible_gray_variation/2)))
	{//!!!!
	prior=1;
	goto L;
	}
           if((grayscale_close>0)&&(grayscale_close<=3)&&(difference_hue_zone<=1))
                   {//grayscale and hue_zones restrictions
	if(difference_hue_zone==0)
	{//the same zone
		if(zone_gray<=3)
		{
	prior=1;
		}
		else
		{
			prior=2;
		}
	goto L;
	}//the same zone
	if(zone_gray<=2)
	{
		if(!difference_gray_zone)
		{
	    prior=1;
		}
		else
		{
		prior=2;
		}
	//goto L;
	}
	else
	   {//zgr>2
	if((ratio_length1==16)&&(Signif_ratio<=2)&&(Density_ratio<4)&&(length_ratio<2))
	   {//small
		prior=1;
		//goto L;
	   }//small
	   else
	     {//not very small
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
	if((grayscale_close<=3)&&(zone_gray<=4)&&(gray_difference<=possible_gray_variation))
	{
	prior=2;
	//goto L;
	}
	}
	    }//not very small
	    }//zgr>2
                   }//grayscale and hue_zones restrictions
if((prior>0)&&(prior<=4))
	 {
       if((ratio_length1<5)&&((Signif_ratio>6)||(Density_ratio>6)))
	   {//incl
		prior+=10;
	   }//incl
	    goto L;
	 }
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
	saturation_close=1;//last_cor28.11.14
	if(grayscale_close==1)
		{//last_cor13.02.18
		if(gray_difference<=possible_gray_variation/3)
		{//last_cor13.02.18
	if(!difference_hue_zone)
				{
					hue_close=1;
					prior=1;
					goto L;
				}
		}
		}//last_cor13.02.18
if(hue_close>0)
	{
	if(!grayscale_close)
	{
		if(gray_difference<possible_gray_variation)
				{
					grayscale_close=2;
					prior=2;
					goto L;
				}
				if((gray_difference<=possible_gray_variation)&&
					((gray_ntersection<3)||(ratio_length_gray<=1)))
				{//last_cor18.10.17
					grayscale_close=2;
					prior=2;
					goto L;
				}
	}
	}
	if(!saturation_difference)
	{//last_cor25.11.15
		if((min_saturation==1)&&(zone_gray>=3)&&(zone_gray_bunch>=3))
		{//last_cor14.12.16
			if(!hue_close)
			{
            if(((bunch_mean_hue>=19)&&(neighbor_bunch_mean_hue<=13))||
				((neighbor_bunch_mean_hue>=19)&&(bunch_mean_hue<=13)))
			{
				if(grayscale_close>0)
				{
					prior=7;
					hue_close=-2;
					goto L;
				}
				else
				{
                prior=6;
				hue_close=-2;
				goto L;
				}
			}
			}
		}//last_cor14.12.16
		if(grayscale_close==1)
		{
			if(!hue_close)
			{
			if(!difference_hue_zone)
			{
				hue_close=1;
				prior=1;
				goto L;
			}
			else
			{
if((difference_hue_zone==1)&&(min_hue_zone==3))
{
	hue_close=2;
	prior=1;
	goto L;
}
			}
			}
		}
		else
		{
			if(grayscale_close==2)
			{
				if((!difference_hue_zone)&&(hue_difference<=4))
							{//last_cor13.11.17
								hue_close=1;
								prior=1;
								goto L;
							}
							else
							{
				if((difference_hue_zone==1)&&(min_hue_zone==3)&&(hue_difference<=6))
				{//last_cor13.11.17
					hue_close=2;
					prior=2;
					goto L;
				}
							}
			}
		}
	}//last_cor25.11.15
	if(grayscale_close==1)
		{//last_cor24.08.15
			if(maximum_zone_gray>=7)
			{
				if((min_hue_zone>=3)&&(max_hue_zone<=4))
				{
					if(hue_difference<=6)
					{
						hue_close=1;
						prior=1;
						goto L;
					}
					else
					{
						if(hue_difference<=9)
					    {
						hue_close=2;
						prior=1;
						goto L;
					    }
						else
						{
							if(hue_difference<=12)
							{//last_cor29.08.15
						    hue_close=3;
						    prior=2;
						    goto L;
							}
						}
					}
				}
			}
		}//last_cor24.08.15
	if(saturation_difference==1)
	{
		if(minimum_zone_gray>=4)
		{
			if((gray_difference>=7)&&(gray_ntersection>=2))
			{
				grayscale_close=0;
				prior=7;
				goto L;
			}
		}
	}
	if(!grayscale_close)
	{//last_cor19.06.15

		if(gray_difference<=possible_gray_variation)
		{
			if(4*gray_difference<=3*possible_gray_variation)
			{
				grayscale_close=2;
			}
			else
			{
				grayscale_close=3;
			}
		}
	}//last_cor19.06.15
	if(!min_saturation)
	{//last_cor08.06.15
		if(grayscale_close==1)
		{
			if(!difference_hue_zone)
			{//last_cor19.06.15
				if(hue_difference<=6)
				{
					hue_close=1;
					prior=1;
					goto L;
				}
				else//last_cor11.09.15
				{
if(!min_saturation)
{
	hue_close=2;
	prior=1;
	goto L;
}
				}//last_cor11.09.15
			}//last_cor19.06.15
			if((gray_difference<=2)||((gray_difference<=4)&&(gray_ntersection<2)))
			{
				if(!hue_close)
				{//hc0
					if(!difference_hue_zone)
					{//dhz0
						if(hue_difference<=8)
						{
							hue_close=1;
						}
						else
						{
						hue_close=2;
						}
					}//dhz0
					else
					{
						if(hue_difference<=4)
						{
							hue_close=1;
						}
					}
				}//hc0
				prior=1;
				goto L;
			}
		}
		else//last_cor19.06.15
		{
			if(grayscale_close==2)
			{
if(maximum_zone_gray>=6)
{
	if(!difference_hue_zone)
	{
		if(!hue_close)
		{//last_cor11.09.15
			if(minimum_zone_gray>=6)
			{
				if(hue_difference<=5)
				{
					hue_close=1;
					prior=1;
					goto L;
				}
				else
				{
					if(hue_difference<=9)
				    {
                     hue_close=2;
					 prior=2;
					 goto L;
					}
				}
			}
		}
		if(hue_close<0)
		{
			if(hue_difference<=8)
			{
				hue_close=3;
				prior=2;
				goto L;
			}
		}
	}
}
else
{
	if(!difference_hue_zone)
	{
		if(hue_difference<=4)
		{
			hue_close=1;
			prior=1;
			goto L;
		}
		else
		{//last_cor08.09.15
			if((min_hue_zone>=3)&&(max_hue_zone<=4))
			{
				if(hue_difference<=8)
				{
					hue_close=2;
					prior=2;
					goto L;
				}
			}
		}//last_cor08.09.15
	}
}
			}
		}//last_cor19.06.15
	}//last_cor08.06.15
	if(hue_close==1)
	{//last_cor22.04.15
		if(grayscale_close==1)
		{
		prior=1;
		goto L;
		}
		else
		{
			if(grayscale_close==2)
			{
				if(3*gray_difference<2*possible_gray_variation)
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
	}//last_cor22.04.15

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
	else//last_cor22.05.15
	{
            if(maximum_zone_gray<=2)
			{//last_cor22.05.15
				if(grayscale_close==1)
				{
				if(gray_difference<=1)
				{//last_cor10.06.15
					if(!difference_hue_zone)
				    {
                    hue_close=1;
					prior=1;
					goto L;
					}
					else
					{
						if(difference_hue_zone==1)
						{
							if(hue_difference<=8)
							{
					hue_close=0;
					prior=2;
					goto L;
							}
						}
						else
						{
							if(min_saturation==1)
							{//last_cor10.06.15
							if(hue_close==-2)
							{
								prior=6;
								goto L;
							}
							}//last_cor10.06.15
						}
					}
				}
				else
				{
					if(!difference_hue_zone)
				    {
                    hue_close=1;
					prior=1;
					goto L;
					}
					else
					{
					hue_close=0;
					prior=3;
					goto L;
					}
				}
				}
            }//last_cor22.05.15
	}
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
if(((gray_ntersection<=1)||(gray_difference<=3))&&(max_saturation<=1)&&(grayscale_close==1)&&
   (gray_difference<=possible_gray_variation/2))
{//last_cor19.09.1327.11.14-28.11.14
	if(hue_close>0)
	{//last_cor27.10.14
	prior=1;
goto L;
	}
	else
	{//last_cor27.11.14
		if(!hue_close)
		{
if((difference_hue_zone<=1)&&(((zone_hue>2)&&(zone_hue<5))||
   ((zone_hue_bunch>2)&&(zone_hue_bunch<5))))
{
	if(!difference_hue_zone)
	{//dhz0
		if(hue_difference<=6)
		{
			hue_close=1;
			prior=1;
			goto L;
		}
		else
		{
			if(hue_difference<=12)
		{
			hue_close=2;
			prior=1;
			goto L;
		}
		}
	}//dhz0
	else
	{
		if(difference_hue_zone<=1)
		{
			if(hue_difference<=4)
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
	}
}
		}
	}//last_cor27.11.14
}//last_cor19.09.13
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
				{//last_cor11.4.142and27.10.14
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
			if((grayscale_close>0)&&(hue_close<0))
			{//last_cor29.04.15
				prior=6;
				goto L;
			}
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
/*if((grayscale_close==1)&&(difference_hue_zone<=1))
{
if((ratio_length1==16)&&(Signif_ratio<=2)&&(Density_ratio<4)&&(length_ratio<4))
	   {//incl1
		prior=1;
		goto L;
	   }//incl1

}*/

if((grayscale_close>=1)&&(grayscale_close<=2)&&(hue_close==1))
	   {//gh
	 prior=1;
       }//gh
       else
      {//e1
		  if((hue_close>1)&&(grayscale_close>=1)&&(grayscale_close<3))
		  {
		  prior=2;
	      }
		  else
		  {
      if(((hue_close==1)&&(grayscale_close==3))||((hue_close==3)&&(grayscale_close==1)))
	  {
		  if(difference_gray_zone==0)
		  {
	  prior=2;
		  }
		  else
		  {
			  prior=3;
		  }
	     }
	  else
	  {
		  if((hue_close>=2)&&(grayscale_close>1))
		  {
if((ratio_length1==16)&&(Signif_ratio<=2)&&(Density_ratio<4)&&(length_ratio<4))
	   {//incl1
		   if(grayscale_close<=2)
		   {
		prior=2;
		   }
		   else
		   {
			prior=3;
		   }
	   }//incl1
		  }
	  }
	  if((grayscale_close>0)&&(grayscale_close<=2)&&(difference_hue_zone<=1)&&(difference_gray_zone<=1)&&(hue_close>=0))
{
if((ratio_length1==16)&&(Signif_ratio<2)&&(Density_ratio<=2)&&(length_ratio<4))
	   {//incl1
		prior=2;
		//goto L;
	   }//incl1

}
		  }
      }//e1
if((grayscale_close==1)&&((!neighbor_bunch_mean_saturation)||
		  (!bunch_mean_saturation)))
	  {//last_cor08.11.17
		  if((difference_hue_zone<=1)&&(hue_difference<=6))
		  {
			  prior=1;
		     goto L;
		  }
	  }//last_cor08.11.17
	 if((prior>0)&&(prior<=4))
	 {
       if((ratio_length1<5)&&((Signif_ratio>6)||(Density_ratio>6)))
	   {//incl
		prior+=10;
	   }//incl
	    goto L;
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
	      }
		  if((hue_close==1)&&(grayscale_close==3)&&(!saturation_difference))
		  {
			  prior=2;
		  }
		  else
		  {
		  if(((hue_close==1)&&(grayscale_close==3))||((hue_close==3)&&(grayscale_close==1)))
		  {
		  prior=3;
	      }
		   else
	      {
		  if((hue_close>2)&&(grayscale_close>1))
		  {
if((ratio_length1==16)||(Signif_ratio<=2)&&(Density_ratio<4)&&(length_ratio<4))
 {//incl1
prior=4;
 }//incl1
		  }
	      }
		  }
      }//e1
if(grayscale_close==1)
{
	if((ratio_length1==16)||(Signif_ratio<1)&&(Density_ratio<=2)&&(length_ratio<2))
 {
prior=4;
 }
}
   if((prior>0)&&(prior<=4))
	 {
       if((ratio_length1<5)&&((Signif_ratio>6)||(Density_ratio>6)))
	   {//incl
		prior+=10;
	   }//incl
	    goto L;
	 }
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
{//s<=2//21.05.13//last_cor16.07.13
	if((!grayscale_close)&&(gray_difference>possible_gray_variation))
	{//last_cor20.01.17
		if((max_saturation==2)&&(min_saturation<2))
		{
			if((ratio_length1_gray1<8)||(ratio_length_gray1<8))
			{
				grayscale_close=-2;
			}
		}
	}//last_cor20.01.17
	if((min_hue_zone==3)&&(max_hue_zone==3))
	{//last_cor15.02.17
		if(max_saturation==2)
		{
			if(min_saturation==1)
			{//last_cor02.03.17
				if(Intersection_hue==3)
				{
					if((hue_difference>=4)&&((bunch_mean_hue>=18)||(neighbor_bunch_mean_hue>=18)))
					{
						hue_close=-2;
						if((gray_ntersection>=2)&&(gray_difference>=5))
						{
							grayscale_close=0;
						}
						prior=6;
						goto L;
					}
				}
			}//last_cor02.03.17
			if((Intersection_hue==3)&&(gray_ntersection==3)&&(Intersection==3))
			{
				if(((bunch_mean_hue<=15)&&(neighbor_bunch_mean_hue>=18))||
					((bunch_mean_hue>=18)&&(neighbor_bunch_mean_hue<=15)))
				{
					if(gray_difference>=5)
					{//last_cor02.03.17
						grayscale_close=0;
						hue_close=0;
						prior=6;
						goto L;
					}
				}
			}
		}
	}//last_cor15.02.17
	if((min_saturation>=1)&&(zone_gray>=3)&&(zone_gray_bunch>=3))
		{//last_cor14.12.16
			if(!hue_close)
			{
            if(((bunch_mean_hue>=19)&&(neighbor_bunch_mean_hue<=13))||
				((neighbor_bunch_mean_hue>=19)&&(bunch_mean_hue<=13)))
			{
				if(grayscale_close>0)
				{
					prior=7;
					hue_close=-2;
					goto L;
				}
				else
				{
                prior=6;
				hue_close=-2;
				goto L;
				}
			}
			}
		}//last_cor14.12.16
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
	{///condlog

		if(saturation_difference<=1)
	    {//last_cor18.09.14
       saturation_close=1;
	    }
		else
		{//last_cor28.10.13
			if((saturation_difference>=imp_sat_var)||((!min_saturation)&&(max_saturation==2)))
			{//last_cor06.04.15
				if((zone_gray>=2)&&(zone_gray_bunch>=2))
				   {
					   if(minimum_zone_gray<6)
					   {
				       saturation_close=-1;
					   }
					   else
					   {
                       saturation_close=3;
					      if((min_hue_zone==4)&&(!difference_hue_zone))
					      {
							  if(hue_difference<=4)
							  {
								  if(!hue_close)
								  {
									  hue_close=1;
									  if(grayscale_close>0)
									  {
										  prior=2;
										  goto L;
									  }

								  }
							  }
							  else
							 {//last_cor20.08.15
                              if(hue_difference<=6)
							  {
								  if(!hue_close)
								  {
									if(grayscale_close>0)
									  {
										  prior=3;
										  goto L;
									  }
								  }
							  }
				     		 }
					      }
					   }
				   }
			}
			else
			{
				if((!difference_hue_zone)&&(grayscale_close==1)&&(maximum_zone_gray>=4))
				{//last_cor18.09.14
			saturation_close=2;
				}
				else
				{
					saturation_close=3;
				}
			}
		}//last_cor28.10.13

	}///condlog
	if((max_saturation>=2)&&(min_saturation>=1))
	{
		if(!saturation_close)
		{
		saturation_close=1;
		}
		if(maximum_zone_gray>=2)
		{
			if(hue_close<0)
			{
				prior=5;
				goto L;
			}
			else
			{
				if(difference_hue_zone>=1)
				{
					if((min_hue_zone==3)&&(max_hue_zone==4))
					{
						if(hue_difference>=5)
						{
						prior=5;
				        goto L;
						}
					}
				}
			}
		}
	}
	if((zone_hue==3)&&(zone_hue_bunch==3))
	{//mzhmazh3
	if(!grayscale_close)
	{//last_cor13.02.17
		if((min_saturation==2)&&(max_saturation==2))
		{//last_cor15.02.17
if((ratio_length_gray<8)&&(ratio_length1_gray<8)&&(gray_difference>=possible_gray_variation)&&
   (gray_ntersection>=2))
{
	grayscale_close=-3;
	if(hue_close>0)
	{
		prior=6;
		goto L;
	}
	else
	{
		prior=5;
		goto L;
	}
}
		}//last_cor15.02.17
if(gray_difference>possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/3)
{
	if((maximum_zone_gray>=4)&&(minimum_zone_gray<=3))
	{
		grayscale_close=-2;
		if((hue_close>0)||(saturation_close>=0))
		{
			prior=6;
			goto L;
		}
		else
		{
		prior=5;
			goto L;
		}
	}
}
	}
			if((neighbor_bunch_mean_hue>=10)&&(neighbor_bunch_mean_hue<=17)&&(bunch_mean_hue>=10)&&
   (bunch_mean_hue<=17))
			{//1017
        if(!hue_close)
		{//hc0
				if(hue_difference<=3)
				{
					hue_close=1;
				}
				else
				{
				if(hue_difference<=6)
				{
					hue_close=2;
				}
				else
				{
					hue_close=3;
				}
				}

		}//hc0
if(!hue_close)
{//hc0
	if(max_saturation=2)
	{
		if(((bunch_mean_hue>=21)&&(neighbor_bunch_mean_hue<=14))||
			((bunch_mean_hue<=14)&&(neighbor_bunch_mean_hue>=21)))
		{
			hue_close=-1;
			if((grayscale_close<=0)||(saturation_close<=0))
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
	}
}//hc0
if((min_saturation==1)&&(max_saturation==2))
		{//misma
			if(!grayscale_close)
			{
				if(minimum_zone_gray>=2)
				{
if(gray_difference>possible_gray_variation)
{
	if(hue_close>0)
	{
		saturation_close=0;
		prior=7;
		goto L;
	}
	else
	{
		saturation_close=0;
		prior=6;
		goto L;
	}
}
else
{
	if(((gray_ntersection==2)&&(ratio_length_gray<7)&&(ratio_length1_gray<7))||
		(gray_ntersection==3))
	{
if(hue_close>0)
	{
		saturation_close=0;
		prior=7;
		goto L;
	}
	else
	{
		saturation_close=0;
		prior=6;
		goto L;
	}
	}
}
				}
			}

		}//misma

			}//1017


	}//mzhmazh3
if(saturation_close==1)
	{//last_cor24.08.15
		if(grayscale_close>0)
		{//last_cor25.11.15
if(grayscale_close==1)
{//gc1
           if(minimum_zone_gray>=3)
		    {//mz3
				if(!hue_close)
					{//hc0
				if(!difference_hue_zone)
				{//dhz0
					if(max_saturation<=1)
					{
						hue_close=2;
						prior=1;
						goto L;
					}

				}//dhz0
				else
				{//edhz0
                    if(difference_hue_zone==1)
				     {
					if((hue_difference<=5)&&(max_saturation<=1))
					{
						hue_close=2;
						prior=2;
						goto L;
					}
					}

				}//edhz0
				   }//hc0
            }//mz3
}//gc1
else
{//egc1
	if(grayscale_close==2)
    {//gc2
		if(minimum_zone_gray>=3)
		    {//mz3
				if(!hue_close)
					{//hc0
				if(!difference_hue_zone)
				{//dhz0
					if(!saturation_difference)
					{
						hue_close=2;
						prior=2;
						goto L;
					}
				}//dhz0
				else
				{//edhz0
                    if(difference_hue_zone==1)
				     {

						hue_close=3;
						prior=2;
						goto L;
					}

				}//edhz0
				   }//hc0
            }//mz3
	}//gc2
}//egc1
		}//last_cor25.11.15

		if(!grayscale_close)
		{
			if((gray_ntersection==3)&&(gray_difference>possible_gray_variation))
			{
				if((min_hue_zone==3)&&(max_hue_zone==3))
				{
					if((min_saturation<=1)&&(max_saturation>=2))
					{
						if((saturation_close>0)||(hue_close>0))
						{
							prior=6;
							goto L;
						}
						else
						{
							prior=5;
							goto L;
						}
					}
				}
			}
		}
if(hue_close==1)
{
		if(grayscale_close==1)
		{
			prior=1;
			goto L;
		}
		else
		{
			   if(!grayscale_close)
			   {
		    if(minimum_zone_gray>=5)
		    {
				if(gray_difference<=possible_gray_variation)
				{
				grayscale_close=2;
				prior=1;
				goto L;
				}
				else
				{
					   if((max_hue_zone<=4)&&(min_hue_zone>=3))
						{
					if(gray_difference<=impossible_gray_variation)
					{
if(gray_difference<=(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/3))
{
grayscale_close=3;
prior=2;
goto L;
}
					}
					    }
				}
		    }
			   }
		}
}
else
{
	if(!hue_close)
	{
if(minimum_zone_gray>=5)
{
   if((max_hue_zone<=4)&&(min_hue_zone>=3))
   {
       if(!difference_hue_zone)
      {
	   if(hue_difference<=4)
	   {
		   hue_close=1;
		   if(!grayscale_close)
		   {
			   if(gray_difference<=possible_gray_variation)
			   {
				   grayscale_close=2;
				   prior=1;
				   goto L;
			   }
		   }
	   }
      }
      else
      {
		  if(hue_difference<=4)
	   {
		   hue_close=2;
		   if(!grayscale_close)
		   {
			   if(gray_difference<=possible_gray_variation)
			   {
				   grayscale_close=2;
				   prior=2;
				   goto L;
			   }
		   }
	   }
      }
   }
}
	}
}
	}//last_cor24.08.15
	if(saturation_close==1)
	{//last_cor26.03.14sc1
		if(min_saturation>=1)
		{//ms1
			if(hue_close==1)
			{//hc1

				if(minimum_zone_gray>=5)
				{//mzg5
					if((zone_hue==4)&&(zone_hue_bunch==4))
					{//zh4
						if(gray_difference<=impossible_gray_variation)
						{//gd<=ig
							if(grayscale_close<0)
							{
								grayscale_close=4;
								prior=4;
								goto L;
							}
						}//gd<=ig
					}//zh4
				}//mzg5
			}//hc1
		}//ms1
		if(grayscale_close==1)
		{//last_cor11.04.15
			if(minimum_zone_gray>=5)
			{//last_cor22.05.15
			 if(!difference_hue_zone)
			 {
				 if(zone_hue==4)
				 {
					 if(hue_difference<=6)
					 {
						 hue_close=1;
						 prior=1;
						 goto L;
					 }
				 }
			 }
			 else
			 {//last_cor25.06.15
				 if(min_saturation==1)
				 {//ms1
if(difference_hue_zone==1)
{//dhz1
	if(max_hue_zone==3)
	{//mhz3
		if(hue_difference<=3)
		{
			hue_close=1;
			prior=1;
			goto L;
		}
		else
		{//e1
         if(hue_difference<=6)
		 {
			 hue_close=2;
			 prior=2;
			 goto L;
		 }
		 else
		 {//e
			 if((gray_difference<=2)&&(gray_ntersection<=1))
			 {

                 if(hue_difference<=8)
		         {
			      hue_close=3;
			     prior=2;
			     goto L;
		         }

			 }
		 }//e
		}//e1
	}//mhz3
	else
	{//last_cor20.08.15
		if((max_hue_zone==4)&&((min_hue_zone==3)||(min_hue_zone==4)))
		{
			    if(minimum_zone_gray>=6)
			    {
			if((length_ratio<=14)||(length_ratio<=2))
			{
				if(hue_difference<=6)
				{
					hue_close=1;
					prior=1;
					goto L;
				}

			}
				}
		}
	}//last_cor20.08.15
}//dhz1
				 }//ms1
				 else
				 {//ems1
					 if((gray_difference<=2)&&(gray_ntersection<=1))
			          {
						  if(difference_hue_zone==1)
						  {
	if(max_hue_zone==3)
	{//mhz3
		if(hue_difference<=4)
		{
			hue_close=2;
			prior=1;
			goto L;
		}
		else
		{
			if(hue_difference<=8)
			{
			hue_close=3;
			prior=2;
			goto L;
			}

		}
	}//mhz3
						  }
					  }
				 }//ems1
			 }//last_cor25.06.15
			}
			else
			{//emzg5last_cor22.05.15
			if((zone_gray<=3)&&(zone_gray_bunch<=3))
			{//zg<=3
				if((gray_ntersection<=1)&&(std::max(ratio_length_gray,ratio_length1_gray)>=12))
				{//last_cor23.06.15
					if(gray_difference<=2)
					{
						if(hue_close<=0)
						{
							if(difference_hue_zone<=1)
							{
								if((zone_hue_bunch!=1)&&(zone_hue!=1))
								{
									if(hue_difference<=4)
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
						                 prior=2;
						                  goto L;
									    }
									}
								}
							}
						}
					}
				}
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
			}//emzg5
		}//last_cor11.04.15
		if((zone_gray<=5)&&(zone_gray_bunch<=5))
		{//last_cor26.03.14
			if(max_saturation>=2)
			{//last_cor03.12.14
				if((difference_hue_zone>=1)&&(hue_difference>=8))
				{
                   if((zone_gray>=2)||(zone_gray_bunch>=2))
				   {
					hue_close=-1;
				   }
				}
			}
			if(max_saturation<=5)
			{//ms
			if(!hue_close)
			{//hc
if((zone_hue>1)&&(zone_hue<4)&&(zone_hue_bunch>1)&&(zone_hue_bunch<4))
					{//zh
						if(hue_difference<=3)
						{
						hue_close=1;

						}
						else
						{//e
							if(hue_difference<=4)
						    {
                            hue_close=2;

							}
							else
							{//last_cor12.09.14
								if((grayscale_close==1)&&(saturation_close==1))
						       {
								       if(!difference_hue_zone)
								        {//last_cor03.12.14
							      if(hue_difference<=6)
							       {
								   prior=2;
								   hue_close=3;
								   goto L;
							       }
								  else
								  {
									  if(hue_difference<=8)
							       {
								   prior=3;
								   hue_close=4;
								   goto L;
							       }
								  }
									   }
						       }
							}
						}//e

					}//zh
			}//hc
			}//ms

		}//last_cor26.03.14
      if((zone_gray>=4)&&(zone_gray_bunch>=4))
	   {//zg3

if(((gray_difference<possible_gray_variation/3)||(gray_difference<=3))&&(gray_ntersection<=1))
{
	if(hue_difference<=5)
	{
		if(!difference_hue_zone)
		{
			hue_close=1;
			prior=1;
            goto L;
		}
		else
		{
			if(difference_hue_zone<=1)
		    {//last_cor23.10.14
				if((max_saturation<=1)||(hue_difference<=2))
				{
			hue_close=2;
		    prior=2;
		    goto L;
				}
			}
		}
	}
}
	  }//zg3
	}//last_cor24.03.14
	if((zone_gray>=4)&&(zone_gray_bunch>=4))
	   {//zg4
		   if((grayscale_close==1)&&(saturation_close>=1)&&(saturation_close<=2))
		   {//last_cor18.09.14
if((gray_difference<possible_gray_variation/3)||(gray_difference<=3))
{
	if(hue_difference<=3)
	{
			hue_close=1;
			prior=1;
            goto L;

	}
	else
	{
	if((hue_difference<=6)&&(!difference_hue_zone))
		{
			hue_close=2;
			prior=2;
            goto L;
		}
	}
}
		   }//last_cor18.09.14
		   if((saturation_close==1)&&(hue_close==2)&&(grayscale_close>0))
		   {//last_cor04.12.14
			   if(grayscale_close==1)
			   {
				   prior=1;
				   goto L;
			   }
			   else
			   {
				   if((length_ratio>=15)||(length_ratio<=1))
				   {//last_cor04.12.14
					   prior=2;
				       goto L;
				   }
			   }
		   }//last_cor04.12.14
	}//zg4
	if(hue_close==1)
	{//last_cor28.05.13
		if(saturation_close==1)
		{//sc1
			if((grayscale_close>=1)&&(grayscale_close<=2))
			{
            prior=1;
            goto L;
			}
			else//egc12
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
						   {//c1
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

                           }//c1
					}//egc
				}//gc>=0
			}//egc12


		}//sc1

	}//last_cor28.05.13
	if((grayscale_close<0)&&(hue_close))
	{//lacorlast_cor27.04.15
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
			prior=5;
            goto L;
			}
			else
			{
			prior=6;
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
		if((grayscale_close==1)&&(difference_hue_zone<=1))
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
	{//ezg1g
if((zone_gray<=2)||(zone_gray_bunch<=2))//last_cor 21.04.13
	{//zg2
		if((grayscale_close==1)&&(difference_hue_zone<=1))
		{
if(union_gray_length<possible_gray_variation)
			{//ugl
				if(saturation_difference==2)
	            {//last_cor15.04.15
if(gray_difference<=possible_gray_variation/4)
{//gd<=
	if(!difference_hue_zone)
	{//dhz0
		if(hue_difference<=4)
		{
			hue_close=1;
			saturation_close=2;
			prior=1;
			goto L;
		}
		else
		{//e2
if(hue_difference<=6)
		{

			hue_close=2;
			saturation_close=2;
			prior=2;
			goto L;

		}
else
{//e1
	if(union_gray_length<possible_gray_variation/2)
			{
            hue_close=3;
            saturation_close=2;
            prior=2;
            goto L;
	        }
	else
	{
	hue_close=3;
    saturation_close=2;
    prior=3;
    goto L;
	}
}//e1
		}//e2
	}
}
	            }//last_cor15.04.15
				if(hue_close>0)
				{//last_cor13.03.15
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
				}//last_cor13.03.15
            }//ugl
             /*elselast_cor13.03.15
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
            }*/
		}
	}//zg2
else
{//last_cor 10.06.13
	if(((zone_gray<=4)||(zone_gray_bunch<=4))&&(saturation_close==1))
	{//zg4

		if(hue_close<=0)
		{//hc
//last_cor06.04.15

			if((difference_hue_zone<=1)&&(((zone_hue>2)&&(zone_hue<5))||
   ((zone_hue_bunch>2)&&(zone_hue_bunch<5))))
			{//dhz//last_cor11.04.14
				if((hue_difference<=4)||((!difference_hue_zone)&&(hue_difference<=6)))
				{
					if(hue_close>=0)
					{//last_cor11.03.15
				prior=2;
				hue_close=2;
				goto L;
					}
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
		{//ep1
			if((hue_close>=2)&&(hue_close<=3)&&(grayscale_close==1)&&(saturation_close>0))
			{//last_cor28.11.14
				prior=1;//last_cor21.01.13
                goto L;
			}
			else
			{//ep2
				if(difference_hue_zone==0)
				{//dhz==0
					if(hue_difference<=4)
					{
						if((saturation_close>0)&&(grayscale_close>0))
						{//last_cor28.11.14
                  if((saturation_close==1)&&(grayscale_close==1))
							{//last_cor29.04.15
                                if(hue_difference<=3)
							    {
								prior=1;
								hue_close=1;
								goto L;
							    }
								else
								{
									prior=2;
									hue_close=2;
									goto L;
								}
							}
							else
							{
		            hue_close=2;//last_cor18.01.13
					prior=2;
                    goto L;
							}//last_cor29.04.15
						}//last_cor28.11.14
					}//hd4
					else
					{//hd4
						if(hue_difference<=6)
					    {//hd6
						if((zone_gray>=6)&&(zone_gray_bunch>=6)&&(saturation_close==1)&&(grayscale_close==1)&&
   (gray_ntersection<=1)&&(3*gray_difference<=possible_gray_variation))
{//last_cor18.06.14
	if(!hue_close)
	{//last_cor11.03.15
	hue_close=3;//last_cor18.01.13

prior=2;
goto L;
	}
}
/*else
{
	if(!hue_close)
	{//last_cor11.03.15
hue_close=3;//last_cor18.01.13
prior=3;
goto L;
	}
}*/
					    }//hd6
					}//hd4
				}//dhz==0

			}//ep2
		}//ep1
	}//gc=1sd<=1
	if((!gray_ntersection)&&(gray_difference<=2)&&(2*gray_difference<possible_gray_variation)&&
		(difference_hue_zone<=1)&&(zone_gray<=4)&&(zone_gray_bunch<=4))
	{//last_cor16.02.13
		if((hue_close>0)&&(saturation_close>0))
		{//last_cor11.03.1506.04.15
		prior=2;
        goto L;
		}
	}
	if((hue_close>0)&&(saturation_close>0))
	{//last_cor28.10.13
		if((saturation_close==1)&&(hue_close==2)&&(grayscale_close>0))
{//last_cor05.12.14
	if((length_ratio>=15)||(length_ratio<=1))
	{//last_cor08.12.14
		if(((end_int>=(StripLength-2*StripWidth))&&(neighbor_end_int>=(StripLength-StripWidth)))
||((neighbor_end_int>=(StripLength-2*StripWidth))&&(end_int>=(StripLength-StripWidth))))
		{//last_cor08.12.14
			if(grayscale_close>=2)
			{
				prior=2;
                goto L;
			}
		}
		if(((beg_int<=2*StripWidth)&&(neighbor_beg_int<=StripWidth))
||((neighbor_beg_int<=2*StripWidth)&&(beg_int<=StripWidth)))
		{//last_cor08.12.14
			if(grayscale_close>=2)
			{
				prior=2;
                goto L;
			}
		}
	}//last_cor08.12.14
}
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
if((min_hue_zone==2)&&(max_hue_zone==2))
{//last_cor20.11.17
	if((min_saturation==2)&&(saturation_difference>=2))
	{
		saturation_close=-1;
		prior=5;
        goto L;
	}
}
if((min_hue_zone==4)&&(max_hue_zone==4))
{
	if(saturation_close<=0)
	{
if(saturation_difference>=4)
{
	if(Intersection_saturation==3)
	{
		saturation_close=-1;
		prior=5;
		goto L;
	}
}
	}
}
if((min_hue_zone==3)&&(max_hue_zone==3))
{//mzhmhz3last_cor10.04.17
	if(min_saturation>=5)
		{//last_cor16.02.18
			if(!hue_close)
			{
				if(hue_difference<=3)
				{
					hue_close=1;
				}
			}
			if(saturation_close==1)
			{
				if(grayscale_close>0)
				{
					prior=1;
					goto L;
				}
				if((gray_difference<=(possible_gray_variation+3))&&(difference_gray_zone<=1))
				{
					grayscale_close=3;
					prior=2;
					goto L;
				}
			}
		}//last_cor16.02.18
if((min_saturation==2)&&(max_saturation==3))
{//mins2
	if((!grayscale_close)||(gray_ntersection>=2)||(gray_ntersection2>=2))
	{//last_cor02.05.17
if(gray_difference>=possible_gray_variation/2)
{
		if((zone_gray_bunch>=3)&&((zone_gray_bunch<=5))&&(zone_gray>=3)&&(zone_gray<=5))
		{
	grayscale_close=-2;
	saturation_close=0;
	prior=6;
	goto L;
		}
}
else
{
if((zone_gray_bunch>=3)&&((zone_gray_bunch<=5))&&(zone_gray>=3)&&(zone_gray<=5))
		{
	grayscale_close=0;
	saturation_close=0;
	prior=7;
	goto L;
		}
}
	}//last_cor02.05.17
if((bunch_mean_hue==22)&&(neighbor_bunch_mean_hue<=19))
{
if(gray_difference>=possible_gray_variation/2)
{
		if((zone_gray_bunch>=3)&&(zone_gray<=2))
		{
if(!hue_close)
{
				hue_close=-3;
				if(grayscale_close>0)
				{
				prior=7;
				goto L;
				}
				else
				{
					prior=6;
					goto L;
				}
}
		}
	}
}
if((bunch_mean_hue<=19)&&(neighbor_bunch_mean_hue==22))
{//bmg<=
	if(gray_difference>=possible_gray_variation/2)
	{//gd
		if((zone_gray_bunch<=2)&&(zone_gray>=3))
		{
			if(!hue_close)
			{
				hue_close=-3;
				if(grayscale_close>0)
				{
				prior=7;
				goto L;
				}
				else
				{
					prior=6;
					goto L;
				}
			}
		}
	}//gd
}//bmg<=
}//mins2
if((hue_close==1)&&(grayscale_close==1))
{//last_cor19.07.17
	if(!saturation_close)
	{
		if(maximum_zone_gray<=3)
		{
			if(min_saturation>=3)
			{
				if(saturation_difference<=poss_sat_variation)
				{
                 saturation_close=1;
				 prior=1;
				 goto L;
				}
				else
				{
					if(saturation_difference<=imp_sat_var)
					{
						saturation_close=2;
						prior=2;
						goto L;
					}
				}
			}
		}
	}
}//last_cor19.07.17
}//mzhmhz3last_cor10.04.17

                 if(!saturation_close)
                 {//last_cor26.11.15-sc0
	if(saturation_difference<=poss_sat_variation)
		{//last_cor17.08.15
			saturation_close=1;

		}//last_cor17.08.15
	else
	{//esdpv
		if((min_hue_zone==3)&&(max_hue_zone==3))
		{//mzhmhz3
			if(min_saturation==3)
			{
				if(saturation_difference>=imp_sat_var)
				{
					saturation_close=-1;
				}
			}
			else
			{//last_cor26.09.16
				if((min_saturation<=2)&&(max_saturation>=4))
				{//msms24
if(((bunch_mean_saturation>=4)&&(zone_gray_bunch>=1)&&(zone_gray>=4))||
   ((neighbor_bunch_mean_saturation>=4)&&(zone_gray_bunch>=4)&&(zone_gray>=1)))
{//bmsnbms
	saturation_close=-1;
	if(!grayscale_close)
	{
	if(gray_difference>possible_gray_variation)
	{
		grayscale_close=-1;
	}
	}
	if(!hue_close)
	{
		if((hue_difference<=3)&&(!difference_hue_zone))
		{
			hue_close=1;
		}
	}
	if((grayscale_close<=0)||(hue_close<=0))
	{
		prior=5;
		goto L;
	}
	else
	{
		prior=6;
		goto L;
	}
}//bmsnbms
				}//msms24
			}//last_cor26.09.16
		}//mzhmhz3
		if((min_saturation==1)&&(saturation_difference>=2))
		{//last_cor17.12.15
			if((bunch_mean_saturation==1)&&(zone_gray_bunch>=3))
			{
				if((neighbor_bunch_mean_saturation>=3)&&(zone_gray>=1))
				{
				saturation_close=-1;
				}
			}
			if((neighbor_bunch_mean_saturation==1)&&(zone_gray>=3))
			{
				if((bunch_mean_saturation>=3)&&(zone_gray_bunch>=1))
				{
				saturation_close=-1;
				}
			}
		}//last_cor17.12.15


   if(saturation_difference<imp_sat_var)
   {//last_cor14.01.12sd<isv
if(saturation_difference<=2)
{//last_cor15.01.16sd<=2
if(max_saturation>4)
{
saturation_close=2;
}
}//last_cor15.01.16sd<=2
else//
{//last_cor14.01.12esd<=2
	if(min_saturation>=6)
	{
saturation_close=2;
	}

}//last_cor14.01.12esd<=2
   }//last_cor14.01.12sd<isv
	}//esdpv
                         }//last_cor26.11.15-sc
if((min_hue_zone==3)&&(max_hue_zone==3))
{//last_cor03.05.17
if((bunch_mean_hue==22)||(neighbor_bunch_mean_hue==22))
{//
	if((neighbor_bunch_mean_hue<=18)||(bunch_mean_hue<=18))
	{
     if((zone_gray_bunch>=1)&&(zone_gray_bunch<=4)&&(zone_gray>=1)&&(zone_gray<=4))
	 {
		 hue_close=-2;
		 if((saturation_close>=0)||(grayscale_close>=0))
		 {
			 prior=6;
			 goto L;
		 }
		 else
		 {
			 prior=5;
			 goto L;
		 }
	 }
	}
	else
	{
		if(hue_difference==3)
		{
			if(gray_difference>=possible_gray_variation/3)
	        {//gd
if((zone_gray_bunch>=1)&&(zone_gray_bunch<=4)&&(zone_gray>=1)&&(zone_gray<=4))
	 {
		 hue_close=-2;
		 if((saturation_close>=0)||(grayscale_close>=0))
		 {
			 prior=6;
			 goto L;
		 }
		 else
		 {
			 prior=5;
			 goto L;
		 }
	 }
			}
		}
	}
}
}//last_cor03.05.17
if(((zone_hue==4)&&(zone_hue_bunch==3))||((zone_hue==3)&&(zone_hue_bunch==4)))
{//last_cor01.09.16
	if(max_saturation>=4)
	{
		if(hue_difference>=4)
		{//last_cor28.03.17
			if(minimum_zone_gray>=1)
			{
				hue_close=-1;
				if((grayscale_close>0)||(saturation_close>0))
				{
					prior=6;
						goto L;
				}
				else
				{
					prior=5;
					goto L;
				}
			}
		}//last_cor28.03.17
		if((saturation_difference>poss_sat_variation))
		{//last_cor14.12.16
			if((Intersection_saturation==3)&&(Intersection_hue==3))
			{
				if(saturation_difference>=imp_sat_var)
				{
					saturation_close=-1;
				}
				if((bunch_mean_hue<=20)||(neighbor_bunch_mean_hue<=20))
				{//last_cor13.01.16
					if(hue_difference>=4)
					{
					hue_close=-1;
					}
					else
					{
						hue_close=-2;
					}
					if(hue_close<0)
					{
						if((saturation_difference<=0)||(grayscale_close<=0))
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
				}
				if((maximum_zone_gray>=1)&&(max_saturation>=8))
				{
					if(grayscale_close>0)
					{
						saturation_close=-2;
						prior=7;
						goto L;
					}
					else
					{
						saturation_close=-2;
						prior=6;
						goto L;
					}

				}
			}
		}//last_cor14.12.16
if(hue_difference>=5)
{
	hue_close=-1;
	if((grayscale_close>=0)||(saturation_close>=0))
	{
		prior=6;
		goto L;
	}
	else
	{
		prior=5;
		goto L;
	}
}
	}
}//last_cor01.09.16
if(((bunch_mean_hue==22)&&(neighbor_bunch_mean_hue<=18))
   ||(bunch_mean_hue<=18)&&(neighbor_bunch_mean_hue==22))
{//last_cor08.03.17
if((minimum_zone_gray>=1)&&(maximum_zone_gray<=2)&&(min_saturation>=4))
	{

		if(hue_close>0)
		{
			if(hue_difference<=4)
			{
			hue_close=-2;
			}
			else
			{
				hue_close=-1;
			}
		}
		if((grayscale_close>0)||(saturation_close>0))
		{
			prior=5;
			goto L;
		}
		else
		{
			prior=5;
			goto L;
		}
  }
}//last_cor08.03.17
if(((zone_hue==2)&&(zone_hue_bunch==3))||((zone_hue==3)&&(zone_hue_bunch==2)))
{//last_cor21.03.17
if(hue_difference>=3)
{
 if(minimum_zone_gray>=1)
 {
	hue_close=-1;
	if((saturation_close>0)||(grayscale_close>0))
	{
		prior=6;
		goto L;
	}
	else
	{
		prior=5;
		goto L;
	}
 }
}
}//last_cor21.03.17
if(((zone_hue==4)&&(zone_hue_bunch==3))||((zone_hue==3)&&(zone_hue_bunch==4)))
{//last_cor08.03.17
	if((minimum_zone_gray>=1)&&(maximum_zone_gray<=2)&&(min_saturation>=4))
	{
	if(hue_difference>=2)
	{
		if(hue_close>0)
		{
			hue_close=0;
		}
		if((grayscale_close>0)||(saturation_close>0))
		{
			prior=5;
			goto L;
		}
		else
		{
			prior=5;
			goto L;
		}
	}
	}
}//last_cor08.03.17
if(((bunch_mean_saturation<=1)&&(neighbor_bunch_mean_saturation>=3))||
   ((bunch_mean_saturation>=3)&&(neighbor_bunch_mean_saturation<=1)))
{//last_cor05.12.16
	if(grayscale_close<0)
	{
    prior=5;
	goto L;
	}
	else
	{
	if((zone_gray>=2)&&(zone_gray_bunch>=2))
    {
		if((zone_hue==3)||(zone_hue_bunch==3))
		{
if((hue_close>0)||(grayscale_close>0))
	{
		prior=6;
		goto L;
	}
	else
	{
		prior=5;
		goto L;
	}
		}
	}
	}
}//last_cor05.12.16
if(ratio_length1==16)
{//last_cor15.02.17
if((hue_close>0)&&(grayscale_close>0))
{
	if(!saturation_close)
	{
		if((Signif_ratio<7)&&(Density_ratio<=8))
		{
			if((saturation_difference<impossible_sat_variation)||
				(bunch_mean_saturation>neighbor_bunch_mean_saturation))
			{
				saturation_close=3;
				prior=2;
				goto L;
			}
		}
		else
		{
			prior=1;
			goto L;
		}
	}
}
}//last_cor15.02.17
if(ratio_length==16)
{//last_cor15.02.17
if((hue_close>0)&&(grayscale_close>0))
{
	if(!saturation_close)
	{
		if((Signif_ratio>10)&&(Density_ratio>=8))
		{
			if((saturation_difference<impossible_sat_variation)||
				(neighbor_bunch_mean_saturation>bunch_mean_saturation))
			{
				saturation_close=3;
				prior=2;
				goto L;
			}
		}
		else
		{
			prior=1;
			goto L;
		}
	}
}
}//last_cor15.02.17
				 if((zone_hue==3)||(zone_hue_bunch==3))
				 {//last_cor27.01.16
					 {//last_cor09.02.17
						 if((min_saturation==2)&&(max_saturation==3))
						 {
							 if(gray_difference>possible_gray_variation)
							 {
								 grayscale_close=-1;
								 if((saturation_close>0)||(hue_close>0))
							     {
								 prior=6;
								 goto L;
							     }
								 else
								 {
								 prior=5;
								 goto L;
								 }
							 }

						 }
					 }//last_cor09.02.17

					 if((bunch_mean_saturation<=2)&&(neighbor_bunch_mean_saturation>=4))
					 {//last_cor05.12.16
if((zone_gray>=2)&&(zone_gray_bunch>=2))
{
	if((hue_close>0)||(grayscale_close>0))
	{
		prior=6;
		goto L;
	}
	else
	{
		prior=5;
		goto L;
	}
}
					 }//last_cor05.12.16
 if((bunch_mean_saturation>=4)&&(neighbor_bunch_mean_saturation<=2))
					 {//last_cor05.12.16
if((zone_gray>=2)&&(zone_gray_bunch>=2))
{
	if((hue_close>0)||(grayscale_close>0))
	{
		prior=6;
		goto L;
	}
	else
	{
		prior=5;
		goto L;
	}
}
					 }//last_cor05.12.16
					 if(((bunch_mean_hue<=16)&&(neighbor_bunch_mean_hue>=20)&&(maximum_zone_gray>=2))||
   ((bunch_mean_hue<=16)&&(neighbor_bunch_mean_hue>=21)&&(maximum_zone_gray>=1))||
   ((bunch_mean_hue>=20)&&(neighbor_bunch_mean_hue<=16)&&(maximum_zone_gray>=2))||
  ((bunch_mean_hue>=21)&&(neighbor_bunch_mean_hue<=16)&&(maximum_zone_gray>=1)) )
{//last_cor20.09.16
	if(min_saturation>=3)
	{//ms
	hue_close=-1;
	    if((saturation_close>0)||(grayscale_close>0))
	     {
		prior=6;
		goto L;
	     }
	      else
	     {
		prior=5;
		goto L;
	     }
	}//ms
}//last_cor20.09.16
if((saturation_close<0)&&(min_saturation==1))
	{//last_cor01.07.16
if((hue_close<=0)||(grayscale_close<=0))
{
	prior=5;
	goto L;
}
else
{
	prior=6;
	goto L;
}
	}//last_cor01.07.16
					 if(!hue_close)
					 {//hc0
						 if((zone_hue==3)&&(zone_hue_bunch==3))
{//last_cor20.04.16
if((neighbor_bunch_mean_hue>=10)&&(neighbor_bunch_mean_hue<=16)&&(bunch_mean_hue>=10)&&
   (bunch_mean_hue<=16))
{//last_cor12.07.16
	if((saturation_close>0)&&(grayscale_close>0))
	{
		if(hue_difference<=3)
		{
			hue_close=1;
			prior=1;
			goto L;
		}
		else
		{
        if(hue_difference<=5)
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
}//last_cor12.07.16
		  //{//last_cor12.07.16

	  if((neighbor_bunch_mean_hue<=16)||(bunch_mean_hue<=16))
		  {//last_cor01.07.16
		  if((neighbor_bunch_mean_hue>=20)||(bunch_mean_hue>=20))
		  {
			  if(hue_difference>=5)
			  {//hd>=
if((saturation_close<=0)||(grayscale_close<=0))
{
prior=5;
goto L;
}
else
{
prior=6;
goto L;
}
			  }//hd>=
		  }
		  }//last_cor01.07.16

	          if(grayscale_close==1)
			  {//last_cor22.04.16
				  if(!hue_close)
				  {//hc0
  if((neighbor_bunch_mean_hue<=16)&&(bunch_mean_hue<=16))
  {//nhmh=16
	  if(hue_difference<=3)
	  {
		  hue_close=1;
	  }

	  if(hue_close==1)
	  {
		  if(!saturation_close)
		  {
			  if((zone_gray_bunch>=3)&&(zone_gray_bunch<=4))
			  {
				  if(min_saturation>=2)
				  {
					  if(saturation_difference==2)
					  {
						  saturation_close=2;
						  prior=2;
						  goto L;
					  }
					  else
					  {
						  if(saturation_difference==3)
					     {
                         saturation_close=3;
						  prior=2;
						  goto L;
						 }
					  }
				  }
			  }
		  }
	  }
  }//nhmh16

				  }//hc0
			  }//last_cor22.04.16
	if(((neighbor_bunch_mean_hue>=19)&&(bunch_mean_hue<=16))||
			 ((neighbor_bunch_mean_hue<=16)&&(bunch_mean_hue>=19)))
	{//cond0
		if((neighbor_bunch_mean_hue>=19)&&(zone_gray>=3)&&(neighbor_bunch_mean_saturation<=2))
		{//cond1
			if(bunch_mean_saturation>=3)
			{
				if(hue_difference==3)
				{
				hue_close=4;
				}
				else
				{
					if(hue_difference<=5)
					{
                hue_close=5;
					}
					else
					{
						hue_close=-1;
					}
				}
				if((saturation_close>0)&&(grayscale_close>0))
				{
					if(hue_close>0)
					{
						prior=7;
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
					prior=5;
					goto L;
				}

			}
		}//cond1
			if((bunch_mean_hue>=19)&&(zone_gray_bunch>=3)&&(bunch_mean_saturation<=2))
		{//cond2
			if(neighbor_bunch_mean_saturation>=3)
			{
				if(hue_difference==3)
				{
				hue_close=4;
				}
				else
				{
					if(hue_difference<=5)
					{
                hue_close=5;
					}
					else
					{
						hue_close=-1;
					}
				}
				if((saturation_close>0)&&(grayscale_close>0))
				{
					if(hue_close>0)
					{
						prior=7;
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
					prior=5;
					goto L;
				}

			}
		}//cond2
	}//cond0
	if(((neighbor_bunch_mean_hue>=20)&&(bunch_mean_hue<=17))||
			 ((neighbor_bunch_mean_hue<=17)&&(bunch_mean_hue>=20)))
	{//cond01
		if((neighbor_bunch_mean_hue>=20)&&(zone_gray>=2)&&(neighbor_bunch_mean_saturation<=3))
		{//cond11
			if((bunch_mean_saturation>=4)&&(neighbor_bunch_mean_gray>bunch_mean_gray)&&
				(gray_difference>=possible_gray_variation/2))
			{
				if(hue_difference==3)
				{
				hue_close=4;
				}
				else
				{
					if(hue_difference<=5)
					{
                hue_close=5;
					}
					else
					{
						hue_close=-1;
					}
				}
				if((saturation_close>0)&&(grayscale_close>0))
				{
					if(hue_close>0)
					{
						prior=7;
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
					prior=5;
					goto L;
				}

			}
		}//cond11

			if((bunch_mean_hue>=20)&&(zone_gray_bunch>=2)&&(bunch_mean_saturation<=3))
		{//cond21
	if((neighbor_bunch_mean_saturation>=4)&&(bunch_mean_gray>neighbor_bunch_mean_gray)&&
				(gray_difference>=possible_gray_variation/2))
			{
				if(hue_difference==3)
				{
				hue_close=4;
				}
				else
				{
					if(hue_difference<=5)
					{
                hue_close=5;
					}
					else
					{
						hue_close=-1;
					}
				}
				if((saturation_close>0)&&(grayscale_close>0))
				{
					if(hue_close>0)
					{
						prior=7;
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
					prior=5;
					goto L;
				}

			}
		}//cond21
	}//cond01
	if(((neighbor_bunch_mean_hue>=20)&&(bunch_mean_hue<=18))||
			 ((neighbor_bunch_mean_hue<=18)&&(bunch_mean_hue>=20)))
	{//cond02
if((neighbor_bunch_mean_hue>=20)&&(zone_gray>=2)&&(neighbor_bunch_mean_saturation<=3))
{//cond12
	if((saturation_difference>=3)&&(neighbor_bunch_mean_saturation<bunch_mean_saturation)
				&&(neighbor_bunch_mean_gray>bunch_mean_gray)&&
(zone_gray_bunch<=1)&&(3*gray_difference>=2*possible_gray_variation))
	{//cond13
				saturation_close=-2;
				grayscale_close=-2;
				if(hue_close>0)
				{
					prior=6;
					goto L;
				}
				else
				{
                  prior=5;
					goto L;
				}
	}//cond13
}//cond12
if((bunch_mean_hue>=20)&&(zone_gray_bunch>=2)&&(bunch_mean_saturation<=3))
{//cond22
	if((saturation_difference>=3)&&(bunch_mean_saturation<neighbor_bunch_mean_saturation)
				&&(bunch_mean_gray>neighbor_bunch_mean_gray)&&
				(gray_difference>=possible_gray_variation))
	{//cond23
				saturation_close=-2;
				grayscale_close=-2;
				if(hue_close>0)
				{
					prior=6;
					goto L;
				}
				else
				{
                  prior=5;
					goto L;
				}
	}//cond23
}//cond22
	}//cond02
}//last_cor20.04.16
		if((min_saturation>=4)&&(minimum_zone_gray>=1))
		 {
		 if(((neighbor_bunch_mean_hue>=19)&&(corrected_bunch_mean_hue<16))||
			 ((neighbor_bunch_mean_hue<16)&&(bunch_mean_hue>=19)))
		 {
			 hue_close=-1;
			 if((saturation_close>0)&&(grayscale_close>0))
			 {
				 prior=6;
				 goto L;
			 }
			 else
			 {
				 prior=5;
				 goto L;
			 }
		 }
		}
						 if(!difference_hue_zone)
						 {//dhz0
							 if((hue_difference<=4)&&(min_saturation>=3))
							 {//hdms3
								    if(max_hue<=18)
								    {
								 if((Intersection_hue<3)||(ratio_length_hue<=2)||(joint_hue_range<=4))
								 {
									 hue_close=1;
								 }
								 else
								 {
	if((hue_difference<=3)&&(neighbor_bunch_mean_hue<=16)&&(bunch_mean_hue<=16))
	{
		hue_close=1;
	}
	else
	{
	 hue_close=2;
	}
								 }
								    }
									else
									{//e18
if(max_hue<=20)
{
	if(hue_difference<=3)
	{
		hue_close=1;
	}
	else
	{
if(hue_difference<=5)
	{
		hue_close=2;
	}
	}
}

									}//e18
							 }//hdms3
						 }//dhz0
					 }//hc0
					         if(!saturation_close)
					         {
					 if(saturation_difference<=imp_sat_var)
					 {
						 if((max_saturation>=6)&&(min_saturation>=3))
						 {
							 saturation_close=2;
						 }
						 else
						 {//last_cor28.01.16
							 if(grayscale_close>0)
							 {
								 if(max_saturation>=5)
								 {
if(((bunch_mean_gray>neighbor_bunch_mean_gray)&&(bunch_mean_saturation<neighbor_bunch_mean_saturation)
   )||
   ((bunch_mean_gray<neighbor_bunch_mean_gray)&&(bunch_mean_saturation>neighbor_bunch_mean_saturation)
   ))
{
	if(min_saturation>=2)
	{
		if((length_ratio>=12)||(length_ratio<=3))
		{
			saturation_close=2;
			prior=2;
			goto L;
		}
		else
		{
			saturation_close=3;
			prior=3;
			goto L;
		}
	}
}
								 }
							 }
						 }//last_cor28.01.16
					 }

				 }//last_cor27.01.16							 }
if((hue_close==1)&&(saturation_close==1))
					 {
						 if((grayscale_close>0)&&(grayscale_close<=2))
						 {
						 prior=1;
						 goto L;
						 }
					 }
else
{

 if((hue_close==1)&&(saturation_close>0)&&(saturation_close<=2)&&(grayscale_close>0))
					 {
						 prior=2;
						 goto L;
					 }
}
				 }//last_cor27.01.16
if(hue_close==1)
{//last_cor24.11.15
	if(grayscale_close==1)
	{//last_cor12.01.16
		      if((saturation_close>0)&&(saturation_close<=2))
		      {
			   prior=1;
			   goto L;
		       }
		       else
		       {
				   if(zone_hue==3)
				   {//last_cor27.06.16
					   if(min_saturation>=2)
					   {
						   if((saturation_difference<imp_sat_var)&&(saturation_difference<=2))
						   {
							   if(!saturation_close)
							   {
								   saturation_close=2;
								   prior=1;
								   goto L;
							   }
						   }
					   }
				   }//last_cor27.06.16
		if(min_saturation>=5)
		{
			if(zone_hue==3)
			{
				if(saturation_difference<imp_sat_var)
				{
                 prior=1;
				 saturation_close=2;
				 goto L;
				}
			}
		}
			  }
	}//last_cor12.01.16
	if(saturation_close==1)
	{//last_cor26.11.15sc1
if(grayscale_close==1)
	{//last_cor26.11.15gc1
		prior=1;
		goto L;
	}//last_cor26.11.15gc1
else
{//egc1
	if(grayscale_close>0)
	{//gc>0
		prior=1;
		goto L;
	}//gc>0
	else
	{//egc>0
		if(zone_hue==3)
		{//zh3
			if(min_saturation>=4)
			{//ms5
if(gray_difference<=possible_gray_variation)
{//last_cor01.12.15gd<p

                    if(gray_ntersection<3)
					{//gn<3
						grayscale_close=1;
						prior=1;
						goto L;
					}//gn<3
					else
					{//egn<3
                        grayscale_close=2;
						prior=1;
						goto L;
					}//egn<3
}//last_cor01.12.15gd<
else
{//egd<
	//if(min_saturation>=8)
	//{//ms8
		if(maximum_zone_gray>=2)
		 {//mzg2
if(gray_difference<=(possible_gray_variation +(impossible_gray_variation-possible_gray_variation)/4))
{
	grayscale_close=2;
	prior=2;
	goto L;
}
  if(gray_ntersection<3)
  {//gn3
     if((ratio_length1_gray>=4)&&(ratio_length_gray>=4))
      {//rlg
if(gray_difference<=(possible_gray_variation +(impossible_gray_variation-possible_gray_variation)/3))
{
	grayscale_close=2;
	prior=2;
	goto L;
}
else
{//last_cor27.01.16
if(gray_difference<
   (possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
	{//last_cor29.01.16
		if(length_ratio<=4)
		{
			grayscale_close=2;
			prior=2;
			goto L;
		}
if(gray_ntersection<=1)
{
	grayscale_close=4;
	prior=3;
	goto L;
}
	}
}
      }//rlg
  }//gn3
		 }//mzg2
	//}//ms8

}//egd<

if(gray_difference<=(possible_gray_variation +(impossible_gray_variation-possible_gray_variation)/2))
				{//gd<=p
					if(gray_ntersection<3)
					{//last_cor30.11.15gn3
						if((ratio_length1_gray>=4)&&(ratio_length_gray>=4))
						{//rl4
							if(maximum_zone_gray>=2)
							{//mzg2
								if((length_ratio>=12)||(length_ratio<=4))
								{//lr12
if(gray_difference<=(possible_gray_variation +(impossible_gray_variation-possible_gray_variation)/3))
{
	grayscale_close=2;
	prior=2;
	goto L;
}
								}//lr12
								else
								{//elr12
									if(gray_difference<=possible_gray_variation)
									{//gdpgv
										grayscale_close=2;
	                                    prior=1;
	                                    goto L;
									}//gdpgv
if((ratio_length1_gray>=6)||(ratio_length_gray>=6))
						{//rl1g
if(gray_difference<=(possible_gray_variation +(impossible_gray_variation-possible_gray_variation)/3))
{
	grayscale_close=2;
	prior=2;
	goto L;
}
                        }//rl1g
								}//elr12
							}//mzg2
						}//rl4
					}//last_cor30.11.15gn3
if(gray_difference<=(possible_gray_variation +(impossible_gray_variation-possible_gray_variation)/2))
{//last_cor29.01.16
	if(length_ratio<=4)
	{
		prior=2;
		grayscale_close=2;
		goto L;
	}
}
else
{
					prior=3;
					grayscale_close=3;
					goto L;
}
				}//gd<=p
			}//ms5
		}//zh3
	}//egc>0
}//egc1
	}//last_cor26.11.15sc1
	else
	{//last_cor14.01.15
		if(saturation_close==2)
		{
			if(grayscale_close==1)
			{
				prior=1;
				goto L;
			}
			else
			{
				if(grayscale_close==2)
				{
					prior=2;
					goto L;
				}
				else
				{
if(((Signif_ratio>=12)||(Signif_ratio<=3))&&((length_ratio>=11)||(length_ratio<=4))&&
   ((Density_ratio>=11)||(Density_ratio<=4)))
					{
					if(gray_difference<impossible_gray_variation)
					{
						if(grayscale_close<0)
						{
							if(maximum_zone_gray>=2)
		                   {//mzg>=2
                          if(zone_hue==3)
                          {//zh3
grayscale_close=4;
prior=2;
goto L;
						  }
							}
						}
					}
					}
					if(!grayscale_close)
					{
						if(max_saturation>=6)
	                    {//ms7//last_cor28.01.16
		if(maximum_zone_gray>=2)
		{//mzg>=2
if(zone_hue==3)
{//zh3
	if(gray_difference<=possible_gray_variation)
	{
    grayscale_close=2;
	prior=2;
	goto L;
	}
	else
	{//egdpvlast_cor28.01.16
		if(min_saturation>=5)
	   {//ms5
if(gray_difference<=(possible_gray_variation +(impossible_gray_variation-possible_gray_variation)/3))
{
	grayscale_close=2;
	prior=2;
	goto L;
}
else//last_cor15.01.16
{//last_cor15.01.16

if(gray_difference<=(possible_gray_variation +(impossible_gray_variation-possible_gray_variation)/2))
{//gd<=
	if(gray_ntersection<3)
	{//gn<3
		if((ratio_length_gray>=6)||(ratio_length1_gray>=6))
		{
	grayscale_close=3;
	prior=2;
	goto L;
		}
		else
		{
		grayscale_close=3;
	    prior=3;
	    goto L;
		}
	}//gn<3
}//gd<=

}//last_cor15.01.16
		}//ms5
   }//egdpvlast_cor28.01.16
}//zh3
		}//mzg>=2
						}
					}
				}
			}
		}
	}//last_cor14.01.15
	if(grayscale_close==1)
	{//last_cor25.11.15gc1
		if(saturation_close==1)
		{//sc1
			prior=1;
			goto L;
		}//sc1
		if(!saturation_close)
		{//sc0
if(zone_hue==3)
{//zh3
	if(min_saturation>=4)
	{//ms4
		if(saturation_difference<=impossible_sat_variation)
		{//sd<
			saturation_close=2;
			prior=1;
			goto L;
		}//sd<
	}//ms4
}//zh3
		}//sc0
	}//last_cor25.11.15gc1
	if(zone_hue==3)
	{//zh3
		if(min_saturation>=6)
		{//ms6
			if(!saturation_close)
			{//last_cor25.11.15sc0
				saturation_close=2;
				if(grayscale_close==1)
				{//last_cor13.01.16
					prior=1;
					goto L;
				}//last_cor13.01.16
				else
				{
					if(grayscale_close==2)
					{
				if((ratio_length1_gray>=5)&&(ratio_length_gray>=5))
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
				if(!grayscale_close)
				{//gc0
					if((maximum_zone_gray>=2)&&(maximum_zone_gray<=5))
					{
if(gray_difference<=(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/3))
{
	grayscale_close=2;
	prior=2;
	goto L;
}
					}
				}//gc0
			}//last_cor25.11.15
			if((length_ratio>=12)||(length_ratio<=3))
			{//lr12
				if(saturation_close>0)
				{//sc0
					if(grayscale_close>0)
					{
						prior=1;
						goto L;
					}
					else
					{//egc0

						if((gray_ntersection!=3)&&(!grayscale_close)&&(maximum_zone_gray<=3))
						{
                        grayscale_close=2;
						prior=2;
						goto L;
						}
					}//egc0
				}//sc0
				/*else
				{//esc0
;//start next day
				}//esc0*/
			}//lr12
		}//ms6
	}//zh3
}//last_cor24.11.15
if(!saturation_close)
{//last_cor16.06.15
	if(saturation_difference<=poss_sat_variation)
		{//last_cor17.08.15
			saturation_close=1;
		}//last_cor17.08.15
	else
	{//elc
		if(saturation_difference>=imp_sat_var)
		{
			if(min_saturation<=5)
			{//last_cor24.11.15
			saturation_close=-1;
			}//last_cor24.11.15
		}
		else
		{//last_cor20.08.15
			if(saturation_difference<=2)
			{
				if(min_saturation>=3)
				{
				saturation_close=2;
				}
			}
		}//last_cor20.08.15
	}//elc

	if(hue_close==1)
	{//last_cor31.08.15hc1
		if(grayscale_close==1)
{//last_cor31.08.15gc1
if(saturation_close>0)
{
	prior=1;
	goto L;
}
else
{//esc>
	if(saturation_difference<imp_sat_var)
	{//sd<isv
		if(saturation_difference<=2)
		{
			if(minimum_zone_gray>=5)
			{
				saturation_close=2;
				prior=1;
				goto L;
			}
		}
	}//sd<isv
}//esc>
}//last_cor31.08.15gc1
    }//last_cor31.08.15hc1
}//last_cor16.06.15
	if(saturation_close==1)
	{//last_cor24.08.15
if(hue_close==1)
{//hc1
		if(grayscale_close==1)
		{
			prior=1;
			goto L;
		}
		else
		{//egc1
			   if(!grayscale_close)
			   {//gc0
		    if(minimum_zone_gray>=5)
		    {//mzg5
				if(gray_difference<=possible_gray_variation)
				{
				grayscale_close=2;
				prior=1;
				goto L;
				}
				else
				{//egd<=
					   if((max_hue_zone<=4)&&(min_hue_zone>=3))
						{//mz
					if(gray_difference<=impossible_gray_variation)
					{//gd
if(gray_difference<=(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/3))
{
grayscale_close=3;
prior=2;
goto L;
}
					}//gd
					    }//mz
				}//egd<=
		    }//mzg5
			   }//gc0
		}//egc1
}//hc1
else
{//ehc1
	if(!hue_close)
	{//hc0
if(minimum_zone_gray>=5)
{//mzg5
   if((max_hue_zone<=4)&&(min_hue_zone>=3))
   {//mhz43
       if(!difference_hue_zone)
      {//dhz0
	   if(hue_difference<=4)
	   {//hd4
		   hue_close=1;
		   if(!grayscale_close)
		   {
			   if(gray_difference<=possible_gray_variation)
			   {
				   grayscale_close=2;
				   prior=1;
				   goto L;
			   }
		   }
	   }//hd4
      }//dhz0
      else
      {//edhz0
		  if(hue_difference<=4)
	   {//hd<=4
		   if((min_hue_zone>=3)&&(max_hue_zone<=4))
		   {//last_cor16.12.15
			   if(hue_difference<=3)
			   {
				   hue_close=1;
				   if(grayscale_close==1)
				   {
					   prior=1;
					   goto L;
				   }
			   }
		   }//last_cor16.12.15
		   if(!hue_close)
		   {//last_cor16.12.15
		   hue_close=2;
		   if(grayscale_close==1)
				   {
					   prior=1;
					   goto L;
				   }
		   }//last_cor16.12.15
		   if(!grayscale_close)
		   {
			   if(gray_difference<=possible_gray_variation)
			   {
				   grayscale_close=2;
				   prior=2;
				   goto L;
			   }
		   }
	   }//hd<=4
      }//edhz0
   }//mhz43
}//mzg5
	}//hc0
}//ehc1
	}//last_cor24.08.15
	else
	{//last_cor28.08.15
		if(saturation_close==2)
		{//sc2
if(minimum_zone_gray>=6)
{//mzg6last_cor24.08.15
	    if(!hue_close)
	    {
			if(!difference_hue_zone)
			{
				if(hue_difference<=3)
				{
					hue_close=1;
				}
				else
				{
					if((min_hue_zone>=3)&&(max_hue_zone==4))
					{
						if(hue_difference<=4)
						{
							hue_close=1;
						}
						else
						{
						if(hue_difference<=6)
						{
							hue_close=2;
						}
						}
					}
				}
			}
			else
			{
				if(difference_hue_zone<=1)
				{
					if((min_hue_zone>=3)&&(max_hue_zone==4))
					{
						if(hue_difference<=4)
						{
							hue_close=1;
						}
						else
						{
                         if(hue_difference<=6)
						{
							hue_close=2;
						}
						}
					}
				}
			}
	    }
	if(!grayscale_close)
	{
		if(gray_difference<=possible_gray_variation)
		{
			grayscale_close=2;
		}
	}
	if((hue_close==1)&&(grayscale_close==2)&(saturation_close==2))
	{
		prior=2;
		goto L;
	}
}

		}
	}
	if(grayscale_close==1)
	{//gsc1
		if(minimum_zone_gray>=6)
		{//mzg6last_cor24.08.15
			if(saturation_close>=0)
			{//sc0
				if(min_saturation==1)
				{//ms1
                           if((max_hue_zone==4)&&(min_hue_zone==3))
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
							prior=2;
							goto L;
						}

						}
						    }
				}//ms1
			}//sc0
		}//mzg6
		if(saturation_close==1)
		{//sc1
			if(hue_close==1)
			{
				prior=1;
				goto L;
			}
			else
			{//e
				if(!hue_close)
				{//hc0
					if(minimum_zone_gray>=6)
					{//mzg
						if((max_hue_zone==4)&&(min_hue_zone==3))
						{
						if(hue_difference<=3)
						{
							hue_close=1;
							prior=1;
							goto L;
						}
						}
					}//mzg6
				}//hc0
			}//e
		}//sc1
	}//gsc1
	else
	{
		if(grayscale_close==2)
		{
			    if(!difference_hue_zone)
				{
					if(max_hue_zone==4)
					{
			if(hue_difference<=4)
			{
if(minimum_zone_gray>=6)
{
	hue_close=1;
	if(saturation_close==1)
	{
		prior=1;
		goto L;
	}
	else
	{
	   if(saturation_close>0)
	   {
		   if((length_ratio>=14)||(length_ratio<=2))
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
}
			}
					}
				}
		}
	}
	if(min_saturation>=5)
	{//last_cor17.06.15
		if(grayscale_close==1)
		{
			if(!difference_hue_zone)
			{
				if(!hue_close)
				{
					if((zone_hue>=3)&&(zone_hue<=4))
					{
						if(hue_difference<=4)
						{
							hue_close=1;
						}
					}
				}
				if((hue_close==1)&&(saturation_close==1))
				{
					prior=1;
					goto L;
				}
			}
		}
	}//last_cor17.06.15
	if(zone_hue==3)
	{//last_cor19.08.15
		   if(min_saturation>=3)
		   {//ms3
		if((hue_close==1)&&(grayscale_close>0))
		{
			if(saturation_close==1)
			{
			prior=1;
			goto L;
			}

		}
		else//last_cor12.10.15
		{//hc1gc
			if(grayscale_close<0)
			{//gc0
				if(maximum_zone_gray>=4)
				{//mzg4
					if((hue_close>0)&&(saturation_close>0))
					{
					prior=7;
					goto L;
					}
					else
					{//ehcsc
						if(hue_close>0)
						{
							prior=6;
							goto L;
						}
						else
						{
							prior=5;
							goto L;
						}
					}//ehcsc
				}//mzg4
		    }//gc0
		}//last_cor12.10.15
		   }//hc1gc

	}//last_cor19.08.15
//}//last_cor16.06.15jump!
if(hue_close==1)
{//last_cor19.05.15
	if(grayscale_close==1)
	{//gc=1
		if(saturation_close>=0)
		{//sc>0
			if((saturation_close>=1)&&(saturation_close<=2))
			{
				prior=1;
				goto L;
			}
			else
			{//esc
				if(!saturation_close)
				{//sc=0
					if(min_saturation>=1)
					{//ms>=1
						if(saturation_difference<=2)
						{//sd<=2
							if(!difference_hue_zone)
							{
								if(zone_hue==4)
								{
if(minimum_zone_gray>=5)
{
	saturation_close=2;
	prior=1;
	goto L;
}
else
{//last_cor22.05.15
if(minimum_zone_gray>=4)
{
   if(strip_number>NumStrip/2)
    {
saturation_close=2;
	prior=1;
	goto L;
    }
}
}//last_cor22.05.15
								}
							}
						}//sd<=2
					}//ms>=1
				}//sc=0
			}//esc
		}//sc>=0
	}//gc=1
	else
	{
		if(grayscale_close==2)
		{
			if(saturation_close>=0)
		{//sc>0
			if(saturation_close==1)
			{
				prior=1;
				goto L;
			}
			else
			{//esc
          if(saturation_close==2)
		  {
          prior=2;
		   goto L;
		  }
		  else
		  {
				if(!saturation_close)
				{//sc=0
					if(min_saturation>=1)
					{//ms>=1
						if(saturation_difference<=2)
						{//sd<=2
							if(!difference_hue_zone)
							{
								if(zone_hue==4)
								{
if(minimum_zone_gray>=5)
{
	saturation_close=2;
	prior=2;
	goto L;
}
								}
							}
						}//sd<=2
					}//ms>=1
				}//sc=0
		  }
			}//esc
		}//sc>=0
		}
	}
}//last_cor19.05.15
//16.12.15

					   if(minimum_zone_gray>=2)
					   {//last_cor21.04.15
						   if(min_saturation==1)
						   {//last_cor04.06.15
							   if(saturation_difference<=1)
							   {
								   saturation_close=1;
							   }
							   else
							   {
								   if(saturation_difference==2)
								   {
									   if(grayscale_close==1)
									   {//last_cor11.06.15
									   saturation_close=2;
									   }//last_cor11.06.15
								   }
							   }
							   if(!difference_hue_zone)
							   {
								   if((zone_hue_bunch==3)||(zone_hue_bunch==4))
								   {
								   if(hue_difference<=4)
								   {
									   hue_close=1;
								   }
								   }
								   else
								   {
									   if(hue_difference<=3)
								       {
										    hue_close=1;
									   }
								   }
							   }
							   if(saturation_close>=0)
							   {
								   if(grayscale_close==1)
								   {
									   if(hue_close==1)
									   {
										   if(saturation_close==1)
										   {
										   prior=1;
										   goto L;
										   }
										   else
										   {
				 if(gray_difference<possible_gray_variation/2)
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
								   }
							   }
						   }//last_cor04.06.15
						   if((saturation_close==1)&&(hue_close==1))
						   {
							   if((grayscale_close>=1)&&(grayscale_close<=2))
							   {
								   prior=1;
								   goto L;
							   }
							   else
							   {
								   if(!grayscale_close)
								   {
									   if((zone_hue==3)&&(zone_hue_bunch==3))
									   {//zh3
										   if(gray_difference<possible_gray_variation)
										   {//eror!28.07.15
                                            grayscale_close=2;
											prior=1;
											goto L;
										   }
										   else
										   {//e
											   if(gray_ntersection<3)
											   {
if(gray_difference<(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
{
	grayscale_close=2;
	prior=2;
	goto L;
}
											   }
										   }//e
									   }//zh3
									   else
									   {
				   if(gray_difference<possible_gray_variation)
					{//last_cor28.07.15
										   if((length_ratio>=13)||(length_ratio<=3))
										   {
                                            grayscale_close=2;
											prior=1;
											goto L;
										   }
										   else
										   {
									        grayscale_close=2;
											prior=2;
											goto L;
										   }
					 }//last_cor28.07.15
									   }
								   }
							   }
						   }
					   }//last_cor21.04.15error!
if((zone_gray<=3)&&(zone_gray_bunch<=3))
		{//last_cor11.04.14
			if(max_saturation==3)
			{//last_cor16.04.15
				if(!saturation_close)
				{
if(saturation_difference==2)
{
	if(gray_difference<possible_gray_variation/4)
	{
saturation_close=2;
if((hue_difference<=4)&&(!difference_hue_zone))
{
	hue_close=1;
	prior=1;
	goto L;
}
	}
}
				}

			}
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
					if((grayscale_close>=1)&&(grayscale_close<=2)&&(saturation_close==1))
					{//gcsc1//last_cor05.11.14
if(difference_hue_zone<=1)
{//dhz1
	if(std::max(zone_hue_bunch,zone_hue)==4)
	{//std::max
		if((grayscale_close==1)&&(!gray_ntersection))
		{//last_cor27.11.14
			if((length_ratio<3)&&(ratio_length1>4)&&(ratio_length<2))
			{
				if(max_saturation<=4)
				{
				if(hue_difference<=7)
		        {
			    hue_close=1;
			    prior=1;
			    goto L;
		        }
				}
			}
		}
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
		{//last_cor11.04.15
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
		{//ms>=3
           if(difference_hue_zone>1)
		    {
            hue_close=-3;
		    }
		   else
		   {//edhz>1
			   if(difference_hue_zone==1)
			   {//dhz1
                  if(hue_difference>10)
			      {
					  hue_close=-2;
			      }
				  else
				  {//ehd10
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
				  }//ehd10
			   }//dhz1
		   }//edhz>1
		}//ms>=3
		else//last_cor25.05.15
		{
        if(max_saturation>=3)
			if(difference_hue_zone>=1)
			{
				if(!hue_close)
				{
					if(Intersection_hue==3)
					{
						if(min_hue_zone==2)
						{
							if(hue_difference>=4)
							{
								hue_close=-2;
								if(!grayscale_close)
								{
									if(gray_ntersection==3)
									{
										grayscale_close=-2;
										prior=5;
										goto L;
									}
									else
									{
										prior=6;
										goto L;
									}
								}
							}
						}
					}
				}
			}
		}//last_cor25.05.15
	}//last_cor15.10.14
if(grayscale_close==1)
{//last_cor25.05.15
if(saturation_close==1)
{
	if((!difference_hue_zone)&&(zone_hue>=3)&&(zone_hue<=4))
	{//last_cor17.06.15
		if(minimum_zone_gray>=4)
		{
			if(hue_difference<=4)
			{
				prior=1;
				hue_close=1;
				goto L;
			}
			else
			{
				if(hue_difference<=7)
			    {
					prior=2;
					hue_close=2;
				    goto L;
				}
			}
		}
	}
}
}//last_cor25.05.15
else
{
	if(difference_hue_zone==1)
	{
		if(minimum_zone_gray>=5)
		{
			if(zone_hue==4)
			{
				if(hue_difference<=3)
				{
					prior=1;
					goto L;
				}
			}
		}
	}
}
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
			   if(!hue_close)
				{//last_cor16.03.15
			hue_close=1;
			   }//last_cor16.03.15
		}
		else
		{//e
			if((difference_hue_zone<=1)&&(hue_difference<=6))
			{//last_cor22.04.14
				if(!hue_close)
				{//last_cor16.03.15
				hue_close=2;
				}//last_cor16.03.15
		    }
			else
			{
				if((difference_hue_zone<=1)&&(hue_difference<=8))
		        {
					if(!hue_close)
				    {//last_cor16.03.15
			        hue_close=3;
					}//last_cor16.03.15
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
			if(!hue_close)
			{//last_cor16.03.15
			hue_close=1;
			}//last_cor16.03.15
		}
		else
		{
			if((difference_hue_zone<=1)&&(hue_difference<=4))
			{
				if(!hue_close)
			    {//last_cor16.03.15
				 hue_close=2;
				}//last_cor16.03.15
		    }
			else
			{
				if((difference_hue_zone<=1)&&(hue_difference<=6))
		        {
				   if(!hue_close)
			       {//last_cor16.03.15
			        hue_close=3;
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
				else
				{//last_cor06.03.15
					if((hue_close==1)&&(hue_difference<=3)&&(max_saturation>=4))
					{
						if((saturation_close>=1)&&(saturation_close<=2))
						{
							if(grayscale_close==1)
							{
								prior=1;
								goto L;
							}
							else
							{
							  if(!grayscale_close)
							  {
								  if((!difference_hue_zone)&&(zone_hue==3))
								  {
									  if(gray_difference<possible_gray_variation)
									  {
										  grayscale_close=1;
										  prior=1;
										  goto L;
									  }

								  }
							  }
							}
						}

					}
				}//last_cor06.03.15
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
				if(!hue_close)
			   {//last_cor16.03.15
			hue_close=2;
			prior=2;
			goto L;
				}
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
					if(!hue_close)
			        {//last_cor16.03.15
			         hue_close=3;
					}

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
{//ezb1
if((zone_gray<=3)||(zone_gray_bunch<=3))
	    {//zgb-18.04.13
if((saturation_difference<imp_sat_var)&&(saturation_difference<=2))
{//sd
	if((saturation_difference<=poss_sat_variation)||(saturation_difference<=1))
	{//last_cor27.05.13and 25.03.14
		if((saturation_difference<poss_sat_variation)||(saturation_difference<=1))
		{//last_cor19.02.15
	saturation_close=1;
		}
	}
	else
	{
		if((min_saturation>1)&&(saturation_difference<=poss_sat_variation))
		{//last_cor19.02.15
	    saturation_close=2;
		}
	}
if(grayscale_close==1)
{//last_cor26.03.14


if(!hue_close)
		{//hc0
			if(hue_difference<=3)
			{//hd<=3
				if(!difference_hue_zone)
				{//!dhz

if(hue_difference==3)
{//hd=3
	                if(zone_hue==1)
					{//zh=1last_cor19.02.15
	    if((!bunch_mean_hue)||(!neighbor_bunch_mean_hue))
	    {//!bmh
	if((zone_gray>=3)||(zone_gray_bunch>=3))
	{//zg3
		if(!saturation_close)
		{//!sc
			if(saturation_difference>poss_sat_variation)
			{
				saturation_close=-1;
				prior=1;
		        goto L;
			}
		}//!sc
	}//zg3
		}//!bmh
		else
		{
			hue_close=2;
		}
                 }//zh=1
                 else
                 {
                 if(zone_hue>=2)
						{
                hue_close=1;
						}
                 }
}//hd=3
else
{
hue_close=1;
}
				}//!dhz
				else
				{//e!dhz
				if(difference_hue_zone==1)
				{//dhz=1
					if((zone_hue>1)&&(zone_hue<4)&&(zone_hue_bunch>1)&&(zone_hue_bunch<4))
					{
						hue_close=2;

					}
					else
					{
                hue_close=3;
					}
				}//dhz=1
				}//e!dhz

			}//hd<=3
		}//hc0
if(saturation_close==1)
{//last_cor26.03.14
	if(hue_close==1)
	{
		prior=1;
		goto L;
	}
	else
	{
       if(hue_close>0)
	    {
			prior=2;
		goto L;
	    }
	}
}//last_cor26.03.14
else
{
	if(saturation_close==2)
{//last_cor26.03.14
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
	   else
	   {
		   if(hue_close==3)
	    {
			prior=3;
		goto L;
	    }
	   }
	}
}//last_cor26.03.14
}
}//last_cor26.03.14
	if(hue_close==1)
	{//last_cor26.03.14
		if((saturation_close>=1)&&(saturation_close<=2))
		{
			if((grayscale_close>=2)&&(gray_difference<=possible_gray_variation))
		    {
             prior=2;
			 goto L;
			}

		}

	}//last_cor26.03.14
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
	if((length_ratio>=12)&&(Density_ratio>=12)&&(Signif_ratio>=13))
	{//last_cor25.03.14
       if(hue_close==1)
		{
			if((saturation_close>=1)&&(saturation_close<=2))
			{
				if(grayscale_close==1)
				{
			prior=1;
			goto L;
			    }
				else
				{
					if(!grayscale_close)
					{
if(gray_difference<=(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
{
grayscale_close=3;
prior=2;
			goto L;
}
					}
				}
			}

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
		  if(!hue_close)
		   {//last_cor12.02.15
			   if(((zone_hue!=2)&&(zone_hue!=1))||(zone_hue_bunch!=2)&&(zone_hue_bunch!=1))
			    {//last_cor15.07.13_17.09.13
			if(hue_difference<=3)
			{//last_cor12.02.15

				prior=2;
				goto L;
			}
			    }//last_cor15.07.13_17.09.13

		   }//last_cor12.02.15
		}//ehc1
			  }//last_cor12.02.15
	}//last_cor28.05.13
}//esd
        }//zgb-18.04.13
}//ezb1
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
		if((saturation_difference>(poss_sat_variation+(imp_sat_var-poss_sat_variation)/2))&&
			(Intersection_saturation>=2)&&(int_length>=2*StripWidth))
	    {//last_cor14.05.14
		saturation_close=-2;
		}
	}
	    }//zgb
}//last cor
if(!saturation_close)
		 {//last_cor08.10.13
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
}//last_cor08.10.13
if((std::max(zone_gray,zone_gray_bunch)<=1)||
   ((std::min(zone_gray,zone_gray_bunch)<=1)&&(grayscale_close>=1)&&(grayscale_close<=2)))
	{//zg1//last_cor28.12.12
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
		if(!saturation_close)
					 {//last_cor28.12.12
						 if((!hue_close)||(!grayscale_close))
						 {//last_cor28.12.13
                         prior=7;
						 goto L;
						 }
					 if(((saturation_distance1<=possible_sat_variation+1)&&(saturation_distance<=possible_sat_variation1+1))||
		((saturation_distance1<impossible_sat_variation)&&(saturation_distance<impossible_sat_variation1)))
     {

		 saturation_close=2;

     }
					 }
					 if(!grayscale_close)
					 {
						 if((!saturation_close)||(!hue_close))
						 {//last_cor28.12.13
                         prior=7;
						 goto L;
						 }
						 if(zone_gray_bunch<=2)
						 {
							 grayscale_close=3;
						 }
					 }//last_cor28.12.12
		         if(!hue_close)
				 {//hue indefinite
					 if((!saturation_close)||(!grayscale_close))
						 {//last_cor28.12.13
                         prior=7;
						 goto L;
						 }
if((!difference_hue_zone)&&((hue_difference<=2)||(hue_difference<=3)&&(node_vicinity)))
				 {//closeness correction

					 hue_close=1;

//last_cor28.12.12
				 }//closeness correction
		if((zone_hue!=1)&&(zone_hue_bunch!=1)&&((std::max(zone_gray,zone_gray_bunch)<=1)))
		{//last_cor08.10.13
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
			if(((zone_gray>1)||(zone_gray_bunch>1))&&(max_saturation>=2))
			{//last_cor26.09.13
			hue_close=-1;
			}
		}
		else
		{
			if((difference_hue_zone>2)||(hue_difference>12))
			{
			  if(((zone_gray>1)||(zone_gray_bunch>1))&&(max_saturation>=2))
			   {//last_cor26.09.13
				hue_close=-2;
			  }
			}
		}
	}
}//e0
		}//last_cor08.10.13
					 }//hue indefinite

if((zone_hue==1)||(zone_hue_bunch==1)&&((std::max(zone_gray,zone_gray_bunch)>=2)))
		{//last_cor08.10.13
			if((hue_difference>=4)&&(difference_hue_zone==1))
			{
				hue_close=-1;
			}
        }//last_cor08.10.13




if((grayscale_close>=1)&&(grayscale_close<=2)&&(saturation_close==1)&&(hue_close==1))
	   {//gh
	 prior=1;
       }//gh
       else
      {//e1
		  if((hue_close>=1)&&(hue_close<=2)&&(grayscale_close>=1)&&(saturation_close>=1)&&(grayscale_close<3))
		  {
		  prior=2;
	      }

		   else
		   {
if((((hue_close==1)&&(grayscale_close==3))||((hue_close==3)&&(grayscale_close==1)))&&(saturation_close>=1))
		  {
		  prior=3;
	      }
  else
  {
if((hue_close>=2)&&(grayscale_close>1)&&(saturation_close>=1))
{
	if(grayscale_close==1)
	{
		prior=2;
	}
	else
	{
if((ratio_length1==16)||(Signif_ratio<=2)&&(Density_ratio<4)&&(length_ratio<=6))
 {//incl1
prior=2;
 }//incl1
	}
}
else
{
     if((hue_close>0)&&(hue_close<=2)&&(grayscale_close>=1)&&(grayscale_close<=2)&&(difference_gray_zone<=1))
	 {//last_cor20.05.15

		 if((ratio_length1==16)||(Signif_ratio<=2)&&(Density_ratio<3)&&(length_ratio<=6))
         {//incl1
         prior=2;
         }//incl1
		 if((saturation_close>=0)&&(grayscale_close==1)&&((!gray_ntersection)||(ratio_length1_gray==16)))
		 {
			 prior=2;
		 }
	 }
		 else
		 {
			 if((hue_close==1)&&(grayscale_close>=1)&&(grayscale_close<=2)&&(difference_gray_zone<=1))
			 {
           if((ratio_length1>=14)||(Signif_ratio<=2)&&(difference_gray_zone<=1)&&(length_ratio<=6)&&(Density_ratio<7)
			   &&(!gray_ntersection))
		   {
		    prior=2;
		   }
			 }
		   else
		   {
			   if((grayscale_close>=1)&&(grayscale_close<=2)&&(Density_ratio<=2)&&(ratio_length1==16)&&(Signif_ratio==0))
			   {
				     if((bunch_density<=3)||(Density_ratio<=1))
					 {
				   if((zone_gray<=1)&&(zone_gray_bunch<=1))
				   {
					   prior=2;
				   }
					 }
			   }
		   }
		 }

}

   }
		  }
      }//e1

	 if((prior>0)&&(prior<=4))
	 {
       if((ratio_length1<5)&&((Signif_ratio>6)||(Density_ratio>6)))
	   {//incl
		prior+=10;
	   }//incl
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
	}//zg1
	else
	{//elsezg1
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
								 else
								 {
if((ratio_length1==16)&&(Signif_ratio<2)&&(Density_ratio<4))
{
	prior=4;
}
								 }
							 }//e<=2
	if((ratio_length1<5)&&((Signif_ratio>6)||(Density_ratio>6)))
	   {//incl
		prior+=10;
	   }//incl
						 goto L;

		}//zhb3
		}//hc1sc1gc0
	if((hue_close==1)&&(grayscale_close==1))
	  {///last_cor80213
		  if((ratio_length1==16)&&(Signif_ratio<2)&&(Density_ratio<2)&&(ratio_length<2))
{
	prior=2;
    goto L;
}
	  }
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
{
	if((difference_hue_zone>=1)&&(Intersection_hue==3)&&(hue_difference>3))
	{//last_cor08.10.13
		if((!grayscale_close)&&(gray_ntersection==3))
		{
			if(saturation_difference>poss_sat_variation)
			{
				if((zone_hue==1)||(zone_hue_bunch==1))
				{
        hue_close=-1;
		prior=5;
		goto L;
				}
			}
		}
	}
if((!difference_hue_zone)&&(hue_difference<=4)||((difference_hue_zone<=1)&&(hue_difference<=3)))
{//last_cor22.0113!!!!!
	if((saturation_close==1)&&(grayscale_close==1))
	{
		if((Density_ratio>=7)&&((Density_ratio<=9)))
		{//last_cor22.01.13
		hue_close=2;
		prior=22;
		goto L;
		}
		else
		{
			if((Signif_ratio<=2)&&(length_ratio<=2)&&(Density_ratio<5))
		{//last_cor22.01.13
		hue_close=2;
		prior=2;
		goto L;
		}
		}
	}
	else
	{
		if((Signif_ratio<=2)&&(length_ratio<=2)&&(Density_ratio<5))
		{
		hue_close=3;
		}
	}

}
else
{
if((hue_difference>=7)||((hue_difference>=5)&&(difference_hue_zone>0)))
       {//last_cor
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
	  }

			}//hue indefinite
                    if(!saturation_close)
					 {//last_cor28.12.12
	if((saturation_distance1<=possible_sat_variation+1)&&(saturation_distance<=possible_sat_variation1+1))
     {//last_cor11.06.15
		 saturation_close=2;
     }
					 }
					if((!grayscale_close)&&(gray_difference<=possible_gray_variation))
					 {//gc0last_cor28.12.12_10.06.14
						 if((zone_gray_bunch<=2)&&(difference_gray_zone<=1))
						 {
							 if(gray_ntersection<=1)
							 {
							 grayscale_close=2;
							 }


						 }
						 else
						 {
if((hue_difference<=1)&&(max_satur_differ<=1)
   &&(gray_difference<(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/4)))
							 {
								 if(gray_ntersection<=1)
								 {
                             grayscale_close=2;
								 }
							 }
						 }
					 }//gc0
if((!difference_hue_zone)&&((hue_difference<=2)||(hue_difference<=2)&&((node_vicinity)||(node_vicinity_neighbor))))
				 {//characteristics correction//last_cor28.12.12
					 hue_close=1;
				 }//characteristics correction


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
if((ratio_length1<13)&&(Signif_ratio<=2)&&(Density_ratio<6)||(length_ratio<6))
	   {
       if((grayscale_close>0)&&(grayscale_close<=2)&&(saturation_close>=0)&&(hue_close>0)&&(hue_close<2))
	 {
	 prior=4;

	 }
       }
    }
}//elsezg1
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
		if((difference_gray_zone<=1)&&(gray_difference<=possible_gray_variation))
		{
			if((gray_ntersection<=1)||(union_gray_length<=possible_gray_variation))
			{//last_cor20.06.14
		prior=2;
		goto L;
			}
		}
	}
}//last cor
     }//e2



       if((prior>0)&&(prior<=4))
	 {
       if((ratio_length1<5)&&((Signif_ratio>6)||(Density_ratio>6)))
	   {//incl
		prior+=10;
	   }//incl
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
		  if(hue_difference<=2)
		  {//last_cor26.09.13
			  hue_close=2;
		  }//last_cor26.09.13
		  if(!saturation_close)
			  {//sc
				  if((max_saturation>=4)&&(zone_gray>=3))
				  {//sd4
					  if(saturation_difference<=imp_sat_var)
					  {
						  if(saturation_difference<=2)
						  {
							  saturation_close=2;
						  }
						  else
						  {
							  saturation_close=3;
						  }
					  }
				  }//sd4
			  }//sc
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
		        else//last_cor07.05.15
				{
					if(hue_close==1)
					{
						if(min_saturation>=5)
						{
							if(minimum_zone_gray>=5)
							{
								if((saturation_close>=1)&&(saturation_close<=2))
								{
									if(!grayscale_close)
									{
										if(gray_difference<=possible_gray_variation)
										{
											grayscale_close=2;
											prior=2;
											goto L;
										}
									}
								}
							}
						}
					}
		        }//last_cor07.05.15
		if(!saturation_close)
			//!?
		{//saturation indefinite
			if((bunch_occurrence>0)&&(Density_ratio>=6)&&(Density_ratio<=10))
				   {//bo
if(Intersection_saturation==3)
					   {//s_i=3
  if((saturation_distance1>=possible_sat_variation)||(saturation_distance>=possible_sat_variation1))
						   {
							   if((ratio_length_satur>=(1+2*poss_sat_variation/3))||
								  (ratio_length_satur>=(1+imp_sat_var/3) ))
							   {//last_cor26_09.13
                                saturation_close=-2;
							   }
							   else
							   {
								   if((ratio_length_satur>=(1+poss_sat_variation/2)))
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
		//!/
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
if(((hue_close>1)&&(hue_close<=2)&&(grayscale_close>=1)&&(saturation_close>=1)&&(grayscale_close<3))||
   ((hue_close==1)&&(saturation_close==1)&&(grayscale_close<=3)&&(grayscale_close>0)))
{
	 prior=2;
}
else
{
	if((((hue_close==1)&&(grayscale_close==3))||((hue_close==3)&&(grayscale_close==1)))&&(saturation_close>=1))
		  {
		  prior=3;
	      }
	else
	{
		if((hue_close==1)&&(grayscale_close==3))
		{
			if((min_saturation>=2)&&(max_satur_differ<=5))
			{//!?
            prior=4;
			goto L;
			}//!?
		}

		if((Signif_ratio<=2)&&(Density_ratio<=4)&&(length_ratio<=2))
		{
			if((grayscale_close>0)&&(grayscale_close<=3)&&(hue_close==1)&&
				(neighbor_bunch_mean_saturation>=6)&&(bunch_mean_saturation>=6))
			{
            prior=2;
			}
		}
if((ratio_length1>13)&&(Signif_ratio<=2)&&(Density_ratio<6)||(length_ratio<6))
	   {
       if((grayscale_close>0)&&(grayscale_close<=2)&&(saturation_close>=0)&&(hue_close>0)&&(hue_close<=3))
	 {
	 prior=4;

	 }
       }
    }
}
if((hue_close==1)&&(saturation_close==1))
{
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
		}
	}
}
     }//e2
		if((prior>0)&&(prior<=4))
	 {
       /*if((ratio_length1<5)&&((Signif_ratio>6)||(Density_ratio>6)))
	   {//incl
		prior+=20;
	   }//incl*/
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
	 if((!grayscale_close)&&(!hue_close))
	 {//last_cor17.07.13
		 if(gray_difference>possible_gray_variation)
		 {
			 if(saturation_close>0)
			 {
				 prior=7;
	             goto L;
			 }
			 else
			 {
				 prior=6;
	             goto L;
			 }
		 }
	 }
	 if((!hue_close)&&(!grayscale_close))
	 {//last_cor02.04.14
		 if(saturation_close==1)
		 {
			 prior=7;
			 goto L;
		 }
		 else
		 {
            prior=6;
			 goto L;
		 }
	 }
	 if(!hue_close)
	 {//last_cor19.02.15
		 if(difference_hue_zone>=1)
		 {
			 if((zone_hue==2)||(zone_hue_bunch==2))
			 {
				 if((bunch_mean_hue<=5)||(neighbor_bunch_mean_hue<=5))
				 {
					 if((zone_gray>=3)||(zone_gray_bunch>=3))
						 {
					 if(hue_difference>=4)
					 {
						     if(max_saturation>=3)
						     {
						 if((saturation_close<=0)||(saturation_difference>poss_sat_variation))
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
					 }
					     }
				 }
			 }
		 }
	 }//last_cor19.02.15
	 else
	 {
		 if(grayscale_close<0)
		 {//last_cor05.03.15
			 if((hue_close>0)&&(saturation_close>0))
			 {
          prior=6;
	      goto L;
			 }
			 else
			 {
			 prior=5;
	         goto L;
			 }

		 }
		 else
		 {
         prior=28;
	     goto L;
		 }
	 }
		}//zg>
	}//1


L:

	if((((prior%10)==0)||(prior%10==4))&&(saturation_close>=-1))
{//last_cor15.02.13prcondlast_cor14.12.16!!!

	if((hue_close==1)&&((grayscale_close==1)||((grayscale_close==2)
		||(
		(gray_ntersection<3)&&(gray_difference<(possible_gray_variation-possible_gray_variation/4))))))
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
					{
					prior=4;
					}
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
	*hue_close_r=hue_close;
	*sat_close_r=saturation_close;
	*gray_close_r=grayscale_close;
return(prior);
}

//-----------------------------------------------------------------------
void

CColorIntervalSelect::Bunch_continuation(int direct,int bunch_number,int bunch_occurrence,int beg_int,
	int end_int,int paint_start,int paint_finish,int* continuation)
{

	int shift;
	int left_covered_boundary;
	int right_covered_boundary;
	int bunch_significance;
	int bunch_density;
	int paint_length;
	int paint_coun;
	int quant;
	int intens_cou;
	int point_location;
	int bunch_new;
	int bunch_occurrence1;
	int bunch_new_significance;
	int blocking;
	int bunch_continuation;
	int neighbor_bunch_density;
	int neighbor_beg_int;
	int neighbor_end_int;
	int neighbor_length;
	int inclusion_relation;
	int density_ratio;
	int start_point;
	int current_point;
	int gap_length;
	int indic_length;
	int ratio_length;
	int ratio_length1;
	int Intersection;
	int test_junction;
	int hue_close;
	int saturation_close;
	int gray_close;

inclusion_relation=0;
bunch_continuation=-1;
indic_length=-1;
ratio_length=-1;
ratio_length1=-1;
Intersection=-1;
gap_length=10000;
shift=NumberOfColoredIntervals*bunch_number;
left_covered_boundary=beg_int-1;
right_covered_boundary=end_int+1;
      if(direct<0)
	  {
      start_point=beg_int;
	  }
	  else
	  {
      start_point=end_int;
	  }
bunch_significance=ColoredIntervalsStructure->Significance[bunch_number];
bunch_density=(2*bunch_significance)/(end_int-beg_int+1);

paint_length=paint_finish-paint_start;

for(paint_coun=0;paint_coun<=paint_length;paint_coun++)
  {//count neighborhood
	  current_point=start_point+direct*paint_coun;
	 quant=*(painted_numbers_colored+current_point);
	 if(quant>1)
	  {//count intersecting bunches
	  //covered_part+=1;
		 for(intens_cou=0;intens_cou<quant;intens_cou++)
		 {//loop internal
		 point_location=intens_cou*PressedLength+current_point;
		 bunch_new=*(bunches_occurred+point_location);
		 bunch_occurrence1=*(visible_bunches+bunch_new);
  bunch_new_significance=ColoredIntervalsStructure->Significance[bunch_new];
		 blocking=*(bunch_blocking+bunch_new);

				if((!bunch_continuation)&&(!blocking)&&(bunch_new!=bunch_number))
				  {//constraint
neighbor_beg_int=ColoredIntervalsStructure->BegInterv[bunch_new];
neighbor_end_int=ColoredIntervalsStructure->EndInterv[bunch_new];
neighbor_length=neighbor_end_int-neighbor_beg_int+1;
if(neighbor_beg_int<=beg_int)
{
	if(neighbor_end_int>=end_int)
	{
inclusion_relation=1;
//covers
	}
	else
	{
		if(neighbor_end_int<beg_int)
		{
		inclusion_relation=-3;
		}//locates to the left and does not intersect
		else
		{
	inclusion_relation=-2;
//locates to the left and intersects
		}
	}
}
else
{
if(neighbor_end_int<=end_int)
	{
inclusion_relation=-1;
//belongs
	}
    else
    {
		if(neighbor_beg_int>end_int)
		{
inclusion_relation=3;
//locates to the right and does not intersect
		}
    inclusion_relation=2;
//locates to the right and intersects
    }
}
neighbor_bunch_density=
(2*bunch_new_significance)/(neighbor_end_int-neighbor_beg_int+1);

density_ratio=(16*bunch_density)/(neighbor_bunch_density+bunch_density);
Intersection=Intersection_measure_ratios(neighbor_beg_int,neighbor_end_int,beg_int,
	end_int,&indic_length,&ratio_length,&ratio_length1);
if(Intersection==3)
{
	ratio_length=0;
}
	//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
	//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
test_junction=Test_Junction_Corrected(bunch_number,bunch_new,bunch_occurrence,
Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
 if((test_junction>0)&&(test_junction<=4))
 {
	 if(!ratio_length1)
	 {

		 if((neighbor_bunch_density>=StripWidth/2)&&(bunch_density>=StripWidth/2))
		 {
			 if(test_junction<=2)
			 {
				 if((inclusion_relation*direct)>=2)
				 {
				 *(continuation+bunch_number)=bunch_new+1;
				 }
				 return;
			 }
			 return;
		 }
	 }
 }
 else
 {
	 if((test_junction>=5)&&(test_junction<=8))
	 {
		 if((neighbor_bunch_density>=StripWidth/2)&&(bunch_density>=StripWidth/2))
		 {
			 *(continuation+bunch_number)=-(bunch_new+1);
				 return;
		 }
	 }
 }

if(density_ratio<=8)
{//dens
	if(inclusion_relation==1)
	{
		left_covered_boundary=end_int;
		right_covered_boundary=beg_int;
	}
	else
	{//e1
    if(inclusion_relation==-1)
    {
    left_covered_boundary=std::max(left_covered_boundary,neighbor_end_int);
    }
else
{
	if(inclusion_relation==3)
{
right_covered_boundary=std::min(right_covered_boundary,neighbor_beg_int);
}
}
	}//e1
}//dens
				}//constraint
		 }//loop internal
	 }//count intersecting bunches
}//count neighborhood
}//procedure end
//-----------------------------------------------------------------------

int

CColorIntervalSelect::Right_hole_finding(int point_start,int point_finish,int* gap_length)
{
	int prior;
	int paint_count;
	int inhabitant;

	prior=-1;
	for(paint_count=point_start;paint_count<=point_finish;paint_count++)
  {//find neighbor
	  inhabitant=*(painted_strip_colored+paint_count);
	  if(inhabitant>0)
	  {
		  prior=inhabitant;
		  *gap_length=paint_count-point_start+1;
		  return(prior);
	  }

  }
*gap_length=-1;
return(prior);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

int

CColorIntervalSelect::Left_hole_finding(int point_start,int point_finish,int* gap_length)
{
	int prior;
	int paint_count;
	int inhabitant;

	prior=-1;
	for(paint_count=point_start;paint_count>=point_finish;paint_count--)
  {//find neighbor
	  inhabitant=*(painted_strip_colored+paint_count);
	  if(inhabitant>0)
	  {
		  prior=inhabitant;
		  *gap_length=point_start-paint_count+1;
		  return(prior);
	  }

  }
*gap_length=-1;
return(prior);
}
//-----------------------------------------------------------------------------
void

CColorIntervalSelect::Strip_value_correction_bunch(int bunch,int paint_start,
int paint_finish,int bunch_signif)
 {
int paint_coun;
int current_inhabitant;
int paint_beg,paint_end;
int val;

	paint_beg=paint_start;
	paint_end=paint_finish;
	if((paint_beg<0)||(paint_end<0)||(paint_beg>(PressedLength-1))||(paint_end>(PressedLength-1)))
	{
		return;
	}

	for(paint_coun=paint_beg;paint_coun<=paint_end;paint_coun++)
	 {
		current_inhabitant=*(painted_strip_colored+paint_coun);

	 if(!current_inhabitant)
		{
		*(painted_strip_colored+paint_coun)=bunch+1;
		*(consistency_colored+paint_coun)=bunch_signif;
		}
		 else
		 {
		  val=*(consistency_colored+paint_coun);

			if(val<bunch_signif)
			{
		*(painted_strip_colored+paint_coun)=bunch+1;
		*(consistency_colored+paint_coun)=bunch_signif;

			}
		 }

	 }

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
	int

	CColorIntervalSelect::Left_extension(int left_neighb)
	{
		int prior;

		prior=-1;
		return(prior);
	}

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
int
CColorIntervalSelect::Measure2_of_proximity(int first_left,int first_right,
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

//-----------------------------------------------------------------------------------
void

CColorIntervalSelect::SimpleMerging(int bunch,int bunch_new)
{
	int bunch_significance;
	int bunch_new_significance;
	int bunch_beg;
	int bunch_end;
	int bunch_new_beg;
	int bunch_new_end;
	int new_beg;
	int new_end;
	int paint_beg;
	int paint_end;
	int new_bunch_significance;
	int new_bunch_density;

	bunch_significance=ColoredIntervalsStructure->Significance[bunch];
	bunch_new_significance=ColoredIntervalsStructure->Significance[bunch_new];

	bunch_beg=ColoredIntervalsStructure->BegInterv[bunch];
	bunch_end=ColoredIntervalsStructure->EndInterv[bunch];
	bunch_new_beg=ColoredIntervalsStructure->BegInterv[bunch_new];
	bunch_new_end=ColoredIntervalsStructure->EndInterv[bunch_new];
	new_bunch_significance=ColoredIntervalsStructure->Significance[bunch_new];
	new_beg=std::min(bunch_beg,bunch_new_beg);
	new_end=std::max(bunch_end,bunch_new_end);

	ColoredIntervalsStructure->BegInterv[bunch]=new_beg;
	ColoredIntervalsStructure->EndInterv[bunch]=new_end;
	ColoredIntervalsStructure->Significance[bunch]+=bunch_new_significance;
	new_bunch_significance=ColoredIntervalsStructure->Significance[bunch];
	new_bunch_density=(2*new_bunch_significance)/(new_end-new_beg+1);
	if((bunch_beg!=new_beg)||(bunch_end!=new_end))
	{//not_coins
	paint_beg=bunch_new_beg/4;//last_cor24.06.16
	paint_end=(bunch_new_end/4);//last_cor24.06.16
	   //if(paint_beg<paint_end)
	   //{
	//paint_end-=1;last_cor24.06.16
	Strip_value_correction_bunch(bunch,paint_beg,paint_end,new_bunch_density);
	    //}
	/*paint_beg=bunch_end/4;
	paint_end=(new_end/4);
	   if(paint_beg<paint_end)
	   {
	paint_beg+=1;
	Strip_value_correction_bunch(bunch,paint_beg,paint_end,new_bunch_density);
	    }*/
	}//not_coins

}




void CColorIntervalSelect::Cleaning(int numb)
{
	int coun_bunch1;
	int coun_bunch;
	int beg_int;
    int end_int;
    int signif;
	int coun_place;
	int bunch_num;
	int coun1_bun;
	int bunch_occurrence;
	int point_beg;
	int point_end;
	int number_of_bunches;
	int occurred_dimension;


	memset(painted_strip_colored,(int) '\0',sizeof(int)*(PressedLength));
memset(painted_numbers_colored,(int) '\0',sizeof(int)*(PressedLength));
memset(visible_bunches,(int) '\0',sizeof(int)*(NumberOfColoredIntervals));

if((numb>=0)&&(numb<=1))
{
Max_number=0;
for(coun_bunch=0;coun_bunch<NumberOfColoredIntervals;coun_bunch++)
{
	if(!*(bunch_blocking+coun_bunch))
  {
beg_int=ColoredIntervalsStructure->BegInterv[coun_bunch];
end_int=ColoredIntervalsStructure->EndInterv[coun_bunch];
signif=ColoredIntervalsStructure->Significance[coun_bunch];
signif=(signif<<1)/(end_int-beg_int+1);
Strip_value_painting_bunch(coun_bunch,beg_int/4,end_int/4,signif,
painted_strip_colored,painted_numbers_colored,consistency_colored,PressedLength);
	}
}

for(coun_place=0;coun_place<(PressedLength/2);coun_place++)
	{
	bunch_num=*(painted_strip_colored+coun_place);
	if(bunch_num)
	{
	(*(visible_bunches+bunch_num-1))++;
	   if(numb==1)
	   {
	number_of_bunches=*(painted_numbers_colored+coun_place);
		if(number_of_bunches>Max_number)
		{
		Max_number=number_of_bunches;
		}
	   }
	 }
	}
for(coun1_bun=0;coun1_bun<NumberOfColoredIntervals;coun1_bun++)
	{
	bunch_occurrence=*(visible_bunches+coun1_bun);
	beg_int=ColoredIntervalsStructure->BegInterv[coun1_bun];
	end_int=ColoredIntervalsStructure->EndInterv[coun1_bun];
	point_beg=beg_int/4;
	point_end=end_int/4;
	bunch_occurrence=(16*bunch_occurrence)/(point_end-point_beg+1);
	*(visible_bunches+coun1_bun)=bunch_occurrence;
	}
  if(numb==1)
  {
occurred_dimension=Max_number*PressedLength;

    bunches_occurred = new int[occurred_dimension];
  }

}



if(numb==2)
{
	occurred_dimension=Max_number*PressedLength;
	memset(bunches_occurred,(int) '\0',sizeof(int)*(occurred_dimension));
/*memset(painted_strip_colored,(int) '\0',sizeof(int)*(PressedLength));
memset(painted_numbers_colored,(int) '\0',sizeof(int)*(PressedLength));*/
	for(coun_bunch1=0;coun_bunch1<NumberOfColoredIntervals;coun_bunch1++)
{
  if(!*(bunch_blocking+coun_bunch1))
  {
beg_int=ColoredIntervalsStructure->BegInterv[coun_bunch1];
end_int=ColoredIntervalsStructure->EndInterv[coun_bunch1];
signif=ColoredIntervalsStructure->Significance[coun_bunch1];
signif=(signif<<1)/(end_int-beg_int+1);
Bunch_painting(coun_bunch1,beg_int,end_int,signif,painted_strip_colored,
painted_numbers_colored,consistency_colored);
Strip_value_painting_bunch(coun_bunch1,beg_int,end_int,signif,
painted_strip_colored_long,painted_numbers_colored_long,consistency_colored_long,StripLength);
	}
}
	for(coun_place=0;coun_place<(PressedLength/2);coun_place++)
	{
	bunch_num=*(painted_strip_colored+coun_place);
	if(bunch_num)
	{
	(*(visible_bunches+bunch_num-1))++;

	 }
	}
for(coun1_bun=0;coun1_bun<NumberOfColoredIntervals;coun1_bun++)
	{
	bunch_occurrence=*(visible_bunches+coun1_bun);
	beg_int=ColoredIntervalsStructure->BegInterv[coun1_bun];
	end_int=ColoredIntervalsStructure->EndInterv[coun1_bun];
	point_beg=beg_int/4;
	point_end=end_int/4;
	bunch_occurrence=(16*bunch_occurrence)/(point_end-point_beg+1);
	*(visible_bunches+coun1_bun)=bunch_occurrence;
	}
}

}
///==================================================================================
int

CColorIntervalSelect::CompleteOrdering(int* bunch_left_adjacent,int* bunch_left_contrast,
int* bunch_right_adjacent,int* bunch_right_contrast,int* last_bunch_number,int* smallest_coord,
int* greatest_coord)
{
	//int test_jun;
		int test_bunch;
		int small_hole;
		int right_small_hole;
		int left_bunch_domin;
		int right_bunch_domin;
		int bunch_occurrence;
		int beg_int;
	    int end_int;
		int bunch_significance;
	    int bunch_density;
		int point_beg;
		int point_end;
		int left_neighbor;
		int left_domin;
		int right_neighbor;
		int right_domin;
		int first_left_occurred;
	    int left_gap_length;
		int paint_finish;
		int paint_start;
		int neighbor_beg_int;
	    int neighbor_end_int;
	    int neighbor_point_beg;
	    int neighbor_point_end;
	    int bunch_new_significance;
	    int neighbor_bunch_density;
	    int density_ratio;
		int new_left_domin_point;
		int new_left_domin;
		int neighbor_end_dominant;
		int Intersection;
		int indic_length;
		int ratio_length;
		int ratio_length1;
		int hue_close;
		int saturation_close;
		int gray_close;
		int first_right_occurred;
	    int right_gap_length;
		int right_neighbor_beg_int;
	    int right_neighbor_end_int;
	    int right_neighbor_point_beg;
	    int right_neighbor_point_end;
	    int right_bunch_new_significance;
	    int right_neighbor_bunch_density;
		int right_neighbor_beg_dominant;
		int difference_neighbors;
		int gap_between_neighbors;
		int test_junction;
		int smallest_left_coordinate;
		int bunch_with_smallest_left_coordinate;
		int greatest_right_coordinate;
	    int bunch_with_greatest_right_coordinate;
		int new_right_neighbor;
		int new_visibility;

		smallest_left_coordinate=StripLength;
		greatest_right_coordinate=0;
	bunch_with_smallest_left_coordinate=-1;
	bunch_with_greatest_right_coordinate=-1;
		//test_jun=-1;
	for(int count_bunch=0;count_bunch<NumberOfColoredIntervals;count_bunch++)
		      {//continuation and contrast
				  test_bunch=*(bunch_blocking+count_bunch);
				  small_hole=0;
				  right_small_hole=0;
				  left_bunch_domin=-1;
				  right_bunch_domin=-1;
			 if (!test_bunch)
			 {//not blocked
				  bunch_occurrence=*(visible_bunches+count_bunch);
		if(bunch_occurrence>0)
		{//bo!=0
				  beg_int=ColoredIntervalsStructure->BegInterv[count_bunch];
	              end_int=ColoredIntervalsStructure->EndInterv[count_bunch];
				  if(beg_int<smallest_left_coordinate)
				  {
				  smallest_left_coordinate=beg_int;
				  bunch_with_smallest_left_coordinate=count_bunch;
				  }
				  bunch_significance=ColoredIntervalsStructure->Significance[count_bunch];
	              bunch_density=(2*bunch_significance)/(end_int-beg_int+1);

				  point_beg=beg_int/4;
		          point_end=end_int/4;


		  	     if(point_beg>=1)
			     {
			     left_neighbor=*(painted_strip_colored+point_beg-1);
				 left_bunch_domin=*(painted_strip_colored+point_beg);
				 left_domin=left_bunch_domin;
				 }
			     else
			     {
		         left_neighbor=*(painted_strip_colored+point_beg);
				 left_bunch_domin=0;//left boundary of the array of visible bunches
				 left_domin=-1;
			     }
			                         if(point_end<StripLength-1/4)
		                             {
		                             right_neighbor=*(painted_strip_colored+point_end+1);
									 right_bunch_domin=*(painted_strip_colored+point_end);
									 right_domin=right_bunch_domin;
									 }
		                             else
		                             {
	                                 right_neighbor=*(painted_strip_colored+point_end);
									 right_bunch_domin=0;//right boundary of the array of visible bunches
									 right_domin=-1;

		                             }
	first_left_occurred=-1;
	left_gap_length=-1;
	if(left_neighbor==0)
		  {//left hole processing
			  if(left_bunch_domin>0)
			  {
				  paint_finish=0;
				  paint_start=point_beg-1;
				  first_left_occurred=Left_hole_finding(paint_start,paint_finish,&left_gap_length);
				  if(first_left_occurred>=0)
				  {
					  left_neighbor=first_left_occurred;
				  }
			  }
	//Bunch_continuation(-1,count_bunch,bunch_occurrence,beg_int,end_int,paint_start,paint_finish,bunch_left_contrast);
		  }//right hole processing

				 left_bunch_domin=left_bunch_domin-1;
				 left_neighbor=left_neighbor-1;
				 if((left_neighbor>=0)&&(left_neighbor!=count_bunch))
				 {//ln
	neighbor_beg_int=ColoredIntervalsStructure->BegInterv[left_neighbor];
	neighbor_end_int=ColoredIntervalsStructure->EndInterv[left_neighbor];
	neighbor_point_beg=neighbor_beg_int/4;
	neighbor_point_end=neighbor_end_int/4;
	bunch_new_significance=ColoredIntervalsStructure->Significance[left_neighbor];
	neighbor_bunch_density=
	(2*bunch_new_significance)/(neighbor_end_int-neighbor_beg_int+1);
	density_ratio=(16*bunch_density)/(neighbor_bunch_density+bunch_density);
	if(neighbor_end_int<beg_int)
	      {//there is a small hole
			  small_hole=beg_int-neighbor_end_int;
			  //change left_neighbor if the hole is large enough!!!
			  if(small_hole>=4)
			  {
				  new_left_domin_point=neighbor_end_int+(beg_int-neighbor_end_int)/2;
				  new_left_domin=*(painted_strip_colored_long+new_left_domin_point);
				  if(new_left_domin>0)
				  {
					  new_left_domin-=1;
				  }
			  }

		  }//there is a small hole
	    if(left_bunch_domin<0)
		{//left bunch dominator
			if(neighbor_point_end<point_end)
			{
	left_bunch_domin=*(painted_strip_colored+neighbor_point_end+1);
			}
		}//left bunch dominator
		else
		{//elb
			if(small_hole>0)
			  {//smh
				  if((left_bunch_domin!=left_neighbor)&&(left_bunch_domin!=count_bunch)&&
					  (left_bunch_domin>=0))
				  {
	              left_neighbor=left_bunch_domin;
	neighbor_beg_int=ColoredIntervalsStructure->BegInterv[left_neighbor];
	neighbor_end_int=ColoredIntervalsStructure->EndInterv[left_neighbor];
	neighbor_point_beg=neighbor_beg_int/4;
	neighbor_point_end=neighbor_end_int/4;
	bunch_new_significance=ColoredIntervalsStructure->Significance[left_neighbor];
	neighbor_bunch_density=
	(2*bunch_new_significance)/(neighbor_end_int-neighbor_beg_int+1);
	density_ratio=(16*bunch_density)/(neighbor_bunch_density+bunch_density);
				  }
			  }//smh
		}//elb


		         if((left_neighbor==left_bunch_domin)||(left_bunch_domin==count_bunch)||
					 (bunch_occurrence>0))
		          {//one of the two bunches is visible at the left end
	if(neighbor_point_end<point_end)
	{
		neighbor_end_dominant=*(painted_strip_colored+neighbor_point_end+1);
	}
	else
	{
	neighbor_end_dominant=0;
	}
	neighbor_end_dominant=neighbor_end_dominant-1;

				 if((left_bunch_domin==count_bunch)||(neighbor_end_dominant==count_bunch)||
					 ((bunch_occurrence>0)&&(neighbor_end_dominant>0)))
			 {//intersection and alternating dominance
	Intersection=Intersection_measure_ratios(neighbor_beg_int,neighbor_end_int,beg_int,
		end_int,&indic_length,&ratio_length,&ratio_length1);
	if(Intersection==3)
	{
		ratio_length=0;
	}
		//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
		//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
	test_junction=Test_Junction_Corrected(count_bunch,left_neighbor,bunch_occurrence,
	Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
	 // if((density_ratio>=3)&&(density_ratio<=12))
	test_junction=test_junction%10;
	  //{//density restriction
		  if(!test_junction)
		  {
			  *(bunch_left_contrast+count_bunch)=0;
			  *(bunch_left_adjacent+count_bunch)=left_neighbor+1;
		  }
	         //if(((test_junction%10)>0)&&((test_junction%10)<=4))
			 if((test_junction>0)&&(test_junction<=4))
	          {
				 *(bunch_left_contrast+count_bunch)=test_junction+1;
				 *(bunch_left_adjacent+count_bunch)=left_neighbor+1;
	          }
			 //else
			 //{
					 if((test_junction%10)==8)
					 {
					 *(bunch_left_contrast+count_bunch)=0;
	                 *(bunch_left_adjacent+count_bunch)=left_neighbor+1;
					 }
			 //}
	  //}//density restriction
	  //else
	  //{
			  /*if(((test_junction%10)>=1)&&((test_junction%10)<=2))
			  {//last_cor
				  *(bunch_left_contrast+count_bunch)=test_junction+1;
				 *(bunch_left_adjacent+count_bunch)=left_neighbor+1;
			  }*/
	  //}
	  //if(((test_junction%10)>4)&&((test_junction%10)<=7))
	  if((test_junction>4)&&(test_junction<=7))
	  {//last_cor12.06.13

	*(bunch_left_contrast+count_bunch)=-(test_junction+1);
	*(bunch_left_adjacent+count_bunch)=-(left_neighbor+1);
	  }

			 }//intersection and alternating dominance
		          }//one of the two bunches is visible at the left end

				 }//ln
				 else
				 {
		  if(left_neighbor==-1)
		  {//hole processing
				  paint_finish=beg_int/4;
				  paint_start=std::max((paint_finish-2),0);
	//Bunch_continuation(-1,count_bunch,bunch_occurrence,beg_int,end_int,paint_start,paint_finish,bunch_left_contrast);
		  }
				 }

	//right continuation
	first_right_occurred=-1;
	right_gap_length=-1;
	if(!right_neighbor)
		  {//right hole processing
			  if(right_bunch_domin>0)
			  {
				  paint_finish=(StripLength-1)/4;
				  paint_start=point_end+1;
	first_right_occurred=Right_hole_finding_modified(paint_start,paint_finish,&right_gap_length);
				  if((right_gap_length<=7)&&(first_right_occurred>=0))
				  {
					  right_neighbor=first_right_occurred;
				  }
			  }
	//Bunch_continuation(-1,count_bunch,bunch_occurrence,beg_int,end_int,paint_start,paint_finish,bunch_left_contrast);
		  }//right hole processing
	else
	{
	new_right_neighbor=right_neighbor-1;
	new_visibility=*(visible_bunches+new_right_neighbor);
	if(!new_visibility)
	{
	if(right_bunch_domin>0)
			  {
				  paint_finish=(StripLength-1)/4;
				  paint_start=point_end+1;
	first_right_occurred=Right_hole_finding_modified(paint_start,paint_finish,&right_gap_length);
				  if((right_gap_length<=10)&&(first_right_occurred>=0))
				  {//last_cor04.11.15
					  right_neighbor=first_right_occurred;
				  }
			  }
	}
	}
				 right_bunch_domin=right_bunch_domin-1;
				 right_neighbor=right_neighbor-1;
				 if((right_neighbor>=0)&&(right_neighbor!=count_bunch))
				 {//rln
	right_neighbor_beg_int=ColoredIntervalsStructure->BegInterv[right_neighbor];
	right_neighbor_end_int=ColoredIntervalsStructure->EndInterv[right_neighbor];
	right_neighbor_point_beg=right_neighbor_beg_int/4;
	right_neighbor_point_end=right_neighbor_end_int/4;
	right_bunch_new_significance=ColoredIntervalsStructure->Significance[right_neighbor];
	right_neighbor_bunch_density=
	(2*right_bunch_new_significance)/(right_neighbor_end_int-right_neighbor_beg_int+1);
	density_ratio=(16*bunch_density)/(right_neighbor_bunch_density+bunch_density);
	if(right_neighbor_beg_int>end_int)
	      {//there is a small hole
			  right_small_hole=right_neighbor_beg_int-end_int;
		  }//there is a small hole
	    if(right_bunch_domin<0)
		{//right bunch dominator
			if(right_neighbor_point_beg>point_beg)
			{
	right_bunch_domin=*(painted_strip_colored+right_neighbor_point_beg-1);
			}
		}//right bunch dominator
		else
		{//erb
	if(right_small_hole>0)
			  {//smh
				  if((right_bunch_domin!=right_neighbor)&&(right_bunch_domin!=count_bunch)&&
					  (right_bunch_domin>=0))
				  {
	              right_neighbor=right_bunch_domin;
	right_neighbor_beg_int=ColoredIntervalsStructure->BegInterv[right_neighbor];
	right_neighbor_end_int=ColoredIntervalsStructure->EndInterv[right_neighbor];
	right_neighbor_point_beg=right_neighbor_beg_int/4;
	right_neighbor_point_end=right_neighbor_end_int/4;
	right_bunch_new_significance=ColoredIntervalsStructure->Significance[right_neighbor];
	right_neighbor_bunch_density=
	(2*right_bunch_new_significance)/(right_neighbor_end_int-right_neighbor_beg_int+1);
	density_ratio=(16*bunch_density)/(right_neighbor_bunch_density+bunch_density);
				  }
			  }//smh

		}//erb

		         //if((right_neighbor==right_bunch_domin)||(right_bunch_domin==count_bunch))
		          //{//one of the two bunches is visible at the right end//last_cor04.11.15
	if(right_neighbor_point_beg>point_beg)
	{
		right_neighbor_beg_dominant=*(painted_strip_colored+right_neighbor_point_beg-1);
	}
	else
	{
	right_neighbor_beg_dominant=0;
	}
	right_neighbor_beg_dominant=right_neighbor_beg_dominant-1;

				 if((right_bunch_domin==count_bunch)||(right_neighbor_beg_dominant==count_bunch)||
					 ((bunch_occurrence>0)&&(right_neighbor_beg_dominant>=0)))
			 {//intersection and alternating dominance
	Intersection=Intersection_measure_ratios(right_neighbor_beg_int,right_neighbor_end_int,beg_int,
		end_int,&indic_length,&ratio_length,&ratio_length1);
	if(Intersection==3)
	{
	ratio_length=0;
	}
		//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
		//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
	test_junction=Test_Junction_Corrected(count_bunch,right_neighbor,bunch_occurrence,
	Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
	test_junction=test_junction%10;
	  //if((density_ratio>=3)&&(density_ratio<=12))
	  //{//density restriction
		  //if(!(test_junction%10))
		  if(!(test_junction))
		  {
			  *(bunch_right_contrast+count_bunch)=0;
			  *(bunch_right_adjacent+count_bunch)=right_neighbor+1;
		  }
	//if(((test_junction%10)>0)&&((test_junction%10)<=4))
	if((test_junction>0)&&(test_junction<=4))
	 {//04
	  *(bunch_right_contrast+count_bunch)=test_junction+1;
	  *(bunch_right_adjacent+count_bunch)=right_neighbor+1;
	}//04

					 if(test_junction==8)
					 {
					 *(bunch_right_contrast+count_bunch)=0;
	                 *(bunch_right_adjacent+count_bunch)=right_neighbor+1;
					 }

	  //}//density restriction
	  /*else
	  {//important error!16.09.13
			  if(((test_junction%10)>=1)&&((test_junction%10)<=2))
			  {//last_cor
				  *(bunch_right_contrast+count_bunch)=test_junction+1;
				 *(bunch_right_adjacent+count_bunch)=right_neighbor+1;//!!!!last_cor04.02.14
			  }
	  }*/
	  //if(((test_junction%10)>4)&&((test_junction%10)<=7))
	  if((test_junction>4)&&(test_junction<=7))
	  {
	*(bunch_right_contrast+count_bunch)=-(test_junction+1);
	*(bunch_right_adjacent+count_bunch)=-(right_neighbor+1);
	 }
			 }//intersection and alternating dominance
		          //}//one of the two bunches is visible at the right end

				 }//rln

				 if((right_neighbor>=0)&&(left_neighbor>=0))
		       {//between close bunches
	if((left_domin>0)&&(right_domin>0))
	{
		difference_neighbors=right_neighbor_point_beg-neighbor_point_end;
	         if(difference_neighbors<=1)
			 {
				 gap_between_neighbors=right_neighbor_beg_int-neighbor_end_int;
			 }
	}
				 }
				 if(!(*(bunch_right_adjacent+count_bunch)))
			 {
				 if(end_int>greatest_right_coordinate)
				 {
					 greatest_right_coordinate=end_int;
	                 bunch_with_greatest_right_coordinate=count_bunch;
				 }

			 }
				 }//bo!=0
			 }//not blocked

	          }//continuation and contrast
			  *last_bunch_number=bunch_with_greatest_right_coordinate;
	          *smallest_coord=smallest_left_coordinate;
			  *greatest_coord=greatest_right_coordinate;
	return(bunch_with_smallest_left_coordinate);
	}
//========================================================================================
int

CColorIntervalSelect::LinearOrdering(int* bunch_left_adjacent,int* bunch_left_contrast,
int* bunch_right_adjacent,int* bunch_right_contrast,int first_bunch_number,int bunch_last)
{
	int success_ordering;
		int number_of_strip_covering;
		int next_bunch_number;
		int right_contrast;
		int next_right_bunch;
		int next_bunch_end;
		int count_bunch;

		success_ordering=-1;
	number_of_strip_covering=0;
	next_bunch_end=-1;
	next_bunch_number=first_bunch_number;
	//old_ordered_bunch_number[0]=first_bunch_number;
	//ordered_bunch_number[first_bunch_number]=0;
	for(count_bunch=0;count_bunch<NumberOfColoredIntervals;count_bunch++)
	{//loop_bunch
	old_ordered_bunch_number[count_bunch]=next_bunch_number;
	ordered_bunch_number[next_bunch_number]=count_bunch;
	if(next_bunch_number==bunch_last)
	{
		break;
	}
	right_contrast=bunch_right_contrast[next_bunch_number];

	   if(right_contrast>0)
	   {
	next_right_bunch=*(bunch_right_adjacent+next_bunch_number)-1;
	   }
	   else
	   {
		   if(right_contrast<0)
		   {
	next_right_bunch=-*(bunch_right_adjacent+next_bunch_number)-1;
		   }
		   else
		   {
			   if(!right_contrast)
			   {
			   next_right_bunch=*(bunch_right_adjacent+next_bunch_number)-1;
			   }
		   }
	   }
	       if(next_right_bunch<NumberOfColoredIntervals)
	        {
	   if(next_right_bunch>=0)
	   {
	   next_bunch_number=next_right_bunch;

	   }
	   else
	   {
	   next_bunch_end=ColoredIntervalsStructure->EndInterv[next_bunch_number];
	   if(next_bunch_end>StripLength-StripWidth)
	   {
		success_ordering=1;
	    number_of_strip_covering=count_bunch+1;
		NumberOfIntervalsInCovering=number_of_strip_covering;
	   return(success_ordering);
	   }
	   else
	   {
	   return(success_ordering);
	   }
	   }
		   }
		   else
		   {
			   return(success_ordering);
		   }
		   next_bunch_end=ColoredIntervalsStructure->EndInterv[next_bunch_number];
	}//loop_bunch
	success_ordering=1;
	number_of_strip_covering=count_bunch+1;
	NumberOfIntervalsInCovering=number_of_strip_covering;
	return(success_ordering);
	}
//-----------------------------------------------------------------------

int

CColorIntervalSelect::Right_hole_finding_modified(int point_start,int point_finish,int* gap_length)
{
	int prior;
	int paint_count;
	int inhabitant;
	int bunch_visibility;

	prior=-1;
	for(paint_count=point_start;paint_count<=point_finish;paint_count++)
  {//find neighbor
	  inhabitant=*(painted_strip_colored+paint_count);
	  if(inhabitant>0)
	  {
       inhabitant=inhabitant-1;
	   bunch_visibility=*(visible_bunches+inhabitant);
	   if(bunch_visibility>0)
	   {
		  prior=inhabitant+1;
		  *gap_length=paint_count-point_start+1;
		  return(prior);
	   }
	  }

  }

*gap_length=-1;
return(prior);
}
