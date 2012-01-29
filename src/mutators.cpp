/*
 * =====================================================================================
 *
 *       Filename:  mutators.cpp
 *
 *    Description:  Manages a list of compound mutators.
 *
 *        Version:  1.0
 *        Created:  25/01/12 09:54:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José M. González (imasen, txemagon), txema.gonz@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "ag.h"
#include "globals.h"
#include "mutation_controller_private.h"
#include "chromosomal.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  power
 *  Description:  Recursive power calculation.
 * =====================================================================================
 */
int power(int base, int exponent){
  if (exponent <= 0)
    return 1;
  return base*power(base, exponent-1);
}				/* -----  end of function power  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mutator_exec
 *  Description:  Execute a compund mutator (verb)
 * =====================================================================================
 */
void mutator_exec(unsigned verb, Chromosome chromo){
  int nb_of_mutators = mutator_elements; 
  int base = nb_of_mutators;
  int index;
  
  while(verb>0){
    index = verb % base;
    if (mutator[index])
      mutator[index](chromo);
    verb /= base;
  }

}				/* -----  end of function mutator_exec  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  random_verb
 *  Description:  Create a random verb
 * =====================================================================================
 */
unsigned random_verb(){
  int nb_of_mutators = mutator_elements;
  int base = nb_of_mutators;
  int max = power(base, inner_working[mutation_length_idx].value );

  return rand() % max;
}				/* -----  end of function random_verb  ----- */

