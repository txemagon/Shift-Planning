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

#include <vector>
#include "manage_time.h"

#define		PAUSE	8	/* Pause after writing status */

#define		SW	5	/* Recommended shift week */
#define		SN	4	/* Recommended staff number */
#define		SNW	3	/* Recommended staff number on weekends */
#define		WL	375	/* Recommended work load in tenths of an hour/week  */
#define		SL	70	/* Given shift length in tenths of an hour  */

#define		SWEP	4	/* Punish extra day with <SWP> penalty points */
#define		SWDP	1	/* Punish minus day with <SWP> penalty points */
#define		HWP	5	/* Punish halving a weekend with <HWP> penalty points */
#define		BW	2	/* Punish weekend injustice with <BW> penalty points */
#define		BCW	4	/* Punish consecutive weekends injustice with <BCW> penalty points */
#define		BFD	1	/* Punish different number of free days with <BFD> penalty points */
#define		FP	3	/* Punish few people with <FP> penalty points */
#define		EP	4	/* Punish extra people with <EP> penalty points */
#define		WLP	1	/* Punish differences in work rate  */

/* Command line default options */
#define	WORKERS 	5	/* Number of workers to plan  */
#define	PERIOD		(5 * WEEK)	/* Number of days to plan  */
#define	POPULATION	500	/* Number of solucion in each generation */

#define	CROSS_RATE	10	/* Cross probability = 1 / CROSS_RATE */
#define	GENERATIONS	9994484	/* Number of generations to compute */
#define	MUT_LEN 	2	/* Maximum Length of the compound mutator */

#define	MAX_KEY		0x100	/* Maximum length of yaml keys  */

using namespace std;

struct TimeInterval
{
  unsigned day_start;		/* Both days included */
  unsigned day_end;
    TimeInterval (unsigned start, unsigned end)
  {
    day_start = start;
    day_end = end;
  }
};				/* ----------  end of struct TimeInterval  ---------- */
typedef struct TimeInterval TimeInterval;

struct ConfigIntVar
{
  const char name[MAX_KEY];
  unsigned value;
};
typedef struct ConfigIntVar ConfigIntVar;

enum Problem_idx
{ workers_idx, period_idx, shift_length_idx };

enum Goals_idx
{ shift_week_idx, staff_number_ix, staff_weekend_number_idx, work_load_idx };

enum Penalty_points_idx
{
  shift_week_extra_penalty_idx,
  shift_week_defect_penalty_idx,
  halving_weekend_penalty_idx,
  weekend_injustice_penalty_idx,
  consecutive_weekend_injustice_idx,
  bad_free_days_penalty_idx,
  few_people_penalty_idx,
  extra_people_penalty_idx,
  work_load_penalty_idx
};

enum Inner_working_idx
{ generations_idx, population_idx, cross_rate_idx, mutation_length_idx };

enum TWorking
{ working, off_work };

struct Summary
{
  unsigned extra_staff_number;
  unsigned defect_staff_number;
  unsigned long *extra_shifts;	/* Separate penalties by worker */
  unsigned *weekends_halved;
  unsigned *consecutive_weekends;
  unsigned *weekends;		/* Amount of free weekends */
  unsigned *freedays;
    vector < TimeInterval > *wrong_shifts;
  unsigned mutations;
};				/* ----------  end of struct Penalty  ---------- */
typedef struct Summary Summary;

struct Penalty
{
  unsigned bad_staff_number;
  unsigned different_weekend_number;
  unsigned different_free_days;
  unsigned weekends_halved;
  unsigned long_shifts;
  unsigned consecutive_weekends;
  unsigned work_load;
};
typedef struct Penalty Penalty;

struct Chromosome
{
  unsigned *gene;		/* Genetic chain (one possible solution) */
  unsigned width;		/* Number of workers */
  unsigned length;		/* Period (number of days) computed */
  unsigned penalty_sum;		/* Keeping the sum of penalties improves the computation speed  */
  Summary summary;		/* Report of the chromosome */
  Penalty penalty;		/* Offenses against the adaptation rules */
};				/* ----------  end of struct Chromosome  ---------- */
typedef struct Chromosome Chromosome;

struct Population
{
  struct Chromosome *person;
  unsigned length;
};				/* ----------  end of struct Population  ---------- */
typedef struct Population Population;

#endif
