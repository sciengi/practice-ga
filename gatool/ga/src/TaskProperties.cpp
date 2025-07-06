#include <TaskProperties.h>


TaskProperties::TaskProperties() {}


TaskProperties::TaskProperties(int field_width, const std::unordered_map<char, int>& types_amount) {
    
    this->types_amount = types_amount;

    figure_count = 0;
    std::for_each(types_amount.begin(), types_amount.end(), [this](auto& pair){
        figure_count += pair.second;
    });


    this->field_width = field_width;

    if (field_width > figure_width_bound) {
        int fig_per_row = field_width / figure_width_bound;
        field_height = figure_height_bound * (figure_count / fig_per_row + 1);

    } else {
        field_height = figure_height_bound * figure_count;
    }


    bits_for_width    = std::ceil(std::log2(field_width));
    field_real_width  = std::pow(2, bits_for_width);

    bits_for_height   = std::ceil(std::log2(field_height));
    field_real_height = std::pow(2, bits_for_height);


    bits_per_figure   = bits_for_width + bits_for_height + bits_for_rotation;


    field_width_bound  = field_real_width  + 2 * offset_width;
    field_height_bound = field_real_height + 2 * offset_height;


    #ifdef DEBUG
    std::cout << "DEBUG( " << __PRETTY_FUNCTION__ << " )\n";
    std::cout 
        << "TASK PROPERTIES:\n"
        << "N = " << figure_count << '\n'
        << "L = " << field_width  << " (L' = " << field_real_width  << ", FL = " << field_width_bound  << ")\n"
        << "H = " << field_height << " (H' = " << field_real_height << ", FH = " << field_height_bound << ")\n" 
        << "BITS PER:\n"
        << "width  = " << bits_for_width
        << "\nheight = " << bits_for_height
        << "\nrotate = " << bits_for_rotation
        << "\nfigure = " << bits_per_figure
        << std::endl;
    #endif

    assert(field_width_bound >= field_real_width);
    assert(field_real_width  >= field_width);

    assert(field_height_bound >= field_real_height);
    assert(field_real_height  >= field_height);
}


TaskProperties get_task() {
    
    std::cout << "Enter L: " << std::flush;
    int field_width; std::cin >> field_width;
    if (field_width < 0) std::runtime_error("L must be greater than zero");
    // TODO: width < 2 -> warn


    std::cout << "\nEnter count for (via space):\nO I T S Z L J" << std::endl;
    int count;
    std::unordered_map<char, int> data;
    for (size_t i = 0; i < TaskProperties::types_count; i++) {
        std::cin >> count;
        data[TaskProperties::figure_types[i]] = count;
    }


    #ifdef DEBUG
    std::cout << "DEBUG(get_task)" << std::endl;
    std::cout << "TYPES AMOUNT" << std::endl;
    std::for_each(data.begin(), data.end(), [](auto& pair){
        std::cout << pair.first << ' ' << pair.second << std::endl;
    });
    std::cout << std::endl;
    #endif

    return TaskProperties(field_width, data);
}
