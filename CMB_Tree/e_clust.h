#ifndef E_CLUST_H
#define E_CLUST_H

#include <iostream>
#include <vector>
#include "glm/glm/vec2.hpp"

using namespace std;


class e_clust{
public:
  string name;
  glm::vec2 origen;
  vector<glm::vec2> center;
  vector<float> distances;

  e_clust(string, glm::vec2, glm::vec2);
  ~e_clust();
};


//Los CLUSTER siempre empiezan con dos elementos
e_clust::e_clust(string _name, glm::vec2 _origen, glm::vec2 _center){
  this->name = _name;
  this->origen = _origen;
  this->center.push_back(_center); //Distances and center must be the same 
  this->distances.push_back(0.0);
}

e_clust::~e_clust(){}

#endif
