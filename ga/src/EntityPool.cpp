#include <EntityPool.h>



EntityPool::EntityPool(const TaskProperties& tp, const GAConfig& ga_config) 
    : m_population_size(ga_config.entity_count), m_pool(2 * ga_config.entity_count, Entity(tp)) { 
    // DEV: m_pool size ~ ga_config (crossover & mutation operations)

    for (size_t i = 0; i < m_population_size; i++) {
        random_fill(m_pool[i].code);
        mark_entity(m_pool[i], ga_config, tp); 
        m_pool[i].status = Entity::USED;
    }
}



size_t EntityPool::size() const { return m_pool.size(); }



Entity* EntityPool::get_entity(size_t index) { return &m_pool[index]; }


Entity* EntityPool::get_free_entity() {
    for (size_t i = 0; i < m_pool.size(); i++) {
        if (m_pool[i].status == Entity::FREE) return &m_pool[i];
    }

    assert(false && "No free entity in pool!");
    return nullptr;
}


size_t EntityPool::get_population(population_t& population, bool with_child) {
  
    population.clear();
    population.reserve(size());

    size_t i = 0; Entity* e;
    for (size_t j = 0; j < m_pool.size(); j++) {
        e = &m_pool[j];
        if (e->status == Entity::USED && (e->is_active + with_child)) {
            
            population.push_back(&m_pool[j]);
        } 
    }

    return population.size(); 
    // DEV: can check by assert wht is it (active only -> ga_config.entity_count)
}


void print_memory_report(const Entity& e, const GAConfig& ga_config) {
 
    long mem_entity = sizeof(Entity) + e.code.capacity() + e.decode.capacity();
    long mem_population = mem_entity * ga_config.entity_count;

    std::cout 
        << "MEMORY REPORT:"
        << "\n---------------"
        << "\nentity: " << mem_entity << " bytes"
        << "\npopulation(" << ga_config.entity_count << "): " << mem_population << " bytes"
        << std::endl;
}


