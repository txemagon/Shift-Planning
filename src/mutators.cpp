/*
 * =====================================================================================
 *
 *       Filename:  mutators.cpp
 *
 *    Description:  Manage a list of compound mutators.
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

#include <stdlib.h>

#include "ag.h"
#include "mutation_controller_private.h"

#define		MUT_LEN	5	/* Maximum Length of the compound mutator */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  create_verb
 *  Description:  Initializes a verb
 * =====================================================================================
 */
Verb
create_verb (Verb * verb)
{
  verb->length = 0;
  verb->mutator_word = NULL;

  return *verb;
}				/* -----  end of function create_verb  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  delete_verb
 *  Description:  Frees the memory occupied by a verb
 * =====================================================================================
 */
void
delete_verb (Verb * verb)
{
  verb->length = 0;
  free (verb->mutator_word);
}				/* -----  end of function delete_verb  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  add_mutator
 *  Description: Allocates a new mutator (pointer to a mutator func) inside a verb.  
 * =====================================================================================
 */
void
add_mutator (void (*new_mutator) (Chromosome chromo), Verb * place)
{
  place->mutator_word =
    (void (**)(Chromosome)) realloc (place->mutator_word,
				     sizeof (void (*)(Chromosome)) *
				     (place->length + 1));
  place->mutator_word[place->length] = new_mutator;
  place->length++;
}				/* -----  end of function add_mutator  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  repeated_variations
 *  Description:  Number of variations with repetition 
 * =====================================================================================
 */
long long int
repeated_variations (unsigned elements, unsigned positions)
{
  if (positions <= 0)
    return 1;
  return elements * repeated_variations (elements, positions - 1);
}				/* -----  end of function repeated_variations  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  generate_mutators
 *  Description:  Creates a list of compound mutators
 * =====================================================================================
 */
void
generate_mutators ()
{
  unsigned max_var = MUT_LEN;
  return;
}				/* -----  end of function generate_mutators  ----- */
