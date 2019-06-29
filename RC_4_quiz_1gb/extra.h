#ifndef EXTRA_H
#define EXTRA_H

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
#include <iostream>
#include "file.h"

using namespace std;

#define CLEAR cout << "\033[2J\033[1;1H" // Limpia la consola

const string reset("\033[0m");
const string red("\033[0;31m");
const string green("\033[0;32m");
const string magenta("\033[0;35m");

string CreateHeader(int message_size_int)
{
    string header = "";
    string message_size_str = to_string(message_size_int);
    for (int i = 0; i < ( 4 - message_size_str.size() ); i++)
        header += "0";

    return header + message_size_str;
}

string ToString(char* input)
{
    string output(input);

    return output;
}

void ChatSend(int chat_socket, string user_name, bool& end_chat)
{
    //char* buffer = new char[1024];
    char buffer[1024];
    string fileName;
    int packageSize = 0;
    int n;
    string message_pool;

    cout << red << "- Send started -" << reset << endl;

    while (end_chat == false){
        cout << "Nombre del archivo a enviar: "; cin >> fileName;
        //ActionFD_PACKAGE(buffer, fileName, user_name, 'F', packageSize);
        message_pool = my_package(fileName, user_name, 'F');

        if(message_pool.size() > 1000){ //Limite maximo para enviar


          for(int position_in_chain = 0; position_in_chain < message_pool.size(); position_in_chain+=1000){
            //cout<<position_in_chain<<" - "<<position_in_chain+1000<<endl;
            string sub_chain = message_pool.substr(position_in_chain, 1000);
            //cout<<sub_chain<<endl<<sub_chain.size()<<endl<<endl;
            strcpy(buffer, sub_chain.c_str());
            if(write(chat_socket, buffer, strlen(buffer)) < 0){
              cout<<"Error en el envio"<<endl;
            }
            cout<<buffer<<endl;
            memset(buffer, 0, sizeof(buffer));
          }

          strcpy(buffer, "fin");
          if(write(chat_socket, buffer, 1000) < 0){
            cout<<"Error en el ultimo envio"<<endl;
          }
          memset(buffer, 0, sizeof(buffer));

        }

        //n = write(chat_socket, buffer, packageSize);
        //if (n < 0) perror("ERROR: Writing to socket");

    }
}

void ChatRecive(int chat_socket, bool& end_chat)
{
    string formater;
    char buffer[1024];
    int n = 0;
    string message_pool;

    cout << "- Recive started -" << endl;

    while (end_chat == false){
        if(read(chat_socket, buffer, 1000) < 0){
          cout<<"Error leendo paquetes"<<endl;
        }

        formater = buffer;
        cout<<formater<<endl;
        memset(buffer, 0, sizeof(buffer));  //Limpieza de buffer

        if(formater == "fin"){
          cout<<"Mensaje enviado completamente"<<endl;
          cout<<message_pool<<endl;
        }

        else{
            if(n == 0){
              stringstream ss(formater);
              string local;

              getline(ss, local, '#'); //
              getline(ss, local, '#'); //action
              getline(ss, local, '#'); //nick size
              getline(ss, local, '#'); //nick
              getline(ss, local, '#'); //nombre ARCHIVO
              getline(ss, local, '#'); //tamaño
              getline(ss, local, '#'); //action

              message_pool = local;
            }
            else{
              message_pool += formater;
            }
        }


//        int header = Read_INT(buffer, 0, 4);
//        cout << "Header: " << header << endl;

        // LEYENDO LA ACCION --------------------------------------------------

//        n = read(chat_socket, buffer, 1);
//        if (n < 0) perror("ERROR: Reading from socket");

//        char action = buffer[0];
//        cout << "Action: " << action << endl;

        // EJECUTANDO LA ACCION --------------------------------------------------
/*
        if (action == 'F')
        {
            // LEYENDO EL TAMANO DEL NICKNAME --------------------------------------------------

            n = read(chat_socket, buffer, 2);
            if (n < 0) perror("ERROR: Reading from socket");

            int nicknameSize = Read_INT(buffer, 0, 2);
            cout << "Nickname size: " << nicknameSize << endl;

            // LEYENDO EL NICKNAME --------------------------------------------------

            n = read(chat_socket, buffer, nicknameSize);
            if (n < 0) perror("ERROR: Reading from socket");

            string nickname = Read_STR(buffer, 0, nicknameSize);
            cout << "Nickname: " << nickname << endl;

            // LEYENDO EL NOMBRE DEL ARCHIVO --------------------------------------------------

            n = read(chat_socket, buffer, header);
            if (n < 0) perror("ERROR: Reading from socket");

            string fileName = Read_STR(buffer, 0, header);
            cout << "File name: " << fileName << endl;

            // LEYENDO EL TAMANO DEL ARCHIVO --------------------------------------------------

            n = read(chat_socket, buffer, 4);
            if (n < 0) perror("ERROR: Reading from socket");

            int fileSize = Read_INT(buffer, 0, 4);
            cout << "File size: " << fileSize << endl;

            // LEYENDO EL ARCHIVO BINARIO --------------------------------------------------

            n = read(chat_socket, buffer, fileSize);
            if (n < 0) perror("ERROR: Reading from socket");

            Read_BIN(buffer, 0, nickname + "_" + fileName, fileSize);
            cout << "The file has been saved as: " << nickname + "_" + fileName << endl;
        }
*/
    }

}

#endif
