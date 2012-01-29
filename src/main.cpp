/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Computes a shift planning using AG.
 *
 *        Version:  1.0
 *        Created:  30/12/11 12:45:09
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author: José M. González (txemagon / imasen), txema.gonz@gmail.com 
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <yaml.h>

#include "ag.h"
#include "filenames.h"
#include "manage_time.h"
#include "chromosomal.h"
#include "screen.h"
#include "mutation_controller.h"

/* Characteristics of what to achieve (input data) */
ConfigIntVar problem[] = {
  {"workers", WORKERS}
  ,
  {"period", PERIOD}
  ,
  {"shift_length", SL}
};

/* Properties of the solution (what to achieve) */
ConfigIntVar goals[] = {
  {"shift_week", SW}
  ,
  {"staff_number", SN}
  ,
  {"staff_weekend_number", SNW}
  ,
  {"work_load", WL}
};

ConfigIntVar penalty_points[] = {
  {"shift_week_extra_penalty", SWEP}
  ,
  {"shift_week_defect_penalty", SWDP}
  ,
  {"halving_weekend_penalty", HWP}
  ,
  {"weekend_injustice_penalty", BW}
  ,
  {"consecutive_weekend_injustice", BCW}
  ,
  {"bad_free_days_penalty", BFD}
  ,
  {"few_people_penalty", FP}
  ,
  {"extra_people_penalty", EP}
  ,
  {"work_load_penalty", WLP}
};

ConfigIntVar inner_working[] = {
  {"generations", GENERATIONS}
  ,
  {"population", POPULATION}
  ,
  {"cross_rate", CROSS_RATE}
  ,
  {"mutation_length", MUT_LEN}

};


Population population;
bool exit_request = false;

void
ex_program (int sig)
{
  printf ("\nAborted by user !!! - Catched signal: %d ... !!\n", sig);
  exit_request = true;
  (void) signal (SIGINT, SIG_DFL);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  set_global
 *  Description:  Sets a global (user defined variable). Returns NULL if 
 *               global not found.
 * =====================================================================================
 */
bool
set_global (const char *key, const char *value)
{
  bool found = false;

  for (unsigned i = 0; i < sizeof (problem) / sizeof (ConfigIntVar); i++)
    if (!strcmp (problem[i].name, key))
      {
	problem[i].value = atoi (value);
	found = true;
      }
  for (unsigned i = 0; i < sizeof (goals) / sizeof (ConfigIntVar); i++)
    if (!strcmp (goals[i].name, key))
      {
	goals[i].value = atoi (value);
	found = true;
      }
  for (unsigned i = 0; i < sizeof (penalty_points) / sizeof (ConfigIntVar);
       i++)
    if (!strcmp (penalty_points[i].name, key))
      {
	penalty_points[i].value = atoi (value);
	found = true;
      }
  for (unsigned i = 0; i < sizeof (inner_working) / sizeof (ConfigIntVar);
       i++)
    if (!strcmp (inner_working[i].name, key))
      {
	inner_working[i].value = atoi (value);
	found = true;
      }
  return found;
}				/* -----  end of function find_global_for  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  load_config_file
 *  Description:  Overrides default values with the ones provided in a defautl file.
 * =====================================================================================
 */
void
load_config_file ()
{
  FILE *config;
  yaml_parser_t parser;
  yaml_event_t event;
  char key[MAX_KEY], value[MAX_KEY];
  bool key_type = true;

  if (config = fopen (CONFIG_FILE, "rb"))
    {
      if (!yaml_parser_initialize (&parser))
	{
	  perror
	    ("Couldn't initialize yaml parser. Proceeding with default options.\n");
	  return;
	}

      yaml_parser_set_input_file (&parser, config);
      do
	{
	  yaml_parser_parse (&parser, &event);
	  switch (event.type)
	    {
	    case YAML_NO_EVENT:
	      puts ("No event!");
	      break;
	      /*  Stream start/end */
	    case YAML_STREAM_START_EVENT:
	      break;
	    case YAML_STREAM_END_EVENT:
	      break;
	      /*  Block delimeters */
	    case YAML_DOCUMENT_START_EVENT:	/* puts("<b>Start Document</b>"); */
	      break;
	    case YAML_DOCUMENT_END_EVENT:	/* puts("<b>End Document</b>");  */
	      break;
	    case YAML_SEQUENCE_START_EVENT:	/* puts("<b>Start Sequence</b>"); */
	      break;
	    case YAML_SEQUENCE_END_EVENT:	/* puts("<b>End Sequence</b>");  */
	      break;
	    case YAML_MAPPING_START_EVENT:	/* puts("<b>Start Mapping</b>"); */
	      break;
	    case YAML_MAPPING_END_EVENT:	/* puts("<b>End Mapping</b>");   */
	      break;
	      /*  Data */
	    case YAML_ALIAS_EVENT:	/* printf("Got alias (anchor %s)\n", event.data.alias.anchor); */
	      break;
	    case YAML_SCALAR_EVENT:
	      if (key_type)
		{
		  strcpy (key, (const char *) event.data.scalar.value);
		  key_type = false;
		}
	      else
		{
		  strcpy (value, (const char *) event.data.scalar.value);
		  if (!set_global (key, value))
		    fprintf (stderr, "\nInvalid user option %s found.\n",
			     key);
		  key_type = true;
		}
	      break;
	    }
	  if (event.type != YAML_STREAM_END_EVENT)
	    yaml_event_delete (&event);
	}
      while (event.type != YAML_STREAM_END_EVENT);

      yaml_event_delete (&event);
      yaml_parser_delete (&parser);
      fclose (config);
    }
}				/* -----  end of function load_config_file  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Main function 
 * =====================================================================================
 */
int
main (int argc, char *argv[])
{
  unsigned period;
  unsigned workers;
  unsigned max_population;

  (void) signal (SIGINT, ex_program);
  srand (time (0));

  load_config_file ();
  period = problem[period_idx].value;
  workers = problem[workers_idx].value;
  max_population = inner_working[population_idx].value;

  population = create_initial_population (workers, period, max_population);

  for (int g = 0; g < inner_working[generations_idx].value && !exit_request;
       g++)
    printf ("\rComputing generation %5u. Best penalty rate %u.      \r", g,
	    mutate_generation (population));

  analyze_aptitude (&(population.person[0]));
  show_chromosome (population.person[0]);
  display_summaries (population.person[0]);
  display_penalties (population.person[0]);
  deallocate_pop (population);

  return EXIT_SUCCESS;
}

/* ----------  end of function main  ---------- */
