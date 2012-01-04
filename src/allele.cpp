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

#include "ag.h"
#include "gene.h"

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
  unsigned times = 0;

  position %= chromo.length;
  if (position % WEEK == SATURDAY + 1)
    position--;
  if (position % WEEK == SATURDAY)
    {
      chromo.gene[position] = random_gene (chromo.width, SNW);
      chromo.gene[position + 1] = random_gene (chromo.width, SNW);
    }
  else
    chromo.gene[position] = random_gene (chromo.width, SN);

  return chromo;
}				/* -----  end of function mutation_gene  ----- */


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
