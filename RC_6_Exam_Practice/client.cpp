#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#include <string>
#include <strings.h>
#include <string.h>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

char message[1000];
char buffer[1024];

int main(){
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

  cout<<"[Conectado]"<<endl;
  string formater;
  cout<<"[Write your message]: ";
  cin>>formater;

  strcpy(message, formater.c_str());
  if(send(clientSocket , message , strlen(message) , 0) < 0){
    cout<<"[Fallo envio en primera conexion]"<<endl;
  }
  if(recv(clientSocket, buffer, 1024, 0) < 0){
    cout<<"[Fallo de lectura del servidor en primera conexion]"<<endl;
  }

  formater = buffer;
  cout<<"[Respuesta del Servidor]: "<<formater<<endl;
  close(clientSocket);
  return 0;
}
