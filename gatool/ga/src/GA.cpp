
#include <GA.h>

 

GA::GA(const TaskProperties& tp, const GAConfig& gc) 
    : m_tp(tp), m_gc(gc),  
      m_epool(tp, gc), m_parents(gc.entity_count), m_next(gc.entity_count) {

    m_epool.get_population(m_population, false);
}


const EntityPool::population_t& GA::get_population() const { return m_population; }


void GA::evolve_population(size_t current_epoch) {
    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);

	std::bernoulli_distribution is_crossover(m_gc.crossover_probability), 
                            	 is_mutation(m_gc.mutation_probability);


    for (size_t i = 0; i < m_population.size(); i++)
           m_population[i]->origin_number = current_epoch; // TODO(DEV): mistake, previous version works properly


    // Choosing parents & Crossover
    panmixia(m_population, m_parents);

    for (size_t i = 0; i < m_gc.entity_count; i++) {
        if (m_parents[i] != i && is_crossover(generator)) {
           
            Entity* c1 = m_epool.get_free_entity();
            Entity* c2 = m_epool.get_free_entity();
            
            discrete_crossover(
                *m_population[i], *m_population[m_parents[i]], *c1, *c2);


            c1->status = Entity::USED; c2->status = Entity::USED;
            c1->is_active = false;     c2->is_active = false;

            c1->is_marked = false; c2->is_marked = false;

            c1->origin_number = current_epoch; c2->origin_number = current_epoch;
        }
    }

    m_epool.get_population(m_population, true); // DEV: current population + childs


    // Mutation 
    for (size_t i = 0; i < m_population.size(); i++) {
        if (is_mutation(generator)) {
            auto ep = m_population[i];
            discrete_mutate(*ep, m_gc.percent_of_mutated_gens);
            ep->origin_number = current_epoch;
            ep->is_marked     = false;
            ep->is_mutated    = true;
        }
    }
    

    // Estimate all population
    for (size_t i = 0; i < m_population.size(); i++) {
        auto ep = m_population[i];
        if (ep->is_marked == false) mark_entity(*ep, m_gc, m_tp);
    }


    // Selection
    truncate_and_choose(m_gc.selection_threshold, m_population, m_next);

    for (size_t i = 0; i < m_population.size(); i++) {
        if (m_population[i]->status == Entity::USED) m_population[i]->status = Entity::FREE;
    }

    for (size_t i = 0; i < m_gc.entity_count; i++) {

        m_next[i]->is_active = true;
        m_next[i]->is_mutated = false;

        if (m_next[i]->status == Entity::RESERVED) {
            m_next[i]->status = Entity::USED;

        } else {
            Entity* dublicate = m_epool.get_free_entity();
            *dublicate = *m_next[i];
        } 
    }

    m_epool.get_population(m_population, false);
}
