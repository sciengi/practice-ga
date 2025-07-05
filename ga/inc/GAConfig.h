#ifndef GACONFIG_H
#define GACONFIG_H

#include <stddef.h>

struct GAConfig {

    size_t entity_count;

    int out_cost;
    int overlay_cost;
    int row_cost;

    float crossover_probability;
    float mutation_probability;
};

#endif
