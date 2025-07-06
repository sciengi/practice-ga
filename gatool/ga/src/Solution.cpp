#include <Solution.h>



coded_solution get_coded_solution(const TaskProperties& tp) { return coded_solution(tp.bits_per_figure * tp.figure_count); }


void random_fill(coded_solution& cs, float p) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::bernoulli_distribution distribution(p);
    for (size_t i = 0; i < cs.size(); i++) if (distribution(generator)) cs[i] = !cs[i];
}



typedef std::vector<Figure> solution;


Figure decode_figure(size_t index, const coded_solution& cs, const TaskProperties& tp) {

    Figure fig;

    size_t field_start = index * tp.bits_per_figure;


    std::bitset<sizeof(Figure::coordx_t)> coded_width;
    for (size_t i = 0; i < tp.bits_for_width; i++)
        coded_width[i] = cs[field_start + i];

    fig.x = coded_width.to_ulong();
    field_start += tp.bits_for_width;
    

    std::bitset<sizeof(Figure::coordy_t)> coded_height;
    for (size_t i = 0; i < tp.bits_for_height; i++)
        coded_height[i] = cs[field_start + i];

    fig.y = coded_height.to_ulong();
    field_start += tp.bits_for_height;


    std::bitset<sizeof(Figure::rot_t)> coded_rotation;
    for (size_t i = 0; i < tp.bits_for_rotation; i++)
        coded_rotation[i] = cs[field_start + i];

    fig.rot = coded_rotation.to_ulong();
    field_start += tp.bits_for_rotation;

    assert(field_start == (index + 1) * tp.bits_per_figure);

    return fig;

    // TODO: 
    // - refactor, split into functions (mb use decltype and template to cover all struct fields)
}


solution decode_solution(const coded_solution& cs, const TaskProperties& tp) {

    solution s;

    for (size_t i = 0; i < tp.figure_count; i++)
        s.push_back(decode_figure(i, cs, tp));

    
    // Linear type distribution
    size_t figure_index = 0;
    for (auto& pair : tp.types_amount) {
        for (int i = pair.second; i != 0; i--)
            s[figure_index++].type = pair.first;
    }

    assert(figure_index == tp.figure_count);

    return s;

    // TODO:
    // - pass function to choose figure types order in coded solution
    // - check idea about realization of function via binary-decimal conversion
    // - check copy/move
}

void place(const Figure& figure, Field& field) {

    auto& pattern = Figure::type_to_form.at(figure.type);

    size_t x = figure.x, y = figure.y;
    for (size_t i = 0; i < pattern.size(); i++) {

        // field[y][x] = i + 1; // DEV: nice trace 

        if (pattern[i] == true) {
            field[y][x] += 1;
        }

        if (figure.rot == Figure::RIGHT) {
            if ((i + 1) % 4 == 0) { x = figure.x; y++; }
            else { x++; }

        } else if (figure.rot == Figure::TOP) {
            if ((i + 1) % 4 == 0) { y = figure.y; x--; }
            else { y++; }

        } else if (figure.rot == Figure::LEFT) {
            if ((i + 1) % 4 == 0) { x = figure.x; y--; }
            else { x--; }

        } else if (figure.rot == Figure::DOWN) {
            if ((i + 1) % 4 == 0) { y = figure.y; x++; }
            else { y--; }
        }
    }
}


void build_solution(const solution& s, Field& field) {


    for (auto& fig : s) place(fig, field);

}

std::ostream& operator<<(std::ostream& stream, const coded_solution& cs) {
    for (size_t i = 0; i < cs.size(); i++) {
        if (cs[i])  stream << '1';
        else        stream << '0';
    }

    return stream;
}

std::ostream& operator<<(std::ostream& stream, const solution& s) {
    for_each(s.begin(), s.end(), [&stream](const Figure& fig){
        stream 
            << "type = " << (char) fig.type
            << ", x = " <<  (long) fig.x
            << ", y = " <<  (long) fig.y
            << ", r = " <<  (long) fig.rot
            << '\n';
    });

    return stream;

    // TODO:
    // - cast to long...
}
