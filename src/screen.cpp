/*
 * =====================================================================================
 *
 *       Filename:  screen.cpp
 *
 *    Description:  Tools for screening generations
 *
 *        Version:  1.0
 *        Created:  30/12/11 23:54:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José M. González (txemagon / imasen), txema.gonz@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ag.h"
#include "globals.h"
#include "manage_time.h"
#include "gene.h"
#include "allele.h"
#include "chromosomal.h"
#include "screen.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  clear_penalties
 *  Description:  reset the penalties in the population.
 * =====================================================================================
 */
void
clear_summary (Population population)
{
  for (unsigned i = 0; i < population.length; i++)
    {
      population.person[i].summary.extra_staff_number = 0;
      population.person[i].summary.defect_staff_number = 0;
      population.person[i].summary.mutations = 0;
      for (unsigned w = 0; w < population.person[i].width; w++)
	{
	  population.person[i].summary.extra_shifts[w] = 0;
	  population.person[i].summary.weekends_halved[w] = 0;
	  population.person[i].summary.consecutive_weekends[w] = 0;
	  population.person[i].summary.weekends[w] = 0;
	  population.person[i].summary.freedays[w] = 0;
	}
    }
}				/* -----  end of function clear_penalties  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  evaluate_long_shifts
 *  Description:  Evaluates long shifts for a given worker. Maximum extra shift 
 *                corresponds to the days to solve minus the recommend shift width.
 *                The program encodes the number of times the recommended shift width
 *                is exceeded in one day, two days, etc. Encoding base is the lowest
 *                power of two value, greater than days - rmsw. See i
 *                evaluate_consecutive_weekends below for further information.
 * =====================================================================================
 */
unsigned long long
evaluate_extra_shifts (Chromosome chromo, unsigned worker)
{
  unsigned long long value = 0;
  unsigned long long power = 0;
  unsigned together = 0;
  unsigned base = get_shift_base (chromo);
  worker %= chromo.width;

  for (int day = 0; day < chromo.length; day++)
    if (is_working (chromo.gene[day], worker))
      {
	together++;
	if (together > goals[shift_week_idx].value)
	  if (power)
	    power *= base;
	  else
	    power = 1;
      }
    else
      {
	if (together > goals[shift_week_idx].value)
	  value += power;
	together = 0;
	power = 0;
      }

  value += power;

  return value;
}				/* -----  end of function evaluate_long_shifts  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  evaluate_weekends_halved
 *  Description:  Returns the number of weekends halved
 * =====================================================================================
 */
unsigned
evaluate_weekends_halved (Chromosome chromo, unsigned worker)
{
  unsigned value = 0;
  worker %= chromo.width;

  for (unsigned day = SATURDAY; day + 1 < chromo.length; day += WEEK)
    value +=
      (is_working (chromo.gene[day], worker) ^
       is_working (chromo.gene[day + 1], worker));

  return value;
}				/* -----  end of function evaluate_weekends_halved  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  evaluate_consecutive_weekends
 *  Description:  Encode the amount of one, two, three weekends together and so on.
 *                The maximum number of weekends worked together is 
 *                mww = chromo.length / WEEK + 1.
 *                Example for "Sat Sun Mon Tue Wed Thu Fri Sat Sun": 9days/7 + 1 => 2
 *                Thus, encoding base shall be mww = 2. In practice, the next power
 *                of two will be taken.
 * =====================================================================================
 */
unsigned
evaluate_consecutive_weekends (Chromosome chromo, unsigned worker)
{
  unsigned value = 0;
  unsigned power = 0;
  unsigned base = get_consweekend_base (chromo);

  worker %= chromo.width;

  for (int day = SATURDAY; day + 1 < chromo.length; day += WEEK)
    if (!is_working_the_weekend (chromo, worker, day / WEEK))
      {
	value += power;
	power = 0;
      }
    else if (!power)
      power = 1;
    else
      power *= base;

  /* Add the final acum */
  value += power;

  return value;
}				/* -----  end of function evaluate_consecutive_weekends ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  evaluate_extra_staff_number
 *  Description:  Evaluates exceeded number of employees working
 * =====================================================================================
 */
unsigned
evaluate_extra_staff_number (Chromosome chromo)
{
  unsigned value = 0;
  for (int day = 0; day < chromo.length; day++)
    {
      unsigned pw = people_working (chromo.gene[day]);
      unsigned staff = goals[staff_number_ix].value;
      if (day % WEEK == SATURDAY || day % WEEK == SATURDAY + 1)
	staff = goals[staff_weekend_number_idx].value;
      if (pw > staff)
	value++;
    }
  return value;
}				/* -----  end of function evaluate_extra_staff_number  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  evaluate_defect_staff_number
 *  Description:  Evaluates low number of employees working
 * =====================================================================================
 */
unsigned
evaluate_defect_staff_number (Chromosome chromo)
{
  unsigned value = 0;
  for (int day = 0; day < chromo.length; day++)
    {
      unsigned pw = people_working (chromo.gene[day]);
      unsigned staff = goals[staff_number_ix].value;
      if (day % WEEK == SATURDAY || day % WEEK == SATURDAY + 1)
	staff = goals[staff_weekend_number_idx].value;
      if (pw < staff)
	value++;
    }
  return value;
}				/* -----  end of function evaluate_defect_staff_number  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  punish_long_shifts
 *  Description:  Punish long shifts for a given worker
 * =====================================================================================
 */
unsigned
punish_long_shifts (Chromosome chromo)
{
  unsigned penalty = 0;
  unsigned base = get_shift_base (chromo);

  for (unsigned worker = 0; worker < chromo.width; worker++)
    for (unsigned long long extra_shifts =
	 chromo.summary.extra_shifts[worker], n = 1; extra_shifts > 0;
	 extra_shifts /= base, n++)
      penalty +=
	(extra_shifts % base) *
	penalty_points[shift_week_extra_penalty_idx].value * n;

  return penalty;
}				/* -----  end of function punish_long_shifts  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  punish_work_load
 *  Description: Punish low work load or high one 
 * =====================================================================================
 */
unsigned
punish_work_load (Chromosome chromo)
{
  unsigned penalty = 0;
  unsigned worked = 0;

  for (unsigned worker = 0; worker < chromo.width; worker++)
    {
      worked = chromo.length - chromo.summary.freedays[worker];

      penalty += (unsigned) (abs (worked -
				  goals[work_load_idx].value * chromo.length /
				  WEEK / problem[shift_length_idx].value)) *
	penalty_points[work_load_idx].value;
    }

  return penalty;
}				/* -----  end of function punish_work_load  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  punish_halving_weekends
 *  Description:  It is undesirable to work only one day in the weekend.  
 * =====================================================================================
 */
unsigned
punish_halving_weekends (Chromosome chromo)
{
  unsigned penalty = 0;


  for (unsigned worker = 0; worker < chromo.width; worker++)
    penalty +=
      chromo.summary.weekends_halved[worker] *
      penalty_points[halving_weekend_penalty_idx].value;

  return penalty;
}				/* -----  end of function punish_halving_weekends  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  punish_consecutive_weekends
 *  Description:  It is very bad to work two weekends together. 
 * =====================================================================================
 */
unsigned
punish_consecutive_weekends (Chromosome chromo)
{
  unsigned acum = 0;
  unsigned penalty = 0;
  unsigned power = 0;
  unsigned base = get_consweekend_base (chromo);

  for (unsigned worker = 0; worker < chromo.width; worker++)
    for (unsigned cw = chromo.summary.consecutive_weekends[worker], n = 1;
	 cw > 0; cw /= base, n++)
      penalty +=
	(cw % base) *
	penalty_points[consecutive_weekend_injustice_idx].value * n;

  return penalty;
}				/* -----  end of function punish consecutive_weekends ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  punish_bad_stuff_number
 *  Description:  Punish low or high number of employees working
 * =====================================================================================
 */
unsigned
punish_bad_staff_number (Chromosome chromo)
{
  return chromo.summary.extra_staff_number *
    penalty_points[extra_people_penalty_idx].value +
    chromo.summary.defect_staff_number *
    penalty_points[few_people_penalty_idx].value;
}				/* -----  end of function punish_bad_stuff_number  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  punish_different_free_days
 *  Description:  computes the number of free_days for each employee and punishes diffs.  
 * =====================================================================================
 */
unsigned
punish_different_free_days (Chromosome chromo)
{

  unsigned penalty = 0;
  unsigned difference;
  unsigned min = 0xFFFFFFFF;

  for (int worker = 0; worker < chromo.width; worker++)
    if (min > chromo.summary.freedays[worker])
      min = chromo.summary.freedays[worker];

  for (int worker = 0; worker < chromo.width; worker++)
    if ((difference = chromo.summary.freedays[worker] - min) > 1)
      penalty +=
	(difference - 1) * penalty_points[bad_free_days_penalty_idx].value;

  return penalty;
}				/* -----  end of function punish_different_weekend_number  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  punish_different_weekend_number
 *  Description:  computes the number of weekend for each employee and punishes diffs.  
 * =====================================================================================
 */
unsigned
punish_different_weekend_number (Chromosome chromo)
{
  unsigned penalty = 0;
  unsigned difference;
  unsigned min = 0xFFFFFFFF;

  for (int worker = 0; worker < chromo.width; worker++)
    if (min > chromo.summary.weekends[worker])
      min = chromo.summary.weekends[worker];

  for (int worker = 0; worker < chromo.width; worker++)
    if ((difference = chromo.summary.weekends[worker] - min) > 1)
      penalty +=
	penalty_points[weekend_injustice_penalty_idx].value *
	(difference - 1);


  return penalty;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  analyze_aptitude
 *  Description:  Annotate features
 * =====================================================================================
 */
void
analyze_aptitude (Chromosome * chromo)
{
  chromo->summary.extra_staff_number = evaluate_extra_staff_number (*chromo);
  chromo->summary.defect_staff_number =
    evaluate_defect_staff_number (*chromo);
  for (int w = 0; w < chromo->width; w++)
    {
      chromo->summary.extra_shifts[w] = evaluate_extra_shifts (*chromo, w);
      chromo->summary.weekends_halved[w] = evaluate_weekends_halved (*chromo, w);
      // chromo->summary.consecutive_weekends[w] = evaluate_consecutive_weekends (*chromo, w);
      chromo->summary.weekends[w] = get_total_weekends (*chromo, w);
      chromo->summary.freedays[w] = get_total_freedays (*chromo, w);
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  check_aptitude
 *  Description:  Sum penalties up
 * =====================================================================================
 */
void
check_aptitude (Population population)
{
  // todo: fill the penalty struct to run statistics
  for (int i = 0; i < population.length; i++)
    {
      analyze_aptitude (&population.person[i]);

      population.person[i].penalty_sum =
	population.person[i].penalty.different_free_days =
	punish_different_free_days (population.person[i]);

      population.person[i].penalty_sum +=
	population.person[i].penalty.long_shifts =
	punish_long_shifts (population.person[i]);

      population.person[i].penalty_sum +=
	population.person[i].penalty.bad_staff_number =
	punish_bad_staff_number (population.person[i]);


      population.person[i].penalty_sum +=
	 population.person[i].penalty.weekends_halved =
	 punish_halving_weekends (population.person[i]);

      /*
      population.person[i].penalty_sum +=
	 population.person[i].penalty.consecutive_weekends =
	 punish_consecutive_weekends (population.person[i]);
*/
      population.person[i].penalty_sum +=
	population.person[i].penalty.different_weekend_number =
	punish_different_weekend_number (population.person[i]);

      population.person[i].penalty_sum +=
	population.person[i].penalty.work_load =
	punish_work_load (population.person[i]);

    }
}				/* -----  end of function check_long_shifts  ----- */
