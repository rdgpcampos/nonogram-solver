#include "line-solver.h"
#include "utils.h"
#include<cassert>
#include <iostream>

#define PADDED_STRING_LENGTH 30

namespace ls = linesolver;

// include more examples to test this function more thoroughly
std::string TestLineToBits() {
    ls::Line line;
    line.is_column = true;
    line.length = 11;
    line.sequence = std::vector<int>{1,2,3};
    line.pos = 1;
    std::vector<ls::Square> squares{
        ls::Square(-1),
        ls::Square(1),
        ls::Square(-1),
        ls::Square(1),                                       
        ls::Square(1),
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(-1)
    };
    line.line = squares;

    // Test line to bits
    std::bitset<MAX_LINE_SIZE> bits = ls::lineToBits(line);
    
    assert(bits == 0b00000010110);

    return utils::padString("lineToBits",PADDED_STRING_LENGTH) + "OK";
}

std::string TestBitsToLine () {
    ls::Line line;
    line.is_column = true;
    line.length = 11;
    line.sequence = std::vector<int>{1,2,3};
    line.pos = 1;
    std::vector<ls::Square> squares{
        ls::Square(-1),
        ls::Square(1),
        ls::Square(-1),
        ls::Square(1),                                       
        ls::Square(1),
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(-1)
    };
    line.line = squares;

    std::bitset<MAX_LINE_SIZE> bits;
    bits.set(1); bits.set(2); bits.set(4);

    assert(line.line == ls::bitsToLine(bits, line.sequence, line.length, line.is_column, line.pos).line);

    return utils::padString("bitsToLine",PADDED_STRING_LENGTH) + "OK";
}

std::string TestIsThereNext() {
    ls::Line line;
    line.is_column = true;
    line.length = 11;
    line.sequence = std::vector<int>{1,2,3};
    line.pos = 1;
    std::vector<ls::Square> squares{
        ls::Square(-1),
        ls::Square(1),
        ls::Square(-1),
        ls::Square(1),                                       
        ls::Square(1),
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(-1)
    };
    line.line = squares;

    assert(ls::isThereNext(line));

    line.line = std::vector<ls::Square>({
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(1),                                       
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(1)
    });

    assert(ls::isThereNext(line) == false);
    return utils::padString("isThereNext",PADDED_STRING_LENGTH) + "OK";
}

std::string TestGetNextLine() {
    ls::Line line;
    line.is_column = true;
    line.length = 11;
    line.sequence = std::vector<int>{1,2,3};
    line.pos = 1;
    std::vector<ls::Square> squares{
        ls::Square(-1),
        ls::Square(1),
        ls::Square(-1),
        ls::Square(1),                                       
        ls::Square(1),
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(-1)
    };
    line.line = squares;

    line = ls::getNextLine(line);
    assert(line.line == std::vector<ls::Square>({
        ls::Square(1),
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(-1),                                       
        ls::Square(1),
        ls::Square(1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(-1)
    }));
    line = ls::getNextLine(line);
    assert(line.line == std::vector<ls::Square>({
        ls::Square(-1),
        ls::Square(1),
        ls::Square(-1),
        ls::Square(-1),                                       
        ls::Square(1),
        ls::Square(1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(-1)
    }));
    line = ls::getNextLine(line);
    assert(line.line == std::vector<ls::Square>({
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(-1),                                       
        ls::Square(1),
        ls::Square(1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(-1)
    }));
    line = ls::getNextLine(line);
    assert(line.line == std::vector<ls::Square>({
        ls::Square(1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),                                       
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(1)
    }));
    line = ls::getNextLine(line);
    assert(line.line == std::vector<ls::Square>({
        ls::Square(1),
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(1),                                       
        ls::Square(1),
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(1)
    }));

    line.line = std::vector<ls::Square>({
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(-1),
        ls::Square(1),                                       
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(-1),
        ls::Square(1),
        ls::Square(1),
        ls::Square(1)
    });

    assert(ls::getNextLine(line).length == 0);

    return utils::padString("getNextLine",PADDED_STRING_LENGTH) + "OK";
}

std::string TestPartialFill() {
    ls::Line line;
    line.is_column = true;
    line.length = 20;
    line.sequence = std::vector<int>{6,3};
    line.pos = 1;
    line.line = std::vector<ls::Square>({
        ls::Square(0),
        ls::Square(0),
        ls::Square(0),
        ls::Square(0),
        ls::Square(0),
        ls::Square(0),
        ls::Square(0),
        ls::Square(1),
        ls::Square(0),
        ls::Square(0),
        ls::Square(0),
        ls::Square(0),
        ls::Square(0),
        ls::Square(0),
        ls::Square(0),
        ls::Square(-1),
        ls::Square(0),
        ls::Square(0),                               
        ls::Square(-1),
        ls::Square(0)
    });

    std::cout << ls::partialFill(line) << "\n";


    return utils::padString("partialFill",PADDED_STRING_LENGTH) + "OK";
}

int main() {
    std::cout << TestLineToBits() << "\n";
    std::cout << TestBitsToLine() << "\n";
    std::cout << TestIsThereNext() << "\n";
    std::cout << TestGetNextLine() << "\n";
    //std::cout << TestPartialFill() << "\n";

    return 0;
}