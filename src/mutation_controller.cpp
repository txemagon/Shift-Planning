/*
 * =====================================================================================
 *
 *       Filename:  mutation_controller.cpp
 *
 *    Description:  Handles the way mutations are made.
 *
 *        Version:  1.0
 *        Created:  15/01/12 16:48:17
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
#include "globals.h"
#include "mutation_controller_private.h"
#include "manage_time.h"
#include "chromosomal.h"
#include "gene.h"
#include "allele.h"
#include "screen.h"
#include "mutators.h"


void (*mutator[]) (Chromosome chromo) =
{				// random_wknd_gene,
NULL,
    mutation_gene, random_rotate_gene, random_shift, interchain, bubble_gene, lower_shift_stress, exch_weekend };

unsigned mutator_elements = sizeof (mutator) / sizeof (void (*)(Chromosome));

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mutate_chromo
 *  Description:  Performs random mutations inside a chromosome. 
 * =====================================================================================
 */
void
mutate_chromo (Chromosome * chromo)
{
  mutator_exec (random_verb (), *chromo);
  chromo->summary.mutations++;
}				/* -----  end of function mutate_chromo  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mutate_generation
 *  Description:  Generates a whole generation and returns the best score rate.  
 * =====================================================================================
 */
unsigned
mutate_generation (Population population)
{

  int chromos = population.length;
  for (int i = 1; i < chromos; i++)
    mutate_chromo (&population.person[i]);

  fix_staff (population);
  check_aptitude (population);
  sort_by_penalty (population);


  // Procreate 
  unsigned position = 21;
  for (unsigned b = 0; b < 20; b++)
    while (position < population.length)
      {
	for (unsigned times = 0;
	     times < 20 - b && position < population.length;
	     times++, position++)
	  {
	    copy (population, position, b);
	    if (inner_working[cross_rate_idx].value
		&& rand () % inner_working[cross_rate_idx].value == 1)
	      cross (population.person[position],
		     population.person[1 + rand () % (position - 1)]);
	  }
      }


  return population.person[0].penalty_sum;
}				/* -----  end of function mutate_generation  ----- */
