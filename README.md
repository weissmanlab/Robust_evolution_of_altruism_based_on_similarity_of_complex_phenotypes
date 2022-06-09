# Robust_evolution_of_altruism_based_on_similarity_of_complex_phenotypes
Installations needed to compile and run the organized code: GNU Scientific Library, make. Recommended for parameter sweeps: GNU Parallel.

How to compile code: Download all .c files, cooperation.h, and the Makefile. Some parameters are decided before compiling the code, others are accepted during execution by the command line. Pre-compiled parameters are listed at the top of the header file, and include number of generations, population size, phenotype dimension, and type of simulation (evolvable thresholds vs two fixed thresholds). Post-compilation parameters include selection strength, benefit, cost, phenotype mutation, strategy mutation rate, initial threshold or strict/generous thresholds, and iteration number. These parameters are input in sim_main.c. Once the pre-compiled parameters have been selected, use the command "make" to compile the code. The code will then be run with the command ./execute_sim followed by the desired values for the post-compiled parameter values, such as this: ./execute_sim 1 1 0.001 10 1 .5 2.5 1, to run the code with selection strength = 1, phenotype mutation = 1, strategy mutation rate = 0.001, benefit = 10, cost = 1, strict threshold = .5, generous threshold = 2.5, and iteration = 1.

How to parameter sweep with GNU Parallel:

Open sweep_vals subdirectory. Enter desired values in respectively named .txt files separated by new lines, for example in INITIAL_THRESH_vals.txt, say I want to test the initial condition of thresholds 1, 4, 6.7, 3, the contents of the txt file would look like this:  
1<br />
4<br />
6.7<br />
3<br />

Exceptions are the ITERATION_vals.txt file and words.txt. In ITERATION, if you want just one copy, leave it as a single line with the entry 1. If you want 3 copies, then the values 1, 2, 3, each separated by a new line. words.txt contains the command to execute execute_sim, so should just be one line containing ./execute_sim.

Now create a txt file of all permutations of the values you'd like to test (named all_permutations.txt) with a command like this:

parallel -a words.txt -a SELECTION_STRENGTH_vals.txt -a PHENOTYPE_MUTATION_vals.txt -a STRATEGY_MUTATION_vals.txt -a BENEFIT_vals.txt -a COST_vals.txt -a STRICT_THRESH_vals.txt -a GENEROUS_THRESH_vals.txt -a ITERATION_vals.txt echo > all_permutations.txt

This is an example for running a simulation with two fixed thresholds, the generous and the strict thresholds, as seen in Figure 6 in the manuscript.

Make sure execute_sim and all_permutations.txt are in the same folder, then have parallel run the cooperation code with all desired parameter combinations using command "parallel < all_permutations.txt"
