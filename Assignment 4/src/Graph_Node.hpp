#ifndef GRAPH_NODE_HPP
#define GRAPH_NODE_HPP
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <iomanip>

class Graph_Node
{

public:
    std::string Node_Name;            // Variable name
    std::vector<int> Children;        // Children of a particular node - these are index of nodes in graph.
    std::vector<std::string> Parents; // Parents of a particular node- note these are names of parents
    std::vector<int> Parents_ind;     // Indices of parents
    std::vector<int> Parents_nvalues; // Parent_nvalues
    int nvalues;                      // Number of categories a variable represented by this node can take
    std::vector<std::string> values;  // Categories of possible values
    std::vector<float> CPT;          // conditional probability table as a 1-d array . Look for BIF format to understand its meaning
    std::vector<int> countsTable;
    int myIndex;

    Graph_Node(std::string name, int n, std::vector<std::string> vals, int count);
    std::string get_name();
    std::vector<int> get_children();
    std::vector<std::string> get_Parents();
    std::vector<float> get_CPT();
    int get_nvalues();
    std::vector<std::string> get_values();
    void set_CPT(std::vector<float> new_CPT);
    void set_Table(std::vector<int> new_Table);
    void set_Parents(std::vector<std::string> Parent_Nodes);
    int add_child(int new_child_index);
    
};

Graph_Node::Graph_Node(std::string name, int n, std::vector<std::string> vals, int count)
{
    Node_Name = name;
    nvalues = n;
    values = vals;
    myIndex=count;
}
std::string Graph_Node::get_name()
{
    return Node_Name;
}
std::vector<int> Graph_Node::get_children()
{
    return Children;
}
std::vector<std::string> Graph_Node::get_Parents()
{
    return Parents;
}
std::vector<float> Graph_Node::get_CPT()
{
    return CPT;
}
int Graph_Node::get_nvalues()
{
    return nvalues;
}
std::vector<std::string> Graph_Node::get_values()
{
    return values;
}
void Graph_Node::set_CPT(std::vector<float> new_CPT)
{
    CPT.clear();
    CPT = new_CPT;
}
void Graph_Node::set_Table(std::vector<int> new_Table)
{
    countsTable.clear();
    for (auto el: new_Table) countsTable.push_back(el);
}
void Graph_Node::set_Parents(std::vector<std::string> Parent_Nodes)
{
    Parents.clear();
    Parents = Parent_Nodes;
}
// add another node in a graph as a child of this node
int Graph_Node::add_child(int new_child_index)
{
    for (int i = 0; i < Children.size(); i++)
    {
        if (Children[i] == new_child_index)
            return 0;
    }
    Children.push_back(new_child_index);
    return 1;
}


#endif