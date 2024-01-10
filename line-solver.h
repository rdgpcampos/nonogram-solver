#ifndef INCLUDED_LINESOLVER
#define INCLUDED_LINESOLVER

#include<vector>
#include<bitset>
#include "utils.h"
#include<ostream>
#include<cassert>

#include<mutex>

#define MAX_LINE_SIZE 64

namespace linesolver {
    class Square {
    private:
        short fill_value;
        mutable std::mutex m;
        //utils::mutex_holder mHolder;

    public:
        Square(short fill_value_ = -1);
        Square(Square const& other);
        short getFillValue() const;
        void setFillValue(short const new_fill_value);

        bool operator== (Square const& b) const;
        Square& operator= (Square const& other);
    };

    struct Line {
        int length = 0;
        std::vector<Square> line;
        std::vector<int> sequence;
        bool is_column;
        int pos;

        friend std::ostream& operator<<(std::ostream& os, const Line& line) {
            for (int i=0; i<line.length; i++) {
                os << line.line[i].getFillValue() << " ";
            }
            return os;
        }
    };

    class Table {
    private:
        int size;
        std::vector< std::vector<Square> > table;
        std::vector< std::vector<int> > sequences; // number sequence in each row/column
        int solved_squares; // continue until this is size^2

    public:
        Table(int const size);
        Line getLine(bool const is_column, int const pos); // get a line from table 
        void pushLine(Line const& line); // update table with given line (don't lock line, lock current square)
        void setASequence(bool const is_column, int const pos, std::vector<int> const& sequence);
        void printTable();
    };

    bool isSolved(Line const& line); // check if line is solved or not
    Line partialFill(Line const& line); // solve obvious squares and return partially solved line
    bool isThereNext(Line const& line); // check if we can move any filled square to the right
    Line getNextLine(Line const& line); // get next line
    std::bitset<MAX_LINE_SIZE> lineToBits(Line const& line); // convert a line to a sequence of bits
    Line bitsToLine(std::bitset<MAX_LINE_SIZE> const& bits, std::vector<int> const& sequence, int length, bool is_column = true, int pos = 0); // convert a sequence of bits to a line
}

#endif