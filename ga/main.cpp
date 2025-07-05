
#include <fstream>
#include <gaops.h>


int main() {

    // Loading data and settings
    TaskProperties tp = get_task();
    
    if(!Figure::types_is_enough(tp)) 
        throw std::runtime_error("Error: Figure types not enough for this task");


    GAConfig ga_config = {
        .entity_count = 50,

        .out_cost = 200,
        .overlay_cost = 100,
        .distance_cost = 0,  // DEV: tricky  
        .row_cost = 10,

        .crossover_probability = 0.8,
        .mutation_probability = 0.5
    };

    ga_config.distance_cost = 100 / (float)(ga_config.entity_count * 4);


    // Creating a pool
    EntityPool epool(tp, ga_config);
    
    EntityPool::population_t population;

    epool.get_population(population, false);


    // GAO spec-settings
    float percent_of_gens = 0.9;
    size_t selection_threshold = 25;
    assert(selection_threshold <= ga_config.entity_count);

    std::vector<size_t> parents(ga_config.entity_count);
    std::vector<Entity*>   next(ga_config.entity_count);


    // Random tools setup
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // DEV: update each start
    std::mt19937_64 generator(seed);

    std::bernoulli_distribution             is_crossover(ga_config.crossover_probability), 
                                            is_mutation(ga_config.mutation_probability);

    // Data logger setup
    std::vector<float> HD_distribution(ga_config.entity_count);
    std::vector<long>  entities_origin(ga_config.entity_count);

    std::fstream result_file("result.csv", std::fstream::out); // TODO: cli or smth
    if (!result_file.is_open()) throw std::runtime_error("Error: cant open a file to write `result.csv`");

    // TODO:
    // - dynamical name generation (time, TaskProperties(figure count) and GAConfig(entities count))


    // Main loop
    long step_count = 10000;
    long threshold_for_print = step_count / 100;
    long current_step = 1;

    for (size_t i = 0; i < population.size(); i++)
            population[i]->origin_number = current_step;

    Entity* best; // DEV: to take after loop

    while (current_step <= step_count) {

        // Choosing parents & Crossover
        panmixia(population, parents);

        for (size_t i = 0; i < ga_config.entity_count; i++) {
            if (parents[i] != i && is_crossover(generator)) {
               
                Entity* c1 = epool.get_free_entity();
                Entity* c2 = epool.get_free_entity();
                
                discrete_crossover(
                    *population[i], *population[parents[i]], *c1, *c2);


                c1->status = Entity::USED; c2->status = Entity::USED;
                c1->is_active = false;     c2->is_active = false;

                c1->is_marked = false; c2->is_marked = false;

                c1->origin_number = current_step; c2->origin_number = current_step;
            }
        }

        epool.get_population(population, true);


        // Mutation (DEV: give a chance for all pool)
        for (size_t i = 0; i < population.size(); i++) {
            if (is_mutation(generator)) {
                auto ep = population[i];
                discrete_mutate(*ep, percent_of_gens);
                ep->origin_number = current_step;
                ep->is_marked     = false;
                ep->is_mutated    = true;
            }
        }
        

        // Estimate all population
        for (size_t i = 0; i < population.size(); i++) {
            auto ep = population[i];
            if (ep->is_marked == false) mark_entity(*ep, ga_config, tp);
        }


        // Selection
        truncate_and_choose(selection_threshold, population, next);

        for (size_t i = 0; i < population.size(); i++) {
            if (population[i]->status == Entity::USED) population[i]->status = Entity::FREE;
        }

        for (size_t i = 0; i < ga_config.entity_count; i++) {

            next[i]->is_active = true;
            next[i]->is_mutated = false;

            if (next[i]->status == Entity::RESERVED) {
                next[i]->status = Entity::USED;

            } else {
                Entity* dublicate = epool.get_free_entity();
                *dublicate = *next[i];
            } 
        }

        epool.get_population(population, false);


        // Metrics
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


        for (size_t i = 0; i < HD_distribution.size(); i++)
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


        if (current_step % (threshold_for_print) == 0)  
            std::cerr << current_step / (float) step_count * 100 << '%' << std::endl;

        current_step++;
    }

    // Result print
    print_memory_report(*best, ga_config);

    std::cout << "\nBEST SOLUTION:\n";
    print_entity_mark(*best);

    std::cout << best->decode << std::endl;

    Field best_field(tp);

    build_solution(best->decode, best_field);

    std::cout << best_field << std::endl;


    result_file.close();
}
