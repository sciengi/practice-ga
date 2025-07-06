#include <Field.h>



Row::Row(size_t offset, int* memory) : m_offset(offset), m_memory(memory) {}

int& Row::operator[](int index) { 
    return m_memory[(index + m_offset)]; 
}

const int& Row::operator[](int index) const { 
    return m_memory[(index + m_offset)]; 
}

int& Row::get(int index) { 
    return m_memory[index]; 
}

const int& Row::get(int index) const { 
    return m_memory[index]; 
}



Field::Field(size_t width, size_t height, size_t offset_x, size_t offset_y) { 
    if (width == 0 || height == 0)
        throw std::invalid_argument("Field parameters must be greater than zero");
    
    m_width  = width;
    m_height = height;

    m_offset_x = offset_x;
    m_offset_y = offset_y;

    m_memory = new int[width * height]();

    // TODO: 
    // - check range of offsets
}

Field::Field(const TaskProperties& tp) 
	: Field(tp.field_width_bound, tp.field_height_bound, tp.offset_width, tp.offset_height) {}

Field::~Field() { delete[] m_memory; }


size_t Field::width(bool real) const { return real ? m_width : m_width - 2 * m_offset_x;   }
size_t Field::height(bool real) const { return real ? m_height : m_height - 2 * m_offset_y; }

size_t Field::offset_x() const { return m_offset_x; }
size_t Field::offset_y() const { return m_offset_y; }


bool Field::is_acceptable_area(size_t x, size_t y) const {
    return     y >= offset_y() 
            && y < height(true) - offset_y()
            && x >= offset_x()
            && x < width(true) - offset_x();
}


Row Field::operator[](int index) { 
    return Row(m_offset_x, &m_memory[(index + m_offset_y) * m_width]); 
}

const Row Field::operator[](int index) const { 
    return Row(m_offset_x, &m_memory[(index + m_offset_y) * m_width]); 
}

Row Field::get(int index) { 
    return Row(m_offset_x, &m_memory[index * m_width]); 
}

const Row Field::get(int index) const {
    return Row(m_offset_x, &m_memory[index * m_width]); 
}

int& Field::get_direct(int index) { return m_memory[index]; }

const int& Field::get_direct(int index) const { return m_memory[index]; }



std::ostream& operator<<(std::ostream& stream, const Field& field) {
   
    int value;
    for (ptrdiff_t i = field.height(true) - 1; i >= 0; i--) {
        for (size_t j = 0; j < field.width(true); j++) {
            value = field.get(i).get(j);
            switch(value) {
                case 0:
                    stream << '.'; break;
                case 1:
                    stream << '#'; break;
                default:
                    stream << 'X'; break;
            };

            stream << ' ';
        }
        stream << '\n';
    }

    return stream;
}
