#ifndef GA_H
#define GA_H

#include <random>
#include <chrono>

#include <TaskProperties.h>
#include <Solution.h>


long count_outer_cells(const Field& field, const TaskProperties& tp);

long count_overlay_figures(const Field& field, const TaskProperties& tp);

long count_used_rows(const Field& field, const TaskProperties& tp);


struct Entity {

    bool is_marked;
    bool in_population; // DEV: move to EntityPool?

    long origin_number;

    long penalty_out;
    long penalty_overlay;

    long fitness;

    long total;
    

    coded_solution code;
    solution       decode;


    Entity(const TaskProperties& tp);
};


struct GAConfig {

    size_t entity_count;

    int out_cost;
    int overlay_cost;
    int row_cost;

    float crossover_probability;
    float mutation_probability;

    // TODO:
    // - make more flexible
};


void print_entity_mark(const Entity& e);

void print_memory_report(const Entity& e, const GAConfig& ga_config);

void mark_entity(Entity& e, const GAConfig& ga_config, const TaskProperties& tp);

void discrete_mutate(Entity& e, float percent_of_gens);

void discrete_crossover(const Entity& p1, const Entity& p2, Entity& c1, Entity& c2, float probability_of_first = 0.5);

float calculate_HD(const Entity& e1, const Entity& e2);


class EntityPool {

private:
    size_t m_population_size;
    std::vector<Entity> m_pool;


public:
    typedef std::vector<Entity*> population_t;


    EntityPool(const TaskProperties& tp, const GAConfig& ga_config);


    size_t size() const;


    Entity* get_entity(size_t index);


    Entity* get_free_entity();


    void get_population(population_t& population);

    void get_all(population_t& all);
};


#endif
