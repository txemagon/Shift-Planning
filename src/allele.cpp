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

#include "manage_time.h"
#include "ag.h"
#include "gene.h"

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
get_first_free_wknd_since ( Chromosome chromo, unsigned worker, unsigned since_day )
{
   int day = 0;
   if (since_day >= chromo.length){
      return -1;
   }

   for (unsigned test = since_day; test<chromo.length; test+=WEEK)
      if (!is_working_the_weekend(chromo, worker, test / WEEK) )
	 return the_next_wekend_as_of(test, chromo.length);
   return -1;
}		/* -----  end of function get_first_free_wknd_since  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rotate_gene
 *  Description:  rotates the bits inside a gen an arbitrary number of positions to the
 *                left. 
 * =====================================================================================
 */
Chromosome
random_rotate_gene (Chromosome chromo, unsigned position)
{
  unsigned places = rand () % chromo.width;

  position %= chromo.length;
  chromo.gene[position] =
    rotate_gene (chromo.gene[position], places, chromo.width);

  return chromo;
}				/* -----  end of function rotate_gene  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mutation_gene
 *  Description:  Mutates a chromomosome several (one or more) times. 
 * =====================================================================================
 */
Chromosome
mutation_gene (Chromosome chromo, unsigned position)
{
  /* Probability of mutating <index of the array> times */
  unsigned mutation_times[] = { 60, 80, 90, 95, 100 };
  unsigned probability = rand () % 100;

  position %= chromo.length;
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

  return chromo;
}				/* -----  end of function mutation_gene  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  random_wknd_gene
 *  Description:  Generates a new weekend.
 * =====================================================================================
 */
   Chromosome
random_wknd_gene (Chromosome chromo, unsigned position )
{
   unsigned day = the_next_wekend_as_of(position, chromo.width); 
   if (day > -1 && day + 1 < chromo.width){
      unsigned random_shift = random_gene (chromo.width, SNW); 
      chromo.gene[position] = random_shift;
      chromo.gene[position + 1] = random_shift;
   }
   return chromo;
}		/* -----  end of function random_wknd_gene  ----- */
