#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <bitset>
#include <random>
#include <chrono>

#include <Figure.h>
#include <Field.h>



typedef std::vector<bool> coded_solution;
typedef std::vector<Figure> solution;

coded_solution get_coded_solution(const TaskProperties& tp);

void random_fill(coded_solution& cs, float p = 0.5);

Figure decode_figure(size_t index, const coded_solution& cs, const TaskProperties& tp);

solution decode_solution(const coded_solution& cs, const TaskProperties& tp);

void place(const Figure& figure, Field& field);

void build_solution(const solution& s, Field& field);

std::ostream& operator<<(std::ostream& stream, const coded_solution& cs);

std::ostream& operator<<(std::ostream& stream, const solution& s);

#endif
