#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void readFile2(string filename, int *n, int *m, int ***G)
{
    ifstream graph;
    graph.open(filename);
    if (!graph.is_open())
    {
        cerr << "Failed to open " << filename << endl;
    }

    string line;
    graph >> *n >> *m;

    int **adjacencyMatrix = new int *[*n];
    for (int i = 0; i < *n; i++)
    {
        adjacencyMatrix[i] = new int[*n]{0};
    }

    for (int i = 0; i < *m; i++)
    {
        int u, v;
        graph >> u >> v;

        // Indexing starts from 0
        u--;
        v--;

        adjacencyMatrix[u][v] = 1;
        adjacencyMatrix[v][u] = 1;
    }

    graph.close();

    *G = adjacencyMatrix;
}

void writeFile2(vector<vector<int>> cs, string filename, int numvar)
{
    ofstream output;
    output.open(filename);
    output << "p cnf " << numvar << " " << cs.size() << endl;
    for (const auto &clause : cs)
    {
        for (int literal : clause)
        {
            output << literal << " ";
        }
        output << "0" << endl;
    }
    output.close();
}

vector<int> readSATOut(string filename, bool *sat, int num)
{
    ifstream SAT;
    SAT.open(filename);
    string success;
    SAT >> success;
    *sat = (success == "SAT") ? 1 : 0;
    vector<int> output;
    if (SAT)
    {
        for (int i = 0; i < num; i++)
        {
            int x;
            SAT >> x;
            output.push_back(x);
        }
    }
    return output;
}

void writeoutput(string filename, string content){
    ofstream finaloutput;
    finaloutput.open(filename);
    finaloutput << content;
    finaloutput.close();
}