#include <iostream>
#include <fstream>

#include "encoding.cpp"
#include "fileIO.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    // Get the Graph
    int n, m, k1, k2;
    int **adjacencyMatrix = nullptr;

    string filename = argv[1];
    

    // Passing as pointers
    readFile((filename + ".graph"), &n, &m, &k1, &k2, &adjacencyMatrix);

    // Solve and write to file
    int numvar=2*n+k1*(n-k1+1)+k2*(n-k2+1);
    writeFile(solve(n,k1,k2,adjacencyMatrix), (filename+".satinput"),numvar);
}
