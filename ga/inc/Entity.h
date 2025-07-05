#ifndef ENTITY_H
#define ENTITY_H

#include <TaskProperties.h>
#include <Solution.h>

struct Entity {

    enum status_t { RESERVED, FREE, USED };


    status_t status;

    bool is_active;
    bool is_mutated;

    bool is_marked;
    

    long origin_number;

    long penalty_out;
    long penalty_overlay;

    long fitness;

    long total;
    

    coded_solution code;
    solution       decode;


    Entity(const TaskProperties& tp);
};

#endif
