/* * SignalZones.cpp *
 *  Created on: Oct 25, 2017
 *      Author: konstantin
 */
//#include "stdafx.h"
#include "ColorVision.h"
#include "ImageProcess.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <algorithm>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static  int hue_zones[NUM_HUES] = { 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
5, 1, 1, 1, 1, 1 };
static  int dev_hue_zone[6] = { 2, 2, 2, 2, 2, 2 };
static  int dev_mean_sat[16] = { 1, 1, 1, 1, 1, 2, 3, 4, 4, 4, 4, 4, 3, 2, 1, 0 };
static  int dev_mean_sat1[16] = { 0, 1, 1, 1, 1, 1, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4 };
static  int imp_dev_mean_sat[16] = { 3, 3, 3, 3, 4, 5, 6, 6, 6, 6, 6, 5, 4, 4, 4, 4 };
static  int imp_dev_mean_sat1[16] = { 1, 2, 2, 3, 3, 3, 3, 4, 5, 5, 5, 6, 6, 7, 6, 6 };

static  int gray_zones[NUM_INTEN1] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2,
2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7 };
//0. 0-6, 1. 7-12, 2. 13-21, 3. 22-30, 4. 31-39, 5. 40-49, 6. 50-57, 7. 58-63
static  int dev_mean[8] = { 5, 8, 8, 10, 10, 10, 12, 8 };
static  int imp_dev_mean[8] = { 14, 16, 18, 18, 16, 16, 14, 12 };

//=========================================================
int

CImageProcess::FindSignalZones(void)
{
	int number_of_bunches;
	int number_of_covering_bunches;
	int disordering;
	int prior;
	int bunch_old_number;
	int beg_int;
	int end_int;
	int int_length;
	int int_signif;
	int int_density;
	int bunch_occurrence;
	int bunch_hue;
	int bunch_sat;
	int bunch_gray;
	int bunch_hue_zone;
	int bunch_gray_zone;
	int last_numbered_strip;
	int first_numbered_strip;
	int last_numbered_strip1;
	int first_numbered_strip1;
	int number_of_bunch_in_strip;
	int number_of_strips_involved;
	int number_of_bunch_in_strip1;
	int number_of_strips_involved1;
	int first_in_ordering_old_number;
	int bunch_ordered_number;
	int residual;
	int left_sect_max;
	int right_sect_max;
	int	left_sect_max_bunch;
	int left_sect_max_bunch_old;
	int left_sect_max_bunch_old_beg;
	int left_sect_max_bunch_old_end;
	int right_sect_max_bunch;
	int right_sect_max_bunch_old;
	int right_sect_max_bunch_old_beg;
	int right_sect_max_bunch_old_end;
	int strip_ratio;

	prior = -1;
	last_numbered_strip = -1;
	first_numbered_strip = -1;
	last_numbered_strip1 = -1;
	first_numbered_strip1 = -1;
	number_of_bunch_in_strip = -1;
	number_of_bunch_in_strip1 = -1;
	number_of_strips_involved = 0;
	number_of_strips_involved1 = 0;
	for (int count_strip = 0; count_strip<((3 * NumStrips) / 4); count_strip++)
	{//strip_loop
		if (count_strip >= LowerSkyFiber)
		{
			break;
		}
		strip_ratio = 16 * count_strip / NumStrips;
		number_of_bunches = ColorInt[count_strip].RefinedNumberOfBunches;
		first_in_ordering_old_number = ColorInt[count_strip].old_ordered_bunch_number[0];
		disordering = ColorInt[count_strip].Disordering;
		if (!disordering)
		{
			number_of_covering_bunches = ColorInt[count_strip].NumberOfIntervalsInCovering;
		}
		else
		{
			goto L;
		}
		left_sect_max = LeftGreenBoundarySection[count_strip];
		if (left_sect_max>0)
		{
			left_sect_max -= 1;
			left_sect_max_bunch = ColorDescrSect[left_sect_max].location_of_section[count_strip];
			if (left_sect_max_bunch>0)
			{
				left_sect_max_bunch -= 1;
				left_sect_max_bunch_old = ColorInt[count_strip].old_bunch_number[left_sect_max_bunch];
				left_sect_max_bunch_old_beg =
					ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[left_sect_max_bunch_old];
				left_sect_max_bunch_old_end =
					ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[left_sect_max_bunch_old];
			}
		}
		else
		{
			left_sect_max = -1;
		}
		right_sect_max = RightGreenBoundarySection[count_strip];
		if (right_sect_max>0)
		{
			right_sect_max -= 1;
			right_sect_max_bunch = ColorDescrSect[right_sect_max].location_of_section[count_strip];
			if (right_sect_max_bunch>0)
			{
				right_sect_max_bunch -= 1;
				right_sect_max_bunch_old = ColorInt[count_strip].old_bunch_number[right_sect_max_bunch];
				right_sect_max_bunch_old_beg =
					ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[right_sect_max_bunch_old];
				right_sect_max_bunch_old_end =
					ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[right_sect_max_bunch_old];
			}
		}
		else
		{
			right_sect_max = -1;
		}
		for (int count_bunch = 0; count_bunch<number_of_covering_bunches; count_bunch++)
		{//bunch_loop
			bunch_old_number = ColorInt[count_strip].old_ordered_bunch_number[count_bunch];
			beg_int = ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[bunch_old_number];
			end_int = ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[bunch_old_number];
			int_length = end_int - beg_int + 1;
			int_signif = ColorInt[count_strip].ColoredIntervalsStructure->Significance[bunch_old_number];
			int_density = (2 * int_signif) / int_length;
			bunch_occurrence = ColorInt[count_strip].visible_bunches[bunch_old_number];//last_cor22.11.16
			bunch_hue = ColorInt[count_strip].ColoredIntervalsStructure->AverageHue[bunch_old_number];
			bunch_sat = ColorInt[count_strip].ColoredIntervalsStructure->AverageSat[bunch_old_number];
			bunch_gray = ColorInt[count_strip].ColoredIntervalsStructure->AverageGray[bunch_old_number];
			if ((strip_ratio <= 2) && (int_length <= ((3 * StripWidth) / 2)) && (bunch_sat <= 3))
			{
				goto M;
			}
			if ((strip_ratio <= 5) && ((int_density >= StripWidth / 2) || ((bunch_sat <= 1) && (int_density <= 7))))
			{
				goto M;
			}
			if ((strip_ratio <= 4) && (bunch_sat <= 1) && (bunch_gray_zone >= 2) && (bunch_gray_zone <= 4))
			{
				goto M;
			}
			bunch_hue_zone = hue_zones[bunch_hue];
			bunch_gray_zone = gray_zones[bunch_gray];
			if ((strip_ratio >= 7) && (int_length >= StripWidth) && (bunch_sat <= 1) && (bunch_gray_zone >= 2) &&
				(int_density >= StripWidth / 2))
			{
				goto M;
			}
			if ((strip_ratio >= 8) && (bunch_sat <= 2) && (bunch_gray_zone >= 2) && (bunch_gray_zone <= 4) &&
				(int_density >= (2 * StripWidth) / 3))
			{
				goto M;
			}

			if ((strip_ratio >= 7) && (bunch_sat <= 1) && (bunch_gray_zone >= 2) && (bunch_gray_zone <= 3) &&
				(int_density >= StripWidth))
			{
				goto M;
			}

			if (left_sect_max >= 0)
			{
				if (end_int<left_sect_max_bunch_old_beg)
				{
					goto M;
				}
			}
			if (right_sect_max >= 0)
			{
				if (beg_int>right_sect_max_bunch_old_end)
				{
					goto M;
				}
			}
			if (bunch_gray >= 6)
			{//bg4
				if (((bunch_hue_zone == 5) && (bunch_hue >= 40) && (bunch_sat >= 4) && (bunch_gray >= 10)) ||
					((bunch_hue_zone == 5) && (bunch_hue >= 41) && (bunch_sat >= 3) && (bunch_gray >= 15)) ||
					(bunch_hue_zone == 1) && ((bunch_sat >= 3)
						|| ((bunch_sat >= 2) && ((bunch_gray_zone >= 2) || (bunch_gray >= 8)))
						|| ((bunch_sat >= 1) && ((bunch_gray_zone >= 2) && (bunch_gray >= 16)))))
				{
					if ((int_length <= DimX / 16) && (beg_int>DimX / 8) && (end_int<(DimX - DimX / 8)))
					{
						if (!StripSignals[count_strip])
						{

							prior = 1;
							last_numbered_strip = count_strip;
							number_of_bunch_in_strip = 0;
							if (first_numbered_strip<0)
							{
								first_numbered_strip = count_strip;
							}
							if (number_of_strips_involved<16)
							{
								StripSignals[count_strip] += 1;
								SignalNumber[16 * number_of_strips_involved + number_of_bunch_in_strip] = count_bunch + 1;

							}
							StripNewNumber[count_strip] = number_of_strips_involved + 1;
							number_of_strips_involved++;
						}
						else
						{
							number_of_bunch_in_strip++;
							if ((number_of_bunch_in_strip<16) && (number_of_strips_involved<16))
							{
								StripSignals[count_strip] += 1;
								SignalNumber[16 * (number_of_strips_involved - 1) + number_of_bunch_in_strip] = count_bunch + 1;
							}

						}
					}
				}
			}//bg4
		M:;
		}//bunch_loop
	L:;
		for (int count_bunch1 = 0; count_bunch1<number_of_bunches; count_bunch1++)
		{//bunch_loop1
			bunch_old_number = ColorInt[count_strip].old_bunch_number[count_bunch1];
			bunch_ordered_number = ColorInt[count_strip].ordered_bunch_number[bunch_old_number];
			if ((!bunch_ordered_number) && (bunch_old_number != first_in_ordering_old_number))
			{//0
				beg_int = ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[bunch_old_number];
				end_int = ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[bunch_old_number];
				if (left_sect_max >= 0)
				{
					if (end_int<left_sect_max_bunch_old_beg)
					{
						goto K;
					}
				}
				if (right_sect_max >= 0)
				{
					if (beg_int>right_sect_max_bunch_old_end)
					{
						goto K;
					}
				}
				int_length = end_int - beg_int + 1;
				int_signif = ColorInt[count_strip].ColoredIntervalsStructure->Significance[bunch_old_number];
				int_density = (2 * int_signif) / int_length;
				residual = (2 * int_signif) % int_length;
				if (2 * residual>int_length)
				{
					int_density += 1;
				}

				bunch_occurrence = ColorInt[count_strip].visible_bunches[bunch_old_number];//last_cor22.11.16
				bunch_hue = ColorInt[count_strip].ColoredIntervalsStructure->AverageHue[bunch_old_number];
				bunch_sat = ColorInt[count_strip].ColoredIntervalsStructure->AverageSat[bunch_old_number];
				bunch_gray = ColorInt[count_strip].ColoredIntervalsStructure->AverageGray[bunch_old_number];
				if ((strip_ratio <= 2) && (int_length <= ((3 * StripWidth) / 2)) && (bunch_sat <= 3))
				{
					goto K;
				}
				if ((strip_ratio <= 6) && (int_length <= 4) && (bunch_sat <= 1) && (int_density <= 4))
				{
					goto K;
				}

				if ((strip_ratio <= 5) && ((int_density <= 3) || ((bunch_sat <= 1) &&
					(((int_density <= 7) && (int_length <= StripWidth / 2)) ||
					(int_density <= 6)))))
				{
					goto K;
				}
				bunch_hue_zone = hue_zones[bunch_hue];
				bunch_gray_zone = gray_zones[bunch_gray];
				if ((strip_ratio <= 9) && (int_length <= 6) && (bunch_sat <= 1) && (int_density <= 3) && (bunch_gray_zone >= 3))
				{
					goto K;
				}
				if ((strip_ratio <= 4) && (bunch_sat <= 1) && (bunch_gray_zone >= 2) && (bunch_gray_zone <= 4))
				{
					goto K;
				}
				if ((strip_ratio <= 5) && (bunch_sat <= 2) && (bunch_gray_zone <= 1))
				{
					goto K;
				}
				if ((strip_ratio <= 6) && (bunch_sat <= 2) && (bunch_gray_zone <= 1) && ((int_length <= 5)))
				{
					goto K;
				}
				if ((strip_ratio <= 6) && (bunch_sat <= 1) && (bunch_gray_zone >= 5) && (int_density <= 3))
				{
					goto K;
				}
				if ((bunch_gray <= 8) && (int_length <= 5) && (int_density <= 3) && (bunch_sat <= 4) && (bunch_hue_zone == 5))
				{
					goto K;
				}
				if ((bunch_gray >= 11) && (int_length <= 3) && (int_density <= 3) && (bunch_sat <= 4) && (bunch_hue_zone == 5))
				{//last_cor22.02.18
					goto K;
				}
				if ((bunch_gray >= 11) && (int_length <= 3) && (int_density <= 3) && (bunch_sat <= 1) && (bunch_hue_zone == 1))
				{//last_cor22.02.18
					goto K;
				}
				if (bunch_gray >= 6)
				{//bg4
					if (((bunch_hue_zone == 5) && (bunch_hue >= 40) && (bunch_sat >= 4) && (bunch_gray >= 10)) ||
						((bunch_hue_zone == 5) && (bunch_hue >= 41) && (bunch_sat >= 3) && (bunch_gray >= 15)) ||
						(bunch_hue_zone == 1) && ((bunch_sat >= 3)
							|| ((bunch_sat >= 2) && ((bunch_gray_zone >= 2) || (bunch_gray >= 8)))
							|| ((bunch_sat >= 1) && ((bunch_gray_zone >= 2) && (bunch_gray >= 16)))))
					{
						if ((int_length <= DimX / 16) && (beg_int>DimX / 8) && (end_int<(DimX - DimX / 8)))
						{
							if (!StripSignalsAdditional[count_strip])
							{

								prior = 1;

								last_numbered_strip1 = count_strip;
								number_of_bunch_in_strip1 = 0;
								if (first_numbered_strip<0)
								{
									first_numbered_strip1 = count_strip;
								}
								if (number_of_strips_involved1<16)
								{
									StripSignalsAdditional[count_strip] += 1;
									SignalNumberAdditional[16 * number_of_strips_involved1 + number_of_bunch_in_strip1] = bunch_old_number + 1;

								}
								StripNewNumberAdditional[count_strip] = number_of_strips_involved1 + 1;
								number_of_strips_involved1++;
							}
							else
							{
								number_of_bunch_in_strip1++;
								if ((number_of_bunch_in_strip1<16) && (number_of_strips_involved1<16))
								{
									StripSignalsAdditional[count_strip] += 1;
									SignalNumberAdditional[16 * (number_of_strips_involved1 - 1) + number_of_bunch_in_strip1] =
										bunch_old_number + 1;
								}

							}
						}
					}
				}//bg4
			}//0
		K:;
		}//bunch_loop1
	}//strip_loop
	return(prior);
}
//===================================================
int

CImageProcess::VerticalLinesConstruct(void)
{
	int number_of_bunches;
	int number_of_covering_bunches;
	int disordering;
	int disordering1;
	int prior;
	int bunch_old_number;
	int beg_int;
	int end_int;
	int int_length;
	int next_strip;
	int oppos_bunch;
	int oppos_beg;
	int	oppos_end;
	int differ1;
	int differ2;
	int oppos_bunch_ordered_number;
	int first_closest;
	int shift_left;
	int point_scale;
	int vert_line_length;
	int vert_line_number;
	int right_contrast;
	int trace;
	int next_oppos;
	int next_oppos_beg;
	int next_oppos_end;
	int next_oppos_bunch_ordered_number;
	int number_of_oppos_bunches;
	int differ1_next;
	int differ2_next;
	int differ1_next_next;
	int differ2_next_next;
	int left_sect_max;
	int right_sect_max;
	int	left_sect_max_bunch;
	int left_sect_max_bunch_old;
	int left_sect_max_bunch_old_beg;
	int left_sect_max_bunch_old_end;
	int right_sect_max_bunch;
	int right_sect_max_bunch_old;
	int right_sect_max_bunch_old_beg;
	int right_sect_max_bunch_old_end;
	int shift_bunch;
	int shift_in_mem;
	int bunch_section;
	int shift_bunch_old;
	int bunch_section_cur;
	int bunch_section_cur_first;
	int bunch_section_cur_last;
	int bunch_section_cur_weight;
	int bunch_section_cur_incline;
	int bunch_section_cur_length;
	int bunch_section_cur_weight_normalized;
	int n_bunch_section_cur;
	int n_bunch_section_cur_first;
	int n_bunch_section_cur_last;
	int n_bunch_section_cur_weight;
	int n_bunch_section_cur_incline;
	int n_bunch_section_cur_length;
	int n_bunch_section_cur_weight_normalized;
	int n_bunch;
	int n_beg_int;
	int n_bunch_old_number;
	int n_end_int;
	int n_bunch_section_cur_sat;
	int n_bunch_length;
	int n_bunch_section_cur_right;
	int n_bunch_section_cur_first_right;
	int n_bunch_section_cur_last_right;
	int n_bunch_section_cur_weight_right;
	int n_bunch_section_cur_incline_right;
	int n_bunch_section_cur_length_right;
	int n_bunch_section_cur_weight_normalized_right;
	int n_bunch_section_cur_sat_right;
	int prev_bunch;
	int prev_beg_int;
	int prev_bunch_old_number;
	int prev_end_int;
	int prev_bunch_section_cur;
	int prev_bunch_section_cur_first;
	int prev_bunch_section_cur_last;
	int prev_bunch_section_cur_weight;
	int prev_bunch_section_cur_incline;
	int prev_bunch_section_cur_length;
	int prev_bunch_section_cur_weight_normalized;
	int prev_bunch_section_cur_sat;
	int prev_bunch_length;
	int prev_bunch_section_cur_right;
	int prev_bunch_section_cur_first_right;
	int prev_bunch_section_cur_last_right;
	int prev_bunch_section_cur_weight_right;
	int prev_bunch_section_cur_incline_right;
	int prev_bunch_section_cur_length_right;
	int prev_bunch_section_cur_weight_normalized_right;
	int prev_bunch_section_cur_sat_right;
	int left_sky_green;
	int first_bunch_old_num;
	int last_fiber_ratio;
	int first_fiber_ratio;
	int dist_left_right;
	int dist_to_right;
	int dist_to_left;
	int left_sect_bound_bunch;
	int right_sect_bound_bunch;
	int left_sect_bound_bunch_beg;
	int left_sect_bound_bunch_end;
	int right_sect_bound_bunch_beg;
	int right_sect_bound_bunch_end;
	int n_bunch_hue;
	int n_bunch_sat;
	int n_bunch_gray;
	int n_bunch_hue_zone;
	int left_bound_bunch_lim;
	int left_bound_bunch_lim_beg;
	int left_bound_bunch_lim_end;
	int right_bound_bunch_lim;
	int right_bound_bunch_lim_beg;
	int right_bound_bunch_lim_end;
	int difference_end;
	int bunch_section_cur_sk_gr_comp;

	bunch_section_cur_sk_gr_comp = 0;
	right_bound_bunch_lim = -1;
	left_bound_bunch_lim = -1;
	left_sect_bound_bunch = -1;
	right_sect_bound_bunch = -1;
	dist_to_right = DimX;
	dist_to_left = DimX;
	dist_left_right = DimX;
	last_fiber_ratio = 0;
	first_fiber_ratio = 16;
	first_bunch_old_num = -1;
	left_sky_green = -1;
	prev_bunch = -1;
	n_bunch_section_cur_right = -1;
	n_bunch_length = -1;
	n_bunch = -1;
	n_bunch_section_cur_length = -1;
	n_bunch_section_cur = -1;
	bunch_section_cur_length = -1;
	bunch_section_cur = -1;
	shift_bunch_old = -1;
	bunch_section = -1;
	shift_bunch = -1;
	left_sect_max = -1;
	right_sect_max = -1;
	left_sect_max_bunch = -1;
	right_sect_max_bunch = -1;
	left_sect_max_bunch_old_beg = -1;
	left_sect_max_bunch_old_end = -1;
	right_sect_max_bunch_old_beg = -1;
	right_sect_max_bunch_old_end = -1;
	prior = -1;
	vert_line_length = -1;
	oppos_bunch_ordered_number = -1;
	first_closest = -1;
	vert_line_number = 0;
	for (int count_strip = 0; count_strip<((3 * NumStrips) / 4); count_strip++)
	{//strip_loop
		shift_in_mem = count_strip*MAX_COL_INT;
		number_of_bunches = ColorInt[count_strip].RefinedNumberOfBunches;
		disordering = ColorInt[count_strip].Disordering;
		if (!disordering)
		{
			number_of_covering_bunches = ColorInt[count_strip].NumberOfIntervalsInCovering;
		}
		else
		{
			goto L;
		}
		if (UpperGreenBoundaryLeft>0)
		{
			left_bound_bunch_lim = LeftGreenBoundaryBunch[UpperGreenBoundaryLeft];
			left_bound_bunch_lim -= 1;
			left_bound_bunch_lim_beg =
				ColorInt[UpperGreenBoundaryLeft].ColoredIntervalsStructure->BegInterv[left_bound_bunch_lim];
			left_bound_bunch_lim_end =
				ColorInt[UpperGreenBoundaryLeft].ColoredIntervalsStructure->EndInterv[left_bound_bunch_lim];
		}
		else
		{
			left_bound_bunch_lim = -1;
		}
		if (UpperGreenBoundaryRight>0)
		{
			right_bound_bunch_lim = RightGreenBoundaryBunch[UpperGreenBoundaryRight];
			right_bound_bunch_lim -= 1;
			right_bound_bunch_lim_beg =
				ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->BegInterv[right_bound_bunch_lim];
			right_bound_bunch_lim_end =
				ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->EndInterv[right_bound_bunch_lim];
		}
		else
		{
			right_bound_bunch_lim = -1;
		}

		left_sect_max = LeftGreenBoundarySection[count_strip];
		if (left_sect_max>0)
		{
			left_sect_max -= 1;
			left_sect_max_bunch = ColorDescrSect[left_sect_max].location_of_section[count_strip];
			if (left_sect_max_bunch>0)
			{
				left_sect_max_bunch -= 1;
				left_sect_max_bunch_old = ColorInt[count_strip].old_bunch_number[left_sect_max_bunch];
				left_sect_max_bunch_old_beg =
					ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[left_sect_max_bunch_old];
				left_sect_max_bunch_old_end =
					ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[left_sect_max_bunch_old];
				left_sect_bound_bunch = LeftGreenBoundaryBunch[count_strip];
				if (left_sect_bound_bunch>0)
				{
					left_sect_bound_bunch -= 1;
					left_sect_bound_bunch_beg =
						ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[left_sect_bound_bunch];
					left_sect_bound_bunch_end =
						ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[left_sect_bound_bunch];
				}
			}
			else
			{
				left_sect_max_bunch_old_beg = 0;
				left_sect_max_bunch_old_end = 0;
				left_sect_bound_bunch_beg = 0;
				left_sect_bound_bunch_end = 0;
			}
		}
		else
		{
			left_sect_max = -1;
		}

		right_sect_max = RightGreenBoundarySection[count_strip];
		if (right_sect_max>0)
		{
			right_sect_max -= 1;
			right_sect_max_bunch = ColorDescrSect[right_sect_max].location_of_section[count_strip];
			right_sect_bound_bunch = RightGreenBoundaryBunch[count_strip];
			if (right_sect_bound_bunch>0)
			{
				right_sect_bound_bunch -= 1;
				right_sect_bound_bunch_beg =
					ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[right_sect_bound_bunch];
				right_sect_bound_bunch_end =
					ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[right_sect_bound_bunch];
			}
			else
			{
				right_sect_max_bunch_old_beg = 0;
				left_sect_max_bunch_old_end = 0;
			}
			if (right_sect_max_bunch>0)
			{
				right_sect_max_bunch -= 1;
				right_sect_max_bunch_old = ColorInt[count_strip].old_bunch_number[right_sect_max_bunch];
				right_sect_max_bunch_old_beg =
					ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[right_sect_max_bunch_old];
				right_sect_max_bunch_old_end =
					ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[right_sect_max_bunch_old];
				if (left_sect_max >= 0)
				{
					dist_left_right = right_sect_bound_bunch_beg - left_sect_bound_bunch_end;
				}
			}
		}
		else
		{
			right_sect_max = -1;
		}
		number_of_bunches = ColorInt[count_strip].NumberOfIntervalsInCovering;
		for (int count_bunch = 0; count_bunch<number_of_covering_bunches; count_bunch++)
		{//bunch_loop
			trace = LineVertTrace[maximum_number_of_ordered_bunches*(count_strip)+count_bunch];
			if (!trace)
			{//trace_cond
				first_closest = -1;
				bunch_old_number = ColorInt[count_strip].old_ordered_bunch_number[count_bunch];
				beg_int = ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[bunch_old_number];
				end_int = ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[bunch_old_number];
				if ((UpperGreenBoundaryLeft>0) && (count_strip>UpperGreenBoundaryLeft))
				{
					difference_end = left_bound_bunch_lim_end - end_int;
					if (difference_end>StripWidth)
					{
						goto M;
					}
				}
				if ((UpperGreenBoundaryRight>0) && (count_strip>UpperGreenBoundaryRight))
				{
					difference_end = end_int - right_bound_bunch_lim_beg;
					if (difference_end>StripWidth)
					{
						goto M;
					}
				}
				if (right_sect_max >= 0)
				{
					dist_to_right = right_sect_bound_bunch_beg - end_int;
					if (((dist_to_right<0)) && (abs(dist_to_right)>StripWidth))
					{
						goto M;
					}
				}
				if (left_sect_max >= 0)
				{
					dist_to_left = end_int - left_sect_bound_bunch_end;
					if (((dist_to_left<0)) && (abs(dist_to_left)>StripWidth))
					{
						goto M;
					}
				}
				bunch_section_cur =
					SectionTraceRight[shift_in_mem + bunch_old_number];
				if ((right_sect_max >= 0) && (beg_int>right_sect_max_bunch_old_beg))
				{
					goto M;
				}
				if ((left_sect_max >= 0) && (end_int<left_sect_max_bunch_old_end))
				{
					goto M;
				}
				if (bunch_section_cur>0)
				{//bc>0
					bunch_section_cur -= 1;
					bunch_section_cur = bunch_section_cur % 1000;
					bunch_section_cur_first =
						ColorSection->section_fibre_first[bunch_section_cur];
					bunch_section_cur_last =
						ColorSection->section_fibre_last[bunch_section_cur];
					last_fiber_ratio = (16 * bunch_section_cur_last) / NumStrips;
					first_fiber_ratio = (16 * bunch_section_cur_first) / NumStrips;
					bunch_section_cur_length = bunch_section_cur_last - bunch_section_cur_first + 1;
					bunch_section_cur_weight =
						ColorSection->section_weight[bunch_section_cur];
					bunch_section_cur_weight_normalized =
						bunch_section_cur_weight / bunch_section_cur_length;
					bunch_section_cur_incline = *(InclinedComponent + bunch_section_cur);
					bunch_section_cur_sk_gr_comp = SkyGreenComponents[bunch_section_cur];
					if ((last_fiber_ratio >= 6) && (first_fiber_ratio <= 2))
					{//last_cor11.03.18
						if (bunch_section_cur_incline<0)
						{
							if (dist_to_right <= 5 * StripWidth)
							{
								if (bunch_section_cur_weight_normalized >= DimX / 4)
								{
									if (bunch_section_cur_length >= NumStrips / 4)
									{
										goto M;
									}
								}
							}
						}
					}//last_cor11.03.18
					if ((right_sect_max >= 0) && (left_sect_max >= 0))
					{
						if (abs(dist_to_right) <= StripWidth)
						{
							if (bunch_section_cur_incline<-2)
							{
								if ((last_fiber_ratio >= 8) && (first_fiber_ratio >= 4))
								{
									if (bunch_section_cur_length >= NumStrips / 6)
									{
										goto M;
									}
								}
							}
						}
						if (dist_left_right <= DimX / 4)
						{
							if (dist_left_right <= DimX / 8)
							{
								if (last_fiber_ratio >= 12)
								{
									if ((bunch_section_cur_length >= NumStrips / 8) && (bunch_section_cur_weight_normalized <= DimX / 8))
									{
										goto M;
									}
								}
							}
							if (((first_fiber_ratio <= 6) && (last_fiber_ratio >= 8)) || (last_fiber_ratio >= 10))
							{
								if ((bunch_section_cur_incline<-2) && (bunch_section_cur_weight >= DimX))
								{
									if (dist_to_right <= StripWidth)
									{
										goto M;
									}
								}
							}
						}
					}
					/*if(bunch_section_cur_first<=(NumStrips/4+2))
					{
					if(bunch_section_cur_length>NumStrips/6)
					{
					if(((bunch_section_cur_incline<0)||
					((bunch_section_cur_weight_normalized>=DimX/2)&&(bunch_section_cur_weight>=6*DimX)))
					&&(bunch_section_cur_weight>=3*DimX)&&
					(bunch_section_cur_weight_normalized>=DimX/4))
					{
					if(end_int>DimX/2)
					{
					goto M;
					}
					}
					}
					}*/
					//sect_incl_curve=*(InclineCurve+section_number);
				}//bc>0
				else
				{
					bunch_section_cur = -1;
				}//ebc>
				prev_bunch = count_bunch - 1;
				if (prev_bunch >= 0)
				{//prev_bunch
					prev_bunch_old_number = ColorInt[count_strip].old_ordered_bunch_number[prev_bunch];
					prev_beg_int = ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[prev_bunch_old_number];
					prev_end_int = ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[prev_bunch_old_number];
					prev_bunch_length = prev_end_int - prev_beg_int + 1;
					prev_bunch_section_cur =
						SectionTraceLeft[shift_in_mem + prev_bunch_old_number];
					if (prev_bunch_section_cur>0)
					{//bc>0
						prev_bunch_section_cur -= 1;
						prev_bunch_section_cur = prev_bunch_section_cur % 1000;
						prev_bunch_section_cur_first =
							ColorSection->section_fibre_first[prev_bunch_section_cur];
						prev_bunch_section_cur_last =
							ColorSection->section_fibre_last[prev_bunch_section_cur];
						prev_bunch_section_cur_length = prev_bunch_section_cur_last - prev_bunch_section_cur_first + 1;
						prev_bunch_section_cur_weight =
							ColorSection->section_weight[prev_bunch_section_cur];
						prev_bunch_section_cur_weight_normalized =
							prev_bunch_section_cur_weight / prev_bunch_section_cur_length;
						prev_bunch_section_cur_incline = *(InclinedComponent + prev_bunch_section_cur);
						prev_bunch_section_cur_sat = ColorSection->section_mean_saturation[prev_bunch_section_cur];
						if (prev_bunch_section_cur_first <= (NumStrips / 4 + 2))
						{
							if (prev_bunch_section_cur_length>NumStrips / 6)
							{
								if (((prev_bunch_section_cur_incline>0) || ((prev_bunch_section_cur_weight_normalized >= DimX / 2) &&
									(prev_bunch_section_cur_weight >= 6 * DimX)))
									&& (prev_bunch_section_cur_weight >= 3 * DimX) &&
									(prev_bunch_section_cur_weight_normalized >= DimX / 4))
								{
									if ((beg_int>(2 * DimX / 3)) && (prev_bunch_length>prev_bunch_section_cur_weight_normalized))
									{
										goto M;
									}
								}
							}
						}
						//sect_incl_curve=*(InclineCurve+section_number);
					}//bc>0
					else
					{
						prev_bunch_section_cur = -1;
					}//ebc>
					prev_bunch_section_cur_right =
						SectionTraceRight[shift_in_mem + prev_bunch_old_number];
					if (prev_bunch_section_cur_right>0)
					{//bc>0
						prev_bunch_section_cur_right -= 1;
						prev_bunch_section_cur_right = prev_bunch_section_cur_right % 1000;
						prev_bunch_section_cur_first_right =
							ColorSection->section_fibre_first[prev_bunch_section_cur_right];
						prev_bunch_section_cur_last_right =
							ColorSection->section_fibre_last[prev_bunch_section_cur_right];
						prev_bunch_section_cur_length_right =
							prev_bunch_section_cur_last_right - prev_bunch_section_cur_first_right + 1;
						prev_bunch_section_cur_weight_right =
							ColorSection->section_weight[prev_bunch_section_cur_right];
						prev_bunch_section_cur_weight_normalized_right =
							prev_bunch_section_cur_weight_right / prev_bunch_section_cur_length_right;
						prev_bunch_section_cur_incline_right = *(InclinedComponent + prev_bunch_section_cur_right);
						prev_bunch_section_cur_sat_right =
							ColorSection->section_mean_saturation[prev_bunch_section_cur_right];
						if (prev_bunch_section_cur_first_right <= (NumStrips / 4 + 2))
						{
							if (prev_bunch_section_cur_length_right>NumStrips / 6)
							{
								if ((prev_bunch_section_cur_incline_right<0) &&
									(prev_bunch_section_cur_weight_right >= 3 * DimX) &&
									(prev_bunch_section_cur_weight_normalized_right >= DimX / 4))
								{
									if (beg_int>DimX / 2)
									{
										goto M;
									}
								}
							}
						}
						//sect_incl_curve=*(InclineCurve+section_number);
					}//bc>0
					else
					{
						prev_bunch_section_cur_right = -1;
					}//ebc>
				}
				n_bunch = count_bunch + 1;
				if (n_bunch<number_of_covering_bunches)
				{
					n_bunch_old_number = ColorInt[count_strip].old_ordered_bunch_number[n_bunch];
					n_beg_int = ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[n_bunch_old_number];
					n_end_int = ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[n_bunch_old_number];
					n_bunch_length = n_end_int - n_beg_int + 1;
					n_bunch_hue = ColorInt[count_strip].ColoredIntervalsStructure->AverageHue[n_bunch_old_number];
					n_bunch_hue_zone = hue_zones[n_bunch_hue];
					n_bunch_sat = ColorInt[count_strip].ColoredIntervalsStructure->AverageSat[n_bunch_old_number];
					n_bunch_gray = ColorInt[count_strip].ColoredIntervalsStructure->AverageGray[n_bunch_old_number];
					if ((left_sect_max >= 0) && (n_end_int <= left_sect_max_bunch_old_end))
					{
						goto M;
					}
					if ((right_sect_max >= 0) && (n_beg_int>right_sect_max_bunch_old_beg))
					{
						goto M;
					}
					n_bunch_section_cur =
						SectionTraceLeft[shift_in_mem + n_bunch_old_number];
					if (n_bunch_section_cur>0)
					{//bc>0
						n_bunch_section_cur -= 1;
						n_bunch_section_cur = n_bunch_section_cur % 1000;
						left_sky_green = SkyGreenComponents[n_bunch_section_cur];
						n_bunch_section_cur_first =
							ColorSection->section_fibre_first[n_bunch_section_cur];
						n_bunch_section_cur_last =
							ColorSection->section_fibre_last[n_bunch_section_cur];
						n_bunch_section_cur_length = n_bunch_section_cur_last - n_bunch_section_cur_first + 1;
						n_bunch_section_cur_weight =
							ColorSection->section_weight[n_bunch_section_cur];
						n_bunch_section_cur_weight_normalized =
							n_bunch_section_cur_weight / n_bunch_section_cur_length;
						n_bunch_section_cur_incline = *(InclinedComponent + n_bunch_section_cur);
						n_bunch_section_cur_sat = ColorSection->section_mean_saturation[n_bunch_section_cur];
						if (n_bunch_section_cur_first <= (NumStrips / 4 + 2))
						{
							if (n_bunch_section_cur_length>NumStrips / 6)
							{
								if (n_bunch_section_cur_length >= 10)
								{
									if (n_bunch_section_cur_incline<0)
									{
										if (n_bunch_section_cur_weight_normalized <= DimX / 6)
										{
											if (end_int>DimX / 2)
											{
												goto M;
											}
										}
									}
								}
								/*if((n_bunch_section_cur_incline>0)&&(n_bunch_section_cur_incline>2)
								&&(n_bunch_section_cur_weight>=3*DimX)&&(n_bunch_section_cur_weight_normalized>=DimX/6))
								{
								if((end_int<DimX/2)||((n_bunch_section_cur_weight>=6*DimX)&&
								(n_bunch_section_cur_weight_normalized>=DimX/2)))
								{
								goto M;//local behavior should be estimated!!!
								}
								}*/
							}
						}
						//sect_incl_curve=*(InclineCurve+section_number);
					}//bc>0
					else
					{
						n_bunch_section_cur = -1;
					}//ebc>
					n_bunch_section_cur_right =
						SectionTraceRight[shift_in_mem + n_bunch_old_number];
					if (n_bunch_section_cur_right>0)
					{//bc>0

						n_bunch_section_cur_right -= 1;
						n_bunch_section_cur_right = n_bunch_section_cur_right % 1000;
						if (n_bunch_section_cur_right == left_sect_max)
						{
							goto M;
						}
						n_bunch_section_cur_first_right =
							ColorSection->section_fibre_first[n_bunch_section_cur_right];
						n_bunch_section_cur_last_right =
							ColorSection->section_fibre_last[n_bunch_section_cur_right];
						n_bunch_section_cur_length_right =
							n_bunch_section_cur_last_right - n_bunch_section_cur_first_right + 1;
						n_bunch_section_cur_weight_right =
							ColorSection->section_weight[n_bunch_section_cur_right];
						n_bunch_section_cur_weight_normalized_right =
							n_bunch_section_cur_weight_right / n_bunch_section_cur_length_right;
						n_bunch_section_cur_incline_right = *(InclinedComponent + n_bunch_section_cur_right);
						n_bunch_section_cur_sat_right = ColorSection->section_mean_saturation[n_bunch_section_cur_right];
						if (n_bunch_section_cur_first_right <= (NumStrips / 4 + 2))
						{
							if ((n_bunch_section_cur_length_right>NumStrips / 6) && (abs(dist_to_left)<StripWidth))
							{
								if ((n_bunch_section_cur_incline_right<0) && (n_bunch_section_cur_weight_right >= 3 * DimX) &&
									(n_bunch_section_cur_weight_normalized_right >= DimX / 4))
								{
									if (end_int<DimX / 2)
									{
										goto M;
									}
								}
							}
						}
						//sect_incl_curve=*(InclineCurve+section_number);
					}//bc>0
					else
					{
						n_bunch_section_cur_right = -1;
					}//ebc>
				}//prev_bunch
				if ((end_int<(left_sect_max_bunch_old_beg + (left_sect_max_bunch_old_end - left_sect_max_bunch_old_beg) / 2))
					&& (left_sect_max_bunch_old_end<DimX / 2) &&
					(left_sect_max_bunch >= 0))
				{
					goto M;
				}
				if ((left_sect_max_bunch >= 0) && (left_sect_max_bunch_old_end<DimX / 2))
				{
					if (end_int == left_sect_max_bunch_old_end)
					{
						shift_bunch = count_bunch + 1;
						if (shift_bunch<number_of_covering_bunches)
						{
							shift_bunch_old = ColorInt[count_strip].old_ordered_bunch_number[shift_bunch];
							bunch_section =
								SectionTraceLeft[shift_in_mem + shift_bunch_old];
							if (bunch_section>0)
							{
								bunch_section -= 1;
								bunch_section = bunch_section % 1000;
							}
							else
							{
								bunch_section = -1;
							}
						}
					}
				}
				if ((beg_int>(right_sect_max_bunch_old_beg + (right_sect_max_bunch_old_end - right_sect_max_bunch_old_beg) / 2)) &&
					(right_sect_max_bunch_old_beg>DimX / 2) &&
					(right_sect_max_bunch >= 0))
				{
					break;
				}
				right_contrast = ColorInt[count_strip].right_continuation[bunch_old_number];
				right_contrast = right_contrast % 10;
				/*if((count_strip>NumStrips/2)&&((end_int<DimX/3)||(end_int>(DimX-DimX/3))))
				{
				end_int=0;
				}*/
				//if((end_int<(DimX-2*StripWidth))&&(beg_int>2*StripWidth))
				if ((end_int<(DimX - StripWidth)) && (right_contrast <= 0))
				{
					int_length = end_int - beg_int + 1;
					shift_left = 2 * StripWidth;
					point_scale = (end_int - std::min(shift_left, int_length / 2)) / 4;
					next_strip = count_strip + 1;
					oppos_bunch = ColorInt[next_strip].painted_strip_colored[point_scale];
					disordering1 = ColorInt[next_strip].Disordering;
					if (!disordering1)
					{//disorder
						if (oppos_bunch>0)
						{
							oppos_bunch -= 1;
						}
						else
						{
							;
						}
						number_of_oppos_bunches = ColorInt[next_strip].NumberOfIntervalsInCovering;
						oppos_bunch_ordered_number = ColorInt[next_strip].ordered_bunch_number[oppos_bunch];
						if (!oppos_bunch_ordered_number)
						{//last_cor06.12.17
							first_bunch_old_num = ColorInt[next_strip].old_ordered_bunch_number[0];
							if (first_bunch_old_num != oppos_bunch)
							{
								goto M;
							}
						}//last_cor06.12.17
						next_oppos_bunch_ordered_number = oppos_bunch_ordered_number + 1;
						if (next_oppos_bunch_ordered_number<number_of_oppos_bunches)
						{
							next_oppos = ColorInt[next_strip].old_ordered_bunch_number[next_oppos_bunch_ordered_number];
							next_oppos_beg =
								ColorInt[next_strip].ColoredIntervalsStructure->BegInterv[next_oppos];
							next_oppos_end =
								ColorInt[next_strip].ColoredIntervalsStructure->EndInterv[next_oppos];
							differ1_next = next_oppos_end - end_int;
							differ2_next = end_int - next_oppos_beg;
						}
						//}
						oppos_beg = ColorInt[next_strip].ColoredIntervalsStructure->BegInterv[oppos_bunch];
						oppos_end = ColorInt[next_strip].ColoredIntervalsStructure->EndInterv[oppos_bunch];

						if (oppos_end<(DimX - 1))
						{
							differ1 = oppos_end - end_int;
							differ2 = end_int - oppos_beg;
							if ((differ1<0) && (next_oppos >= 0))
							{
								if (abs(differ1_next)<abs(differ1))
								{
									oppos_bunch = next_oppos;
									oppos_beg = next_oppos_beg;
									oppos_end = next_oppos_end;
									differ1 = differ1_next;
									differ2 = differ2_next;
									oppos_bunch_ordered_number = next_oppos_bunch_ordered_number;
								}
							}
							if ((n_bunch>0) && ((differ1>0)))
							{
								differ1_next_next = oppos_end - n_end_int;
								differ2_next_next = n_end_int - oppos_beg;
								if (abs(differ1_next_next)<abs(differ1))
								{
									goto M;
								}
							}
							//if(differ2>=0)
							//{
							if (((abs(differ1) <= (StripWidth - 1)) || (abs(differ2) <= (StripWidth - 1))))
							{
								if (abs(differ1) <= abs(differ2))
								{
									if (oppos_bunch_ordered_number >= 0)
									{
										first_closest = oppos_bunch_ordered_number;
									}

								}
								else
								{
									if (oppos_bunch_ordered_number>0)
									{
										first_closest = oppos_bunch_ordered_number - 1;
									}
								}

							}
							//}
							/*else
							{
							if(abs(differ1)<StripWidth/2)
							{
							first_closest=oppos_bunch_ordered_number;
							}
							}*/

							if (first_closest >= 0)
							{
								if (NumberOfVertLines >= 32)
								{
									return(2);
								}
								else
								{
									vert_line_length = VerticalLinePassing(count_bunch, next_strip, first_closest,
										end_int, vert_line_number);
									if (vert_line_length>0)
									{
										VerticalLinesLength[NumberOfVertLines] = vert_line_length;
										VertLineFirstStrip[NumberOfVertLines] = count_strip + 1;
										NumberOfVertLines++;
										vert_line_number++;
									}
								}
							}
						}
					}//disorder
				}
			}//trace_cond
		M:;
		}//bunch_loop
	L:;
	}//strip_loop
	return(prior);
}
//===================================================
int

CImageProcess::VerticalLinePassing(int count_bun, int next_str, int first_cl, int first_end,
	int v_line_number)
{
	int prior;
	int chain_length;
	int count_bunch;
	int bunch_old_number;
	int beg_int;
	int end_int;
	int int_length;
	int shift_left;
	int point_scale;
	int next_strip;
	int oppos_bunch;
	int oppos_beg;
	int oppos_end;
	int oppos_bunch_ordered_number;
	int disordering;
	int differ1;
	int differ2;
	int next_oppos;
	int next_oppos_beg;
	int next_oppos_end;
	int next_oppos_bunch_ordered_number;
	int previous_oppos;
	int previous_oppos_beg;
	int previous_oppos_end;
	int previous_oppos_bunch_ordered_number;
	int number_of_oppos_bunches;
	int differ1_next;
	int differ2_next;
	int trace_vert;
	int trace_vert_next;
	int bunch_old_number_first;
	int beg_int_first;
	int end_int_first;
	int differ1_first;
	int differ2_first;
	int f_strip;
	int first_bunch_old_num;//last_cor06.12.17
	int left_sect_max;//last_cor07.12.17
	int right_sect_max;
	int	left_sect_max_bunch;
	int left_sect_max_bunch_old;
	int left_sect_max_bunch_old_beg;
	int left_sect_max_bunch_old_end;
	int right_sect_max_bunch;
	int right_sect_max_bunch_old;
	int right_sect_max_bunch_old_beg;
	int right_sect_max_bunch_old_end;
	int dist_left_right;
	int dist_to_right;
	int dist_to_left;
	int left_sect_bound_bunch;
	int right_sect_bound_bunch;
	int left_sect_bound_bunch_beg;
	int left_sect_bound_bunch_end;
	int right_sect_bound_bunch_beg;
	int right_sect_bound_bunch_end;
	int shift_in_mem;
	int oppos_old_number;
	int bunch_section_cur;
	int bunch_section_cur_first;
	int bunch_section_cur_last;
	int bunch_section_cur_weight;
	int bunch_section_cur_incline;
	int bunch_section_cur_length;
	int bunch_section_cur_weight_normalized;
	int first_fiber_ratio;
	int last_fiber_ratio;
	int next_beg;
	int next_end;
	int next_section_bunch;
	int bunch_section_cur_type;
	int total_dev;//onlyublast_cor05.06.18

	bunch_section_cur_type = 0;
	next_section_bunch = -1;
	next_beg = -1;
	next_end = -1;
	oppos_old_number = -1;
	bunch_section_cur = -1;
	bunch_section_cur_first = -1;
	bunch_section_cur_last = -1;
	bunch_section_cur_weight = -1;
	bunch_section_cur_incline = 0;
	bunch_section_cur_length = 0;
	bunch_section_cur_weight_normalized = 0;
	left_sect_bound_bunch = -1;
	right_sect_bound_bunch = -1;
	dist_to_right = DimX;
	dist_to_left = DimX;
	dist_left_right = DimX;
	left_sect_max = -1;
	right_sect_max = -1;
	left_sect_max_bunch = -1;
	right_sect_max_bunch = -1;
	left_sect_max_bunch_old_beg = -1;
	left_sect_max_bunch_old_end = -1;
	right_sect_max_bunch_old_beg = -1;
	right_sect_max_bunch_old_end = -1;
	first_bunch_old_num = -1;//last_cor06.12.17
	prior = -1;
	chain_length = 0;
	next_oppos_end = -1;
	oppos_bunch = -1;
	next_oppos = -1;
	differ1_next = DimX;
	differ2_next = DimX;
	differ1 = DimX;
	differ2 = DimX;
	count_bunch = first_cl;
	f_strip = next_str - 1;
	bunch_old_number_first = ColorInt[f_strip].old_ordered_bunch_number[count_bun];
	beg_int_first =
		ColorInt[f_strip].ColoredIntervalsStructure->BegInterv[bunch_old_number_first];
	end_int_first =
		ColorInt[f_strip].ColoredIntervalsStructure->EndInterv[bunch_old_number_first];
	for (int count_strip = next_str; count_strip<((3 * NumStrips) / 4); count_strip++)
	{//strip_loop
		bunch_old_number = ColorInt[count_strip].old_ordered_bunch_number[count_bunch];
		beg_int = ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[bunch_old_number];
		end_int = ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[bunch_old_number];
		total_dev=abs(end_int-end_int_first);
		int_length = end_int - beg_int + 1;
		shift_left = StripWidth;
		point_scale = (end_int - std::min(shift_left, int_length / 2)) / 4;
		next_strip = count_strip + 1;
		shift_in_mem = next_strip*MAX_COL_INT;
		if ((next_strip >= LowerSkyFiber)||(total_dev>=3*StripWidth))
		{//onlyublast_cor05.06.18
			if (!chain_length)
			{
				return(prior);
			}
			else
			{
				if (chain_length >= 1)
				{
					chain_length++;
					if (chain_length<24)
					{
						VerticalContrastCurves[24 * (v_line_number)+chain_length] = count_bunch + 1;
						LineVertTrace[maximum_number_of_ordered_bunches*(count_strip)+count_bunch] =
							v_line_number + 1;
					}
					else
					{
						chain_length--;
					}
				}
				return(chain_length + 1);
			}
		}
		oppos_bunch = ColorInt[next_strip].painted_strip_colored[point_scale];
		disordering = ColorInt[next_strip].Disordering;
		if (!disordering)
		{
			if (oppos_bunch>0)
			{
				oppos_bunch -= 1;
			}
			else
			{
				;
			}
			number_of_oppos_bunches = ColorInt[next_strip].NumberOfIntervalsInCovering;
			oppos_bunch_ordered_number = ColorInt[next_strip].ordered_bunch_number[oppos_bunch];
			if ((oppos_bunch_ordered_number == (number_of_oppos_bunches - 1)) && (number_of_oppos_bunches>0))
			{//last_cor15.02.18
				return(prior);
			}//last_cor15.02.18
			if (!oppos_bunch_ordered_number)
			{//last_cor06.12.17
				first_bunch_old_num = ColorInt[next_strip].old_ordered_bunch_number[0];
				if (first_bunch_old_num != oppos_bunch)
				{
					if (!chain_length)
					{
						return(prior);
					}
					else
					{//return_cond
						if (chain_length >= 1)
						{
							chain_length++;
							if (chain_length<24)
							{
								VerticalContrastCurves[24 * (v_line_number)+chain_length] = count_bunch + 1;
								LineVertTrace[maximum_number_of_ordered_bunches*(count_strip)+count_bunch] =
									v_line_number + 1;
							}
							else
							{
								chain_length--;
							}
						}
						return(chain_length + 1);
					}//return_cond
				}
			}//last_cor06.12.17
			left_sect_max = LeftGreenBoundarySection[next_strip];
			if (left_sect_max>0)
			{
				left_sect_max -= 1;
				left_sect_max_bunch = ColorDescrSect[left_sect_max].location_of_section[next_strip];
				if (left_sect_max_bunch>0)
				{
					left_sect_max_bunch -= 1;
					left_sect_max_bunch_old_beg =
						ColorInt[next_strip].ColoredIntervalsStructure->BegInterv[left_sect_max_bunch];
					left_sect_max_bunch_old_end =
						ColorInt[next_strip].ColoredIntervalsStructure->EndInterv[left_sect_max_bunch];
				}
				else
				{
					left_sect_max_bunch_old_beg = 0;
					left_sect_max_bunch_old_end = 0;
				}
				left_sect_max_bunch_old = ColorInt[next_strip].old_bunch_number[left_sect_max_bunch];
				left_sect_max_bunch_old_beg =
					ColorInt[next_strip].ColoredIntervalsStructure->BegInterv[left_sect_max_bunch_old];
				left_sect_max_bunch_old_end =
					ColorInt[next_strip].ColoredIntervalsStructure->EndInterv[left_sect_max_bunch_old];
				left_sect_bound_bunch = LeftGreenBoundaryBunch[next_strip];
				if (left_sect_bound_bunch>0)
				{
					left_sect_bound_bunch -= 1;
					left_sect_bound_bunch_beg =
						ColorInt[next_strip].ColoredIntervalsStructure->BegInterv[left_sect_bound_bunch];
					left_sect_bound_bunch_end =
						ColorInt[next_strip].ColoredIntervalsStructure->EndInterv[left_sect_bound_bunch];
				}
			}
			else
			{
				left_sect_max = -1;
			}
			right_sect_max = RightGreenBoundarySection[next_strip];
			if (right_sect_max>0)
			{
				right_sect_max -= 1;
				right_sect_max_bunch = ColorDescrSect[right_sect_max].location_of_section[next_strip];
				right_sect_bound_bunch = RightGreenBoundaryBunch[next_strip];
				if (right_sect_bound_bunch>0)
				{
					right_sect_bound_bunch -= 1;
					right_sect_bound_bunch_beg =
						ColorInt[next_strip].ColoredIntervalsStructure->BegInterv[right_sect_bound_bunch];
					right_sect_bound_bunch_end =
						ColorInt[next_strip].ColoredIntervalsStructure->EndInterv[right_sect_bound_bunch];
				}
				else
				{
					right_sect_max_bunch_old_beg = 0;
					left_sect_max_bunch_old_end = 0;
				}
				if (right_sect_max_bunch>0)
				{
					right_sect_max_bunch -= 1;
					right_sect_max_bunch_old = ColorInt[next_strip].old_bunch_number[right_sect_max_bunch];
					right_sect_max_bunch_old_beg =
						ColorInt[next_strip].ColoredIntervalsStructure->BegInterv[right_sect_max_bunch_old];
					right_sect_max_bunch_old_end =
						ColorInt[next_strip].ColoredIntervalsStructure->EndInterv[right_sect_max_bunch_old];
					if (left_sect_max >= 0)
					{
						dist_left_right = right_sect_bound_bunch_beg - left_sect_bound_bunch_end;
					}
				}
			}
			else
			{
				right_sect_max = -1;
			}
			trace_vert =
				LineVertTrace[maximum_number_of_ordered_bunches*(next_strip)+oppos_bunch_ordered_number];

			next_oppos_bunch_ordered_number = oppos_bunch_ordered_number + 1;

			if (next_oppos_bunch_ordered_number<number_of_oppos_bunches)
			{
				trace_vert_next =
					LineVertTrace[maximum_number_of_ordered_bunches*(next_strip)+next_oppos_bunch_ordered_number];
				next_oppos = ColorInt[next_strip].old_ordered_bunch_number[next_oppos_bunch_ordered_number];
				next_oppos_beg =
					ColorInt[next_strip].ColoredIntervalsStructure->BegInterv[next_oppos];
				next_oppos_end =
					ColorInt[next_strip].ColoredIntervalsStructure->EndInterv[next_oppos];
				if (trace_vert_next>0)
				{
					differ1_next = DimX;
					differ2_next = DimX;
				}
				else
				{
					differ1_next = next_oppos_end - end_int;
					differ2_next = end_int - next_oppos_beg;
				}
			}
			oppos_beg = ColorInt[next_strip].ColoredIntervalsStructure->BegInterv[oppos_bunch];
			oppos_end = ColorInt[next_strip].ColoredIntervalsStructure->EndInterv[oppos_bunch];

			if (trace_vert>0)
			{
				differ1 = DimX;
				differ2 = DimX;
			}
			else
			{
				differ1 = oppos_end - end_int;
				differ2 = end_int - oppos_beg;
				if (differ1_next>2 * StripWidth)
				{
					if ((abs(differ2_next)<abs(differ1)) &&
						(abs(differ2_next) <= (StripWidth - 1)) && (abs(differ1) <= (3 * StripWidth / 2)))
					{//last_cor13.02.18
						differ1 = differ2_next;
					}
				}
				differ1_first = oppos_end - end_int_first;
				differ2_first = end_int_first - oppos_beg;
			}
			if ((differ1>StripWidth) && (differ1_next>StripWidth))
			{
				previous_oppos_bunch_ordered_number = oppos_bunch_ordered_number - 1;
				if (previous_oppos_bunch_ordered_number >= 0)
				{
					previous_oppos =
						ColorInt[next_strip].old_ordered_bunch_number[previous_oppos_bunch_ordered_number];
					previous_oppos_beg =
						ColorInt[next_strip].ColoredIntervalsStructure->BegInterv[previous_oppos];
					previous_oppos_end =
						ColorInt[next_strip].ColoredIntervalsStructure->EndInterv[previous_oppos];
				}
			}
			if ((differ1<0) && (next_oppos >= 0))
			{
				if (abs(differ1_next)<abs(differ1))
				{
					oppos_bunch = next_oppos;
					oppos_beg = next_oppos_beg;
					oppos_end = next_oppos_end;
					differ1 = differ1_next;
					differ2 = differ2_next;
					oppos_bunch_ordered_number = next_oppos_bunch_ordered_number;
				}
			}
			if ((abs(differ1)>(3 * StripWidth / 2)) && (abs(differ2)>(3 * StripWidth / 2)))
			{//last_cor13.02.18
				if (!chain_length)
				{
					return(prior);
				}
				else
				{//return_cond
					if (chain_length >= 1)
					{
						chain_length++;
						if (chain_length<24)
						{
							VerticalContrastCurves[24 * (v_line_number)+chain_length] = count_bunch + 1;
							LineVertTrace[maximum_number_of_ordered_bunches*(count_strip)+count_bunch] =
								v_line_number + 1;
						}
						else
						{
							chain_length--;
						}
					}
					return(chain_length + 1);
				}//return_cond
			}
			else
			{
				if ((((abs(differ1) <= (StripWidth - 1)) ||
					((abs(differ1)<StripWidth) && (abs(differ1_first)<StripWidth)))
					|| (abs(differ2) <= (StripWidth - 1))))
				{//last_cor13.02.18
					if (abs(differ1) <= abs(differ2))
					{
						if (oppos_bunch_ordered_number >= 0)
						{
							oppos_old_number =
								ColorInt[next_strip].old_ordered_bunch_number[oppos_bunch_ordered_number];
							bunch_section_cur =
								SectionTraceRight[shift_in_mem + oppos_old_number];
							next_section_bunch = NextSection(oppos_bunch_ordered_number, next_strip, &next_beg, &next_end);
							if (bunch_section_cur>0)
							{//bc>0
								bunch_section_cur -= 1;
								bunch_section_cur = bunch_section_cur % 1000;
								bunch_section_cur_first =
									ColorSection->section_fibre_first[bunch_section_cur];
								bunch_section_cur_last =
									ColorSection->section_fibre_last[bunch_section_cur];
								last_fiber_ratio = (16 * bunch_section_cur_last) / NumStrips;
								first_fiber_ratio = (16 * bunch_section_cur_first) / NumStrips;
								bunch_section_cur_length = bunch_section_cur_last - bunch_section_cur_first + 1;
								bunch_section_cur_weight =
									ColorSection->section_weight[bunch_section_cur];
								bunch_section_cur_weight_normalized =
									bunch_section_cur_weight / bunch_section_cur_length;
								bunch_section_cur_incline = *(InclinedComponent + bunch_section_cur);
								bunch_section_cur_type = *(SkyGreenComponents + bunch_section_cur);
								if ((last_fiber_ratio >= 12) && (first_fiber_ratio >= 7))
								{//lfr
									if ((bunch_section_cur_length >= NumStrips / 8) && (bunch_section_cur_weight_normalized <= DimX / 8))
									{
										if (!chain_length)
										{
											return(prior);
										}
										else
										{//return_cond
											if (chain_length >= 1)
											{
												chain_length++;
												if (chain_length<24)
												{
													VerticalContrastCurves[24 * (v_line_number)+chain_length] = count_bunch + 1;
													LineVertTrace[maximum_number_of_ordered_bunches*(count_strip)+count_bunch] =
														v_line_number + 1;
												}
												else
												{
													chain_length--;
												}
											}
											return(chain_length + 1);
										}//return_cond
									}
								}//lfr
								if ((dist_left_right <= DimX / 8) || ((dist_left_right <= DimX / 3) && (first_fiber_ratio >= 8)))
								{
									if ((last_fiber_ratio >= 12) && (first_fiber_ratio >= 7))
									{//lfr
										if ((bunch_section_cur_length >= NumStrips / 8) && (bunch_section_cur_weight_normalized <= DimX / 8))
										{
											if (!chain_length)
											{
												return(prior);
											}
											else
											{//return_cond
												if (chain_length >= 1)
												{
													chain_length++;
													if (chain_length<24)
													{
														VerticalContrastCurves[24 * (v_line_number)+chain_length] = count_bunch + 1;
														LineVertTrace[maximum_number_of_ordered_bunches*(count_strip)+count_bunch] =
															v_line_number + 1;
													}
													else
													{
														chain_length--;
													}
												}
												return(chain_length + 1);
											}//return_cond
										}
									}//lfr
								}
								/*if((right_sect_max>=0)&&(left_sect_max>=0))
								{//rsmlsm
								if(dist_left_right<=DimX/4)
								{
								if(dist_left_right<=DimX/8)
								{
								if(last_fiber_ratio>=12)
								{
								if((bunch_section_cur_length>=NumStrips/8)&&(bunch_section_cur_weight_normalized<=DimX/8))
								{
								goto M;
								}
								}
								}
								if(((first_fiber_ratio<=6)&&(last_fiber_ratio>=8))||(last_fiber_ratio>=10))
								{
								if((bunch_section_cur_incline<-2)&&(bunch_section_cur_weight>=DimX))
								{
								if(dist_to_right<=StripWidth)
								{
								goto M;
								}
								}
								}
								}
								}//rsmlsm*/
								/*if(bunch_section_cur_first<=(NumStrips/4+2))
								{//bscf
								if(bunch_section_cur_length>NumStrips/6)
								{
								if(((bunch_section_cur_incline<0)||
								((bunch_section_cur_weight_normalized>=DimX/2)&&(bunch_section_cur_weight>=6*DimX)))
								&&(bunch_section_cur_weight>=3*DimX)&&
								(bunch_section_cur_weight_normalized>=DimX/4))
								{
								if(end_int>DimX/2)
								{
								goto M;
								}
								}
								}
								}//bscf*/
								//sect_incl_curve=*(InclineCurve+section_number);
							}//bc>0
							else
							{
								bunch_section_cur = -1;
							}//ebc>
							if (!chain_length)
							{
								VerticalContrastCurves[24 * (v_line_number)] = count_bun + 1;
								LineVertTrace[maximum_number_of_ordered_bunches*(next_str - 1) + count_bun] =
									v_line_number + 1;
								chain_length++;
								VerticalContrastCurves[24 * (v_line_number)+chain_length] = count_bunch + 1;
								LineVertTrace[maximum_number_of_ordered_bunches*(count_strip)+count_bunch] =
									v_line_number + 1;

							}
							else
							{
								chain_length++;
								if (chain_length<24)
								{
									//VerticalContrastCurves[24*v_line_number+chain_length]=oppos_bunch_ordered_number+1;
									VerticalContrastCurves[24 * v_line_number + chain_length] = count_bunch + 1;
									LineVertTrace[maximum_number_of_ordered_bunches*count_strip + count_bunch] =
										v_line_number + 1;
								}
								else
								{
									chain_length--;
								}
							}
							count_bunch = oppos_bunch_ordered_number;
						}

					}
					else
					{
						if (oppos_bunch_ordered_number>0)
						{

							if (!chain_length)
							{
								VerticalContrastCurves[24 * v_line_number] = count_bun + 1;
								LineVertTrace[maximum_number_of_ordered_bunches*(next_str - 1) + count_bun] =
									v_line_number + 1;
								chain_length++;
								VerticalContrastCurves[24 * (v_line_number)+chain_length] = count_bunch + 1;
								LineVertTrace[maximum_number_of_ordered_bunches*(count_strip)+count_bunch] =
									v_line_number + 1;
							}
							else
							{
								chain_length++;
								if (chain_length<24)
								{
									VerticalContrastCurves[24 * v_line_number + chain_length] = count_bunch + 1;
									LineVertTrace[maximum_number_of_ordered_bunches*count_strip + count_bunch] =
										v_line_number + 1;
								}
								else
								{
									chain_length--;
								}
							}
							count_bunch = oppos_bunch_ordered_number - 1;
						}
					}


				}
				else
				{
					if (chain_length >= 1)
					{
						chain_length++;
						if (chain_length<24)
						{
							VerticalContrastCurves[24 * (v_line_number)+chain_length] = count_bunch + 1;
							LineVertTrace[maximum_number_of_ordered_bunches*(count_strip)+count_bunch] =
								v_line_number + 1;
							return(chain_length + 1);
						}
						else
						{
							chain_length--;
							return(chain_length);
						}
					}
					else
					{
						return(prior);
					}
				}
			}
		}//disorder
		else
		{
			if (chain_length >= 1)
			{
				chain_length++;
				if (chain_length<24)
				{
					VerticalContrastCurves[24 * (v_line_number)+chain_length] = count_bunch + 1;
					LineVertTrace[maximum_number_of_ordered_bunches*(count_strip)+count_bunch] =
						v_line_number + 1;
					return(chain_length + 1);
				}
				else
				{
					chain_length--;
					return(chain_length);
				}
			}
			else
			{
				return(prior);
			}
		}
	}//strip_loop
	if (chain_length >= 1)
	{
		return(chain_length + 1);
	}
	else
	{
		return(prior);
	}
}
//===================================================
int

CImageProcess::ConnectedVerticalLines(void)
{
	int prior;
	int first_strip;
	int last_strip;
	int disordering;
	int number_of_covering_bunches;
	int strip_count;
	int first_bunch;
	int start_bunch;
	int trace;
	int trace_left_sec;
	int trace_left_sec_weight;
	int trace_right_sec;
	int trace_right_sec_weight;
	int start_trace_left_sec;
	int start_trace_right_sec;
	int shift_bun;
	int count_bunch_old;
	int count_bunch_old_beg;
	int count_bunch_old_end;
	int count_bunch_old_length;
	int	count_bunch_old_hue;
	int	count_bunch_old_sat;
	int	count_bunch_old_gray;
	int start_bunch_old;
	int	start_bunch_old_hue;
	int	start_bunch_old_sat;
	int	start_bunch_old_gray;
	int next_vert_line[24];
	int vert_lines[8];
	int line_length;
	int number_of_vert_lines_in_fiber;
	int connected_signals;
	int connected_signals_main;
	int connected_signals_additional;
	int connected_signals_right;
	int connected_signals_right_main;
	int connected_signals_right_additional;
	int connected_moving_to_us;

	prior = -1;
	trace_left_sec_weight = -1;
	trace_right_sec_weight = -1;

	for (int count_line = 0; count_line<NumberOfVertLines; count_line++)
	{//lines_loop
		line_length = VerticalLinesLength[count_line];
		connected_signals = ConnectedVertLines[count_line];
		connected_signals_main = connected_signals % 100;
		connected_signals_additional = (connected_signals - connected_signals_main) / 100;
		connected_signals_right = ConnectedVertLinesRight[count_line];
		connected_signals_right_main = connected_signals_right % 100;
		connected_signals_right_additional =
			(connected_signals_right - connected_signals_right_main) / 100;
		connected_moving_to_us = ConnectedLeftBounVert[count_line];
		memset(next_vert_line, (int) '\0', sizeof(int)*(line_length));
		first_strip = VertLineFirstStrip[count_line] - 1;
		last_strip = first_strip + line_length - 1;
		strip_count = 0;
		for (int count_strip = first_strip; count_strip<last_strip; count_strip++)
		{//strip_loop
			disordering = ColorInt[count_strip].Disordering;
			if (!disordering)
			{
				number_of_covering_bunches = ColorInt[count_strip].NumberOfIntervalsInCovering;
			}
			/*else
			{
			goto L;
			}*/
			first_bunch = VerticalContrastCurves[24 * (count_line)+strip_count];
			shift_bun = count_strip*MAX_COL_INT;
			if (first_bunch>0)
			{//fb>
				start_bunch = first_bunch - 1;
				start_bunch_old =
					ColorInt[count_strip].old_ordered_bunch_number[start_bunch];
				start_bunch_old_hue =
					ColorInt[count_strip].ColoredIntervalsStructure->AverageHue[start_bunch_old];
				start_bunch_old_sat =
					ColorInt[count_strip].ColoredIntervalsStructure->AverageSat[start_bunch_old];
				start_bunch_old_gray =
					ColorInt[count_strip].ColoredIntervalsStructure->AverageGray[start_bunch_old];
				start_trace_left_sec =
					SectionTraceLeft[shift_bun + start_bunch_old];
				if (start_trace_left_sec>0)
				{
					start_trace_left_sec = start_trace_left_sec % 1000;
					start_trace_left_sec -= 1;
					//start_trace_left_sec_weight=ColorSection->section_weight[trace_left_sec];
				}
				else
				{
					start_trace_left_sec = -1;
				}
				start_trace_right_sec =
					SectionTraceRight[shift_bun + start_bunch_old];
				if (start_trace_right_sec>0)
				{
					start_trace_right_sec = start_trace_right_sec % 1000;
					start_trace_right_sec -= 1;
					//start_trace_right_sec_weight=ColorSection->section_weight[trace_right_sec];
				}
				else
				{
					start_trace_right_sec = -1;
				}
				number_of_vert_lines_in_fiber = 0;
				for (int count_bunch = first_bunch; count_bunch<number_of_covering_bunches; count_bunch++)
				{//bunch_loop
					memset(vert_lines, (int) '\0', sizeof(int)*(8));
					count_bunch_old =
						ColorInt[count_strip].old_ordered_bunch_number[count_bunch];
					count_bunch_old_beg =
						ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[count_bunch_old];
					count_bunch_old_end =
						ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[count_bunch_old];
					count_bunch_old_length = count_bunch_old_end - count_bunch_old_beg + 1;
					count_bunch_old_hue =
						ColorInt[count_strip].ColoredIntervalsStructure->AverageHue[count_bunch_old];
					count_bunch_old_sat =
						ColorInt[count_strip].ColoredIntervalsStructure->AverageSat[count_bunch_old];
					count_bunch_old_gray =
						ColorInt[count_strip].ColoredIntervalsStructure->AverageGray[count_bunch_old];
					trace_left_sec =
						SectionTraceLeft[shift_bun + count_bunch_old];
					if (trace_left_sec>0)
					{
						trace_left_sec = trace_left_sec % 1000;
						trace_left_sec -= 1;
						trace_left_sec_weight = ColorSection->section_weight[trace_left_sec];
					}
					else
					{
						trace_left_sec = -1;
					}
					trace_right_sec =
						SectionTraceRight[shift_bun + count_bunch_old];
					if (trace_right_sec>0)
					{
						trace_right_sec = trace_right_sec % 1000;
						trace_right_sec -= 1;
						trace_right_sec_weight = ColorSection->section_weight[trace_right_sec];
					}
					else
					{
						trace_right_sec = -1;
					}
					trace = LineVertTrace[maximum_number_of_ordered_bunches*(count_strip)+count_bunch];
					if (trace>0)
					{
						if (number_of_vert_lines_in_fiber<8)
						{
							vert_lines[number_of_vert_lines_in_fiber] = trace - 1;
							number_of_vert_lines_in_fiber++;
							prior = 1;
						}
					}
				}////bunch_loop
			}//fb>
			strip_count++;
		}//strip_loop
	}//lines_loop
	return(prior);
}
//===================================================
int

CImageProcess::IntersectingVerticalLines(void)
{
	int prior;
	int first_strip;
	int last_strip;
	int line_length;
	int first_strip_next;
	int last_strip_next;
	int line_length_next;
	int first_boundary;
	int start_bunch;
	int start_bunch_old;
	//int start_bunch_old_beg;
	int start_bunch_old_end;
	int last_bunch;
	int last_bunch_old;
	//int last_bunch_old_beg;
	int last_bunch_old_end;
	int next_bunch;
	int next_bunch_old;
	//int next_bunch_old_beg;
	int next_bunch_old_end;
	int next_last_bunch;
	int next_last_bunch_old;
	//int next_last_bunch_old_beg;
	int next_last_bunch_old_end;
	int minimum_dist_plus;
	int minimum_distance_plus_line;
	int minimum_dist_minus;
	int minimum_distance_minus_line;
	int minimum_dist_plus_intersecting;
	int minimum_distance_plus_line_intersecting;
	int minimum_dist_minus_intersecting;
	int minimum_distance_minus_line_intersecting;
	int dist_between_lines;
	int Intersection;
	int indic_length;
	int ratio_length;
	int ratio_length1;
	int connected_signals;
	int connected_signals_main;
	int connected_signals_additional;
	int connected_signals_right;
	int connected_signals_right_main;
	int connected_signals_right_additional;
	int connected_moving_to_us;
	int start_bunch_old_beg;
	int next_bunch_old_beg;
	int width_max_length;
	int width_max_length_next;
	int width_max;
	int coincidence;
	int coincidence_max_plus;
	int coincidence_max_minus;
	int first_last_deviation ;
	int relative_first_last_deviation;
	int first_last_deviation_next;
	int relative_first_last_deviation_next;
	int number_of_points_in_intersection;
	int horizontal_distance;
	int horizontal_distance1;
	int length_max;
	int length_max1;
	int dist_vert;


	prior = -1;
	//return(prior);
	dist_between_lines = -1;
	for (int count_line = 0; count_line<NumberOfVertLines; count_line++)
	{//lines_loop1
		minimum_dist_plus = DimX;
		minimum_distance_plus_line = -1;
		minimum_dist_minus = DimX;
		minimum_distance_minus_line = -1;
		minimum_dist_plus_intersecting = DimX;
		minimum_distance_plus_line_intersecting = -1;
		minimum_dist_minus_intersecting = DimX;
		minimum_distance_minus_line_intersecting = -1;
		length_max=-1;
		length_max1=-1;

		line_length = VerticalLinesLength[count_line];
		connected_signals = ConnectedVertLines[count_line];
		connected_signals_main = connected_signals % 100;
		connected_signals_additional = (connected_signals - connected_signals_main) / 100;
		connected_signals_right = ConnectedVertLinesRight[count_line];
		connected_signals_right_main = connected_signals_right % 100;
		connected_signals_right_additional =
			(connected_signals_right - connected_signals_right_main) / 100;
		connected_moving_to_us = ConnectedLeftBounVert[count_line];
		first_strip = VertLineFirstStrip[count_line] - 1;
		last_strip = first_strip + line_length - 1;
		if (line_length >= 6)
		{
			width_max_length = (3*StripWidth*line_length)/2;
		}
		else
		{
			if (line_length > 3)
			{
				width_max_length = 3 * StripWidth*line_length;
			}
			else
			{
				width_max_length = 4 * StripWidth*line_length;
			}
		}
		start_bunch = VerticalContrastCurves[24 * (count_line)];
		if (start_bunch>0)
		{//sb>
			start_bunch -= 1;
			start_bunch_old =
				ColorInt[first_strip].old_ordered_bunch_number[start_bunch];
			start_bunch_old_beg =
				ColorInt[first_strip].ColoredIntervalsStructure->BegInterv[start_bunch_old];
			start_bunch_old_end =
				ColorInt[first_strip].ColoredIntervalsStructure->EndInterv[start_bunch_old];
		}//sb>
		last_bunch = VerticalContrastCurves[24 * (count_line)+last_strip - first_strip];
		if (last_bunch > 0)
		{//lb
			last_bunch -= 1;
			last_bunch_old =
				ColorInt[last_strip].old_ordered_bunch_number[last_bunch];
			//last_bunch_old_beg =
				//ColorInt[last_strip].ColoredIntervalsStructure->BegInterv[last_bunch_old];
			last_bunch_old_end =
				ColorInt[last_strip].ColoredIntervalsStructure->EndInterv[last_bunch_old];
		}//lb
		if ((start_bunch >= 0) && (last_bunch >= 0))
		{
			first_last_deviation = last_bunch_old_end - start_bunch_old_end;
			relative_first_last_deviation = first_last_deviation / (line_length - 1);
			if (abs(relative_first_last_deviation) > StripWidth / 3)
			{//last_cor27.03.18
				goto L;
			}//last_cor27.03.18
		}
		 //if((connected_signals>0)||(connected_signals_right>0)||(connected_moving_to_us>0))
		 //{//condition_additional
			coincidence_max_plus = 0;
			coincidence_max_minus = 0;
			for (int count_line1 = 0; count_line1 < NumberOfVertLines; count_line1++)
			{//lines_loop2
				if (count_line1 != count_line)
				{
					line_length_next = VerticalLinesLength[count_line1];
					first_strip_next = VertLineFirstStrip[count_line1] - 1;
					last_strip_next = first_strip_next + line_length_next - 1;
if ((first_strip >= first_strip_next) && (first_strip <= last_strip_next))
{
	if (last_strip >= last_strip_next)
	{
		number_of_points_in_intersection = last_strip_next-first_strip  + 1;
	}
	else
	{
		number_of_points_in_intersection = last_strip - first_strip + 1;
	}
}
else
{
	if ((first_strip_next >= first_strip) && (first_strip_next <= last_strip))
	{
		if (last_strip_next >= last_strip)
		{
			number_of_points_in_intersection = last_strip-first_strip_next+ 1;
		}
		else
		{
			number_of_points_in_intersection = last_strip_next - first_strip_next + 1;
		}
	}
	else
	{
		number_of_points_in_intersection = -1;
	}
}
					//if ((first_strip_next >= (first_strip - 4)) && (last_strip_next<=(last_strip+4))
					if (abs(first_strip_next - first_strip) <= 8)
					{
						if (line_length_next >= 6)
						{
							width_max_length_next = (3*StripWidth*line_length_next)/2;
						}
						else
						{
							if (line_length_next > 3)
							{
								width_max_length_next = 3*StripWidth*line_length_next;
							}
							else
							{
								width_max_length_next = 4 * StripWidth*line_length_next;
							}
						}
						width_max = std::max(width_max_length, width_max_length_next);
						next_bunch = VerticalContrastCurves[24 * (count_line1)];
						if (next_bunch > 0)
						{//nb>
							next_bunch -= 1;
							next_bunch_old =
								ColorInt[first_strip_next].old_ordered_bunch_number[next_bunch];
							next_bunch_old_beg =
								ColorInt[first_strip_next].ColoredIntervalsStructure->BegInterv[next_bunch_old];
							next_bunch_old_end =
								ColorInt[first_strip_next].ColoredIntervalsStructure->EndInterv[next_bunch_old];
							next_last_bunch =
								VerticalContrastCurves[24 * (count_line1)+last_strip_next - first_strip_next];
							if (next_last_bunch > 0)
							{//lb
								next_last_bunch -= 1;
								next_last_bunch_old =
									ColorInt[last_strip_next].old_ordered_bunch_number[next_last_bunch];
								//next_last_bunch_old_beg =
			//ColorInt[last_strip_next].ColoredIntervalsStructure->BegInterv[next_last_bunch_old];
								next_last_bunch_old_end =
									ColorInt[last_strip_next].ColoredIntervalsStructure->EndInterv[next_last_bunch_old];
								first_last_deviation_next = next_last_bunch_old_end - next_bunch_old_end;
								relative_first_last_deviation_next = first_last_deviation_next / (line_length_next - 1);
								if (abs(relative_first_last_deviation) > StripWidth / 3)
								{//last_cor27.03.18
									goto N;
								}//last_cor27.03.18
							}//lb
horizontal_distance = abs(next_last_bunch_old_end - start_bunch_old_end);
horizontal_distance1 = abs(next_bunch_old_end - last_bunch_old_end);
dist_between_lines = next_bunch_old_end - start_bunch_old_end;
							Intersection =
								ColorInt[first_strip].Intersection_measure_ratios(first_strip, last_strip,
									first_strip_next, last_strip_next, &indic_length, &ratio_length, &ratio_length1);
							coincidence = std::min(ratio_length, ratio_length1);
							if (Intersection >= 2)
							{
								if (first_strip < first_strip_next)
								{
									dist_vert = last_strip - first_strip_next;
									if ((horizontal_distance <= 3 * StripWidth) && (abs(dist_vert) < 3))
									{
										if (line_length_next > length_max)
										{
											length_max = line_length_next;
											UpperClosestLine[count_line] = count_line1 + 1;
										}
									}
								}
								if (first_strip_next < first_strip)
								{
									dist_vert = last_strip_next - first_strip;
									if ((horizontal_distance <= 3 * StripWidth) && (abs(dist_vert) < 3))
									{
										if (line_length_next > length_max1)
										{
											length_max1 = line_length_next;
											LowerClosestLine[count_line] = count_line1 + 1;
										}
									}
								}
							}
if ((Intersection >= 0) && (Intersection<3)&&
(abs(dist_between_lines) <= width_max) && (coincidence >= 2))
{
prior = 1;
first_boundary = FirstConnectedVertline[count_line];
if (!first_boundary)
{
FirstConnectedVertline[count_line] = count_line1 + 1;
}
LastConnectedVertline[count_line] = count_line1 + 1;
		if (dist_between_lines > 0)
		{
if (dist_between_lines < minimum_dist_plus_intersecting)
{
	if (number_of_points_in_intersection > 1)
	{
		minimum_distance_plus_line_intersecting = count_line1;
		minimum_dist_plus_intersecting = dist_between_lines;
	}
}
		}
		else
		{
		if (dist_between_lines < minimum_dist_minus_intersecting)
		{
			if (number_of_points_in_intersection > 1)
			{
				minimum_distance_minus_line_intersecting = count_line1;
				minimum_dist_minus_intersecting = dist_between_lines;
			}
		}
		}
		}
							if ((Intersection >= 0) && (Intersection <= 1) && (coincidence >= 6) &&
								(abs(dist_between_lines) <= width_max))
							{//Intcoins
								if (dist_between_lines > 0)
								{
									if (coincidence > coincidence_max_plus)
									{
										coincidence_max_plus = coincidence;
										minimum_distance_plus_line = count_line1;
										minimum_dist_plus = dist_between_lines;
									}
									else
									{
										if (coincidence == coincidence_max_plus)
										{
											if (dist_between_lines < minimum_dist_plus)
											{
												minimum_distance_plus_line = count_line1;
												minimum_dist_plus = dist_between_lines;
											}
										}
									}

								}

								if (dist_between_lines < 0)
								{
									dist_between_lines = -dist_between_lines;
									if (coincidence > coincidence_max_minus)
									{
										coincidence_max_minus = coincidence;
										minimum_distance_minus_line = count_line1;
										minimum_dist_plus = dist_between_lines;
									}
									else
									{
										if (coincidence == coincidence_max_minus)
										{
											if (dist_between_lines < minimum_dist_minus)
											{
												minimum_distance_minus_line = count_line1;
												minimum_dist_minus = dist_between_lines;
											}
										}
									}

								}
							}//Intcoins

						}//nb>
						else
						{
							break;
						}
					}
				}
			N:;
			}//lines_loop2
			 //}//condition_additional
			if (minimum_distance_plus_line_intersecting >= 0)
			{
				RightClosestLineIntersecting[count_line] =
					minimum_distance_plus_line_intersecting + 1;
			}
			if (minimum_distance_minus_line_intersecting >= 0)
			{
				LeftClosestLineIntersecting[count_line] =
					minimum_distance_minus_line_intersecting + 1;
			}
			if (minimum_distance_plus_line >= 0)
			{
				RightClosestLine[count_line] = minimum_distance_plus_line + 1;
			}
			if (minimum_distance_minus_line >= 0)
			{
				LeftClosestLine[count_line] = minimum_distance_minus_line + 1;
			}
		L:;
		}//lines_loop1
	return(prior);
}

//===================================================
int

CImageProcess::BreakingVerticalLines(void)
{
	int prior;
	int left_boun;
	int right_boun;

	prior = -1;
	for (int count_line = 0; count_line<NumberOfVertLines; count_line++)
	{//lines_loop
		left_boun = FirstConnectedVertline[count_line];
		if (left_boun>0)
		{
			left_boun -= 1;
		}
		else
		{
			left_boun = -1;
		}
		right_boun = LastConnectedVertline[count_line];
		if (right_boun>0)
		{
			right_boun -= 1;
		}
		else
		{
			right_boun = -1;
		}
	}//lines_loop
	return(prior);
}
//===================================================
int

CImageProcess::VerticalLinesSignalsConnected(void)
{
	int prior;
	int first_strip;
	int last_strip;
	int line_length;
	int start_bunch;
	int start_bunch_old;
	int start_bunch_old_beg;
	int start_bunch_old_end;
	int strip_signal_indication;
	int new_strip_number;
	int signal_bunch_number;
	int signal_bunch_number_old;
	int signal_bunch_number_old_beg;
	int signal_bunch_number_old_end;
	int count_signal_strip_in_intersection;
	int count_signal_strip_in_intersection1;
	int bunch_order_diff;
	int bunch_signal_distance;
	int number_of_signals_closed_left;
	int number_of_signals_closed_right;
	int number_of_signals_closed_left1;
	int number_of_signals_closed_right1;
	int bunch_signal_distance_oppos;
	//int number_of_signal_close_lines;

	bunch_signal_distance_oppos = DimX;
	prior = -1;
	bunch_signal_distance = DimX;
	count_signal_strip_in_intersection = 0;
	count_signal_strip_in_intersection1 = 0;
	for (int count_line = 0; count_line<NumberOfVertLines; count_line++)
	{//lines_loop
		line_length = VerticalLinesLength[count_line];
		first_strip = VertLineFirstStrip[count_line] - 1;
		last_strip = first_strip + line_length - 1;
		number_of_signals_closed_left = 0;
		number_of_signals_closed_right = 0;
		number_of_signals_closed_left1 = 0;
		number_of_signals_closed_right1 = 0;
		for (int count_strip = first_strip; count_strip <= last_strip; count_strip++)
		{//strip_loop
			strip_signal_indication = StripSignals[count_strip];
			if (strip_signal_indication >= 1)
			{
				count_signal_strip_in_intersection++;
				new_strip_number = StripNewNumber[count_strip] - 1;
				start_bunch = VerticalContrastCurves[24 * (count_line)+count_strip - first_strip];
				if (start_bunch>0)
				{//sb>
					start_bunch -= 1;
					start_bunch_old =
						ColorInt[count_strip].old_ordered_bunch_number[start_bunch];
					start_bunch_old_beg =
						ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[start_bunch_old];
					start_bunch_old_end =
						ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[start_bunch_old];
				}//sb>
				for (int count_bunch = 0; count_bunch<16; count_bunch++)
				{//bunch_Signal_loop
					signal_bunch_number = SignalNumber[16 * new_strip_number + count_bunch];
					if (signal_bunch_number>0)
					{
						signal_bunch_number -= 1;
						signal_bunch_number_old =
							ColorInt[count_strip].old_ordered_bunch_number[signal_bunch_number];
						signal_bunch_number_old_beg =
							ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[signal_bunch_number_old];
						signal_bunch_number_old_end =
							ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[signal_bunch_number_old];
						bunch_order_diff = signal_bunch_number - start_bunch;
						if (bunch_order_diff>0)
						{
							bunch_signal_distance = signal_bunch_number_old_end - start_bunch_old_end;
							bunch_signal_distance_oppos = signal_bunch_number_old_beg - start_bunch_old_end;
							if ((abs(bunch_order_diff) <= 2) && (bunch_signal_distance >= 0)
								&& (abs(bunch_signal_distance_oppos) <= (7 * StripWidth / 4)))
							{
								number_of_signals_closed_right++;
								VerticalContrastCurvesClosedSignals[24 * (count_line)+count_strip - first_strip] =
									signal_bunch_number + 1;
							}
						}
						else
						{
							bunch_signal_distance = signal_bunch_number_old_beg - start_bunch_old_end;
							bunch_signal_distance_oppos = signal_bunch_number_old_end - start_bunch_old_end;
							if (((bunch_order_diff<0) && (abs(bunch_order_diff) <= 2) && (bunch_signal_distance >= 0)
								&& (abs(bunch_signal_distance_oppos) <= (7 * StripWidth / 4))) || (!bunch_order_diff))
							{
								number_of_signals_closed_left++;
								VerticalContrastCurvesClosedSignals[24 * (count_line)+count_strip - first_strip] =
									signal_bunch_number + 1;
							}
						}

					}
					else
					{
						break;
					}
				}//bunch_Signal_loop
			}
		}//strip_loop
		if ((number_of_signals_closed_left>0) || (number_of_signals_closed_right>0))
		{
			if (number_of_signals_closed_left>0)
			{
				ConnectedVertLines[count_line] = number_of_signals_closed_left;
			}
			if (number_of_signals_closed_right>0)
			{
				ConnectedVertLinesRight[count_line] = number_of_signals_closed_right;
			}
			NumberOfVertLinesCloseToSignals++;
		}
		for (int count_strip1 = first_strip; count_strip1 <= last_strip; count_strip1++)
		{//strip_loop1
			strip_signal_indication = StripSignalsAdditional[count_strip1];
			if (strip_signal_indication >= 1)
			{
				count_signal_strip_in_intersection1++;
				new_strip_number = StripNewNumberAdditional[count_strip1] - 1;
				start_bunch = VerticalContrastCurves[24 * (count_line)+count_strip1 - first_strip];
				if (start_bunch>0)
				{//sb>
					start_bunch -= 1;
					start_bunch_old =
						ColorInt[count_strip1].old_ordered_bunch_number[start_bunch];
					start_bunch_old_beg =
						ColorInt[count_strip1].ColoredIntervalsStructure->BegInterv[start_bunch_old];
					start_bunch_old_end =
						ColorInt[count_strip1].ColoredIntervalsStructure->EndInterv[start_bunch_old];
				}//sb>
				for (int count_bunch = 0; count_bunch<16; count_bunch++)
				{//bunch_Signal_loop
					signal_bunch_number = SignalNumberAdditional[16 * new_strip_number + count_bunch];
					if (signal_bunch_number>0)
					{
						signal_bunch_number -= 1;
						signal_bunch_number_old_beg =
							ColorInt[count_strip1].ColoredIntervalsStructure->BegInterv[signal_bunch_number];
						signal_bunch_number_old_end =
							ColorInt[count_strip1].ColoredIntervalsStructure->EndInterv[signal_bunch_number];
						//if(signal_bunch_number_old_end>start_bunch_old_end)
						//{
						bunch_signal_distance = signal_bunch_number_old_end - start_bunch_old_end;
						bunch_signal_distance_oppos = signal_bunch_number_old_beg - start_bunch_old_end;

						//}
						if (((bunch_signal_distance_oppos >= 0) && (bunch_signal_distance_oppos <= (7 * StripWidth / 4))) ||
							((bunch_signal_distance >= 0) && (abs(bunch_signal_distance_oppos) <= bunch_signal_distance) &&
							(abs(bunch_signal_distance_oppos) <= StripWidth) && (bunch_signal_distance <= 2 * StripWidth)))
						{
							number_of_signals_closed_right1++;
							VerticalContrastCurvesClosedSignals[24 * (count_line)+count_strip1 - first_strip] =
								signal_bunch_number + 1;
						}

						if (((bunch_signal_distance <= 0) && (abs(bunch_signal_distance) <= (7 * StripWidth / 4))) ||
							((bunch_signal_distance_oppos <= 0) &&
							(abs(bunch_signal_distance) <= abs(bunch_signal_distance_oppos) &&
								(abs(bunch_signal_distance) <= StripWidth) && (abs(bunch_signal_distance_oppos) <= 2 * StripWidth))))
						{
							number_of_signals_closed_left1++;
							VerticalContrastCurvesClosedSignals[24 * (count_line)+count_strip1 - first_strip] =
								signal_bunch_number + 1;
						}

					}
					else
					{
						break;
					}
				}//bunch_Signal_loop
			}
		}//strip_loop1
		if ((number_of_signals_closed_left1>0) || (number_of_signals_closed_right1>0))
		{
			if (number_of_signals_closed_left1>0)
			{
				ConnectedVertLines[count_line] += 100 * number_of_signals_closed_left1;
			}
			if (number_of_signals_closed_right1>0)
			{
				ConnectedVertLinesRight[count_line] += 100 * number_of_signals_closed_right1;
			}
			NumberOfVertLinesCloseToSignals1++;
		}
	}//lines_loop
	return(prior);
}
//===================================================
int

CImageProcess::VerticalPartsofGreenBoundary(void)
{
	int prior;
	int bunch_boundary;
	int bunch_boundary_end;
	int bunch_boundary_next;
	int bunch_boundary_end_next;
	int ends_difference;
	int step_zero;
	int step_count;
	int final_step;
	int glob_diffeence;
	int previous_success;

	prior = -1;
	step_zero = -1;
	final_step = -1;
	step_count = 0;
	previous_success = 0;
	glob_diffeence = DimX;
	if (UpperGreenBoundaryLeft>0)
	{
		for (int count_point = 0; count_point<UpperGreenBoundaryLeft - 1; count_point++)
		{//points_loop
			bunch_boundary = LeftGreenBoundaryBunch[count_point];
			bunch_boundary_next = LeftGreenBoundaryBunch[count_point + 1];
			if ((bunch_boundary>0) && (bunch_boundary_next>0))
			{
				bunch_boundary -= 1;
				bunch_boundary_end =
					ColorInt[count_point].ColoredIntervalsStructure->EndInterv[bunch_boundary];
				bunch_boundary_next -= 1;
				bunch_boundary_end_next =
					ColorInt[count_point + 1].ColoredIntervalsStructure->EndInterv[bunch_boundary_next];
				ends_difference = bunch_boundary_end_next - bunch_boundary_end;
				if (previous_success == 1)
				{
					glob_diffeence = ends_difference / (step_count + 1);
				}
				if ((abs(ends_difference)<StripWidth) ||
					((previous_success == 1) && (glob_diffeence <= StripWidth / 2) && (ends_difference <= 3 * StripWidth)))
				{
					if (step_zero<0)
					{
						step_zero = count_point;
					}
					previous_success = 1;
					step_count++;
				}
				else
				{
					glob_diffeence = DimX;
					previous_success = 0;
					if (abs(ends_difference)>10 * StripWidth)
					{
						if (step_count >= 3)
						{
							prior = 1;
							final_step = step_zero + step_count - 1;
							FillingVerticalPartsofGreenBoundary(step_zero, final_step);
							step_count = 0;
						}
					}
				}
			}
		}//points_loop
	}
	if (previous_success == 1)
	{
		if (step_count >= 3)
		{
			prior = 1;
			final_step = step_zero + step_count - 1;
			FillingVerticalPartsofGreenBoundary(step_zero, final_step);
			step_count = 0;
		}
	}
	return(prior);
}
//===================================================
void

CImageProcess::FillingVerticalPartsofGreenBoundary(int first_step, int last_step)
{
	for (int count_point = first_step; count_point <= last_step; count_point++)
	{//points_loop
		LeftGreenBoundaryVert[count_point] = 1;
	}//points_loop
}
//===================================================
void

CImageProcess::VerticalConnectedToBoundary(void)
{
	int line_length;
	int first_strip;
	int last_strip;
	int first_bunch;
	int first_bunch_old;
	int first_bunch_old_beg;
	int first_bunch_old_end;
	int last_bunch_old;
	int last_bunch_old_beg;
	int last_bunch_old_end;
	int start_bunch;
	int start_bunch_old;
	int start_bunch_old_beg;
	int start_bunch_old_end;
	int left_sect_max;
	int left_sect_max_bunch;
	int left_sect_max_bunch_old;
	int left_sect_max_bunch_old_beg;
	int left_sect_max_bunch_old_end;
	int left_sect_bound_bunch;
	int left_sect_bound_bunch_beg;
	int left_sect_bound_bunch_end;
	int right_sect_max;
	int right_sect_max_bunch;
	int right_sect_max_bunch_old;
	int right_sect_max_bunch_old_beg;
	int right_sect_max_bunch_old_end;
	int right_sect_bound_bunch;
	int right_sect_bound_bunch_beg;
	int right_sect_bound_bunch_end;
	int count_vert_r_side;
	int vert_line_bound;
	int dist_to_right;
	int dist_to_left;
	int dist_left_right;
	int dist_to_left1;
	int dist_left_ratio;
	int dist_right_ratio;
	int count_vert_ratio;
	int vert_line_signal_right;
	int vert_line_signal_right_main;
	int vert_line_signal_right_additional;
	int last_bunch;
	int first_last_deviation;
	int relative_first_last_deviation;
	int first_bunch_section;
	int first_left_sky_green;
	int first_bunch_cur_first;
	int first_bunch_cur_last;
	int first_bunch_cur_length;
	int first_bunch_cur_weight;
	int first_bunch_cur_weight_normalized;
	int first_bunch_cur_incline;
	int first_bunch_cur_sat;
	int first_bunch_cur_first_ratio;
	int first_bunch_cur_last_ratio;
	int last_bunch_section;
	int last_left_sky_green;
	int last_bunch_cur_first;
	int last_bunch_cur_last;
	int last_bunch_cur_length;
	int last_bunch_cur_weight;
	int last_bunch_cur_weight_normalized;
	int last_bunch_cur_incline;
	int last_bunch_cur_sat;
	int last_bunch_cur_first_ratio;
	int last_bunch_cur_last_ratio;
	int first_strip_ratio;
	int last_strip_ratio;
	int left_sect_max_first;
	int right_sect_max_first;
	int left_sect_max_last;
	int right_sect_max_last;
	int left_closest_line;
	int right_closest_line;
	int left_distance_closest_line;
	int right_distance_closest_line;
	int first_boundary_line;
	int last_boundary_line;
	int first_bunch_next;
	int first_bunch_next_old;
	int first_bunch_next_old_beg;
	int first_bunch_next_old_end;
	int first_bunch_next_section;
	int left_sky_green;
	int first_bunch_next_cur_first;
	int first_bunch_next_cur_last;
	int first_bunch_next_cur_length;
	int first_bunch_next_cur_weight;
	int first_bunch_next_cur_weight_normalized;
	int first_bunch_next_cur_incline;
	int first_bunch_next_cur_sat;
	int first_bunch_next_cur_first_ratio;
	int first_bunch_next_cur_last_ratio;
	int first_bunch_old_length;
	int first_bunch_next_old_length;
	int last_bunch_next;
	int last_bunch_next_old;
	int last_bunch_next_old_beg;
	int last_bunch_next_old_end;
	int last_bunch_next_section;
	int last_left_next_sky_green;
	int last_bunch_next_cur_first;
	int last_bunch_next_cur_last;
	int last_bunch_next_cur_length;
	int last_bunch_next_cur_weight;
	int last_bunch_next_cur_weight_normalized;
	int last_bunch_next_cur_incline;
	int last_bunch_next_cur_sat;
	int last_bunch_next_cur_first_ratio;
	int last_bunch_next_cur_last_ratio;
	int last_bunch_old_length;
	int last_bunch_next_old_length;
	int last_bunch_next_section_left;
	int last_left_next_sky_green_left;
	int last_bunch_next_cur_first_left;
	int last_bunch_next_cur_last_left;
	int last_bunch_next_cur_length_left;
	int last_bunch_next_cur_weight_left;
	int last_bunch_next_cur_weight_normalized_left;
	int last_bunch_next_cur_incline_left;
	int last_bunch_next_cur_sat_left;
	int last_bunch_next_cur_first_ratio_left;
	int last_bunch_next_cur_last_ratio_left;
	int next_incline_local;
	int next_incline_local_first;
	int next_incline_local_last;
	int next_incline_local_left;
	int next_incline_local_left_first;
	int next_incline_local_left_last;
	int intersection_length;
	int intersection_length_left;
	int number_of_covering_bunches;
	int left_green_boun_bunch;
	int left_green_boun_bunch_end;
	int first_bunch_distance_to_left_boun;
	int right_green_boun_bunch;
	int left_green_boun_bunch_last;
	int right_green_boun_bunch_last;
	//int line_length;
	/*int first_strip_connected;
	int last_strip_connected;
	int first_bunch_connected;
	int first_bunch_old_connected;
	int first_bunch_old_beg_connected;
	int first_bunch_old_end_connected;
	int last_bunch_old_connected;
	int last_bunch_old_beg_connected;
	int last_bunch_old_end_connected;
	int bunch_connected;
	int bunch_connected_old;
	int bunch_connected_old_beg;
	int bunch_connected_old_end;
	int distance_between_connected_lines;*/

	first_bunch_old = -1;
	left_green_boun_bunch_end=-1;
	first_bunch_distance_to_left_boun = DimX;
	next_incline_local_left = 0;
	next_incline_local = 0;
	number_of_covering_bunches = 0;
	first_bunch_next_section = -1;
	first_bunch_next_old = -1;
	first_bunch_next = -1;
	dist_left_right = DimX;
	dist_left_ratio = -1;
	dist_right_ratio = -1;
	for (int count_line = 0; count_line<NumberOfVertLines; count_line++)
	{//lines_loop1
		count_vert_r_side = 0;
		line_length = VerticalLinesLength[count_line];
		first_strip = VertLineFirstStrip[count_line] - 1;
		last_strip = first_strip + line_length - 1;
		first_bunch = VerticalContrastCurves[24 * (count_line)];
		if (first_bunch > 0)
		{//fb
			first_bunch -= 1;
			first_bunch_old =
				ColorInt[first_strip].old_ordered_bunch_number[first_bunch];
			first_bunch_old_beg =
ColorInt[first_strip].ColoredIntervalsStructure->BegInterv[first_bunch_old];
			first_bunch_old_end =
ColorInt[first_strip].ColoredIntervalsStructure->EndInterv[first_bunch_old];
			first_bunch_old_length = first_bunch_old_end - first_bunch_old_beg + 1;
			last_bunch = VerticalContrastCurves[24 * (count_line)+last_strip - first_strip];
			if (last_bunch > 0)
			{//lb
				last_bunch -= 1;
				last_bunch_old =
					ColorInt[last_strip].old_ordered_bunch_number[last_bunch];
				last_bunch_old_beg =
					ColorInt[last_strip].ColoredIntervalsStructure->BegInterv[last_bunch_old];
				last_bunch_old_end =
					ColorInt[last_strip].ColoredIntervalsStructure->EndInterv[last_bunch_old];
				last_bunch_old_length = last_bunch_old_end - last_bunch_old_beg + 1;
				last_strip_ratio = (16 * last_strip) / (NumStrips - 1);
				first_strip_ratio = (16 * first_strip) / (NumStrips - 1);
				first_last_deviation = last_bunch_old_end - first_bunch_old_end;
				relative_first_last_deviation = first_last_deviation / (line_length - 1);
				if (abs(relative_first_last_deviation) > StripWidth / 3)
				{//last_cor27.03.18
					goto L;
				}//last_cor27.03.18
			}//lb
			else
			{
				return;
			}
		}//fb
		else
		{
			return;
		}
		left_closest_line = LeftClosestLine[count_line];
		if (left_closest_line>0)
		{
			left_closest_line -= 1;
		}
		else
		{
			left_closest_line = -1;
		}
		right_closest_line = RightClosestLine[count_line];
		if (right_closest_line>0)
		{
			right_closest_line -= 1;
		}
		else
		{
			right_closest_line = -1;
		}
		left_distance_closest_line = LeftClosestLineIntersecting[count_line];
		if (left_distance_closest_line>0)
		{
			left_distance_closest_line -= 1;
		}
		else
		{
			left_distance_closest_line = -1;
		}
		right_distance_closest_line = RightClosestLineIntersecting[count_line];
		if (right_distance_closest_line>0)
		{
			right_distance_closest_line -= 1;
		}
		else
		{
			right_distance_closest_line = -1;
		}
		first_boundary_line= FirstConnectedVertline[count_line];
		if (first_boundary_line > 0)
		{
			first_boundary_line -= 1;
		}
		last_boundary_line= LastConnectedVertline[count_line];
		if (last_boundary_line > 0)
		{
			last_boundary_line -= 1;
		}
		left_sect_max_first = LeftGreenBoundarySection[first_strip];
		if (left_sect_max_first>0)
		{
			left_sect_max_first -= 1;
		}
		right_sect_max_first = RightGreenBoundarySection[first_strip];
		if (right_sect_max_first>0)
		{
			right_sect_max_first -= 1;
		}
		left_sect_max_last = LeftGreenBoundarySection[last_strip];
		if (left_sect_max_last>0)
		{
			left_sect_max_last -= 1;
		}
		right_sect_max_last = RightGreenBoundarySection[last_strip];
		if (right_sect_max_last>0)
		{
			right_sect_max_last -= 1;
		}
		left_green_boun_bunch= LeftGreenBoundaryBunch[first_strip];
		if (left_green_boun_bunch>0)
		{
			left_green_boun_bunch -= 1;
			left_green_boun_bunch_end =
ColorInt[first_strip].ColoredIntervalsStructure->EndInterv[left_green_boun_bunch];
			first_bunch_distance_to_left_boun =
abs(first_bunch_old_beg - left_green_boun_bunch_end);
		}
		else
		{
			left_green_boun_bunch = -1;
		}
		right_green_boun_bunch= RightGreenBoundaryBunch[first_strip];
		if (right_green_boun_bunch>0)
		{
			right_green_boun_bunch -= 1;
		}
		else
		{
			right_green_boun_bunch = -1;
		}
		left_green_boun_bunch_last = LeftGreenBoundaryBunch[last_strip];
		if (left_green_boun_bunch_last>0)
		{
			left_green_boun_bunch_last -= 1;
		}
		else
		{
			left_green_boun_bunch_last = -1;
		}
		right_green_boun_bunch_last = RightGreenBoundaryBunch[last_strip];
		if (right_green_boun_bunch_last>0)
		{
			right_green_boun_bunch_last -= 1;
		}
		else
		{
			right_green_boun_bunch_last = -1;
		}
				first_bunch_section =
				SectionTraceLeft[first_strip*MAX_COL_INT + first_bunch_old];
			first_bunch_section = first_bunch_section % 1000;
			if (first_bunch_section>0)
			{
				first_bunch_section -= 1;
				first_left_sky_green = SkyGreenComponents[first_bunch_section];
				first_left_sky_green = first_left_sky_green % 10;
				first_bunch_cur_first =
					ColorSection->section_fibre_first[first_bunch_section];
				first_bunch_cur_first_ratio = (16 * first_bunch_cur_first) / (NumStrips - 1);
				first_bunch_cur_last =
					ColorSection->section_fibre_last[first_bunch_section];
				first_bunch_cur_last_ratio = (16 * first_bunch_cur_last) / (NumStrips - 1);
				first_bunch_cur_length = first_bunch_cur_last - first_bunch_cur_first + 1;
				first_bunch_cur_weight =
					ColorSection->section_weight[first_bunch_section];
				first_bunch_cur_weight_normalized =
					first_bunch_cur_weight / first_bunch_cur_length;
				first_bunch_cur_incline = *(InclinedComponent + first_bunch_section);
				first_bunch_cur_sat = ColorSection->section_mean_saturation[first_bunch_section];
			}
			number_of_covering_bunches = ColorInt[first_strip].NumberOfIntervalsInCovering;
			first_bunch_next = first_bunch + 1;
			if (first_bunch_next<number_of_covering_bunches)
			{//fbn<
				first_bunch_next_old =
					ColorInt[first_strip].old_ordered_bunch_number[first_bunch_next];
				first_bunch_next_old_beg =
					ColorInt[first_strip].ColoredIntervalsStructure->BegInterv[first_bunch_next_old];
				first_bunch_next_old_end =
					ColorInt[first_strip].ColoredIntervalsStructure->EndInterv[first_bunch_next_old];
				first_bunch_next_old_length = first_bunch_next_old_end - first_bunch_next_old_beg + 1;
				first_bunch_next_section =
					SectionTraceRight[first_strip*MAX_COL_INT + first_bunch_next_old];
				first_bunch_next_section = first_bunch_next_section % 1000;
				if (first_bunch_next_section>0)
				{
					first_bunch_next_section -= 1;
					left_sky_green = SkyGreenComponents[first_bunch_next_section];
					left_sky_green = left_sky_green % 10;
					first_bunch_next_cur_first =
						ColorSection->section_fibre_first[first_bunch_next_section];
					first_bunch_next_cur_first_ratio = (16 * first_bunch_next_cur_first) / (NumStrips - 1);
					first_bunch_next_cur_last =
						ColorSection->section_fibre_last[first_bunch_next_section];
					first_bunch_next_cur_last_ratio = (16 * first_bunch_next_cur_last) / (NumStrips - 1);
					first_bunch_next_cur_length = first_bunch_next_cur_last - first_bunch_next_cur_first + 1;
					first_bunch_next_cur_weight =
						ColorSection->section_weight[first_bunch_next_section];
					first_bunch_next_cur_weight_normalized =
						first_bunch_next_cur_weight / first_bunch_next_cur_length;
					first_bunch_next_cur_incline = *(InclinedComponent + first_bunch_next_section);
					first_bunch_next_cur_sat = ColorSection->section_mean_saturation[first_bunch_next_section];
				}
			}//fbn<
			else
			{
				first_bunch_next = -1;
			}
		//if (last_bunch>0)
		//{//lb
			last_bunch_section =
				SectionTraceRight[last_strip*MAX_COL_INT + last_bunch_old];
			last_bunch_section = last_bunch_section % 1000;
			if (last_bunch_section>0)
			{
				last_bunch_section -= 1;
				last_left_sky_green = SkyGreenComponents[last_bunch_section];
				last_left_sky_green = last_left_sky_green % 10;
				last_bunch_cur_first =
					ColorSection->section_fibre_first[last_bunch_section];
				last_bunch_cur_first_ratio = (16 * last_bunch_cur_first) / (NumStrips - 1);
				last_bunch_cur_last =
					ColorSection->section_fibre_last[last_bunch_section];
				last_bunch_cur_last_ratio = (16 * last_bunch_cur_last) / (NumStrips - 1);
				last_bunch_cur_length = last_bunch_cur_last - last_bunch_cur_first + 1;
				last_bunch_cur_weight =
					ColorSection->section_weight[last_bunch_section];
				last_bunch_cur_weight_normalized =
					last_bunch_cur_weight / last_bunch_cur_length;
				last_bunch_cur_incline = *(InclinedComponent + last_bunch_section);
				last_bunch_cur_sat = ColorSection->section_mean_saturation[last_bunch_section];
			}
			number_of_covering_bunches = ColorInt[last_strip].NumberOfIntervalsInCovering;
			last_bunch_next = last_bunch + 1;
			if (last_bunch_next<number_of_covering_bunches)
			{//lbn<
				last_bunch_next_old =
					ColorInt[last_strip].old_ordered_bunch_number[last_bunch_next];
				last_bunch_next_old_beg =
					ColorInt[last_strip].ColoredIntervalsStructure->BegInterv[last_bunch_next_old];
				last_bunch_next_old_end =
					ColorInt[last_strip].ColoredIntervalsStructure->EndInterv[last_bunch_next_old];
				last_bunch_next_old_length = last_bunch_next_old_end - last_bunch_next_old_beg + 1;
				last_bunch_next_section =
					SectionTraceRight[last_strip*MAX_COL_INT + last_bunch_next_old];
				last_bunch_next_section = last_bunch_next_section % 1000;
				if (last_bunch_next_section>0)
				{//lbn>0
					last_bunch_next_section -= 1;
					last_left_next_sky_green = SkyGreenComponents[last_bunch_next_section];
					last_bunch_next_cur_first =
						ColorSection->section_fibre_first[last_bunch_next_section];
					last_bunch_next_cur_first_ratio = (16 * last_bunch_next_cur_first) / (NumStrips - 1);
					last_bunch_next_cur_last =
						ColorSection->section_fibre_last[last_bunch_next_section];
					last_bunch_next_cur_last_ratio = (16 * last_bunch_next_cur_last) / (NumStrips - 1);
					last_bunch_next_cur_length = last_bunch_next_cur_last - last_bunch_next_cur_first + 1;
					last_bunch_next_cur_weight =
						ColorSection->section_weight[last_bunch_next_section];
					last_bunch_next_cur_weight_normalized =
						last_bunch_next_cur_weight / last_bunch_next_cur_length;
					last_bunch_next_cur_incline = *(InclinedComponent + last_bunch_next_section);
					last_bunch_next_cur_sat = ColorSection->section_mean_saturation[last_bunch_next_section];
				}
			}//lbn<
			if ((first_strip_ratio <= 4) || (left_distance_closest_line >= 0))
			{
				if ((first_bunch_next >= 0) && (first_bunch_next_section >= 0))
				{
					if ((first_bunch_next_cur_last_ratio >= 6) && (first_bunch_next_cur_first_ratio <= 2))
					{
						if ((first_bunch_next_cur_weight >= 3 * DimX) &&
							(first_bunch_next_cur_weight_normalized >= DimX / 4))
						{
							if (line_length >= 4)
							{
								if (left_green_boun_bunch!= first_bunch_old)
								{
									if((abs(first_bunch_cur_incline)<=1)&&(first_bunch_next_cur_incline<=1))
									{//onlyublast_cor05.06.18
									RightBounVert[count_line] = 1;
									}//onlyublast_cor05.06.18
									//goto L;
								}
							}
						}
					}

				}
			}
				if ((line_length >= 4)&&(right_closest_line>=0))
				{//ll>=

					if ((first_bunch >= 0) && (first_bunch_section >= 0))
					{
						if (last_left_sky_green == 2)
						{
							if (!first_left_sky_green)
							{
								if ((first_bunch_cur_last_ratio >= 6) && (first_bunch_cur_first_ratio <= 2) &&
									(first_bunch_cur_sat <= 2))
								{
									if ((first_bunch_cur_weight >= 3 * DimX) &&
										(first_bunch_cur_weight_normalized >= DimX / 4))
									{
										if (line_length >= 4)
										{
											ConnectedLeftBounVert[count_line] = 1;
										}
									}
								}
								if ((first_bunch_cur_incline>0) && (first_bunch_cur_sat <= 2))
								{
									if (((first_bunch_cur_last - first_strip) >= 3) && ((first_bunch_cur_last - first_strip)<line_length))
									{
										ConnectedLeftBounVert[count_line] = 1;
									}

								}
							}
						}
					}
				}//ll>=
				/*if ((first_strip_ratio <= 5) && (last_strip_ratio >= 7))
				{
					;
				}
			if (abs(relative_first_last_deviation) <= StripWidth / 2)
			{
				if (right_closest_line>0)
				{
					;
				}
			}
			if (abs(relative_first_last_deviation)>(3 * StripWidth) / 4)
			{
				goto L;
			}*/
		//}
		intersection_length_left = 0;
		intersection_length = 0;
		for (int count_strip = first_strip; count_strip <= last_strip; count_strip++)
		{//strip_loop
			start_bunch = VerticalContrastCurves[24 * (count_line)+count_strip - first_strip];
			if (start_bunch>0)
			{//sb>
				start_bunch -= 1;
				start_bunch_old =
					ColorInt[count_strip].old_ordered_bunch_number[start_bunch];
				start_bunch_old_beg =
					ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[start_bunch_old];
				start_bunch_old_end =
					ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[start_bunch_old];
				last_bunch_old_length = last_bunch_old_end - last_bunch_old_beg + 1;
				last_bunch_section =
					SectionTraceRight[count_strip*MAX_COL_INT + start_bunch_old];
				last_bunch_section = last_bunch_section % 1000;
				if (last_bunch_section>0)
				{
					last_bunch_section -= 1;
					last_left_sky_green = SkyGreenComponents[last_bunch_section];
					last_left_sky_green = last_left_sky_green % 10;
					last_bunch_cur_first =
						ColorSection->section_fibre_first[last_bunch_section];
					last_bunch_cur_first_ratio = (16 * last_bunch_cur_first) / (NumStrips - 1);
					last_bunch_cur_last =
						ColorSection->section_fibre_last[last_bunch_section];
					last_bunch_cur_last_ratio = (16 * last_bunch_cur_last) / (NumStrips - 1);
					last_bunch_cur_length = last_bunch_cur_last - last_bunch_cur_first + 1;
					last_bunch_cur_weight =
						ColorSection->section_weight[last_bunch_section];
					last_bunch_cur_weight_normalized =
						last_bunch_cur_weight / last_bunch_cur_length;
					last_bunch_cur_incline = *(InclinedComponent + last_bunch_section);
					last_bunch_cur_sat = ColorSection->section_mean_saturation[last_bunch_section];
					number_of_covering_bunches = ColorInt[count_strip].NumberOfIntervalsInCovering;
				}
				last_bunch_next = start_bunch + 1;
				if (last_bunch_next<number_of_covering_bunches)
				{//lbn<
					last_bunch_next_old =
						ColorInt[count_strip].old_ordered_bunch_number[last_bunch_next];
					last_bunch_next_old_beg =
						ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[last_bunch_next_old];
					last_bunch_next_old_end =
						ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[last_bunch_next_old];
					last_bunch_next_old_length = last_bunch_next_old_end - last_bunch_next_old_beg + 1;
					last_bunch_next_section =
						SectionTraceRight[count_strip*MAX_COL_INT + last_bunch_next_old];
					last_bunch_next_section = last_bunch_next_section % 1000;
					if (last_bunch_next_section>0)
					{//lbns
						last_bunch_next_section -= 1;
						last_left_next_sky_green = SkyGreenComponents[last_bunch_next_section];
						last_bunch_next_cur_first =
							ColorSection->section_fibre_first[last_bunch_next_section];
						last_bunch_next_cur_first_ratio = (16 * last_bunch_next_cur_first) / (NumStrips - 1);
						last_bunch_next_cur_last =
							ColorSection->section_fibre_last[last_bunch_next_section];
						last_bunch_next_cur_last_ratio = (16 * last_bunch_next_cur_last) / (NumStrips - 1);
						last_bunch_next_cur_length = last_bunch_next_cur_last - last_bunch_next_cur_first + 1;
						last_bunch_next_cur_weight =
							ColorSection->section_weight[last_bunch_next_section];
						last_bunch_next_cur_weight_normalized =
							last_bunch_next_cur_weight / last_bunch_next_cur_length;
						last_bunch_next_cur_incline = *(InclinedComponent + last_bunch_next_section);
						next_incline_local = *(InclineLocal + last_bunch_next_section);
						next_incline_local_first = *(InclineLocalFirst + last_bunch_next_section);
						next_incline_local_last = *(InclineLocalLast + last_bunch_next_section);
						last_bunch_next_cur_sat = ColorSection->section_mean_saturation[last_bunch_next_section];
						if (abs(relative_first_last_deviation) <= StripWidth / 3)
						{//last_cor11.03.18rfld<
	if((first_strip_ratio <= 4) || (left_distance_closest_line >= 0))
							if ((last_bunch_next_cur_weight>5 * DimX) && (last_bunch_next_cur_weight_normalized >= DimX / 3))
							{
								if ((last_bunch_next_cur_first_ratio <= 1) && (last_bunch_next_cur_last_ratio >= 4) &&
									(last_bunch_next_cur_sat <= 2) && (last_left_sky_green != 2))
								{
									if (line_length >= 8)
									{
										//if ()
										//{
											RightBounVert[count_line] = 1;
											intersection_length++;
										//}
									}
								}
							}
						}//last_cor11.03.18rfld<
					}//lbns
					last_bunch_next_section_left =
						SectionTraceLeft[count_strip*MAX_COL_INT + last_bunch_next_old];
					last_bunch_next_section_left = last_bunch_next_section_left % 1000;
					if (last_bunch_next_section_left>0)
					{//lbnsl
						last_bunch_next_section_left -= 1;
						last_left_next_sky_green_left = SkyGreenComponents[last_bunch_next_section];
						last_bunch_next_cur_first_left =
							ColorSection->section_fibre_first[last_bunch_next_section_left];
						last_bunch_next_cur_first_ratio_left = (16 * last_bunch_next_cur_first_left) / (NumStrips - 1);
						last_bunch_next_cur_last_left =
							ColorSection->section_fibre_last[last_bunch_next_section_left];
						last_bunch_next_cur_last_ratio_left = (16 * last_bunch_next_cur_last) / (NumStrips - 1);
						last_bunch_next_cur_length_left = last_bunch_next_cur_last_left -
							last_bunch_next_cur_first_left + 1;
						last_bunch_next_cur_weight_left =
							ColorSection->section_weight[last_bunch_next_section_left];
						last_bunch_next_cur_weight_normalized_left =
							last_bunch_next_cur_weight_left / last_bunch_next_cur_length_left;
						last_bunch_next_cur_incline_left = *(InclinedComponent + last_bunch_next_section_left);
						next_incline_local_left = *(InclineLocal + last_bunch_next_section_left);
						next_incline_local_left_first = *(InclineLocalFirst + last_bunch_next_section_left);
						next_incline_local_left_last = *(InclineLocalLast + last_bunch_next_section_left);
						last_bunch_next_cur_sat_left = ColorSection->section_mean_saturation[last_bunch_next_section_left];
						if (abs(relative_first_last_deviation) <= StripWidth / 3)
						{//last_cor11.03.18rfld<
							if ((last_bunch_next_cur_first_left>(last_bunch_next_cur_length_left - 2)) &&
								(last_bunch_next_cur_last_ratio_left >= 7) &&
								(last_bunch_next_cur_first_ratio_left<last_bunch_next_cur_last_ratio_left) &&
								(last_left_sky_green != 2))
							{
								if (line_length >= 8)
								{
									RightBounVert[count_line] = 1;
									intersection_length_left++;
								}

							}
						}//last_cor11.03.18rfld<
					}//lbnsl
				}//lbn<
				left_sect_max = LeftGreenBoundarySection[count_strip];
				if (left_sect_max>0)
				{
					left_sect_max -= 1;
					left_sect_max_bunch = ColorDescrSect[left_sect_max].location_of_section[count_strip];
					if (left_sect_max_bunch>0)
					{
						left_sect_max_bunch -= 1;
						left_sect_max_bunch_old = ColorInt[count_strip].old_bunch_number[left_sect_max_bunch];
						left_sect_max_bunch_old_beg =
							ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[left_sect_max_bunch_old];
						left_sect_max_bunch_old_end =
							ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[left_sect_max_bunch_old];
					}
					else
					{
						left_sect_max_bunch_old_beg = 0;
						left_sect_max_bunch_old_end = 0;
					}

					left_sect_bound_bunch = LeftGreenBoundaryBunch[count_strip];
					if (left_sect_bound_bunch>0)
					{
						left_sect_bound_bunch -= 1;
						left_sect_bound_bunch_beg =
							ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[left_sect_bound_bunch];
						left_sect_bound_bunch_end =
							ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[left_sect_bound_bunch];
					}
					dist_to_left = start_bunch_old_end - left_sect_bound_bunch_end;
					dist_to_left1 = start_bunch_old_beg - left_sect_bound_bunch_end;
				}
				else
				{
					left_sect_max = -1;
					dist_to_left1 = start_bunch_old_beg;
					dist_to_left = start_bunch_old_end;
				}
				right_sect_max = RightGreenBoundarySection[count_strip];
				if (right_sect_max>0)
				{
					right_sect_max -= 1;
					right_sect_max_bunch = ColorDescrSect[right_sect_max].location_of_section[count_strip];
					right_sect_bound_bunch = RightGreenBoundaryBunch[count_strip];
					if (right_sect_bound_bunch>0)
					{
						right_sect_bound_bunch -= 1;
						right_sect_bound_bunch_beg =
							ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[right_sect_bound_bunch];
						right_sect_bound_bunch_end =
							ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[right_sect_bound_bunch];
					}
					else
					{
						right_sect_max_bunch_old_beg = 0;
						left_sect_max_bunch_old_end = 0;
					}
					if (right_sect_max_bunch>0)
					{
						right_sect_max_bunch -= 1;
						right_sect_max_bunch_old = ColorInt[count_strip].old_bunch_number[right_sect_max_bunch];
						right_sect_max_bunch_old_beg =
							ColorInt[count_strip].ColoredIntervalsStructure->BegInterv[right_sect_max_bunch_old];
						right_sect_max_bunch_old_end =
							ColorInt[count_strip].ColoredIntervalsStructure->EndInterv[right_sect_max_bunch_old];
						dist_to_right = right_sect_bound_bunch_beg - start_bunch_old_end;
						if (left_sect_max >= 0)
						{
							dist_left_right = right_sect_bound_bunch_beg - left_sect_bound_bunch_end;
						}
						else
						{
							dist_left_right = right_sect_bound_bunch_beg;
						}
					}
				}
				else
				{
					right_sect_max = -1;
					dist_to_right = DimX - start_bunch_old_end;
				}
				if (left_sect_max>0)
				{
					dist_left_ratio = (16 * dist_to_left) / dist_left_right;
					dist_right_ratio = (16 * dist_to_right) / dist_left_right;
					vert_line_bound = LeftGreenBoundaryVert[count_strip];
					vert_line_signal_right = ConnectedVertLinesRight[count_line];
					vert_line_signal_right_main = vert_line_signal_right % 100;
					vert_line_signal_right_additional =
						(vert_line_signal_right - vert_line_signal_right_main) / 100;

					if ((vert_line_bound>0) && (dist_left_ratio<dist_right_ratio) && (dist_left_ratio >= 0) &&
						(dist_left_ratio <= 6) && (!vert_line_signal_right))
					{
						count_vert_r_side++;
					}
				}
			}//sb>
		}//strip_loop
		count_vert_ratio = (16 * count_vert_r_side) / line_length;
		if (right_closest_line >= 0)
		{//last_cor28.03.18add conditions!
			if (count_vert_ratio >= 3)
			{
				if (abs(relative_first_last_deviation) <= StripWidth / 3)
				{//last_cor11.03.18
					ConnectedLeftBounVert[count_line] = 1;
				}
			}
		}//last_cor28.03.18
	L:;
	}//lines_loop1
}
//===================================================
void

CImageProcess::RoadComponents(void)
{
	int last_section;
	int number_of_left_sections;
	int last_fiber;
	int first_fiber;
	int section_length;
	int min_left;
	int max_right;
	int mean_lower;
	int mean_upper;
	int last_strip_ratio;
	int first_fiber_ratio;
	int section_hue;
	int section_hue_zone;
	int section_gray;
	int section_gray_zone;
	int section_sat;
	int	section_weight;
	int	section_weight_normalized;
	int	section_incline;
	int	section_incl_curve;
	int	section_vert;
	int last_bunch;
	int first_bunch;


	last_section = ColorSection->Number_of_sections;
	number_of_left_sections = ColorSection->Number_of_sections_left;

	for (int count_sec = 0; count_sec<last_section; count_sec++)
	{//loop_road_finding
		last_fiber = ColorSection->section_fibre_last[count_sec];
		first_fiber = ColorSection->section_fibre_first[count_sec];
		section_length = last_fiber - first_fiber + 1;
		min_left = ColorSection->section_left_end[count_sec];
		max_right = ColorSection->section_right_end[count_sec];
		//if(section_length>=4)
		//{
		//sect_geom=SectionGeometry(count_sec,&mean_lower,&mean_upper);
		mean_lower = ColorSection->section_lower_mean_length[count_sec];
		mean_upper = ColorSection->section_upper_mean_length[count_sec];
		last_strip_ratio = (16 * last_fiber) / (NumStrips - 1);
		first_fiber_ratio = (16 * first_fiber) / (NumStrips - 1);
		section_hue = ColorSection->section_mean_hue[count_sec];
		section_hue_zone = hue_zones[section_hue];
		section_gray = ColorSection->section_mean_gray[count_sec];
		section_gray_zone = gray_zones[section_gray];
		section_sat = ColorSection->section_mean_saturation[count_sec];
		section_weight = ColorSection->section_weight[count_sec];
		section_weight_normalized = section_weight / (last_fiber - first_fiber + 1);
		section_incline = InclinedComponent[count_sec];
		section_incl_curve = InclineCurve[count_sec];
		section_vert = *(VerticalComponent + count_sec);
		last_bunch = ColorSection->DescrSec[count_sec].location_of_section[last_fiber];
		first_bunch = ColorSection->DescrSec[count_sec].location_of_section[first_fiber];

	}//loop_road_finding
}
//===================================================

int

CImageProcess::VerticalComponentsInBoundary(int section_number, int* ends_coordinates,
	int* verticl_parts, int sign)
{
	int prior;

	prior = -1;
	return(prior);
}
//===================================================

int

CImageProcess::NextSection(int bunch_number, int strip_number, int* next_beg, int* next_end)
{
	int prior;
	int last_bunch;
	int shift_in_mem;
	int bunch_old_number;
	int beg_int;
	int end_int;
	int bunch_section_cur;
	int bunch_section_cur_first;
	int bunch_section_cur_last;
	int last_fiber_ratio;
	int first_fiber_ratio;
	int bunch_section_cur_length;
	int bunch_section_cur_weight;
	int bunch_section_cur_weight_normalized;
	int bunch_section_cur_incline;

	return(-1);
	beg_int = -1;
	end_int = -1;
	prior = -1;
	shift_in_mem = strip_number*MAX_COL_INT;
	last_bunch = ColorInt[strip_number].NumberOfIntervalsInCovering;
	for (int count_bunch = bunch_number + 1; count_bunch <= last_bunch; count_bunch++)
	{//loop_section_finding
		bunch_old_number = ColorInt[strip_number].old_ordered_bunch_number[count_bunch];
		beg_int = ColorInt[strip_number].ColoredIntervalsStructure->BegInterv[bunch_old_number];
		end_int = ColorInt[strip_number].ColoredIntervalsStructure->EndInterv[bunch_old_number];
		bunch_section_cur =
			SectionTraceRight[shift_in_mem + bunch_old_number];
		if (bunch_section_cur>0)
		{//bc>0
			bunch_section_cur -= 1;
			bunch_section_cur = bunch_section_cur % 1000;
			prior = count_bunch;
			bunch_section_cur_first =
				ColorSection->section_fibre_first[bunch_section_cur];
			bunch_section_cur_last =
				ColorSection->section_fibre_last[bunch_section_cur];
			last_fiber_ratio = (16 * bunch_section_cur_last) / NumStrips;
			first_fiber_ratio = (16 * bunch_section_cur_first) / NumStrips;
			bunch_section_cur_length = bunch_section_cur_last - bunch_section_cur_first + 1;
			bunch_section_cur_weight =
				ColorSection->section_weight[bunch_section_cur];
			bunch_section_cur_weight_normalized =
				bunch_section_cur_weight / bunch_section_cur_length;
			bunch_section_cur_incline = *(InclinedComponent + bunch_section_cur);
			*next_beg = beg_int;
			*next_end = end_int;
			return(prior);
		}//bc>0
	}//loop_section_finding
	return(prior);
}
