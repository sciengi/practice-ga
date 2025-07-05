#ifndef GAO_H
#define GAO_H

#include <random>
#include <chrono>

#include <Field.h>
#include <Entity.h>
#include <GAConfig.h>
#include <EntityPool.h>



void discrete_mutate(Entity& e, float percent_of_gens);



void discrete_crossover(const Entity& p1, const Entity& p2, Entity& c1, Entity& c2, float probability_of_first = 0.5);



void panmixia(const EntityPool::population_t& all, std::vector<size_t>& parents);



void truncate_and_choose(size_t threshold, const EntityPool::population_t& all, std::vector<Entity*>& choosen);



#endif
