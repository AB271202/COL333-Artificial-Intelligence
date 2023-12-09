#include <fstream>
#include <iostream>
// #include <vector>
#include <bits/stdc++.h>
#include <random>
#include <algorithm>
// #include <ctime>
#include <chrono>
#include <set>

#define now() std::chrono::high_resolution_clock::now()
#define duration(stop, start) std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
#define timetype std::chrono::high_resolution_clock::time_point
#define f(z) for (int i = 0; i < z; i++)
using namespace std;

#include "SportsLayout.h"

SportsLayout::SportsLayout(string inputfilename)
{

    readInInputFile(inputfilename);
    mapping = new int[z];
}

bool SportsLayout::check_output_format()
{

    vector<bool> visited(l, false);
    for (int i = 0; i < z; i++)
    {
        if ((mapping[i] >= 1 && mapping[i] <= l))
        {
            if (!visited[mapping[i] - 1])
                visited[mapping[i] - 1] = true;
            else
            {
                cout << "Repeated locations, check format\n";
                return false;
            }
        }
        else
        {
            cout << "Invalid location, check format\n";
            return false;
        }
    }

    return true;
}

long long SportsLayout::cost_fn()
{

    long long cost = 0;

    for (int i = 0; i < z; i++)
    {
        for (int j = 0; j < z; j++)
        {
            cost += (long long)N[i][j] * (long long)T[mapping[i] - 1][mapping[j] - 1];
        }
    }

    return cost;
}

void SportsLayout::readInInputFile(string inputfilename)
{
    fstream ipfile;
    ipfile.open(inputfilename, ios::in);
    if (!ipfile)
    {
        cout << "No such file\n";
        exit(0);
    }
    else
    {

        ipfile >> time;
        ipfile >> z;
        ipfile >> l;

        if (z > l)
        {
            cout << "Number of zones more than locations, check format of input file\n";
            exit(0);
        }

        long long **tempT;
        long long **tempN;

        tempT = new long long *[l];
        for (int i = 0; i < l; ++i)
            tempT[i] = new long long[l];

        tempN = new long long *[z];
        for (int i = 0; i < z; ++i)
            tempN[i] = new long long[z];

        for (int i = 0; i < z; i++)
        {
            for (int j = 0; j < z; j++)
                ipfile >> tempN[i][j];
        }

        for (int i = 0; i < l; i++)
        {
            for (int j = 0; j < l; j++)
                ipfile >> tempT[i][j];
        }

        ipfile.close();

        T = tempT;
        N = tempN;
    }
}

void SportsLayout::write_to_file(string outputfilename)
{

    // Open the file for writing
    ofstream outputFile(outputfilename);

    // Check if the file is opened successfully
    if (!outputFile.is_open())
    {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }

    for (int i = 0; i < z; i++)
        outputFile << mapping[i] << " ";

    // Close the file
    outputFile.close();

    cout << "Allocation written to the file successfully." << endl;
}

// Returns a random mapping of the z zones to location
int *SportsLayout::random_mapping()
{
    int *new_array = new int[z];
    std::vector<int> sequence;
    for (int i = 1; i <= l; ++i)
    {
        sequence.push_back(i);
    }
    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(sequence.begin(), sequence.end(), gen);
    for (int i = 0; i < z; ++i)
    {
        new_array[i] = sequence[i];
    }
    return new_array;
}
// Calculates the cost of the mapping with a particular neighbour
long long SportsLayout::neighbour_cost_fn(int *current_mapping)
{

    long long cost = 0;

    for (int i = 0; i < z; i++)
    {
        for (int j = 0; j < z; j++)
        {
            cost += (long long)N[i][j] * (long long)T[current_mapping[i] - 1][current_mapping[j] - 1];
        }
    }

    return cost;
}



int *SportsLayout::opt_neighbour(int *current_mapping, long long *opt, long long current_cost)
{
    // Checking new locations first
    // Creating a set of unvisited locations
    set<int> unvisited;
    for (int i = 1; i <= l; i++)
        unvisited.insert(i); // O(l)
    for (int i = 0; i < z; i++)
    {
        if ((unvisited.find(current_mapping[i]) != unvisited.end()))
            unvisited.erase(current_mapping[i]); // O(z)
    }

    // Now iterating in O(z^2(l-z))
    long long opt_val = current_cost;
    int optneighbour[z];
    int newneighbour[z];

    // Can't just equate the pointers! Both will get changed

    for (int i = 0; i < z; i++)
        newneighbour[i] = current_mapping[i];

    for (auto newelement : unvisited)
    {
        for (int k = 0; k < z; k++)
        {

            newneighbour[k] = newelement;
            long long neighbour_cost = current_cost;
            for (int i = 0; i < z; i++)
            {
                neighbour_cost += (long long)N[i][k] * ((long long)T[newneighbour[i] - 1][newneighbour[k] - 1] - (long long)T[current_mapping[i] - 1][current_mapping[k] - 1]) + (long long)N[k][i] * ((long long)T[newneighbour[k] - 1][newneighbour[i] - 1] - (long long)T[current_mapping[k] - 1][current_mapping[i] - 1]);
            }
            // cout<<((neighbour_cost==neighbour_cost_fn(newneighbour))? "Yes":"No")<<endl;
            if (neighbour_cost < opt_val)
            {
                opt_val = neighbour_cost;
                for (int i = 0; i < z; i++)
                    optneighbour[i] = newneighbour[i];
            }
            newneighbour[k] = current_mapping[k];
        }
    }
    // cout<<"Newloc\n";
    // for(int i=0; i<3; i++){cout<<optneighbour[i]<<" ";}
    // cout<<endl;
    // Now checking for swaps
    // Intermediate hybrid!
    for (int i = 0; i < z; i++)
        newneighbour[i] = current_mapping[i];

    int intermediateneighbour[z];
    for (int i = 0; i < z; i++)
        intermediateneighbour[i] = current_mapping[i];

    for (int k = 0; k < z; k++)
    {
        for (int l = k + 1; l < z; l++)
        {
            intermediateneighbour[k] = current_mapping[l];
            //         for(int i=0; i<3; i++){cout<<intermediateneighbour[i]<<" ";}
            // cout<<endl;
            newneighbour[k] = current_mapping[l];
            newneighbour[l] = current_mapping[k];
            //         for(int i=0; i<3; i++){cout<<newneighbour[i]<<" ";}
            // cout<<endl;
            long long neighbour_cost = current_cost;
            // neighbour_cost=neighbour_cost_fn(newneighbour);
            for (int i = 0; i < z; i++)
            {
                neighbour_cost += (long long)N[i][k] * ((long long)T[intermediateneighbour[i] - 1][intermediateneighbour[k] - 1] - (long long)T[current_mapping[i] - 1][current_mapping[k] - 1]) + (long long)N[k][i] * ((long long)T[intermediateneighbour[k] - 1][intermediateneighbour[i] - 1] - (long long)T[current_mapping[k] - 1][current_mapping[i] - 1]);
                neighbour_cost += (long long)N[i][l] * ((long long)T[newneighbour[i] - 1][newneighbour[l] - 1] - (long long)T[intermediateneighbour[i] - 1][intermediateneighbour[l] - 1]) + (long long)N[l][i] * ((long long)T[newneighbour[l] - 1][newneighbour[i] - 1] - (long long)T[intermediateneighbour[l] - 1][intermediateneighbour[i] - 1]);
            }
            // cout<<((neighbour_cost==neighbour_cost_fn(newneighbour))? "Yes":"No")<<endl;
            if (neighbour_cost < opt_val)
            {
                opt_val = neighbour_cost;
                for (int i = 0; i < z; i++)
                    optneighbour[i] = newneighbour[i];
            }
            intermediateneighbour[k] = current_mapping[k];
            newneighbour[k] = current_mapping[k];
            newneighbour[l] = current_mapping[l];
        }
    }

    // TRANSFER OPTNEIGHBOUR TO HEAP
    int *optneighbourheap = new int[z];
    for (int i = 0; i < z; i++)
        optneighbourheap[i] = optneighbour[i];

    // GIVE COST BY USING A POINTER
    // cout<<"lod"<<"\n";
    *opt = opt_val;
    return optneighbourheap;
}

int *SportsLayout::greedy_hill_climbing(int *current_mapping, int max_iterations, long long *value, timetype initialTime, int* lm)
{
    // int *current_mapping = old_mapping;
    long long current_value = neighbour_cost_fn(current_mapping);

    for (int i = 0; i < max_iterations; i++)
    // while (1)
    {
        long long best_neighbour_value;
        int *best_neighbour = SportsLayout::opt_neighbour(current_mapping, &best_neighbour_value, current_value);

        if (best_neighbour == nullptr)
        {
            // Handle allocation failure, e.g., by exiting the loop or returning an error value.
            break;
        }

        if (best_neighbour_value >= current_value)
        {
            // delete[] best_neighbour; // Don't forget to free the memory
            break;
        }

        // delete[] current_mapping; // Use delete[] for arrays
        current_mapping = best_neighbour;
        current_value = best_neighbour_value;
        auto d = duration(now(), initialTime);
        int kl;
        if (time>2){kl=20;}
        else{
            kl=10;
        }
        if (int(d.count()) / 100000 >= (600.0 * time - kl)) break;

    }

    // cout << current_value << endl;
    *value = current_value;
    return current_mapping;
}

int *SportsLayout::random_restart_greedy_hill_climbing(int max_iterations)
{
    int *best_mapping;
    long long best_value = __LONG_LONG_MAX__;

    auto initialTime = now();
    int maxtimegap = 0;
    int oldtime = 0;
    int numrestarts = 0;
    int *current_mapping = random_mapping();
    int lm = 0;
    long long value=0;
    while (1)
    {
        if (lm == 0 || 0.95*value>best_value)
            {current_mapping = random_mapping();}
        // else
        // {
        // }
        
        int *mapping = greedy_hill_climbing(current_mapping, max_iterations, &value, initialTime, &lm);
        if (value < best_value)
        {
            // delete[] best_mapping;
            best_mapping = mapping;
            current_mapping = mapping;
            best_value = value;
        }
        // cout << (long long)best_value << endl;
        auto finalTime = now();
        auto d = duration(finalTime, initialTime);
        maxtimegap = std::max(maxtimegap, int(d.count()) - oldtime);
        oldtime = d.count();

        int kl;
        if (time>2){kl=20;}
        else{
            kl=10;
        }
        if ((double(d.count()) / 100000.0) >= (600.0 * time - maxtimegap / 100000.0 - kl))
        {
            // cout << "Number of restarts: " << numrestarts << endl;
            break;
        }
        numrestarts++;
    }
    int *best_mapping_heap = new int[z];
    f(z) best_mapping_heap[i] = best_mapping[i];
    return best_mapping_heap;
}



int *SportsLayout::best_first_neighbour(int *current_mapping, long long *opt, long long current_cost)
{
    // Checking new locations first
    // Creating a set of unvisited locations
    set<int> unvisited;
    for (int i = 1; i <= l; i++)
        unvisited.insert(i); // O(l)
    for (int i = 0; i < z; i++)
    {
        if ((unvisited.find(current_mapping[i]) != unvisited.end()))
            unvisited.erase(current_mapping[i]); // O(z)
    }

    // Now iterating in O(z^2(l-z))
    long long opt_val = current_cost;
    int optneighbour[z];
    int newneighbour[z];

    // Can't just equate the pointers! Both will get changed
    bool bt = true;
    for (int i = 0; i < z; i++)
        newneighbour[i] = current_mapping[i];

    for (auto newelement : unvisited)
    {
        for (int k = 0; k < z; k++)
        {

            newneighbour[k] = newelement;
            long long neighbour_cost = current_cost;
            for (int i = 0; i < z; i++)
            {
                neighbour_cost += (long long)N[i][k] * ((long long)T[newneighbour[i] - 1][newneighbour[k] - 1] - (long long)T[current_mapping[i] - 1][current_mapping[k] - 1]) + (long long)N[k][i] * ((long long)T[newneighbour[k] - 1][newneighbour[i] - 1] - (long long)T[current_mapping[k] - 1][current_mapping[i] - 1]);
            }
            // cout<<((neighbour_cost==neighbour_cost_fn(newneighbour))? "Yes":"No")<<endl;
            if (neighbour_cost < opt_val)
            {
                opt_val = neighbour_cost;
                for (int i = 0; i < z; i++)
                    optneighbour[i] = newneighbour[i];
                bt = false;
                break;
            }
            newneighbour[k] = current_mapping[k];
        }
        if (!bt)
            break;
    }
    if (!bt)
    {
        int *optneighbourheap = new int[z];
        for (int i = 0; i < z; i++)
            optneighbourheap[i] = optneighbour[i];

        // GIVE COST BY USING A POINTER
        *opt = opt_val;
        return optneighbourheap;
    }
    else
    {
        // Now checking for swaps
        // Intermediate hybrid!
        for (int i = 0; i < z; i++)
            newneighbour[i] = current_mapping[i];

        int intermediateneighbour[z];
        bool b = true;
        for (int i = 0; i < z; i++)
            intermediateneighbour[i] = current_mapping[i];

        for (int k = 0; k < z; k++)
        {
            for (int l = k + 1; l < z; l++)
            {
                intermediateneighbour[k] = current_mapping[l];
                newneighbour[k] = current_mapping[l];
                newneighbour[l] = current_mapping[k];
                long long neighbour_cost = current_cost;
                for (int i = 0; i < z; i++)
                {
                    neighbour_cost += (long long)N[i][k] * ((long long)T[intermediateneighbour[i] - 1][intermediateneighbour[k] - 1] - (long long)T[current_mapping[i] - 1][current_mapping[k] - 1]) + (long long)N[k][i] * ((long long)T[intermediateneighbour[k] - 1][intermediateneighbour[i] - 1] - (long long)T[current_mapping[k] - 1][current_mapping[i] - 1]);
                    neighbour_cost += (long long)N[i][l] * ((long long)T[newneighbour[i] - 1][newneighbour[l] - 1] - (long long)T[intermediateneighbour[i] - 1][intermediateneighbour[l] - 1]) + (long long)N[l][i] * ((long long)T[newneighbour[l] - 1][newneighbour[i] - 1] - (long long)T[intermediateneighbour[l] - 1][intermediateneighbour[i] - 1]);
                }
                // cout<<((neighbour_cost==neighbour_cost_fn(newneighbour))? "Yes":"No")<<endl;
                if (neighbour_cost < opt_val)
                {
                    opt_val = neighbour_cost;
                    for (int i = 0; i < z; i++)
                        optneighbour[i] = newneighbour[i];
                    b = false;
                    break;
                }
                intermediateneighbour[k] = current_mapping[k];
                newneighbour[k] = current_mapping[k];
                newneighbour[l] = current_mapping[l];
            }
            if (!b)
                break;
        }

        // TRANSFER OPTNEIGHBOUR TO HEAP
        int *optneighbourheap = new int[z];
        for (int i = 0; i < z; i++)
            optneighbourheap[i] = optneighbour[i];

        // GIVE COST BY USING A POINTER
        *opt = opt_val;
        return optneighbourheap;
    }
}

int *SportsLayout::best_neighbour_first(int *current_mapping, int max_iterations, long long *value, timetype initialTime, int *lm) // changed old_mapping to current_mapping
{
    // int *current_mapping = old_mapping;
    long long current_value = neighbour_cost_fn(current_mapping);

    for (int i = 0; i < max_iterations; i++)
    // while (1)
    {
        long long best_neighbour_value;
        // cout<<"I am here"<<endl;
        int *best_neighbour = SportsLayout::best_first_neighbour(current_mapping, &best_neighbour_value, current_value);

        if (best_neighbour == nullptr)
        {
            // cout<<"HOW CAN I BE HERE?"<<endl;
            // Handle allocation failure, e.g., by exiting the loop or returning an error value.
            break;
        }

        if (best_neighbour_value >= current_value)
        {
            // cout<<"HERE"<<endl;
            *lm = 0;
            delete[] best_neighbour; // Don't forget to free the memory
            break;
        }
        // cout<<"HERE2"<<endl;
        *lm = 1;
        // delete[] current_mapping; // Use delete[] for arrays
        current_mapping = best_neighbour;
        current_value = best_neighbour_value;
        auto d = duration(now(), initialTime);
        int kl;
        if (time>2){kl=20;}
        else{
            kl=10;
        }
        if (int(d.count()) / 100000 >= (600.0 * time - kl)) break;
    }

    // cout << current_value << endl;
    *value = current_value;
    return current_mapping;
}

int *SportsLayout::best_neighbour_first_with_restarts(int max_iterations)
{
    int *best_mapping;
    long long best_value = __LONG_LONG_MAX__;

    auto initialTime = now();
    int maxtimegap = 0;
    int oldtime = 0;
    int numrestarts = 0;
    int *current_mapping = random_mapping();
    int lm = 0;
    long long value=0;
    while (1)
    {
        if (lm == 0 || (0.5*value>best_value))
            {current_mapping = random_mapping();}


        // f(z) cout<<current_mapping[i];
        // cout<<endl;
        // cout<<neighbour_cost_fn(current_mapping)<<endl;
        int *mapping = best_neighbour_first(current_mapping, max_iterations, &value, initialTime, &lm);
        if (value < best_value)
        {
            // delete[] best_mapping;
            best_mapping = mapping;
            current_mapping = mapping;
            best_value = value;
        }
        cout << "Value"<< value<<endl;
        // cout << (long long)best_value << endl;
        if (neighbour_cost_fn(current_mapping)>value) current_mapping = mapping;
        auto finalTime = now();
        auto d = duration(finalTime, initialTime);
        maxtimegap = std::max(maxtimegap, int(d.count()) - oldtime);
        oldtime = d.count();
        int kl;
        if (time>2){kl=20;}
        else{
            kl=10;
        }
        if ((double(d.count()) / 100000.0) >= (600.0 * time - maxtimegap / 100000.0 - kl))
        {
            // cout << "Number of restarts: " << numrestarts << endl;
            break;
        }
        numrestarts++;
    }
    int *best_mapping_heap = new int[z];
    f(z) best_mapping_heap[i] = best_mapping[i];
    return best_mapping_heap;
}

void SportsLayout::compute_allocation()
{
    auto initialTime = now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(initialTime);

    if (l>250 && z>250)
    mapping = best_neighbour_first_with_restarts(200);
    else
    mapping = random_restart_greedy_hill_climbing(pow(10, 4));
    // std::cout << std::endl;
    auto finalTime = now();
}