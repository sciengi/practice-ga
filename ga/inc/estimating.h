#ifndef ESTIMATING_H
#define ESTIMATING_H

#include <TaskProperties.h>
#include <Field.h>
#include <Entity.h>
#include <GAConfig.h>



long count_outer_cells(const Field& field, const TaskProperties& tp);

long count_overlay_figures(const Field& field, const TaskProperties& tp);

long count_cluster_distance(const Field& field, const TaskProperties& tp);

long count_used_rows(const Field& field, const TaskProperties& tp);


void mark_entity(Entity& e, const GAConfig& ga_config, const TaskProperties& tp);


void print_entity_mark(const Entity& e);



float calculate_HD(const Entity& e1, const Entity& e2);



#endif
