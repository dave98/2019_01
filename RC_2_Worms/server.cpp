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
#include<thread>
#include"c_worm.h"
#include"PlayerControl.h"

char client_message[2000]; //Buffer de recepcion
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int server_socket_use = 0;
extern vector<c_worm*> players;
vector<vector<char>> mat = vector<vector<char>>(20, vector<char>(90, 0));
vector<vector<int>> fruits = vector<vector<int>>(0, vector<int>(2, 0));


void * socketThread(void *arg){
  int newSocket = *((int *)arg);
  recv(newSocket , client_message , 2000 , 0); //Funcion que recibe mensajes de un socket
                                               //Retorna el tamaño del buffer descartando los bytes que no entran en el buffer
                                              //Si no hay mensajes en el socket, se espera hasta recibir uno. Se produce un error en caso el socket este
                                              // en modo bloqueante
  // Send message to the client socket
  pthread_mutex_lock(&lock); // Bloqueo for race conditions.
  char* message = (char*)malloc(sizeof(client_message)+20); //
  strcpy(message,"Hello Client : ");
  strcat(message,client_message);
  strcat(message,"\n");
  strcpy(buffer,message);
  free(message); //Liberamos el espacio reservado en malloc
  pthread_mutex_unlock(&lock);// Desbloqueamos el thread

  sleep(1);
  send(newSocket,buffer,sizeof(buffer),0); //Transmitir el mensaje a otro socket
  cout<<"Saliendo del socket de recepcion en server: "<<server_socket_use<<endl; server_socket_use++;
  close(newSocket);
  pthread_exit(NULL);
}
void* answer_thread(void* arg){
  char client_message[1024];
  char buffer[1024];
  string client_string;

  int newSocket = *((int*) arg);
  recv(newSocket, client_message, 1024, 0);
  cout<<"[Mensaje Recibido]"<<endl; cout<<client_message<<endl;
  client_string = client_message;
  server_interpreter(client_string, mat, fruits);


  pthread_mutex_lock(&lock);
  char* message = (char*)malloc(sizeof(client_message) + 20);
  strcpy(message, "[Cliente Recibido]: ");
  strcat(message, client_message); strcat(message, "\n");
  strcpy(buffer, message);
  free(message);
  pthread_mutex_unlock(&lock);

  sleep(1);
  send(newSocket, buffer, sizeof(buffer), 0);
  close(newSocket);
  pthread_exit(NULL);
}
void* multiple_answer_thread(int arg){
  //cout<<__PRETTY_FUNCTION__<<endl;

  char client_message[1024];
  char my_buffer[1024];
  string formater;

  int newSocket = arg;
  recv(newSocket, client_message, 1024, 0);
  //cout<<"[Mensaje Recibido]: "<<client_message<<" [Size]: "<<strlen(client_message)<<endl;

  formater = client_message;
  formater = server_interpreter(formater, mat, fruits);

  strcpy(buffer, formater.c_str());
  send(newSocket, buffer, sizeof(buffer), 0);

  close(newSocket);
}

void* print_matriz_thread(){
  bool is_continue = true;
  while(is_continue){
    update_players(mat, fruits);
    update_fruits(mat, fruits);
    print_matrix_server(mat, fruits);
    cout << "\x1B[2J\x1B[H";
    sleep(1);
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
