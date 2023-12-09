#include "AlarmNetwork.hpp"

int main(int argv, const char *argc[]){
    clock_t begin = clock();

    AlarmNetwork alarmNetwork = AlarmNetwork();
    std::string bifFile = argc[1];
    std::string dataFile = argc[2];

    alarmNetwork.readNetwork(bifFile);
    
    alarmNetwork.Alarm.initialize_CPTs();
    alarmNetwork.readData(dataFile);

    // std::cout<< "Before Counts initialize"<<std::endl;
    // alarmNetwork.initCountsTable();
    // std::cout<< "Counts initialized"<<std::endl;
    alarmNetwork.EM();
    
    alarmNetwork.write_network();
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time taken: " << elapsed_secs << " seconds" << std::endl;
    return 0;
}