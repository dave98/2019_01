#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<stdlib.h>
#include"c_worm.h"

using namespace std;

vector<c_worm*> players; //Extern values

//----------------HEADERS------------------
string server_interpreter(string, vector<vector<char>>&, vector<vector<int>>&);
void client_interpreter(string);
void update_players(vector<vector<char>>&, vector<vector<int>>&); //Funciona siempre y cuando players sea un vector externo
void update_fruits(vector<vector<char>>&, vector<vector<int>>&);
void has_muerto();

void matrix_format(vector<vector<char>>&);
void print_matrix_server(vector<vector<char>>&, vector<vector<int>>&);
bool hit_matrix_side(int, int, vector<vector<char>>&);
void print_matrix_general(vector<vector<char>>&); // usable tanto por el cliente como el server
string matriz_to_string(vector<vector<char>>&, vector<vector<int>>&);
void string_to_matriz(string);

//Interpreta los mensajes que el usuario le envia al servidor
//Cada elemento de la clase worm se encuentra separados por "-"
string server_interpreter(string message, vector<vector<char>>& mat, vector<vector<int>>& fruits){
  stringstream ss(message);
  string local_status;
  string answer;

  getline(ss, local_status, '#');
  if(local_status == "0"){ //Creando nuevo jugador
    //cout<<"[Nuevo jugador]"<<endl;
    string _nick;
    string _avatar;

    c_worm* temp = new c_worm();
    getline(ss, local_status, '#');
    _nick = local_status;
    getline(ss, local_status, '#');
    _avatar = local_status;

    temp->id = players.size();//Solo se permiten un maximo de 9 jugadores
    temp->is_alive = true;
    temp->x_head = (rand() % (mat[0].size()-4) + 1);
    temp->y_head = (rand() % (mat.size()-4) + 1);
    temp->nickname = _nick; //En este caso local status es el segundo componentes del string y toma el valor para el Nickname
    temp->avatar = _avatar;

    players.push_back(temp);
    answer = temp->to_string();
  }
  else if(local_status == "1"){
    //cout<<"[Actualizacion de jugador]"<<endl;
    getline(ss, local_status, '#'); //Getting
    int local_id = stoi(local_status);
    answer = players[local_id]->to_string(); // Obtenermos la informacion
                                        //actualizada de el jugador con el id señalado
  }

  else if(local_status == "2"){
    //cout<<"[Cambio de direccion]"<<endl;
    getline(ss, local_status, '#');//Getting ID
    int local_id = stoi(local_status);
    getline(ss, local_status, '#'); // Getting x direction
    int to_x = stoi(local_status);
    getline(ss, local_status, '#'); // Getting x direction
    int to_y = stoi(local_status);

    players[local_id]->x_directions = to_x;
    players[local_id]->y_directions = to_y;
    answer = players[local_id]->to_string();
  }
  else if(local_status == "3"){
    //cout<<"[Recolecta de matriz]"<<endl;
    answer = matriz_to_string(mat, fruits);
  }
  return answer;
}

//Toma a toda la funcion y durante la iteraccion hace que todos los gusanos cambien de lugar
void update_players(vector<vector<char>>& mat, vector<vector<int>>& fruits){
  for(unsigned int i = 0; i < players.size(); i++){
    players[i]->x_head += players[i]->x_directions; // x_directions toma valor de -1, 1, 0
    players[i]->y_head += players[i]->y_directions;

    if(hit_matrix_side(players[i]->x_head, players[i]->y_head, mat)){
      players[i]->is_alive = false;

      for(unsigned int k = 0; k < players[i]->body_x.size();  k++){
        vector<int> temp = vector<int>(2, 0);
        temp[0] = players[i]->body_x[k];
        temp[1] = players[i]->body_y[k];
        fruits.push_back(temp);
      }

    }
    else{
      for(int j = players[i]->score-1; j >= 1; j--){ //Acomodamos la posicion del cuerpo segun la posicion anterior
        players[i]->body_x[j] = players[i]->body_x[j-1];
        players[i]->body_y[j] = players[i]->body_y[j-1];
      }
      players[i]->body_x[0] = players[i]->x_head; //Primer elemento del vector corresponde  a la cabeza
      players[i]->body_y[0] = players[i]->y_head;
    }
  }
  return;
}

void update_fruits(vector<vector<char>>& p_mat, vector<vector<int>>& mat){
  if(mat.size() < 5){
    int more_f = (rand()%5) + 1;
    for(int i = 0; i < more_f; i++){
      vector<int> new_fruit = vector<int>(2, 0);
      new_fruit[0] = ( rand()%((int)p_mat[0].size()-4) )  + 1;   //for x
      new_fruit[1] = ( rand()%((int)p_mat.size()-4)    ) + 1; //for y
      //cout<<"Flag: "<<mat.size()<<" - "<<more_f<<endl;
      mat.push_back(new_fruit);
    }
  }

  vector<int> non_taken_position = vector<int>(0,0);
  vector<vector<int>> temp_mat = vector<vector<int>>(0, vector<int>(0,0));
  for(unsigned int i = 0; i < mat.size(); i++){
      bool is_taken = false;
      for(unsigned int j = 0; j < players.size(); j++){
          if(players[j]->is_alive){
            if( (players[j]->x_head == mat[i][0] ) &&  (players[j]->y_head == mat[i][1]) ){
              players[j]->score++;
              players[j]->body_x.push_back( players[j]->body_x[ players[j]->body_x.size()-1] );
              players[j]->body_y.push_back( players[j]->body_y[ players[j]->body_y.size()-1] );
              is_taken = true;
            }
          }
      }
      if(is_taken == false){
        non_taken_position.push_back(i);
      }
  }

  for(unsigned int i= 0; i < non_taken_position.size(); i++){
    temp_mat.push_back(mat[ non_taken_position[i] ]);
  }
  mat.clear();
  for(unsigned int i = 0; i < temp_mat.size(); i++){
    mat.push_back(temp_mat[i]);
  }

  return;
}

void has_muerto(){
  cout<<"------------------------------------ HAZ MUERTO--------------------------"<<endl;
  cout<<"------------------------------------ HAZ MUERTO--------------------------"<<endl;
  cout<<"------------------------------------ HAZ MUERTO--------------------------"<<endl;
  cout<<"------------------------------------ HAZ MUERTO--------------------------"<<endl;
  cout<<"------------------------------------ HAZ MUERTO--------------------------"<<endl;
}


//OPERACIONES CON MATRICES
//Establece la matriz a una configurción de dibujos sin gusanos
void matrix_format(vector<vector<char>>& mat){
  for(unsigned int i = 0; i < mat.size(); i++){
    for(unsigned int j = 0; j < mat[i].size(); j++){
        if ( (i == 0) || (i == mat.size()-1) || (j==0) || ( j == mat[i].size()-1) ){
          mat[i][j] = '+';
        }
        else{
          mat[i][j] = ' ';
        }
    }
  }
}


//Verifica si un jugador a chocado con los limites de la matriz
bool hit_matrix_side(int x, int y, vector<vector<char>>& mat){
  bool answer = false; //Inicialmente no choca con nada
  if( (x <= 0) || ( x >= (int)mat[0].size()-1) ){
    answer = true;
  }
  if( (y <= 0) || ( y >= (int)mat.size()-1) ){
    answer = true;
  }
  return answer;
}

//Modela matriz para el servidor
void print_matrix_server(vector<vector<char>>& mat, vector<vector<int>>& fruits){
  matrix_format(mat);
  //Imprimir a jugadores
  for(unsigned int i = 0; i < players.size(); i++){
    if(players[i]->is_alive){
      for(unsigned int j = 0; j < players[i]->body_x.size(); j++){
        mat[  players[i]->body_y[j]  ][ players[i]->body_x[j] ] = players[i]->avatar[0];
      }
    }
  }

  for(unsigned int i = 0; i < fruits.size(); i++){
    mat[fruits[i][1]][fruits[i][0]] = '*';
  }
  //Imprimir resultados en conjunto
  print_matrix_general(mat);
}

//Funcion que sirve para imprimir una matriz con todas las configuraciones ya hechas
void print_matrix_general(vector<vector<char>>& mat){
  for(unsigned int i = 0; i < mat.size(); i++){
    for(unsigned int j = 0; j < mat[i].size(); j++){
      cout<<mat[i][j];
    }
    cout<<endl;
  }
  cout<<endl;
}

//Funcion en donde el servidor genera un string para enviarselo al Cliente
//El string contiene el tamaño de la matriz, el numero de jugadores, el avatar de cada jugador
// y las posiciones de su cuerpo tanto en  x como en y
string matriz_to_string(vector<vector<char>>& mat, vector<vector<int>>& fruits){
    stringstream answer;
    vector<c_worm* > temp;
    for(unsigned int i = 0; i < players.size(); i++){
      if(players[i]->is_alive){
       temp.push_back(players[i]);
      }
    }

    answer << (int)mat.size() << '#'
           << (int)mat[0].size() << '#'
           //<< (int)players.size(); //Numero de jugdores
           << (int)temp.size();
    for(unsigned int i = 0; i < temp.size(); i++){
      answer << '#' << temp[i]->avatar << '#' <<(int)temp[i]->body_x.size();
      for(unsigned int j = 0; j < temp[i]->body_x.size(); j++){
        answer << '#' << temp[i]->body_x[j] << '#' <<temp[i]->body_y[j];
      }
    }

    answer << '#' <<(int)fruits.size();
    for(unsigned int i = 0; i < fruits.size(); i++){
      answer << '#' << fruits[i][0] <<'#' << fruits[i][1]; //X-Y
    }
    return answer.str();
}

//Funcion para el cliente, en donde a partir de un string genera una matriz con todos los valores que
//el servidor ha especificado
void string_to_matriz(string message){
  stringstream ss(message);
  string local_status;

  getline(ss, local_status, '#');
  int y_size = stoi(local_status);
  getline(ss, local_status, '#');
  int x_size = stoi(local_status); //Hasta acá las dos primeras posiciones

  vector<vector<char>> temp = vector<vector<char>>(y_size, vector<char>(x_size,' '));
  matrix_format(temp);

  getline(ss, local_status, '#'); //Numero de jugadores
  int player_number = stoi(local_status);
  for(int i = 0; i < player_number; i++){
    char actual_avatar;
    int cuantos_de_avatar;
    getline(ss, local_status, '#');
    actual_avatar = local_status[0];
    getline(ss, local_status, '#');
    cuantos_de_avatar = stoi(local_status);

    for(int j = 0; j < cuantos_de_avatar; j++){
      getline(ss, local_status, '#');
      int at_x = stoi(local_status);
      getline(ss, local_status, '#');
      int at_y = stoi(local_status);

      temp[at_y][at_x] = actual_avatar;
    }
  }

  getline(ss, local_status, '#'); //Numero de  fruits
  int fruits_number = stoi(local_status);
  for(int i = 0; i < fruits_number; i++){
    getline(ss, local_status, '#');
    int at_x = stoi(local_status);
    getline(ss, local_status, '#');
    int at_y = stoi(local_status);

    temp[at_y][at_x] = '*';
  }
  print_matrix_general(temp);
}

#endif
