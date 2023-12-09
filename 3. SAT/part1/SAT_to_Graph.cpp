#include <iostream>
#include <vector>
#include <fstream>

#include "fileIO.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    string filename = argv[1];
    bool sat;
    int k1, k2;
    int numvar = 2*getN(filename+".graph", &k1, &k2 ); 
    vector<int> satresult = readSATOut(filename+".satoutput", &sat, numvar);
    
    string finaloutput;
    if (sat){
        finaloutput="#1\n";
        for(int i=0; i<numvar/2; i++){
            
            if (satresult[i]>0) {k1--;finaloutput+=to_string(satresult[i])+" ";}
            if (k1==0) break;
        }
        // Remove last space
        finaloutput.erase(finaloutput.size() - 1);
        finaloutput+="\n#2\n";
        for(int i=numvar/2;i<numvar; i++){
            
            if (satresult[i]>0) {k2--;finaloutput+=to_string(satresult[i]-numvar/2)+" ";}
            if (k2==0) break;
        }
        finaloutput.erase(finaloutput.size() - 1);
        
    }
    else{
        finaloutput="0";
    }
    writeoutput(filename+".mapping", finaloutput);
}
