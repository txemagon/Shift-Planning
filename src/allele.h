/*
 * =====================================================================================
 *
 *       Filename:  allele.h
 *
 *    Description:  allele.cpp headers
 *
 *        Version:  1.0
 *        Created:  02/01/12 15:56:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José M. González (txemagon / imasen), txema.gonz@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  ALLELE_H
#define  ALLELE_H


Chromosome mutation_gene (Chromosome chromo, unsigned position);
Chromosome random_rotate_gene (Chromosome chromo, unsigned position);
unsigned get_total_weekends (Chromosome chromo, unsigned worker);
unsigned get_total_freedays (Chromosome chromo, unsigned worker);
bool is_working_the_weekend (Chromosome chromo, unsigned worker,
			     unsigned week);
int get_first_free_wknd_since ( Chromosome chromo, unsigned worker, unsigned since_day );
Chromosome random_wknd_gene (Chromosome chromo, unsigned position );

#endif /* ----- #ifndef ALLELE_H  ----- */
