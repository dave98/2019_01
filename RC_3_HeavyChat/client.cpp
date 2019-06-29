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

using namespace std;
int n_thread = 0;

void* cientThread(void *arg)
{
  cout<<"En hilo: "<<n_thread<<endl; n_thread++;
  char message[1024];
  char buffer[1024];
  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  // Create the socket.
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  //Configure settings of the server address
 // Address family is Internet
  serverAddr.sin_family = AF_INET;
  //Set port number, using htons function
  serverAddr.sin_port = htons(7799);
 //Set IP address to localhost
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  //Connect the socket to the server using the address
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);



  strcpy(message,"Hello");



  if( send(clientSocket , message , strlen(message) , 0) < 0){
    printf("Send failed\n");
  }
  //Read the message from the server into the buffer
  if(recv(clientSocket, buffer, 1024, 0) < 0){
    printf("Receive failed\n");
  }
  //Print the received message
  printf("Data received: %s\n",buffer);
  close(clientSocket);
  pthread_exit(NULL);
}


void* first_thread_connection(void* player){
  cout<<"[Estableciendo Conexion]"<<endl;
  char message[1024];
  char buffer[1024];
  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;


  clientSocket = socket(PF_INET, SOCK_STREAM, 0);   // Create the socket.
  //Configure settings of the server address
  serverAddr.sin_family = AF_INET;  // Address family is Internet
  serverAddr.sin_port = htons(7799);   //Set port number, using htons function
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //Set IP address to localhost
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);   //Connect the socket to the server using the address
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  cout<<"[Mensaje]: "<<formater<<endl; cout<<"[Longitud de mensaje]: "<<strlen(message)<<endl;
  if( send(clientSocket , message , strlen(message) , 0) < 0){
    printf("Fallo primer envío\n");
  }
  //Read the message from the server into the buffer
  if(recv(clientSocket, buffer, 1024, 0) < 0){
    printf("Fallo recibiendo la respuesta\n");
  }
  //Print the received message
  cout<<"[Respuesta del Servidor]: "<<buffer<<endl;
  close(clientSocket);
  pthread_exit(NULL);
}


int main(){
  string local_nick;
  string local_message;
  pthread_t from_server;
  pthread_t to_server;

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


//ESTABLECIENDO PRIMERA CONEXION
  if(pthread_create(&to_server, NULL, first_thread_connection, reinterpret_cast<void* >(playe1r) ) != 0){
    cout<<"[Imposible establecer el avatar en el servidor]"<<endl;
  }
  sleep(1);
  pthread_join(to_server, NULL);
    cout<<"[Avatar establecido]"<<endl;

/*
  cin>>local_message;
  strcpy(message, local_message.c_str());
  if(send(clientSocket, message, strlen(message), 0) < 0){cout<<"Error enviando al servidor";} //Enviando mensaje al servidor
  if(recv(clientSocket, buffer, 1024, 0) < 0){ cout<<"No se recibió la respuesta del servidor";} //Recibiendo respuesta del servidor

  cout<<"[Respuesta del servidor]"<<endl;
  cout<<buffer<<endl;
  close(clientSocket);
  return 0;
*/}

/*
int main(){
  int i = 0;
  pthread_t tid[51];
  while(i< 50){
    if( pthread_create(&tid[i], NULL, cientThread, NULL) != 0 ) //ID de thread nuevo, atributos del thread, funcion, y parametros de la misma
           printf("Failed to create thread\n"); //En caso de error.
    i++;
  }

  sleep(20);
  i = 0;

  while(i< 50){
     pthread_join(tid[i++],NULL); //Uniendo Thread i con el valor de salida del mismo en Null
     printf("%d:\n",i);
  }
  return 0;
}*/

/*#include <iostream>
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
*/
