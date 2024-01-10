#include "line-solver.h"
#include<iostream>
#include<thread>

namespace ls = linesolver;

void solveTableLine(ls::Table *table, bool const is_column, std::vector<int> const positions) {
    unsigned int n = 0;
    int pos = 0;
    std::vector<bool> all_solved;
    bool break_flag = false;

    for (int i=0; i<(int)positions.size(); i++) {
        all_solved.push_back(false);
    }

    while (n < 10000) {
        break_flag = true;
        for (int i=0; i<(int)positions.size(); i++) {
            if (!all_solved[i]) {
                pos = positions[i];
                table->pushLine(ls::partialFill(table->getLine(is_column, pos)));
                n++;
                break_flag = false;
            }
            if (ls::isSolved(table->getLine(is_column, pos))) all_solved[i] = true;
        }
        if (break_flag) break;
    }

    return;
}

int main() {
    unsigned long const table_size = 20;

    // create grid
    ls::Table table((int)table_size);
    table.setASequence(true,0,std::vector<int>({5}));
    table.setASequence(true,1,std::vector<int>({3,7,2}));
    table.setASequence(true,2,std::vector<int>({3,2,7}));
    table.setASequence(true,3,std::vector<int>({1,1,1,1,2,2}));
    table.setASequence(true,4,std::vector<int>({1,1,4,4,3}));
    table.setASequence(true,5,std::vector<int>({1,1,2,1,3}));
    table.setASequence(true,6,std::vector<int>({1,1,2,1,1,1,1}));
    table.setASequence(true,7,std::vector<int>({1,1,4,1,1}));
    table.setASequence(true,8,std::vector<int>({1,1,1,2,2,2}));
    table.setASequence(true,9,std::vector<int>({1,1,2,6,3}));
    table.setASequence(true,10,std::vector<int>({1,2,10}));
    table.setASequence(true,11,std::vector<int>({2,2,4,3}));
    table.setASequence(true,12,std::vector<int>({2,3,4}));
    table.setASequence(true,13,std::vector<int>({4,5}));
    table.setASequence(true,14,std::vector<int>({17}));
    table.setASequence(true,15,std::vector<int>({1,2,9}));
    table.setASequence(true,16,std::vector<int>({1,3,9}));
    table.setASequence(true,17,std::vector<int>({3,9}));
    table.setASequence(true,18,std::vector<int>({11}));
    table.setASequence(true,19,std::vector<int>({5}));
    table.setASequence(false,0,std::vector<int>({6}));
    table.setASequence(false,1,std::vector<int>({2,3}));
    table.setASequence(false,2,std::vector<int>({2,6}));
    table.setASequence(false,3,std::vector<int>({2,2,1}));
    table.setASequence(false,4,std::vector<int>({5,5,3,2}));
    table.setASequence(false,5,std::vector<int>({2,2,2,3,1}));
    table.setASequence(false,6,std::vector<int>({1,3,3,1,1,1,1}));
    table.setASequence(false,7,std::vector<int>({3,4,1,1,1,1,1}));
    table.setASequence(false,8,std::vector<int>({2,4,2,1,3}));
    table.setASequence(false,9,std::vector<int>({1,1,3,1,6}));
    table.setASequence(false,10,std::vector<int>({1,1,1,4,6}));
    table.setASequence(false,11,std::vector<int>({2,3,6}));
    table.setASequence(false,12,std::vector<int>({2,1,2,6}));
    table.setASequence(false,13,std::vector<int>({2,1,2,5}));
    table.setASequence(false,14,std::vector<int>({3,3,6}));
    table.setASequence(false,15,std::vector<int>({8,1,5}));
    table.setASequence(false,16,std::vector<int>({2,1,6}));
    table.setASequence(false,17,std::vector<int>({5,8}));
    table.setASequence(false,18,std::vector<int>({3,7}));
    table.setASequence(false,19,std::vector<int>({9}));

    // create threads and divide their responsibilities (which thread is responsible for which lines)
    std::vector<std::thread> threads;

    // figure out number of threads
    unsigned long const max_threads = table_size*2;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads!=0?2*(hardware_threads/2):2,max_threads);
    unsigned long const block_size = table_size/(num_threads/2);

    // create threads
    std::vector<int> block;
    // columns
    for (int i=0; i<(int)num_threads/2-1; i++) {
        block.clear();
        for (int j=0; j<(int)block_size; j++) block.push_back(block_size*i+j);
        threads.emplace_back(solveTableLine,&table,true,block);
    }
    // last columns
    block.clear();
    for (int j=(int)block_size*(num_threads/2-1); j<(int)table_size; j++) block.push_back(j);
    threads.emplace_back(solveTableLine,&table,true, block);

    // rows
    for (int i=0; i<(int)num_threads/2-1; i++) {
        block.clear();
        for (int j=0; j<(int)block_size; j++) block.push_back(block_size*i+j);
        threads.emplace_back(solveTableLine,&table,false,block);
    }
    // last rows
    block.clear();
    for (int j=(int)block_size*(num_threads/2-1); j<(int)table_size; j++) block.push_back(j);
    threads.emplace_back(solveTableLine,&table,false, block);
    
    // join all
    for (auto& th : threads) {
        if (th.joinable()) th.join();
    }

    // output result
    table.printTable();

    return 0;
}