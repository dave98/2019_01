#include<iostream>
#include"extra.h"

using namespace std;



int main(){
  struct sockaddr_in stSockAddr;
  int port = 3030;
  string host_name;
  int n;
  int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  cout << endl << "Nick: ";
  cin >> host_name;

  memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = htons(port);
  stSockAddr.sin_addr.s_addr = INADDR_ANY;

  if ( bind(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)) < 0 ){
      cout<<"Error durante binding"<<endl;
      close(SocketFD);
      exit(EXIT_FAILURE);
  }

  if (listen(SocketFD, 10) < 0){
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
      thread t_recive(ChatRecive, ConnectFD, ref(end_chat));

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
