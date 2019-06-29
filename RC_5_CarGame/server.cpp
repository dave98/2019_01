/* Server code in C */
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<fcntl.h> // for open
#include<unistd.h> // for close
#include<pthread.h>
#include<chrono>
#include<thread>
#include"c_car.h"
#include"PlayerControl.h"

char client_message[2000]; //Buffer de recepcion
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int server_socket_use = 0;
extern vector<c_car*> runners;
extern int server_actual_state;
vector<vector<char>> mat = vector<vector<char>>(20, vector<char>(90, ' '));
vector<vector<int>> obstacles = vector<vector<int>>(0, vector<int>(2, 0));

void* multiple_answer_thread(int arg){
  //cout<<__PRETTY_FUNCTION__<<endl;

  char client_message[1024];
  char my_buffer[1024];
  string formater;

  int newSocket = arg;
  recv(newSocket, client_message, 1024, 0);
  //cout<<"[Mensaje Recibido]: "<<client_message<<" [Size]: "<<strlen(client_message)<<endl;

  formater = client_message;
  formater = server_request_interpreter(formater, mat, obstacles);

  strcpy(buffer, formater.c_str());
  send(newSocket, buffer, sizeof(buffer), 0);

  close(newSocket);
}

void* print_matriz_thread(){
  bool is_continue = true;
  int limit_state = 3;
  while(is_continue){
    server_update_players(mat, obstacles);
    server_update_obstacles(mat, obstacles);
    server_print_way(mat, obstacles, server_actual_state);

    //Definiendo animacion a la pista
    server_actual_state++;
    if(server_actual_state >= limit_state){
      server_actual_state = 0;
    }
    cout << "\x1B[2J\x1B[H"; //Cleaning Screen
    //sleep(1); //Time within rendered
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}


int main(){
  int serverSocket, newSocket;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage; //sockaddr_storage equivalente a sockaddr_in o sockaddr_in6

  socklen_t addr_size;
  //Create the socket.
  serverSocket = socket(PF_INET, SOCK_STREAM, 0);
  // Configure settings of the server address struct
  serverAddr.sin_family = AF_INET;   // Address family = Internet
  serverAddr.sin_port = htons(7799); //Estableciendo el puerto, no puede ser menor a 1024
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");    //Set IP address to localhost
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);   //Set all bits of the padding field to 0
  bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));   //Bind the address struct to the socket
  if(listen(serverSocket,50)==0){ cout<<"[Listening]"<<endl; }   //Listen on the socket, with 40 max connection requests queued
  else{ cout<<"[Not listening]"<<endl; }
  pthread_t tid[60];
  pthread_t chariot;
  int i = 0;

  //Lanzamos la impresion de la matriz
  thread(print_matriz_thread).detach();
  while(1){
    //Accept call creates a new socket for the incoming connection
    addr_size = sizeof(serverStorage);
    newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
    thread(multiple_answer_thread, newSocket).detach();
  }
  return 0;
}
