#include <iostream>
#include <vector>


using namespace std;

int S(int i, int j, int n, int k, int base) {
    return base+(n-k+1)*(j-1)+(i-j);
}

vector<vector<int>> solve(int n, int k1, int k2, int ** G) {
    vector<vector<int>> cs;
    
    // Both vertices can't be in a clique if no edge
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (G[i][j]==0) {
                cs.push_back({-(i+1), -(j+1)}); // First clique
                cs.push_back({-(n+i+1), -(n+j+1)}); // Second clique
            }
        }
    }


    // A vertex cannot be in first as well as the second clique
    for (int i=0; i<n; i++){
        cs.push_back({-(i+1),-(n+i+1)});
    }


    int base= 2*n+1;
    int var_base=0;
    // Cardinality Constraint
    // NOTE: S_{n,k} === S_{i,j} Here j will index the row and i the column 
    // First Clique
    cs.push_back({S(n,k1,n,k1,base)});
    for (int j = 1; j <= k1; ++j) {
        for (int i = j; i <= n - k1 + j; ++i) {
            
            if (i == j && j == 1) {
                // S_{1,1} => x_1
                cs.push_back({-S(1, 1, n, k1, base), 1+var_base});
                // x_1 => S_{1,1}
                cs.push_back({-(1+var_base), S(1, 1, n, k1, base)});
            } else if (i == j) {
                // S_{i,i} => (x_i and S_{i-1,i-1})
                cs.push_back({-S(i, i, n, k1, base), i+var_base});
                cs.push_back({-S(i, i, n, k1, base), S(i - 1, i - 1, n, k1, base)});
                // x_i and S_{i-1,i-1} => S_{i,i}
                cs.push_back({-(i+var_base), -S(i - 1, i - 1, n, k1, base), S(i, i, n, k1, base)});
            }else if (j == 1) {
                // S_{i,1} => (x_i or S_{i-1,1})
                cs.push_back({-S(i, 1, n, k1, base), (i+var_base), S(i-1,1,n,k1,base)});
                // (x_i or S_{i-1,1}) => S_{i,1}
                cs.push_back({-(i+var_base), S(i, 1, n, k1, base)});
                cs.push_back({-S(i-1,1,n,k1,base), S(i, 1, n, k1, base)});
                
            } else {
                // S_{i,j} => S_{i-1,j} or (S_{i-1,j-1} and x_i)
                cs.push_back({-S(i, j, n, k1, base), S(i - 1, j, n, k1, base), S(i - 1, j - 1, n, k1, base)});
                cs.push_back({-S(i, j, n, k1, base), S(i - 1, j, n, k1, base), (i+var_base)});
                // S_{i-1,j} or (S_{i-1,j-1} and x_i) => S_{i,j}
                cs.push_back({-S(i - 1, j, n, k1, base), S(i, j, n, k1, base)});
                cs.push_back({-S(i - 1, j - 1, n, k1, base), -(i+var_base), S(i, j, n, k1, base)});
            }
        }
    }

    // Second Clique
    base+= k1*(n-k1+1);
    var_base+=n;
    cs.push_back({S(n,k2,n,k2,base)});
    for (int j = 1; j <= k2; ++j) {
        for (int i = j; i <= n - k2 + j; ++i) {
            
            if (i == j && j == 1) {
                // S_{1,1} => x_1
                cs.push_back({-S(1, 1, n, k2, base), 1+var_base});
                // x_1 => S_{1,1}
                cs.push_back({-(1+var_base), S(1, 1, n, k2, base)});
            } else if (i == j) {
                // S_{i,i} => (x_i and S_{i-1,i-1})
                cs.push_back({-S(i, i, n, k2, base), i+var_base});
                cs.push_back({-S(i, i, n, k2, base), S(i - 1, i - 1, n, k2, base)});
                // x_i and S_{i-1,i-1} => S_{i,i}
                cs.push_back({-(i+var_base), -S(i - 1, i - 1, n, k2, base), S(i, i, n, k2, base)});
            }else if (j == 1) {
                // S_{i,1} => (x_i or S_{i-1,1})
                cs.push_back({-S(i, 1, n, k2, base), (i+var_base), S(i-1,1,n,k2,base)});
                // (x_i or S_{i-1,1}) => S_{i,1}
                cs.push_back({-(i+var_base), S(i, 1, n, k2, base)});
                cs.push_back({-S(i-1,1,n,k2,base), S(i, 1, n, k2, base)});
                
            } else {
                // S_{i,j} => S_{i-1,j} or (S_{i-1,j-1} and x_i)
                cs.push_back({-S(i, j, n, k2, base), S(i - 1, j, n, k2, base), S(i - 1, j - 1, n, k2, base)});
                cs.push_back({-S(i, j, n, k2, base), S(i - 1, j, n, k2, base), (i+var_base)});
                // S_{i-1,j} or (S_{i-1,j-1} and x_i) => S_{i,j}
                cs.push_back({-S(i - 1, j, n, k2, base), S(i, j, n, k2, base)});
                cs.push_back({-S(i - 1, j - 1, n, k2, base), -(i+var_base), S(i, j, n, k2, base)});
            }
        }
    }

    
    return cs;
}