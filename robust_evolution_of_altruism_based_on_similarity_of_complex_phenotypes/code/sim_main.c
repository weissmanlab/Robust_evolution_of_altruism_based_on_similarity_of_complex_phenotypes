#include "cooperation.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_statistics_double.h>

//#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

int main(int argc, char *argv[])
{
/*set up for random variable */

	const gsl_rng_type *T;
	gsl_rng * r;
	gsl_rng_env_setup();

	int current_id = getpid();

        simulationseed = gsl_rng_default_seed;

	gsl_rng_default_seed = time(NULL) + current_id;
	gsl_rng_default = gsl_rng_mt19937;
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

//	srand(time(NULL));

	simulationseed = gsl_rng_default_seed;

	SURVEY = 10;

/* here make an iterating function to print repeats of the same simulation. but for now... */
	

		SELECTION_STRENGTH = atof(argv[1]);
		PHENOTYPE_MUTATION = atof(argv[2]);
		STRATEGY_MUTATION = atof(argv[3]);
		BENEFIT = atof(argv[4]);
		COST = atof(argv[5]);

		if(BINARY == 1)
		{
			STRICT_THRESH = atof(argv[6]);
			GENEROUS_THRESH = atof(argv[7]);
			ITERATION = atoi(argv[8]);
		} else if(BINARY == 0)
		{
			INITIAL_HARDCODED_THRESH = atof(argv[6]);
			ITERATION = atoi(argv[7]);
		}

	
	initialize();

	int current_iteration = ITERATION;	

	double sample_times[(int) (log((GENERATIONS - 1000))/log(10)/0.01)];

	if(LOGTIME == 1)
	{

		for(int i = 0; i < (int) (log((GENERATIONS - 1000))/log(10)/0.01); i++)
		{
			sample_times[i] = pow(10, i*0.01);

		}
	} else
	{
		for(int i = 0; i < (int) (log((GENERATIONS - 1000))/log(10)/0.01); i++)
		{
			sample_times[i] = -1;
		}
	}

	int time_index = 0;
	
	int preferred_time = sample_times[time_index];
	
	for (int time = 0; time <= GENERATIONS; time++)
	{

		if(LOGTIME == 1)
		{
			if(time > preferred_time && preferred_time != (int) sample_times[ (int) (log((GENERATIONS - 1000))/log(10)/0.01) - 1])
			{
				while(time > preferred_time)
				{
					time_index++;
					preferred_time = sample_times[time_index];
				}
			}
		
			if(time == 0 || time == (int) preferred_time){
		
				append_files(time, current_iteration, &stat_output, &fitness_output, &parameter_output);
		
				if( preferred_time != (int) sample_times[ (int) (log((GENERATIONS - 1000))/log(10)/0.01) - 1])
				{
					time_index++;
				}
				preferred_time = sample_times[time_index];
			};
		} else
		{
			append_files(time, current_iteration, &stat_output, &fitness_output, &parameter_output);
		}

		interaction();
		
		new_generation(r, time);


	}

	gsl_rng_free (r);

}




