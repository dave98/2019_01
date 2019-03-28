#include<iostream>
#include<stdio.h>
#include<chrono>
#include<thread>
#include<string>
#include<vector>
#include"functions.h"

using namespace std;

int main(){
  string c_1;
  string c_2;

  cout<<"Cadena 1: "<<endl;
  cin>>c_1;
  cout<<"Cadema 2: "<<endl;
  cin>>c_2;

  vector<vector<int>> mat = vector<vector<int>>(c_1.size() + 1, vector<int>(c_2.size() + 1, 0));

  print_matrix<int>(mat);
  fill_matrix<int>(mat);
  print_matrix<int>(mat); // Reviewed

  std::thread a;

  return 0;
}
