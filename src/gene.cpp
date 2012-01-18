/*
 * =====================================================================================
 *
 *       Filename:  gene.cpp
 *
 *    Description:  Gene depth manipulation routines. Ensure an srand call somewhere 
 *                  in the main function.
 *
 *        Version:  1.0
 *        Created:  30/12/11 22:15:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José M. González (txemagon / imasen), txema.gonz@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "ag.h"
#include "globals.h"
#include "gene.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  is_working
 *  Description:  returns 0 if is not working, 1 otherwise.
 * =====================================================================================
 */
unsigned
is_working (unsigned gene, unsigned worker)
{
  return ! !(gene & (1 << worker));
}				/* -----  end of function is_working  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  people_working
 *  Description:  returns the number of people working in a given day.
 * =====================================================================================
 */
unsigned
people_working (unsigned gene)
{
  unsigned pw = 0;
  while (gene > 0)
    {
      pw += gene % 2;
      gene >>= 1;
    }
  return pw;
}				/* -----  end of function people_working  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rotate_gene
 *  Description:  rotates a gene to the left for positive values of the places param. 
 * =====================================================================================
 */
unsigned
rotate_gene (unsigned gene, int places, unsigned width)
{
  unsigned rotated = gene;
  unsigned place = (unsigned) places;
  unsigned times = place / width + 1;

  place = place % width;
  for (unsigned time = 0; time < times; time++)
    if (places > 0)
      {
	rotated = gene << place;
	rotated &= (1 << width) - 1;
	rotated |= (gene >> (width - place));
      }
    else
      {
	rotated = gene >> place;
	rotated &= (1 << width) - 1;
	rotated |= (gene << (width - place));
      }

  return rotated & ((1 << width) - 1);
}				/* -----  end of function rotate_gene  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  random_gene
 *  Description:  Creates a random gene, where each if its <WORKERS> bits represents 
 *                wheather an employee is really working or not. Each gene is a day 
 *                of work.
 * =====================================================================================
 */
unsigned
random_gene (unsigned workers, unsigned ones)
{
  static const char max_workers = sizeof (unsigned) * BITS_BYTE;
  unsigned gene = 0;
  unsigned added = 0;

  if (workers > max_workers)
    {				/* Safety checks */
      workers = max_workers;
      warn ("Number of workers changed to %i.", workers);
    }

  while (added < ones)
    {
      if (gene % 2 == 0)
	{
	  gene += 1;
	  added++;
	}

      unsigned places = rand () % workers;
      gene = rotate_gene (gene, places, workers);
    }

  return gene;
}				/* -----  end of function random_gene  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  set_worker
 *  Description:  Sets the value for a given worker. 
 * =====================================================================================
 */
unsigned
set_worker (unsigned gene, unsigned worker, enum TWorking value)
{
  if (value == working)
    gene |= (1 << worker);
  else
    gene &= ~(1 << worker);

  return gene;
}				/* -----  end of function set_worker  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  xchg_workers
 *  Description:  exchange the value of two workers inside a gene.
 * =====================================================================================
 */
unsigned
xchg_workers (unsigned gene, unsigned worker1, unsigned worker2)
{
  bool is_working1 = is_working (gene, worker1);
  gene =
    set_worker (gene, worker1, (enum TWorking) is_working (gene, worker2));
  gene = set_worker (gene, worker2, (enum TWorking) is_working1);

  return gene;
}				/* -----  end of function xchg_workers  ----- */
