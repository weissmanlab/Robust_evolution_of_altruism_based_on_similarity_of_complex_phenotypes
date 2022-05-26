#include <stdio.h>
#include <gsl/gsl_rng.h>

/*Header file for the cooperation code! */

/* SET IMMUTABLE PARAMETERS */

#define BINARY 0 /* compete two strategies? 0 no, 1 yes */
#define POPULATION 1000 /* population size */
#define GENERATIONS 8000 /* for how many generations the simulation runs, this value should be greater than 1000 */
#define PHENOTYPE_DIMENSION 3 /* number of continuous phenotypes */

/* multiplicative kernel vs. additive.*/

#define MULTIPLICATIVE_KERNEL false
#define ADDITIVE_KERNEL true

#define SIGMA .1 /* This is the scale of the noncooperation (the cut off on the additive kernel) */

double SELECTION_STRENGTH;
double PHENOTYPE_MUTATION;
double STRATEGY_MUTATION;
double BENEFIT;
double COST;
int ITERATION;
double STRICT_THRESH;
double GENEROUS_THRESH;
double HARDCODED_ZETA;
double STRICT_PERCENT;
double GENEROUS_PERCENT;
int SURVEY;

/* SET INITIAL CONDITIONS */

unsigned long int simulationseed;

//BURN_IN: time before printing values occurs
#define BURN_IN 10

#define STEP_SIZE 10 /* Leave at 1 to print every time step */

//LOGTIME: would you like printing to be on the log scale? Then set LOGTIME = 1, otherwise set to 0.
#define LOGTIME 0 /*Set STEP_SIZE = 1 if LOGTIME = 1. */

//Set initial conditions
double INITIAL_HARDCODED_THRESH;
#define INITIAL_PHENOTYPE 300.0

//Introduce random variation around selected IC (i.e. 0.5 around 300 => IC values scattered from 150 to 450)
#define SCATTER_HARDCODED_THRESH 0.0
#define SCATTER_PHENOTYPE 0.5

/* SET OUTPUT FILES */

#define stat_on true /*print summary file (saves a lot of space) */
#define fitnesses_on false /*print details of each individual %step_size time points. */
FILE *stat_output, *fitness_output;

/* add a file just for the parameters */
FILE *parameter_output;

/* the program's most-used arrays */

double phenotypes[POPULATION][PHENOTYPE_DIMENSION];
double payoffs[POPULATION];

double hard_coded_thresholds[POPULATION];

int sibling_donations;
int siblings[POPULATION];

/*quantify helping without mutual games */

int total_donations;
int donation_events[POPULATION];

double results[POPULATION];

/* the program's functions */

void initialize();

void interaction();

void new_generation(const gsl_rng* r, int time);

void append_files(int time, int current_iteration, FILE**, FILE**, FILE**);

