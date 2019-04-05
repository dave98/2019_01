/* Server code in C */
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

int main(void)
{
  int port = 4000; //Especificamos el puerto
  char msg[1024]; //Size of the message

  sockaddr_in server_Adress;
  bzero((char*)&server_Adress, sizeof(server_Adress));
  server_Adress.sin_family = AF_INET;
  server_Adress.sin_addr.s_addr = htonl(INADDR_ANY);
  server_Adress.sin_port = htons(port);

  int status_ser = socket(AF_INET, SOCK_STREAM, 0);
  if(status_ser < 0){cout<<"Errro estableciendo el socket"<<endl; exit(0);}

  //BINDING
  int status_bind = bind(status_ser, (struct sockaddr*)&server_Adress, sizeof(server_Adress));
  if(status_bind < 0){cout<<"Error durante enlace de socket"<<endl; exit(0);}

  cout<<"[Esperando]"<<endl;
  listen(status_ser, 10);//Definido entre SOCK_STREAM o SOCK_SEQPACKET
                         //Maximo de conexiones en cola
  //A partir de acá esperamos una conexion por parte del cliente
  sockaddr_in new_socket;//Recibia conexion, creamos un nuevo socket para contestar
  socklen_t new_socket_size = sizeof(new_socket);
  //Conexion aceptada
  int n_socket_accept =  accept(status_ser, (sockaddr*)&new_socket, &new_socket_size);
  if(n_socket_accept < 0){cout<<"No se pudo acceptar la conexion"<<endl; exit(0);}
  //A este la conexion se ha compeltado
  while(1){
    cout<<"Esperando respuesta del cliente"<<endl;
    memset(&msg, 0, sizeof(msg)); //Buffer limpio
    recv(n_socket_accept, (char*)&msg, sizeof(msg), 0);
    //Analizar si el cliente ha cerrado su sesion
    cout<<"Cliente: [ "<<msg<<" ]"<<endl;

    string my_message;
    cin>>my_message;
    memset(&msg, 0, sizeof(msg));//Buffer limpio antes de enviar nuestro mensaje
    strcpy(msg, my_message.c_str());//Paso de string al buffer
    send(n_socket_accept, (char*)&msg, strlen(msg), 0);//Enviamos el mensaje al buffer
  }

  return 0;
}
