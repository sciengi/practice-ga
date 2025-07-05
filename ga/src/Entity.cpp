#include <Entity.h>



Entity::Entity(const TaskProperties& tp) 
    : status(FREE), is_active(true), is_mutated(false), is_marked(false), code(get_coded_solution(tp)) {}

