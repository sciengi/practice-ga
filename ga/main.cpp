
#include <fstream>

#include <GA.h>



bool compare_to_sort(const Entity* left, const Entity* right) { return left->total < right->total; }


int main() {

    TaskProperties tp = get_task();
    
    if(!Figure::types_is_enough(tp)) 
        throw std::runtime_error("Error: Figure types not enough for this task");



    GAConfig ga_config = {
        .entity_count = 50,
        .out_cost = 100,
        .overlay_cost = 50,
        .row_cost = 10,
        .crossover_probability = 0.8,
        .mutation_probability = 0.3
    };


    EntityPool epool(tp, ga_config);

    EntityPool::population_t population, all;

    epool.get_population(population);
    epool.get_all(all);


    // GA spec-settings
    float percent_of_gens = 0.5;
    size_t selection_threshold = 50;
    assert(selection_threshold <= ga_config.entity_count);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 generator(seed);
    std::uniform_int_distribution<size_t>   choise_from_population(0, ga_config.entity_count - 1),
                                            choise_from_selection(0, selection_threshold);
    std::bernoulli_distribution             is_crossover(ga_config.crossover_probability), 
                                            is_mutation(ga_config.mutation_probability);


    std::fstream result_file("result.csv", std::fstream::out); // TODO: cli or smth
    if (!result_file.is_open()) throw std::runtime_error("Error: cant open a file to write `result.csv`");

    long step_count = 500000;

    long current_step = 1;
    while (current_step <= step_count) {

        // Choosing parents & Crossover
        for (size_t i = 0; i < ga_config.entity_count; i++) {

            size_t partner_index = choise_from_population(generator);

            if (partner_index != i && is_crossover(generator)) {
               
                Entity* c1 = epool.get_free_entity();
                Entity* c2 = epool.get_free_entity();

                discrete_crossover(
                        *epool.get_entity(i), *epool.get_entity(partner_index), *c1, *c2);

                c1->is_marked = false;
                c2->is_marked = false;
            }
        }

        
        // Mutation (DEV: give a chance for all pool)
        for (size_t i = 0; i < epool.size(); i++) {
            if (is_mutation(generator)) {
                auto ep = epool.get_entity(i);
                discrete_mutate(*ep, percent_of_gens);
                ep->is_marked = false;
            }
        }

        
        // Estimate all population
        for (size_t i = 0; i < epool.size(); i++) {
            auto ep = epool.get_entity(i);
            if (ep->is_marked == false) mark_entity(*ep, ga_config, tp);
        }


        // Selection
        std::sort(all.begin(), all.end(), compare_to_sort);

        for (size_t i = 0; i < population.size(); i++)   population[i]->in_population = false;
        for (size_t i = 0; i < selection_threshold; i++) all[i]       ->in_population = true;

        for (size_t i = 0; i < ga_config.entity_count - selection_threshold; i++) {
            
            size_t selected = choise_from_selection(generator);

            auto ep = epool.get_free_entity();
            *ep = *all[selected];
            ep->in_population = true;
        }

        epool.get_population(population);


        // Metrics
        long best_total = all.front()->total;
        long best_penalty = best_total - all.front()->fitness;

        double mean_total = 0;
        double mean_penalty = 0;

        for(auto ep : population) {
            mean_total += ep->total;
            mean_penalty += ep->total - ep->fitness;
        }

        mean_total /= population.size();
        mean_penalty /= population.size();

        result_file 
            << current_step << ' '
            << best_total << ' '
            << mean_total << ' '
            << best_penalty << ' '
            << mean_penalty 
            << std::endl;

        std::cerr << current_step / (float) step_count * 100 << '%' << std::endl;

        current_step++;
    }

    Entity* best = all.front();
    
    print_memory_report(*best, ga_config);

    std::cout << "BEST SOLUTION:\n";
    print_entity_mark(*best);

    std::cout << best->decode << std::endl;

    Field best_field(tp);

    build_solution(best->decode, best_field);

    std::cout << best_field << std::endl;


    result_file.close();
}

// TODO:
// - change assert to exception in Figure::check_types
