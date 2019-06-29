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
#include <cstdlib>
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

    cout  <<  "What to send: " << endl;

    while (end_chat == false){
        cin>>message_pool; //Save the number
        cout<<"[Enviando]: "<<message_pool<<endl;
        strcpy(buffer, message_pool.c_str());
        if(write(chat_socket, buffer, 1000) < 0){
          cout<<"Error en el envio"<<endl;
        }
        memset(buffer, 0, sizeof(buffer)); //Cleanig pool of messages
    }
}


void ChatRecive_Server(int chat_socket, bool& end_chat)
{
    string formater;
    int number_for;
    char buffer[1024];
    int n = 0;
    string message_pool;
    stringstream pass_pool;

    cout << "What to recived " << endl;

    while (end_chat == false){
        if(read(chat_socket, buffer, 1000) < 0){
          cout<<"Error leendo paquetes"<<endl;
        }

        formater = buffer;
        number_for = atoi(formater.c_str());
        cout<<number_for<<endl;
        cout<<"[Recibiendo]: "<<number_for<<endl;

        number_for += 7;
        cout<<"[Retornando]: "<<number_for<<endl;

        memset(buffer, 0, sizeof(buffer));  //Limpieza de buffer
        pass_pool << number_for;
        message_pool = pass_pool.str();
        strcpy(buffer, message_pool.c_str());

        if(write(chat_socket, buffer, 1000) < 0){
          cout<<"Fallo en la respuesta"<<endl;
        }
        memset(buffer, 0, sizeof(buffer));  //Limpieza de buffer

    }
}


void ChatRecive_Client(int chat_socket, bool& end_chat)
{
    string formater;
    int number_for;
    char buffer[1024];
    int n = 0;
    string message_pool;
    stringstream pass_pool;

    cout << "What to recived " << endl;

    while (end_chat == false){
        if(read(chat_socket, buffer, 1000) < 0){
          cout<<"Error leendo paquetes"<<endl;
        }

        formater = buffer;
        number_for = atoi(formater.c_str());
        cout<<number_for<<endl;
        cout<<"[Recibiendo Client]: "<<number_for<<endl;

    }
}


#endif
