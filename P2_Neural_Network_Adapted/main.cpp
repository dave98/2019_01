/*
    main.cpp - Multilayer Perceptron Main
    Created by Alberto Tudela, March 13 2013
    Under license GPLv3
*/

#include <iostream>
#include <vector>
#include "MLP.hpp"

using namespace std;

string output = "redxor.sinapsis";

int main(){

  MLP perce(4, 17, 12, 3);
  perce.SetExternalConfiguration(0.01, 0.005, 1000);
  cout << perce.Leer("non_sintetic.txt") << endl;;
  //MLP perce("redxor.sinapsis");

//system ("pause");
vector<double> eval(17, 0.0);
vector<double> answer;

//0.182712 0.850151 0.697829
eval[0] = 0.667703; //+
eval[1] = 0.0968609; //+
eval[2] = 0.663889; //+
eval[3] = 0.86936; //+
eval[4] = 0.609415; //+
eval[5] = 0.805887; // +
eval[6] = 0.693463; // +
eval[7] = 0.387296; // +
eval[8] = 0.39948; // +
eval[9] = 0.801968; // +
eval[10] = 0.3754; // +
eval[11] = 0.111926; // +
eval[12] = 0.115739; // +
eval[13] =  0.137557; //+
eval[14] = 0.195687; //+
eval[15] = 0.163212 ; //+
eval[16] =  0.638789 ; //+

answer =  perce.Activar(eval);
for(unsigned int i = 0; i < answer.size(); i++){
  cout<<i<<" -> "<<answer[i]<<endl;
}

cout<<"[FINALIZADO]"<<endl;
perce.Guardar(&output[0]);
//cout<<"[GUARDADO]"<<endl;

return 0;
}


/*
//MLP perce(3, 2, 2, 1); //Num de capas (siempre mayor o igual a 3),
                       //Numero de neuronas de entrada
                       //Numero de neuronas en capas intermedias
                       //Numero de neuronas de salida
//MLP perce("redxor.sinapsis");

double errorT;
vector<vector<double> > pentrada(4,vector<double>(2)) , psalida(4,vector<double>(2));
vector<double> yp;



//Seccion de entrenamiento
pentrada[0][0]=0; pentrada[0][1]=0; psalida[0][0]=0;
pentrada[1][0]=0; pentrada[1][1]=1; psalida[1][0]=1;
pentrada[2][0]=1; pentrada[2][1]=0; psalida[2][0]=1;
pentrada[3][0]=1; pentrada[3][1]=1; psalida[3][0]=0;

//perce.MostrarPesos(); perce.MostrarUmbrales();
//perce.MostrarActivaciones(); perce.MostrarDeltas();
//perce.Leer("abelone.train");


int rep = perce.Entrenar(pentrada, psalida, errorT, 0.00001);
perce.Guardar(output);
perce.MostrarPesos(); perce.MostrarUmbrales();
perce.MostrarActivaciones(); perce.MostrarDeltas();

cout<<"El error Total cometido es: "<<errorT<<" y se ha alcanzado en "<<rep<<" repeticiones."<<endl;
pentrada[0][0]=0; pentrada[0][1]=0; psalida[0][0]=0; yp=perce.Activar(pentrada[0]);
cout<<"Para el vector ("<<pentrada[0][0]<<","<<pentrada[0][1]<<") -> y= "<<(yp[0])<<endl;
pentrada[1][0]=0; pentrada[1][1]=1; psalida[1][0]=1; yp=perce.Activar(pentrada[1]);
cout<<"Para el vector ("<<pentrada[1][0]<<","<<pentrada[1][1]<<") -> y= "<<(yp[0])<<endl;
pentrada[2][0]=1; pentrada[2][1]=0; psalida[2][0]=1; yp=perce.Activar(pentrada[2]);
cout<<"Para el vector ("<<pentrada[2][0]<<","<<pentrada[2][1]<<") -> y= "<<(yp[0])<<endl;
pentrada[3][0]=1; pentrada[3][1]=1; psalida[3][0]=0; yp=perce.Activar(pentrada[3]);
cout<<"Para el vector ("<<pentrada[3][0]<<","<<pentrada[3][1]<<") -> y= "<<(yp[0])<<endl;
*/
