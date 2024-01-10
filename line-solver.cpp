#include "line-solver.h"
#include "utils.h"
#include <mutex>

#include <iostream>

namespace ls = linesolver;

// Square
// constructor
ls::Square::Square(short fill_value_) : fill_value(fill_value_) {}

// copy operator
ls::Square::Square(ls::Square const& other) {
    std::lock_guard<std::mutex> lock(other.m);
    this->fill_value = other.fill_value;
}

// assignment operator
ls::Square& ls::Square::operator= (ls::Square const& other) {
    if(&other != this) {
        std::unique_lock<std::mutex> lock_this(this->m, std::defer_lock);
        std::unique_lock<std::mutex> lock_other(other.m, std::defer_lock);

        std::lock(lock_this, lock_other);

        this->fill_value = other.fill_value;
    }

    return *this;
}

short ls::Square::getFillValue() const { return fill_value; }
bool ls::Square::operator== (ls::Square const& b) const {
            return (this->fill_value == b.getFillValue());
}
void ls::Square::setFillValue(short const new_fill_value) { 
    //std::lock_guard<std::mutex> lock(this->mHolder.mutex);
    std::lock_guard<std::mutex> lock(this->m);
    this->fill_value = new_fill_value; 
    return;
}

// Table
ls::Table::Table(int const size_) : size(size_) {
    for (int i=0; i<size_; i++) {
        // create size_^2 squares
        table.push_back(std::vector<Square>());
        for (int j=0; j<size_; j++) {
            table[i].push_back(Square(0));
        }
        // push 2 times size sequences
        sequences.push_back({});
        sequences.push_back({});
    }
    solved_squares = 0;
}

ls::Line ls::Table::getLine(bool const is_column, int const pos) {
    ls::Line ret_line;
    ret_line.length = this->size;
    ret_line.pos = pos;
    ret_line.is_column = is_column;

    assert(this->sequences.size() == (unsigned long)2*this->size);
    if (is_column) {
        ret_line.sequence = this->sequences[pos];
        for (int i=0; i<size; i++) {
            ret_line.line.push_back(this->table[pos][i]);
        }
    } else {
        ret_line.sequence = this->sequences[this->size+pos];
        for (int i=0; i<size; i++) {
            ret_line.line.push_back(this->table[i][pos]);
        }
    }

    return ret_line;
}

void ls::Table::pushLine(ls::Line const& line) {
    if (line.is_column) {
        for (int i=0; i<this->size; i++) {
            if(line.line[i].getFillValue() == 0) continue;
            this->table[line.pos][i].setFillValue(line.line[i].getFillValue());
        }
    } else {
        for (int i=0; i<this->size; i++) {
            if(line.line[i].getFillValue() == 0) continue;
            this->table[i][line.pos].setFillValue(line.line[i].getFillValue());
        }
    }

    return;
}

void ls::Table::setASequence(bool const is_column, int const pos, std::vector<int> const& sequence) {
    if(is_column) this->sequences[pos] = sequence;
    else this->sequences[this->size+pos] = sequence;
    return;
}

void ls::Table::printTable() {
    char mark;
    for (int i=0; i<this->size; i++) {
        for (int j=0; j<this->size; j++) {
            switch (this->table[j][i].getFillValue()) {
                case 0:
                    mark = '-';
                    break;
                case 1:
                    mark = 'O';
                    break;
                case -1:
                    mark = 'X';
                    break;
            }
            std::cout << mark;
        }
        std::cout << "\n";
    }

    return;
}

bool ls::isSolved(ls::Line const& line) {
    for (auto const& element : line.line) {
        if (element.getFillValue() == 0) return false;
    }
    return true;
}

ls::Line ls::partialFill(ls::Line const& line) {
    ls::Line ret_line;
    ls::Line tmp_line;
    bool invalid_line = false;

    // initialize temporary line
    tmp_line.is_column = line.is_column;
    tmp_line.length = line.length;
    tmp_line.pos = line.pos;
    tmp_line.sequence = line.sequence;

    for (const auto& seq : tmp_line.sequence) {
        for (int i=0; i<seq; i++) {
            tmp_line.line.push_back(ls::Square(1));
        }
        if (tmp_line.line.size() < (unsigned long)tmp_line.length) tmp_line.line.push_back(ls::Square(-1));
    }

    while (tmp_line.line.size() < (unsigned long)tmp_line.length) {
        tmp_line.line.push_back(ls::Square(-1));
    }

    do {
        invalid_line = false;
        // check if this line attempt is valid, if not get next line attempt
        for (int i=0; i<line.length; i++) {
            if (line.line[i].getFillValue() != 0 && tmp_line.line[i].getFillValue() != line.line[i].getFillValue()) {
                invalid_line = true;
                break;
            }
        }
        if (invalid_line) {
            tmp_line = ls::getNextLine(tmp_line);
            continue;
        }

        // if return line has not been set yet make it equal to the first valid attempt

        if (ret_line.length == 0) {
            ret_line = tmp_line;
            tmp_line = ls::getNextLine(tmp_line);
            continue;
        }


        for (int i=0; i<line.length; i++) {
            if (ret_line.line[i].getFillValue() != 0 && tmp_line.line[i].getFillValue() != ret_line.line[i].getFillValue()) 
                ret_line.line[i].setFillValue(0);
        }

        tmp_line = ls::getNextLine(tmp_line);
    } while (tmp_line.length > 0);

    return ret_line;
}

// check if anything can be pushed further right 
bool ls::isThereNext(ls::Line const& line) {
    int cur_pos = line.length-1;
    for (auto seq_it = line.sequence.rbegin(); seq_it != line.sequence.rend(); ++seq_it) {

        for (int i=0; i<*seq_it; i++) {
            if (line.line[cur_pos--].getFillValue() != (short)1) {
                return true;
            }
        }
        cur_pos--;
    }

    return false;
}

// push to the right
ls::Line ls::getNextLine(ls::Line const& line) {

    // if there isn't a next line, return
    if (!(ls::isThereNext(line))) return ls::Line();

    // convert line to bits
    std::bitset<MAX_LINE_SIZE> line_as_bits = ls::lineToBits(line);

    // get next line as bits
    std::bitset<MAX_LINE_SIZE> next_line_as_bits(utils::getNextBitPermutation(line_as_bits.to_ullong()));

    // return next line converted from bits
    return ls::bitsToLine(next_line_as_bits,line.sequence,line.length,line.is_column,line.pos);
}

// convert a line to a bitset
std::bitset<MAX_LINE_SIZE> ls::lineToBits(ls::Line const& line) {
    std::bitset<MAX_LINE_SIZE> bits;
    int cur_pos = 0;
    int cur_pos_bit = 0;
    int cur_seq = 0;

    assert(line.length <= MAX_LINE_SIZE);

    while (cur_pos < line.length) {
        if(line.line[cur_pos].getFillValue() == (short)1) {
            bits.set(cur_pos_bit++,true);
            cur_pos += line.sequence[cur_seq++]+1;
        }
        else {
            bits.set(cur_pos_bit++,false);
            cur_pos++;
        }
    }

    return bits;
}

// convert a bitset back to a line
ls::Line ls::bitsToLine(std::bitset<MAX_LINE_SIZE> const& bits, 
                        std::vector<int> const& sequence, 
                        int length, 
                        bool is_column, 
                        int pos) {

    assert(length <= MAX_LINE_SIZE);

    ls::Line ret_line;
    int cur_pos_bit = 0;
    int cur_seq = 0;

    ret_line.is_column = is_column;
    ret_line.pos = pos;
    ret_line.sequence = sequence;
    ret_line.length = length;

    while (ret_line.line.size() < (unsigned long)length) {
        if(bits[cur_pos_bit++]) {
            for (int i=0; i<sequence[cur_seq]; i++) {
                ret_line.line.push_back(ls::Square(1));
            }
            if ((unsigned long)cur_seq < sequence.size()-1) {
                cur_seq++;
                ret_line.line.push_back(ls::Square(-1));
            }
        }
        else {
            ret_line.line.push_back(ls::Square(-1));
        }
    }

    return ret_line;
}
