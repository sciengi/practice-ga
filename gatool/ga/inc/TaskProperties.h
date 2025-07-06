#ifndef TASKPROPERTIES_H
#define TASKPROPERTIES_H

#include <iostream>
#include <unordered_map>

#include <algorithm>

#include <cassert>
#include <cmath>


struct TaskProperties {

    static constexpr char figure_types[] = "OITSZLJ";
    static constexpr size_t types_count = sizeof(figure_types) - 1;

    static const int figure_width_bound  = 4;
    static const int figure_height_bound = 4;
    static const int offset_width  = figure_width_bound  - 1;
    static const int offset_height = figure_height_bound - 1;

    static const int bits_for_rotation = 2;


    int figure_count;
    std::unordered_map<char, int> types_amount;

    int field_width;
    int field_height;

    int field_real_width;
    int field_real_height;

    int field_width_bound;
    int field_height_bound;


    int bits_for_width;
    int bits_for_height;
    // int bits_for_rotation; // fixed by task
    int bits_per_figure;


    TaskProperties();

    TaskProperties(int field_width, const std::unordered_map<char, int>& types_amount);
};

TaskProperties get_task();

#endif
