/*
 *
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
#include <math.h>
#include <string.h>
#include <time.h>
#include <algorithm>






using namespace std;



static  int hue_zones[NUM_HUES] = {1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
5, 1, 1, 1, 1, 1};
static  int dev_hue_zone[6] = {2, 2, 2, 2, 2, 2};
static  int dev_mean_sat[16] = {1, 1, 1, 1, 1, 2, 3, 4, 4, 4, 4, 4, 3, 2, 1, 0};
static  int dev_mean_sat1[16] = {0, 1, 1, 1, 1, 1, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4};
static  int imp_dev_mean_sat[16] = {3, 3, 3, 3, 4, 5, 6, 6, 6, 6, 6, 5, 4, 4, 4, 4};
static  int imp_dev_mean_sat1[16] = {1, 2, 2, 3, 3, 3, 3, 4, 5, 5, 5, 6, 6, 7, 6, 6};

static  int gray_zones[NUM_INTEN1] = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2,
2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7};
//0. 0-6, 1. 7-12, 2. 13-21, 3. 22-30, 4. 31-39, 5. 40-49, 6. 50-57, 7. 58-63
static  int dev_mean[8] = {5, 8, 8, 10, 10, 10, 12, 8};
static  int imp_dev_mean[8] = {14, 16, 18, 18, 16, 16, 14, 12};





int CImageProcess::MaximumGreenComp(int left_right)
{
    int prior;
		int first_section;
		int last_section;
		int last_fiber;
		int first_fiber;
		int number_of_left_sec;
		int tot_num_sec;
		int min_left;
		int max_right;
		int width_length;
		int	fiber_length;
		int shape_ratio;
		int last_srip_ratio;
		int first_strip_ratio;
		int invert_shape_ratio;
		int weight_of_section;
		int section_hue;
		int section_hue_zone;
		int section_gray;
		int section_gray_zone;
		int section_saturation;
		int section_green_max;
		int weight_max;
		int section_green_max_lb;
		int weight_max_lb;
		int section_green_max_rb;
		int weight_max_rb;
		int adjacent_contrast_object;

			    adjacent_contrast_object=-1;
		        prior=0;
				weight_max=0;
		        section_green_max=-1;
				weight_max_lb=0;
				section_green_max_lb=-1;
				weight_max_rb=0;
		        section_green_max_rb=-1;
				number_of_left_sec=ColorSection->Number_of_sections_left;
				tot_num_sec=ColorSection->Number_of_sections;
		if(left_right)
		{
		first_section=0;
		last_section=number_of_left_sec;
		}
		else
		{
		first_section=number_of_left_sec;
		last_section=tot_num_sec;
		}

		for(int count_sec=first_section;count_sec<last_section;count_sec++)
				{//loop_green_finding

						if(*(SkyGreenComponents+count_sec)==1)
						{
							goto L;
						}
			    last_fiber=ColorSection->section_fibre_last[count_sec];
				first_fiber=ColorSection->section_fibre_first[count_sec];
				/*last_bunch=ColorSection->DescrSec[count_sec].location_of_section[last_fiber];
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
				}//fb>*/
				section_hue=ColorSection->section_mean_hue[count_sec];
		        section_hue_zone=hue_zones[section_hue];
		        section_gray=ColorSection->section_mean_gray[count_sec];
		        section_gray_zone=gray_zones[section_gray];
		        section_saturation=ColorSection->section_mean_saturation[count_sec];
				if((section_hue_zone<2)||(section_hue_zone>3)||(section_hue>21)||(section_saturation<=1))
				{
					goto L;
				}
				if((section_gray_zone>=6)&&(section_saturation>=3))
				{
					goto L;
				}
		if((section_gray_zone>=2)&&(section_saturation<=3)&&(section_hue>=18))
				{//last_cor03.11.15
					goto L;
				}
		if((section_gray_zone>=2)&&(section_saturation<=1)&&(section_hue>=16))
				{//last_cor03.11.15
					goto L;
				}

				min_left=ColorSection->section_left_end[count_sec];
			    max_right=ColorSection->section_right_end[count_sec];
				width_length=max_right-min_left+1;
				fiber_length=last_fiber-first_fiber+1;
				shape_ratio=width_length/(StripWidth*fiber_length);
				last_srip_ratio=(16*last_fiber)/(NumStrips-1);
				first_strip_ratio=(16*first_fiber)/(NumStrips-1);
				invert_shape_ratio=(StripWidth*fiber_length)/width_length;
				weight_of_section=ColorSection->section_weight[count_sec];
				if(!last_srip_ratio)
				{
					goto L;
				}
				if((weight_of_section>200)||((section_hue_zone==3)&&(section_saturation>=4))||
		((section_hue>=10)&&(section_hue<=18)&&(section_gray_zone>=2)&&((section_saturation>=3)||
		 ((section_saturation==2)&&(section_gray_zone>=4)))))
				{//last_cor15.11.16
		adjacent_contrast_object=OpposGreenComponents(left_right,count_sec);
				}
				if((weight_of_section>weight_max)&&(section_saturation>=3))
				{
		weight_max=weight_of_section;
		section_green_max=count_sec;
		prior=1;
				}
		//if(!first_section)
		//{//fs0
			if(min_left<DimX/3)
			{
				if((weight_of_section>weight_max_lb)&&(section_saturation>=4))
				{
		weight_max_lb=weight_of_section;
		section_green_max_lb=count_sec;
		prior=1;
				}
			}
			if(max_right>2*DimX/3)
			{
				if((weight_of_section>weight_max_rb)&&(section_saturation>=3))
				{
		weight_max_rb=weight_of_section;
		section_green_max_rb=count_sec;
		prior=1;
				}
			}

		//}//fs0


		L:;

			     }//loop_green_finding
		if(!first_section)
		{
			GreenMainLeft=section_green_max;
			GreenMainLeftLb=section_green_max_lb;
		    GreenMainLeftRb=section_green_max_rb;
		}
		else
		{
			GreenMainRight=section_green_max;
			GreenMainRightLb=section_green_max_lb;
		    GreenMainRightRb=section_green_max_rb;
		}
		return(prior);
		}




int CImageProcess::OpposGreenComponents(int left_right,int section_number)
{
    int section_beg;
    int section_end;

    int count_fiber;
		int section_bunch;
		int bunch_old_number;
		int right_contrast;
		int right_adjacent;
		int left_contrast;
		int left_adjacent;
		int oppos_contrast;
		int oppos_adjacent;
		int number_sect;
		int num_sect_right;
		int beg_int;
		int end_int;
		int number_of_bunches;
		int number_of_bunches_prev;
		int number_of_bunches_prev1;
		int number_of_bunches_next;
		int section_hue;
		int section_sat;
		int section_hue_zone;
		int section_gray;
		int section_weight;
		int Intersection;
	    int indic_length;
	    int ratio_length;
	    int ratio_length1;
		int section_previous;
		int extreme_left_boundary_count;
		int extreme_right_boundary_count;
		int oppos_extreme_left_boundary_count;
		int oppos_extreme_right_boundary_count;
		int section_length;
		int section_gray_zone;
		int prior;
		int first_green_adjacent;
		int last_green_adjacent;
		int maximum_non_green_adjacent;
		int non_green_length;
		int first_n_green;
		int last_n_green;
		int last_gr_in_chain;
		int first_green_chain_length;
		int success_oppos;
		int bunch_ordered_number;
		int bunch_adjacent_ordered;
		int number_of_covering_bunches;
		int disordering;
		int n_g_first;
		int n_g_last;
		int n_g_sum;
		int without_green_adjacent;
		int array_num;
		int total_with_green_adjacent;
		int old_first_green_adjacent;
	    int old_first_beg;
		int old_first_end;
		int old_first_length;
		int old_first_signif;
		int old_first_density;
		int green_adjacent_section;
		int green_adjacent_section_weight;
		int green_adjacent_section_beg;
		int green_adjacent_section_end;
		int green_adjacent_section_length;
		int number_of_right_adjacent_green_sections;
		int number_of_dif_right_adjacent_green_sections;
		int old_first_n_green_adjacent;
	    int old_first_n_beg;
		int old_first_n_end;
		int old_first_n_length;
		int old_first_n_signif;
		int old_first_n_density;
		int n_green_adjacent_section;
		int last_green_adjacent_section;
		int max_green_adj_section;
	    int max_green_adj_section_bunch;
		int max_n_green_adj_section;
		int max_n_green_adj_section_bunch;
		int first_green_adjacent_oppos;
		int last_green_adjacent_oppos;
		int maximum_non_green_adjacent_oppos;
		int non_green_length_oppos;
	    int first_n_green_oppos;
		int last_n_green_oppos;
		int last_gr_in_chain_oppos;
		int first_green_chain_length_oppos;
	    int max_green_adj_section_oppos;
		int max_green_adj_section_bunch_oppos;
		int max_n_green_adj_section_oppos;
		int max_n_green_adj_section_bunch_oppos;
		int first_in_ordering_old_number;
		int left_adjacent_ordered;
		int right_adjacent_ordered;
		int success_inverse;
		int sec_ordered_bunch;
		int sc_gr_comp;
		int sc_gr_comp_red;
		int old_oppos_adjacent;
	    int op_adj_beg;
	    int op_adj_end;
		int mng_old;
	    int mng_beg;
		int mng_end;
	    int mng_length;
	    int mng_section;
	    int mng_section_weight;
	    int mng_section_first;
	    int mng_section_last;
		int mng_section_oppos;
	    int mng_section_weight_oppos;
	    int mng_section_first_oppos;
	    int mng_section_last_oppos;
		int w_mng_old;
	    int w_mng_beg;
		int w_mng_end;
	    int w_mng_length;
	    int w_mng_section_weight;
	    int w_mng_section_first;
	    int w_mng_section_last;
		int w_mng_section_oppos;
		int w_mng_section_oppos_bunch;
	    int w_mng_section_weight_oppos;
	    int w_mng_section_first_oppos;
	    int w_mng_section_last_oppos;
		int w_mg_old;
	    int w_mg_beg;
		int w_mg_end;
	    int w_mg_length;
	    int w_mg_section_weight;
	    int w_mg_section_first;
	    int w_mg_section_last;
		int w_mg_section_oppos;
	    int w_mg_section_oppos_bunch;
	    int w_mg_section_weight_oppos;
	    int w_mg_section_first_oppos;
	    int w_mg_section_last_oppos;
		int f_g_bunch_length_ratio;
		int f_g_f_n_length_ratio;
		int f_g_f_n_signif_ratio;
		int section_bunch_previous;
		int section_bunch_previous1;
		int section_bunch_next;
		int bunch_old_number_previous;
		int bunch_old_number_previous1;
		int bunch_old_number_next;
		int beg_previous;
		int end_previous;
		int beg_previous1;
		int end_previous1;
		int	beg_next;
		int end_next;
		int int_length;
		int	fiber_residual;
		int fiber_residual_new;
		int side_fiber_residual;
		int sect_incline;
		int sect_vert;
		int residual_ratio1;
		int residual_ratio2;
		int residual_ratio3;
		int entrance_to_oppos_border;
		int entrance_to_oppos_border_now;
		int old_last_gr_in_chain_oppos;
		int oppos_green_beg;
		int old_last_gr_in_chain_oppos_beg;
	    int oppos_green_end;
		int old_last_gr_in_chain_oppos_end;
		int old_first_green_adjacent_oppos;
		int old_first_green_adjacent_oppos_beg;
		int old_first_green_adjacent_oppos_end;
		int number_of_boundary_points;
		int oppos_green_length;
		int previous_success;
		int min_left;
		int max_right;
		int	width_length;
		int	fiber_length;
		int	shape_ratio;
		int	last_srip_ratio;
		int	first_strip_ratio;
		int	invert_shape_ratio;
		int convex_dif;
		int abs_convex_dif;
		int convex_dif_new;
		int abs_convex_dif_new;
		int old_first_n_green_adjacent_oppos;
		int old_first_n_green_adjacent_oppos_beg;
		int old_first_n_green_adjacent_oppos_end;
		int old_first_n_green_adjacent_oppos_length;
	    int first_n_green_adjacent_oppos_sec;
		int green_adjacent_section_oppos;
	    int green_adjacent_section_oppos_weight;
		int number_of_boun_points_in_oppos;
		int green_adjacent_section_oppos_beg;
		int green_adjacent_section_oppos_end;
		int green_adjacent_section_oppos_length;
		int adj_sec_green[8];
		int count_adj_green_sec;
		int green_adjacent_section_previous;
		int adj_sec_green_oppos[8];
		int count_adj_green_sec_oppos;
		int green_adjacent_section_oppos_previous;
		int adj_sec_n_green_max[8];
	    int adj_sec_green_max[8];
		int count_adj_n_green_sec_max;
		int count_adj_green_sec_max;
		int n_green_adjacent_section_previous;
		int adj_sec_n_green_max_oppos[8];
		int adj_sec_green_max_oppos[8];
		int count_adj_n_green_sec_max_oppos;
		int count_adj_green_sec_max_oppos;
		int n_green_adjacent_section_previous_oppos;
		int green_adjacent_section_previous_oppos;
		int n_green_adjacent_section_weight;
		int n_green_adjacent_section_weight_normalized;
		int boun_point_num;
		int f_g_f_n_length_ratio1;
		int max_adj_sec;
	    int max_adj_sec_weight;
		int closeness_to_boundary;
		int first_n_green_adjacent_oppos_sec_weight;
	    int first_n_green_adjacent_oppos_sec_weight_normalized;
	    int first_n_green_adjacent_oppos_sec_beg;
		int first_n_green_adjacent_oppos_sec_end;
		int first_n_green_adjacent_oppos_sec_length;
		int n_green_adjacent_section_beg;
		int n_green_adjacent_section_end;
		int n_green_adjacent_section_length;
		int first_max_adj_sec_strip;
		int last_max_adj_sec_strip;
		int lenght_dir_oppos_n_green_ratio;
		int length_dir_oppos_ng_g_ratio;
		int old_first_green_adjacent_oppos_length;
		int green_adjacent_section_weight_normalized;
		int green_max_n_ratio;
		int int_signif;
	    int int_density;
		int dist_next_green;
	    int dist_next_green_previous;
		int w_mng_section_sky;
		int w_mng_section_sky_oppos;
		int dist_to_wmng;
	    int dist_to_wmng1;
		int dist_to_wmng_previous;
	    int dist_to_wmg_previous;
		int dist_to_wmg;
		int section_weight_normalized;
		int sect_persp;
		int section_first_bunch;
		int section_last_bunch;
		int section_first_bunch_beg;
		int section_last_bunch_beg;
		int section_first_bunch_end;
		int section_last_bunch_end;
		int section_first_bunch_length;
		int section_last_bunch_length;
		int first_second_sec_ratio;
		int new_neighbor;
		int old_new_neighbor;
	    int old_new_neighbor_beg;
		int old_new_neighbor_end;
		int old_new_neighbor_length;
	    int old_new_neighbor_sat;
		int old_new_neighbor_hue;
	    int old_new_neighbor_gray;
		int b_a_o_neighbor_beg;
		int b_a_o_neighbor_end;
		int b_a_o_neighbor_length;
		int n_n_gr_ratio;
		int hue_close;
		int saturation_close;
		int gray_close;
		int n_o_g_contrast;
		int bunch_occurrence;
		int new_neighbor_hue_zone;
		int b_a_old;
		int differ_prev;
		int differ_prev1;
		int differ_next;
		int differ_prev_new;
		int w_mng_section_oppos_incline;
		int w_mng_section_incline;
	    int w_mg_section_oppos_incline;
		int w_mg_section_incline;
		int f_g_bunch_density_ratio;
		int accumulated_weight;
		int nearest_green_sect;
		int section_fit;
		int number_of_boun_points_in_max_adj;
		int w_mg_sec_length;
	    int w_mg_boun_point_ratio;
		int max_adj_sec_green;
	    int max_adj_sec_green_weight;
	    int first_max_adj_sec_green_strip;
	    int last_max_adj_sec_green_strip;
		int w_mg_max_old;
		int w_mg_max_old_ordered;
		int mgm_incline;
		int w_mg_section_oppos1;
	    int w_mg_section_oppos_bunch1;
		int w_mng_section_oppos1;
		int w_mng_section_oppos_bunch1;
		int adjacent_boundary;
		int residual_ratio10;
		int residual_ratio20;
		int residual_ratio30;
		int old_first_green_adjacent_lg;
	    int old_first_beg_lg;
		int old_first_end_lg;
	    int old_first_length_lg;
	    int old_first_signif_lg;
	    int old_first_density_lg;
	    int f_g_bunch_length_ratio_lg;
	    int f_g_bunch_density_ratio_lg;
	    int n_o_g_contrast_lg;
	    int indic1_length;
		int ratio1_length;
		int ratio1_length1;
		int hue1_close;
		int saturation1_close;
		int gray1_close;
		int fiber_residual_new_lg;
	    int convex_dif_new_lg;
		int residual_ratio2_lg;
	    int residual_ratio3_lg;
	    int residual_ratio10_lg;
	    int abs_convex_dif_new_lg;
		int dist_to_frame_bound_last;
		int w_mg_section_weight_normalized;
	    int w_mg_section_hue;
	    int w_mg_section_sat;
	    int w_mg_section_gray;
	    int w_mg_section_hue_zone;
	    int w_mg_section_gray_zone;
		int w_mg_current_sec_weight_ratio;
		int green_adjacent_section_hue;
	    int green_adjacent_section_sat;
	    int green_adjacent_section_gray;
	    int green_adjacent_section_hue_zone;
	    int green_adjacent_section_gray_zone;
	    int green_adjacent_section_incline;
		int differ_prev_new_fg;
		int differ_prev_new_lg;
		int rt_differ;
		int extreme_right_boundary_now;
		int extreme_left_boundary_now;
		int number_of_roof_green;
		int last_roof_green;
		int boundary_bunch;
	    int left_end_bb;
		int right_end_bb;
		//int differ_boundary;
		int section_boun_prev;
		int number_bound_points;
		int section_beg_prev;
	    int section_end_prev;
	    int section_length_prev;
		//int m_n_g_f_prev_sec;
		int bound_b_existing;
	    int sect_b_existing;
	    int section_max_existing;
		int section_max_existing_bunch;
		int previous_mnga;
		int previous_mnga_bunch;
		int dist1gr_to_wmng;
		int sect_incl_curve;
		int differ_boundary_sign;
		int dist_wmng_to_mga;
		int dist_wmng_oppos_to_sect;
		int w_mg_incl_curve;
		int w_mg_incl_curve_oppos;
		int w_mng_incl_curve;
	    int w_mng_hue;//last_cor31.10.16
	    int w_mng_sat;
	    int w_mng_gray;
	    int w_mng_hue_zone;
	    int w_mng_gray_zone;//last_cor31.10.16
		int w_mng_incl_curve_oppos;
		int w_mng_old_oppos;
		int w_mng_beg_oppos;
	    int w_mng_end_oppos;
	    int w_mng_length_oppos;
	    int w_mng_hue_oppos;//last_cor31.10.16
	    int w_mng_sat_oppos;
	    int w_mng_gray_oppos;
	    int w_mng_hue_zone_oppos;
	    int w_mng_gray_zone_oppos;//last_cor31.10.16
		int w_mng_section_first_ratio;
	    int w_mng_section_last_ratio;//last_cor31.10.16
		int w_mng_section_first_ratio_oppos;
	    int w_mng_section_last_ratio_oppos;//last_cor31.10.16
		int prev_mnga_bunch_beg;
		int prev_mnga_bunch_end;
		int differ_boundary_abs;
	    int differ_boundary_oppos;
	    int differ_boundary_oppos_abs;
		int green_adjacent_section_op;
		int	green_adjacent_section_op_oppos;
		int green_adjacent_section_op_weight;
		int green_adjacent_section_op_beg;
		int green_adjacent_section_op_end;
		int green_adjacent_section_op_length;
		int green_adjacent_section_op_incline;
	    int green_adjacent_section_op_incl_curve;
		int number_of_boundary_points_adjacent_oppos;
		int last_left_bound_fiber;
		int last_right_bound_fiber;
		int condition_number;
		int success_left;
		int success_right;
		int differ_bb_fga;
		int prev_boun_bunch;
	    int left_end_bb_prev;
	    int right_end_bb_prev;
	    int differ_boundary_sign_prev;
	    int differ_boundary_abs_prev;
		int new_boundary_section;
		int w_mng_section_weight_normalized;
		int bunch_adjacent_ordered_old;
	    int bunch_adjacent_ordered_old_beg;
	    int bunch_adjacent_ordered_old_end;
	    int bunch_adjacent_ordered_old_length;
	    int bunch_adjacent_ordered_old_hue;
	    int bunch_adjacent_ordered_old_sat;
	    int bunch_adjacent_ordered_old_gray;
	    int bunch_adjacent_ordered_old_hue_zone;
	    int bunch_adjacent_ordered_old_gray_zone;
	    int bunch_adjacent_ordered_section;
	    int bunch_adjacent_ordered_section_oppos;
		int green_adjacent_section_incl_curve;
		int bunch_hue;//last_cor22.11.16
	    int bunch_sat;
	    int bunch_gray;
	    int bunch_hue_zone;
	    int bunch_gray_zone;
		int green_bunch_finding;//last_cor08.12.16
		int last_gr;
		int tot_gr_length;
		int first_gr;
	    int last_section_gr_bunch;
		int last_gr_section;
		int first_gr_section_bunch;
	    int first_gr_section;//last_cor08.12.16
		int green_oppos_boundary_sec;//last_cor09.12.16
	    int green_oppos_boundary_bunch;//last_cor09.12.16
		int green_oppos_boundary_bunch_beg;
		int green_oppos_boundary_bunch_end;
		int last_gr_old;//!12.12.16
		    	int last_gr_beg;//!last_cor!12.12.16
		    	int last_gr_end;//!last_cor!12.12.16
		    	int last_gr_section_bunch_old;
		    	int last_gr_section_bunch_beg;
		    	int last_gr_section_bunch_end;//!last_cor!12.12.16
		    	int dist_form_lt_bunch_to_wmng;
		    	int dist_form_lt_sec_bunch_to_wmng;
				int prev_sec_ng_max;//last_cor21.12.16
	            int prev_sec_g_max;//last_cor21.12.16
				int right_oppos_green_max;//last_cor27.12.16
				int Intersection_green;//last_cor10.01.17
	            int indic_length_green;
				int ratio_length_green;
				int ratio_length1_green;
				int Intersection_green1;//last_cor14.04.17
		            int indic_length_green1;
					int ratio_length_green1;
					int ratio_length1_green1;//last_cor14.04.17
				int section_last_bunch_new_num;
				int mgso_weight;//last25.01.17
		int mgso_incl;
		int mgso_incl_curve;
		int mgso_hue;
		int mgso_sat;
		int mgso_gray;
		int mgso_hue_zone;
		int mgso_gray_zone;
		int mgso_col_comp;
		int mgso_beg;
		int mgso_end;
		int mgso_length;
		int section_boun_prev_bunch;
		int green_lg_before;
		int green_lg_before1;
		int prev_mnga_bunch_length;
		int dist_fga_wmg;
		int prev_sec_ng_max_end;
	    int prev_sec_ng_max_beg;
		int first_gr_section_weight;//last_cor21.03.17
		int last_gr_section_weight;
		int dist_gobs_mgas;
		int	dist_gobs_mgas_abs;
		int section_bunch_shift;
		int section_bunch_shift_abs;
		int w_mng_section_oppos2;//last_cor18.04.17
		int w_mng_section_oppos2_weight;
		int w_mng_section_oppos2_incline;
		int	w_mng_section_oppos2_incl_curve;
		int w_mng_section_oppos2_beg;
	     int w_mng_section_oppos2_end;//last_cor18.04.17
		 int w_mg_vert;
		 int w_mg_vert_oppos;
		 int w_mg_section_oppos2;
		 int w_mg_section_weight_oppos2;
	     int w_mg_section_first_oppos2;
	     int w_mg_section_last_oppos2;
	     int w_mg_section_oppos_incline2;
	     int w_mg_incl_curve_oppos2;
	     int w_mg_vert_oppos2;
		 int w_mg_section_color;//last_cor02.05.17
		 int mng_hue;//last_cor05.05.17
	     int mng_sat;
	     int mng_gray;
		 int mng_hue_zone;
		 int mng_gray_zone;
		 //onlyub
		 int prev_mnga_bunch_hue;
		 int prev_mnga_bunch_hue_zone;
		 int prev_mnga_bunch_gray;
		 int prev_mnga_bunch_gray_zone;
		 int upper_section;//onlyublast_cor22.08.17
		  int non_green_section_max;
		  int non_green_section_max_incline;
		  int non_green_section_max_incl_curve;
		  int non_green_section_max_weight;
		  int non_green_section_max_first;
		  int non_green_section_max_first_bunch;
		  int non_green_section_max_first_bunch_beg;
		  int non_green_section_max_first_bunch_end;
		  int non_green_section_max_last;
		  int non_green_section_max_last_bunch;
		  int non_green_section_max_last_bunch_beg;
		  int non_green_section_max_last_bunch_end;
		  int upper_section_weight;
		 int upper_section_fiber_first;
		 int upper_section_incline;
		 int upper_section_incl_curve;
		 int upper_section_bunch;
		 int upper_sectiom_buch_beg;
		 int upper_section_bunch_end;//onlyublast_cor22.08.17
		 int prev_mnga_bunch_sat;
		 int l_side_boun;//last_cor19.06.17
		 int r_side_boun;
         int oppos_boun_sec;//last_cor19.07.17
		 int boundary_bunch_oppos;
		 int boundary_bunch_oppos_beg;
		 int boundary_bunch_oppos_end;//last_cor19.07.17
		 int bao_scgr;//last_cor20..11.17
         int bao_section_weight;
         int bao_section_first;
	     int bao_section_last;
	     int bao_section_weight_normalized;
	     int bao_section_first_ratio;
	     int bao_section_last_ratio;
	     int bao_section_incline;
		 int sect_incline_local;//last_cor14.02.18
		 int local_beg;
		 int local_end;//last_cor14.02.18
		int* distance_to_green;
		int* green_bunch_number;
		int* adjacent_n_green_length;
		int* green_section_number;
		int* max_gr_adj_sec;
		int* max_gr_adj_sec_bunch;
		int* sec_bunches_ordered_numbers;
		int* first_n_g_bunch;
		int* max_n_green_bunch;
		int* green_bunch_number_oppos;
		int* adjacent_n_green_length_oppos;
		int* green_section_number_oppos;
		int* max_gr_adj_sec_oppos;
		int* max_gr_adj_sec_bunch_oppos;
		int* max_n_green_bunch_oppos;

		 sect_incline_local=0;//last_cor14.02.18
		 local_beg=-1;
		 local_end=-1;//last_cor14.02.18
		upper_section=-1;//onlyublast_cor22.0817
		            non_green_section_max=-1;
		            non_green_section_max_incline=0;
		            non_green_section_max_incl_curve=0;
		           	non_green_section_max_weight=-1;
		           	non_green_section_max_first=-1;
		           	non_green_section_max_last=-1;
		           	non_green_section_max_first_bunch=-1;
		           	non_green_section_max_first_bunch_beg=-1;
		           	non_green_section_max_first_bunch_end=-1;
		           	non_green_section_max_last_bunch=-1;
		           	non_green_section_max_last_bunch_beg=-1;
		           	non_green_section_max_last_bunch_end=-1;
		            upper_section_weight=-1;
					upper_section_fiber_first=-1;
					upper_section_incline=0;
					upper_section_incl_curve=0;
					upper_section_bunch=-1;
					upper_sectiom_buch_beg=-1;
					upper_section_bunch_end=-1;//onlyublast_cor22.0817
		//onlyub17.05.17
		//onlyub
		        boundary_bunch_oppos=-1;
		     boundary_bunch_oppos_beg=-1;
			 boundary_bunch_oppos_end=-1;//last_cor19.07.17
				oppos_boun_sec=-1;//last_cor19.07.17
		        l_side_boun=0;
		        r_side_boun=0;//last_cor19.06.17
			    prev_mnga_bunch_hue=-1;
				prev_mnga_bunch_hue_zone=-1;
				prev_mnga_bunch_gray=-1;
				prev_mnga_bunch_gray_zone=-1;
				prev_mnga_bunch_sat=-1;
		mng_hue=-1;//last_cor05.05.17
	    mng_sat=-1;
	    mng_gray=-1;
		mng_hue_zone=-1;
		mng_gray_zone=-1;
		w_mg_section_color=0;//last_cor02.05.17
		w_mg_section_oppos2=-1;//last_cor24.04.17
		 w_mg_section_weight_oppos2=-1;
	     w_mg_section_first_oppos2=-1;
	     w_mg_section_last_oppos2=-1;
	     w_mg_section_oppos_incline2=0;
	     w_mg_incl_curve_oppos2=0;
	     w_mg_vert_oppos2=0;
		w_mg_vert_oppos=-1;//last_cor24.04.17
		w_mg_vert=-1;
		w_mng_section_oppos2_beg=-1;//last_cor18.04.17
	    w_mng_section_oppos2_end=-1;
		w_mng_section_oppos2_weight=-1;
		w_mng_section_oppos2_incline=0;
		w_mng_section_oppos2_incl_curve=0;
		Intersection_green1=-1;//last_cor14.04.17
		    indic_length_green1=-1;
			ratio_length_green1=-1;
			ratio_length1_green1=-1;//last_cor14.04.17
		prev_mnga_bunch_length=-1;
		prev_sec_ng_max_end=-1;
	    prev_sec_ng_max_beg=-1;
		section_boun_prev_bunch=-1;
		mgso_weight=0;//last_cor 25.01.17
		mgso_incl=0;
		mgso_incl_curve=0;
		mgso_hue=-1;
		mgso_sat=-1;
		mgso_gray=-1;
		mgso_hue_zone=-1;
		mgso_gray_zone=-1;
		mgso_col_comp=0;
		mgso_beg=-1;
		mgso_end=-1;
		mgso_length=-1;
		section_last_bunch_new_num=-1;
		Intersection_green=-1;//last_cor10.01.17
	    indic_length=-1;
		ratio_length=-1;
		ratio_length1=-1;
		right_oppos_green_max=-1;//last_cor27.12.16
		prev_sec_ng_max=-1;//last_cor21.12.16
	    prev_sec_g_max=-1;
		dist_form_lt_bunch_to_wmng=-1;
		dist_form_lt_sec_bunch_to_wmng=-1;
		last_gr_section_bunch_old=-1;
		last_gr_section_bunch_beg=-1;
		last_gr_section_bunch_end=-1;
			            last_gr_beg=-1;//!last_cor!12.12.16
						last_gr_end=-1;
						last_gr_old=-1;
		green_oppos_boundary_bunch_beg=-1;
		green_oppos_boundary_bunch_end=-1;
		green_oppos_boundary_bunch=-1;//last_cor09.12.16
		green_oppos_boundary_sec=-1;
		last_gr=-1;//last_cor08.12.16
		tot_gr_length=0;
		first_gr=-1;
	    last_section_gr_bunch=-1;
		last_gr_section=-1;
		first_gr_section_bunch=-1;
	    first_gr_section=-1;
		green_bunch_finding=-1;//last_cor08.12.16
		bunch_hue=-1;//last_cor22.11.16
	    bunch_sat=-1;
	    bunch_gray=-1;
	    bunch_hue_zone=-1;
	    bunch_gray_zone=-1;
		bunch_adjacent_ordered_old=-1;
	bunch_adjacent_ordered_old_beg=-1;
	bunch_adjacent_ordered_old_end=-1;
	bunch_adjacent_ordered_old_length=-1;
	bunch_adjacent_ordered_old_hue=-1;
	bunch_adjacent_ordered_old_sat=-1;
	bunch_adjacent_ordered_old_gray=-1;
	bunch_adjacent_ordered_old_hue_zone=-1;
	bunch_adjacent_ordered_old_gray_zone=-1;
	bunch_adjacent_ordered_section=-1;
	bunch_adjacent_ordered_section_oppos=-1;
		condition_number=-1;
		differ_boundary_sign=-DimX;
		section_max_existing_bunch=-1;
		bound_b_existing=-1;
	    sect_b_existing=-1;
	    section_max_existing=-1;
		//m_n_g_f_prev_sec=-1;
		section_beg_prev=-1;
	    section_end_prev=-1;
	    section_length_prev=-1;
		section_boun_prev=-1;
		number_bound_points=-1;
	    //differ_boundary=-1;
		boundary_bunch=-1;
	    left_end_bb=-1;
		right_end_bb=-1;
		green_adjacent_section_hue=-1;
	    green_adjacent_section_sat=-1;
	    green_adjacent_section_gray=-1;
	    green_adjacent_section_hue_zone=-1;
	    green_adjacent_section_gray_zone=-1;
	    green_adjacent_section_incline=-1;
		w_mg_section_weight_normalized=-1;
	    w_mg_section_hue=-1;
	    w_mg_section_sat=-1;
	    w_mg_section_gray=-1;
	    w_mg_section_hue_zone=-1;
	    w_mg_section_gray_zone=-1;
		residual_ratio2_lg=-1;
	    residual_ratio3_lg=-1;
	    residual_ratio10_lg=-1;
	    abs_convex_dif_new_lg=-1;
		fiber_residual_new_lg=-1;
	    convex_dif_new_lg=-DimX;
		old_first_green_adjacent_lg=-1;
	    old_first_beg_lg=-1;
		old_first_end_lg=-1;
	    old_first_length_lg=-1;
	    old_first_signif_lg=-1;
	    old_first_density_lg=-1;
	    f_g_bunch_length_ratio_lg=-1;
	    f_g_bunch_density_ratio_lg=-1;
	    n_o_g_contrast_lg=-1;
	    indic1_length=-1;
		ratio1_length=-1;
		ratio1_length1=-1;
		hue1_close=-1;
		saturation1_close=-1;
		gray1_close=-1;
		nearest_green_sect=-1;
		dist_next_green_previous=-1;
		dist_to_wmng_previous=-1;
	    dist_to_wmg_previous=-1;
		Intersection=3;
	    indic_length=1;
	    ratio_length=0;
	    ratio_length1=0;
		hue_close=-1;
		saturation_close=-1;
		gray_close=-1;
		convex_dif=-DimX;
		previous_success=-1;
		entrance_to_oppos_border=0;
		w_mng_old=-1;
	    w_mng_beg=-1;
		w_mng_end=-1;
	    w_mng_length=-1;
		w_mg_length=-1;
	    w_mng_section_weight=-1;
	    w_mng_section_first=-1;
	    w_mng_section_last=-1;
	    w_mng_section_oppos=-1;
		 w_mng_section_oppos_bunch=-1;
	    w_mng_section_weight_oppos=-1;
	    w_mng_section_first_oppos=-1;
	    w_mng_section_last_oppos=-1;
		w_mg_section_weight=-1;
	    w_mg_section_first=-1;
	    w_mg_section_last=-1;
	    w_mg_section_oppos=-1;
		w_mg_section_oppos_bunch=-1;
	    w_mg_section_weight_oppos=-1;
	    w_mg_section_first_oppos=-1;
	    w_mg_section_last_oppos=-1;
		mng_old=-1;
	    mng_beg=-1;
		mng_end=-1;
	    mng_length=-1;
	    mng_section=-1;
	    mng_section_weight=-1;
	    mng_section_first=-1;
	    mng_section_last=-1;
	    mng_length=-1;
	    mng_section_oppos=-1;
	    mng_section_weight_oppos=-1;
	    mng_section_first_oppos=-1;
	    mng_section_last_oppos=-1;
		old_oppos_adjacent=-1;
	    op_adj_beg=-1;
	    op_adj_end=-1;
		right_adjacent_ordered=-1;
		sec_ordered_bunch=-1;
		left_adjacent_ordered=-1;
		first_in_ordering_old_number=-1;
		distance_to_green=NULL;
		green_bunch_number=NULL;
		adjacent_n_green_length=NULL;
		first_n_g_bunch=NULL;
		max_n_green_bunch=NULL;
		green_section_number=NULL;
		max_gr_adj_sec=NULL;
		max_gr_adj_sec_bunch=NULL;
		sec_bunches_ordered_numbers=NULL;
		green_bunch_number_oppos=NULL;
		adjacent_n_green_length_oppos=NULL;
		green_section_number_oppos=NULL;
		max_gr_adj_sec_oppos=NULL;
		max_gr_adj_sec_bunch_oppos=NULL;
		max_n_green_bunch_oppos=NULL;
		success_inverse=-1;
		first_green_adjacent_oppos=-1;
		last_green_adjacent_oppos=-1;
		maximum_non_green_adjacent_oppos=-1;
		non_green_length_oppos=0;
	    first_n_green_oppos=-1;
		last_n_green_oppos=-1;
		last_gr_in_chain_oppos=-1;
		first_green_chain_length_oppos=0,
	    max_green_adj_section_oppos=-1;
		max_green_adj_section_bunch_oppos=-1;
		max_green_adj_section=-1;
		max_green_adj_section_bunch=-1;
		max_n_green_adj_section_oppos=-1;
		max_n_green_adj_section_bunch_oppos=-1;
		max_n_green_adj_section=-1;
		max_n_green_adj_section_bunch=-1;
		number_of_dif_right_adjacent_green_sections=0;
		last_green_adjacent_section=-1;
		number_of_right_adjacent_green_sections=0;
		green_adjacent_section=-1;
		old_first_green_adjacent=-1;
		n_green_adjacent_section=-1;
		old_first_n_green_adjacent=-1;
		total_with_green_adjacent=0;
		without_green_adjacent=0;
		n_g_sum=0;
		n_g_first=-1;
		n_g_last=-1;
		first_n_green=-1;
		last_n_green=-1;
		disordering=-1;
		first_green_chain_length=0;
		number_of_covering_bunches=0;
		bunch_adjacent_ordered=-1;
		bunch_ordered_number=-1;
		first_green_adjacent=-1;
		last_green_adjacent=-1;
		maximum_non_green_adjacent=-1;
		non_green_length=0;
		last_gr_in_chain=-1;
		success_oppos=-1;
		section_length=0;
		//Intersection=-1;
		oppos_contrast=-1;
		oppos_adjacent=-1;
		section_previous=-1;
		//adj_total_weight=0;
		//adj_weight_ratio=0;
		prior=0;
		extreme_left_boundary_count=0;
		extreme_right_boundary_count=0;
		oppos_extreme_left_boundary_count=0;
		oppos_extreme_right_boundary_count=0;
		number_of_boundary_points=0;
		if(!left_right)
		{//last_cor02.11.1617.11.16
		last_left_bound_fiber=0;
		last_right_bound_fiber=0;
		}
		else
		{//last_cor17.11.16
		last_right_bound_fiber=0;
		last_left_bound_fiber=0;
		}
		number_sect=ColorSection->Number_of_sections_left;
	num_sect_right=ColorSection->Number_of_sections_right;
	section_beg=ColorSection->section_fibre_first[section_number];
	    section_end=ColorSection->section_fibre_last[section_number];
		section_length=section_end-section_beg+1;
		section_weight=ColorSection->section_weight[section_number];
		sect_incline=*(InclinedComponent+section_number);
		sect_incl_curve=*(InclineCurve+section_number);
		sect_incline_local=*(InclineLocal+section_number);//last_cor14.02.18
		local_beg=*(InclineLocalFirst+section_number);//last_cor14.02.18
		if(local_beg>0)
		{
        local_beg-=1;
		}
		local_end=*(InclineLocalLast+section_number);//last_cor14.02.18
		if(local_end>0)
		{
        local_end-=1;
		}
		sect_vert=*(VerticalComponent+section_number);
		sc_gr_comp=*(SkyGreenComponents+section_number);
	    sc_gr_comp_red=sc_gr_comp%10;
		section_hue=ColorSection->section_mean_hue[section_number];
		section_hue_zone=hue_zones[section_hue];
		section_sat=ColorSection->section_mean_saturation[section_number];
		section_gray=ColorSection->section_mean_gray[section_number];
		section_first_bunch=
			ColorDescrSect[section_number].location_of_section[section_beg];
	 section_first_bunch-=1;
	 section_first_bunch=ColorInt[section_beg].old_bunch_number[section_first_bunch];
	section_first_bunch_beg=
	ColorInt[section_beg].ColoredIntervalsStructure->BegInterv[section_first_bunch];
	section_first_bunch_end=
	ColorInt[section_beg].ColoredIntervalsStructure->EndInterv[section_first_bunch];
	section_first_bunch_length=section_first_bunch_end-section_first_bunch_beg+1;
	section_last_bunch=
			ColorDescrSect[section_number].location_of_section[section_end];
	 section_last_bunch-=1;
	 section_last_bunch=ColorInt[section_end].old_bunch_number[section_last_bunch];
	section_last_bunch_beg=
	ColorInt[section_end].ColoredIntervalsStructure->BegInterv[section_last_bunch];
	section_last_bunch_end=
	ColorInt[section_end].ColoredIntervalsStructure->EndInterv[section_last_bunch];
	section_last_bunch_length=section_last_bunch_end-section_last_bunch_beg+1;
	if(left_right)
	{//lr//last_cor16.11.16
	dist_to_frame_bound_last=DimX-section_first_bunch_end+1;
	}//lr
	else
	{//elrlast_cor16.11.16
		dist_to_frame_bound_last=section_first_bunch_beg;//last_cor16.11.16
if(UpperGreenBoundaryRight>0)
		{//onlyublast_cor22.08.17
			upper_section=RightGreenBoundarySection[UpperGreenBoundaryRight];
			if(upper_section>0)
			{
				upper_section-=1;
				upper_section=upper_section%1000;//!!!!errorlast_cor11.12.17
			upper_section_weight=ColorSection->section_weight[upper_section];
			upper_section_fiber_first=ColorSection->section_fibre_first[upper_section];
			upper_section_incline=*(InclinedComponent+upper_section);
			upper_section_incl_curve=*(InclineCurve+upper_section);
			upper_section_bunch=RightGreenBoundaryBunch[UpperGreenBoundaryRight];
            upper_section_bunch-=1;//!!!errorlast_cor11.12.17
			upper_sectiom_buch_beg=ColorInt[section_beg].ColoredIntervalsStructure->BegInterv[upper_section_bunch];
			upper_section_bunch_end=ColorInt[section_beg].ColoredIntervalsStructure->EndInterv[upper_section_bunch];
			non_green_section_max=RightAdjacentNonGreenSectionMax[UpperGreenBoundaryRight];
			if(non_green_section_max>0)
			{
			non_green_section_max-=1;
			non_green_section_max_incline=*(InclinedComponent+non_green_section_max);
			non_green_section_max_incl_curve=*(InclineCurve+upper_section);
			non_green_section_max_weight=ColorSection->section_weight[non_green_section_max];
			non_green_section_max_first=ColorSection->section_fibre_first[non_green_section_max];
			non_green_section_max_last=ColorSection->section_fibre_last[non_green_section_max];

			non_green_section_max_first_bunch=ColorDescrSect[non_green_section_max].location_of_section[non_green_section_max_first];
			non_green_section_max_first_bunch-=1;
			non_green_section_max_first_bunch=ColorInt[non_green_section_max_first].old_bunch_number[non_green_section_max_first_bunch];
		    non_green_section_max_first_bunch_beg=
		    		ColorInt[non_green_section_max_first].ColoredIntervalsStructure->BegInterv[non_green_section_max_first_bunch];
			non_green_section_max_first_bunch_end=
					ColorInt[non_green_section_max_first].ColoredIntervalsStructure->EndInterv[non_green_section_max_first_bunch];
			non_green_section_max_last_bunch=ColorDescrSect[non_green_section_max].location_of_section[non_green_section_max_last];
			non_green_section_max_last_bunch-=1;
			non_green_section_max_last_bunch=
					ColorInt[non_green_section_max_last].old_bunch_number[non_green_section_max_last_bunch];
			non_green_section_max_last_bunch_beg=
					ColorInt[non_green_section_max_last].ColoredIntervalsStructure->BegInterv[non_green_section_max_last_bunch];
			non_green_section_max_last_bunch_end=
					ColorInt[non_green_section_max_last].ColoredIntervalsStructure->EndInterv[non_green_section_max_last_bunch];
			}
			      if((section_first_bunch_end>DimX/2)&&(section_first_bunch_end>upper_sectiom_buch_beg)&&(UpperGreenBoundaryRight>NumStrips/3))
			      {
			    	  prior=-1;
			    	  goto M;
			      }
			}
		}//onlyublast_cor22.08.17
	}//elr
	first_second_sec_ratio=
	(16*section_last_bunch_length)/(section_first_bunch_length+section_last_bunch_length);
		section_gray_zone=gray_zones[section_gray];
		min_left=ColorSection->section_left_end[section_number];
		max_right=ColorSection->section_right_end[section_number];
		width_length=max_right-min_left+1;
		fiber_length=section_end-section_beg+1;

			shape_ratio=width_length/(StripWidth*fiber_length);
			last_srip_ratio=(16*section_end)/(NumStrips-1);
			first_strip_ratio=(16*section_beg)/(NumStrips-1);
			invert_shape_ratio=(StripWidth*fiber_length)/width_length;
			section_weight_normalized=section_weight/section_length;
			sect_persp=(16*section_weight_normalized)/width_length;
	//last_cor24.01.17
	section_last_bunch_new_num=ColorInt[section_end].ordered_bunch_number[section_last_bunch];
	if(!section_last_bunch_new_num)
	{
		if(left_right)
		{
	right_contrast=ColorInt[section_end].right_continuation[section_last_bunch];
	right_adjacent=ColorInt[section_end].right_adjacent[section_last_bunch];
		}
		else
		{
	left_contrast=ColorInt[section_end].left_continuation[section_last_bunch];
	left_adjacent=ColorInt[section_end].left_adjacent[section_last_bunch];
		}
	}
	if(!section_last_bunch_new_num)
	{
		if((section_weight<=4*StripWidth)&&(last_srip_ratio<=5))
		{
			if(min_left>3*StripWidth)
	        {
			    if(sc_gr_comp_red==2)
			    {
	            *(SkyGreenComponents+section_number)=-1;
			    }
				prior=-1;
			    goto M;
			}
		}
	}
	if((section_last_bunch_new_num>0)&&(((min_left>DimX/2)&&(left_right))||
	   ((max_right<DimX/2)&&(!left_right))))
	{//slnn
		   if(last_srip_ratio<=5)
		   {//lstr
			   if(left_right)
			   {
	oppos_adjacent=section_last_bunch_new_num+1;
			   }
			   else
			   {
	oppos_adjacent=section_last_bunch_new_num-1;
			   }
			   if((oppos_adjacent>=0)&&(oppos_adjacent<ColorInt[section_end].NumberOfIntervalsInCovering))
			   {//opa<
	success_inverse=
	NextGreenComponents(!left_right,oppos_adjacent,section_end,section_last_bunch,
	section_number,l_side_boun,r_side_boun,
	&first_green_adjacent_oppos,&last_green_adjacent_oppos,&maximum_non_green_adjacent_oppos,
	&non_green_length_oppos,&first_n_green_oppos,&last_n_green_oppos,&last_gr_in_chain_oppos,
	&first_green_chain_length_oppos,&max_green_adj_section_oppos,&max_green_adj_section_bunch_oppos,
	&w_mg_section_oppos1,&w_mg_section_oppos_bunch1,&max_n_green_adj_section_oppos,
	&max_n_green_adj_section_bunch_oppos,&w_mng_section_oppos1,&w_mng_section_oppos_bunch1,
	&green_lg_before,0);//last_cor19.06.17
	if(max_green_adj_section_oppos>=0)
	{//mgso
		mgso_weight=ColorSection->section_weight[max_green_adj_section_oppos];
		mgso_incl=*(InclinedComponent+max_green_adj_section_oppos);
		mgso_incl_curve=*(InclineCurve+max_green_adj_section_oppos);
		mgso_hue=ColorSection->section_mean_hue[max_green_adj_section_oppos];
		mgso_sat=ColorSection->section_mean_saturation[max_green_adj_section_oppos];
		mgso_gray=ColorSection->section_mean_gray[max_green_adj_section_oppos];
		mgso_hue_zone=hue_zones[mgso_hue];
		mgso_gray_zone=gray_zones[mgso_gray];
		mgso_col_comp=*(SkyGreenComponents+max_green_adj_section_oppos);
		mgso_col_comp=mgso_col_comp%10;
		mgso_beg=ColorSection->section_fibre_first[max_green_adj_section_oppos];
		mgso_end=ColorSection->section_fibre_last[max_green_adj_section_oppos];
		mgso_length=mgso_end-mgso_beg+1;
		if(((mgso_incl)||(mgso_incl_curve))&&(mgso_weight>=4*DimX))
		{
	if((first_n_green_oppos>=0)&&(first_n_green_oppos==max_n_green_adj_section_bunch_oppos))
	{
		if((section_weight<=DimX/2)||((section_weight<=DimX)&&((section_sat+1)<mgso_sat)))
		{
			if(left_right)
			{
				if(first_n_green_oppos<max_green_adj_section_bunch_oppos)
				{
			*(SkyGreenComponents+section_number)=-1;
			prior=-1;
			goto M;
				}
			}
			else
			{
				if(first_n_green_oppos>max_green_adj_section_bunch_oppos)
				{
				*(SkyGreenComponents+section_number)=-1;
			     prior=-1;
			    goto M;
				}
			}
		}
	}
		}
		if((mgso_incl)&&(mgso_incl_curve)&&(mgso_col_comp==2))
		{
			if((mgso_weight>=3*DimX)&&(section_weight<=DimX/2))
			{
	if(left_right)
	{
		if((first_n_green_oppos>=0)&&(first_n_green_oppos<max_green_adj_section_bunch_oppos))
				{
					if((mgso_incl<0)&&(mgso_incl_curve<0))
					{
					*(SkyGreenComponents+section_number)=-1;
	                prior=-1;
				    goto M;
					}
				}
	}
	else
	{
	if((first_n_green_oppos>=0)&&(first_n_green_oppos>max_green_adj_section_bunch_oppos))
				{
					if((mgso_incl>0)&&(mgso_incl_curve>0))
					{
					*(SkyGreenComponents+section_number)=-1;
	                prior=-1;
				    goto M;
					}
				}
	}
			}
		}
	    if((!sc_gr_comp_red)&&(mgso_col_comp==2))
		{//scg
			if((mgso_weight>=2*DimX)&&((mgso_incl)||(mgso_incl_curve)))
			{//wei

			if(left_right)
			{
	if((first_n_green_oppos>=0)&&(first_n_green_oppos<max_green_adj_section_bunch_oppos))
				{
					if((mgso_incl<0)||(mgso_incl_curve<0))
					{
					*(SkyGreenComponents+section_number)=-1;
	                prior=-1;
				    goto M;
					}
				}
			}
			else
			{
	if((first_n_green_oppos>=0)&&(first_n_green_oppos>max_green_adj_section_bunch_oppos))
				{
					if((mgso_incl>0)||(mgso_incl_curve>0))
					{
					*(SkyGreenComponents+section_number)=-1;
	                prior=-1;
				    goto M;
					}
				}
			}
			}//wei

	    }//scg

	}//mgso
			   first_green_adjacent_oppos=-1;
		  last_green_adjacent_oppos=-1;
		  maximum_non_green_adjacent=-1;
		  maximum_non_green_adjacent_oppos=-1;
		  non_green_length_oppos=-1;
	      first_n_green_oppos=-1;
		  last_n_green_oppos=-1;
		  last_gr_in_chain_oppos=-1;
		  first_green_chain_length_oppos=-1;
	      max_green_adj_section_oppos=-1;
		  max_green_adj_section_bunch_oppos=-1;
		  w_mg_section_oppos1=-1;
		  w_mg_section_oppos_bunch1=-1;
		  max_n_green_adj_section_oppos=-1;
		  max_n_green_adj_section_bunch_oppos=-1;
		  w_mng_section_oppos1=-1;
		  w_mng_section_oppos_bunch1=-1;
			   }//opa<
		   }//lstr
	}//slnn
	//last_cor24.01.17
			if(((first_second_sec_ratio<=8)||(shape_ratio>=4))&&(section_hue>=19)&&(last_srip_ratio<=2))
			{
				prior=-1;
				goto M;
			}
	if((first_second_sec_ratio<=8)&&(section_gray_zone<=1)&&(last_srip_ratio<=2)&&(section_sat<=3))
			{
				prior=-1;
				goto M;
			}
			if((sect_persp>=11)&&(section_gray_zone<=1)&&(last_srip_ratio<=2)&&(section_hue>=20)
				&&(section_sat<=3))
			{
				prior=-1;
				goto M;
			}
			if(section_gray<=4)
			{
			prior=-1;
				goto M;
			}
	if((section_hue>=16)&&(section_gray_zone<=1)&&(last_srip_ratio<=2)
	   &&(section_weight_normalized<=4*StripWidth)&&((min_left>2*StripWidth)
	   ||(max_right>(StripLength-2*StripWidth))))
			{
	        prior=-1;
				goto M;
			}
	if((section_weight_normalized<=6*StripWidth)&&(last_srip_ratio<=4)&&(first_strip_ratio<=1)&&
	   (section_weight<=600)&&(section_gray_zone<=1)&&(dist_to_frame_bound_last>(DimX/8)))
	{//addd convex_cond!!!
	 return(-1);
	}
	if((section_hue>=20)&&(section_gray_zone<1)&&(last_srip_ratio<=6)&&(!sect_incl_curve)&&
	   (section_sat<=4))
			{
	        prior=-1;
				goto M;
			}
	/*if((section_hue>=18)&&(section_gray_zone<=1)&&(first_strip_ratio>=8))
			{
	        prior=-1;
				goto M;
			}*/
			if((section_hue>=18)&&(section_gray_zone<=1)&&(section_weight_normalized<=2*StripWidth)&&
				(last_srip_ratio<=8))
			{
				prior=-1;
				goto M;
			}
	/*if((section_hue>=18)&&(section_gray_zone<=1)&&(last_srip_ratio<=2))
			{
	        prior=-1;
				goto M;
			}*/
			if((section_hue>=18)&&(section_gray_zone<=1)&&(last_srip_ratio<=2)&&(section_sat<=4)&&
				(sc_gr_comp_red!=2))
			{
	        prior=-1;
				goto M;
			}
			if(section_weight_normalized<=StripWidth)
			{
				prior=-1;
				goto M;
			}
			if((last_srip_ratio<=1)&&(section_weight_normalized<=4*StripWidth))
			{
	        prior=-1;
				goto M;
			}
			if((last_srip_ratio<=3)&&(section_gray_zone<1))
			{
				prior=-1;
				goto M;
			}
			if((last_srip_ratio<=8)&&(section_sat<=1)&&(section_gray>=16))
			{//last_cor15.11.16
				prior=-1;
				goto M;
			}
			if((last_srip_ratio<=2)&&(section_gray_zone>=2))
			{
				if((section_hue>=17)&&(section_sat<=4)&&(dist_to_frame_bound_last>DimX/8))
				{//last_cor16.11.16
				prior=-1;
				goto M;
				}
				if((section_hue>=19)&&(section_sat<=4)&&(section_gray_zone>=3))
				{
				prior=-1;
				goto M;
				}
			}
			if(last_srip_ratio<=7)
			{
			   if(section_hue>=21)
				{
				  if(section_gray_zone>=2)
				  {
				  prior=-1;
				  goto M;
				  }
				  else
				  {
					  if((section_sat<=6)&&((section_weight<=300)||((section_weight<=400)
						  &&(section_first_bunch_beg>=5*StripWidth))))
					  {
						prior=-1;
				       goto M;
					  }
				  }
			    }
			}
			if(last_srip_ratio<=3)
			{
				if((section_weight<=150)&&(section_gray_zone<=2)&&(section_sat<=6)
					&&(dist_to_frame_bound_last>DimX/8))
				{//last_cor16.11.16
				prior=-1;
				goto M;
				}
			}
	        if(last_srip_ratio<=3)
			{
				if((section_weight<=150)&&(section_gray_zone<=1)&&
					(dist_to_frame_bound_last>DimX/8))
				{//last_cor16.11.16
				prior=-1;
				goto M;
				}
			}

	/*if((section_hue>=8)&&(section_hue<=17))
	{//sh817
		if(section_sat>=6)
		{//secs6
			     if(section_gray_zone<=5)
				 {//grz5
			if((section_gray_zone>=2)||((section_gray_zone>=1)&&(section_sat>=8)))
			{
				*(SkyGreenComponents+section_number)=2;
			}
			else
			{
	if(section_gray_zone>=1)
	{
	*(SkyGreenComponents+section_number)=12;
	}
			}
				 }//grz5
		}//secs6
		else
		{//last_cor26.01.16
	if(section_sat>=3)
	{
		if((section_gray_zone>=1)&&(section_gray_zone<=3))
		{
			*(SkyGreenComponents+section_number)=22;
		}
	}
	else
	{
	if((section_sat>=2)&&(section_gray_zone>=2))
	{
		if((section_weight>=200)&&(last_srip_ratio>=8))
		{
			*(SkyGreenComponents+section_number)=32;
		}
	}
	}
		}//last_cor26.01.16
	}//sh817
	else
	{//esh817
		//if()
		if((section_hue>=5)&&(section_hue<=7))
		{
	      if((section_gray_zone>=2)&&(section_gray_zone<=4))
	      {
			  if(section_sat>=6)
			  {
				  *(SkyGreenComponents+section_number)=3;
			  }
			  else
			  {
	             if(section_sat>=3)
			     {
				 *(SkyGreenComponents+section_number)=13;
			     }
				 else
				 {
					 if(section_sat>=2)
			         {
				     *(SkyGreenComponents+section_number)=23;
			         }
				 }
			  }
		  }
		}
	}//esh817*/

	/*if((sc_gr_comp_red!=2)&&(section_gray_zone>1))
	{//last_cor03.11.16
	prior=-1;
	goto M;
	}*/
	if(!sc_gr_comp)
	{
		if((first_strip_ratio<=1)&&(last_srip_ratio<=6))
		{
			if(((min_left>=DimX/4)&&
				(min_left<=(3*DimX/4)))||((max_right>=DimX/4)&&(max_right<=(3*DimX/4))))
			{
				prior=-1;
				goto M;
			}
		}
	}
	if(sc_gr_comp_red==3)
	{//last_cor21.03.1720.11.17
		if(left_right)
		{
			if(sect_incline>=0)
			{
		prior=-1;
	goto M;
			}
		}
		else
		{
if(sect_incline<=0)
			{
		prior=-1;
	goto M;
			}
		}
	}//last_cor21.03.17
	if(sc_gr_comp_red==2)
	{
		if(last_srip_ratio<=1)
		{
	if((section_sat<=3)&&(section_gray_zone<=2))
	{
	prior=-1;
	goto M;
	}
		}
		if(section_beg>=3)
		{
	nearest_green_sect=NearestGreenUnder(section_number,section_beg,section_first_bunch);
		}
	}
	if(section_length>1)
	{
	sec_bunches_ordered_numbers=new int[section_length];
	distance_to_green=new int[section_length];
	green_bunch_number=new int[section_length];
	adjacent_n_green_length=new int[section_length];
	green_section_number=new int[section_length];
	max_gr_adj_sec=new int[section_length];
	max_gr_adj_sec_bunch=new int[section_length];
	first_n_g_bunch=new int[section_length];
	max_n_green_bunch=new int[section_length];
	memset(distance_to_green,(int) '\0',sizeof(int)*(section_length));
	memset(green_bunch_number,(int) '\0',sizeof(int)*(section_length));
	memset(adjacent_n_green_length,(int) '\0',sizeof(int)*(section_length));
	memset(green_section_number,(int) '\0',sizeof(int)*(section_length));
	memset(max_gr_adj_sec,(int) '\0',sizeof(int)*(section_length));
	memset(max_gr_adj_sec_bunch,(int) '\0',sizeof(int)*(section_length));
	memset(sec_bunches_ordered_numbers,(int) '\0',sizeof(int)*(section_length));
	memset(first_n_g_bunch,(int) '\0',sizeof(int)*(section_length));
	memset(max_n_green_bunch,(int) '\0',sizeof(int)*(section_length));

	green_bunch_number_oppos=new int[section_length];
	adjacent_n_green_length_oppos=new int[section_length];
	green_section_number_oppos=new int[section_length];
	max_gr_adj_sec_oppos=new int[section_length];
	max_gr_adj_sec_bunch_oppos=new int[section_length];
	max_n_green_bunch_oppos=new int[section_length];
	memset(green_bunch_number_oppos,(int) '\0',sizeof(int)*(section_length));
	memset(adjacent_n_green_length_oppos,(int) '\0',sizeof(int)*(section_length));
	memset(green_section_number_oppos,(int) '\0',sizeof(int)*(section_length));
	memset(max_gr_adj_sec_oppos,(int) '\0',sizeof(int)*(section_length));
	memset(max_gr_adj_sec_bunch_oppos,(int) '\0',sizeof(int)*(section_length));
	memset(max_n_green_bunch_oppos,(int) '\0',sizeof(int)*(section_length));
	}
	memset(adj_sec_green,(int) '\0',sizeof(int)*(8));
	count_adj_green_sec=0;
	memset(adj_sec_green_oppos,(int) '\0',sizeof(int)*(8));
	count_adj_green_sec_oppos=0;
	memset(adj_sec_n_green_max,(int) '\0',sizeof(int)*(8));
	count_adj_n_green_sec_max=0;
	memset(adj_sec_green_max,(int) '\0',sizeof(int)*(8));
	count_adj_green_sec_max=0;
	memset(adj_sec_n_green_max_oppos,(int) '\0',sizeof(int)*(8));
	count_adj_n_green_sec_max_oppos=0;
	memset(adj_sec_green_max_oppos,(int) '\0',sizeof(int)*(8));
	count_adj_green_sec_max_oppos=0;
	closeness_to_boundary=0;
	number_of_boundary_points=0;

	 w_mg_max_old=-1;
	 max_adj_sec=-1;
	 max_adj_sec_weight=-1;
	 max_adj_sec_weight=-1;
	 adjacent_boundary=0;

		number_of_roof_green=0;
		last_roof_green=-1;
	  for(count_fiber=section_beg;count_fiber<=section_end;count_fiber++)
	  {//secloop
		  boundary_bunch_oppos=-1;
		     boundary_bunch_oppos_beg=-1;
			 boundary_bunch_oppos_end=-1;//last_cor19.07.17
		  oppos_boun_sec=-1;//last_cor19.07.17
		  l_side_boun=0;
		  r_side_boun=0;//last_cor19.06.17
		  w_mg_section_color=0;//last_cor02.05.17
		  w_mg_section_oppos2=-1;//last_cor24.04.17
		  w_mg_vert_oppos=-1;//last_cor24.04.17
		  w_mg_vert=-1;
		  w_mng_section_oppos2=-1;//last_cor18.04.17
		  Intersection_green1=-1;//last_cor14.04.17
		  section_bunch_shift=-1;//last_cor30.03.17
		  section_bunch_shift_abs=-1;
		  dist_gobs_mgas_abs=-1;//last_cor28.03.17
		  last_gr_section_weight=-1;//last_cor21.03.17
		  first_gr_section_weight=-1;
		  dist_fga_wmg=-DimX;
		  prev_mnga_bunch_length=-1;
		  green_lg_before1=0;
		  green_lg_before=0;//last_cor10.03.17
		  section_boun_prev_bunch=-1;
		  Intersection_green=-1;//last_cor10.01.17
		  right_oppos_green_max=-1;//last_cor27.12.16
		  prev_sec_ng_max=-1;//last_cor21.12.16
	    prev_sec_g_max=-1;
		  green_bunch_finding=-1;
			            dist_form_lt_bunch_to_wmng=-1;//!last_cor!12.12.16
			  	    	dist_form_lt_sec_bunch_to_wmng=-1;
		  green_oppos_boundary_sec=-1;//last_cor09.12.16
		  first_gr=-1;//last_cor08.12.16
	    first_gr_section=-1;//last_cor08.12.16
		  w_mng_section_weight_normalized=-1;
		  w_mng_section_first_ratio=-1;
	      w_mng_section_last_ratio=-1;
		  w_mng_section_first_ratio_oppos=-1;
	      w_mng_section_last_ratio_oppos=-1;
		  w_mng_hue=-1;
	      w_mng_sat=-1;
	      w_mng_gray=-1;
	      w_mng_hue_zone=-1;
	      w_mng_gray_zone=-1;
		  w_mng_hue_oppos=-1;
	      w_mng_sat_oppos=-1;
	      w_mng_gray_oppos=-1;
	      w_mng_hue_zone_oppos=-1;
	      w_mng_gray_zone_oppos=-1;
		  new_boundary_section=-1;
		  prev_boun_bunch=-1;
	      left_end_bb_prev=-1;
	      right_end_bb_prev=-1;
	      differ_boundary_sign_prev=-1;
	      differ_boundary_abs_prev=-1;
		  condition_number=-1;
		  success_left=0;
		  success_right=0;
		  number_of_boun_points_in_max_adj=-1;
		  number_of_boundary_points_adjacent_oppos=0;
		  green_adjacent_section_op_incline=-1;
	      green_adjacent_section_op_incl_curve=-1;
		  green_adjacent_section_op_weight=-1;
		  green_adjacent_section_op_beg=-1;
		  green_adjacent_section_op_end=-1;
		  green_adjacent_section_op_length=-1;
		  green_adjacent_section_op=-1;
		  green_adjacent_section_op_oppos=-1;
		  differ_boundary_abs=-1;
	      differ_boundary_oppos=-1;
	      differ_boundary_oppos_abs=-1;
		  prev_mnga_bunch_beg=-1;
		  prev_mnga_bunch_end=-1;
		  w_mng_old_oppos=-1;
		  w_mng_beg_oppos=-1;
	      w_mng_end_oppos=-1;
	      w_mng_length_oppos=-1;
		  dist_wmng_to_mga=-DimX;
		  differ_boundary_sign=-DimX;
		  dist1gr_to_wmng=-DimX;
		  dist_wmng_to_mga=-DimX;
		  dist_wmng_oppos_to_sect=-DimX;
		  w_mg_incl_curve=0;
		  w_mg_incl_curve_oppos=0;
		  w_mng_incl_curve=0;
		  w_mng_incl_curve_oppos=0;
		  previous_mnga=-1;
		  previous_mnga_bunch=-1;
		  bound_b_existing=-1;
	      sect_b_existing=-1;
	      section_max_existing=-1;
		  section_max_existing_bunch=-1;
		  //m_n_g_f_prev_sec=-1;
		  section_beg_prev=-1;
	      section_end_prev=-1;
	      section_length_prev=-1;
		  section_boun_prev=-1;
		  number_bound_points=-1;
	      //differ_boundary=-1;
		  boundary_bunch=-1;
	      left_end_bb=-1;
		  right_end_bb=-1;
		  extreme_right_boundary_now=0;
		  extreme_left_boundary_now=0;
		  rt_differ=16;
		  differ_prev_new_fg=DimX;
	      differ_prev_new_lg=DimX;
		  w_mg_current_sec_weight_ratio=-1;
		  abs_convex_dif_new=-1;
		  abs_convex_dif=-1;
		  residual_ratio10=-1;
		  residual_ratio20=-1;
		  residual_ratio30=-1;
		  //max_adj_sec_weight=-1;
		  w_mg_section_oppos1=-1;
	      w_mg_section_oppos_bunch1=1;
		  w_mng_section_oppos1=-1;
		  w_mng_section_oppos_bunch1=-1;
		  w_mg_sec_length=0;
	      w_mg_boun_point_ratio=0;
		  number_of_boun_points_in_max_adj=-1;
		  section_fit=-1;
		  old_first_length=DimX;
	      old_first_density=2*StripWidth;
		  w_mng_section_oppos_incline=0;
		  w_mg_section_oppos_incline=0;
		  w_mng_section_incline=0;
		  w_mg_section_incline=0;
		  dist_next_green=-1;
		  f_g_bunch_length_ratio=100;
		  residual_ratio3=-1;
		  dist_to_wmng=-DimX;
		  dist_to_wmg=-DimX;
		  convex_dif_new=-DimX;
		  //previous_success=-1;
		  w_mng_section_sky=-1;
		  w_mng_section_sky_oppos=-1;
		  first_max_adj_sec_strip=-1;
		  last_max_adj_sec_strip=-1;
		  first_n_green_adjacent_oppos_sec=-1;
		  n_green_adjacent_section_weight=-1;
		  green_adjacent_section_oppos=-1;
		  fiber_residual=-1;
		  fiber_residual_new=-1;
		  oppos_green_length=-1;
		  bunch_adjacent_ordered=-1;
		  entrance_to_oppos_border_now=0;
		  success_inverse=-1;
		  success_oppos=-1;
		  first_green_adjacent=-1;
		  last_green_adjacent=-1;
		  maximum_non_green_adjacent=-1;
		  non_green_length=-1;
	      first_n_green=-1;
		  last_n_green=-1;
		  last_gr_in_chain=-1;
		  first_green_chain_length=-1;
	      max_green_adj_section=-1;
		  first_green_adjacent_oppos=-1;
		  last_green_adjacent_oppos=-1;
		  maximum_non_green_adjacent=-1;
		  maximum_non_green_adjacent_oppos=-1;
		  non_green_length_oppos=-1;
	      first_n_green_oppos=-1;
		  last_n_green_oppos=-1;
		  last_gr_in_chain_oppos=-1;
		  first_green_chain_length_oppos=-1;
	      max_green_adj_section_oppos=-1;
		  max_green_adj_section_bunch_oppos=-1;
		  w_mg_section_oppos1=-1;
		  w_mg_section_oppos_bunch1=-1;
		  max_n_green_adj_section_oppos=-1;
		  max_n_green_adj_section_bunch_oppos=-1;
		  w_mng_section_oppos1=-1;
		  w_mng_section_oppos_bunch1=-1;
		  last_green_adjacent_section=-1;
		  green_adjacent_section=-1;
		  green_adjacent_section_incl_curve=-1;
		  mng_section=-1;
		  mng_section_oppos=-1;
		  mng_section_weight=-1;
		  mng_section_weight_oppos=-1;
		  max_n_green_adj_section=-1;
		  max_n_green_adj_section_bunch=-1;
		  w_mng_section_oppos=-1;
		   w_mng_section_oppos_bunch=-1;
		  w_mng_section_weight=-1;
		  w_mng_section_weight_oppos=-1;
		  section_bunch_previous=-1;
		  section_bunch_next=-1;
		  side_fiber_residual=-1;
		  oppos_green_beg=-1;
		  oppos_green_end=-1;
		  differ_bb_fga=DimX;

		  array_num=count_fiber-section_beg;
		  number_of_bunches=ColorInt[count_fiber].RefinedNumberOfBunches;
		  disordering=ColorInt[count_fiber].Disordering;
		  if(!disordering)
		  {
		  number_of_covering_bunches=ColorInt[count_fiber].NumberOfIntervalsInCovering;
		  }
		  else
		  {
			  goto L;
		  }
		  first_in_ordering_old_number=ColorInt[count_fiber].old_ordered_bunch_number[0];
	  section_bunch=ColorDescrSect[section_number].location_of_section[count_fiber];
	  if(section_bunch>0)
	  {
	 section_bunch-=1;
	 if((section_bunch>=number_of_bunches)||(section_bunch<0))
	 {
		 prior=-2;
		 goto M;
	 }
	 bunch_old_number=ColorInt[count_fiber].old_bunch_number[section_bunch];
	beg_int=ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[bunch_old_number];
	end_int=ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[bunch_old_number];
	int_length=end_int-beg_int+1;
	int_signif=ColorInt[count_fiber].ColoredIntervalsStructure->Significance[bunch_old_number];
	int_density=(2*int_signif)/int_length;
	bunch_occurrence=ColorInt[count_fiber].visible_bunches[bunch_old_number];//last_cor22.11.16
	bunch_hue=ColorInt[count_fiber].ColoredIntervalsStructure->AverageHue[bunch_old_number];
	bunch_sat=ColorInt[count_fiber].ColoredIntervalsStructure->AverageSat[bunch_old_number];
	bunch_gray=ColorInt[count_fiber].ColoredIntervalsStructure->AverageGray[bunch_old_number];
	bunch_hue_zone=hue_zones[bunch_hue];
	bunch_gray_zone=gray_zones[bunch_gray];
	if((bunch_sat<=1)&&(section_sat<=2))
	{//last_cor22.11.16
		break;
	}
	if((bunch_sat<=2)&&(section_sat<=2)&&(bunch_gray_zone>=2)&&(sc_gr_comp_red!=2))
	{
		break;
	}
	left_contrast=ColorInt[count_fiber].left_continuation[bunch_old_number];
	left_adjacent=ColorInt[count_fiber].left_adjacent[bunch_old_number];
	if(left_right)
	{//lr
	bound_b_existing=RightGreenBoundaryBunch[count_fiber];
	sect_b_existing=RightGreenBoundarySection[count_fiber];
	section_max_existing=RightAdjacentNonGreenSectionMax[count_fiber];
	}//lr
	else
	{//elr
	bound_b_existing=LeftGreenBoundaryBunch[count_fiber];
	sect_b_existing=LeftGreenBoundarySection[count_fiber];
	section_max_existing=LeftAdjacentNonGreenSectionMax[count_fiber];
	}//elr
	if(bound_b_existing>0)
	{//bbe>0
	bound_b_existing--;
	sect_b_existing--;
	//bound_b_existing--;
	    if(section_max_existing>0)
		{
	    section_max_existing--;
		section_max_existing_bunch=
			ColorDescrSect[section_max_existing].location_of_section[count_fiber];
		  if(section_max_existing_bunch>0)
		  {
			  section_max_existing_bunch--;
	section_max_existing_bunch=ColorInt[count_fiber].ordered_bunch_number[section_max_existing_bunch];
		  }
		}
		else
		{
		section_max_existing=-1;
		}
	}//bbe>0
	else
	{
	bound_b_existing=-1;
	sect_b_existing=-1;
	}
	if(left_contrast<0)
	{//lc<0
		left_contrast=-left_contrast;
		left_contrast-=1;
		left_adjacent=-left_adjacent;
		left_adjacent-=1;
		left_contrast=left_contrast%10;
		oppos_contrast=left_contrast;
		//oppos_adjacent=left_adjacent;
	left_adjacent_ordered=ColorInt[count_fiber].ordered_bunch_number[left_adjacent];

	}//lc<0
	else
	{//elc<0
		if(left_contrast>0)
		{//lc>0last_cor22.01.15
	left_contrast-=1;
	left_adjacent-=1;
	oppos_contrast=left_contrast;
	//oppos_adjacent=left_adjacent;
	left_adjacent_ordered=ColorInt[count_fiber].ordered_bunch_number[left_adjacent];

		}//lc>0
		else
		{//elc>0
		if(!left_contrast)
		{//lc0
			oppos_contrast=left_contrast;
			if(beg_int<StripWidth)
			{
	            extreme_left_boundary_now=1;
				extreme_left_boundary_count++;
				//goto L;
			}
		}//lc0
		}////elc>0last_cor22.01.15
	}//elc<0
	right_contrast=ColorInt[count_fiber].right_continuation[bunch_old_number];
	right_adjacent=ColorInt[count_fiber].right_adjacent[bunch_old_number];
	if(right_contrast<0)
	{//rc<0
		right_contrast=-right_contrast;
		right_contrast-=1;
		right_adjacent=-right_adjacent;
		right_adjacent-=1;
		right_contrast=right_contrast%10;
		oppos_contrast=right_contrast;
		right_adjacent_ordered=ColorInt[count_fiber].ordered_bunch_number[right_adjacent];

	}//rc<0
	else
	{//erc<0
		   if(right_contrast>0)
		   {//last_cor22.01.16
			   right_contrast-=1;
			   right_contrast=right_contrast%10;
			   right_adjacent-=1;
			   oppos_contrast=right_contrast;
	right_adjacent_ordered=ColorInt[count_fiber].ordered_bunch_number[right_adjacent];
		   }
		   else
		   {//erc>0
		if(!right_contrast)
		{//rc0
			if(end_int>(StripLength-StripWidth))
			{
			extreme_right_boundary_now=1;
			extreme_right_boundary_count++;
			if((extreme_right_boundary_now)&&(extreme_left_boundary_now)&&(sc_gr_comp_red==2))
			{
				if(count_fiber>( section_beg+(section_end-section_beg)/2))
				{
			number_of_roof_green++;
		    last_roof_green=count_fiber;
				}
			}
			//goto L;
			}
	oppos_contrast=right_contrast;
		}//rc0
		   }////erc>0last_cor22.01.16
	}//erc<0
	bunch_ordered_number=ColorInt[count_fiber].ordered_bunch_number[bunch_old_number];
	if((!bunch_ordered_number)&&(bunch_old_number!=first_in_ordering_old_number))
	{//last_cor30.10.17
	sec_bunches_ordered_numbers[array_num]=-1;
	//goto L;
	}
	else
	{
	sec_bunches_ordered_numbers[array_num]=bunch_ordered_number+1;
	}
	sec_ordered_bunch=sec_bunches_ordered_numbers[array_num];
	  }
	  else
	  {
		  prior=-2;
		  goto M;
	  }
	if(!left_right)
	{//lr
	if(right_contrast>0)
	{//rc><0
	if(bunch_ordered_number>0)
	     {//bon>0
			 if(bunch_ordered_number<(number_of_covering_bunches-1))
			 {
		bunch_adjacent_ordered=bunch_ordered_number+1;
			 }
			 else
			 {
	bunch_adjacent_ordered=-2;
			 }
	    }//bon>0
		 else
		 {//ebon>0
	        if(!bunch_ordered_number)
	        {//bon0
				if(sec_ordered_bunch<0)
				{//sob<
				if((right_adjacent_ordered>0)&&(right_adjacent_ordered<number_of_covering_bunches))
				{
	             bunch_adjacent_ordered=right_adjacent_ordered;
				}
				}//sob<
				else
				{//esob<
					if(sec_ordered_bunch>0)
					{
		    entrance_to_oppos_border++;
			entrance_to_oppos_border_now=1;
	if(number_of_covering_bunches==1)
	{
	bunch_adjacent_ordered=-2;
	oppos_adjacent=-2;
	}
	else
	{
	bunch_adjacent_ordered=bunch_ordered_number+1;
	}
					}
				}//esob<
			}//bon0
		 }//ebon>0
	}//rc><0
	else
	{//erc><0
		if(!right_contrast)
		{//rc0
			if(bunch_ordered_number>0)
	     {//bon>0
			 if(bunch_ordered_number<(number_of_covering_bunches-1))
			 {
		bunch_adjacent_ordered=bunch_ordered_number+1;
			 }
			 else
			 {
	bunch_adjacent_ordered=-2;
			 }
	    }//bon>0
		 else
		 {//ebon>0
	        if(!bunch_ordered_number)
	        {//bon0
					if(sec_ordered_bunch>0)
					{
	if(number_of_covering_bunches==1)
	{
	bunch_adjacent_ordered=-2;
	oppos_adjacent=-2;
	}
	else
	{
	bunch_adjacent_ordered=bunch_ordered_number+1;
	}
					}
			}//bon0
		 }//ebon>0
		}//rc0
	}//erc><0
	}//lr
	else
	{//elr
	if(left_contrast>0)
	{//lc<>0
		if(bunch_ordered_number>0)
	     {
		bunch_adjacent_ordered=bunch_ordered_number-1;
	     }
		 else
		 {//ebon>0
	        if(!bunch_ordered_number)
	        {//bon0
				    if(sec_ordered_bunch<0)
				    {
				if(left_adjacent_ordered>0)
				{
	             bunch_adjacent_ordered=left_adjacent_ordered;
				}
					}
					else
					{
	if(sec_ordered_bunch>0)
	{
	bunch_adjacent_ordered=-2;
	}
					}
			}//bon0
		 }//ebon>0
	}//lc<>0
	else
	{//elc<>0
	if(!left_contrast)
	{
		if(bunch_ordered_number>0)
	     {
		bunch_adjacent_ordered=bunch_ordered_number-1;
	     }
		 else
		 {//ebon>0
	        if(!bunch_ordered_number)
	        {//bon0
	if(sec_ordered_bunch>0)
	{
	bunch_adjacent_ordered=-2;
	}
			}//bon0
		 }//ebon>0
	}
	}//elc<>0
	}//elr

	if(left_right)
	{//lr
	if(bunch_ordered_number>0)
	{//bon>0
		if(end_int>=(StripLength-StripWidth))
		{
			entrance_to_oppos_border++;
			entrance_to_oppos_border_now=1;
		}
		if(bunch_ordered_number<(number_of_covering_bunches-1))
		{
	oppos_adjacent=bunch_ordered_number+1;
		}
		else
		{
			oppos_adjacent=-2;
		}
	}//bon>0
	else
	{//ebon>0
		if(!bunch_ordered_number)
		{//bon0
	if(sec_ordered_bunch>0)
	{//sob>
		if(number_of_covering_bunches>1)
		{
			oppos_adjacent=bunch_ordered_number+1;
		}
		else
		{
			oppos_adjacent=-2;
		}
	}//sob>
	else
	{//esob>
	if(sec_ordered_bunch<0)
	{//sob<
		if(right_adjacent_ordered>0)
		{
			oppos_adjacent=right_adjacent_ordered;
		}
		else
		{
			oppos_adjacent=-2;
		}
	}//sob<
	}//esob>
		}//bon0
	}//ebon>0
	}//lr
		else
		{
	if(bunch_ordered_number>0)
	{//bon>0
		if(beg_int<=StripWidth)
		{
			if(!entrance_to_oppos_border_now)
			{
			entrance_to_oppos_border++;
			entrance_to_oppos_border_now=1;
			}
		}
	oppos_adjacent=bunch_ordered_number-1;
	}//bon>0
	else
	{//ebon>0
		if(!bunch_ordered_number)
		{//bon0
	if(sec_ordered_bunch<0)
	{//sob<
		if(left_adjacent_ordered>0)
		{
			oppos_adjacent=left_adjacent_ordered;
		}
	}//sob<
	else
	{
		if(sec_ordered_bunch>0)
		{
			if(!entrance_to_oppos_border_now)
			{
	entrance_to_oppos_border++;
	entrance_to_oppos_border_now=1;
			}
	oppos_adjacent=-2;
		}
	}
		}//bon0
	}//ebon>0
		}

		if(number_of_covering_bunches)
		{//ncb
			if(bunch_adjacent_ordered>=0)
			{//bao>=
				if(left_right)
				{
					if((!bunch_ordered_number)&&(sec_ordered_bunch>0))
					{
						success_oppos=-2;
					}
				}
				else
				{
					if(bunch_ordered_number==(number_of_covering_bunches-1))
					{
					success_oppos=-2;
					}
				}
				//if((success_oppos!=-2)&&(bunch_adjacent_ordered>=0))
				//{//sop
	if(count_fiber>0)
	{//cf>0last_cor20.02.17
	    if(left_right)
		{//lr
	boundary_bunch=RightGreenBoundaryBunch[count_fiber-1];
	if(boundary_bunch>0)
	{//>0
	boundary_bunch--;
	left_end_bb=ColorInt[count_fiber-1].ColoredIntervalsStructure->BegInterv[boundary_bunch];
	right_end_bb=ColorInt[count_fiber-1].ColoredIntervalsStructure->EndInterv[boundary_bunch];
	r_side_boun=left_end_bb;
	differ_boundary_sign=beg_int-left_end_bb;
	if((min_left>right_end_bb)&&(differ_boundary_sign>3*StripWidth))
	{//last_cor14.12.17
		break;
	}
	differ_boundary_abs=abs(differ_boundary_sign);
	differ_boundary_oppos=end_int-left_end_bb;
	differ_boundary_oppos_abs=abs(differ_boundary_oppos);
	green_oppos_boundary_sec=RightAdjacentGreenSectionMax[count_fiber-1];//last_cor09.12.16
	if(green_oppos_boundary_sec>0)
	{
	green_oppos_boundary_sec--;
	green_oppos_boundary_bunch=
	ColorDescrSect[green_oppos_boundary_sec].location_of_section[count_fiber-1];
	green_oppos_boundary_bunch-=1;
	green_oppos_boundary_bunch=
	ColorInt[count_fiber-1].old_bunch_number[green_oppos_boundary_bunch];
	green_oppos_boundary_bunch_beg=
	ColorInt[count_fiber-1].ColoredIntervalsStructure->BegInterv[green_oppos_boundary_bunch];
	green_oppos_boundary_bunch_end=
	ColorInt[count_fiber-1].ColoredIntervalsStructure->EndInterv[green_oppos_boundary_bunch];
    l_side_boun=green_oppos_boundary_bunch_end;
	green_oppos_boundary_bunch=
	ColorInt[count_fiber-1].ordered_bunch_number[green_oppos_boundary_bunch];
	}
	else
	{//last_cor27.12.16
		green_oppos_boundary_sec=-1;
	}
	section_boun_prev=RightGreenBoundarySection[count_fiber-1];//last_cor25.12.16
	  if(section_boun_prev>0)
	  {//sbp>0
		section_boun_prev--;
	section_beg_prev=ColorSection->section_fibre_first[section_boun_prev];
	section_end_prev=ColorSection->section_fibre_last[section_boun_prev];
	section_length_prev=section_end_prev-section_beg_prev+1;
	//last-cor26.01.17
	section_boun_prev_bunch=
	ColorDescrSect[section_boun_prev].location_of_section[count_fiber-1];
	if(section_boun_prev_bunch>0)
	{
	section_boun_prev_bunch-=1;
	section_boun_prev_bunch=ColorInt[count_fiber-1].old_bunch_number[section_boun_prev_bunch];
	}
	     if(section_boun_prev!=section_number)
		 {//sbp!=sn
	number_bound_points=LeftRightSectionNumberOfRightGreenBoundaryPoints[section_boun_prev];
	section_bunch_shift=left_end_bb-end_int;
	section_bunch_shift_abs=abs(section_bunch_shift);
		 }//sbp!=sn
	  }//sbp>0
	  else
	  {
	  section_boun_prev=-1;
	  }
	previous_mnga=RightAdjacentNonGreenSectionMax[count_fiber-1];//last_cor15.12.16
	if(previous_mnga>0)
		 {//pmnga>0
			 previous_mnga--;
			 previous_mnga_bunch=
	 ColorDescrSect[previous_mnga].location_of_section[count_fiber-1];
	if(previous_mnga_bunch>0)
		  {//pb>
			  previous_mnga_bunch--;
	previous_mnga_bunch=ColorInt[count_fiber-1].old_bunch_number[previous_mnga_bunch];
	prev_mnga_bunch_beg=
	ColorInt[count_fiber-1].ColoredIntervalsStructure->BegInterv[previous_mnga_bunch];
	prev_mnga_bunch_end=
	ColorInt[count_fiber-1].ColoredIntervalsStructure->EndInterv[previous_mnga_bunch];
	prev_mnga_bunch_length=prev_mnga_bunch_end-prev_mnga_bunch_beg+1;
	//onlyub17.05.17
	prev_mnga_bunch_hue=
			ColorInt[count_fiber-1].ColoredIntervalsStructure->AverageHue[previous_mnga_bunch];
		prev_mnga_bunch_hue_zone=hue_zones[prev_mnga_bunch_hue];
		prev_mnga_bunch_gray=
		ColorInt[count_fiber-1].ColoredIntervalsStructure->AverageGray[previous_mnga_bunch];
		prev_mnga_bunch_gray_zone=gray_zones[prev_mnga_bunch_gray];
		prev_mnga_bunch_sat=
				ColorInt[count_fiber-1].ColoredIntervalsStructure->AverageSat[previous_mnga_bunch];
	previous_mnga_bunch=ColorInt[count_fiber-1].ordered_bunch_number[previous_mnga_bunch];
		  }//pb>
		  else
		  {//epb>
			  previous_mnga_bunch=-1;
		  }//epb>
		 }//pmnga>0
		 else
		 {//epmnga>0
			 previous_mnga=-1;
		 }//epmnga>0//last_cor15.12.16
	}//>0
	else//last_cor28.02.17
	{
		boundary_bunch=-1;
	}
		}//lr
		else
		{//elr
	boundary_bunch=LeftGreenBoundaryBunch[count_fiber-1];
	oppos_boun_sec=RightGreenBoundarySection[count_fiber];//last_cor19.07.17
	boundary_bunch_oppos=RightGreenBoundaryBunch[count_fiber];//last_cor19.07.17
	if(oppos_boun_sec>0)
	{//last_cor19.07.17
		oppos_boun_sec-=1;
		if(boundary_bunch_oppos>0)
		{
			boundary_bunch_oppos-=1;
			boundary_bunch_oppos_beg=
ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[boundary_bunch_oppos];
			boundary_bunch_oppos_end=
ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[boundary_bunch_oppos];
		}
		if(end_int>boundary_bunch_oppos_beg)
		{
			break;
		}
	}
	else
	{
		oppos_boun_sec=-1;
	}
	if(boundary_bunch>0)
	{//bb>0//last_cor20.02.17
	boundary_bunch--;
	left_end_bb=ColorInt[count_fiber-1].ColoredIntervalsStructure->BegInterv[boundary_bunch];
	right_end_bb=ColorInt[count_fiber-1].ColoredIntervalsStructure->EndInterv[boundary_bunch];
	l_side_boun=right_end_bb;
	differ_boundary_sign=end_int-right_end_bb;
	differ_boundary_abs=abs(differ_boundary_sign);
	differ_boundary_oppos=beg_int-right_end_bb;
	differ_boundary_oppos_abs=abs(differ_boundary_oppos);
	green_oppos_boundary_sec=LeftAdjacentGreenSectionMax[count_fiber-1];//last_cor09.12.16
	if(green_oppos_boundary_sec>0)
	{
	green_oppos_boundary_sec--;
	green_oppos_boundary_bunch=
	ColorDescrSect[green_oppos_boundary_sec].location_of_section[count_fiber-1];
	 green_oppos_boundary_bunch-=1;
	green_oppos_boundary_bunch=
	ColorInt[count_fiber-1].old_bunch_number[green_oppos_boundary_bunch];
	green_oppos_boundary_bunch_beg=
	ColorInt[count_fiber-1].ColoredIntervalsStructure->BegInterv[green_oppos_boundary_bunch];
	green_oppos_boundary_bunch_end=
	ColorInt[count_fiber-1].ColoredIntervalsStructure->EndInterv[green_oppos_boundary_bunch];
	r_side_boun=green_oppos_boundary_bunch_beg;
	green_oppos_boundary_bunch=
	ColorInt[count_fiber-1].ordered_bunch_number[green_oppos_boundary_bunch];
	}
	else
	{
	green_oppos_boundary_sec=-1;
	}
	section_boun_prev=LeftGreenBoundarySection[count_fiber-1];//last_cor25.12.16
	  if(section_boun_prev>0)
	  {//sbp>0
		section_boun_prev--;
	section_beg_prev=ColorSection->section_fibre_first[section_boun_prev];
	section_end_prev=ColorSection->section_fibre_last[section_boun_prev];
	section_length_prev=section_end_prev-section_beg_prev+1;
	section_boun_prev_bunch=
	ColorDescrSect[section_boun_prev].location_of_section[count_fiber-1];
	if(section_boun_prev_bunch>0)
	{
	section_boun_prev_bunch-=1;
	section_boun_prev_bunch=ColorInt[count_fiber-1].old_bunch_number[section_boun_prev_bunch];
	}
	     if(section_boun_prev!=section_number)
		 {//sbp!=sn
	number_bound_points=LeftRightSectionNumberOfRightGreenBoundaryPoints[section_boun_prev];
	section_bunch_shift=beg_int-right_end_bb;
	section_bunch_shift_abs=abs(section_bunch_shift);
	     }//sbp!=sn
	  }//sbp>0
	  else
	  {
		  section_boun_prev=-1;
	  }
	previous_mnga=LeftAdjacentNonGreenSectionMax[count_fiber-1];//last_cor15.12.16
		 if(previous_mnga>0)
		 {//pmnga
			 previous_mnga--;
			 previous_mnga_bunch=
	ColorDescrSect[previous_mnga].location_of_section[count_fiber-1];
		  if(previous_mnga_bunch>0)
		  {//pb>
			  previous_mnga_bunch--;
	previous_mnga_bunch=ColorInt[count_fiber-1].old_bunch_number[previous_mnga_bunch];
	prev_mnga_bunch_beg=
	ColorInt[count_fiber-1].ColoredIntervalsStructure->BegInterv[previous_mnga_bunch];
	prev_mnga_bunch_end=
	ColorInt[count_fiber-1].ColoredIntervalsStructure->EndInterv[previous_mnga_bunch];
	prev_mnga_bunch_length=prev_mnga_bunch_end-prev_mnga_bunch_beg+1;
	//onlyub17.05.17
		prev_mnga_bunch_hue=
				ColorInt[count_fiber-1].ColoredIntervalsStructure->AverageHue[previous_mnga_bunch];
			prev_mnga_bunch_hue_zone=hue_zones[prev_mnga_bunch_hue];
			prev_mnga_bunch_gray=
			ColorInt[count_fiber-1].ColoredIntervalsStructure->AverageGray[previous_mnga_bunch];
			prev_mnga_bunch_gray_zone=gray_zones[prev_mnga_bunch_gray];
			prev_mnga_bunch_sat=
					ColorInt[count_fiber-1].ColoredIntervalsStructure->AverageSat[previous_mnga_bunch];
	previous_mnga_bunch=ColorInt[count_fiber-1].ordered_bunch_number[previous_mnga_bunch];

		  }//pb>
		  else
		  {//epb
	      previous_mnga_bunch=-1;
		  }//epb
		 }//pmnga
		 else
		 {//epmnga
			 previous_mnga=-1;
		 }//epmnga
	}//bb>0//last_cor20.02.17
	else//last_cor28.02.17
	{
		boundary_bunch=-1;
	}
	   }//elr
	}//cf>0last_cor20.02.17
	if((success_oppos!=-2)&&(bunch_adjacent_ordered>=0))
				{//sop
	success_oppos=
	NextGreenComponents(left_right,bunch_adjacent_ordered,count_fiber,bunch_old_number,
	section_number,l_side_boun,r_side_boun,&first_green_adjacent,&last_green_adjacent,&maximum_non_green_adjacent,
	&non_green_length,&first_n_green,&last_n_green,&last_gr_in_chain,&first_green_chain_length,
	&max_green_adj_section,&max_green_adj_section_bunch,&w_mg_section_oppos,
	&w_mg_section_oppos_bunch,&max_n_green_adj_section,&max_n_green_adj_section_bunch,
	&w_mng_section_oppos,&w_mng_section_oppos_bunch,&green_lg_before,0);

	if(success_oppos==-3)
	{
		prior=-1;
		goto M;
	}
	if((max_green_adj_section<0))
	{//onlyublast_cor23.08.17additionalconditionrequired!!!
		if(last_srip_ratio<=1)
		{
			   if(left_right)
			   {
			if(((sect_incline<0)&&(sect_incline<-6))||((DimX-section_last_bunch_beg)>=DimX/4))
			{
				prior=-1;
				goto M;
			}
			   }

		}

	}//onlyublast_cor23.08.17
	if(success_oppos==3)
	{//last_cor27.12.16
		if(!sc_gr_comp_red)
		{//last_cor21.04.17
			if(section_hue_zone==3)
			{
				if(section_sat>=4)
				{
	if((bunch_adjacent_ordered==1)&&(bunch_adjacent_ordered<=max_n_green_adj_section_bunch))
	{
		if((sect_incline>0)||(sect_incl_curve>0))
		{
			if((section_hue<=20)&&(section_gray_zone<=1))
			{
				if(count_fiber>=NumStrips/4)
				{
	sc_gr_comp_red=2;
				}
			}
		}
	}
				}
			}
		}//last_cor21.04.17
		right_oppos_green_max=RightGreenBoundarySection[count_fiber]-1;
	}
	//if((max_n_green_adj_section_bunch>=0)
		  // &&(abs(max_n_green_adj_section_bunch-bunch_adjacent_ordered)>2)&&(count_fiber<(UpperGreenBoundaryRight+3)))
			if((max_n_green_adj_section_bunch>=0)&&(first_green_adjacent>=0)
				   &&(abs(max_n_green_adj_section_bunch-bunch_adjacent_ordered)>2))
		{//condlastgr!last_cor12.12.16
			if(bunch_adjacent_ordered!=first_green_adjacent)
			{
		green_bunch_finding=GreenStripDescription(bunch_adjacent_ordered,max_n_green_adj_section_bunch,
		count_fiber,left_right,&last_gr,&tot_gr_length,&first_gr,&last_section_gr_bunch,
		&last_gr_section,&first_gr_section_bunch,&first_gr_section);
			}
			else
			{
				if(left_right)
				{
		if(max_n_green_adj_section_bunch!=(bunch_adjacent_ordered-1))
		{
		green_bunch_finding=GreenStripDescription((bunch_adjacent_ordered-1),max_n_green_adj_section_bunch,
		count_fiber,left_right,&last_gr,&tot_gr_length,&first_gr,&last_section_gr_bunch,
		&last_gr_section,&first_gr_section_bunch,&first_gr_section);
		}
				}
				else
				{
		if(max_n_green_adj_section_bunch!=(bunch_adjacent_ordered+1))
		{
		green_bunch_finding=GreenStripDescription((bunch_adjacent_ordered+1),max_n_green_adj_section_bunch,
		count_fiber,left_right,&last_gr,&tot_gr_length,&first_gr,&last_section_gr_bunch,
		&last_gr_section,&first_gr_section_bunch,&first_gr_section);
		}
				}
			}
			if((green_bunch_finding>=0)&&(first_gr>=0))
			{//!last_cor!12.12.16
				last_gr_old=ColorInt[count_fiber].old_ordered_bunch_number[last_gr];
				last_gr_beg=
	ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[last_gr_old];
				last_gr_end=
	ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[last_gr_old];
				if(left_right)
				{
	dist_form_lt_bunch_to_wmng=last_gr_beg-w_mng_end;
				}
				else
				{
	dist_form_lt_bunch_to_wmng=w_mng_beg-last_gr_end;
				}
				if(first_gr_section>=0)
				{
	last_gr_section_bunch_old=ColorInt[count_fiber].old_ordered_bunch_number[last_section_gr_bunch];
	last_gr_section_bunch_beg=
			ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[last_gr_section_bunch_old];
	last_gr_section_bunch_end=
	ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[last_gr_section_bunch_old];
	first_gr_section_weight=ColorSection->section_weight[first_gr_section];//Last_cor21.03.17
	last_gr_section_weight=ColorSection->section_weight[last_gr_section];//Last_cor21.03.17
	if(left_right)
				{
	dist_form_lt_sec_bunch_to_wmng=last_gr_section_bunch_beg-w_mng_end;
	/*if((first_gr_section_bunch>max_green_adj_section_bunch)&&
	   ((first_gr_section_weight>=DimX/4)||(last_gr_section_weight>=DimX/4)))
	{//last_cor21.03.17last_cor29.05.17last_cor29.05.17onlyub
		goto L;
	}//last_cor21.03.17*/
				}
				else
				{
	dist_form_lt_sec_bunch_to_wmng=w_mng_beg-last_gr_section_bunch_end;
	/*if((first_gr_section_bunch<max_green_adj_section_bunch)&&
	   ((first_gr_section_weight>=DimX/4)||(last_gr_section_weight>=DimX/4)))
	{//last_cor21.03.17last_cor29.05.17onlyub
		goto L;
	}//last_cor21.03.17*/
				}
				}
			}
		}//condlastgr!last_cor12.12.16
	if(bunch_adjacent_ordered>=0)
	{//bao>=
	bunch_adjacent_ordered_old=
	ColorInt[count_fiber].old_ordered_bunch_number[bunch_adjacent_ordered];
	bunch_adjacent_ordered_old_beg=
	ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[bunch_adjacent_ordered_old];
	bunch_adjacent_ordered_old_end=
	ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[bunch_adjacent_ordered_old];
	bunch_adjacent_ordered_old_length=bunch_adjacent_ordered_old_end-
	bunch_adjacent_ordered_old_beg+1;
	bunch_adjacent_ordered_old_hue=
	ColorInt[count_fiber].ColoredIntervalsStructure->AverageHue[bunch_adjacent_ordered_old];
	bunch_adjacent_ordered_old_sat=
	ColorInt[count_fiber].ColoredIntervalsStructure->AverageSat[bunch_adjacent_ordered_old];
	bunch_adjacent_ordered_old_gray=
	ColorInt[count_fiber].ColoredIntervalsStructure->AverageGray[bunch_adjacent_ordered_old];
	bunch_adjacent_ordered_old_hue_zone=hue_zones[bunch_adjacent_ordered_old_hue];
	bunch_adjacent_ordered_old_gray_zone=gray_zones[bunch_adjacent_ordered_old_gray];
	if(left_right)
	{//lr
	bunch_adjacent_ordered_section=
	SectionTraceLeft[count_fiber*MAX_COL_INT+bunch_adjacent_ordered_old];
	if(bunch_adjacent_ordered_section>0)
	{
	bunch_adjacent_ordered_section=bunch_adjacent_ordered_section%1000;
	bunch_adjacent_ordered_section-=1;
	bao_scgr=*(SkyGreenComponents+bunch_adjacent_ordered_section);
	bao_scgr=bao_scgr%10;
bao_section_weight=
ColorSection->section_weight[bunch_adjacent_ordered_section];
bao_section_first=
	ColorSection->section_fibre_first[bunch_adjacent_ordered_section];
	bao_section_last=
	ColorSection->section_fibre_last[bunch_adjacent_ordered_section];
	bao_section_weight_normalized=bao_section_weight/(bao_section_last-bao_section_first+1);
	bao_section_first_ratio=(16*bao_section_first)/(NumStrips-1);//last_cor31.10.16
	bao_section_last_ratio=(16*bao_section_last)/(NumStrips-1);
	bao_section_incline=
	*(InclinedComponent+bunch_adjacent_ordered_section);

		if(bao_scgr==2)
		{
if((bao_section_first>=section_beg)&&(bao_section_last>=section_end))
{
	if(bao_section_incline<0)
	{
		if((bao_section_weight>=3*DimX)&&(bao_section_weight_normalized>=DimX/4))
		{
			break;
		}
	}
}
		}
	}
	else
	{
	bunch_adjacent_ordered_section=-1;
	}
	bunch_adjacent_ordered_section_oppos=
	SectionTraceRight[count_fiber*MAX_COL_INT+bunch_adjacent_ordered_old];
	if(bunch_adjacent_ordered_section_oppos>0)
	{
	bunch_adjacent_ordered_section_oppos=bunch_adjacent_ordered_section_oppos%1000;
	bunch_adjacent_ordered_section_oppos-=1;
	}
	else
	{
	bunch_adjacent_ordered_section_oppos=-1;
	}
	}//lr
	else
	{//elr
	bunch_adjacent_ordered_section=
	SectionTraceRight[count_fiber*MAX_COL_INT+bunch_adjacent_ordered_old];
	if(bunch_adjacent_ordered_section>0)
	{
	bunch_adjacent_ordered_section-=1;
	bunch_adjacent_ordered_section=bunch_adjacent_ordered_section%1000;
	bao_scgr=*(SkyGreenComponents+bunch_adjacent_ordered_section);
	bao_scgr=bao_scgr%10;
bao_section_weight=
ColorSection->section_weight[bunch_adjacent_ordered_section];
bao_section_first=
	ColorSection->section_fibre_first[bunch_adjacent_ordered_section];
	bao_section_last=
	ColorSection->section_fibre_last[bunch_adjacent_ordered_section];
	bao_section_weight_normalized=bao_section_weight/(bao_section_last-bao_section_first+1);
	bao_section_first_ratio=(16*bao_section_first)/(NumStrips-1);//last_cor31.10.16
	bao_section_last_ratio=(16*bao_section_last)/(NumStrips-1);
	bao_section_incline=
	*(InclinedComponent+bunch_adjacent_ordered_section);
	if(bao_scgr==2)
		{
if((bao_section_first>=section_beg)&&(bao_section_last>=section_end))
{
	if(bao_section_incline>0)
	{
		if((bao_section_weight>=3*DimX)&&(bao_section_weight_normalized>=DimX/4))
		{
			break;
		}
	}
}
		}
	}
	else
	{
	bunch_adjacent_ordered_section=-1;
	}
	bunch_adjacent_ordered_section_oppos=
	SectionTraceLeft[count_fiber*MAX_COL_INT+bunch_adjacent_ordered_old];
	if(bunch_adjacent_ordered_section_oppos>0)
	{
	bunch_adjacent_ordered_section_oppos-=1;
	bunch_adjacent_ordered_section_oppos=bunch_adjacent_ordered_section_oppos%1000;
	}
	else
	{
	bunch_adjacent_ordered_section_oppos=-1;
	}
	}//elr
	}
	if(first_green_adjacent>=0)
	{//fga>=0
	green_bunch_number[array_num]=first_green_adjacent+1;
	old_first_green_adjacent=
	ColorInt[count_fiber].old_ordered_bunch_number[first_green_adjacent];
	old_first_beg=
	ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[old_first_green_adjacent];
	old_first_end=
	ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[old_first_green_adjacent];
	old_first_length=old_first_end-old_first_beg+1;
	old_first_signif=
	ColorInt[count_fiber].ColoredIntervalsStructure->Significance[old_first_green_adjacent];
	old_first_density=(2*old_first_signif)/old_first_length;
	f_g_bunch_length_ratio=(16*old_first_length)/(old_first_length+int_length);
	f_g_bunch_density_ratio=(16*old_first_density)/(old_first_density+int_density);
	n_o_g_contrast=
	ColorInt[count_fiber].Test_Junction_Corrected(bunch_old_number,old_first_green_adjacent,
	bunch_occurrence,
	Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
		n_o_g_contrast=n_o_g_contrast%10;
		if(n_o_g_contrast>3)
		{
			if((hue_close==1)&&(((gray_close>=1)&&(gray_close<=2))||(saturation_close==1)))
			{
				n_o_g_contrast=3;
			}
		}
	if((last_gr_in_chain>=0)&&(last_gr_in_chain!=first_green_adjacent))
		{//lgich
	old_first_green_adjacent_lg=
	ColorInt[count_fiber].old_ordered_bunch_number[last_gr_in_chain];
	old_first_beg_lg=
	ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[old_first_green_adjacent_lg];
	old_first_end_lg=
	ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[old_first_green_adjacent_lg];
	old_first_length_lg=old_first_end_lg-old_first_beg_lg+1;
	old_first_signif_lg=
	ColorInt[count_fiber].ColoredIntervalsStructure->Significance[old_first_green_adjacent_lg];
	old_first_density_lg=(2*old_first_signif_lg)/old_first_length_lg;
	f_g_bunch_length_ratio_lg=(16*old_first_length_lg)/(old_first_length_lg+int_length);
	f_g_bunch_density_ratio_lg=(16*old_first_density_lg)/(old_first_density_lg+int_density);
	n_o_g_contrast_lg=
	ColorInt[count_fiber].Test_Junction_Corrected(bunch_old_number,old_first_green_adjacent_lg,
	bunch_occurrence,
	3,indic1_length,ratio1_length,ratio_length1,&hue1_close,&saturation1_close,&gray1_close);
		n_o_g_contrast_lg=n_o_g_contrast_lg%10;
		if(n_o_g_contrast_lg>3)
		{
			if((hue1_close==1)&&((gray1_close>=1)&&(gray1_close<=2)))
			{
				n_o_g_contrast_lg=3;
			}
		}
		}//lgich
	if(left_right)
	{
	green_adjacent_section=
	SectionTraceLeft[count_fiber*MAX_COL_INT+old_first_green_adjacent];
	green_adjacent_section_oppos=
	SectionTraceRight[count_fiber*MAX_COL_INT+old_first_green_adjacent];
	dist_next_green=beg_int-old_first_end;
	if((abs(dist_next_green<2*StripWidth))&&(n_o_g_contrast>=1)&&(n_o_g_contrast<=3))
	     {
			 if(RightGreenBoundaryBunch[count_fiber]==old_first_green_adjacent+1)
			 {
				 adjacent_boundary++;
			 }
	     }
	}
	else
	{
	green_adjacent_section=
	SectionTraceRight[count_fiber*MAX_COL_INT+old_first_green_adjacent];

	green_adjacent_section_oppos=
	SectionTraceLeft[count_fiber*MAX_COL_INT+old_first_green_adjacent];
	dist_next_green=old_first_beg-end_int;
	if((abs(dist_next_green<2*StripWidth))&&(n_o_g_contrast>=1)&&(n_o_g_contrast<=3))
	     {
			 if(LeftGreenBoundaryBunch[count_fiber]==old_first_green_adjacent+1)
			 {
				 adjacent_boundary++;
			 }
	     }
	}
	green_adjacent_section=green_adjacent_section%1000;
	green_adjacent_section_oppos=green_adjacent_section_oppos%1000;
	if(green_adjacent_section>0)
	{//gas>0
		green_adjacent_section-=1;
		if(!count_adj_green_sec)
		{
		adj_sec_green[0]=green_adjacent_section+1;
		count_adj_green_sec++;
		}

		if(count_adj_green_sec>=1)
		{
	green_adjacent_section_previous=adj_sec_green[(count_adj_green_sec-1)]-1;
	if((green_adjacent_section!=green_adjacent_section_previous)&&
	   (count_adj_green_sec<=7))
	{
	adj_sec_green[count_adj_green_sec]=green_adjacent_section+1;
	count_adj_green_sec++;
	}
		}

		green_section_number[array_num]=green_adjacent_section+1;
		green_adjacent_section_weight=ColorSection->section_weight[green_adjacent_section];
		green_adjacent_section_beg=ColorSection->section_fibre_first[green_adjacent_section];
		green_adjacent_section_end=ColorSection->section_fibre_last[green_adjacent_section];
		green_adjacent_section_length=green_adjacent_section_end-green_adjacent_section_beg+1;
		green_adjacent_section_weight_normalized=green_adjacent_section_weight/
			green_adjacent_section_length;
		green_adjacent_section_hue=ColorSection->section_mean_hue[green_adjacent_section];
	green_adjacent_section_sat=ColorSection->section_mean_saturation[green_adjacent_section];
	green_adjacent_section_gray=ColorSection->section_mean_gray[green_adjacent_section];
	green_adjacent_section_hue_zone=hue_zones[green_adjacent_section_hue];
	green_adjacent_section_gray_zone=gray_zones[green_adjacent_section_gray];
	green_adjacent_section_incline=
	*(InclinedComponent+green_adjacent_section);
	green_adjacent_section_incl_curve=
	*(InclineCurve+green_adjacent_section);
		/*if(green_adjacent_section!=last_green_adjacent_section)!!!!
		{
	    last_green_adjacent_section=green_adjacent_section;
		number_of_dif_right_adjacent_green_sections++;
		}
	number_of_right_adjacent_green_sections++;*/

	}//gas>0
	else
	{
	green_adjacent_section=-1;
	}
		if(max_green_adj_section>=0)
	{//mgas
	w_mg_old=ColorInt[count_fiber].old_ordered_bunch_number[max_green_adj_section_bunch];
	w_mg_beg=ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[w_mg_old];
	w_mg_end=ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[w_mg_old];
	w_mg_length=w_mg_end-w_mg_beg+1;
	if(left_right)
	{
	dist_to_wmg=beg_int-w_mg_end;
	if(first_green_adjacent==last_gr_in_chain)
	{
	dist_fga_wmg=old_first_beg-w_mg_end;
	}
	else
	{
	dist_fga_wmg=old_first_beg_lg-w_mg_end;
	}
	}
	else
	{
	dist_to_wmg=w_mg_beg-end_int;
	if(first_green_adjacent==last_gr_in_chain)
	{
	dist_fga_wmg=w_mg_end-old_first_end;
	}
	else
	{
	dist_fga_wmg=w_mg_end-old_first_end_lg;
	}
	}

	w_mg_section_weight=ColorSection->section_weight[max_green_adj_section];
	w_mg_section_first=
	ColorSection->section_fibre_first[max_green_adj_section];
	w_mg_section_last=
	ColorSection->section_fibre_last[max_green_adj_section];
	w_mg_section_color=
	SkyGreenComponents[max_green_adj_section]%10;
	w_mg_section_incline=
	*(InclinedComponent+max_green_adj_section);
	number_of_boun_points_in_max_adj=
	LeftRightSectionNumberOfRightGreenBoundaryPoints[max_green_adj_section];
	w_mg_incl_curve=*(InclineCurve+max_green_adj_section);
	w_mg_vert=*(VerticalComponent+max_green_adj_section);
	w_mg_sec_length=w_mg_section_last-w_mg_section_first+1;
	w_mg_section_weight_normalized=w_mg_section_weight/w_mg_sec_length;
	w_mg_section_hue=ColorSection->section_mean_hue[max_green_adj_section];
	w_mg_section_sat=ColorSection->section_mean_saturation[max_green_adj_section];
	w_mg_section_gray=ColorSection->section_mean_gray[max_green_adj_section];
	w_mg_section_hue_zone=hue_zones[w_mg_section_hue];
	w_mg_section_gray_zone=gray_zones[w_mg_section_gray];//error25.01.17
	w_mg_boun_point_ratio=(16*number_of_boun_points_in_max_adj)/w_mg_sec_length;
	w_mg_current_sec_weight_ratio=(16*w_mg_section_weight)/(w_mg_section_weight+section_weight);
	if(left_right)
		{//last_cor24.04.17
	w_mg_section_oppos2=
	SectionTraceLeft[count_fiber*MAX_COL_INT+w_mg_old];
		}
		else
		{
	w_mg_section_oppos2=
	SectionTraceRight[count_fiber*MAX_COL_INT+w_mg_old];
		}//last_cor24.04.17
	if(w_mg_section_oppos2>0)
	{//last_cor24.04.17
	w_mg_section_oppos2=w_mg_section_oppos2%1000;
	w_mg_section_oppos2-=1;
	w_mg_section_weight_oppos2=ColorSection->section_weight[w_mg_section_oppos2];
	w_mg_section_first_oppos2=
	ColorSection->section_fibre_first[w_mg_section_oppos2];
	w_mg_section_last_oppos2=
	ColorSection->section_fibre_last[w_mg_section_oppos2];
	w_mg_section_oppos_incline2=
	*(InclinedComponent+w_mg_section_oppos2);
	w_mg_incl_curve_oppos2=*(InclineCurve+w_mg_section_oppos2);
	w_mg_vert_oppos2=*(VerticalComponent+w_mg_section_oppos2);
	}//last_cor24.04.17
	//w_mng_section_sky=SkyGreenComponents[max_green_adj_section];
	if(!count_adj_green_sec_max)
		{
		adj_sec_green_max[0]=max_green_adj_section+1;
		count_adj_green_sec_max++;
		}

		if(count_adj_green_sec_max>=1)
		{
			if(count_adj_green_sec_max==1)
			{
	max_adj_sec_green=adj_sec_green_max[0]-1;
	mgm_incline=
	*(InclinedComponent+max_adj_sec_green);
	max_adj_sec_green_weight=ColorSection->section_weight[max_adj_sec_green];
	first_max_adj_sec_green_strip=ColorSection->section_fibre_first[max_adj_sec_green];
	last_max_adj_sec_green_strip=ColorSection->section_fibre_last[max_adj_sec_green];
	if(last_max_adj_sec_green_strip>count_fiber)
	{
	w_mg_max_old=ColorDescrSect[max_adj_sec_green].location_of_section[count_fiber];
	if(w_mg_max_old>0)
	{
	w_mg_max_old--;
	w_mg_max_old=ColorInt[count_fiber].old_bunch_number[w_mg_max_old];
	w_mg_max_old_ordered=ColorInt[count_fiber].ordered_bunch_number[w_mg_max_old];
	}
	}
			}
	green_adjacent_section_previous=adj_sec_green_max[(count_adj_green_sec_max-1)]-1;
	if((max_green_adj_section!=green_adjacent_section_previous)&&
	   (count_adj_green_sec_max<=7))
	{
	adj_sec_green_max[count_adj_green_sec_max]=max_green_adj_section+1;
	count_adj_green_sec_max++;
	}
		}
	}//mgas

		              if(w_mg_section_oppos>=0)
		               {//rso
	w_mg_section_weight_oppos=ColorSection->section_weight[w_mg_section_oppos];
	w_mg_section_first_oppos=
	ColorSection->section_fibre_first[w_mg_section_oppos];
	w_mg_section_last_oppos=
	ColorSection->section_fibre_last[w_mg_section_oppos];
	w_mg_section_oppos_incline=
	*(InclinedComponent+w_mg_section_oppos);
	w_mg_incl_curve_oppos=*(InclineCurve+w_mg_section_oppos);
	w_mg_vert_oppos=*(VerticalComponent+w_mg_section_oppos);
	if(!count_adj_green_sec_max_oppos)
		{
		adj_sec_green_max_oppos[0]=w_mg_section_oppos+1;
		count_adj_green_sec_max_oppos++;
		}
	/*if(count_adj_n_green_sec_max_oppos>=1)
				{
				max_adj_sec=adj_sec_n_green_max_oppos[0]-1;
	            max_adj_sec_weight=ColorSection->section_weight[max_adj_sec];
	            first_max_adj_sec_strip=ColorSection->section_fibre_first[max_adj_sec];
				last_max_adj_sec_strip=ColorSection->section_fibre_last[max_adj_sec];
				}*/
		if(count_adj_green_sec_max_oppos>=1)
		{
	green_adjacent_section_previous_oppos=
	adj_sec_green_max_oppos[(count_adj_green_sec_max_oppos-1)]-1;
	if((w_mg_section_oppos!=green_adjacent_section_previous_oppos)&&
	   (count_adj_green_sec_max_oppos<=7))
	{
	adj_sec_green_max_oppos[count_adj_green_sec_max_oppos]=w_mg_section_oppos+1;
	count_adj_green_sec_max_oppos++;
	}
		}
					  }//rso


	if(green_adjacent_section_oppos>0)
	{//gaso>0
		green_adjacent_section_oppos-=1;
		if(!count_adj_green_sec_oppos)
		{
		adj_sec_green_oppos[0]=green_adjacent_section_oppos+1;
		count_adj_green_sec_oppos++;
		}

		if(count_adj_green_sec_oppos>=1)
		{
	green_adjacent_section_oppos_previous=adj_sec_green_oppos[(count_adj_green_sec_oppos-1)]-1;
	if((green_adjacent_section_oppos!=green_adjacent_section_oppos_previous)&&
	   (count_adj_green_sec_oppos<=7))
	{
	adj_sec_green_oppos[count_adj_green_sec_oppos]=green_adjacent_section_oppos+1;
	count_adj_green_sec_oppos++;
	}
		}

		//green_section_number[array_num]=green_adjacent_section;
		green_adjacent_section_oppos_weight=
		ColorSection->section_weight[green_adjacent_section_oppos];
		green_adjacent_section_oppos_beg=
		ColorSection->section_fibre_first[green_adjacent_section_oppos];
		green_adjacent_section_oppos_end=
		ColorSection->section_fibre_last[green_adjacent_section_oppos];
		green_adjacent_section_oppos_length=
		green_adjacent_section_oppos_end-green_adjacent_section_oppos_beg+1;
		number_of_boun_points_in_oppos=
	LeftRightSectionNumberOfRightGreenBoundaryPoints[green_adjacent_section_oppos];
	}//gaso>0
	else
	{
	green_adjacent_section_oppos=-1;
	}
	}//fga>=0
	if(max_n_green_adj_section>=0)
	{//mngas
	w_mng_old=ColorInt[count_fiber].old_ordered_bunch_number[max_n_green_adj_section_bunch];

		if(left_right)
		{//last_cor18.04.17
	w_mng_section_oppos2=
	SectionTraceLeft[count_fiber*MAX_COL_INT+w_mng_old];
		}
		else
		{
	w_mng_section_oppos2=
	SectionTraceRight[count_fiber*MAX_COL_INT+w_mng_old];
		}//last_cor18.04.17
	if(w_mng_section_oppos2>0)
	{//last_cor18.04.17
	w_mng_section_oppos2=w_mng_section_oppos2%1000;
	w_mng_section_oppos2-=1;
	w_mng_section_oppos2_weight=ColorSection->section_weight[w_mng_section_oppos2];
	w_mng_section_oppos2_incline=*(InclinedComponent+w_mng_section_oppos2);
	w_mng_section_oppos2_incl_curve=*(InclineCurve+w_mng_section_oppos2);
	w_mng_section_oppos2_beg=ColorSection->section_fibre_first[w_mng_section_oppos2];
	w_mng_section_oppos2_end=ColorSection->section_fibre_last[w_mng_section_oppos2];
	}
	else
	{
	w_mng_section_oppos2=-1;
	}//last_cor18.04.17
	w_mng_beg=ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[w_mng_old];
	w_mng_end=ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[w_mng_old];
	w_mng_length=w_mng_end-w_mng_beg+1;
	w_mng_hue=
	ColorSection->section_mean_hue[max_n_green_adj_section];
	w_mng_sat=
	ColorSection->section_mean_saturation[max_n_green_adj_section];
	w_mng_gray=
	ColorSection->section_mean_gray[max_n_green_adj_section];
	w_mng_hue_zone=hue_zones[w_mng_hue];
	w_mng_gray_zone=gray_zones[w_mng_gray];
	if(left_right)
		{
		dist_to_wmng=beg_int-w_mng_end;
		if(max_green_adj_section>=0)
		{
			dist_wmng_to_mga=w_mng_beg-w_mg_end;
		}
		if(first_green_adjacent>=0)
		{
			if(first_green_adjacent==last_gr_in_chain)
					{//last_cor13.12.16
		dist1gr_to_wmng=old_first_beg-w_mng_end;
					}
			       else
			       {
				    dist1gr_to_wmng=old_first_beg_lg-w_mng_end;
			        }
		}
		}
		else
		{
		dist_to_wmng=w_mng_beg-end_int;
		if(first_green_adjacent>=0)
		{
			if(first_green_adjacent==last_gr_in_chain)
			{//last_cor13.12.16
		dist1gr_to_wmng=w_mng_beg-old_first_end;
			}
			else
			{
				dist1gr_to_wmng=w_mng_beg-old_first_end_lg;
			}
		}
		if(max_green_adj_section>=0)
		{
			dist_wmng_to_mga=w_mg_beg-w_mng_end;
		}
		}

	w_mng_section_weight=ColorSection->section_weight[max_n_green_adj_section];
	w_mng_section_first=
	ColorSection->section_fibre_first[max_n_green_adj_section];
	w_mng_section_last=
	ColorSection->section_fibre_last[max_n_green_adj_section];
	w_mng_section_weight_normalized=w_mng_section_weight/(w_mng_section_last-w_mng_section_first+1);
	w_mng_section_first_ratio=(16*w_mng_section_first)/(NumStrips-1);//last_cor31.10.16
	w_mng_section_last_ratio=(16*w_mng_section_last)/(NumStrips-1);
	w_mng_section_incline=
	*(InclinedComponent+max_n_green_adj_section);
	w_mng_incl_curve=*(InclineCurve+max_n_green_adj_section);
	//last_cor31.10.16

	//last_cor31.10.16
	w_mng_section_sky=SkyGreenComponents[max_n_green_adj_section];
	if(!count_adj_n_green_sec_max)
		{
		adj_sec_n_green_max[0]=max_n_green_adj_section+1;
		count_adj_n_green_sec_max++;
		     if(count_adj_n_green_sec_max==1)
		     {
				 accumulated_weight=w_mng_section_weight;
		     }
		}

		if(count_adj_n_green_sec_max>=1)
		{
	n_green_adjacent_section_previous=adj_sec_n_green_max[(count_adj_n_green_sec_max-1)]-1;
	if((max_n_green_adj_section!=n_green_adjacent_section_previous)&&
	   (count_adj_n_green_sec_max<=7))
	{
	adj_sec_n_green_max[count_adj_n_green_sec_max]=max_n_green_adj_section+1;
	section_fit=SectionCorrespondence(n_green_adjacent_section_previous,max_n_green_adj_section);
	count_adj_n_green_sec_max++;
	}
	//if(count_adj_n_green_sec_max_oppos==1)
				//{
				/*max_adj_sec=adj_sec_n_green_max[0]-1;
				if(max_adj_sec>=0)
				{
	            max_adj_sec_weight=ColorSection->section_weight[max_adj_sec];
	            first_max_adj_sec_strip=ColorSection->section_fibre_first[max_adj_sec];
				last_max_adj_sec_strip=ColorSection->section_fibre_last[max_adj_sec];
				}*/
				//}
		}
	}//mngas
	            max_adj_sec=adj_sec_n_green_max[0]-1;
				if(max_adj_sec>=0)
				{
	            max_adj_sec_weight=ColorSection->section_weight[max_adj_sec];
	            first_max_adj_sec_strip=ColorSection->section_fibre_first[max_adj_sec];
				last_max_adj_sec_strip=ColorSection->section_fibre_last[max_adj_sec];
				}
		              if(w_mng_section_oppos>=0)
		               {//rso
	w_mng_old_oppos=ColorInt[count_fiber].old_ordered_bunch_number[w_mng_section_oppos_bunch];
	w_mng_beg_oppos=ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[w_mng_old_oppos];
	w_mng_end_oppos=ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[w_mng_old_oppos];
	w_mng_length_oppos=w_mng_end_oppos-w_mng_beg_oppos+1;
	w_mng_section_weight_oppos=ColorSection->section_weight[w_mng_section_oppos];
	w_mng_section_first_oppos=
	ColorSection->section_fibre_first[w_mng_section_oppos];
	w_mng_section_last_oppos=
	ColorSection->section_fibre_last[w_mng_section_oppos];
	w_mng_section_first_ratio_oppos=(16*w_mng_section_first_oppos)/(NumStrips-1);//last_cor31.10.16
	w_mng_section_last_ratio_oppos=(16*w_mng_section_last_oppos)/(NumStrips-1);
	w_mng_section_oppos_incline=
	*(InclinedComponent+w_mng_section_oppos);
	w_mng_incl_curve_oppos=*(InclineCurve+w_mng_section_oppos);
	w_mng_hue_oppos=ColorSection->section_mean_hue[w_mng_section_oppos];//last_cor31.10.16
	w_mng_sat_oppos=ColorSection->section_mean_saturation[w_mng_section_oppos];
	w_mng_gray_oppos=ColorSection->section_mean_gray[w_mng_section_oppos];
	w_mng_hue_zone_oppos=hue_zones[w_mng_hue_oppos];
	w_mng_gray_zone_oppos=gray_zones[w_mng_gray_oppos];
	if(left_right)
	{
	dist_wmng_oppos_to_sect=beg_int-w_mng_end_oppos;
	}
	else
	{
	dist_wmng_oppos_to_sect=w_mng_beg_oppos-end_int;
	}
	w_mng_section_sky_oppos=
	SkyGreenComponents[w_mng_section_oppos];
	if(!count_adj_n_green_sec_max_oppos)
		{
		adj_sec_n_green_max_oppos[0]=w_mng_section_oppos+1;
		count_adj_n_green_sec_max_oppos++;
		}

		if(count_adj_n_green_sec_max_oppos>=1)
		{
	n_green_adjacent_section_previous_oppos=
	adj_sec_n_green_max_oppos[(count_adj_n_green_sec_max_oppos-1)]-1;
	if((w_mng_section_oppos!=n_green_adjacent_section_previous_oppos)&&
	   (count_adj_n_green_sec_max_oppos<=7))
	{
	adj_sec_n_green_max[count_adj_n_green_sec_max_oppos]=w_mng_section_oppos+1;
	count_adj_n_green_sec_max_oppos++;
	}
		}
					  }//rso

				  if(first_green_adjacent==bunch_adjacent_ordered)
				  {//fga=bao
					  if(left_right)
					  {
	if(!old_first_beg)
	{
		if(count_fiber>=NumStrips/2)
		{
			break;
		}
	}
	else
	{
		if((last_gr_in_chain>=0)&&(last_gr_in_chain!=first_green_adjacent))
		{//lgich
	if(!old_first_beg_lg)
	{
		if(count_fiber>=NumStrips/2)
		{
			break;
		}
	}
		}//lgich
	}
					  }
					  else
					  {
	if(old_first_end==(DimX-1))
	{
		if(count_fiber>=NumStrips/2)
		{
			break;
		}
	}
	else
	{
		if((last_gr_in_chain>=0)&&(last_gr_in_chain!=first_green_adjacent))
		{//lgich
	if(old_first_end_lg==(DimX-1))
	{
		if(count_fiber>=NumStrips/2)
		{
			break;
		}
	}
		}//lgich
	}
					  }
				  }//fga=bao
	if(((w_mng_section_sky_oppos==1)||(w_mng_section_sky==1))&&((count_fiber>=(3*NumStrips/4))||
	   ((count_fiber>=(2*NumStrips/3))&&((w_mng_section_weight_oppos>=1000)||
	   (w_mng_section_weight>=1000)))))
	{
		break;
	}
	if((w_mng_section_sky_oppos==1)&&(w_mng_section_sky==1)&&(count_fiber>=(3*NumStrips)/5))
	{
		break;
	}
	if((w_mng_section_sky_oppos==1)&&(w_mng_section_weight_oppos>=500)&&
	   (w_mng_section_first_oppos>NumStrips/2)&&(w_mng_section_last_oppos>=(3*NumStrips)/4))
	{
	break;
	}
	if((w_mng_section_sky==1)&&(w_mng_section_weight>=500)&&
	   (w_mng_section_first>NumStrips/2)&&(w_mng_section_last>=(3*NumStrips)/4))
	{
	break;
	}
	/*if((dist_to_wmg>=0)&&(dist_to_wmng>2*dist_to_wmg)&&(dist_to_wmg<=2*StripWidth)&&
	   (first_green_adjacent>=0))
	{
	goto L;
	}*/
	//last_cor20.02.17
	/*if((first_green_adjacent>=0)&&(n_o_g_contrast>=1)&&(n_o_g_contrast<=3)last_cor29.0517onlyub
	   &&(((green_adjacent_section>=0)&&(green_adjacent_section_weight>300))
	   ||(abs(dist_next_green)>=4*StripWidth)))
	{
		   if((max_n_green_adj_section_bunch>=0)&&(w_mng_section_weight>=1500))
		    {
		if(left_right)
		{//lr
			if((first_green_adjacent>max_n_green_adj_section_bunch))
			{
				goto L;
			}
		}//lr
		else
		{
	if((first_green_adjacent<max_n_green_adj_section_bunch))
			{
				goto L;
			}
		}
		   }
	}*/
	if(maximum_non_green_adjacent<0)
	{
	first_n_g_bunch[array_num]=first_n_green+1;
	max_n_green_bunch[array_num]=maximum_non_green_adjacent+1;
	adjacent_n_green_length[array_num]=non_green_length;
	}
	else
	{//emnga<0
	first_n_g_bunch[array_num]=first_n_green+1;
	old_first_n_green_adjacent=ColorInt[count_fiber].old_ordered_bunch_number[first_n_green];
	old_first_n_beg=
	ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[old_first_n_green_adjacent];
	old_first_n_end=
	ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[old_first_n_green_adjacent];
	old_first_n_length=old_first_n_end-old_first_n_beg+1;
	old_first_n_signif=
	ColorInt[count_fiber].ColoredIntervalsStructure->Significance[old_first_n_green_adjacent];
	old_first_n_density=(2*old_first_n_signif)/old_first_n_length;
	if(left_right)
	{
	n_green_adjacent_section=
	SectionTraceRight[count_fiber*MAX_COL_INT+old_first_n_green_adjacent];
	}
	else
	{
	n_green_adjacent_section=
	SectionTraceLeft[count_fiber*MAX_COL_INT+old_first_n_green_adjacent];
	}
	n_green_adjacent_section=n_green_adjacent_section%1000;
	if(n_green_adjacent_section>0)
	{//ngas>0
		n_green_adjacent_section-=1;
		n_green_adjacent_section_beg=ColorSection->section_fibre_first[n_green_adjacent_section];
		n_green_adjacent_section_end=ColorSection->section_fibre_last[n_green_adjacent_section];
		n_green_adjacent_section_length=
	n_green_adjacent_section_end-n_green_adjacent_section_beg+1;
		n_green_adjacent_section_weight=
	ColorSection->section_weight[n_green_adjacent_section];
		n_green_adjacent_section_weight_normalized=
	n_green_adjacent_section_weight/n_green_adjacent_section_length;
	}//ngas>0
	adjacent_n_green_length[array_num]=non_green_length;
	mng_old=ColorInt[count_fiber].old_ordered_bunch_number[maximum_non_green_adjacent];
	mng_beg=ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[mng_old];
	mng_end=ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[mng_old];
	mng_length=mng_end-mng_beg+1;
	mng_hue=
	ColorInt[count_fiber].ColoredIntervalsStructure->AverageHue[mng_old];
	mng_sat=
	ColorInt[count_fiber].ColoredIntervalsStructure->AverageSat[mng_old];
	mng_gray=
	ColorInt[count_fiber].ColoredIntervalsStructure->AverageGray[mng_old];
	mng_hue_zone=hue_zones[mng_hue];
	mng_gray_zone=gray_zones[mng_gray];
	mng_section=SectionTraceRight[count_fiber*MAX_COL_INT + mng_old];
	mng_section_oppos=SectionTraceLeft[count_fiber*MAX_COL_INT + mng_old];
	if(mng_section>0)
	{//mng>0
	mng_section=mng_section%1000;
		              if(mng_section>0)
		               {//rs
			           mng_section-=1;
	mng_section_weight=ColorSection->section_weight[mng_section];
	mng_section_first=
	ColorSection->section_fibre_first[mng_section];
	mng_section_last=
	ColorSection->section_fibre_last[mng_section];
					  }
	}//mng>0
	if(mng_section_oppos>0)
	{//mngso>
	mng_section_oppos=mng_section_oppos%1000;
		              if(mng_section_oppos>0)
		               {//rso
			           mng_section_oppos-=1;
	mng_section_weight_oppos=ColorSection->section_weight[mng_section_oppos];
	mng_section_first_oppos=
	ColorSection->section_fibre_first[mng_section_oppos];
	mng_section_last_oppos=
	ColorSection->section_fibre_last[mng_section_oppos];
					  }//rso
	}//mngso>
	n_g_first=last_n_green;
	n_g_last=first_n_green;
	if(n_g_first>=0)
		{
	n_g_sum=NonGreenSummation(left_right,n_g_first,n_g_last,count_fiber);
	adjacent_n_green_length[array_num]=n_g_sum;
		}
	max_n_green_bunch[array_num]=maximum_non_green_adjacent+1;
	}//emnga<0

	          if(left_right)
				{//lr
	if((end_int>(StripLength-StripWidth))||(((bunch_ordered_number==(number_of_covering_bunches-1))&&
	((bunch_ordered_number>0)||(sec_ordered_bunch>0)))))
		{
						success_inverse=-2;
						oppos_extreme_right_boundary_count++;
		}
	if(((bunch_adjacent_ordered==(number_of_covering_bunches-1))&&(bunch_adjacent_ordered>=0)&&
	   (end_int>(StripLength-StripWidth)))||
						((bunch_ordered_number==(number_of_covering_bunches-1))&&
	((bunch_ordered_number>0)||(sec_ordered_bunch>0))))
					{//endcond
		if((first_green_adjacent<0)||
	((first_green_adjacent==last_green_adjacent)&& (dist_next_green>old_first_length)&&
	 (dist_next_green>StripWidth)&&(int_density>2*old_first_density)))
		{
	if(sc_gr_comp_red==2)
	{
	  if(w_mng_section_weight>=1000)
	  {
		if(section_weight>=300)
		{
			if(!RightGreenBoundaryBunch[count_fiber])
			{
			condition_number=0;
		    success_right=1;
			new_boundary_section=section_number;
			goto L;
			}
			else
			{
				goto L;
			}
		}
		if(sc_gr_comp_red==2)
		{
	if((non_green_length>=DimX/2)&&(oppos_extreme_right_boundary_count>0))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
			{
			condition_number=2;
		    success_right=1;
			new_boundary_section=section_number;
			goto L;
			}
	        else
			{
				goto L;
			}
	}
		}
	  }
	}
		}

					}//endcond
				}//lr
				else
				{//elr
					if(((!bunch_ordered_number)&&(sec_ordered_bunch>0))||(beg_int<StripWidth))
					{
	oppos_extreme_left_boundary_count++;
	if((first_green_adjacent<0)||
	((first_green_adjacent>=0)&&(first_green_adjacent==last_green_adjacent)&& (dist_next_green>old_first_length)&&
	 (dist_next_green>StripWidth)&&(int_density>2*old_first_density)))
		{
	if(sc_gr_comp_red==2)
	{
	 if(w_mng_section_weight>=1000)
	 {
		if(section_weight>=300)
		{
			if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=1;
	success_left=1;
	new_boundary_section=section_number;
			goto L;
			}
			else
			{
				goto L;
			}
		}
		if(sc_gr_comp_red==2)
		{
	if((non_green_length>=DimX/2)&&(oppos_extreme_left_boundary_count>0))
	{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=3;
	success_left=1;
	new_boundary_section=section_number;
			goto L;
			}
	else
			{
				goto L;
			}
	}
		}
	 }
	}
		}
					success_inverse=-2;
					}
				}//elr
				}//sop
				if((oppos_adjacent>=0)&&(success_inverse!=-2))
				{//oasi
	old_oppos_adjacent=
	ColorInt[count_fiber].old_ordered_bunch_number[oppos_adjacent];
	op_adj_beg=ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[old_oppos_adjacent];
	op_adj_end=ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[old_oppos_adjacent];
	success_inverse=
	NextGreenComponents(!left_right,oppos_adjacent,count_fiber,bunch_old_number,section_number,
	0,0,&first_green_adjacent_oppos,&last_green_adjacent_oppos,&maximum_non_green_adjacent_oppos,
	&non_green_length_oppos,&first_n_green_oppos,&last_n_green_oppos,&last_gr_in_chain_oppos,
	&first_green_chain_length_oppos,&max_green_adj_section_oppos,&max_green_adj_section_bunch_oppos,
	&w_mg_section_oppos1,&w_mg_section_oppos_bunch1,&max_n_green_adj_section_oppos,
	&max_n_green_adj_section_bunch_oppos,&w_mng_section_oppos1,&w_mng_section_oppos_bunch1,
	&green_lg_before1,0);
	if(first_green_adjacent_oppos>=0)
	{//fgao>=
		old_first_green_adjacent_oppos=
	ColorInt[count_fiber].old_ordered_bunch_number[first_green_adjacent_oppos];
		old_first_green_adjacent_oppos_beg=
	ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[old_first_green_adjacent_oppos];
		old_first_green_adjacent_oppos_end=
	ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[old_first_green_adjacent_oppos];
	old_first_green_adjacent_oppos_length=old_first_green_adjacent_oppos_end-
	old_first_green_adjacent_oppos_beg+1;

	if(first_green_adjacent_oppos!=last_gr_in_chain_oppos)
						{
	old_last_gr_in_chain_oppos=
	ColorInt[count_fiber].old_ordered_bunch_number[last_gr_in_chain_oppos];
	old_last_gr_in_chain_oppos_beg=
	ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[old_last_gr_in_chain_oppos];
	old_last_gr_in_chain_oppos_end=
	ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[old_last_gr_in_chain_oppos];
	oppos_green_beg=old_last_gr_in_chain_oppos_beg;
	oppos_green_end=old_last_gr_in_chain_oppos_end;
						}
	                    else
	                    {
	oppos_green_beg=old_first_green_adjacent_oppos_beg;
	oppos_green_end=old_first_green_adjacent_oppos_end;
	                    }
	if(left_right)
	{//lr
	green_adjacent_section_op=
	SectionTraceLeft[count_fiber*MAX_COL_INT+old_first_green_adjacent_oppos];
	green_adjacent_section_op_oppos=
	SectionTraceRight[count_fiber*MAX_COL_INT+old_first_green_adjacent_oppos];
	//dist_next_green=beg_int-old_first_end;
	}//lr
	else
	{//elr
	green_adjacent_section_op=
	SectionTraceRight[count_fiber*MAX_COL_INT+old_first_green_adjacent_oppos];

	green_adjacent_section_op_oppos=
	SectionTraceLeft[count_fiber*MAX_COL_INT+old_first_green_adjacent_oppos];
	//dist_next_green=old_first_beg-end_int;
	}//elr
	green_adjacent_section_op=green_adjacent_section_op%1000;
	green_adjacent_section_op_oppos=green_adjacent_section_op_oppos%1000;
	if(green_adjacent_section_op>0)
	{//gas>0
		green_adjacent_section_op-=1;
		green_adjacent_section_op_weight=ColorSection->section_weight[green_adjacent_section_op];
		green_adjacent_section_op_beg=ColorSection->section_fibre_first[green_adjacent_section_op];
		green_adjacent_section_op_end=ColorSection->section_fibre_last[green_adjacent_section_op];
		green_adjacent_section_op_length=
			green_adjacent_section_op_end-green_adjacent_section_op_beg+1;
		number_of_boundary_points_adjacent_oppos=
	LeftRightSectionNumberOfRightGreenBoundaryPoints[green_adjacent_section_op];
		//green_adjacent_section_op_weight_normalized=green_adjacent_section_weight_op/
			//green_adjacent_section_op_length;
		/*green_adjacent_section_op_hue=ColorSection->section_mean_hue[green_adjacent_section_op];
	green_adjacent_section_op_sat=ColorSection->section_mean_saturation[green_adjacent_section_op];
	green_adjacent_section_op_gray=ColorSection->section_mean_gray[green_adjacent_section_op];
	green_adjacent_section_op_hue_zone=hue_zones[green_adjacent_section_op_hue];
	green_adjacent_section_gray_zone=gray_zones[green_adjacent_section_op_gray];*/
	green_adjacent_section_op_incline=
	*(InclinedComponent+green_adjacent_section_op);
	green_adjacent_section_op_incl_curve=
	*(InclineCurve+green_adjacent_section_op);
	}//gas>0
	else
	{
	green_adjacent_section_op=-1;
	}
	}//fgao>=
	else
	{
		oppos_green_beg=beg_int;
	    oppos_green_end=end_int;
	}
	/*if(first_green_adjacent>=0)
	{//fga>=0

		if(max_green_adj_section>=0)
	{//mgas
	w_mg_old=ColorInt[count_fiber].old_ordered_bunch_number[max_green_adj_section_bunch];
	w_mg_beg=ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[w_mg_old];
	w_mg_end=ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[w_mg_old];
	w_mg_length=w_mg_end-w_mg_beg+1;
	if(left_right)
	{
	dist_to_wmg=beg_int-w_mg_end;
	}
	else
	{
	dist_to_wmg=w_mg_beg-end_int;
	}

	w_mg_section_weight=ColorSection->section_weight[max_green_adj_section];
	w_mg_section_first=
	ColorSection->section_fibre_first[max_green_adj_section];
	w_mg_section_last=
	ColorSection->section_fibre_last[max_green_adj_section];
	w_mg_section_incline=
	*(InclinedComponent+max_green_adj_section);
	number_of_boun_points_in_max_adj=
	LeftRightSectionNumberOfRightGreenBoundaryPoints[max_green_adj_section];
	w_mg_incl_curve=*(InclineCurve+max_green_adj_section);
	w_mg_sec_length=w_mg_section_last-w_mg_section_first+1;
	w_mg_section_weight_normalized=w_mg_section_weight/w_mg_sec_length;
	w_mg_section_hue=ColorSection->section_mean_hue[max_green_adj_section];
	w_mg_section_sat=ColorSection->section_mean_saturation[max_green_adj_section];
	w_mg_section_gray=ColorSection->section_mean_gray[max_green_adj_section];
	w_mg_section_hue_zone=hue_zones[w_mg_section_hue];
	w_mg_section_gray_zone=gray_zones[w_mg_section_hue];
	w_mg_boun_point_ratio=(16*number_of_boun_points_in_max_adj)/w_mg_sec_length;
	w_mg_current_sec_weight_ratio=(16*w_mg_section_weight)/(w_mg_section_weight+section_weight);
	//w_mng_section_sky=SkyGreenComponents[max_green_adj_section];
	if(!count_adj_green_sec_max)
		{
		adj_sec_green_max[0]=max_green_adj_section+1;
		count_adj_green_sec_max++;
		}

		if(count_adj_green_sec_max>=1)
		{
			if(count_adj_green_sec_max==1)
			{
	max_adj_sec_green=adj_sec_green_max[0]-1;
	mgm_incline=
	*(InclinedComponent+max_adj_sec_green);
	max_adj_sec_green_weight=ColorSection->section_weight[max_adj_sec_green];
	first_max_adj_sec_green_strip=ColorSection->section_fibre_first[max_adj_sec_green];
	last_max_adj_sec_green_strip=ColorSection->section_fibre_last[max_adj_sec_green];
	if(last_max_adj_sec_green_strip>count_fiber)
	{
	w_mg_max_old=ColorDescrSect[max_adj_sec_green].location_of_section[count_fiber];
	if(w_mg_max_old>0)
	{
	w_mg_max_old--;
	w_mg_max_old=ColorInt[count_fiber].old_bunch_number[w_mg_max_old];
	w_mg_max_old_ordered=ColorInt[count_fiber].ordered_bunch_number[w_mg_max_old];
	}
	}
			}
	green_adjacent_section_previous=adj_sec_green_max[(count_adj_green_sec_max-1)]-1;
	if((max_green_adj_section!=green_adjacent_section_previous)&&
	   (count_adj_green_sec_max<=7))
	{
	adj_sec_green_max[count_adj_green_sec_max]=max_green_adj_section+1;
	count_adj_green_sec_max++;
	}
		}
	}//mgas

		              if(w_mg_section_oppos>=0)
		               {//rso
	w_mg_section_weight_oppos=ColorSection->section_weight[w_mg_section_oppos];
	w_mg_section_first_oppos=
	ColorSection->section_fibre_first[w_mg_section_oppos];
	w_mg_section_last_oppos=
	ColorSection->section_fibre_last[w_mg_section_oppos];
	w_mg_section_oppos_incline=
	*(InclinedComponent+w_mg_section_oppos);
	w_mg_incl_curve_oppos=*(InclineCurve+w_mg_section_oppos);
	if(!count_adj_green_sec_max_oppos)
		{
		adj_sec_green_max_oppos[0]=w_mg_section_oppos+1;
		count_adj_green_sec_max_oppos++;
		}

		if(count_adj_green_sec_max_oppos>=1)
		{
	green_adjacent_section_previous_oppos=
	adj_sec_green_max_oppos[(count_adj_green_sec_max_oppos-1)]-1;
	if((w_mg_section_oppos!=green_adjacent_section_previous_oppos)&&
	   (count_adj_green_sec_max_oppos<=7))
	{
	adj_sec_green_max_oppos[count_adj_green_sec_max_oppos]=w_mg_section_oppos+1;
	count_adj_green_sec_max_oppos++;
	}
		}
					  }//rso


	if(green_adjacent_section_oppos>0)
	{//gaso>0
		green_adjacent_section_oppos-=1;
		if(!count_adj_green_sec_oppos)
		{
		adj_sec_green_oppos[0]=green_adjacent_section_oppos+1;
		count_adj_green_sec_oppos++;
		}

		if(count_adj_green_sec_oppos>=1)
		{
	green_adjacent_section_oppos_previous=adj_sec_green_oppos[(count_adj_green_sec_oppos-1)]-1;
	if((green_adjacent_section_oppos!=green_adjacent_section_oppos_previous)&&
	   (count_adj_green_sec_oppos<=7))
	{
	adj_sec_green_oppos[count_adj_green_sec_oppos]=green_adjacent_section_oppos+1;
	count_adj_green_sec_oppos++;
	}
		}

		//green_section_number[array_num]=green_adjacent_section;
		green_adjacent_section_oppos_weight=
		ColorSection->section_weight[green_adjacent_section_oppos];
		green_adjacent_section_oppos_beg=
		ColorSection->section_fibre_first[green_adjacent_section_oppos];
		green_adjacent_section_oppos_end=
		ColorSection->section_fibre_last[green_adjacent_section_oppos];
		green_adjacent_section_oppos_length=
		green_adjacent_section_oppos_end-green_adjacent_section_oppos_beg+1;
		number_of_boun_points_in_oppos=
	LeftRightSectionNumberOfRightGreenBoundaryPoints[green_adjacent_section_oppos];
	}//gaso>0
	else
	{
	green_adjacent_section_oppos=-1;
	}
	}//fga>=0*/
	                       if(first_n_green_oppos>=0)
	                       {//fngo>=
		old_first_n_green_adjacent_oppos=
	ColorInt[count_fiber].old_ordered_bunch_number[first_n_green_oppos];
		old_first_n_green_adjacent_oppos_beg=
	ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[old_first_n_green_adjacent_oppos];
		old_first_n_green_adjacent_oppos_end=
	ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[old_first_n_green_adjacent_oppos];
	old_first_n_green_adjacent_oppos_length=old_first_n_green_adjacent_oppos_end-
	old_first_n_green_adjacent_oppos_beg+1;
	if(left_right)
	{
	first_n_green_adjacent_oppos_sec=
	SectionTraceRight[count_fiber*MAX_COL_INT+old_first_n_green_adjacent_oppos];
	}
	else
	{
	first_n_green_adjacent_oppos_sec=
	SectionTraceLeft[count_fiber*MAX_COL_INT+old_first_n_green_adjacent_oppos];
	}
	first_n_green_adjacent_oppos_sec=first_n_green_adjacent_oppos_sec%1000;
	if(first_n_green_adjacent_oppos_sec>0)
	{//gaos>0
		first_n_green_adjacent_oppos_sec-=1;
	    first_n_green_adjacent_oppos_sec_beg=
	ColorSection->section_fibre_first[first_n_green_adjacent_oppos_sec];
		first_n_green_adjacent_oppos_sec_end=
	ColorSection->section_fibre_last[first_n_green_adjacent_oppos_sec];
		first_n_green_adjacent_oppos_sec_length=
	first_n_green_adjacent_oppos_sec_end-first_n_green_adjacent_oppos_sec_beg+1;
		first_n_green_adjacent_oppos_sec_weight=
	ColorSection->section_weight[first_n_green_adjacent_oppos_sec];
	    first_n_green_adjacent_oppos_sec_weight_normalized=
	first_n_green_adjacent_oppos_sec_weight/first_n_green_adjacent_oppos_sec_length;
	}//gaos>0
	else
	{
	first_n_green_adjacent_oppos_sec=-1;
	}
	                       }//fngo>=

	if((first_green_adjacent_oppos==oppos_adjacent)&&(first_green_adjacent_oppos>=0))
	{//fgo=oa
	            if(left_right)
				{//lr
	oppos_green_length=oppos_green_end-beg_int+1;
		if((op_adj_end>=(StripLength-StripWidth))||(oppos_green_end>=(StripLength-StripWidth)))
		{
			entrance_to_oppos_border++;
			entrance_to_oppos_border_now=1;
		//}

			if(first_green_adjacent<0)
			{
	if(sc_gr_comp_red==2)
	{
		/*if(((section_weight>=DimX/2)&&((w_mng_section_weight>=2*DimX)||
	(w_mng_section_weight_oppos>=2*DimX)))
			||((section_weight>=2*DimX)&&((sect_incline<0)||(sect_incl_curve<0)))||
			(section_weight>=4*DimX))*/
	if(((section_weight>=DimX/2)&&((w_mng_section_weight>=2*DimX)||
	(w_mng_section_weight_oppos>=2*DimX)))
			||(((section_weight>=2*DimX)&&((sect_incline<0)||(sect_incl_curve<0)))&&
			((w_mng_section_weight>=DimX)||(w_mng_section_weight_oppos>=DimX))&&
			(non_green_length>=DimX/4)))
		{//last_cor23.12.16
			if((count_fiber<NumStrips/4)&&(!RightGreenBoundaryBunch[count_fiber]))
			{
			condition_number=4;
		    success_right=1;
			new_boundary_section=section_number;
			goto L;
			}
		}
	}
			}
		}
				}//lr
				else
				{//elr
					oppos_green_length=end_int-oppos_green_beg+1;
					if((op_adj_beg<=StripWidth)||(oppos_green_beg<=StripWidth))
		             {//opab
						 entrance_to_oppos_border++;
						 entrance_to_oppos_border_now=1;
					 //}

	if(first_green_adjacent<0)
		{//fga
	if(sc_gr_comp_red==2)
	{
		/*if(((section_weight>=DimX/2)&&((w_mng_section_weight>=2*DimX)||
	(w_mng_section_weight_oppos>=2*DimX)))
			||((section_weight>=2*DimX)&&((sect_incline>0)||(sect_incl_curve>0)))||
			(section_weight>=4*DimX))*/
	if(((section_weight>=DimX/2)&&((w_mng_section_weight>=2*DimX)||
	(w_mng_section_weight_oppos>=2*DimX)))
			||(((section_weight>=2*DimX)&&((sect_incline<0)||(sect_incl_curve<0)))&&
			((w_mng_section_weight>=DimX)||(w_mng_section_weight_oppos>=DimX))&&
			(non_green_length>=DimX/4)))
		{
	if(count_fiber<NumStrips/4)
	{
			if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=5;
	new_boundary_section=section_number;
	success_left=1;
			goto L;
			}
			else
			{
				goto L;
			}
	}
		}
	}
		}//fga
					}//opab
				}//elr
	}//fgo=oa
	else
	{//efgo=oa
	         if(first_n_green_oppos==oppos_adjacent)
			 {//fng=oa
	lenght_dir_oppos_n_green_ratio=(16*non_green_length_oppos)/
	(non_green_length_oppos+w_mng_length);
	length_dir_oppos_ng_g_ratio=(16*non_green_length_oppos)/
	(std::max(int_length,old_first_green_adjacent_oppos_length)+non_green_length_oppos);
				 if(left_right)
		         {//lr
	 if(first_green_adjacent_oppos==(first_n_green_oppos+1))
	 {//fga0=fngo+1

		      if(first_n_green_adjacent_oppos_sec<0)
			  {//fngs<0
	if((old_first_n_green_adjacent_oppos_length<=StripWidth)||
	   ((lenght_dir_oppos_n_green_ratio<=2)&&(length_dir_oppos_ng_g_ratio<=2)))
	{//of<=4
	if((oppos_green_end>=StripLength-StripWidth)||
	   (first_green_adjacent_oppos==number_of_covering_bunches-1))
		             {//opab
						 entrance_to_oppos_border++;
						 entrance_to_oppos_border_now=1;
	 if((first_green_adjacent<0))
		{//fga<0
	if((sc_gr_comp_red==2))
	{
		if(((section_weight>=DimX/2)&&(w_mng_section_weight>=DimX))||((section_weight>=4*DimX)&&
			((sect_incline<0)||(sect_incl_curve<0))))
		{
			if(!RightGreenBoundaryBunch[count_fiber])
			{
			condition_number=6;
		    success_right=1;
			new_boundary_section=section_number;
			goto L;
			}
			else
			{
				goto L;
			}
		}
	}
		}//fga<0
	                 }//opab
	                 /*else
	                 {//epab

	                 }//eopab*/
	}//of<=4

			  }//fngs<0
	 }//fga0=fngo+1
				 }//lr
				 else
				 {//elr
	if((first_green_adjacent_oppos==(first_n_green_oppos-1))&&(first_green_adjacent_oppos>=0))
	 {//fga0=fngo-1
		      if(first_n_green_adjacent_oppos_sec<0)
			  {//fngs<0
	if((old_first_n_green_adjacent_oppos_length<=StripWidth)||
	   ((lenght_dir_oppos_n_green_ratio<=2)&&(length_dir_oppos_ng_g_ratio<=2)))
	{//of<=4
	if((oppos_green_beg<=StripWidth)||
	   (!first_green_adjacent_oppos))
		             {//opab
						 entrance_to_oppos_border++;
						 entrance_to_oppos_border_now=1;
	 if((first_green_adjacent<0))
		{//fga<0
	if(sc_gr_comp_red==2)
	{
		if(((section_weight>=DimX/2)&&(w_mng_section_weight>=DimX))||((section_weight>=4*DimX)&&
			((sect_incline>0)||(sect_incl_curve>0))))
		{
			if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=7;
	new_boundary_section=section_number;
	success_left=1;
			goto L;
			}
			else
			{
				goto L;
			}
		}
	}
		}//fga<0
	                 }//opab
	                 else
	                 {//epab

	                 }//eopab
	}//of<=4

			  }//fngs<0
	 }//fga0=fngo+1
				 }//elr
				 }//fng=oa
	}//efgo=0a

				}//oasi
	if(count_fiber>0)
	{//cf>0last_cor07.12.16
	if(left_right)
	{//lr0
	if(boundary_bunch>=0)
	{//!0
		if(max_green_adj_section>=0)
		{//mgas>=
			if(green_oppos_boundary_sec>=0)
			{//grobs
				//last_cor28.03.17
				dist_gobs_mgas=green_oppos_boundary_bunch_end-w_mg_end;
				dist_gobs_mgas_abs=abs(dist_gobs_mgas);
				if(max_green_adj_section==green_oppos_boundary_sec)
				{//last_cor06.12.17
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mg_end,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
				}//last_cor06.12.17
				if(max_green_adj_section_bunch<max_n_green_adj_section_bunch)
					{//last_cor16.08.17mgasb<mngasb
		if(previous_mnga>=0)
		{//last_cor16.08.17pmnga>=0
			if(green_oppos_boundary_bunch<previous_mnga_bunch)
			{
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mg_end,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
			}
			else
			{
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mg_end,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
			}
		}//last_cor16.08.17pmnga>=0
		else
		{//epmnga>=0
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mg_end,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
		}//epmnga>=0
				}//last_cor16.08.17mgasb<mngasb
				else
				{//last_cor15.08.17emgasb<mngasb
					if(max_n_green_adj_section>=0)
					{//last_cor17.08.17mnga>=0
			if(previous_mnga>=0)
			{//pmnga>=0
					if(green_oppos_boundary_bunch<previous_mnga_bunch)
					{//last_cor16.08.17
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mng_beg,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
					}
					else
					{
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mng_beg,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
					}//last_cor16.08.17
			}//pmnga>=0
			else
			{
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mng_beg,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
			}//last_cor16.08.17
					}//last_cor17.08.17mnga>=0
				}//last_cor15.08.17emgasb<mngasb
	if(previous_mnga>=0)
	{//pmnga>=0cor
				if((previous_mnga_bunch>green_oppos_boundary_bunch)&&
					(max_green_adj_section_bunch<max_n_green_adj_section_bunch))
				{//last_cor05.04.17pmnga<
				if((first_green_adjacent==bunch_adjacent_ordered)&&(first_green_adjacent>=0))
				{//fga=bao
	if(first_green_adjacent==last_gr_in_chain)
			{
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mg_end,old_first_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
			}
			else
			{
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mg_end,old_first_beg_lg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
			}
				}//fga=bao
				else
				{//efga=bao
	if((max_n_green_adj_section>=0)&&(first_green_adjacent<max_n_green_adj_section_bunch)&&(first_green_adjacent>=0))
	{//last_cor21.04.17onlyublast_cor12.05.17
		if(first_green_adjacent>max_green_adj_section_bunch)
		{//onlyublast_cor11.05.17
Intersection_green=
ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
old_first_end,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
		}
		else
		{
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mg_end,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
		}//onlyublast_cor11.05.17
	}//last_cor21.04.17
	else
	{//emnga>=0&fga
		if(max_n_green_adj_section<0)
		{//mnga<0
			if(w_mng_section_oppos<0)
			{//w_mngso<0
if((maximum_non_green_adjacent>=0)&&(maximum_non_green_adjacent>max_green_adj_section_bunch)
		&&(first_green_adjacent<maximum_non_green_adjacent))
{
	Intersection_green=
			ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
			old_first_end,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
}
		}//w_mngso<0
		else
		{//ew_mngso<0
	if((w_mng_section_oppos_bunch>max_green_adj_section_bunch)&&(first_green_adjacent<w_mng_section_oppos_bunch)&&
			(first_green_adjacent<maximum_non_green_adjacent))
	{
		Intersection_green=
		ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
		old_first_end,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
	}
		}//ew_mngso<0
		}//mnga<0
	}//onlyublast_cor12.05.17//emnga>=0&fga
	                if(max_green_adj_section==green_oppos_boundary_sec)
					{//last_cor13.03.17
		if((last_gr_in_chain>max_green_adj_section_bunch)&&(dist_fga_wmg>=7*StripWidth))
		{
			if(first_green_adjacent==last_gr_in_chain)
			{//last_cor29.05.17onlyub
				if(first_green_adjacent>max_n_green_adj_section_bunch)
				{
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mg_end,old_first_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
				}
			}//last_cor29.05.17onlyub
			else
			{//last_cor29.05.17onlyub
				if(last_gr_in_chain>max_n_green_adj_section_bunch)
				{
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mg_end,old_first_beg_lg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
				}
			}
		}
					}
				}//efga=bao
				}//last_cor05.04.17pmnga<
	}////pmnga>=0cor
	else
	{//last_cor21.04.17
		if(first_green_adjacent!=max_green_adj_section_bunch)
		{//onlyub18.05.17
		if(first_green_adjacent<max_n_green_adj_section_bunch)
		{//last_cor21.04.17
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mg_end,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
		}//last_cor21.04.17first_green_adjacent
		else
		{
	if((first_green_adjacent==last_gr_in_chain)&&(max_n_green_adj_section_bunch>=0))//last_cor05.05.17
			{
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mg_end,old_first_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
			}
			else
			{
				if(max_n_green_adj_section_bunch>=0)
				{
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mg_end,old_first_beg_lg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
				}
			}
		}
	}//last_cor21.04.17
	}//onlyub18.05.17
	if((Intersection_green>1)&&(Intersection_green<3))
	{//onlyub17.05.17
		if((ratio_length_green<=8)&&(dist_gobs_mgas_abs>=8*StripWidth))
		{//onlyub19.05.17last_cor26.05.17
if((max_green_adj_section!=green_oppos_boundary_sec)&&(previous_mnga!=max_n_green_adj_section))
			{
			   if(((w_mg_beg-green_oppos_boundary_bunch_end)>3*StripWidth)&&(ratio_length_green<=7)&&
					   (dist_gobs_mgas_abs>DimX/6))
			   {
				   if(count_fiber>NumStrips/4)
				   {
					   break;
				   }
				   else
				   {
			goto L;
				   }
			   }
			}
	if((previous_mnga>=0)&&(max_n_green_adj_section>=0))//error_onlyub18.05.17
	{
		if((prev_mnga_bunch_hue_zone!=w_mng_hue_zone)&&(w_mng_hue_zone==3)&&(w_mng_sat>=3)&&
				(w_mng_gray_zone>=2))
		{
			if(count_fiber>=NumStrips/4)
			{
				goto L;
			}
		}
	}
		}

	}//onlyub
				if((Intersection_green==3)&&(ratio_length_green>=StripWidth))
					{//last_cor31.03.17
						goto L;
					}//last_cor31.03.17
				if(max_green_adj_section!=green_oppos_boundary_sec)
				{//last_cor28.03.17
	if((previous_mnga_bunch>green_oppos_boundary_bunch)&&(previous_mnga!=max_n_green_adj_section)&&
	   (dist_gobs_mgas_abs>=15*StripWidth)&&(((Intersection_green>=0)&&(ratio_length_green<6))||
	   (Intersection_green<0)||(Intersection_green==3)))//last_cor29.05.17onlyub
					{
						goto L;
					}
				}//last_cor28.03.17
		    }//grobs
			else//last_cor14.04.17
			{//egrobs
	if(previous_mnga>=0)
	{//prevmnga>=0
		if(max_green_adj_section_bunch<max_n_green_adj_section_bunch)
					{//last_cor17.08.17
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mg_end,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
                   }//last_cor17.08.17
					else
					{//last_cor17.08.17
						if(max_n_green_adj_section>=0)
						{//last_cor17.08.17
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mng_beg,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
						}//last_cor17.08.17
					}//last_cor17.08.17
	if((first_green_adjacent==bunch_adjacent_ordered)&&(first_green_adjacent>=0))
				{//fga=bao
	if(first_green_adjacent==last_gr_in_chain)
			{//fga=lgch
if(max_green_adj_section_bunch<max_n_green_adj_section_bunch)
{//last_cor18.08.17
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mg_end,old_first_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
}//last_cor18.08.17
else
{
	if(max_n_green_adj_section>=0)
	{//last_cor18.08.17
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mng_beg,old_first_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
	}//last_cor18.08.17
}//last_cor18.08.17
	        }//fga=lgch
			else
			{
if(max_green_adj_section_bunch<max_n_green_adj_section_bunch)
{//last_cor18.08.17
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mg_end,old_first_beg_lg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
}//last_cor18.08.17
else
{
	if(max_n_green_adj_section>=0)
	{//last_cor18.08.17
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mng_beg,old_first_beg_lg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
	}
}//last_cor18.08.17
			}
				}//fga=bao
				else
				{//efga=bao
	if(max_green_adj_section==green_oppos_boundary_sec)
					{//last_cor13.03.17
		if((last_gr_in_chain>max_green_adj_section_bunch)&&(dist_fga_wmg>=7*StripWidth))
		{
			if(first_green_adjacent==last_gr_in_chain)
			{
if(max_green_adj_section_bunch<max_n_green_adj_section_bunch)
					{//last_cor18.08.17
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mg_end,old_first_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
	                }//last_cor18.08.17
else
{
	if(max_n_green_adj_section>=0)
	{//last_cor18.08.17
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mng_beg,old_first_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
	}
}//last_cor18.08.17
			}
			else
			{
if(max_green_adj_section_bunch<max_n_green_adj_section_bunch)
					{//last_cor18.08.17
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mg_end,old_first_beg_lg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
	                }//last_cor18.08.17
else
{
if(max_n_green_adj_section>=0)
	{//last_cor18.08.17
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mng_beg,old_first_beg_lg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
    }//last_cor18.08.17
}
			}
		}
					}
				}//efga=bao
	if(Intersection_green==3)
	{//last_cor14.04.17
		if(w_mg_end>left_end_bb)
		{
		goto L;
		}
	}//last_cor14.04.17
	}//prevmnga>=0
			}//egrobslast_cor14.04.17
		}//mgas>=
		else//last_cor14.04.17
		{//emgas
			if(green_oppos_boundary_sec>=0)
			{//grobs
if(max_n_green_adj_section>=0)
{//last_cor18.08.17
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mng_beg,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
}//last_cor18.08.17
			}//grobs
			else
            {
                    if(previous_mnga>=0)
					{//prevmnga>=0
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
	w_mng_beg,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
					}//prevmnga>=0
            }//last_cor18.08.17
					if(previous_mnga>=0)
					{//prevmnga>=0
	if(green_oppos_boundary_sec>=0)
			{//grobs
if(max_n_green_adj_section>=0)
{//last_cor18.08.17
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mng_beg,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
}//last_cor18.08.17

	if((first_green_adjacent==bunch_adjacent_ordered)&&(first_green_adjacent>=0))
	{//fga=bao
		if(first_green_adjacent==last_gr_in_chain)
		{
			if(max_n_green_adj_section>=0)
	        {//last_cor18.08.17
Intersection_green=
ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mng_beg,old_first_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
Intersection_green1=
ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
w_mng_beg,old_first_beg,&indic_length_green1,&ratio_length_green1,&ratio_length1_green1);
			}//last_cor18.08.17
		}
				else
				{
            if(max_n_green_adj_section>=0)
	        {//last_cor18.08.17
Intersection_green=
ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb,
	w_mng_beg,old_first_beg_lg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
Intersection_green1=
ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
w_mng_beg,old_first_beg_lg,&indic_length_green1,&ratio_length_green1,&ratio_length1_green1);
            }//last_cor18.08.17
				}
	}//fga=bao
					else
					{//efga=bao
						if(max_n_green_adj_section>=0)
	                    {//last_cor18.08.17
		Intersection_green1=
		ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
		w_mng_beg,beg_int,&indic_length_green1,&ratio_length_green1,&ratio_length1_green1);
						}//last_cor18.08.17

					}//efga=bao
	}//grobs
	else
	{//egrobs
		if(max_n_green_adj_section>=0)
		{//mnga>=0
	if((first_green_adjacent==bunch_adjacent_ordered)&&(first_green_adjacent>=0))
	{//fga=bao
		if(first_green_adjacent==last_gr_in_chain)
		{
		Intersection_green1=
		ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
		w_mng_beg,old_first_beg,&indic_length_green1,&ratio_length_green1,&ratio_length1_green1);
		}
				else
				{
		Intersection_green1=
		ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
		w_mng_beg,old_first_beg_lg,&indic_length_green1,&ratio_length_green1,&ratio_length1_green1);
				}
	}//fga=bao
					else
					{//efga=bao
		Intersection_green1=
		ColorInt[count_fiber].Intersection_measure_ratios(prev_mnga_bunch_beg,left_end_bb,
		w_mng_beg,beg_int,&indic_length_green1,&ratio_length_green1,&ratio_length1_green1);

					}//efga=bao
		}//mnga>=0
	}//egrobs
					}//prevmnga>=0
		}////emgaslast_cor14.04.17
	if((max_green_adj_section>=0)&&(max_n_green_adj_section>=0)&&
			((Intersection_green>=2)||
			((green_oppos_boundary_sec>=0)&&((Intersection_green<0)||(Intersection_green==3)))))
	{//last_cor29.03.17last_cor16.08.17
		if(max_green_adj_section_bunch>max_n_green_adj_section_bunch)
		{
			if((w_mng_section_oppos_bunch<max_green_adj_section_bunch)&&(green_oppos_boundary_sec>=0))
			{//last_cor05.04.1715.08.17
			goto L;
			}
			/*else
			{//e
				if(w_mng_section_oppos>=0)
				{
	if(max_green_adj_section==green_oppos_boundary_sec)
		{//mgas=gobs
	if((!Intersection_green)&&(ratio_length_green>=12)&&(ratio_length1_green>=15))
	{
	if(((first_green_adjacent<w_mng_section_oppos_bunch)||(!green_lg_before))&&
	   (w_mng_section_oppos_bunch>max_green_adj_section_bunch))
	{
		if(((dist_gobs_mgas<0)||(dist_gobs_mgas_abs<=2*StripWidth))
			&&((differ_boundary_sign<0)||(differ_boundary_abs<=2*StripWidth)))
		{
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=206;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
		}
	}
	}
		}//mga=gobs
				}
			}//e*/
		}
	}//last_cor29.03.17
	if(first_green_adjacent>=0)
	{
		if(first_green_adjacent==last_gr_in_chain)
		{
		differ_bb_fga=left_end_bb-old_first_beg;
		}
		else//last_cor04.05.17
		{
			//if(first_gr<0)
				//{last_cor13.04.17Question!
		    differ_bb_fga=left_end_bb-old_first_beg_lg;
				//}
				/*else
				{
		      differ_bb_fga=left_end_bb-last_gr_beg;
				}*/
		}

		if(first_green_adjacent==bunch_adjacent_ordered)
		                       	{//last_cor07.12.16!may be incorrect
		if((differ_bb_fga>0)&&(abs(differ_bb_fga)>=10*StripWidth)&&(count_fiber>(2*NumStrips)/3)
			&&(previous_mnga!=max_n_green_adj_section)&&
	   (green_oppos_boundary_sec!=max_green_adj_section))
		{
			break;
		}
		                        }//last_cor07.12.16
	}
	if((max_green_adj_section>=0)&&(max_green_adj_section==green_oppos_boundary_sec))
		if((max_n_green_adj_section<0)&&(w_mng_section_oppos<0))
		{//last_cor03.05.17
		if(((3*green_lg_before)>(2*dist_to_wmg))&&(green_lg_before>=3*StripWidth))
		{
			if(count_fiber>=NumStrips/2)
			{
				goto L;
			}
		}
		}//last_cor03.05.17
	if((differ_boundary_sign<0)&&(differ_boundary_abs>=15*StripWidth)&&(count_fiber>NumStrips/3)&&
	   ((ratio_length_green<=8)&&(Intersection_green>=0)||(Intersection_green<0)||
	 (Intersection_green==3))&&
	   (previous_mnga!=max_n_green_adj_section)&&(green_oppos_boundary_sec!=max_green_adj_section))
	{//last_cor29.03.17
		break;
	}
	//onlyuberror!last_cor26.0517
	if((section_bunch_shift_abs>=0)&&(section_bunch_shift>=0)
&&(section_bunch_shift_abs>=8*StripWidth)
	 &&(count_fiber>NumStrips/3)&&
	 (((ratio_length_green<=4)&&(Intersection_green>2))||(Intersection_green<0)||
	 (Intersection_green==3))&&
	 (differ_boundary_sign<0)&&(differ_boundary_abs>=15*StripWidth)&&(beg_int<DimX/2))
	{//last_cor30.03.17
		if(green_oppos_boundary_sec!=max_green_adj_section)
		{//last_cor02.05.17
		break;
		}
	}
	//onlyuberror!last_cor26.0517
	if((section_bunch_shift>=0)&&(section_bunch_shift_abs>=0)&&(section_bunch_shift_abs>=12*StripWidth)
	 &&(count_fiber>NumStrips/6)&&
	 (((ratio_length_green<=7)&&(Intersection_green>2))||(Intersection_green<0)||
	 (Intersection_green==3))&&
	 (differ_boundary_sign<0)&&(differ_boundary_abs>=18*StripWidth)&&(beg_int<DimX/2))
	{//last_cor30.03.1702.05.17
		break;
	}//last_cor30.03.17
	if((differ_boundary_sign<0)&&(differ_boundary_abs>=10*StripWidth)&&
	   (count_fiber>(2*NumStrips)/3)&&(previous_mnga!=max_n_green_adj_section)&&
	   (green_oppos_boundary_sec!=max_green_adj_section))
		                       	{//last_cor07.12.16!may be incorrect
		                       		break;
		                       	}//last_cor07.12.16
	/*if((green_adjacent_section>=0)&&(first_green_adjacent>max_n_green_adj_section_bunch)
	   &&(green_adjacent_section>section_number)&&(green_adjacent_section_weight>DimX/4)&&
	   (first_green_adjacent>max_green_adj_section_bunch))
	{
		goto L;
	}*/
	//last_cor21.03.17
	if((max_n_green_adj_section<0)&&(max_green_adj_section>=0))
	{//last_cor10.03.17
		if((last_gr_in_chain==max_green_adj_section_bunch)||
			(first_green_adjacent>max_green_adj_section_bunch))
		{
		    if(previous_mnga>=0)
		    {
				if(((10*green_lg_before)>(8*dist_to_wmg))&&(green_lg_before>=3*StripWidth))
				{
	            goto L;
				}
		    }
		}
	}
	   if((max_n_green_adj_section>=0)&&(max_green_adj_section>=0)&&
		   (max_green_adj_section==green_oppos_boundary_sec))
	   {//last_cor10.03.1729.03.17
	if((first_green_adjacent<max_n_green_adj_section_bunch)&&(dist_wmng_to_mga>=0))
	{//last_cor29.03.17
		if((last_gr_in_chain==max_green_adj_section_bunch)||
			(first_green_adjacent>max_green_adj_section_bunch))
		{
			if((previous_mnga>=0)&&(old_first_end>prev_mnga_bunch_end))
			{//last_cor21.04.17
				if((10*green_lg_before)>(8*dist_wmng_to_mga))
				{
	            goto L;
				}
			}
			/*if((w_mng_length<dist_wmng_to_mga/4)&&(w_mng_length<prev_mnga_bunch_length/4))
			{
	if(abs(differ_bb_fga)>=10*StripWidth)
	{
		goto L;
	}
			}*/
		}
	}
	   }//last_cor10.03.17
	if(max_green_adj_section>=0)
	{//last_cor17.02.17
		if(max_green_adj_section==green_oppos_boundary_sec)
		{//mgas==grobs
		if(max_n_green_adj_section>=0)
							{//mngas>=
								if(max_n_green_adj_section==previous_mnga)
								{//mngas=pmnga
		if(first_green_adjacent<max_n_green_adj_section_bunch)
		{//fga<mnga
		    if((differ_boundary_abs>=0)&&(differ_boundary_abs<=6*StripWidth))
		    {//dba>=
		    	if((Intersection_green>=0)&&(Intersection_green<=1)&&
		    			(ratio_length_green>=4)&&(ratio_length1_green>=14))
		    	{//ig01
		    	if(!RightGreenBoundaryBunch[count_fiber])
		    				{
		    		condition_number=250;
		    		success_right=1;
		    		new_boundary_section=section_number;
		    		goto L;
		    				}
		    		        else
		    				{
		    					goto L;
		    				}
		    	}//ig01
		    }//dba
		}//fga<mnga
								}//mngas==pmnga
							}//mngas>=
		}//mgas==grobs
			if((Intersection_green>=0)&&(Intersection_green<=1)&&(differ_boundary_abs>=0))
			{//ig01

				if((!Intersection_green)&&(ratio_length_green>=12)&&(ratio_length1_green>=12))
				{//onlyub19.05.17
					if(max_n_green_adj_section>=0)
					{//mngas>=
						if(max_n_green_adj_section==previous_mnga)
						{//mngas=pmnga
if((differ_boundary_sign<0)&&(differ_boundary_abs<=5*StripWidth))
{//last_cor09.11.17
if((section_weight>=DimX)&&(sect_incline<0))
{
if(differ_bb_fga>2*StripWidth)
{
if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=254;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
}
			}
}//last_cor09.11.17
if(first_green_adjacent<max_n_green_adj_section_bunch)
{//fga<mnga
    if((differ_boundary_abs>=0)&&(differ_boundary_abs<=6*StripWidth))
    {//dba>=
    	if(!RightGreenBoundaryBunch[count_fiber])
    				{
    		condition_number=242;
    		success_right=1;
    		new_boundary_section=section_number;
    		goto L;
    				}
    		        else
    				{
    					goto L;
    				}
    }//dba
}//fga<mnga
else
{//onlyublast_cor26.05.17
if(((first_green_adjacent-1)==max_n_green_adj_section_bunch)||
		(first_green_adjacent==last_gr_in_chain))
{//fgacond
	if(first_gr<0)
	{//fgr
	if(((differ_bb_fga>0)&&(abs(differ_bb_fga)<=6*StripWidth))||(abs(differ_bb_fga)<=2*StripWidth))
		{//distcond
		if(!RightGreenBoundaryBunch[count_fiber])
					{
			condition_number=246;
			success_right=2;
			new_boundary_section=section_number;
			goto L;
					}
			        else
					{
						goto L;
					}
		}//distcond
	}//fgr
}//fgacond
else
{
    if(first_green_adjacent!=last_gr_in_chain)
     {
    	if((first_gr<0)||(last_gr==last_gr_in_chain))
    	{
    		if(((differ_bb_fga>0)&&(abs(differ_bb_fga)<=6*StripWidth))||(abs(differ_bb_fga)<=2*StripWidth))
    				{//distcond
    				if(!RightGreenBoundaryBunch[count_fiber])
    							{
    					condition_number=248;
    					success_right=3;
    					new_boundary_section=section_number;
    					goto L;
    							}
    					        else
    							{
    								goto L;
    							}
    				}//distcond
    	}

     }
}
}//onlyublast_cor26.05.17
						}//mngas=pmnga
					}//mngas>=0
				}//onlyub19.05.17

	if(((differ_boundary_sign<0)&&((differ_boundary_abs<=5*StripWidth)||
			((differ_boundary_abs<=7*StripWidth)&&(ratio_length_green>=11)&&(ratio_length1_green>=14))))||
	   (differ_boundary_abs<=2*StripWidth))
	{
		if((differ_boundary_abs<StripWidth)&&(ratio_length_green>=11)&&(ratio_length1_green>=11))
		{//last_cor09.11.17
			if((section_weight>=DimX)&&(sect_incline<0))
			{
if(differ_bb_fga>2*StripWidth)
{
if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=252;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
}
			}
		}//last_cor09.11.17
	 if(((ratio_length_green>=6)&&(ratio_length1_green>=10))||
	((max_green_adj_section==green_oppos_boundary_sec)&&
	 (ratio_length_green>=7)&&(ratio_length1_green>=7))||((green_oppos_boundary_sec<0)&&
	 (std::max(ratio_length_green,ratio_length1_green)>=8)&&
	 (std::min(ratio_length_green,ratio_length1_green)>=4)))
	 {//last_cor21.04.17
	if(max_n_green_adj_section>=0)
	{//last_cor24.03.17onlyub16.05.17
	//if(((first_green_adjacent>=0)&&(first_green_adjacent<max_n_green_adj_section_bunch))||
	   //(first_green_adjacent<0))
	if((first_green_adjacent>=0)&&(first_green_adjacent<max_n_green_adj_section_bunch))//last_cor19.05.17
	{//last_cor18.04.17
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=182;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
	}//last_cor18.04.17
	else
		{//onlyublast_cor11.04.17
			if(first_green_adjacent==bunch_adjacent_ordered)
			{
				if(last_gr_in_chain==first_green_adjacent)
				{
	if(((differ_bb_fga>=0)&&(abs(differ_bb_fga)<=6*StripWidth))||(abs(differ_bb_fga)<=2*StripWidth))
	{
		if(!RightGreenBoundaryBunch[count_fiber])
					{
			condition_number=230;
			success_right=2;
			new_boundary_section=section_number;
			goto L;
					}
			        else
					{
						goto L;
					}
	}

				}
			}
		}//onlyublast_cor11.04.17
	}
	else
	{//last_cor03.04.17
		if((w_mng_section_oppos>=0)&&((w_mng_section_weight_oppos>=2*DimX)||
			((!Intersection_green)&&(ratio_length_green>=10)&&(ratio_length1_green>=14)&&
			(ratio_length1_green>ratio_length_green))))
		{//wmngo
		if(max_green_adj_section==green_oppos_boundary_sec)
		{//mgas=gobs
	if(((first_green_adjacent<w_mng_section_oppos_bunch)||(!green_lg_before))&&
	   (w_mng_section_oppos_bunch>max_green_adj_section_bunch))
	{
		if((((dist_gobs_mgas<0)&&(dist_gobs_mgas_abs<=4*StripWidth))
			||(dist_gobs_mgas_abs<=2*StripWidth))
			&&(((differ_boundary_sign<0)&&((differ_boundary_abs<=5*StripWidth)))||
			(differ_boundary_abs<=2*StripWidth)))
		{
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=204;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
		}

	}
		}//mga=gobs
		}//wmngo
		else
		{//ewmno
		if(max_green_adj_section==green_oppos_boundary_sec)
		{//mgas=gobs
			if((!Intersection_green)&&(ratio_length1_green>=15)&&(ratio_length_green>=10))
			{//intg
	if((!green_lg_before)||((first_green_adjacent<maximum_non_green_adjacent)))
	{//glgblast_cor05.05.17
		if(((dist_gobs_mgas<0)||(dist_gobs_mgas_abs<=2*StripWidth))
	&&(((differ_boundary_sign<0)&&(differ_boundary_abs<=5*StripWidth))
	   ||(differ_boundary_abs<=2*StripWidth)))
		{
	if((maximum_non_green_adjacent>=0)&&(maximum_non_green_adjacent>max_green_adj_section_bunch)&&
	   ((mng_sat>=0)&&(mng_sat<=1)||((mng_hue_zone!=3)&&(mng_sat>=0)&&(mng_sat<=2))))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=206;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
	}
		}

	}//glgb

			}//intg
		}//mgas=gobs
		}//ewmno
	}//last_cor03.04.17
	 }
	  }
	if(max_n_green_adj_section>=0)
	{//last_cor18.04.17
		if((first_green_adjacent>=0)&&(first_green_adjacent>max_n_green_adj_section_bunch))
		{
			if(max_n_green_adj_section==previous_mnga)
			{
				if(first_green_adjacent==last_gr_in_chain)
				{//fga=lgc
					if((first_gr<0)||((first_gr>=0)&&(last_gr==last_gr_in_chain)))
					{//last_cor19.04.17
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=208;
	success_right=2;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
					}
					else
					{//last_cor19.04.17
	if((first_gr>=0)&&(last_gr!=last_gr_in_chain))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=210;
	success_right=4;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
	}
					}//last_cor19.04.17
				}//fga=lgc
				else
				{//last_cor19.04.17
					if(first_gr<0)
					{//fgr<
						if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=212;
	success_right=3;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
					}//fgr<
				}//last_cor19.04.17
			}
		}
	}//last_cor18.04.17
			}//ig01
			if((Intersection_green>=0)&&(Intersection_green<=2)&&(differ_boundary_abs>=0))
			{//ig01last_cor01.03.17
				if(max_green_adj_section==green_oppos_boundary_sec)
				{
				if((w_mg_section_incline>0)&&(w_mg_incl_curve>0)&&(w_mg_section_weight>=DimX/3))
				{
	if(((differ_boundary_sign<0)&&(differ_boundary_abs<=5*StripWidth))||
	   (differ_boundary_abs<=3*StripWidth))
	{
	   if((ratio_length_green>=4)&&(ratio_length1_green>=4)&&(previous_mnga>=0)&&
		   (previous_mnga==max_n_green_adj_section))
	   {//last-cor19.04.17
	if((first_green_adjacent<bunch_adjacent_ordered)&&(dist_fga_wmg>=0)&&(dist_fga_wmg<=3*StripWidth))
		  {//last_cor13.03.17
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=184;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
		  }//last_cor20.02.17
	   }
	}
				}
			    }
	if((((differ_boundary_sign<0)&&(differ_boundary_abs<=5*StripWidth))||
	   (differ_boundary_abs<=2*StripWidth))&&(differ_boundary_abs>=0))
	  {
	if((max_green_adj_section==green_oppos_boundary_sec)&&
	   (ratio_length_green>=4)&&(ratio_length1_green>=4))
	 {
		 if((w_mg_section_incline>0)&&(w_mg_incl_curve>0)&&(w_mg_section_weight>=DimX/3))
		 {
	 if((first_green_adjacent<bunch_adjacent_ordered)&&(dist_fga_wmg>=0)&&(dist_fga_wmg<=3*StripWidth))
		  {//last_cor13.03.17
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=186;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
		  }//last_cor20.02.17
		 }
	 }
	  }
			}//ig01
	}//last_cor17.02.17
	if(w_mng_section_oppos>=0)
	{//onlyub18.05.17
		if((Intersection_green>=0)&&(Intersection_green<=1)&&(differ_boundary_abs>=0))
					{//ig01

			if(((differ_boundary_sign<0)&&((differ_boundary_abs<=5*StripWidth)||
					((differ_boundary_abs<=7*StripWidth)&&(ratio_length_green>=11)&&(ratio_length1_green>=14))))||
			   (differ_boundary_abs<=2*StripWidth))
			{
			 if((ratio_length_green>=12)&&(ratio_length1_green>=13))
					 {
				        if(first_green_adjacent<w_mng_section_oppos_bunch)
					    {
				        	if(!RightGreenBoundaryBunch[count_fiber])
				        				{
				        		condition_number=236;
				        		success_right=1;
				        		new_boundary_section=section_number;
				        		goto L;
				        				}
				        		        else
				        				{
				        					goto L;
				        				}
					    }
					 }
			}
					}//ig01
	}//onlyub18.05.17
	if((first_green_adjacent<0)&&(max_green_adj_section<0))
	{//onlyub16.05.17
		if((Intersection_green1>=0)&&(Intersection_green1<=2))
		{
			if(((differ_boundary_sign<0)&&(differ_boundary_abs<=5*StripWidth))||
				   (differ_boundary_abs<=2*StripWidth))
				  {
				if((count_fiber<=NumStrips/2)||(max_n_green_adj_section==previous_mnga))
				{
					if(!RightGreenBoundaryBunch[count_fiber])
								{
						condition_number=234;
						success_right=1;
						new_boundary_section=section_number;
						goto L;
								}
						        else
								{
									goto L;
								}
				}

				  }
		}
	}//onlyub16.05.17
	if((first_green_adjacent<0)||(first_green_adjacent<max_n_green_adj_section_bunch))
	{//last_cor23.12.16
		if((max_green_adj_section>=0)&&(differ_boundary_abs>=0))
		{//last_cor10.02.17
	 if(((differ_boundary_sign<0)&&(differ_boundary_abs<=6*StripWidth))||
	   (differ_boundary_abs<=2*StripWidth))
	  {
		  if((!Intersection_green)&&(ratio_length1_green>=12)&&(ratio_length_green>=12))
		  {
			  if((w_mng_section_weight>DimX/4)&&((w_mng_incl_curve<0)||(w_mng_section_incline<0)||
				  (w_mng_incl_curve_oppos>0)||(w_mng_section_oppos_incline>0)||
				  ((max_green_adj_section==green_oppos_boundary_sec)&&(w_mng_sat<=2))))
			  {
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=180;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
			  }
		  }
	if((w_mg_section_incline>0)&&(w_mg_incl_curve>0)&&(w_mg_section_weight>=DimX/3))
	{

		if(Intersection_green>=0)
		{
			if(Intersection_green<=1)
			{
	if(((first_green_adjacent>=0)&&(first_green_adjacent<bunch_adjacent_ordered)&&
	   (dist_fga_wmg>=0)&&(dist_fga_wmg<=3*StripWidth))||(first_green_adjacent<0))
	{//last_cor13.03.17
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=176;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
	}//last_cor13.03.17
			}
		}

	}
	 }

		}//last_cor10.02.17
		if((max_n_green_adj_section>=0)&&(previous_mnga>=0)&&
			(previous_mnga==max_n_green_adj_section))
		{//last_cor23.12.16
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=158;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}//last_cor23.12.16

		if((max_n_green_adj_section>=0)&&(bunch_adjacent_ordered==max_n_green_adj_section_bunch))
		{//last_cor23.12.16
			if((((w_mng_section_incline<0)||(w_mng_incl_curve<0))&&((w_mng_section_weight>=2*DimX)||
				((w_mng_section_weight>=DimX)&&(non_green_length>=DimX/4)))&&
				(w_mng_sat<=1)))
			{//comp_cond
	if((((differ_boundary_sign<0)&&(differ_boundary_abs<=6*StripWidth))||
	   (differ_boundary_abs<=2*StripWidth))&&(differ_boundary_abs>=0))
	{
		if(count_fiber<=NumStrips/2)
		{
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=154;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}
	}
			}//comp_cond
		}//last_cor23.12.16
	}//last_cor14.12.16
	else
	{//last_cor15.12.16
			if((max_n_green_adj_section>=0)&&(first_green_adjacent>=0)&&
				((first_green_adjacent==bunch_adjacent_ordered)||
				(first_green_adjacent>max_n_green_adj_section_bunch)))
			{//last_cor09.02.17and1502.17
				if(first_green_adjacent==last_gr_in_chain)
				{
					if((first_gr<0)||(first_green_adjacent==first_gr))//onlyub16.05.17
					{//last_cor16.02.17
					if((!Intersection_green)&&(ratio_length1_green>=12)&&(ratio_length_green>=12))
					{
	if(((differ_bb_fga>0)&&(abs(differ_bb_fga)<=5*StripWidth))||(abs(differ_bb_fga)<=2*StripWidth))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=178;
	success_right=2;
	new_boundary_section=section_number;
	goto L;
		}
	else
			{
				goto L;
			}
	}
	else
	{
		if(first_n_green==max_n_green_adj_section_bunch)
		{
			if(max_n_green_adj_section==previous_mnga)
			{
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=178;
	success_right=2;
	new_boundary_section=section_number;
	goto L;
		}
	else
			{
				goto L;
			}
			}
		}
	}
					}
					}//last_cor16.02.17
	if(((w_mg_section_incline>0)||(w_mg_incl_curve>0))&&((w_mg_section_weight>=DimX/3)||
	   (non_green_length>=DimX/4)))
	{//last_cor10.02.17
	    if(((w_mng_section_weight>=DimX)||(w_mng_section_weight_oppos>=DimX))&&
			(non_green_length>=DimX/4))
		{
	if(((differ_bb_fga>0)&&(abs(differ_bb_fga)<=6*StripWidth))||(abs(differ_bb_fga)<=2*StripWidth))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=176;
	success_right=2;
	new_boundary_section=section_number;
	goto L;
		}
	else
			{
				goto L;
			}
	}
	    }
	}//last_cor10.02.17
	if(((differ_bb_fga>0)&&(abs(differ_bb_fga)<=6*StripWidth))||(abs(differ_bb_fga)<=2*StripWidth))
	{
		if(((w_mng_section_weight>=3*DimX)||(w_mng_section_weight_oppos>=3*DimX))&&
			(non_green_length>=DimX/4))
		{
	if((w_mng_section_first_oppos<=NumStrips/3)||(w_mng_section_first<=NumStrips/3))
	{
		if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=174;
	success_right=2;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
	}
		}
	}
				}//Question!last_cor13.04.17
				/*else
				{//last_cor06.03.17
					if(first_gr>=0)
					{
						if(max_n_green_adj_section==previous_mnga)
						{
							if((last_gr_in_chain==(max_n_green_adj_section_bunch+1))||
								(abs(dist_form_lt_bunch_to_wmng)<=StripWidth))//needs correction!!!
							{
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=190;
	success_right=3;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
							}
	if(((differ_bb_fga>0)&&(abs(differ_bb_fga)<=6*StripWidth))||(abs(differ_bb_fga)<=2*StripWidth))
	{//last_cor05.04.17
	    if(((w_mng_section_weight>=3*DimX)||(w_mng_section_weight_oppos>=3*DimX))&&
			(non_green_length>=DimX/4))
		{
			if((w_mg_section_weight>DimX)&&((w_mg_section_incline>0)||(w_mg_incl_curve>0)))
			{
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=206;
	if(last_gr_in_chain==last_gr)
	{
	success_right=3;
	}
	else
	{
	success_right=4;
	}
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
			}
	    }
	}//last_cor05.04.17
						}
					}
				}//last_cor06.03.17*/
			}//last_cor09.02.17
		if((green_bunch_finding!=-1)&&(first_gr==-1))
		{
			if(previous_mnga==max_n_green_adj_section)
		 {//last_cor23.12.16
	if(!RightGreenBoundaryBunch[count_fiber])
			{
				if(first_green_adjacent==last_gr_in_chain)
				{
	condition_number=160;
	success_right=2;
	new_boundary_section=section_number;
	goto L;
				}
				else
				{
	condition_number=162;
	success_right=3;
	new_boundary_section=section_number;
	goto L;
				}
			}
	else
			{
				goto L;
			}
		}
	if((((w_mng_section_incline<0)||(w_mng_incl_curve<0))&&((w_mng_section_weight>=2*DimX)||
				((w_mng_section_weight>=DimX)&&(non_green_length>=DimX/4)))&&
				(w_mng_sat<=1)))
			{//comp_cond
	if(((differ_bb_fga>0)&&(abs(differ_bb_fga)<=6*StripWidth))||
	   (abs(differ_bb_fga)<=2*StripWidth))
	{
		if((count_fiber<=NumStrips/2)&&(sc_gr_comp_red==2))
		{
	if(!RightGreenBoundaryBunch[count_fiber])
			{
				if(first_green_adjacent==last_gr_in_chain)
				{
	condition_number=156;
	success_right=2;
	new_boundary_section=section_number;
	goto L;
				}
				else
				{
					if(first_gr<0)
					{
				condition_number=164;
	            success_right=3;
	            new_boundary_section=section_number;
	            goto L;
					}
				}
			}
	else
			{
				goto L;
			}
		}
	}
			}//comp_cond
		}
	}//last_cor15.12.16
	if((max_n_green_adj_section>=0)&&(max_green_adj_section>=0)&&(Intersection_green>=0)&&
	   (Intersection_green<=1))
	{//last_cor02.05.17
		if(max_green_adj_section==green_oppos_boundary_sec)
		{
	if((w_mg_section_incline>0)&&(w_mg_incl_curve>0)&&(w_mg_section_weight>=2*DimX)&&
			(w_mg_section_color==2))
		{//last_cor02.05.17
			if(((w_mng_section_oppos_incline>0)&&(w_mng_incl_curve_oppos>0))||
				((w_mng_section_incline<0)&&(w_mng_incl_curve<0)))
			{
				if(first_green_adjacent<max_n_green_adj_section_bunch)
				{

	if((non_green_length>=DimX/4)||((w_mng_section_weight>DimX/2)&&
	   (w_mng_section_weight_normalized>=DimX/8)))
	{
		if(count_fiber<=NumStrips/2)
		{
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=232;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
		}
	}
				}
			}
		}//last_cor02.05.17
		}
	}//last_cor02.05.17
	}//!0
	else
	{
	boundary_bunch=-1;
	if(!UpperGreenBoundaryRight)
			{//onlyub18.05.17
		/*if((max_n_green_adj_section>=0)&&(w_mng_section_oppos>=0))
			{
				if((max_green_adj_section>=0)&&(w_mg_section_oppos>=0))
				{//last_cor25.04.17
					if((sect_incline<0)&&(sect_incl_curve<0))
					{
						if((w_mg_section_incline>0)&&(w_mg_incl_curve))
						{
							if((w_mg_section_weight>DimX)&&(w_mg_section_first<=NumStrips/4))
							{
								if((w_mng_section_weight>DimX/3)&&(non_green_length>=DimX/4))
								{
									if(first_green_adjacent<max_n_green_adj_section_bunch)
									{
				if(!RightGreenBoundaryBunch[count_fiber])
			{
					condition_number=218;
				    success_right=1;
					new_boundary_section=section_number;
					goto L;
			}
					else
					{
						goto L;
					}
									}
								}
							}
						}
					}
				}//last_cor25.04.17*/
if(max_n_green_adj_section>=0)
{//onlyublast_cor19.05.17
	if(max_green_adj_section>=0)
	{//mgas>=
		if((w_mg_section_incline>0)||(w_mg_incl_curve>0))
		{//incl
			if(count_fiber<=NumStrips/3)
			{//ns
if(((mng_length>DimX/2)||(non_green_length>(2*DimX)/3))&&(sc_gr_comp_red==2)&&
   (sect_incline<0)&&(sect_incl_curve<0)&&(section_weight>DimX)&&(w_mg_section_weight>DimX/3)&&
   (w_mg_section_incline>0))
				{//last_cor15.02.18
					if((first_green_adjacent<maximum_non_green_adjacent)||(!green_lg_before))
					{
if(!RightGreenBoundaryBunch[count_fiber])
								    {
										condition_number=258;
									    success_right=1;
										new_boundary_section=section_number;
										goto L;
								    }
										else
										{
											goto L;
										}
					}
				}//last_cor15.02.18
				if((sc_gr_comp_red==2)&&(((sect_incline<0)||(sect_incl_curve<0))
||(w_mng_section_incline<0)||(w_mng_incl_curve<0)||((w_mg_section_weight>2*DimX)&&(w_mng_section_weight>DimX))))
				{//sc2
					if(w_mg_section_weight>DimX)
					{//weightmg
					  if((w_mng_section_weight>DimX/3)&&((non_green_length>=DimX/4)||(dist_to_wmg>=DimX/4)))
						{//weightwmng
							if((first_green_adjacent<max_n_green_adj_section_bunch)||(!green_lg_before))
							{//fga<
									if(!RightGreenBoundaryBunch[count_fiber])
								    {
										condition_number=240;
									    success_right=1;
										new_boundary_section=section_number;
										goto L;
								    }
										else
										{
											goto L;
										}
							}//fga<
						}//weightwmng
					 }//weightmg
				}//sc2
			}//ns
			else
			{
if(count_fiber<=NumStrips/2)
{//ns1last_cor28.11.17
	if((max_n_green_adj_section>=0)&&(w_mng_section_weight>=DimX))
	{
if((w_mng_section_incline<0)||(w_mng_incl_curve<0))
{
	if((sc_gr_comp_red==2)&&((sect_incline<0)||(sect_incl_curve<0)))
	{
		if(first_green_adjacent<max_n_green_adj_section_bunch)
		{
			if((w_mg_section_first<NumStrips/4)&&(w_mg_section_weight>=3*DimX))
			{
if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=256;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
			}
		}
	}
}
	}
}//ns1last_cor28.11.17
			}
	    }//incl
	}//mgas>=
}//onlyublast_cor19.05.17
		}//onlyub18.05.17
	if((UpperGreenBoundaryRight>0)&&(UpperGreenBoundaryRight<NumStrips))
		{
		prev_boun_bunch=RightGreenBoundaryBunch[UpperGreenBoundaryRight]-1;
		left_end_bb_prev=
		ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->BegInterv[prev_boun_bunch];
		right_end_bb_prev=
		ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->EndInterv[prev_boun_bunch];
		differ_boundary_sign_prev=beg_int-left_end_bb_prev;
		differ_boundary_abs_prev=abs(differ_boundary_sign_prev);
		prev_sec_ng_max=RightAdjacentNonGreenSectionMax[UpperGreenBoundaryRight]-1;//last_cor21.12.16
		if(prev_sec_ng_max>=0)
		{//pmnga>=0
		prev_sec_ng_max_end=
		ColorSection->section_fibre_last[prev_sec_ng_max];
		prev_sec_ng_max_beg=
		ColorSection->section_fibre_first[prev_sec_ng_max];
		previous_mnga_bunch=
		 ColorDescrSect[prev_sec_ng_max].location_of_section[UpperGreenBoundaryRight];
		//if(w_mng_section_weight>=DimX/6)
		//{//last_cor13.03.17

		//}//last_cor13.03.17
		      if(previous_mnga_bunch>0)
			  {//pb>
				  previous_mnga_bunch--;
		previous_mnga_bunch=ColorInt[UpperGreenBoundaryRight].old_bunch_number[previous_mnga_bunch];
		prev_mnga_bunch_beg=
		ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->BegInterv[previous_mnga_bunch];
		prev_mnga_bunch_end=
		ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->EndInterv[previous_mnga_bunch];
		prev_mnga_bunch_length=prev_mnga_bunch_end-prev_mnga_bunch_beg+1;
		//onlyub17.05.16

		previous_mnga_bunch=
		ColorInt[UpperGreenBoundaryRight].ordered_bunch_number[previous_mnga_bunch];
			  }//pb>

		}//pmnga>=0
		else
		{
		prev_sec_ng_max=-1;
		}
		prev_sec_g_max=RightAdjacentGreenSectionMax[UpperGreenBoundaryRight]-1;

		if(prev_sec_g_max>=0)
		{//prgsm
		green_oppos_boundary_bunch=
		ColorDescrSect[prev_sec_g_max].location_of_section[UpperGreenBoundaryRight];
		green_oppos_boundary_bunch-=1;
		green_oppos_boundary_bunch=
		ColorInt[UpperGreenBoundaryRight].old_bunch_number[green_oppos_boundary_bunch];
		green_oppos_boundary_bunch_beg=
		ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->BegInterv[green_oppos_boundary_bunch];
		green_oppos_boundary_bunch_end=
		ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->EndInterv[green_oppos_boundary_bunch];
		green_oppos_boundary_bunch=
		ColorInt[UpperGreenBoundaryRight].ordered_bunch_number[green_oppos_boundary_bunch];
		if(max_green_adj_section>=0)
		{//last_cor02.05.17
		Intersection_green=
		ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb_prev,
		w_mg_end,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
		}//last_cor02.05.17
		}//prgsm
		else
		{
		prev_sec_g_max=-1;
		}//onlyub18.05.17
		if((Intersection_green>=0)&&(Intersection_green<3))
		{//onlyub18.05.17
		if((ratio_length_green<=4)&&(((previous_mnga>=0)&&(prev_mnga_bunch_length>4*w_mng_length))||(previous_mnga<0)))
		{//onlyub18.05.17
if(((previous_mnga>=0)&&(w_mng_section_weight<prev_mnga_bunch_length/2))||((previous_mnga<0)&&
		((left_end_bb_prev-green_oppos_boundary_bunch_end)>=DimX/6)&&
		(w_mng_length<(left_end_bb_prev-green_oppos_boundary_bunch_end)/2)))
{
goto L;
}
		}//onlyub18.05.17
		}//onlyub18.05.17
		if((Intersection_green>=0)&&(Intersection_green<=1))
				{//onlyub18.05.17
if((ratio_length_green>=12)&&(ratio_length1_green>=13))
{
     if((max_n_green_adj_section>=0)&&
    		 (max_n_green_adj_section_bunch>max_green_adj_section_bunch)&&(w_mng_length>=DimX/3))
     {
    	 if(((count_fiber-UpperGreenBoundaryRight)<=2)&&(count_fiber<=NumStrips/4))
    	 {
  if(((differ_boundary_sign_prev<0)&&(differ_boundary_abs_prev<=6*StripWidth))||
		  ((differ_boundary_abs_prev<=2*StripWidth)&&(differ_boundary_abs_prev>0)))
  {
	  if((first_green_adjacent<max_n_green_adj_section_bunch)&&((sc_gr_comp_red==2)))
	  	{//fgac
	  	if(!RightGreenBoundaryBunch[count_fiber])
	  	    {
	  	condition_number=238;
	  	success_right=1;
	  	new_boundary_section=section_number;
	  	goto L;
	  		}
	  		else
	  			{
	  				goto L;
	  			}
	  	}//fgac
  }
    	 }
     }
}
if(!Intersection_green)
{//onlyub19.05.17
    if((w_mg_section_incline>0)&&(w_mg_incl_curve>0))
    {
  if((max_n_green_adj_section>=0)&&(((w_mng_section_incline<0)||(w_mng_incl_curve<0))||
		  ((sect_incline<0)&&(sect_incl_curve<0))))
  {
     if((w_mng_section_weight>DimX/2)&&((w_mg_section_weight>DimX)||(section_weight>DimX)))
      {
    	 if((count_fiber<=NumStrips/2)&&((sc_gr_comp_red==2)))
    	 {
    		 if(!RightGreenBoundaryBunch[count_fiber])
    		 	  	    {
    		 	  	condition_number=244;
    		 	  	success_right=1;
    		 	  	new_boundary_section=section_number;
    		 	  	goto L;
    		 	  		}
    		 	  		else
    		 	  			{
    		 	  				goto L;
    		 	  			}
    	 }
      }

  }
    }
}//onlyub19.05.17
				}//onlyub18.05.17
	if((count_fiber-UpperGreenBoundaryRight)>=3)
	{//onlyublast_cor11.05.17
		if(max_n_green_adj_section>=0)
		{
	if(count_fiber>=NumStrips/3)
	{
		if((w_mng_hue_zone==3)&&(w_mng_gray_zone<=1)&&(w_mng_sat>=6))
		{
if((w_mng_section_last_ratio<=6)&&(w_mng_section_weight_normalized<=2*StripWidth))
{
goto L;
}
		}
	}
		}

	}//onlyublast_cor11.05.17
	if((UpperGreenBoundaryRight<=NumStrips/3)||((count_fiber-UpperGreenBoundaryRight)<=2))
	{//last_cor28.04.17
		if((count_fiber<NumStrips/2)&&(sc_gr_comp_red==2))
		{//cf
	if((max_n_green_adj_section>=0)&&(max_green_adj_section>=0))
	{//mngs
	if((sect_incline<0)||(sect_incl_curve<0)||(section_weight>=DimX/3)||((dist_to_wmng<=2*StripWidth)&&
	(w_mng_section_incline<0)||(w_mng_incl_curve<0)
	||(w_mng_section_oppos_incline>0)||(w_mng_incl_curve_oppos>0)))
			{
					if(((w_mg_section_incline>0)&&(w_mg_incl_curve>0))||
		(((w_mg_section_incline>0)||(w_mg_incl_curve>0))&&(w_mg_section_weight>=2*DimX)))
					{
		if((section_weight>2*DimX)||((sect_incline<0)&&(sect_incl_curve<0))||(((sect_incline<0)||(sect_incl_curve<0))&&
			((section_weight>DimX)||(w_mg_section_weight>=2*DimX)||(w_mng_section_weight>DimX))))
						{
	if(((w_mng_section_incline<0)&&(w_mng_incl_curve<0))||
	   (((w_mng_section_incline<0)||(w_mng_incl_curve<0))&&(w_mng_section_weight>DimX)))
	{
		if((dist_to_wmng<=2*StripWidth)&&(dist_to_wmg>=DimX/8))
		{
		                   if(((w_mng_section_weight>=DimX/2)&&(section_length>=4))||
		   ((w_mng_section_weight>=DimX/4)&&(w_mg_section_weight>=2*DimX)&&(w_mg_section_first<=NumStrips/4)))
							{
	if(first_green_adjacent<max_n_green_adj_section_bunch)
	{//fgac
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=224;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
	}//fgac
		                    }
		}
	}
						}
		if((w_mg_section_incline>0)&&(w_mg_incl_curve>0)&&(w_mg_section_weight>=2*DimX)&&
			(w_mg_section_color==2))
		{//last_cor02.05.17
			if(((w_mng_section_oppos_incline>0)&&(w_mng_incl_curve_oppos>0))||
				((w_mng_section_incline<0)&&(w_mng_incl_curve<0)))
			{
				if(first_green_adjacent<max_n_green_adj_section_bunch)
				{
					if(!max_green_adj_section_bunch)
					{
	if((non_green_length>=DimX/2)||((w_mng_section_weight>DimX/2)&&
	   (w_mng_section_weight_normalized>=DimX/4)))
	{
		if(count_fiber<=NumStrips/3)
		{
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=232;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
		}
	}
					}
				}
			}
		}//last_cor02.05.17
						if((w_mg_section_weight>=2*DimX)&&(w_mg_sec_length>=NumStrips/4))
						{
							if(w_mng_section_weight>=DimX)
							{

									if(dist_to_wmg>DimX/2)
									{
	if(first_green_adjacent<max_n_green_adj_section_bunch)
	{//fgac
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=222;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
	}//fgac
									}

							}
						}
					}
			}
	/*if(w_mg_section_weight>=6*DimX)
	{//last_cor02.05.17
		if((w_mg_section_color==2)&&(count_fiber<NumStrips/3)&&(sc_gr_comp_red==2))
		{
			if((w_mg_section_first<=NumStrips/4)&&(w_mg_section_last>=(3*NumStrips)/4))
			{
				if(first_green_adjacent<max_n_green_adj_section_bunch)
				{
	if((non_green_length>=DimX/2)&&(!max_green_adj_section_bunch))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=230;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
	}
				}
				else
				{
	if(first_gr<0)
	{
		if((first_green_adjacent==last_gr_in_chain)&&(green_adjacent_section_oppos<=0))
		{
			if((non_green_length>=DimX/2)&&(!max_green_adj_section_bunch))
			{
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=228;
	success_right=2;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
			}
		}
	}
				}
			}
		}
	}//last_cor02.05.17*/
	}//mngas

		}//cf
	}//last_cor28.04.17
	//onlyub18.05.17
	/*if((UpperGreenBoundaryRight>0)&&(UpperGreenBoundaryRight<NumStrips))
	{
	prev_boun_bunch=RightGreenBoundaryBunch[UpperGreenBoundaryRight]-1;
	left_end_bb_prev=
	ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->BegInterv[prev_boun_bunch];
	right_end_bb_prev=
	ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->EndInterv[prev_boun_bunch];
	differ_boundary_sign_prev=beg_int-left_end_bb_prev;
	differ_boundary_abs_prev=abs(differ_boundary_sign_prev);
	prev_sec_ng_max=RightAdjacentNonGreenSectionMax[UpperGreenBoundaryRight]-1;//last_cor21.12.16
	if(prev_sec_ng_max>=0)
	{//pmnga>=0
	prev_sec_ng_max_end=
	ColorSection->section_fibre_last[prev_sec_ng_max];
	prev_sec_ng_max_beg=
	ColorSection->section_fibre_first[prev_sec_ng_max];
	previous_mnga_bunch=
	 ColorDescrSect[prev_sec_ng_max].location_of_section[UpperGreenBoundaryRight];
	//if(w_mng_section_weight>=DimX/6)
	//{//last_cor13.03.17

	//}//last_cor13.03.17
	      if(previous_mnga_bunch>0)
		  {//pb>
			  previous_mnga_bunch--;
	previous_mnga_bunch=ColorInt[UpperGreenBoundaryRight].old_bunch_number[previous_mnga_bunch];
	prev_mnga_bunch_beg=
	ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->BegInterv[previous_mnga_bunch];
	prev_mnga_bunch_end=
	ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->EndInterv[previous_mnga_bunch];
	prev_mnga_bunch_length=prev_mnga_bunch_end-prev_mnga_bunch_beg+1;
	//onlyub17.05.16

	previous_mnga_bunch=
	ColorInt[UpperGreenBoundaryRight].ordered_bunch_number[previous_mnga_bunch];
		  }//pb>

	}//pmnga>=0
	else
	{
	prev_sec_ng_max=-1;
	}
	prev_sec_g_max=RightAdjacentGreenSectionMax[UpperGreenBoundaryRight]-1;

	if(prev_sec_g_max>=0)
	{//prgsm
	green_oppos_boundary_bunch=
	ColorDescrSect[prev_sec_g_max].location_of_section[UpperGreenBoundaryRight];
	green_oppos_boundary_bunch-=1;
	green_oppos_boundary_bunch=
	ColorInt[UpperGreenBoundaryRight].old_bunch_number[green_oppos_boundary_bunch];
	green_oppos_boundary_bunch_beg=
	ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->BegInterv[green_oppos_boundary_bunch];
	green_oppos_boundary_bunch_end=
	ColorInt[UpperGreenBoundaryRight].ColoredIntervalsStructure->EndInterv[green_oppos_boundary_bunch];
	green_oppos_boundary_bunch=
	ColorInt[UpperGreenBoundaryRight].ordered_bunch_number[green_oppos_boundary_bunch];
	if(max_green_adj_section>=0)
	{//last_cor02.05.17
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(green_oppos_boundary_bunch_end,left_end_bb_prev,
	w_mg_end,beg_int,&indic_length_green,&ratio_length_green,&ratio_length1_green);
	}//last_cor02.05.17
	}//prgsm
	else
	{
	prev_sec_g_max=-1;
	}*/
	//onlyub18.05.17
	    if((count_fiber<NumStrips/2)&&(sc_gr_comp_red==2))
		{//cf1//last_cor02.05.17
	if((max_n_green_adj_section>=0)&&(max_green_adj_section>=0))
	{//mngs1
	if((w_mg_section_incline>0)&&(w_mg_incl_curve>0)&&(w_mg_section_weight>=2*DimX)&&
	   (w_mg_section_first<=NumStrips/4))
	{//w_mgi
	if((w_mng_section_oppos_incline>0)&&(w_mng_incl_curve_oppos>0)&&
	   (w_mng_section_weight>=DimX/3)&&
	(w_mng_section_weight_normalized>=DimX/8))
	{//norm
		if(prev_sec_g_max==max_green_adj_section)
		{//psgm
			if(first_green_adjacent<max_n_green_adj_section_bunch)
			{//fga<mngas
			if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=226;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
			}//fga<mngas
	    }//psgm
	}//norm
	}//norm
	}//mngs1
	    }//cf1//last_cor02.05.17
	if((prev_sec_ng_max>=0)&&(max_n_green_adj_section>=0))
	{//last_cor07.04.17
	if((w_mng_hue_zone==3)&&(w_mng_gray_zone>=1)&&(w_mng_gray_zone<=2))
	   {
	      if(w_mng_sat>=4)
		  {
	/*if((((count_fiber-UpperGreenBoundaryRight)>=3)||((count_fiber>UpperGreenBoundaryRight)
	   &&(prev_mnga_bunch_length<=5*StripWidth)&&((prev_mnga_bunch_end-w_mg_beg)>=StripWidth)))
				  &&(w_mng_section_last_ratio>=7)&&
				  ((count_fiber-prev_sec_ng_max_end)>=3))*/
	if(((((count_fiber-UpperGreenBoundaryRight)>=3)&&((count_fiber-prev_sec_ng_max_end)>=3))
	   ||((count_fiber>UpperGreenBoundaryRight)
	   &&(prev_mnga_bunch_length<=5*StripWidth)&&
	   (((max_green_adj_section>=0)&&((w_mg_end-left_end_bb_prev)>=StripWidth/2))||
	   ((first_green_adjacent>=0)&&((old_first_end-left_end_bb_prev>=StripWidth/2)))
	   ||((max_green_adj_section<0)&&((w_mng_beg-left_end_bb_prev)>=StripWidth/2)))))
				  &&(w_mng_section_last_ratio>=7))
			  {
				  break;
			  }
		  }
	   }
	}//last_cor07.04.17
	if((prev_sec_ng_max>=0)&&(max_n_green_adj_section>=0))
	{//last_cor07.04.17
		if(((count_fiber-UpperGreenBoundaryRight)<=3)&&(count_fiber<=NumStrips/2))
		{//last_cor24.04.17
			if((prev_sec_ng_max==max_n_green_adj_section)&&(prev_sec_g_max==max_green_adj_section))
			{
				if(first_green_adjacent<max_n_green_adj_section_bunch)
				{
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=216;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
				}
			}
		}//last_cor24.04.17
	if((w_mng_hue_zone==3)&&(w_mng_gray_zone>=1)&&(w_mng_gray_zone<=2))
	   {
	      if(w_mng_sat>=4)
		  {
	if(((count_fiber>UpperGreenBoundaryRight)&&(prev_mnga_bunch_length<=5*StripWidth)&&
	   (((prev_sec_g_max>=0)&&(((green_oppos_boundary_bunch_end-beg_int)>=StripWidth/2)))||
	   ((count_fiber>=NumStrips/2)&&((prev_mnga_bunch_beg-beg_int>=StripWidth/2)))))
				  &&(w_mng_section_last_ratio>=7))
			  {
				  break;
			  }
		  }
	   }
	}//last_cor07.04.17
	section_boun_prev=RightGreenBoundarySection[UpperGreenBoundaryRight];//last_cor27.12.16
	  if(section_boun_prev>0)
	  {//sbp>0
		section_boun_prev--;
	section_beg_prev=ColorSection->section_fibre_first[section_boun_prev];
	section_end_prev=ColorSection->section_fibre_last[section_boun_prev];
	section_length_prev=section_end_prev-section_beg_prev+1;
	  }//sbp>0last_cor27.12.16
	  else
	  {
	  section_boun_prev=-1;
	  }//last_cor27.12.16



	if((count_fiber-UpperGreenBoundaryRight)<=2)
	{//last_cor15.03.17
		if(differ_boundary_abs_prev<=5*StripWidth)
		{
			if((max_n_green_adj_section>=0)&&(w_mng_section_weight>2*StripWidth))
			{
				if(prev_sec_ng_max==max_n_green_adj_section)
				{
					if(first_green_adjacent<max_n_green_adj_section_bunch)
					{
	if(!RightGreenBoundaryBunch[count_fiber])
	    {
	condition_number=192;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
					}
				}
			}
		}
	}//last_cor15.03.17
	if(!UpperGreenBoundaryRight)
		{//last_cor15.03.17
			if((section_weight>DimX/2)&&(sc_gr_comp_red==2))
			{
	if(((max_green_adj_section>=0)&&(w_mg_section_weight>=DimX/2))||
	   ((w_mg_section_oppos>=0)&&(w_mg_section_weight_oppos>=DimX/2)))
	{
					if((max_n_green_adj_section>=0)&&(w_mng_section_weight>=2*DimX))
					{
	if((w_mng_section_incline<0)||((w_mng_incl_curve>-2)&&(w_mng_incl_curve<-1)))
	{
		if((first_green_adjacent<max_n_green_adj_section_bunch)&&(dist_next_green>=DimX/3))
		{
	if(!RightGreenBoundaryBunch[count_fiber])
		{//last_cor15.03.17
	condition_number=194;
	success_right=1;
	new_boundary_section=section_number;
		}
	else
	{
		goto L;
	}
		}
	}
					}
	}
			}
		}//last_cor15.03.17
	if((count_fiber>UpperGreenBoundaryRight)&&(count_fiber<(UpperGreenBoundaryRight+4)))
	{//last_cor10.03.17
	if(differ_boundary_abs_prev>DimX/3)
				 {
	 if(prev_sec_ng_max!=max_n_green_adj_section)
	 {//last_cor19.04.17
		         break;
	 }
				 }
	}//last_cor10.03.17
	     if((count_fiber>(UpperGreenBoundaryRight+2))&&(count_fiber>NumStrips/3))
			 {//last_cor05.11.1620.12.16
				 if(differ_boundary_abs_prev>DimX/6)
				 {
	 if(prev_sec_ng_max!=max_n_green_adj_section)
	 {//last_cor19.04.17
		         break;
	 }
				 }
			 }
	}
	}
		}//lr
		else
		{//elr
	//boundary_bunch=LeftGreenBoundaryBunch[count_fiber-1];
	if(boundary_bunch>=0)
	{//bb!0//last_cor09.12.16

		   if(max_green_adj_section>=0)
		   {//mgas
	if(green_oppos_boundary_sec>0)
	{//gobslast_cor14.04.17
			   dist_gobs_mgas=w_mg_beg-green_oppos_boundary_bunch_beg;
				dist_gobs_mgas_abs=abs(dist_gobs_mgas);
				if(max_green_adj_section_bunch>max_n_green_adj_section_bunch)
					{//last_cor16.08.17mgasb>mngasb
		if(previous_mnga>=0)
		{//last_cor16.08.17pmnga>=0
			if(green_oppos_boundary_bunch>previous_mnga_bunch)
			{//gobs>pmna
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,green_oppos_boundary_bunch_beg,
	end_int,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
			}//gobs>pmna
			else
			{//egobs>pmna
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,prev_mnga_bunch_end,
	end_int,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
			}//egobs>pmna
		}//last_cor16.08.17pmnga>=0
		else
		{//last_cor16.08.17epmnga>=0
Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,green_oppos_boundary_bunch_beg,
	end_int,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
		}//last_cor16.08.17epmnga>=0
				}//last_cor16.08.17mgasb>mngasb

				if(previous_mnga>=0)
				{
		if(previous_mnga_bunch<green_oppos_boundary_bunch)
		{//last_cor05.04.17pmnga<
			   if((first_green_adjacent==bunch_adjacent_ordered)&&(first_green_adjacent>=0))
			   {//fga=bao
	if(first_green_adjacent==last_gr_in_chain)
	{//last_cor29.03.17
		if((first_gr<0)||(last_gr==last_gr_in_chain))
		{
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,green_oppos_boundary_bunch_beg,
	old_first_end,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
		}
		else
		{
		     if(first_gr>=0)
		     {
		    	 Intersection_green=
		    	 	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,green_oppos_boundary_bunch_beg,
		    	 	last_gr_end,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
		     }
		}
	}
	else
	{
		if((first_gr<0)||(last_gr==last_gr_in_chain))
		{//onlyublast_cor31.05.17
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,green_oppos_boundary_bunch_beg,
	old_first_end_lg,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
		}//onlyublast_cor31.05.17
		else
		{
		if(first_gr>=0)
				     {
				    	 Intersection_green=
				    	 	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,green_oppos_boundary_bunch_beg,
				    	 	last_gr_end,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
				     }
		}
	}
			   }//fga=bao
			   else
			   {//fga!=bao
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,green_oppos_boundary_bunch_beg,
	end_int,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
	if((max_green_adj_section==green_oppos_boundary_sec)&&(max_n_green_adj_section>=0)&&
		(first_green_adjacent<max_n_green_adj_section_bunch))
	{//last_cor29.03.17mgas=gobslast_cor05.11.17
		if((last_gr_in_chain<max_green_adj_section_bunch)&&(dist_fga_wmg>=7*StripWidth))
		{//lg<mga
			if(first_green_adjacent==last_gr_in_chain)
			{//fga=lg
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,green_oppos_boundary_bunch_beg,
	old_first_end,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
			}//fga=lg
			else
			{//fga!=lg
				if(last_gr_in_chain<max_n_green_adj_section_bunch)
				{//last_cor05.11.17
			Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,green_oppos_boundary_bunch_beg,
	old_first_end_lg,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
				}
			}//fga!=lg
		}//lg<mga
	}//last_cor29.03.17mgas=gobs
			   }//fga!=bao
		}//last_cor05.04.17pmnga
				}//pmnga>=0
			   if((Intersection_green==3)&&(ratio_length_green>=StripWidth))
					{//last_cor31.03.17
						goto L;
					}//last_cor31.03.17
	if(max_green_adj_section!=green_oppos_boundary_sec)
				{//last_cor28.03.17
	if((previous_mnga_bunch<green_oppos_boundary_bunch)&&
	   (dist_gobs_mgas_abs>=15*StripWidth)&&(((Intersection_green>=0)&&(ratio_length_green<6))||
	   (Intersection_green<0)||(Intersection_green==3)))
					{
						goto L;
					}
				}//last_cor28.03.17
	}//gobs
	else
	{//last_cor14.04.17
		if(previous_mnga>=0)
		{//pmnga>=0
	          if((first_green_adjacent==bunch_adjacent_ordered)&&(first_green_adjacent>=0))
	           {//fga=bao
	if(first_green_adjacent==last_gr_in_chain)
	{//last_cor29.03.17
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,prev_mnga_bunch_end,
	old_first_end,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
	}
	else
	{
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,prev_mnga_bunch_end,
	old_first_end_lg,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
	}
			   }//fga=bao
			   else
			   {//fga!=bao
	Intersection_green=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,prev_mnga_bunch_end,
	end_int,w_mg_beg,&indic_length_green,&ratio_length_green,&ratio_length1_green);
	           }//fga!=bao
		}//pmnga>=0
	}//last_cor14.04.17

		   }//mgas
		   else//last_cor14.04.07
		   {//emgas
		if(previous_mnga>=0)
		{//pmnga>=0last_cor14.04.17
			if(green_oppos_boundary_sec>0)
	    {//gobslast_cor14.04.17
			   if((first_green_adjacent==bunch_adjacent_ordered)&&(first_green_adjacent>=0))
			   {//fga=bao
	if(first_green_adjacent==last_gr_in_chain)
	{//last_cor29.03.17
	Intersection_green1=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,prev_mnga_bunch_end,
	old_first_end,w_mg_beg,&indic_length_green1,&ratio_length_green1,&ratio_length1_green1);
	}
	else
	{
	Intersection_green1=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,prev_mnga_bunch_end,
	old_first_end_lg,w_mg_beg,&indic_length_green1,&ratio_length_green1,&ratio_length1_green1);
	}
			   }//fga=bao
			   else
			   {//fga!=bao
	Intersection_green1=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,prev_mnga_bunch_end,
	end_int,w_mg_beg,&indic_length_green1,&ratio_length_green1,&ratio_length1_green1);

			   }//fga!=bao
			}//gobslast_cor14.04.17
			else
			{//egobslast_cor14.04.17
				if(max_n_green_adj_section>=0)
		        {//mnga>=0
	if((first_green_adjacent==bunch_adjacent_ordered)&&(first_green_adjacent>=0))
			   {//fga=bao
	if(first_green_adjacent==last_gr_in_chain)
	{//last_cor29.03.17
	Intersection_green1=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,prev_mnga_bunch_end,
	old_first_end,w_mng_end,&indic_length_green1,&ratio_length_green1,&ratio_length1_green1);
	}
	else
	{
	Intersection_green1=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,prev_mnga_bunch_end,
	old_first_end_lg,w_mng_end,&indic_length_green1,&ratio_length_green1,&ratio_length1_green1);
	}
			   }//fga=bao
			   else
			   {//fga!=bao
	Intersection_green1=
	ColorInt[count_fiber].Intersection_measure_ratios(right_end_bb,prev_mnga_bunch_end,
	end_int,w_mng_end,&indic_length_green1,&ratio_length_green1,&ratio_length1_green1);

			   }//fga!=bao
				}//mnga>=0
				}//egobslast_cor14.04.17
		}//pmnga>=0last_cor14.04.17
		}//emgaslast_cor14.04.17


		if((max_green_adj_section>=0)&&(max_n_green_adj_section>=0)&&
			(((Intersection_green<3)&&(Intersection_green>=0))||
			((green_oppos_boundary_sec>=0)&&(Intersection_green<0)||(Intersection_green==3))))
	{//last_cor27.03.17
		if(max_green_adj_section_bunch<max_n_green_adj_section_bunch)
		{
	if(w_mng_section_oppos_bunch>max_green_adj_section_bunch)
			{//last_cor05.04.17
			goto L;
			}
		}
	}//last_cor27.03.17
	if(first_green_adjacent>=0)
	{
		if(first_green_adjacent==last_gr_in_chain)
		{
		differ_bb_fga=right_end_bb-old_first_end;
		}
		else
		{
			//if(first_gr<0)//last_cor04.05.17
			//{
		differ_bb_fga=right_end_bb-old_first_end_lg;
			//}
			//else
			//{
		//differ_bb_fga=right_end_bb-last_gr_end;
			//}
		}

	if(first_green_adjacent==bunch_adjacent_ordered)
		                       	{//last_cor07.12.16!may be incorrect

		if((differ_bb_fga<0)&&(abs(differ_bb_fga)>=10*StripWidth)
			&&(previous_mnga!=max_n_green_adj_section)&&
	   (green_oppos_boundary_sec!=max_green_adj_section)&&(count_fiber>(2*NumStrips)/3))
		{//last_cor31.01.17
			break;
		}
		                        }//last_cor07.12.16
	}
	if((differ_boundary_sign<0)&&(differ_boundary_abs>=15*StripWidth)&&(count_fiber>NumStrips/3)&&
	(((ratio_length_green<=8)&&(Intersection_green>=0))||(Intersection_green<0)||
	 (Intersection_green==3))&&
	 ((previous_mnga!=max_n_green_adj_section)&&(green_oppos_boundary_sec!=max_green_adj_section)))
	{//last_cor29.03.17
		break;
	}
	//onlyuberror!last_cor26.05.17
	if((section_bunch_shift>=0)&&(section_bunch_shift_abs>=0)&&(section_bunch_shift_abs>=8*StripWidth)
	 &&(count_fiber>NumStrips/3)&&
	 (((ratio_length_green<=7)&&(Intersection_green>=0))||(Intersection_green<0)||
	 (Intersection_green==3))&&
	 (differ_boundary_abs>=15*StripWidth)&&(end_int>DimX/2))
	{//last_cor30.03.17
		break;
	}//last_cor30.03.17
	//onlyuberror!last_cor26.05.17
	if((section_bunch_shift>=0)&&(section_bunch_shift_abs>=0)&&(section_bunch_shift_abs>=12*StripWidth)
	 &&(count_fiber>NumStrips/6)&&
	 (((ratio_length_green<=7)&&(Intersection_green>=0))||(Intersection_green<0)||
	 (Intersection_green==3))&&
	 (differ_boundary_abs>=18*StripWidth)&&(end_int>DimX/2))
	{//last_cor30.03.17
		break;
	}//last_cor30.03.17
	if((differ_boundary_sign>0)&&(differ_boundary_abs>=10*StripWidth)
	   &&(previous_mnga!=max_n_green_adj_section)&&
	   (green_oppos_boundary_sec!=max_green_adj_section)&&(count_fiber>(2*NumStrips)/3))
		                       	{//last_cor07.12.16!may be incorrect
		                       		break;
		                       	}//last_cor07.12.16
	/*if((green_adjacent_section>=0)&&(first_green_adjacent<max_n_green_adj_section_bunch)
	   &&(green_adjacent_section>section_number)&&(green_adjacent_section_weight>DimX/4)&&
	   (first_green_adjacent<max_green_adj_section_bunch))
	{
		goto L;
	}*/
	if((max_n_green_adj_section<0)&&(max_green_adj_section>=0))
	{//last_cor10.03.17
		if((last_gr_in_chain==max_green_adj_section_bunch)||
			(first_green_adjacent<max_green_adj_section_bunch)&&(dist_wmng_to_mga>=0))
		{//last_cor29.03.17
		    if(previous_mnga>=0)
		    {
				if(((10*green_lg_before)>(8*dist_to_wmg))&&(green_lg_before>=3*StripWidth))
				{
	            goto L;
				}
		    }
		}
	}
	   if((max_n_green_adj_section>=0)&&(max_green_adj_section>=0)&&
		   (max_green_adj_section==green_oppos_boundary_sec))
	   {//last_cor10.03.17
	if(first_green_adjacent>max_n_green_adj_section_bunch)
	{
		if((last_gr_in_chain==max_green_adj_section_bunch)||
			(first_green_adjacent<max_green_adj_section_bunch))
		{
			if(old_first_beg<prev_mnga_bunch_beg)
			{
				if((10*green_lg_before)>(8*dist_wmng_to_mga))
				{
	            goto L;
				}
			}
			if((w_mng_length<dist_wmng_to_mga/4)&&(w_mng_length<prev_mnga_bunch_length/4))
			{
	if(abs(differ_bb_fga)>=10*StripWidth)
	{
		goto L;
	}
			}
		}
	}
	   }//last_cor10.03.17
	if(success_oppos!=3)
	{
	if((max_n_green_adj_section<0)&&(w_mng_section_oppos<0))
	{
		if(((Intersection_green>=0)&&(ratio_length_green>=14)&&(ratio_length1_green<7))||
			((green_oppos_boundary_sec>0)&&(max_green_adj_section)&&(Intersection_green<0)))
		{//last_cor29.03.17
			if(count_fiber>UpperGreenBoundaryRight)
			{
	            prior=-1;
			    goto M;
			}
		}
	}
	}

	if(max_green_adj_section>=0)
	{//last_cor02.03.17
if((Intersection_green>=0)&&(Intersection_green<=1))
{//onlyublast_cor30.05.17
	if(success_oppos==3)
	{//last_cor21.11.17
if((differ_boundary_sign>=0)&&(differ_boundary_abs<=3*StripWidth))
{
	if(ratio_length1_green>=14)
	{
if((sect_incline>0)&&(sect_incl_curve>0)&&(w_mg_section_weight>=DimX/3))
 {
if(((max_n_green_adj_section>=0)&&(first_green_adjacent>max_n_green_adj_section_bunch))
		||((w_mng_section_oppos>=0)&&(first_green_adjacent>w_mng_section_oppos_bunch)&&
		(w_mng_section_weight_oppos>=3*DimX)))
{//last_cor11.03.18
if(!LeftGreenBoundaryBunch[count_fiber])
						{
				condition_number=225;
				success_left=1;
				//new_boundary_section=section_number;
				goto L;
				       }
				        else
						{
							goto L;
						}
}
}
	}
}
	}//last_cor21.11.17
	{//last_cor21.11.17
		if(max_green_adj_section==green_oppos_boundary_sec)
		{
			if((abs(dist_to_wmng)<=StripWidth)||
((first_green_adjacent>max_n_green_adj_section_bunch)&&(abs(dist_to_wmng)<=2*StripWidth)))
			{
if((differ_boundary_sign>=0)&&(differ_boundary_abs<=5*StripWidth))
{
if(!LeftGreenBoundaryBunch[count_fiber])
						{
				condition_number=223;
				success_left=1;
				//new_boundary_section=section_number;
				goto L;
				       }
				        else
						{
							goto L;
						}
}
			}
		}
	}//last_cor21.11.17
	if(max_n_green_adj_section>=0)
		{
if(previous_mnga==max_n_green_adj_section)
{
	   if(max_green_adj_section>=0)
	   {
	if(max_green_adj_section==green_oppos_boundary_sec)
	{//mgas=gobs
		if(first_green_adjacent>=0)
		{
	if(first_green_adjacent<max_n_green_adj_section_bunch)
			{
if( (ratio_length_green>=5)&&(ratio_length1_green>=14))
{
	if(success_oppos==3)
	{
if((first_green_adjacent==last_gr_in_chain)&&((first_gr<0)||((first_gr>=0)&&(last_gr==last_gr_in_chain))))
		{//fga=lgic
			if(!LeftGreenBoundaryBunch[count_fiber])
						{
				condition_number=217;
				success_left=2;
				//new_boundary_section=section_number;
				goto L;
				       }
				        else
						{
							goto L;
						}
		}//fga=lgic
else
{
	if((first_green_adjacent!=last_gr_in_chain)&&((first_gr<0)||((first_gr>=0)&&(last_gr==last_gr_in_chain))))
	{
		if(!LeftGreenBoundaryBunch[count_fiber])
								{
						condition_number=219;
						success_left=3;
						//new_boundary_section=section_number;
						goto L;
						       }
						        else
								{
									goto L;
								}
	}
}
	}

}
			}
		}
	}//mgas=gobs
	   }
}

		}
}//onlyublast_cor30.05.17
		if((Intersection_green>=0)&&(Intersection_green<=1)&&(differ_boundary_abs>=0)&&(ratio_length_green>=6))
			{//ig01
	if(max_n_green_adj_section>=0)
	{//last_cor21.03.17mngas>=0
				if(max_green_adj_section==green_oppos_boundary_sec)
				{//mgas=gobs
	if(first_green_adjacent>max_n_green_adj_section_bunch)
	{//fga>
		if(success_oppos==3)
		{//so3
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=185;
	success_left=1;
	//new_boundary_section=section_number;
	goto L;
	       }
	        else
			{
				goto L;
			}
		}//so3
	}//fga>
				}//mgas=gobs
				else//last_cor26.04.17
				{
	if((ratio_length_green>=12)&&(ratio_length1_green>=10))
	{
		if(first_green_adjacent>max_n_green_adj_section_bunch)
		{//fgacond4
	  if(((differ_boundary_sign>0)&&(differ_boundary_abs<=5*StripWidth))||
	   (differ_boundary_abs<=2*StripWidth))
	  {
	 if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=207;
	success_left=1;
	//new_boundary_section=section_number;
	goto L;
	       }
	        else
			{
				goto L;
			}
	  }
		}//fgacond4
		else
		{
	if(((differ_bb_fga<0)&&(abs(differ_bb_fga)<=5*StripWidth))||(abs(differ_bb_fga)<=2*StripWidth))
	{
	   if((first_green_adjacent==last_green_adjacent)||((first_green_adjacent==last_gr_in_chain)&&
		   (first_gr<0)))
	   {//fgacond1
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=209;
	success_left=2;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
	   }//fgacond1
	if((first_green_adjacent!=last_gr_in_chain)&&
		   ((first_gr<0)||((first_gr>=0)&&(last_gr=last_gr_in_chain))))
	{//fgacond2
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=211;
	success_left=3;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
	}
			}//fgacond0
		}
	}
	}//last_cor26.04.17
	}//last_cor21.03.17mngas>=0
	if(((differ_boundary_sign>0)&&(differ_boundary_abs<=5*StripWidth))||
	   (differ_boundary_abs<=2*StripWidth))
	  {
	 if(((ratio_length_green>=6)&&(ratio_length1_green>=10))||
	((max_green_adj_section==green_oppos_boundary_sec)&&
	 (ratio_length_green>=7)&&(ratio_length1_green>=7)))
	 {
		  if(first_green_adjacent>bunch_adjacent_ordered)
		  {//last_cor20.02.17
			  if((max_n_green_adj_section>=0)||(w_mng_section_oppos>=0))
			  {
				  if((first_green_adjacent>max_n_green_adj_section_bunch)||
					  (first_green_adjacent>w_mng_section_oppos_bunch))
				  {
	        if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=173;
	success_left=1;
	//new_boundary_section=section_number;
	goto L;
	       }
	        else
			{
				goto L;
			}
				  }
			  }
		  }//last_cor20.02.17
	 }
	}
			}//ig01
	if((Intersection_green>=0)&&(Intersection_green<=2)&&(differ_boundary_abs>=0))
			{//ig01last_cor01.03.17
				if(max_green_adj_section==green_oppos_boundary_sec)
				{
				if((w_mg_section_incline<0)&&(w_mg_incl_curve<0)&&(w_mg_section_weight>=DimX/3))
				{
	if((((differ_boundary_sign>0)&&(differ_boundary_abs<=5*StripWidth))||
	   (differ_boundary_abs<=3*StripWidth))&&(differ_boundary_abs<=0))
	{
	   if((ratio_length_green>=4)&&(ratio_length1_green>=4)&&(previous_mnga>=0)&&
		   (previous_mnga==max_n_green_adj_section))
	   {//last_cor19.04.17
	if(first_green_adjacent>bunch_adjacent_ordered)
		  {//last_cor20.02.17
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=177;
	success_left=1;
	//new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
		  }//last_cor20.02.17
	   }
	}
				}
			    }
	if(((differ_boundary_sign>0)&&(differ_boundary_abs<=5*StripWidth))||
	   ((differ_boundary_abs<=2*StripWidth)&&(differ_boundary_abs<=0)))
	  {//last_cor10.03.17
	if((max_green_adj_section==green_oppos_boundary_sec)&&
	   (ratio_length_green>=4)&&(ratio_length1_green>=4))
	 {
		 if((sect_incline>0)&&(sect_incl_curve>0)&&(w_mg_section_weight>=DimX/3))
		 {
if(((max_n_green_adj_section>=0)&&(first_green_adjacent>max_n_green_adj_section_bunch))
		||((w_mng_section_oppos>=0)&&(first_green_adjacent>w_mng_section_oppos_bunch)))
{//onlyublast_cor30.05.17
		  //if((first_green_adjacent>bunch_adjacent_ordered)&&
			  //((differ_boundary_sign>0)||(differ_boundary_abs<abs(differ_bb_fga))))
		  //{//last_cor20.02.17
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=175;
	success_left=1;
	//new_boundary_section=section_number;
	goto L;
			}
	        else
			{
				goto L;
			}
		  //}//last_cor20.02.17
}//onlyublast_cor30.05.17
		 }
	 }
	  }
			}//ig01
	}//last_cor02.03.17
	else//last_cor26.04.17
	{
		if(max_n_green_adj_section>=0)
		{//mngas>=
	if((Intersection_green1>=0)&&(Intersection_green1<=1)&&(ratio_length_green1>=8))
	{//ig101
		if((first_green_adjacent<0)||(first_green_adjacent>max_n_green_adj_section_bunch))
		{
	if(((differ_boundary_sign>0)&&(differ_boundary_abs<=5*StripWidth))||
	   ((differ_boundary_abs<=2*StripWidth)&&(differ_boundary_abs<=0)))
	{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=203;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
	}
		}
			if((first_green_adjacent>=0)&&(first_green_adjacent<max_n_green_adj_section_bunch))
			{//fgacond0
	if(((differ_bb_fga<0)&&(abs(differ_bb_fga)<=5*StripWidth))||(abs(differ_bb_fga)<=2*StripWidth))
	{
	   if((first_green_adjacent==last_green_adjacent)||((first_green_adjacent==last_gr_in_chain)&&
		   (first_gr<0)))
	   {//fgacond1
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=201;
	success_left=2;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
	   }//fgacond1
	if((first_green_adjacent!=last_gr_in_chain)&&
		   ((first_gr<0)||((first_gr>=0)&&(last_gr=last_gr_in_chain))))
	{//fgacond2
		if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=205;
	success_left=3;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
	}//fgacond2
	}
	else
	{
		if((differ_bb_fga<0)&&(abs(differ_bb_fga)>5*StripWidth)&&(abs(differ_bb_fga)<=7*StripWidth))
		{
			if((differ_boundary_sign>0)&&(differ_boundary_abs<=2*StripWidth))
			{
	if((first_green_adjacent==last_green_adjacent)||((first_green_adjacent==last_gr_in_chain)&&
		   (first_gr<0)))
	   {//fgacond3
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=213;
	success_left=2;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
	   }//fgacond3
	if((first_green_adjacent!=last_gr_in_chain)&&
		   ((first_gr<0)||((first_gr>=0)&&(last_gr=last_gr_in_chain))))
	{//fgacond4
		if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=215;
	success_left=3;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
	}//fgacond4
			}
		}
	}
			}//fgacond0

	}//ig101
		}//mngas>=
	}//last_cor26.04.17
	if((max_n_green_adj_section_bunch>=0)&&
	   ((first_green_adjacent<0)||(first_green_adjacent>max_n_green_adj_section_bunch)))
	{//last_cor14.12.1627.12.16
		if((max_n_green_adj_section>=0)&&(previous_mnga>=0)&&
			(previous_mnga==max_n_green_adj_section)&&(sc_gr_comp_red==2))
		{//last_cor23.12.16
			if(((differ_boundary_sign>=0)&&(differ_boundary_abs<=5*StripWidth))||
	   ((differ_boundary_abs<=2*StripWidth)&&(differ_boundary_sign<=0)))
			{//last_cor10.03.17
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=153;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
			}//last_cor10.03.17
		}//last_cor23.12.16
		if(bunch_adjacent_ordered==max_n_green_adj_section_bunch)
		{
			if((w_mng_section_incline>0)||(w_mng_incl_curve>0)&&
				((w_mng_section_weight>=2*DimX)||
				((w_mng_section_weight>=DimX)&&(non_green_length>=DimX/4)))&&
				(w_mng_sat<=1)&&(sc_gr_comp_red==2))
			{
	if(((differ_boundary_sign>0)&&(differ_boundary_abs<=6*StripWidth))||
	   (differ_boundary_abs<=2*StripWidth))
	{
		if(count_fiber<=NumStrips/2)
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=151;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}
	}
			}
		}
	}//last_cor14.12.16
	    if(success_oppos==3)
		{
			if(previous_mnga==max_n_green_adj_section)
			{//last_cor07.12.16
					if((first_green_adjacent>=0)&&(first_green_adjacent==bunch_adjacent_ordered))
					{
		if(((differ_bb_fga<0)&&(abs(differ_bb_fga)<=5*StripWidth))||
			(abs(differ_bb_fga)<=3*StripWidth))
		{
	if(first_green_adjacent==last_gr_in_chain)
	{
	if((max_n_green_adj_section_bunch==(first_green_adjacent+1))||
	   (max_n_green_adj_section_bunch==(first_green_adjacent+2)))
	{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
				if(sc_gr_comp_red==2)
				{
	condition_number=1111;
	new_boundary_section=section_number;
	success_left=2;
	goto L;
				}
			}
	        else
			{
				goto L;
			}
	}
	}
		}
					}
				}//last_cor07.12.16
			if((differ_boundary_sign>0)&&(differ_boundary_abs<=5*StripWidth))
			{
	if((green_oppos_boundary_sec>=0)&&(green_oppos_boundary_sec==max_green_adj_section))
	{//last_cor13.01.16
	if(!Intersection_green)
	{
		if((ratio_length1_green>=14)&&(ratio_length_green>=8))
		{
			if(((first_green_adjacent>bunch_adjacent_ordered)&&(dist_next_green>3*StripWidth)&&
				(first_green_adjacent>max_n_green_adj_section_bunch))||
				(first_green_adjacent==max_green_adj_section_bunch))
			{
	if((!LeftGreenBoundaryBunch[count_fiber])&&(sc_gr_comp_red==2))
			{
	condition_number=157;
	new_boundary_section=section_number;
	success_left=1;
	goto L;
			}
	else
			{
				goto L;
			}
			}
		}
	}
	}//last_cor13.01.16
	if((first_green_adjacent>=0)&&(max_n_green_adj_section_bunch>=0)&&
					(first_green_adjacent>=max_n_green_adj_section_bunch)
	&&(max_green_adj_section>=0)&&(max_green_adj_section==right_oppos_green_max))
				{//last_cor27.12.16
	if((!LeftGreenBoundaryBunch[count_fiber])&&(sc_gr_comp_red==2))
			{
	condition_number=111;
	new_boundary_section=section_number;
	success_left=1;
	goto L;
			}
	else
			{
				goto L;
			}
				}
			}
		}
	}//bb!0
	else
	{//ebb!0
	boundary_bunch=-1;
	if((UpperGreenBoundaryRight>0)&&(count_fiber<UpperGreenBoundaryRight))
	{//last_cor19.07.17
		if(success_oppos==3)
		{
if(max_n_green_adj_section>=0)
{//wmnga
   if((max_green_adj_section>=0)&&(max_green_adj_section==oppos_boun_sec))
   {
if((sc_gr_comp_red==2)&&((sect_incline>0)||(sect_incl_curve>0)||(w_mng_section_incline>0)||
   (w_mng_incl_curve>0)||(w_mng_section_oppos_incline<0)||(w_mng_incl_curve_oppos<0)))
{
     if(first_green_adjacent>max_n_green_adj_section_bunch)
	 {
if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=221;
	new_boundary_section=section_number;
	success_left=1;
	goto L;
			}
	else
			{
				goto L;
			}
	 }
}
   }
}//mnga
if((w_mng_section_oppos>=0)&&(w_mng_section_weight_oppos>DimX))
{//wmngao
   if((max_green_adj_section>=0)&&(max_green_adj_section==oppos_boun_sec))
   {
if((sc_gr_comp_red==2)&&(((sect_incline>0)||(sect_incl_curve>0))&&
   ((w_mng_section_oppos_incline<0)||(w_mng_incl_curve_oppos<0))))
{
     if(first_green_adjacent>max_n_green_adj_section_bunch)
	 {//fga<
if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=227;
	new_boundary_section=section_number;
	success_left=1;
	goto L;
			}
	else
			{
				goto L;
			}
	 }//fga<
	 else
	 {
		 if((dist_fga_wmg>DimX/2)&&(first_green_adjacent==last_gr_in_chain)&&
			 (green_adjacent_section<0))
		 {
if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=229;
	new_boundary_section=section_number;
	success_left=2;
	goto L;
			}
	else
			{
				goto L;
			}
		 }
	 }
}
   }
}//mngao
		}
	}//last_cor19.07.17
	if((!UpperGreenBoundaryLeft)&&(UpperGreenBoundaryRight>0))
	{//last_cor26.04.17
		if(count_fiber<UpperGreenBoundaryRight)
		{
			if(max_n_green_adj_section>=0)
			{
				if(w_mng_section_oppos>=0)
				{
	if(max_n_green_adj_section_bunch>w_mng_section_oppos_bunch)
	{
		 if((sc_gr_comp_red==2)&&(((sect_incline>0)&&(sect_incl_curve>0))||
			 ((section_weight>=4*DimX)&&(last_srip_ratio>=10))))
		 {
	   if((first_green_adjacent>=0)&&(first_green_adjacent<w_mng_section_oppos_bunch))
	   {
	if((max_green_adj_section<0)||(w_mg_section_weight<DimX/4)&&(last_srip_ratio>=10))
	{
	if((non_green_length>DimX/2)||((w_mng_section_weight+w_mng_section_weight_oppos)>=2*DimX))
	{
		if((first_green_adjacent>max_n_green_adj_section_bunch)||(first_green_adjacent<0))
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=197;
	new_boundary_section=section_number;
	success_left=1;
	goto L;
			}
	else
			{
				goto L;
			}
		}
		if((first_green_adjacent+1==w_mng_section_oppos_bunch)||
			(first_green_adjacent==last_green_adjacent)||
			((first_green_adjacent==last_gr_in_chain)&&(first_gr<0)))
		{//fgacond
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=195;
	new_boundary_section=section_number;
	success_left=2;
	goto L;
			}
	else
			{
				goto L;
			}
		}//fgacond
		if((first_green_adjacent!=last_gr_in_chain)&&((first_gr<0)||
			((first_gr>=0)&&(last_gr==last_gr_in_chain))))
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=199;
	new_boundary_section=section_number;
	success_left=3;
	goto L;
			}
	else
			{
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
		}
	}//last_cor26.04.17
	if(!UpperGreenBoundaryLeft)
		{//onlyub18.05.17
		if(count_fiber<=NumStrips/3)
		{
			if((sc_gr_comp_red==2)&&((sect_incline>0)||(sect_incl_curve>0)))
			{
	        ;
			}
		}
	}//onlyub18.05.17
	if((UpperGreenBoundaryLeft>0)&&(UpperGreenBoundaryLeft<NumStrips))
	{//up<
	prev_boun_bunch=LeftGreenBoundaryBunch[UpperGreenBoundaryLeft]-1;
	prev_sec_ng_max=LeftAdjacentNonGreenSectionMax[UpperGreenBoundaryLeft]-1;//last_cor21.12.16
	if(prev_sec_ng_max>=0)
	{//pmnga>=0last_cor25.12.16
	prev_sec_ng_max_end=
	ColorSection->section_fibre_last[prev_sec_ng_max];
	prev_sec_ng_max_beg=
	ColorSection->section_fibre_first[prev_sec_ng_max];

	previous_mnga_bunch=
	 ColorDescrSect[prev_sec_ng_max].location_of_section[UpperGreenBoundaryLeft];
	if(w_mng_section_weight>=DimX/6)
	{//last_cor13.03.17
	   if((w_mng_hue_zone==3)&&(w_mng_gray_zone>=1)&&(w_mng_gray_zone<=2))
	   {
	      if(w_mng_sat>=4)
		  {
			  if(((count_fiber-UpperGreenBoundaryLeft)>=3)&&(w_mng_section_last_ratio>=7)&&
				  ((count_fiber-prev_sec_ng_max_end)>=3))
			  {
				  break;
			  }
		  }
	   }
	}//last_cor13.03.17
	      if(previous_mnga_bunch>0)
	      {//pb>
			  previous_mnga_bunch--;
	previous_mnga_bunch=ColorInt[UpperGreenBoundaryLeft].old_bunch_number[previous_mnga_bunch];
	prev_mnga_bunch_beg=
	ColorInt[UpperGreenBoundaryLeft].ColoredIntervalsStructure->BegInterv[previous_mnga_bunch];
	prev_mnga_bunch_end=
	ColorInt[UpperGreenBoundaryLeft].ColoredIntervalsStructure->EndInterv[previous_mnga_bunch];
	previous_mnga_bunch=
	ColorInt[UpperGreenBoundaryLeft].ordered_bunch_number[previous_mnga_bunch];
		  }//pb>
		  else
		  {
	previous_mnga_bunch=-1;
		  }
	}//pmnga>=0
	else
	{
	prev_sec_ng_max=-1;
	}
	prev_sec_g_max=LeftAdjacentGreenSectionMax[UpperGreenBoundaryLeft]-1;
	if(prev_sec_g_max>=0)
	{//prgsm
	green_oppos_boundary_bunch=
	ColorDescrSect[prev_sec_g_max].location_of_section[UpperGreenBoundaryLeft];
	green_oppos_boundary_bunch-=1;
	green_oppos_boundary_bunch=
	ColorInt[UpperGreenBoundaryLeft].old_bunch_number[green_oppos_boundary_bunch];
	green_oppos_boundary_bunch_beg=
	ColorInt[UpperGreenBoundaryLeft].ColoredIntervalsStructure->BegInterv[green_oppos_boundary_bunch];
	green_oppos_boundary_bunch_end=
	ColorInt[UpperGreenBoundaryLeft].ColoredIntervalsStructure->EndInterv[green_oppos_boundary_bunch];
	green_oppos_boundary_bunch=
	ColorInt[UpperGreenBoundaryLeft].ordered_bunch_number[green_oppos_boundary_bunch];
	}//prgsm
	else
	{
	prev_sec_g_max=-1;
	}
	section_boun_prev=RightGreenBoundarySection[UpperGreenBoundaryLeft];//last_cor27.12.16
	  if(section_boun_prev>0)
	  {//sbp>0
		section_boun_prev--;
	section_beg_prev=ColorSection->section_fibre_first[section_boun_prev];
	section_end_prev=ColorSection->section_fibre_last[section_boun_prev];
	section_length_prev=section_end_prev-section_beg_prev+1;
	  }//sbp>0last_cor27.12.16
	  else
	  {
	  section_boun_prev=-1;
	  }//last_cor27.12.16
	left_end_bb_prev=
	ColorInt[UpperGreenBoundaryLeft].ColoredIntervalsStructure->BegInterv[prev_boun_bunch];
	right_end_bb_prev=
	ColorInt[UpperGreenBoundaryLeft].ColoredIntervalsStructure->EndInterv[prev_boun_bunch];
	differ_boundary_sign_prev=end_int-right_end_bb_prev;
	differ_boundary_abs_prev=abs(differ_boundary_sign_prev);
	if((count_fiber-UpperGreenBoundaryLeft)<=2)
	{//last_cor15.03.17
		if(differ_boundary_abs_prev<=5*StripWidth)
		{
			if((max_n_green_adj_section>=0)&&(w_mng_section_weight>2*StripWidth))
			{
				if(prev_sec_ng_max==max_n_green_adj_section)
				{
					if(first_green_adjacent>max_n_green_adj_section_bunch)
					{
	if(!LeftGreenBoundaryBunch[count_fiber])
	    {
	condition_number=181;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
					}
				}
			}
		}
	}//last_cor15.03.17
	if((first_green_adjacent>=0)&&(bunch_adjacent_ordered==first_green_adjacent))
	{
	differ_boundary_sign_prev=old_first_end-right_end_bb_prev;
	differ_boundary_abs_prev=abs(differ_boundary_sign_prev);
	}
	if((count_fiber>UpperGreenBoundaryRight)&&(count_fiber<(UpperGreenBoundaryRight+2)))
	{//last_cor10.03.17
	if(differ_boundary_abs_prev>DimX/4)
				 {
		         break;
				 }
	}//last_cor10.03.17
	if((UpperGreenBoundaryRight>NumStrips/3)&&(UpperGreenBoundaryRight<NumStrips))
		{//last_cor20.12.16
		if(count_fiber>UpperGreenBoundaryRight)
		{
			if((count_fiber>UpperGreenBoundaryLeft)&&(count_fiber>NumStrips/2)&&(success_oppos!=3))
			{//last_cor20.12.16
				if(((differ_boundary_sign_prev<0)&&(differ_boundary_abs_prev>3*StripWidth))
					&&(differ_boundary_abs_prev>=0))
						{
							break;
						}
			}
			if(((differ_boundary_sign_prev<0)&&(differ_boundary_abs_prev>4*StripWidth))
				&&(differ_boundary_abs_prev>=0))
			{
				break;
			}
		}
		}//last_cor20.12.16
	if((count_fiber>UpperGreenBoundaryLeft)&&(count_fiber>NumStrips/3)
	   &&(count_fiber>UpperGreenBoundaryRight)&&(success_oppos!=3))
		 {//last_cor05.11.16last_cor21.12.16
	 differ_boundary_abs_prev=differ_boundary_abs_prev/(count_fiber-UpperGreenBoundaryLeft);
			 if((differ_boundary_abs_prev>6*StripWidth)||
	(((count_fiber-UpperGreenBoundaryLeft)>=3)&&(differ_boundary_abs_prev>4*StripWidth))||
	(((count_fiber-UpperGreenBoundaryLeft)>=5)&&(count_fiber>NumStrips/3)))
			 {
	 if(prev_sec_ng_max!=max_n_green_adj_section)
	 {//last_cor19.04.17
	         break;
	 }
			 }
		 }//last_cor05.11.16last_cor21.12.16
	if(success_oppos==3)
	{//last_cor13.01.16
	if((prev_sec_g_max>=0)&&(prev_sec_g_max==max_green_adj_section))
	{
		if(((max_n_green_adj_section_bunch>=0)&&
	(first_green_adjacent>=max_n_green_adj_section_bunch))||
	((w_mng_section_oppos_bunch>=0)&&(first_green_adjacent>=w_mng_section_oppos_bunch)))
		{
	if(((differ_boundary_sign_prev>0)&&(differ_boundary_abs_prev<5*StripWidth))||
	   (differ_boundary_abs_prev<=3*StripWidth))
	{
	if((!LeftGreenBoundaryBunch[count_fiber])&&(sc_gr_comp_red==2))
			{
	condition_number=155;
	new_boundary_section=section_number;
	success_left=1;
	goto L;
			}
	else
			{
				goto L;
			}
	}
	    }
	}
	}

	}//up<
	}//ebb!0
		}//elr
	}//cf>0last_cor07.12.16
			}//bao>=
	if(first_green_adjacent<0)
	{
	green_bunch_number[array_num]=first_green_adjacent+1;
	distance_to_green[array_num]=-1;
	without_green_adjacent++;
	}

	if(first_green_adjacent_oppos<0)
	{
	green_bunch_number_oppos[array_num]=first_green_adjacent_oppos+1;
	}
	else
	{
	green_bunch_number_oppos[array_num]=first_green_adjacent_oppos+1;
	}
	if(maximum_non_green_adjacent_oppos<0)
	{
	max_n_green_bunch_oppos[array_num]=maximum_non_green_adjacent_oppos+1;
	adjacent_n_green_length_oppos[array_num]=non_green_length_oppos+1;
	}
	else
	{
	max_n_green_bunch_oppos[array_num]=maximum_non_green_adjacent_oppos+1;
	adjacent_n_green_length_oppos[array_num]=non_green_length_oppos;
	}
	if(max_green_adj_section>=0)
			{
			max_gr_adj_sec[array_num]=max_green_adj_section+1;
	        max_gr_adj_sec_bunch[array_num]=max_green_adj_section_bunch+1;
			}
			else
			{
	        max_gr_adj_sec[array_num]=max_green_adj_section+1;
	        max_gr_adj_sec_bunch[array_num]=max_green_adj_section_bunch+1;
			}
			if(max_green_adj_section_oppos>=0)
			{
			max_gr_adj_sec_oppos[array_num]=max_green_adj_section_oppos+1;
	        max_gr_adj_sec_bunch_oppos[array_num]=max_green_adj_section_bunch_oppos+1;
			}
			else
			{
	        max_gr_adj_sec[array_num]=max_green_adj_section_oppos+1;
	        max_gr_adj_sec_bunch[array_num]=max_green_adj_section_bunch_oppos+1;
			}

	if(left_right)
		 {//last_cor3110.16lr
	if((max_n_green_adj_section>=0)&&(w_mng_section_oppos>=0))
	{
		if((max_green_adj_section>=0)&&(w_mg_section_oppos>=0))
		{//last_cor25.04.17
			if((sect_incline<0)&&(sect_incl_curve<0))
			{
				if((w_mg_section_incline>0)&&(w_mg_incl_curve))
				{
					if((w_mg_section_weight>DimX)&&(w_mg_section_first<=NumStrips/4))
					{
						if((w_mng_section_weight>DimX/3)&&(non_green_length>=DimX/4))
						{
							if(first_green_adjacent<max_n_green_adj_section_bunch)
							{
		if(!RightGreenBoundaryBunch[count_fiber])
	{
			condition_number=218;
		    success_right=1;
			new_boundary_section=section_number;
			goto L;
	}
			else
			{
				goto L;
			}
							}
						}
					}
				}
			}
		}//last_cor25.04.17
	if(((w_mng_section_first_ratio>=7)&&(w_mng_section_last_ratio>=10))||
	  ((w_mng_section_first_ratio_oppos>=7)&&(w_mng_section_last_ratio_oppos>=10)) )
	{
		if(((w_mng_section_incline>0)||(w_mng_incl_curve>0))&&((w_mng_section_oppos_incline<0)||
	(w_mng_incl_curve_oppos<0)))
		{
	   if(((w_mng_hue_zone==4)&&(w_mng_gray_zone>=6)&&((w_mng_sat>=1)||(w_mng_gray_zone>=7)))||
	((w_mng_hue_zone_oppos==4)&&(w_mng_gray_zone_oppos>=6)&&
	 ((w_mng_sat_oppos>=1)||(w_mng_gray_zone_oppos>=7))))
	   {
	//FinalGreenStripNearRoad=count_fiber;???
	   break;
	   }
		}
	}
	}

	}//lr
		 else
		 {//elr
	if((max_n_green_adj_section>=0)&&(w_mng_section_oppos>=0))
	{
		if((max_green_adj_section>=0)&&(w_mg_section_oppos>=0))
		{//last_cor25.04.17
			if((sect_incline>0)&&(sect_incl_curve>0))
			{
				if((w_mg_section_incline<0)&&(w_mg_incl_curve<0))
				{
					if((w_mg_section_weight>DimX)&&(w_mg_section_first<=NumStrips/4))
					{
						if((w_mng_section_weight>DimX/3)&&(non_green_length>=DimX/4))
						{
							if(first_green_adjacent>max_n_green_adj_section_bunch)
							{
		if(!LeftGreenBoundaryBunch[count_fiber])
	{
			condition_number=193;
		    success_left=1;
			new_boundary_section=section_number;
			goto L;
	}
			else
			{
				goto L;
			}
							}
						}
					}
				}
			}
		}//last_cor25.04.17
	if(((w_mng_section_first_ratio>=7)&&(w_mng_section_last_ratio>=10))||
	  ((w_mng_section_first_ratio_oppos>=7)&&(w_mng_section_last_ratio_oppos>=10)) )
	{
		if(((w_mng_section_incline<0)||(w_mng_incl_curve<0))&&((w_mng_section_oppos_incline>0)||
	(w_mng_incl_curve_oppos>0)))
		{
	   if(((w_mng_hue_zone==4)&&(w_mng_gray_zone>=6)&&((w_mng_sat>=1)||(w_mng_gray_zone>=7)))||
	((w_mng_hue_zone_oppos==4)&&(w_mng_gray_zone_oppos>=6)&&
	 ((w_mng_sat_oppos>=1)||(w_mng_gray_zone_oppos>=7))))
	   {
	//FinalGreenStripNearRoad=count_fiber;???
	   break;
	   }
		}
	}
	}

		 }//last_cor3110.16!elr
	if(((max_n_green_adj_section<0)&&(w_mng_section_oppos<0))||
		((w_mng_section_weight<=5*StripWidth)&&(w_mng_section_weight_oppos<=5*StripWidth)))
		 {//last_cor3110.16
	 if((max_n_green_adj_section>=0)&&(w_mng_section_first_ratio>=8))
	 {//mngs>=wmng>=
		 if((max_green_adj_section>=0))
		 {
			 if((non_green_length<=2*StripWidth)||(dist_to_wmg<=2*StripWidth))
			 {

	 if((w_mng_hue_zone==3)&&(max_n_green_adj_section_bunch==bunch_adjacent_ordered))
	 {
	 if((first_n_green_oppos<0)||(non_green_length_oppos<=2*StripWidth))
		 {
	 if((w_mg_section_weight>=DimX)&&(section_weight>=DimX))
				 {//wei
	FinalGreenStripNearRoad=count_fiber;
	             }//wei
		 }
		 break;
	 }


			 }
		 }
	 }//mngs>=wmng>=
	 if((w_mng_section_oppos>=0)&&(w_mng_section_first_ratio_oppos>=8))
	 {//mngs>=wmng>=oppos
		 if((max_green_adj_section>=0))
		 {
			 if((non_green_length<=2*StripWidth)||(dist_to_wmg<=2*StripWidth))
			 {

	 if((w_mng_hue_zone_oppos==3)&&(w_mng_section_oppos_bunch==bunch_adjacent_ordered))
	 {
	 if((first_n_green_oppos<0)||(non_green_length_oppos<=2*StripWidth))
		 {
	if((w_mg_section_weight>=DimX)&&(section_weight>=DimX))
				 {
	FinalGreenStripNearRoad=count_fiber;
	             }
		 }
		 break;

	 }

			 }
		 }
	 }//mngs>=wmng>=oppos
	 if((w_mng_section_oppos<0)&&(max_n_green_adj_section<0))
	 {//mngs>=wmng>=oppos
		 if((max_green_adj_section>=0))
		 {//mgas>=
			 if((non_green_length<=2*StripWidth)||(dist_to_wmg<=2*StripWidth))
			 {//dist<=2
	 if((((bunch_adjacent_ordered_old_sat>=3)&&(bunch_adjacent_ordered_old_gray_zone>=2))||
		 ((bunch_adjacent_ordered_old_sat>=4)&&(bunch_adjacent_ordered_old_gray_zone==1)))&&
		 ((bunch_adjacent_ordered_old_hue_zone==3)||(bunch_adjacent_ordered_old_hue_zone==4)))
	 {
		 if((first_n_green_oppos<0)||(non_green_length_oppos<=2*StripWidth))
		 {
	if((w_mg_section_weight>=DimX)&&(section_weight>=DimX))
				 {
	//FinalGreenStripNearRoad=count_fiber;
	             }
		 }
		 break;
	 }
			 }//dist<=2
		 }//mgas>=
	 }//mngs>=wmng>=oppos
		 }//last_cor3110.1602.11.16
	if((max_n_green_adj_section>=0)&&(w_mng_section_oppos>=0)&&(sc_gr_comp_red==2)&&
	   (count_fiber<NumStrips/2)&&(boundary_bunch<0))
	{//last_cor31.03.17
	if(max_green_adj_section>=0)
	{
	w_mg_vert_oppos2=*(VerticalComponent+w_mg_section_oppos2);
		if(w_mg_section_weight>6*DimX)
		{//last_cor24.04.17
			if(((w_mg_section_oppos>=0)&&(max_green_adj_section_bunch==w_mg_section_oppos_bunch)
				&&(w_mg_vert_oppos>=1)&&
				(w_mg_vert_oppos<=2))||((w_mg_section_oppos2>=0)&&
				(w_mg_vert_oppos2>=1)&&(w_mg_vert_oppos2<=2)))
			{//vert
				if(left_right)
				 {
	if(first_green_adjacent<max_n_green_adj_section_bunch)
	{//fga<
		if(!UpperGreenBoundaryRight)
		{//ugr=0
			if(count_fiber<=NumStrips/3)
			{
			if((section_sat>=5)&&(section_gray_zone<=3)&&(section_gray_zone>=1))
			{
				if(non_green_length>=3*DimX/4)
				{
					if((dist_to_wmg>DimX/2)&&(w_mng_section_weight>DimX/2))
					{
	if(!RightGreenBoundaryBunch[count_fiber])
	{
			condition_number=216;
		    success_right=1;
			new_boundary_section=section_number;
			goto L;
	}
			else
			{
				goto L;
			}
					}
				}
			}
			}//cf<
			else
			{
				if((w_mng_section_incline<0)||(w_mng_incl_curve<0))
				{
					if((w_mng_section_weight>DimX/2)&&(non_green_length>=DimX/4))
					{
						if(section_weight>=DimX/4)
						{
	if(!RightGreenBoundaryBunch[count_fiber])
	{
			condition_number=220;
		    success_right=1;
			new_boundary_section=section_number;
			goto L;
	}
			else
			{
				goto L;
			}
						}

					}
				}
			}
		}//ugr=0
		if((w_mng_section_weight>DimX)||(non_green_length>=DimX/2))
		{//wmngw
			if(section_weight>DimX)
			{//sw
	if(!RightGreenBoundaryBunch[count_fiber])
	{
			condition_number=214;
		    success_right=1;
			new_boundary_section=section_number;
			goto L;
	}
			else
			{
				goto L;
			}
			}//sw
		}//wmngw
	}//fga<
				}//lr
				else
				{//elr
	if(first_green_adjacent>max_n_green_adj_section_bunch)
	{//fga>
	if((!UpperGreenBoundaryRight)&&(count_fiber<=NumStrips/3))
		{//ugr=0
			if((section_sat>=5)&&(section_gray_zone<=3)&&(section_gray_zone>=1))
			{
				if(non_green_length>=3*DimX/4)
				{
					if((dist_to_wmg>DimX/2)&&(w_mng_section_weight>DimX/2))
					{
	if(!LeftGreenBoundaryBunch[count_fiber])
	{
			condition_number=191;
		    success_left=1;
			new_boundary_section=section_number;
			goto L;
	}
			else
			{
				goto L;
			}
					}
				}
			}
		}//ugr=0
		if((w_mng_section_weight>DimX)||(non_green_length>=DimX/2))
		{//wmngw
			if(section_weight>DimX)
			{//sw
	if(!LeftGreenBoundaryBunch[count_fiber])
	{
			condition_number=189;
		    success_left=1;
			new_boundary_section=section_number;
			goto L;
	}
			else
			{
				goto L;
			}
			}//sw
		}//wmngw
	}//fga>
				}//elr
			}//vert
		}//last_cor24.04.17
		if((w_mng_section_weight>DimX)&&(w_mg_section_weight>DimX))
		{
			if(left_right)
			{
	if(((w_mng_section_incline<0)||(w_mng_incl_curve<0))||((w_mng_section_oppos_incline>0)||
	(w_mng_incl_curve_oppos>0)))
		{
	if((first_green_adjacent<max_n_green_adj_section_bunch)&&
	   ((w_mg_section_incline>0)&&(w_mg_incl_curve>0)))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
			{
			condition_number=202;
		    success_right=1;
			new_boundary_section=section_number;
			goto L;
			}
			else
			{
				goto L;
			}
	}
		}
			}
			else
			{
	if(((w_mng_section_incline>0)||(w_mng_incl_curve>0))||((w_mng_section_oppos_incline<0)||
	(w_mng_incl_curve_oppos<0)))
		{
	if((first_green_adjacent>max_n_green_adj_section_bunch)&&
	   ((w_mg_section_incline<0)&&(w_mg_incl_curve<0)))
	{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=187;
	new_boundary_section=section_number;
	success_left=1;
			goto L;
			}
			else
			{
				goto L;
			}
	}
		}
			}
		}
	}
	}//last_cor31.03.17
	if(((w_mng_length_oppos>=DimX/2)||(w_mng_length>=DimX/2)||(mng_length>=DimX/2))&&
	   (((success_inverse==-2)||(entrance_to_oppos_border_now==1))
	   ||((4*non_green_length_oppos<non_green_length))))
	{//si=-2
	            if(left_right)
				{//lr
			if(first_green_adjacent<0)
			{
	if(sc_gr_comp_red==2)
	{
		if(((section_weight>=DimX/2)&&((w_mng_section_weight>=2*DimX)||
	(w_mng_section_weight_oppos>=2*DimX)))
			||((section_weight>=2*DimX)&&((sect_incline<0)||(sect_incl_curve<0)))||
			(section_weight>=4*DimX))
		{
			if(!RightGreenBoundaryBunch[count_fiber])
			{
			condition_number=402;
		    success_right=1;
			new_boundary_section=section_number;
			goto L;
			}
			else
			{
				goto L;
			}
		}
	}
			}
	            }//lr
				else
				{//elr
	if(first_green_adjacent<0)
		{//fga
	if(sc_gr_comp_red==2)
	{
		if(((section_weight>=DimX/2)&&((w_mng_section_weight>=2*DimX)||
	(w_mng_section_weight_oppos>=2*DimX)))
			||((section_weight>=2*DimX)&&((sect_incline>0)||(sect_incl_curve>0)))||
			(section_weight>=4*DimX))
		{
			if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=501;
	new_boundary_section=section_number;
	success_left=1;
			goto L;
			}
			else
			{
				goto L;
			}
		}
	}
	}//fga
				}//elr
	}//si=-2
	//last_cor05.12.16
		if(count_fiber>section_beg)
		{//cf>
	section_bunch_previous=ColorDescrSect[section_number].location_of_section[(count_fiber-1)];
	number_of_bunches_prev=ColorInt[(count_fiber-1)].RefinedNumberOfBunches;
	  if(section_bunch_previous>0)
	  {//sbp
	 section_bunch_previous-=1;
	  }
	  else
	  {
	  section_bunch_previous=-1;
	  }
	 if(section_bunch_previous>=number_of_bunches_prev)
	 {
		 prior=-2;
		 goto M;
	 }
	bunch_old_number_previous=ColorInt[(count_fiber-1)].old_bunch_number[section_bunch_previous];
	beg_previous=
	ColorInt[(count_fiber-1)].ColoredIntervalsStructure->BegInterv[bunch_old_number_previous];
	end_previous=
	ColorInt[(count_fiber-1)].ColoredIntervalsStructure->EndInterv[bunch_old_number_previous];
	if(left_right)
	{//lr
	differ_prev=beg_int-beg_previous;
	}//lr
	else
	{//elr
	differ_prev=end_int-end_previous;
	}//elr
	    if(count_fiber>(section_beg+1))
		{//cf+>
	section_bunch_previous1=ColorDescrSect[section_number].location_of_section[(count_fiber-2)];
	number_of_bunches_prev1=ColorInt[(count_fiber-2)].RefinedNumberOfBunches;
	  if(section_bunch_previous1>0)
	  {//sbp
	 section_bunch_previous1-=1;
	  }
	  else
	  {
	  section_bunch_previous1=-1;
	  }
	 if(section_bunch_previous1>=number_of_bunches_prev1)
	 {
		 prior=-2;
		 goto M;
	 }
	bunch_old_number_previous1=ColorInt[(count_fiber-2)].old_bunch_number[section_bunch_previous1];
	beg_previous1=
	ColorInt[(count_fiber-2)].ColoredIntervalsStructure->BegInterv[bunch_old_number_previous1];
	end_previous1=
	ColorInt[(count_fiber-2)].ColoredIntervalsStructure->EndInterv[bunch_old_number_previous1];
	if(left_right)
	{//lr
	differ_prev1=beg_previous-beg_previous1;
	}//lr
	else
	{//elr
	differ_prev1=end_previous-end_previous1;
	}//elr
	    }//cf+>

		}//cf>
		if(count_fiber<section_end)
		{//cf<
	section_bunch_next=ColorDescrSect[section_number].location_of_section[(count_fiber+1)];
	number_of_bunches_next=ColorInt[(count_fiber+1)].RefinedNumberOfBunches;
	if(section_bunch_next>0)
	  {//sbp
	 section_bunch_next-=1;
	  }
	else
	{
		section_bunch_next=-1;
	}
	 if(section_bunch_next>=number_of_bunches_next)
	 {
		 prior=-2;
		 goto M;
	 }
	bunch_old_number_next=ColorInt[(count_fiber+1)].old_bunch_number[section_bunch_next];
	beg_next=
	ColorInt[(count_fiber+1)].ColoredIntervalsStructure->BegInterv[bunch_old_number_next];
	end_next=
	ColorInt[(count_fiber+1)].ColoredIntervalsStructure->EndInterv[bunch_old_number_next];
	if(left_right)
	{//lr
	differ_next=beg_next-beg_int;
	}//lr
	else
	{//elr
	differ_next=end_next-end_int;
	}//elr
		}//cf<
	if((count_fiber>section_beg)&&(count_fiber<section_end))
	{//cf><
	if(left_right)
	{//lr
	fiber_residual=abs(beg_int-beg_next)+abs(beg_int-beg_previous);
	side_fiber_residual=abs((beg_previous-beg_next));
	convex_dif=side_fiber_residual/2-(beg_int-std::min(beg_next,beg_previous));
	}
	else
	{
	fiber_residual=abs(end_int-end_next)+abs(end_int-end_previous);
	side_fiber_residual=abs((end_previous-end_next));
	convex_dif=side_fiber_residual/2-(end_int-std::min(end_next,end_previous));
	}
	abs_convex_dif=abs(convex_dif);
	}//cf><
		if((bunch_adjacent_ordered>=0)&&(bunch_adjacent_ordered==max_n_green_adj_section_bunch))
		{//bao=mngasb
	if((previous_mnga>=0)&&(max_n_green_adj_section==previous_mnga))
	{//pmga=mngas
		if(left_right)
		{//lr
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=8;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}//lr
		else
		{//elr
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=9;
	new_boundary_section=section_number;
	success_left=1;
	goto L;
			}
	else
			{
				goto L;
			}
		}//elr
	}//pmga=mngas
		}//bao=mngasb
	if((previous_success==(count_fiber-1))&&(previous_success>=0))
	 {//prsuc
	 if((first_green_adjacent<0)&&(bunch_adjacent_ordered>=0))
					{//fga<0
			if((max_n_green_adj_section==previous_mnga)&&(previous_mnga>=0))
			{//mga=pm
		if(left_right)
	    {//lr
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=118;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}//lr
		else
		{//elr
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=119;
	new_boundary_section=section_number;
	success_left=1;
	goto L;
			}
	else
			{
				goto L;
			}
		}//lr
			 }//mga=pm
			else
			{//emga=pm

				if((differ_boundary_abs<=7*StripWidth)&&(differ_boundary_abs>=0))
				{//<=7
	if((max_adj_sec>=0)&&(max_adj_sec_weight>=4*DimX)&&(non_green_length>=DimX/6))
		    {//majs1

	if(left_right)
	    {//lr
		if((differ_boundary_sign<0)&&(differ_boundary_abs>=0))
		{
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=188;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}
		}//lr
		else
		{//elr
		if(differ_boundary_sign>0)
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=171;
	new_boundary_section=section_number;
	success_left=1;
	goto L;
			}
	else
			{
				goto L;
			}
		}
		}//lr
	     }//majs1
				}//<=7
			}//emga=pm
	     if((section_weight>=1000)||((section_weight>=400)&&(sc_gr_comp_red==2)&&(sect_incline)))
		 {//weigt_cond

			 if((closeness_to_boundary+number_of_boundary_points>=array_num/2)
				 &&((closeness_to_boundary>0)||(number_of_boundary_points>0)))
			 {//cl_nb_cond
				 if((fiber_residual>=0)&&(abs_convex_dif<=2*StripWidth))
				 {//fr>=0
	if((max_adj_sec>=0)||(max_n_green_adj_section_bunch>=0))
	{//mas>=
		if((max_adj_sec_weight>=2500)||(w_mng_section_weight>=2500)
			||((boundary_bunch>=0)&&(differ_boundary_abs<5*StripWidth)))
		{//w_cond_max
	      if(!left_right)
	{//lr
		if((sect_incline>0)||(sect_incl_curve>0)||(differ_boundary_sign>0))
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=11;
	new_boundary_section=section_number;
	success_left=1;
	goto L;
			}
	else
			{
				goto L;
			}
		}
	}//lr
	else
	{//elr
		if((sect_incline<0)||(sect_incl_curve<0)||
			((differ_boundary_sign<0)&&(differ_boundary_abs>=0)))
		{
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=10;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}
	}//elr
		}//w_cond_max
	}//mas>=
				 }//fr>=0
			 }//cl_nb_cond
		 }//weigt_cond
					}//fga<0
	                else
	                {//fga>=0
		if((max_n_green_adj_section<0)&&(w_mng_section_oppos<0))
			{
						if(left_right)
						{//lr
	if((first_green_adjacent<maximum_non_green_adjacent)&&(non_green_length>DimX/6))
	{
		if((boundary_bunch>=0)&&(differ_boundary_sign<0)&&(differ_boundary_abs<=4*StripWidth))
		{
	if((section_weight>=1000)&&(sc_gr_comp_red==2)&&(max_adj_sec>=0)&&(max_adj_sec_weight>=2500))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=12;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
	}
		}
	if((sect_incline<0)||(sect_incl_curve<0))
		{
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=14;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}
	}
						}//lr
						else
						{//elr
	if((first_green_adjacent>maximum_non_green_adjacent)&&(non_green_length>DimX/6))
	{
		if((boundary_bunch>=0)&&(differ_boundary_sign>0)&&(differ_boundary_abs<=4*StripWidth))
		{
	if((section_weight>=1000)&&(sc_gr_comp_red==2)&&(max_adj_sec>=0)&&(max_adj_sec_weight>=2500))
	{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=13;
	new_boundary_section=section_number;
	success_left=1;
	goto L;
			}
	else
			{
				goto L;
			}
	}
		}
	if((sect_incline>0)||(sect_incl_curve>0))
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=15;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}
	}
						}//elr
		}
	                }//fga>=0
	}//prsuc
	if(success_oppos==3)
	{
		if(first_green_adjacent>max_n_green_adj_section_bunch)
		{//last_cor14.02.17
			if(non_green_length>=DimX/4)
			{
	if((max_green_adj_section>=0)&&(right_oppos_green_max==max_green_adj_section)&&
	   (sc_gr_comp_red==2))
	{
	if(!LeftGreenBoundaryBunch[count_fiber])
				{
	condition_number=169;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
				}
	else
			{
				goto L;
			}
	}
			}
		}//last_cor14.02.17
		if((first_green_adjacent_oppos==oppos_adjacent)&&(first_green_adjacent_oppos>=0))
	{
		if((max_n_green_adj_section>=0)&&(first_green_adjacent>max_n_green_adj_section_bunch))
		{//fga>mngaslast_cor12.01.16
				if((green_adjacent_section_op>=0)&&(number_of_boundary_points_adjacent_oppos>0))
				{
	if(!LeftGreenBoundaryBunch[count_fiber])
				{
	condition_number=17;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
				}
	else
			{
				goto L;
			}
				}
		}//fga>mngas
		else
		{//efga>mngas
			if((last_gr_in_chain==first_green_adjacent)&&(first_green_adjacent>=0))
			{
				if(((max_n_green_adj_section_bunch-1)==first_green_adjacent)||
					(((first_n_green-1)==first_green_adjacent)&&
					((max_n_green_adj_section_bunch-1)==first_n_green))||
	((abs(dist1gr_to_wmng)<=2*StripWidth)&&(boundary_bunch>=0)&&(differ_boundary_abs<=4*StripWidth)))
				{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=19;
	success_left=2;
	goto L;
	        }
	else
			{
				goto L;
			}
				}
			}
		}//efga>mngas
	}
	}
	          if(first_green_adjacent>=0)
			   {//fga>0
	      if(fiber_residual>=0)
		  {//fr
	if(left_right)
	{//lr
	fiber_residual_new=abs(old_first_beg-beg_next)+abs(old_first_beg-beg_previous+1);
	convex_dif_new=side_fiber_residual/2-(old_first_beg-std::min(beg_next,beg_previous));
	}
	else
	{
	fiber_residual_new=abs(old_first_end-end_next)+abs(old_first_end-end_previous+1);
	convex_dif_new=abs(side_fiber_residual/2-(old_first_end-std::min(end_next,end_previous)));
	}
	abs_convex_dif_new=abs(convex_dif_new);
	residual_ratio1=(16*(side_fiber_residual+1))/(side_fiber_residual+fiber_residual+1);
	residual_ratio2=(16*(fiber_residual_new+1))/(fiber_residual_new+fiber_residual+1);
	residual_ratio3=(16*(fiber_residual_new+1))/(fiber_residual_new+side_fiber_residual+1);
	residual_ratio10=(16*(abs_convex_dif_new+1))/(abs_convex_dif_new+abs_convex_dif+1);
	residual_ratio20=(16*(fiber_residual+1))/(fiber_residual_new+fiber_residual+1);
	residual_ratio30=(16*(fiber_residual_new+1))/(fiber_residual_new+side_fiber_residual+1);
	if((last_gr_in_chain>=0)&&(last_gr_in_chain!=first_green_adjacent))
	{
	if(left_right)
	{//lr
	fiber_residual_new_lg=abs(old_first_beg_lg-beg_next)+abs(old_first_beg_lg-beg_previous+1);
	convex_dif_new_lg=side_fiber_residual/2-(old_first_beg_lg-std::min(beg_next,beg_previous));
	}//lr
	else
	{
	fiber_residual_new_lg=abs(old_first_end_lg-end_next)+abs(old_first_end_lg-end_previous+1);
	convex_dif_new_lg=abs(side_fiber_residual/2-(old_first_end_lg-std::min(end_next,end_previous)));
	}
	residual_ratio2_lg=(16*(fiber_residual_new_lg+1))/(fiber_residual_new_lg+fiber_residual+1);
	residual_ratio3_lg=
	(16*(fiber_residual_new_lg+1))/(fiber_residual_new_lg+side_fiber_residual+1);
	abs_convex_dif_new_lg=abs(convex_dif_new_lg);
	residual_ratio10_lg=(16*(abs_convex_dif_new_lg+1))/(abs_convex_dif_new_lg+abs_convex_dif_new+1);

	}


	      }//fr
		  if(boundary_bunch>=0)
		  {//bb>=0
			if((boundary_bunch==section_boun_prev_bunch)&&(section_boun_prev==section_number))
			{//bb=sblast_cor31.03.17
	if((abs_convex_dif<=StripWidth)&&(abs_convex_dif_new>abs_convex_dif)&&
	   (residual_ratio1>=7)&&(residual_ratio1<=9)&&(residual_ratio10>10))
	{//ratio
	if(left_right)
	{//lr
			if((sect_incline<0)&&(sect_incl_curve<0)&&(previous_mnga==max_n_green_adj_section))
			{//last_cor19.04.17
	if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=172;
	success_right=1;//last_cor26.01.17
	goto L;
	             }
	else
			{
				goto L;
			}
			}
	}//lr
	else
	{//elr
	if((sect_incline>0)&&(sect_incl_curve>0)&&(max_n_green_adj_section>=0)
	   &&(first_green_adjacent>max_n_green_adj_section_bunch))
		//if((sect_incline>0)&&(sect_incl_curve>0))
			{
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=167;
	success_left=1;//last_cor26.01.17change
	goto L;
	             }
	else
			{
				goto L;
			}
			}
	}//elr
	}//ratio
			}
			if(first_green_adjacent==bunch_adjacent_ordered)
			{//fga=bao//last_cor15.12.16!!!!serious error
				if(differ_boundary_abs<=5*StripWidth)
				{//dba<=5

	if((w_mng_section_weight_oppos>=2*DimX)||(w_mng_section_weight>=2*DimX)||
	   (max_n_green_adj_section==previous_mnga))
	{
	if(left_right)
	{//lr
		if(((max_n_green_adj_section>=0)&&(first_green_adjacent<max_n_green_adj_section_bunch))||
	((w_mng_section_oppos>=0)&&(first_green_adjacent<w_mng_section_oppos_bunch)))
		{//last_cor27.02.17
		   if(((differ_boundary_sign<0)||(differ_boundary_abs<=2*StripWidth))
			   &&(differ_boundary_abs>=0))
		   {
	if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=138;
	success_right=1;//last_cor14.12.16serious error!
	goto L;
	             }
	else
			{
				goto L;
			}

		   }
		}
	}//lr
	else
	{//elr
	if(((max_n_green_adj_section>=0)&&(first_green_adjacent>max_n_green_adj_section_bunch))||
	((w_mng_section_oppos>=0)&&(first_green_adjacent>w_mng_section_oppos_bunch)))
	{
	   if((differ_boundary_sign>0)||(differ_boundary_abs<=2*StripWidth))
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=133;
	success_left=1;//last_cor14.12.16serious error!27.02.17
	goto L;
				 }
	else
			{
				goto L;
			}
	   }
	}
	}//elr
	}
				}//dba<=5

			//if(first_green_adjacent==bunch_adjacent_ordered)
			//{//fga>=0//last_cor15.12.16
	if(abs(differ_bb_fga)<=5*StripWidth)
	{//dbf<5S
		if(bunch_adjacent_ordered==first_green_adjacent)
		{//bao=fga
			if(((w_mng_section_weight_oppos>=2*DimX)||(w_mng_section_weight>=2*DimX))||
	((max_adj_sec>=0)&&(max_adj_sec_weight>=4*DimX)&&(non_green_length>=DimX/6)))
		    {//majs
	if(last_gr_in_chain!=first_green_adjacent)
	{//lgic!=
	            if(left_right)
				{//lr
		if((differ_bb_fga>0)||(abs(differ_bb_fga)<=2*StripWidth))
		{
	             if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=134;
	success_right=3;
	goto L;
	             }
				 else
			{
				goto L;
			}
		}
				}//lr
				else
				{//elr
	    if((differ_bb_fga<0)||(abs(differ_bb_fga)<=2*StripWidth))
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=127;
	success_left=3;
	goto L;
				 }
	else
			{
				goto L;
			}
	    }
	            }//elr

	}//lgic!=
	else
	{//elgic!=
	if(green_adjacent_section>=0)
			{
	        new_boundary_section=green_adjacent_section;
			}
			else
			{
			new_boundary_section=section_number;
			}
	if(left_right)
				{//lr

		if((differ_bb_fga>0)||(abs(differ_bb_fga)<=2*StripWidth))
		{
	             if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=136;
	success_right=2;
	goto L;
	             }
		}
				}//lr
				else
				{//elr
	if((differ_bb_fga<0)||(abs(differ_bb_fga)<=2*StripWidth))
		{//last_cor11.11.16
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=129;
	success_left=2;
	goto L;
				 }
	else
			{
				goto L;
			}
	    }//last_cor11.11.16
	            }//elr
	new_boundary_section=-1;
	}//elgic!=
			}//majs
	        if(section_boun_prev==section_number)
			{//sbp=snlast_cor17.11.16
	if(last_gr_in_chain==first_green_adjacent)
	{//lg=fga
	if((max_adj_sec>=0)&&(max_adj_sec_weight>=(3*DimX/2))&&(non_green_length>=DimX/3))
	{//mwngl
	            if(left_right)
				{//lr
	if(((differ_bb_fga>0)&&(abs(differ_bb_fga)<=5*StripWidth))||
			((abs(differ_bb_fga)<=2*StripWidth)))
	{//dbbfg
	if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=144;
	success_right=2;
	goto L;
	             }
	else
			{
				goto L;
			}
	}//dbbfg
	            }//lr
				else
				{//elr
	if(((differ_bb_fga<0)&&(abs(differ_bb_fga)<=5*StripWidth))||
			((abs(differ_bb_fga)<=2*StripWidth)))
	{
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=149;
	success_left=2;
	goto L;
	             }
	else
			{
				goto L;
			}
	}
	               }//elr
	}//mwngl
	}//lg=fga
	        }//sbp=snast_cor17.11.16
		}//bao=fga
	}//dbf<5S
	}//fga>=0

	if(success_oppos==3)
	{//last_cor11.11.16so=3
		    if(section_boun_prev==section_number)
			{//sbp=sn
	       if((differ_boundary_sign>0)&&(differ_boundary_abs<=5*StripWidth))
			{
	if((max_n_green_adj_section>=0)&&(max_n_green_adj_section_bunch<first_green_adjacent))
	   {//last_cor12.01.17
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=159;
	success_left=1;
	goto L;
	}
	else
			{
				goto L;
			}
		}
	        }
	if(((differ_bb_fga<0)&&(abs(differ_bb_fga)<=5*StripWidth))||
			((abs(differ_bb_fga)<=2*StripWidth)))
	{//conddistbb
	  if(max_n_green_adj_section_bunch>first_green_adjacent)
	   {
	if((last_gr_in_chain>=0)&&((max_n_green_adj_section_bunch==(last_gr_in_chain+1))||
			(max_n_green_adj_section_bunch==(last_gr_in_chain+2))))
	{
	      if(first_green_adjacent==last_gr_in_chain)
	      {
	if(!LeftGreenBoundaryBunch[count_fiber])
	{
	      condition_number=161;
	      success_left=2;
	      goto L;
	}
	else
			{
				goto L;
			}
	      }
		  else
		  {
	if(!LeftGreenBoundaryBunch[count_fiber])
	{
		  condition_number=163;
	      success_left=3;
	      goto L;
	}
	else
			{
				goto L;
			}
		  }
	}
	   }
	}//conddistbb
	//else
	//{//econdbb
	if((differ_bb_fga<0)&&(abs(differ_bb_fga)<5*StripWidth))
	{//last_cor22.11.1627.02.17><error was corrected
	if((differ_boundary_sign>0)&&(differ_boundary_abs<=4*StripWidth))
			{
				if((bunch_adjacent_ordered==first_green_adjacent)&&(bunch_adjacent_ordered>=0))
				{
	if((bunch_adjacent_ordered_old_sat<=3)&&(bunch_adjacent_ordered_old_gray_zone<=2)&&
	   (bunch_sat>4))
				{
	if(!LeftGreenBoundaryBunch[count_fiber])
	{
		  condition_number=165;
	      success_left=1;
	      goto L;
	}
	else
			{
				goto L;
			}
				}
				}
	        }
	}//last_cor22.11.16
	//}//econdbb
		     }//sbp=sn
	}//last_cor11.11.16so=3
		if((max_n_green_adj_section==previous_mnga)&&(previous_mnga>=0))
		{//so3mnga=pmngalast_cor11.11.16
			if(green_adjacent_section>=0)
			{
	        new_boundary_section=green_adjacent_section;
			}
			else
			{
			new_boundary_section=section_number;
			}
			if(section_boun_prev==section_number)
			{//sbp=sn

	if(left_right)
				{//lr
	if((max_n_green_adj_section_bunch==(first_green_adjacent-1))||
	   ((last_gr_in_chain==first_green_adjacent)&&(abs(dist1gr_to_wmng)<=2*StripWidth)))
	{
	if(((differ_bb_fga>0)&&(abs(differ_bb_fga)<=5*StripWidth))||(abs(differ_bb_fga)<=2*StripWidth))
	{//last_cor04.04.17
	             if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=132;
	success_right=2;
	goto L;
	             }
				 else
			{
				goto L;
			}
	}//last_cor04.04.17
	}
				}//lr
				else
				{//elr
	if((max_n_green_adj_section_bunch==(first_green_adjacent+1))||
	   ((last_gr_in_chain==first_green_adjacent)&&(abs(dist1gr_to_wmng)<=2*StripWidth)))
	{
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=125;
	success_left=2;
	goto L;
				 }
	else
			{
				goto L;
			}
	}
	            }//elr
			}//sbp=sn
	else
	{//esbp=sn
	            if(left_right)
				{//lr
	if((max_n_green_adj_section_bunch==(first_green_adjacent-1))||
	   ((last_gr_in_chain==first_green_adjacent)&&(abs(dist1gr_to_wmng)<=2*StripWidth)))
	{//conddist=
	if(((abs(differ_bb_fga)<=5*StripWidth)&&(differ_bb_fga>0))
	   ||(abs(differ_bb_fga)<=2*StripWidth))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=120;
	success_right=2;
	goto L;
				 }
	else
			{
				goto L;
			}
	}
	}//conddist=
				}//lr
				else
				{//elr
	if((max_n_green_adj_section_bunch==(first_green_adjacent-1))||
	   ((last_gr_in_chain==first_green_adjacent)&&(abs(dist1gr_to_wmng)<=2*StripWidth)))
	{//conddist=1
		if(((abs(differ_bb_fga)<=5*StripWidth)&&(differ_bb_fga<0))
	   ||(abs(differ_bb_fga)<=2*StripWidth))
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=121;
	success_left=2;
	goto L;
				 }
	else
			{
				goto L;
			}
		}
	}//conddist=1
				}//elr
			}//esbp=sn
		}//so3mnga=pmngalast_cor11.11.16
		}//bb>=0
	if(boundary_bunch<0)
	{//bb<0
	if(left_right)
			   {//lr
				   if((UpperGreenBoundaryRight>0)&&(UpperGreenBoundaryRight<=2*NumStrips/3))
				   {
	if((((count_fiber-UpperGreenBoundaryRight)<=3)&&
	(((differ_boundary_sign_prev<0)&&(differ_boundary_abs_prev<=7*StripWidth))||
	 (differ_boundary_abs_prev<=2*StripWidth)))&&(differ_boundary_abs>=0))
	{//last_cor21.12.16
		if((bunch_adjacent_ordered==max_n_green_adj_section_bunch)||
	((max_n_green_adj_section_bunch>=0)&&(first_green_adjacent<max_n_green_adj_section_bunch)))
		{
			if(((prev_sec_ng_max>=0)&&(prev_sec_ng_max==max_n_green_adj_section))||
				(((section_weight>DimX)||(sect_incl_curve<0)||(sect_incline<0))
				&&(w_mng_section_weight>DimX)))
			{
	if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=130;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
				 }
	else
			{
				goto L;
			}
			}
		}
	}
				   }
			   }//lr
			   else
			   {//elr
			   }//elr
	}//bb<0

		  if(w_mng_section_oppos_bunch>=0)
		  {
	            if(left_right)
				{//lr
	if(first_green_adjacent<w_mng_section_oppos_bunch)
	{//fga<

	 if(w_mng_section_oppos_incline>0)
	{
		if(w_mng_section_weight_oppos>=DimX/2)
		{
			if((section_weight>=100)&&(sc_gr_comp_red==2))
			{
	if((number_bound_points>0)&&((differ_boundary_sign<0)&&(differ_boundary_abs<=3*StripWidth)
	   &&(differ_boundary_abs>=0)))
	{
				if((non_green_length>=DimX/3))
				{
	if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=18;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
				 }
	else
			{
				goto L;
			}
				}
	}
			}
		}
	}
	}//fga<
				}//lr
		  }

		  if(max_n_green_adj_section_bunch>=0)
		  {//mngab>=0
	            if(left_right)
				{//lr
	if((first_green_adjacent<max_n_green_adj_section_bunch))
	{//fga<
	//efgalast_cor13.03.17
	if((first_green_adjacent==max_green_adj_section_bunch)&&(count_fiber<NumStrips/4))
	{
		if(sc_gr_comp_red==2)
	    {
	if((section_weight>=DimX/2)&&(w_mg_section_weight>=2*DimX))
	{
		if((w_mng_incl_curve<0)&&(w_mng_section_weight>DimX/2))
		{
			if(non_green_length>=DimX/2)
			{
				if((boundary_bunch<0)&&(!UpperGreenBoundaryRight))
				{
	condition_number=198;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
				}
			}
		}
	}
		}
	}
	//efga
		if(count_fiber<=NumStrips/6)
		{//cf<1/6last_cor15.11.16
	if(non_green_length>DimX/2)
	{
	if(sc_gr_comp_red==2)
				{
			if(max_n_green_adj_section_oppos<0)
			{
	if((w_mng_incl_curve<0)||(w_mng_section_incline<0)||(sect_incl_curve<0)||(sect_incline<0))
	{
	condition_number=140;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
	}
			}
				}
	}
			if((w_mng_section_weight>=2*DimX)&&(w_mng_section_weight_normalized>=DimX/2))
			{
				if(sc_gr_comp_red==2)
				{
	if(max_n_green_adj_section_oppos<0)
	{
	condition_number=142;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
	}
				}
			}
		}//cf<1/6
		if((w_mg_section_weight>=4*DimX)&&((w_mng_incl_curve<0)||(w_mng_section_incline<0))&&
			(w_mg_section_first<=NumStrips/3)&&(count_fiber<=NumStrips/2))
		{//wwclast_cor09.11.16!!!15.11.16error_onlyub18.0517
			if((boundary_bunch>=0)&&(ratio_length_green>=8)&&(ratio_length1_green>=12)
					&&(Intersection_green>=0)&&(Intersection_green<3))
			{//last_cor30.03.17
				if(max_green_adj_section==green_oppos_boundary_sec)
				{
					if((abs(dist_to_wmng)<=StripWidth)||(first_green_adjacent<max_n_green_adj_section_bunch))
					{
	if(!RightGreenBoundaryBunch[count_fiber])
	{
	condition_number=196;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
	}
					}
				}
			}
	if((sect_incl_curve<0)||(sect_incline<0))
	{
	if(abs(dist_wmng_to_mga)<=3*StripWidth)
	{
		if(w_mng_section_weight_normalized>=DimX/12)
		{
	condition_number=126;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
	}
	}
		}//wwc
	     if(bound_b_existing>=0)
	     {//bb_e>=0
			 if(max_n_green_adj_section_bunch==section_max_existing_bunch)
	         {///mxngcoins
		 if((boundary_bunch>=0)&&(differ_boundary_sign<0)&&(differ_boundary_abs<=4*StripWidth))
				 {
	condition_number=20;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
				 }
	         }///mxngcoins
	     }//bb_e>=0
		 else
		 {//ebb_e>=0
	 /*if((previous_success==(count_fiber-1))&&(previous_success>=0))onlyublast_cor11.05.17
	 {//prsuc1
		 if(previous_mnga==max_n_green_adj_section)
		 {
	condition_number=22;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		 }
		 else
		 {//else
			 if((abs(differ_boundary_sign)<=4*StripWidth)&&(differ_boundary_abs>=0))
			 {//dist
				 if((section_weight>=3*DimX)&&(w_mg_section_weight>=2*DimX))
				 {//wcon
	if(sc_gr_comp_red==2)
		     {
	if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=114;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
				 }
	else
			{
				goto L;
			}
		     }
				 }//wcon
			 }//dist
		 }//else
	 }//prsuc1*/
	 if(number_of_boundary_points>0)
	 {
		 if((section_weight>=3*DimX)&&(w_mg_section_weight>=3*DimX)&&((w_mng_section_weight)>=DimX))
		 {
			 if(bunch_adjacent_ordered==max_n_green_adj_section_bunch)
			 {
				 if((max_n_green_adj_section==max_adj_sec)||(w_mng_section_incline<0)||
	(w_mng_section_oppos_incline>0)||(w_mng_incl_curve_oppos>0)||(w_mng_incl_curve<0))
				 {
	if(sc_gr_comp_red==2)
		     {
	if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=128;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
				 }
	else
			{
				goto L;
			}
		     }
				 }
			 }
		 }
	 }
	if((number_of_boundary_points>=array_num/2)&&(number_of_boundary_points>=3))
	{
	 if((section_weight>=2*DimX)&&(w_mg_section_weight>=2*DimX)&&((w_mng_section_weight)>=2*DimX))
	 {
		 if((w_mng_section_incline<0)||(w_mng_section_oppos_incline>0)||(w_mng_incl_curve_oppos>0)||
	(w_mng_incl_curve<0))
		 {
		     if(sc_gr_comp_red==2)
		     {
	if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=106;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
				 }
	else
			{
				goto L;
			}
		     }
		 }
	 }
	}
		 }//ebb_e>=0
	 if((w_mg_section_incline>0)&&((sect_incline<0)||(sect_incl_curve<0)))
	{
		if((w_mg_section_weight>=2*DimX)||((w_mg_sec_length>=5)&&(w_mg_section_weight>=DimX)))
		{
			if((section_weight>=100)&&(sc_gr_comp_red==2)&&(abs(dist_wmng_to_mga)<=6*StripWidth))
			{
				if((w_mng_section_weight>=DimX/2)&&(max_adj_sec_weight>=2*DimX))
				{
	if(!RightGreenBoundaryBunch[count_fiber])
				 {
	condition_number=24;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
				 }
	else
			{
				goto L;
			}
				}
			}
		}
	}
	}//fga<
				}//lr
				else
				{//elr
	if(first_green_adjacent>max_n_green_adj_section_bunch)
	{//fga>
		if(count_fiber<=NumStrips/6)
		{//cf<1/6
	if(non_green_length>DimX/2)
	{
	if(sc_gr_comp_red==2)
				{
			if(max_n_green_adj_section_oppos<0)
			{
	if((w_mng_incl_curve>0)||(w_mng_section_incline>0)||(sect_incl_curve>0)||(sect_incline>0))
	{//last_cor15.11.16
		if(!LeftGreenBoundaryBunch[count_fiber])
		{//last_cor15.03.17
	condition_number=145;
	success_left=1;
	new_boundary_section=section_number;
		}
		else
		{
			goto L;
		}
	}
			}
				}
	}
			if((w_mng_section_weight>=2*DimX)&&(w_mng_section_weight_normalized>=DimX/2))
			{
				if(sc_gr_comp_red==2)
				{
	if(max_n_green_adj_section_oppos<0)
	{
		if(!LeftGreenBoundaryBunch[count_fiber])
		{//last_cor15.03.17
	condition_number=147;
	success_left=1;
	new_boundary_section=section_number;
		}
		else
		{
			goto L;
		}
	}
				}
			}
		}//cf<1/6
		if(!UpperGreenBoundaryLeft)
		{//last_cor15.03.17
			if((section_weight>DimX/2)&&(sc_gr_comp_red==2))
			{
	if(((max_green_adj_section>=0)&&(w_mg_section_weight>=DimX/2))||
	   ((w_mg_section_oppos>=0)&&(w_mg_section_weight_oppos>=DimX/2)))
	{
					if((max_n_green_adj_section>=0)&&(w_mng_section_weight>=2*DimX))
					{
	if((w_mng_section_incline>0)||((w_mng_incl_curve>=1)&&(w_mng_incl_curve<=2)))
	{
		if((first_green_adjacent>=max_n_green_adj_section_bunch)&&(dist_next_green>=DimX/3))
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
		{//last_cor15.03.17
	condition_number=179;
	success_left=1;
	new_boundary_section=section_number;
		}
	else
	{
		goto L;
	}
		}
	}
					}
	}
			}
		}//last_cor15.03.17
	if((w_mg_section_weight>=4*DimX)&&((w_mng_incl_curve>0)||(w_mng_section_incline>0))&&
			(w_mg_section_first<=NumStrips/3)&&(count_fiber<=NumStrips/2))
		{//wwclast_cor09.11.16!!!15.11.16
	if((sect_incl_curve>0)||(sect_incline>0))
	{
	if(abs(dist_wmng_to_mga)<=3*StripWidth)
	{
		if(w_mng_section_weight_normalized>=DimX/12)
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
		{//last_cor15.03.17
	condition_number=123;
	success_left=1;
	new_boundary_section=section_number;
		}
	else
	{
		goto L;
	}
		}
	}
	}
		}//wwc
		if((number_of_boun_points_in_max_adj>0)&&(boundary_bunch>=0))
		{//last_cor04.05.17
			if((max_n_green_adj_section_oppos>0)||(max_n_green_adj_section>=0))
			{
				if(((sect_incl_curve>0)&&(sect_incl_curve<=2))||((sect_incline>0)&&(sect_incl_curve>0)))
				{
	if(((section_hue_zone==3)&&(section_sat>=7)&&(section_hue<=20))||(sc_gr_comp_red==2))
	{
		if((w_mng_section_weight>=100)||(w_mng_section_weight_oppos>=100))
		{
	if((Intersection_green>=0)&&(Intersection_green<=1))
	{
		if(((differ_boundary_sign>0)&&(differ_boundary_abs<=5*StripWidth))||
	   (differ_boundary_abs<=2*StripWidth))
	  {
		if(!LeftGreenBoundaryBunch[count_fiber])
		{
	condition_number=23;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
		}
	}
		}

	}
				}
			}
		}
		if(bound_b_existing>=0)
	     {//bb_e>=0
			 if(max_n_green_adj_section_bunch==section_max_existing_bunch)
	         {///mxngcoins
	condition_number=25;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
	         }///mxngcoins
	     }//bb_e>=0
		else
		 {//ebb_e>=0
	if((previous_success==(count_fiber-1))&&(previous_success>=0))
	 {//prsuc2
		 if(previous_mnga==max_n_green_adj_section)
		 {
	condition_number=27;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
		 }
		 else
		 {//else
			 if((abs(differ_boundary_sign)<=4*StripWidth)&&(differ_boundary_abs>=0))
			 {//dist
				 if((section_weight>=3*DimX)&&(w_mg_section_weight>=2*DimX))
				 {//wcon
	if(sc_gr_comp_red==2)
		     {
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=117;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
				 }
	else
			{
				goto L;
			}
		     }
				 }//wcon
			 }//dist
		 }//else
	 }//prsuc2
	if(success_oppos==3)
	{//so3
	if((number_of_boundary_points>=array_num/2)&&(number_of_boundary_points>0))
	{//nbp
	if((section_weight>=DimX))
	 {//sw>=
		 if((w_mng_section_incline>0)||(w_mng_section_oppos_incline<0)||(w_mng_incl_curve_oppos<0)||
	(w_mng_incl_curve>0))
		 {//incl
		     if(sc_gr_comp_red==2)
		     {
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=115;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
				 }
	else
			{
				goto L;
			}
		     }
		 }//incl
	 }//sw>=
	}//nbp
	}//so3
	if((number_of_boun_points_in_max_adj>0)&&(number_of_boun_points_in_max_adj>=w_mg_sec_length/3))
	{
		if((section_weight>=DimX/6)&&(sc_gr_comp_red==2))
		{
			if((w_mng_section_weight>=2*DimX)||
				((w_mng_section_weight>=DimX)&&((w_mng_section_incline>0)||(w_mng_incl_curve>0))))
			{
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=131;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
				 }
	else
			{
				goto L;
			}
			}

		}
	}
	if((number_of_boundary_points>=array_num/2)&&(number_of_boundary_points>=2))
	{
	 if((section_weight>=2*DimX)&&(w_mg_section_weight>=2*DimX)&&(w_mng_section_weight)>=2*DimX)
	 {
		 if((w_mng_section_incline>0)||(w_mng_section_oppos_incline<0)||(w_mng_incl_curve_oppos<0)||
	(w_mng_incl_curve>0))
		 {
		     if(sc_gr_comp_red==2)
		     {
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=113;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
				 }
	else
			{
				goto L;
			}
		     }
		 }
	 }
	}
		 }//ebb_e>=0
	if((w_mg_section_incline<0)&&((sect_incline>0)||(sect_incl_curve>0)))
	{
		if((w_mg_section_weight>=2*DimX)||((w_mg_sec_length>=5)&&(w_mg_section_weight>=DimX)))
		{
			if((section_weight>=100)&&(sc_gr_comp_red==2)&&(abs(dist_wmng_to_mga)<=6*StripWidth))
			{
				if(w_mng_section_weight>=DimX/2)
				{
	if(!LeftGreenBoundaryBunch[count_fiber])
				 {
	condition_number=29;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
				 }
	else
			{
				goto L;
			}
				}
			}
		}
	}
	}//fga>
				}//elr


		  }//mngab>=0
		  //else
		 // {
	if(w_mng_section_oppos>=0)
	{//mngab_op>=0
	            if(left_right)
				{//lr
	if(first_green_adjacent<w_mng_section_oppos_bunch)
	{//fga<
		if(w_mng_section_oppos_incline>0)
		{//mngdi
	if(w_mng_section_sky_oppos!=1)
	{//skop
		if(boundary_bunch>=0)
		{//bb
			if((differ_boundary_sign>0)&&(differ_boundary_abs<5*StripWidth))
			{//dbs
				if((maximum_non_green_adjacent_oppos<0)||(success_inverse<0))
				{//mop
					if(w_mng_section_weight_oppos>=DimX/3)
					{//weight
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=26;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
					}//weight
				}//mop
			}//dbs
		}//bb
	}//skop
		}//mngdi
	}//fga<
				}//lr
				else
				{//elr
	if(first_green_adjacent>max_n_green_adj_section_bunch)
	{//fga>
	    if(w_mng_section_oppos_incline<0)
		{//mngdi
	if(w_mng_section_sky_oppos!=1)
	{//skop
		if(boundary_bunch>=0)
		{//bb
			if((differ_boundary_sign<0)&&(differ_boundary_abs<5*StripWidth))
			{//dbs
				if((maximum_non_green_adjacent_oppos<0)||(success_inverse<0))
				{//mop
					if(w_mng_section_weight_oppos>=DimX/3)
					{//weight
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=33;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
					}//weight
				}//mop
			}//dbs
		}//bb
	}//skop
	    }//mngdi
	}//fga>
				}//elr
		  }//mngab_op>=0
		  //}//emnga>
	if(left_right)
	{//lr

			    if((max_n_green_adj_section_bunch>=0)&&
					(first_green_adjacent<max_n_green_adj_section_bunch))
			    {//mngsb
	if((w_mng_section_weight>=5000)||(w_mng_section_incline<0)||
	   ((w_mg_section_incline>0)&&(w_mg_section_weight>400)))
		{//inc<0
			if(w_mng_section_weight>=3000)
			{//wmngsw
	if(last_max_adj_sec_strip>=section_beg)
	{//lmas>=
		if(first_max_adj_sec_strip<=(NumStrips/6))
		{//fas<=
	if(!RightGreenBoundaryBunch[count_fiber])
		{
	condition_number=28;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
	else
			{
				goto L;
			}
		}//fas<=
	}//lmas>=
			}//wmngsw
			else
			{//ewmngsw
				if((w_mg_section_incline>0)&&(w_mg_incl_curve>=1)&&(w_mg_incl_curve<=3)&&
					(w_mg_section_weight>=DimX/2))
				{//inclw
					if((w_mng_section_incline<0)&&(w_mng_section_weight>=DimX)&&
	((boundary_bunch>=0)&&(differ_boundary_abs<4*StripWidth)&&(abs(dist_to_wmng)<=2*StripWidth)))
					{//bbdb
						if((section_weight>=DimX)&&(sc_gr_comp_red==2))
						{//swgr
	if(!RightGreenBoundaryBunch[count_fiber])
		{
	condition_number=30;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
	else
			{
				goto L;
			}
						}//swgr
					}//bbdb
				}//inclw
			}//ewmngsw
		}//inc<0
				}//mngsb
	if(first_green_adjacent<max_n_green_adj_section_bunch)
	{//fga<max_n_green
		if((w_mng_section_sky!=1)&&(w_mng_section_sky_oppos!=1))
		{//not_sky
		if((sect_incline<0)&&(number_of_boundary_points>0)&&(abs(dist_wmng_to_mga)<=6*StripWidth))
		{//sinbp
			if((section_weight>=1000)&&(max_adj_sec>=0))
			{//swmas
				if((max_adj_sec_weight>=1500)||(w_mng_section_weight_oppos>=2000)
					||(w_mng_section_weight>=2000))
				{//masw>=

	if(last_max_adj_sec_strip>=section_beg)
	{//lmass>=
	if(!RightGreenBoundaryBunch[count_fiber])
		{
	condition_number=32;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
	else
			{
				goto L;
			}
	}//lmass>=
				}//masw>=
			}//swmas
		}//sinbp

		if(((section_weight>=1000)||((sect_incline<0)&&(section_weight>=300)))&&
			((w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000)))
				{//w_cond
		if(!RightGreenBoundaryBunch[count_fiber])
		{
	condition_number=34;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}

				}//w_cond
		}//not_sky
	}//fga<max_n_green
		        }//lr
				else
				{//elr
	if(max_n_green_adj_section_bunch>=0)
	{//mngsb
		if(first_green_adjacent>max_n_green_adj_section_bunch)
		{//fga>max_n_green
			if((w_mng_section_sky!=1)&&(w_mng_section_sky_oppos!=1))
		{//not_sky
	if((w_mng_section_weight_oppos>=5000)||(w_mng_section_weight>=5000)||(w_mng_section_incline>0)||
	   ((w_mg_section_incline<0)&&(w_mg_section_weight>400)))
		{//inc>0
		if((w_mng_section_weight>=3000)&&((section_weight>100)||(abs(dist_to_wmng)<5*StripWidth)))
			{//wmngsw>=
	if((last_max_adj_sec_strip>=section_beg)||(w_mng_section_last>=section_beg))
	{//lmass>=
		if((first_max_adj_sec_strip<=(NumStrips/4))||(w_mng_section_first<=(NumStrips/4))||
			(w_mng_section_first_oppos<=(NumStrips/4)))
		{//fmass<=
			if(sc_gr_comp_red==2)
			{//last_cor14.03.18
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=35;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;

			}
			}
		}//fmass<=
	}//lmass>=
			}//wmngsw>=
		else
			{//ewmngsw
				if((w_mg_section_incline<0)&&(w_mg_incl_curve<=-1)&&(w_mg_incl_curve>=-3)&&
					(w_mg_section_weight>=DimX/2))
				{//inclw
					if((w_mng_section_incline>0)&&(w_mng_section_weight>=DimX)&&
	((boundary_bunch>=0)&&(differ_boundary_abs<4*StripWidth)&&(abs(dist_to_wmng)<=2*StripWidth)))
					{//bbdb
						if((section_weight>=DimX)&&(sc_gr_comp_red==2))
						{//swgr
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=37;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
						}//swgr
					}//bbdb
				}//inclw
			}//ewmngsw
		if((w_mng_section_incline>0)&&(max_n_green_adj_section_bunch==bunch_adjacent_ordered)&&(sc_gr_comp_red==2))
		{
			if(dist_next_green>=DimX/2)
			{
				if((success_inverse==-2)||((maximum_non_green_adjacent_oppos<0)))
				{
					if((section_weight>=400)&&(w_mng_section_weight>=400))
					{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=39;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
					}
				}
			}
		}
		}//inc>0
			}//not sky
	}//fga>max_n_green
				}//mngsb
				}//elr
	if((first_n_green==bunch_adjacent_ordered)&&(bunch_adjacent_ordered>=0))
	{//fng=oa

	    if((max_n_green_adj_section_oppos<0)&&(w_mng_section_oppos1<0))
		{//mngop<0
			if((first_green_adjacent==max_green_adj_section_bunch)||
	(first_green_adjacent==w_mg_section_oppos_bunch))
			{//fg==wmg
	if(dist_to_wmg>3*StripWidth)//last_cor28.02.17
	{//dist
		if((adjacent_boundary+number_of_boundary_points)>=(array_num/2+1))
		{//+
	if((w_mg_section_weight_oppos>=1500)||(w_mg_section_weight>=1500))
	   //(((w_mg_section_oppos_incline>0)&&(w_mg_section_weight_oppos>1000))))
	{//1500
		if((max_adj_sec>=0)&&(max_adj_sec_weight>=3*DimX)&&(differ_boundary_abs>=0)&&
			((previous_mnga==max_adj_sec)||(differ_boundary_abs<=4*StripWidth)))
		{
			if(max_n_green_adj_section>=0)
			{
	            if(left_right)
				{//lr
	if((w_mg_section_incline>0)&&(differ_prev<0)&&(count_fiber>section_beg))
	{

		if(first_green_adjacent<max_green_adj_section_bunch)
		{
		if(!RightGreenBoundaryBunch[count_fiber])
		{//!
	condition_number=36;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}//!
		else
			{
				goto L;
			}
		}
	}
				}//lr
				else
				{//elr
	if((w_mg_section_incline<0)&&(differ_prev>0)&&(count_fiber>section_beg))
	{
		if(first_green_adjacent>max_green_adj_section_bunch)
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
		{
	condition_number=41;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
		}
	else
			{
				goto L;
			}
		}
	}
				}//elr
			}
		}
	}//1500
		}//+

	}//dist
			}//fg==wmg
		}//mngop<0
	 if((previous_success==(count_fiber-1))&&(previous_success>=0))
	 {//prsuc

		if((fiber_residual>=0)&&(side_fiber_residual<=5*StripWidth))
		{//fr>=

	if(section_weight>=300)
		{//sw1
			if(((w_mng_section_oppos>=0)||(max_n_green_adj_section>=0)||
	   (max_adj_sec>=0))&&((w_mng_section_sky_oppos!=1)&&(w_mng_section_sky!=1)))
			   {//cond42
	      if((w_mng_section_weight_oppos>1500)||
	   (max_adj_sec_weight>1500)||(w_mng_section_weight>=1500))
		  {//con_weight1
			  if((sect_incline)&&(abs(dist_wmng_to_mga)<=6*StripWidth))
			  {//sect_incl
	if((abs_convex_dif<StripWidth)||((residual_ratio10>9)&&(abs_convex_dif<2*StripWidth)))
	{
	if((residual_ratio1>=7)&&(residual_ratio1<=8))
	{
		if(((residual_ratio2>8)&&(residual_ratio3>8))||((residual_ratio10>9)&&(residual_ratio2>=8)&&
			(residual_ratio3>=8)))
		{
	if(!left_right)
	{//lr
		if((sect_incline>0)&&(max_n_green_adj_section>=0)&&
			(first_green_adjacent>max_n_green_adj_section_bunch))
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=43;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}
	}//lr
	else
	{//elr
		if((sect_incline<0)&&(max_n_green_adj_section>=0)&&
			(first_green_adjacent<max_n_green_adj_section_bunch))
		{//last_cor28.02.17
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=38;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}
	}//elr
		}
	}
	}
			  }//sect_incl
	if(old_first_n_length<=2*StripWidth)
		{//l<=2
			  if((n_o_g_contrast>=1)&&(n_o_g_contrast<=3))
			  {//contr
	if((residual_ratio1<=6)&&(residual_ratio2<=7))
	{//rr1rr2
				if((residual_ratio10>=0)&&(residual_ratio10<=8)&&(abs_convex_dif_new<abs_convex_dif))
				{//rr10
					if(first_green_adjacent==last_gr_in_chain)
					{
	if(left_right)
	{//lr
		if((first_gr<0)||((first_gr>=0)&&(last_gr==last_gr_in_chain)))
		{//last_cor19.04.17
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=40;
	success_right=2;
	goto L;
			}
	else
			{
				goto L;
			}
		}//last_cor19.04.17
	}//lr
	else
	{//elr
		if((first_gr<0)||((first_gr>=0)&&(last_gr==last_gr_in_chain)))
		{//last_cor19.04.17
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=45;
	success_left=2;
	goto L;
			}
	else
			{
				goto L;
			}
		}//last_cor19.04.17
	}//elr
					}//fga=lgich
				}//rr10
	}//rr1rr2
			  }//contr
	}//l<=2
		  }//con_weight1
			}//cond42
	}//sw1

		}//fr>=
	 }//prsuc
	}//fng=oa
	if(first_green_adjacent==bunch_adjacent_ordered)
	{//fg=oa
		if(green_adjacent_section>=0)
		{//gas>=0
	if((green_adjacent_section_weight_normalized>2*StripWidth)&&(green_adjacent_section_weight>=150))
	{
		if(green_adjacent_section_gray_zone>=2)
		{
	//green_adjacent_section_incline
			goto L;
		}
	}
		}//gas>=0
		else
		{
			if(left_right)
		    {//lr
	if((first_green_adjacent==(max_n_green_adj_section_bunch+1))||
	   ((first_green_adjacent==last_gr_in_chain)&&(abs(dist1gr_to_wmng)<=2*StripWidth)))
	{
		if(boundary_bunch>=0)
		{
	if((differ_bb_fga>0)&&(differ_bb_fga<=6*StripWidth))
	{
		if((max_n_green_adj_section==previous_mnga)&&(previous_mnga>=0))
		{
	if(!RightGreenBoundaryBunch[count_fiber])
		{//!R???
	condition_number=116;
	success_right=2;
	goto L;
	   }//!R
	else
			{
				goto L;
			}
		}
	}
		}
	}
			}//lr
			else
			{//elr
	if((first_green_adjacent==(max_n_green_adj_section_bunch-1))||
	   ((first_green_adjacent==last_gr_in_chain)&&(abs(dist1gr_to_wmng)<=2*StripWidth)))
	{
		if(boundary_bunch>=0)
		{
	if((differ_bb_fga<0)&&(abs(differ_bb_fga)<=6*StripWidth))
	{
		if((max_n_green_adj_section==previous_mnga)&&(previous_mnga>=0))
		{
	if(!LeftGreenBoundaryBunch[count_fiber])
		{//!L???
	condition_number=115;
	success_left=2;
	goto L;
	   }//!L
	else
			{
				goto L;
			}
		}
	}
		}
	}
			}//elr
			if(abs(dist1gr_to_wmng)<=2*StripWidth)
			{
	 if((previous_success==(count_fiber-1))&&(previous_success>=0))
	 {//prsuc4
		 if((w_mng_length>=DimX/6)&&(w_mng_section_weight>=DimX/3))
		 {
			 if((max_adj_sec_weight>3*DimX)&&(max_adj_sec>=0))
	         {//cond_weight10
	if(left_right)
		{//lr
	if((first_green_adjacent==(max_n_green_adj_section_bunch+1))||
	   (first_green_adjacent==last_gr_in_chain))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
		{//!R???
	condition_number=110;
	success_right=2;
	goto L;
	   }//!R
	else
			{
				goto L;
			}
	}
	    }//lr
	   else
	   {//elr
	if((first_green_adjacent==(max_n_green_adj_section_bunch-1))||
	   (first_green_adjacent==last_gr_in_chain))
	{
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=113;
	success_left=2;
	goto L;
	        }
	else
			{
				goto L;
			}
	}
	   }//elr
			 }//cond_weight10
		 }
	 }//prsuc4
			}
			if(success_oppos==3)
			{//so3
				if(boundary_bunch>=0)
				{//bb>=
	   if((previous_success==(count_fiber-1))&&(previous_success>=0)&&(ratio_length_green))
	 {//prsuc
	if(first_green_adjacent>=0)
	{//fga>=
			if((first_green_adjacent==last_gr_in_chain)&&((first_gr<0)||(last_gr==last_gr_in_chain)))//onlyublast_cor31.05.17
			{//fga=
				if((Intersection_green>=0)&&(Intersection_green<=2)&&(ratio_length_green>=6)&&(ratio_length1_green>=12))
						{//Int_grlast_cor01.06.17onlyub
	if(left_right)
		{//lr
	if((max_adj_sec_weight>3*DimX)&&(max_adj_sec>=0))
	{

	if(!RightGreenBoundaryBunch[count_fiber])
		{//!R???
	condition_number=44;
	success_right=2;
	goto L;
	   }//!R
	else
			{
				goto L;
			}

		}
	    }//lr
	   else
	   {//elr
	if((max_adj_sec_weight>3*DimX)&&(max_adj_sec>=0))
	{//cond_weight
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=49;
	success_left=2;
	goto L;
	        }
	else
			{
				goto L;
			}
	}//cond_weight
	   }//elr
			}//Int_grlast_cor01.06.17onlyub
	}//fga=
	}//fga>=0
	   }//prsuc
				}//bb>=
	if((max_n_green_adj_section>=0)&&(abs(dist1gr_to_wmng)<=2*StripWidth)&&(first_green_adjacent==last_gr_in_chain))
	{//<=St
	   if(left_right)
		{//lr
	if((w_mng_section_weight_oppos>DimX/3)||(w_mng_section_weight>DimX/3))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
		{//!R???
	condition_number=46;
	success_right=2;
	goto L;
	   }//!R
	else
			{
				goto L;
			}
	}
	    }//lr
	   else
	   {//elr
	if((w_mng_section_weight_oppos>DimX/3)||(w_mng_section_weight>DimX/3))
	{//cond_weight
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=51;
	success_left=2;
	goto L;
	        }
	else
			{
				goto L;
			}
	}//cond_weight
	   }//elr
	}//<=St
	else
	{
		if((first_green_adjacent!=last_gr_in_chain))
		{
	if(left_right)
		{//lr
			if(last_gr_in_chain==(max_n_green_adj_section_bunch+1))
			{//lg==
	if((w_mng_section_weight_oppos>DimX/3)||(w_mng_section_weight>DimX/3))
	{//wwop
	if(!RightGreenBoundaryBunch[count_fiber])
		{//!R???
	condition_number=48;
	success_right=3;
	goto L;
	   }//!R
	else
			{
				goto L;
			}
	}//wwop
			}//lg==
	    }//lr
	   else
	   {//elr
		   if(last_gr_in_chain==(max_n_green_adj_section_bunch-1))
			{//lg==
	if((w_mng_section_weight_oppos>DimX/3)||(w_mng_section_weight>DimX/3))
	{//cond_weight
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=53;
	success_left=3;
	goto L;
	        }
	else
			{
				goto L;
			}
	}//cond_weight
		   }//lg==
	   }//elr
		}
	}

			}//so3
		}
	if((previous_success==(count_fiber-1))&&(previous_success>=0))
	{//prsuc
		if(max_n_green_adj_section_bunch>=0)
		{//mngab>=0
		if(left_right)
		{//lr
			if((max_n_green_adj_section_bunch==(first_green_adjacent-1))||
	((abs(dist1gr_to_wmng)<=StripWidth))||
	((max_n_green_adj_section_bunch==(first_green_adjacent-2))&&(first_n_green==(first_green_adjacent-1))
	 &&(last_gr_in_chain==first_green_adjacent)))
			{//mng=fga-1last_cor17.04.17
		if(first_n_green==(first_green_adjacent-1))
		{
	if((max_n_green_adj_section_bunch==(first_green_adjacent-2))||
	   (w_mg_section_oppos_bunch==(first_green_adjacent-2)))
	{
		if((w_mng_section_oppos_incline>0)||(w_mng_section_incline<0)||(sect_incline<0))
		{
	if((w_mng_section_weight_oppos>DimX/3)||(w_mng_section_weight>DimX/3))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
		{//!R
	condition_number=50;
	success_right=2;
	goto L;
	   }//!R
	else
			{
				goto L;
			}
	}
		}
	}
		}
	if((w_mng_section_weight_oppos>1100)||(w_mng_section_weight>1100))
	{//wcon
		  if(previous_mnga==max_n_green_adj_section)
		  {//pmga=mnga
	if(!RightGreenBoundaryBunch[count_fiber])
		{//!Rlast_cor17.04.17
			if((first_gr<0)||((first_gr>=0)&&(last_gr==last_gr_in_chain)))
			{
			if((first_green_adjacent==last_gr_in_chain))
			{
	condition_number=52;
	success_right=2;
	goto L;
			}
			else
			{
	condition_number=210;
	success_right=3;
	goto L;
			}
			}
	   }//!R
	else
			{
				goto L;
			}
		  }//pmga=mnga
	}//wcon
			}//mng=fga-1
			else
	       {
	if((last_gr_in_chain!=first_green_adjacent)&&((last_gr_in_chain-1)==max_n_green_adj_section_bunch))
	{//lgich
		if((w_mng_section_weight_oppos>3000)||(w_mng_section_weight>3000))
	{//w2cond
		if(previous_mnga==max_n_green_adj_section)
		{//pmga1=mnga
	        if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=54;
	success_right=3;
	goto L;
	        }
			else
			{
				goto L;
			}
		}//pmga1=mnga

	}//w2cond
	}//lgich
	if(last_gr_in_chain==first_green_adjacent)
	{//lgc=fga
	    if(previous_mnga==max_n_green_adj_section)
		{//pmga3=mnga
			if((w_mng_section_weight_oppos>3000)||(w_mng_section_weight>3000))
			{//w4cond
	if(!RightGreenBoundaryBunch[count_fiber])
		{//!R
	condition_number=56;
	success_right=2;
	goto L;
	   }//!R
	else
			{
				goto L;
			}
			}//w4cond
		}//pmga3=mnga
	}//lgc=fga
	       }
		}//lr
		else
		{//elr
	if((max_n_green_adj_section_bunch==(first_green_adjacent+1))||
	((first_green_adjacent<max_n_green_adj_section_bunch)&&(abs(dist1gr_to_wmng)<=3*StripWidth))||
	((max_n_green_adj_section_bunch==(first_green_adjacent+2))
	 &&(last_gr_in_chain==first_green_adjacent)))
	//if(max_n_green_adj_section_bunch==first_green_adjacent+1)
			{//mnga=fga+1
	if((w_mng_section_weight_oppos>3000)||(w_mng_section_weight>3000))
	{//w1cond
		if(previous_mnga==max_n_green_adj_section)
		{//pmga=mnga
	        if(!LeftGreenBoundaryBunch[count_fiber])
			{
				if(last_gr_in_chain==first_green_adjacent)
				{//last_cor04.08.17
	condition_number=55;
	success_left=2;
	goto L;
				}
				else
				{
					if((first_gr<0)||(first_gr=last_gr_in_chain))
					{
                     condition_number=550;
	                  success_left=3;
	                  goto L;
					}
				}
	        }
			else
			{
				goto L;
			}
		}//pmga=mnga

	}//w1cond
			}//mnga=fga+1
	        else
	       {
	if((last_gr_in_chain!=first_green_adjacent)&&((last_gr_in_chain+1)==max_n_green_adj_section_bunch))
	{//lgich
		if((w_mng_section_weight_oppos>3000)||(w_mng_section_weight>3000))
	{//w2cond
		if(previous_mnga==max_n_green_adj_section)
		{//pmga1=mnga
	        if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=57;
	success_left=3;
	goto L;
	        }
			else
			{
				goto L;
			}
		}//pmga1=mnga

	}//w2cond
	}//lgich
	if(last_gr_in_chain==first_green_adjacent)
	{//lgc=fga
	    if(previous_mnga==max_n_green_adj_section)
		{//pmga2=mnga
			if((w_mng_section_weight_oppos>3000)||(w_mng_section_weight>3000))
			{//w3cond
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=59;
	success_left=2;
	goto L;
	        }
	else
			{
				goto L;
			}
			}//w3cond
		}//pmga2=mnga
	}//lgc=fga
	       }
		}//elr
		}//mngab>=0
	if((fiber_residual>=0)&&(side_fiber_residual<=5*StripWidth)&&(abs(dist_next_green)<=5*StripWidth))
	{//fr1>=0
		if((residual_ratio2<=8)&&(residual_ratio1<=8))
			{//rr23
				if((residual_ratio10>=0)&&(residual_ratio10<=7))
				{//rr10
	if(section_weight>=300)
		{//sw1
			if(((w_mng_section_oppos>=0)||(max_n_green_adj_section>=0)||
	   (max_adj_sec>=0))&&((w_mng_section_sky_oppos!=1)&&(w_mng_section_sky!=1)))
			{//cond42
	      if((w_mng_section_weight_oppos>1500)||
	   (max_adj_sec_weight>1500)||(w_mng_section_weight>=1500))
	      {//con_weight1
	if((last_gr_in_chain>=0)&&(last_gr_in_chain!=first_green_adjacent))
	{//lgrinch
	    if((fiber_residual_new_lg<fiber_residual_new)&&(abs_convex_dif_new_lg<abs_convex_dif_new)&&
			(fiber_residual_new_lg<fiber_residual)&&(abs_convex_dif_new_lg<abs_convex_dif))
		{//frlg2
	if((residual_ratio3_lg<residual_ratio3)&&(residual_ratio3_lg>=7)&&(residual_ratio3_lg<=8))
	{//frlg1
	   if((residual_ratio10_lg<residual_ratio10)&&(residual_ratio10<=7))
	   {//frlg
	if((n_o_g_contrast_lg>=1)&&(n_o_g_contrast_lg<=3))
			  {//contr
	if(left_right)
	{//lr
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=58;
	success_right=3;
	goto L;
			}
	else
			{
				goto L;
			}
	}//lr
	else
	{//elr
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=61;
	success_left=3;
	goto L;
			}
	else
			{
				goto L;
			}
	}//elr
			  }//contr
	   }//frlg
	}//frlg1
		}//frlg2
	}//lgrinch
	else
	{//elgrinch

			  if((n_o_g_contrast>=1)&&(n_o_g_contrast<=3)&&(abs(dist_to_wmng)<=4*StripWidth))
			  {//contrnew
	if(left_right)
	{//lr
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=60;
	success_right=2;
	goto L;
			}
	else
			{
				goto L;
			}
	}//lr
	else
	{//elr
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=63;
	success_left=2;
	goto L;
			}
	else
			{
				goto L;
			}
	}//elr
			  }//contrnew
	}//elgrinch
		  }//con_weight1
			}//cond42
	}//sw1
				}//rr10
		 }//rr23
	}//fr1>=
		if((fiber_residual>=0)&&(side_fiber_residual<=4*StripWidth))
		{//fr1>=0
			if((residual_ratio2>=8)&&(fiber_residual_new>=fiber_residual)&&(residual_ratio3>=8)&&
				(fiber_residual_new>=side_fiber_residual))
			{//rr23
				if((residual_ratio10>=0)&&(residual_ratio10>=9))
				{//rr10
	if((section_weight>=300)&&(abs(dist_to_wmng)<=3*StripWidth))
		{//sw2
			if(((w_mng_section_oppos>=0)||(max_n_green_adj_section>=0))&&((w_mng_section_sky_oppos!=1)&&(w_mng_section_sky!=1)))
			{//cond43
	      if((w_mng_section_weight_oppos>1500)||
	   (max_adj_sec_weight>1500)||(w_mng_section_weight>=1500))
	      {//con_weight2
	if(left_right)
	{//lr
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=62;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
	}//lr
	else
	{//elr
	if(!LeftGreenBoundaryBunch[count_fiber])
			{//!lg
	condition_number=65;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}//!lg
	else
			{
				goto L;
			}
	}//elr
		  }//con_weight2
			}//cond43
	}//sw2
				}//rr10
			}//rr23
		}//fr1>=
		else
		{//cfr
			if(count_fiber==section_end)
			{//cf
	if(left_right)
	{//lr
	differ_prev_new_fg=old_first_beg-beg_previous;
	}//lr
	else
	{//elr
	differ_prev_new_fg=old_first_end-end_previous;
	}//elr
	if(last_gr_in_chain>=0)
	{//lgich>=0
		if(left_right)
	{//lr
	differ_prev_new_lg=old_first_beg_lg-beg_previous;
	}//lr
	else
	{//elr
	differ_prev_new_lg=old_first_end_lg-end_previous;
	}//elr
		if(last_gr_in_chain!=first_green_adjacent)
		{//lgich!fga
			rt_differ=(16*abs(differ_prev_new_lg))/(abs(differ_prev_new_lg)+abs(differ_prev));
			if(rt_differ<=7)
			{//rd7
	if(section_weight>=300)
		{//sw1l
			if(((w_mng_section_oppos>=0)||(max_n_green_adj_section>=0))&&((w_mng_section_sky_oppos!=1)&&(w_mng_section_sky!=1)))
			{//cond42l
	      if((w_mng_section_weight_oppos>1500)||
	   (max_adj_sec_weight>1500)||(w_mng_section_weight>=1500))
	      {//con_weight1l
	if((n_o_g_contrast_lg>=1)&&(n_o_g_contrast_lg<=3))
			  {//contr
	if(left_right)
	{//lr
	if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=64;
	success_right=3;
	goto L;
			}
	else
			{
				goto L;
			}
	}//lr
	else
	{//elr
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=67;
	success_left=3;
	goto L;
			}
	else
			{
				goto L;
			}
	}//elr
			  }//contr
		  }//con_weight1l
			}//cond42l
	   }//sw1l
			}//rd7
		}//lgich!fga
	}//lgich>=0

			}//cf
		}//efr
	}//prsuc
	}//fg=oa
			   }//fga>0
			   else
			   {
	if(left_right)
				{//lr
					if((section_weight>=3*DimX)&&(sc_gr_comp_red==2))
					{//swsgrc
	if((success_inverse==-2)||(first_n_green_oppos<0))
	{
		if(UpperGreenBoundaryRight<=0)
		{
	if((w_mng_section_weight>=5*DimX)||((w_mng_section_incline<0)&&(w_mng_section_weight>=3*DimX))||
	   ((w_mng_incl_curve<0)&&(w_mng_section_weight>=2*DimX)))
	{
	if(!RightGreenBoundaryBunch[count_fiber])
		{
	condition_number=122;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
	else
			{
				goto L;
			}
	}
		}
	}
					}//swsgrc
					if(boundary_bunch<0)
					{//bb>0
	if((UpperGreenBoundaryRight>0)&&(UpperGreenBoundaryRight<=(2*NumStrips)/3))
	{
	if((count_fiber-UpperGreenBoundaryRight)<=3)
	{
		if(((differ_boundary_sign_prev<0)&&(differ_boundary_abs_prev<=7*StripWidth)
			&&(differ_boundary_abs_prev>=0)))
		{
	if((section_weight>=DimX)&&(sc_gr_comp_red==2)&&(w_mng_section_weight>=DimX))
	{
		if(!RightGreenBoundaryBunch[count_fiber])
		{
	condition_number=124;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
	}
		}
	}
	}
					}//bb>0
			    if(max_n_green_adj_section_bunch>=0)
			    {//mngsb
	if((w_mng_section_weight>=5000)||(w_mng_section_incline<0))
		{//inc<0
			if(w_mng_section_weight>=3000)
			{//wmngsw
	if(last_max_adj_sec_strip>=section_beg)
	{//lmas>=
		if(first_max_adj_sec_strip<=(NumStrips/6))
		{//fas<=
	if(!RightGreenBoundaryBunch[count_fiber])
		{
	condition_number=66;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
	else
			{
				goto L;
			}
		}//fas<=
	}//lmas>=
			}//wmngsw
		}//inc<0
				}//mngsb
		if((sect_incline<0)&&(number_of_boundary_points>0))
		{//sinbp
			if((section_weight>=1000)&&(max_adj_sec>=0))
			{//swmas
				if((max_adj_sec_weight>=1500)||(w_mng_section_weight_oppos>=2000)
					||(w_mng_section_weight>=2000))
				{//masw>=

	if(last_max_adj_sec_strip>=section_beg)
	{//lmass>=
	if(!RightGreenBoundaryBunch[count_fiber])
		{
	condition_number=68;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
	else
			{
				goto L;
			}
	}//lmass>=
				}//masw>=
			}//swmas
		}//sinbp
		if(((section_weight>=1000)||((sect_incline<0)&&(section_weight>=300)))&&
			((w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000)))
				{//w_cond
		if(!RightGreenBoundaryBunch[count_fiber])
		{
	condition_number=70;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}

				}//w_cond
		        }//lr
				else
				{//elr
	if(max_n_green_adj_section_bunch>=0)
	{//mngsb
	if((w_mng_section_weight>=5000)||(w_mng_section_incline>0))
		{//inc>0
			if(w_mng_section_weight>=3000)
			{//wmngsw>=
	if(last_max_adj_sec_strip>=section_beg)
	{//lmass>=
		if(first_max_adj_sec_strip<=(NumStrips/6))
		{//fmass<=
	if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=69;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}//fmass<=
	}//lmass>=
			}//wmngsw>=
		}//inc>0
		if(((section_weight>=1000)||((sect_incline<0)&&(section_weight>=300)))
			&&((w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000)))
				{//w_cond
					if(sc_gr_comp_red==2)
					{//last_cor13.03.18
			if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=71;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
			else
			{
				goto L;
			}
					}//last_cor13.03.18
				}//w_cond
				}//mngsb
				}//elr
			   }

	if(left_right)
	{//lr
		    if(bunch_adjacent_ordered>=0)
			{//bao0
	if((w_mng_section_last_oppos>=NumStrips/2)&&
	   ((w_mng_section_oppos_incline>0)||(w_mng_section_incline<0))&&
	   (sect_incline<0))
	{//condsi<0
		               if((abs(dist_to_wmng)<3*StripWidth)||(((abs(dist_to_wmng)<4*StripWidth)&&
	((previous_success==(count_fiber-1))&&(previous_success>=0)))))
					   {//dist
		if(section_weight>=300)
		{//sw
			if(((w_mng_section_oppos>=0)||(max_n_green_adj_section>=0)||
	   (max_adj_sec>=0))&&((w_mng_section_sky_oppos!=1)&&(w_mng_section_sky!=1)))
			{//cond41
	if((w_mng_section_weight_oppos>2000)||
		   ((max_adj_sec_weight>3*DimX)&&(w_mng_section_weight>=DimX/3))||(w_mng_section_weight>=3*DimX))
		{//last_cor06.12.16!!!!
		if(!RightGreenBoundaryBunch[count_fiber])
			  {
	condition_number=72;
	success_right=1;
	new_boundary_section=section_number;
	          goto L;
			  }
		else
			{
				goto L;
			}
	}
			}//cond41
		}//sw
					   }//dist
	}//condsi<0
			if(first_green_adjacent==bunch_adjacent_ordered)
			{//fga=bao

				if(((first_green_adjacent==last_green_adjacent)||
	(first_green_adjacent==last_gr_in_chain))&&
	 ((max_green_adj_section_bunch<max_n_green_adj_section_bunch)))
				{//one_green_out_cond
	f_g_bunch_length_ratio=(16*old_first_length)/(old_first_length+int_length);
	//convex_dif_new=side_fiber_residual/2-(old_first_beg-std::min(beg_next,beg_previous));
	if(green_adjacent_section<0)
	{//gas<0
		if(((convex_dif<0)&&(convex_dif_new>=0)&&(fiber_residual>=0)&&
			(abs(convex_dif_new<(side_fiber_residual/2))))||((convex_dif<0)&&(convex_dif_new<0)&&
			(abs(convex_dif_new)<abs(convex_dif))))
		{//convex_cond
	if((section_weight>=2000)&&((w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000)||
	   (w_mng_section_weight>=2000)||(w_mng_section_weight_oppos>=2000)))
				{//w_cond
					if((entrance_to_oppos_border>=2)&&(entrance_to_oppos_border>=array_num/2))
					{//entr_cond
			if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=74;
	success_right=2;
	goto L;
			}
			else
			{
				goto L;
			}
					}//entr_cond

				}//w_cond
		}//convex_cond
		else
		{//econvex_cond
			if(fiber_residual>=0)
			{
				if((differ_next>0)&&(differ_prev<0))
					if((!n_o_g_contrast)||(n_o_g_contrast>=4))
					{
	if(((section_weight>=500)||((sect_incline<0)&&(section_weight>=300)))&&
			((w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000)))
				{//w_cond
					if((entrance_to_oppos_border>=2)&&(entrance_to_oppos_border>=array_num/2))
					{//entr_cond
						if(!RightGreenBoundaryBunch[count_fiber])
						{
	condition_number=76;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
						}
						else
			{
				goto L;
			}
					}//entr_cond

				}//w_cond
					}
			}
	if(f_g_bunch_length_ratio<=2)
	{//small-cond
		if((section_weight>=2000)&&
			((w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000)))
				{//w_cond
					if((entrance_to_oppos_border>=2)&&(entrance_to_oppos_border>=array_num/2))
					{//entr_cond
			    if(!RightGreenBoundaryBunch[count_fiber])
				{
	condition_number=78;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			   }
				else
			{
				goto L;
			}
					}//entr_cond

				}//w_cond
	}//small-cond
		}//econvex_cond
	if((fiber_residual_new<fiber_residual)&&(fiber_residual>=0))
	{//rescond1
		if(residual_ratio1<7)
		{//rescon2
			if((residual_ratio2<4)&&(residual_ratio3>=7)&&(residual_ratio3<=9))
			{//rescond3
	if((section_weight>=2000)&&((mng_section_weight_oppos>=2000)||(mng_section_weight>=2000)||
	   (w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000)))
				{//w_cond1
					if((entrance_to_oppos_border>=2)&&(entrance_to_oppos_border>=array_num/2))
					{//entr_cond1
			if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=80;
	success_right=2;
	goto L;
			}
			else
			{
				goto L;
			}
					}//entr_cond1

				}//w_cond1
			}//rescond3
		}//rescon2
	}//rescond1
	if(f_g_bunch_density_ratio<=7)
	{
		if(abs(dist_to_wmng)<2*StripWidth)
		{
			if(w_mng_section_incline<0)
			{
	if((w_mng_section_weight_oppos>=3000)||(w_mng_section_weight>=3000))
	{
		if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=82;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
	}
			}
		}
	}
	}//gas<0
	else
	{//egas<0
		if(RightGreenBoundaryBunch[count_fiber]>0)
		{
			goto L;
		}
		if((section_weight>=1000)&&(sect_incline<0))
		{//swsi
	if(fiber_residual<0)
	{//fr<0

	   if((abs(dist_to_wmng)<StripWidth)||(first_green_adjacent<max_n_green_adj_section_bunch)||
		   (old_first_length<StripWidth))
	   {
	if((w_mng_section_weight_oppos>=3000)||(w_mng_section_weight>=3000))
	{
		if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=84;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
	}
	   }
	}//fr<0
	else
	{//efr<
		if(((convex_dif<0)&&(convex_dif_new>=0)&&(fiber_residual>=0)&&
			(abs(convex_dif_new<(side_fiber_residual/2))))||((convex_dif<0)&&(convex_dif_new<0)&&
			(abs(convex_dif_new)<abs(convex_dif))))
		{//convex_cond
	if((w_mng_section_weight_oppos>=3000)||(w_mng_section_weight>=3000))
	{
		if(!RightGreenBoundaryBunch[count_fiber])
		{
	condition_number=86;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
	}
		}//convex_cond
	}//efr<

		}//swsi
	}//egas<0
				}//one_gree_out_cond
	if(green_adjacent_section>=0)
	{
	distance_to_green[array_num]=1;
	}
	else
	{

					if(first_green_chain_length<(2*StripWidth))
					{
						without_green_adjacent++;
						distance_to_green[array_num]=3;
					}
				    else
				    {
				total_with_green_adjacent++;
	distance_to_green[array_num]=2;
				    }
	}
			}//fga=boa
			else
			{//efga=bao
				if(first_n_green==bunch_adjacent_ordered)
				{//fng=bfo
	f_g_f_n_length_ratio1=(16*old_first_n_length)/(old_first_n_length+int_length);

	if((previous_success==(count_fiber-1))&&(previous_success>=0))
	{//prsucc
		if((max_n_green_adj_section>=0)&&(max_n_green_adj_section==previous_mnga))
		{//max_ng_adj-condlast_cor12.04.17
		if((abs(dist_to_wmng)<=2*StripWidth)||(first_green_adjacent<0)||
	((first_green_adjacent>=0)&&(first_green_adjacent<max_n_green_adj_section_bunch)&&
	 (max_n_green_adj_section_bunch>=0)))
		{//relation_cond
			if(max_adj_sec_weight>=4*DimX)
			{
				if(last_max_adj_sec_strip==(w_mng_section_first-1))
				{
				if(!RightGreenBoundaryBunch[count_fiber])
			  {
	condition_number=88;
	success_right=1;
	new_boundary_section=section_number;
	          goto L;
			  }
				else
			{
				goto L;
			}
				}
			}
		}//relation_cond
		}//max_ng_adj-condlast_cor12.04.17
	     if((section_weight>=DimX)&&(max_n_green_adj_section>=0)
			 &&(max_n_green_adj_section==previous_mnga))
		 {//weigt_cond
			 if((closeness_to_boundary+number_of_boundary_points>=array_num/2)
				 &&(closeness_to_boundary>0))
			 {//cl_nb_cond
	if(max_adj_sec>=0)
	{//mas>=
		if(max_adj_sec_weight>=4*DimX)
		{//w_cond_max
	if((count_fiber>NumStrips/2)&&(count_fiber>last_max_adj_sec_strip))
		  {//cf>cond
		if((!green_lg_before)||(abs(dist_to_wmng)<=StripWidth)||(first_green_adjacent<0)||
	((first_green_adjacent>=0)&&(first_green_adjacent<max_n_green_adj_section_bunch)&&
	 (max_n_green_adj_section_bunch>=0)))
		{//relation_cond1last_cor12.04.17
			  if(!RightGreenBoundaryBunch[count_fiber])
			  {
	condition_number=90;
	success_right=1;
	new_boundary_section=section_number;
	          goto L;
			  }
			  else
			{
				goto L;
			}
		}//relation_cond1last_cor12.04.17
		  }//cf>cond
		}//w_cond_max
	}//mas>=
			 }//cl_nb_cond
			 if((f_g_f_n_length_ratio1>=4)||(count_fiber>(2*(NumStrips/3))))
			 {//length_condlast_cor12.04.17
				 if((max_n_green_adj_section>=0)&&(max_n_green_adj_section==previous_mnga))
				 {
	if((entrance_to_oppos_border>=2)&&(entrance_to_oppos_border>=array_num/2)&&
	   (number_of_boundary_points>=array_num/2)&&(number_of_boundary_points>=3))
	{//entr_cond
	      if((count_fiber>NumStrips/2)&&(count_fiber>last_max_adj_sec_strip))
		  {//cf_cond
			  if( max_adj_sec_weight>=4*DimX)
			  {
	if((!green_lg_before)||(abs(dist_to_wmng)<=StripWidth)||(first_green_adjacent<0)||
	((first_green_adjacent>=0)&&(first_green_adjacent<max_n_green_adj_section_bunch)&&
	 (max_n_green_adj_section_bunch>=0)))
	{//relation_cond2last_cor12.04.17
			  if(!RightGreenBoundaryBunch[count_fiber])
			  {
	condition_number=92;
	success_right=1;
	new_boundary_section=section_number;
	          goto L;
			  }
			  else
			{
				goto L;
			}
	}//relation_cond2last_cor12.04.17
			  }
		  }//cf_cond
	}//entr_cond
				 }
			 }//length_cond
		 }//weigt_cond
	}//prsucc

	if((first_green_adjacent==(first_n_green-1))&&(first_green_adjacent>=0))
	{//fgafng
		f_g_f_n_length_ratio=(16*old_first_n_length)/(old_first_n_length+old_first_length);
		f_g_f_n_signif_ratio=(16*old_first_n_signif)/(old_first_n_signif+old_first_signif);
		   if(first_green_adjacent==last_gr_in_chain)
		   {//fga=lgrch
		if(green_adjacent_section<0)
		{//gas<0
	if(f_g_f_n_length_ratio1<=2)
	{//small_cond1
	if(first_green_adjacent>0)
	{
		new_neighbor=first_green_adjacent-1;
		old_new_neighbor=
	ColorInt[count_fiber].old_ordered_bunch_number[new_neighbor];
	    old_new_neighbor_beg=
	ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[old_new_neighbor];
		old_new_neighbor_end=
	ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[old_new_neighbor];
		old_new_neighbor_length=old_new_neighbor_end-old_new_neighbor_beg;
	    old_new_neighbor_sat=
	ColorInt[count_fiber].ColoredIntervalsStructure->AverageSat[old_new_neighbor];
		old_new_neighbor_hue=
	ColorInt[count_fiber].ColoredIntervalsStructure->AverageHue[old_new_neighbor];
		new_neighbor_hue_zone=hue_zones[old_new_neighbor_hue];
	    old_new_neighbor_gray=
	ColorInt[count_fiber].ColoredIntervalsStructure->AverageGray[old_new_neighbor];
	n_n_gr_ratio=(16*old_new_neighbor_length)/(old_new_neighbor_length+old_first_length);
	/*n_o_g_contrast=
	ColorInt[count_fiber].Test_Junction_Corrected(bunch_old_number,old_first_green_adjacent,
	bunch_occurrence,
	Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
		n_o_g_contrast=n_o_g_contrast%10;*/
		if((n_o_g_contrast>=1)&&(n_o_g_contrast<=2))
		{//nog
			if((n_n_gr_ratio>=12)&&((old_new_neighbor_sat<=1)||(new_neighbor_hue_zone<2)||
				(new_neighbor_hue_zone>3)))
			{
	/*if(count_adj_n_green_sec_max_oppos>=1)
				{
				max_adj_sec=adj_sec_n_green_max_oppos[0]-1;
	            max_adj_sec_weight=ColorSection->section_weight[max_adj_sec];
				}*/
			if(((closeness_to_boundary+number_of_boundary_points)>=array_num/2)||
				((section_weight>=1000)&&(sect_incline<0)))
			{//cl
	if(((w_mng_section_oppos>=0)||(max_n_green_adj_section>=0)||
	   (max_adj_sec>=0))&&((w_mng_section_sky_oppos!=1)&&(w_mng_section_sky!=1)))
				{//cond4
	if((w_mng_length>(DimX/4))||(w_mng_section_weight>2000)||(w_mng_section_weight_oppos>2000)||
	   (max_adj_sec_weight>2500))
	{
	        distance_to_green[array_num]=-2;
	if((success_inverse==-2)||((first_green_adjacent_oppos<0)&&(first_n_green_oppos<0))||
	   (entrance_to_oppos_border_now)||
	   (((entrance_to_oppos_border+number_of_boundary_points)>array_num/2)))
			{
				if(green_adjacent_section<0)
				{
					if(!RightGreenBoundaryBunch[count_fiber])
					{
	condition_number=94;
	success_right=2;
	goto L;
	}
			else
			{
				goto L;
			}
				}
			}
	}
				}//cond4
			}//cl
			}
		}//nog
	}
	}//small_cond1
			if((old_first_n_length<=StripWidth)||(f_g_f_n_length_ratio<=3)||(f_g_f_n_signif_ratio<=3))
			{//small_cond
				if((count_fiber>section_beg)&&(count_fiber<section_end))
				{//between_cond
	//fiber_residual_new=abs(old_first_beg-beg_next)+abs(old_first_beg-beg_previous);
	//residual_ratio1=(16*(side_fiber_residual+1))/(side_fiber_residual+fiber_residual+2);
	//residual_ratio2=(16*(fiber_residual_new+1))/(fiber_residual_new+fiber_residual+2);
	//residual_ratio3=(16*(fiber_residual_new+1))/(fiber_residual_new+side_fiber_residual+2);
	if(fiber_residual_new<fiber_residual)
	{//rescond1
		if(residual_ratio1<7)
		{//rescon2
			if((residual_ratio2<7)&&(residual_ratio3>=7)&&(residual_ratio3<=9))
			{//rescond3
	if((section_weight>=2000)&&((w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000)))
				{
					if((entrance_to_oppos_border>=2)&&(entrance_to_oppos_border>=array_num/2))
					{
			if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=96;
	success_right=2;
	goto L;
			}
			else
			{
				goto L;
			}
					}

				}
			}//rescond3
		}//rescon2
	}//rescond1
				}//between_cond
			}//small_cond
		}//gas<0

		   }//fga=lgrch
		   if(green_adjacent_section_oppos>=0)
		   {
	boun_point_num=LeftRightSectionNumberOfRightGreenBoundaryPoints[green_adjacent_section_oppos];
			   if(((old_first_n_length<=6)||
	((f_g_f_n_length_ratio<=2)&&(f_g_f_n_length_ratio1<=2)))&&(boun_point_num>0))
			   {
				   closeness_to_boundary++;
			   }
		   }
		//{//gas<0)
	}//fgafng
				}//fng=bfo
	if(count_fiber<=NumStrips/8)
				{//close_zone
	if((first_green_adjacent<max_n_green_adj_section_bunch)&&(max_n_green_adj_section>=0)
	   &&(w_mng_length>DimX/4)&&(non_green_length>=DimX/2)&&(abs(dist_to_wmng)<(DimX/8)))
	{//cond7last_cor12.04.17
		if((sc_gr_comp_red==2)&&(sect_incline<0)&&(section_weight>=DimX))
		{
			if(!RightGreenBoundaryBunch[count_fiber])
			{
	condition_number=98;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
			else
			{
				goto L;
			}
		}
	}//cond7
				}//close_zone
		/*if((first_green_adjacent<max_n_green_adj_section_bunch)&&
	   (max_n_green_adj_section_bunch>=0))
		{//cond3
			if(count_adj_n_green_sec_max_oppos>=1)
				{
				max_adj_sec=adj_sec_n_green_max_oppos[0]-1;
	            max_adj_sec_weight=ColorSection->section_weight[max_adj_sec];
				}
	if((((closeness_to_boundary+number_of_boundary_points)>=array_num/2)&&(section_weight>=500))||
				((section_weight>=300)&&(sect_incline<0)))
			{//cl
	if(((w_mng_section_oppos>=0)||(max_n_green_adj_section>=0)||
	   (max_adj_sec>=0))&&((w_mng_section_sky_oppos!=1)&&(w_mng_section_sky!=1)))
				{//cond4
	if(((w_mng_length>(DimX/4))&&(abs(dist_to_wmng)<(DimX/8)))||(w_mng_section_weight>2000)||(w_mng_section_weight_oppos>2000)||
	   ((max_adj_sec_weight>3000)&&(abs(dist_wmng_to_mga)<=6*StripWidth)))
	{
	        distance_to_green[array_num]=-2;
	if((success_inverse==-2)||((first_green_adjacent_oppos<0)&&(first_n_green_oppos<0))||
	   (entrance_to_oppos_border_now)||
	   ((count_fiber>NumStrips/2)&&(entrance_to_oppos_border>=array_num/3))||
	   ((entrance_to_oppos_border>=array_num/3)&&(number_of_boundary_points>=array_num/2)))
			{
				if(!RightGreenBoundaryBunch[count_fiber])
				{
					if(sc_gr_comp_red==2)
					{//last_cor17.03.17
	condition_number=100;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
					}//last_cor17.03.17
				}
				else
			{
				goto L;
			}
			}
	}
				}//cond4
			}//cl
		}//cond3*/
			}//efga=bao
	if((first_green_adjacent==(bunch_adjacent_ordered-1))&&(first_green_adjacent>=0))
	{//fgabao
		if(green_adjacent_section>=0)
		{
			if(green_adjacent_section_weight>=section_weight)
			{
				goto L;
			}
		}
		if(f_g_bunch_length_ratio<=2)
		{//fgblr
	b_a_old=
	ColorInt[count_fiber].old_ordered_bunch_number[bunch_adjacent_ordered];
	b_a_o_neighbor_beg=
	ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[b_a_old];
	b_a_o_neighbor_end=
	ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[b_a_old];
	b_a_o_neighbor_length=b_a_o_neighbor_end-b_a_o_neighbor_beg+1;
	f_g_f_n_length_ratio=(16*b_a_o_neighbor_length)/(b_a_o_neighbor_length+int_length);
	if((b_a_o_neighbor_length<=StripWidth)&&(f_g_f_n_length_ratio<=2))
	{//bao_n_l
	n_o_g_contrast=
	ColorInt[count_fiber].Test_Junction_Corrected(bunch_old_number,old_first_green_adjacent,
	bunch_occurrence,
	Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
		n_o_g_contrast=n_o_g_contrast%10;
		if((n_o_g_contrast>=1)&&(n_o_g_contrast<=2))
		{//nog
		if(first_green_adjacent>max_n_green_adj_section_bunch)
		{
	/*if(count_adj_n_green_sec_max_oppos>=1)
				{
				max_adj_sec=adj_sec_n_green_max_oppos[0]-1;
	            max_adj_sec_weight=ColorSection->section_weight[max_adj_sec];
				}*/
			/*if(((closeness_to_boundary+number_of_boundary_points)>=array_num/2)||
				((section_weight>=100)&&(sect_incline<0)))
			{//cl
	if(((w_mng_section_oppos>=0)||(max_n_green_adj_section>=0)||
	   (max_adj_sec>=0))&&((w_mng_section_sky_oppos!=1)&&(w_mng_section_sky!=1)))
				{//cond4
	if((w_mng_length>(DimX/4))||(w_mng_section_weight>2000)||(w_mng_section_weight_oppos>2000)||
	   (max_adj_sec_weight>2500))
	{
	        distance_to_green[array_num]=-2;
	if((success_inverse==-2)||((first_green_adjacent_oppos<0)&&(first_n_green_oppos<0))||
	   (entrance_to_oppos_border_now)||
	   (((entrance_to_oppos_border+number_of_boundary_points)>array_num/2)))
			{
				if(last_gr_in_chain==first_green_adjacent)
				{
					if(previous_mnga>=0)
					{
				if(!RightGreenBoundaryBunch[count_fiber])
				{//!R
	condition_number=102;
	success_right=2;
	goto L;
				}//!R
				else
			{
				goto L;
			}
					}
				}
			}
	}
				}//cond4//last_cor04.05.17
			}//cl*/
		}
		}//nog

	}//bao_n_l
		}//fgblr

	}//fga=bao
			}//bao0
			else
			{
				distance_to_green[array_num]=-10;
			}

	}//lr
	else
	{//elr
	if(bunch_adjacent_ordered>=0)
		    {//bao0
			if(first_green_adjacent==bunch_adjacent_ordered)
				{//fga=bao
					/*if(count_adj_n_green_sec_max_oppos>=1)
				{
				max_adj_sec=adj_sec_n_green_max_oppos[0]-1;
	            max_adj_sec_weight=ColorSection->section_weight[max_adj_sec];
	            first_max_adj_sec_strip=ColorSection->section_fibre_first[max_adj_sec];
				last_max_adj_sec_strip=ColorSection->section_fibre_last[max_adj_sec];
				}*/
				if(((first_green_adjacent==last_green_adjacent)||
	((first_green_adjacent==last_gr_in_chain)&&(residual_ratio2<=9)&&(first_green_adjacent>=0)))&&
	 ((max_green_adj_section_bunch>max_n_green_adj_section_bunch)&&(max_n_green_adj_section_bunch>=0)))
				{//one_green_out_cond
	f_g_f_n_length_ratio=(16*old_first_length)/(old_first_length+int_length);
	if(fiber_residual>=0)
	{
	convex_dif_new=side_fiber_residual/2-(old_first_end-std::min(end_next,end_previous));
	}
	if(array_num>=2)
	{//an>=
		differ_prev_new=old_first_end-end_previous;

	if((differ_prev1>0)&&(differ_prev>0)&&(differ_prev_new>2*differ_prev)&&
	   (differ_prev_new>2*differ_prev1))
		{//conv
			if((right_contrast>=5)&&(right_contrast<=6)&&(abs(dist_to_wmng)<=2*StripWidth))
			{//contr
	if((section_weight>=DimX)||((section_weight>=DimX/2)&&(sc_gr_comp_red==2)&&(sect_incline>0)))
		 {//weigt_cond
			 if((closeness_to_boundary+number_of_boundary_points>=array_num/2)
				 &&((closeness_to_boundary>0)||(number_of_boundary_points)))
			 {//cl_nb_cond
	if((max_adj_sec>=0)&&(max_n_green_adj_section_bunch>=0))
	{//mas>=last_cor12.04.17
		if(((max_adj_sec_weight>=4*DimX)&&(w_mng_section_weight>=DimX/2))
			||(w_mng_section_weight>=4*DimX))
		{//w_cond_max

			  if(!LeftGreenBoundaryBunch[count_fiber])
			  {
	condition_number=73;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			  }
			  else
			{
				goto L;
			}

		}//w_cond_max
	}//mas>=
			 }//cl_nb_cond
	}//weigt_cond
			}//contr
		}//conv
	}//an>=
	if(green_adjacent_section<0)
	{//gas<0


		if(((convex_dif<0)&&(convex_dif_new>=0)&&(fiber_residual>=0)&&
			(abs(convex_dif_new)<(side_fiber_residual/2)))||((convex_dif<0)&&(convex_dif_new<0)&&
			(abs(convex_dif_new)<abs(convex_dif))))
		{//convex_cond
	if((section_weight>=2000)&&((mng_section_weight_oppos>=2000)||(mng_section_weight>=2000)||
	   (w_mng_section_weight>=2000)||(w_mng_section_weight_oppos>=2000)))
				{//w_cond
					if((entrance_to_oppos_border>=2)&&(entrance_to_oppos_border>=array_num/2))
					{//entr_cond
						if(!LeftGreenBoundaryBunch[count_fiber])
			           {
	condition_number=75;
	success_left=2;
	goto L;
						}
						else
			{
				goto L;
			}
					}//entr_cond

				}//w_cond
		}//convex_cond
		else
		{//econvex_cond
	if(f_g_f_n_length_ratio<=2)
	{//small-cond
		if(max_n_green_adj_section_bunch==(first_green_adjacent+1))
		{//=+1
			if(entrance_to_oppos_border_now>0)
			{
				if(number_of_boun_points_in_max_adj>0)
				{
		if(w_mg_boun_point_ratio>8)
		{
			if((hue_close>0)&&(saturation_close>0))
			{
	if((mng_section_weight_oppos>=2000)||(mng_section_weight>=2000)||
	   (w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000))
				{//w_cond10
	          if(!LeftGreenBoundaryBunch[count_fiber])
			  {
	condition_number=77;
	success_left=2;
	goto L;
	          }
			  else
			{
				goto L;
			}
	            }//w_cond10

			}
			else
			{
	if((mng_section_weight_oppos>=2000)||(mng_section_weight>=2000)||
	   (w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000))
				{//w_cond11
	if(!LeftGreenBoundaryBunch[count_fiber])
			  {
	condition_number=79;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
	           }
	else
			{
				goto L;
			}
	            }//w_cond11
			}
		}
				}



			}
		}
		if((section_weight>=2000)&&((w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000)||
			(w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000)))
				{//w_cond
					if((entrance_to_oppos_border>=2)&&(entrance_to_oppos_border>=array_num/2))
					{//entr_cond
						if(!LeftGreenBoundaryBunch[count_fiber])
			            {
	condition_number=81;
	success_left=2;
	goto L;
						}
						else
			{
				goto L;
			}
					}//entr_cond

				}//w_cond
	}//small-cond
		}//econvex_cond
	if(fiber_residual_new<fiber_residual)
	{//rescond1
		if(residual_ratio1<7)
		{//rescon2
			if((residual_ratio2<4)&&(residual_ratio3>=7)&&(residual_ratio3<=9))
			{//rescond3
	if((section_weight>=2000)&&((mng_section_weight_oppos>=2000)||(mng_section_weight>=2000)||
	   (w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000)))
				{//w_cond1
					if((entrance_to_oppos_border>=2)&&(entrance_to_oppos_border>=array_num/2))
					{//entr_cond1
	condition_number=83;
	success_left=2;
	goto L;
					}//entr_cond1

				}//w_cond1
			}//rescond3
		}//rescon2
	}//rescond1
	}//gas<0
				}//one_gree_out_cond

	if(green_adjacent_section>=0)
	{
	distance_to_green[array_num]=1;
	}
	else
	{

					if(first_green_chain_length<(2*StripWidth))
					{
						without_green_adjacent++;
						distance_to_green[array_num]=3;
					}
				    else
				    {
				total_with_green_adjacent++;
	distance_to_green[array_num]=2;
				    }
	}
			}//fga=lga
			else
			{//e=
			//if(first_green_adjacent<last_n_green)
				if(first_n_green==bunch_adjacent_ordered)
				{//fng=bfo
	f_g_f_n_length_ratio1=(16*old_first_n_length)/(old_first_n_length+int_length);
	/*if(count_adj_n_green_sec_max_oppos>=1)
				{
				max_adj_sec=adj_sec_n_green_max_oppos[0]-1;
	            max_adj_sec_weight=ColorSection->section_weight[max_adj_sec];
	            first_max_adj_sec_strip=ColorSection->section_fibre_first[max_adj_sec];
				last_max_adj_sec_strip=ColorSection->section_fibre_last[max_adj_sec];
				}*/
	if((previous_success==(count_fiber-1))&&(previous_success>=0))
	{//prsucclast_cor12.04.17
	if((abs(dist_to_wmng)<=2*StripWidth)||(first_green_adjacent<0)||
				((first_green_adjacent>=0)&&
				(first_green_adjacent>max_n_green_adj_section_bunch)&&
				(max_n_green_adj_section_bunch>=0)))
		{
			if((max_adj_sec_weight>=4*DimX)&&(max_n_green_adj_section==previous_mnga))
			{
				if(last_max_adj_sec_strip==(w_mng_section_first-1))
				{
				if(!LeftGreenBoundaryBunch[count_fiber])
			  {
	condition_number=85;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			  }
				else
			{
				goto L;
			}
				}
			}
		}
	     /*if((section_weight>=1000)||((section_weight>=400)&&(sc_gr_comp_red==2)&&(sect_incline>0)))
		 {//weigt_cond
			 if((closeness_to_boundary+number_of_boundary_points>=array_num/2)
				 &&((closeness_to_boundary>0)||(number_of_boundary_points>0)))
			 {//cl_nb_cond
	if((max_adj_sec>=0)||(max_n_green_adj_section_bunch>=0))
	{//mas>=
		if((max_adj_sec_weight>=2500)||(w_mng_section_weight>=2500))
		{//w_cond_max
	      if((count_fiber>NumStrips/3)&&(count_fiber>last_max_adj_sec_strip)&&(last_max_adj_sec_strip>0))
		  {//cf>cond
			  if(!LeftGreenBoundaryBunch[count_fiber])
			  {
	condition_number=87;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			  }
			  else
			{
				goto L;
			}
		  }//cf>cond
	      else
	      {
	if(((sect_incline>0)&&(number_of_boundary_points>=array_num/2))
	   ||((number_of_boundary_points>=(2*array_num/3))))
	{
		if(!LeftGreenBoundaryBunch[count_fiber])
			  {
	condition_number=89;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			  }
			  else
			{
				goto L;
			}
	}
	      }
		}//w_cond_max
	}//mas>=
			 }//cl_nb_cond
			 if((f_g_f_n_length_ratio1>=4)||(count_fiber>(2*(NumStrips/3))))
			 {//length_cond
	if((entrance_to_oppos_border>=2)&&(entrance_to_oppos_border>=array_num/2)&&
	   (number_of_boundary_points>=array_num/2)&&(number_of_boundary_points>=3))
	{//entr_cond
	      if((count_fiber>NumStrips/2)&&(count_fiber>last_max_adj_sec_strip))
		  {//cf_cond
			  if( max_adj_sec_weight>=3000)
			  {
			  if(!LeftGreenBoundaryBunch[count_fiber])
			  {
	condition_number=91;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			  }
			  else
			{
				goto L;
			}
			  }
		  }//cf_cond
	}//entr_cond
			 }//length_cond
		 }//weigt_condtest all conditions!!!*/
		/* if((first_green_adjacent==(first_n_green+1))&&(first_green_adjacent>=0))
	     {//fgafnglast_cor12.04.17
			 if(fiber_residual_new>=0)
			 {
	if((residual_ratio1>=7)&&(residual_ratio1<=9)&&(residual_ratio2>=10))
	{//res_cond
		if(((section_weight>=3*DimX)||((section_weight>=DimX)&&(sc_gr_comp_red==2)
			&&(sect_incline>0)))&&
	((w_mng_section_weight_oppos>=3*DimX)||(w_mng_section_weight>=3*DimX)))
				{//w_cond
					if((closeness_to_boundary+number_of_boundary_points)>=array_num/2)
					{
						if(!LeftGreenBoundaryBunch[count_fiber])
						{
	condition_number=93;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
						}
						else
			{
				goto L;
			}
					}

				}//w_cond
	}//res_cond
			 }
		 }//fgafng*/
	}//prsucc
	if((first_green_adjacent==(first_n_green+1))&&(first_green_adjacent>=0))
	{//fgafng
		f_g_f_n_length_ratio=(16*old_first_n_length)/(old_first_n_length+old_first_length);
		f_g_f_n_signif_ratio=(16*old_first_n_signif)/(old_first_n_signif+old_first_signif);
		   if(first_green_adjacent==last_gr_in_chain)
		   {
		if((green_adjacent_section<0)||(green_adjacent_section_weight_normalized<=StripWidth))
		{
			if(old_first_n_length>=StripWidth)
			{
				if((count_fiber>section_beg)&&(count_fiber<section_end))
				{
	//fiber_residual_new=abs(old_first_beg-beg_next)+abs(old_first_beg-beg_previous);
	//residual_ratio1=(16*(side_fiber_residual+1))/(side_fiber_residual+fiber_residual+2);
	//residual_ratio2=(16*(fiber_residual_new+1))/(fiber_residual_new+fiber_residual+2);
	//residual_ratio3=(16*(fiber_residual_new+1))/(fiber_residual_new+side_fiber_residual+2);

	if(fiber_residual_new<fiber_residual)
	{
		if(residual_ratio1<7)
		{
			if((residual_ratio2<4)&&(residual_ratio3>=7)&&(residual_ratio3<=9))
			{
	if((section_weight>=2000)&&((w_mng_section_weight_oppos>=2000)||(w_mng_section_weight>=2000)))
				{
					if((entrance_to_oppos_border>=2)&&(entrance_to_oppos_border>=array_num/2))
					{
						if(!LeftGreenBoundaryBunch[count_fiber])
						{
	condition_number=95;
	success_left=2;
	goto L;
						}
						else
			{
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
		   }
	}//fgafng
				}//fng=bfo
				//else
				//{//efng=bfo
				if(count_fiber<=NumStrips/8)
				{//close_zone
	if((first_green_adjacent>maximum_non_green_adjacent)&&
	   (first_green_adjacent>=0)&&(maximum_non_green_adjacent>=0)&&(mng_length>DimX/4)&&
	   (non_green_length>=DimX/2))
	{//cond7
		if((sc_gr_comp_red==2)&&(sect_incline>0)&&(section_weight>=500))
		{
			if(!LeftGreenBoundaryBunch[count_fiber])
			{
	condition_number=97;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
			else
			{
				goto L;
			}
		}
	}//cond7
				}//close_zone
		if((((first_green_adjacent>max_n_green_adj_section_bunch))||
			(first_green_adjacent<0))&&(max_n_green_adj_section_bunch>0))
		{//cond3
			if(first_green_adjacent==max_green_adj_section_bunch)
			{//fga=mgasb
				if((sc_gr_comp_red==2)&&(sect_incline>0)&&(section_weight>=200))
				{
					if((number_of_boun_points_in_max_adj>0)&&(w_mg_boun_point_ratio>=8))
					{
	if(!LeftGreenBoundaryBunch[count_fiber])
	{
	condition_number=99;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
	}
	else
			{
				goto L;
			}
					}
				}

			}//fga=mgasb
			if(success_oppos==3)
			{
	if((first_green_adjacent_oppos==oppos_adjacent)&&(first_green_adjacent_oppos>=0))
	{
				if((green_adjacent_section_op>=0)&&(number_of_boundary_points_adjacent_oppos>0))
				{
	if(!LeftGreenBoundaryBunch[count_fiber])
				{
	condition_number=101;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
				}
	else
			{
				goto L;
			}
				}
	}
			}

	/*if((((w_mng_section_oppos>=0)&&(w_mng_section_first_oppos<=NumStrips/2))||
	   ((max_n_green_adj_section>=0)&&(w_mng_section_first<=NumStrips/2))||
	   ((max_adj_sec>=0)&&(first_max_adj_sec_strip<=NumStrips/2)))&&
	   ((w_mng_section_sky_oppos!=1)&&(w_mng_section_sky!=1)))
				{//cond4
	if((w_mng_length>(DimX/4))||(w_mng_section_weight>2000)||(w_mng_section_weight_oppos>2000)||
	   (max_adj_sec_weight>3000))
	{
	        distance_to_green[array_num]=-2;
	if((success_inverse==-2)||((first_green_adjacent_oppos<0)&&(first_n_green_oppos<0))||
	   (entrance_to_oppos_border_now)||
	   ((count_fiber>NumStrips/2)&&(entrance_to_oppos_border>array_num/2)
	   &&(number_of_boundary_points>0)))
			{
				if(!LeftGreenBoundaryBunch[count_fiber])
				{
	condition_number=103;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
				}
				else
			{
				goto L;
			}
			}
	        else
	        {
				if(bunch_adjacent_ordered==max_n_green_adj_section_bunch)
				{
	if((w_mng_section_weight>4000)||(w_mng_section_weight_oppos>4000)||
	   (max_adj_sec_weight>4000)&&(section_gray_zone==2))
	{
		if(!LeftGreenBoundaryBunch[count_fiber])
		{
	condition_number=105;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
		}
		else
			{
				goto L;
			}
	}
				}
	        }
	}
				}//cond4*/
		}//cond3
	//}//e
				//}//efng=bfo

			}//e=
		//}//efga

			}//bao0
			else
			{
				distance_to_green[array_num]=-10;
			}

	}//elr
	if((max_n_green_adj_section==previous_mnga)&&
	   (bunch_adjacent_ordered==max_n_green_adj_section_bunch)&&(bunch_adjacent_ordered>=0))
	{//bao=mngasb
	if(count_fiber>NumStrips/2)
	{//cf>0
		if(left_right)
	    {//lr
	if(boundary_bunch>=0)
	{//!0
	if(differ_boundary_abs<5*StripWidth)
	{//db<
	if(section_boun_prev>=0)
	{
	     if(section_boun_prev!=section_number)
		 {//sbp!=sn
	if(number_bound_points>=section_length_prev/2)
	{
		if(differ_boundary_abs<3*StripWidth)
	    {
	if((!RightGreenBoundaryBunch[count_fiber])&&(sc_gr_comp_red==2))
			{
	condition_number=104;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		}
	}
		 }//sbp!=sn
		 else
		 {//sbp=sn
	if((previous_success==(count_fiber-1))&&(previous_success>=0))
	{//prsucc
	if((!RightGreenBoundaryBunch[count_fiber])&&(sc_gr_comp_red==2))
			{
	condition_number=106;
	success_right=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
	}//prsucc
		 }//sbp=sn
	}
	}//db<
	}//!0
		}//lr
		else
		{//elr
	if(boundary_bunch>=0)
	{//!0
	if(differ_boundary_abs<5*StripWidth)
	{
	if(section_boun_prev>=0)
	{
	     if(section_boun_prev!=section_number)
		 {//sbp!=sn
	number_bound_points=LeftRightSectionNumberOfRightGreenBoundaryPoints[section_boun_prev];
	  if(number_bound_points>=section_length_prev/2)
	  {
		  if(differ_boundary_abs<3*StripWidth)
	      {
	if((!LeftGreenBoundaryBunch[count_fiber])&&(sc_gr_comp_red==2))
			{
	condition_number=107;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
		  }
	  }
	     }
		 else
		 {
	if((previous_success==(count_fiber-1))&&(previous_success>=0))
	{//prsucc
	if((!LeftGreenBoundaryBunch[count_fiber])&&(sc_gr_comp_red==2))
			{
	condition_number=109;
	success_left=1;
	new_boundary_section=section_number;
	goto L;
			}
	else
			{
				goto L;
			}
	}//prsucc
		 }
	}
	}
	}//!0
		}//elr
	}//cf>0
	}//bao=mngasb
		}//ncb


		dist_next_green_previous=dist_next_green;
		dist_to_wmng_previous=dist_to_wmng;
	    dist_to_wmg_previous=dist_to_wmg;
	L:;
		if(left_right)
	    {//lr
	if(success_right>0)
	{//sr>0
		if(success_right==1)
		{//sr1
	RightGreenBoundaryBunch[count_fiber]=bunch_old_number+1;
		}//sr1
		else
		{//esr1
			if(success_right==2)
		   {//sr2
	RightGreenBoundaryBunch[count_fiber]=old_first_green_adjacent+1;
		   }//sr2
			else
			{//esr2
	         if(success_right==3)
		     {//sr3
	RightGreenBoundaryBunch[count_fiber]=old_first_green_adjacent_lg+1;
		     }//sr3
			 else
			 {//last_cor05.04.17
				 if(success_right==4)
				 {
	RightGreenBoundaryBunch[count_fiber]=
	ColorInt[count_fiber].old_ordered_bunch_number[last_gr]+1;
				 }
			 }
			}//esr2
		}//esr1
	if(new_boundary_section<0)
	{
	RightGreenBoundarySection[count_fiber]=section_number+1;
	}
	else
	{
	RightGreenBoundarySection[count_fiber]=new_boundary_section+1;
	}
	RightAdjacentNonGreenSectionMax[count_fiber]=max_n_green_adj_section+1;
	RightAdjacentGreenSectionMax[count_fiber]=max_green_adj_section+1;//last_cor09.12.16
	LeftRightSectionNumberOfRightGreenBoundaryPoints[section_number]++;
	number_of_boundary_points++;
	previous_success=count_fiber;
	if(count_fiber>last_right_bound_fiber)
			{
		    last_right_bound_fiber=count_fiber;
			}
	if(count_fiber>UpperGreenBoundaryRight)
	{//last_cor17.11.16
	UpperGreenBoundaryRight=count_fiber;
	}
	}//sr>0
		}//lr
		else
		{//elr
	if(success_left>0)
	{//sl>0
		if(success_left==1)
		{//sl1
	LeftGreenBoundaryBunch[count_fiber]=bunch_old_number+1;
		}//sl1
		else
		{//esl1
			if(success_left==2)
		   {//sl2
	LeftGreenBoundaryBunch[count_fiber]=old_first_green_adjacent+1;
		   }//sl2
			else
			{//esl2
	         if(success_left==3)
		     {//sl3
	LeftGreenBoundaryBunch[count_fiber]=old_first_green_adjacent_lg+1;
		     }//sl3
			 else
			 {//last_cor19.04.17
				 if(success_left==4)
				 {
	LeftGreenBoundaryBunch[count_fiber]=
	ColorInt[count_fiber].old_ordered_bunch_number[last_gr]+1;
				 }
			 }
			}//esl2
		}//esl1

	LeftGreenBoundarySection[count_fiber]=section_number+1;
	LeftAdjacentNonGreenSectionMax[count_fiber]=max_n_green_adj_section+1;
	LeftAdjacentGreenSectionMax[count_fiber]=max_green_adj_section+1;//last_cor09.12.16
	LeftRightSectionNumberOfRightGreenBoundaryPoints[section_number]++;
	number_of_boundary_points++;
	previous_success=count_fiber;
	if(count_fiber>last_left_bound_fiber)
			{
		    last_left_bound_fiber=count_fiber;
			}
	if(count_fiber>UpperGreenBoundaryLeft)
	{//last_cor17.11.16
	UpperGreenBoundaryLeft=count_fiber;
	}
	}//sl>0
		}//elr
	}//secloop

	/*number_of_roof_green;
	if((last_roof_green>0)&&(number_of_roof_green>=2))
	{
	   if(left_right)
	   {
		if(last_roof_green>UpperGreenBoundaryLeft)
		{
	UpperGreenBoundaryLeft=last_roof_green;
		}
	   }

	}*/
	M:;

	if(section_length>1)
	{
		if(distance_to_green!=NULL)
		{
	delete [] distance_to_green;
		}
		if(green_bunch_number!=NULL)
		{
	delete [] green_bunch_number;
		}
		if(adjacent_n_green_length!=NULL)
		{
	delete [] adjacent_n_green_length;
		}
		if(green_section_number!=NULL)
		{
	delete [] green_section_number;
		}
		if(max_gr_adj_sec!=NULL)
		{
	delete [] max_gr_adj_sec;
		}
		if(max_gr_adj_sec_bunch!=NULL)
		{
	delete [] max_gr_adj_sec_bunch;
		}
		if(sec_bunches_ordered_numbers!=NULL)
		{
	delete [] sec_bunches_ordered_numbers;
		}
	if(first_n_g_bunch!=NULL)
		{
	delete [] first_n_g_bunch;
		}
		if(max_n_green_bunch!=NULL)
		{
	delete [] max_n_green_bunch;
		}

	if(green_bunch_number_oppos!=NULL)
		{
	delete [] green_bunch_number_oppos;
	    }
	if(adjacent_n_green_length_oppos!=NULL)
		{
	delete [] adjacent_n_green_length_oppos;
	    }
	if(green_section_number_oppos!=NULL)
		{
	delete [] green_section_number_oppos;
	    }
	if(max_gr_adj_sec_oppos!=NULL)
		{
	delete [] max_gr_adj_sec_oppos;
	    }
	if(max_gr_adj_sec_bunch_oppos!=NULL)
		{
	delete [] max_gr_adj_sec_bunch_oppos;
	    }
	if(max_n_green_bunch_oppos!=NULL)
		{
	delete [] max_n_green_bunch_oppos;
	    }
	}
	/*if(!left_right)
	{
		if(last_left_bound_fiber>UpperGreenBoundaryLeft)
		{//!!!!16
	UpperGreenBoundaryLeft=last_left_bound_fiber;
		}
	}
	else
	{
		if(last_right_bound_fiber>UpperGreenBoundaryRight)
		{
	UpperGreenBoundaryRight=last_right_bound_fiber;
		}
	}*/
	return(prior);
	}


//=========================================================
/*int

CImageProcess::Test_Junction_Sections(int sect,int adj_sect,int Intersection_Measure,
int indic_length,int ratio_length,int ratio_length1,
int *hue_close_r,int *sat_close_r,int* gray_close_r)
{
int sect_mean_hue;
int corrected_sect_mean_hue;
int neighbor_sect_mean_hue;
int corrected_neighbor_sect_mean_hue;
int neighbor_sect_minimal_hue;
int neighbor_sect_maximal_hue;
int sect_mean_saturation;
int sect_maximal_hue;
int sect_minimal_hue;
int corrected_sect_minimal_hue;
int corrected_sect_maximal_hue;
int neighbor_sect_mean_saturation;
int corrected_neighbor_sect_minimal_hue;
int corrected_neighbor_sect_maximal_hue;
int sect_mean_gray;
int min_hue_zone;//last_cor17.04.15
int max_hue_zone;
int neighbor_sect_mean_gray;
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
int sect_significance;
int neighbor_significance;
int Signif_ratio;
int sect_density;
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
int zone_hue_sect;
int difference_hue_zone;
int zone_gray;
int zone_gray_sect;
int difference_gray_zone;
int sect_minimal_gray;
int sect_maximal_gray;
int neighbor_minimum_gray;
int neighbor_maximum_gray;
int min_gray_dist;
int left_gray;
int right_gray;
int min_sect_gray_dist;
int left_sect_gray;
int right_sect_gray;
int sect_gray_length;
int neighbor_gray_length;
int wide_gray_gap;
int neighbor_sect_maximal_saturation;
int neighbor_sect_minimal_saturation;
int min_sat_dist;
int left_sat;
int right_sat;
int difference_hue_left;
int difference_hue_right;
int left_hue;
int right_hue;
int min_hue_dist;
int left_sect_hue;
int right_sect_hue;
int min_hue_sect_dist;
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
int sect_minimal_saturation;
int sect_maximal_saturation;
int left_sat_sect;
int right_sat_sect;
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


beg_int=ColorSection->section_fibre_first[sect];
end_int=ColorSection->section_fibre_last[sect];
int_length=end_int-beg_int+1;
neighbor_beg_int=ColorSection->section_fibre_first[adj_sect];
neighbor_end_int=ColorSection->section_fibre_last[adj_sect];
neighbor_length=neighbor_end_int-neighbor_beg_int+1;
if((neighbor_beg_int<=beg_int)&&(neighbor_end_int>=end_int))
{
inclusion_relation=1;
}
//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
	//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller
sect_significance=ColorSection->section_weight[sect];
neighbor_significance=ColorSection->section_weight[adj_sect];
Signif_ratio=(16*sect_significance)/(sect_significance+neighbor_significance);

sect_density=2*sect_significance/int_length;
neighbor_density=2*neighbor_significance/neighbor_length;
Density_ratio=(16*sect_density)/(sect_density+neighbor_density);
length_ratio=(16*int_length)/(int_length+neighbor_length);

node_vicinity=0;
node_vicinity_neighbor=0;
sect_mean_hue=ColorSection->section_mean_hue[sect];
if(sect_mean_hue>=NUM_HUES)
{
sect_mean_hue-=NUM_HUES;
}
sect_minimal_hue=ColorSection->DescrSec[sect].lower_hue;
if(sect_minimal_hue>=NUM_HUES)
{
sect_minimal_hue-=NUM_HUES;
}

sect_maximal_hue=ColorSection->DescrSec[sect].upper_hue;
if(sect_maximal_hue>=NUM_HUES)
{
sect_maximal_hue-=NUM_HUES;
}
zone_hue_sect=hue_zones[sect_mean_hue];
neighbor_sect_mean_hue=ColorSection->section_mean_hue[adj_sect];
if(neighbor_sect_mean_hue>=NUM_HUES)
{
neighbor_sect_mean_hue-=NUM_HUES;
}
zone_hue=hue_zones[neighbor_sect_mean_hue];
hue_bound=dev_hue_zone[zone_hue];
difference_hue_zone=abs(zone_hue_sect-zone_hue);
		  difference_hue_zone=std::min(difference_hue_zone,5-difference_hue_zone);

hue_dif=abs(sect_mean_hue-neighbor_sect_mean_hue);
hue_difference=std::min(hue_dif,NUM_HUES-hue_dif);
min_hue_zone=std::min(zone_hue_sect,zone_hue);//last_cor17.04.15
max_hue_zone=std::max(zone_hue_sect,zone_hue);
if(hue_dif>hue_difference)
{
	if(sect_mean_hue<neighbor_sect_mean_hue)
	{
		corrected_sect_mean_hue=sect_mean_hue+NUM_HUES;
		corrected_neighbor_sect_mean_hue=neighbor_sect_mean_hue;
	}
	else
	{
		if(sect_mean_hue>neighbor_sect_mean_hue)
	    {
		corrected_sect_mean_hue=sect_mean_hue;
		corrected_neighbor_sect_mean_hue=neighbor_sect_mean_hue+NUM_HUES;
	    }
	}
}
else
{
	corrected_sect_mean_hue=sect_mean_hue;
	corrected_neighbor_sect_mean_hue=neighbor_sect_mean_hue;
}

hue_dif=abs(sect_mean_hue-sect_maximal_hue);
right_sect_hue=std::min(hue_dif,NUM_HUES-hue_dif);
if(hue_dif>right_sect_hue)
{
corrected_sect_maximal_hue=sect_maximal_hue+NUM_HUES;
     if(corrected_sect_mean_hue<0)
	 {
	 corrected_sect_mean_hue=sect_mean_hue;
	 }

}
else
{
	corrected_sect_maximal_hue=sect_maximal_hue;
}

hue_dif=abs(sect_mean_hue-sect_minimal_hue);
left_sect_hue=std::min(hue_dif,NUM_HUES-hue_dif);
min_hue_sect_dist=std::min(left_sect_hue,right_sect_hue);
if(hue_dif>left_sect_hue)
{
    if(corrected_sect_mean_hue<0)
	{
corrected_sect_mean_hue=sect_mean_hue+NUM_HUES;
	}
}

neighbor_sect_minimal_hue=ColorSection->DescrSec[adj_sect].lower_hue;
if(neighbor_sect_minimal_hue>=NUM_HUES)
{
neighbor_sect_minimal_hue-=NUM_HUES;
}
neighbor_sect_maximal_hue=ColorSection->DescrSec[adj_sect].upper_hue;
if(neighbor_sect_maximal_hue>=NUM_HUES)
{
neighbor_sect_maximal_hue-=NUM_HUES;
}
hue_dif=abs(neighbor_sect_maximal_hue-neighbor_sect_mean_hue);
right_hue=std::min(hue_dif,NUM_HUES-hue_dif);
if(hue_dif>right_hue)
{
corrected_neighbor_sect_maximal_hue=neighbor_sect_maximal_hue+NUM_HUES;
if(corrected_neighbor_sect_mean_hue<0)
{
corrected_neighbor_sect_mean_hue=neighbor_sect_mean_hue;
}
}
hue_dif=abs(neighbor_sect_mean_hue-neighbor_sect_minimal_hue);
left_hue=std::min(hue_dif,NUM_HUES-hue_dif);
if(hue_dif>left_hue)
{
	if(corrected_neighbor_sect_mean_hue<0)
	{
corrected_neighbor_sect_mean_hue=neighbor_sect_mean_hue+NUM_HUES;
	}
}
min_hue_dist=std::min(left_hue,right_hue);
if(corrected_neighbor_sect_mean_hue<0)
{
corrected_neighbor_sect_mean_hue=neighbor_sect_mean_hue;
}
if(corrected_sect_mean_hue<0)
	{
corrected_sect_mean_hue=sect_mean_hue;
	}
hue_dif=abs(corrected_neighbor_sect_mean_hue-corrected_sect_mean_hue);
hue_dif1=std::min(hue_dif,NUM_HUES-hue_dif);
if(hue_dif1<hue_dif)
{
	if(corrected_neighbor_sect_mean_hue>corrected_sect_mean_hue)
	{
		corrected_sect_mean_hue+=NUM_HUES;
	}
	else
	{
    corrected_neighbor_sect_mean_hue+=NUM_HUES;
	}
}
if((neighbor_sect_minimal_hue<=neighbor_sect_mean_hue)&&(sect_minimal_hue<=sect_mean_hue))
{//cond0
corrected_neighbor_sect_mean_hue=neighbor_sect_mean_hue;
corrected_sect_mean_hue=sect_mean_hue;
}//cond0
else
{//econd0
if(neighbor_sect_minimal_hue>neighbor_sect_mean_hue)
{//cond1
hue_dif=neighbor_sect_minimal_hue-neighbor_sect_mean_hue;
	   if(hue_dif>(NUM_HUES-hue_dif))
	   {//cond2
corrected_neighbor_sect_mean_hue=neighbor_sect_mean_hue+NUM_HUES;
if(sect_mean_hue<NUM_HUES/2)
{//last_cor21.05.15
	corrected_sect_mean_hue=sect_mean_hue+NUM_HUES;
}//last_cor21.05.15
	   }//cond2
	   else
	   {//econd2
       corrected_sect_mean_hue=sect_mean_hue;
	   }//econd2

}//cond1
//else
//{//econd1
	      if(sect_minimal_hue>sect_mean_hue)
	      {//last_cor21.05.15
hue_dif=sect_minimal_hue-sect_mean_hue;
    if(hue_dif>(NUM_HUES-hue_dif))
	{//cond3
corrected_sect_mean_hue=sect_mean_hue+NUM_HUES;
if(neighbor_sect_mean_hue<NUM_HUES/2)
{//last_cor21.05.15
	corrected_neighbor_sect_mean_hue=neighbor_sect_mean_hue+NUM_HUES;
}//last_cor21.05.15
    }//cond3
	else
	{//econd3
corrected_sect_mean_hue=sect_mean_hue;
	}//econd3

		  }//last_cor21.05.15

//}//econd1

}//econd0
left_sect_hue=corrected_sect_mean_hue-min_hue_sect_dist;
right_sect_hue=corrected_sect_mean_hue+min_hue_sect_dist;

left_hue=corrected_neighbor_sect_mean_hue-min_hue_dist;
right_hue=corrected_neighbor_sect_mean_hue+min_hue_dist;
difference_hue_left=abs(corrected_sect_mean_hue-left_hue);
difference_hue_left=std::min(difference_hue_left,NUM_HUES-difference_hue_left);
difference_hue_right=abs(corrected_sect_mean_hue-right_hue);
difference_hue_right=std::min(difference_hue_right,NUM_HUES-difference_hue_right);
Intersection_hue=ColorInt[0].Intersection_measure_ratios(left_sect_hue,right_sect_hue,
left_hue,right_hue,&indic_length_hue,&ratio_length_hue,&ratio_length1_hue);
coinsidence1_hue=std::max(ratio_length_hue,ratio_length1_hue);
joint_hue_range=std::max(right_sect_hue,right_hue)-std::min(left_sect_hue,left_hue);
if(Intersection_hue==3)
{
ratio_length_hue=0;
//ratio_length1_hue=0;
}
if((hue_difference<=1)||((hue_difference<=2)&&((coinsidence1_hue>=9)||(joint_hue_range<=6))))
{
	hue_close=1;
}
else
{
if(((difference_hue_zone>=1)&&(hue_difference>=10))||((Intersection_hue==3)&&(ratio_length_hue>=8)))
	{//last_cor08.12.15
		hue_close=-2;
	}
}
if(((sect_mean_hue>=46)&&(sect_mean_hue<48))||
((sect_mean_hue>=0)&&(sect_mean_hue<=2))||
((sect_mean_hue>=30)&&(sect_mean_hue<=34))||
((sect_mean_hue>=12)&&(sect_mean_hue<=20)))
{
node_vicinity=1;
}
if(((neighbor_sect_mean_hue>=46)&&(neighbor_sect_mean_hue<=48))||
((neighbor_sect_mean_hue>=0)&&(neighbor_sect_mean_hue<=2))||
((neighbor_sect_mean_hue>=30)&&(neighbor_sect_mean_hue<=34))||
((neighbor_sect_mean_hue>=12)&&(neighbor_sect_mean_hue<=20)))
{
node_vicinity_neighbor=1;
}
sect_mean_saturation=ColorSection->section_mean_saturation[sect];
sect_minimal_saturation=ColorSection->DescrSec[sect].lower_sat;
sect_maximal_saturation=ColorSection->DescrSec[sect].upper_sat;
min_sat_dist=std::min(sect_maximal_saturation-sect_mean_saturation,sect_mean_saturation-sect_minimal_saturation);
left_sat_sect=sect_mean_saturation-min_sat_dist;
right_sat_sect=min_sat_dist+sect_mean_saturation;
neighbor_sect_mean_saturation=
ColorSection->section_mean_saturation[adj_sect];
max_saturation=std::max(sect_mean_saturation,neighbor_sect_mean_saturation);
min_saturation=std::min(sect_mean_saturation,neighbor_sect_mean_saturation);
//!?
neighbor_sect_minimal_saturation=ColorSection->DescrSec[adj_sect].lower_sat;
neighbor_sect_maximal_saturation=ColorSection->DescrSec[adj_sect].upper_sat;
min_sat_dist=std::min(neighbor_sect_maximal_saturation-neighbor_sect_mean_saturation,
				 neighbor_sect_mean_saturation-neighbor_sect_minimal_saturation);
left_sat=neighbor_sect_mean_saturation-min_sat_dist;
right_sat=min_sat_dist+neighbor_sect_mean_saturation;
abs_satur=abs(sect_mean_saturation-left_sat);
abs_satur1=abs(sect_mean_saturation-right_sat);
abs_satur=std::min(abs_satur,abs_satur1);
possible_sat_variation=dev_mean_sat[neighbor_sect_mean_saturation];
possible_sat_variation1=dev_mean_sat1[neighbor_sect_mean_saturation];
impossible_sat_variation=imp_dev_mean_sat[neighbor_sect_mean_saturation];
impossible_sat_variation1=imp_dev_mean_sat1[neighbor_sect_mean_saturation];
if(neighbor_sect_mean_saturation>=sect_mean_saturation)
	{//last_cor11.09.13
	saturation_distance=neighbor_sect_mean_saturation-sect_mean_saturation;
	saturation_distance1=0;
	poss_sat_variation=dev_mean_sat[sect_mean_saturation];
	poss_sat_variation=std::max(possible_sat_variation1,poss_sat_variation);
	imp_sat_var=imp_dev_mean_sat[sect_mean_saturation];
		imp_sat_var=std::max(impossible_sat_variation1,imp_sat_var);//last_cor08.12.15
	}
	else
	{
	saturation_distance1=sect_mean_saturation-neighbor_sect_mean_saturation;
	saturation_distance=0;
	poss_sat_variation=dev_mean_sat1[sect_mean_saturation];
		poss_sat_variation=std::max(poss_sat_variation,possible_sat_variation);
	imp_sat_var=imp_dev_mean_sat1[sect_mean_saturation];
		imp_sat_var=std::max(imp_sat_var,impossible_sat_variation);
	}
max_satur_differ=std::max(saturation_distance,saturation_distance1);
saturation_difference=
abs(sect_mean_saturation-neighbor_sect_mean_saturation);//last_cor08.12.15
    Intersection_saturation=ColorInt[0].Intersection_measure_ratios(left_sat,right_sat,left_sat_sect,
	right_sat_sect,&indic_length_satur,&ratio_length_satur,&ratio_length1_satur);
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
		if(saturation_difference>imp_sat_var)
		{
			saturation_close=-2;
		}
	}//last_cor08.12.15
	if(Intersection_saturation==3)
	{
		ratio_length_satur=0;//???
	}
sect_mean_gray=ColorSection->section_mean_gray[sect];
sect_minimal_gray=ColorSection->DescrSec[sect].lower_gray;
sect_maximal_gray=ColorSection->DescrSec[sect].upper_gray;
min_sect_gray_dist=std::min(sect_maximal_gray-sect_mean_gray,sect_mean_gray-sect_minimal_gray);
left_sect_gray=sect_mean_gray-min_sect_gray_dist;
right_sect_gray=min_sect_gray_dist+sect_mean_gray;
sect_gray_length=2*min_sect_gray_dist+1;
zone_gray_sect=gray_zones[sect_mean_gray];
neighbor_sect_mean_gray=
ColorSection->section_mean_gray[adj_sect];
neighbor_minimum_gray=ColorSection->DescrSec[adj_sect].lower_gray;
neighbor_maximum_gray=ColorSection->DescrSec[adj_sect].upper_gray;
min_gray_dist=std::min(neighbor_maximum_gray-neighbor_sect_mean_gray,neighbor_sect_mean_gray-neighbor_minimum_gray);
left_gray=neighbor_sect_mean_gray-min_gray_dist;
right_gray=min_gray_dist+neighbor_sect_mean_gray;
union_gray_left=std::min(left_sect_gray,left_gray);
union_gray_right=std::max(right_sect_gray,right_gray);
union_gray_length=union_gray_right-union_gray_left;
zone_gray=gray_zones[neighbor_sect_mean_gray];
neighbor_gray_length=2*min_gray_dist+1;
difference_gray_zone=abs(zone_gray_sect-zone_gray);
possible_gray_variation=std::min(dev_mean[neighbor_sect_mean_gray/8],dev_mean[sect_mean_gray/8]);
impossible_gray_variation=std::min(imp_dev_mean[neighbor_sect_mean_gray/8],
imp_dev_mean[sect_mean_gray/8]);//last_cor5.07.13
maximum_zone_gray=std::max(zone_gray,zone_gray_sect);
minimum_zone_gray=std::min(zone_gray,zone_gray_sect);

gray_ntersection=ColorInt[0].Intersection_measure_ratios(left_gray,right_gray,left_sect_gray,
	right_sect_gray,&indic_length_gray,&ratio_length_gray,&ratio_length1_gray);
gray_ntersection1=ColorInt[0].Intersection_measure_ratios(left_gray,right_gray,left_sect_gray,
	sect_mean_gray,&indic_length_gray1,&ratio_length_gray1,&ratio_length1_gray1);
gray_ntersection2=ColorInt[0].Intersection_measure_ratios(left_gray,right_gray,sect_mean_gray,
	right_sect_gray,&indic_length_gray2,&ratio_length_gray2,&ratio_length1_gray2);
if(gray_ntersection==3)
{//intersection is empty
	if(union_gray_length>(possible_gray_variation+(impossible_gray_variation-possible_gray_variation)/2))
	{
	if(((ratio_length_gray+1)>=sect_gray_length)||((ratio_length_gray+1)>=neighbor_gray_length))
	{
	wide_gray_gap=1;
	}
    }
}

	//intersection1=0 very strong intersection; 1, strong; 2, only exist; 3, does not exist
	//indic_length=3, first much greater; 2, greater, 1, smaller, but not much, 0, much smaller





gray_difference=abs(sect_mean_gray-neighbor_sect_mean_gray);

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
   (((sect_mean_gray>=left_gray)&&(sect_mean_gray<=right_gray))||
   ((neighbor_sect_mean_gray>=left_sect_gray)&&(neighbor_sect_mean_gray<=right_sect_gray))))
		    {//gr2
		    grayscale_close=1;
			}//gr2
else
{
	if((gray_difference<=possible_gray_variation)&&
		((((sect_mean_gray>=left_gray)&&(sect_mean_gray<=right_gray))||
((neighbor_sect_mean_gray>=left_sect_gray)&&(neighbor_sect_mean_gray<=right_sect_gray)))))
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
		(((sect_mean_gray>=left_gray)&&(sect_mean_gray<=right_gray))||
((neighbor_sect_mean_gray>=left_sect_gray)&&(neighbor_sect_mean_gray<=right_sect_gray))))
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
				   if((Density_ratio>=6)&&(Density_ratio<=10))
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
							   (((sect_mean_gray<left_gray)||(sect_mean_gray>right_gray))&&
	  ((neighbor_sect_mean_gray<left_sect_gray)||(neighbor_sect_mean_gray>right_sect_gray))))
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
	if((sect_mean_gray<left_gray)||(sect_mean_gray>right_gray)||(ratio_length_gray<3)||
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
gray_ntersection3=ColorInt[0].Intersection_measure_ratios(sect_minimal_gray,
sect_maximal_gray,neighbor_minimum_gray,neighbor_maximum_gray,&indic_length_gray3,
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
if((hue_close<0)&&(saturation_close<0)&&(grayscale_close!=0))
{//last_cor08.12.15
	prior=5;
	goto L;
}
if((hue_close<0)&&(saturation_close<0)&&(grayscale_close!=0))
{//last_cor08.12.15
	prior=5;
	goto L;
}
if((sect_mean_saturation==0)&&(neighbor_sect_mean_saturation==0))
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
if((sect_mean_saturation<=1)&&(neighbor_sect_mean_saturation<=1))
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
	if((zone_gray<=1)||(zone_gray_sect<=1))
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
if((zone_gray<=2)||(zone_gray_sect<=2))//last_cor 21.04.13
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
	if(((zone_gray<=4)||(zone_gray_sect<=4))&&(saturation_close==1))
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
   ((zone_hue_sect>2)&&(zone_hue_sect<5))))
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
		if((zone_gray>=5)&&(zone_gray_sect>=5))
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
                         if((zone_hue>2)||(zone_hue_sect>2))
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
			if((!sect_mean_saturation)||(!neighbor_sect_mean_saturation))
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
                         if((zone_hue>2)||(zone_hue_sect>2))
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

		  }
      }//e1
if((grayscale_close==1)&&((!neighbor_sect_mean_saturation)||
		  (!sect_mean_saturation)))
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

      }//e1

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
if((sect_mean_saturation<=2)&&(neighbor_sect_mean_saturation<=2))
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
	if((saturation_difference==0)||((sect_mean_saturation>=1)&&(neighbor_sect_mean_saturation>=1)&&
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
				if((zone_gray>=2)&&(zone_gray_sect>=2))
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
if((difference_hue_zone>=1)&&(hue_difference>=4)&&((zone_gray>=3)||(zone_gray_sect>=3)))
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
		if((zone_gray<=1)||(zone_gray_sect<=1))//last_cor 8.04.13
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
if((zone_gray<=2)||(zone_gray_sect<=2))//last_cor 21.04.13
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
	if(((zone_gray<=4)||(zone_gray_sect<=4))&&(saturation_close==1))
	{//zg4

		if(hue_close<=0)
		{//hc
			if((difference_hue_zone<=1)&&(((zone_hue>2)&&(zone_hue<5))||
   ((zone_hue_sect>2)&&(zone_hue_sect<5))))
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
					    if((zone_gray>=6)&&(zone_gray_sect>=6)&&(saturation_close==1)&&(grayscale_close==1)&&
   (gray_ntersection<=1)&&(3*gray_difference<=possible_gray_variation))
{//last_cor18.06.14
	hue_close=3;//last_cor18.01.13
prior=2;
goto L;
}

					    }//dhz6
					}//edhz4
				}//dhz0

			}//ep2
		}//ep1
	}
if((!gray_ntersection)&&(gray_difference<=2)&&(2*gray_difference<possible_gray_variation)&&
		(difference_hue_zone<=1)&&(zone_gray<=4)&&(zone_gray_sect<=4))
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
if((zone_gray<=3)&&(zone_gray_sect<=3))
		{//last_cor11.04.14
			if(maximum_zone_gray<=1)
			{//last_cor14.04.14

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
	if(std::max(zone_hue_sect,zone_hue)==4)
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
			if((zone_gray<=3)&&(zone_gray_sect<=3))
			{
if(std::min(zone_gray,zone_gray_sect)<=1)
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
					if(((zone_hue_sect==3)||(zone_hue==3))&&(difference_hue_zone<=1))
					{
						if(hue_difference<=3)
						{
                        hue_close=1;
						prior=1;
						goto L;
						}
						else
						{
							if((difference_hue_zone==1)&&((zone_hue_sect==2)||(zone_hue==2)))
					{//last_cor18.02.15
						if((sect_mean_hue<=5)||(neighbor_sect_mean_hue<=5))
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
if((zone_gray<=4)&&(zone_gray_sect<=4))
		{//last_cor27.03.14
			if(hue_difference<=2)
{//last_cor09.04.14
	hue_close=1;
}
			if(max_saturation<=4)
			{//ms
			if(!hue_close)
			{//hc
if((zone_hue>1)&&(zone_hue<4)&&(zone_hue_sect>1)&&(zone_hue_sect<4)&&
   (std::min(neighbor_sect_mean_hue,sect_mean_hue)>5))
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
if((zone_gray>=1)&&(zone_gray_sect>=1))
{//last_cor07.03.14

	if((zone_gray>=2)||(zone_gray_sect>=2))
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
						  if(((zone_hue_sect>=1)&&(zone_hue_sect<=2))
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
	if(((zone_gray>=2)&&((neighbor_sect_mean_saturation>5)))||
		((zone_gray_sect>=2)&&(sect_mean_saturation>5)))
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
if((!zone_gray)||(!zone_gray_sect))
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

	}//gc2
	}//egs
}//!zg
if((zone_gray<=1)||(zone_gray_sect<=1))
	    {//zgb1-16.04.13
			if(std::max(zone_gray,zone_gray_sect)>=2)
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
			if((zone_gray>1)||(zone_gray_sect>1))
			{//last_cor11.09.13
        saturation_close=-1;
			}
		}
	}
			}//last_cor12.06.13

				if(hue_close==1)
				{
					if((saturation_close>0)||((zone_gray<=1)&&(zone_gray_sect<=1)))
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
if((zone_gray<=3)||(zone_gray_sect<=3))
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
					if((zone_hue>1)&&(zone_hue<4)&&(zone_hue_sect>1)&&(zone_hue_sect<4))
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
if((zone_hue>1)&&(zone_hue<4)&&(zone_hue_sect>1)&&(zone_hue_sect<4))
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
			if(((zone_hue!=2)&&(zone_hue!=1))||(zone_hue_sect!=2)&&(zone_hue_sect!=1))
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
	    if((zone_gray>=2)&&(zone_gray_sect>=2))
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
		(((sect_mean_saturation>=left_sat)&&(sect_mean_saturation<=right_sat))||(abs_satur<=1)))
	 {//s
     saturation_close=1;
	 }//s
else
{//e=1
if((sect_mean_saturation>=8)&&(neighbor_sect_mean_saturation>=8)&&(zone_gray_sect<=2)&&
		(zone_gray<=2))
	{
		saturation_close=2;
	}
	else
	{
    if(((saturation_distance1<=possible_sat_variation)&&(saturation_distance<=possible_sat_variation1))||
		((sect_mean_saturation>=left_sat)&&(sect_mean_saturation<=right_sat)&&
		(neighbor_sect_mean_saturation>=left_sat_sect)&&
		(neighbor_sect_mean_saturation<=right_sat_sect)))
     {//last_cor10.07.13
		 saturation_close=2;
     }
     else
    {
		if(((saturation_distance1<=possible_sat_variation+1)&&(saturation_distance<=possible_sat_variation1+1))&&
		((sect_mean_saturation>=left_sat)&&(sect_mean_saturation<=right_sat)
		&&(neighbor_sect_mean_saturation>=left_sat_sect)&&
		(neighbor_sect_mean_saturation<=right_sat_sect)))
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
		if(zone_hue_sect==3)
		{//green
			if(zone_gray_sect<=2)
						 {//last_cor03.04.15
					 if(gray_ntersection<2)
					 {
							 if(zone_gray_sect<=1)
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
		((sect_mean_saturation>=left_sat)&&(sect_mean_saturation<=right_sat)))
     {
		 saturation_close=2;
     }
					 }
					 if(!grayscale_close)
					 {
						 if(zone_gray_sect<=2)
						 {
							 grayscale_close=3;
						 }
					 }
				 }//closeness correction

		if((((!difference_hue_zone)&&(hue_difference<=8))||((hue_difference<=6)&&(difference_hue_zone<=1))||
			((left_hue<=corrected_sect_mean_hue)&&(corrected_sect_mean_hue<=right_hue))))
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

}
else
{
     if((hue_close>0)&&(hue_close<=2)&&(grayscale_close>=1)&&(grayscale_close<=2)&&(difference_gray_zone<=1))
	 {//last_cor20.05.15

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
		if((zone_hue_sect==3)&&(!difference_hue_zone))
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
			((left_hue<=corrected_sect_mean_hue)&&(corrected_sect_mean_hue<=right_hue)))
         {//h2
			if(((zone_hue_sect!=2)&&(zone_hue!=2))||((zone_hue_sect==2)&&(zone_hue!=1))||
((zone_hue_sect!=1)&&(zone_hue==2)))
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
		((sect_mean_saturation>=left_sat)&&(sect_mean_saturation<=right_sat)))
     {
		 saturation_close=2;
     }
					 }
					 if(!grayscale_close)
					 {
						 if(zone_gray_sect<=2)
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
			if((Density_ratio>=6)&&(Density_ratio<=10))
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
	if((hue_close==1)&&(std::min(sect_mean_saturation,neighbor_sect_mean_saturation)>=3)&&
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
if((Intersection_hue==3)&&(Intersection_saturation==3)&&(zone_gray>2)&&((zone_gray_sect>2)))
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

	*hue_close_r=hue_close;
	*sat_close_r=saturation_close;
	*gray_close_r=grayscale_close;
return(prior);


}*/
//=========================================================
int

CImageProcess::NextGreenComponents(int left_right,int oppos_bunch_number,int strip_number,
int start_bunch_old_num,int sec_num,int left_side_boun,int right_side_boun,
int* first_green_neighbor,int* last_green_neighbor,int* maximum_non_green,int* non_gr_length,
int* first_non_green,int* last_non_green,int* last_green_in_chain,int* first_green_length,
int* max_green_section,int* max_green_section_bunch,int* max_green_oppos_section,
int* max_green_oppos_section_bunch,int* max_weight_n_green_section,
int* max_weight_n_green_section_bunch,int* max_weight_n_green_section_oppos,
int* max_weight_n_green_section_oppos_bunch,int* gr_length_before,int shift_to_next)
{
	int prior;
		int bunch_first;
		int bunch_last;
		int disorder;
		int new_bunch_number;
		int old_bunch_number;
		int old_bunch_hue;
		int old_bunch_saturation;
		int old_bunch_gray;
		int old_start_bunch_hue;
		int old_start_bunch_saturation;
		int old_start_bunch_gray;
		int old_buch_beg;
		int old_bunch_end;
		int old_bunch_length;
		int maximum_length_bunch;
		int maximum_length;
		int maximum_non_green_bunch_length;
		int maximum_non_green_bunch;
		int non_green_feature;
		int old_bunch_hue_zone;
		int old_bunch_gray_zone;
		int old_start_bunch_hue_zone;
		int old_start_bunch_gray_zone;
		int first_green_adjacent;
		int test_junction;
		int hue_close;
		int saturation_close;
		int gray_close;
		int bunch_occurrence;
		int Intersection;
		int indic_length;
		int ratio_length;
		int ratio_length1;
		int last_green_adjacent;
		int non_green_total_length;
		int green_chain;
		int last_green_adjacent_in_chain;
		int belongs_to_green;
	    int belongs_to_yellow;
		int green_first_length;
		int first_n_green_adjacent;
		int last_n_green_adjacent;
		int belongs_to_green_start;
		int belongs_to_yellow_start;
		int green_section_max_weight;
		int m_green_section;
		int m_green_section_bunch;
		int green_section_oppos_max_weight;
	    int m_green_section_oppos;
		int m_green_section_oppos_bunch;
		int m_n_green_sec_weight;
		int m_n_green_section_in_weight;
		int m_n_green_sec_weight_bunch;
		int m_n_green_sec_oppos_weight;
	    int m_n_green_section_oppos_in_weight;
	    int m_n_green_sec_oppos_weight_bunch;
		int bunch_section;
		int bunch_section_oppos;
		int bunch_section_sat;
		int bunch_section_oppos_sat;
		int bunch_section_hue;
	    int bunch_section_gray;
		int bunch_section_incline;
		int bunch_section_hue_zone;
	    int bunch_section_gray_zone;
		int bunch_section_weight;
		int bunch_section_oppos_hue;
	    int bunch_section_oppos_gray;
		int bunch_section_oppos_incline;
	    int bunch_section_oppos_hue_zone;
	    int bunch_section_oppos_gray_zone;
		int bunch_section_oppos_weight;
		int sec_sky;
		int sec_oppos_sky;
		int initial_bunch_section_sat;
		int initial_bunch_section_hue;
	    int initial_bunch_section_gray;
		int initial_bunch_section_hue_zone;
	    int initial_bunch_section_gray_zone;
		int initial_section_incline;
		int initial_section_vert;
		int initial_section_label;
		int bunch_geom_feature;
	    int bunch_section_fiber_first;
	    int bunch_section_fiber_last;
		int fiber_first_ratio;
		int fiber_last_ratio;
		int distant_green;
		int bunch_section_fiber_first_oppos;
	    int bunch_section_fiber_last_oppos;
		int fiber_first_ratio_oppos;
		int fiber_last_ratio_oppos;
		int distant_green_oppos;
		int bunch_sec_incl_curve;
		int bunch_sec_incl_curve_oppos;
		int bound_bunch_gr_oppos;
		int bound_sec_gr_oppos;
		int bound_sec_n_gr_oppos;
		int bound_bunch_beg;
		int bound_bunch_end;
		int bound_sec_gr_oppos_bunch;
		int bound_sec_gr_oppos_bunch_beg;
		int bound_sec_gr_oppos_bunch_end;
	    int bound_sec_n_gr_oppos_bunch;
		int	bound_sec_n_gr_oppos_bunch_beg;
	    int bound_sec_n_gr_oppos_bunch_end;
		int number_of_sections_left;
		int final_bunch;
		int pre_final_bunch;
		int number_bound_oppos;
		int mean_lower_sec;
	    int mean_upper_sec;
	    int upper_lower_ratio_sec;
		int mean_lower_sec_oppos;
	    int mean_upper_sec_oppos;
	    int upper_lower_ratio_sec_oppos;
		int sec_non_fit;
		int sec_non_fit_oppos;
		int bunch_section_fiber_length;
		int bunch_section_fiber_length_oppos;
		int bunch_section_weight_normalized;
		int bunch_section_weight_normalized_oppos;
		int shift_in_array;
		int shift_in_mem;
		int color_object;
		int bunch_sec_vert_curve;
		int bunch_sec_vert_curve_oppos;
		int height_width_ratio;
		int height_width_ratio_oppos;
		int final_section;
		int sect_dist;
		int sect_dist_opt;
		int first_old;
		int first_old_beg;
		int first_old_end;
		int incline_opt;
		int incl_curve_opt;
		int green_length_tot;//last_cor10.03.17
		int green_length_before;//last_cor23.03.17
		int green_length_before_oppos;
		int green_dist;
		int next_gr_sec;
		int next_gr_sec_oppos;
		int m_n_green_sec_weight_bunch_beg;
	    int m_n_green_sec_weight_bunch_end;
		int max_n_dist;

		max_n_dist=-1;
		next_gr_sec_oppos=-1;
		next_gr_sec=-1;
		green_dist=0;//last_cor23.03.17
		green_length_before=0;
		green_length_before_oppos=0;
		green_length_tot=0;//last_cor10.03.17
		incline_opt=-1;
		 incl_curve_opt=-1;
		 sect_dist_opt=-1;
		final_section=-1;
		height_width_ratio_oppos=-1;
		height_width_ratio=-1;
		color_object=0;
		bunch_section_weight_normalized=-1;
		bunch_section_weight_normalized_oppos=-1;
		bunch_section_fiber_length=-1;
		bunch_section_fiber_length_oppos=-1;
		final_bunch=-1;
		bound_sec_n_gr_oppos_bunch=-1;
			bound_sec_n_gr_oppos_bunch_beg=-1;
	        bound_sec_n_gr_oppos_bunch_end=-1;
		bound_sec_gr_oppos_bunch=-1;
		bound_sec_gr_oppos_bunch_beg=-1;
		bound_sec_gr_oppos_bunch_end=-1;
		bound_bunch_beg=-1;
	    bound_bunch_end=-1;
		bound_sec_n_gr_oppos=-1;
		bound_bunch_gr_oppos=-1;
		bound_sec_gr_oppos=-1;
		sec_sky=-1;
	    sec_oppos_sky=-1;
		m_n_green_sec_weight=0;
	    m_n_green_sec_weight_bunch=-1;
		m_n_green_section_in_weight=-1;
		m_green_section_bunch=-1;
		m_n_green_sec_oppos_weight=0;
	    m_n_green_section_oppos_in_weight=-1;
	    m_n_green_sec_oppos_weight_bunch=-1;
		//oppos_bunch=-1;
		//green_section=-1;
		//green_section_weight=0;
		green_section_max_weight=0;
		m_green_section=-1;
		m_green_section_oppos=-1;
		green_section_oppos_max_weight=0;
		m_green_section_oppos_bunch=-1;
		belongs_to_yellow_start=0;
		belongs_to_green_start=0;
		first_n_green_adjacent=-1;
		last_n_green_adjacent=-1;
		green_first_length=0;
		belongs_to_green=0;
		belongs_to_yellow=0;
		last_green_adjacent_in_chain=-1;
		green_chain=0;
		non_green_total_length=0;
		last_green_adjacent=-1;
		first_green_adjacent=-1;
		Intersection=3;
	    indic_length=1;
	    ratio_length=0;
	    ratio_length1=0;
		hue_close=-1;
		saturation_close=-1;
		gray_close=-1;
		maximum_length=0;
		maximum_length_bunch=-1;
		maximum_non_green_bunch_length=0;
	    maximum_non_green_bunch=-1;
		prior=0;
		disorder=ColorInt[strip_number].Disordering;
		shift_in_array=strip_number*MaximumNumberOfCoveringElements;
		shift_in_mem=strip_number*MAX_COL_INT;
		if(!disorder)
		{//do
			prior=1;
		bunch_first=oppos_bunch_number;
	    first_old=ColorInt[strip_number].old_ordered_bunch_number[bunch_first];
		first_old_beg=ColorInt[strip_number].ColoredIntervalsStructure->BegInterv[first_old];
		first_old_end=ColorInt[strip_number].ColoredIntervalsStructure->EndInterv[first_old];
	if(!left_right)
	{
		final_bunch=ColorInt[strip_number].NumberOfIntervalsInCovering;
		bunch_last=final_bunch-1;
	}
	else
	{
	bunch_last=2*bunch_first;
	}

	number_of_sections_left=
	ColorSection->Number_of_sections_left;
	initial_bunch_section_sat=ColorSection->section_mean_saturation[ sec_num];
	initial_bunch_section_hue=ColorSection->section_mean_hue[ sec_num];
	initial_bunch_section_gray=ColorSection->section_mean_gray[ sec_num];
	initial_bunch_section_hue_zone=hue_zones[initial_bunch_section_hue];
	initial_bunch_section_gray_zone=gray_zones[initial_bunch_section_gray];
	initial_section_incline=InclinedComponent[sec_num];
	initial_section_vert=VerticalComponent[sec_num];
	initial_section_label=(SkyGreenComponents[sec_num])%10;
	old_start_bunch_hue=
	ColorInt[strip_number].ColoredIntervalsStructure->AverageHue[start_bunch_old_num];
	old_start_bunch_hue_zone=hue_zones[old_start_bunch_hue];
	old_start_bunch_saturation=
	ColorInt[strip_number].ColoredIntervalsStructure->AverageSat[start_bunch_old_num];
	old_start_bunch_gray=
	ColorInt[strip_number].ColoredIntervalsStructure->AverageGray[start_bunch_old_num];
	old_start_bunch_gray_zone=gray_zones[old_start_bunch_gray];
	if((old_start_bunch_hue>=8)&&(old_start_bunch_hue<=17))
	{//sh817
		if(old_start_bunch_saturation>=6)
		{//secs6
			     if(old_start_bunch_gray_zone<=5)
				 {//grz5
			if(old_start_bunch_gray_zone>=2)
			{//osz2
				belongs_to_green_start=1;
			}//osz2
			else
			{//eosz2
	if(old_start_bunch_gray_zone>=1)
	{
	belongs_to_green_start=2;
	}
	else
	{//eosz1
		if(!old_start_bunch_gray_zone)
		{
			if(old_start_bunch_gray>=5)
			{
			belongs_to_green_start=4;
			}
		}
	}//eosz1
			}//osz2
				 }//grz5
		}//secs6
		else
		{//esecs6
	if(old_start_bunch_saturation>=3)
	{

		if((old_start_bunch_gray_zone>=1)&&(old_start_bunch_gray_zone<=3))
		{
			belongs_to_green_start=3;
		}
	}

		}//esecs6
	}//sh817
	else
	{//esh817
		if((old_start_bunch_hue>=5)&&(old_start_bunch_hue<=7))
		{
	      if((old_start_bunch_gray_zone>=2)&&(old_start_bunch_gray_zone<=4))
	      {
			  if(old_start_bunch_saturation>=6)
			  {
				  belongs_to_yellow_start=1;
			  }
			  else
			  {
	             if(old_start_bunch_saturation>=3)
			     {
				 belongs_to_yellow_start=1;
			     }
				 else
				 {
					 if(old_start_bunch_saturation>=2)
			         {
				     belongs_to_yellow_start=2;
			         }
				 }
			  }
		  }
		}
	}//esh817
	if(sec_num>=number_of_sections_left)
	{
	bound_bunch_gr_oppos=RightGreenBoundaryBunch[strip_number];
	final_section=RightGreenBoundarySection[strip_number];
	if(final_section>0)
	{
	final_section--;
	}
	if(bound_bunch_gr_oppos>0)
	{
		bound_bunch_gr_oppos--;
		pre_final_bunch=
	ColorInt[strip_number].ordered_bunch_number[bound_bunch_gr_oppos];
		if(pre_final_bunch>0)
		{
			final_bunch=pre_final_bunch;
		}
	bound_bunch_beg=
	ColorInt[strip_number].ColoredIntervalsStructure->BegInterv[bound_bunch_gr_oppos];
	bound_bunch_end=
	ColorInt[strip_number].ColoredIntervalsStructure->EndInterv[bound_bunch_gr_oppos];
	//}
	bound_sec_gr_oppos=RightGreenBoundarySection[strip_number];
	if(bound_sec_gr_oppos>0)
	{
		bound_sec_gr_oppos--;
	bound_sec_gr_oppos_bunch=
	ColorDescrSect[bound_sec_gr_oppos].location_of_section[strip_number];
	  if(bound_sec_gr_oppos_bunch>0)
	  {
	 bound_sec_gr_oppos_bunch-=1;
	 bound_sec_gr_oppos_bunch=ColorInt[strip_number].old_bunch_number[bound_sec_gr_oppos_bunch];
	 bound_sec_gr_oppos_bunch_beg=
	ColorInt[strip_number].ColoredIntervalsStructure->BegInterv[bound_sec_gr_oppos_bunch];
	bound_sec_gr_oppos_bunch_end=
	ColorInt[strip_number].ColoredIntervalsStructure->EndInterv[bound_sec_gr_oppos_bunch];
	  }
	}
	bound_sec_n_gr_oppos=RightAdjacentNonGreenSectionMax[strip_number];
	 if(bound_sec_n_gr_oppos>0)
	 {
	 bound_sec_n_gr_oppos--;
	 bound_sec_n_gr_oppos_bunch=
	ColorDescrSect[bound_sec_n_gr_oppos].location_of_section[strip_number];
	    if(bound_sec_n_gr_oppos_bunch>0)
	    {
			bound_sec_n_gr_oppos_bunch--;
	bound_sec_n_gr_oppos_bunch=ColorInt[strip_number].old_bunch_number[bound_sec_n_gr_oppos_bunch];
	bound_sec_n_gr_oppos_bunch_beg=
	ColorInt[strip_number].ColoredIntervalsStructure->BegInterv[bound_sec_n_gr_oppos_bunch];
	bound_sec_n_gr_oppos_bunch_end=
	ColorInt[strip_number].ColoredIntervalsStructure->EndInterv[bound_sec_n_gr_oppos_bunch];
		}
	 }
	}
	}
	if((shift_to_next+bunch_first)<=bunch_last)
	{
		bunch_first+=shift_to_next;
	}
	bunch_occurrence=ColorInt[strip_number].visible_bunches[start_bunch_old_num];
	      m_n_green_sec_weight_bunch_beg=0;
	      m_n_green_sec_weight_bunch_end=0;
	for(int count_bunch=bunch_first;count_bunch<=bunch_last;count_bunch++)
	  {//secloop

		  sect_dist=-1;
		  upper_lower_ratio_sec_oppos=-1;
		  upper_lower_ratio_sec=-1;
		  bunch_sec_vert_curve_oppos=0;
		  bunch_sec_vert_curve=0;
		  color_object=0;
		  sec_non_fit=0;
		sec_non_fit_oppos=0;
	     mean_lower_sec=-1;
	    mean_upper_sec=-1;
	    upper_lower_ratio_sec=-1;
		mean_lower_sec_oppos=-1;
	    mean_upper_sec_oppos=-1;
	    upper_lower_ratio_sec_oppos=-1;
		  bunch_sec_incl_curve=-DimX;
		  bunch_sec_incl_curve_oppos=-DimX;
		  sec_oppos_sky=-1;
		  sec_sky=-100;
		  distant_green_oppos=0;
		  distant_green=0;
		  bunch_section_oppos_incline=0;
		  bunch_section_incline=0;
		  bunch_geom_feature=0;
		  bunch_section_incline=0;
	      bunch_section_hue_zone=-1;
	      bunch_section_gray_zone=-1;
		  belongs_to_yellow=0;
		  belongs_to_green=0;
		  bunch_section_oppos=-1;
		  bunch_section=-1;
		  bunch_section_weight=-1;
	      bunch_section_hue=-1;
	      bunch_section_gray=-1;
		  bunch_section_oppos_sat=-1;
		  bunch_section_sat=-1;
		  bunch_section_oppos_hue=-1;
	      bunch_section_oppos_gray=-1;
		  non_green_feature=0;
		  number_bound_oppos=0;
		  if(!left_right)
	      {
		  new_bunch_number=bunch_first+(count_bunch-bunch_first);
		  }
		  else
		  {
		  new_bunch_number=bunch_first-(count_bunch-bunch_first);
		  }
		  old_bunch_number=ColorInt[strip_number].old_ordered_bunch_number[new_bunch_number];
	old_bunch_hue=ColorInt[strip_number].ColoredIntervalsStructure->AverageHue[old_bunch_number];
	old_bunch_hue_zone=hue_zones[old_bunch_hue];
	old_bunch_saturation=
	ColorInt[strip_number].ColoredIntervalsStructure->AverageSat[old_bunch_number];
	old_bunch_gray=ColorInt[strip_number].ColoredIntervalsStructure->AverageGray[old_bunch_number];
	old_bunch_gray_zone=gray_zones[old_bunch_gray];
	old_buch_beg=ColorInt[strip_number].ColoredIntervalsStructure->BegInterv[old_bunch_number];
	old_bunch_end=ColorInt[strip_number].ColoredIntervalsStructure->EndInterv[old_bunch_number];
	old_bunch_length=old_bunch_end-old_buch_beg+1;
	if(left_right)
	{
		/*if(new_bunch_number<final_bunch)
		{
		prior=3;
		return(prior);
		}*/
	bunch_section=
	SectionTraceRight[shift_in_mem+old_bunch_number];
	bunch_section_oppos=
	SectionTraceLeft[shift_in_mem+old_bunch_number];
	}
	else
	{
		/*if(new_bunch_number>final_bunch)
		{
		prior=3;
		return(prior);
		}*/
	bunch_section=
	SectionTraceLeft[shift_in_mem+old_bunch_number];
	bunch_section_oppos=
	SectionTraceRight[shift_in_mem+old_bunch_number];
	}

	bunch_section=bunch_section%1000;
	bunch_section_oppos=bunch_section_oppos%1000;
	if(bunch_section>0)
	{//gas>0
		bunch_section-=1;
	bunch_section_sat=ColorSection->section_mean_saturation[bunch_section];
	bunch_section_hue=ColorSection->section_mean_hue[bunch_section];
	bunch_section_gray=ColorSection->section_mean_gray[bunch_section];
	sec_sky=*(SkyGreenComponents+bunch_section)%10;
	bunch_section_incline=InclinedComponent[bunch_section];
	bunch_sec_incl_curve=InclineCurve[bunch_section];
	bunch_sec_vert_curve=VerticalComponent[bunch_section];
	bunch_section_hue_zone=hue_zones[bunch_section_hue];
	bunch_section_gray_zone=gray_zones[bunch_section_gray];
	bunch_section_weight=
	ColorSection->section_weight[bunch_section];
	bunch_section_fiber_first=ColorSection->section_fibre_first[bunch_section];
	bunch_section_fiber_last=ColorSection->section_fibre_last[bunch_section];
	bunch_section_fiber_length=
	bunch_section_fiber_last-bunch_section_fiber_first+1;
	bunch_section_weight_normalized=bunch_section_weight/bunch_section_fiber_length;
	fiber_first_ratio=(16*bunch_section_fiber_first)/NumStrips;
	fiber_last_ratio=(16*bunch_section_fiber_last)/NumStrips;
	mean_lower_sec=ColorSection->section_lower_mean_length[bunch_section];
	mean_upper_sec=ColorSection->section_upper_mean_length[bunch_section];
	if(left_right)
	{
		sect_dist=first_old_end-old_bunch_end;
	}
	else
	{
	sect_dist=old_buch_beg-first_old_beg;
	}
	if((mean_lower_sec>0)&&(mean_upper_sec>0))
	{
	upper_lower_ratio_sec=(8*mean_upper_sec)/mean_lower_sec;
	height_width_ratio=
	16*(StripWidth*(bunch_section_fiber_length-1))/(mean_lower_sec+mean_upper_sec);
	}

	if((bunch_section_hue_zone!=3)&&(bunch_section_sat>=3)&&(bunch_section_gray_zone>=1))
	{//last_cor31.03.17
		if(bunch_section_sat>=4)
		{
		color_object=1;
		}
		else
		{
	if(bunch_section_gray_zone>2)
			{
				color_object=1;
			}
		}
	}
	if(strip_number<NumStrips/2)
	{//str<1/2
	if((mean_lower_sec<=2*StripWidth)&&(mean_upper_sec<=2*StripWidth))
	{
		 if((bunch_section_gray_zone>=2)&&(bunch_section_sat>=3))
		 {
	if(upper_lower_ratio_sec>=7)
	{
		sec_non_fit=1;
	}
		 }
	}
	}//str<1/2
	if((fiber_first_ratio>=7)||(fiber_first_ratio>=5)&&(fiber_last_ratio>=8))
	{//fr>=7onlyublast_cor11.05.17
		if(bunch_section_hue_zone==3)
		{
if(bunch_section_gray_zone<=2)
{//onlyublast_cor11.05.17
   if(bunch_section_sat>=5)
   {
	   distant_green=1;
   }
}//onlyublast_cor11.05.17
			if(!sec_sky)
			{//last_cor12.01.17
				if(bunch_section_sat>=3)
				{
	if(upper_lower_ratio_sec>=10)
	{
		distant_green=1;
	}
				}
			}//last_cor12.01.17
	if((bunch_section_gray_zone>=2)&&(bunch_section_sat>=3))
	{
		if((bunch_section_weight_normalized<=2*StripWidth)&&(bunch_section_weight<=DimX/12))
		{
	sec_non_fit=1;
		}

	}
		}
	}//fr>=7
	if((bunch_section_hue_zone==3)&&(bunch_section_gray_zone<=1)&&(bunch_section_sat>=5)&&
	   (bunch_section_hue<=20))
	{
		if((fiber_last_ratio>=14)&&(fiber_first_ratio>=4))
		{
			if(bunch_section_weight>=DimX)
			{
		distant_green=1;
		    }
		}
		if(fiber_last_ratio>10)
		{
		if(bunch_section_weight>=3*DimX)
			{
		distant_green=1;
		    }
		}
	}
	}
	else
	{
		bunch_section=-1;
	}
	if(bunch_section_oppos>0)
	{//gas>0
		bunch_section_oppos-=1;
	bunch_section_oppos_sat=ColorSection->section_mean_saturation[bunch_section_oppos];
	bunch_section_oppos_hue=ColorSection->section_mean_hue[bunch_section_oppos];
	bunch_section_oppos_gray=ColorSection->section_mean_gray[bunch_section_oppos];
	sec_oppos_sky=*(SkyGreenComponents+bunch_section_oppos)%10;
	bunch_section_oppos_incline=InclinedComponent[bunch_section_oppos];
	bunch_sec_incl_curve_oppos=InclineCurve[bunch_section_oppos];
	bunch_sec_vert_curve_oppos=VerticalComponent[bunch_section_oppos];
	bunch_section_oppos_hue_zone=hue_zones[bunch_section_oppos_hue];
	bunch_section_oppos_gray_zone=gray_zones[bunch_section_oppos_gray];
	bunch_section_oppos_weight=
	ColorSection->section_weight[bunch_section_oppos];
	bunch_section_fiber_first_oppos=ColorSection->section_fibre_first[bunch_section_oppos];
	bunch_section_fiber_last_oppos=ColorSection->section_fibre_last[bunch_section_oppos];
	bunch_section_fiber_length_oppos=
	bunch_section_fiber_last_oppos-bunch_section_fiber_first_oppos+1;
	bunch_section_weight_normalized_oppos=
	bunch_section_oppos_weight/bunch_section_fiber_length_oppos;
	fiber_first_ratio_oppos=(16*bunch_section_fiber_first_oppos)/NumStrips;
	fiber_last_ratio_oppos=(16*bunch_section_fiber_last_oppos)/NumStrips;
	mean_lower_sec_oppos=ColorSection->section_lower_mean_length[bunch_section_oppos];
	mean_upper_sec_oppos=ColorSection->section_upper_mean_length[bunch_section_oppos];

	if((mean_lower_sec_oppos>0)&&(mean_upper_sec_oppos>0))
	{
	upper_lower_ratio_sec_oppos=(8*mean_upper_sec_oppos)/mean_lower_sec_oppos;
	height_width_ratio_oppos=
	16*(StripWidth*(bunch_section_fiber_length_oppos-1))/(mean_lower_sec_oppos+mean_upper_sec_oppos);
	}

	if((bunch_section_oppos_hue_zone!=3)&&(bunch_section_oppos_sat>=3)&&
	   (bunch_section_oppos_gray_zone>=1))
	{//last_cor31.03.17
		if(bunch_section_oppos_sat>=4)
		{
		color_object=1;
		}
		else
		{
			if(bunch_section_oppos_gray_zone>2)
			{
				color_object=1;
			}
		}
	}
	if(strip_number<NumStrips/2)
	{//str<1/2
	if((mean_lower_sec_oppos>0)&&(mean_lower_sec_oppos<=2*StripWidth)&&
	   (mean_upper_sec_oppos>0)&&(mean_upper_sec_oppos<=2*StripWidth))
	{
		 if((bunch_section_oppos_gray_zone>=2)&&(bunch_section_oppos_sat>=3))
		 {
	if(upper_lower_ratio_sec_oppos>=7)
	{
		sec_non_fit_oppos=1;
	}
		 }
	}
	}//str<1/2
	if(fiber_first_ratio_oppos>=7)
	{//fr>=7
		if(bunch_section_oppos_hue_zone==3)
		{
			if(!sec_oppos_sky)
			{//last_cor12.01.17
				if(bunch_section_oppos_sat>=3)
				{
	if(upper_lower_ratio_sec_oppos>=10)
	{
		distant_green_oppos=1;
	}
				}
			}//last_cor12.01.17
	if((bunch_section_oppos_gray_zone>=2)&&(bunch_section_oppos_sat>=3))
	{
		if((bunch_section_weight_normalized_oppos<=2*StripWidth)
			&&(bunch_section_oppos_weight<=DimX/12))
		{
	sec_non_fit_oppos=1;
		}

	}
		}
	}//fr>=7
	number_bound_oppos=
	LeftRightSectionNumberOfRightGreenBoundaryPoints[bunch_section_oppos];
	/*if(sec_num>=number_of_sections_left)
	{
	   if(!left_right)
		{
	if(number_bound_oppos>0)
	    {
	prior=-3;
	goto M;
	    }
	    }
	}*/
	if((bunch_section_oppos_hue_zone==3)&&(bunch_section_oppos_gray_zone<=1)&&
	   (bunch_section_oppos_sat>=5))
	{

		if((fiber_last_ratio_oppos>=14)&&(fiber_first_ratio_oppos>=4))
		{
		distant_green_oppos=1;
		}
	}
	}
	else
	{
		bunch_section_oppos=-1;
	}
	if(!left_right)
	{
		if((bunch_section_incline>0)&&(bunch_section_oppos_incline<0)&&
			((bunch_section_oppos_weight>=2000)||(bunch_section_weight>=2000)))
		{
		bunch_geom_feature=1;
		}
	}
	else
	{
		if((bunch_section_incline<0)&&(bunch_section_oppos_incline>0)&&
			((bunch_section_oppos_weight>=2000)||(bunch_section_weight>=2000)))
			{
		bunch_geom_feature=1;
		   }
	}

	if((sec_oppos_sky==1)||(sec_sky==1))
	{
		prior=-3;
		goto M;
	}
	if(((sec_oppos_sky==2)||(sec_sky==2))&&(old_bunch_hue_zone==3)&&(old_bunch_saturation>1))
		{//onlyublast_cor05.06.17
	belongs_to_green=1;
		}
	else
	{//e
	if(((old_bunch_hue>=8)&&(old_bunch_hue<=18))||
	   ((bunch_section_hue>=8)&&(bunch_section_hue<=18)&&(bunch_section_gray>=7))||
	   ((strip_number>=NumStrips/2)&&(old_bunch_hue>18)&&(old_bunch_hue<=20)))
	{//sh817last_cor13.04.17onlyublast_cor05.06.17
		if(old_bunch_saturation>=4)
		{//secs6
			     if(old_bunch_gray_zone<=5)
				 {//grz5
			if(old_bunch_gray_zone>=2)
			{//ogz2
				belongs_to_green=1;
			}//ogz2
			else
			{//eogz2
	   if(old_bunch_gray_zone>=1)
	   {//ogz1
	   belongs_to_green=2;
	   }//ogz1
	    else
	   {//eogz1
		if(!old_bunch_gray_zone)
		{
			if(old_bunch_gray>=5)
			{
			belongs_to_green=4;
			}
		}

	    }//eogz1
			}//eogz2
				 }//grz5
		}//secs6
		else
		{//esecs6
	if((old_bunch_saturation>=3)||(bunch_section_oppos_sat>=3)||(bunch_section_sat>=3)||
	   (sec_oppos_sky==2)||(sec_sky==2))
	{


		if((old_bunch_gray_zone>1)&&(old_bunch_gray_zone<=4))
		{
			if((old_bunch_saturation>=2)&&((sec_oppos_sky==2)||(sec_sky==2)))
			{
			belongs_to_green=3;
			}
			else
			{
				if(old_bunch_saturation>=3)
				{//last_cor15.12.16
					if(old_bunch_saturation>3)
					{
					belongs_to_green=4;
					}
					if(old_bunch_saturation==3)
					{
						if(old_bunch_gray_zone<=3)
						{
						belongs_to_green=4;
						}
						else
						{
							if((sec_oppos_sky==2)||(sec_sky==2))
							{
	                         belongs_to_green=5;
							}
							else
							{
								if((strip_number>=(NumStrips/2 - 2))&&(old_bunch_gray_zone==4))
								{
									belongs_to_green=6;
								}
							}
						}
					}
				}//last_cor15.12.16
			}
		}

	}
		}//esecs6
	}//sh817
	else
	{//esh817
		if((old_bunch_hue>=18)&&(old_bunch_hue<=20)&&(strip_number>=NumStrips/2))
		{//last_cor05.04.17
			if(old_bunch_gray_zone==2)
			{
				if(old_bunch_saturation>=4)
				{
					belongs_to_green=4;
				}
			}
		}//last_cor05.04.17
		if((old_bunch_hue>=5)&&(old_bunch_hue<=7)
			||(bunch_section_hue>=5)&&(bunch_section_hue<=7)&&(bunch_section_gray>=7))
		{
	      if((old_bunch_gray_zone>=2)&&(old_bunch_gray_zone<=4))
	      {
			  if(old_bunch_saturation>=6)
			  {
				  belongs_to_yellow=1;
			  }
			  else
			  {
	             if(old_bunch_saturation>=3)
			     {
				 belongs_to_yellow=1;
			     }
				 else
				 {
					 if(old_bunch_saturation>=2)
			         {
				     belongs_to_yellow=2;
			         }
				 }
			  }
		  }
		}
	}//esh817
	}//e
	if((bunch_section>=0)&&(bunch_section_sat<=1))
	{
	belongs_to_green=0;
	non_green_feature=1;
	}
	       if(!belongs_to_green)
	       {//btg01last_cor25.08.16
			   if(((distant_green)||(distant_green_oppos))&&(old_bunch_hue_zone==3))
			   {
			   if(old_bunch_hue<=20)
			   {
				   belongs_to_green=4;
			   }
			   }
		   }//btg01last_cor25.08.16
	                    if(!belongs_to_green)
	                    {//btg0
	if((old_bunch_hue_zone<2)||(old_bunch_hue_zone>3)||(old_bunch_hue>20)||(old_bunch_saturation<=1))
			{
				non_green_feature=1;
			}
			if((old_bunch_gray_zone>=6)&&(old_bunch_saturation>=3))
			{
				non_green_feature=1;
			}
	/*if((old_bunch_gray_zone>=2)&&(old_bunch_saturation<=3)&&(old_bunch_hue>=18))
			{//last_cor03.11.15
				non_green_feature=1;
			}*/
	if((old_bunch_gray_zone>=2)&&(old_bunch_saturation<=1))
			{//last_cor03.11.15
				non_green_feature=1;
			}
	if((old_bunch_gray_zone>=2)&&(old_bunch_saturation<=2)&&(old_bunch_hue>=19))
			{//last_cor03.11.15
				non_green_feature=1;
			}
	if((old_bunch_gray_zone>=4)&&(old_bunch_saturation<=2))
			{//last_cor03.11.15
				non_green_feature=1;
			}
	if((initial_section_label==2)&&((sec_oppos_sky==3)||(sec_sky==3))&&(bunch_geom_feature>0))
	{
	non_green_feature=1;
	}
						}//btg0
	test_junction=
	ColorInt[strip_number].Test_Junction_Corrected(start_bunch_old_num,old_bunch_number,bunch_occurrence,
	Intersection,indic_length,ratio_length,ratio_length1,&hue_close,&saturation_close,&gray_close);
		test_junction=test_junction%10;
	if((!non_green_feature)&&((sec_sky==initial_section_label)||(sec_oppos_sky==initial_section_label)||
	   (belongs_to_green>0)))
	{//ngf!0
	  if(((test_junction>=1)&&(test_junction<=3))||(belongs_to_green>0)||
		  ((hue_close==1)&&(saturation_close>=1)&&(saturation_close<=2)&&
		  (old_bunch_gray_zone>=1)))
	  {//tj13
		if(!(first_green_adjacent+1))
		{
	//if((test_junction>=1)&&(test_junction<=2))
	//{
	first_green_adjacent=new_bunch_number;
	green_chain=0;
	//}
		}
		              if((bunch_section>=0)&&((sec_oppos_sky==2)||(sec_sky==2)))
		               {//rs
		   if((bunch_section_weight>green_section_max_weight)&&(count_bunch!=bunch_first))
		   {
			   if((green_section_max_weight>0)&&(m_n_green_sec_weight>0))
			   {
			       if(left_right)
			       {
	               max_n_dist=m_n_green_sec_weight_bunch_beg-old_bunch_end;
			       }
			       else
			       {
				   max_n_dist=old_buch_beg-m_n_green_sec_weight_bunch_end;
			       }
			   }
	   green_dist=green_length_tot-green_length_before;
	   if((((green_section_max_weight>DimX/2)&&((green_dist>8*StripWidth)))||
		   ((non_green_total_length>DimX/8)&&(sect_dist>=DimX/6)&&
		   (green_dist>10*StripWidth)))&&(green_section_max_weight>0))
	   {
		   next_gr_sec=bunch_section;
	   }
	   else
	   {
	if((m_n_green_section_in_weight>=0)||
			   ((first_n_green_adjacent>=0)&&(non_green_total_length>StripWidth)))
	{//last_cor05.04.17
		   if((count_bunch-bunch_first)>=1)
		   {//last_cor05.04.17
	   green_section_max_weight=bunch_section_weight;
	    m_green_section=bunch_section;
		m_green_section_bunch=new_bunch_number;
		incline_opt=bunch_section_incline;
		incl_curve_opt=bunch_sec_incl_curve;
		sect_dist_opt=sect_dist;
		green_length_before=green_length_tot;

		   }//last_cor05.04.17
	}
	   }
		   }
					   }//rs
	                 if((bunch_section_oppos>=0)&&((sec_oppos_sky==2)||(sec_sky==2)))
		               {//rso
		   if((bunch_section_oppos_weight>green_section_oppos_max_weight)&&
			   (count_bunch!=bunch_first))
		   {
	green_dist=green_length_tot-green_length_before_oppos;
	if(((green_section_oppos_max_weight>DimX/2)||((non_green_total_length>DimX/8)
	  &&(sect_dist>=DimX/6)&&(green_dist>2*StripWidth)))&&(green_section_oppos_max_weight>0))
	   {
		   next_gr_sec_oppos=bunch_section;
	   }
	   else
	   {
		   if((m_n_green_section_in_weight>=0)||
			   ((first_n_green_adjacent>=0)&&(non_green_total_length>StripWidth)))
		   {
	    green_section_oppos_max_weight=bunch_section_oppos_weight;
	    m_green_section_oppos=bunch_section_oppos;
		m_green_section_oppos_bunch=new_bunch_number;
	green_length_before_oppos=green_length_tot;
		   }
	   }
		   }
					   }//rso
	last_green_adjacent=new_bunch_number;
	ColorOfBasicBunches[shift_in_array+new_bunch_number]=1;
	green_length_tot+=old_bunch_length;
	//last_cor07.12.16
	if(!green_chain)
	{
		last_green_adjacent_in_chain=new_bunch_number;
		green_first_length+=old_bunch_length;
	}
	  }//tj13

	}//ngf!0
	if(old_bunch_length>maximum_length)
	{
	maximum_length=old_bunch_length;
	maximum_length_bunch=new_bunch_number;
	}
	//if(((non_green_feature)||((test_junction>=5)&&(hue_close<=0)))&&(old_bunch_saturation<=2))
	if(((non_green_feature)||((test_junction>=5)&&(hue_close<=0)&&(!belongs_to_green))))
	{//ngtjhc
		if(first_green_adjacent>=0)
		  {
	      green_chain=1;
		  }
		if(!(first_n_green_adjacent+1))
		{
			first_n_green_adjacent=new_bunch_number;
		}
	last_n_green_adjacent=new_bunch_number;
		non_green_total_length+=old_bunch_length;
	/*if(left_right)
	{
		n_green_section=SectionTraceRight[strip_number*MAX_COL_INT + oppos_bunch];
	}
	else
	{
	n_green_section=SectionTraceLeft[strip_number*MAX_COL_INT + oppos_bunch];
	}*/
		//n_green_section=n_green_section%1000;
		              if((bunch_section>=0)&&(!distant_green))
					  //if(bunch_section>=0)
		               {//rs
		   if((bunch_section_weight>m_n_green_sec_weight)&&(!sec_non_fit)&&((!color_object)
			   ||(old_bunch_length>=DimX/4)||(bunch_section_weight_normalized>=DimX/4)||
			   ((height_width_ratio_oppos>=0)&&(height_width_ratio_oppos<=3))||
			   ((height_width_ratio>=0)&&(height_width_ratio<=3))))
		   {//last_cor28.03.17
	   m_n_green_sec_weight=bunch_section_weight;
	   m_n_green_section_in_weight=bunch_section;
	   m_n_green_sec_weight_bunch=new_bunch_number;
	   m_n_green_sec_weight_bunch_beg=old_buch_beg;
	   m_n_green_sec_weight_bunch_end=old_bunch_end;
		   }//last_cor28.03.17

					   }//rs
					  if((bunch_section_oppos>=0)&&(!distant_green_oppos))
		               {//rso
		   if((bunch_section_oppos_weight>m_n_green_sec_oppos_weight)&&(!sec_non_fit_oppos)
&&((!color_object)||((bunch_section_oppos_weight>=4*DimX)&&(bunch_section_oppos_hue_zone==4)&&
   (mean_lower_sec_oppos>=DimX/2)&&((height_width_ratio_oppos>=0)&&(height_width_ratio_oppos<=4)))))
		   {//last_cor19.07.17
			   if((!m_n_green_sec_oppos_weight)
				   ||(new_bunch_number!=m_n_green_sec_weight_bunch))
			   {//last_cor21.03.17
	   m_n_green_sec_oppos_weight=bunch_section_oppos_weight;
	   m_n_green_section_oppos_in_weight=bunch_section_oppos;
	   m_n_green_sec_oppos_weight_bunch=new_bunch_number;
			   }//last_cor21.03.17
		   }

					   }//rso

	if(old_bunch_length>maximum_non_green_bunch_length)
	{
	maximum_non_green_bunch_length=old_bunch_length;
	maximum_non_green_bunch=new_bunch_number;
	}
	}//ngtjhc
	if(sec_num>=number_of_sections_left)
	{
		if(!left_right)
		{
			if((bunch_section>=0)&&(final_section>=0))
		{//last_cor13.02.17
			if(bunch_section==final_section)
			{
				prior=3;
				goto M;
			}
		}
			/*if(new_bunch_number>=final_bunch)
		    {
				if(oppos_bunch_number<final_bunch)
				{//last_cor01.10.16
		    prior=3;
				}
		    goto M;

		    }*/
		}
	}
	  }//secloop
		}//do
	M:;
	*first_green_neighbor=first_green_adjacent;
	*last_green_neighbor=last_green_adjacent;
	*maximum_non_green=maximum_non_green_bunch;
	*non_gr_length=non_green_total_length;
	*last_green_in_chain=last_green_adjacent_in_chain;
	*first_green_length=green_first_length;
	*first_non_green=first_n_green_adjacent;
	*last_non_green=last_n_green_adjacent;
	*max_green_section=m_green_section;
	*max_green_section_bunch=m_green_section_bunch;
	*max_green_oppos_section=m_green_section_oppos;
	*max_green_oppos_section_bunch=m_green_section_oppos_bunch;
	*max_weight_n_green_section=m_n_green_section_in_weight;
	*max_weight_n_green_section_bunch=m_n_green_sec_weight_bunch;
	*max_weight_n_green_section_oppos=m_n_green_section_oppos_in_weight;
	*max_weight_n_green_section_oppos_bunch=m_n_green_sec_oppos_weight_bunch;
	*gr_length_before=green_length_before;
	return(prior);
}
//=========================================================
int

CImageProcess::NonGreenSummation(int left_right,int first_bunch,int last_bunch,int strip_number)
{
	int bunch_beg;
    int bunch_end;
	int bunch_old_num;
	int bunch_length;
	int total_length;

	total_length=0;
for(int count_bunch=first_bunch;count_bunch<=last_bunch;count_bunch++)
{//bunloop
	bunch_old_num=ColorInt[strip_number].old_ordered_bunch_number[count_bunch];
bunch_beg=ColorInt[strip_number].ColoredIntervalsStructure->BegInterv[bunch_old_num];
bunch_end=ColorInt[strip_number].ColoredIntervalsStructure->EndInterv[bunch_old_num];
bunch_length=bunch_end-bunch_beg;
total_length+=bunch_length;
}//bunloop
total_length+=1;
	return(total_length);
}
//=========================================================
int

CImageProcess::NearestGreenUnder(int sect_number,int first_sec_strip,int bunch_sec)
{
	int prior;
    int sect_oppos_number;
	int number_of_oppos_sections;
	int sect_green_comp;
	int sect_green_comp_red;
	int section_oppos_hue;
	int section_oppos_sat;
	int section_oppos_gray;
	int section_oppos_end;
	int section_oppos_weight;
	int sect_oppos_incline;
	int sect_oppos_vert;
	int bunch_beg;
	int bunch_end;
	int last_bunch;
	int last_bunch_beg;
	int last_bunch_end;
	int previous_bunch;
	int previous_bunch_beg;
	int previous_bunch_end;
	int Intersection;
	int indic_length;
	int ratio_length;
	int ratio_length1;
	int numb_left_sec;
	int dev1;
	int dev2;

	prior=-1;
number_of_oppos_sections=ColorSection->sections_under[(sect_number*8)+7];
numb_left_sec=ColorSection->Number_of_sections_left;
if(!number_of_oppos_sections)
{
	return(prior);
}
bunch_beg=
ColorInt[first_sec_strip].ColoredIntervalsStructure->BegInterv[bunch_sec];
bunch_end=
ColorInt[first_sec_strip].ColoredIntervalsStructure->EndInterv[bunch_sec];
for(int count_sec=0;count_sec<number_of_oppos_sections;count_sec++)
{//secloop
sect_oppos_number=ColorSection->sections_under[(sect_number*8)+count_sec]-1;
sect_green_comp=*(SkyGreenComponents+sect_oppos_number);
sect_green_comp_red=sect_green_comp%10;
section_oppos_hue=ColorSection->section_mean_hue[sect_oppos_number];
section_oppos_sat=ColorSection->section_mean_saturation[sect_oppos_number];
section_oppos_gray=ColorSection->section_mean_gray[sect_oppos_number];
   if(sect_green_comp_red==2)
   {
section_oppos_end=ColorSection->section_fibre_last[sect_oppos_number];
section_oppos_weight=ColorSection->section_weight[sect_oppos_number];
sect_oppos_incline=*(InclinedComponent+sect_oppos_number);
sect_oppos_vert=*(VerticalComponent+sect_oppos_number);

last_bunch=ColorDescrSect[sect_oppos_number].location_of_section[section_oppos_end];
last_bunch-=1;
last_bunch=ColorInt[section_oppos_end].old_bunch_number[last_bunch];
last_bunch_beg=
ColorInt[section_oppos_end].ColoredIntervalsStructure->BegInterv[last_bunch];
last_bunch_end=
ColorInt[section_oppos_end].ColoredIntervalsStructure->EndInterv[last_bunch];
Intersection=
ColorInt[first_sec_strip].Intersection_measure_ratios(bunch_beg,bunch_end,
last_bunch_beg,last_bunch_end,&indic_length,&ratio_length,&ratio_length1);
previous_bunch=
ColorDescrSect[sect_oppos_number].location_of_section[(section_oppos_end-1)];
previous_bunch-=1;
previous_bunch=ColorInt[section_oppos_end-1].old_bunch_number[previous_bunch];
previous_bunch_beg=
ColorInt[(section_oppos_end-1)].ColoredIntervalsStructure->BegInterv[previous_bunch];
previous_bunch_end=
ColorInt[(section_oppos_end-1)].ColoredIntervalsStructure->EndInterv[previous_bunch];
if(sect_oppos_number<numb_left_sec)
{
	dev1=last_bunch_beg-bunch_beg;
	dev2=previous_bunch_beg-last_bunch_beg;
}
else
{
dev1=bunch_end-last_bunch_end;
dev2=last_bunch_end-previous_bunch_end;
}
   }
}//secloop

	return(prior);
}
//===================================================================
int

CImageProcess::SectionCorrespondence(int prev_sect_number,int next_sec_number)
{
	int prior;
	int numb_left_sec;
	int last_prev_sec_fiber;
	int first_next_sec_fiber;
	int last_prev_bunch;
	int bunch_old_number_prev;
	int last_prev_bunch1;
	int bunch_old_number_prev1;
	int first_next_bunch;
	int bunch_old_number_next;
	int first_next_bunch1;
	int bunch_old_number_next1;
	int beg_int_prev;
	int end_int_prev;
	int beg_int_next;
	int end_int_next;
	int beg_int_prev1;
	int end_int_prev1;
	int beg_int_next1;
	int end_int_next1;
	int Intersection;
	int indic_length;
	int ratio_length;
	int ratio_length1;
	int test_junction;

	prior=-1;
numb_left_sec=ColorSection->Number_of_sections_left;
first_next_sec_fiber=ColorSection->section_fibre_first[next_sec_number];
last_prev_sec_fiber=ColorSection->section_fibre_last[prev_sect_number];
last_prev_bunch=
ColorDescrSect[prev_sect_number].location_of_section[last_prev_sec_fiber];
last_prev_bunch1=
ColorDescrSect[prev_sect_number].location_of_section[(last_prev_sec_fiber-1)];
  if(last_prev_bunch>0)
  {
 last_prev_bunch-=1;
  }
  else
  {
	 prior=-2;
	 return(prior);
  }
 bunch_old_number_prev=ColorInt[last_prev_sec_fiber].old_bunch_number[last_prev_bunch];
beg_int_prev=
ColorInt[last_prev_sec_fiber].ColoredIntervalsStructure->BegInterv[bunch_old_number_prev];
end_int_prev=
ColorInt[last_prev_sec_fiber].ColoredIntervalsStructure->EndInterv[bunch_old_number_prev];
if(last_prev_bunch1>0)
  {
 last_prev_bunch1-=1;
  }
  else
  {
	 prior=-2;
	 return(prior);
  }
bunch_old_number_prev1=ColorInt[(last_prev_sec_fiber-1)].old_bunch_number[last_prev_bunch1];
beg_int_prev1=
ColorInt[(last_prev_sec_fiber-1)].ColoredIntervalsStructure->BegInterv[bunch_old_number_prev1];
end_int_prev1=
ColorInt[(last_prev_sec_fiber-1)].ColoredIntervalsStructure->EndInterv[bunch_old_number_prev1];

first_next_bunch=
ColorDescrSect[next_sec_number].location_of_section[first_next_sec_fiber];
  if(first_next_bunch>0)
  {
 first_next_bunch-=1;
  }
  else
  {
	 prior=-2;
	 return(prior);
  }
 bunch_old_number_next=ColorInt[first_next_sec_fiber].old_bunch_number[first_next_bunch];
beg_int_next=
ColorInt[first_next_sec_fiber].ColoredIntervalsStructure->BegInterv[bunch_old_number_next];
end_int_next=
ColorInt[first_next_sec_fiber].ColoredIntervalsStructure->EndInterv[bunch_old_number_next];
first_next_bunch1=
ColorDescrSect[next_sec_number].location_of_section[(first_next_sec_fiber+1)];
  if(first_next_bunch1>0)
  {
 first_next_bunch1-=1;
  }
  else
  {
	 prior=-2;
	 return(prior);
  }
 bunch_old_number_next1=ColorInt[(first_next_sec_fiber+1)].old_bunch_number[first_next_bunch1];
beg_int_next1=
ColorInt[(first_next_sec_fiber+1)].ColoredIntervalsStructure->BegInterv[bunch_old_number_next1];
end_int_next1=
ColorInt[(first_next_sec_fiber+1)].ColoredIntervalsStructure->EndInterv[bunch_old_number_next1];
Intersection=
ColorInt[last_prev_sec_fiber].Intersection_measure_ratios(beg_int_prev,end_int_prev,
beg_int_next,end_int_next,&indic_length,&ratio_length,&ratio_length1);
test_junction=
Test_Junction_Different_Strips(bunch_old_number_prev,bunch_old_number_next,Intersection,
indic_length,ratio_length,ratio_length1,last_prev_sec_fiber,first_next_sec_fiber);
test_junction=test_junction%10;
  /*for(count_similar=0;count_similar<number_of_bunches;count_similar++)
  {
right_contrast=ColorInt[count_fiber].right_continuation[bunch_old_number];
right_adjacent=ColorInt[count_fiber].right_adjacent[bunch_old_number];*/



	return(prior);
}
//===========================================================================
void

CImageProcess::ColorShapeSectionClassification(void)
{
	int last_section;
	//int last_left_section;
	int last_fiber;
	int first_fiber;
	int section_length;
	int	first_bunch;
	int section_hue;
    int section_hue_zone;
    int section_gray;
    int section_gray_zone;
    int section_sat;
	int section_weight;
	//int sect_geom;
	int last_strip_ratio;
	int first_fiber_ratio;
	int section_incline;
	int	section_incl_curve;
	int first_fiber_beg;
	int first_fiber_end;
	int	first_fiber_length;
	int last_fiber_beg;
	int last_fiber_end;
	int	last_fiber_length;
	int number_of_left_sections;
	int bound_cond;
	int section_weight_normalized;
	int mean_lower;
	int mean_upper;
	int upper_lower_ratio;
	int section_vert;
	int sect_oppos;
	int sect_oppos_incline;
	int sect_oppos_inct_curve;
	int sect_oppos_vert;
	int sect_oppos_weight;
	int sect_oppos_sky;
	int sect_oppos_first_strip;
	int sect_oppos_last_strip;
	int last_bunch;
	int min_left;
	int max_right;

sect_oppos_first_strip=-1;
sect_oppos_last_strip=-1;
	sect_oppos_incline=0;
	sect_oppos_inct_curve=0;
	sect_oppos_vert=0;
	sect_oppos_weight=0;
	section_vert=0;
sect_oppos=-1;
	upper_lower_ratio=-1;
mean_lower=0;
mean_upper=0;
first_fiber_ratio=-1;
	bound_cond=-1;
	last_section=ColorSection->Number_of_sections;
	number_of_left_sections=ColorSection->Number_of_sections_left;

	for(int count_sec=0;count_sec<last_section;count_sec++)
	{//loop_green_finding
			section_weight_normalized=0;

				if(*(SkyGreenComponents+count_sec)==1)
				{
					goto L;
				}

	    last_fiber=ColorSection->section_fibre_last[count_sec];
		first_fiber=ColorSection->section_fibre_first[count_sec];
		section_length=last_fiber-first_fiber+1;
min_left=ColorSection->section_left_end[count_sec];
max_right=ColorSection->section_right_end[count_sec];
			//if(section_length>=4)
			//{
//sect_geom=SectionGeometry(count_sec,&mean_lower,&mean_upper);
mean_lower=ColorSection->section_lower_mean_length[count_sec];
mean_upper=ColorSection->section_upper_mean_length[count_sec];
if((mean_lower>0)&&(mean_upper>0))
{
upper_lower_ratio=(8*mean_upper)/mean_lower;
}
			//}
		last_strip_ratio=(16*last_fiber)/(NumStrips-1);
		last_bunch=ColorSection->DescrSec[count_sec].location_of_section[last_fiber];
		first_bunch=ColorSection->DescrSec[count_sec].location_of_section[first_fiber];
		first_fiber_ratio=(16*first_fiber)/(NumStrips-1);
		if(last_bunch>0)
		{//lb>
			last_bunch--;
			last_bunch=ColorInt[last_fiber].old_bunch_number[last_bunch];
			last_fiber_beg=
				ColorInt[last_fiber].ColoredIntervalsStructure->BegInterv[last_bunch];
			last_fiber_end=
				ColorInt[last_fiber].ColoredIntervalsStructure->EndInterv[last_bunch];
			last_fiber_length=last_fiber_end-last_fiber_beg+1;
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
		}//fb>
		section_hue=ColorSection->section_mean_hue[count_sec];
        section_hue_zone=hue_zones[section_hue];
        section_gray=ColorSection->section_mean_gray[count_sec];
        section_gray_zone=gray_zones[section_gray];
        section_sat=ColorSection->section_mean_saturation[count_sec];
		section_weight=ColorSection->section_weight[count_sec];
		section_weight_normalized=section_weight/(last_fiber-first_fiber+1);
		section_incline=InclinedComponent[count_sec];
		section_incl_curve=InclineCurve[count_sec];
		section_vert=*(VerticalComponent+count_sec);
		if(count_sec>=number_of_left_sections)
		{
			if(first_fiber_beg<2*StripWidth)
			{
				bound_cond=1;
			}
			sect_oppos=
SectionTraceLeft[first_fiber*MAX_COL_INT+first_bunch];
			if(sect_oppos>0)
			{
sect_oppos=sect_oppos%1000;
sect_oppos-=1;
sect_oppos_incline=InclinedComponent[sect_oppos];
sect_oppos_inct_curve=InclineCurve[sect_oppos];
sect_oppos_vert=*(VerticalComponent+sect_oppos);
sect_oppos_weight=ColorSection->section_weight[sect_oppos];
sect_oppos_sky=*(SkyGreenComponents+sect_oppos)%10;
sect_oppos_first_strip=ColorSection->section_fibre_first[sect_oppos];
sect_oppos_last_strip=ColorSection->section_fibre_last[sect_oppos];
			}
		}
		else
		{
            if(first_fiber_end>=(DimX-2*StripWidth))
			{
				bound_cond=1;
			}
sect_oppos=
SectionTraceRight[first_fiber*MAX_COL_INT+first_bunch];
			if(sect_oppos>0)
			{
sect_oppos=sect_oppos%1000;
sect_oppos-=1;
sect_oppos_incline=InclinedComponent[sect_oppos];
sect_oppos_inct_curve=InclineCurve[sect_oppos];
sect_oppos_vert=*(VerticalComponent+sect_oppos);
sect_oppos_weight=ColorSection->section_weight[sect_oppos];
sect_oppos_sky=*(SkyGreenComponents+sect_oppos)%10;
sect_oppos_first_strip=ColorSection->section_fibre_first[sect_oppos];
sect_oppos_last_strip=ColorSection->section_fibre_last[sect_oppos];
		}
		}
           /*if(last_strip_ratio<=2)
	       {
		if((section_gray_zone<=1)&&(section_sat<=3)&&(section_hue>=17))
		{
			goto L;
		}
	      }*/
		   /*if((last_strip_ratio<=2)&&(section_gray_zone>=2)&&(!bound_cond))
		{
			if((section_hue>=19)&&(section_sat<=3))
			{
			goto L;
			}
			if((section_hue>=19)&&(section_sat<=4)&&(section_gray_zone>=3))
			{
				goto L;
			}
		}*/
		if((section_hue>=8)&&(section_hue<=17))
{//sh817
	if(section_sat>=6)
	{//secs6
		     if(section_gray_zone<=5)
			 {//grz5
		if((section_gray_zone>=2)||((section_gray_zone>=1)&&(section_sat>=8)))
		{
			*(SkyGreenComponents+count_sec)=2;
			goto L;
		}
		else
		{
if(section_gray_zone>=1)
{
*(SkyGreenComponents+count_sec)=12;
goto L;
}
		}
			 }//grz5
	}//secs6
	else
	{//last_cor26.01.16
if(section_sat>=3)
{

	if((section_gray_zone>=1)&&((section_gray_zone<=4)||(section_gray<=43)))
	{
		*(SkyGreenComponents+count_sec)=22;
		goto L;
	}
}

	}//last_cor26.01.16
	if((section_hue>=12)&&(section_hue<=16))
	{
if(first_fiber_ratio>=8)
{
	if(section_sat>=2)
	{
if((section_gray_zone>=3)&&(section_gray_zone<=4))
{
*(SkyGreenComponents+count_sec)=82;
goto L;
}
	}
}
	}
}//sh817
else
{//esh817
	if((bound_cond>0)&&(last_strip_ratio<=4))
	{
if((section_sat>=6)&&(section_hue>=17)&&((section_hue<=19)||
   ((section_hue<=21)&&(upper_lower_ratio>=10))))
{//sathuelast_cor13.03.17
if((section_gray_zone>=1)&&(section_gray_zone<=2))
{
	if((section_weight>=DimX/6)&&(section_weight_normalized>=5*StripWidth))
	 {
         if(count_sec>=number_of_left_sections)
		 {
			 if((section_incline>0)&&(section_incl_curve>0))
			 {
*(SkyGreenComponents+count_sec)=62;
goto L;

			 }
		 }
		 else
		 {
		 if((section_incline<0)&&(section_incl_curve<0))
			 {
*(SkyGreenComponents+count_sec)=62;
goto L;
			 }
		 }
	}
}
}
	}
	if((section_hue>=18)&&((section_hue<=20)||(section_hue_zone==3)&&(section_sat>=5)))//onlyublast_cor17.05.17
	{//sh1820
		{
			if(first_fiber_ratio>=7)
			{//last_cor28.04.17
				if((section_gray_zone>=2)&&(section_gray_zone<=4))
				{
					if((section_sat>=4)&&(first_fiber_length>=4*StripWidth))
					{
*(SkyGreenComponents+count_sec)=52;
goto L;
					}
				}
			}//last_cor28.04.17
			if(last_strip_ratio>=12)
			{//onlyublast_cor25.08.17
if(section_weight_normalized>DimX/4)
{
	               if((section_gray_zone>=1)&&(section_gray_zone<=4))
					{
	            	   if(section_sat>=3)
	            	   {
	 if(section_weight>=4*DimX)
	 {
		 *(SkyGreenComponents+count_sec)=52;
		 goto L;
	 }
	            	   }
					}
}
			}//onlyublast_cor25.08.17
			if((last_strip_ratio>8)&&(bound_cond>0))
			{//onlyublast_cor25.08.17
				if(count_sec<number_of_left_sections)
				{
if(section_incline<0)
{
	if(section_weight_normalized>DimX/4)
	{
		               if((section_gray_zone>=1)&&(section_gray_zone<=4))
						{
		            	   if(section_sat>=3)
		            	   {
		 if(section_weight>=4*DimX)
		 {
			 *(SkyGreenComponents+count_sec)=52;
			 goto L;
		 }
		            	   }
						}
	}
}
				}
				else
				{
					if(section_incline>0)
					{
						if(section_weight_normalized>DimX/4)
						{
							               if((section_gray_zone>=1)&&(section_gray_zone<=4))
											{
							            	   if(section_sat>=3)
							            	   {
							 if(section_weight>=4*DimX)
							 {
								 *(SkyGreenComponents+count_sec)=52;
								 goto L;
							 }
							            	   }
											}
						}
					}
				}

			}//onlyublast_cor25.08.17
if((((!section_gray_zone)&&(section_gray>=5))||(section_gray_zone>=1))&&(section_gray_zone<=2)&&(section_sat>=5))//onlyub
{
	if(last_strip_ratio>=7)//onlyub
	{
		if(first_fiber_ratio>=5)
		{
			*(SkyGreenComponents+count_sec)=52;
			goto L;
		}
	}
  if(count_sec<number_of_left_sections)
  {//last_cor25.04.17
	  if(last_strip_ratio>=5)
	  {
		  if((sect_oppos>=0)&&(section_vert>0)&&((sect_oppos_incline>0)
			  ||(sect_oppos_inct_curve>0)))
		  {
			  if(section_weight>=DimX)
			  {
*(SkyGreenComponents+count_sec)=52;
goto L;
			  }
		  }
	  }
  }//last_cor25.04.17
  else
  {
	  if((section_incline>0)&&((section_incl_curve>0)||(last_strip_ratio>=7)||
			  (sect_oppos_vert>0)||(section_weight>=4*DimX)))
	  {//onlyub18.05.17
	      if(section_sat>=5)
	     {
if(first_fiber_ratio<=2)
{
if(bound_cond==1)
{
	*(SkyGreenComponents+count_sec)=52;
	goto L;
}
}
	     }
	  }//onlyub18.05.17
if((section_sat>=5)&&(last_strip_ratio>=5))
	  {
		  if((sect_oppos>=0)&&((section_incline>0)||(section_incl_curve>0))&&
			  (sect_oppos_vert>0)&&(bound_cond>0))
		  {
			  if(section_weight>=DimX)
			  {
*(SkyGreenComponents+count_sec)=52;
goto L;
			  }
		  }
	  }
  }
   if(last_strip_ratio>=8)
   {
	   if(section_sat>=3)
	   {
if(section_weight_normalized>=DimX/8)
{
	if((upper_lower_ratio>=6)||((upper_lower_ratio>=5)&&(section_weight>4*DimX)&&
		(section_weight_normalized>=DimX/4)&&(section_sat>=4)))
	{
*(SkyGreenComponents+count_sec)=52;
goto L;
	}
}
else
{
if((section_weight_normalized>=DimX/16)&&((section_weight>=DimX/2)||((section_sat>=5)&&
   (section_weight>=DimX/3)))&&(last_strip_ratio>=14))
{
	if((upper_lower_ratio>=8)||((section_sat>=5)&&(upper_lower_ratio>=7)&&
		(first_fiber_ratio>=10)))
	{
*(SkyGreenComponents+count_sec)=52;
goto L;
	}
}
}
	   }
	   else
	   {//last_cor31.03.17
if((section_sat>=5)&&(section_gray_zone>=1)&&(section_gray_zone<=2))
{
    if((bound_cond>0)&&(last_strip_ratio>=10)&&(section_length>=NumStrips/6))
	{
if((section_weight>=3*DimX)&&(section_weight_normalized>=DimX/12))
{
if(count_sec>=number_of_left_sections)
		 {
			 if((section_incline>0)||(section_incl_curve>0))
			 {
*(SkyGreenComponents+count_sec)=62;
goto L;

			 }
		 }
		 else
		 {
		 if((section_incline<0)||(section_incl_curve<0))
			 {
*(SkyGreenComponents+count_sec)=62;
goto L;
			 }
		 }
}
    }
}
	   }//last_cor31.03.17
   }
}
		}
	}//sh1820

	if((section_hue_zone==3)&&(section_hue>=18)&&(section_sat>=6))
	{
		if((last_strip_ratio>=14)&&(section_length>=NumStrips/4))
		{
			if(section_weight>=4*StripWidth)
			{
				if(((section_gray_zone>=1)&&(section_gray_zone<=2))||(section_gray>=5))
                {
if(bound_cond>0)
{
*(SkyGreenComponents+count_sec)=72;
goto L;
}
				}
			}
		}
	}
	if((section_hue>=5)&&(section_hue<=7))
	{
      if((section_gray_zone>=2)&&(section_gray_zone<=5))
      {
		  if(section_sat>=6)
		  {
			  *(SkyGreenComponents+count_sec)=3;
			  goto L;
		  }
		  else
		  {
             if(section_sat>=3)
		     {
			 *(SkyGreenComponents+count_sec)=13;
			 goto L;
		     }
			 else
			 {
				 if((section_sat>=3)&&((section_sat==2)&&(section_gray_zone<=2)))
		         {
			     *(SkyGreenComponents+count_sec)=23;
				 goto L;
		         }
			 }
		  }
	  }
	}
}//esh817
		if((section_hue<=21)&&(section_hue_zone==3)&&(*(SkyGreenComponents+count_sec)==0))
		{//shshz
if(section_gray_zone<=1)
{//sgz1
if(section_sat>=8)
{//ss8
if(last_strip_ratio>=14)
{
	*(SkyGreenComponents+count_sec)=42;
	goto L;
}
    if(bound_cond==1)
	{
if(count_sec>=number_of_left_sections)
{
	if(((section_incline>0)&&(section_incl_curve>0))||
		((section_incl_curve>=1)&&(section_incl_curve<=2)))
	{
*(SkyGreenComponents+count_sec)=42;
goto L;
	}
}
else
{
if(((section_incline<0)&&(section_incl_curve<0))||
		((section_incl_curve>=-2)&&(section_incl_curve<=-1)))
	{
*(SkyGreenComponents+count_sec)=42;
goto L;
	}
}
	}
}//ss8
}//sgz1
else
{
if(section_gray_zone>=2)
{
	if(section_sat>=5)
	{
if((last_strip_ratio>=12)||((last_strip_ratio>=11)&&(section_weight_normalized>=DimX/8)))
{
	*(SkyGreenComponents+count_sec)=42;
	goto L;
}
	}
}
}
		}//shshz

L:;
		if((*(SkyGreenComponents+count_sec)%10)==3)
		{//last_cor20.11.17
if(count_sec>=number_of_left_sections)
{
if((section_incline>0)&&(section_incline>=2)&&
   (last_strip_ratio>=8)&&(section_weight>=3*DimX)&&
   (section_weight_normalized>=DimX/4)&&(min_left<=DimX/8))
{
*(SkyGreenComponents+count_sec)=2;
}
}
		}//last_cor20.11.17
	}
}

//===================================================
int

CImageProcess::SectionGeometry(int section_num,int* meam_l,int* mean_u)
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
	int section_weight;
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


	prior=-1;
	upper_sum=0;
	lower_sum=0;
	upper_count=0;
	lower_count=0;
	upper_mean=-1;
	lower_mean=-1;
last_left_section=ColorSection->Number_of_sections_left;
	if(section_num<last_left_section)
				{
				left_right=0;
				}
				else
				{
				left_right=1;
				}
last_fiber=ColorSection->section_fibre_last[section_num];
first_fiber=ColorSection->section_fibre_first[section_num];
mid_fiber=(last_fiber-first_fiber)/2 + first_fiber;
section_length=last_fiber-first_fiber+1;
last_strip_ratio=(16*last_fiber)/(NumStrips-1);
section_weight=ColorSection->section_weight[section_num];
if(section_length>=4)
{
	/*incline_points=new int[section_length];
	oppos_incline_points=new int[section_length];
	seq_points=new int[section_length];
	oppos_seq_points=new int[section_length];*/
	count_point=0;
/*memset(incline_points,(int) '\0',sizeof(int)*(section_length));
memset(oppos_incline_points,(int) '\0',sizeof(int)*(section_length));*/
first_boun=-1;
last_boun=-1;
   for(int count_fiber=first_fiber;count_fiber<=last_fiber;count_fiber++)
   {//loop_fiber
current_bunch=ColorSection->DescrSec[section_num].location_of_section[count_fiber];
		if(current_bunch>0)
		{//cb>
			current_bunch--;
			current_bunch=ColorInt[count_fiber].old_bunch_number[current_bunch];
			current_fiber_beg=
				ColorInt[count_fiber].ColoredIntervalsStructure->BegInterv[current_bunch];
			current_fiber_end=
				ColorInt[count_fiber].ColoredIntervalsStructure->EndInterv[current_bunch];
			current_fiber_length=current_fiber_end-current_fiber_beg+1;
if(count_fiber<=mid_fiber)
{
lower_sum+=current_fiber_length;
lower_count++;
}
else
{
upper_sum+=current_fiber_length;
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
	if(lower_count>0)
	{
lower_mean=lower_sum/lower_count;
	}
	if(upper_count>0)
	{
upper_mean=upper_sum/upper_count;
	}
	*meam_l=lower_mean;
	*mean_u=upper_mean;
	prior=1;
   /*delete [] incline_points;
   delete [] oppos_incline_points;
   delete [] seq_points;
   delete [] oppos_seq_points;*/
}
	return(prior);
}

//===============================================================================
int

CImageProcess::GreenStripDescription(int adjacent_bunch,int max_section_bunch,
int strp_number,int left_r,int* last_green,int* tot_green_length,int* first_green,
int* last_section_green_bunch,int* last_green_section,int * first_green_section_bunch,
int* first_green_section)
{//pr
	int prior;
	int bunch_first;
	int last_member;
	int color_sign;
	int current_bunch_number;
	int first_gr;
	int last_gr;
	int first_green_sec;
	int last_green_sec;
	int shift_in_array;
	int shift_in_mem;
	int bunch_section;
	int old_bunch_number;
	int old_bunch_number_beg;
    int old_bunch_number_end;
    int old_bunch_number_length;
	int gr_length;
	int first_gr_sec_bunch;
	int last_gr_sec_bunch;


last_gr_sec_bunch=-1;
first_gr_sec_bunch=-1;
old_bunch_number_beg=-1;
old_bunch_number_end=-1;
old_bunch_number_length=0;
prior=-2;//last_cor15.12.16
first_gr=-1;
last_gr=-1;
bunch_first=adjacent_bunch;
shift_in_array=strp_number*MaximumNumberOfCoveringElements;
shift_in_mem=strp_number*MAX_COL_INT;
first_green_sec=-1;
last_green_sec=-1;
gr_length=0;
if(left_r)
{
last_member=bunch_first-max_section_bunch;
}
else
{
last_member=max_section_bunch-bunch_first;
}
for(int count_bunch=0;count_bunch<last_member;count_bunch++)
  {//secloop
if(left_r)
{
current_bunch_number=bunch_first-count_bunch;
}
else
{
current_bunch_number=bunch_first+count_bunch;
}
old_bunch_number=ColorInt[strp_number].old_ordered_bunch_number[current_bunch_number];
	  color_sign=ColorOfBasicBunches[shift_in_array+current_bunch_number];
 if(color_sign==1)
 {//cs=1
old_bunch_number_beg=
ColorInt[strp_number].ColoredIntervalsStructure->BegInterv[old_bunch_number];
old_bunch_number_end=
ColorInt[strp_number].ColoredIntervalsStructure->EndInterv[old_bunch_number];
old_bunch_number_length=old_bunch_number_end-old_bunch_number_beg+1;
gr_length+=old_bunch_number_length;
	  if(first_gr==-1)
	  {
first_gr=current_bunch_number;
prior=1;
	  }
last_gr=current_bunch_number;

 if(left_r)
{
 bunch_section=
SectionTraceLeft[shift_in_mem+old_bunch_number];

 }
 else
 {
bunch_section=
SectionTraceRight[shift_in_mem+old_bunch_number];
 }

bunch_section=bunch_section%1000;

if(bunch_section>0)
{//gas>0
bunch_section--;
    if(first_green_sec==-1)
    {
    first_green_sec=bunch_section;
	first_gr_sec_bunch=current_bunch_number;
    }
last_green_sec=bunch_section;
last_gr_sec_bunch=current_bunch_number;
}
 }//cs=1
  }//secloop
*last_green=last_gr;
*tot_green_length=gr_length;
*first_green=first_gr;
*last_section_green_bunch=last_gr_sec_bunch;
*last_green_section=last_green_sec;
*first_green_section_bunch=first_gr_sec_bunch;
*first_green_section=first_green_sec;
return(prior);
}//pr
////////////////////////////////////////////////////////////////////////////////////
int

CImageProcess::LongestPlusMinusChain(int section_number,int section_length,int* section_ends,
int* next_member_appropriate,int* chains_appropriate,int* chain_first_member,
int* number_of_ch,int sign)
{
	int prior;
	int curr_member;
	int next_member;
	int first_end;
	int differ;
	//int aver_differ;
	int first_strip;
	int first_strip_ratio;
	int last_strip;
    int last_strip_ratio;
	int next_appropriate;
	int number_of_chains;
	int chain_test;
	//int shift_len;
	int close_excessive;

	close_excessive=-1;
	prior=-1;
	chain_test=-1;
	number_of_chains=0;

first_strip=ColorSection->section_fibre_first[section_number];
first_strip_ratio=(16*first_strip)/NumStrips;
last_strip=ColorSection->section_fibre_last[section_number];
last_strip_ratio=(16*last_strip)/NumStrips;
first_end=section_length-1;
for(int count_end=0;count_end<first_end;count_end++)
  {//secloop
	  curr_member=*(section_ends+count_end);
for(int count_shift=count_end+1;count_shift<section_length;count_shift++)
  {//sec1loop
	  next_member=*(section_ends+count_shift);
      differ=next_member-curr_member;

	  if(sign>0)
	  {
		  if(differ>0)
		  {
*(next_member_appropriate+count_end)=count_shift;
prior=1;
break;
		  }
	  }
	  if(sign<0)
	  {
		  if(differ<0)
		  {
*(next_member_appropriate+count_end)=count_shift;
prior=1;
break;
		  }
	  }
}//sec1loop
  }//secloop
for(int count_chain=0;count_chain<first_end;count_chain++)
{//chain_loop
next_appropriate=*(next_member_appropriate+count_chain);
if(count_chain>close_excessive)
{
curr_member=*(section_ends+count_chain);
if(next_appropriate==(count_chain+1))
{
	next_member=*(section_ends+count_chain+1);
	differ=abs(next_member-curr_member);
	if(differ<=5*StripWidth)
	{
	//shift_len=first_end-count_chain;
chain_test=AppropriateChain(count_chain,first_end,next_member_appropriate,
chains_appropriate,chain_first_member,section_ends,sign,number_of_chains);
	}
	else
	{
    chain_test=count_chain;
	}
    if(chain_test>count_chain)
    {
    number_of_chains++;
	close_excessive=chain_test;
    }
}
}
else
{
if(close_excessive>0)
{
*(chains_appropriate+count_chain)=close_excessive;
}
}
}//chain_loop
*number_of_ch=number_of_chains;
return(prior);
}
////////////////////////////////////////////////////////////////////////////////////
int

CImageProcess::AppropriateChain(int first_appropriate,int shift_length,int* appropriate_numbers,
int* chains_appropriate,int* chain_first,int* sect_ends,int sign,int chain_number)
{
	int next_appropriate;
	int prior;
	int count_chain;
	int first_coordinate;
	int second_coordinate;
	int differ;
	int previos_appropriate;

prior=-1;
previos_appropriate=first_appropriate+1;//last_cor29
  for(count_chain=(first_appropriate+1);count_chain<shift_length;count_chain++)
  {//chain_loop
first_coordinate=*(sect_ends+previos_appropriate);
next_appropriate=*(appropriate_numbers+count_chain);
second_coordinate=*(sect_ends+next_appropriate);
differ=abs(second_coordinate-first_coordinate);
if((next_appropriate>(count_chain+1))||(!next_appropriate)||(differ>5*StripWidth))
{
prior=count_chain;
*(chains_appropriate+first_appropriate)=prior;
break;
}
previos_appropriate=next_appropriate;
  }//chain_loop
if(prior<0)
{
*(chains_appropriate+first_appropriate)=count_chain;
prior=count_chain;
}
*(chain_first+chain_number)=first_appropriate+1;
return(prior);
}
////////////////////////////////////////////////////////////////////////////////////
int

CImageProcess::ChainLongest(int sec_length,int number_of_chains,int* coordinates,int* appropriate_numbers,
int* chains_appropriate,int* chain_first,int* last_chain_last,int* chains_prolongation,int sign)
{
int max_chain_length;
int max_chain_number;
int chain_last;
int chain_length;
int chain_first_member;
int count_chain;
int number_of_connected_chains;
int next_chain_first;
int next_chain_last;
int first_coordinate;
int second_coordinate;
int differ;
int differ1;
int abs_differ1;
int boundary_chain;
int previous_last;
int next_chain_first_coordinate;

max_chain_length=0;
chain_first_member=-1;
max_chain_number=-1;
number_of_connected_chains=0;
boundary_chain=-1;

for(count_chain=0;count_chain<number_of_chains;count_chain++)
  {//chain_loop
chain_first_member=*(chain_first+count_chain);
if(chain_first_member>0)
{
	chain_first_member-=1;
}
chain_last=*(chains_appropriate+chain_first_member);
if((chain_last-chain_first_member)<=1)
{
	goto L;
}
chain_length=chain_last-chain_first_member+1;
if(chain_length>max_chain_length)
{
max_chain_length=chain_length;
max_chain_number=count_chain;
}

previous_last=chain_last;
   if(count_chain>boundary_chain)
   {
boundary_chain=count_chain;
for(int count_chain1=count_chain+1;count_chain1<number_of_chains;count_chain1++)
  {//chain_loop1
next_chain_first=*(chain_first+count_chain1);

if(next_chain_first>0)
{
	next_chain_first-=1;
	next_chain_first_coordinate=*(coordinates+next_chain_first);
	if((next_chain_first-previous_last)<=3)
	{
    next_chain_last=*(chains_appropriate+next_chain_first);
	first_coordinate=*(coordinates+previous_last-1);
	second_coordinate=*(coordinates+next_chain_last-1);
	differ=second_coordinate-first_coordinate;
	differ1=next_chain_first_coordinate-*(coordinates+previous_last);
	abs_differ1=abs(differ1);
	if(sign>0)
	{
if((differ>0)&&(abs_differ1<9*StripWidth))
{
	boundary_chain=count_chain1;
	previous_last=next_chain_last;
	number_of_connected_chains++;
}
else
{
	          if(count_chain<32)
			   {
				   if((boundary_chain+1)==count_chain1)
				   {
	*(chains_prolongation+count_chain)=boundary_chain;
	break;
				   }
				   else
				   {
					   break;
				   }
	           }
}
	}
	else
	{
       if(sign<0)
	   {
		   if((differ<0)&&(abs_differ1<9*StripWidth))
           {
	       boundary_chain=count_chain1;
           previous_last=next_chain_last;
		   number_of_connected_chains++;
           }
		   else
		   {
			   if(count_chain<32)
			   {
				   if((boundary_chain+1)==count_chain1)
			   *(chains_prolongation+count_chain)=boundary_chain;
			   break;
			   }
			   else
			   {
				   break;
			   }
		   }
	   }
	}
	}
	else
	{
		break;
	}
}
}//chain_loop1

if(boundary_chain==(number_of_chains-1))
{
	if(count_chain<32)
			   {
				   if(boundary_chain>count_chain)
			   *(chains_prolongation+count_chain)=boundary_chain;
			   }
}
   }
L:;
  }//chain_loop
*last_chain_last=chain_last;
return(max_chain_number);
}



int CImageProcess::StraightApproximation(int* array_elements,int* paint,int first_member, int last_member,int* reliability,int left_right)
{
	int node_coord1;
	int node_coord;
	int total_shift_curve;
	int pos_shift_curve;
	int total_pos_curve;
	int neg_shift_curve;
	int total_neg_curve;
	int shift_zero;
	int shift_zero1_plus;
	int shift_zero1_minus;
	int hist_arg_div;
	int shift_divisor;
	int residual;
	int count_pos;
	int count_neg;
	int prior;
	int abs_arg;
	int hist_arg_neg_incl;
	int hist_arg_pos_incl;
	int node_coord0;
	int hist_arg;
	int hist_arg1;
	int dim_new_hist;
	int maximum_red;
	int triple_maximum_red;
    int maximum_value_red;
    int triple_maximum_value_red;
    int one_pix_neighborhood_red;
    int two_pix_neighborhood_red;
	int zero_two_pix_neighb_red;
	int num_loc_max;
    int second_max;
	int second_max_value;
	int beg_noticable;
	int end_noticable;
	int noticable_sum;
	int noticable_sum_length;
	int max_weight;
    int small_angle_weight;
    int one_max_weight;
    int two_max_weight;
    int noticable_sum_weight;
	int count_zero;
	int* local_maxim_points;
	int shift_pos[16];
	int shift_neg[16];
	int hist_pos[64];
	int hist_neg[64];
	int pos_deviation_max[16];
    int neg_deviation_max[16];

    count_zero=0;
	max_weight=-1;
    small_angle_weight=-1;
    one_max_weight=-1;
    two_max_weight=-1;
    noticable_sum_weight=-1;
	noticable_sum_length=-1;
	triple_maximum_red=-1;
    maximum_value_red=-1;
    triple_maximum_value_red=-1;
    one_pix_neighborhood_red=-1;
    two_pix_neighborhood_red=-1;
	zero_two_pix_neighb_red=-1;
	num_loc_max=-1;
    second_max=-1;
	second_max_value=-1;
	beg_noticable=-1;
	end_noticable=-1;
	noticable_sum=-1;
	dim_new_hist=12;
	prior=-1;
	count_pos=0;
	count_neg=0;
	shift_zero=0;
	shift_zero1_plus=0;
    shift_zero1_minus=0;
	total_shift_curve=0;
	total_pos_curve=0;
	pos_shift_curve=0;
	neg_shift_curve=0;
	total_neg_curve=0;
local_maxim_points=new int[dim_new_hist];
memset(local_maxim_points,(int) '\0',(sizeof(int))*dim_new_hist);
memset(hist_pos,(int) '\0',(sizeof(int))*64);
memset(hist_neg,(int) '\0',(sizeof(int))*64);
memset(shift_pos,(int) '\0',(sizeof(int))*16);
memset(shift_neg,(int) '\0',(sizeof(int))*16);
memset(pos_deviation_max,(int) '\0',(sizeof(int))*16);
memset(neg_deviation_max,(int) '\0',(sizeof(int))*16);
	node_coord0=array_elements[0];
for(int count_mem=first_member;count_mem<last_member;count_mem++)
    {//cycle members
		node_coord1=*(array_elements+count_mem+1);
		node_coord=*(array_elements+count_mem);
			node_coord1-=node_coord;
			//node_coord1=node_coord1;
total_shift_curve+=node_coord1;
if(node_coord1>0)
{//nc>=
	pos_shift_curve++;
	total_pos_curve+=node_coord1;
	     if(node_coord1<63)
	      {
	      node_coord1=paint[node_coord1];

	if(node_coord1<15)
	{
shift_pos[node_coord1]++;
	}
	else
	{
	shift_pos[15]++;

	}
	        }
		 else
		 {
			 node_coord1=paint[63];
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
	      if(node_coord1<0)
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
	}
   else
   {
shift_neg[15]++;
   }
     }
else
{
	node_coord1=paint[63];
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
		  else
		  {
		  shift_zero++;
		  }
}
node_coord=*(array_elements+count_mem);
    for(int count_shift=count_mem;count_shift<last_member;count_shift++)
        {//cycle shift
hist_arg=*(array_elements+count_shift+1)-node_coord;
hist_arg1=*(array_elements+count_shift+1)-*(array_elements+count_shift);

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
		if(hist_arg>0)
		{
		hist_arg_div++;
		}
		else
		{
			hist_arg_div--;
		}
	}
}
if(hist_arg>0)
{//ha
	count_pos++;
	if(hist_arg_div<63)
	{
		hist_pos[hist_arg_div]++;
		hist_arg_pos_incl=paint[hist_arg_div];
		if(hist_arg_pos_incl>=1)
		{//last_cor24.07.17
pos_deviation_max[hist_arg_pos_incl]++;
		}
		else
		{
		shift_zero1_plus++;
		}
	}
	else
	{
		hist_pos[63]++;
	}
}//ha
else
{//e1
if(!hist_arg)
{
	count_zero++;
	shift_zero1_plus++;

}
else
{
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
		if(hist_arg_neg_incl>=1)
		{//last_cor24.07.17
neg_deviation_max[hist_arg_neg_incl]++;
		}
		else
		{
			shift_zero1_minus++;
		}
	       }
	        else
	        {
		    hist_neg[63]++;
	        }
	}
}
}//e1
	}//cycle shift
    }//cycle members
	if(left_right)
	{
		if(count_neg>0)
		{
maximum_red=MaximumTripleMaximumReduced(neg_deviation_max,dim_new_hist,&triple_maximum_red,
&maximum_value_red,&triple_maximum_value_red,&one_pix_neighborhood_red,
&two_pix_neighborhood_red,&zero_two_pix_neighb_red,local_maxim_points,&num_loc_max,
&second_max,&second_max_value,&beg_noticable,&end_noticable,&noticable_sum);
max_weight=(32*maximum_value_red)/(count_neg);
small_angle_weight=(32*shift_zero1_minus)/(count_neg);
//small_angle_weight_plus=(32*(shift_zero1_plus+count_zero))/(count_pos);
//small_angle_weight=(32*(shift_zero1_plus+count_zero+shift_zero1_plus))/(count_pos+count_neg);
one_max_weight=(32*one_pix_neighborhood_red)/(count_neg);
two_max_weight=(32*two_pix_neighborhood_red)/(count_neg);
noticable_sum_weight=(32*noticable_sum)/(count_neg);
noticable_sum_length=end_noticable-beg_noticable+1;
		}
	}
	else
	{
maximum_red=MaximumTripleMaximumReduced(pos_deviation_max,dim_new_hist,&triple_maximum_red,
&maximum_value_red,&triple_maximum_value_red,&one_pix_neighborhood_red,
&two_pix_neighborhood_red,&zero_two_pix_neighb_red,local_maxim_points,&num_loc_max,
&second_max,&second_max_value,&beg_noticable,&end_noticable,&noticable_sum);
        if(count_pos>0)
		{
maximum_red=MaximumTripleMaximumReduced(pos_deviation_max,dim_new_hist,&triple_maximum_red,
&maximum_value_red,&triple_maximum_value_red,&one_pix_neighborhood_red,
&two_pix_neighborhood_red,&zero_two_pix_neighb_red,local_maxim_points,&num_loc_max,
&second_max,&second_max_value,&beg_noticable,&end_noticable,&noticable_sum);
max_weight=(32*maximum_value_red)/(count_pos);
small_angle_weight=(32*(shift_zero1_plus+count_zero))/(count_pos);
one_max_weight=(32*one_pix_neighborhood_red)/(count_pos);
two_max_weight=(32*two_pix_neighborhood_red)/(count_pos);
noticable_sum_weight=(32*noticable_sum)/(count_pos);
noticable_sum_length=end_noticable-beg_noticable+1;
       }
	}
	if((max_weight>=14)&&((one_max_weight>=28)||
		((maximum_red==triple_maximum_red)&&(one_max_weight>=24)&&(two_max_weight>=28))))
	{
		*reliability=1;
		if(left_right)
		{
			prior=-maximum_red;
		}
		else
		{
        prior=maximum_red;
		}
	}
	else
	{
if((one_max_weight>=28)||
		((maximum_red==triple_maximum_red)&&(one_max_weight>=24)&&((two_max_weight>=28)||
		(max_weight>=18)||((second_max<0)&&((one_max_weight>=23)&&(max_weight>=14))))))
{
if(max_weight>=10)
{
*reliability=2;
		if(left_right)
		{
			prior=-maximum_red;
		}
		else
		{
        prior=maximum_red;
		}
}
}
	}
    delete[] local_maxim_points;
	return(prior);
}




int CImageProcess::SequenceOfChainsLongest(int sec_length,int sect_num,int number_of_chains,
                                                        int* section_array,int* chains_prolongated,int* chain_first,int* chains_appropriate,
                                                    int* first_prolong,int* max_length)
{
	int count_chain;
	int chain_sequence_last;
	int chain_length;
	int chain_total_length;
	int maximum_chain_length;
	int sequence_optimal;
	int chain_first_member;
	int chain_last_member;
	int num_of_chains;
	int first_prolongated;
	int chain_first_previous;
	int chain_last_previous;
	int hole_length;
	int chain_previous_length;
	int initial_first_member;
	int initial_last_member;
	int intial_first_member_coordinate;
	int intial_last_member_coordinate;

   initial_first_member=-1;
   initial_last_member=-1;
   intial_first_member_coordinate=-1;
   intial_last_member_coordinate=-1;
	chain_previous_length=-1;
	hole_length=-1;
	chain_first_previous=-1;
	chain_last_previous=-1;
	first_prolongated=-1;
	chain_total_length=0;
maximum_chain_length=-1;
sequence_optimal=-1;
	num_of_chains=std::min(number_of_chains,31);
	for(count_chain=0;count_chain<number_of_chains;count_chain++)
{//chain_loop
	chain_total_length=0;
chain_sequence_last=chains_prolongated[count_chain];
  if((chain_sequence_last>0)&&(chain_sequence_last>count_chain))
  {
	if(first_prolongated<0)
	{
		first_prolongated=count_chain;
	}
initial_first_member=*(chain_first+count_chain);
initial_first_member-=1;
initial_last_member=*(chains_appropriate+initial_first_member);
intial_first_member_coordinate=*(section_array+initial_first_member);
chain_first_member=*(chain_first+count_chain);
chain_first_member--;
chain_last_member=*(chains_appropriate+chain_first_member);
chain_total_length=chain_last_member-chain_first_member+1;
for(int count_chain1=count_chain+1;count_chain1<=chain_sequence_last;count_chain1++)
{//chain_loop1
chain_first_member=*(chain_first+count_chain1);
         if(chain_first_member>0)
		 {
chain_first_member--;
chain_last_member=*(chains_appropriate+chain_first_member);
chain_length=chain_last_member-chain_first_member+1;
if(count_chain1>count_chain)
{
chain_first_previous=*(chain_first+count_chain1-1);
chain_first_previous-=1;
chain_last_previous=*(chains_appropriate+chain_first_previous);
chain_previous_length=chain_last_previous-chain_first_previous+1;
hole_length=chain_first_member-chain_last_previous;
}
if((hole_length>0)&&(hole_length<3))
{
chain_total_length+=chain_length;
}
else
{
	break;
}
		 }
}//chain_loop1
if(chain_total_length>maximum_chain_length)
{
maximum_chain_length=chain_total_length;
sequence_optimal=count_chain;
}
  }

}//chain_loop
	*first_prolong = first_prolongated;
    *max_length = maximum_chain_length;
	return (sequence_optimal);
}

