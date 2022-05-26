#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cooperation.h"

void interaction()
{

/*store individuals to be interacted with */

int store[POPULATION][SURVEY];

	for (int i = 0 ; i < POPULATION; i++)
        {

		for (int j = 0; j < SURVEY; j++)
		{

			int rand_individual = rand()%POPULATION;

			store[i][j] = rand_individual;
		}	

                results[i] = 0;
        }

/* update acting threshes */

	double acting_thresholds[POPULATION];

	for (int i = 0; i < POPULATION; i++)
	{

		acting_thresholds[i] = hard_coded_thresholds[i];
	}

	total_donations = 0;
	
	for (int i = 0; i < POPULATION; i++)
        {

		int individual_donations = 0;

                for (int j = 0; j < SURVEY; j++)
                {

			double distance_to_survey = 0;

			for (int k = 0; k < PHENOTYPE_DIMENSION; k++)
			{

				distance_to_survey += (phenotypes[i][k] - phenotypes[store[i][j]][k])*(phenotypes[i][k] - phenotypes[store[i][j]][k]);

			}

			distance_to_survey = sqrt(distance_to_survey);

			if(distance_to_survey < acting_thresholds[i])
			{

				results[store[i][j]] += BENEFIT;
			
				individual_donations += 1;
				total_donations += 1;

			} else
			{

				results[i] += COST;
			
			}

		}
	
		donation_events[i] = individual_donations;

	}

}

