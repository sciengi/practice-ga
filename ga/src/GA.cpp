#include <GA.h>



long count_outer_cells(const Field& field, const TaskProperties& tp) {

    long count = 0;
    for (size_t i = 0; i < field.height(true); i++) {
        for (size_t j = 0; j < field.width(true); j++) {
            if (field.is_acceptable_area(j, i)) continue;
            count += field.get(i).get(j);
        }
    }

    return count;
}


long count_overlay_figures(const Field& field, const TaskProperties& tp) {

    long count = 0; int value;
    for (size_t i = 0; i < field.height(true); i++) {
        for (size_t j = 0; j < field.width(true); j++) {
             value = field.get(i).get(j);
             if (value > 1) count += value;
        }
    }

    return count;
}


long count_used_rows(const Field& field, const TaskProperties& tp) {

    long count = 0;
    for (size_t i = 0; i < field.height(true); i++) {
        for (size_t j = 0; j < field.width(true); j++) {
            if (field.get(i).get(j) != 0) {
                count += 1;
                break;
            }
        }
    }

    return count;
}


Entity::Entity(const TaskProperties& tp) : is_marked(false), in_population(false) { code = get_coded_solution(tp); }



void print_entity_mark(const Entity& e) {
    std::cout 
        << "ENTITY MARK:" 
        << "\ntotal             = " << e.total 
        << "\n----------------------"
        << "\nfitness           = " << e.fitness
        << "\npenalty (out)     = " << e.penalty_out
        << "\npenalty (overlay) = " << e.penalty_overlay
        << "\n" << std::endl;
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


void mark_entity(Entity& e, const GAConfig& ga_config, const TaskProperties& tp) {
   
    // prepare
    e.decode = decode_solution(e.code, tp);

    Field field(tp);
    build_solution(e.decode, field);

    #ifdef DEBUG
    std::cout << "BUILD:" << '\n' << field << std::endl;
    #endif


    e.penalty_out     = ga_config.out_cost     * count_outer_cells(field, tp);
    e.penalty_overlay = ga_config.overlay_cost * count_overlay_figures(field, tp);
    
    e.fitness         = ga_config.row_cost     * count_used_rows(field, tp);

    e.total = e.fitness + e.penalty_out + e.penalty_overlay;

    e.is_marked = true;
}


void discrete_mutate(Entity& e, float percent_of_gens) {

    float probabily_for_percent = percent_of_gens * e.code.size() / (e.code.size() + 1);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::bernoulli_distribution distribution(probabily_for_percent);

    for (size_t i = 0; i < e.code.size(); i++) {
        if (distribution(generator)) e.code[i] = !e.code[i];
    }
}


void discrete_crossover(const Entity& p1, const Entity& p2, Entity& c1, Entity& c2, float probability_of_first) {

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::bernoulli_distribution distribution(probability_of_first);

    for (size_t i = 0; i < p1.code.size(); i++) {
        if (distribution(generator)) {
            c1.code[i] = p1.code[i];
            c2.code[i] = p2.code[i];
        } else {
            c1.code[i] = p2.code[i];
            c1.code[i] = p1.code[i];
        } 
    }
}


float calculate_HD(const Entity& e1, const Entity& e2) {

    int hit_count = 0; // DEV: hit/miss count available
    for (size_t i = 0; i < e1.code.size(); i++) {
        if (e1.code[i] == e2.code[i]) hit_count++;
    }

    return hit_count / (float) e1.code.size();
}


EntityPool::EntityPool(const TaskProperties& tp, const GAConfig& ga_config) 
    : m_population_size(ga_config.entity_count), m_pool(2 * ga_config.entity_count, Entity(tp)) { 
    // DEV: m_pool size ~ ga_config (crossover & mutation operations)

    for (size_t i = 0; i < m_population_size; i++) {
        random_fill(m_pool[i].code);
        mark_entity(m_pool[i], ga_config, tp); 
        m_pool[i].in_population = true;
    }
}


size_t EntityPool::size() const { return m_pool.size(); }


Entity* EntityPool::get_entity(size_t index) { return &m_pool[index]; }


Entity* EntityPool::get_free_entity() {
    for (size_t i = 0; i < m_pool.size(); i++) {
        if (!m_pool[i].in_population) return &m_pool[i];
    }

    assert(false && "No free entity in pool!");
    return nullptr;
}


void EntityPool::get_population(population_t& population) {
    
    assert(population.size() <= m_population_size);
    
    population.resize(m_population_size);

    size_t i = 0;
    for (size_t j = 0; j < m_pool.size(); j++) {
        if (m_pool[j].in_population) population[i++] = &m_pool[j]; 
    }
}

void EntityPool::get_all(population_t& all) {
    
    all.resize(m_pool.size());
    for (size_t j = 0; j < m_pool.size(); j++) all[j] = &m_pool[j];
}
