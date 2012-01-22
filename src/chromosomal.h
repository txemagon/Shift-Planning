/*
 * =====================================================================================
 *
 *       Filename:  chromosomal.h
 *
 *    Description:  Chromosome.cpp headers
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

#ifndef CHROMOSOME_H
#define CHROMOSOME_H

void show_chromosome (Chromosome chromosome);
Population create_initial_population (unsigned workers, unsigned period,
				      unsigned max_population);
void deallocate_pop (Population population);
void sort_by_penalty (Population population);
void copy (Population population, unsigned dest, unsigned src);
void fix_staff (Population population);
void cross (Chromosome chromo1, Chromosome chromo2);
void display_penalties (Chromosome chromo);
void display_summaries (Chromosome chromo);

#endif
