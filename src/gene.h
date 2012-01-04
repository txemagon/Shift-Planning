/*
 * =====================================================================================
 *
 *       Filename:  gene.h
 *
 *    Description:  gene.cpp header file
 *
 *        Version:  1.0
 *        Created:  30/12/11 22:16:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José M. González (txemagon / imasen), txema.gonz@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  GENE_H
#define  GENE_H

#define	BITS_BYTE	8	/* Number of bits per byte (defined for verbosity) */

unsigned random_gene (unsigned workers, unsigned ones);
unsigned rotate_gene (unsigned gene, int places, unsigned width);
unsigned is_working (unsigned gene, unsigned worker);
unsigned people_working (unsigned gene);

#endif /* ----- #ifndef GENE_H  ----- */
