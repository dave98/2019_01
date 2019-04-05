/* Client code in C */
#include <iostream>
#include <string>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

int main(){
  int port = 4000;
  char msg[1024];

  struct hostent* host = gethostbyname("127.0.0.1");
  sockaddr_in client_Adress;
  bzero((char*)&client_Adress, sizeof(client_Adress));
  client_Adress.sin_family = AF_INET;
  client_Adress.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
  client_Adress.sin_port = htons(port);

  int status_cli = socket(AF_INET, SOCK_STREAM, 0);
  //Iniciando la conexion
  int status_conect = connect(status_cli, (sockaddr*)&client_Adress, sizeof(client_Adress));

  if(status_conect < 0){ cout<<"Fallo conexion al servidor"<<endl; exit(0);}
  cout<<"Conexion establecida"<<endl;
  while(1){
    string my_message;//Enviamos nuestro mensaje
    cin>>my_message;
    memset(msg, 0, sizeof(msg));//Buffer limpio
    strcpy(msg, my_message.c_str());
    send(status_cli, (char*)&msg, sizeof(msg), 0);
    //Esperamos respuesta del servidor
    memset(&msg, 0, sizeof(msg));//Buffer limpio para respuesta del server
    recv(status_cli, (char*)&msg, sizeof(msg), 0);
    cout<<"Server: [ "<<msg<<" ]"<<endl;

  }
  return 0;
}
