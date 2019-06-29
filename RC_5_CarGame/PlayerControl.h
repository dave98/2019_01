#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<stdlib.h>
#include"c_car.h"

using namespace std;

vector<c_car*> runners; // Extern values;
int server_actual_state;

struct c_pair{
  int x;
  int y;
};

//---------------- CAR HEADERS ------------------------------
string server_request_interpreter(string, vector<vector<char>>&, vector<vector<int>>&);  // -- SERVER --
void server_update_players(vector<vector<char>>&, vector<vector<int>>&); // -- SERVER --
  bool detect_collision(int, vector<vector<char>>& ,vector<vector<int>>&);
    bool way_collision(int, vector<vector<char>>&); //Colision con los limites de la pista
    bool obstacle_colision(int, vector<vector<int>>&);
    bool car_collision(c_pair, c_pair, c_pair, c_pair);
    void car_collision_v2(int, int);
void server_update_obstacles(vector<vector<char>>&, vector<vector<int>>&);
void client_death_message(); //Mensaje del jugador en caso muera
void server_print_way(vector<vector<char>>&, vector<vector<int>>&);
  void base_way(vector<vector<char>>&, int);
  void print_matrix(vector<vector<char>>&);
string server_matriz_to_string(vector<vector<char>>&, vector<vector<int>>&, int);
void client_string_to_matriz(string);
//-----------------------------------------------------------

// SERVER - Interpreta los mensajes que el usuario le envia al servidor
//Cada elemento de la clase car se encuentra separados por SPACE_SYMBOL
string server_request_interpreter(string message, vector<vector<char>>& mat, vector<vector<int>>& obstacles){
  stringstream ss(message);
  string local_status;
  string answer;

  getline(ss, local_status, SPACE_SYMBOL);// Determina que operacion procede con el servidor
  // PETICION PARA NUEVO JUGADOR
  if(local_status == "0"){
    string _nick;
    string _avatar;
    c_car* temp = new c_car();

    getline(ss, local_status, SPACE_SYMBOL);
    _nick = local_status;
    getline(ss, local_status, SPACE_SYMBOL);
    _avatar = local_status;

    temp->id= runners.size();
    temp->is_alive = true;
    temp->x_head = 10; //CHANGE LATER
    temp->y_head = 10; //CHANGE LATER
    temp->nickname = _nick;
    temp->avatar = _avatar;

    runners.push_back(temp); //MUTEX
    answer = temp->to_string();
  }
  // PETICION PARA ACTUALIZAR UN JUGADOR
  else if(local_status == "1"){
    getline(ss, local_status, SPACE_SYMBOL);
    int local_id = stoi(local_status);
    answer = runners[local_id]->to_string(); //Clase mapeada
    //AÑADIR MATRIZ DE PISTA
  }
  // MODIFICANDO LA DIRECCION DE UN RUNNER
  else if(local_status == "2"){
    getline(ss, local_status, SPACE_SYMBOL);
    int local_id = stoi(local_status);
    getline(ss, local_status, SPACE_SYMBOL);
    int to_x = stoi(local_status);
    getline(ss, local_status, SPACE_SYMBOL);
    int to_y = stoi(local_status);
    //En este punto tenemos los datos para identificar al runner que cambia de direccion
    runners[local_id]->change_direction(to_x, to_y);
    answer = runners[local_id]->to_string();
    //No se da la peticion de respuesta
    //NO LO OLVIDES
  }
  //RESPONDIENDO A UNA PETICION DE MATRIZ
  else if(local_status == "3"){
    answer = server_matriz_to_string(mat, obstacles, server_actual_state);
  }
  return answer;
}

// SERVER - Toma la matriz y hace que todos los carros actualizen su posicion a corde a la direccion que llevan
void server_update_players(vector<vector<char>>& mat, vector<vector<int>>& obstacles){
    for(unsigned int i = 0; i < runners.size(); i++){
      if(runners[i]->is_alive){
        if(!detect_collision(i, mat, obstacles)){
            runners[i]->x_head += runners[i]->x_directions; //Movemos el carro perteneciente
            runners[i]->y_head += runners[i]->y_directions;
            runners[i]->x_directions = 0;
            runners[i]->y_directions = 0;
        }
        else{
          runners[i]->is_alive = false;
        }

      }
    }
}


// SERVER - Detecta las colisiones del carro con el id actual frente a los otros carros, además detecta si el carro choca
// con los obstaculos. En el primer caso el carro simplemente no podrá seguir moviendose en la direccion que se planteaba
// si el choque es con un obstaculo el jugador muere
bool detect_collision(int external_id, vector<vector<char>>& mat, vector<vector<int>>& obstacles){
  //RUTINA PARA EVALUAR CHOQUE CON LOS LIMITES DE LA PISTA
  if(way_collision(external_id, mat)){
    return true;
  }
  //RUTINA PARA EVALUAR EL CHOQUE CON LOS OBSTACULOS
  if( obstacle_colision(external_id, obstacles)){
    return true;
  }

  //RUTINA PARA EVALUAR EL CHOQUE CON OTROS CARROS
  for(unsigned int i = 0; i < runners.size(); i++){
    if((int)i != external_id){ // Verifico de no chocarme conmigo mismo
      //c_pair c1_lu; c1_lu.x = runners[external_id]->x_head - 1; c1_lu.y = runners[external_id]->y_head - 1; // MI CARRO
      //c_pair c1_rd; c1_lu.x = runners[external_id]->x_head + 1; c1_lu.y = runners[external_id]->y_head + 1;
      //c_pair c2_lu; c2_lu.x = runners[i]->x_head - 1; c2_lu.y = runners[i]->y_head - 1; // CARRO A ANALIZAR
      //c_pair c2_rd; c2_rd.x = runners[i]->x_head - 1; c2_rd.y = runners[i]->y_head - 1;

      /*if(car_collision(c1_lu, c1_rd, c2_lu, c2_rd)){
        return true;
      }*/
      car_collision_v2(external_id, i);

    }
  }
  return false;
}

bool way_collision(int external_id, vector<vector<char>>& mat){
  c_pair car_lu; car_lu.x = runners[external_id]->x_head - 1; car_lu.y = runners[external_id]->y_head - 1;
  c_pair car_rd; car_rd.x = runners[external_id]->x_head + 1; car_rd.y = runners[external_id]->y_head + 1;

  if( (car_lu.x  <= 0) || (car_lu.y <= 0) || (car_rd.x >= (int)mat[0].size()-1) || (car_rd.y >= (int)mat.size()-1) ){
    runners[external_id]->is_alive = false;
    return true;
  }
  return false;

}

bool obstacle_colision(int external_id, vector<vector<int>>& obstacles){
  for(unsigned int i = 0; i < obstacles.size(); i++){
    if( obstacles[i][0] == runners[external_id]->x_head && obstacles[i][1] == runners[external_id]->y_head){
      return true;
    }
  }
  return false;
}

bool car_collision(c_pair car1_lu, c_pair car1_rd, c_pair car2_lu, c_pair car2_rd){
  if( (car1_lu.x > car2_rd.x) || (car2_lu.x > car1_rd.x) ){
    return false;
  }
  if( (car1_lu.y < car2_rd.y) || (car2_lu.y < car1_rd.y) ){
    return false;
  }
  return true;
}

void car_collision_v2(int my_car, int enemy_car){
  //Llanta izquierda:
  if( (runners[my_car]->x_head - 1  == runners[enemy_car]->x_head) && (runners[my_car]->y_head - 1  == runners[enemy_car]->y_head) ){
    runners[enemy_car]->is_alive = false;
    return;
  }
  if( (runners[my_car]->x_head + 1  == runners[enemy_car]->x_head) && (runners[my_car]->y_head - 1  == runners[enemy_car]->y_head) ){
    runners[enemy_car]->is_alive = false;
    return;
  }
  if( (runners[my_car]->x_head - 1  == runners[enemy_car]->x_head) && (runners[my_car]->y_head + 1  == runners[enemy_car]->y_head) ){
    runners[enemy_car]->is_alive = false;
    return;
  if( (runners[my_car]->x_head + 1  == runners[enemy_car]->x_head) && (runners[my_car]->y_head + 1  == runners[enemy_car]->y_head) ){
  }
    runners[enemy_car]->is_alive = false;
    return;
  }

}
/*0 0 car collision rectangle this->x_head-1, this->y_head-1 TO this->x_head+1, this->y_head+1
   a
  0 0*/

void server_update_obstacles(vector<vector<char>>& mat, vector<vector<int>>& obstacles){
    //cout<<"Actualizando Obstaculos"<<endl;
    int min_in_screen = 15;
    int distance = 3;
    if( (int)obstacles.size() < min_in_screen){
      int more_obstacles = (rand()%3) + 1;
      for(int i = 0; i < more_obstacles; i++){
        vector<int> new_obstacle = vector<int>(2, 0); //(X, Y);
        new_obstacle[0] = ( rand()%((int)mat[0].size() - 3) ) + 1; //El obstaculo se crea en cualquier direccion en x
        new_obstacle[1] = 1; // Los primeros obstaculos aparecen en la 2da linea
        obstacles.push_back(new_obstacle);
      }
    }

    obstacles[0][1] += 1; //El primer obstculo baja un grado para abajo
    if(obstacles[0][1] >= (int)mat.size() - 1){
      obstacles[0][0] = ( rand()%((int)mat[0].size() - 3) ) + 1;
      obstacles[0][1] = 0;
    }
    for(unsigned int i = 1; i < obstacles.size(); i++){
      if( ((obstacles[i-1][1] - distance) > 0) || (obstacles[i-1][1] == 0) ){
        obstacles[i][1] += 1;
      }
      if(obstacles[i][1] >= (int)mat.size() ){
        obstacles[i][0] = ( rand()%((int)mat[0].size() - 3) ) + 1;
        obstacles[i][1] = 0;
      }
    }
    //cout<<"Finalizando Obstaculos"<<endl;

}

void client_death_message(){
  cout<<"                                                                       "<<endl;
  cout<<"                                                                       "<<endl;
  cout<<"                                                                       "<<endl;
  cout<<"                                                                       "<<endl;
  cout<<"                              HAZ MUERTO                               "<<endl;
  cout<<"                                                                       "<<endl;
  cout<<"                                                                       "<<endl;
  cout<<"                                                                       "<<endl;
  cout<<"                                                                       "<<endl;
}

void base_way(vector<vector<char>>& mat, int way_state){ //way_state es una variable para simular el efecto de animación
  int distance_sides = 3;
  for(unsigned int i = 0; i < mat.size(); i++){
    for(unsigned int j  = 0; j < mat[i].size(); j++){
      mat[i][j] = ' ';
    }
  }
  for(unsigned int i = way_state; i < mat.size(); i+=distance_sides){
    mat[i][0] = '|';
    mat[i][mat[0].size()-1] = '|';
  }
}

void print_matrix(vector<vector<char>>& mat){
  for(unsigned int i = 0; i < mat.size(); i++){
    for(unsigned int j = 0; j < mat[0].size(); j++){
      cout<< mat[i][j];
    }
    cout<<endl;
  }
  cout<<endl;
}

void server_print_way(vector<vector<char>>& mat, vector<vector<int>>& obstacles, int way_state){ //Servicio de impresion para el servidor
  base_way(mat, way_state);
  for(unsigned int i = 0; i < runners.size(); i++){
    if(runners[i]->is_alive){
      //Graficando carrros para cada corredor
      mat[ runners[i]->y_head - 1 ][ runners[i]->x_head - 1 ] = '0';
      mat[ runners[i]->y_head - 1 ][ runners[i]->x_head + 1 ] = '0';
      mat[ runners[i]->y_head ][ runners[i]->x_head ] = runners[i]->avatar[0];
      mat[ runners[i]->y_head + 1 ][ runners[i]->x_head - 1 ] = '0';
      mat[ runners[i]->y_head + 1 ][ runners[i]->x_head + 1 ] = '0';
    }
  }
  // Graficando los obstaculos
  for(unsigned int i = 0; i < obstacles.size(); i++){
    mat[ obstacles[i][1] ][ obstacles[i][0] ] = '*';
  }

  print_matrix(mat);
}

string server_matriz_to_string(vector<vector<char>>& mat, vector<vector<int>>& obstacles, int way_state){
  stringstream answer;
  vector<c_car*> temp;
  for(unsigned int i = 0; i < runners.size(); i++){
    if(runners[i]->is_alive){
      temp.push_back(runners[i]);
    }
  }

  answer << way_state << SPACE_SYMBOL
         << (int)mat.size() << SPACE_SYMBOL
         << (int)mat[0].size() << SPACE_SYMBOL
         << (int)temp.size();
  for(unsigned int i = 0; i < temp.size(); i++){
    answer << SPACE_SYMBOL << temp[i]->avatar << SPACE_SYMBOL << temp[i]->x_head << SPACE_SYMBOL << temp[i]->y_head;
  }
  answer << SPACE_SYMBOL << (int)obstacles.size();
  for(unsigned int i = 0; i < obstacles.size(); i++){
    answer << SPACE_SYMBOL << obstacles[i][0] /*X*/ << SPACE_SYMBOL << obstacles[i][1]/*Y*/;
  }
  return answer.str();
}

void client_string_to_matriz(string message){
  stringstream ss(message);
  string local_status;

  getline(ss, local_status, SPACE_SYMBOL);
  int way_state = stoi(local_status); //Estado de la pista
  getline(ss, local_status, SPACE_SYMBOL);
  int way_y_size = stoi(local_status);
  getline(ss, local_status, SPACE_SYMBOL);
  int way_x_size = stoi(local_status);

  vector<vector<char>> client_way = vector<vector<char>>(way_y_size, vector<char>(way_x_size, ' '));
  base_way(client_way, way_state);
  //HASTA ACA LA INFO DE LA MATRIZ SIN JUGADORES

  getline(ss, local_status, SPACE_SYMBOL);
  int runner_number = stoi(local_status);
  for(int i = 0; i < runner_number; i++){
    getline(ss, local_status, SPACE_SYMBOL);
    string local_avatar = local_status;
    getline(ss, local_status, SPACE_SYMBOL);
    int x_pos = stoi(local_status);
    getline(ss, local_status, SPACE_SYMBOL);
    int y_pos = stoi(local_status);

    client_way[y_pos - 1][x_pos - 1] = '0';
    client_way[y_pos - 1][x_pos + 1] = '0';
    client_way[y_pos][x_pos] = local_avatar[0];
    client_way[y_pos + 1][x_pos - 1] = '0';
    client_way[y_pos + 1][x_pos + 1] = '0';
  }
  getline(ss, local_status, SPACE_SYMBOL);
  int obstacles_number = stoi(local_status);
  for(int i = 0; i < obstacles_number; i++){
    getline(ss, local_status, SPACE_SYMBOL);
    int x_pos = stoi(local_status);
    getline(ss, local_status, SPACE_SYMBOL);
    int y_pos = stoi(local_status);

    client_way[y_pos][x_pos] = '*';
  }
  print_matrix(client_way);

}
/*--------------------------------------------------------------------------------------*/

#endif
