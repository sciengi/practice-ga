
#include <GA.h>
#include <StatManager.h>


int main() {

    TaskProperties tp = get_task();
    
    if(!Figure::types_is_enough(tp)) 
        throw std::runtime_error("Error: Figure types not enough for this task");


    GAConfig gc = {
        .entity_count = 50,

        .out_cost = 200,
        .overlay_cost = 100,
        .distance_cost = 0,  // DEV: tricky  
        .row_cost = 10,

        .crossover_probability = 0.8,

        .mutation_probability = 0.1,
        .percent_of_mutated_gens = 0.1,

        .selection_threshold = 25 
    };

    gc.distance_cost = 100 / (float)(gc.entity_count * 4);

    
    GA ga(tp, gc);
    StatManager sm(gc);


    size_t epoch_count = 10000;
    size_t current_epoch = 1;
    size_t threshold_to_print = epoch_count / 100; 
    while (current_epoch <= epoch_count) {
        ga.evolve_population(current_epoch);
        sm.fetch_stat(current_epoch, ga.get_population());
        if (current_epoch % threshold_to_print == 0) 
            std::cout << current_epoch / threshold_to_print << '%' << std::endl;
        current_epoch++;
    }


    auto best = sm.get_best();
    print_memory_report(*best, gc);

    std::cout << "\nBEST SOLUTION:\n";
    print_entity_mark(*best);

    std::cout << best->decode << std::endl;

    Field best_field(tp);

    build_solution(best->decode, best_field);

    std::cout << best_field << std::endl;
}
