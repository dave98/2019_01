#ifndef CLIENT_H
#define CLIENT_H

#include "extra.h"

void ClientService()
{
    struct sockaddr_in stSockAddr;
    int port = 3030;
    string client_name;
    int Res;
    int n;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    cout << endl << "Nick: ";
    cin >> client_name;

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port);
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);

    if (connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)) < 0){
        cout<<"Error en la conexion"<<endl;
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    cout<<"[Conectado]"<<endl;
    bool end_chat = false;

    //Desplegando threads
    thread t_send(ChatSend, SocketFD, client_name, ref(end_chat));
    thread t_recive(ChatRecive, SocketFD, ref(end_chat));

    t_send.detach();
    t_recive.detach();

    do
    {
    }
    while (end_chat == false);

    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);
}

#endif
