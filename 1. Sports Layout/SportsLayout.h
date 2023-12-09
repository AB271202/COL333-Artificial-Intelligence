
#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H

#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <chrono>
using namespace std;
#define timetype std::chrono::high_resolution_clock::time_point
class SportsLayout{

    private:
    long long z,l;
    long long** T;
    long long **N;
    float time;
    int *mapping;

    public:
    SportsLayout(string inputfilename);

    bool check_output_format();

    // void readOutputFile(string output_filename);
    
    long long cost_fn();

    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);

    int* random_mapping();


    int* opt_neighbour(int *mapping, long long * opt, long long current_cost);
    
    long long neighbour_cost_fn(int* current_mapping);

    int* greedy_hill_climbing(int *mapping, int max_iterations, long long* value, timetype time, int* lm);
    
    int* random_restart_greedy_hill_climbing(int max_iterations);


    int* best_neighbour_first(int *old_mapping, int max_iterations, long long *value, timetype initialTime, int* lm);
    int *best_first_neighbour(int *current_mapping, long long *opt, long long current_cost);
    int* best_neighbour_first_with_restarts(int max_iterations);
    
    void compute_allocation();


};


#endif
