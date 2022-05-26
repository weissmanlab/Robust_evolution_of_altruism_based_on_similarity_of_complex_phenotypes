#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_statistics_double.h>
#include "cooperation.h"

void new_generation(const gsl_rng *r, int time)
{

/*Now we repopulate for the next generation, weighted by the success in the previous games.*/

	double fitness_weights[POPULATION];

        for (int i = 0; i < POPULATION; i++)
        {

                fitness_weights[i] = 1 + (results[i])*SELECTION_STRENGTH;

        }

        double shadow_phenotypes[POPULATION][PHENOTYPE_DIMENSION];

	double shadow_hardcoded_thresholds[POPULATION];

        for (int i = 0; i < POPULATION; i++)
        {

                for (int j = 0; j < PHENOTYPE_DIMENSION; j++)
                {

                        shadow_phenotypes[i][j] = phenotypes[i][j];

                }

		shadow_hardcoded_thresholds[i] = hard_coded_thresholds[i];

        }

/*Use the multinomial draw from GSL */

        int K = POPULATION; /* # "categories" */
        unsigned int n[POPULATION]; /*array to return results of draw */

        gsl_ran_multinomial(r, K, POPULATION, fitness_weights, n);

        int counter = 0;

/*Here we fill the array with all the individuals listed in n[] */

        for (int i = 0; i < POPULATION; i++)
        {

                for(int j = 0; j < n[i]; j++)
                {

                        for (int k = 0; k < PHENOTYPE_DIMENSION; k++)
                        {
                                phenotypes[j+counter][k] = shadow_phenotypes[i][k];
                        }

			hard_coded_thresholds[j+counter] = shadow_hardcoded_thresholds[i];


                }

                counter += n[i];

        }

/*add a phenotype mutation */

        for (int i = 0; i < POPULATION; i++)
        {

                for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
                {

                        phenotypes[i][j] += gsl_ran_gaussian(r, PHENOTYPE_MUTATION);

                }

        }



/* If time is greater than the burn in, we allow strategy mutation. This is the "invasion" aspect.*/


        if (time > BURN_IN)
        {
                for (int i = 0; i < POPULATION; i++)
                {

                        if(gsl_ran_bernoulli(r, STRATEGY_MUTATION) == 1)
                        {

				if(BINARY == 1)
				{

					if(hard_coded_thresholds[i] == STRICT_THRESH)
					{
						hard_coded_thresholds[i] = GENEROUS_THRESH;
					} else
					{
						hard_coded_thresholds[i] = STRICT_THRESH;
					}

				} if(BINARY == 0)
				{

					if(MULTIPLICATIVE_KERNEL == true)
					{
                                		hard_coded_thresholds[i] *= gsl_ran_lognormal(r, HARDCODED_ZETA, sqrt(-1*2*HARDCODED_ZETA));

					} else if(ADDITIVE_KERNEL == true)
					{
						double output = 0;
						int found = 0;
					
						while(found == 0)
						{

							output = gsl_ran_gaussian(r, SIGMA);

							if(output > (-1)*hard_coded_thresholds[i])
							{

								found = 1;

							}

						}

						hard_coded_thresholds[i] += output;
					}

				}

                        }

                }
        }

}
