#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include "glm/glm/vec2.hpp"
#include "e_clust.h"

using namespace std;

vector<e_clust*> tree;
int start_on = 0;
int step = 10;

vector<string> splitStrings(string str, char dl)
{
    string word = "";
    int num = 0;
    str = str + dl;
    int l = str.size();

    vector<string> substr_list;
    for (int i = 0; i < l; i++) {
          if (str[i] != dl)
            word = word + str[i];

        else {
            if ((int)word.size() != 0)
                substr_list.push_back(word);
            word = "";
        }
    }

    return substr_list;
}

bool is_in_tree(string look_for, int& index){
  for(unsigned int i = 0; i < tree.size(); i++){
    if(tree[i]->name == look_for){
      index = i;
      return true;
    }
  }
  index = 0;
  return false;
}

class cluster{
public:
  cluster();
  ~cluster();

  string router;
  int var_cant;
  vector<string> elements;
  vector<vector<float>> distances;

  void read_matriz(int);
    float get_distances(vector<float>&, vector<float>&); //Devuelve la distancia entre dos vectores característicos

  void aglomerativo_minimo(int);
    float find_minimo(int&, int&);
    void fix_cluster_less(int, int, int, float);
    void fix_cluster_big(int, int, int, float);
    void fix_cluster_prom(int, int, int, float);

  void print_data();

};


cluster::cluster(){
  this->router = "route.txt";
  this->var_cant = 8;
  this->elements = vector<string>(0, "");
  this->distances = vector<vector<float>>(0, vector<float>(0, 100.0));

}
cluster::~cluster(){}

void cluster::read_matriz(int max_elements){
  fstream reader(this->router.c_str(), ios::in);
  vector<vector<float>> storing = vector<vector<float>>(0, vector<float>(0, 0.0));
  string word;
  //int max_elements = 130;

  for (int i = 0; i < this->var_cant; i++) {
		reader >> word;
	}

  int elements_in = 0;
  bool _cont = true;
	while (_cont and reader >> word){
		this->elements.push_back(word);
		vector<float> temp = vector<float>(this->var_cant - 1, 0.0);
		for (int i = 0; i < this->var_cant - 1; i++) {
			reader >> word;
			temp[i] = stof(word);
		}
		storing.push_back(temp);
    elements_in++;
    if(elements_in > max_elements){
      _cont = false;
    }
	}

  this->distances = vector<vector<float>>(storing.size(), vector<float>(storing.size(), 0.0)); //Configuramos el tamaño de la matriz
	for (unsigned int i = 0; i < storing.size(); i++) {
		for (unsigned int j = i; j < storing.size(); j++) {
			distances[i][j] = this->get_distances(storing[i], storing[j]);
		}
	}
  cout<<"[Proceso de lectura finalizado]"<<endl;
}

float cluster::get_distances(vector<float>& a, vector<float>& b){
  float answer = 0.0;
  for (unsigned int i = 0; i < a.size(); i++) { //a y b deben ser del mismo tamaño
    answer += pow((a[i] - b[i]), 2.0);
  }
  answer = sqrtf(answer);
  return answer;
}

void cluster::print_data(){
  for(unsigned int i = 0; i < distances.size(); i++){
    for(unsigned int j = 0; j < distances[i].size(); j++){
      cout<<this->distances[i][j]<<" ";
    }
    cout<<endl;
  }
  cout<<endl;
}

void cluster::aglomerativo_minimo(int mood){
  int indexer = 1;
  // O para menor, 1 para mayor y 2 para ponderado
  int analyzed_elements = 1;

  while ((int)this->distances.size() > analyzed_elements) {
    int _x = 0; int _y = 0;
    float minimo = find_minimo(_y, _x);
    if (mood == 0) {
      fix_cluster_less(_y, _x, indexer, minimo);
    }
    else if (mood == 1) {
      fix_cluster_big(_y, _x, indexer, minimo);
    }
    else {
      fix_cluster_prom(_y, _x, indexer, minimo);
    }
    indexer++;
  }

  //print_v(ele, true);
  //cout << "Minimo en: " << duration1 << " microsegundos" << endl;
}

float cluster::find_minimo(int& _y, int& _x){
  //Considerando la inexistencia de distancias negativas
  _y = 0; _x = 1;
  float answer = this->distances[_y][_x]; //Ubicación de la primera distancia minima
  for (unsigned int i = 0; i < this->distances.size(); i++) {
    for (unsigned int j = i + 1; j < this->distances[i].size(); j++) {
      if (this->distances[i][j] < answer) {
        answer = this->distances[i][j];
        _y = i;
        _x = j;
      }
    }
  }
  return answer;
}

void cluster::fix_cluster_less(int _y, int _x, int etapa, float min){
  int _x_index = 0;
  int _y_index = 0;

  //_y siempre será menor a _x, es decir distribuido horizontalmente siempre estará antes que _x
	//Ejecutando hasta el quiebre de y
	for (unsigned int i = 0; i < _y; i++) {
		if (_y_index == _x) {
			_y_index++; //No hay comparacion
		}
		else if(_x_index == _y){
			_x_index++; //Tampoco hay comparación
		}
		else {
			if (_x_index == _x) {
				_x_index++;
				//Do nothing
			}
			else if (_x_index < _x) { //Verificamos si _x ha quebrado a la izquierda
				if (this->distances[_x_index][_x] < this->distances[_y_index][_y]) {
					this->distances[_y_index][_y] = this->distances[_x_index][_x];
				}
				_y_index++;
				_x_index++;
			}
			else {
				if (this->distances[_x][_x_index] < this->distances[_y_index][_y]) {
					this->distances[_y_index][_y] = this->distances[_x][_x_index];
				}
				_y_index++;
				_x_index++;
			}
			//Comparacion
		}
	}
  _y_index++;
	for (unsigned int i = _y + 1; i < this->distances.size(); i++) {
		if (_y_index == _x) {
			_y_index++; //No hay comparacion
		}
		else if (_x_index == _y) {
			_x_index++; //Tampoco hay comparación
		}
		else {
			if (_x_index == _x) {
				_x_index++;
				//Do nothing else
			}
			else if (_x_index < _x) { //Verificamos si _x ha quebrado a la izquierda
				if (this->distances[_x_index][_x] < this->distances[_y][_y_index]) {
					this->distances[_y_index][_y] = this->distances[_x_index][_x];
				}
				_y_index++;
				_x_index++;
			}
			else {
				if (this->distances[_x][_x_index] < this->distances[_y][_y_index]) {
					this->distances[_y][_y_index] = this->distances[_x][_x_index];

				}
				_y_index++;
				_x_index++;
				//Comparacion
			}
		}
	}
  this->elements[_y] = this->elements[_y] + " "+ this->elements[_x]; //Asignacion de los cluster

  vector<string> tempo = splitStrings(this->elements[_y], ' ');
  float minim_x = -999999; //Indica el nodo más a la izquierda
  float max_x = 999999; //Indica el nodo más a la derecha
  vector<e_clust* > temp_vector;
  for(unsigned int i = 0; i < tempo.size(); i++){
    int pos_in_tree = 0;
    if(!is_in_tree(tempo[i], pos_in_tree)){
      e_clust* temp = new e_clust(tempo[i], glm::vec2(start_on, 0), glm::vec2(start_on, 0) );
      start_on -= step;
      tree.push_back(temp);
      temp_vector.push_back(temp);

      if(minim_x < temp->center[temp->center.size() - 1].x){
        minim_x = temp->center[temp->center.size() - 1].x;
      }
      else if(max_x > temp->center[temp->center.size() - 1].x){
        max_x = temp->center[temp->center.size() - 1].x;
      }
      //No esta en el arbol
    }
    else{
      temp_vector.push_back(tree[pos_in_tree]);

      if(minim_x < tree[pos_in_tree]->center[tree[pos_in_tree]->center.size() - 1].x){
        minim_x = tree[pos_in_tree]->center[tree[pos_in_tree]->center.size() - 1].x;
      }
      else if(max_x > tree[pos_in_tree]->center[tree[pos_in_tree]->center.size() - 1].x){
        max_x = tree[pos_in_tree]->center[tree[pos_in_tree]->center.size() - 1].x;
      }

    }
    //Todos los nodos nuevos estan en el arbol
  }
  float mid_x = (minim_x + max_x)/2;
  for(unsigned int i = 0; i < temp_vector.size(); i++){
    temp_vector[i]->center.push_back(glm::vec2(mid_x, etapa));
    temp_vector[i]->distances.push_back(min);
  }

	//cout << "Antes: Y -> " << dist.size() << " X -> " << dist[0].size() << endl;
	//Eliminamos la columna x, tenemos la certeza de que y está antes que x
	for (unsigned int i = 0; i < this->distances.size(); i++) {
		this->distances[i].erase(this->distances[i].begin() + _x);
	}
	//Eliminamos la fila x
	this->distances.erase(this->distances.begin() + _x);
	this->elements.erase(this->elements.begin() + _x);
/*
  for(unsigned int i = 0; i < this->elements.size(); i++){
    cout<<this->elements[i]<<endl;
  }
  cout<<endl<<endl<<endl<<endl;*/
}

void cluster::fix_cluster_big(int _y, int _x, int etapa, float min){
  int _x_index = 0;
  int _y_index = 0;

  //_y siempre será menor a _x, es decir distribuido horizontalmente siempre estará antes que _x
	//Ejecutando hasta el quiebre de y
	for (unsigned int i = 0; i < _y; i++) {
		if (_y_index == _x) {
			_y_index++; //No hay comparacion
		}
		else if(_x_index == _y){
			_x_index++; //Tampoco hay comparación
		}
		else {
			if (_x_index == _x) {
				_x_index++;
				//Do nothing
			}
			else if (_x_index < _x) { //Verificamos si _x ha quebrado a la izquierda
				if (this->distances[_x_index][_x] > this->distances[_y_index][_y]) {
					this->distances[_y_index][_y] = this->distances[_x_index][_x];
				}
				_y_index++;
				_x_index++;
			}
			else {
				if (this->distances[_x][_x_index] > this->distances[_y_index][_y]) {
					this->distances[_y_index][_y] = this->distances[_x][_x_index];
				}
				_y_index++;
				_x_index++;
			}
			//Comparacion
		}
	}
  _y_index++;
	for (unsigned int i = _y + 1; i < this->distances.size(); i++) {
		if (_y_index == _x) {
			_y_index++; //No hay comparacion
		}
		else if (_x_index == _y) {
			_x_index++; //Tampoco hay comparación
		}
		else {
			if (_x_index == _x) {
				_x_index++;
				//Do nothing else
			}
			else if (_x_index < _x) { //Verificamos si _x ha quebrado a la izquierda
				if (this->distances[_x_index][_x] > this->distances[_y][_y_index]) {
					this->distances[_y_index][_y] = this->distances[_x_index][_x];
				}
				_y_index++;
				_x_index++;
			}
			else {
				if (this->distances[_x][_x_index] > this->distances[_y][_y_index]) {
					this->distances[_y][_y_index] = this->distances[_x][_x_index];

				}
				_y_index++;
				_x_index++;
				//Comparacion
			}
		}
	}
  this->elements[_y] = this->elements[_y] + " "+ this->elements[_x]; //Asignacion de los cluster

  vector<string> tempo = splitStrings(this->elements[_y], ' ');
  float minim_x = -999999; //Indica el nodo más a la izquierda
  float max_x = 999999; //Indica el nodo más a la derecha
  vector<e_clust* > temp_vector;
  for(unsigned int i = 0; i < tempo.size(); i++){
    int pos_in_tree = 0;
    if(!is_in_tree(tempo[i], pos_in_tree)){
      e_clust* temp = new e_clust(tempo[i], glm::vec2(start_on, 0), glm::vec2(start_on, 0) );
      start_on -= step;
      tree.push_back(temp);
      temp_vector.push_back(temp);

      if(minim_x < temp->center[temp->center.size() - 1].x){
        minim_x = temp->center[temp->center.size() - 1].x;
      }
      else if(max_x > temp->center[temp->center.size() - 1].x){
        max_x = temp->center[temp->center.size() - 1].x;
      }
      //No esta en el arbol
    }
    else{
      temp_vector.push_back(tree[pos_in_tree]);

      if(minim_x < tree[pos_in_tree]->center[tree[pos_in_tree]->center.size() - 1].x){
        minim_x = tree[pos_in_tree]->center[tree[pos_in_tree]->center.size() - 1].x;
      }
      else if(max_x > tree[pos_in_tree]->center[tree[pos_in_tree]->center.size() - 1].x){
        max_x = tree[pos_in_tree]->center[tree[pos_in_tree]->center.size() - 1].x;
      }

    }
    //Todos los nodos nuevos estan en el arbol
  }
  float mid_x = (minim_x + max_x)/2;
  for(unsigned int i = 0; i < temp_vector.size(); i++){
    temp_vector[i]->center.push_back(glm::vec2(mid_x, etapa));
    temp_vector[i]->distances.push_back(min);
  }

	//cout << "Antes: Y -> " << dist.size() << " X -> " << dist[0].size() << endl;
	//Eliminamos la columna x, tenemos la certeza de que y está antes que x
	for (unsigned int i = 0; i < this->distances.size(); i++) {
		this->distances[i].erase(this->distances[i].begin() + _x);
	}
	//Eliminamos la fila x
	this->distances.erase(this->distances.begin() + _x);
	this->elements.erase(this->elements.begin() + _x);
/*
  for(unsigned int i = 0; i < this->elements.size(); i++){
    cout<<this->elements[i]<<endl;
  }
  cout<<endl<<endl<<endl<<endl;*/
}

void cluster::fix_cluster_prom(int _y, int _x, int etapa, float min){
  int _x_index = 0;
  int _y_index = 0;

  //_y siempre será menor a _x, es decir distribuido horizontalmente siempre estará antes que _x
	//Ejecutando hasta el quiebre de y
	for (unsigned int i = 0; i < _y; i++) {
		if (_y_index == _x) {
			_y_index++; //No hay comparacion
		}
		else if(_x_index == _y){
			_x_index++; //Tampoco hay comparación
		}
		else {
			if (_x_index == _x) {
				_x_index++;
				//Do nothing
			}
			else if (_x_index < _x) { //Verificamos si _x ha quebrado a la izquierda
				this->distances[_y_index][_y] = (this->distances[_x_index][_x] + this->distances[_y_index][_y] ) / 2;
				_y_index++;
				_x_index++;
			}
			else {
			  this->distances[_y_index][_y] = (this->distances[_x][_x_index] + this->distances[_y_index][_y]) / 2;
				_y_index++;
				_x_index++;
			}
			//Comparacion
		}
	}
  _y_index++;
	for (unsigned int i = _y + 1; i < this->distances.size(); i++) {
		if (_y_index == _x) {
			_y_index++; //No hay comparacion
		}
		else if (_x_index == _y) {
			_x_index++; //Tampoco hay comparación
		}
		else {
			if (_x_index == _x) {
				_x_index++;
				//Do nothing else
			}
			else if (_x_index < _x) { //Verificamos si _x ha quebrado a la izquierda
        this->distances[_y_index][_y] = (this->distances[_x_index][_x] + this->distances[_y_index][_y] ) / 2;
				_y_index++;
				_x_index++;
			}
			else {
        this->distances[_y_index][_y] = (this->distances[_x][_x_index] + this->distances[_y_index][_y]) / 2;
				_y_index++;
				_x_index++;
				//Comparacion
			}
		}
	}
  this->elements[_y] = this->elements[_y] + " "+ this->elements[_x]; //Asignacion de los cluster

  vector<string> tempo = splitStrings(this->elements[_y], ' ');
  float minim_x = -999999; //Indica el nodo más a la izquierda
  float max_x = 999999; //Indica el nodo más a la derecha
  vector<e_clust* > temp_vector;
  for(unsigned int i = 0; i < tempo.size(); i++){
    int pos_in_tree = 0;
    if(!is_in_tree(tempo[i], pos_in_tree)){
      e_clust* temp = new e_clust(tempo[i], glm::vec2(start_on, 0), glm::vec2(start_on, 0) );
      start_on -= step;
      tree.push_back(temp);
      temp_vector.push_back(temp);

      if(minim_x < temp->center[temp->center.size() - 1].x){
        minim_x = temp->center[temp->center.size() - 1].x;
      }
      else if(max_x > temp->center[temp->center.size() - 1].x){
        max_x = temp->center[temp->center.size() - 1].x;
      }
      //No esta en el arbol
    }
    else{
      temp_vector.push_back(tree[pos_in_tree]);

      if(minim_x < tree[pos_in_tree]->center[tree[pos_in_tree]->center.size() - 1].x){
        minim_x = tree[pos_in_tree]->center[tree[pos_in_tree]->center.size() - 1].x;
      }
      else if(max_x > tree[pos_in_tree]->center[tree[pos_in_tree]->center.size() - 1].x){
        max_x = tree[pos_in_tree]->center[tree[pos_in_tree]->center.size() - 1].x;
      }

    }
    //Todos los nodos nuevos estan en el arbol
  }
  float mid_x = (minim_x + max_x)/2;
  for(unsigned int i = 0; i < temp_vector.size(); i++){
    temp_vector[i]->center.push_back(glm::vec2(mid_x, etapa));
    temp_vector[i]->distances.push_back(min);
  }

	//cout << "Antes: Y -> " << dist.size() << " X -> " << dist[0].size() << endl;
	//Eliminamos la columna x, tenemos la certeza de que y está antes que x
	for (unsigned int i = 0; i < this->distances.size(); i++) {
		this->distances[i].erase(this->distances[i].begin() + _x);
	}
	//Eliminamos la fila x
	this->distances.erase(this->distances.begin() + _x);
	this->elements.erase(this->elements.begin() + _x);
/*
  for(unsigned int i = 0; i < this->elements.size(); i++){
    cout<<this->elements[i]<<endl;
  }
  cout<<endl<<endl<<endl<<endl;*/
}


#endif
