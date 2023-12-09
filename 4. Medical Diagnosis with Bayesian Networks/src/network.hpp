#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "Graph_Node.hpp"

class network
{
public:
    std::list<Graph_Node> Pres_Graph;
    int addNode(Graph_Node node);
    int netSize();
    int get_index(std::string val_name);
    std::list<Graph_Node>::iterator get_nth_node(int n);
    std::list<Graph_Node>::iterator search_node(std::string val_name);
    void initialize_CPTs();
};

int network::addNode(Graph_Node node)
{
    Pres_Graph.push_back(node);
    return 0;
}

int network::netSize()
{
    return Pres_Graph.size();
}
// get the index of node with a given name
int network::get_index(std::string val_name)
{
    std::list<Graph_Node>::iterator listIt;
    int count = 0;
    for (listIt = Pres_Graph.begin(); listIt != Pres_Graph.end(); listIt++)
    {
        if (listIt->get_name().compare(val_name) == 0)
            return count;
        count++;
    }
    return -1;
}
// get the node at nth index
std::list<Graph_Node>::iterator network::get_nth_node(int n)
{
    std::list<Graph_Node>::iterator listIt;
    int count = 0;
    for (listIt = Pres_Graph.begin(); listIt != Pres_Graph.end(); listIt++)
    {
        if (count == n)
            return listIt;
        count++;
    }
    return listIt;
}
// get the iterator of a node with a given name
std::list<Graph_Node>::iterator network::search_node(std::string val_name)
{
    std::list<Graph_Node>::iterator listIt;
    for (listIt = Pres_Graph.begin(); listIt != Pres_Graph.end(); listIt++)
    {
        if (listIt->get_name().compare(val_name) == 0)
            return listIt;
    }

    std::cout << "node not found\n";
    return listIt;
}

void network::initialize_CPTs()
{
    std::list<Graph_Node>::iterator it;
    for (it = Pres_Graph.begin(); it != Pres_Graph.end(); it++)
    {
        int nvalues = it->nvalues;
        std::vector<float> CPT = it->get_CPT();
        for (int i = 0; i < CPT.size(); i++)
        {
            CPT[i] = (float)rand() / RAND_MAX;
        }
        //normalize
        float sum = 0.0f;
        int cols = CPT.size() / nvalues;
        for (int i = 0; i < cols; i++)
        {
            sum = 0.0f;
            for (int j = 0; j < nvalues; j++)
            {
                sum += CPT[i + j * cols];
            }
            for (int j = 0; j < nvalues; j++)
            {
                CPT[i + j * cols] /= sum;
            }
        }
        it->set_CPT(CPT);
    }
}


#endif