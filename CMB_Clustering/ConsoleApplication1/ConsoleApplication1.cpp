// ConsoleApplication1.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <chrono>
#include "functions.h"

using namespace std;
using namespace chrono;

void aglomerativo_minimal_v2(vector<vector<float>>&, vector<string>&);
	float find_minimo(vector<vector<float>>&, int&, int&);
	void fix_cluster_less(vector<vector<float>>&, int, int, vector<string>&); //Junta los elementos que requieran ser acamodados en un solo cluster
	void fix_cluster_big(vector<vector<float>>&, int, int, vector<string>&); //Junta los elementos que requieran ser acamodados en un solo cluster
	void fix_cluster_prom(vector<vector<float>>&, int, int, vector<string>&); //Junta los elementos que requieran ser acamodados en un solo cluster

void disociativos(vector<vector<float>>&, vector<string>&, int);
	void get_distance_to_cluster(vector<vector<float>>&, vector<float>&);
	void get_distance_to_element(vector<vector<float>>&, vector<float>&, int);
	int get_pos_biggest_value(vector<float>&);
	void find_patners_for_actual(vector<vector<float>>&, vector<string>&, vector<float>&, int, vector<int>&, vector<string>&, int);

void read_matriz(string, int, vector<string>&, vector<vector<float>>&);
	float get_distances(vector<float>&, vector<float>&); //Devuelve la distancia entre dos vectores característicos
	
int main()
{
	string route = "route.txt";
	//cout << "Ruta: " << endl;
	//cin >> route;

	int var_cant = 8; //Cantidad de variables con las que vamos a trabajar
	//cout << "Cantidad: " << endl;
	//cin >> var_cant;

	vector<string> elements = vector<string>(0, "");
	vector<vector<float>> distances = vector<vector<float>>(0, vector<float>(0, 100.0));//Inicio de la matriz con todo en cero

	read_matriz(route, var_cant, elements, distances);
	//print_v(distances);
	//aglomerativo_minimal_v2(distances, elements);
	disociativos(distances, elements, 1);
	return 0;
}

//Lee los datos del archivo en route
void read_matriz(string route, int size, vector<string>& elements, vector<vector<float>>& distances) {
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	fstream reader(route.c_str(), ios::in);
	vector<vector<float>> storing = vector<vector<float>>(0, vector<float>(0, 0.0));
	string word;
	int index = 0;
	int number = 0;

	//Descartando la primera fila
	for (int i = 0; i < size; i++) {
		reader >> word;
	}

	while (reader >> word)
	{
		elements.push_back(word);
		vector<float> temp = vector<float>(size - 1, 0.0);
		for (int i = 0; i < size - 1; i++) {
			reader >> word;
			temp[i] = stof(word);
		}
		storing.push_back(temp);
	}
	//Hasta completamos la lectura de los datos.
	//cout << storing.size() << "  -  " << distances.size() << endl;

	distances = vector<vector<float>>(storing.size(), vector<float>(storing.size(), 0.0)); //Configuramos el tamaño de la matriz
	for (unsigned int i = 0; i < storing.size(); i++) {
		for (unsigned int j = i; j < storing.size(); j++) {
			distances[i][j] = get_distances(storing[i], storing[j]);
		}
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration1 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Matriz procesada en: " << duration1 << " microsegundos" << endl;
	return;
}
float get_distances(vector<float>& a, vector<float>& b) {  //Devuelve la distancia de A a B
	float answer = 0.0;
	for (unsigned int i = 0; i < a.size(); i++) { //a y b deben ser del mismo tamaño
		answer += pow((a[i] - b[i]), 2.0);
	}
	answer = sqrtf(answer);
	return answer;
}


void aglomerativo_minimal_v2(vector<vector<float>>& dist, vector<string>& ele) {
	int indexer = 0;
	int mood = 1;  // O para menor, 1 para mayor y 2 para ponderado
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	while ((int)dist.size() > 1) {
		int _x = 0; int _y = 0;
		float minimo = find_minimo(dist, _y, _x);
		if (mood == 0) {
			fix_cluster_less(dist, _y, _x, ele);
		}
		else if (mood == 1) {
			fix_cluster_big(dist, _y, _x, ele);
		}
		else {
			fix_cluster_prom(dist, _y, _x, ele);
		}
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration1 = duration_cast<microseconds>(t2 - t1).count();

	print_v(ele, true);
	cout << "Minimo en: " << duration1 << " microsegundos" << endl;

}
float find_minimo(vector<vector<float>>& dist, int& _y, int& _x) {
	//Considerando la inexistencia de distancias negativas
	_y = 0; _x = 1;
	float answer = dist[_y][_x]; //Ubicación de la primera distancia minima	
	for (unsigned int i = 0; i < dist.size(); i++) {
		for (unsigned int j = i + 1; j < dist[i].size(); j++) {
			if (dist[i][j] < answer) {
				answer = dist[i][j];
				_y = i;
				_x = j;
			}
		}
	}
	return answer;
}
void fix_cluster_less(vector<vector<float>>& dist, int _y, int _x, vector<string>& ele) {
	//Primero bajamos por Y, llegamos al punto de quiebre y nos desenvolvemos por x.
	//No es bueno deshabilitar slots puesto que esto consume espacio
	//Siempre borraremos los integrantes de _x
	int _x_index = 0;
	int _y_index = 0;

	//_y siempre será menor a _x, es decir distribuido horizontalmente siempre estará antes que _x
	//Ejecutando hasta el quiebre de y
	for (unsigned int i = 0; i < _y; i++) {
		if (_y_index == _x) {
			_y_index++; //No hay comparacion
		}
		else if(_x_index == _y){
			_x_index++; //Tampoco hay comparación
		}
		else {
			if (_x_index == _x) {
				_x_index++;
				//Do nothing
			}
			else if (_x_index < _x) { //Verificamos si _x ha quebrado a la izquierda
				if (dist[_x_index][_x] < dist[_y_index][_y]) {
					dist[_y_index][_y] = dist[_x_index][_x];
				}
				_y_index++;
				_x_index++;
			}
			else {
				if (dist[_x][_x_index] < dist[_y_index][_y]) {
					dist[_y_index][_y] = dist[_x][_x_index];
				}
				_y_index++;
				_x_index++;
			}
			//Comparacion
		}
	}

	_y_index++;
	for (unsigned int i = _y + 1; i < dist.size(); i++) {
		if (_y_index == _x) {
			_y_index++; //No hay comparacion
		}
		else if (_x_index == _y) {
			_x_index++; //Tampoco hay comparación
		}
		else {
			if (_x_index == _x) {
				_x_index++;
				//Do nothing else
			}
			else if (_x_index < _x) { //Verificamos si _x ha quebrado a la izquierda
				if (dist[_x_index][_x] < dist[_y][_y_index]) {
					dist[_y_index][_y] = dist[_x_index][_x];
				}
				_y_index++;
				_x_index++;
			}
			else {
				if (dist[_x][_x_index] < dist[_y][_y_index]) {
					dist[_y][_y_index] = dist[_x][_x_index];
					
				}
				_y_index++;
				_x_index++;
				//Comparacion
			}
		}
	}
	ele[_y] = ele[_y] + " - " + ele[_x]; //Asignacion de los cluster

	//cout << "Antes: Y -> " << dist.size() << " X -> " << dist[0].size() << endl;
	//Eliminamos la columna x, tenemos la certeza de que y está antes que x
	for (unsigned int i = 0; i < dist.size(); i++) {
		dist[i].erase(dist[i].begin() + _x);
	}
	//Eliminamos la fila x
	dist.erase(dist.begin() + _x);
	ele.erase(ele.begin() + _x);

}
void fix_cluster_big(vector<vector<float>>& dist, int _y, int _x, vector<string>& ele) {
	//Primero bajamos por Y, llegamos al punto de quiebre y nos desenvolvemos por x.
	//No es bueno deshabilitar slots puesto que esto consume espacio
	//Siempre borraremos los integrantes de _x
	int _x_index = 0;
	int _y_index = 0;

	//_y siempre será menor a _x, es decir distribuido horizontalmente siempre estará antes que _x
	//Ejecutando hasta el quiebre de y
	for (unsigned int i = 0; i < _y; i++) {
		if (_y_index == _x) {
			_y_index++; //No hay comparacion
		}
		else if (_x_index == _y) {
			_x_index++; //Tampoco hay comparación
		}
		else {
			if (_x_index == _x) {
				_x_index++;
				//Do nothing
			}
			else if (_x_index < _x) { //Verificamos si _x ha quebrado a la izquierda
				if (dist[_x_index][_x] > dist[_y_index][_y]) {
					dist[_y_index][_y] = dist[_x_index][_x];
					dist[_x_index][_x] = 0; //<--ERASE
				}
				else {
					dist[_x_index][_x] = 0; //<--ERASE
				}
				_y_index++;
				_x_index++;
			}
			else {
				if (dist[_x][_x_index] > dist[_y_index][_y]) {
					dist[_y_index][_y] = dist[_x][_x_index];
					dist[_x][_x_index] = 0; //<--ERASE
				}
				else {
					dist[_x][_x_index] = 0; // <--ERASE
				}
				_y_index++;
				_x_index++;
			}
			//Comparacion
		}
	}

	_y_index++;
	for (unsigned int i = _y + 1; i < dist.size(); i++) {
		if (_y_index == _x) {
			_y_index++; //No hay comparacion
		}
		else if (_x_index == _y) {
			_x_index++; //Tampoco hay comparación
		}
		else {
			if (_x_index == _x) {
				_x_index++;
				//Do nothing else
			}
			else if (_x_index < _x) { //Verificamos si _x ha quebrado a la izquierda
				if (dist[_x_index][_x] > dist[_y][_y_index]) {
					dist[_y_index][_y] = dist[_x_index][_x];
					dist[_x_index][_x] = 0; //<--ERASE
				}
				else {
					dist[_x_index][_x] = 0; //<--ERASE
				}
				_y_index++;
				_x_index++;
			}
			else {
				if (dist[_x][_x_index] > dist[_y][_y_index]) {
					dist[_y][_y_index] = dist[_x][_x_index];
					dist[_x][_x_index] = 0; //<--ERASE

				}
				else {
					dist[_x][_x_index] = 0; // <--ERASE
				}
				_y_index++;
				_x_index++;
				//Comparacion
			}
		}
	}
	ele[_y] = ele[_y] + " - " + ele[_x]; //Asignacion de los cluster

	//cout << "Antes: Y -> " << dist.size() << " X -> " << dist[0].size() << endl;
	//Eliminamos la columna x, tenemos la certeza de que y está antes que x
	for (unsigned int i = 0; i < dist.size(); i++) {
		dist[i].erase(dist[i].begin() + _x);
	}
	//Eliminamos la fila x
	dist.erase(dist.begin() + _x);
	ele.erase(ele.begin() + _x);
	//cout << "Antes: Y -> " << dist.size() << " X -> " << dist[0].size() << endl;
	//print_m(dist);
	//print_v(ele, true);

}
void fix_cluster_prom(vector<vector<float>>& dist, int _y, int _x, vector<string>& ele) {
	//Primero bajamos por Y, llegamos al punto de quiebre y nos desenvolvemos por x.
	//No es bueno deshabilitar slots puesto que esto consume espacio
	//Siempre borraremos los integrantes de _x
	int _x_index = 0;
	int _y_index = 0;

	//_y siempre será menor a _x, es decir distribuido horizontalmente siempre estará antes que _x
	//Ejecutando hasta el quiebre de y
	for (unsigned int i = 0; i < _y; i++) {
		if (_y_index == _x) {
			_y_index++; //No hay comparacion
		}
		else if (_x_index == _y) {
			_x_index++; //Tampoco hay comparación
		}
		else {
			if (_x_index == _x) {
				_x_index++;
				//Do nothing
			}
			else if (_x_index < _x) { //Verificamos si _x ha quebrado a la izquierda
				
				dist[_y_index][_y] = (dist[_x_index][_x] + dist[_y_index][_y] ) / 2;
				_y_index++;
				_x_index++;
			}
			else {
				dist[_y_index][_y] = (dist[_x][_x_index] + dist[_y_index][_y]) / 2;
				
				_y_index++;
				_x_index++;
			}
			//Comparacion
		}
	}

	_y_index++;
	for (unsigned int i = _y + 1; i < dist.size(); i++) {
		if (_y_index == _x) {
			_y_index++; //No hay comparacion
		}
		else if (_x_index == _y) {
			_x_index++; //Tampoco hay comparación
		}
		else {
			if (_x_index == _x) {
				_x_index++;
				//Do nothing else
			}
			else if (_x_index < _x) { //Verificamos si _x ha quebrado a la izquierda
				
				dist[_y_index][_y] = (dist[_x_index][_x] + dist[_y_index][_y]) / 2;
				_y_index++;
				_x_index++;
			}
			else {
				dist[_y_index][_y] = (dist[_x][_x_index] + dist[_y_index][_y]) / 2;
				
				_y_index++;
				_x_index++;
				//Comparacion
			}
		}
	}
	ele[_y] = ele[_y] + " - " + ele[_x]; //Asignacion de los cluster

	//cout << "Antes: Y -> " << dist.size() << " X -> " << dist[0].size() << endl;
	//Eliminamos la columna x, tenemos la certeza de que y está antes que x
	for (unsigned int i = 0; i < dist.size(); i++) {
		dist[i].erase(dist[i].begin() + _x);
	}
	//Eliminamos la fila x
	dist.erase(dist.begin() + _x);
	ele.erase(ele.begin() + _x);
}


void disociativos(vector<vector<float>>& dist, vector<string>& ele, int mood){
	int min_size = 2; //Describe el minimo restante de elementos por analizar para detener el proceso de clustering
	vector<string> clustered = vector<string>(0, "");
	vector<float> distances_to_the_cluster = vector<float>(dist.size(), 0.0);
		get_distance_to_cluster(dist, distances_to_the_cluster);
	vector<int> pos_to_delete = vector<int>(0, 0);


	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	while (dist.size() > min_size) {
		int victim = get_pos_biggest_value(distances_to_the_cluster);
		find_patners_for_actual(dist, ele, distances_to_the_cluster, victim, pos_to_delete, clustered, 0);
		if (pos_to_delete.size() != 0) {
			//Ahora ele esta incluido dentro de la lista de elementos a eliminar
			while (pos_to_delete.size() != 0) {
				distances_to_the_cluster.erase(distances_to_the_cluster.begin() + pos_to_delete[pos_to_delete.size() - 1]);
				ele.erase(ele.begin() + pos_to_delete[pos_to_delete.size() - 1]);
				dist.erase(dist.begin() + pos_to_delete[pos_to_delete.size() - 1]);
				for (unsigned int i = 0; i < dist.size(); i++) {
					dist[i].erase(dist[i].begin() + pos_to_delete[pos_to_delete.size() - 1]);
				}
				pos_to_delete.pop_back();
			}
		}

	}
	
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration1 = duration_cast<microseconds>(t2 - t1).count();
	
	print_v(clustered, true);
	//cout << endl << endl;
	//cout << "[ ";
	//print_v(ele);
	//cout << " ]" << endl;
	cout << "Minimo en: " << duration1 << " microsegundos" << endl;

}
void get_distance_to_cluster(vector<vector<float>>& dist, vector<float>& rep) {
	for (unsigned int i = 0; i < dist.size(); i++) {
		//min 		
		float min = 99999; //BEWARE could produce malfunctions
		for (unsigned int j = 0; j < i; j++) {
			if (dist[j][i] < min) {
				min = dist[j][i];
			}
		}
		for (unsigned int j = i; j < dist.size(); j++) {
			if (dist[i][j] < min) {
				min = dist[i][j];
			}
		}
		rep[i] = min; //Cada posicion del rep le corresponde su  distancia al cluster
	}
}


void get_distance_to_element(vector<vector<float>>& dist, vector<float>& rep, int ele) {
	//Trabaja con el vector que limita las distancias entre un punto y el resto de los puntos (no la distancia del punto con el cluster)
	//Ele recorre su interseccion con cada elemento de dist y almacena en la posicion de ele en rep el valor correspodiente
	for (unsigned int i = 0; i < ele; i++){
		rep[i] = dist[i][ele];
	}
	for (unsigned int i = ele; i < dist.size(); i++) {
		rep[i] = dist[ele][i];
	}
	/*Notar que tomo en cuenta que el tamaño de dist (o sea se) la matriz principal irá disminuyendo conforme vaya procesando os datos*/
}
void find_patners_for_actual(vector<vector<float>>& dist, vector<string>& elements, vector<float>& d_to_cluster, int ele, vector<int>& pos_to_delete, vector<string>& clustered, int mood) {
	//dist and clust deben tener el mismo tamaño
	string ele_to_clustered = "[ ";
	float temp_dif = 0.0;
	pos_to_delete.clear(); //Clear posible remanents

	for (unsigned int i = 0; i < ele; i++) {
		temp_dif = d_to_cluster[i] - dist[i][ele];
		if (temp_dif >= 0) {
			pos_to_delete.push_back(i);
			ele_to_clustered += elements[ele] + " , ";
			elements.push_back(elements[ele] + elements[i]);
		}
	}
	for (unsigned int i = ele; i < dist.size(); i++) {
		temp_dif = d_to_cluster[i] - dist[ele][i];
		if (temp_dif >= 0) {
			pos_to_delete.push_back(i);
			ele_to_clustered += elements[ele] + " , ";
			elements.push_back(elements[ele] + elements[i]);
		}
	}

	ele_to_clustered.pop_back();
	ele_to_clustered.pop_back();
	ele_to_clustered += " ]";
	clustered.push_back(ele_to_clustered);
}
int get_pos_biggest_value(vector<float>& rep) {	//Retorna la posicion del mayor valor en el vector
	int answer = 0;
	float max = rep[0];
	for (unsigned int i = 1; i < rep.size(); i++) {
		if (rep[i] > max) {
			max = rep[i];
			answer = i;
		}
	}
	return answer;
}

