#include <StatManager.h>



StatManager::StatManager(const GAConfig& gc) 
    : HD_distribution(gc.entity_count), entities_origin(gc.entity_count) {

    result_file.open("result.csv", std::fstream::out);

    if (!result_file.is_open()) throw std::runtime_error("Error: cant open a file to write `result.csv`");
    // TODO: dynamical name generation (time, TaskProperties(figure count) and GAConfig(entities count))
}


StatManager::~StatManager() { result_file.close(); }


const Entity* StatManager::get_best() const { return best; }


void StatManager::fetch_stat(size_t current_step, const EntityPool::population_t& population) {

    best = *std::min_element(population.begin(), population.end(), 
            [](const Entity* left, const Entity* right){ return left->total < right->total; });

    long best_total = best->total;
    long best_penalty = best_total - best->fitness;

    double mean_total = 0;
    double mean_penalty = 0;

    for(auto ep : population) {
        mean_total += ep->total;
        mean_penalty += ep->total - ep->fitness;
    }

    mean_total /= population.size();
    mean_penalty /= population.size();


    for (size_t i = 0; i < HD_distribution.size(); i++) // TODO(DEV): move to Entity?
        HD_distribution[i] = calculate_HD(*best, *population[i]);

    for (size_t i = 0; i < entities_origin.size(); i++)
        entities_origin[i] = population[i]->origin_number;


    result_file 
        << current_step << ' '
        << best_total << ' '
        << mean_total << ' '
        << best_penalty << ' '
        << mean_penalty << ' ';
    
    for (auto& v : HD_distribution) 
        result_file << v << ' ';

    for (auto& v : entities_origin) 
        result_file << v << ' ';

    result_file << std::endl;
}

