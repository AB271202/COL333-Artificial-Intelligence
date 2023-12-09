#include <iostream>
#include <bits/stdc++.h>
#include <fstream>

#include "SportsLayout.h"

using namespace std;

int main(int argc, char** argv )
{

     // Parse the input.
    if ( argc < 3 )
    {   
        cout<<"Missing arguments\n";
        cout<<"Correct format : \n";
        cout << "./main <input_filename> <output_filename>";
        exit ( 0 );
    }
    string inputfilename ( argv[1] );
    string outputfilename ( argv[2] );
    
    SportsLayout *s  = new SportsLayout( inputfilename );
    
    // int state[] = {4,2,3};
    // long long *opt;
    // int current_cost= s->neighbour_cost_fn(state);
    // cout<<"InitialCost "<<current_cost<<endl;
    // int* nbr = s->opt_neighbour(state, opt, current_cost);
    // for(int i=0; i<3; i++){cout<<nbr[i]<<endl;}
    // cout<<*opt<<endl;
    s->compute_allocation();
    s->write_to_file(outputfilename);

    long long cost = s->cost_fn ();
    cout<< "cost:"<<cost<<endl;


    return 0;

}