#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <gsl/gsl_statistics_double.h>
#include "cooperation.h"

/*youll need step size and iterator and time */

void append_files(int time, int current_iteration, FILE **stat_output, FILE **fitness_output, FILE **parameter_output)
{

	if(time == 0)
	{

/*Creating the files to output to. */

	        if(stat_on == true)
	        {

	                char stat_template[256];

			char parameter_template[256];

			if(BINARY == 1)
			{

				snprintf(stat_template, 256, "STATISTICS_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_STR%.5f_GEN%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, GENERATIONS, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, STRICT_THRESH, GENEROUS_THRESH, current_iteration);

				snprintf(parameter_template, 256, "Parameters_STATISTICS_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_STR%.5f_GEN%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, GENERATIONS, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, STRICT_THRESH, GENEROUS_THRESH, current_iteration);			
	
			} else if(BINARY == 0)
			{

				snprintf(stat_template, 256, "STATISTICS_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_InitialThresh%.5f_additivesigma%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, GENERATIONS, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_HARDCODED_THRESH, SIGMA, current_iteration);

				snprintf(parameter_template, 256, "Parameters_STATISTICS_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_InitialThresh%.5f_additivesigma%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, GENERATIONS, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_HARDCODED_THRESH, SIGMA, current_iteration);

			}

			*stat_output = fopen(stat_template, "w");

			fprintf(*stat_output, "time, num_GENEROUS, num_STRICTs, avg_hc_thresh, var_hc_thresh, DONATIONS");

			*parameter_output = fopen(parameter_template, "w");

			fprintf(*parameter_output, "SELECTION STRENGTH %f\nSURVEY %d\nBURN IN TIME %d\nSTEP SIZE %d\nLOG TIME %d\nINITIAL HARDCODED THRESH %f\nINITIAL PHENOTYPE %f\nSCATTER HARDCODED THRESH PERCENTAGE %f\nSCATTER PHENOTYPE PERCENTAGE %f\nSIMULATION SEED %lu", SELECTION_STRENGTH, SURVEY, BURN_IN, STEP_SIZE, LOGTIME, INITIAL_HARDCODED_THRESH, INITIAL_PHENOTYPE, SCATTER_HARDCODED_THRESH, SCATTER_PHENOTYPE, simulationseed);

	        }

		if(fitnesses_on == true)
                {

                        char fitness_template[256];

                        if(BINARY == 1)
                        {

                                snprintf(fitness_template, 256, "FITNESSES_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_STR%.5f_GEN%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, GENERATIONS, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, STRICT_THRESH, GENEROUS_THRESH, current_iteration);


                        } if(BINARY == 0)
                        {

                                snprintf(fitness_template, 256, "FITNESSES_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_HZ%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, GENERATIONS, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, HARDCODED_ZETA, current_iteration);

                        }

                        *fitness_output = fopen(fitness_template, "w");

                }

		for(int i = 0; i < PHENOTYPE_DIMENSION; i++)
                {

                        if(stat_on == true)
                        {

                                fprintf(*stat_output, ", PHENOTYPE_MEAN_%d, PHENOTYPE_VAR_%d", (i+1), (i+1));

                        }

                        if(fitnesses_on == true)
                        {
                                fprintf(*fitness_output, "PHENOTYPE_%d, ", i+1);
                        }

                }

		if(fitnesses_on == true)
                {       
                        
                        fprintf(*fitness_output, "TIME, INDIVIDUAL, DISTANCE_TO_MEAN, HARDCODED_THRESH, SCORE, DONATIONS");
                
			fprintf(*fitness_output, "\n");

                }

                if(stat_on == true)
                {
                        fprintf(*stat_output, "\n");
                }


	}else if(time < GENERATIONS)
	{
		if(fitnesses_on == true && time%STEP_SIZE == 0)
               	{

			double phenotype_mean[PHENOTYPE_DIMENSION];

			for (int i = 0; i < PHENOTYPE_DIMENSION; i++)
			{

				for (int j = 0; j < POPULATION; j++)
				{

					phenotype_mean[i] += phenotypes[j][i];

				}

				 phenotype_mean[i] = phenotype_mean[i]/((double) POPULATION);

			}

                        for (int i = 0; i < POPULATION; i++)
                        {

				double distance = 0;

                                for (int j = 0; j < PHENOTYPE_DIMENSION; j++)
                                {

                                        distance += (phenotypes[i][j] - phenotype_mean[j])*(phenotypes[i][j] - phenotype_mean[j]);

                                        fprintf(*fitness_output, "%.3f, ", phenotypes[i][j]);

                                }

                                distance = sqrt(distance);

                                fprintf(*fitness_output, "%d, %d, %.5f, %.5f, %.15f, %d", time, i, distance, hard_coded_thresholds[i], results[i], donation_events[i]);

				fprintf(*fitness_output, "\n");

                        }


                }

		if(stat_on == true && time%STEP_SIZE == 0)
                {

			int num_stricts = 0;
			int num_generous = 0;			

			if(BINARY == 1)
			{

                                for (int i = 0; i < POPULATION; i++)
                                {

                                        if(hard_coded_thresholds[i] == STRICT_THRESH)
                                        {
                                                num_stricts++;
                                        } else
                                        {
                                                num_generous++;
                                        }

                                };


			}

			double hard_mean = gsl_stats_mean(hard_coded_thresholds, 1, POPULATION);
			double hard_var = gsl_stats_variance(hard_coded_thresholds, 1, POPULATION);

			fprintf(*stat_output, "%d, %d, %d, %f, %f, %d", time, num_generous, num_stricts, hard_mean, hard_var, total_donations);

			for(int i = 0; i < PHENOTYPE_DIMENSION; i++)
			{

				double phenotype_dimension[POPULATION];

				for (int j = 0; j < POPULATION; j++)
				{
					phenotype_dimension[j] = phenotypes[j][i];
				}

				fprintf(*stat_output, ", %f, %f", gsl_stats_mean(phenotype_dimension, 1, POPULATION), gsl_stats_variance(phenotype_dimension, 1, POPULATION));
			}

			fprintf(*stat_output, "\n");
		}


	}else
	{

        	if(stat_on == true)
        	{
        	        fclose(*stat_output);
        	}

        	if(fitnesses_on == true)
        	{
        	        fclose(*fitness_output);
        	}
	}

}





