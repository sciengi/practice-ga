#ifndef FIELD_H
#define FIELD_H

#include <TaskProperties.h>

class Row {
private:
    size_t m_offset;
    int*   m_memory;

public:

    Row(size_t offset, int* memory);

    int& operator[](int index);

    const int& operator[](int index) const;
    
    int& get(int index);

    const int& get(int index) const;

};

class Field {

private:
    size_t m_width;
    size_t m_height;

    size_t m_offset_x;
    size_t m_offset_y;

    int*   m_memory;

public:
    Field(size_t width, size_t height, size_t offset_x, size_t offset_y);

    Field(const TaskProperties& tp);

    ~Field();


    size_t width(bool real = false) const;
    size_t height(bool real = false) const;

    size_t offset_x() const;
    size_t offset_y() const;


    bool is_acceptable_area(size_t x, size_t y) const;


    Row operator[](int index);

    const Row operator[](int index) const;

    Row get(int index);

    const Row get(int index) const;

    int& get_direct(int index);
    
    const int& get_direct(int index) const;

    // TODO:
    // - check all places with []/get, choose the best
};

std::ostream& operator<<(std::ostream& stream, const Field& field);

#endif
