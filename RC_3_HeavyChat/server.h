#ifndef SERVER_H
#define SERVER_H

#include "extra.h"

void HostService(){
    struct sockaddr_in stSockAddr; //Socket Data
    int port;
    string host_name;
    int n;

    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //Socket Initialization

    cout << endl << "Username: "; // Conexion Configuration
    cin >> host_name;
    cout << "Port: ";
    cin >> port;

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in)); // Socket Starting
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    bind(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in));
    listen(SocketFD, 50);

    for(;;) //Chat started
    {
        cout << "[Listening]" << endl;
        int ConnectFD;
        ConnectFD = accept(SocketFD, NULL, NULL);
        if (0 > ConnectFD) {
            cout<<"Error aceptando la conexion"<<endl;
            close(SocketFD);
            exit(0);
        }

        bool end_chat = false;

        // CREANDO Y INICIANDO LAS THREADS --------------------------------------------------

        thread t_send(ChatSend, ConnectFD, host_name, ref(end_chat));
        thread t_recive(ChatRecive, ConnectFD, ref(end_chat));

        t_send.detach();
        t_recive.detach();

        do
        {
           // WAITING CHAT TO END
        }
        while (end_chat == false);
        shutdown(ConnectFD, SHUT_RDWR);
        close(ConnectFD);
    }

    close(SocketFD);
}

#endif
