#include <estimating.h>



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


long count_cluster_distance(const Field& field, const TaskProperties& tp) {

    long count = 0; int value;
    for (size_t i = 0; i < field.height(true); i++) {
        for (size_t j = 0; j < field.width(true); j++) {
             value = field.get(i).get(j);
             if (value > 0) count += value * i;
        }
    }

    return count;

    // TODO:
    // - make second method with cluster-distance
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


void mark_entity(Entity& e, const GAConfig& ga_config, const TaskProperties& tp) {
   
    e.decode = decode_solution(e.code, tp);

    Field field(tp);
    build_solution(e.decode, field);

    e.penalty_out      = ga_config.out_cost      * count_outer_cells(field, tp);
    e.penalty_overlay  = ga_config.overlay_cost  * count_overlay_figures(field, tp);
    e.penalty_distance = ga_config.distance_cost * count_cluster_distance(field, tp);
    
    e.fitness          = ga_config.row_cost      * count_used_rows(field, tp);

    e.total = e.fitness + e.penalty_out + e.penalty_overlay + e.penalty_distance;

    e.is_marked = true;
}


void print_entity_mark(const Entity& e) {
    std::cout 
        << "ENTITY MARK:" 
        << "\ntotal             = " << e.total 
        << "\n----------------------"
        << "\nfitness           = " << e.fitness
        << "\npenalty (out)     = " << e.penalty_out
        << "\npenalty (overlay) = " << e.penalty_overlay
        << "\npenalty (dist)    = " << e.penalty_distance
        << "\n" << std::endl;
}


float calculate_HD(const Entity& e1, const Entity& e2) {

    int hit_count = 0; // DEV: hit/miss count available
    for (size_t i = 0; i < e1.code.size(); i++) {
        if (e1.code[i] == e2.code[i]) hit_count++;
    }

    return hit_count / (float) e1.code.size();
}

