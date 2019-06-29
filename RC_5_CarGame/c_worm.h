#ifndef C_WORM_H
#define C_WORM_H

#include<iostream>
#include<vector>
#include<string>
#include<sstream>

using namespace std;

//Debe de ser definido tanto en el server como en el cliente.
class c_worm{
public:
  int id; //Dado por el server
  bool is_alive; //Dentro de la matriz define si el gusano se va a dibujar
  int x_directions; // 1 derecha, -1 izquierda, 0 no hay movimiento
  int y_directions; // 1 arriba, -1 abajo, 0 no hay movimiento
  int score; //Manzanas devoradas
  int x_head; //Posicion de la cabeza en X
  int y_head; //Posicion de la cabeza en Y
  vector<int> body_x; //Posicion del cuerpo en x, es del tamaño del score e incluye la cabeza.
  vector<int> body_y; //Equivalente para Y
  string nickname;
  string avatar;

  c_worm();
  ~c_worm();

  void get_description();
  string send_first_connection();
  string update_connection();
  void change_direction(int, int);
  string get_direction();
  string get_matriz();

  string to_string();
  void to_class(string);
};

c_worm::c_worm(){
  this->id = 0;
  this->is_alive = false;
  this->x_directions = 0;//Sin movimiento
  this->y_directions = 0;
  this->score = 4;
  this->x_head = 0; //Una posicion aleatoria debe ser dada por el server.
  this->y_head = 0; //
  this->nickname = "";
  this->avatar = "D"; //Defecto
  this->body_x = vector<int>(this->score, this->x_head);
  this->body_y = vector<int>(this->score, this->y_head);
}

c_worm::~c_worm(){}

string c_worm::send_first_connection(){
  string answer;
  answer += '0';
  answer += '#';
  answer += this->nickname;
  answer += '#';
  answer += this->avatar;
  return answer;
}

string c_worm::update_connection(){
  stringstream answer;
  answer << '1' << '#'
         << this->id;

  return answer.str();
}

void c_worm::change_direction(int x, int y){
  this->x_directions = x;
  this->y_directions = y;
}

string c_worm::get_direction(){
  stringstream answer;
  answer << '2' << '#'
         << this->id << '#'
         << this->x_directions << '#'
         << this->y_directions;
  return answer.str();
}

string c_worm::get_matriz(){
  stringstream answer;
  answer << '3';
  return answer.str();
}

string c_worm::to_string(){
  stringstream answer;
  answer << '0'<<'#'
         <<this->id<<'#'
         <<this->is_alive<<'#'
         <<this->x_directions<<'#'
         <<this->y_directions<<'#'
         <<this->score<<'#'
         <<this->x_head<<'#'
         <<this->y_head<<'#'
         <<this->nickname<<'#'
         <<this->avatar;

  for(unsigned int i = 0; i < this->body_x.size(); i++){
    answer<<'#'<<body_x[i]<<'#'<<body_y[i];
  }
  return answer.str();
}

void c_worm::to_class(string message){
  //cout<<"[Iniciando To_class]"<<endl;
  //cout<<message<<endl;
  stringstream ss(message);
  stringstream aa;
  string local_status;

  getline(ss, local_status, '#'); //Descartamos primer parametro, es solo para server

  getline(ss, local_status, '#'); // ID
  this->id = stoi(local_status);
  getline(ss, local_status, '#'); // is_alive
  this->is_alive = (bool)(stoi(local_status));
  getline(ss, local_status, '#'); // x_directions
  this->x_directions = stoi(local_status);
  getline(ss, local_status, '#'); // y_directions
  this->y_directions = stoi(local_status);
  getline(ss, local_status, '#'); // score
  this->score = stoi(local_status);
  getline(ss, local_status, '#'); // x_head
  this->x_head = stoi(local_status);
  getline(ss, local_status, '#'); // y_head
  this->y_head = stoi(local_status);
  getline(ss, local_status, '#'); // nickname
  this->nickname = local_status;
  getline(ss, local_status, '#'); // avatar
  this->avatar = local_status;

  this->body_x.clear();
  this->body_y.clear();
  for(int i = 0; i < this->score; i++){
    getline(ss, local_status, '#');
    this->body_x.push_back(stoi(local_status));
    getline(ss, local_status, '#');
    this->body_y.push_back(stoi(local_status));
  }
}

void c_worm::get_description(){
  cout<<"ID: "<<this->id<<endl;
  cout<<"Alive "<<this->is_alive<<endl;
  cout<<"Nickname: "<<this->nickname<<endl;
  cout<<"Avatar: "<<this->avatar<<endl;
  cout<<"x_directions: "<<this->x_directions<<endl;
  cout<<"y_directions: "<<this->y_directions<<endl;
  cout<<"Score: "<<this->score<<endl;
  cout<<"x_head: "<<this->x_head<<endl;
  cout<<"y_head: "<<this->y_head<<endl;
}
#endif
