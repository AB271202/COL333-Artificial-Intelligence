#ifndef ALARMNETWORK_HPP
#define ALARMNETWORK_HPP

#include "Graph_Node.hpp"
#include "network.hpp"

class AlarmNetwork
// maintains all the functions which we have to use for alarm network
{
public:
    std::ofstream outfile;
    network Alarm;

    // Save searching time
    std::unordered_map<std::string, int> Names_to_index;
    std::vector<std::unordered_map<std::string, int>> Values_to_index;
    std::vector<int> variable_nvalues;

    std::vector<std::vector<int>> dataset; // see whether it is string or int with interpretation given to each string  {true -> 1 , mdium -> 5 etc.}
    std::vector<std::pair<int, std::vector<float>>> missingValue;

    // constructor : makes the whole graph
    AlarmNetwork();
    // read the bif file
    void readNetwork(std::string bifFile);
    // read the data file
    void readData(std::string dataFile);
    // initialize the parents
    void initCountsTable();
    // esimate the probabilities
    void normalize2(std::vector<float> &newCPT, int nvalues, std::vector<int> &parents);

    //-----------EM functions----------------
    bool E_step();
    // void M_step();
    float M_step();
    void EM();

    void write_network();
};
template <class T>
void disp(std::vector<T> v)
{
    for (auto i : v)
        std::cout << i << " ";
    std::cout << std::endl;
}

AlarmNetwork::AlarmNetwork()
{
    outfile.open("solved_alarm.bif");
}

void AlarmNetwork::readNetwork(std::string bifFile)
{
    std::string line, temp, name;
    int find = 0, r = 0;
    std::ifstream infile(bifFile);
    std::vector<std::string> values;
    int count = 0;
    if (infile.is_open())
    {
        while (!infile.eof())
        {
            std::stringstream ss;
            std::getline(infile, line);
            ss.str(line);
            ss >> temp;

            if (r == 0)
                outfile << line << "\n";

            if (temp.compare("variable") == 0)
            {
                ss >> name;
                this->Names_to_index[name] = count;
                getline(infile, line);
                outfile << line << "\n";
                count++;

                std::stringstream ss2;
                ss2.str(line);
                for (int i = 0; i < 4; i++)
                    ss2 >> temp;

                values.clear();
                std::unordered_map<std::string, int> valueToIndex;
                int index = 0;
                while (temp.compare("};") != 0)
                {
                    values.push_back(temp);
                    valueToIndex[temp] = index;
                    index++;
                    ss2 >> temp;
                }
                this->Values_to_index.push_back(valueToIndex);
                Graph_Node newNode = Graph_Node(name, values.size(), values, count - 1);
                this->Alarm.addNode(newNode);
            }
            else if (temp.compare("probability") == 0)
            {
                if (r == 0)
                {
                    r = 1;
                }
                ss >> temp;
                ss >> temp;
                std::list<Graph_Node>::iterator listIt;
                std::list<Graph_Node>::iterator listIt1;
                listIt = Alarm.search_node(temp);
                int index = Alarm.get_index(temp);
                ss >> temp;
                values.clear();
                while (temp.compare(")") != 0)
                {
                    listIt1 = Alarm.search_node(temp);
                    listIt1->add_child(index);
                    values.push_back(temp);
                    ss >> temp;
                }
                listIt->set_Parents(values);
                getline(infile, line);

                std::stringstream ss2;
                ss2.str(line);
                ss2 >> temp;
                ss2 >> temp;
                std::vector<float> curr_CPT;
                std::string::size_type sz;
                while (temp.compare(";") != 0)
                {
                    curr_CPT.push_back(atof(temp.c_str()));
                    ss2 >> temp;
                }
                listIt->set_CPT(curr_CPT);
            }
            else
            {
            }
        }
        if (find == 1)
            infile.close();
    }
}

void AlarmNetwork::readData(std::string dataFile)
{
    std::ifstream infile(dataFile);
    std::string line;
    std::vector<int> translatedData;
    bool hasMissingValue = false;
    while (!infile.eof())
    {
        hasMissingValue = false;
        translatedData.clear();
        for (int i = 0; i < this->Alarm.netSize(); i++)
        {
            infile >> line;
            if (line.compare("\"?\"") == 0)
            {
                this->missingValue.push_back(std::make_pair(i, std::vector<float>{0.0f}));
                hasMissingValue = true;
                translatedData.push_back(-1);
            }
            else
            {
                translatedData.push_back(this->Values_to_index[i][line]);
            }
        }
        if (!hasMissingValue)
            this->missingValue.push_back(std::make_pair(-1, std::vector<float>{0.0f}));

        this->dataset.push_back(translatedData);
    }
    // for(auto row: this->dataset) disp(row);
    for (auto it = this->Alarm.Pres_Graph.begin(); it != this->Alarm.Pres_Graph.end(); it++)
    {
        for (int i = 0; i < it->get_Parents().size(); i++)
        {
            it->Parents_ind.push_back(this->Names_to_index[it->Parents[i]]);
            it->Parents_nvalues.push_back((this->Alarm.get_nth_node(this->Names_to_index[it->Parents[i]]))->nvalues);
        }
        this->variable_nvalues.push_back(it->nvalues);
    }
}

int get_cpt_idx(std::list<Graph_Node>::iterator node, int nodeValue, std::vector<int> ParentValues)
{
    // auto parents = node->get_Parents_int();
    auto parents_nvalues = node->Parents_nvalues;
    int idx = 0;
    for (int i = 0; i < ParentValues.size(); i++)
    {
        idx = idx * parents_nvalues[i] + ParentValues[i];
    }
    return idx + node->get_CPT().size() / node->get_nvalues() * nodeValue;
}

void AlarmNetwork::initCountsTable()
{
    // auto node = this->Alarm.Pres_Graph.begin();
    for (auto node = this->Alarm.Pres_Graph.begin(); node != this->Alarm.Pres_Graph.end(); node++)
    {
        auto parents = node->Parents_ind;
        std::vector<int> temp_Table(node->CPT.size(), 0);
        for (int i = 0; i < this->dataset.size(); i++)
        {
            auto row = this->dataset[i];
            // std::cout<<"Got row"<<std::endl;
            std::vector<int> parent_values;
            parent_values.clear();
            int question_mark = (node->myIndex == missingValue[i].first) ? node->myIndex : -1;
            for (auto index : parents)
            {
                if (index == missingValue[i].first)
                    question_mark = index;
                parent_values.push_back(row[index]);
            }
            // std::cout<<"Got parent values"<<std::endl;
            if (question_mark != -1)
                continue;
            int idx = get_cpt_idx(node, row[node->myIndex], parent_values);
            std::cout << idx;
            temp_Table[idx] += 1;
            // std::cout << "Updated temptable " << i << std::endl;
        }
        node->set_Table(temp_Table);
    }
}

void AlarmNetwork::normalize2(std::vector<float> &newCPT, int nvalues, std::vector<int> &parents)
{
    int cols = newCPT.size() / nvalues;
    std::vector<bool> reliableCols;
    // basic normalization with smoothing
    int i=0;
    while ( i < cols )
    {
        float sum = 0.0f;
        // float smoothing_factor = ((sum) / (nvalues)) * 0.04f;
        float smoothing_factor = 0;
        bool zeroflag = false;
        for (int j = 0; j < nvalues; j++)
        {
            sum += newCPT[i + j * cols];
            zeroflag = zeroflag | (newCPT[i + j * cols]==0);
        }
        if (sum == 0.0f)
            smoothing_factor = 1.0f;
        else if (zeroflag)
            smoothing_factor = ((sum) / (nvalues)) * 0.04f;
            // smoothing_factor =( sum/(nvalues*3.0f))*0.01f;
        for (int j = 0; j < nvalues; j++)
        {
            newCPT[i+j*cols]=(newCPT[i+j*cols]+ smoothing_factor)/(sum+nvalues*(smoothing_factor));
            // newCPT[i + j * cols] = (newCPT[i + j * cols] + 0.01f) / (sum + nvalues * (0.02f));
        }
        if (sum == 0.0f)
            reliableCols.push_back(false);
        else
            reliableCols.push_back(true);
        i++;
    }

    for (int i = 0; i < reliableCols.size(); i++)
    {
        // apply some averaging with respect to parents and its dependence
        if (!reliableCols[i])
        {
            int possible_values = 1;
            std::vector<int> parent_values;
            std::vector<int> rel_cols;
            int cpy_i = i;
            for (auto parent : parents)
            {
                possible_values *= this->variable_nvalues[parent];
            }
            int possible_values_cpy = possible_values;
            for (auto parent : parents)
            {
                possible_values_cpy /= this->variable_nvalues[parent];
                parent_values.push_back(cpy_i / possible_values_cpy);
                cpy_i %= possible_values_cpy;
            }
            for (int j = 0; j < parents.size(); j++)
            {
                for (int k = 0; k < variable_nvalues[parents[j]]; k++)
                {
                    if (k != parent_values[j])
                    {
                        int bkup = parent_values[j];
                        parent_values[j] = k;
                        int temp = 0;
                        possible_values_cpy = possible_values;
                        for (int l = 0; l < parents.size(); l++)
                        {
                            possible_values_cpy /= this->variable_nvalues[parents[l]];
                            temp += parent_values[l] * possible_values_cpy;
                        }
                        if (reliableCols[temp])
                            rel_cols.push_back(temp);
                        parent_values[j] = bkup;
                    }
                }
            }
            for (int j = 0; j < nvalues; j++)
            {
                float avg = 0.0f;
                for (auto r : rel_cols)
                    avg += newCPT[r + j * cols];
                newCPT[i + j * cols] = avg / (rel_cols.size());
            }
        }
    }

}

// debugging
//  sum=0.0f;
//  for (int j=0; j<nvalues; j++){
//      sum+=newCPT[i+j*cols];
//  }
//  std::cout<<"Sum of new CPT values "<<sum<<std::endl;

float to_CPT(std::list<Graph_Node>::iterator node, int nodeValue, std::vector<int> ParentValues)
{
    auto cpt = node->get_CPT();
    return cpt[get_cpt_idx(node, nodeValue, ParentValues)];
}

std::vector<int> get_parent_values(std::list<Graph_Node>::iterator node, std::vector<int> row, int missing_value)
{
    std::vector<int> parent_values;
    for (auto idx : node->Parents_ind)
    {
        auto x = row[idx];
        if (x != -1)
            parent_values.push_back(x);
        else
            parent_values.push_back(missing_value);
    }
    return parent_values;
}

bool AlarmNetwork::E_step()
{
    bool hasChanged = false;
    for (int i = 0; i < this->dataset.size(); i++)
    {
        if (this->missingValue[i].first == -1)
            continue;
        auto row = this->dataset[i];
        auto node = this->Alarm.get_nth_node(this->missingValue[i].first);
        std::vector<float> probs;
        float sum = 0.0f;
        probs.clear();
        float maxProb = -1.0f;
        int maxProbIndex = -1;
        // for (auto value: node->get_values()){
        for (int j = 0; j < node->get_nvalues(); j++)
        {
            float ans = to_CPT(node, j, get_parent_values(node, row, -1));
            for (auto child : node->get_children())
            {
                auto child_node = this->Alarm.get_nth_node(child);
                auto x = to_CPT(child_node, row[child], get_parent_values(child_node, row, j));
                ans *= x;
            }
            if (maxProb < ans)
            {
                maxProb = ans;
                maxProbIndex = j;
            }
            probs.push_back(ans);
            sum += ans;
        }
        if (this->dataset[i][this->missingValue[i].first] != maxProbIndex)
            hasChanged = true;
        this->dataset[i][this->missingValue[i].first] = maxProbIndex;

        // Normalize
        for (int k = 0; k < probs.size(); k++)
            probs[k] /= sum;

        this->missingValue[i].second.clear();
        for (auto el : probs)
            this->missingValue[i].second.push_back(el);
        // for(auto el:this->missingValue[i].second) std::cout<<el<<" ";
        // std::cout<<std::endl;
    }
    return hasChanged;
}

// void AlarmNetwork::M_step()
float AlarmNetwork::M_step()
{
    // Will take about O(dn) time for this step, going through the database for each node
    for (auto node_it = this->Alarm.Pres_Graph.begin(); node_it != this->Alarm.Pres_Graph.end(); node_it++)
    {
        int cptSize = node_it->CPT.size();
        std::vector<int> parents = node_it->Parents_ind;
        std::vector<float> newCPT(cptSize, 0.0f);
        for (int i = 0; i < this->dataset.size(); i++)
        {
            int temp = 0;
            std::vector<int> parents_values;
            for (int j = 0; j < parents.size(); j++)
                parents_values.push_back(dataset[i][parents[j]]);
            temp = get_cpt_idx(node_it, dataset[i][node_it->myIndex], parents_values);
            newCPT[temp]++;
        }

        // normalize(normal) this CPT
        normalize2(newCPT, node_it->nvalues, node_it->Parents_ind);
        node_it->set_CPT(newCPT);
    }
    return 0.0f;
        // Will take about O(dn) time for this step, going through the database for each node
        // float net_difference=0;
        // for (auto node = this->Alarm.Pres_Graph.begin(); node != this->Alarm.Pres_Graph.end(); node++)
        // {
        //     std::vector<float> tempTable;
        //     tempTable.clear();
        //     for(auto something: node->countsTable) tempTable.push_back((float) something);
        //     auto parents = node->Parents_ind; // This will have the indices of the parents
        //     for(int i=0; i<this->dataset.size();i++){
        //         auto row = this->dataset[i];
        //         std::vector<int> parent_values;
        //         parent_values.clear();
        //         int question_mark=(node->myIndex==missingValue[i].first) ? node->myIndex:-1;
        //         int parent_index=-1;
        //         for (int index=0; index<parents.size(); index++) {
        //             if (parents[index]==missingValue[i].first) {
        //                 question_mark=parents[index];
        //                 parent_index=index;
        //             }
        //             parent_values.push_back(row[parents[index]]);
        //         }
        //         // std::cout<<"Node name"<<node->get_name()<<row[node->myIndex]<<std::endl;
        //         // std::cout<<"Parent values: ";
        //         // disp(parent_values);
        //         // auto idx = get_cpt_idx(node,row[node->myIndex] ,parent_values);
        //         // std::cout<<"IDX got successfully:"<<idx<<std::endl;
        //         // tempTable2[idx]++;
        //         if (question_mark==-1) continue;
        //         if (question_mark==node->myIndex){
        //             for(int index=0; index<node->get_nvalues(); index++){
        //                 tempTable[get_cpt_idx(node, index, parent_values)] += this->missingValue[i].second[index];
        //             }
        //         }
        //         else{
        //             for(int index=0; index<node->get_nvalues(); index++){
        //                 for(int pindex=0; pindex<node->Parents_nvalues[parent_index]; pindex++){
        //                     parent_values[parent_index]=pindex;
        //                     tempTable[get_cpt_idx(node, index, parent_values)] += this->missingValue[i].second[pindex];
        //                 }
        //             }
        //          }
        //     }
        //     // Take tempTable to the CPT by normalizing
        //     normalize2(tempTable, node->nvalues, node->Parents_ind);
        //     int m = (node->get_CPT().size())/(node->get_nvalues());
        //     for (int i=0; i<m; i++){
        //         float sum = 0.0f;
        //         for(int j=0; j<node->get_nvalues(); j++){sum+=tempTable[m*j+i];}
        //         for(int j=0; j<node->get_nvalues(); j++){tempTable[m*j+i]/=sum;}
        //     }
        //     float difference=0;
        //     for(int k=0; k<tempTable.size(); k++) difference+=abs(tempTable[k]-node->CPT[k]);
        //     net_difference+=difference;
        //     node->set_CPT(tempTable);
        // }
        // return net_difference;
    
}

void AlarmNetwork::EM()
{
    bool hasChanged = true;
    clock_t begin = clock();
    int iter = 0;
    while (hasChanged)
    {
        clock_t currTime = clock();
        double timeElapsed = double(currTime - begin) / CLOCKS_PER_SEC;
        // std::cout<<"Iteration "<<iter<<" Time Elapsed "<<timeElapsed<< " hasChanged "<< hasChanged<<"\n";
        // std::cout << "Iteration " << iter << " Time Elapsed " << timeElapsed << "\n";
        if (timeElapsed > 110)
            break;
        hasChanged = hasChanged && this->E_step();
        // this->E_step();
        float cur_diff=this->M_step();
        // std::cout<<"Difference of CPTs:"<<cur_diff<<std::endl;
        // if (cur_diff<0.0001) break;
        iter++;
    }
}

void AlarmNetwork::write_network()
{
    std::list<Graph_Node>::iterator listIt;
    int u = 0;
    for (listIt = Alarm.Pres_Graph.begin(); listIt != Alarm.Pres_Graph.end(); listIt++)
    {
        int sz = listIt->CPT.size();
        std::string name = listIt->Node_Name;

        if (u == 1)
        {
            outfile << "\nprobability (  " << name;
            for (int i = 0; i < listIt->Parents.size(); i++)
            {
                outfile << "  " << listIt->Parents[i];
            }
            outfile << " ) { //" << listIt->Parents.size() + 1 << " variable(s) and " << sz << " values\n";
        }
        u = 1;
        outfile << "\ttable ";
        for (int i = 0; i < listIt->CPT.size(); i++)
        {
            outfile << std::fixed << std::setprecision(4) << listIt->CPT[i] << " ";
        }
        outfile << ";\n";
        outfile << "}";
    }
}

#endif