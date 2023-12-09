#include <iostream>
#include <vector>


using namespace std;

int S(int i, int j, int n, int k, int base) {
    return base+(n-k+1)*(j-1)+(i-j);
}

vector<vector<int>> solve2(int n, int k, int ** G) {
    vector<vector<int>> cs;
    
    // Both can't be in a clique if no edge
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (G[i][j]==0) {
                vector<int> clause;
                clause.push_back(-(i+1));
                clause.push_back(-(j+1));
                cs.push_back(clause);
            }
        }
    }

    int base= n+1;
    
    // Cardinality Constraint
    // Note: S_{n,k} === S_{i,j} Here j will index the row and i the column 

    cs.push_back({S(n,k,n,k,base)});
    for (int j = 1; j <= k; ++j) {
        for (int i = j; i <= n - k + j; ++i) {
            
            if (i == j && j == 1) {
                // S_{1,1} => x_1
                cs.push_back({-S(1, 1, n, k, base), 1});
                // x_1 => S_{1,1}
                cs.push_back({-1, S(1, 1, n, k, base)});
            } else if (i == j) {
                // S_{i,i} => (x_i and S_{i-1,i-1})
                cs.push_back({-S(i, i, n, k, base), i});
                cs.push_back({-S(i, i, n, k, base), S(i - 1, i - 1, n, k, base)});
                // x_i and S_{i-1,i-1} => S_{i,i}
                cs.push_back({-i, -S(i - 1, i - 1, n, k, base), S(i, i, n, k, base)});
            }else if (j == 1) {
                // S_{i,1} => (x_i or S_{i-1,1})
                cs.push_back({-S(i, 1, n, k, base), i, S(i-1,1,n,k,base)});
                // (x_i or S_{i-1,1}) => S_{i,1}
                cs.push_back({-i, S(i, 1, n, k, base)});
                cs.push_back({-S(i-1,1,n,k,base), S(i, 1, n, k, base)});
                
            } else {
                // S_{i,j} => S_{i-1,j} or (S_{i-1,j-1} and x_i)
                cs.push_back({-S(i, j, n, k, base), S(i - 1, j, n, k, base), S(i - 1, j - 1, n, k, base)});
                cs.push_back({-S(i, j, n, k, base), S(i - 1, j, n, k, base), i});
                // S_{i-1,j} or (S_{i-1,j-1} and x_i) => S_{i,j}
                cs.push_back({-S(i - 1, j, n, k, base), S(i, j, n, k, base)});
                cs.push_back({-S(i - 1, j - 1, n, k, base), -i, S(i, j, n, k, base)});
            }
        }
    }
    
    return cs;
}
