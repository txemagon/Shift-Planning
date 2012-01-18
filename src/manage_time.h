/*
 * =====================================================================================
 *
 *       Filename:  manage_time.h
 *
 *    Description:  manage_time header file
 *
 *        Version:  1.0
 *        Created:  15/01/12 17:05:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José M. González (txemagon / imasen), txema.gonz@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  MANAGE_TIME_H
#define  MANAGE_TIME_H

#define		MONDAY  	0
#define		TUESDAY  	1
#define		WEDNESDAY	2
#define		THURSDAY	3
#define		FRIDAY  	4
#define		SATURDAY	5
#define		SUNDAY  	6
#define		WEEK    	7	/* Number of days in a week */

enum WeekDay
{ monday, tuesday, wednesday, thursday, friday, saturday, sunday };

extern enum WeekDay first_day_of_week;

int get_day (int day_of_week);
int the_next_wekend_as_of (unsigned day, unsigned max);

#endif   /* ----- #ifndef MANAGE_TIME_H  ----- */
