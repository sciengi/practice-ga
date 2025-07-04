#include <Figure.h>

const size_t Figure::max_bit_count = std::max(std::max(sizeof(Figure::coordx_t), sizeof(Figure::coordy_t)), sizeof(Figure::rot_t));


const std::unordered_map<char, std::array<int, 8>> Figure::type_to_form = {
    {
        {'O', { 1, 1, 0, 0,
                1, 1, 0, 0 }}, 

        {'I', { 1, 1, 1, 1,
                0, 0, 0, 0 }},

        {'T', { 1, 1, 1, 0,
                0, 1, 0, 0 }}, 

        {'S', { 1, 1, 0, 0,
                0, 1, 1, 0 }}, 

        {'Z', { 0, 1, 1, 0,
                1, 1, 0, 0 }}, 

        {'L', { 1, 0, 0, 0,
                1, 1, 1, 0 }}, 

        {'J', { 1, 1, 1, 0,
                1, 0, 0, 0 }}, 
    }
};


bool Figure::types_is_enough(const TaskProperties& tp) {
    return std::numeric_limits<Figure::coordx_t>::digits >= tp.bits_for_width 
        && std::numeric_limits<Figure::coordy_t>::digits >= tp.bits_for_height 
        && std::numeric_limits<Figure::rot_t>   ::digits >= tp.bits_for_rotation;
}


