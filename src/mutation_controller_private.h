/*
 * =====================================================================================
 *
 *       Filename:  mutation_controller_private.h
 *
 *    Description:  Private part of the mutation controller
 *
 *        Version:  1.0
 *        Created:  16/01/12 15:06:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José M. González (txemagon / imasen), txema.gonz@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  MUTATION_CONTROLLER_PRIVATE_H
#define  MUTATION_CONTROLLER_PRIVATE_H


#define		MUT_NB	5	/* Number of mutators  */

#include "chromosomal.h"
#include "allele.h"

void (*mutator[]) (Chromosome chromo) =
{
  mutation_gene,
    // random_wknd_gene,
random_rotate_gene, random_shift, interchain, bubble_gene};

struct Verb
{
  int length;
  void (**mutator_word) (Chromosome chromo);
};

typedef struct Verb Verb;


#endif /* ----- #ifndef MUTATION_CONTROLLER_PRIVATE_H  ----- */
