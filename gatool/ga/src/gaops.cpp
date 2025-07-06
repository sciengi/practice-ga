#include <gaops.h>



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


void panmixia(const EntityPool::population_t& population, std::vector<size_t>& parents) {

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 generator(seed);
    std::uniform_int_distribution<size_t> choose_from_population(0, parents.size() - 1);
    
    for (size_t i = 0; i < parents.size(); i++) {
        size_t partner_index = choose_from_population(generator);
        parents[i] = partner_index;
    }
}



void truncate_and_choose(size_t threshold, const EntityPool::population_t& all, std::vector<Entity*>& choosen) {

    EntityPool::population_t sorted = all;
    std::sort(sorted.begin(), sorted.end(), [](const Entity* left, const Entity* right) { return left->total < right->total; });

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 generator(seed);
    std::uniform_int_distribution<size_t> choose_from_selection(0, threshold);

    size_t index;
    for (size_t i = 0; i < choosen.size(); i++) {
        index = choose_from_selection(generator);
        sorted[index]->status = Entity::RESERVED;
        choosen[i] = sorted[index]; 
    }
}

