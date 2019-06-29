#ifndef C_CAR_H
#define C_CAR_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

char SPACE_SYMBOL = '_';

class c_car{
public:
  int id; //Dado por el server
  bool is_alive;
  int x_directions;
  int y_directions;
  int x_head;
  int y_head;
  int score;
  string nickname;
  string avatar;

  c_car();
  ~c_car();

  void get_description();
  string first_connection_request(); // --CLIENT --
  string update_connection_request(); // --CLIENT --
  string change_direction_request(); // --CLIENT --
  string get_matriz_request();

  void change_direction(int, int); // -- CLIENT - SERVER --
  string to_string(); // -- SERVER --
  void to_class(string); // -- CLIENT --
};


c_car::c_car(){
  this->id = 0;
  this->is_alive = false;
  this->nickname = "";
  this->avatar = "D"; //Defecto
  this->score = 0;
  this->x_head = 0;
  this->y_head = 0;
  this->x_directions = 0;
  this->y_directions = 0;
}

c_car::~c_car(){}

void c_car::get_description(){
  cout<<"ID: "<<this->id<<endl;
  cout<<"Alive: "<<this->is_alive<<endl;
  cout<<"Nickname: "<<this->nickname<<endl;
  cout<<"Avatar: "<<this->avatar<<endl;
  cout<<"Score: "<<this->score<<endl;
  cout<<"x_head: "<<this->x_head<<endl;
  cout<<"y_head: "<<this->y_head<<endl;
  cout<<"x_directions: "<<this->x_directions<<endl;
  cout<<"y_directions: "<<this->y_directions<<endl;
}
// CLIENT - Funcion para solicitar los datos básicos que el cliente necesita
string c_car::first_connection_request(){
  stringstream answer;
  answer << '0' << SPACE_SYMBOL;
  answer << this->nickname << SPACE_SYMBOL;
  answer << this->avatar;
  return answer.str();
}
// CLIENT - Para usar esta funcion, el cliente ya debe tener la id provista por el server
string c_car::update_connection_request(){
  stringstream answer;
  answer << '1' << SPACE_SYMBOL;
  answer << this->id <<endl;
  return answer.str();
}
// CLIENT - Funcion para cambiar la direccion del jugador, el client hace el Cambio
// y se lo envia al servidor con la inicial '2'. El servidor accede a la petición
// sin devolver alguna respuesta.
string c_car::change_direction_request(){
  stringstream answer;
  answer << '2' << SPACE_SYMBOL;
  answer << this->id << SPACE_SYMBOL;
  answer << this->x_directions << SPACE_SYMBOL;
  answer << this->y_directions;
  return answer.str();
}

//CLIENT - Funcion para obtener un string de la matriz
string c_car::get_matriz_request(){
  stringstream answer;
  answer << '3';
  return answer.str();
}

// CLIENT - SERVER Funcion que actualiza la direccion del carro, tanto para el server
// como para el cliente
void c_car::change_direction(int _x, int _y){
  this->x_directions = _x;
  this->y_directions = _y;
}
// SERVER - Funcion que le permite al servidor enviar la configuracion actual de la clase
// para el cliente. El cliente traduce la clase para adaptarse
string c_car::to_string(){
  stringstream answer;
  answer << '0' << SPACE_SYMBOL /*No util para el servidor*/
         << this->id << SPACE_SYMBOL
         << this->is_alive << SPACE_SYMBOL
         << this->nickname << SPACE_SYMBOL
         << this->avatar << SPACE_SYMBOL
         << this->score << SPACE_SYMBOL
         << this->x_head << SPACE_SYMBOL
         << this->y_head << SPACE_SYMBOL
         << this->x_directions << SPACE_SYMBOL
         << this->y_directions;
  return answer.str();
}
// CLIENT - Funcion que usa el cliente para transformar el string que recibe del servidor
// para actualizar su datos en el cliente.
void c_car::to_class(string incoming_message){
  stringstream ss(incoming_message);
  stringstream aa;
  string local_status;

  getline(ss, local_status, SPACE_SYMBOL); // Primer simbolo descartado. util solo para el server

  getline(ss, local_status, SPACE_SYMBOL); // id
    this->id = stoi(local_status);
  getline(ss, local_status, SPACE_SYMBOL); // is_alive
    this->is_alive = (bool)(stoi(local_status));
  getline(ss, local_status, SPACE_SYMBOL); // nickname
    this->nickname = local_status;
  getline(ss, local_status, SPACE_SYMBOL); // Avatar
    this->avatar = local_status;
  getline(ss, local_status, SPACE_SYMBOL); // score
    this->score = stoi(local_status);
  getline(ss, local_status, SPACE_SYMBOL); // x_head
    this->x_head = stoi(local_status);
  getline(ss, local_status, SPACE_SYMBOL); // y_head
    this->y_head = stoi(local_status);
  getline(ss, local_status, SPACE_SYMBOL); // x_directions
    this->x_directions = stoi(local_status);
  getline(ss, local_status, SPACE_SYMBOL); // y_directions
    this->y_directions = stoi(local_status);
}

















#endif
