#ifndef GACONFIG_H
#define GACONFIG_H

#include <stddef.h>

struct GAConfig {

    size_t entity_count;

    float out_cost;
    float overlay_cost;
    float distance_cost;
    float row_cost;

    float crossover_probability;

    float mutation_probability;        
    float percent_of_mutated_gens;

    size_t selection_threshold; 
};

#endif
