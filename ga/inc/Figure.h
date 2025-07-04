#ifndef FIGURE_H
#define FIGURE_H

#include <array>
#include <unordered_map>
#include <limits>

#include <TaskProperties.h>



struct Figure {
    typedef int8_t coordx_t;
    typedef int8_t coordy_t;
    typedef int8_t rot_t;
    typedef int8_t type_t;

    static const size_t max_bit_count; 

    enum { RIGHT = 0, TOP, LEFT, DOWN };

    static const std::unordered_map<char, std::array<int, 8>> type_to_form;


    type_t type;
    coordx_t x;
    coordy_t y;
    rot_t rot;
    
    static bool types_is_enough(const TaskProperties& tp);
};

#endif
