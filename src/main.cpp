/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Computes a shift planning using AG.
 *
 *        Version:  1.0
 *        Created:  30/12/11 12:45:09
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author: José M. González (txemagon / imasen), txema.gonz@gmail.com 
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include "ag.h"
#include "chromosomal.h"
#include "gene.h"
#include "allele.h"
#include "screen.h"

/* Command line default options */
#define	WORKERS 	5	/* Number of workers to plan  */
#define	PERIOD		(5 * WEEK)	/* Number of days to plan  */
#define	POPULATION	600	/* Number of solucion in each generation */
#define	GENERATIONS	49999	/* Number of generations to compute */

Population population;
bool exit_request = false;

void
ex_program (int sig)
{
  printf ("Aborted by user !!! - Catched signal: %d ... !!\n", sig);
  exit_request = true;
  (void) signal (SIGINT, SIG_DFL);
}

    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  main
     *  Description:  Main controller 
     * =====================================================================================
     */
int
main (int argc, char *argv[])
{
  unsigned period = PERIOD;
  unsigned workers = WORKERS;
  unsigned max_population = POPULATION;
  unsigned max_times, max_rotations, max_mutations;

  (void) signal (SIGINT, ex_program);
  srand (time (0));

  population = create_initial_population (workers, period, max_population);

  for (int g = 0; g < GENERATIONS && !exit_request; g++)
    {
      max_times = rand () % POPULATION / 4;
      max_mutations = rand () % 4;
      for (unsigned times = 0; times < max_times; times++)
	for (unsigned mutations = 0; mutations < max_mutations; mutations++)
	  {
	    unsigned s = 1 + rand () % (population.length - 1);
	    mutation_gene (population.person[s],
			   rand () % population.person[s].length);
	  }

      // Mutate thru rotation 
      max_times = rand () % POPULATION / 4;
      max_rotations = rand () % 2;
      for (unsigned times = 0; times < max_times; times++)
	for (unsigned rotations = 0; rotations < max_rotations; rotations++)
	  {
	    unsigned s = 1 + rand () % (population.length - 1);
	    random_rotate_gene (population.person[s],
				rand () % population.person[s].length);
	  }


      max_times = rand () % POPULATION / 4;
      for (unsigned times = 0; times < max_times; times++)
	{
	  random_shift (population.
			person[1 + rand () % (population.length - 1)]);
	  cross (population.person[1 + rand () % (population.length - 1)],
		 population.person[1 + rand () % (population.length - 1)]);
	  interchain (population.
		      person[1 + rand () % (population.length - 1)]);
	}


      fix_staff (population);
      check_aptitude (population);
      sort_by_penalty (population);


      // Procreate 
      unsigned position = 1;
      for (unsigned b = 0; b < 20; b++)
	while (position < population.length - 20)
	  {
	    for (unsigned times = 0; times < 20 - b; times++, position++)
	      copy (population, 20 + position, b);
	  }

      printf
	("\rComputing generation %5u. Best penalty rate %u.                                       \r",
	 g, population.person[0].penalty_sum);
      // fflush (stdout); // Flushing is very time consuming.
    }


  analyze_aptitude (&(population.person[0]));
  show_chromosome (population.person[0]);
  display_penalties (population.person[0]);
  deallocate_pop (population);

  return EXIT_SUCCESS;
}

/* ----------  end of function main  ---------- */
