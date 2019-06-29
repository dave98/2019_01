/* Client code in C */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#include <iostream>
#include <string>
#include <strings.h>
#include <string.h>
#include <fstream>
#include <thread>
#include <chrono>
#include "c_car.h"
#include "PlayerControl.h"
using namespace std;
int n_thread = 0;
bool is_cont = true;


void* first_thread_connection(void* player){
  cout<<"[Estableciendo Conexion]"<<endl;
  char message[1024];
  char buffer[1024];
  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  string formater;
  c_car* _player = reinterpret_cast<c_car*>(player);


  clientSocket = socket(PF_INET, SOCK_STREAM, 0);   // Create the socket.
  //Configure settings of the server address
  serverAddr.sin_family = AF_INET;  // Address family is Internet
  serverAddr.sin_port = htons(7799);   //Set port number, using htons function
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //Set IP address to localhost
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);   //Connect the socket to the server using the address
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  formater = _player->first_connection_request();
  strcpy(message, formater.c_str());
  //cout<<"[Mensaje]: "<<formater<<endl; cout<<"[Longitud de mensaje]: "<<strlen(message)<<endl;
  if(send(clientSocket , message , strlen(message) , 0) < 0){
    cout<<"[Fallo envio en primera conexion]"<<endl;
  }
  //Read the message from the server into the buffer
  if(recv(clientSocket, buffer, 1024, 0) < 0){
    cout<<"[Fallo de lectura del servidor en primera conexion]"<<endl;
  }
  formater = buffer;
  _player->to_class(formater);

  if(_player->is_alive == false){
    is_cont = false;
    client_death_message();
    sleep(3);
    exit(0);
  }

  close(clientSocket);
  pthread_exit(NULL);
}

void* player_update_thread(void* player){
  //cout<<"[Actualizando a jugador]"<<endl;
  char my_message[1024];
  char server_buffer[1024];
  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  c_car* _player = reinterpret_cast<c_car*>(player);
  string formater;

  clientSocket = socket(PF_INET, SOCK_STREAM, 0);   // Create the socket.
  //Configure settings of the server address
  serverAddr.sin_family = AF_INET;  // Address family is Internet
  serverAddr.sin_port = htons(7799);   //Set port number, using htons function
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //Set IP address to localhost
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);   //Connect the socket to the server using the address
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  formater = _player->update_connection_request();
  strcpy(my_message, formater.c_str());
  if(send(clientSocket , my_message , strlen(my_message) , 0) < 0){
    cout<<"Fallo en la actualizacion del jugador"<<endl;
  }

  //Read the message from the server into the buffer
  if(recv(clientSocket, server_buffer, 1024, 0) < 0){
    cout<<"Fallo recibiendo la respuesta de la actualizacion de jugador"<<endl;
  }
  //Print the received message
  formater = server_buffer;
  _player->to_class(formater);

  if(_player->is_alive == false){
    is_cont = false;
    client_death_message();
    sleep(3);
    exit(0);
  }

  close(clientSocket);
  pthread_exit(NULL);
}

void* player_directions_thread(void* player){
  c_car* _player = reinterpret_cast<c_car*>(player);
  char dir;
  while(is_cont){
    cin>>dir;
    if(dir == 'w'){
      _player->change_direction(0, -1);
    }
    else if(dir == 's'){
      _player->change_direction(0, 1);
    }
    else if(dir == 'a'){
      _player->change_direction(-1, 0);
    }
    else if(dir == 'd'){ //for ''
      _player->change_direction(1, 0);
    }

    //cout<<"[Cambiando direccion]"<<endl;
    char my_message[1024];
    char server_buffer[1024];
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    string formater;

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);   // Create the socket.
    serverAddr.sin_family = AF_INET;  // Address family is Internet
    serverAddr.sin_port = htons(7799);   //Set port number, using htons function
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //Set IP address to localhost
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);   //Connect the socket to the server using the address
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

    formater = _player->change_direction_request();
    strcpy(my_message, formater.c_str());
    if(send(clientSocket , my_message , strlen(my_message) , 0) < 0){
      cout<<"Fallo en el cambio de direccion del jugador"<<endl;
    }

    //Read the message from the server into the buffer
    if(recv(clientSocket, server_buffer, 1024, 0) < 0){
      cout<<"Fallo recibiendo la respuesta para cambio de direccion"<<endl;
    }
    //Print the received message
    formater = server_buffer;
    _player->to_class(formater);

    if(_player->is_alive == false){
      is_cont = false;
      client_death_message();
      sleep(3);
      exit(0);
    }

    close(clientSocket);
  }
}

void* player_matrix_thread(void* player){
  while(is_cont){
    //cout<<"[Solicitando matriz de dibujo]"<<endl;
    char my_message[1024];
    char server_buffer[1024];
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    c_car* _player = reinterpret_cast<c_car*>(player);
    string formater;

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);   // Create the socket.
    //Configure settings of the server address
    serverAddr.sin_family = AF_INET;  // Address family is Internet
    serverAddr.sin_port = htons(7799);   //Set port number, using htons function
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //Set IP address to localhost
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);   //Connect the socket to the server using the address
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

    formater = _player->get_matriz_request();
    strcpy(my_message, formater.c_str());
    if(send(clientSocket , my_message , strlen(my_message) , 0) < 0){
      cout<<"Fallo en la actualizacion  de matriz del jugador"<<endl;
    }

    //Read the message from the server into the buffer
    if(recv(clientSocket, server_buffer, 1024, 0) < 0){
      cout<<"Fallo recibiendo la respuesta para actualizacion de matriz"<<endl;
    }
    //Print the received message
    formater = server_buffer;
    client_string_to_matriz(formater);
    cout << "\x1B[2J\x1B[H";
    close(clientSocket);
    //sleep(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

int main(){
  bool is_update = true;
  int frame = 0;
  string local_nick;
  string local_avatar;
  string local_message;
  pthread_t from_server;
  pthread_t to_server;
  pthread_t key_thread;
  pthread_t matriz_thread;

  char message[1000];
  char buffer[1024];
  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;


  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7799);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
  addr_size = sizeof(serverAddr);
  connect(clientSocket, (struct sockaddr*) &serverAddr, addr_size);

  cout<<"[Conexion al servidor establecida]"<<endl;
  cout<<"[Ingrese su nick]: "<<endl;
  cin>>local_nick;
  cout<<"[Ingrese su avatar]: "<<endl;
  cin>>local_avatar;

//ESTABLECIENDO PRIMERA CONEXION
  cout<<"[Creando avatar]"<<endl;
  c_car* player = new c_car();
  player->nickname = local_nick;
  player->avatar = local_avatar;
  if(pthread_create(&to_server, NULL, first_thread_connection, reinterpret_cast<void* >(player) ) != 0){
    cout<<"[Imposible establecer el avatar en el servidor]"<<endl;
    close(clientSocket);
    exit(0);
  }
  pthread_join(to_server, NULL); //Hasta aca ya se haestablecido el avatar
////////////////////////////////
  if(pthread_create(&key_thread, NULL, player_directions_thread, reinterpret_cast<void*>(player)) !=0){
    cout<<"[No se puedo iniciar el controlador del teclado]"<<endl;
    close(clientSocket);
    exit(0);
  }
  pthread_detach(key_thread); //Desplegando el thread que controla las entradas del teclado
  if(pthread_create(&matriz_thread, NULL, player_matrix_thread, reinterpret_cast<void*>(player)) !=0){
    cout<<"[No se puedo iniciar el controlador de matriz]"<<endl;
    close(clientSocket);
    exit(0);
  }
  pthread_detach(matriz_thread);
////////////////////////////////
  while(is_cont){
    //cout<<"[Iniciando actualizacion]: "<<frame<<endl; frame++;
    if(pthread_create(&to_server, NULL, player_update_thread, reinterpret_cast<void* >(player)) != 0){
      cout<<"[Imposible arrojar hilo de actualizacion]"<<endl;
      close(clientSocket);
      exit(0);
    }
    pthread_join(to_server, NULL);
    //sleep(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(600));

  }

  close(clientSocket);
  exit(0);
}
