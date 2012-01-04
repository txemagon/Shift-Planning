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

#include "ag.h"
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
clear_penalties (Population population)
{
  for (unsigned i = 0; i < population.length; i++)
    {
      population.person[i].summary.bad_staff_number = 0;
      population.person[i].summary.different_weekend_number = 0;
      population.person[i].summary.different_free_days = 0;
      for (unsigned w = 0; w < population.person[i].width; w++)
	{
	  population.person[i].summary.long_shifts[w] = 0;
	  population.person[i].summary.weekends_halved[w] = 0;
	  population.person[i].summary.consecutive_weekends[w] = 0;
	  population.person[i].summary.weekends[w] = 0;
	  population.person[i].summary.freedays[w] = 0;
	}
    }
}				/* -----  end of function clear_penalties  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  punish_long_shifts
 *  Description:  Punish long shifts for a given worker
 * =====================================================================================
 */
unsigned
punish_long_shifts (Chromosome chromo, unsigned worker)
{
  unsigned penalty = 0;
  unsigned together = 0;
  worker %= chromo.width;

  for (int day = 0; day < chromo.length; day++)
    if (is_working (chromo.gene[day], worker))
      {
	together++;
	if (together > SW)
	  penalty += SWP;
      }
    else
      together = 0;

  return penalty;
}				/* -----  end of function punish_long_shifts  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  punish_work_load
 *  Description: Punish low work load or high one 
 * =====================================================================================
 */
unsigned
punish_work_load (Chromosome chromo, unsigned worker)
{
  unsigned penalty = 0;
  unsigned worked = 0;

  worker %= chromo.width;
  for (unsigned week = 0; week < chromo.length; week++)
    for (unsigned day = 0; day < WEEK; day++)
      if (is_working (chromo.gene[day], worker))
	worked++;
  penalty = (unsigned) (abs (worked - WL * WEEK)) * WLP;

  return penalty;
}				/* -----  end of function punish_work_load  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  punish_halving_weekends
 *  Description:  It is undesirable to work only one day in the weekend.  
 * =====================================================================================
 */
unsigned
punish_halving_weekends (Chromosome chromo, unsigned worker)
{
  unsigned penalty = 0;
  worker %= chromo.width;

  for (unsigned day = SATURDAY; day + 1 < chromo.length; day += WEEK)
    penalty +=
      (is_working (chromo.gene[day], worker) ^
       is_working (chromo.gene[day + 1], worker)) * HWP;

  return penalty;
}				/* -----  end of function punish_halving_weekends  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  punish_consecutive_weekends
 *  Description:  It is very bad to work two weekends together. 
 * =====================================================================================
 */
unsigned
punish_consecutive_weekends (Chromosome chromo, unsigned worker)
{
  unsigned penalty = 0;
  worker %= chromo.width;

  for (int day = SATURDAY + WEEK; day + 1 < chromo.length; day += WEEK)
    if ((is_working (chromo.gene[day], worker)
	 || is_working (chromo.gene[day + 1], worker))
	&& (is_working (chromo.gene[day - WEEK], worker)
	    || is_working (chromo.gene[day + 1 - WEEK], worker)))
      penalty += BCW;

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
  unsigned penalty = 0;
  for (int day = 0; day < chromo.length; day++)
    {
      unsigned pw = people_working (chromo.gene[day]);
      unsigned staff = SN;
      if (day % WEEK == SATURDAY || day % WEEK == SATURDAY + 1)
	staff = SNW;
      if (pw < staff)
	penalty += FP;
      if (pw > staff)
	penalty += EP;
    }
  return penalty;
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
  unsigned *freedays = (unsigned *) malloc (chromo.width * sizeof (unsigned));
  unsigned min = 0xFFFFFFFF;


  for (int worker = 0; worker < chromo.width; worker++)
    freedays[worker] = 0;

  for (int day = 0; day < chromo.length; day++)
    for (int worker = 0; worker < chromo.width; worker++)
      if (!is_working (chromo.gene[day], worker))
	freedays[worker]++;

  for (int worker = 0; worker < chromo.width; worker++)
    if (min > freedays[worker])

      for (int worker = 0; worker < chromo.width; worker++)
	penalty += (freedays[worker] - min) * BFD;

  free (freedays);
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
  unsigned *weekends = (unsigned *) malloc (chromo.width * sizeof (unsigned));
  unsigned min = 0;


  for (int worker = 0; worker < chromo.width; worker++)
    weekends[worker] = 0;

  for (int day = SATURDAY; day + 1 < chromo.length; day += WEEK)
    for (int worker = 0; worker < chromo.width; worker++)
      {
	if (!is_working (chromo.gene[day], worker))
	  weekends[worker]++;
	if (!is_working (chromo.gene[day + 1], worker))
	  weekends[worker]++;
      }

  for (int worker = 0; worker < chromo.width; worker++)
    if (min > weekends[worker])
      min = weekends[worker];

  for (int worker = 0; worker < chromo.width; worker++)
    penalty += BW * (weekends[worker] - min);


  free (weekends);
  return penalty;
}				/* -----  end of function punish_different_weekend_number  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  check_aptitude
 *  Description:  Sum penalties up
 * =====================================================================================
 */
void
check_aptitude (Population population)
{
  for (int i = 0; i < population.length; i++)
    {
      population.person[i].penalty_sum =
	punish_bad_staff_number (population.person[i]);
      population.person[i].penalty_sum +=
	punish_different_weekend_number (population.person[i]);
      population.person[i].penalty_sum +=
	punish_different_free_days (population.person[i]);
      for (int w = 0; w < population.person[i].width; w++)
	{
	  population.person[i].penalty_sum +=
	    punish_long_shifts (population.person[i], w);
	  population.person[i].penalty_sum +=
	    punish_halving_weekends (population.person[i], w);
	  population.person[i].penalty_sum +=
	    punish_consecutive_weekends (population.person[i], w);
	  population.person[i].penalty_sum +=
	    punish_work_load (population.person[i], w);

	}
    }
}				/* -----  end of function check_long_shifts  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  analyze_aptitude
 *  Description:  Annotate penalties
 * =====================================================================================
 */
void
analyze_aptitude (Chromosome * chromo)
{
  chromo->penalty_sum =
    chromo->summary.bad_staff_number = punish_bad_staff_number (*chromo);
  chromo->penalty_sum +=
    chromo->summary.different_weekend_number =
    punish_different_weekend_number (*chromo);
  chromo->penalty_sum +=
    chromo->summary.different_free_days =
    punish_different_free_days (*chromo);
  for (int w = 0; w < chromo->width; w++)
    {
      chromo->penalty_sum +=
	chromo->summary.long_shifts[w] = punish_long_shifts (*chromo, w);
      chromo->penalty_sum +=
	chromo->summary.weekends_halved[w] =
	punish_halving_weekends (*chromo, w);
      chromo->penalty_sum +=
	chromo->summary.consecutive_weekends[w] =
	punish_consecutive_weekends (*chromo, w);
      chromo->summary.weekends[w] = get_total_weekends (*chromo, w);
      chromo->summary.freedays[w] = get_total_freedays (*chromo, w);


    }
}
