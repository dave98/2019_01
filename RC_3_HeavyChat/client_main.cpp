#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <thread>
#include <unistd.h>
#include "file.h"

using namespace std;


string CreateHeader(int);
string ToString(char*);
void ChatSend(int, string, bool&);
void ChatRecive(int, bool&);


string CreateHeader(int message_size_int){
    string header = "";
    string message_size_str = to_string(message_size_int);
    for (int i = 0; i < ( 4 - message_size_str.size() ); i++)
        header += "0";
    return header + message_size_str;
}

string ToString(char* input){
    string output(input);
    return output;
}

void ChatSend(int chat_socket, string user_name, bool& end_chat){
    char* buffer = new char[8192];
    string fileName;
    int packageSize = 0;
    int n;

    do{
        ActionFD_PACKAGE(buffer, fileName, user_name, 'F', packageSize);
        n = write(chat_socket, buffer, packageSize);
    } while (end_chat == false);
}

void ChatRecive(int chat_socket, bool& end_chat){
    char* buffer = new char[8192];
    int n;
    do{
        n = read(chat_socket, buffer, 4); //Reading from socket
        int header = Read_INT(buffer, 0, 4);

        n = read(chat_socket, buffer, 1); //Reading action
        char action = buffer[0];
        cout << "Action: " << action << endl;

        if (action == 'F'){
            // LEYENDO EL TAMANO DEL NICKNAME --------------------------------------------------
            n = read(chat_socket, buffer, 2);
            int nicknameSize = Read_INT(buffer, 0, 2);

            // LEYENDO EL NICKNAME --------------------------------------------------
            n = read(chat_socket, buffer, nicknameSize);
            string nickname = Read_STR(buffer, 0, nicknameSize);
            // LEYENDO EL NOMBRE DEL ARCHIVO --------------------------------------------------

            n = read(chat_socket, buffer, header);
            string fileName = Read_STR(buffer, 0, header);

            // LEYENDO EL TAMANO DEL ARCHIVO --------------------------------------------------
            n = read(chat_socket, buffer, 4);
            int fileSize = Read_INT(buffer, 0, 4);

            // LEYENDO EL ARCHIVO BINARIO --------------------------------------------------
            n = read(chat_socket, buffer, fileSize);
            Read_BIN(buffer, 0, nickname + "_" + fileName, fileSize);
        }
        int wait; cin >> wait;
    }while (end_chat == false);
}

int main()
{
  struct sockaddr_in stSockAddr;
  int port;
  string client_name;
  int Res;
  int n;

  int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  cout << endl << "Username: ";
  cin >> client_name;
  cout << "Port: ";
  cin >> port;

  memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = htons(port);
  Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);

  bool end_chat = false;
  thread t_send(ChatSend, SocketFD, client_name, ref(end_chat));
  thread t_recive(ChatRecive, SocketFD, ref(end_chat));
  cout<<"fLAG"<<endl;

  t_send.detach();
  t_recive.detach();

  do{
     // WAITING CHAT TO END
  }
  while (end_chat == false);
  shutdown(SocketFD, SHUT_RDWR);
  close(SocketFD);
}
