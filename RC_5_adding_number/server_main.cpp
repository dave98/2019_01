#include<iostream>
#include"extra.h"

using namespace std;



int main(){
  struct sockaddr_in stSockAddr;
  struct sockaddr_in stSockAddr_2;
  int port = 3030;
  int port_2 = 3031;
  string host_name;
  int n;
  int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  int SocketFD_2 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  cout<<"Socket 1 - Server: "<<SocketFD<<endl;
  cout<<"Socket 2 - Server: "<<SocketFD_2<<endl;

  cout<<"Server" <<endl;

  memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = htons(port);
  stSockAddr.sin_addr.s_addr = INADDR_ANY;

  memset(&stSockAddr_2, 0, sizeof(struct sockaddr_in));
  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = htons(port_2);
  stSockAddr.sin_addr.s_addr = INADDR_ANY;


  if ( bind(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)) < 0 ){
      cout<<"Error durante binding"<<endl;
      close(SocketFD);
      exit(EXIT_FAILURE);
  }

  if ( bind(SocketFD_2, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)) < 0 ){
     cout<<"Error durante binding"<<endl;
     close(SocketFD_2);
     exit(EXIT_FAILURE);
  }


  if (listen(SocketFD, 10) < 0){
      cout<<"Error en el listen"<<endl;
      close(SocketFD);
      exit(EXIT_FAILURE);
  }

  if (listen(SocketFD_2, 10) < 0){
      cout<<"Error en el listen"<<endl;
      close(SocketFD);
      exit(EXIT_FAILURE);
  }


  for(;;){
      cout << "[Listening]: "  << endl;
      int ConnectFD;
      ConnectFD = accept(SocketFD, NULL, NULL);
      if (0 > ConnectFD){
          cout<<"Error durante accept"<<endl;
          close(SocketFD);
          exit(EXIT_FAILURE);
      }

      bool end_chat = false;

      //Desplegando threads
      thread t_send(ChatSend, ConnectFD, host_name, ref(end_chat));
      thread t_recive(ChatRecive_Server, ConnectFD, ref(end_chat));

      t_send.detach();
      t_recive.detach();

      do{
      }
      while (end_chat == false);

      shutdown(ConnectFD, SHUT_RDWR);
      close(ConnectFD);
  }

  close(SocketFD);
  return 0;
}
