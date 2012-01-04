/*
 * =====================================================================================
 *
 *       Filename:  ag.h
 *
 *    Description:  General AG structures.
 *
 *        Version:  1.0
 *        Created:  31/12/11 00:17:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José M. González (txemagon / imasen), txema.gonz@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  AG_H
#define  AG_H


#define		PAUSE	8	/* Pause after writing status */

#define		SW	5	/* Recommended shift week */
#define		SN	4	/* Recommended staff number */
#define		SNW	3	/* Recommended staff number on weekends */
#define		WL	37.5	/* Recommended work load in hours/week  */

#define		SWP	1	/* Punish extra day with <SWP> penalty points */
#define		HWP	3	/* Punish halving a weekend with <HWP> penalty points */
#define		BW	3	/* Punish weekend injustice with <BW> penalty points */
#define		BCW	2	/* Punish consecutive weekends injustice with <BCW> penalty points */
#define		BFD	1	/* Punish different number of free days with <BFD> penalty points */
#define		FP	25	/* Punish few people with <FP> penalty points */
#define		EP	20	/* Punish extra people with <EP> penalty points */
#define		WLP	1	/* Punish differences in work rate  */


#define		SATURDAY	5	/* Monday = 0 */
#define		WEEK    	7	/* Number of days in a week */


struct Summary
{
  unsigned bad_staff_number;
  unsigned different_weekend_number;
  unsigned different_free_days;
  unsigned *long_shifts;	/* Separate penalties by worker */
  unsigned *weekends_halved;
  unsigned *consecutive_weekends;
  unsigned *weekends;
  unsigned *freedays;
};				/* ----------  end of struct Penalty  ---------- */
typedef struct Summary Summary;

struct Chromosome
{
  unsigned *gene;		/* Genetic chain (one possible solution) */
  unsigned width;		/* Number of workers */
  unsigned length;		/* Period (number of days) computed */
  unsigned penalty_sum;		/* Keeping the sum of penalties improves the computation speed  */
  Summary summary;		/* Offenses against the adaptation rules */
};				/* ----------  end of struct Chromosome  ---------- */
typedef struct Chromosome Chromosome;

struct Population
{
  struct Chromosome *person;
  unsigned length;
};				/* ----------  end of struct Population  ---------- */
typedef struct Population Population;

#endif
