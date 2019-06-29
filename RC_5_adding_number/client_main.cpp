#include<iostream>
#include"extra.h"

using namespace std;

int main(){
    struct sockaddr_in stSockAddr;
    struct sockaddr_in stSockAddr_2;
    int port = 3030;
    int port_2 = 3031;
    string client_name;
    int Res;
    int n;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int SocketFD_2 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


    cout<<"Socket 1 - Client: "<<SocketFD<<endl;
    cout<<"Socket 2 - Client: "<<SocketFD_2<<endl;


    cout << "Client" << endl;

    //Socket_1
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port);
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);
    if (connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)) < 0){
        cout<<"Error en la conexion 1"<<endl;
        close(SocketFD);
        exit(EXIT_FAILURE);
    }


    cout<<"[Conectado]"<<endl;
    bool end_chat = false;

    //Desplegando threads
    thread t_send(ChatSend, SocketFD, client_name, ref(end_chat));
    thread t_recive(ChatRecive_Client, SocketFD, ref(end_chat));

    t_send.detach();
    t_recive.detach();

    do
    {
    }
    while (end_chat == false);

    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);
  return 0;
}
