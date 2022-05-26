/*Set the initial phenotype and strategy distribution */
#include <stdlib.h>
#include "cooperation.h"

void initialize()
{
        
	for(int i = 0; i < POPULATION; i++)
        {

                for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
                {

                        phenotypes[i][j] = INITIAL_PHENOTYPE;
                }

		if(BINARY == 1)
		{              

			if(rand()%2 == 0)
			{     
				hard_coded_thresholds[i] = STRICT_THRESH;
			} else
			{       
				hard_coded_thresholds[i] = GENEROUS_THRESH;
			}
		} if(BINARY == 0)
		{
	                
	                hard_coded_thresholds[i] = INITIAL_HARDCODED_THRESH;

			if(SCATTER_HARDCODED_THRESH != 0)
			{
				if(i < POPULATION/2)
				{
					hard_coded_thresholds[i] *= (1 + (double)rand()/RAND_MAX*SCATTER_HARDCODED_THRESH);
				} else
				{
					hard_coded_thresholds[i] *= (1 - (double)rand()/RAND_MAX*SCATTER_HARDCODED_THRESH);
				}
			}
		}

	}

}
