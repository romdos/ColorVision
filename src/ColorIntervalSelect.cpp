/*
 *
 *
 *
 *
 *
 *
 *
 */





#include "ColorIntervalSelect.h"
#include "Strip.h"



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>





using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

static  int dev_mean[8] = {5, 8, 8, 10, 10, 10, 12, 8};
static  int imp_dev_mean[8] = {12, 14, 16, 16, 14, 14, 12, 12};
static  int dev_mean_sat[16] = {1, 1, 1, 2, 2, 3, 3, 4, 4, 4, 4, 4, 3, 2, 1, 0};
static  int dev_mean_sat1[16] = {0, 1, 1, 1, 2, 3, 3, 4, 4, 4, 5, 5, 5, 6, 7, 7};
static  int hue_zones[NUM_HUES] = {1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
5, 1, 1, 1, 1, 1};
static  int gray_zones[NUM_INTEN1] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7};
static  int dev_hue_zone[6] = {2, 2, 2, 2, 2, 2};



TIntColor::~TIntColor()
{
	delete[] NumberOfColorBunch;
	delete[] PosPeakHue;
	delete[] TotCMHue;
	delete[] NegPeakHue;
	delete[] TotCMSat;
	delete[] LeftHueValue;
	delete[] RightHueValue;
	delete[] LowerSatValue;
	delete[] UpperSatValue;
	delete[] TriangleHue;
	delete[] TypeOfColor;

}



CColorIntervalSelect::CColorIntervalSelect()
{
	NumInterestingIntensities = 0;
	NumberOfColoredIntervals = 0;
	NumberOfColorlessIntervals = 0;
	HueCorrection[0] = 0;
	HueCorrection[1] = 0;
	HueCorrection[2] = 0;
	HueCorrection[3] = 0;
	HueCorrection[4] = 1;
	HueCorrection[5] = 1;
	HueCorrection[6] = 1;
	HueCorrection[7] = 2;
	HueCorrection[8] = 2;
	HueCorrection[9] = 3;
	HueCorrection[10] = 3;
	HueCorrection[11] = 4;
	HueCorrection[12] = 5;
	HueCorrection[13] = 6;
	HueCorrection[14] = 6;
	HueCorrection[15] = 7;
	intensities_with_colored_int = NULL;
		OldNumbers = NULL;
		painted_strip_saturation = NULL;
		painted_numbers_saturation = NULL;
		saturation_consistency = NULL;
		painted_strip_colored = NULL;
		painted_numbers_colored = NULL;
		consistency_colored = NULL;
		painted_strip_colored_long = NULL;
		painted_numbers_colored_long = NULL;
		consistency_colored_long = NULL;
		bunch_blocking = NULL;
		bunches_occurred = NULL;
		new_bunch_number = NULL;
		old_bunch_number = NULL;
		bunch_connections = NULL;
		visible_bunches = NULL;
		left_continuation = NULL;
		right_continuation = NULL;
		left_adjacent = NULL;
		right_adjacent = NULL;
}




CColorIntervalSelect::~CColorIntervalSelect()
{
	if (intensities_with_colored_int != NULL)
		{
			delete[] intensities_with_colored_int;
			intensities_with_colored_int = NULL;
		}
		if (OldNumbers != NULL)
		{
			delete[] OldNumbers;
			OldNumbers = NULL;
		}
		if (painted_strip_saturation != NULL)
		{
			delete[] painted_strip_saturation;
			painted_strip_saturation = NULL;
		}
		if (painted_numbers_saturation != NULL)
		{
			delete[] painted_numbers_saturation;
			painted_numbers_saturation = NULL;
		}
		if (saturation_consistency != NULL)
		{
			delete[] saturation_consistency;
			saturation_consistency = NULL;
		}
		if (painted_strip_colored != NULL)
		{
			delete[] painted_strip_colored;
			painted_strip_colored = NULL;
		}
		if (painted_numbers_colored != NULL)
		{
			delete[] painted_numbers_colored;
			painted_numbers_colored = NULL;
		}
		if (consistency_colored != NULL)
		{
			delete[] consistency_colored;
			consistency_colored = NULL;
		}
		if (painted_strip_colored_long != NULL)
		{
			delete[] painted_strip_colored_long;
			painted_strip_colored_long = NULL;
		}
		if (painted_numbers_colored_long != NULL)
		{
			delete[] painted_numbers_colored_long;
			painted_numbers_colored_long = NULL;
		}
		if (consistency_colored_long != NULL)
		{
			delete[] consistency_colored_long;
			consistency_colored_long = NULL;
		}

}



void CColorIntervalSelect::FindingStructureParametrs(int* num_of_int,
		int*intens_with_int, int* OldNumb, int* NumInterestingIntensities)
{
	int int_cou;
	int MaxNumInt;
	int num_int;
	int coun_ini;
	int NumInterestIntens;

	NumberOfColoredIntervals = 0;
	NumberOfColorlessIntervals = 0;

	StripWidth = StripCurrent->StripWidth;
	dimX = StripCurrent->DimX;
	dimY = StripCurrent->DimY;
	GGBorGGR = StripCurrent->GGBorGGR;
	StripLength = StripCurrent->StripLength;
	MaxNumInt = 0;
	NumInterestIntens = 0;
	TotalNumInt = 0;

	for (int_cou = 0; int_cou < NUM_INTEN; int_cou++)
	{
		num_int = num_of_int[int_cou];

		if (num_int)
		{
			TotalNumInt += num_int;
			OldNumb[NumInterestIntens] = int_cou;
			intens_with_int[int_cou] = NumInterestIntens;
			NumInterestIntens++;
			if (num_int > MaxNumInt)
			{
				MaxNumInt = num_int;
			}
		}
		else
		{
			intens_with_int[int_cou] = -1;
		}
	}
	*NumInterestingIntensities = NumInterestIntens;
	if (!MaxNumInt)
	{
		NumberOfIntervals = 0;
		NumberOfColoredIntervals = 0;
		NumberOfIntervals1 = 0;
		NumberOfColorlessIntervals = 0;
		return;
	}

	MaximalNumber = MaxNumInt;


	ColorInform = new TIntColor[NumInterestIntens];


	for (coun_ini = 0; coun_ini < NumInterestIntens; coun_ini++)
	{
		ColorInform[coun_ini].num_of_int = MaxNumInt;
		ColorInform[coun_ini].NumberOfColorBunch = new int[MaxNumInt];
		ColorInform[coun_ini].PosPeakHue = new int[MaxNumInt];
		ColorInform[coun_ini].TotCMHue = new int[MaxNumInt];
		ColorInform[coun_ini].NegPeakHue = new int[MaxNumInt];
		ColorInform[coun_ini].TotCMSat = new int[MaxNumInt];
		ColorInform[coun_ini].LeftHueValue = new int[MaxNumInt];
		ColorInform[coun_ini].RightHueValue = new int[MaxNumInt];
		ColorInform[coun_ini].LowerSatValue = new int[MaxNumInt];
		ColorInform[coun_ini].UpperSatValue = new int[MaxNumInt];
		ColorInform[coun_ini].TriangleHue = new int[MaxNumInt];
		ColorInform[coun_ini].TypeOfColor = new int[MaxNumInt];
	}

	Colored_intervals_findingColorDivision(NumInterestIntens, OldNumbers,
			num_of_int, StripCurrent->IntAllInform);
	/*Colored_intervals_finding(NumInterestIntens,
	 OldNumbers,num_of_int,StripCurrent->IntAllInform);*/

}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void

CColorIntervalSelect::HueSaturationSideCalculation(int num_intensity,
		int TheOtherRatio, int gray, int* Hue, int* Saturation, int* TriangleSide,
		int* CircleHue, int* DoubleSaturation)
{
	float Ratio1;
	float Ratio2;
	float Saturation1;
	//float ClassicalSaturation; both definitions define the same object
	float Difsut;
	float FHue;
	int ISatur;
	int IDoubleSatur;
	int IHue;
	int IDoubleHue;
	float FHueDouble;



	          if(num_intensity>31)
			  {//6
	Ratio1=(float)num_intensity/(float)62;
	           if(TheOtherRatio>=31)
			   {
	Ratio2=(float)TheOtherRatio/(float)62;
			   }
			   else
			   {
	Ratio2=(float)TheOtherRatio/(float)63;
			   }
	if(num_intensity>=TheOtherRatio)
		{//1
		FHue=(16*Ratio2*(2*Ratio1-1))/(Ratio1-2*Ratio2*(1-Ratio1));
		FHueDouble=(32*Ratio2*(2*Ratio1-1))/(Ratio1-2*Ratio2*(1-Ratio1));
		IDoubleHue=(int)FHueDouble;
		if(FHueDouble-IDoubleHue>=0.5)
		 {//last_cor 14.04.15
			 IDoubleHue+=1;
		 }
		IHue=(int)FHue;
		if(IDoubleHue<16)
		{
	IDoubleHue=HueCorrection[IDoubleHue];
		}
		else
		{
	IDoubleHue=std::min(31,IDoubleHue);
	     IDoubleHue=31-IDoubleHue;
	     IDoubleHue=(7-HueCorrection[IDoubleHue])+8;
		}
		  if((num_intensity==32)&&(TheOtherRatio==31))
		  {
	      Saturation1=0;
		  }
		  else
		  {
		Saturation1=(16*(Ratio1-2*Ratio2*(1-Ratio1)))/(Ratio2+Ratio1*(1-Ratio2));
		  }
		*TriangleSide=0;
		/*if((num_intensity>32)&&(TheOtherRatio==31))
	             {
		         IHue++;
				 IDoubleHue++;
	             }*/
		}//1
		else
		{//2
		 FHue=(16*(Ratio2-Ratio1))/(Ratio2-2*Ratio1*(1-Ratio2))+16;
		 //FHue=32*(1-(Ratio1*(2*Ratio2-1))/(Ratio2-2*Ratio1*(1-Ratio2)));
		 //FHue=16*(1-(Ratio1*(2*Ratio2-1))/(Ratio2-2*Ratio1*(1-Ratio2)))+16;
		 //FHueDouble=32*(1-(Ratio1*(2*Ratio2-1))/(Ratio2-2*Ratio1*(1-Ratio2)));
		 FHueDouble=(32*(Ratio2-Ratio1))/(Ratio2-2*Ratio1*(1-Ratio2));
		 IHue=(int)FHue;
		 IDoubleHue=(int)FHueDouble;
		 if(FHueDouble-IDoubleHue>=0.5)
		 {//last_cor 14.04.15
			 IDoubleHue+=1;
		 }
		 Saturation1=(16*(Ratio2-2*Ratio1*(1-Ratio2)))/(Ratio2+Ratio1*(1-Ratio2));
						*TriangleSide=1;
	             if(IDoubleHue<16)
		         {
	             IDoubleHue=HueCorrection[IDoubleHue]+16;
		         }
		         else
		         {
	             IDoubleHue=std::min(31,IDoubleHue);
	             IDoubleHue=31-IDoubleHue;
	             IDoubleHue=(7-HueCorrection[IDoubleHue])+24;
		         }

		}//2
	          }//6
			  else
			  {//7
				  if(num_intensity==31)
				  {
					  if(TheOtherRatio==32)
					  {
	                  IHue=0;
					  IDoubleHue=0;
					  *Saturation=0;
					  *DoubleSaturation=0;
					  *TriangleSide=0;
					  goto N;
					  }
					  else
					  {
	                  if(TheOtherRatio==31)
					  {
					  IHue=31;
					  IDoubleHue=31;
					  *Saturation=0;
					  *DoubleSaturation=0;
					  *TriangleSide=0;
					  goto N;
					  }
					  }
				  }
	 Ratio1=(float)num_intensity/(float)63;
	           if(TheOtherRatio>=31)
			   {
	Ratio2=(float)TheOtherRatio/(float)62;
			   }
			   else
			   {
	Ratio2=(float)TheOtherRatio/(float)63;
			   }

		 if(TheOtherRatio<=num_intensity)
		{//5

		FHue=(16*(1-Ratio1)*(1-2*Ratio2))/(1-Ratio2*(3-2*Ratio1))+16;
		FHueDouble=(32*(1-Ratio1)*(1-2*Ratio2))/(1-Ratio2*(3-2*Ratio1));
		IHue=(int)FHue;
		IDoubleHue=(int)FHueDouble;
		if(FHueDouble-IDoubleHue>=0.5)
		 {//last_cor 14.04.15
			 IDoubleHue+=1;
		 }
		float R=(Ratio2*(1-Ratio1))/(1-Ratio2*Ratio1);
		Saturation1=(16*(1-Ratio2*(3-2*Ratio1)))/(1-Ratio2*Ratio1);
		*TriangleSide=1;
		         if(IDoubleHue<16)
		         {
	             IDoubleHue=HueCorrection[IDoubleHue]+16;
		         }
		         else
		         {
	             IDoubleHue=std::min(31,IDoubleHue);
	             IDoubleHue=31-IDoubleHue;
	             IDoubleHue=(7-HueCorrection[IDoubleHue])+24;
		         }

		}//5


			  else
			  {//8
	FHue=(16*(Ratio2-Ratio1))/(1-Ratio1*(3-2*Ratio2))+32;
		FHueDouble=(32*(Ratio2-Ratio1))/(1-Ratio1*(3-2*Ratio2));
		IHue=(int)FHue;
		IDoubleHue=(int)FHueDouble;
		if(FHueDouble-IDoubleHue>=0.5)
		 {//last_cor 14.04.15
			 IDoubleHue+=1;
		 }
		//float G=Ratio1*(1-Ratio2)/(1-Ratio1*Ratio2);
		Saturation1=16*((1-Ratio1*(3-2*Ratio2))/(1-Ratio2*Ratio1));
		*TriangleSide=2;
		         if(IDoubleHue<16)
		         {
	             IDoubleHue=HueCorrection[IDoubleHue]+32;
		         }
		         else
		         {
	             IDoubleHue=std::min(31,IDoubleHue);
	             IDoubleHue=31-IDoubleHue;
	             IDoubleHue=(7-HueCorrection[IDoubleHue])+40;
		         }
			  }//8
		}//7

	/*ClassicalSaturation=
	1-3*std::min(std::min(Ratio1*(1-Ratio2),Ratio1*Ratio2),Ratio2*(1-Ratio1))/(Ratio2+Ratio1*(1-Ratio2));
	ClassicalSaturation=16*ClassicalSaturation;*/

		ISatur=(int)Saturation1;

		IDoubleSatur=(int)(2*Saturation1);
		 Difsut=Saturation1-ISatur;

		 if(ISatur==1)
		 	 {//last_cor17.03.17
		 	 if(Difsut>0.5)
		 	 {
		 	 *Saturation=ISatur+1;
		 	 }
		 	 else
		 	 {
		 	 *Saturation=ISatur;
		 	 }
		 	 }
		 	 else
		 	 {
		 		if (ISatur >= 2)
		 				{//last_cor17.03.17
		 					if ((gray<30) || ((gray<35) && (num_intensity >= 37)))
		 					{//last_cor23.03.17and17.07.17
		 						if ((Difsut>0.4) || ((FHue >= 12) && (FHue <= 17) && (Difsut>0.2) && (num_intensity >= 36) && (TheOtherRatio >= 37))
		 							&& (ISatur<15))
		 						{
		 							*Saturation = ISatur + 1;
		 						}
		 						else
		 						{
		 							*Saturation = ISatur;
		 						}
		 					}
		 					else
		 					{
		 						if ((Difsut>0.5) && (ISatur<15))
		 						{
		 							*Saturation = ISatur + 1;
		 						}
		 						else
		 						{
		 							*Saturation = ISatur;
		 						}
		 					}//last_cor23.03.17
		 				}
		 				else
		 				{
		 					if (Difsut>0.8)
		 					{
		 						*Saturation = ISatur + 1;
		 					}
		 					else
		 					{
		 						*Saturation = ISatur;
		 					}
		 				}
		 			}//last_cor17.03.17

		 Difsut=2*Saturation1-IDoubleSatur;
		 	 if(IDoubleSatur>=4)
		 	 {//last_cor17.03.17
		 	 if((Difsut>0.4)&&(IDoubleSatur<30))
		 	 {
		 	 IDoubleSatur+=1;
		 	 }
		 	 }
		 	 else
		 	 {
		 if((Difsut>0.7)&&(IDoubleSatur<30))
		 	 {
		 	 IDoubleSatur+=1;
		 	 }
		 	 }//last_cor17.03.17

		 *DoubleSaturation=IDoubleSatur;

	N:
		*Hue=IHue;
		*CircleHue=IDoubleHue;

	}
//-----------------------------------------------------------------------------
void

CColorIntervalSelect::HueSaturationSideCalculationGGR(int num_intensity,
		int TheOtherRatio, int* Hue, int* Saturation, int* TriangleSide,
		int* CircleHue, int* DoubleSaturation)
{
	float Ratio1;
	float Ratio2;
	float Saturation1;
	//float ClassicalSaturation; both definitions define the same object
	float Difsut;
	float FHue;
	int ISatur;
	int IDoubleSatur;
	int IHue;
	int IDoubleHue;
	float FHueDouble;


	          if(num_intensity>31)
			  {//6
	 Ratio1=(float)num_intensity/(float)62;
	           if(TheOtherRatio>=31)
			   {
	Ratio2=(float)TheOtherRatio/(float)62;
			   }
			   else
			   {
	Ratio2=(float)TheOtherRatio/(float)63;
			   }
	if(num_intensity>=TheOtherRatio)
		{//1 triangle2
		FHue=(16*(Ratio1-Ratio2))/(Ratio1-2*Ratio2*(1-Ratio1))+16;
		FHueDouble=(32*(Ratio1-Ratio2))/(Ratio1-2*Ratio2*(1-Ratio1));
		IDoubleHue=(int)FHueDouble;
		IHue=(int)FHue;
		if(IDoubleHue<16)
		{
	IDoubleHue=HueCorrection[IDoubleHue]+16;
		}
		else
		{
	IDoubleHue=std::min(31,IDoubleHue);
	     IDoubleHue=31-IDoubleHue;
	     IDoubleHue=(7-HueCorrection[IDoubleHue])+24;
		}
		Saturation1=(16*(Ratio1-2*Ratio2*(1-Ratio1)))/(Ratio1+Ratio2*(1-Ratio1));
		*TriangleSide=1;
		}//1
		else
		{//2 triangle1
		 FHue=(16*Ratio1*(2*Ratio2-1))/(Ratio2-2*Ratio1*(1-Ratio2));
		 //FHue=32*(1-(Ratio1*(2*Ratio2-1))/(Ratio2-2*Ratio1*(1-Ratio2)));
		 //FHue=16*(1-(Ratio1*(2*Ratio2-1))/(Ratio2-2*Ratio1*(1-Ratio2)))+16;
		 //FHueDouble=32*(1-(Ratio1*(2*Ratio2-1))/(Ratio2-2*Ratio1*(1-Ratio2)));
		 FHueDouble=(32*Ratio1*(2*Ratio2-1))/(Ratio2-2*Ratio1*(1-Ratio2));
		 IHue=(int)FHue;
		 IDoubleHue=(int)FHueDouble;
		 Saturation1=(16*(Ratio2-2*Ratio1*(1-Ratio2)))/(Ratio2+Ratio1*(1-Ratio2));
						*TriangleSide=0;
	             if(IDoubleHue<16)
		         {
	             IDoubleHue=HueCorrection[IDoubleHue];
		         }
		         else
		         {
	             IDoubleHue=std::min(31,IDoubleHue);
	             IDoubleHue=31-IDoubleHue;
	             IDoubleHue=(7-HueCorrection[IDoubleHue])+8;
		         }


		}//2
	          }//6
			  else
			  {//7
				  Ratio1=(float)num_intensity/(float)63;
	           if(TheOtherRatio>=31)
			   {
	Ratio2=(float)TheOtherRatio/(float)62;
			   }
			   else
			   {
	Ratio2=(float)TheOtherRatio/(float)63;
			   }
		 if(num_intensity>=63-TheOtherRatio)
		{//5 triangle1

		FHue=(16*Ratio1*(2*Ratio2-1))/(Ratio2-2*Ratio1*(1-Ratio2));
		//(16*(Ratio1+Ratio2-1))/(2*Ratio1*(1-Ratio2)+3*Ratio2-2);
		FHueDouble=(32*Ratio1*(2*Ratio2-1))/(Ratio2-2*Ratio1*(1-Ratio2));
		IHue=(int)FHue;
		IDoubleHue=(int)FHueDouble;
		Saturation1=(16*(Ratio2-2*Ratio1*(1-Ratio2)))/(Ratio1+Ratio2*(1-Ratio1));
		*TriangleSide=0;
		         if(IDoubleHue<16)
		         {
	             IDoubleHue=HueCorrection[IDoubleHue];
		         }
		         else
		         {
	             IDoubleHue=std::min(31,IDoubleHue);
	             IDoubleHue=31-IDoubleHue;
	             IDoubleHue=(7-HueCorrection[IDoubleHue])+8;
		         }

		}//5

			  else
			  {//8 triangle3
	FHue=(16*Ratio2*(1-2*Ratio1))/(1-Ratio1*(1+2*Ratio2))+32;
		FHueDouble=(32*Ratio2*(1-2*Ratio1))/(1-Ratio1*(1+2*Ratio2));
		IHue=(int)FHue;
		IDoubleHue=(int)FHueDouble;
		Saturation1=(16*(1-Ratio1*(1+2*Ratio2)))/(1-Ratio1*(1-Ratio2));
		*TriangleSide=2;
		         if(IDoubleHue<16)
		         {
	             IDoubleHue=HueCorrection[IDoubleHue]+32;
		         }
		         else
		         {
	             IDoubleHue=std::min(31,IDoubleHue);
	             IDoubleHue=31-IDoubleHue;
	             IDoubleHue=(7-HueCorrection[IDoubleHue])+40;
		         }
			  }//8
		}//7

		ISatur=(int)Saturation1;
		IDoubleSatur=(int)(2*Saturation1);
		 Difsut=Saturation1-ISatur;
		if(ISatur<=1)
		 {
		 if(Difsut>0.4)
		 {
		 *Saturation=ISatur+1;
		 }
		 else
		 {
		 *Saturation=ISatur;
		 }
		 }
		 else
		 {
		 if((Difsut>0.5)&&(ISatur<15))
		 {
		 *Saturation=ISatur+1;
		 }
		 else
		 {
		 *Saturation=ISatur;
		 }
		 }
		 	 Difsut=2*Saturation1-IDoubleSatur;

		 if((Difsut>0.5)&&(IDoubleSatur<30))
		 {
		 IDoubleSatur+=1;
		 }

		 *Hue=IHue;
		*CircleHue=IDoubleHue;
		*DoubleSaturation=IDoubleSatur;
	}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void

CColorIntervalSelect::Strip_value_painting_saturation(int intens, int beg_int,
		int end_int, int num_int, int intens_saturation, int* painted_strip_sc,
		int* painted_numbers_sc, int* saturation_consistency_sc)
{
	int paint_coun;
	int current_inhabitant;
	int paint_start,paint_finish;
	int val;
	int current_beg;
	int current_end;
	int current_intens;
	int current_number;
	int current_length;
	int int_length;

	int_length=end_int-beg_int+1;
		paint_start=beg_int>>PRESSING;
		paint_finish=end_int>>PRESSING;
		for(paint_coun=paint_start;paint_coun<=paint_finish;paint_coun++)
		 {
			current_inhabitant=*(painted_strip_sc+paint_coun);

		  if(!current_inhabitant)
			{
			*(painted_strip_sc+paint_coun)=intens+1;
			*(painted_numbers_sc+paint_coun)=num_int;
			*(saturation_consistency_sc+paint_coun)=intens_saturation;
			}
			 else
			 {
			  val=*(saturation_consistency_sc+paint_coun);
				if(val<intens_saturation)
				{
			*(painted_strip_sc+paint_coun)=intens+1;
			*(saturation_consistency_sc+paint_coun)=intens_saturation;
			*(painted_numbers_sc+paint_coun)=num_int;
				}
				else
				{
	if(val==intens_saturation)
				{
	current_intens=*(painted_strip_sc+paint_coun)-1;
	current_number=*(painted_numbers_sc+paint_coun);
	current_beg=StripCurrent->IntAllInform[current_intens].BegInt[current_number];
	current_end=StripCurrent->IntAllInform[current_intens].EndInt[current_number];
	current_length=current_end-current_beg+1;
	        if(current_length<int_length)
			{
			*(painted_strip_sc+paint_coun)=intens+1;
			*(saturation_consistency_sc+paint_coun)=intens_saturation;
			*(painted_numbers_sc+paint_coun)=num_int;
			}
				}
				}
			 }

		 }

	}
//-----------------------------------------------------------------------------
void

CColorIntervalSelect::AnalysisColoredRanges(int* visibility, int* inver_num,
		int* interv_num, int* interval_intens)
{
	int* last_intensity_interval_investigated;
	 int* last_interval_continuation_investigated;
	  int count_scale;
	 int current_inhabitant;
	 int num_interval;
	 int last_int_explored;
	 int new_number_of_intensity;
	// int count_scale1;
	 int count_scale2;
	 int current_inhabitant1;
	 int current_inhabitant2;
	 int new_number_of_intensity1;
	 //int num_interval1;
	 int num_interval2;
	 int total_number;
	// std::uint8_t* painted_strip;
	 //int* painted_numbers;
	 int* involved_intervals;
	 int NumberOfIntervalsInvolved;
	 int* InvolvedCollection;
	 int MaxLength;
	 int interv_length;
	 int beg_interval;
	 int end_interval;
	 int number_of_intervals[8];
	 int count;
	 int* interval_zone;
	 int zone_number;
	 int inter_num;
	 int coun_zone;
	 int cou_int;
	 int bound_int;
	 int co_interval;
	 int count_lev;
	 int count_interval;
	 int num_intens;
	 int NumInt;
	 int begin_int;
	 int end_int;
	 int* hues_field;
	 int number_of_clusters;
	 int signif;
	 int signif1;
	 int iteration_step;
	 int* number_of_bunch_members;
	 int* bunches_members;
	// int* number_of_colorless_members;
	// int* colorless_members;
	 int start_point_of_members;
	 int success;
	 int number_mem;
	 int seed_length;
	 int bunch_length;
	 int length_ratio;
	 int better_repr;
	 int max_ratio;
	 int extended_intens;
	 int extended_int;
	 int Gray_Mean;
	 int Average_Hue;
	 int assigned_intervals;
	 int seed_sat;
	 int Str_length;
	 int visible;
	 int probe_point;
	 int bunch_located;
	 int checking;
	 int seed_consist;

	 NumberOfIntervalsInvolved=0;
	 MaxLength=0;
	 number_of_clusters=0;
	 start_point_of_members=0;
	 last_intensity_interval_investigated=new int[NUM_INTEN];
	 last_interval_continuation_investigated=new int[NUM_INTEN];
	 involved_intervals=new int[TotalNumInt];
	 InvolvedCollection=new int[TotalNumInt];
	 hues_field=new int[NUM_HUES];
	 number_of_bunch_members=new int[MAX_COL_INT];
	 bunches_members=new int[TotalNumInt];
	Str_length=StripCurrent->StripLength;

	 //painted_strip=StripCurrent->valuable_intensity;
	 //painted_numbers=StripCurrent->valuable_interval;

	memset(involved_intervals,(int) '\0',sizeof(int)*(TotalNumInt));
	memset(number_of_intervals,(int) '\0',sizeof(int)*(8));
	memset(hues_field,(int) '\0',sizeof(int)*(NUM_HUES));
	  for(count_scale2=0;count_scale2<PressedLength;count_scale2++)
		 {
		 current_inhabitant2=*(painted_strip_colored+count_scale2);
		  if(current_inhabitant2)
			{
		  current_inhabitant2-=1;
		  num_interval2=*(painted_numbers_colored+count_scale2);
		  new_number_of_intensity1=intensities_with_colored_int[current_inhabitant2];
			if(ColorInform[new_number_of_intensity1].TypeOfColor[num_interval2]>=0)
			{
			total_number=*(inver_num+NUM_INTEN*num_interval2+current_inhabitant2);
			  if(!*(involved_intervals+total_number))
			  {
	beg_interval=
	StripCurrent->IntAllInform[current_inhabitant2].BegInt[num_interval2];
	end_interval=
	StripCurrent->IntAllInform[current_inhabitant2].EndInt[num_interval2];
	 interv_length= end_interval-beg_interval+1;
	  *(involved_intervals+total_number)=interv_length;
	  visible=*(visibility+total_number);
	//if(((visible>=4)||((visible>=2)&&(interv_length<=4*StripWidth)))&&((interv_length>=2*StripWidth)||(visible>=8)))
	if(((visible>=4)||((visible>=2)&&(interv_length>=4*StripWidth)))&&((interv_length>=2*StripWidth)||(visible>=8)))
	{
	*(InvolvedCollection+NumberOfIntervalsInvolved)=total_number;
	  NumberOfIntervalsInvolved++;



	 if(interv_length>MaxLength)
	 {
	 MaxLength=interv_length;
	 }
	}
			  }
			 }
			}
		  }
		  interval_zone=new int[8*NumberOfIntervalsInvolved];
		  for(count=0;count<NumberOfIntervalsInvolved;count++)
		 {
		 total_number=*(InvolvedCollection+count);
		 interv_length=*(involved_intervals+total_number);
		 zone_number=(interv_length*8)/(MaxLength+1);
		 inter_num=zone_number*NumberOfIntervalsInvolved+
		 number_of_intervals[zone_number];
		 number_of_intervals[zone_number]++;
		 *(interval_zone+inter_num)=total_number;
		 }

	memset(painted_strip_colored,(int) '\0',sizeof(int)*(PressedLength));
	memset(painted_numbers_colored,(int) '\0',sizeof(int)*(PressedLength));

	for(coun_zone=0;coun_zone<8;coun_zone++)
			{//cz
			 inter_num=(7-coun_zone)*NumberOfIntervalsInvolved;
			 bound_int=number_of_intervals[7-coun_zone];
	 for(cou_int=0;cou_int<bound_int;cou_int++)
	 {//ci
	 total_number=*(interval_zone+inter_num+cou_int);
	 current_inhabitant1= *(interval_intens+total_number);
	co_interval=*(interv_num+total_number);
	new_number_of_intensity1=intensities_with_colored_int[current_inhabitant1];
	if(ColorInform[new_number_of_intensity1].NumberOfColorBunch[co_interval]<0)
	{
	memset(last_interval_continuation_investigated,(int) '\0',sizeof(int)*(NUM_INTEN));
	iteration_step=0;
	if(NumberOfColoredIntervals<MAX_COL_INT-1)
	{
	success=GroupingCloseColoredIntervals(current_inhabitant1,co_interval,
	last_interval_continuation_investigated,iteration_step,bunches_members,
	inver_num,&start_point_of_members,interval_intens,interv_num);
	}
	else
	{
	success=0;
	}
	if(success)
	{//succ
		  if(NumberOfColoredIntervals==1)
		  {
		  *(number_of_bunch_members+0)=0;
	     number_mem=0;
		  }
		  else
		  {
		  *(number_of_bunch_members+NumberOfColoredIntervals-1)=
		  *(number_of_bunch_members+NumberOfColoredIntervals-2)+
		  ColoredIntervalsStructure->Cardinality[NumberOfColoredIntervals-2];
		  number_mem=*(number_of_bunch_members+NumberOfColoredIntervals-1);
		  }
	begin_int=StripCurrent->IntAllInform[current_inhabitant1].BegInt[co_interval];
	end_int=StripCurrent->IntAllInform[current_inhabitant1].EndInt[co_interval];
	seed_length=end_int-begin_int+1;
	bunch_length=ColoredIntervalsStructure->EndInterv[NumberOfColoredIntervals-1]-
	ColoredIntervalsStructure->BegInterv[NumberOfColoredIntervals-1]+1;
	length_ratio=(16*seed_length)/bunch_length;
		if((length_ratio<12)&&(seed_length>2*StripWidth)&&(success==2))
		{
	max_ratio=Find_Better_Representative(current_inhabitant1,co_interval,
	&better_repr,bunches_members,number_mem,interv_num,interval_intens);
				  if(max_ratio)
				  {
				  success=0;
	iteration_step=1;
	extended_int=*(interv_num+better_repr);
	extended_intens=*(interval_intens+better_repr);
	memset(last_interval_continuation_investigated,(int) '\0',sizeof(int)*(NUM_INTEN));
	if(NumberOfColoredIntervals<MAX_COL_INT-1)
	{
	success=GroupingCloseColoredIntervals(extended_intens,extended_int,
	last_interval_continuation_investigated,iteration_step,bunches_members,
	inver_num,&start_point_of_members,interval_intens,interv_num);
	}
	else
	{
	success=0;
	}
				  }
	           else
				  {//e
	           success=0;
				  iteration_step=1;
				  extended_int=co_interval;
				  extended_intens=current_inhabitant1;
	memset(last_interval_continuation_investigated,(int) '\0',sizeof(int)*(NUM_INTEN));
	if(NumberOfColoredIntervals<MAX_COL_INT-1)
	{
	success=GroupingCloseColoredIntervals(extended_intens,extended_int,
	last_interval_continuation_investigated,iteration_step,bunches_members,
	inver_num,&start_point_of_members,interval_intens,interv_num);

	}
	else
	{
	success=0;
	}
				  }//e

		}//succ
		          if(success)
				  {
	begin_int=ColoredIntervalsStructure->BegInterv[NumberOfColoredIntervals-1];
	end_int=ColoredIntervalsStructure->EndInterv[NumberOfColoredIntervals-1];
	signif=ColoredIntervalsStructure->Significance[NumberOfColoredIntervals-1];
	signif=(signif<<1)/(end_int-begin_int+1);
	Strip_value_painting_bunch(NumberOfColoredIntervals-1,begin_int/4,end_int/4,signif,
	painted_strip_colored,painted_numbers_colored,consistency_colored,PressedLength);
	//last_cor22.10.13
				  }
	}//ci
				/*if(number_of_clusters<NumberOfColoredIntervals)
				{

	Hue_Painting(ColoredIntervalsStructure->LowerHueValue[number_of_clusters],
	ColoredIntervalsStructure->UpperHueValue[number_of_clusters],
	number_of_clusters,hues_field);
	number_of_clusters=NumberOfColoredIntervals;
				} */
	}//cz
	 }
			}
	memset(painted_strip_saturation,(int) '\0',sizeof(int)*(3*PressedLength));
	 for(count_lev=0;count_lev<NumInterestingIntensities;count_lev++)
	 {
	  num_intens=OldNumbers[count_lev];
	 NumInt=StripCurrent->num_of_int[num_intens];
	//except for pure black
	 if((num_intens<136))
	 {
		for(count_interval=0;count_interval<NumInt;count_interval++)
		{
		if((ColorInform[count_lev].NumberOfColorBunch[count_interval]<0)&&
	(ColorInform[count_lev].TypeOfColor[count_interval]>=0)&&
	(ColorInform[count_lev].TypeOfColor[count_interval]<100))
				 {
	begin_int=StripCurrent->IntAllInform[num_intens].BegInt[count_interval];
	end_int=StripCurrent->IntAllInform[num_intens].EndInt[count_interval];
	seed_length=end_int-begin_int+1;
	seed_sat=ColorInform[count_lev].TotCMSat[count_interval];
	signif1=StripCurrent->IntAllInform[num_intens].Signif[count_interval];
	signif=(signif1<<1)/seed_length;
	probe_point=(begin_int+end_int)/2;
	bunch_located=*(painted_strip_colored+probe_point/4);
	seed_consist=*(consistency_colored+probe_point/4);
	Average_Hue=ColorInform[count_lev].TotCMHue[count_interval];
	if(bunch_located)
	{
		bunch_located-=1;
	checking=Additional_Junction(bunch_located,count_lev,count_interval);
	}
	if((!bunch_located)||(signif>StripWidth/2)||((checking<1)&&((!bunch_located)||(checking<0)||((signif>StripWidth/3)||
	   ((seed_length>2*StripWidth)&&(signif>=2)))&&
	   ((seed_sat>=2)||((seed_sat==1)&&(signif1>Str_length/8)))||((seed_sat>=3)&&(signif>=2)))))
	{

	if((Average_Hue>8)&&(Average_Hue<24))
	{
	end_int+=2*PressedLength;
	begin_int+=2*PressedLength;
	}
	if((Average_Hue>23)&&(Average_Hue<38))
	{
	end_int+=4*PressedLength;
	begin_int+=4*PressedLength;
	}
	if((num_intens<134)||(seed_sat>6))
	Strip_value_painting_saturation(num_intens,begin_int,end_int,count_interval,
	seed_sat,
	painted_strip_saturation,painted_numbers_saturation,saturation_consistency);
	}
				 }
		 }
	  }
	 }
	memset(last_intensity_interval_investigated,(int) '\0',sizeof(int)*(NUM_INTEN));

	for(count_scale=0;count_scale<3*PressedLength;count_scale++)
		 {
		 current_inhabitant=*(painted_strip_saturation+count_scale);
		  if(current_inhabitant)
			{
			current_inhabitant-=1;
			num_interval=*(painted_numbers_saturation+count_scale);
	last_int_explored=*(last_intensity_interval_investigated+current_inhabitant)-1;
	new_number_of_intensity=intensities_with_colored_int[current_inhabitant];
	if((ColorInform[new_number_of_intensity].NumberOfColorBunch[num_interval]<0)
	&&(num_interval!=last_int_explored))
	{
	*(last_intensity_interval_investigated+current_inhabitant)=num_interval+1;
	memset(last_interval_continuation_investigated,(int) '\0',sizeof(int)*(NUM_INTEN));
	iteration_step=0;
	if(NumberOfColoredIntervals<MAX_COL_INT-1)
	{
	success=GroupingCloseColoredIntervals(current_inhabitant,num_interval,
	last_interval_continuation_investigated,iteration_step,bunches_members,
	inver_num,&start_point_of_members,interval_intens,interv_num);
	}
	else
	{
	success=0;
	}
	if(success)
	{
	if(NumberOfColoredIntervals==1)
		  {
		  *(number_of_bunch_members+0)=0;
		  number_mem=0;
		  }
		  else
		  {
		  *(number_of_bunch_members+NumberOfColoredIntervals-1)=
		  *(number_of_bunch_members+NumberOfColoredIntervals-2)+
		  ColoredIntervalsStructure->Cardinality[NumberOfColoredIntervals-2];
		  number_mem=*(number_of_bunch_members+NumberOfColoredIntervals-1);
		  }

	begin_int=StripCurrent->IntAllInform[current_inhabitant].BegInt[num_interval];
	end_int=StripCurrent->IntAllInform[current_inhabitant].EndInt[num_interval];
	seed_length=end_int-begin_int+1;
	bunch_length=ColoredIntervalsStructure->EndInterv[NumberOfColoredIntervals-1]-
	ColoredIntervalsStructure->BegInterv[NumberOfColoredIntervals-1]+1;
	length_ratio=(16*seed_length)/bunch_length;
		if((length_ratio<12)&&(seed_length>2*StripWidth)&&(success==2))
		{
	  max_ratio=Find_Better_Representative(current_inhabitant,num_interval,
	  &better_repr,bunches_members,number_mem,interv_num,interval_intens);
		  if(max_ratio)
				  {
	iteration_step=1;
	extended_int=*(interv_num+better_repr);
	extended_intens=*(interval_intens+better_repr);
	memset(last_interval_continuation_investigated,(int) '\0',sizeof(int)*(NUM_INTEN));
	if(NumberOfColoredIntervals<MAX_COL_INT-1)
	{
	success=GroupingCloseColoredIntervals(extended_intens,extended_int,
	last_interval_continuation_investigated,iteration_step,bunches_members,
	inver_num,&start_point_of_members,interval_intens,interv_num);
	}
	else
	{
	success=0;
	}
				  }
				 else
				  {
				  iteration_step=1;
				  extended_int=num_interval;
				  extended_intens=current_inhabitant;
	memset(last_interval_continuation_investigated,(int) '\0',sizeof(int)*(NUM_INTEN));
	if(NumberOfColoredIntervals<MAX_COL_INT-1)
	{
	success=GroupingCloseColoredIntervals(extended_intens,extended_int,
	last_interval_continuation_investigated,iteration_step,bunches_members,
	inver_num,&start_point_of_members,interval_intens,interv_num);
	}
	else
	{
	success=0;
	}
				  }
		}
	}
	}
			}
		 }

	memset(painted_strip_saturation,(int) '\0',sizeof(int)*(3*PressedLength));
	for(count_lev=0;count_lev<NumInterestingIntensities;count_lev++)
	 {
	  num_intens=OldNumbers[count_lev];
	 NumInt=StripCurrent->num_of_int[num_intens];

	 if((num_intens<=124)||(num_intens>=131))
	  {
		for(count_interval=0;count_interval<NumInt;count_interval++)
		{
		if((ColorInform[count_lev].NumberOfColorBunch[count_interval]<0)&&
	(ColorInform[count_lev].TypeOfColor[count_interval]>=0)&&
	(ColorInform[count_lev].TypeOfColor[count_interval]<100))
				 {
	begin_int=StripCurrent->IntAllInform[num_intens].BegInt[count_interval];
	end_int=StripCurrent->IntAllInform[num_intens].EndInt[count_interval];
	signif=StripCurrent->IntAllInform[num_intens].Signif[count_interval];
	signif=(signif<<1)/(end_int-begin_int+1);
	if(signif>=StripWidth/2)
	{
	Gray_Mean=StripCurrent->IntAllInform[num_intens].GrayScaleMean[count_interval];
	if(Gray_Mean<=20)
	{
	end_int+=2*PressedLength;
	begin_int+=2*PressedLength;
	}
	else
	{
		if(Gray_Mean>=50)
	    {
	end_int+=4*PressedLength;
	begin_int+=4*PressedLength;
		}
	}
	Strip_value_painting_saturation(num_intens,begin_int,end_int,count_interval,
	signif,painted_strip_saturation,painted_numbers_saturation,
	saturation_consistency);
	}
				 }
		 }
	  }
	 }
	 memset(last_intensity_interval_investigated,(int) '\0',sizeof(int)*(NUM_INTEN));
	 for(count_scale=0;count_scale<3*PressedLength;count_scale++)
		 {
		 current_inhabitant=*(painted_strip_saturation+count_scale);
		  if(current_inhabitant)
			{
			current_inhabitant-=1;
			num_interval=*(painted_numbers_saturation+count_scale);
	last_int_explored=*(last_intensity_interval_investigated+current_inhabitant)-1;
	new_number_of_intensity=intensities_with_colored_int[current_inhabitant];
	if((ColorInform[new_number_of_intensity].NumberOfColorBunch[num_interval]<0)
	&&(num_interval!=last_int_explored))
	{
	*(last_intensity_interval_investigated+current_inhabitant)=num_interval+1;
	memset(last_interval_continuation_investigated,(int) '\0',sizeof(int)*(NUM_INTEN));
	iteration_step=0;
	if(NumberOfColoredIntervals<MAX_COL_INT-1)
	{
	success=GroupingCloseColoredIntervals(current_inhabitant,num_interval,
	last_interval_continuation_investigated,iteration_step,bunches_members,
	inver_num,&start_point_of_members,interval_intens,interv_num);
	}
	else
	{
	success=0;
	}
	if(success)
	{
	if(NumberOfColoredIntervals==1)
		  {
		  *(number_of_bunch_members+0)=0;
		  number_mem=0;
		  }
		  else
		  {
		  *(number_of_bunch_members+NumberOfColoredIntervals-1)=
		  *(number_of_bunch_members+NumberOfColoredIntervals-2)+
		  ColoredIntervalsStructure->Cardinality[NumberOfColoredIntervals-2];
		  number_mem=*(number_of_bunch_members+NumberOfColoredIntervals-1);
		  }

	begin_int=StripCurrent->IntAllInform[current_inhabitant].BegInt[num_interval];
	end_int=StripCurrent->IntAllInform[current_inhabitant].EndInt[num_interval];
	seed_length=end_int-begin_int+1;
	bunch_length=ColoredIntervalsStructure->EndInterv[NumberOfColoredIntervals-1]-
	ColoredIntervalsStructure->BegInterv[NumberOfColoredIntervals-1]+1;
	length_ratio=(16*seed_length)/bunch_length;
		if((length_ratio<12)&&(seed_length>2*StripWidth)&&(success==2))
		{
	  max_ratio=Find_Better_Representative(current_inhabitant,num_interval,
	  &better_repr,bunches_members,number_mem,interv_num,interval_intens);
	  if(max_ratio)
				  {
	iteration_step=1;
	extended_int=*(interv_num+better_repr);
	extended_intens=*(interval_intens+better_repr);
	memset(last_interval_continuation_investigated,(int) '\0',sizeof(int)*(NUM_INTEN));
	if(NumberOfColoredIntervals<MAX_COL_INT-1)
	{
	success=GroupingCloseColoredIntervals(extended_intens,extended_int,
	last_interval_continuation_investigated,iteration_step,bunches_members,
	inver_num,&start_point_of_members,interval_intens,interv_num);
	}
	else
	{
	success=0;
	}
				  }
				  else
				  {
				  iteration_step=1;
				  extended_int=num_interval;
				  extended_intens=current_inhabitant;
	memset(last_interval_continuation_investigated,(int) '\0',sizeof(int)*(NUM_INTEN));
	if(NumberOfColoredIntervals<MAX_COL_INT-1)
	{
	success=GroupingCloseColoredIntervals(extended_intens,extended_int,
	last_interval_continuation_investigated,iteration_step,bunches_members,
	inver_num,&start_point_of_members,interval_intens,interv_num);
	}
	else
	{
	success=0;
	}
				  }
		}
	}
	}
			}
		 }

	 ColoredIntervalsStructure->num_of_int=NumberOfColoredIntervals;
	 if(!NumberOfColoredIntervals)
	 {
	 RefinedNumberOfBunches=0;
	 assigned_intervals=TotalNumInt;
	 }
	 else
	 {
		 BunchClassification();
		 assigned_intervals=*(number_of_bunch_members+NumberOfColoredIntervals-1);
		 assigned_intervals=TotalNumInt+1-assigned_intervals;
	 }




		//number_of_colorless_members=new int[MAX_COLLESS_INT];
		//colorless_members=new int[assigned_intervals];
	/*AnalysisColorLessBack(inver_num,last_interval_continuation_investigated,
	 visibility,interv_num,interval_intens,number_of_colorless_members,
	 colorless_members);*/
	 delete[] last_intensity_interval_investigated;
	 delete[] last_interval_continuation_investigated;
	 delete[] involved_intervals;
	 delete[] InvolvedCollection;
	 delete[] interval_zone;
	 delete[] hues_field;
	 delete[] number_of_bunch_members;
	 delete[] bunches_members;
	}
//-----------------------------------------------------------------------------
int

CColorIntervalSelect::GroupingCloseColoredIntervals(int intensity,int num_int,
int* continuation_investigated,int iter_step,int* members,int* inver_num,
int* start_poin,int* intens_num,int* inter_num)
{
int intensity_red;
int beg_int;
int end_int;
int beg_int1;
int end_int1;
int beg_int_new;
int end_int_new;
int paint_start,paint_finish;
int paint_coun;
int intens_cou;
int quant;
int intensity_new;
int intensity_red_new;
int interval_new;
int point_location;
int* quantity;
int* intensity_list;
int* interv_occurred;
int new_number_of_intensity;
int new_number_of_new_intensity;
int intersection1;
int intersection2;
int intersection3;
int last_investigated_interval;
int paint_step;
int cycle_limit;
int left_hue_boundary;
int right_hue_boundary;
int orig_left_hue_boundary;
int orig_right_hue_boundary;
int left_sat_boun;
int orig_left_sat_boun;
int right_sat_boun;
int left_bound_sat;
int right_bound_sat;
int orig_right_sat_boun;
int new_left_sat_boun;
int new_right_sat_boun;
int new_left_hue_boundary;
int new_right_hue_boundary;
int grain_significance;
int new_impact_of_significance;
int signif_new;
int signif_basic;
int signif_balance;
int signif_balance1;
int number_of_extentions;
int intensity_interval_left;
int intensity_interval_right;
int indic_length;
int ratio_length;
int ratio_length1;
int greater_found;
int intensity_balance;
int intensity_new_balance;
//int abs_balance_difference;
int initial_gr_minimum;
int initial_gr_maximum;
int initial_gr_range;
int minimum_grayscale;
int maximum_grayscale;
int left_bound_gray;
int right_bound_gray;
int new_minimum_grayscale;
int new_maximum_grayscale;
int new_left_bound_gray;
int new_right_bound_gray;
int mean_grayscale;
int new_gr_range;
int new_mean_grayscale;
//int internal_signif;
//int external_signif;
int intens_zero;
int intens_new_zero;
int intens_zero1;
int intens_new_zero1;
int neighb_pos_fraction;
int neighb_neg_fraction;
int new_neighb_pos_fraction;
int new_neighb_neg_fraction;
int mean_signif;
//int strip_width;
int saturation_distance;
int saturation_distance1;
int mean_gr_distance;
//int length_hue;
//int left_length_hue;
//int right_length_hue;
int Mean_hue_bunch;
float Bunch_hue;
int Mean_hue_new;
int Mean_hue_difference;
int Mean_sat_bunch;
float Bunch_sat;
int Mean_sat_new;
int Mean_gray;
int min_gray;
float Bunch_gray;
int prior;
int total_number;
int total_number_new;
int orig_seed_hue;
int node_vicinity;
int node_vicinity_new;
int max_mean_gray_jump;
int int_length;
int possible_variation;
int possible_sat_variation;
int possible_sat_variation1;
int small_interval;
int small_interval_new;
int int_length_new;
int seed_density;
int applicant_density;
int density_ratio;
int Last_image_pixel;


small_interval=0;
intersection1=64;
intersection2=64;
max_mean_gray_jump=64;
greater_found=0;
ratio_length=-1;
ratio_length1=-1;
prior=0;
indic_length=-1;
node_vicinity=0;

intensity_red=StripCurrent->IntAllInform[intensity].MeanMRatio[num_int];


//strip_width=StripCurrent->StripWidth;

if(!iter_step)
{//first iteration
beg_int=StripCurrent->IntAllInform[intensity].BegInt[num_int];
end_int=StripCurrent->IntAllInform[intensity].EndInt[num_int];
int_length=end_int-beg_int+1;
if(int_length<=StripWidth)
{
small_interval=1;
}
grain_significance=StripCurrent->IntAllInform[intensity].Signif[num_int];
seed_density=(2*grain_significance)/int_length;
signif_basic=(16*grain_significance)/int_length;
number_of_extentions=0;
minimum_grayscale=StripCurrent->IntAllInform[intensity].LowerGrayScale[num_int];
maximum_grayscale=StripCurrent->IntAllInform[intensity].UpperGrayScale[num_int];

mean_grayscale=StripCurrent->IntAllInform[intensity].GrayScaleMean[num_int];
Mean_gray=mean_grayscale;
left_bound_gray=std::min((maximum_grayscale-mean_grayscale),(mean_grayscale-minimum_grayscale));
right_bound_gray=mean_grayscale+left_bound_gray;
left_bound_gray=mean_grayscale-left_bound_gray;
minimum_grayscale=left_bound_gray;
maximum_grayscale=right_bound_gray;
//initial_gr_minimum=minimum_grayscale;
//initial_gr_maximum=maximum_grayscale;
initial_gr_minimum=left_bound_gray;
initial_gr_maximum=right_bound_gray;
possible_variation=dev_mean[Mean_gray/8];
intensity_interval_left=intensity_red;
intensity_interval_right=intensity_red;
new_number_of_intensity=intensities_with_colored_int[intensity];
left_hue_boundary=ColorInform[new_number_of_intensity].LeftHueValue[num_int];
right_hue_boundary=ColorInform[new_number_of_intensity].RightHueValue[num_int];
left_sat_boun=ColorInform[new_number_of_intensity].LowerSatValue[num_int];
right_sat_boun=ColorInform[new_number_of_intensity].UpperSatValue[num_int];
Mean_sat_bunch=ColorInform[new_number_of_intensity].TotCMSat[num_int];

left_bound_sat=std::min((right_sat_boun-Mean_sat_bunch),(Mean_sat_bunch-left_sat_boun));
right_bound_sat=Mean_sat_bunch+left_bound_sat;
left_bound_sat=Mean_sat_bunch-left_bound_sat;
orig_seed_hue=ColorInform[new_number_of_intensity].TotCMHue[num_int];
if(orig_seed_hue<0)
{
return(prior);
}
if(((orig_seed_hue>=44)&&(orig_seed_hue<NUM_HUES))||
((orig_seed_hue>=0)&&(orig_seed_hue<4))||
((orig_seed_hue>=28)&&(orig_seed_hue<36))||
((orig_seed_hue>=12)&&(orig_seed_hue<20)))
{
node_vicinity=1;
}
Mean_hue_bunch=orig_seed_hue;
Bunch_hue=(float)Mean_hue_bunch;


Bunch_sat=(float)Mean_sat_bunch;

}//first iteration
else
{//second iteration
beg_int=
ColoredIntervalsStructure->BegInterv[NumberOfColoredIntervals-1];
end_int=
ColoredIntervalsStructure->EndInterv[NumberOfColoredIntervals-1];
beg_int1=StripCurrent->IntAllInform[intensity].BegInt[num_int];
end_int1=StripCurrent->IntAllInform[intensity].EndInt[num_int];
int_length=end_int-beg_int+1;
number_of_extentions=
ColoredIntervalsStructure->Cardinality[NumberOfColoredIntervals-1];
//grain_significance=StripCurrent->IntAllInform[intensity].Signif[num_int];
grain_significance=
ColoredIntervalsStructure->Significance[NumberOfColoredIntervals-1];
signif_basic=(16*grain_significance)/int_length;
seed_density=(2*grain_significance)/int_length;
minimum_grayscale=
ColoredIntervalsStructure->LowerGrayscale[NumberOfColoredIntervals-1];
maximum_grayscale=
ColoredIntervalsStructure->UpperGrayscale[NumberOfColoredIntervals-1];
mean_grayscale=
ColoredIntervalsStructure->AverageGray[NumberOfColoredIntervals-1];
left_bound_gray=std::min((maximum_grayscale-mean_grayscale),(mean_grayscale-minimum_grayscale));
right_bound_gray=mean_grayscale+left_bound_gray;
left_bound_gray=mean_grayscale-left_bound_gray;
minimum_grayscale=left_bound_gray;
maximum_grayscale=right_bound_gray;
initial_gr_minimum=
StripCurrent->IntAllInform[intensity].LowerGrayScale[num_int];
initial_gr_maximum=
StripCurrent->IntAllInform[intensity].UpperGrayScale[num_int];
Mean_gray=
ColoredIntervalsStructure->AverageGray[NumberOfColoredIntervals-1];
possible_variation=dev_mean[Mean_gray/8];
intensity_interval_left=
ColoredIntervalsStructure->LowerIntensity[NumberOfColoredIntervals-1];
intensity_interval_right=
ColoredIntervalsStructure->UpperIntensity[NumberOfColoredIntervals-1];
left_hue_boundary=
ColoredIntervalsStructure->LowerHueValue[NumberOfColoredIntervals-1];
right_hue_boundary=
ColoredIntervalsStructure->UpperHueValue[NumberOfColoredIntervals-1];
left_sat_boun=
ColoredIntervalsStructure->LowerSatValue[NumberOfColoredIntervals-1];
right_sat_boun=
ColoredIntervalsStructure->UpperSatValue[NumberOfColoredIntervals-1];
Mean_hue_bunch=
ColoredIntervalsStructure->AverageHue[NumberOfColoredIntervals-1];
Bunch_hue=(float)Mean_hue_bunch;
Mean_sat_bunch=
ColoredIntervalsStructure->AverageSat[NumberOfColoredIntervals-1];
left_bound_sat=std::min((right_sat_boun-Mean_sat_bunch),(Mean_sat_bunch-left_sat_boun));
right_bound_sat=Mean_sat_bunch+left_bound_sat;
Bunch_sat=(float)Mean_sat_bunch;
if(((Mean_hue_bunch>=44)&&(Mean_hue_bunch<NUM_HUES))||
((Mean_hue_bunch>=0)&&(Mean_hue_bunch<4))||
((Mean_hue_bunch>=28)&&(Mean_hue_bunch<36))||
((Mean_hue_bunch>=12)&&(Mean_hue_bunch<20)))
{
node_vicinity=1;
}
}//second iteration
possible_sat_variation=dev_mean_sat[Mean_sat_bunch];
possible_sat_variation1=dev_mean_sat1[Mean_sat_bunch];
if(Mean_sat_bunch<=3)
{
max_mean_gray_jump=24;
}
initial_gr_range=initial_gr_maximum-initial_gr_minimum+1;
intensity_balance=
StripCurrent->IntAllInform[intensity].MainOpponentRatiosBalance[num_int];
intens_zero=
StripCurrent->IntAllInform[intensity].ZeroRatio[num_int];
intens_zero1=
StripCurrent->IntAllInform[intensity].ZeroRatio1[num_int];
neighb_pos_fraction=
StripCurrent->IntAllInform[intensity].NeighborPosFraction[num_int];
neighb_neg_fraction=
StripCurrent->IntAllInform[intensity].NeighborNegFraction[num_int];


		 if(!iter_step)
		  {
			  if(*start_poin<TotalNumInt)
			  {
		  total_number=*(inver_num+NUM_INTEN*num_int+intensity);
		  *(members+*start_poin)=total_number;

		  (*start_poin)++;
			  }
			  else
			  {
				  return(0);
			  }

		  }
Last_image_pixel=(StripLength-1)>>PRESSING;
if(!iter_step)
{
paint_start=beg_int>>PRESSING;
paint_finish=end_int>>PRESSING;
}
else
{
paint_start=beg_int1>>PRESSING;
paint_finish=end_int1>>PRESSING;
}
if(int_length<=StripWidth)
{
paint_finish=std::min(paint_finish+2,Last_image_pixel);
paint_start=std::max(0,paint_start-2);
}
cycle_limit=(paint_finish-paint_start)/2;

quantity=StripCurrent->quantity_of_intensities;
intensity_list=StripCurrent->intensities_occurred;
interv_occurred=StripCurrent->interval_occurred;

orig_left_hue_boundary=left_hue_boundary;
orig_right_hue_boundary=right_hue_boundary;

orig_left_sat_boun=left_sat_boun;
orig_right_sat_boun=right_sat_boun;

for(paint_coun=0;paint_coun<=cycle_limit;paint_coun++)
	 {
	 paint_step=paint_start+2*paint_coun;
	 quant=*(quantity+paint_step);
		 for(intens_cou=0;intens_cou<quant;intens_cou++)
		 {
		 point_location=intens_cou*PressedLength+paint_step;
		 intensity_new=*(intensity_list+point_location);
		 interval_new=*(interv_occurred+point_location);

		 intensity_red_new=StripCurrent->IntAllInform[intensity_new].MeanMRatio[interval_new];
		 new_number_of_new_intensity=intensities_with_colored_int[intensity_new];
if(((intensity_new==intensity)&&(interval_new==num_int)&&(!iter_step))
||
(ColorInform[new_number_of_new_intensity].NumberOfColorBunch[interval_new]>=0)||
(ColorInform[new_number_of_new_intensity].TypeOfColor[interval_new]<0)||
(ColorInform[new_number_of_new_intensity].TypeOfColor[interval_new]>100))
		 {
		 goto L;
		 }
small_interval_new=0;
			last_investigated_interval=
		 *(continuation_investigated+intensity_new)-1;

		  if(interval_new>last_investigated_interval)
		 {
		 *(continuation_investigated+intensity_new)=interval_new+1;
		 intens_new_zero=
StripCurrent->IntAllInform[intensity_new].ZeroRatio[interval_new];
		 intens_new_zero1=
StripCurrent->IntAllInform[intensity_new].ZeroRatio1[interval_new];
		 intensity_new_balance=
StripCurrent->IntAllInform[intensity_new].MainOpponentRatiosBalance[interval_new];
new_impact_of_significance=
	 StripCurrent->IntAllInform[intensity_new].Signif[interval_new];

new_neighb_pos_fraction=
StripCurrent->IntAllInform[intensity_new].NeighborPosFraction[interval_new];
new_neighb_neg_fraction=
StripCurrent->IntAllInform[intensity_new].NeighborNegFraction[interval_new];

	beg_int_new=StripCurrent->IntAllInform[intensity_new].BegInt[interval_new];
	end_int_new=StripCurrent->IntAllInform[intensity_new].EndInt[interval_new];
    int_length_new=end_int_new-beg_int_new+1;
	applicant_density=(2*new_impact_of_significance)/int_length_new;
	if(int_length_new<=StripWidth)
	{
	small_interval_new=1;
	}
	signif_new=(16*new_impact_of_significance)/(end_int_new-beg_int_new+1);
	signif_balance=(16*signif_new)/(signif_new+signif_basic);
	signif_balance1=(16*new_impact_of_significance)/(new_impact_of_significance+grain_significance);
	density_ratio=(16*applicant_density)/(applicant_density+seed_density);
	new_minimum_grayscale=
	StripCurrent->IntAllInform[intensity_new].LowerGrayScale[interval_new];
	new_maximum_grayscale=
	StripCurrent->IntAllInform[intensity_new].UpperGrayScale[interval_new];
	new_mean_grayscale=
	StripCurrent->IntAllInform[intensity_new].GrayScaleMean[interval_new];
	new_gr_range=new_maximum_grayscale-new_minimum_grayscale+1;
	new_left_bound_gray=std::min((new_maximum_grayscale-new_mean_grayscale),(new_mean_grayscale-new_minimum_grayscale));
    new_right_bound_gray=new_mean_grayscale+new_left_bound_gray;
    new_left_bound_gray=new_mean_grayscale-new_left_bound_gray;

	if(abs(new_mean_grayscale-Mean_gray)>max_mean_gray_jump)
	{
	goto L;
	}
  new_minimum_grayscale=new_left_bound_gray;
new_maximum_grayscale=new_right_bound_gray;
	/*intersection1=Intersection_measure(beg_int,end_int,beg_int_new,
	end_int_new,&indic_length);*/
	intersection1=Intersection_measure_ratios(beg_int,end_int,beg_int_new,
	end_int_new,&indic_length,&ratio_length,&ratio_length1);
	if(intersection1==3)
	{
		ratio_length=0;
	}
	//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
	//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller

			if((intersection1<=2)||((intersection1==3)&&(indic_length==3)&&(int_length<=StripWidth)))
			{

	new_left_hue_boundary=
	ColorInform[new_number_of_new_intensity].LeftHueValue[interval_new];
	new_right_hue_boundary=
	ColorInform[new_number_of_new_intensity].RightHueValue[interval_new];
	Mean_hue_new=
	ColorInform[new_number_of_new_intensity].TotCMHue[interval_new];
	Mean_sat_new=
	ColorInform[new_number_of_new_intensity].TotCMSat[interval_new];
	new_left_sat_boun=
	ColorInform[new_number_of_new_intensity].LowerSatValue[interval_new];
	new_right_sat_boun=
	ColorInform[new_number_of_new_intensity].UpperSatValue[interval_new];
	if(((Mean_sat_bunch==0)||((Mean_sat_bunch==1)&&(intensity>123)&&(abs(intensity-intensity_new<=1))))&&(Mean_sat_new<=1))
	{
	intersection2=8;
	}
	else
	{
	intersection2=ColorIntervalComparison(orig_left_hue_boundary,
	orig_right_hue_boundary,new_left_hue_boundary,new_right_hue_boundary);
	}


	          if(Mean_sat_bunch>=2)
			  {
	if((Mean_hue_bunch<7)&&(Mean_hue_new>=8)&&(Mean_hue_new<=21))
	{
	 goto L;
	}
	if((Mean_hue_bunch>=8)&&(Mean_hue_new<7)&&(Mean_hue_bunch<=21))
	{
	 goto L;
	}
	   if((Mean_sat_new<=1)&&(indic_length<=1))
	   {
		   goto L;
	   }
			  }
	if(intersection2==9)
	{
	goto L;
	}
	node_vicinity_new=0;
	if(((Mean_hue_new>=44)&&(Mean_hue_new<NUM_HUES))||
((Mean_hue_new>=0)&&(Mean_hue_new<4))||
((Mean_hue_new>=28)&&(Mean_hue_new<36))||((Mean_hue_bunch>=12)&&(Mean_hue_bunch<20)))
{
node_vicinity_new=1;
}
	intersection3=GrayIntersection(left_bound_gray,right_bound_gray,
new_left_bound_gray,new_right_bound_gray);
	if(Mean_sat_bunch>=Mean_sat_new)
	{
		saturation_distance=Mean_sat_bunch-Mean_sat_new;
		saturation_distance1=0;
	}
	else
	{
	saturation_distance1=Mean_sat_new-Mean_sat_bunch;
	saturation_distance=0;
	}
/* saturation_distance=0;
if(new_left_sat_boun>orig_right_sat_boun)
{
saturation_distance=new_left_sat_boun-orig_right_sat_boun;
}
if(orig_left_sat_boun>new_right_sat_boun)
{
saturation_distance=orig_left_sat_boun-new_right_sat_boun;
}*/
mean_gr_distance=abs(new_mean_grayscale-mean_grayscale);
min_gray=std::min(mean_grayscale,new_mean_grayscale);
if((new_mean_grayscale<10)&&(mean_gr_distance>10)&&(Mean_sat_bunch>8))
{
goto L;
}

	if((indic_length==0)&&(intersection1>=2)&&(intersection2!=5)&&
	(intersection2>=3))
	{
	goto L;
	}
	if((2*mean_gr_distance>min_gray)&&(min_gray>22))
	{
	goto L;
	}
	if((orig_left_sat_boun>12)&&(mean_gr_distance>15)&&(saturation_distance>3))
	{
	goto L;
	}

   if((Mean_hue_bunch<8)&&(Mean_hue_new>32))
				 {
				 Mean_hue_difference=Mean_hue_bunch+NUM_HUES-Mean_hue_new;
				 }
				 else
				 {
				 if((Mean_hue_bunch>32)&&(Mean_hue_new<8))
				 {
				 Mean_hue_difference=Mean_hue_new+NUM_HUES-Mean_hue_bunch;
				 }
				 else
				 {
				 Mean_hue_difference=abs(Mean_hue_bunch-Mean_hue_new);
				 }
				 }
                     if((Mean_sat_bunch<=1)&&(Mean_sat_new<=1)&&(intersection1>=2))
					 {
						 goto L;
					 }
					 if((Mean_hue_difference<=1)&&(saturation_distance<=1))
					 {
                      intersection2=0;
					 }
      if(((Mean_sat_bunch==0)||((Mean_sat_bunch==1)&&(!iter_step)))&&(Mean_sat_new<=1))
		                        {
	if((Mean_sat_bunch==0)&&(Mean_sat_new==1)&&
		((indic_length==3)||(signif_balance<=4))&&(Mean_hue_difference>8)&&(abs(intensity-intensity_new)>2))
	{
		goto L;
	}
if((Mean_sat_bunch==1)&&(Mean_sat_new==0)&&
   ((indic_length>=2)||(signif_balance<=4))&&(Mean_hue_difference>8)&&(abs(intensity-intensity_new)>2))
	{
		goto L;
	}
if((Mean_sat_bunch==1)&&(Mean_sat_new==1)&&
   (Mean_hue_difference>8)&&(abs(intensity-intensity_new)>2))
	{
		goto L;
	}
if((Mean_sat_bunch==1)&&(Mean_sat_new>=2)&&
   (Mean_hue_difference>=6))
	{//last_cor06.02.17
		goto L;
	}
if((Mean_sat_bunch>=2)&&(Mean_sat_new==1)&&
   (Mean_hue_difference>=6))
	{//last_cor06.02.17
		goto L;
	}
if(((mean_gr_distance<=possible_variation)&&((intersection1<=1)&&(indic_length>=2)
   &&(abs(intensity_red-intensity_red_new)<=2)&&(abs(intensity-intensity_new)<=2))||
   ((intersection1==0)&&(indic_length>=1)&&
   (abs(intensity-intensity_new)<=1)||(abs(intensity_red-intensity_red_new)<=1))))
{
number_of_extentions++;
                if((signif_balance>3)&&(signif_balance<12)&&((signif_balance1>3||
((density_ratio>=5)||((intersection1<=1)&&(density_ratio>=3)))&&(density_ratio<=9))))//last_cor08.09.14
				 {//sb
				 beg_int=std::min(beg_int,beg_int_new);
				 end_int=std::max(end_int,end_int_new);
				 if((right_hue_boundary>32)&&(new_right_hue_boundary<8))
				{
			  new_right_hue_boundary=new_right_hue_boundary+NUM_HUES;
				}

				 if((right_hue_boundary<8)&&(new_right_hue_boundary>32))
					{
					right_hue_boundary=right_hue_boundary+NUM_HUES;
					}
					if((left_hue_boundary>32)&&(new_left_hue_boundary<8))
				{
				new_left_hue_boundary=new_left_hue_boundary+NUM_HUES;
				}
				  if((left_hue_boundary<8)&&(new_left_hue_boundary>32))
				{
				left_hue_boundary=left_hue_boundary+NUM_HUES;
				}

				 left_hue_boundary=std::min(left_hue_boundary,new_left_hue_boundary);
				 right_hue_boundary=std::max(right_hue_boundary,new_right_hue_boundary);
				 }//sb
grain_significance+=new_impact_of_significance;
                 if((Bunch_hue<8)&&(Mean_hue_new>32))
				 {
				 Bunch_hue+=NUM_HUES;
				 }
				 if((Bunch_hue>32)&&(Mean_hue_new<8))
				 {
				 Mean_hue_new+=NUM_HUES;
				 }
				 if((signif_balance1>1)&&(density_ratio>2))
				 {//signif_dense_condlast_cor01.02.17
	Bunch_hue=((float)(Bunch_hue*(float)grain_significance+
	(float)Mean_hue_new*(float)new_impact_of_significance))/
	(float)(grain_significance+new_impact_of_significance);
	if(Bunch_hue>(NUM_HUES-1))
	{
		Bunch_hue=Bunch_hue-NUM_HUES;
	}
	Mean_hue_bunch=(int)Bunch_hue;
	                    if((Bunch_hue-Mean_hue_bunch)>0.5)
	                      {
	                      Mean_hue_bunch+=1;
	                      }
						if(Mean_hue_bunch>(NUM_HUES-1))
	{
		Mean_hue_bunch=Mean_hue_bunch-NUM_HUES;
	}
    Bunch_sat=(float)(Bunch_sat*grain_significance+
	Mean_sat_new*new_impact_of_significance)/
	(float)(grain_significance+new_impact_of_significance);
	Mean_sat_bunch=(int)Bunch_sat;
	/*if((Bunch_sat-Mean_sat_bunch)>0.5)
	{
	Mean_sat_bunch+=1;
	}*/
	if(Bunch_sat<2)
	{//last_cor17.12.17
	if((Bunch_sat-Mean_sat_bunch)>0.8)
	{
	Mean_sat_bunch+=1;
	}
	}
	else
	{
     if((Mean_sat_bunch==2)&&((Bunch_sat-Mean_sat_bunch)>0))
	 {
		 if((Bunch_sat-Mean_sat_bunch)>0.35)
		 {
         Mean_sat_bunch+=1;
		 }
	 }
	 if(Mean_sat_bunch>=3)
	 {
		 if((Bunch_sat-Mean_sat_bunch)>0.48)
	{
	Mean_sat_bunch+=1;
	}
	 }
	}//last_cor17.02.17
	Bunch_gray=((float)Mean_gray*(float)grain_significance+
	(float)(new_mean_grayscale*new_impact_of_significance))/
	(float)(grain_significance+new_impact_of_significance);
	Mean_gray=(int)Bunch_gray;
				 }//signif_dense_condlast_cor01.02.17
	if((Mean_hue_bunch>(NUM_HUES-1))&&(left_hue_boundary<8))
	 {
	 left_hue_boundary+=NUM_HUES;
	 }
	 if((Mean_hue_bunch>(NUM_HUES-1))&&(right_hue_boundary<8))
	 {
	 right_hue_boundary+=NUM_HUES;
	 }
	             if((signif_balance1>2)&&(density_ratio>2))
				 {//signif_dense_cond
	 left_sat_boun=std::min(left_sat_boun,new_left_sat_boun);
	 right_sat_boun=std::max(right_sat_boun,new_right_sat_boun);
	// minimum_grayscale=std::min(minimum_grayscale,new_minimum_grayscale);
	 //maximum_grayscale=std::max(maximum_grayscale,new_maximum_grayscale);16.06.14
	 minimum_grayscale=std::min(minimum_grayscale,new_minimum_grayscale);
	 maximum_grayscale=std::max(maximum_grayscale,new_maximum_grayscale);
	 intensity_interval_left=std::min(intensity_interval_left,intensity_red_new);
	 intensity_interval_right=std::max(intensity_interval_right,intensity_red_new);
				 }//signif_dense_cond
	if(!iter_step)
		  {//itst
	 ColorInform[new_number_of_new_intensity].NumberOfColorBunch[interval_new]=
	 NumberOfColoredIntervals;
	      }//itst
	      else
	      {
    ColorInform[new_number_of_new_intensity].NumberOfColorBunch[interval_new]=
	 NumberOfColoredIntervals-1;
	      }
	 total_number_new=*(inver_num+NUM_INTEN*interval_new+intensity_new);
*(members+*start_poin)=total_number_new;
(*start_poin)++;
}
		                        }
		                        else
		                           {//usual
if(small_interval==0)
{//big
	if((Mean_sat_bunch==1)&&(Mean_sat_new>=2)&&
   (Mean_hue_difference>=6))
	{//last_cor06.02.17
		goto L;
	}
if((Mean_sat_bunch>=2)&&(Mean_sat_new==1)&&
   (Mean_hue_difference>=6))
	{//last_cor06.02.17
		goto L;
	}
	if(((intersection1==0)&&(indic_length>=1))||((intersection1<=1)&&(indic_length>=2)))
	{//level0
		if((Mean_hue_difference<=1)||((Mean_hue_difference<=2)&&((intersection2<=3)||(intersection2==5)
			||(mean_gr_distance<=possible_variation/2)))||
			((Mean_hue_difference<=3)&&(node_vicinity)&&(node_vicinity_new)
			||((mean_gr_distance<=possible_variation/2)))||
			((Mean_hue_difference<=4)&&(node_vicinity)&&(node_vicinity_new)&&(intersection2<=1)))
		{//level1
			if(((saturation_distance1<=possible_sat_variation)&&(saturation_distance<=possible_sat_variation1))||
				((left_bound_sat<=Mean_sat_new)&&(Mean_sat_new<=right_bound_sat)))
			{//level2
if((mean_gr_distance<=possible_variation/2)||
   ((intersection3<=1)&&(mean_gr_distance<=possible_variation))||
   ((left_bound_gray<=new_mean_grayscale)&&(new_mean_grayscale<=right_bound_gray)&&(intersection3<=1)))
{//level3
number_of_extentions++;

}//level3
else
{
goto L;
}
			}//level2
			else
			{
            goto L;
			}
		}//level1
		else
		{
        goto L;
		}
	}//level0
else
{
goto L;
}
}//big
else
{//small
if(((intersection1==0)&&(indic_length>=1))||((intersection1<=1)&&(indic_length>=2)))
	{//level0
		if((Mean_hue_difference<=1)||((Mean_hue_difference<=2)&&((intersection2<=3)||(intersection2==5)))||
			((Mean_hue_difference<=3)&&(node_vicinity)&&(node_vicinity_new))||
			((Mean_hue_difference<=4)&&(node_vicinity)&&(node_vicinity_new)&&(intersection2<=1)))
		{//level1
			if(((saturation_distance1<=possible_sat_variation)&&(saturation_distance<=possible_sat_variation1))||
				((left_bound_sat<=Mean_sat_new)&&(Mean_sat_new<=right_bound_sat)))
			{//level2
if((mean_gr_distance<=(possible_variation)/2)||
   ((!intersection3)&&(mean_gr_distance<=possible_variation))||
   ((left_bound_gray<=new_mean_grayscale)&&(new_mean_grayscale<=right_bound_gray)&&(intersection3<=1)))
{//level3

number_of_extentions++;

}//level3
else
{
goto L;
}
			}//level2
			else
			{
            goto L;
			}
		}//level1
		else
		{
        goto L;
		}
	}//level0
else
{
goto L;
}
}//small
   /*if(((!intersection3)||(mean_gr_distance<=(dev_mean[possible_variation])/2))&&(((intersection1==0)&&((intersection2<=3)||
  ((Mean_hue_difference<=1)&&(saturation_distance<=3)&&(Mean_sat_bunch>=2)&&(intersection1<=1))||
  ((Mean_hue_difference<=3)&&(node_vicinity)&&(node_vicinity_new)&&(intersection1<=1))))||
  ((intersection1==1)&&(indic_length)&&((intersection2<=3)||
  ((Mean_hue_difference<=3)&&(node_vicinity)&&(node_vicinity_new)))&&
  (saturation_distance<3)&&(Mean_sat_bunch>=3)&&(Mean_sat_new>=3))||
  ((intersection1==0)&&((intersection2==5)||(intersection2==0)))||
  ((intersection1==1)&&(saturation_distance<3)&&
  ((intersection2<=2)||(intersection2==5)||((Mean_hue_difference<=2)&&
  (node_vicinity)&&(node_vicinity_new))))||
  ((intersection1==2)&&((intersection2<=1)||
  ((Mean_hue_difference<=2)&&(node_vicinity)&&(node_vicinity_new))
  &&(saturation_distance<3)&&(Mean_sat_bunch>=3)&&(Mean_sat_new>=3)))))*/
	//{//join


	/*length_hue=right_hue_boundary-left_hue_boundary;
	left_length_hue=left_hue_boundary-new_left_hue_boundary;
	right_length_hue=new_right_hue_boundary-right_hue_boundary;

	if((2*left_length_hue>length_hue)&& (length_hue>3)&&
	((2*new_impact_of_significance)<grain_significance))
	{
	goto K;
	}
	if((2*right_length_hue>length_hue)&& (length_hue>3)&&
	((2*new_impact_of_significance)<grain_significance))
	{
	goto K;
	} */

if(((density_ratio>=3)&&(density_ratio<=9)&&(intersection1<=1)&&(ratio_length>=12)&&(ratio_length1>=12))||
   ((signif_balance>3)&&(signif_balance1>3))||
					 ((signif_balance>=6)&&(signif_balance1>=2))
					 ||((signif_balance>3)&&((intersection1<=1))))
				   {//b0
				 beg_int=std::min(beg_int,beg_int_new);
				 end_int=std::max(end_int,end_int_new);


				 if(((new_impact_of_significance<<3)>grain_significance)&&
				 (Mean_hue_difference<=3))
			{//j1


			 if((right_hue_boundary>32)&&(new_right_hue_boundary<8))
				{
			  new_right_hue_boundary=new_right_hue_boundary+NUM_HUES;
				}

				 if((right_hue_boundary<8)&&(new_right_hue_boundary>32))
					{
					right_hue_boundary=right_hue_boundary+NUM_HUES;
					}
					if((left_hue_boundary>32)&&(new_left_hue_boundary<8))
				{
				new_left_hue_boundary=new_left_hue_boundary+NUM_HUES;
				}
				  if((left_hue_boundary<8)&&(new_left_hue_boundary>32))
				{
				left_hue_boundary=left_hue_boundary+NUM_HUES;
				}

				 left_hue_boundary=std::min(left_hue_boundary,new_left_hue_boundary);
				 right_hue_boundary=std::max(right_hue_boundary,new_right_hue_boundary);


		     }//j1
				 }//b0
//		K: ;
		 if((Bunch_hue<8)&&(Mean_hue_new>32))
				 {
				 Bunch_hue+=NUM_HUES;
				 }
				 if((Bunch_hue>32)&&(Mean_hue_new<5))
				 {
				 Mean_hue_new+=NUM_HUES;
				 }
				 //if((signif_balance1>3)&&(density_ratio>3))
				 //{//signif_dense_cond
	 Bunch_hue=((float)(Bunch_hue*(float)grain_significance+
	(float)Mean_hue_new*(float)new_impact_of_significance))/
	(float)(grain_significance+new_impact_of_significance);
	 if(Bunch_hue>(NUM_HUES-1))
	{
		Bunch_hue=Bunch_hue-NUM_HUES;
	}
	Mean_hue_bunch=(int)Bunch_hue;
	                    if((Bunch_hue-Mean_hue_bunch)>0.5)
	                      {
	                      Mean_hue_bunch+=1;
	                      }
						if(Mean_hue_bunch>(NUM_HUES-1))
	{
		Mean_hue_bunch=Mean_hue_bunch-NUM_HUES;
	}
	Bunch_sat=(Bunch_sat*(float)grain_significance+
	(float)Mean_sat_new*new_impact_of_significance)/
	(float)(grain_significance+new_impact_of_significance);
	Mean_sat_bunch=(int)Bunch_sat;
	if(Bunch_sat<2)
	{//last_cor17.12.17
	if((Bunch_sat-Mean_sat_bunch)>0.8)
	{
	Mean_sat_bunch+=1;
	}
	}
	else
	{
     if((Mean_sat_bunch==2)&&((Bunch_sat-Mean_sat_bunch)>0))
	 {
		 if((Bunch_sat-Mean_sat_bunch)>0.35)
		 {
         Mean_sat_bunch+=1;
		 }
	 }
	 if(Mean_sat_bunch>=3)
	 {
		 if((Bunch_sat-Mean_sat_bunch)>0.48)
	{
	Mean_sat_bunch+=1;
	}
	 }
	}//last_cor17.02.17
		  Bunch_gray=((float)Mean_gray*(float)grain_significance+
	(float)new_mean_grayscale*(float)new_impact_of_significance)/
	(float)(grain_significance+new_impact_of_significance);
	Mean_gray=(int)Bunch_gray;
	if((Bunch_gray-Mean_gray)>0.5)
	{
	Mean_gray+=1;
	}
	 if((Mean_hue_bunch>(NUM_HUES-1))&&(left_hue_boundary<8))
	 {
	 left_hue_boundary+=NUM_HUES;
	 }
	 if((Mean_hue_bunch>(NUM_HUES-1))&&(right_hue_boundary<8))
	 {
	 right_hue_boundary+=NUM_HUES;
	 }
	 if((signif_balance1>2)&&(density_ratio>2))
				 {//signif_dense_cond
	 left_sat_boun=std::min(left_sat_boun,new_left_sat_boun);
	 right_sat_boun=std::max(right_sat_boun,new_right_sat_boun);
	 //minimum_grayscale=std::min(minimum_grayscale,new_minimum_grayscale);
	 //maximum_grayscale=std::max(maximum_grayscale,new_maximum_grayscale);16.06.14
	 minimum_grayscale=std::min(minimum_grayscale,new_minimum_grayscale);
	 maximum_grayscale=std::max(maximum_grayscale,new_maximum_grayscale);
	 intensity_interval_left=std::min(intensity_interval_left,intensity_red_new);
	 intensity_interval_right=std::max(intensity_interval_right,intensity_red_new);
	              }//signif_dense_cond
	 grain_significance+=new_impact_of_significance;

	 if(!iter_step)
		  {//itst
	 ColorInform[new_number_of_new_intensity].NumberOfColorBunch[interval_new]=
	 NumberOfColoredIntervals;
	      }//itst
	      else
	      {
    ColorInform[new_number_of_new_intensity].NumberOfColorBunch[interval_new]=
	 NumberOfColoredIntervals-1;
	      }
	 total_number_new=*(inver_num+NUM_INTEN*interval_new+intensity_new);
*(members+*start_poin)=total_number_new;
(*start_poin)++;

	//}//join
	                               }//usual
 if((signif_balance1>6)&&(signif_balance>5)&&(mean_gr_distance<=possible_variation/2)&&(Mean_hue_difference<=1))
								   {
		if(((intersection1<=1)&&(indic_length<=1))&&(intersection2<=3)&&
		((abs(intensity_red-intensity_red_new)<2)||
		((abs(intensity_red-intensity_red_new)<=2)&&(signif_balance>=6))))
	{
	greater_found=1;
	}
			}
		                           }
		 }
	 L: ;
		 }

	 }

	 if(!number_of_extentions)
	 {
	 mean_signif=2*grain_significance/(end_int-beg_int+1);
	 if((mean_signif<=StripWidth/6))
	 {
    (*start_poin)--;
	 goto M;
	 }

	 }
	 if((number_of_extentions>0)||(!greater_found))
		 {
		  if(iter_step)
		  {
		  NumberOfColoredIntervals--;
		  ColoredIntervalsStructure->Cardinality[NumberOfColoredIntervals]=
		 number_of_extentions;

		  }
		  else
		  {
        ColorInform[new_number_of_intensity].NumberOfColorBunch[num_int]=
		 NumberOfColoredIntervals;
        ColoredIntervalsStructure->Cardinality[NumberOfColoredIntervals]=
		 number_of_extentions+1;
		  }
		 //*(continuation_investigated+intensity)=num_int+1;
		 ColoredIntervalsStructure->BegInterv[NumberOfColoredIntervals]=
		 beg_int;
		 ColoredIntervalsStructure->EndInterv[NumberOfColoredIntervals]=
		 end_int;
		 ColoredIntervalsStructure->GrainIntensity[NumberOfColoredIntervals]=
		 intensity;
		 ColoredIntervalsStructure->LowerIntensity[NumberOfColoredIntervals]=
		 intensity_interval_left;
		 ColoredIntervalsStructure->UpperIntensity[NumberOfColoredIntervals]=
		 intensity_interval_right;
		 ColoredIntervalsStructure->Significance[NumberOfColoredIntervals]=
		 grain_significance;
		 ColoredIntervalsStructure->LowerHueValue[NumberOfColoredIntervals]=
		 left_hue_boundary;
		 ColoredIntervalsStructure->UpperHueValue[NumberOfColoredIntervals]=
		 right_hue_boundary;
		 ColoredIntervalsStructure->LowerSatValue[NumberOfColoredIntervals]=
		 left_sat_boun;
		 ColoredIntervalsStructure->UpperSatValue[NumberOfColoredIntervals]=
		 right_sat_boun;
		 ColoredIntervalsStructure->AverageHue[NumberOfColoredIntervals]=
		 Mean_hue_bunch;
		 ColoredIntervalsStructure->AverageSat[NumberOfColoredIntervals]=
		 Mean_sat_bunch;
       ColoredIntervalsStructure->LowerGrayscale[NumberOfColoredIntervals]=
		 minimum_grayscale;
		 ColoredIntervalsStructure->UpperGrayscale[NumberOfColoredIntervals]=
		 maximum_grayscale;
		ColoredIntervalsStructure->AverageGray[NumberOfColoredIntervals]=
		 Mean_gray;
		 NumberOfColoredIntervals++;
		 if(greater_found)
		 {
		 prior=2;
		 }
		 else
		 {
		 prior=1;
		 }
		 }
  M:
  return(prior);
}
//-----------------------------------------------------------------------------
int

CColorIntervalSelect::ColorIntervalComparison(int left_boundary,
		int right_boundary, int left_boundary_new, int right_boundary_new)
{
	int max_left;
	int min_right;
	int indicator;
	int max_length;
	int min_length;
	int length_intersec;
	int first_left;
	int first_right;
	int second_left;
	int second_right;
	int number_of_extended;
	int direction_minimum;
	int shifted_first_left;
	int first_difference;
	int second_difference;
	int interval_distance;
	int enveloping_first;
	int enveloping_second;
	int enveloping_length;

	indicator=100;
	if((left_boundary<=right_boundary)&&(left_boundary_new<=right_boundary_new))
	{
	max_left=std::max(left_boundary,left_boundary_new);
	min_right=std::min(right_boundary,right_boundary_new);
	if(left_boundary<=left_boundary_new)
	{
	 number_of_extended=0;
	 first_left=left_boundary;
	 first_right=right_boundary;
	 second_left=left_boundary_new;
	 second_right=right_boundary_new;
	}
	else
	{
	number_of_extended=1;
	first_left=left_boundary_new;
	 first_right=right_boundary_new;
	 second_left=left_boundary;
	 second_right=right_boundary;
	}
	if((second_right-first_left)>=NUM_HUES)
	{
	min_right=second_right-NUM_HUES;
	max_left=first_left;
	}

	min_length=
	std::min(right_boundary-left_boundary+1,right_boundary_new-left_boundary_new+1);
	max_length=
	std::max(right_boundary-left_boundary+1,right_boundary_new-left_boundary_new+1);
	if(max_left>min_right)
				{
				enveloping_first=first_left;
				enveloping_second=second_right;
				shifted_first_left=first_left+NUM_HUES;
				first_difference=shifted_first_left-second_right;
				second_difference=second_left-first_right;
				  if(first_difference>=second_difference)
					{
					interval_distance=second_difference;
					direction_minimum=0;
					enveloping_length=enveloping_second-enveloping_first+1;
					}
						else
						{
						interval_distance=first_difference;
						 direction_minimum=1;
						 enveloping_length=first_right+NUM_HUES-second_left;
						}
							 if((interval_distance>=2*max_length)&&(enveloping_length>=8))
							  {
							  indicator=9;
							  return(indicator);
							  }
							  else
							  {
	 if((enveloping_length<=3)||((interval_distance+min_length)<=2))
	 {
	 indicator=5;
	 return(indicator);
	 }
							  }

	             indicator=8;
					 }

			if(min_right>=max_left)
			 {
	       enveloping_first=first_left;
			 enveloping_second=second_right;
			 enveloping_length=enveloping_second-enveloping_first+1;

			 length_intersec=min_right-max_left+1;
			  if(length_intersec==max_length)
			  {
				indicator=0;
			  }
				 else
				 {
				 if(length_intersec==min_length)
			  {
				indicator=1;
			  }
				  else
				  {
					 if(2*length_intersec>=max_length)
					 {
					  indicator=2;
					 }
					 else
					 {
					  if(2*length_intersec>=min_length)
					 {
					  indicator=3;
					 }
						  else
						 {
						 indicator=4;
						 }
					 }
				  }
				  if((enveloping_length<=4)&&(indicator>=3))
					 {
					 indicator=5;
					 return(indicator);
	 }
				 }
			 }
	}
	return(indicator);
	}
//----------------------------------------------------------------------------
int

CColorIntervalSelect::Intersection_measure(int beg_first, int end_first,
		int beg_second, int end_second, int* greater)
{
	int inter_beg,inter_end,max_len,min_len;
	int prior;
	int length_first;
	int length_second;
	int inter_length;

	length_first=end_first-beg_first+1;
	length_second=end_second-beg_second+1;
	inter_beg=std::max(beg_first,beg_second);
	inter_end=std::min(end_first,end_second);
	inter_length=inter_end-inter_beg+1;
	min_len=std::min(length_first,length_second);
	max_len=std::max(length_first,length_second);
	*greater=0;
		if(length_first>=2*length_second)
		{
		 *greater=3;
		}
					else
				  {
						 if(length_first>=length_second)
						  {
						  *greater=2;
						  }
							  else
								{
								  if(2*length_first>=length_second)
									{
									*greater=1;
									}
								}
				 }
	if(inter_beg>=inter_end)
	{
	prior=3;
	}
	else
	 {
	  if(((inter_length)*2)>(max_len))
	  {
	  prior=0;
	  }
	  else
	  {
	  if(((inter_length)*2)>(min_len))
	  {
	  prior=1;
	  }
		else
		{
		 prior=2;
		}
	  }
	 }
			return(prior);
	}
//----------------------------------------------------------------------------
int

CColorIntervalSelect::Intersection_measure_ratios(int beg_first, int end_first,
		int beg_second, int end_second, int* greater, int* inter_length_ratio,
		int* inter_length_ratio1)
{
	int inter_beg,inter_end,max_len,min_len;
	int prior;
	int length_first;
	int length_second;
	int inter_length;
	int length_ratio;
	int length_ratio1;


	length_first=end_first-beg_first+1;
	length_second=end_second-beg_second+1;
	inter_beg=std::max(beg_first,beg_second);
	inter_end=std::min(end_first,end_second);
	inter_length=inter_end-inter_beg+1;
	min_len=std::min(length_first,length_second);
	max_len=std::max(length_first,length_second);
	*greater=0;

		if(length_first>=2*length_second)
		{
		 *greater=3;
		}
					else
				  {
						 if(length_first>=length_second)
						  {
						  *greater=2;
						  }
							  else
								{
								  if(2*length_first>=length_second)
									{
									*greater=1;
									}
								}
				 }
	if(inter_beg>inter_end)
	{
	*inter_length_ratio=inter_beg-inter_end;
	*inter_length_ratio1=0;
	prior=3;
	}
	else
	 {
	length_ratio=(16*inter_length)/length_first;
	length_ratio1=(16*inter_length)/length_second;
	*inter_length_ratio=length_ratio;
	*inter_length_ratio1=length_ratio1;
	  if(((inter_length)*2)>(max_len))
	  {
	  prior=0;
	  }
	  else
	  {
	  if(((inter_length)*2)>(min_len))
	  {
	  prior=1;
	  }
		else
		{
		 prior=2;
		}
	  }
	 }
			return(prior);
	}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int

CColorIntervalSelect::GrayIntersection(int beg_first, int end_first,
		int beg_second, int end_second)
{
	int min_length;
	int ratio_length;
	int length_first;
	int length_second;
	int max_first;
	int min_second;
	int intersec_length;
	int prior;

	prior=2;
	length_first=end_first-beg_first+1;
	length_second=end_second-beg_second+1;
	min_length=std::min(length_first,length_second);
	max_first=std::max(beg_first,beg_second);
	min_second=std::min(end_first,end_second);

	if(max_first<=min_second)
	{//1
		intersec_length=min_second-max_first+1;
		ratio_length=(16*intersec_length)/length_second;
		if(ratio_length==16)
		{
			prior=0;

		}
		else
		{//2
			   if(ratio_length>=12)
			    {
	             prior=1;
			    }

		}//2

	}//1
	return(prior);
		/*if(end_first<beg_second)
		{
		 gap_length=beg_second-end_first+1;
		 if(gap_length>min_length/2)
		 {
		 prior=1;
		 return(prior);
		 }
		}
		else
		{
			if(end_second<beg_first)
			{
			gap_length=beg_first-end_second+1;
			  if(gap_length>min_length/2)
				{
				prior=1;
				return(prior);
				}
			 }
			else
			{
	max_first=std::max(beg_first,beg_second);
	min_second=std::min(end_first,end_second);
	intersec_length=min_second-max_first;
			}
		}*/
	//return(prior);
	}
//-----------------------------------------------------------------------------
void

CColorIntervalSelect::VisibilityFinding(int* visibility_measure,
		int* interv_num, int* interval_intens, int* inver_num, int beg_loop,
		int end_loop, int* painted_strip, int* painted_numbers)
{
	int count_scale;
	int current_inhabitant;
	int num_interval;
	//int new_number_of_intensity;
	int total_number;
	int count_int;
	int interval_length;
	int beg_int;
	int end_int;
	int num_intens;
	int num_interv;
	int intermed;


	for(count_scale=0;count_scale<PressedLength;count_scale++)
		 {
		 current_inhabitant=*(painted_strip+count_scale);
		  if(current_inhabitant)
			{
			current_inhabitant-=1;
			num_interval=*(painted_numbers+count_scale);
		  //new_number_of_intensity=intensities_with_colored_int[current_inhabitant];
	//if((ColorInform[new_number_of_intensity].TypeOfColor[num_interval]>=0)&&
	//(current_inhabitant))
	//if(current_inhabitant)
	//{
	total_number=*(inver_num+NUM_INTEN*num_interval+current_inhabitant);
	(*(visibility_measure+total_number))++;
	//}
			}
		 }
		for(count_int=beg_loop;count_int<end_loop;count_int++)
		 {
		 intermed=*(visibility_measure+count_int);
				if(intermed)
				 {
				 intermed=16*intermed;
		 num_intens=*(interval_intens+count_int);
		 num_interv=*(interv_num+count_int);
	beg_int=StripCurrent->IntAllInform[num_intens].BegInt[num_interv];
	end_int=StripCurrent->IntAllInform[num_intens].EndInt[num_interv];
	interval_length=end_int/2-beg_int/2+1;

		 *(visibility_measure+count_int)=intermed/(interval_length+1);
				 }
		 //intermed=0;
		 }
	}
//-----------------------------------------------------------------------------
void

CColorIntervalSelect::Hue_Painting(int beg_int, int end_int, int cluster_num,
		int* painted_hue_range)
{
	int count_hue;

	 for(count_hue=beg_int;count_hue<end_int;count_hue++)
		 {
		 *(painted_hue_range+count_hue)=cluster_num+1;
		 }
	 }
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void

CColorIntervalSelect::Colored_intervals_findingColorDivision(int NumInterestingIntensities,
int* OldNumbers,int* num_of_int,TIntCharact* IntAllInform)
{
int count_lev;
int count_interval;
int num_intens;
int num_intens_red;
int NumInt;
int color_deviation;
int color_deviation1;
int new_color_deviation;
int new_color_deviation1;
int Peak_point;
int First_outlet;
int Second_outlet;
int First_outlet1;
int Second_outlet1;
int LeftPeakBoundary;
int RightPeakBoundary;
int LeftPeakBoundary1;
int RightPeakBoundary1;
int Peak_point1;
int TheOtherRat;
int PeakHue;
int PeakCircleHue;
int PeakSaturation;
int PeakDoubleSaturation;
int PeakSide;
int PosCMHue;
int PosCMCircleHue;
int PosCMSaturation;
int PosCMDoubleSaturation;
int PosCMSide;
int PeakHue1;
int PeakCircleHue1;
int PeakSaturation1;
int PeakDoubleSaturation1;
int PeakSide1;
int NegCMHue;
int NegCMCircleHue;
int NegCMSaturation;
int NegCMDoubleSaturation;
int NegCMSide;
int ZeroHue;
int ZeroCircleHue;
int ZeroSaturation;
int ZeroDoubleSaturation;
int ZeroSide;
int LeftBounHue;
int LeftBounCircleHue;
int LeftBounSaturation;
int LeftBounDoubleSaturation;
int LeftBounSide;
int LeftBounHue1;
int LeftBounCircleHue1;
int LeftBounSaturation1;
int LeftBounDoubleSaturation1;
int LeftBounSide1;
int RightBounHue;
int RightBounCircleHue;
int RightBounSaturation;
int RightBounDoubleSaturation;
int RightBounSide;
int RightBounHue1;
int RightBounCircleHue1;
int RightBounSaturation1;
int RightBounDoubleSaturation1;
int RightBounSide1;
int zero_ratio;
int zero_ratio1;
int peak_fraction;
int peak_fraction1;
int neighb_pos_fraction;
int neighb_neg_fraction;
int indicat_zero;
int indicat_pos;
int indicat_neg;
int first_second_ratio;
int max_saturation;
//int max_left_saturation;
//int max_right_saturation;
int begin_int;
int end_int;
int interval_length;
//int coun_zero;
//int num_int_zero;
int mean_hue_deviation;
int mean_hue_deviation1;
int mean_hue_deviation2;
int min_hue_dev;
int max_hue_dev;
int* interval_intensity;
int* interval_number;
int* inverted_number;
int* interval_visibility;
int MInt;
int signif;
//int StrWidth;
//int range;
//int range_neg;
//int range_pos;
int Gray_mean;
int Gray_min;
int Gray_max;
int min_gr_to_mean;
int max_gr_to_mean;
int possible_variation;
//int dev_mean[8];
int num_scale_gr;
//int critical_sat;
float CM_point;
float CM_point1;
float mean_hue;
float mean_saturation;
int Hue_Dev;
int CM;
int CM1;
int CM2;
int dev1;
int dev2;
int color_range;
int left_color_division_boun;
int right_color_division_boun;
int Center_of_mass;
int Center_of_mass1;

left_color_division_boun=StripCurrent->left_bound_of_color_resolution;
right_color_division_boun=StripCurrent->right_bound_of_color_resolution;
 // LeftBounHue=-1;
  NumberOfIntervals=0;
  /*dev_mean[0]=5;
  dev_mean[1]=8;
  dev_mean[2]=8;
  dev_mean[3]=10;
  dev_mean[4]=10;
  dev_mean[5]=10;
  dev_mean[6]=12;
  dev_mean[7]=8;*/
  MInt=ColorInform[0].num_of_int;
  interval_intensity=new int[TotalNumInt];
  interval_number=new int[TotalNumInt];
  interval_visibility=new int[TotalNumInt];
  inverted_number=new int[NUM_INTEN*MInt];
  memset(interval_visibility,(int) '\0',sizeof(int)*(TotalNumInt));

for(count_lev=0;count_lev<NumInterestingIntensities;count_lev++)
 {//cycle0
	 color_range=-1;
  num_intens=OldNumbers[count_lev];
  if((num_intens>60)&&(num_intens<71))
  {
  num_intens_red=num_intens-39;
  color_range=0;
//G/(G+B), R/(R+B) the hue varies from blue to cyan range (22-31), only negative;
//G/(G+R), R/(R+B) the hue varies from blue to violet; range (22-31), zero and negative
  }
    else
    {//gen
       if((num_intens>70)&&(num_intens<81))
       {
       num_intens_red=num_intens-49;
	   color_range=1;
//G/(G+B), R/(R+B) the hue varies from violet to red; range (22-31), only positive;
//G/(G+R), R/(R+B) the hue varies from red to green-yellow; range (22-31), only positive
       }
       else
       {//gen1
	     if((num_intens>80)&&(num_intens<95))
          {
		   num_intens_red=num_intens-49;
		   color_range=2;
//G/(G+B), G/(G+R) the hue varies from red to orange; range (32-45), only negative;
//G/(G+R), G/(G+B) the hue varies from blue to cyan; range (32-45), negative and zero
	      }
	      else
	      {//gen2
			  if((num_intens>94)&&(num_intens<=108))
              {
		       num_intens_red=num_intens-63;
			   color_range=3;
//G/(G+B), G/(G+R) the hue varies from green-yellow to cyan, only positive; range (32-45)
//G/(G+R), G/(G+B) the hue varies from green to green-yellow; range (32-45), green, only positive
	           }
	           else
	           {//gen3
				   //range (46-60)
				   if((num_intens>108)&&(num_intens<=123))
				   {
                   num_intens_red=num_intens-63;
                   color_range=6;

//G/(G+B), G/(G+R) green-yellow, green-cyan, only positive; range (46-60)
//G/(G+R), G/(G+B) cyan, green-a bit yellow, only positive

				   }
				   else
				   {


//values were not changed
				num_intens_red=num_intens;
				if((num_intens>left_color_division_boun)&&(num_intens<=31))
{
	color_range=4;
//G/(G+B), G/(G+R) from blue to violet within the chosen range (22-31); negative and zero
//G/(G+R), G/(G+B) from red to violet, only positive
}
else
{//e
	if(num_intens<=left_color_division_boun)
{
	color_range=9;
//G/(G+B), G/(G+R) from blue-a bit cyan to red within the chosen range (0-21); negative, positive and zero
//G/(G+R), G/(G+B) from blue to orange within the chosen range (0-21);, negative, positive and zero
}
	if((num_intens>31)&&(num_intens<right_color_division_boun))
    {
	color_range=5;
//G/(G+B), G/(G+R) from orange to green-yellow within the chosen range(32-41); negative and zero
//G/(G+R), G/(G+B) from cyan to green , positive
    }
	if(num_intens>=right_color_division_boun)
	{
	color_range=7;
//G/(G+B), G/(G+R) from red to green-yellow within the chosen range(42-60); negative and zero
//G/(G+R), G/(G+B) from cyan to blue within the chosen range(42-60), zero and negative
	}
}//e
					   //}
				   }

//values without changes

			}//gen3
		  }//gen2
	   }//gen1
    }//gen


 NumInt=num_of_int[num_intens];

	for(count_interval=0;count_interval<NumInt;count_interval++)
	{//cycle1
	PeakHue=-1;
	PeakCircleHue=-1;
	PeakSaturation=-1;
	PeakSide=-1;
	Center_of_mass=-1;
	Center_of_mass1=-1;
	ZeroDoubleSaturation=-1;
	PeakDoubleSaturation=-1;
	PeakHue1=-1;
	PeakCircleHue1=-1;
	PeakSaturation1=-1;
	PeakSide1=-1;
	PosCMHue=-1;
	PosCMCircleHue=-1;
	PosCMSaturation=-1;
	NegCMSaturation=-1;
	NegCMCircleHue=-1;
	PosCMDoubleSaturation=-1;

	ZeroSaturation=-1;
	ZeroHue=-1;
	ZeroCircleHue=-1;
	LeftBounSaturation=-1;
    RightBounSaturation=-1;
	LeftBounSaturation1=-1;
    RightBounSaturation1=-1;
	ColorInform[count_lev].NumberOfColorBunch[count_interval]=-1;

	first_second_ratio=
	IntAllInform[num_intens].MainOpponentRatiosBalance[count_interval];
	  if(first_second_ratio!=-1)
	  {//cond firstsec
 indicat_zero=-1;
 indicat_pos=-1;
 indicat_neg=-1;

	Peak_point=IntAllInform[num_intens].ColorPeak[count_interval];
	Peak_point1=IntAllInform[num_intens].ColorPeak1[count_interval];
	zero_ratio=IntAllInform[num_intens].ZeroRatio[count_interval];
	zero_ratio1=IntAllInform[num_intens].ZeroRatio1[count_interval];
	peak_fraction=IntAllInform[num_intens].PeakFraction[count_interval];
	peak_fraction1=IntAllInform[num_intens].PeakFraction1[count_interval];
	First_outlet=IntAllInform[num_intens].FirstOutlet[count_interval];
	First_outlet1=IntAllInform[num_intens].FirstOutlet1[count_interval];
	Second_outlet=IntAllInform[num_intens].SecondOutlet[count_interval];
	Second_outlet1=IntAllInform[num_intens].SecondOutlet1[count_interval];
neighb_pos_fraction=IntAllInform[num_intens].NeighborPosFraction[count_interval];
neighb_neg_fraction=IntAllInform[num_intens].NeighborNegFraction[count_interval];
Gray_mean=IntAllInform[num_intens].GrayScaleMean[count_interval];
Gray_min=IntAllInform[num_intens].LowerGrayScale[count_interval];
Gray_max=IntAllInform[num_intens].UpperGrayScale[count_interval];
min_gr_to_mean=std::min(abs(Gray_mean-Gray_min),abs(Gray_mean-Gray_max));
max_gr_to_mean=std::max(abs(Gray_mean-Gray_min),abs(Gray_mean-Gray_max));
num_scale_gr=Gray_mean/8;
possible_variation=dev_mean[num_scale_gr];
begin_int=StripCurrent->IntAllInform[num_intens].BegInt[count_interval];
end_int=StripCurrent->IntAllInform[num_intens].EndInt[count_interval];
interval_length=end_int-begin_int+1;
signif=StripCurrent->IntAllInform[num_intens].Signif[count_interval];
signif=(signif<<1)/(interval_length);
//StrWidth=StripCurrent->StripWidth;
if((num_intens>=124)&&(num_intens<=135))
 {
 num_intens_red=IntAllInform[num_intens].MeanMRatio[count_interval];
 color_range=8;
 }

	                    if(zero_ratio>0)
	                    {//0zr
	           if(num_intens_red!=31)
	            {//1
	 TheOtherRat=31;
	     if(GGBorGGR)
		 {
	 HueSaturationSideCalculation(num_intens_red,TheOtherRat,Gray_mean,&ZeroHue,
	 &ZeroSaturation,&ZeroSide,&ZeroCircleHue,&ZeroDoubleSaturation);
		 }
		 else
		 {
HueSaturationSideCalculationGGR(num_intens_red,TheOtherRat,&ZeroHue,
	 &ZeroSaturation,&ZeroSide,&ZeroCircleHue,&ZeroDoubleSaturation);
		 }

	 indicat_zero=1;
	 if(num_intens_red>31)
	 {
	 ZeroHue=7;
	 ZeroSide=0;
	 ZeroCircleHue=7;
	 }
	 else
	 {
	 ZeroHue=31;
	 ZeroSide=1;
	 ZeroCircleHue=31;
	 }
	            }//1
	            else
	            {
	            ZeroHue=31;
	            ZeroCircleHue=31;
	            ZeroSaturation=0;
	            }
                        }//0zr
	if(neighb_pos_fraction>0)
	{//0pos
		if(Peak_point!=-1)
		{//1pos
		   color_deviation=(StripCurrent->invert_color_difference2[Peak_point]+
           StripCurrent->invert_color_difference1[Peak_point])/2;
           TheOtherRat=31+color_deviation;
            if(GGBorGGR)
		    {
HueSaturationSideCalculation(num_intens_red,TheOtherRat,Gray_mean,&PeakHue,
	&PeakSaturation,&PeakSide,&PeakCircleHue,&PeakDoubleSaturation);
			}
			else
			{
HueSaturationSideCalculationGGR(num_intens_red,TheOtherRat,&PeakHue,
	&PeakSaturation,&PeakSide,&PeakCircleHue,&PeakDoubleSaturation);
			}
		}//1pos
indicat_pos=1;
Center_of_mass=StripCurrent->IntAllInform[num_intens].CenterOfMass[count_interval];
            if(Center_of_mass>0)
			{//cm
CM_point=((float)Center_of_mass)/((float)16);
CM=(int)CM_point;

dev1=(StripCurrent->invert_color_difference2[CM+1]+
  StripCurrent->invert_color_difference1[CM+1])/2;
dev2=(StripCurrent->invert_color_difference2[CM]+
  StripCurrent->invert_color_difference1[CM])/2;
 CM_point=CM_point-(float)CM;
 CM_point=((float)(dev1-dev2))*CM_point+(float)dev2;
 new_color_deviation=(int)CM_point;
 if((CM_point-new_color_deviation)>0.5)
 {
 new_color_deviation+=1;
 }
 if(new_color_deviation!=color_deviation)
 {
 TheOtherRat=31+new_color_deviation;
           if(GGBorGGR)
		    {
	 HueSaturationSideCalculation(num_intens_red,TheOtherRat,Gray_mean,&PosCMHue,
 &PosCMSaturation,&PosCMSide,&PosCMCircleHue,&PosCMDoubleSaturation);
		   }
		   else
		   {
HueSaturationSideCalculationGGR(num_intens_red,TheOtherRat,&PosCMHue,
 &PosCMSaturation,&PosCMSide,&PosCMCircleHue,&PosCMDoubleSaturation);
		   }

 }
 else
 {
 PosCMHue=PeakHue;
 PosCMCircleHue=PeakCircleHue;
 PosCMSaturation=PeakSaturation;
 PosCMSide=PeakSide;
 }
			}//cm
			else
			{
			PosCMHue=-1;
			PosCMCircleHue=-1;
			PosCMSaturation=-1;
			PosCMSide=-1;
			}


                    if(First_outlet!=-1)
                     {//0fo
LeftPeakBoundary=First_outlet;
color_deviation=(StripCurrent->invert_color_difference2[First_outlet]+
  StripCurrent->invert_color_difference1[First_outlet])/2;
  TheOtherRat=31+color_deviation;
            if(GGBorGGR)
		    {
HueSaturationSideCalculation(num_intens_red,TheOtherRat,Gray_mean,&LeftBounHue,
&LeftBounSaturation,&LeftBounSide,&LeftBounCircleHue,&LeftBounDoubleSaturation);
			}
			else
			{
HueSaturationSideCalculationGGR(num_intens_red,TheOtherRat,&LeftBounHue,
&LeftBounSaturation,&LeftBounSide,&LeftBounCircleHue,&LeftBounDoubleSaturation);
			}


                      }//0fo
                    else
                    {
	LeftPeakBoundary=Peak_point;
	LeftBounHue=PeakHue;
	LeftBounCircleHue=PeakCircleHue;
	LeftBounSaturation=PeakSaturation;
	LeftBounSide=PeakSide;
	                 }

  if(Second_outlet<Peak_point)
  {
  Second_outlet=-1;
  }
                                     if(Second_outlet!=-1)
                                      {//sout
RightPeakBoundary=Second_outlet;
color_deviation=(StripCurrent->invert_color_difference2[Second_outlet]+
  StripCurrent->invert_color_difference1[Second_outlet])/2;
  TheOtherRat=31+color_deviation;
            if(GGBorGGR)
		    {
HueSaturationSideCalculation(num_intens_red,TheOtherRat,Gray_mean,&RightBounHue,
&RightBounSaturation,&RightBounSide,&RightBounCircleHue,&RightBounDoubleSaturation);
			}
			else
			{
HueSaturationSideCalculationGGR(num_intens_red,TheOtherRat,&RightBounHue,
&RightBounSaturation,&RightBounSide,&RightBounCircleHue,&RightBounDoubleSaturation);
			}

                                      }//sout
                                      else
                                      {
	RightPeakBoundary=Peak_point;
	RightBounHue=PeakHue;
	RightBounCircleHue=PeakCircleHue;
	RightBounSaturation=PeakSaturation;
	RightBounSide=PeakSide;
                                      }
		/* if(RightBounSaturation<PeakSaturation)
		  {
		  RightBounSaturation=PeakSaturation;
		  }*/
	}//0pos
						if(neighb_neg_fraction>0)
		                    {//0neg>0
                   if(Peak_point1!=-1)
				   {//1pp1
color_deviation1=(StripCurrent->invert_color_difference2[Peak_point1]+
  StripCurrent->invert_color_difference1[Peak_point1])/2;
  TheOtherRat=31-color_deviation1;
  if((color_range==8)&&(neighb_neg_fraction>=10)&&(num_intens_red==31))
    {//last_cor16.05.16
  num_intens_red=32;
    }//last_cor16.05.16
                 if(GGBorGGR)
		         {
	HueSaturationSideCalculation(num_intens_red,TheOtherRat,Gray_mean,&PeakHue1,
	&PeakSaturation1,&PeakSide1,&PeakCircleHue1,&PeakDoubleSaturation1);
				 }
				 else
				 {
HueSaturationSideCalculationGGR(num_intens_red,TheOtherRat,&PeakHue1,
	&PeakSaturation1,&PeakSide1,&PeakCircleHue1,&PeakDoubleSaturation1);
				 }
				   }//1pp1
	indicat_neg=1;
Center_of_mass1=StripCurrent->IntAllInform[num_intens].CenterOfMass1[count_interval];
                   if(Center_of_mass1>0)
				   {
CM_point1=((float)Center_of_mass1)/((float)16);

CM1=(int)CM_point1;
dev1=(StripCurrent->invert_color_difference2[CM1+1]+
  StripCurrent->invert_color_difference1[CM1+1])/2;
dev2=(StripCurrent->invert_color_difference2[CM1]+
  StripCurrent->invert_color_difference1[CM1])/2;
 CM_point1=CM_point1-(float)CM1;
 CM_point1=((float)(dev1-dev2))*CM_point1+(float)dev2;
 new_color_deviation1=(int)CM_point1;
 if((CM_point1-new_color_deviation1)>0.5)
 {
 new_color_deviation1+=1;
 }
 if(new_color_deviation1!=color_deviation1)
 {
 TheOtherRat=31-new_color_deviation1;
                 if(GGBorGGR)
		         {
 HueSaturationSideCalculation(num_intens_red,TheOtherRat,Gray_mean,&NegCMHue,
 &NegCMSaturation,&NegCMSide,&NegCMCircleHue,&NegCMDoubleSaturation);
				 }
				 else
				 {
 HueSaturationSideCalculationGGR(num_intens_red,TheOtherRat,&NegCMHue,
 &NegCMSaturation,&NegCMSide,&NegCMCircleHue,&NegCMDoubleSaturation);
				 }

 }
 else
 {
 NegCMHue=PeakHue1;
 NegCMCircleHue=PeakCircleHue1;
 NegCMSaturation=PeakSaturation1;
 NegCMSide=PeakSide1;
 }
				   }

				   if(First_outlet1!=-1)
                   {
RightPeakBoundary1=First_outlet1;
color_deviation1=(StripCurrent->invert_color_difference2[First_outlet1]+
  StripCurrent->invert_color_difference1[First_outlet1])/2;
  TheOtherRat=31-color_deviation1;
                 if(GGBorGGR)
		         {
HueSaturationSideCalculation(num_intens_red,TheOtherRat,Gray_mean,&RightBounHue1,
&RightBounSaturation1,&RightBounSide1,&RightBounCircleHue1,&RightBounDoubleSaturation1);
				 }
				 else
				 {
HueSaturationSideCalculationGGR(num_intens_red,TheOtherRat,&RightBounHue1,
&RightBounSaturation1,&RightBounSide1,&RightBounCircleHue1,&RightBounDoubleSaturation1);
				 }

                   }
                   else
                   {
	               RightPeakBoundary1=Peak_point1;
	               RightBounHue1=PeakHue1;
	               RightBounCircleHue1=PeakCircleHue1;
	               RightBounSaturation1=PeakSaturation1;
	               RightBounSide1=PeakSide1;
                   }
if(Second_outlet1<Peak_point1)
  {
  Second_outlet1=-1;
  }
                            if(Second_outlet1!=-1)
                              {//sout
LeftPeakBoundary1=Second_outlet1;
color_deviation1=(StripCurrent->invert_color_difference2[Second_outlet1]+
  StripCurrent->invert_color_difference1[Second_outlet1])/2;
  TheOtherRat=31-color_deviation1;
                if(GGBorGGR)
		         {
HueSaturationSideCalculation(num_intens_red,TheOtherRat,Gray_mean,&LeftBounHue1,
&LeftBounSaturation1,&LeftBounSide1,&LeftBounCircleHue1,&LeftBounDoubleSaturation1);
				}
				else
				{
HueSaturationSideCalculationGGR(num_intens_red,TheOtherRat,&LeftBounHue1,
&LeftBounSaturation1,&LeftBounSide1,&LeftBounCircleHue1,&LeftBounDoubleSaturation1);
				}

                              }//sout
                              else
                              {
	                          LeftPeakBoundary1=Peak_point1;
	                          LeftBounHue1=PeakHue1;
							  LeftBounCircleHue1=PeakCircleHue1;
	                          LeftBounSaturation1=PeakSaturation1;
	                          LeftBounSide1=PeakSide1;
                              }
		  /*if(LeftBounSaturation1<PeakSaturation1)
		  {
		  LeftBounSaturation1=PeakSaturation1;
		  }*/
							}//0neg>0
if(color_range>=0)
{//0cr
//short color ranges

		if(GGBorGGR)
		{//1GGB
			//if(num_intens_red>31)
	       // {//nir>31
				if((color_range==0)||(color_range==2))
				{//cr=2
//G/(G+B), R/(R+B) the hue varies from blue to cyan, only negative
//G/(G+B), G/(G+R) the hue varies from red to orange, only negative
CM=NegCMCircleHue;
CM1=NegCMSaturation;
   if(CM<0)
   {
ColorInform[count_lev].TypeOfColor[count_interval]=-1;
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
ColorInform[count_lev].LeftHueValue[count_interval]=-1;
ColorInform[count_lev].RightHueValue[count_interval]=-1;
ColorInform[count_lev].LowerSatValue[count_interval]=-1;
ColorInform[count_lev].UpperSatValue[count_interval]=-1;
ColorInform[count_lev].TotCMHue[count_interval]=-1;
ColorInform[count_lev].TotCMSat[count_interval]=-1;
ColorInform[count_lev].TriangleHue[count_interval]=-1;
goto L;
   }
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=PeakCircleHue1;
          if(PeakCircleHue1>=0)
		  {
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue1;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue1;
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
max_saturation=std::max(std::max(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
		  }
		  else
		  {
ColorInform[count_lev].LeftHueValue[count_interval]=CM;
ColorInform[count_lev].RightHueValue[count_interval]=CM;
ColorInform[count_lev].LowerSatValue[count_interval]=CM1;
max_saturation=CM1;
		  }
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
CM2=NegCMHue;
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
if(color_range==0)
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=40;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>6))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=41;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=42;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=43;
goto L;
			     }
				 if((num_intens>123)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=44;
goto L;
			     }
				 if (signif<StripWidth / 8)
				 {//last_cor17.11.17
				 ColorInform[count_lev].TypeOfColor[count_interval] = -40;
				 goto L;
				 }
				 else
				 {
				 ColorInform[count_lev].TypeOfColor[count_interval] = 45;
				 goto L;
				 }
			 }
}

if(color_range==2)
{
             if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=10;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=11;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=12;
goto L;
			     }
if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=13;
goto L;
			     }
if(signif<StripWidth/2)
{//last_cor30.11.15
ColorInform[count_lev].TypeOfColor[count_interval]=-10;
goto L;
}
else
{
	ColorInform[count_lev].TypeOfColor[count_interval]=14;
goto L;
}
			 }
}
				}//cr=2
				if((color_range==1)||(color_range==3)||(color_range==6))
				{//cr=1,3,6
//cr=1 G/(G+B), R/(R+B) the hue varies from violet to red, only positive; range (22-31)
//cr=3 G/(G+B), G/(G+R) the hue varies from yellow-green to cyan, only positive; range (32-45)
//cr=6 G/(G+B), G/(G+R) green-yellow, green-cyan, only positive; range (46-60)
CM=PosCMCircleHue;
CM1=PosCMSaturation;
         if(CM<0)
          {
ColorInform[count_lev].TypeOfColor[count_interval]=-1;
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
ColorInform[count_lev].LeftHueValue[count_interval]=-1;
ColorInform[count_lev].RightHueValue[count_interval]=-1;
ColorInform[count_lev].LowerSatValue[count_interval]=-1;
ColorInform[count_lev].UpperSatValue[count_interval]=-1;
ColorInform[count_lev].TotCMHue[count_interval]=-1;
ColorInform[count_lev].TotCMSat[count_interval]=-1;
ColorInform[count_lev].TriangleHue[count_interval]=-1;
goto L;
          }
ColorInform[count_lev].PosPeakHue[count_interval]=PeakCircleHue;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
       if(PeakCircleHue>=0)
	   {
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue;
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation,LeftBounSaturation),PeakSaturation);
max_saturation=std::max(std::max(RightBounSaturation,LeftBounSaturation),PeakSaturation);
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
	   }
	   else
	   {
	   ColorInform[count_lev].LeftHueValue[count_interval]=CM;
	   ColorInform[count_lev].RightHueValue[count_interval]=CM;
	   ColorInform[count_lev].LowerSatValue[count_interval]=CM1;
	   ColorInform[count_lev].UpperSatValue[count_interval]=CM1;
	   }
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
CM2=PosCMHue;
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
if(color_range==1)
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=60;
goto L;
			 }
			 else
			 {
	if(interval_length<=StripWidth)
	  {
				 if((CM1>=1)&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=61;
goto L;
			     }
				 if((signif>StripWidth/6)&&(min_gr_to_mean<(3*possible_variation)/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=62;
goto L;
			     }
	  }

				 if((CM1>=2)&&(signif>StripWidth/2)&&(min_gr_to_mean<(3*possible_variation)/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=61;
goto L;
			     }

				 if(((CM1>=1)&&(min_gr_to_mean<(3*possible_variation)/2))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=62;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=63;
goto L;
			     }
				 if((max_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=64;
goto L;
				 }
				 if (signif<StripWidth / 8)//last_cor07.11.17
				 {
				 ColorInform[count_lev].TypeOfColor[count_interval] = -60;
				 goto L;
				 }
				 else
				 {
				 ColorInform[count_lev].TypeOfColor[count_interval] = 60;
				 goto L;
				 }
			 }
}
if(color_range==3)
{
             if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=30;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=31;
goto L;
			     }
				 if(((CM1>=2)&&(Gray_mean>14))&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=32;
goto L;
			     }

				 if(((CM1>=1)&&(Gray_mean>20))&&(signif>StripWidth/2))
			     {//last_cor23.04.15
ColorInform[count_lev].TypeOfColor[count_interval]=33;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>14))&&(signif>StripWidth/4)&&(min_gr_to_mean<possible_variation))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=34;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<=possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=35;
goto L;
			     }
if((CM1>=0)&&(min_gr_to_mean<=possible_variation)&&(signif>=StripWidth/6))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=35;
goto L;
			     }

ColorInform[count_lev].TypeOfColor[count_interval]=-30;
goto L;
			 }
}
if(color_range==6)
{
             if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=25;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=26;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/4)&&(min_gr_to_mean<possible_variation))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=27;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>14))&&(signif>StripWidth/4)&&(min_gr_to_mean<possible_variation))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=27;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=28;
goto L;
			     }
ColorInform[count_lev].TypeOfColor[count_interval]=-25;
goto L;
			 }
}
//goto L;
				}//cr=1,3,6
				if((color_range==4)||(color_range==5)||(color_range==7))
				{//cr=4,5,7
////4 coordinates G/(G+B), R/(R+B) the hue varies from blue to violet G/(G+B) within the range (22,31), zero and negative
//5 coordinates G/(G+B), G/(G+R); G/(G+B)from orange to green-yellow within the chosen range(32-41), zero and negative
//7 coordinates G/(G+B), G/(G+R); G/(G+B)from red to orange within the chosen range(32-41), zero and negative
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=PeakCircleHue1;
					if((ZeroCircleHue>=0)&&(NegCMCircleHue>=0))
					{//zhneg5
mean_hue=
((float)(ZeroCircleHue*zero_ratio+NegCMCircleHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
mean_saturation=((float)(ZeroSaturation*zero_ratio+
 NegCMSaturation*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
CM=(int)mean_hue;
 if((mean_hue-CM)>0.5)
 {
 CM+=1;
 }

 CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
 ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue1;
ColorInform[count_lev].RightHueValue[count_interval]=ZeroCircleHue;
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(std::min(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1),ZeroSaturation);
max_saturation=std::max(std::max(std::max(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1),ZeroSaturation);
mean_hue=
((float)(ZeroHue*zero_ratio+NegCMHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
CM2=(int)mean_hue;
 if((mean_hue-CM2)>0.5)
 {
 CM2+=1;
 }
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
					}//zhneg5
					else
					{//e5
						if(zero_ratio>0)
						{//zr5
	if(ZeroCircleHue>=0)
	{
CM=ZeroCircleHue;
CM1=ZeroSaturation;
ColorInform[count_lev].LeftHueValue[count_interval]=ZeroCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=ZeroCircleHue+1;
max_saturation=ZeroSaturation;
ColorInform[count_lev].LowerSatValue[count_interval]=ZeroSaturation;
ColorInform[count_lev].TriangleHue[count_interval]=ZeroHue;

	}
	else
	{
ColorInform[count_lev].LeftHueValue[count_interval]=-1;
ColorInform[count_lev].RightHueValue[count_interval]=-1;
ColorInform[count_lev].TriangleHue[count_interval]=ZeroHue;
ColorInform[count_lev].UpperSatValue[count_interval]=ZeroSaturation;
ColorInform[count_lev].LowerSatValue[count_interval]=ZeroSaturation;
ColorInform[count_lev].TotCMHue[count_interval]=ZeroHue;
ColorInform[count_lev].TotCMSat[count_interval]=ZeroSaturation;
ColorInform[count_lev].TypeOfColor[count_interval]=-16;
goto L;
	}
						}//zr5
						else
						{
						CM=NegCMCircleHue;
						CM1=NegCMSaturation;
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue1;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue1;
max_saturation=std::max(std::max(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
ColorInform[count_lev].TriangleHue[count_interval]=NegCMHue;

						}
					}//e5
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
if(color_range==4)
{
if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=50;
goto L;
			 }
			 else
			 {
				 if((CM1>=2)&&(min_gr_to_mean<(3*possible_variation)/2)&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=51;
goto L;
			     }
				 if(((CM1>=1)&&(min_gr_to_mean<(3*possible_variation)/2))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=52;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/6))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=53;
goto L;
			     }
				 if((max_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=54;
goto L;
				 }
				 if((min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=55;
goto L;
				 }
				 if((signif<=2)&&(min_gr_to_mean<(3*possible_variation)/2))
				 {
					 ColorInform[count_lev].TypeOfColor[count_interval]=56;
                     goto L;
				 }
				 if (signif<StripWidth / 8)
				 {//last_cor17.11.17
				 ColorInform[count_lev].TypeOfColor[count_interval] = -50;
				 goto L;
				 }
				 else
				 {
				 ColorInform[count_lev].TypeOfColor[count_interval] = 57;
				 goto L;
				 }
			 }
}
if(color_range==5)
{
if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=15;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>20))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=16;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>40))&&(signif>StripWidth/8)&&(min_gr_to_mean<possible_variation))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=17;
goto L;
			     }
				 if(((CM1>=1)&&(((min_gr_to_mean<possible_variation))&&(signif>StripWidth/8))||(signif>StripWidth/2)))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=18;
goto L;
			     }
ColorInform[count_lev].TypeOfColor[count_interval]=-15;
goto L;
			 }
}
if(color_range==7)
{
if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=10;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=11;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/4)&&(min_gr_to_mean<possible_variation))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=12;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=13;
goto L;
			     }
				 if(signif<StripWidth/2)
				 {//last_cor30.11.15
				 	ColorInform[count_lev].TypeOfColor[count_interval]=-10;
				 	goto L;
				 }
				 else
				 {
				 	ColorInform[count_lev].TypeOfColor[count_interval]=14;
				 	goto L;
				 }
			 }
}


				}//cr=4,5,7

    if(color_range==8)
     {
if((neighb_pos_fraction)&&(!neighb_neg_fraction)&&(!zero_ratio))
{
CM=PosCMCircleHue;
CM1=PosCMSaturation;
         if(CM<0)
          {
ColorInform[count_lev].TypeOfColor[count_interval]=-1;
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
ColorInform[count_lev].LeftHueValue[count_interval]=-1;
ColorInform[count_lev].RightHueValue[count_interval]=-1;
ColorInform[count_lev].LowerSatValue[count_interval]=-1;
ColorInform[count_lev].UpperSatValue[count_interval]=-1;
ColorInform[count_lev].TotCMHue[count_interval]=-1;
ColorInform[count_lev].TotCMSat[count_interval]=-1;
ColorInform[count_lev].TriangleHue[count_interval]=-1;
goto L;
          }
ColorInform[count_lev].PosPeakHue[count_interval]=PeakCircleHue;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
       if(PeakCircleHue>=0)
	   {
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue;
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation,LeftBounSaturation),PeakSaturation);
max_saturation=std::max(std::max(RightBounSaturation,LeftBounSaturation),PeakSaturation);
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
	   }
	   else
	   {
	   ColorInform[count_lev].LeftHueValue[count_interval]=CM;
	   ColorInform[count_lev].RightHueValue[count_interval]=CM;
	   ColorInform[count_lev].LowerSatValue[count_interval]=CM1;
	   ColorInform[count_lev].UpperSatValue[count_interval]=CM1;
	   }
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
CM2=PosCMHue;
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
if(num_intens_red<32)
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=60;
goto L;
			 }
			 else
			 {
				 if((CM1>=2)&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=61;
goto L;
			     }
				 if((CM1>=1)&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=62;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/6))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=63;
goto L;
			     }
if(((max_gr_to_mean<possible_variation)||((min_gr_to_mean<possible_variation)&&((Gray_mean<10)||(Gray_mean>50))))&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=64;
goto L;
				 }
if((CM1<=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=64;
goto L;
                 }
if (signif<StripWidth / 8)
{//last_cor07.11.17
ColorInform[count_lev].TypeOfColor[count_interval] = -60;
goto L;
}
else
{
ColorInform[count_lev].TypeOfColor[count_interval] = 60;
goto L;
}
			 }
}
else
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=10;
goto L;
			 }
			 else
			 {
				 if((CM1>=2)&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=11;
goto L;
			     }
				 if((CM1>=1)&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=12;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/6))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=13;
goto L;
			     }
if(((max_gr_to_mean<possible_variation)||((min_gr_to_mean<possible_variation)&&((Gray_mean<10)||(Gray_mean>50))))&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=14;
goto L;
				 }
if((CM1<=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=14;
goto L;
                 }
if(signif<StripWidth/2)
{//last_cor30.11.15
	ColorInform[count_lev].TypeOfColor[count_interval]=-10;
	goto L;
}
else
{
	ColorInform[count_lev].TypeOfColor[count_interval]=14;
	goto L;
}
			 }
}

goto L;
     }
    else
    {
if((!neighb_pos_fraction)&&(neighb_neg_fraction)&&(!zero_ratio))
{
	CM=NegCMCircleHue;
CM1=NegCMSaturation;
   if(CM<0)
   {
ColorInform[count_lev].TypeOfColor[count_interval]=-1;
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
ColorInform[count_lev].LeftHueValue[count_interval]=-1;
ColorInform[count_lev].RightHueValue[count_interval]=-1;
ColorInform[count_lev].LowerSatValue[count_interval]=-1;
ColorInform[count_lev].UpperSatValue[count_interval]=-1;
ColorInform[count_lev].TotCMHue[count_interval]=-1;
ColorInform[count_lev].TotCMSat[count_interval]=-1;
ColorInform[count_lev].TriangleHue[count_interval]=-1;
goto L;
   }
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=PeakCircleHue1;
          if(PeakCircleHue1>=0)
		  {
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue1;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue1;
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
max_saturation=std::max(std::max(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
		  }
		  else
		  {
ColorInform[count_lev].LeftHueValue[count_interval]=CM;
ColorInform[count_lev].RightHueValue[count_interval]=CM;
ColorInform[count_lev].LowerSatValue[count_interval]=CM1;
max_saturation=CM1;
		  }
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
CM2=NegCMHue;
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
if(num_intens_red<32)
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=40;
goto L;
			 }
			 else
			 {
				 if((CM1>=2)&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=41;
goto L;
			     }
				 if((CM1>=1)&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=42;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/6))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=43;
goto L;
			     }
if(((max_gr_to_mean<possible_variation)||((min_gr_to_mean<possible_variation)&&((Gray_mean<10)||(Gray_mean>50))))&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=44;
goto L;
			     }
if((CM1<=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=44;
goto L;
                 }
if (signif<StripWidth / 8)
{//last_cor17.11.17
ColorInform[count_lev].TypeOfColor[count_interval] = -40;
goto L;
}
else
{
ColorInform[count_lev].TypeOfColor[count_interval] = 45;
goto L;
}
			 }
}
else
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=30;
goto L;
			 }
			 else
			 {
				 if((CM1>=2)&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=31;
goto L;
			     }
				 if((CM1>=1)&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=32;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/6))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=33;
goto L;
			     }
if(((max_gr_to_mean<possible_variation)||((min_gr_to_mean<possible_variation)&&((Gray_mean<10)||(Gray_mean>50))))&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=34;
goto L;
			     }
if((CM1<=1)&&(min_gr_to_mean<=possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=34;
goto L;
                 }
if((CM1>=0)&&(min_gr_to_mean<=possible_variation)&&(signif>=StripWidth/6))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=35;
goto L;
                 }
ColorInform[count_lev].TypeOfColor[count_interval]=-30;
goto L;
			 }
}

}
else
{
	if((!neighb_pos_fraction)&&(neighb_neg_fraction)&&(zero_ratio))
        {
			ColorInform[count_lev].PosPeakHue[count_interval]=-1;
			if((ZeroCircleHue>=0)&&(NegCMCircleHue>=0))
					{
mean_hue=
((float)(ZeroCircleHue*zero_ratio+NegCMCircleHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
mean_saturation=((float)(ZeroSaturation*zero_ratio+
 NegCMSaturation*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
CM=(int)mean_hue;
 if((mean_hue-CM)>0.5)
 {
 CM+=1;
 }

 CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
 mean_hue=
((float)(ZeroHue*zero_ratio+NegCMHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
CM2=(int)mean_hue;
 if((mean_hue-CM2)>0.5)
 {
 CM2+=1;
 }
			     }
			else
			{
				if((ZeroCircleHue<0)&&((NegCMCircleHue>=0)))
				{
CM=NegCMCircleHue;
mean_saturation=((float)(ZeroSaturation*zero_ratio+
 NegCMSaturation*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
CM2=NegCMHue;
				}
				else
				{
                CM=-1;
                CM1=0;
				CM2=-1;
				}
			}
		ColorInform[count_lev].NegPeakHue[count_interval]=PeakCircleHue1;
		ColorInform[count_lev].LeftHueValue[count_interval]=CM;
		ColorInform[count_lev].RightHueValue[count_interval]=CM;
		ColorInform[count_lev].LowerSatValue[count_interval]=CM1;
        ColorInform[count_lev].UpperSatValue[count_interval]=CM1;
		ColorInform[count_lev].TotCMHue[count_interval]=CM;
		ColorInform[count_lev].TotCMSat[count_interval]=CM1;
		ColorInform[count_lev].TriangleHue[count_interval]=CM2;
	    }
	else
	{
		if((neighb_pos_fraction)&&(!neighb_neg_fraction)&&(zero_ratio))
        {
		ColorInform[count_lev].NegPeakHue[count_interval]=-1;
		if((ZeroCircleHue>=0)&&(PosCMCircleHue>=0))
					{
mean_hue=
((float)(ZeroCircleHue*zero_ratio+PosCMCircleHue*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction));
mean_saturation=((float)(ZeroSaturation*zero_ratio+
 PosCMSaturation*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction));
CM=(int)mean_hue;
 if((mean_hue-CM)>0.5)
 {
 CM+=1;
 }

 CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
 mean_hue=
((float)(ZeroHue*zero_ratio+PosCMHue*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction));
CM2=(int)mean_hue;
 if((mean_hue-CM2)>0.5)
 {
 CM2+=1;
 }


		    }
			else
			{
				if((ZeroCircleHue<0)&&((PosCMCircleHue>=0)))
				{
CM=PosCMCircleHue;
mean_saturation=((float)(ZeroSaturation*zero_ratio+
 PosCMSaturation*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction));
CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
CM2=PosCMHue;
				}
				else
				{
                CM=-1;
                CM1=0;
				CM2=-1;
				}
			}
		ColorInform[count_lev].PosPeakHue[count_interval]=PeakCircleHue;
		ColorInform[count_lev].LeftHueValue[count_interval]=CM;
		ColorInform[count_lev].RightHueValue[count_interval]=CM;
		ColorInform[count_lev].LowerSatValue[count_interval]=CM1;
        ColorInform[count_lev].UpperSatValue[count_interval]=CM1;
		ColorInform[count_lev].TotCMHue[count_interval]=CM;
		ColorInform[count_lev].TotCMSat[count_interval]=CM1;
		ColorInform[count_lev].TriangleHue[count_interval]=CM2;
		}
		else
		{
			if((!neighb_pos_fraction)&&(!neighb_neg_fraction)&&(zero_ratio))
            {//7
				ColorInform[count_lev].NegPeakHue[count_interval]=-1;
				ColorInform[count_lev].PosPeakHue[count_interval]=-1;
				CM1=ZeroSaturation;
				CM=ZeroCircleHue;
				CM2=ZeroHue;
		ColorInform[count_lev].LeftHueValue[count_interval]=CM;
		ColorInform[count_lev].RightHueValue[count_interval]=CM;
		ColorInform[count_lev].LowerSatValue[count_interval]=CM1;
        ColorInform[count_lev].UpperSatValue[count_interval]=CM1;
		ColorInform[count_lev].TotCMHue[count_interval]=CM;
		ColorInform[count_lev].TotCMSat[count_interval]=CM1;
		ColorInform[count_lev].TriangleHue[count_interval]=CM2;
			}//7
			else
			{//8
				if((neighb_pos_fraction)&&(neighb_neg_fraction)&&(zero_ratio))
                {//3
ColorInform[count_lev].PosPeakHue[count_interval]=PeakCircleHue;
    ColorInform[count_lev].NegPeakHue[count_interval]=PeakCircleHue1;
mean_hue=
((float)(ZeroCircleHue*zero_ratio+PosCMCircleHue*neighb_pos_fraction+
NegCMCircleHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_pos_fraction+neighb_neg_fraction));
mean_saturation=((float)(ZeroSaturation*zero_ratio+NegCMSaturation*neighb_neg_fraction+
 PosCMSaturation*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction+neighb_neg_fraction));
CM=(int)mean_hue;
 if((mean_hue-CM)>0.5)
 {
 CM+=1;
 }
 Hue_Dev=std::min(abs(CM-PosCMCircleHue),abs(CM-NegCMCircleHue));
Hue_Dev=std::min(abs(CM-ZeroCircleHue),Hue_Dev);
 CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
 ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].LeftHueValue[count_interval]=CM;
ColorInform[count_lev].RightHueValue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
ColorInform[count_lev].LowerSatValue[count_interval]=CM1;
ColorInform[count_lev].UpperSatValue[count_interval]=CM1;
 mean_hue=
((float)(ZeroHue*zero_ratio+PosCMHue*neighb_pos_fraction+NegCMHue*neighb_neg_fraction))/
((float)(zero_ratio+neighb_pos_fraction+neighb_neg_fraction));
CM2=(int)mean_hue;
 if((mean_hue-CM2)>0.5)
 {
 CM2+=1;
 }
 ColorInform[count_lev].TriangleHue[count_interval]=CM2;
      if((Hue_Dev>4)&&((num_intens<133)||(min_gr_to_mean>possible_variation))&&(CM1>1))
	  {//4
    	  if(signif<StripWidth/4)
    	  		  {//last_cor25.12.15
    	  			  ColorInform[count_lev].TypeOfColor[count_interval]=-70;
    	  			  goto L;
    	  		  }
    	  		  else
    	  		  {
    	  			  ColorInform[count_lev].TypeOfColor[count_interval]=70;
    	  			  goto L;
    	  		  }//last_cor25.12.15
	  }//4
	           }//3
				else
				{//5
		if((neighb_pos_fraction)&&(neighb_neg_fraction)&&(!zero_ratio))
                {//9
ColorInform[count_lev].PosPeakHue[count_interval]=PeakCircleHue;
    ColorInform[count_lev].NegPeakHue[count_interval]=PeakCircleHue1;
mean_hue=
((float)(PosCMCircleHue*neighb_pos_fraction+
NegCMCircleHue*neighb_neg_fraction))/((float)(neighb_pos_fraction+neighb_neg_fraction));
mean_saturation=((float)(NegCMSaturation*neighb_neg_fraction+
 PosCMSaturation*neighb_pos_fraction))/((float)(neighb_pos_fraction+neighb_neg_fraction));
CM=(int)mean_hue;
 if((mean_hue-CM)>0.5)
 {
 CM+=1;
 }
 Hue_Dev=std::min(abs(CM-PosCMCircleHue),abs(CM-NegCMCircleHue));

 CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
 ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].LeftHueValue[count_interval]=CM;
ColorInform[count_lev].RightHueValue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
ColorInform[count_lev].LowerSatValue[count_interval]=CM1;
ColorInform[count_lev].UpperSatValue[count_interval]=CM1;
 mean_hue=
((float)(PosCMHue*neighb_pos_fraction+NegCMHue*neighb_neg_fraction))/
((float)(neighb_pos_fraction+neighb_neg_fraction));
CM2=(int)mean_hue;
 if((mean_hue-CM2)>0.5)
 {
 CM2+=1;
 }
 ColorInform[count_lev].TriangleHue[count_interval]=CM2;
      if((Hue_Dev>4)&&((num_intens<133)||(min_gr_to_mean>possible_variation))&&(CM1>1))
	  {//10
    	  if(signif<StripWidth/4)
    	  		  {//last_cor25.12.15
    	  			  ColorInform[count_lev].TypeOfColor[count_interval]=-70;
    	  			  goto L;
    	  		  }
    	  		  else
    	  		  {
    	  			  ColorInform[count_lev].TypeOfColor[count_interval]=70;
    	  			  goto L;
    	  		  }//last_cor25.12.15
	  }//10
	           }//9

				}//5
			}//8
		}
	}
	     if(num_intens_red<32)
		 {//1
if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=70;
goto L;
			 }
			 else
			 {//2
				 if((CM1>=2)&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=71;
goto L;
			     }
				 if((CM1>=1)&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=72;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/6))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=73;
goto L;
			     }
if(((max_gr_to_mean<possible_variation)||((min_gr_to_mean<possible_variation)&&((Gray_mean<10)||(Gray_mean>50))))&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=74;
goto L;
			     }
if((CM1<=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2)&&(num_intens>123))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=74;
goto L;
                 }
if(signif<StripWidth/4)
{//last_cor25.12.15
	ColorInform[count_lev].TypeOfColor[count_interval]=-70;
	goto L;
}
else
{
	ColorInform[count_lev].TypeOfColor[count_interval]=73;
	goto L;
}//last_cor25.12.15
			 }//2

		 }//1
		 else
		 {
			 if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=80;
goto L;
			 }
			 else
			 {
				 if((CM1>=2)&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=81;
goto L;
			     }
				 if((CM1>=1)&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=82;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/6))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=83;
goto L;
			     }
if(((max_gr_to_mean<possible_variation)||((min_gr_to_mean<possible_variation)&&((Gray_mean<10)||(Gray_mean>50))))&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=84;
goto L;
			     }
if((CM1<=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=84;
goto L;
                 }
if(signif<StripWidth/8)
{
ColorInform[count_lev].TypeOfColor[count_interval]=-80;
goto L;
}
else
{
ColorInform[count_lev].TypeOfColor[count_interval]=83;
goto L;
}
			 }

		 }
goto L;

}
    }
goto L;
	}
                   if(color_range==9)
				   {//cr=931
//coordinates G/(G+B), R/(R+G), G/(G+B) within (0-21)
//with the hue varying from blue and slightly cyan to red,
//may be with zero, negative, and positive parts

if(PosCMCircleHue<0)
{//pos=0
//hue varies from blue and slightly cyan to violet
					if((ZeroCircleHue>=0)&&(NegCMCircleHue>=0))
					{//zhn31
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=PeakCircleHue1;
mean_hue=
((float)(ZeroCircleHue*zero_ratio+NegCMCircleHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
mean_saturation=((float)(ZeroSaturation*zero_ratio+
 NegCMSaturation*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
CM=(int)mean_hue;
 if((mean_hue-CM)>0.5)
 {
 CM+=1;
 }

 CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
mean_hue_deviation=abs(CM-NegCMCircleHue);
//mean_hue_deviation1=abs(CM-PosCMCircleHue);
mean_hue_deviation2=abs(CM-ZeroCircleHue);
min_hue_dev=std::min(mean_hue_deviation,mean_hue_deviation2);
max_hue_dev=std::max(mean_hue_deviation,mean_hue_deviation2);
 ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue1;
ColorInform[count_lev].RightHueValue[count_interval]=ZeroCircleHue;
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(std::min(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1),ZeroSaturation);
max_saturation=std::max(std::max(std::max(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1),ZeroSaturation);
mean_hue=
((float)(ZeroHue*zero_ratio+NegCMHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
CM2=(int)mean_hue;
 if((mean_hue-CM2)>0.5)
 {
 CM2+=1;
 }
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
					}//zhn31
					else
					{//e431
						if(zero_ratio>0)
						{//zr431
	if(ZeroCircleHue>=0)
	{

CM=ZeroCircleHue;
CM1=ZeroSaturation;
ColorInform[count_lev].LeftHueValue[count_interval]=ZeroCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=ZeroCircleHue+1;
max_saturation=ZeroSaturation;
ColorInform[count_lev].LowerSatValue[count_interval]=ZeroSaturation;
ColorInform[count_lev].TriangleHue[count_interval]=ZeroHue;
	}
	else
	{
		ColorInform[count_lev].TypeOfColor[count_interval]=-51;
		ColorInform[count_lev].LeftHueValue[count_interval]=-1;
		ColorInform[count_lev].RightHueValue[count_interval]=-1;
		ColorInform[count_lev].LowerSatValue[count_interval]=0;
		ColorInform[count_lev].TriangleHue[count_interval]=-1;
		ColorInform[count_lev].UpperSatValue[count_interval]=0;
		ColorInform[count_lev].TotCMHue[count_interval]=-1;
ColorInform[count_lev].TotCMSat[count_interval]=0;
goto L;
	}
						}//zr431
						else
						{
						CM=NegCMCircleHue;
						CM1=NegCMSaturation;
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue1 ;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue1;
max_saturation=std::max(std::max(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
ColorInform[count_lev].TriangleHue[count_interval]=NegCMHue;

						}
					}//e431
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=56;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=57;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2)&&(min_gr_to_mean<possible_variation))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=58;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=59;
goto L;
			     }
				 if((max_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=59;
goto L;
				 }
				 if((min_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=59;
goto L;
				 }
ColorInform[count_lev].TypeOfColor[count_interval]=-55;
goto L;
			 }
}//pos=0
else
{//pos>0
	//hue varies from violet to red
	if(NegCMCircleHue<0)
	{//neg=0
ColorInform[count_lev].PosPeakHue[count_interval]=PeakCircleHue;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
					if((ZeroCircleHue>=0)&&(PosCMCircleHue>=0))
					{//zhpos4
mean_hue=
((float)(ZeroCircleHue*zero_ratio+PosCMCircleHue*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction));
mean_saturation=((float)(ZeroSaturation*zero_ratio+
 PosCMSaturation*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction));
CM=(int)mean_hue;
 if((mean_hue-CM)>0.5)
 {
 CM+=1;
 }

 CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
 ColorInform[count_lev].LeftHueValue[count_interval]=ZeroCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue;
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(std::min(RightBounSaturation,LeftBounSaturation),PeakSaturation),ZeroSaturation);
max_saturation=std::max(std::max(std::max(RightBounSaturation,LeftBounSaturation),PeakSaturation),ZeroSaturation);
mean_hue=
((float)(ZeroHue*zero_ratio+PosCMHue*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction));
CM2=(int)mean_hue;
 if((mean_hue-CM2)>0.5)
 {
 CM2+=1;
 }
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
					}//zhpos4
					else
					{//e4
						if(zero_ratio>0)
						{//zr4
	if(ZeroCircleHue>=0)
	{
CM=ZeroCircleHue;
CM1=ZeroSaturation;
ColorInform[count_lev].LeftHueValue[count_interval]=ZeroCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=ZeroCircleHue+1;
max_saturation=ZeroSaturation;
ColorInform[count_lev].LowerSatValue[count_interval]=ZeroSaturation;
ColorInform[count_lev].TriangleHue[count_interval]=ZeroHue;

	}
	else
	{
ColorInform[count_lev].LeftHueValue[count_interval]=-1;
ColorInform[count_lev].RightHueValue[count_interval]=-1;
ColorInform[count_lev].TriangleHue[count_interval]=ZeroHue;
ColorInform[count_lev].UpperSatValue[count_interval]=ZeroSaturation;
ColorInform[count_lev].LowerSatValue[count_interval]=ZeroSaturation;
ColorInform[count_lev].TotCMHue[count_interval]=ZeroHue;
ColorInform[count_lev].TotCMSat[count_interval]=ZeroSaturation;
ColorInform[count_lev].TypeOfColor[count_interval]=-16;
goto L;
	}
						}//zr4
						else
						{
						CM=PosCMCircleHue;
						CM1=PosCMSaturation;
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue;
max_saturation=std::max(std::max(RightBounSaturation,LeftBounSaturation),PeakSaturation);
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation,LeftBounSaturation),PeakSaturation);
ColorInform[count_lev].TriangleHue[count_interval]=PosCMHue;

						}
					}//e4
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=60;
goto L;
			 }
			 else
			 {
	if(interval_length<=StripWidth)
	  {
				 if((CM1>=1)&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=61;
goto L;
			     }
				 if((signif>StripWidth/6)&&(min_gr_to_mean<(3*possible_variation)/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=62;
goto L;
			     }
	  }
				 if(((CM1>=2)&&(min_gr_to_mean<(3*possible_variation)/2))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=61;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=62;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=63;
goto L;
			     }
				 if((max_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=64;
goto L;
				 }
				 if((min_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=65;
goto L;
				 }
				                                        if (signif<StripWidth / 8)
				 										{//last_cor07.11.17
				 											ColorInform[count_lev].TypeOfColor[count_interval] = -60;
				 											goto L;
				 										}
				 										else
				 										{
				 											ColorInform[count_lev].TypeOfColor[count_interval] = 60;
				 											goto L;
				 										}
			 }
	}//neg=0
	else
	{
		if(ZeroCircleHue>=0)
		{//general case
mean_hue=
((float)(ZeroCircleHue*zero_ratio+PosCMCircleHue*neighb_pos_fraction+
NegCMCircleHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_pos_fraction+neighb_neg_fraction));
mean_saturation=((float)(ZeroSaturation*zero_ratio+NegCMSaturation*neighb_neg_fraction+
 PosCMSaturation*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction+neighb_neg_fraction));
max_saturation=std::max(std::max(PeakSaturation,ZeroSaturation),PeakSaturation1);
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(ZeroSaturation,PeakSaturation),PeakSaturation1);
		}
		else
		{//particular case ZeroCircleHue=0
mean_hue=
((float)(PosCMCircleHue*neighb_pos_fraction+
NegCMCircleHue*neighb_neg_fraction))/((float)(neighb_pos_fraction+neighb_neg_fraction));
mean_saturation=((float)(NegCMSaturation*neighb_neg_fraction+
 PosCMSaturation*neighb_pos_fraction))/((float)(neighb_pos_fraction+neighb_neg_fraction));
max_saturation=std::max(PeakSaturation,PeakSaturation1);
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(PeakSaturation,PeakSaturation1);
		}

ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
CM=(int)mean_hue;
 if((mean_hue-CM)>0.5)
 {
 CM+=1;
 }
mean_hue_deviation=abs(CM-NegCMCircleHue);
mean_hue_deviation1=abs(CM-PosCMCircleHue);
 if(ZeroCircleHue>=0)
	 {
mean_hue_deviation2=abs(CM-ZeroCircleHue);
     }
     else
    {
mean_hue_deviation2=0;
    }
 CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
ColorInform[count_lev].PosPeakHue[count_interval]=PeakCircleHue;
ColorInform[count_lev].NegPeakHue[count_interval]=PeakCircleHue1;
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
if(ZeroCircleHue>=0)
	 {
mean_hue=
((float)(ZeroHue*zero_ratio+PosCMHue*neighb_pos_fraction+
NegCMHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_pos_fraction+neighb_neg_fraction));
     }
else
{
mean_hue=
((float)(PosCMHue*neighb_pos_fraction+
NegCMHue*neighb_neg_fraction))/((float)(neighb_pos_fraction+neighb_neg_fraction));
}
CM2=(int)mean_hue;
 if((mean_hue-CM2)>0.5)
 {
 CM2+=1;
 }
 ColorInform[count_lev].TriangleHue[count_interval]=CM2;
 if(mean_hue_deviation<=2)
 {
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue1;
ColorInform[count_lev].RightHueValue[count_interval]=std::max(CM,RightBounCircleHue1);
	         if(CM1>=3)
			 {
				 if(CM<31)
				 {
ColorInform[count_lev].TypeOfColor[count_interval]=45;
goto L;
				 }
				 else
				 {
					 if(CM>=39)
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=65;
goto L;
					 }
					 else
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=55;
goto L;
					 }
				 }

			 }
			 else
			 {//egensat
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
if(CM>=39)
				 {
ColorInform[count_lev].TypeOfColor[count_interval]=66;
goto L;
				 }
				 else
				 {
					 if(CM>=31)
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=56;
goto L;
					 }
					 else
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=46;
goto L;
					 }
				 }
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
if(CM>=39)
				 {
ColorInform[count_lev].TypeOfColor[count_interval]=67;
goto L;
				 }
				 else
				 {
					 if(CM>=31)
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=57;
goto L;
					 }
					 else
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=47;
goto L;
					 }
				 }
			     }
			 }
 }
 if(mean_hue_deviation1<=2)
 {
ColorInform[count_lev].LeftHueValue[count_interval]=std::min(CM,LeftBounCircleHue);
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue;
if(CM1>=3)
			 {

					 if(CM>=39)
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=65;
goto L;
					 }
					 else
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=5;
goto L;
					 }


			 }
			 else
			 {//egensat
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {

					 if(CM>=39)
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=66;
goto L;
					 }
					 else
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=56;
goto L;
					 }

			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {

					 if(CM>=39)
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=67;
goto L;
					 }
ColorInform[count_lev].TypeOfColor[count_interval]=57;
goto L;

			     }
			 }

 }
 ColorInform[count_lev].TypeOfColor[count_interval]=-57;
goto L;
	}
}//pos>0 general situation

//goto L;
				  }//cr=931


		}//1GGB
		else
		{//e1GGR
				if((color_range==1)||(color_range==3)||(color_range==4)||(color_range==5)||(color_range==6))
				{//1cr=1,3,4,5,6

//cr1 G/(G+R), R/(R+B) the hue varies from red to green-yellow; range (22-31), only positive
//cr3 G/(G+R), G/(G+B) the hue varies from green to green-yellow; range (32-45), only positive
//cr4 G/(G+R), R/(R+B) the hue varies from red to violet; range (22-31), only positive
//cr5 G/(G+R), G/(G+B) the hue varies from green to cyan; range (32-45), only positive
//cr6 G/(G+R), G/(G+B) the hue varies from green green-a bit yellow to cyan; range (46-60), only positive
CM=PosCMCircleHue;
CM1=PosCMSaturation;
         if(CM<0)
          {
ColorInform[count_lev].TypeOfColor[count_interval]=-1;
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
ColorInform[count_lev].LeftHueValue[count_interval]=-1;
ColorInform[count_lev].RightHueValue[count_interval]=-1;
ColorInform[count_lev].LowerSatValue[count_interval]=-1;
ColorInform[count_lev].UpperSatValue[count_interval]=-1;
ColorInform[count_lev].TotCMHue[count_interval]=-1;
ColorInform[count_lev].TotCMSat[count_interval]=-1;
ColorInform[count_lev].TriangleHue[count_interval]=-1;
goto L;
          }
ColorInform[count_lev].PosPeakHue[count_interval]=PeakCircleHue;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
       if(PeakCircleHue>=0)
	   {
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue;
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation,LeftBounSaturation),PeakSaturation);
max_saturation=std::max(std::max(RightBounSaturation,LeftBounSaturation),PeakSaturation);
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
	   }
	   else
	   {
	   ColorInform[count_lev].LeftHueValue[count_interval]=CM;
	   ColorInform[count_lev].RightHueValue[count_interval]=CM;
	   ColorInform[count_lev].LowerSatValue[count_interval]=CM1;
	   ColorInform[count_lev].UpperSatValue[count_interval]=CM1;
	   }
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
CM2=PosCMHue;
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
if(color_range==1)
{
if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=10;
goto L;
			 }
			 else
			 {//ek
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=11;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=12;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=13;
goto L;
			     }
				 if(signif<StripWidth/2)
				 				 {//last_cor30.11.15
				 					 ColorInform[count_lev].TypeOfColor[count_interval]=-10;
				 					 goto L;
				 				 }
				 				 else
				 				 {
				 					 ColorInform[count_lev].TypeOfColor[count_interval]=14;
				 					 goto L;
				 				 }
			 }//ek
}
if(color_range==3)
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=20;
goto L;
			 }
			 else
			 {//ek
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=21;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=22;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=23;
goto L;
			     }
ColorInform[count_lev].TypeOfColor[count_interval]=-20;
goto L;
			 }//ek
}
if(color_range==4)
{
             if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=60;
goto L;
			 }
			 else
			 {
				 if(interval_length<=StripWidth)
	  {
				 if((CM1>=1)&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=61;
goto L;
			     }
				 if((signif>StripWidth/6)&&(min_gr_to_mean<(3*possible_variation)/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=62;
goto L;
			     }
	  }
				 if(((CM1>=2)&&(min_gr_to_mean<(3*possible_variation)/2))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=61;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=62;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>14))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=62;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=63;
goto L;
			     }
				                                   if (signif<StripWidth / 8)
				 									{//last_cor07.11.17
				 										ColorInform[count_lev].TypeOfColor[count_interval] = -60;
				 										goto L;
				 									}
				 									else
				 									{
				 										ColorInform[count_lev].TypeOfColor[count_interval] = 60;
				 										goto L;
				 									}
			 }
}
if(color_range==5)
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=30;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=31;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=32;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/8))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=33;
goto L;
			     }
 if((CM1>=0)&&(min_gr_to_mean<=possible_variation)&&(signif>=StripWidth/6))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=35;
goto L;
                 }
ColorInform[count_lev].TypeOfColor[count_interval]=-30;
goto L;
			 }
}
if(color_range==6)
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=35;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=36;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=37;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=38;
goto L;
			     }
ColorInform[count_lev].TypeOfColor[count_interval]=-35;
goto L;
			 }
}
				}//1cr=1,3,4,5,6

if((color_range==0)||(color_range==2)||(color_range==7))
{//1cr=0,2,7
//cr0 G/(G+R), R/(R+B) the hue varies from blue to violet; range (22-31), zero and negative
//cr2 G/(G+R), G/(G+B) the hue varies from blue to cyan; range (32-45), zero and negative
//cr7 G/(G+R), G/(G+B) from cyan to blue; within the chosen range(42-60), zero and negative

ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=PeakCircleHue1;
					if((ZeroCircleHue>=0)&&(NegCMCircleHue>=0))
					{
mean_hue=
((float)(ZeroCircleHue*zero_ratio+NegCMCircleHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
mean_saturation=((float)(ZeroSaturation*zero_ratio+
 NegCMSaturation*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
CM=(int)mean_hue;
 if((mean_hue-CM)>0.5)
 {
 CM+=1;
 }

 CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
 if((color_range==2)||(color_range==7))
 {
 ColorInform[count_lev].LeftHueValue[count_interval]=ZeroCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue1;
 }
 else
 {
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue1;
ColorInform[count_lev].RightHueValue[count_interval]=ZeroCircleHue;
 }
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(std::min(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1),ZeroSaturation);
max_saturation=std::max(std::max(std::max(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1),ZeroSaturation);
mean_hue=
((float)(ZeroHue*zero_ratio+NegCMHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
CM2=(int)mean_hue;
 if((mean_hue-CM2)>0.5)
 {
 CM2+=1;
 }
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
					}
					else
					{
						if(zero_ratio>0)
						{
CM=ZeroCircleHue;
CM1=ZeroSaturation;
ColorInform[count_lev].LeftHueValue[count_interval]=ZeroCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=ZeroCircleHue+1;
max_saturation=ZeroSaturation;
ColorInform[count_lev].LowerSatValue[count_interval]=ZeroSaturation;
ColorInform[count_lev].TriangleHue[count_interval]=ZeroHue;
                        }
						else
						{
						CM=NegCMCircleHue;
						CM1=NegCMSaturation;
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue1;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue1;
max_saturation=std::max(std::max(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
ColorInform[count_lev].TriangleHue[count_interval]=NegCMHue;

						}
					}
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
if(color_range==0)
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=50;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(min_gr_to_mean<(3*possible_variation)/2))&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=51;
goto L;
			     }
				 if(((CM1>=1)&&(min_gr_to_mean<(3*possible_variation)/2))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=52;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=53;
goto L;
			     }
				 if((signif<=2)&&(min_gr_to_mean<(3*possible_variation)/2))
				 {
				 ColorInform[count_lev].TypeOfColor[count_interval]=56;
                 goto L;
				 }
				                                    if (signif<StripWidth / 8)
				 									{//last_cor17.11.17
				 										ColorInform[count_lev].TypeOfColor[count_interval] = -50;
				 										goto L;
				 									}
				 									else
				 									{
				 										ColorInform[count_lev].TypeOfColor[count_interval] = 57;
				 										goto L;
				 									}
			 }
}
if(color_range==2)
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=40;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>6))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=41;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=42;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=43;
goto L;
			     }
				 if((num_intens>123)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=44;
goto L;
			     }
				                                    if (signif<StripWidth / 8)
				 									{//last_cor17.11.17
				 										ColorInform[count_lev].TypeOfColor[count_interval] = -40;
				 										goto L;
				 									}
				 									else
				 									{
				 										ColorInform[count_lev].TypeOfColor[count_interval] = 45;
				 										goto L;
				 									}
			 }
}
if(color_range==7)
{
             if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=45;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=46;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=47;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=48;
goto L;
			     }
				 if((num_intens>123)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=49;
goto L;
			     }
ColorInform[count_lev].TypeOfColor[count_interval]=-41;
goto L;
			 }
}
}//1cr=0,2,7
if(color_range==8)
     {//cr=8
if((neighb_pos_fraction)&&(!neighb_neg_fraction)&&(!zero_ratio))
{
CM=PosCMCircleHue;
CM1=PosCMSaturation;
         if(CM<0)
          {
ColorInform[count_lev].TypeOfColor[count_interval]=-1;
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
ColorInform[count_lev].LeftHueValue[count_interval]=-1;
ColorInform[count_lev].RightHueValue[count_interval]=-1;
ColorInform[count_lev].LowerSatValue[count_interval]=-1;
ColorInform[count_lev].UpperSatValue[count_interval]=-1;
ColorInform[count_lev].TotCMHue[count_interval]=-1;
ColorInform[count_lev].TotCMSat[count_interval]=-1;
ColorInform[count_lev].TriangleHue[count_interval]=-1;
goto L;
          }
ColorInform[count_lev].PosPeakHue[count_interval]=PeakCircleHue;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
       if(PeakCircleHue>=0)
	   {
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue;
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation,LeftBounSaturation),PeakSaturation);
max_saturation=std::max(std::max(RightBounSaturation,LeftBounSaturation),PeakSaturation);
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
	   }
	   else
	   {
	   ColorInform[count_lev].LeftHueValue[count_interval]=CM;
	   ColorInform[count_lev].RightHueValue[count_interval]=CM;
	   ColorInform[count_lev].LowerSatValue[count_interval]=CM1;
	   ColorInform[count_lev].UpperSatValue[count_interval]=CM1;
	   }
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
CM2=PosCMHue;
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
if(num_intens_red<32)
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=65;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=66;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=67;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4)&&(Gray_mean>9))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=68;
goto L;
			     }
				 if((max_gr_to_mean<possible_variation)&&(signif>=StripWidth/4)&&(Gray_mean>9))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=69;
goto L;
				 }
ColorInform[count_lev].TypeOfColor[count_interval]=-65;
goto L;
			 }
}
else
{
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=30;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=31;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=32;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<=possible_variation)&&(signif>=StripWidth/4)&&(Gray_mean>9))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=33;
goto L;
			     }
				 if((max_gr_to_mean<=possible_variation)&&(signif>=StripWidth/4)&&(Gray_mean>9))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=34;
goto L;
				 }
 if((CM1>=0)&&(min_gr_to_mean<=possible_variation)&&(signif>=StripWidth/6))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=35;
goto L;
                 }
                                     if (signif<StripWidth / 8)
 										{//last_cor30.05.18
 											ColorInform[count_lev].TypeOfColor[count_interval] = -30;
 																					goto L;
 										}
 										else
 										{
 										ColorInform[count_lev].TypeOfColor[count_interval] = 37;
 										goto L;
 										}//last_cor30.05.18
			 }
}
//mean_hue=0;
//((float)(ZeroHue*zero_ratio+NegCMHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
//mean_saturation=(float)PosCMSaturation;
//((float)(ZeroSaturation*zero_ratio+
// NegCMSaturation*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
goto L;
     }
    else
    {//2
if((!neighb_pos_fraction)&&(neighb_neg_fraction)&&(!zero_ratio))
{//only neg
	CM=NegCMCircleHue;
CM1=NegCMSaturation;
   if(CM<0)
   {
ColorInform[count_lev].TypeOfColor[count_interval]=-1;
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
ColorInform[count_lev].LeftHueValue[count_interval]=-1;
ColorInform[count_lev].RightHueValue[count_interval]=-1;
ColorInform[count_lev].LowerSatValue[count_interval]=-1;
ColorInform[count_lev].UpperSatValue[count_interval]=-1;
ColorInform[count_lev].TotCMHue[count_interval]=-1;
ColorInform[count_lev].TotCMSat[count_interval]=-1;
ColorInform[count_lev].TriangleHue[count_interval]=-1;
goto L;
   }
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=PeakCircleHue1;
          if(PeakCircleHue1>=0)
		  {
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue1;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue1;
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
max_saturation=std::max(std::max(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
		  }
		  else
		  {
ColorInform[count_lev].LeftHueValue[count_interval]=CM;
ColorInform[count_lev].RightHueValue[count_interval]=CM;
ColorInform[count_lev].LowerSatValue[count_interval]=CM1;
max_saturation=CM1;
		  }
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
CM2=NegCMHue;
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
if(num_intens_red<32)
{//<32
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=50;
goto L;
			 }
			 else
			 {//3
				 if(((CM1>=2)&&(min_gr_to_mean<(3*possible_variation)/2))&&(signif>StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=51;
goto L;
			     }
				 if(((CM1>=1)&&(min_gr_to_mean<(3*possible_variation)/2))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=52;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=53;
goto L;
			     }
				 if((num_intens>123)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=54;
goto L;
			     }
				 if((signif<=2)&&(min_gr_to_mean<(3*possible_variation)/2))
				 {
					 ColorInform[count_lev].TypeOfColor[count_interval]=56;
                     goto L;
				 }
				                                            if (signif<StripWidth / 8)
				 											{//last_cor17.11.17
				 												ColorInform[count_lev].TypeOfColor[count_interval] = -50;
				 												goto L;
				 											}
				 											else
				 											{
				 												ColorInform[count_lev].TypeOfColor[count_interval] = 57;
				 												goto L;
				 											}
			 }//3
}//<32
else
{//1
	if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=40;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>6))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=41;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=42;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=43;
goto L;
			     }
				 if((num_intens>123)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=44;
goto L;
			     }
				                                           if (signif<StripWidth / 8)
				 											{//last_cor17.11.17
				 												ColorInform[count_lev].TypeOfColor[count_interval] = -40;
				 												goto L;
				 											}
				 											else
				 											{
				 												ColorInform[count_lev].TypeOfColor[count_interval] = 45;
				 												goto L;
				 											}
			 }
}//1
//mean_hue=24;
}//only neg
else
{
	//ColorInform[count_lev].TypeOfColor[count_interval]=-1;
    ColorInform[count_lev].PosPeakHue[count_interval]=-1;
    ColorInform[count_lev].NegPeakHue[count_interval]=-1;
    ColorInform[count_lev].LeftHueValue[count_interval]=-1;
    ColorInform[count_lev].RightHueValue[count_interval]=-1;
    ColorInform[count_lev].LowerSatValue[count_interval]=0;
    ColorInform[count_lev].UpperSatValue[count_interval]=0;
    ColorInform[count_lev].TotCMHue[count_interval]=-1;
    ColorInform[count_lev].TotCMSat[count_interval]=0;
    ColorInform[count_lev].TriangleHue[count_interval]=-1;
	     if(num_intens_red<32)
		 {
	if((num_intens>123)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=74;
goto L;
			     }
	if(signif<StripWidth/8)
	{//last_cor25.12.15
ColorInform[count_lev].TypeOfColor[count_interval]=-70;
goto L;
	}
	else
	{
ColorInform[count_lev].TypeOfColor[count_interval]=73;
goto L;
	}//last_cor25.12.15
		 }
		 else
		 {
		 if((num_intens>123)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=84;
goto L;
			     }
		 if(signif<=StripWidth/8)
		 {
ColorInform[count_lev].TypeOfColor[count_interval]=-80;
goto L;
		 }
		 else
		 {
ColorInform[count_lev].TypeOfColor[count_interval]=84;
goto L;
		 }
		 }
goto L;
      /*if((!neighb_pos_fraction)&&(!neighb_neg_fraction)&&(zero_ratio))
        {
        mean_hue=24;
        }
       if((neighb_pos_fraction)&&(neighb_neg_fraction)&&(!zero_ratio))
        {
        mean_hue=24;
        }
        if((!neighb_pos_fraction)&&(neighb_neg_fraction)&&(!zero_ratio))
         {
         mean_hue=24;
         }
        if((neighb_pos_fraction)&&(neighb_neg_fraction)&&(zero_ratio))
         {
         mean_hue=24;
         }*/
}
    }
goto L;
	}
	if(color_range==9)
				   {//cr=9
//coordinates G/(G+R), R/(R+G), G/(G+B) within (0-21)
//with the hue varying from blue and slightly cyan to red,
//may be with zero, negative, and positive parts

if(PosCMCircleHue<0)
{//pos=0
//hue varies from blue to violet
					if((ZeroCircleHue>=0)&&(NegCMCircleHue>=0))
					{//zhn31
ColorInform[count_lev].PosPeakHue[count_interval]=-1;
ColorInform[count_lev].NegPeakHue[count_interval]=PeakCircleHue1;
mean_hue=
((float)(ZeroCircleHue*zero_ratio+NegCMCircleHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
mean_saturation=((float)(ZeroSaturation*zero_ratio+
 NegCMSaturation*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
CM=(int)mean_hue;
 if((mean_hue-CM)>0.5)
 {
 CM+=1;
 }

 CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
mean_hue_deviation=abs(CM-NegCMCircleHue);
//mean_hue_deviation1=abs(CM-PosCMCircleHue);
mean_hue_deviation2=abs(CM-ZeroCircleHue);
min_hue_dev=std::min(mean_hue_deviation,mean_hue_deviation2);
max_hue_dev=std::max(mean_hue_deviation,mean_hue_deviation2);
 ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue1;
ColorInform[count_lev].RightHueValue[count_interval]=ZeroCircleHue;
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(std::min(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1),ZeroSaturation);
max_saturation=std::max(std::max(std::max(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1),ZeroSaturation);
mean_hue=
((float)(ZeroHue*zero_ratio+NegCMHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_neg_fraction));
CM2=(int)mean_hue;
 if((mean_hue-CM2)>0.5)
 {
 CM2+=1;
 }
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
					}//zhn31
					else
					{//e431
						if(zero_ratio>0)
						{//zr431
	if(ZeroCircleHue>=0)
	{

CM=ZeroCircleHue;
CM1=ZeroSaturation;
ColorInform[count_lev].LeftHueValue[count_interval]=ZeroCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=ZeroCircleHue+1;
max_saturation=ZeroSaturation;
ColorInform[count_lev].LowerSatValue[count_interval]=ZeroSaturation;
ColorInform[count_lev].TriangleHue[count_interval]=ZeroHue;
	}
	else
	{
		ColorInform[count_lev].TypeOfColor[count_interval]=-51;
		ColorInform[count_lev].LeftHueValue[count_interval]=-1;
		ColorInform[count_lev].RightHueValue[count_interval]=-1;
		ColorInform[count_lev].LowerSatValue[count_interval]=0;
		ColorInform[count_lev].TriangleHue[count_interval]=-1;
		ColorInform[count_lev].UpperSatValue[count_interval]=0;
		ColorInform[count_lev].TotCMHue[count_interval]=-1;
ColorInform[count_lev].TotCMSat[count_interval]=0;
goto L;
	}
						}//zr431
						else
						{
						CM=NegCMCircleHue;
						CM1=NegCMSaturation;
ColorInform[count_lev].LeftHueValue[count_interval]=RightBounCircleHue1;
ColorInform[count_lev].RightHueValue[count_interval]=LeftBounCircleHue1;
max_saturation=std::max(std::max(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation1,LeftBounSaturation1),PeakSaturation1);
ColorInform[count_lev].TriangleHue[count_interval]=NegCMHue;

						}
					}//e431
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=56;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=57;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=58;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=59;
goto L;
			     }
				 if((max_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=59;
goto L;
				 }
				 if((min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=59;
goto L;
				 }
				 if(signif<StripWidth/4)
				 				 {//last_cor25.12.15
				 					 ColorInform[count_lev].TypeOfColor[count_interval]=-55;
				 					 goto L;
				 				 }
				 				 else
				 				 {
				 					 ColorInform[count_lev].TypeOfColor[count_interval]=50;
				 					 goto L;
				 				 }//last_cor25.12.15
			 }
}//pos=0
else
{//pos>0
	//hue varies from violet to red
	if(NegCMCircleHue<0)
	{//neg=0
ColorInform[count_lev].PosPeakHue[count_interval]=PeakCircleHue;
ColorInform[count_lev].NegPeakHue[count_interval]=-1;
					if((ZeroCircleHue>=0)&&(PosCMCircleHue>=0))
					{//zhpos4
mean_hue=
((float)(ZeroCircleHue*zero_ratio+PosCMCircleHue*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction));
mean_saturation=((float)(ZeroSaturation*zero_ratio+
 PosCMSaturation*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction));
CM=(int)mean_hue;
 if((mean_hue-CM)>0.5)
 {
 CM+=1;
 }

 CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
 ColorInform[count_lev].LeftHueValue[count_interval]=ZeroCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue;
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(std::min(RightBounSaturation,LeftBounSaturation),PeakSaturation),ZeroSaturation);
max_saturation=std::max(std::max(std::max(RightBounSaturation,LeftBounSaturation),PeakSaturation),ZeroSaturation);
mean_hue=
((float)(ZeroHue*zero_ratio+PosCMHue*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction));
CM2=(int)mean_hue;
 if((mean_hue-CM2)>0.5)
 {
 CM2+=1;
 }
ColorInform[count_lev].TriangleHue[count_interval]=CM2;
					}//zhpos4
					else
					{//e4
						if(zero_ratio>0)
						{//zr4
	if(ZeroCircleHue>=0)
	{
CM=ZeroCircleHue;
CM1=ZeroSaturation;
ColorInform[count_lev].LeftHueValue[count_interval]=ZeroCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=ZeroCircleHue+1;
max_saturation=ZeroSaturation;
ColorInform[count_lev].LowerSatValue[count_interval]=ZeroSaturation;
ColorInform[count_lev].TriangleHue[count_interval]=ZeroHue;

	}
	else
	{
ColorInform[count_lev].LeftHueValue[count_interval]=-1;
ColorInform[count_lev].RightHueValue[count_interval]=-1;
ColorInform[count_lev].TriangleHue[count_interval]=ZeroHue;
ColorInform[count_lev].UpperSatValue[count_interval]=ZeroSaturation;
ColorInform[count_lev].LowerSatValue[count_interval]=ZeroSaturation;
ColorInform[count_lev].TotCMHue[count_interval]=ZeroHue;
ColorInform[count_lev].TotCMSat[count_interval]=ZeroSaturation;
ColorInform[count_lev].TypeOfColor[count_interval]=-16;
goto L;
	}
						}//zr4
						else
						{
						CM=PosCMCircleHue;
						CM1=PosCMSaturation;
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue;
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue;
max_saturation=std::max(std::max(RightBounSaturation,LeftBounSaturation),PeakSaturation);
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(RightBounSaturation,LeftBounSaturation),PeakSaturation);
ColorInform[count_lev].TriangleHue[count_interval]=PosCMHue;

						}
					}//e4
ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
if(CM1>=3)
			 {
ColorInform[count_lev].TypeOfColor[count_interval]=60;
goto L;
			 }
			 else
			 {
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=61;
goto L;
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=62;
goto L;
			     }
				 if((CM1>=1)&&(min_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=63;
goto L;
			     }
				 if((max_gr_to_mean<possible_variation)&&(signif>=StripWidth/4))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=64;
goto L;
				 }
				 if((min_gr_to_mean<possible_variation)&&(signif>=StripWidth/2))
			     {
ColorInform[count_lev].TypeOfColor[count_interval]=65;
goto L;
				 }
				 if(signif<StripWidth/4)
				 				 {//last_cor25.12.15
				 					 ColorInform[count_lev].TypeOfColor[count_interval]=-60;
				 					 goto L;
				 				 }
				 				 else
				 				 {
				 					 ColorInform[count_lev].TypeOfColor[count_interval]=65;
				 					 goto L;
				 				 }//last_cor25.12.15
			 }
	}//neg=0
	else
	{
		if(ZeroCircleHue>=0)
		{//general case
mean_hue=
((float)(ZeroCircleHue*zero_ratio+PosCMCircleHue*neighb_pos_fraction+
NegCMCircleHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_pos_fraction+neighb_neg_fraction));
mean_saturation=((float)(ZeroSaturation*zero_ratio+NegCMSaturation*neighb_neg_fraction+
 PosCMSaturation*neighb_pos_fraction))/((float)(zero_ratio+neighb_pos_fraction));
max_saturation=std::max(std::max(PeakSaturation,ZeroSaturation),PeakSaturation1);
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(std::min(ZeroSaturation,PeakSaturation),PeakSaturation1);
		}
		else
		{//particular case ZeroCircleHue=0
mean_hue=
((float)(PosCMCircleHue*neighb_pos_fraction+
NegCMCircleHue*neighb_neg_fraction))/((float)(neighb_pos_fraction+neighb_neg_fraction));
mean_saturation=((float)(NegCMSaturation*neighb_neg_fraction+
 PosCMSaturation*neighb_pos_fraction))/((float)(neighb_pos_fraction+neighb_neg_fraction));
max_saturation=std::max(PeakSaturation,PeakSaturation1);
ColorInform[count_lev].LowerSatValue[count_interval]=
std::min(PeakSaturation,PeakSaturation1);
		}

ColorInform[count_lev].UpperSatValue[count_interval]=max_saturation;
CM=(int)mean_hue;
 if((mean_hue-CM)>0.5)
 {
 CM+=1;
 }
mean_hue_deviation=abs(CM-NegCMCircleHue);
mean_hue_deviation1=abs(CM-PosCMCircleHue);
 if(ZeroCircleHue>=0)
	 {
mean_hue_deviation2=abs(CM-ZeroCircleHue);
     }
     else
    {
mean_hue_deviation2=0;
    }
 CM1=(int)mean_saturation;
 if((mean_saturation-CM1)>0.5)
 {
 CM1+=1;
 }
ColorInform[count_lev].PosPeakHue[count_interval]=PeakCircleHue;
ColorInform[count_lev].NegPeakHue[count_interval]=PeakCircleHue1;
ColorInform[count_lev].TotCMHue[count_interval]=CM;
ColorInform[count_lev].TotCMSat[count_interval]=CM1;
if(ZeroCircleHue>=0)
	 {
mean_hue=
((float)(ZeroHue*zero_ratio+PosCMHue*neighb_pos_fraction+
NegCMHue*neighb_neg_fraction))/((float)(zero_ratio+neighb_pos_fraction+neighb_neg_fraction));
     }
else
{
mean_hue=
((float)(PosCMHue*neighb_pos_fraction+
NegCMHue*neighb_neg_fraction))/((float)(neighb_pos_fraction+neighb_neg_fraction));
}
CM2=(int)mean_hue;
 if((mean_hue-CM2)>0.5)
 {
 CM2+=1;
 }
 ColorInform[count_lev].TriangleHue[count_interval]=CM2;
 if(mean_hue_deviation<=2)
 {
ColorInform[count_lev].LeftHueValue[count_interval]=LeftBounCircleHue1;
ColorInform[count_lev].RightHueValue[count_interval]=std::max(CM,RightBounCircleHue1);
	         if(CM1>=3)
			 {
				 if(CM<31)
				 {
ColorInform[count_lev].TypeOfColor[count_interval]=45;
goto L;
				 }
				 else
				 {
					 if(CM>=39)
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=65;
goto L;
					 }
					 else
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=55;
goto L;
					 }
				 }

			 }
			 else
			 {//egensat
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {
if(CM>=39)
				 {
ColorInform[count_lev].TypeOfColor[count_interval]=66;
goto L;
				 }
				 else
				 {
					 if(CM>=31)
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=56;
goto L;
					 }
					 else
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=46;
goto L;
					 }
				 }
			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {
if(CM>=39)
				 {
ColorInform[count_lev].TypeOfColor[count_interval]=67;
goto L;
				 }
				 else
				 {
					 if(CM>=31)
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=57;
goto L;
					 }
					 else
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=47;
goto L;
					 }
				 }
			     }
			 }
 }
 else
 {
 if(mean_hue_deviation1<=2)
 {
ColorInform[count_lev].LeftHueValue[count_interval]=std::min(CM,LeftBounCircleHue);
ColorInform[count_lev].RightHueValue[count_interval]=RightBounCircleHue;
if(CM1>=3)
			 {

					 if(CM>=39)
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=65;
goto L;
					 }
					 else
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=5;
goto L;
					 }


			 }
			 else
			 {//egensat
				 if(((CM1>=2)&&(Gray_mean>10))&&(signif>StripWidth/2))
			     {

					 if(CM>=39)
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=66;
goto L;
					 }
					 else
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=56;
goto L;
					 }

			     }
				 if(((CM1>=1)&&(Gray_mean>30))&&(signif>StripWidth/2))
			     {

					 if(CM>=39)
					 {
ColorInform[count_lev].TypeOfColor[count_interval]=67;
goto L;
					 }
ColorInform[count_lev].TypeOfColor[count_interval]=57;
goto L;

			     }
			 }

 }
 else
 {
	 if(signif<StripWidth/4)
		 {//last_cor25.12.15
			 ColorInform[count_lev].TypeOfColor[count_interval]=-57;
			 goto L;
		 }
		 else
		 {
			 ColorInform[count_lev].TypeOfColor[count_interval]=57;
			 goto L;
		 }//last_cor25.12.15
 }
 }
	}
}//pos>0 general situation

//goto L;
				  }//cr=931
		} //else_inversion of ratios
}//0cr
L:
if((ColorInform[count_lev].TypeOfColor[count_interval]>=0)&&
(ColorInform[count_lev].TypeOfColor[count_interval]<100))
{
*(interval_intensity+NumberOfIntervals)=num_intens;
*(interval_number+NumberOfIntervals)=count_interval;
*(inverted_number+NUM_INTEN*count_interval+num_intens)=NumberOfIntervals;
NumberOfIntervals++;
//we take into account the intervals that have one color

Strip_value_painting_saturation(num_intens,begin_int,end_int,count_interval,
signif,painted_strip_colored,painted_numbers_colored,consistency_colored);
}
}//cond firstsec
else
{
ColorInform[count_lev].NumberOfColorBunch[count_interval]=-1;
ColorInform[count_lev].TypeOfColor[count_interval]=-100;
}
	 }//cycle1

	/*else
	{
	num_int_zero=num_of_int[num_intens];
 for(coun_zero=0;coun_zero<num_int_zero;coun_zero++)
 {
 ColorInform[count_lev].NumberOfColorBunch[coun_zero]=-1;
 }

	}*/
  }//cycle0

 VisibilityFinding(interval_visibility,interval_number,interval_intensity,
 inverted_number,0,NumberOfIntervals,painted_strip_colored,
 painted_numbers_colored);
 AnalysisColoredRanges(interval_visibility,inverted_number,
  interval_number,interval_intensity);


  delete[] interval_intensity;
  delete[] interval_number;
  delete[] inverted_number;
  delete[] interval_visibility;
}
//-------------------
int

CColorIntervalSelect::Additional_Junction(int bunch, int intensity,
		int interval)
{
	int bunch_mean_hue;
	int corrected_bunch_mean_hue;
	int bunch_minimal_hue;
	int bunch_maximal_hue;
	int corrected_bunch_minimal_hue;
	int corrected_bunch_maximal_hue;
	int bunch_mean_saturation;
	int bunch_maximal_saturation;
	int bunch_minimal_saturation;
	int bunch_mean_gray;
	int bunch_minimum_gray;
	int bunch_maximum_gray;
	int prior;
	int hue_difference;
	int saturation_difference;
	int gray_difference;
	int beg_int;
	int end_int;
	int int_length;
	int length_ratio;
	int Intersection;
	int indic_length;
	int inclusion_relation;
	int bunch_significance;
	int Signif_ratio;
	int bunch_density;
	int Density_ratio;
	int possible_gray_variation;
	int possible_gray_variation0;
	int possible_sat_variation;
	int possible_sat_variation1;
	int saturation_distance;
	int saturation_distance1;
	int ratio_length;
	int ratio_length1;
	int hue_bound;
	int zone_hue;
	int zone_hue_bunch;
	int zone_gray;
	int zone_gray_bunch;
	int min_gray_dist;
	int left_gray;
	int right_gray;
	int min_sat_dist;
	int left_sat;
	int right_sat;
	int left_hue;
	int right_hue;
	int min_hue_dist;
	int hue_close;
	int saturation_close;
	int grayscale_close;
	int intens;
	int beg_intens_int;
	int end_intens_int;
	int seed_length;
	int seed_sat;
	int seed_mean_hue;
	int corrected_seed_mean_hue;
	int seed_gray;
	int seed_significance;
	int seed_density;
	int impossible_gray_variation;

	inclusion_relation=0;
	indic_length=-1;
	ratio_length=-1;
	ratio_length1=-1;
	prior=0;
	hue_close=0;
	saturation_close=0;
	grayscale_close=0;

	beg_int=ColoredIntervalsStructure->BegInterv[bunch];
	end_int=ColoredIntervalsStructure->EndInterv[bunch];
	bunch_significance=ColoredIntervalsStructure->Significance[bunch];

	int_length=end_int-beg_int+1;
	bunch_density=(2*bunch_significance)/int_length;
	intens=OldNumbers[intensity];
	beg_intens_int=StripCurrent->IntAllInform[intens].BegInt[interval];
	end_intens_int=StripCurrent->IntAllInform[intens].EndInt[interval];
	bunch_mean_hue=ColoredIntervalsStructure->AverageHue[bunch];
	corrected_bunch_mean_hue=bunch_mean_hue;
	bunch_minimal_hue=ColoredIntervalsStructure->LowerHueValue[bunch];
	corrected_bunch_minimal_hue=bunch_minimal_hue;
	bunch_maximal_hue=ColoredIntervalsStructure->UpperHueValue[bunch];
	corrected_bunch_maximal_hue=bunch_maximal_hue;
	bunch_mean_saturation=ColoredIntervalsStructure->AverageSat[bunch];
	if(bunch_mean_hue>=NUM_HUES)
	{
		bunch_mean_hue=bunch_mean_hue-NUM_HUES;
	}
	zone_hue_bunch=hue_zones[bunch_mean_hue];
	bunch_mean_gray=ColoredIntervalsStructure->AverageGray[bunch];
	zone_gray_bunch=gray_zones[bunch_mean_gray];

	seed_length=end_intens_int-beg_intens_int+1;
	if((beg_int<=beg_intens_int)&&(end_int>=end_intens_int))
	{
	inclusion_relation=1;
	}

	Intersection=Intersection_measure_ratios(beg_int,end_int,beg_intens_int,
		end_intens_int,&indic_length,&ratio_length,&ratio_length1);
	if(Intersection==3)
	{
		ratio_length=0;
	}
		//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
		//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
	seed_sat=ColorInform[intensity].TotCMSat[interval];
	seed_mean_hue=ColorInform[intensity].TotCMHue[interval];
	if(seed_mean_hue>=NUM_HUES)
	{
		seed_mean_hue=seed_mean_hue-NUM_HUES;
	}
	corrected_seed_mean_hue=seed_mean_hue;

	zone_hue=hue_zones[seed_mean_hue];
	seed_gray=StripCurrent->IntAllInform[intens].GrayScaleMean[interval];
	zone_gray=gray_zones[seed_gray];
	seed_significance=StripCurrent->IntAllInform[intens].Signif[interval];
	seed_density=(seed_significance<<1)/seed_length;
	Signif_ratio=(16*seed_significance)/(bunch_significance+seed_significance);
	Density_ratio=(16*seed_density)/(bunch_density+seed_density);
	length_ratio=(16*seed_length)/(int_length+seed_length);

	bunch_minimal_saturation=ColoredIntervalsStructure->LowerSatValue[bunch];
	bunch_maximal_saturation=ColoredIntervalsStructure->UpperSatValue[bunch];
	min_sat_dist=std::min(bunch_maximal_saturation-bunch_mean_saturation,
					 bunch_mean_saturation-bunch_minimal_saturation);
	left_sat=bunch_mean_saturation-min_sat_dist;
	right_sat=min_sat_dist+bunch_mean_saturation;
	possible_sat_variation=dev_mean_sat[bunch_mean_saturation];
	possible_sat_variation1=dev_mean_sat1[bunch_mean_saturation];

	if(bunch_mean_saturation>=seed_sat)
		{
		saturation_distance=bunch_mean_saturation-seed_sat;
		saturation_distance1=0;
		}
		else
		{
		saturation_distance1=seed_sat-bunch_mean_saturation;
		saturation_distance=0;
		}


	bunch_minimum_gray=ColoredIntervalsStructure->LowerGrayscale[bunch];
	bunch_maximum_gray=ColoredIntervalsStructure->UpperGrayscale[bunch];
	min_gray_dist=std::min(bunch_maximum_gray-bunch_mean_gray,bunch_mean_gray-bunch_minimum_gray);
	left_gray=bunch_mean_gray-min_gray_dist;
	right_gray=min_gray_dist+bunch_mean_gray;

	possible_gray_variation=dev_mean[bunch_mean_gray/8];
	impossible_gray_variation=imp_dev_mean[bunch_mean_gray/8];
	possible_gray_variation0=possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2;

	if((seed_mean_hue<10)&&(seed_mean_hue>=0)&&(bunch_mean_hue>39))
	{
	corrected_seed_mean_hue=seed_mean_hue+NUM_HUES;
	}

	if((seed_mean_hue>39)&&(bunch_mean_hue<10)&&
	(bunch_mean_hue>=0))
	{
	corrected_bunch_mean_hue=bunch_mean_hue+NUM_HUES;
	  if(bunch_minimal_hue<10)
	  {
	  corrected_bunch_minimal_hue=bunch_minimal_hue+NUM_HUES;
	  corrected_bunch_maximal_hue=bunch_maximal_hue+NUM_HUES;
	  }

	}
	hue_difference=
	abs(corrected_bunch_mean_hue-corrected_seed_mean_hue);
	min_hue_dist=
	std::min(corrected_bunch_maximal_hue-corrected_bunch_mean_hue,
	corrected_bunch_mean_hue-corrected_bunch_minimal_hue);
	left_hue=corrected_bunch_mean_hue-min_hue_dist;
	right_hue=corrected_bunch_mean_hue+min_hue_dist;
	saturation_difference=
	abs(bunch_mean_saturation-seed_sat);
	gray_difference=abs(bunch_mean_gray-seed_gray);
	hue_bound=dev_hue_zone[zone_hue];

	//if(inclusion_relation)
	//{//inclusion
		if(zone_gray<=1)
		{//zg1
			if((((zone_hue==zone_hue_bunch)&&(hue_difference<=5))||(hue_difference<=4)||
				((left_hue<=corrected_seed_mean_hue)&&(corrected_seed_mean_hue<=right_hue))))
			{//h
				hue_close=1;
			}//h

		if(((saturation_distance1<=possible_sat_variation)&&(saturation_distance<=possible_sat_variation1))||
			((seed_sat>=left_sat)&&(seed_sat<=right_sat)))
		 {//s
	     saturation_close=1;
		 }//s

			 if((zone_gray_bunch<=1)||(gray_difference<=possible_gray_variation)||
				 ((seed_gray>=left_gray)&&(seed_gray<=right_gray)))
			 {//grdif
				 grayscale_close=1;
			 }//grdif
			 else
				 {
					if(gray_difference>impossible_gray_variation)
					{
	                grayscale_close=-1;
					}
				}

	     if(grayscale_close&&saturation_close&&hue_close)
		 {
		 prior=1;
		 }

		}//zg1
		else
		{//1
			if(zone_gray<=2)
		    {//zg2
	if(((zone_hue==zone_hue_bunch)&&(hue_difference<=4))||(hue_difference<=3)||
				((left_hue<=corrected_seed_mean_hue)&&(corrected_seed_mean_hue<=right_hue)))
	         {//h2
				hue_close=1;
			 }//h2
	         else
			 {
				 if((hue_difference>=8)||((hue_difference>=6)&&(zone_hue!=zone_hue_bunch)))
				 {
					 hue_close=-1;
				 }
	         }


	   if(((saturation_distance1<=possible_sat_variation)&&(saturation_distance<=possible_sat_variation1))||
	   ((seed_sat>=left_sat)&&(seed_sat<=right_sat)))
	{//s2
	saturation_close=1;
	}//s2
				if((gray_difference<=possible_gray_variation)||
				 ((seed_gray>=left_gray)&&(seed_gray<=right_gray)))
			    {//gr2
			    grayscale_close=1;
				}//gr2
				else
				{
					if(gray_difference>impossible_gray_variation)
					{
	                grayscale_close=-1;
					}
				}
			if(grayscale_close&&saturation_close&&hue_close)
		    {
	  	    prior=1;
		    }
			else
			{
				if((grayscale_close<0)||(hue_close<0))
				{
					prior=-1;
					return(prior);
				}
				else
				            {//4
				if(grayscale_close&&saturation_close)
				{//gs
					if(inclusion_relation)
					{
						if((ratio_length<=1)&&(Density_ratio<=2))
						{
							if((zone_hue==zone_hue_bunch)&&(hue_difference<=4))
							{
	                        prior=1;
							}
						}
					}
				}//gs
				else
				{//5
					     if(hue_close&&saturation_close)
				         {
					if(inclusion_relation)
					{
						if(((ratio_length<=1)&&(Density_ratio<=1))||((ratio_length==0)&&(Density_ratio<=2)))
						{
							if(gray_difference<=(possible_gray_variation+5))
							{
	                        prior=1;
							}
						}
					}
				          }
				}//5
						   }//4
			}

			}//zg2
			else
			{//zg>
				if((ratio_length<=1)&&(Density_ratio<=2)&&(ratio_length1==16))
						{
							if(((seed_length<=3)||((seed_length<=5)&&(Density_ratio<=1)))&&(Signif_ratio==0))
							{
								if(((zone_hue==zone_hue_bunch)||(hue_difference<=5))
	&&((gray_difference<=possible_gray_variation0)||((seed_gray>=left_gray)&&(seed_gray<=right_gray))
	   ||(zone_gray_bunch=zone_gray)))
								{
							prior=1;
							return(prior);
								}
							}
				        }
				if((gray_difference<=possible_gray_variation)||((seed_gray>=left_gray)&&(seed_gray<=right_gray)))
		         {//gr3
	             grayscale_close=1;

				 }//gr3
				else
				{
	            if(gray_difference>impossible_gray_variation)
					{
	                grayscale_close=-1;
					}
				}
	   if((hue_difference<=hue_bound))
	   {//h3
		hue_close=1;
	   }//h3
	   else
			 {
				 if((hue_difference>=8)||((hue_difference>=6)&&(zone_hue!=zone_hue_bunch)))
				 {
					 hue_close=-1;
				 }
	         }
	   if(((saturation_distance1<=possible_sat_variation)&&(saturation_distance<=possible_sat_variation1))
	   ||((seed_sat>=left_sat)&&(seed_sat<=right_sat)))
		 {//s3
	saturation_close=1;
	     }//s3
	      if((grayscale_close==1)&&(saturation_close==1)&&(hue_close==1))
		    {
	  	    prior=1;
		    }
			else
		    {//e-gsh
				           if((grayscale_close<0)||(hue_close<0))
				            {
					        prior=-1;
							return(prior);
				            }
				            else
				            {//3
				if((grayscale_close==1)&&(saturation_close==1))
				{//gs
					if((inclusion_relation==1))
					{
						if((ratio_length<=1)&&(Density_ratio<=2))
						{
							if((zone_hue==zone_hue_bunch)&&(hue_difference<=4))
							{
	                        prior=1;
							}
						}
					}
				}//gs
				else
				{//2
					     if((hue_close==1)&&(saturation_close==1))
				         {
					if(inclusion_relation)
					{
						if(((ratio_length<=1)&&(Density_ratio<=1))||((ratio_length==0)&&(Density_ratio<=2)))
						{
							if(gray_difference<=(possible_gray_variation+5))
							{
	                        prior=1;
							}
						}
					}
				          }
				}//2
						   }//3

		    }//e-gsh

			}//zg>
		}//1
	//}//inclusion

	return(prior);
	}
