// Strip.cpp: implementation of the CStrip class.
//
//////////////////////////////////////////////////////////////////////

#include "ColorVision.h"
#include "Strip.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
using namespace std;


#define RED_COEF    0.3  // 0.299
#define GREEN_COEF  0.59  // 0.587
#define BLUE_COEF   0.11  // 0.114
#define LOW_INTENSITY 12
#define LOW_INTENSITY1 33
#define LOW_INTENSITY2 46
#define LOW_INTENSITY3 23

CStrip::CStrip(void)
{
	left_bound_of_color_resolution = 21;
	right_bound_of_color_resolution = 46;
	left_shift1[0] = 1;
	left_shift1[1] = 1;
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
	left_shift1[12] = 12;
	left_shift1[13] = 13;
	left_shift1[14] = 14;
	left_shift1[15] = 15;

	left_shift2[0] = 1;
	left_shift2[1] = 1;
	left_shift2[2] = 1;
	left_shift2[3] = 1;
	left_shift2[4] = 1;
	left_shift2[5] = 1;
	left_shift2[6] = 1;
	left_shift2[7] = 1;
	left_shift2[8] = 1;
	left_shift2[9] = 1;
	left_shift2[10] = 1;
	left_shift2[11] = 1;
	left_shift2[12] = 1;
	left_shift2[13] = 1;
	left_shift2[14] = 1;
	left_shift2[15] = 1;

	right_shift1[0] = 1;
	right_shift1[1] = 1;
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
	right_shift1[12] = 1;
	right_shift1[13] = 1;
	right_shift1[14] = 1;
	right_shift1[15] = 1;

	right_shift2[0] = 1;
	right_shift2[1] = 1;
	right_shift2[2] = 3;
	right_shift2[3] = 4;
	right_shift2[4] = 4;
	right_shift2[5] = 5;
	right_shift2[6] = 5;
	right_shift2[7] = 6;
	right_shift2[8] = 6;
	right_shift2[9] = 7;
	right_shift2[10] = 7;
	right_shift2[11] = 8;
	right_shift2[12] = 8;
	right_shift2[13] = 9;
	right_shift2[14] = 10;
	right_shift2[15] = 11;

	/* graygrades[0]=135;
	 graygrades[1]=134;
	 graygrades[2]=134;
	 graygrades[3]=132;
	 graygrades[4]=132;
	 graygrades[5]=132;
	 graygrades[6]=132;
	 graygrades[7]=130;
	 graygrades[8]=130;
	 graygrades[9]=128;
	 graygrades[10]=128;
	 graygrades[11]=128;
	 graygrades[12]=128;
	 graygrades[13]=126;
	 graygrades[14]=126;
	 graygrades[15]=124;*/

	 graygrades[0]=135;
	   graygrades[1]=135;
	   graygrades[2]=130;
	   graygrades[3]=130;
	   graygrades[4]=130;
	   graygrades[5]=128;
	   graygrades[6]=128;
	   graygrades[7]=128;
	   graygrades[8]=128;
	   graygrades[9]=126;
	   graygrades[10]=126;
	   graygrades[11]=126;
	   graygrades[12]=126;
	   graygrades[13]=124;
	   graygrades[14]=124;
	   graygrades[15]=124;

	   /*graygrades1[0]=135;
	   graygrades1[1]=133;
	   graygrades1[2]=133;
	   graygrades1[3]=131;
	   graygrades1[4]=131;
	   graygrades1[5]=131;
	   graygrades1[6]=131;
	   graygrades1[7]=129;
	   graygrades1[8]=129;
	   graygrades1[9]=129;
	   graygrades1[10]=127;
	   graygrades1[11]=127;
	   graygrades1[12]=127;
	   graygrades1[13]=125;
	   graygrades1[14]=125;
	   graygrades1[15]=124;*/

	   graygrades1[0]=135;
	   graygrades1[1]=131;//last_cor03.04.15
	   graygrades1[2]=129;
	   graygrades1[3]=129;
	   graygrades1[4]=129;
	   graygrades1[5]=127;
	   graygrades1[6]=127;
	   graygrades1[7]=127;
	   graygrades1[8]=127;
	   graygrades1[9]=125;
	   graygrades1[10]=125;
	   graygrades1[11]=125;
	   graygrades1[12]=125;
	   graygrades1[13]=124;
	   graygrades1[14]=124;
	   graygrades1[15]=124;

	   graygrades2[0]=135;
	   graygrades2[1]=134;
	   graygrades2[2]=134;
	   graygrades2[3]=133;
	   graygrades2[4]=133;
	   graygrades2[5]=132;
	   graygrades2[6]=132;
	   graygrades2[7]=132;
	   graygrades2[8]=132;
	   graygrades2[9]=131;
	   graygrades2[10]=131;
	   graygrades2[11]=131;
	   graygrades2[12]=131;
	   graygrades2[13]=124;
	   graygrades2[14]=124;
	   graygrades2[15]=124;
	TotalTime=0;
}
//=====================================================

CStrip::~CStrip(void) {
	delete[] hist_fun;
	delete[] hist_sum;
	delete[] num_of_int;
	delete[] beg_point;
	delete[] end_point;
	delete[] thick_beg;
	delete[] thick_end;
	delete[] thick_stat;
	delete[] thick_stat_input;
	delete[] thick_break_beg;
	delete[] thick_break_end;
	delete[] thick_break_stat;
	delete[] thick_prev_beg;
	delete[] thick_prev_end;
	delete[] thick_prev_stat;
	delete[] thick_first;
	delete[] thick_last;
	delete[] jump_len;
	delete[] jump_end;
	delete[] import_beg;
	delete[] import_end;

	delete[] gray_min;
	delete[] gray_max;
	delete[] gray_mean;
	delete[] opponent1_min;
	delete[] opponent1_max;
	delete[] opponent1_mean;

	delete[] valuable_intensity;
	delete[] valuable_interval;
	delete[] bright_consistency;
	delete[] quantity_of_intensities;
	delete[] intensities_occurred;
	delete[] interval_occurred;

	delete[] IntAllInform;

	delete[] hist_fung;
	delete[] hist_sumg;
	delete[] num_of_intg;
	delete[] beg_pointg;
	delete[] end_pointg;
	delete[] thick_begg;
	delete[] thick_endg;
	delete[] thick_statg;
	delete[] thick_stat_inputg;
	delete[] thick_break_begg;
	delete[] thick_break_endg;
	delete[] thick_break_statg;
	delete[] thick_prev_begg;
	delete[] thick_prev_endg;
	delete[] thick_prev_statg;
	delete[] thick_firstg;
	delete[] thick_lastg;
	delete[] jump_leng;
	delete[] jump_endg;
	delete[] import_begg;
	delete[] import_endg;

	delete[] valuable_intensityg;
	delete[] valuable_intervalg;
	delete[] bright_consistencyg;
	delete[] quantity_of_intensitiesg;
	delete[] intensities_occurredg;
	delete[] interval_occurredg;

	delete[] IntAllInformGray;

}
//=====================================================
//This program performs geometric description of intensities in a particular
//strip

void CStrip::Loc_stat_geom_double(bool NumPair)
{
	int ncoor2;
	int con_Dim;
	long point_start;
	register int ncoor1;
	unsigned char inten_red,inten_green,inten_blue;
	int max_intensity;
	int min_intensity;
	int gray_scale;
	unsigned char inten_opponent1;
	int actual_inten_opponent1;
	//unsigned char inten_opponent2;
	int interm;
	long int entry_point;
	int last_end[NUM_INTEN];
	int first_pix[NUM_INTEN];
	int last_pix[NUM_INTEN];
	int last_endg[NUM_INTEN1];
	int first_pixg[NUM_INTEN1];
	int last_pixg[NUM_INTEN1];
	int* frequency_of_color_differ;
	int* inten_count1;
	int fine_color_balance;
	//int fine_color_balance1;
	float satur;
	int incr1, incr2;
	int residual;
	int scale_pos;//last_cor02.04.15
	int res_scale_pos;
	int redgreen;
		int redblue;
		int randg;
		int randb;
		int actual_strip_number;
		int point_sat;

//	CColorVisionApp *pApp;
//	pApp= (CColorVisionApp *)AfxGetApp ();
	scale_pos=0;
	res_scale_pos=0;

	frequency_of_color_differ=new int[NUM_INTEN*(DimDifference1)];
	memset(last_end,(int) '\0',sizeof(int)*NUM_INTEN);
	memset(last_endg,(int) '\0',sizeof(int)*NUM_INTEN1);

	con_Dim=(3*DimX+Cor_Width);
	 if(HorizontalVertical==0)
					{
if(Feature==1)
{
	actual_strip_number=num_strip;
}
else
{
	if(!Feature)
	{
		actual_strip_number=NumbStr-1-num_strip;
	}
	else
	{
		actual_strip_number=num_strip;
	}
}
		 point_start=((long)con_Dim)*(long)(StripWidthPrev*(actual_strip_number));

		 for(ncoor2=0;ncoor2<DimX;ncoor2++)
		 {
		 entry_point=(long)(3*ncoor2)+point_start;


		 	for(ncoor1=0;ncoor1<StripWidth;ncoor1++)
		  {

		      inten_blue=*(intensi+entry_point++);
		      inten_green=*(intensi+entry_point++);
		      inten_red=*(intensi+entry_point);

		  max_intensity=std::max(inten_blue,std::max(inten_green,inten_red));
		  min_intensity=std::min(inten_blue,std::min(inten_green,inten_red));
		  satur=((float)(max_intensity-min_intensity)/((float) max_intensity));
		  point_sat = (int)(15 * satur);
		  entry_point=entry_point+con_Dim-2;
		  gray_scale=BLUE_COEF*inten_blue+GREEN_COEF*inten_green+RED_COEF*inten_red;
		 	  gray_scale=gray_scale >> 2;
		 	  scale_pos=gray_scale/4;//last_cor02.04.15
		 res_scale_pos=gray_scale%4;
		 if(res_scale_pos>=2)
		 {
		 	/*if((scale_pos>0)&&(scale_pos<15))
		 	{
		 scale_pos+=1;
		 	}*/
		 	if(scale_pos<15)
		 	{
		 scale_pos+=1;
		 	}
		 }
		 	  if(num_of_intg[gray_scale]<MAX_INT_NUMBER-1)
		      {
		  StripCharacteristicsFindingGray(gray_scale,ncoor2,first_pixg,last_pixg,last_endg);
		 	  }
		 	 if((max_intensity>=LOW_INTENSITY)||((gray_scale<=3)&&(inten_blue<=6)))
		 	 {//11
		 		                 if(NumPair)
		 		                 {//7
		 //two coordinate systems G/(G+B), G/(G+R) and G/(G+B), R/(R+B) are imployed
		 //the first coordinate system is used for the points where G/(G+B)>0.5, the second one, for the points where G/(G+B)<=0.5
		 							 if (inten_blue + inten_green != 0)
		 							 {
		 								 interm = (int)((255 * (long)inten_green) / (inten_green + inten_blue));

		 								 /*residual=((255*(long)inten_green)%(inten_green+inten_blue));
		 								 if(residual>=(inten_green+inten_blue)/2)
		 								 {
		 								 if(interm!=255)
		 								 {
		 								 interm+=1;
		 								 }
		 								 }*/
		 								 inten_opponent1 = (unsigned char)(correct_int[interm]);
		 								 /*if((num_strip==27)&&(inten_opponent1==32)&&(ncoor2>=115))
		 								 {
		 								 gray_scale=gray_scale;
		 								 }*/
		 								 randg = inten_red + inten_green;
		 								 if (randg)
		 								 {
		 									 redgreen = ((int)(255 * (long)inten_green / (inten_red + inten_green))) / 4;
		 								 }
		 								 else
		 								 {
		 									 redgreen = 63;
		 								 }
		 								 randb = inten_red + inten_blue;
		 								 if (randb)
		 								 {
		 									 redblue = ((int)(255 * (long)inten_red / (inten_red + inten_blue))) / 4;
		 								 }
		 								 else
		 								 {
		 									 redblue = 63;
		 								 }
		 								 if (inten_opponent1 == 31)
		 								 {//last_cor12.09.14

		 									 if ((satur>0.05) && (redgreen >= 30))
		 									 {//last_cor17.06.15
		 										 if (inten_green>inten_blue)
		 										 {
		 											 inten_opponent1 += 1;
		 										 }
		 										 else
		 										 {//last_cor19.05.15
		 											 if ((inten_blue - inten_green) >= 2)
		 											 {
		 												 inten_opponent1 -= 1;
		 											 }
		 										 }//last_cor19.05.15
		 									 }
		 								 }
		 								 if (inten_opponent1<32)
		 								 {
		 									 fine_color_balance = redblue;
		 									 if (satur>0.06)
		 									 {
		 										 if (inten_red>inten_blue)
		 										 {
		 											 if (fine_color_balance == 31)
		 											 {
		 												 fine_color_balance += 1;
		 											 }
		 										 }
		 										 if (inten_red<inten_blue)
		 										 {
		 											 if (fine_color_balance == 31)
		 											 {
		 												 fine_color_balance -= 1;
		 											 }
		 										 }
		 									 }
		 									 /*if((inten_opponent1>24)&&(inten_opponent1<30)&&(num_strip==18))
		 									 {
		 									 fine_color_balance+=1;
		 									 fine_color_balance-=1;
		 									 }*/
		 									 //in this half-triangle this function is determined everywhere
		 								 }
		 								 else
		 								 {
		 									 fine_color_balance = redgreen;
		 									 if (inten_green<(inten_red-3))
		 									 {//last_cor21.03.18
		 										 if ((fine_color_balance == 31))
		 										 {
		 											 fine_color_balance -= 1;
		 										 }
		 									 }
		 									 if (inten_green>(inten_red+3))
		 									 {
		 										 if ((fine_color_balance == 31))
		 										 {
		 											 fine_color_balance += 1;
		 										 }
		 									 }//last_cor21.03.18
		 									 //in this half-triangle this function is determined everywhere
		 								 }
		 								 //fine_color_balance1=((int)(255*(long)inten_green/(inten_red+inten_green)))/4;
		 							 }
		 							 else
		 							 {
		 								 if (!inten_red)
		 								 {
		 									 //pure dark
		 									 actual_inten_opponent1 = 31;
		 									 inten_opponent1 = 135;
		 									 fine_color_balance = 31;
		 									 goto L;
		 								 }
		 								 else
		 								 {
		 									 //pure red
		 									 interm = 127;
		 									 inten_opponent1 = (unsigned char)(correct_int[interm]);
		 									 fine_color_balance = ((int)(255 * (long)inten_red / (inten_red + inten_blue))) / 4;
		 									 actual_inten_opponent1 = inten_opponent1;
		 									 inten_opponent1 = inten_opponent1 + 49;
		 									 goto L;
		 								 }
		 							 }
		 							 /*if((inten_opponent1>42)&&((inten_opponent1%2)==1))
		 							 {
		 							 inten_opponent1=inten_opponent1+1;
		 							 }*/
		 							 /*if ((num_strip == 29) && (ncoor2>320))
		 							 {//test26.02.18
		 								 inten_opponent1+ = 1;
		 								 inten_opponent1-=1;
		 							 }*/
		 							 if (((gray_scale <= 5) && (abs(fine_color_balance - inten_opponent1) <= 2) &&
		 								 (satur<0.26)) || (gray_scale <= 3) || ((gray_scale <= 4) && (satur<0.5)))
		 							 {//last_cor31.01.18
		 								 actual_inten_opponent1 = 31;
		 								 inten_opponent1 = 135;
		 								 fine_color_balance = 31;
		 								 goto L;
		 							 }
		 							 if (((abs(fine_color_balance - 31) <= 1) && (abs(inten_opponent1 - 31) <= 1) && (satur<0.08)) ||
		 								 ((gray_scale >= 45) && (satur<0.11) && (inten_opponent1>31) && (inten_opponent1 <= 32)))
		 							 {//last_cor17.03.17last_cor21.08.1713.08.17
		 								 actual_inten_opponent1 = inten_opponent1;
		 								 inten_opponent1 = graygrades2[scale_pos];//last_cor02.04.15
		 								 goto L;
		 							 }
		 							 else
		 							 {
		 								 if (((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale <= 12)) ||
		 									 ((abs(fine_color_balance - 31) <= 3) && (abs(inten_opponent1 - 31) <= 3) && (gray_scale <= 10)))
		 								 {
		 									 actual_inten_opponent1 = inten_opponent1;
		 									 inten_opponent1 = graygrades2[scale_pos];
		 									 goto L;
		 								 }
		 							 }
		 							 if (inten_opponent1>left_bound_of_color_resolution)
		 							 {//6
		 								 if (inten_opponent1<32)
		 								 {//5
		 								  //the coordinate system on the color triangle is generated by G/(G+B) and R/(R+B)
		 									 incr1 = 31 - inten_opponent1;


		 									 if (fine_color_balance>31)
		 									 {//triangle3
		 										 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05) &&
		 											 (gray_scale <= 30))
		 										 {
		 											 actual_inten_opponent1 = (int)inten_opponent1;
		 											 inten_opponent1 = graygrades2[scale_pos];
		 											 goto L;
		 										 }
		 										 /*else
		 										 {
		 										 if((abs(fine_color_balance-31)<=3)&&(abs(inten_opponent1-31)<=3)&&(gray_scale<=20))
		 										 {
		 										 actual_inten_opponent1=(int)inten_opponent1;
		 										 inten_opponent1=graygrades[gray_scale/4];
		 										 goto L;
		 										 }
		 										 }*/
		 										 /*if((abs(inten_opponent1-31)<=3)&&((gray_scale>=50)||(gray_scale<16))&&(satur<0.3))
		 										 {
		 										 actual_inten_opponent1=(int)inten_opponent1;
		 										 inten_opponent1=graygrades[gray_scale/4];
		 										 goto L;
		 										 }*/
		 										 actual_inten_opponent1 = inten_opponent1;
		 										 inten_opponent1 = inten_opponent1 + 49;
		 										 goto L;
		 										 //red, violet, only positive deviations
		 										 //colorless, gray, white pixels are not separated
		 									 }
		 									 else
		 									 {//4
		 										 if (fine_color_balance<inten_opponent1)
		 										 {//00 the left part of triangle2
		 										  //blue, cyan, negative, except for colorless
		 										  //introduction of gray shades and the white shade
		 										  /*if(((gray_scale>=62)||(gray_scale<10))&&(abs(fine_color_balance-31)<7)&&(abs(inten_opponent1-31)<7))
		 										  {//last_cor03.04.15//last_cor06.02.18
		 										  actual_inten_opponent1=(int)inten_opponent1;
		 										  inten_opponent1=graygrades2[scale_pos];
		 										  //inten_opponent1=135;
		 										  goto L;
		 										  }*/
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05) &&
		 												 (gray_scale <= 30))
		 											 {//last_cor03.04.15
		 												 actual_inten_opponent1 = (int)inten_opponent1;
		 												 inten_opponent1 = graygrades2[scale_pos];
		 												 goto L;
		 											 }
		 											 else
		 											 {
		 												 if ((abs(fine_color_balance - 31) <= 4) && (abs(inten_opponent1 - 31) <= 3) &&
		 													 (((gray_scale >= 60) && (satur<0.05)) || (gray_scale<14)))
		 												 {//last_cor20.05.15
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades[scale_pos];
		 													 goto L;
		 												 }
		 												 /*else
		 												 {//last_cor13.06.13
		 												 if((fine_color_balance<30)&&(inten_opponent1<=30)&&(gray_scale<=8))
		 												 {//last_cor02.04.15
		 												 actual_inten_opponent1=(int)inten_opponent1;
		 												 inten_opponent1=graygrades[scale_pos];
		 												 if(inten_opponent1==135)
		 												 {//last_cor02.04.15
		 												 actual_inten_opponent1=31;
		 												 fine_color_balance=31;
		 												 }
		 												 goto L;
		 												 }
		 												 }//last_cor13.06.13*/
		 											 }
		 											 if ((fine_color_balance <= 10) && (abs(inten_opponent1 - 31)<2) && (gray_scale >= 60) && (satur<0.05))
		 											 {//last_cor03.04.15last_cor26.02.18
		 												 actual_inten_opponent1 = (int)inten_opponent1;
		 												 inten_opponent1 = graygrades2[scale_pos];
		 												 goto L;
		 											 }
		 											 /*if((abs(inten_opponent1-31)<2)&&(gray_scale<=40)&&(satur<0.4))
		 											 {//last_cor28.05.15
		 											 actual_inten_opponent1=(int)inten_opponent1;
		 											 inten_opponent1=graygrades2[scale_pos];
		 											 goto L;
		 											 }*/
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = inten_opponent1 + 39;
		 											 //blue, cyan, only negative
		 										 }//00

		 										 else
		 										 {//3
		 										  //blue, violet without change within the chosen range (22-31); negative and zero
		 										  //introduction of gray shades and the white shade
		 											 if (((gray_scale >= 60) || (gray_scale<8)) && (abs(fine_color_balance - 31)<7) && (abs(inten_opponent1 - 31)<7))
		 											 {//last_cor03.04.15
		 												 actual_inten_opponent1 = (int)inten_opponent1;
		 												 inten_opponent1 = graygrades2[scale_pos];
		 												 //inten_opponent1=135;
		 												 goto L;
		 											 }
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05) && (gray_scale <= 30))
		 											 {//last_cor03.04.15
		 												 actual_inten_opponent1 = (int)inten_opponent1;
		 												 inten_opponent1 = graygrades2[scale_pos];
		 												 goto L;
		 											 }
		 											 /*else
		 											 {
		 											 if((abs(fine_color_balance-31)<=3)&&(abs(inten_opponent1-31)<=3)&&(gray_scale<=20))
		 											 {
		 											 actual_inten_opponent1=(int)inten_opponent1;
		 											 inten_opponent1=graygrades[gray_scale/4];
		 											 goto L;
		 											 }
		 											 }*/
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 goto L;
		 										 }//3

		 									 }//4
		 								 }//5
		 								 else
		 								 {//8
		 									 if ((inten_opponent1>42) && ((inten_opponent1 % 2) == 1))
		 									 {
		 										 inten_opponent1 = inten_opponent1 + 1;
		 									 }
		 									 if (inten_opponent1<right_bound_of_color_resolution)
		 									 {//9

		 										 incr2 = inten_opponent1 - 32;
		 										 if (fine_color_balance <= 31 - right_shift2[incr2])
		 										 {//the left part of triangle1
		 										  //red, orange; nonstandard division oriented to natural scenes, only negative
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05) && (gray_scale <= 30))
		 											 {//last_cor03.04.15
		 												 actual_inten_opponent1 = (int)inten_opponent1;
		 												 inten_opponent1 = graygrades2[scale_pos];
		 												 goto L;
		 											 }
		 											 else
		 											 {
		 												 if ((abs(fine_color_balance - 31) <= 3) && (abs(inten_opponent1 - 31) <= 3) && (gray_scale <= 8))
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades1[scale_pos];
		 													 goto L;
		 												 }
		 											 }
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = inten_opponent1 + 49;
		 											 goto L;
		 										 }//the left part of triangle1
		 										 else
		 										 {//000
		 											 if (fine_color_balance >= 31 + right_shift1[incr2])
		 											 {//001the middle part of triangle1
		 											  //green-yellow, cyan, only positive
		 											  //introduction of gray shades and the white shade
		 												 if (num_strip <= (NumbStr / 2))
		 												 {//last_cor23.04.15
		 													 if ((((gray_scale<7) && (satur<0.05)) || ((gray_scale>52) && (satur<0.1)))
		 														 && (abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31)<2))
		 													 {//last_cor23.04.15
		 														 actual_inten_opponent1 = (int)inten_opponent1;
		 														 inten_opponent1 = graygrades2[scale_pos];//last_cor23.04.15
		 														 goto L;
		 													 }
		 												 }
		 												 else
		 												 {
		 													 if ((gray_scale<7) && (abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 													 {//last_cor23.04.15
		 														 actual_inten_opponent1 = (int)inten_opponent1;
		 														 inten_opponent1 = graygrades2[scale_pos];//last_cor23.04.15
		 														 goto L;
		 													 }
		 												 }//last_cor23.04.15

		 												 if (gray_scale<12)
		 												 {//<12
		 													 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 													 {
		 														 actual_inten_opponent1 = (int)inten_opponent1;
		 														 inten_opponent1 = graygrades2[scale_pos];
		 														 goto L;
		 													 }
		 													 /* if(fine_color_balance<=33)
		 													 {
		 													 if((inten_opponent1>40)||(inten_opponent1<=33))
		 													 {
		 													 actual_inten_opponent1=(int)inten_opponent1;
		 													 inten_opponent1=135;
		 													 goto L;
		 													 }
		 													 }*/
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades1[scale_pos];
		 													 if (inten_opponent1 == 135)
		 													 {//last_cor02.04.15
		 														 actual_inten_opponent1 = 31;
		 														 fine_color_balance = 31;
		 													 }
		 													 goto L;
		 												 }//<12

		 												 if (gray_scale <= 20)
		 												 {//003
		 													 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 													 {
		 														 actual_inten_opponent1 = (int)inten_opponent1;
		 														 inten_opponent1 = graygrades2[scale_pos];
		 														 goto L;
		 													 }
		 													 /*if(((fine_color_balance-31)<=3)&&((inten_opponent1-31)<=3)&&(satur<0.2))
		 													 {
		 													 actual_inten_opponent1=(int)inten_opponent1;
		 													 inten_opponent1=graygrades1[gray_scale/4];
		 													 goto L;
		 													 }*/
		 													 if (inten_opponent1>40)
		 													 {
		 														 if (fine_color_balance <= 34)
		 														 {
		 															 actual_inten_opponent1 = (int)inten_opponent1;
		 															 inten_opponent1 = graygrades1[scale_pos];
		 															 goto L;
		 														 }
		 													 }
		 													 if (fine_color_balance >= 40)
		 													 {
		 														 if (inten_opponent1 <= 34)
		 														 {
		 															 actual_inten_opponent1 = (int)inten_opponent1;
		 															 inten_opponent1 = graygrades1[scale_pos];
		 															 goto L;
		 														 }
		 													 }

		 												 }//003
		 												 if (num_strip <= (NumbStr / 2))
		 												 {//last_cor23.04.15
		 													 if (((fine_color_balance - 31) <= 2) && ((inten_opponent1 - 31) <= 2) &&
		 														 ((satur<0.05) || ((gray_scale >= 50) && (satur<0.1)) || (((gray_scale <= 8) && (satur <= 0.1)) ||
		 														 ((gray_scale <= 12) && (satur <= 0.08)))))
		 													 {//last_cor06.04.15and23.04.15
		 														 actual_inten_opponent1 = (int)inten_opponent1;
		 														 inten_opponent1 = graygrades2[scale_pos];
		 														 goto L;
		 													 }
		 													 else
		 													 {//last_cor23.04.15
		 														 if (((inten_opponent1 - 31) <= 1) && ((fine_color_balance - 31) <= 2))
		 														 {
		 															 if ((gray_scale >= 50) && (satur<0.1))
		 															 {
		 																 actual_inten_opponent1 = (int)inten_opponent1;
		 																 inten_opponent1 = graygrades2[scale_pos];
		 																 goto L;
		 															 }
		 														 }
		 													 }//last_cor23.04.15
		 												 }
		 												 else
		 												 {
		 													 if (((fine_color_balance - 31) <= 2) && ((inten_opponent1 - 31) <= 2) &&
		 														 ((satur<0.05) || ((gray_scale >= 60) && (satur<0.03)) || (((gray_scale <= 8) && (satur <= 0.1)) ||
		 														 ((gray_scale <= 12) && (satur <= 0.08)))))
		 													 {//last_cor06.04.15and23.04.15
		 														 actual_inten_opponent1 = (int)inten_opponent1;
		 														 inten_opponent1 = graygrades2[scale_pos];
		 														 goto L;
		 													 }
		 												 }

		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = inten_opponent1 + 63;
		 												 goto L;
		 											 }//001
		 											 else
		 											 {//007
		 											  //without change of the value, from orange to green-yellow within the chosen range(32-45), zero and negative
		 											  //introduction of gray shades and the white shade
		 												 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades2[scale_pos];
		 													 goto L;
		 												 }

		 												 if ((gray_scale<7) && (abs(fine_color_balance - 31) <= 1))
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = 135;
		 													 goto L;
		 												 }
		 												 if (gray_scale<12)
		 												 {//<12
		 													 if ((abs(fine_color_balance - 31)<3) && (abs(inten_opponent1 - 31)<3) && (satur<0.05) && (gray_scale <= 30))
		 													 {//last_cor03.04.15
		 														 actual_inten_opponent1 = (int)inten_opponent1;
		 														 inten_opponent1 = graygrades2[scale_pos];
		 														 goto L;
		 													 }
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades1[scale_pos];
		 													 goto L;
		 												 }//<12
		 												 if (gray_scale <= 20)
		 												 {//003
		 													 if ((abs(fine_color_balance - 31)<3) && (abs(inten_opponent1 - 31)<3) && (satur<0.05) && (gray_scale <= 30))
		 													 {//last_cor03.04.15
		 														 actual_inten_opponent1 = (int)inten_opponent1;
		 														 inten_opponent1 = graygrades2[scale_pos];
		 														 goto L;
		 													 }
		 													 /*if((inten_opponent1>40))
		 													 {
		 													 if((inten_opponent1<=35)&&(satur<0.35))
		 													 {
		 													 actual_inten_opponent1=(int)inten_opponent1;
		 													 inten_opponent1=graygrades1[gray_scale/4];
		 													 goto L;
		 													 }
		 													 }*/

		 												 }//003
		 												 if (gray_scale >= 60)
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades2[scale_pos];
		 													 goto L;
		 												 }

		 												 actual_inten_opponent1 = inten_opponent1;
		 												 goto L;
		 												 //without change of the value, from orange to green-yellow within the chosen range(32-45), zero and negative
		 											 }//007
		 										 }//000

		 									 }//9
		 									 else
		 									 {//10
		 									  //range (46-60)

		 										 if (fine_color_balance>31)
		 										 {
		 											 //green-yellow, green-cyan; a nonstandard division oriented to natural scenes, only positive
		 											 //black, gray and white shades have not been chosen in this range yet
		 											 if (gray_scale <= 10)
		 											 {//003last_cor03.09.13

		 												 if (fine_color_balance <= 36)
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades1[scale_pos];
		 													 goto L;
		 												 }

		 											 }//003
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = inten_opponent1 + 63;
		 											 goto L;
		 										 }
		 										 else
		 										 {
		 											 //without change of the value, from red to yellow-green within the chosen range(46-60), zero and negative

		 											 /*if(gray_scale<=12)
		 											 {
		 											 actual_inten_opponent1=(int)inten_opponent1;
		 											 inten_opponent1=graygrades1[gray_scale/4];
		 											 goto L;
		 											 }*/

		 											 actual_inten_opponent1 = inten_opponent1;
		 											 goto L;
		 										 }
		 									 }//10
		 								 }//8
		 							 }//6
		 							 else
		 							 {//12
		 							  //from red to slightly green blue for the others from (0-21), deviations are positive, zero, and negative
		 							  //black, gray and white shades have not been chosen in this range yet and the range was not separated
		 							  //the coordinate system G/(G+B), R/(R+B)is employed
		 								 if ((inten_opponent1<16) && ((inten_opponent1 % 2) == 1))
		 								 {
		 									 inten_opponent1 = inten_opponent1 + 1;
		 								 }
		 								 actual_inten_opponent1 = inten_opponent1;
		 								 goto L;
		 							 }//12
		 						 }//7
		 						 else
		 						 {//0000
		 						  //inverted color ratios (G/(G+R), G/(G+B))
		 						  //two coordinate systems G/(G+R), G/(G+B) and G/(G+R), R/(R+B)
		 						  //the first coordinate system is used for points where G/(G+R)<=0.5, the second one for the points where G/(G+R)>0.5
		 							 if (inten_red + inten_green != 0)
		 							 {
		 								 interm = (int)((255 * (long)inten_green) / (inten_green + inten_red));

		 								 inten_opponent1 = (unsigned char)(correct_int[interm]);
		 								 if (inten_opponent1 == 31)
		 								 {
		 									 if (inten_green>inten_red)
		 									 {
		 										 inten_opponent1 += 1;
		 									 }
		 									 else
		 									 {//last_cor19.05.15
		 										 if ((inten_blue - inten_green) >= 2)
		 										 {
		 											 inten_opponent1 -= 1;
		 										 }
		 									 }//last_cor19.05.15
		 								 }
		 								 if (inten_opponent1<32)
		 								 {
		 									 fine_color_balance = ((int)(255 * (long)inten_red / (inten_red + inten_blue))) / 4;
		 									 if (inten_red>inten_blue)
		 									 {
		 										 if (fine_color_balance == 31)
		 										 {
		 											 fine_color_balance += 1;
		 										 }
		 									 }
		 									 if (inten_red<inten_blue)
		 									 {
		 										 if (fine_color_balance == 31)
		 										 {
		 											 fine_color_balance -= 1;
		 										 }
		 									 }
		 									 //in this half-triangle this function is determined everywhere
		 								 }
		 								 else
		 								 {
		 									 fine_color_balance = ((int)(255 * (long)inten_green / (inten_blue + inten_green))) / 4;
		 									 //in this half-triangle this function is determined everywhere
		 								 }
		 							 }
		 							 else
		 							 {
		 								 if (!inten_blue)
		 								 {
		 									 //pure dark
		 									 actual_inten_opponent1 = 31;
		 									 inten_opponent1 = 135;
		 									 fine_color_balance = 31;
		 									 goto L;
		 								 }
		 								 else
		 								 {
		 									 //pure blue
		 									 interm = 127;
		 									 inten_opponent1 = (unsigned char)(correct_int[interm]);
		 									 fine_color_balance = ((int)(255 * (long)inten_red / (inten_red + inten_blue))) / 4;
		 									 actual_inten_opponent1 = inten_opponent1;
		 									 inten_opponent1 = inten_opponent1 + 39;
		 									 fine_color_balance = 0;
		 									 goto L;
		 								 }

		 							 }

		 							 if (inten_opponent1>left_bound_of_color_resolution)
		 							 {//1coordinates G/(G+R), R/(R+B)
		 								 if (inten_opponent1<32)
		 								 {//2
		 									 incr1 = 31 - inten_opponent1;
		 									 if (fine_color_balance <= 31)
		 									 {//3 the right part of triangle3
		 									  //from blue to violet,  negative and zero
		 									  //introduction of the black shade, gray shades and the white shade
		 										 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 										 {
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = graygrades[gray_scale / 4];
		 											 goto L;
		 										 }
		 										 else
		 										 {
		 											 if ((abs(fine_color_balance - 31) <= 3) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 55)
		 												 && (satur<0.05))
		 											 {
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = graygrades[gray_scale / 4];
		 												 goto L;
		 											 }
		 										 }
		 										 actual_inten_opponent1 = inten_opponent1;
		 										 inten_opponent1 = inten_opponent1 + 39;
		 										 goto L;
		 									 }//3
		 									 else
		 									 {//4
		 										 if ((63 - fine_color_balance)<inten_opponent1)
		 										 {
		 											 //red, green-yellow, only positive
		 											 //black, gray and white shades have not been chosen in this range yet
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.1))
		 											 {
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = graygrades[gray_scale / 4];
		 												 goto L;
		 											 }
		 											 else
		 											 {
		 												 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 58))
		 												 {
		 													 actual_inten_opponent1 = inten_opponent1;
		 													 inten_opponent1 = graygrades[gray_scale / 4];
		 													 goto L;
		 												 }
		 											 }
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = inten_opponent1 + 49;
		 											 goto L;
		 										 }
		 										 //from violet to red for the points within the range (22-31) without change, only positive
		 										 //introduction of the black shade, gray shades, and the white shade
		 										 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 										 {
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = graygrades[gray_scale / 4];
		 											 goto L;
		 										 }
		 										 actual_inten_opponent1 = inten_opponent1;
		 									 }//4
		 									 goto L;
		 								 }//2
		 								 else
		 								 {//5 G/(G+R), G/(G+B) coordinates
		 									 if (inten_opponent1<right_bound_of_color_resolution)
		 									 {//6
		 										 incr2 = inten_opponent1 - 32;
		 										 if (fine_color_balance <= 31)
		 										 {//the left part of triangle2
		 										  //blue, cyan, negative and zero
		 										  //introduction of the black shade, gray shades, and the white shade
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.1))
		 											 {
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = graygrades1[gray_scale / 4];
		 												 goto L;
		 											 }
		 											 else
		 											 {
		 												 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 55) && (satur<0.05))
		 												 {
		 													 actual_inten_opponent1 = inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto L;
		 												 }
		 											 }
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = inten_opponent1 + 49;
		 											 goto L;
		 										 }
		 										 else
		 										 {
		 											 if (fine_color_balance >= inten_opponent1)
		 											 {//the left part of triangle1
		 											  //green, green-yellow, only positive
		 												 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.1))
		 												 {
		 													 actual_inten_opponent1 = inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto L;
		 												 }
		 												 else
		 												 {
		 													 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 58))
		 													 {
		 														 actual_inten_opponent1 = inten_opponent1;
		 														 inten_opponent1 = graygrades1[gray_scale / 4];
		 														 goto L;
		 													 }
		 												 }
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = inten_opponent1 + 63;
		 											 }
		 											 //cyan, green without change within the range (32-45), only positive
		 											 //the right part of triangle2
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.1))
		 											 {
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = graygrades1[gray_scale / 4];
		 												 goto L;
		 											 }
		 											 else
		 											 {
		 												 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 55))
		 												 {
		 													 actual_inten_opponent1 = inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto L;
		 												 }
		 											 }
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 goto L;
		 										 }
		 									 }//6
		 									 else
		 									 {//7
		 									  //range (46-60)
		 										 if (fine_color_balance>31)
		 										 {
		 											 //cyan, green-a bit yellow, only positive; a nonstandard division oriented to natural scenes
		 											 //the small left part of triangle1 and the right part of triangle2
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.1))
		 											 {
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = graygrades1[gray_scale / 4];
		 												 goto L;
		 											 }
		 											 else
		 											 {
		 												 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 55))
		 												 {
		 													 actual_inten_opponent1 = inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto L;
		 												 }
		 											 }
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = inten_opponent1 + 63;
		 											 goto L;
		 										 }

		 										 //without change of the value, from blue to cyan within the chosen range(46-60), zero and negative
		 										 //the left part of triangle2
		 										 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.1))
		 										 {
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = graygrades1[gray_scale / 4];
		 											 goto L;
		 										 }
		 										 else
		 										 {
		 											 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 55))
		 											 {
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = graygrades1[gray_scale / 4];
		 												 goto L;
		 											 }
		 										 }
		 										 actual_inten_opponent1 = inten_opponent1;
		 									 }//7
		 								 }//5
		 							 }//1
		 							 else
		 							 {//8
		 							  //range (0-21) from orange to blue for the others, negative, zero, and positive deviations
		 							  //the coordinate system G/(G+R), R/(R+B)
		 								 actual_inten_opponent1 = inten_opponent1;
		 								 goto L;

		 							 }//8

		 						 }
		 					 }//11
		 					 else
		 					 {
		 						 actual_inten_opponent1 = 31;
		 						 inten_opponent1 = 135;
		 						 fine_color_balance = 31;
		 						 //inten_opponent2=0;

		 					 }



		 				 L:
		 					 /*if((inten_opponent1==31)&&(num_strip>=12)&&(fine_color_balance>40))
		 					 {
		 					 inten_opponent1=inten_opponent1;
		 					 }*/
		 					 inten_count1 =
		 						 frequency_of_color_differ + ((int)inten_opponent1)*(DimDifference1);

		 					 if (num_of_int[inten_opponent1]<MAX_INT_NUMBER - 1)
		 					 {
		 						 StripCharacteristicsFinding(inten_opponent1, gray_scale, actual_inten_opponent1,
		 							 inten_count1, ncoor2, hist_sum, hist_fun, first_pix, last_pix, beg_point, end_point,
		 							 thick_first, thick_last, thick_stat_input, import_end, last_end,
		 							 jump_len, jump_end, num_of_int, import_beg, valuable_intensity,
		 							 bright_consistency, thick_beg, thick_end, thick_stat, thick_prev_beg,
		 							 thick_prev_end, thick_prev_stat, thick_break_beg, thick_break_end,
		 							 thick_break_stat, 0, fine_color_balance,point_sat);

		 					 }


		 //StripCharacteristicsFindingGray(gray_scale,ncoor2,first_pixg,last_pixg,last_endg);

		 	}
		 }
		 /*if(pApp->m_ParamHaveBeenChanged)
		   {
		 	  return;
		 } */
		 FinalCorrection(thick_last,gray_min,gray_max,gray_mean,opponent1_min,opponent1_max,opponent1_mean,
		 thick_beg,thick_end,import_beg,import_end,thick_break_beg,thick_break_end,thick_prev_beg,thick_prev_end,
		 thick_first,num_of_int,hist_fun,hist_sum,thick_stat,thick_stat_input,
		 last_end,jump_len,jump_end,thick_break_stat,thick_prev_stat,
		 valuable_intensity,bright_consistency,frequency_of_color_differ,0);

		 FinalCorrectionGray(last_endg);

		 				}
		 				else
		 				{
		 				point_start=StripWidthPrev*3*num_strip;
		 for(ncoor1=0;ncoor1<DimY;ncoor1++)
		 {
		 entry_point=(long)ncoor1*(long)(con_Dim)+(long)(point_start);


		 	for(ncoor2=0;ncoor2<StripWidth;ncoor2++)
		  {

		      inten_blue=*(intensi+entry_point++);
		      inten_green=*(intensi+entry_point++);
		      inten_red=*(intensi+entry_point++);

		  max_intensity=std::max(inten_blue,std::max(inten_green,inten_red));
		  min_intensity=std::min(inten_blue,std::min(inten_green,inten_red));
		  satur=((float)(max_intensity-min_intensity)/((float) max_intensity));
		  point_sat = (int)(15 * satur);
		  gray_scale=BLUE_COEF*inten_blue+GREEN_COEF*inten_green+RED_COEF*inten_red;
		 	  gray_scale=gray_scale >> 2;
		 	  if(num_of_intg[gray_scale]<MAX_INT_NUMBER-1)
		       {

		 StripCharacteristicsFindingGray(gray_scale,ncoor1,first_pixg,last_pixg,last_endg);
		 	  }
		 	   if(max_intensity>=LOW_INTENSITY)
		 	 {//11
		 		                 if(NumPair)
		 		                 {//7
		 //two coordinate systems G/(G+B), G/(G+R) and G/(G+B), R/(R+B) are imployed
		 //the first coordinate system is used for the points where G/(G+B)>0.5, the second one, for the points where G/(G+B)<=0.5
		 							 if (inten_blue + inten_green != 0)
		 							 {
		 								 interm = (int)((255 * (long)inten_green) / (inten_green + inten_blue));

		 								 residual = ((255 * (long)inten_green) % (inten_green + inten_blue));
		 								 if (residual >= (inten_green + inten_blue) / 2)
		 								 {
		 									 if (interm != 255)
		 									 {
		 										 interm += 1;
		 									 }
		 								 }
		 								 inten_opponent1 = (unsigned char)(correct_int[interm]);
		 								 if (inten_opponent1 == 31)
		 								 {
		 									 if (inten_green>inten_blue)
		 									 {
		 										 inten_opponent1 += 1;
		 									 }
		 								 }
		 								 if (inten_opponent1<32)
		 								 {
		 									 fine_color_balance = ((int)(255 * (long)inten_red / (inten_red + inten_blue))) / 4;
		 									 if (inten_red>inten_blue)
		 									 {
		 										 if (fine_color_balance == 31)
		 										 {
		 											 fine_color_balance += 1;
		 										 }
		 									 }
		 									 if (inten_red<inten_blue)
		 									 {
		 										 if (fine_color_balance == 31)
		 										 {
		 											 fine_color_balance -= 1;
		 										 }
		 									 }

		 								 }
		 								 else
		 								 {
		 									 fine_color_balance = ((int)(255 * (long)inten_green / (inten_red + inten_green))) / 4;
		 									 if (inten_green<inten_red)
		 									 {
		 										 if ((fine_color_balance == 31))
		 										 {
		 											 fine_color_balance -= 1;
		 										 }
		 									 }
		 									 if (inten_green>inten_red)
		 									 {
		 										 if ((fine_color_balance == 31))
		 										 {
		 											 fine_color_balance += 1;
		 										 }
		 									 }
		 								 }
		 								 //fine_color_balance1=((int)(255*(long)inten_green/(inten_red+inten_green)))/4;
		 							 }
		 							 else
		 							 {
		 								 if (!inten_red)
		 								 {
		 									 //pure dark
		 									 actual_inten_opponent1 = 31;
		 									 inten_opponent1 = 135;
		 									 fine_color_balance = 31;
		 									 goto M;
		 								 }
		 								 else
		 								 {
		 									 //pure red
		 									 interm = 127;
		 									 inten_opponent1 = (unsigned char)(correct_int[interm]);
		 									 fine_color_balance = ((int)(255 * (long)inten_red / (inten_red + inten_blue))) / 4;
		 									 if (fine_color_balance == 31)
		 									 {
		 										 if (inten_red>inten_blue)
		 										 {
		 											 fine_color_balance += 1;
		 										 }
		 									 }
		 									 actual_inten_opponent1 = inten_opponent1;
		 									 inten_opponent1 = inten_opponent1 + 49;
		 									 goto M;
		 								 }
		 							 }

		 							 if (inten_opponent1>left_bound_of_color_resolution)
		 							 {//6
		 								 if (inten_opponent1<32)
		 								 {//5
		 								  //the coordinate system on the color triangle is generated by G/(G+B) and R/(R+B)
		 									 incr1 = 31 - inten_opponent1;


		 									 if (fine_color_balance>31)
		 									 {//triangle3
		 										 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 										 {
		 											 actual_inten_opponent1 = (int)inten_opponent1;
		 											 inten_opponent1 = graygrades[gray_scale / 4];
		 											 goto M;
		 										 }
		 										 else
		 										 {
		 											 if ((abs(fine_color_balance - 31) <= 5) && (abs(inten_opponent1 - 31) <= 5) && ((gray_scale >= 50) || (gray_scale<12)))
		 											 {
		 												 actual_inten_opponent1 = (int)inten_opponent1;
		 												 inten_opponent1 = graygrades[gray_scale / 4];
		 												 goto M;
		 											 }
		 										 }
		 										 actual_inten_opponent1 = inten_opponent1;
		 										 inten_opponent1 = inten_opponent1 + 49;
		 										 goto M;
		 										 //red, violet, only positive deviations
		 										 //colorless, gray, white pixels are not separated
		 									 }
		 									 else
		 									 {//4
		 										 if (fine_color_balance <= inten_opponent1)
		 										 {//00 the left part of triangle2
		 										  //blue, cyan, negative, except for colorless
		 										  //introduction of gray shades and the white shade
		 											 if ((gray_scale<8) && (abs(fine_color_balance - 31)<7) && (abs(inten_opponent1 - 31)<7))
		 											 {
		 												 actual_inten_opponent1 = (int)inten_opponent1;
		 												 inten_opponent1 = 135;
		 												 goto M;
		 											 }
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 											 {
		 												 actual_inten_opponent1 = (int)inten_opponent1;
		 												 inten_opponent1 = graygrades[gray_scale / 4];
		 												 goto M;
		 											 }
		 											 else
		 											 {
		 												 if ((abs(fine_color_balance - 31) <= 4) && (abs(inten_opponent1 - 31) <= 3) && ((gray_scale >= 50) || (gray_scale<12)))
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades[gray_scale / 4];
		 													 goto M;
		 												 }
		 											 }
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = inten_opponent1 + 39;
		 											 //blue, cyan, only negative
		 										 }//00

		 										 else
		 										 {//3
		 										  //blue, violet without change within the chosen range (22-31); negative and zero
		 										  //introduction of gray shades and the white shade
		 											 if ((gray_scale<8) && (abs(fine_color_balance - 31)<7) && (abs(inten_opponent1 - 31)<7))
		 											 {
		 												 actual_inten_opponent1 = (int)inten_opponent1;
		 												 inten_opponent1 = 135;
		 												 goto M;
		 											 }
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 											 {
		 												 actual_inten_opponent1 = (int)inten_opponent1;
		 												 inten_opponent1 = graygrades[gray_scale / 4];
		 												 goto M;
		 											 }
		 											 else
		 											 {
		 												 if ((abs(fine_color_balance - 31) <= 7) && (abs(inten_opponent1 - 31) <= 5) && ((gray_scale >= 50) || (gray_scale<12)))
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades[gray_scale / 4];
		 													 goto M;
		 												 }
		 											 }
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 goto M;
		 										 }//3

		 									 }//4
		 								 }//5
		 								 else
		 								 {//8
		 									 if ((inten_opponent1>42) && ((inten_opponent1 % 2) == 1))
		 									 {
		 										 inten_opponent1 = inten_opponent1 + 1;
		 									 }
		 									 if (inten_opponent1<right_bound_of_color_resolution)
		 									 {//9

		 										 incr2 = inten_opponent1 - 32;
		 										 if (fine_color_balance <= 31 - right_shift2[incr2])
		 										 {//the left part of triangle1
		 										  //red, orange; nonstandard division oriented to natural scenes, only negative
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = inten_opponent1 + 49;
		 											 goto M;
		 										 }
		 										 else
		 										 {//000
		 											 if (fine_color_balance >= 31 + right_shift1[incr2])
		 											 {//001the middle part of triangle1
		 											  //green-yellow, cyan, only positive
		 											  //introduction of gray shades and the white shade
		 												 if ((gray_scale<7) && (abs(fine_color_balance - 31) <= 2))
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = 135;
		 													 goto M;
		 												 }

		 												 if (gray_scale<12)
		 												 {
		 													 if (fine_color_balance <= 33)
		 													 {
		 														 if ((inten_opponent1>40) || (inten_opponent1 <= 33))
		 														 {
		 															 actual_inten_opponent1 = (int)inten_opponent1;
		 															 inten_opponent1 = 135;
		 															 goto M;
		 														 }
		 													 }
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto M;
		 												 }

		 												 if (gray_scale <= 10)
		 												 {//003last_cor03.09.13
		 													 if (((fine_color_balance - 31) <= 3) && ((inten_opponent1 - 31) <= 3) && (satur<0.1))
		 													 {
		 														 actual_inten_opponent1 = (int)inten_opponent1;
		 														 inten_opponent1 = graygrades1[gray_scale / 4];
		 														 goto M;
		 													 }

		 													 if ((inten_opponent1 <= 40))
		 													 {
		 														 if (fine_color_balance>40)
		 														 {
		 															 actual_inten_opponent1 = (int)inten_opponent1;
		 															 inten_opponent1 = graygrades1[gray_scale / 4];
		 															 goto M;
		 														 }
		 													 }

		 												 }//003
		 												 if (((fine_color_balance - 31) <= 3) && ((inten_opponent1 - 31) <= 2) && ((satur<0.15) || (gray_scale >= 50)))
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto M;
		 												 }
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = inten_opponent1 + 63;
		 												 goto M;
		 											 }//001
		 											 else
		 											 {//007
		 											  //without change of the value, from orange to green-yellow within the chosen range(32-45), zero and negative
		 											  //introduction of gray shades and the white shade
		 												 if ((gray_scale<7) && (abs(fine_color_balance - 31) <= 1))
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = 135;
		 													 goto M;
		 												 }

		 												 if (gray_scale<12)
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto M;
		 												 }
		 												 if (gray_scale <= 20)
		 												 {//003
		 													 if ((inten_opponent1 <= 40))
		 													 {
		 														 if ((inten_opponent1 <= 35) && (satur<0.35))
		 														 {
		 															 actual_inten_opponent1 = (int)inten_opponent1;
		 															 inten_opponent1 = graygrades1[gray_scale / 4];
		 															 goto M;
		 														 }
		 													 }
		 													 else
		 													 {//004
		 														 actual_inten_opponent1 = (int)inten_opponent1;
		 														 inten_opponent1 = graygrades1[gray_scale / 4];
		 														 goto M;
		 													 }//004
		 												 }//003
		 												 if (gray_scale >= 55)
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto M;
		 												 }
		 												 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.1))
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto M;
		 												 }

		 												 actual_inten_opponent1 = inten_opponent1;
		 												 goto M;

		 												 //without change of the value, from orange to green-yellow within the chosen range(32-45), zero and negative
		 											 }//007
		 										 }//000

		 									 }//9
		 									 else
		 									 {//10
		 									  //range (46-60)
		 										 if (fine_color_balance>31)
		 										 {
		 											 //green-yellow, green-cyan; a nonstandard division oriented to natural scenes, only positive
		 											 //black, gray and white shades have not been chosen in this range yet
		 											 if (gray_scale <= 12)
		 											 {//003last_cor03.09.13
		 												 if (fine_color_balance <= 36)
		 												 {
		 													 actual_inten_opponent1 = (int)inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto M;
		 												 }
		 											 }//003

		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = inten_opponent1 + 63;
		 											 goto M;
		 										 }


		 										 //without change of the value, from red to yellow-green within the chosen range(46-60), zero and negative
		 										 //black, gray and white shades are not chosen in this range yet
		 										 actual_inten_opponent1 = inten_opponent1;
		 										 goto M;
		 									 }//10
		 								 }//8
		 							 }//6
		 							 else
		 							 {//12
		 							  //from red to slightly green blue for the others from (0-21), deviations are positive, zero, and negative
		 							  //black, gray and white shades have not been chosen in this range yet and the range was not separated
		 							  //the coordinate system G/(G+B), R/(R+B)is employed

		 								 if ((inten_opponent1<16) && ((inten_opponent1 % 2) == 1))
		 								 {
		 									 inten_opponent1 = inten_opponent1 + 1;
		 								 }
		 								 actual_inten_opponent1 = inten_opponent1;
		 								 goto M;
		 								 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 								 {
		 									 actual_inten_opponent1 = inten_opponent1;
		 									 inten_opponent1 = graygrades[gray_scale / 4];
		 									 goto M;
		 								 }
		 								 else
		 								 {
		 									 if ((abs(fine_color_balance - 31) <= 3) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 50))
		 									 {
		 										 actual_inten_opponent1 = inten_opponent1;
		 										 inten_opponent1 = graygrades[gray_scale / 4];
		 										 goto M;
		 									 }
		 								 }
		 								 actual_inten_opponent1 = inten_opponent1;
		 								 goto M;
		 							 }//12
		 						 }//7
		 						 else
		 						 {//0000
		 						  //inverted color ratios (G/(G+R), G/(G+B))
		 						  //two coordinate systems G/(G+R), G/(G+B) and G/(G+R), R/(R+B)
		 						  //the first coordinate system is used for points where G/(G+R)<=0.5, the second one for the points where G/(G+R)>0.5
		 							 if (inten_red + inten_green != 0)
		 							 {
		 								 interm = (int)((255 * (long)inten_green) / (inten_green + inten_red));

		 								 inten_opponent1 = (unsigned char)(correct_int[interm]);
		 								 if (inten_opponent1 == 31)
		 								 {
		 									 if (inten_green>inten_red)
		 									 {
		 										 inten_opponent1 += 1;
		 									 }
		 								 }
		 								 if (inten_opponent1<32)
		 								 {
		 									 fine_color_balance = ((int)(255 * (long)inten_red / (inten_red + inten_blue))) / 4;
		 									 if (inten_red>inten_blue)
		 									 {
		 										 if (fine_color_balance == 31)
		 										 {
		 											 fine_color_balance += 1;
		 										 }
		 									 }
		 									 if (inten_red<inten_blue)
		 									 {
		 										 if (fine_color_balance == 31)
		 										 {
		 											 fine_color_balance -= 1;
		 										 }
		 									 }
		 								 }
		 								 else
		 								 {
		 									 fine_color_balance = ((int)(255 * (long)inten_green / (inten_blue + inten_green))) / 4;
		 								 }
		 							 }
		 							 else
		 							 {
		 								 if (!inten_blue)
		 								 {
		 									 //pure dark
		 									 actual_inten_opponent1 = 31;
		 									 inten_opponent1 = 135;
		 									 fine_color_balance = 31;
		 									 goto M;
		 								 }
		 								 else
		 								 {
		 									 //pure blue
		 									 interm = 127;
		 									 inten_opponent1 = (unsigned char)(correct_int[interm]);
		 									 //fine_color_balance=((int)(255*(long)inten_red/(inten_red+inten_blue)))/4;
		 									 actual_inten_opponent1 = inten_opponent1;
		 									 inten_opponent1 = inten_opponent1 + 39;
		 									 fine_color_balance = 0;
		 									 goto M;
		 								 }

		 							 }

		 							 if (inten_opponent1>left_bound_of_color_resolution)
		 							 {//1coordinates G/(G+R), R/(R+B)
		 								 if (inten_opponent1<32)
		 								 {//2
		 									 incr1 = 31 - inten_opponent1;
		 									 if (fine_color_balance <= 31)
		 									 {//3 the right part of triangle3
		 									  //from blue to violet,  negative and zero
		 									  //introduction of the black shade, gray shades and the white shade
		 										 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 										 {
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = graygrades[gray_scale / 4];
		 											 goto M;
		 										 }
		 										 else
		 										 {
		 											 if ((abs(fine_color_balance - 31) <= 3) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 55))
		 											 {
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = graygrades[gray_scale / 4];
		 												 goto M;
		 											 }
		 										 }
		 										 actual_inten_opponent1 = inten_opponent1;
		 										 inten_opponent1 = inten_opponent1 + 39;
		 										 goto M;
		 									 }//3
		 									 else
		 									 {//4
		 										 if ((63 - fine_color_balance)<inten_opponent1)
		 										 {
		 											 //red, green-yellow, only positive
		 											 //black, gray and white shades have not been chosen in this range yet
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 											 {
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = graygrades[gray_scale / 4];
		 												 goto M;
		 											 }
		 											 else
		 											 {
		 												 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 58))
		 												 {
		 													 actual_inten_opponent1 = inten_opponent1;
		 													 inten_opponent1 = graygrades[gray_scale / 4];
		 													 goto M;
		 												 }
		 											 }
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = inten_opponent1 + 49;
		 											 goto M;
		 										 }
		 										 //from violet to red for the points within the range (22-31) without change, only positive
		 										 //introduction of the black shade, gray shades, and the white shade
		 										 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31)<2) && (satur<0.05))
		 										 {
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = graygrades[gray_scale / 4];
		 											 goto M;
		 										 }
		 										 actual_inten_opponent1 = inten_opponent1;
		 										 goto M;
		 									 }//4
		 									 goto M;
		 								 }//2
		 								 else
		 								 {//5 G/(G+R), G/(G+B) coordinates
		 									 if (inten_opponent1<right_bound_of_color_resolution)
		 									 {//6
		 										 incr2 = inten_opponent1 - 32;
		 										 if (fine_color_balance <= 31)
		 										 {//the left part of triangle2
		 										  //blue, cyan, negative and zero
		 										  //introduction of the black shade, gray shades, and the white shade
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.1))
		 											 {
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = graygrades1[gray_scale / 4];
		 												 goto M;
		 											 }
		 											 else
		 											 {
		 												 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 50))
		 												 {
		 													 actual_inten_opponent1 = inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto M;
		 												 }
		 											 }
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = inten_opponent1 + 49;
		 											 goto M;
		 										 }
		 										 else
		 										 {
		 											 if (fine_color_balance >= inten_opponent1)
		 											 {//the left part of triangle1
		 											  //green, green-yellow, only positive
		 												 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.1))
		 												 {
		 													 actual_inten_opponent1 = inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto M;
		 												 }
		 												 else
		 												 {
		 													 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 58))
		 													 {
		 														 actual_inten_opponent1 = inten_opponent1;
		 														 inten_opponent1 = graygrades1[gray_scale / 4];
		 														 goto M;
		 													 }
		 												 }
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = inten_opponent1 + 63;
		 											 }
		 											 //cyan, green without change within the range (32-45), only positive
		 											 //the right part of triangle2
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.1))
		 											 {
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = graygrades1[gray_scale / 4];
		 												 goto M;
		 											 }
		 											 else
		 											 {
		 												 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 55))
		 												 {
		 													 actual_inten_opponent1 = inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto M;
		 												 }
		 											 }
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 goto M;
		 										 }
		 									 }//6
		 									 else
		 									 {//7
		 									  //range (46-60)
		 										 if (fine_color_balance>31)
		 										 {
		 											 //cyan, green-a bit yellow, only positive; a nonstandard division oriented to natural scenes
		 											 //the small left part of triangle1 and the right part of triangle2
		 											 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.1))
		 											 {
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = graygrades1[gray_scale / 4];
		 												 goto M;
		 											 }
		 											 else
		 											 {
		 												 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 55))
		 												 {
		 													 actual_inten_opponent1 = inten_opponent1;
		 													 inten_opponent1 = graygrades1[gray_scale / 4];
		 													 goto M;
		 												 }
		 											 }
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = inten_opponent1 + 63;
		 											 goto M;
		 										 }

		 										 //without change of the value, from blue to cyan within the chosen range(46-60), zero and negative
		 										 //the left part of triangle2
		 										 if ((abs(fine_color_balance - 31)<2) && (abs(inten_opponent1 - 31)<2) && (satur<0.1))
		 										 {
		 											 actual_inten_opponent1 = inten_opponent1;
		 											 inten_opponent1 = graygrades1[gray_scale / 4];
		 											 goto M;
		 										 }
		 										 else
		 										 {
		 											 if ((abs(fine_color_balance - 31) <= 2) && (abs(inten_opponent1 - 31) <= 2) && (gray_scale >= 55))
		 											 {
		 												 actual_inten_opponent1 = inten_opponent1;
		 												 inten_opponent1 = graygrades1[gray_scale / 4];
		 												 goto M;
		 											 }
		 										 }

		 									 }//7
		 								 }//5
		 							 }//1
		 							 else
		 							 {//8
		 							  //range (0-21) from orange to blue for the others, negative, zero, and positive deviations
		 							  //the coordinate system G/(G+R), R/(R+B)
		 								 actual_inten_opponent1 = inten_opponent1;
		 								 goto M;

		 							 }//8

		 						 }
		 					 }//11
		 					 else
		 					 {
		 						 actual_inten_opponent1 = 31;
		 						 inten_opponent1 = 135;
		 						 fine_color_balance = 31;
		 					 }




		 				 M:
		 					 inten_count1 =
		 						 frequency_of_color_differ + ((int)inten_opponent1)*(DimDifference1);
		 					 if (num_of_int[inten_opponent1]<MAX_INT_NUMBER - 1)
		 					 {
		 						 StripCharacteristicsFinding(inten_opponent1, gray_scale, actual_inten_opponent1,
		 							 inten_count1, ncoor1, hist_sum, hist_fun, first_pix, last_pix, beg_point, end_point,
		 							 thick_first, thick_last, thick_stat_input, import_end, last_end,
		 							 jump_len, jump_end, num_of_int, import_beg, valuable_intensity,
		 							 bright_consistency, thick_beg, thick_end, thick_stat, thick_prev_beg,
		 							 thick_prev_end, thick_prev_stat, thick_break_beg, thick_break_end,
		 							 thick_break_stat, 0, fine_color_balance,point_sat);
		 					 }

		 //StripCharacteristicsFindingGray(gray_scale,ncoor1,first_pixg,last_pixg,last_endg);


		 	}
		 }
		 FinalCorrection(thick_last,gray_min,gray_max,gray_mean,opponent1_min,opponent1_max,opponent1_mean,
		 thick_beg,thick_end,import_beg,import_end,thick_break_beg,thick_break_end,thick_prev_beg,
		 thick_prev_end,thick_first,num_of_int,hist_fun,hist_sum,thick_stat,thick_stat_input,
		 last_end,jump_len,jump_end,thick_break_stat,thick_prev_stat,
		 valuable_intensity,bright_consistency,frequency_of_color_differ,0);

		 FinalCorrectionGray(last_endg);

		 		 }
		   delete[] frequency_of_color_differ;
		   	}
//=====================================================
void
CStrip::StripCharacteristicsFinding(unsigned char inten,
		 int gray_intensity, int actual_opponent1, int* dif_int_count1, int coor1,
		 int* hist_su, int* hist_fu, int* first_pi, int* last_pi, int* beg_poin,
		 int* end_poin, int* thick_firs, int* thick_las, int* thick_stat_inpu,
		 int* import_en, int* last_en, int* jump_le, int* jump_en, int* num_of_in,
		 int* import_be, unsigned char* value_int, int* bright_cons, int* thick_be,
		 int* thick_en, int* thick_sta, int* thick_prev_be, int* thick_prev_en,
		 int* thick_prev_sta, int* thick_break_be, int* thick_break_en,
		 int* thick_break_sta, int Coun, int color_balance, int saturation)
 {
	 int ncoor1;
	 int jump_coor, jump_pare, save_beg_pare, save_beg_pare1;
	 int intens_signif;
	 int hole;
	 int intensity_cons;

	 int color_inten;
	 int color_inten1;
	 int color_inten2;
	 int boundary1;
	 int boundary2;
	 int max_bound;//last_cor05.02.18
	 int	min_bound;
	 int min_shift;//last_cor19.03.18
	 int max_bound_cor;//last_cor19.03.18
	 int	min_bound_cor;//last_cor19.03.18
	 int gray_jump_threshold;//last_cor26.03.18
	 int	mean_val;//last_cor05.02.18
	 int interv_length;////last_cor05.02.18
	 int mean_differ;
	 int sign_calc;
	 int jump_pare1;

	 float Aver;
	 float Aver1;
	 int number_mean;

	 ncoor1 = coor1;
	 sign_calc = 0;
	 jump_pare = 0;
	 jump_pare1 = 0;
	 if (saturation >= 4)
	 {
		 gray_jump_threshold = 10;
	 }
	 else
	 {
		 gray_jump_threshold = 6;
	 }



	 if (hist_su[inten] == 0)
	 {
		 first_pi[inten] = ncoor1;
		 last_pi[inten] = ncoor1;
		 hist_su[inten]++;
		 return;
	 }
	 jump_coor = ncoor1 - last_pi[inten];
	 save_beg_pare = end_poin[inten];
	 if (save_beg_pare)
	 {//1

		 jump_pare = ncoor1 - save_beg_pare;
		 //jump_pare1=ncoor1-save_beg_pare1;
		 if (jump_pare == 0)
		 {//2
			 hist_su[inten]++;
			 if (!thick_las[inten])
			 {
				 return;
			 }
		 }//2
	 }//1
	 last_pi[inten] = ncoor1;
	 if ((jump_coor == 0) && (!thick_las[inten]))
	 {
		 return;
	 }
	 if (jump_coor<2)//change 1 instead of 2
	 {//5
		 if (jump_coor == 1)
		 {
			 hist_fu[inten]++;
			 end_poin[inten] = ncoor1;
		 }
	 }//5
	 else
	 {
		 return;
	 }
	 if (hist_fu[inten] == 1)
	 {
		 beg_poin[inten] = ncoor1 - jump_coor;
		 return;
	 }
	 //if(jump_pare<4)//change 4 instead of 5
	 if (jump_pare<3)//change 3 instead of 5
	 {
		 if (!thick_las[inten])
		 {
			 thick_firs[inten] = save_beg_pare - 1;
			 import_en[inten] = ncoor1;
			 thick_las[inten] = ncoor1;
			 gray_min[inten] = gray_intensity;
			 gray_max[inten] = gray_intensity;
			 gray_mean[inten] = (float)gray_intensity;
			 //if(inten>123)
			 //{
			 opponent1_min[inten] = actual_opponent1;
			 opponent1_max[inten] = actual_opponent1;
			 opponent1_mean[inten] = (float)actual_opponent1;
			 // }
			 thick_stat_inpu[inten] = hist_fu[inten] + hist_su[inten] - 4;

			 memset(dif_int_count1, (int) '\0', sizeof(int)*(DimDifference1));
		 }
		 else
		 {//e
			 interv_length = thick_las[inten] - thick_firs[inten] + 1;
			 mean_val = (int)gray_mean[inten];
			 mean_differ = abs((int)mean_val - gray_intensity);
			 /*if(interv_length<=StripWidth/4)
			 {
			 if((mean_differ>12)&&(gray_intensity>mean_val))
			 {
			 gray_min[inten]+=1;
			 gray_mean[inten]+=1;
			 }
			 }*/
			 max_bound = gray_max[inten];//last_cor05.02.18
			 min_bound = gray_min[inten];
			 min_shift = std::min(abs(max_bound - mean_val), abs(mean_val - min_bound));
			 max_bound_cor = mean_val + min_shift;
			 min_bound_cor = mean_val - min_shift;


			 sign_calc = 0;
			 if (interv_length >= StripWidth / 2)
				 //if((interv_length>=StripWidth/2)||((interv_length>StripWidth/3)&&(mean_differ>12)))
			 {
				 if (((gray_intensity>max_bound_cor) || (gray_intensity<min_bound_cor)) &&
					 (mean_differ >= gray_jump_threshold))
				 {
					 sign_calc = 1;
				 }
			 }

			 if (!sign_calc)
			 {//sc
				 gray_min[inten] = std::min(min_bound, gray_intensity);
				 gray_max[inten] = std::max(max_bound, gray_intensity);
				 number_mean = hist_su[inten] + hist_fu[inten];
				 Aver = ((float)((number_mean - 1)*gray_mean[inten] + gray_intensity)) /
					 ((float)(number_mean));
				 gray_mean[inten] = Aver;
				 if (inten>123)
				 {
					 opponent1_min[inten] = std::min(opponent1_min[inten], actual_opponent1);
					 opponent1_max[inten] = std::max(opponent1_max[inten], actual_opponent1);
					 Aver1 = ((float)((number_mean - 1)*opponent1_mean[inten] + actual_opponent1)) /
						 ((float)(number_mean));
					 opponent1_mean[inten] = Aver1;
				 }

				 color_inten = color_balance - DimDifference + 1;
				 color_inten1 = abs(color_inten);
				 color_inten1 = std::min(31, color_inten1);
				 color_inten1 = opponent_color_difference[color_inten1];
				 color_inten2 = color_inten1 + (DimDifference2);

				 if (color_inten >= 0)
				 {
					 (*(dif_int_count1 + color_inten1))++;
				 }
				 else
				 {
					 (*(dif_int_count1 + color_inten2))++;
				 }
				 import_en[inten] = ncoor1;
				 thick_las[inten] = ncoor1;
			 }//sc
			 else
			 {
				 if (jump_pare == 0)
				 {//jp0
					 hist_su[inten]--;
				 }//jp0
			 }
			 jump_pare1 = end_poin[inten] - thick_las[inten];
			 if (jump_pare1 >= 2)
			 {
				 goto N;
			 }
		 }//e
		 return;
	 }//jp<=4
 N:;
	 if (thick_las[inten])
	 {

		 if (last_en[inten])
		 {

			 hole = thick_firs[inten] - last_en[inten] - 1;
			 if (hole>jump_le[inten])
			 {
				 jump_le[inten] = hole;
				 jump_en[inten] = thick_firs[inten] - 1;
			 }
		 }
		 intens_signif = hist_fu[inten] + hist_su[inten] - thick_stat_inpu[inten];
		 intensity_cons = ((intens_signif) << 1) /
			 (thick_las[inten] - thick_firs[inten] + 1);
		 if (intensity_cons>0)
		 {
			 Strip_value_painting(inten, thick_firs[inten], thick_las[inten],
				 intensity_cons, value_int, bright_cons);
			 Important_interval(thick_firs[inten], thick_las[inten], gray_min[inten],
				 gray_max[inten], gray_mean[inten], opponent1_min[inten], opponent1_max[inten], opponent1_mean[inten],
				 inten, intens_signif, thick_be, thick_en, num_of_in, thick_sta, hist_fu, hist_su, thick_stat_inpu,
				 thick_prev_be, thick_prev_en, thick_prev_sta, thick_break_be, thick_break_en,
				 thick_break_sta, Coun);
			 if (Coun == 0)
			 {
				 IntAllInform[inten].ColorPeak[num_of_in[inten] - 1] = -1;
			 }


			 boundary1 = 7;
			 boundary2 = 7 + DimDifference2;
			 OpponentIntensityFinding(dif_int_count1, DimDifference2, boundary1, boundary2,
				 inten, intens_signif, Coun, 1);
			 //memset(dif_int_count1,(int) '\0',sizeof(int)*(DimDifference1));


			 last_en[inten] = thick_las[inten];
		 }
		 if (num_of_in[inten] == 1)
		 {
			 import_be[inten] = thick_firs[inten];
		 }
		 thick_las[inten] = 0;
	 }
	 /*ncoor1=coor1;

	 if(hist_su[inten]==0)
	 {
	 first_pi[inten]=ncoor1;
	 last_pi[inten]=ncoor1;
	 hist_su[inten]++;
	 return;
	 }
	 jump_coor=ncoor1-last_pi[inten];
	 save_beg_pare=end_poin[inten];
	 if(save_beg_pare)
	 {//1

	 jump_pare=ncoor1-save_beg_pare;
	 if(jump_pare==0)
	 {//2
	 hist_su[inten]++;
	 if(thick_las[inten])
	 {//3
	 interv_length=thick_las[inten]-thick_firs[inten]+1;
	 max_bound=gray_max[inten];//last_cor05.02.18
	 min_bound=gray_min[inten];
	 mean_val=(int)gray_mean[inten];
	 mean_differ=abs((int)mean_val-gray_intensity);
	 sign_calc=0;
	 if(interv_length>StripWidth/2)
	 {
	 if(((gray_intensity>max_bound)||(gray_intensity<min_bound))&&(mean_differ>=8))
	 {
	 sign_calc=1;
	 }
	 }
	 if(!sign_calc)
	 {
	 gray_min[inten]=std::min(min_bound,gray_intensity);
	 gray_max[inten]=std::max(max_bound,gray_intensity);
	 number_mean=hist_su[inten]+hist_fu[inten];
	 Aver=((float)((number_mean-1)*gray_mean[inten]+gray_intensity))/
	 ((float)(number_mean));
	 gray_mean[inten]=Aver;

	 if(inten>123)
	 {//4
	 opponent1_min[inten]=std::min(opponent1_min[inten],actual_opponent1);
	 opponent1_max[inten]=std::max(opponent1_max[inten],actual_opponent1);
	 Aver1=((float)((number_mean-1)*opponent1_mean[inten]+actual_opponent1))/
	 ((float)(number_mean));
	 opponent1_mean[inten]=Aver1;
	 }//4


	 color_inten=color_balance-DimDifference+1;
	 color_inten1=abs(color_inten);
	 color_inten1=std::min(31,color_inten1);
	 color_inten1=opponent_color_difference[color_inten1];
	 color_inten2=color_inten1+(DimDifference2);

	 if(color_inten>=0)
	 {
	 (*(dif_int_count1+color_inten1))++;
	 }
	 else
	 {
	 (*(dif_int_count1+color_inten2))++;
	 }
	 }//sc
	 }//3
	 return;
	 }//2
	 }//1
	 last_pi[inten]=ncoor1;
	 if(jump_coor==0)
	 {
	 return;
	 }
	 if(jump_coor<2)//change 1 instead of 2
	 {//jc<2

	 hist_fu[inten]++;
	 end_poin[inten]=ncoor1;
	 if(thick_las[inten])
	 {
	 interv_length=thick_las[inten]-thick_firs[inten]+1;
	 max_bound=gray_max[inten];//last_cor05.02.18
	 min_bound=gray_min[inten];
	 mean_val=(int)gray_mean[inten];
	 mean_differ=abs((int)mean_val-gray_intensity);
	 sign_calc=0;
	 if(interv_length>StripWidth/2)
	 {
	 if(((gray_intensity>max_bound)||(gray_intensity<min_bound))&&(mean_differ>=8))
	 {
	 sign_calc=1;
	 }
	 }
	 if(!sign_calc)
	 {
	 gray_min[inten]=std::min(min_bound,gray_intensity);
	 gray_max[inten]=std::max(max_bound,gray_intensity);
	 number_mean=hist_su[inten]+hist_fu[inten];
	 Aver=((float)((number_mean-1)*gray_mean[inten]+gray_intensity))/
	 ((float)(number_mean));
	 gray_mean[inten]=Aver;
	 if(inten>123)
	 {
	 opponent1_min[inten]=std::min(opponent1_min[inten],actual_opponent1);
	 opponent1_max[inten]=std::max(opponent1_max[inten],actual_opponent1);
	 Aver1=((float)((number_mean-1)*opponent1_mean[inten]+actual_opponent1))/
	 ((float)(number_mean));
	 opponent1_mean[inten]=Aver1;
	 }

	 color_inten=color_balance-DimDifference+1;
	 color_inten1=abs(color_inten);
	 color_inten1=std::min(31,color_inten1);
	 color_inten1=opponent_color_difference[color_inten1];
	 color_inten2=color_inten1+(DimDifference2);

	 if(color_inten>=0)
	 {
	 (*(dif_int_count1+color_inten1))++;
	 }
	 else
	 {
	 (*(dif_int_count1+color_inten2))++;
	 }
	 import_en[inten]=ncoor1;
	 thick_las[inten]=ncoor1;
	 }//sc
	 }
	 }//jc<2
	 else
	 {
	 return;
	 }
	 if(hist_fu[inten]==1)
	 {
	 beg_poin[inten]=ncoor1-jump_coor;
	 return;
	 }
	 if(jump_pare<4)//change 4 instead of 5
	 {
	 if(!thick_las[inten])
	 {
	 thick_firs[inten]=save_beg_pare-1;
	 gray_min[inten]=gray_intensity;
	 gray_max[inten]=gray_intensity;
	 gray_mean[inten]=(float)gray_intensity;
	 //if(inten>123)
	 //{
	 opponent1_min[inten]=actual_opponent1;
	 opponent1_max[inten]=actual_opponent1;
	 opponent1_mean[inten]=(float)actual_opponent1;
	 // }
	 import_en[inten]=ncoor1;
	 thick_las[inten]=ncoor1;
	 thick_stat_inpu[inten]=hist_fu[inten]+hist_su[inten]-4;

	 memset(dif_int_count1,(int) '\0',sizeof(int)*(DimDifference1));
	 }
	 else
	 {
	 if(jump_pare>1)
	 {//jp>1
	 interv_length=thick_las[inten]-thick_firs[inten]+1;
	 max_bound=gray_max[inten];//last_cor05.02.18
	 min_bound=gray_min[inten];
	 mean_val=(int)gray_mean[inten];
	 mean_differ=abs((int)mean_val-gray_intensity);
	 sign_calc=0;
	 if(interv_length>StripWidth/2)
	 {
	 if(((gray_intensity>max_bound)||(gray_intensity<min_bound))&&(mean_differ>=8))
	 {
	 sign_calc=1;
	 }
	 }
	 if(!sign_calc)
	 {//sc
	 gray_min[inten]=std::min(gray_min[inten],gray_intensity);
	 gray_max[inten]=std::max(max_bound,gray_intensity);
	 number_mean=hist_su[inten]+hist_fu[inten];
	 Aver=((float)((number_mean-1)*gray_mean[inten]+gray_intensity))/
	 ((float)(number_mean));
	 gray_mean[inten]=Aver;
	 if(inten>123)
	 {
	 opponent1_min[inten]=std::min(opponent1_min[inten],actual_opponent1);
	 opponent1_max[inten]=std::max(opponent1_max[inten],actual_opponent1);
	 Aver1=((float)((number_mean-1)*opponent1_mean[inten]+actual_opponent1))/
	 ((float)(number_mean));
	 opponent1_mean[inten]=Aver1;
	 }
	 import_en[inten]=ncoor1;
	 thick_las[inten]=ncoor1;
	 }//sc
	 }//jp>1
	 }
	 //import_en[inten]=ncoor1;
	 //thick_las[inten]=ncoor1;

	 return;
	 }
	 if(thick_las[inten])
	 {

	 if(last_en[inten])
	 {

	 hole=thick_firs[inten]-last_en[inten]-1;
	 if(hole>jump_le[inten])
	 {
	 jump_le[inten]=hole;
	 jump_en[inten]=thick_firs[inten]-1;
	 }
	 }
	 intens_signif=hist_fu[inten]+hist_su[inten]-thick_stat_inpu[inten];
	 intensity_cons=((intens_signif)<<1)/
	 (thick_las[inten]-thick_firs[inten]+1);
	 if(intensity_cons>0)
	 {
	 Strip_value_painting(inten,thick_firs[inten],thick_las[inten],
	 intensity_cons,value_int,bright_cons);
	 Important_interval(thick_firs[inten],thick_las[inten],gray_min[inten],
	 gray_max[inten],gray_mean[inten],opponent1_min[inten],opponent1_max[inten],opponent1_mean[inten],
	 inten,intens_signif,thick_be,thick_en,num_of_in,thick_sta,hist_fu,hist_su,thick_stat_inpu,
	 thick_prev_be,thick_prev_en,thick_prev_sta,thick_break_be,thick_break_en,
	 thick_break_sta,Coun);
	 if(Coun==0)
	 {
	 IntAllInform[inten].ColorPeak[num_of_in[inten]-1]=-1;
	 }


	 boundary1=7;
	 boundary2=7+DimDifference2;
	 OpponentIntensityFinding(dif_int_count1,DimDifference2,boundary1,boundary2,
	 inten,intens_signif,Coun,1);
	 //memset(dif_int_count1,(int) '\0',sizeof(int)*(DimDifference1));


	 last_en[inten]=thick_las[inten];
	 }
	 if(num_of_in[inten]==1)
	 {
	 import_be[inten]=thick_firs[inten];
	 }
	 thick_las[inten]=0;
	 }*/
 }
//=====================================================
//An auxiliary program determining chief intensities, input parameters
//determine an intensity and its interval
void CStrip::Strip_value_painting(unsigned char intens, int beg_int,
		int end_int, int intens_consist, unsigned char* valuable_inten,
		int* bright_consist) {
	int paint_coun;
	int current_inhabitant;
	int paint_start, paint_finish;
	int val;
	int point_location;
	int quant;
	int limit;

	limit = NUM_INTEN / 4;
	paint_start = beg_int >> PRESSING;
	paint_finish = end_int >> PRESSING;
	for (paint_coun = paint_start; paint_coun <= paint_finish; paint_coun++) {
		current_inhabitant = *(valuable_inten + paint_coun);
		quant = *(quantity_of_intensities + paint_coun);
		point_location = quant * PressedLength + paint_coun;
		if (quant < limit) {
			*(intensities_occurred + point_location) = intens;
			*(interval_occurred + point_location) = num_of_int[intens];
			(*(quantity_of_intensities + paint_coun))++;
		}
		if (!current_inhabitant) {
			*(valuable_inten + paint_coun) = intens + 1;
			*(valuable_interval + paint_coun) = *(num_of_int + intens);
			*(bright_consist + paint_coun) = intens_consist;
		} else {
			val = *(bright_consist + paint_coun);
			if (val < intens_consist) {
				*(valuable_inten + paint_coun) = intens + 1;
				*(bright_consist + paint_coun) = intens_consist;
				*(valuable_interval + paint_coun) = *(num_of_int + intens);
			}
		}

	}

}
//=====================================================
//progral selects chief interval for particular intensity, input parameters
//define intensity, its significance and its interval
void CStrip::Important_interval(int begin_interval, int end_interval,
		int min_val, int max_val, float mean_val, int min_opponent1,
		int max_opponent1, float mean_opponent1, unsigned char intensi,
		int signi, int* thick_be, int* thick_en, int* num_of_in,
		int* thick_sta, int* hist_fu, int* hist_su, int* thick_stat_inpu,
		int* thick_prev_be, int* thick_prev_en, int* thick_prev_sta,
		int* thick_break_be, int* thick_break_en, int* thick_break_sta,
		int num_comp)

/*void
 TStrip::Important_interval(int begin_interval,int end_interval,
 unsigned char intensi,int signi,int* thick_be,int* thick_en,int* num_of_in,
 int* thick_sta,int* hist_fu,int* hist_su,int* thick_stat_inpu,
 int* thick_prev_be,int* thick_prev_en,int* thick_prev_sta,
 int* thick_break_be,int* thick_break_en,int* thick_break_sta,
 int num_comp) */
{
	int subtr, prev_length, subtpr;
	int prev_beg, prev_end;
	int int_mean;

	subtr = end_interval - begin_interval;
	prev_beg = thick_be[intensi];
	prev_end = thick_en[intensi];
	num_of_in[intensi]++;
	prev_length = prev_end - prev_beg;
	if (num_comp == 0) {
		IntAllInform[intensi].num_of_int = num_of_in[intensi];
		IntAllInform[intensi].MainOpponentRatiosBalance[num_of_in[intensi] - 1]
				= -1;
		IntAllInform[intensi].BegInt[num_of_in[intensi] - 1] = begin_interval;
		IntAllInform[intensi].Signif[num_of_in[intensi] - 1] = signi;
		IntAllInform[intensi].EndInt[num_of_in[intensi] - 1] = end_interval;
		IntAllInform[intensi].LowerGrayScale[num_of_in[intensi] - 1] = min_val;
		IntAllInform[intensi].UpperGrayScale[num_of_in[intensi] - 1] = max_val;
		IntAllInform[intensi].GrayScaleMean[num_of_in[intensi] - 1]
				= (int) mean_val;
		IntAllInform[intensi].LowerMRatio[num_of_in[intensi] - 1]
				= min_opponent1;
		IntAllInform[intensi].UpperMRatio[num_of_in[intensi] - 1]
				= max_opponent1;
		int_mean = (int) mean_opponent1;
		if (mean_opponent1 - int_mean > 0.5) {
			int_mean += 1;
		}
		IntAllInform[intensi].MeanMRatio[num_of_in[intensi] - 1]
				= (int) int_mean;
	}

	if (!prev_end) {
		thick_be[intensi] = begin_interval;
		thick_en[intensi] = end_interval;
		thick_sta[intensi] = hist_fu[intensi] + hist_su[intensi];
		return;
	}
	if (subtr > prev_length) {
		thick_be[intensi] = begin_interval;
		thick_en[intensi] = end_interval;
		thick_sta[intensi] = hist_fu[intensi] + hist_su[intensi]
				- thick_stat_inpu[intensi];
		if ((!thick_break_en[intensi]) && (thick_prev_en[intensi])) {
			thick_break_be[intensi] = thick_prev_be[intensi];
			thick_break_en[intensi] = thick_prev_en[intensi];
			thick_break_sta[intensi] = thick_prev_sta[intensi];
		}
		thick_prev_be[intensi] = prev_beg;
		thick_prev_en[intensi] = prev_end;
		thick_prev_sta[intensi] = thick_sta[intensi];
	} else {
		if (thick_break_en[intensi]) {
			subtpr = thick_break_en[intensi] - thick_break_be[intensi];
		} else {
			subtpr = 2048;
			thick_break_be[intensi] = begin_interval;
			thick_break_en[intensi] = end_interval;
			thick_break_sta[intensi] = hist_fu[intensi] + hist_su[intensi]
					- thick_stat_inpu[intensi];
			goto ahead;
		}
		if (subtr > subtpr) {
			if (!thick_prev_en[intensi]) {
				thick_prev_be[intensi] = thick_break_be[intensi];
				thick_prev_en[intensi] = thick_break_en[intensi];
				thick_prev_sta[intensi] = thick_break_sta[intensi];
			}
			thick_break_be[intensi] = begin_interval;
			thick_break_en[intensi] = end_interval;
			thick_break_sta[intensi] = hist_fu[intensi] + hist_su[intensi]
					- thick_stat_inpu[intensi];
		} else if (!thick_prev_en[intensi]) {
			thick_prev_be[intensi] = begin_interval;
			thick_prev_en[intensi] = end_interval;
			thick_prev_sta[intensi] = hist_fu[intensi] + hist_su[intensi]
					- thick_stat_inpu[intensi];
		}
	}
	ahead: ;
}
//=====================================================
void CStrip::FinalCorrection(int* thick_las, int* gray_mi, int* gray_ma,
		float* gray_mea, int *mratio_min, int *mratio_max, float *mratio_mean,
		int* thick_be, int* thick_en, int* import_be, int* import_en,
		int* thick_break_be, int* thick_break_en, int* thick_prev_be,
		int* thick_prev_en, int* thick_firs, int* num_of_in, int* hist_fu,
		int* hist_su, int* thick_sta, int* thick_stat_inpu, int* last_en,
		int* jump_le, int* jump_en, int* thick_break_sta, int* thick_prev_sta,
		unsigned char* value_int, int* bright_cons, int* diff_color, int Coun) {
	int intensity_cons;
	int end, beg, break_beg, break_end, prev_beg, prev_end;
	int first, last;
	int hole;
	int subtr1;
	int* dif_int_count;
	int boundary1;
	int boundary2;
	int intens_signif;

	for (int inu = 0; inu < NUM_INTEN; inu++) {
		if (!thick_las[inu]) {
			if (!thick_en[inu]) {
				import_be[inu] = 0;
				import_en[inu] = 0;
			}
			goto pus;
		}
		if (num_of_in[inu] > MAX_INT_NUMBER - 2) {
			goto pus;
		}
		end = thick_en[inu];
		beg = thick_be[inu];
		break_beg = thick_break_be[inu];
		break_end = thick_break_en[inu];
		prev_beg = thick_prev_be[inu];
		prev_end = thick_prev_en[inu];
		first = thick_firs[inu];
		last = thick_las[inu];

		if (num_of_in[inu] == 0) {
			import_be[inu] = first;
			import_en[inu] = last;
			thick_be[inu] = first;
			thick_en[inu] = last;
			thick_sta[inu] = hist_fu[inu] + hist_su[inu];
			intensity_cons = ((thick_sta[inu]) << 1) / (last - first + 1);
			if (intensity_cons > 0) {
				if (Coun == 0) {
					IntAllInform[inu].num_of_int = num_of_in[inu] + 1;
					IntAllInform[inu].BegInt[num_of_in[inu]] = first;
					IntAllInform[inu].Signif[num_of_in[inu]] = thick_sta[inu];
					IntAllInform[inu].EndInt[num_of_in[inu]] = last;
					IntAllInform[inu].LowerGrayScale[num_of_in[inu]]
							= gray_mi[inu];
					IntAllInform[inu].UpperGrayScale[num_of_in[inu]]
							= gray_ma[inu];
					IntAllInform[inu].GrayScaleMean[num_of_in[inu]]
							= (int) gray_mea[inu];
					IntAllInform[inu].LowerMRatio[num_of_in[inu]]
							= mratio_min[inu];
					IntAllInform[inu].UpperMRatio[num_of_in[inu]]
							= mratio_max[inu];
					IntAllInform[inu].MeanMRatio[num_of_in[inu]]
							= (int) mratio_mean[inu];
				}

				intens_signif = hist_fu[inu] + hist_su[inu]
						- thick_stat_inpu[inu];

				boundary1 = 7;
				boundary2 = 7 + DimDifference2;
				dif_int_count = diff_color + inu * (DimDifference1);

				Strip_value_painting(inu, first, last, intensity_cons,
						value_int, bright_cons);
				num_of_in[inu]++;
				OpponentIntensityFinding(dif_int_count, DimDifference2,
						boundary1, boundary2, inu, intens_signif, Coun, 1);
			}
			goto pus;
		}
		intensity_cons = ((hist_fu[inu] + hist_su[inu] - thick_stat_inpu[inu])
				<< 1) / (last - first + 1);

		if (intensity_cons > 0) {
			if (Coun == 0) {
				IntAllInform[inu].num_of_int = num_of_in[inu] + 1;
				IntAllInform[inu].BegInt[num_of_in[inu]] = first;
				IntAllInform[inu].Signif[num_of_in[inu]] = hist_fu[inu]
						+ hist_su[inu] - thick_stat_inpu[inu];
				IntAllInform[inu].EndInt[num_of_in[inu]] = last;
				IntAllInform[inu].LowerGrayScale[num_of_in[inu]] = gray_mi[inu];
				IntAllInform[inu].UpperGrayScale[num_of_in[inu]] = gray_ma[inu];
				IntAllInform[inu].GrayScaleMean[num_of_in[inu]]
						= (int) gray_mea[inu];
				IntAllInform[inu].LowerMRatio[num_of_in[inu]] = mratio_min[inu];
				IntAllInform[inu].UpperMRatio[num_of_in[inu]] = mratio_max[inu];
				IntAllInform[inu].MeanMRatio[num_of_in[inu]]
						= (int) mratio_mean[inu];
			}

			intens_signif = hist_fu[inu] + hist_su[inu] - thick_stat_inpu[inu];

			boundary1 = 7;
			boundary2 = 7 + DimDifference2;
			dif_int_count = diff_color + inu * (DimDifference1);

			Strip_value_painting(inu, first, last, intensity_cons, value_int,
					bright_cons);
			num_of_in[inu]++;
			OpponentIntensityFinding(dif_int_count, DimDifference2, boundary1,
					boundary2, inu, intens_signif, Coun, 1);
		}

		else {

			goto pus;
		}
		if (last_en[inu]) {

			hole = thick_firs[inu] - last_en[inu] - 1;
			if (hole > jump_le[inu]) {
				jump_le[inu] = hole;
				jump_en[inu] = first - 1;
			}
		}
		import_en[inu] = last;
		if ((end - beg) < (last - first)) {

			if (prev_end) {
				if (!break_end) {
					thick_break_be[inu] = prev_beg;
					thick_break_en[inu] = prev_end;
					thick_break_sta[inu] = thick_prev_sta[inu];
				} else {
					if ((prev_end - prev_beg) > (break_end - break_beg)) {
						thick_break_be[inu] = prev_beg;
						thick_break_en[inu] = prev_end;
						thick_break_sta[inu] = thick_prev_sta[inu];
					}
				}
			}
			thick_prev_be[inu] = beg;
			thick_prev_en[inu] = end;
			thick_prev_sta[inu] = thick_sta[inu];
			thick_be[inu] = first;
			thick_en[inu] = last;
			thick_sta[inu] = hist_fu[inu] + hist_su[inu] - thick_stat_inpu[inu];

		} else {
			if (!break_end) {
				thick_break_be[inu] = first;
				thick_break_en[inu] = last;
				thick_break_sta[inu] = hist_fu[inu] + hist_su[inu]
						- thick_stat_inpu[inu];
			} else {
				subtr1 = break_end - break_beg;
				if ((subtr1) < (last - first)) {
					if (!prev_end) {
						thick_prev_be[inu] = break_beg;
						thick_prev_en[inu] = break_end;
						thick_prev_sta[inu] = thick_break_sta[inu];
					} else {
						if ((prev_end - prev_beg) < (last - first)) {
							thick_prev_be[inu] = break_beg;
							thick_prev_en[inu] = break_end;
							thick_prev_sta[inu] = thick_break_sta[inu];
						}
					}
					thick_break_be[inu] = first;
					thick_break_en[inu] = last;
					thick_break_sta[inu] = hist_fu[inu] + hist_su[inu]
							- thick_stat_inpu[inu];
				} else {
					if (!prev_end) {
						thick_prev_be[inu] = first;
						thick_prev_en[inu] = last;
						thick_prev_sta[inu] = hist_fu[inu] + hist_su[inu]
								- thick_stat_inpu[inu];
					} else {
						if ((prev_end - prev_beg) < (last - first)) {
							thick_prev_be[inu] = first;
							thick_prev_en[inu] = last;
							thick_prev_sta[inu] = hist_fu[inu] + hist_su[inu]
									- thick_stat_inpu[inu];
						}
					}
				}
			}
		}

		pus: IntAllInform[inu].num_of_int = num_of_in[inu];
	}
}
//=====================================================
void CStrip::OpponentIntensityFinding(int* inten_opp_count, int start1,
		int bound1, int bound2, unsigned char inten_first, int signif,
		int coun, int num_opp) {
	int coun_int;
	int coun_int1;
	int optimal_inten;
	int optimal_inten_value;
	int nonneg_optimal_inten;
	int nonneg_optimal_inten_value;
	int current_value;
	int maximal_nonzero;
	int maximal_nonzero_value;
	int previous_maximum;
	int previous_maximum_value;
	int abs_optimal_inten;
	int abs_optimal_inten_value;
	int optimal_inten1;
	int optimal_inten_value1;
	int current_value1;
	int maximal_nonzero1;
	int maximal_nonzero_value1;
	int previous_maximum1;
	int previous_maximum_value1;
	int sum;
	int sum_zero;
	int sum_positive;
	int sum_positive1;
	int sum_positive2;
	int sum1;
	int sum_zero1;
	int sum_zero2;
	int remaining;
	int new_signif;
	/* int color_lead;
	 int color_second; */
	int first_second_ratio;
	int zero_fraction;
	int zero_fraction1;
	int positive_fraction;
	int optimum_fraction;
	int abs_optimum_fraction;
	int nonneg_optimum_fraction;
	int positive_fraction1;
	int positive_fraction2;
	int optimum_fraction1;
	int left_outlet;
	int right_outlet;
	int left_outlet1;
	int right_outlet1;
	int pointer_opponent_pair;
	int absolute_total_deviation;
	int comparison_positive_zero;
	int sum_fraction;
	int sum_fraction1;
	int comparison_positive_zero1;
	int sum_optimal_neighbor;
	int first_neighbor;
	int last_neighbor;
	int neighbor_count;
	int neighbor_fraction;
	int sum_optimal_neighbor1;
	int first_neighbor1;
	int last_neighbor1;
	int neighbor_count1;
	int neighbor_fraction1;
	int num_int;
	int Center_of_mass;
	int Center_of_mass1;
	int current_neighbor;
	int current_neighbor1;
	float center;

	remaining = signif / 16;
	/* color_lead=-100;
	 color_second=-100;*/

	/*if((num_opp==0)||((int)inten_first<59))
	 {
	 return;
	 }*/
	optimal_inten_value = 0;
	optimal_inten = -1;
	nonneg_optimal_inten = -1;
	abs_optimal_inten = -1;
	maximal_nonzero = -1;
	previous_maximum = -1;
	sum_positive = 0;
	sum_zero = inten_opp_count[0];
	sum_zero1 = inten_opp_count[start1 + 1];
	sum_zero2 = sum_zero + sum_zero1 + inten_opp_count[1];
	pointer_opponent_pair = 8;
	sum_optimal_neighbor = 0;
	sum_optimal_neighbor1 = 0;
	Center_of_mass = 0;
	Center_of_mass1 = 0;

	num_int = num_of_int[inten_first] - 1;

	for (coun_int = 1; coun_int <= bound1; coun_int++) {
		current_value = *(inten_opp_count + coun_int);
		sum_positive += current_value;
		if (current_value > optimal_inten_value) {
			if (8 * optimal_inten_value > current_value) {
				previous_maximum_value = optimal_inten_value;
				previous_maximum = optimal_inten;
			}
			optimal_inten_value = current_value;
			optimal_inten = coun_int;
		} else {
			if (8 * current_value > optimal_inten_value) {
				maximal_nonzero_value = current_value;
				maximal_nonzero = coun_int;
			}
		}
		if ((!(signif - sum_positive)) || ((!current_value) && ((signif
				- sum_positive) < remaining))) {
			break;
		}
	}
	sum = sum_positive + sum_zero;
	sum_positive2 = sum_positive - inten_opp_count[1];
	if (sum_positive > 0) {
		if (previous_maximum > 0) {
			first_neighbor = previous_maximum;
		} else {
			first_neighbor = optimal_inten;
		}
		if ((maximal_nonzero > 0) && (maximal_nonzero > optimal_inten)) {
			last_neighbor = maximal_nonzero;
		} else {
			maximal_nonzero = -1;
			last_neighbor = optimal_inten;
		}

		for (neighbor_count = first_neighbor; neighbor_count <= last_neighbor; neighbor_count++) {
			current_neighbor = *(inten_opp_count + neighbor_count);
			sum_optimal_neighbor += current_neighbor;
			Center_of_mass += neighbor_count * current_neighbor;
		}
		if (sum_optimal_neighbor) {
			center = ((float) Center_of_mass) / (float) sum_optimal_neighbor;
			Center_of_mass = (int) (16 * center);
		}
	}

	optimal_inten_value1 = 0;
	optimal_inten1 = -1;
	maximal_nonzero1 = -1;
	previous_maximum1 = -1;
	sum1 = 0;
	new_signif = signif - sum;
	for (coun_int1 = start1 + 1; coun_int1 <= bound2; coun_int1++) {
		current_value1 = *(inten_opp_count + coun_int1);
		sum1 += current_value1;
		if (current_value1 > optimal_inten_value1) {
			if (8 * optimal_inten_value1 > current_value1) {
				previous_maximum_value1 = optimal_inten_value1;
				previous_maximum1 = optimal_inten1;
			}
			optimal_inten_value1 = current_value1;
			optimal_inten1 = coun_int1 - start1;
		} else {
			if (8 * current_value1 > optimal_inten_value1) {
				maximal_nonzero_value1 = current_value1;
				maximal_nonzero1 = coun_int1 - start1;
			}
		}
		if ((!current_value1) && ((new_signif - sum1) < remaining)) {
			break;
		}
	}
	sum_positive1 = sum1 - sum_zero1;
	if (sum1) {
		if (optimal_inten1) {
			if (previous_maximum1 > 0) {
				first_neighbor1 = previous_maximum1;
			} else {
				first_neighbor1 = optimal_inten1;
			}
			if ((maximal_nonzero1 > 0) && (maximal_nonzero1) > optimal_inten1) {
				last_neighbor1 = maximal_nonzero1;
			} else {
				maximal_nonzero1 = -1;
				last_neighbor1 = optimal_inten1;
			}
			for (neighbor_count1 = first_neighbor1; neighbor_count1
					<= last_neighbor1; neighbor_count1++) {
				current_neighbor1 = *(inten_opp_count + start1
						+ neighbor_count1);
				sum_optimal_neighbor1 += current_neighbor1;
				Center_of_mass1 += neighbor_count1 * current_neighbor1;
			}
			if (sum_optimal_neighbor1) {
				center = ((float) Center_of_mass1)
						/ (float) sum_optimal_neighbor1;
				Center_of_mass1 = (int) (16 * center);
			}
		}
	}
	if (sum + sum1) {
		if (num_opp == 1) {
			if (optimal_inten >= 0) {
				optimum_fraction = (16 * optimal_inten_value) / (sum + sum1);
				neighbor_fraction = (16 * sum_optimal_neighbor) / (sum + sum1);
			} else {
				optimum_fraction = 0;
				neighbor_fraction = 0;
			}
			zero_fraction = (16 * sum_zero) / (sum + sum1);
			zero_fraction1 = (16 * sum_zero2) / (sum + sum1);
			first_second_ratio = (16 * sum1) / (sum + sum1);
			positive_fraction = (16 * sum_positive) / (sum + sum1);
			positive_fraction1 = (16 * sum_positive1) / (sum + sum1);
			positive_fraction2 = (16 * sum_positive2) / (sum + sum1);
			absolute_total_deviation = positive_fraction + first_second_ratio;
			sum_fraction = zero_fraction + positive_fraction;
			sum_fraction1 = zero_fraction + sum1;
			if (optimal_inten1 >= 0) {
				optimum_fraction1 = (16 * optimal_inten_value1) / (sum + sum1);
				neighbor_fraction1 = (16 * sum_optimal_neighbor1)
						/ (sum + sum1);
			} else {
				optimum_fraction1 = 0;
				neighbor_fraction1 = 0;
			}
			if (sum_fraction) {
				comparison_positive_zero = (16 * positive_fraction)
						/ sum_fraction;
			}
			if (sum_fraction1) {
				comparison_positive_zero1 = (16 * sum1) / sum_fraction1;
			}

			if (absolute_total_deviation) {
				pointer_opponent_pair = (16 * positive_fraction)
						/ absolute_total_deviation;
			}
			if (sum_zero > optimal_inten_value) {
				nonneg_optimal_inten_value = sum_zero;
				nonneg_optimal_inten = 0;
				nonneg_optimum_fraction = zero_fraction;
			} else {
				nonneg_optimal_inten_value = optimal_inten_value;
				nonneg_optimal_inten = optimal_inten;
				nonneg_optimum_fraction = optimum_fraction;
			}
			if (nonneg_optimal_inten_value >= optimal_inten_value1) {
				abs_optimal_inten_value = nonneg_optimal_inten_value;
				abs_optimal_inten = nonneg_optimal_inten;
				abs_optimum_fraction = nonneg_optimum_fraction;
			} else {
				abs_optimal_inten_value = optimal_inten_value1;
				abs_optimal_inten = optimal_inten1;
				abs_optimum_fraction = optimum_fraction1;
			}

			/*	if(abs_optimal_inten==0)
			 {
			 if((!positive_fraction)&&(!first_second_ratio))
			 {
			 color_lead=0;
			 goto M;
			 }
			 if((!positive_fraction2)&&(!positive_fraction1))
			 {
			 color_lead=1;
			 goto M;
			 }
			 else
			 {
			 color_lead=2;
			 goto M;
			 }
			 }
			 else
			 {
			 color_lead=3;
			 }
			 L:
			 ; */
			if (previous_maximum > -1) {
				left_outlet = previous_maximum;
			} else {
				left_outlet = -1;
			}
			if (maximal_nonzero > -1) {
				right_outlet = maximal_nonzero;
			} else {
				right_outlet = -1;
			}
			if (previous_maximum1 > -1) {
				left_outlet1 = previous_maximum1;
			} else {
				left_outlet1 = -1;
			}
			if (maximal_nonzero1 > -1) {
				right_outlet1 = maximal_nonzero1;
			} else {
				right_outlet1 = -1;
			}

			if (coun == 0) {
				IntAllInform[inten_first].ZeroRatio[num_int] = zero_fraction;
				IntAllInform[inten_first].ZeroRatio1[num_int] = zero_fraction1;
				IntAllInform[inten_first].MainOpponentRatiosBalance[num_int]
						= first_second_ratio;
				IntAllInform[inten_first].PositiveFraction[num_int]
						= positive_fraction;
				IntAllInform[inten_first].NegativeFraction[num_int]
						= positive_fraction1;
				IntAllInform[inten_first].ColorPeak[num_int] = optimal_inten;
				IntAllInform[inten_first].PeakFraction[num_int]
						= optimum_fraction;
				IntAllInform[inten_first].FirstOutlet[num_int] = left_outlet;
				IntAllInform[inten_first].SecondOutlet[num_int] = right_outlet;
				IntAllInform[inten_first].ColorPeak1[num_int] = optimal_inten1;
				IntAllInform[inten_first].PeakFraction1[num_int]
						= optimum_fraction1;
				IntAllInform[inten_first].FirstOutlet1[num_int] = left_outlet1;
				IntAllInform[inten_first].SecondOutlet1[num_int]
						= right_outlet1;
				IntAllInform[inten_first].PositiveFraction2[num_int]
						= positive_fraction2;
				IntAllInform[inten_first].NeighborPosFraction[num_int]
						= neighbor_fraction;
				IntAllInform[inten_first].NeighborNegFraction[num_int]
						= neighbor_fraction1;
				IntAllInform[inten_first].CenterOfMass[num_int]
						= Center_of_mass;
				IntAllInform[inten_first].CenterOfMass1[num_int]
						= Center_of_mass1;
			}

		}
	} else {
		if (coun == 0) {
			IntAllInform[inten_first].MainOpponentRatiosBalance[num_int] = -1;
		}

	}
}
//=====================================================
void CStrip::StripCharacteristicsFindingGray(unsigned char inten, int coor1,
		int* first_pi, int* last_pi, int* last_en) {
	int ncoor1;
	int jump_coor, jump_pare, save_beg_pare;
	int intens_signif;
	int hole;
	int intensity_cons;

	//jump_pare=0;
	ncoor1 = coor1;
	if (hist_sumg[inten] == 0) {
		first_pi[inten] = ncoor1;
		last_pi[inten] = ncoor1;
		hist_sumg[inten]++;
		return;
	}
	jump_coor = ncoor1 - last_pi[inten];
	save_beg_pare = end_pointg[inten];
	if (save_beg_pare) {
		jump_pare = ncoor1 - save_beg_pare;
		if (jump_pare == 0) {
			hist_sumg[inten]++;
			return;
		}
	}
	last_pi[inten] = ncoor1;
	if (jump_coor == 0) {
		return;
	}
	if (jump_coor <= 2) {
		hist_fung[inten]++;
		end_pointg[inten] = ncoor1;

	} else {
		return;
	}
	if (hist_fung[inten] == 1) {
		beg_pointg[inten] = ncoor1 - jump_coor;
		return;
	}
	if (jump_pare <= 5) {
		if (!thick_lastg[inten]) {
			thick_firstg[inten] = save_beg_pare - 1;
			thick_stat_inputg[inten] = hist_fung[inten] + hist_sumg[inten] - 4;

		}
		import_endg[inten] = ncoor1;
		thick_lastg[inten] = ncoor1;
		return;
	}
	if (thick_lastg[inten]) {

		if (last_en[inten]) {

			hole = thick_firstg[inten] - last_en[inten] - 1;
			if (hole > jump_leng[inten]) {
				jump_leng[inten] = hole;
				jump_endg[inten] = thick_firstg[inten] - 1;
			}
		}
		intens_signif = hist_fung[inten] + hist_sumg[inten]
				- thick_stat_inputg[inten];
		intensity_cons = ((intens_signif) << 1) / (thick_lastg[inten]
				- thick_firstg[inten] + 1);
		if (intensity_cons > 0) {

			Strip_value_painting1(inten, thick_firstg[inten],
					thick_lastg[inten], intensity_cons);
			Important_interval1(thick_firstg[inten], thick_lastg[inten], inten,
					intens_signif);

			last_en[inten] = thick_lastg[inten];
		}
		if (num_of_intg[inten] == 1) {
			import_begg[inten] = thick_firstg[inten];
		}
		thick_lastg[inten] = 0;
	}
}
//=====================================================
void CStrip::FinalCorrectionGray(int* last_en)

{
	int intensity_cons;
	int end, beg, break_beg, break_end, prev_beg, prev_end;
	int first, last;
	int hole;
	int subtr1;

	for (int inu = 0; inu < NUM_INTEN1; inu++) {
		if (!thick_lastg[inu]) {
			if (!thick_endg[inu]) {
				import_begg[inu] = 0;
				import_endg[inu] = 0;
			}
			goto pus;
		}
		if (num_of_intg[inu] > MAX_INT_NUMBER - 2) {
			goto pus;
		}
		end = thick_endg[inu];
		beg = thick_begg[inu];
		break_beg = thick_break_begg[inu];
		break_end = thick_break_endg[inu];
		prev_beg = thick_prev_begg[inu];
		prev_end = thick_prev_endg[inu];
		first = thick_firstg[inu];
		last = thick_lastg[inu];

		if (num_of_intg[inu] == 0) {
			import_begg[inu] = first;
			import_endg[inu] = last;
			thick_begg[inu] = first;
			thick_endg[inu] = last;
			thick_statg[inu] = hist_fung[inu] + hist_sumg[inu];
			intensity_cons = ((thick_statg[inu]) << 1) / (last - first + 1);
			if (intensity_cons > 0) {

				IntAllInformGray[inu].num_of_int = num_of_intg[inu] + 1;
				IntAllInformGray[inu].BegInt[num_of_intg[inu]] = first;
				IntAllInformGray[inu].Signif[num_of_intg[inu]]
						= thick_statg[inu];
				IntAllInformGray[inu].EndInt[num_of_intg[inu]] = last;

				Strip_value_painting1(inu, first, last, intensity_cons);
				num_of_intg[inu]++;
			}

			goto pus;
		}

		intensity_cons = ((hist_fung[inu] + hist_sumg[inu]
				- thick_stat_inputg[inu]) << 1) / (last - first + 1);

		if (intensity_cons > 0) {
			IntAllInformGray[inu].num_of_int = num_of_intg[inu] + 1;
			IntAllInformGray[inu].BegInt[num_of_intg[inu]] = first;
			IntAllInformGray[inu].Signif[num_of_intg[inu]] = hist_fung[inu]
					+ hist_sumg[inu] - thick_stat_inputg[inu];
			IntAllInformGray[inu].EndInt[num_of_intg[inu]] = last;

			Strip_value_painting1(inu, first, last, intensity_cons);
			num_of_intg[inu]++;
		} else {

			goto pus;
		}
		if (last_en[inu]) {

			hole = thick_firstg[inu] - last_en[inu] - 1;
			if (hole > jump_leng[inu]) {
				jump_leng[inu] = hole;
				jump_endg[inu] = first - 1;
			}
		}
		import_endg[inu] = last;
		if ((end - beg) < (last - first)) {

			if (prev_end) {
				if (!break_end) {
					thick_break_begg[inu] = prev_beg;
					thick_break_endg[inu] = prev_end;
					thick_break_statg[inu] = thick_prev_statg[inu];
				} else {
					if ((prev_end - prev_beg) > (break_end - break_beg)) {
						thick_break_begg[inu] = prev_beg;
						thick_break_endg[inu] = prev_end;
						thick_break_statg[inu] = thick_prev_statg[inu];
					}
				}
			}
			thick_prev_begg[inu] = beg;
			thick_prev_endg[inu] = end;
			thick_prev_statg[inu] = thick_statg[inu];
			thick_begg[inu] = first;
			thick_endg[inu] = last;
			thick_statg[inu] = hist_fung[inu] + hist_sumg[inu]
					- thick_stat_inputg[inu];

		} else {
			if (!break_end) {
				thick_break_begg[inu] = first;
				thick_break_endg[inu] = last;
				thick_break_statg[inu] = hist_fung[inu] + hist_sumg[inu]
						- thick_stat_inputg[inu];
			} else {
				subtr1 = break_end - break_beg;
				if ((subtr1) < (last - first)) {
					if (!prev_end) {
						thick_prev_begg[inu] = break_beg;
						thick_prev_endg[inu] = break_end;
						thick_prev_statg[inu] = thick_break_statg[inu];
					} else {
						if ((prev_end - prev_beg) < (last - first)) {
							thick_prev_begg[inu] = break_beg;
							thick_prev_endg[inu] = break_end;
							thick_prev_statg[inu] = thick_break_statg[inu];
						}
					}
					thick_break_begg[inu] = first;
					thick_break_endg[inu] = last;
					thick_break_statg[inu] = hist_fung[inu] + hist_sumg[inu]
							- thick_stat_inputg[inu];
				} else {
					if (!prev_end) {
						thick_prev_begg[inu] = first;
						thick_prev_endg[inu] = last;
						thick_prev_statg[inu] = hist_fung[inu] + hist_sumg[inu]
								- thick_stat_inputg[inu];
					} else {
						if ((prev_end - prev_beg) < (last - first)) {
							thick_prev_begg[inu] = first;
							thick_prev_endg[inu] = last;
							thick_prev_statg[inu] = hist_fung[inu]
									+ hist_sumg[inu] - thick_stat_inputg[inu];
						}
					}
				}
			}
		}

		pus: IntAllInformGray[inu].num_of_int = num_of_intg[inu];
	}
}
//---------------------------------------------------------------------------
void CStrip::Strip_value_painting1(unsigned char intens, int beg_int,
		int end_int, int intens_consist) {
	int paint_coun;
	int current_inhabitant;
	int paint_start, paint_finish;
	int val;
	int point_location;
	int quant;
	int limit;

	limit = NUM_INTEN1 / 4;
	paint_start = beg_int >> PRESSING;
	paint_finish = end_int >> PRESSING;
	for (paint_coun = paint_start; paint_coun <= paint_finish; paint_coun++) {
		current_inhabitant = *(valuable_intensityg + paint_coun);
		quant = *(quantity_of_intensitiesg + paint_coun);
		point_location = quant * PressedLength + paint_coun;
		if (quant < limit) {
			*(intensities_occurredg + point_location) = intens;
			*(interval_occurredg + point_location) = num_of_intg[intens];
		}
		if (!current_inhabitant) {
			*(valuable_intensityg + paint_coun) = intens + 1;
			*(valuable_intervalg + paint_coun) = *(num_of_intg + intens);
			*(bright_consistencyg + paint_coun) = intens_consist;
		} else {
			val = *(bright_consistencyg + paint_coun);
			if (val < intens_consist) {
				*(valuable_intensityg + paint_coun) = intens + 1;
				*(bright_consistencyg + paint_coun) = intens_consist;
				*(valuable_intervalg + paint_coun) = *(num_of_intg + intens);
			}
		}
		(*(quantity_of_intensitiesg + paint_coun))++;
	}

}

//----------------------------------------------------------------
void CStrip::Important_interval1(int begin_interval, int end_interval,
		unsigned char intensi, int signi) {
	int subtr, prev_length, subtpr;
	int prev_beg, prev_end;

	subtr = end_interval - begin_interval;
	prev_beg = thick_begg[intensi];
	prev_end = thick_endg[intensi];
	num_of_intg[intensi]++;
	prev_length = prev_end - prev_beg;
	IntAllInformGray[intensi].num_of_int = num_of_intg[intensi];
	IntAllInformGray[intensi].BegInt[num_of_intg[intensi] - 1] = begin_interval;
	IntAllInformGray[intensi].Signif[num_of_intg[intensi] - 1] = signi;
	IntAllInformGray[intensi].EndInt[num_of_intg[intensi] - 1] = end_interval;

	if (!prev_end) {
		thick_begg[intensi] = begin_interval;
		thick_endg[intensi] = end_interval;
		thick_statg[intensi] = hist_fung[intensi] + hist_sumg[intensi];
		return;
	}
	if (subtr > prev_length) {
		thick_begg[intensi] = begin_interval;
		thick_endg[intensi] = end_interval;
		thick_statg[intensi] = hist_fung[intensi] + hist_sumg[intensi]
				- thick_stat_inputg[intensi];
		if ((!thick_break_endg[intensi]) && (thick_prev_endg[intensi])) {
			thick_break_begg[intensi] = thick_prev_begg[intensi];
			thick_break_endg[intensi] = thick_prev_endg[intensi];
			thick_break_statg[intensi] = thick_prev_statg[intensi];
		}
		thick_prev_begg[intensi] = prev_beg;
		thick_prev_endg[intensi] = prev_end;
		thick_prev_statg[intensi] = thick_statg[intensi];
	} else {
		if (thick_break_endg[intensi]) {
			subtpr = thick_break_endg[intensi] - thick_break_begg[intensi];
		} else {
			subtpr = 2048;
			thick_break_begg[intensi] = begin_interval;
			thick_break_endg[intensi] = end_interval;
			thick_break_statg[intensi] = hist_fung[intensi]
					+ hist_sumg[intensi] - thick_stat_inputg[intensi];
			goto ahead;
		}
		if (subtr > subtpr) {
			if (!thick_prev_endg[intensi]) {
				thick_prev_begg[intensi] = thick_break_begg[intensi];
				thick_prev_endg[intensi] = thick_break_endg[intensi];
				thick_prev_statg[intensi] = thick_break_statg[intensi];
			}
			thick_break_begg[intensi] = begin_interval;
			thick_break_endg[intensi] = end_interval;
			thick_break_statg[intensi] = hist_fung[intensi]
					+ hist_sumg[intensi] - thick_stat_inputg[intensi];
		} else if (!thick_prev_endg[intensi]) {
			thick_prev_begg[intensi] = begin_interval;
			thick_prev_endg[intensi] = end_interval;
			thick_prev_statg[intensi] = hist_fung[intensi] + hist_sumg[intensi]
					- thick_stat_inputg[intensi];
		}
	}
	ahead: ;
}
