#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

int main(){
  string a = "Hola, como estas";
  //cout<<a<<endl;

  string b = "";
  string deli = " ,";

  //char* temp_a = new char[a.length() + 1];
  char temp_a[a.length() + 2];
  strcpy(temp_a, a.c_str());
  char* temp_b;

  temp_b = strtok(temp_a, deli.c_str());
  while(temp_b != NULL  ){
    b = temp_b;
    cout<<b<<endl;
    temp_b = strtok(NULL, deli.c_str());
  }


   return 0;
}
