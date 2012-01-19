/*
 * =====================================================================================
 *
 *       Filename:  manage_time.cpp
 *
 *    Description:  Handle time matters.
 *
 *        Version:  1.0
 *        Created:  11/01/12 22:25:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José M. González (txemagon / imasen), txema.gonz@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "manage_time.h"

enum WeekDay first_day_of_week = monday;	/* Default value */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_day
 *  Description:  Offsets the week day taking the first_day_of_week into account.
 * =====================================================================================
 */
int
get_day (int day_of_week)
{
  return day_of_week - first_day_of_week;
}				/* -----  end of function get_day  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  the_next_wekend_as_of
 *  Description:  return the number of day of the next saturday. -1 when exceeding time
 *                limits.
 * =====================================================================================
 */
int
the_next_wekend_as_of (unsigned day, unsigned max)
{
  unsigned week = day / week;
  day = week * WEEK + SATURDAY;
  if (day >= max)
    day = -1;
  return day;
}				/* -----  end of function the_next_wekend_as_of  ----- */
