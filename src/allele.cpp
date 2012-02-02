/*
 * =====================================================================================
 *
 *       Filename:  allele.cpp
 *
 *    Description:  Allele level operation.
 *
 *        Version:  1.0
 *        Created:  02/01/12 15:54:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José M. González (txemagon / imasen), txema.gonz@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <vector>

#include "manage_time.h"
#include "ag.h"
#include "globals.h"
#include "gene.h"

using namespace std;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_shift_base
 *  Description:  Returns base for extra shift period encodings
 * =====================================================================================
 */
unsigned
get_shift_base (Chromosome chromo)
{
  unsigned base = 1;

  while (base < chromo.length - goals[shift_week_idx].value)
    base <<= 1;

  return base;
}				/* -----  end of function get_shift_base  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_consweekend_base
 *  Description:  Returns the base for consecutive weekend encoding
 * =====================================================================================
 */
unsigned
get_consweekend_base (Chromosome chromo)
{
  unsigned base = 1;
  unsigned mww = chromo.length / WEEK + 1;

  while (base < mww)
    base <<= 1;

  return base;
}				/* -----  end of function get_consweekend_base  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_total_freedays
 *  Description:  counts the amount of free days of a given worker. 
 * =====================================================================================
 */
unsigned
get_total_freedays (Chromosome chromo, unsigned worker)
{
  unsigned total = 0;

  worker %= chromo.width;

  for (unsigned day = 0; day < chromo.length; day++)
    if (!(chromo.gene[day] & (1 << worker)))
      total++;
  return total;
}				/* -----  end of function get_total_freedays  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_total_weekends
 *  Description:  counts the amount of complete free weekends of a given worker. 
 * =====================================================================================
 */
unsigned
get_total_weekends (Chromosome chromo, unsigned worker)
{
  unsigned total = 0;

  worker %= chromo.width;

  for (unsigned day = SATURDAY; day + 1 < chromo.length; day += WEEK)
    if (!(chromo.gene[day] & (1 << worker))
	&& !(chromo.gene[day + 1] & (1 << worker)))
      total++;
  return total;
}				/* -----  end of function get_total_freedays  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_bad_shifts
 *  Description:  Writes a list of the long shifts
 * =====================================================================================
 */
vector < TimeInterval > &get_bad_shifts (Chromosome chromo, unsigned worker,
					 vector < TimeInterval > &list)
{
  unsigned together = 0;
  bool interval = false;

  for (unsigned day = 0; day < chromo.length; day++)
    if (is_working (chromo.gene[day], worker))
      {
	together++;
	if (together == goals[shift_week_idx].value + 1)
	  interval = true;
      }
    else
      {
	if (interval)
	  {
	    list.push_back (TimeInterval (day - together, day - 1));
	    interval = false;
	  }
	together = 0;
      }
  unsigned day = chromo.length - 1;
  if (interval)
    {
      list.push_back (TimeInterval (day - together, day - 1));
      interval = false;
    }


  return list;
}				/* -----  end of function get_bad_shifts  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  is_working_the_weekend
 *  Description:  Returns true if the employee is working the given week number
 * =====================================================================================
 */
bool
is_working_the_weekend (Chromosome chromo, unsigned worker, unsigned week)
{

  int test_day = week * WEEK + SATURDAY;
  return is_working (chromo.gene[test_day], worker)
    || is_working (chromo.gene[test_day + 1], worker);
}				/* -----  end of function is_working_the_weekend  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_first_free_wknd_since
 *  Description:  returns the first day of a free weekend for a given employee. 
 *                -1 if not found. When the search starts on SUNDAY this weekend
 *                is discarded.
 * =====================================================================================
 */
int
get_first_free_wknd_since (Chromosome chromo, unsigned worker,
			   unsigned since_day)
{
  int day = 0;
  if (since_day >= chromo.length)
    {
      return -1;
    }

  for (unsigned test = since_day; test < chromo.length; test += WEEK)
    if (!is_working_the_weekend (chromo, worker, test / WEEK))
      return the_next_wekend_as_of (test, chromo.length);
  return -1;
}				/* -----  end of function get_first_free_wknd_since  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_one_normal_day
 *  Description:  Returns a non weekend day.
 * =====================================================================================
 */
unsigned
get_one_normal_day (unsigned length)
{
  unsigned day;

  do
    {
      day = rand () % length;
    }
  while (day % WEEK >= SATURDAY);

  return day;
}				/* -----  end of function get_one_normal_day  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rotate_gene
 *  Description:  rotates the bits inside a gen an arbitrary number of positions to the
 *                left. 
 * =====================================================================================
 */
void
random_rotate_gene (Chromosome chromo)
{
  unsigned places = rand () % chromo.width;
  unsigned position = get_one_normal_day (chromo.length);

  chromo.gene[position] =
    rotate_gene (chromo.gene[position], places, chromo.width);

}				/* -----  end of function rotate_gene  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mutation_gene
 *  Description:  Mutates a chromomosome several (one or more) times. 
 * =====================================================================================
 */
void
mutation_gene (Chromosome chromo)
{
  /* Probability of mutating <index of the array> times */
  unsigned mutation_times[] = { 60, 80, 90, 95, 100 };
  unsigned probability = rand () % 100;
  unsigned position = get_one_normal_day (chromo.length);

  chromo.gene[position] =
    random_gene (chromo.width, goals[staff_number_ix].value);
  /*
     if (position % WEEK == SATURDAY + 1)
     position--;
     if (position % WEEK == SATURDAY)
     {
     unsigned random_shift = random_gene (chromo.width, SNW);
     chromo.gene[position] = random_shift;
     chromo.gene[position + 1] = random_shift;
     }
     else
     chromo.gene[position] = random_gene (chromo.width, SN);
   */
}				/* -----  end of function mutation_gene  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  random_wknd_gene
 *  Description:  Generates a new weekend.
 * =====================================================================================
 */
void
random_wknd_gene (Chromosome chromo)
{
  unsigned position = rand () % chromo.length;
  unsigned day = the_next_wekend_as_of (position, chromo.width);
  if (day > -1 && day + 1 < chromo.width)
    {
      unsigned random_shift =
	random_gene (chromo.width, goals[staff_weekend_number_idx].value);
      chromo.gene[position] = random_shift;
      chromo.gene[position + 1] = random_shift;
    }
}				/* -----  end of function random_wknd_gene  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  random_shift
 *  Description:  Changes the shift from one person to another.
 * =====================================================================================
 */
void
random_shift (Chromosome chromo)
{
  unsigned start;
  unsigned length;
  int rotations = rand () % (2 * chromo.width);
  start = rand () % (chromo.length / WEEK);
  start *= WEEK;
  // length = rand () % (chromo.length - start);
  length = WEEK - 2;

  for (unsigned g = start; g < start + length; g++)
    chromo.gene[g] = rotate_gene (chromo.gene[g], rotations, chromo.width);

}				/* -----  end of function cross  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  interchain
 *  Description:  Changes one whole week inside a single solution (chromosome)
 * =====================================================================================
 */
void
interchain (Chromosome chromo)
{
  unsigned start1, start2;
  unsigned worker1, worker2;
  unsigned buffer = 0;
  unsigned mask1 = 0, mask2 = 0;

  start1 = rand () % chromo.length / WEEK;
  start2 = rand () % chromo.length / WEEK;
  start1 *= WEEK;
  start2 *= WEEK;
  worker1 = rand () % chromo.width;
  worker2 = rand () % chromo.width;

  mask1 = 1 << worker1;
  mask2 = 1 << worker2;

  for (unsigned day = 0; day < WEEK - 2; day++)
    {
      buffer <<= 1;
      buffer |= ! !(chromo.gene[start1 + day] & mask1);
      chromo.gene[start1 + day] &= (~mask1);
      chromo.gene[start1 + day] |=
	(! !(mask2 & chromo.gene[start2 + day]) << worker1);
      chromo.gene[start2 + day] &= 0xFFFFFFFF ^ (1 << worker2);
      chromo.gene[start2 + day] |= (buffer & 1) << worker2;
    }

}				/* -----  end of function interchain  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  bubble
 *  Description:  Exchanges two genes 
 * =====================================================================================
 */
void
bubble_gene (Chromosome chromo)
{
  unsigned pos1 = get_one_normal_day (chromo.length);
  unsigned pos2 = get_one_normal_day (chromo.length);

  unsigned buffer = chromo.gene[pos1];
  chromo.gene[pos1] = chromo.gene[pos2];
  chromo.gene[pos2] = buffer;
}				/* -----  end of function bubble  ----- */
