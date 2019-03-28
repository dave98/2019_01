#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include<iostream>
#include<vector>

using namespace std;

template<typename T>
void print_matrix(vector<vector<T>>& m){
  for(unsigned int i = 0; i < m.size(); i++){
    for(unsigned int j = 0; j < m[i].size(); j++){
      cout<<m[i][j]<<" ";
    }
    cout<<endl;
  }
  cout<<endl;
}

//Asuming that dimensions are over 1 in both cases
template<typename T>
void fill_matrix(vector<vector<T>>& m){
  int decrement = 0;
  for(unsigned int i = 0; i < m.size(); i++){
    m[i][0] = decrement;
    decrement -= 2;
  }
  decrement = 0;
  for(unsigned int i = 0; i < m[0].size(); i++){
    m[0][i] = decrement;
    decrement -= 2;
  }
}

#endif
