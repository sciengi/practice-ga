#ifndef GA_H
#define GA_H

#include <gaops.h>

class GA {
private:
    TaskProperties m_tp;
    GAConfig m_gc;

    EntityPool m_epool;
    EntityPool::population_t m_population;

    std::vector<size_t> m_parents;
    std::vector<Entity*>   m_next;

public:
    GA(const TaskProperties& tp, const GAConfig& gc);


    const EntityPool::population_t& get_population() const;  


    void evolve_population(size_t current_epoch = 1);
};

#endif
