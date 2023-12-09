#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>

#include "encoding2.cpp"
#include "fileIO2.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    // Get the Graph
    int n, m;
    int **adjacencyMatrix = nullptr;

    string filename = argv[1];

    // Passing as pointers
    readFile2(filename + ".graph", &n, &m, &adjacencyMatrix);

    // Binary Search

    int l = 1, r = min(int((1+sqrt(8*m+1))/2) + 1,n), clique = 1;
    cout<< r <<endl;
    bool sat;
    vector<int> answer;

    while (l <= r)
    {
        int k = (l + r) / 2;
        cout << "Trying k=" << k << endl;
        // Solve
        vector<vector<int>> cs = solve2(n, k, adjacencyMatrix);
        // Write to file
        int numvar = n+k*(n-k+1);
        writeFile2(cs, "minisat.in", numvar);
        // Run minisat
        int result = system("./minisat minisat.in minisat.out");
        // Get the result
        vector<int> satresult = readSATOut("minisat.out", &sat, numvar);

        if (sat)
        {
            clique = k;
            answer = satresult;
            l = k + 1;
        }
        else
        {
            r = k - 1;
        }
    }

    string finaloutput;
    if (clique > 1)
    {
        finaloutput= "#1\n";
        for (int thing = 0; thing < n; thing++)
        {
            if (answer[thing] > 0)
                finaloutput+=to_string(answer[thing])+" ";
        }
        finaloutput.erase(finaloutput.size() - 1);
    }
    else
    {
        finaloutput="#1\n1";
    }
    writeoutput(filename+".mapping", finaloutput);
}