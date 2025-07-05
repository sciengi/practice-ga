#ifndef ENTITYPOOL_H
#define ENTITYPOOL_H

#include <Entity.h>
#include <GAConfig.h>
#include <estimating.h>


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


    size_t get_population(population_t& population, bool with_child); 
};



void print_memory_report(const Entity& e, const GAConfig& ga_config);



#endif
