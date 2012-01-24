/*
 * =====================================================================================
 *
 *       Filename:  chromosomal.h
 *
 *    Description: Code for creating and erasing a population of chromosomes.  
 *
 *        Version:  1.0
 *        Created:  30/12/11 17:42:17
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
#include <math.h>
#include <unistd.h>

#include "ag.h"
#include "globals.h"
#include "manage_time.h"
#include "gene.h"
#include "allele.h"
#include "chromosomal.h"

#define		MAX_TRANSLATION		0x100	/* String for displaying numerical info */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  show_chromosome
 *  Description:  Displays a chromosome.
 * =====================================================================================
 */
void
show_chromosome (Chromosome chromosome)
{
  const char *named_day = "LMXJVSD";
  char *shift = (char *) malloc (chromosome.width * sizeof (char) + 1);

  printf ("workers 1 -> %u\n", chromosome.width);
  for (unsigned day = 0; day < chromosome.length; day++)
    {
      printf ("%c - %3u: ", named_day[day % WEEK], day + 1);
      printf ("%s",
	      inspect_gene (shift, chromosome.gene[day], chromosome.width));
      printf ("\n");
    }
  free (shift);
}				/* -----  end of function show_chromosome  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  display_summaries()
 *  Description:  
 * =====================================================================================
 */
void
display_summaries (Chromosome chromo)
{
  char translation[MAX_TRANSLATION];

  printf ("\nSUMMARY\n");
  printf ("=======\n\n");
  printf ("Extra staff: %u\n", chromo.summary.extra_staff_number);
  printf ("Defect staff: %u\n", chromo.summary.defect_staff_number);

  for (unsigned worker = 0; worker < chromo.width; worker++)
    {
      printf ("\nWorker %2u\n", worker + 1);
      printf ("---------\n");
      printf ("Extra shifts: %u\n%s\n", chromo.summary.extra_shifts[worker],
	      display_amounts (translation,
			       chromo.summary.extra_shifts[worker],
			       get_shift_base (chromo)));
      printf ("Weekends halved: %u\n",
	      chromo.summary.weekends_halved[worker]);
      printf ("Free weekends: %u\n", chromo.summary.weekends[worker]);
      printf ("Free days: %u\n", chromo.summary.freedays[worker]);
    }

}				/* -----  end of function display_summaries()  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  display_penalties
 *  Description:  Displays the different summary fields.
 * =====================================================================================
 */
void
display_penalties (Chromosome chromo)
{
  unsigned worked;

  printf ("\nTOTALS\n");
  printf ("======\n");
  printf ("Bad staff number: %u\n", chromo.penalty.bad_staff_number);
  printf ("Different weekend number: %u\n",
	  chromo.penalty.different_weekend_number);
  printf ("Different free days: %u\n", chromo.penalty.different_free_days);
  printf ("Long Shifts: %u\n", chromo.penalty.long_shifts);
  printf ("Weekends Halved: %u\n", chromo.penalty.weekends_halved);
  printf ("Consecutive Weekends: %u\n", chromo.penalty.consecutive_weekends);

}				/* -----  end of function display_penalties  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_weekends
 *  Description:  Initializes the weekends keeping the turn.
 * =====================================================================================
 */
void
init_weekends (Chromosome chromo)
{
  unsigned initial_worker = rand () % chromo.width;
  unsigned people_free =
    problem[workers_idx].value - goals[staff_weekend_number_idx].value;
  unsigned distance;
  unsigned up_worker;
  char *shift = (char *) malloc (chromo.width * sizeof (char) + 1);

  for (unsigned day = SATURDAY, position = initial_worker;
       day < chromo.length; day += WEEK, position += people_free)
    {
      position %= chromo.width;
      for (unsigned worker = 0; worker < chromo.width; worker++)
	{
	  up_worker = worker;
	  if (worker < position)
	    up_worker += chromo.width;
	  distance = up_worker - position;
	  chromo.gene[day] = set_worker (chromo.gene[day], worker,
					 distance <
					 people_free ? off_work : working);
	  if (day + 1 < chromo.length)
	    chromo.gene[day + 1] = set_worker (chromo.gene[day + 1], worker,
					       distance <
					       people_free ? off_work :
					       working);

	}
    }
  free (shift);

}				/* -----  end of function init_weekends  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_chromosome
 *  Description:  Makes a chromosome putting all the genes together. Each gene is a day
 *                of work.
 * =====================================================================================
 */
Chromosome
init_chromosome (unsigned workers, unsigned period)
{
  static unsigned n_chromo = 0;
  Chromosome chromo;
  printf ("\r                                                  ");
  printf ("\rGenerating random chromosome: %6u\r", ++n_chromo);
  fflush (stdout);

  chromo.length = period;
  chromo.width = workers;
  chromo.penalty_sum = 0;
  chromo.summary.extra_shifts =
    (unsigned *) malloc (workers * sizeof (unsigned));
  chromo.summary.weekends_halved =
    (unsigned *) malloc (workers * sizeof (unsigned));
  chromo.summary.consecutive_weekends =
    (unsigned *) malloc (workers * sizeof (unsigned));
  chromo.summary.weekends = (unsigned *) malloc (workers * sizeof (unsigned));
  chromo.summary.freedays = (unsigned *) malloc (workers * sizeof (unsigned));
  chromo.gene = (unsigned *) malloc (period * sizeof (unsigned));


  for (int i = 0; i < chromo.length; i++)
    chromo.gene[i] =
      random_gene (chromo.width,
		   (i % WEEK == SATURDAY
		    || i % WEEK == SATURDAY + 1) ?
		   goals[staff_number_ix].value :
		   goals[staff_weekend_number_idx].value);

  init_weekends (chromo);
  return chromo;
}				/* -----  end of function init_chromosome  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  create_initial_population
 *  Description:  Allocates and fills a pool of random chromosomes.
 * =====================================================================================
 */
Population
create_initial_population (unsigned workers, unsigned period,
			   unsigned max_population)
{
  Population population;

  population.length = max_population;
  population.person =
    (Chromosome *) malloc (max_population * sizeof (Chromosome));

  for (unsigned i = 0; i < max_population; i++)	/* Create initial population */
    population.person[i] = init_chromosome (workers, period);

  return population;
}				/* -----  end of function create_initial_population  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  deallocate_pop
 *  Description:  Removes the population from memory.
 * =====================================================================================
 */
void
deallocate_pop (Population population)
{
  for (unsigned i = 0; i < population.length; i++)
    {				/* Remove population */
      free (population.person[i].summary.extra_shifts);
      free (population.person[i].summary.weekends_halved);
      free (population.person[i].summary.consecutive_weekends);
      free (population.person[i].summary.weekends);
      free (population.person[i].summary.freedays);
      free (population.person[i].gene);
    }
  free (population.person);
}				/* -----  end of function deallocate_pop  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sort_by_penalty
 *  Description:  Sorts a population according to its penalty rate (lowest first).
 * =====================================================================================
 */
void
sort_by_penalty (Population population)
{
  Chromosome buffer;
  for (unsigned i = 0; i < population.length - 1; i++)
    for (unsigned c = i + 1; c < population.length; c++)
      if (population.person[i].penalty_sum > population.person[c].penalty_sum)
	{
	  buffer = population.person[i];
	  population.person[i] = population.person[c];
	  population.person[c] = buffer;
	}
}				/* -----  end of function sort_by_penalty  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  copy
 *  Description:  Overwrites the value of chromosome with the value of another.
 * =====================================================================================
 */
void
copy (Population population, unsigned dest, unsigned src)
{
  if (population.person[src].length == population.person[dest].length)
    {
      population.person[dest].width = population.person[src].width;
      population.person[dest].length = population.person[src].length;
      population.person[dest].penalty_sum =
	population.person[src].penalty_sum;
      population.person[dest].summary = population.person[src].summary;
      memcpy (population.person[dest].gene, population.person[src].gene,
	      population.person[src].length);
    }
}				/* -----  end of function copy  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  fix_staff
 *  Description:  Mends the number of people working
 * =====================================================================================
 */
void
fix_staff (Population population)
{
  for (int i = 0; i < population.length; i++)
    for (int j = 0; j < population.person[i].length; j++)
      {
	int expected_staff = goals[staff_number_ix].value;
	if (j % WEEK == SATURDAY || j % WEEK == SATURDAY + 1)
	  expected_staff = goals[staff_weekend_number_idx].value;
	if (expected_staff != people_working (population.person[i].gene[j]))
	  population.person[i].gene[j] =
	    random_gene (population.person[i].width, expected_staff);
      }

}				/* -----  end of function fix_staff  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  cross
 *  Description:  Cross two sections of different chromosomes
 * =====================================================================================
 */
void
cross (Chromosome chromo1, Chromosome chromo2)
{
  unsigned start1, start2;
  unsigned length;
  unsigned *buffer;

  start1 = rand () % chromo1.length / WEEK;
  start2 = rand () % chromo2.length / WEEK;
  start1 *= WEEK;
  start2 *= WEEK;
  length =
    rand () % (unsigned) fmin (chromo1.length - start1,
			       chromo2.length - start2);
  length = WEEK;

  buffer = (unsigned *) malloc (length * sizeof (unsigned));
  memcpy (buffer, &(chromo1.gene[start1]), length);
  memcpy (&(chromo1.gene[start1]), &(chromo2.gene[start2]), length);
  memcpy (&(chromo2.gene[start2]), buffer, length);
  free (buffer);
}				/* -----  end of function cross  ----- */
