/*
 *
 *
 *
 *
 *
 *
 */





#ifndef COLORINTERVALSELECT_H
#define COLORINTERVALSELECT_H




#include "Strip.h"





class CColorIntervalSelect
{
public:
	CColorIntervalSelect();
	virtual ~CColorIntervalSelect();
public:
	int dimX;
	int dimY;
	bool GGBorGGR;
	//Input_Data *InputDataCS;
	//bool FindLabels;
	bool FindSky;
	int StripWidth;
	int StripLength;
	int PressedLength;
	int TotalNumInt;
	int MaximalNumber;
	int Max_number;
	int strip_number;
	int NumStrip;
	int NumberOfIntervals; //number of the initial colored intervals
	int NumberOfIntervals1; //number of the initial colorless intervals
	int RefinedNumberOfBunches;
	int Disordering;
	int HueCorrection[16];

	CStrip* StripCurrent; //copies of the class constructed
	TIntColor* ColorInform;
	TIntColored* ColoredIntervalsStructure;
	TIntColoredCharacteristics* ColorBunchesCharacteristics;
	TIntColorLessBack* ColorLessIntervalsStructure;
    bool VideoCameraIsLoaded;

	int* intensities_with_colored_int;
	int NumInterestingIntensities;
	int NumberOfColoredIntervals; //number of integrated colored intervals
	int NumberOfColorlessIntervals; //number of integrated colorless intervals
	int NumberOfIntervalsInCovering;
	int* OldNumbers;
	int* painted_strip_saturation; // array of saturated intensities
	int* painted_numbers_saturation; //array of numbers of saturated intervals
	int* saturation_consistency;
	int* painted_strip_colored; // array of colored intensities
	int* painted_numbers_colored; //array of the numbers of colored intervals
	int* consistency_colored;
	int* painted_strip_colored_long;// array of colored intensities
	int* painted_numbers_colored_long;//array of the numbers of colored intervals
	int* consistency_colored_long;
	int* bunch_blocking;
	int* bunches_occurred;
	int* new_bunch_number;
	int* old_bunch_number;
	int* ordered_bunch_number;
	int* old_ordered_bunch_number;
	int* bunch_connections;
	int* visible_bunches;
	int* left_continuation;
	int* right_continuation;
	int* left_adjacent;
	int* right_adjacent;
	int* new_left_continuation;
	int* new_right_continuation;
	int* new_left_adjacent;
	int* new_right_adjacent;


	void FindingStructureParametrs(int* num_of_int, int* intens_with_int,
			int* OldNumb, int* NumInterestingIntensities);

	void HueSaturationSideCalculation(int num_intensity,int TheOtherRatio,
			int gray,int* Hue,int* Saturation,int* TriangleSide,
			int* CircleHue,int* DoubleSaturation);

	void HueSaturationSideCalculationGGR(int num_intensity, int TheOtherRatio,
			int* Hue, int* Saturation, int* TriangleSide, int* CircleHue,
			int* DoubleSaturation);

	void Strip_value_painting_saturation(int intens, int beg_int, int end_int,
			int num_int, int intens_saturation, int* painted_strip_sc,
			int* painted_numbers_sc, int* saturation_consistency_sc);

	void AnalysisColoredRanges(int* visibility, int* inver_num, int* interv_num,
			int* interval_intens);

	int GroupingCloseColoredIntervals(int intensity, int num_int,
			int* continuation_investigated, int iter_step, int* members,
			int* inver_num, int* start_poin, int* intens_num, int* inter_num);

	int Find_Better_Representative(int intens, int num_int, int* total_int_new,
			int* interval_collection, int start_point, int* interv_num,
			int* interval_intens);

	int Intersection_measure(int beg_first, int end_first, int beg_second,
			int end_second, int* greater);

	int Intersection_measure_ratios(int beg_first, int end_first,
			int beg_second, int end_second, int* greater,
			int* inter_length_ratio, int* inter_length_ratio1);

	int ColorIntervalComparison(int left_boundary, int right_boundary,
			int left_boundary_new, int right_boundary_new);

	int GrayIntersection(int beg_first, int end_first, int beg_second,
			int end_second);

	void

	VisibilityFinding(int* visibility_measure, int* interv_num,
			int* interval_intens, int* inver_num, int beg_loop, int end_loop,
			int* painted_strip, int* painted_numbers);

	void Hue_Painting(int beg_int, int end_int, int cluster_num,
			int* painted_hue_range);

	void BunchClassification();

	void Strip_value_painting_bunch(int bunch, int beg_int, int end_int,
			int bunch_signif, int* bunch_strip, int* number_of_bunch,
			int* bunch_significance,int array_length);

	int Test_Junction(int bunch, int neighbor_bunch, int occurrence);

	int

	Test_Junction_Corrected(int bunch,int neighbor_bunch,int occurrence,
	int Intersection,int indic_length,int ratio_length,int ratio_length1,
	int *hue_close_r,int *sat_close_r,int* gray_close_r);

	int

	Additional_Junction(int bunch, int intensity, int interval);

	void
	Bunch_painting(int bunch_number, int beg_int, int end_int,
			int bunch_consist, int * valuable_bunch, int* quantity_of_bunches,
			int* bunch_consistency);

	void
	Bunch_relation(int bunch, int* visible_bunches, int* intersecting_bunches,
			int* number_of_intersect, int* inters_type);

	void

	AnalysisColorLessBack(int* inver_num,
			int* last_interval_continuation_investigated, int* visibility,
			int* interv_num, int* interval_intens, int* members,
			int* number_members);

	int

	ColorLessGrouping(int intensity, int interval_number, int* inver_num,
			int* continuation_investigated, int* members, int* start_poin,
			int* visibility);

	int

	Test_Interaction(int difference_hue_zone, int hue_difference,
			int admissible_hue_dif, int sat_difference, int admissible_sat_dif,
			int bunch_mean_saturation, int neighbor_bunch_mean_saturation);

	int

	Measure1_of_proximity(int first_left, int first_right, int second_left,
			int second_right, int *proximity1, int *proximity2, int* greater);

	void

	Merging(int bunch, int bunch_new);
	void

	Extension(int bunch, int bunch_new);
	void

	Colored_intervals_findingColorDivision(int NumInterestingIntensities,
			int* OldNumbers, int* num_of_int, TIntCharact* IntAllInform);
	void

	Bunch_continuation(int direct, int bunch_number, int bunch_occurrence,
			int beg_int, int end_int, int paint_start, int paint_finish,
			int* continuation);
	int

	Right_hole_finding(int point_start, int point_finish, int* gap_length);

	int

	Left_hole_finding(int point_start, int point_finish, int* gap_length);

	void

	Strip_value_correction_bunch(int bunch, int paint_start, int paint_finish,
			int bunch_signif);

	int

	Left_extension(int left_neighb);

	int

	Measure2_of_proximity(int first_left,int first_right,
	int second_left,int second_right,int* proximity1,int* proximity2,int* left_shift,int* right_shift,
	int* ind_length,int* inter_length_ratio,int* inter_length_ratio1);

	void

	SimpleMerging(int bunch,int bunch_new);

	void

    Cleaning(int numb);

	int

	CompleteOrdering(int* bunch_left_adjacent,int* bunch_left_contrast,
	int* bunch_right_adjacent,int* bunch_right_contrast,int* last_bunch_number,int* smallest_coord,
	int* greatest_coord);

	int

	LinearOrdering(int* bunch_left_adjacent,int* bunch_left_contrast,
	int* bunch_right_adjacent,int* bunch_right_contrast,int first_bunch_number,int bunch_last);

	int

	Right_hole_finding_modified(int point_start,int point_finish,int* gap_length);

};

#endif