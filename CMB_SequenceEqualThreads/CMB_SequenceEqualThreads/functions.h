#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include<iostream>
#include<vector>
#include<mutex>
#include<Windows.h>
#include"BoolTuple.h"

using namespace std;
mutex mtx;

string generate(int x) {
	string answer = "";

	char gen[4];
	gen[0] = 'A';
	gen[1] = 'G';
	gen[2] = 'C';
	gen[3] = 'T';

	while (answer.size() < x) {
		answer += gen[rand() % 4];
	}
	return answer;
}

template<typename T>
void print_matrix(vector<vector<T>>& m) {
	for (unsigned int i = 0; i < m.size(); i++) {
		for (unsigned int j = 0; j < m[i].size(); j++) {
			cout << m[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

template<typename T = BoolTuple>
void print_matrix(vector<vector<vector<T>>>& n) {
	for (unsigned int i = 0; i < n.size(); i++) {
		for (unsigned int j = 0; j < n[i].size(); j++) {
			if (n[i][j].empty()) {
				cout << " 0 ";
			}
			else {
				cout << "->";
				for (unsigned int k = 0; k < n[i][j].size(); k++) {
					cout << n[i][j][k];
				}
				cout << "<-";
			}
		}
		cout << endl;
	}
	cout << endl;
}

void print_chain(string m) {

}


//Asuming that dimensions are over 1 in both cases
template<typename T>
void fill_matrix(vector<vector<T>>& m, vector<vector<vector<BoolTuple>>>& n) { // m y n tienen el mismo tamaño
	int decrement = 0;
	for (unsigned int i = 0; i < m.size(); i++) {
		m[i][0] = decrement;
		n[i][0][2] = BoolTuple(false, true);
		decrement -= 2;
	}
	decrement = 0;
	for (unsigned int i = 0; i < m[0].size(); i++) {
		m[0][i] = decrement;
		n[0][i][0] = BoolTuple(true, false);
		decrement -= 2;
	}

	//Dando formato a las direcciones 
	for (unsigned int i = 1; i < m.size(); i++) {
		for (unsigned int j = 1; j < m[i].size(); j++) {
			n[i][j][0] = BoolTuple(true, false);
			n[i][j][1] = BoolTuple(true, true);
			n[i][j][2] = BoolTuple(false, true);
		}
	}
}


template<typename T = int> 
void evaluate_matrix_x(vector<vector<T>>& m, vector<vector<vector<BoolTuple>>>& r, string a_y, string b_x, int pos_x, int pos_y) { //Avanza diagonalmente pos_X actual column, pos_y actual file
	//thread que recorre la matriz HORIZONTALMENTE  
	// pos_x indica la posicion en horizontal de donde voy a partir 
	// pos_y indica TODA la fila en donde se va realizar dicha operación, por eso es un valor ESTÁTICO
	// r es una matriz compuesta por una lista de punteros que indican todos los posible lugares a donde podemos
	//   saltar desde esa posición
	//lock_guard<mutex> lck(mtx);

	for (unsigned int i = pos_x; i < m[0].size(); i++) {
		vector<bool> directions(3, true); //Indicamos que consideramos todos los caminos como posibles

		int e_value = 0;
		if (a_y[pos_y - 1] != b_x[i - 1]) {
			e_value = -1;
		}
		else{ e_value = 1; }
	
		//Orden de los valores de izquierda para arriba
		T b_value = m[pos_y][i - 1] - 2; //Left value
			directions[1] = false; 
			directions[2] = false;

		if (b_value == (m[pos_y - 1][i - 1] + e_value)) {
			directions[1] = true;
		}
		else if (b_value < (m[pos_y - 1][i - 1] + e_value)) { // Diagonal Value
			b_value = m[pos_y - 1][i - 1] + e_value;
			directions[0] = false;
			directions[1] = true;
		}
		else if (b_value == (m[pos_y - 1][i] - 2)) {
			directions[2] = true;
		}
		else if (b_value < (m[pos_y - 1][i] - 2)) { // Up value
			b_value = m[pos_y - 1][i] - 2;
			directions[0] = false;
			directions[1] = false;
			directions[2] = true;
		}

		//cout << "X: " << pos_y << " - " << i << " - x: " << m[pos_y][i - 1] << " yx: " << m[pos_y - 1][i - 1] << " y: "<< m[pos_y - 1][i] << " r: " << b_value << endl;

		m[pos_y][i] = b_value;
		for (unsigned int k = 0; k < directions.size(); k++) {
			if (directions[k] == false) {
				r[pos_y][i][k].x = 0; //Invalidamos la direccion y no apuntamos a nada
				r[pos_y][i][k].y = 0;
			}
		}
	}
}

template<typename T = int>
void evaluate_matrix_y(vector<vector<T>>& m, vector<vector<vector<BoolTuple>>>& r, string a_y, string b_x, int pos_x, int pos_y) {
	//thread que recorre la matriz VERTICALMENTE 
	//pos_x indica la columna que tomaremos como referencia el recorrido, por eso es un valor ESTÁTICO
	//pos_y indica el inicio para recorrer la columna que tomamos como referencia.
	//lock_guard<mutex> lck(mtx);

	for (unsigned int i = pos_y; i < m.size(); i++) {
		vector<bool> directions = vector<bool>(3, true);
		int e_value = 0;
		if (a_y[i - 1] != b_x[pos_x - 1]) {
			e_value = -1;
		}
		else { e_value = 1; }

		T b_value = m[i-1][pos_x] - 2; //Left value
		directions[0] = false;
		directions[1] = false;

		if (b_value == (m[i - 1][pos_x - 1] + e_value)) {
			directions[1] = true;
		}
		else if (b_value < (m[i - 1][pos_x - 1] + e_value)){ //Diagonal value
			b_value = m[i - 1][pos_x - 1] + e_value;
			directions[2] = false;
			directions[1] = true;
		}
		else if (b_value == (m[i][pos_x - 1] - 2)) {
			directions[0] = true;
		}
		else if (b_value < (m[i][pos_x-1] - 2)) { // Up value
			b_value = m[i][pos_x -1 ] - 2;
			directions[2] = false;
			directions[1] = false;
			directions[0] = true;
		}

		m[i][pos_x] = b_value;
		for (unsigned int k = 0; k < directions.size(); k++) {
			if (directions[k] == false) {
				r[i][pos_x][k].x = 0; //Invalidamos la direccion y no apuntamos a nada
				r[i][pos_x][k].y = 0;
			}
		}
		//cout << "Y: " << i << " - " << pos_x << " - x: " << m[i][pos_x - 1] << " yx: " << m[i - 1][pos_x - 1] << " y: " << m[i - 1][pos_x] << " r: " << b_value << " -  Y_W: "<< a_y[i - 1] <<" - X_W: "<< b_x[pos_x - 1] <<endl;
	}
}

template <typename T = int>
void find_routes(vector<vector<T>>& m, vector<vector<vector<BoolTuple>>>& r, string a_y, string b_x, bool print_chain) {
	a_y = '-' + a_y;
	b_x = '-' + b_x;
	int start_y = 0; // Dan el tamaño de la matriz e indican las letras a tomar
							  //Consideramos el valor del gap en la palabra, por eso no se agrega el "-1"
	int start_x = 0; // La alineación se hace tomando las letras de la cadena horizontal 
	int counter = 100; // Cantidad de rutas a extraer


	vector<string> part_y = vector<string>(0, "-"); //Almacena segmentos de caminos ya recorridos para Y vertical
	vector<string> part_x = vector<string>(0, "-"); //Almacena segmentos de caminos ya recorridos para X horizontal
	vector<IntTuple> bifurcacion = vector<IntTuple>(0, IntTuple()); //Indica el punto de una bifurcación, esta a la par de part_x y part_y
	vector<int> sentido = vector<int>(0, 0);

	vector<string> pool_y = vector<string>(0, ""); //Almacena todas las posible alineaciones.
	vector<string> pool_x = vector<string>(0, "");
	bool cont = true;
	bool is_all = true;
	bool quitar = true;
	int actual_counter = 0;
	int sense = 0;
	//Cuando viene de la diagonal consideramos ambos valores
	//Si hay salto para un costado es un gap para la palabra vertical
	//Si hay un salto para arriba es un gap para la palabra horizontal
	//Si hay mas de un salto consideramos un camino diferente el cual habrá que guardar
	string y = ""; //VERTICAL primera palabra insertada
	string x = "";	//HORIZONTAL segunda palabra insertada

	bifurcacion.push_back(IntTuple((int)b_x.size()-1, (int)a_y.size()-1)); //Insertamos inicio de los recorridos
	sentido.push_back(0);
	part_y.push_back(y);  //Las dos primeras letras estan vacias
	part_x.push_back(x);

	while (is_all) {
		//cout << "Flag" << endl;
		//start_y = bifurcacion[0].y; //Damos los primeros valores para iniciar el camino
		//start_x = bifurcacion[0].x;
		//y = part_y[0]; //Si hay segmento de cadena en este punto se los damos al resultado para que los una a la respuesta
		//x = part_x[0];
		start_y = bifurcacion[bifurcacion.size() - 1].y;
		start_x = bifurcacion[bifurcacion.size() - 1].x;
		sense = sentido[sentido.size() - 1];
		y = part_y[part_y.size() - 1];
		x = part_x[part_x.size() - 1];

		if (y.size() != 0) {
			y.pop_back();
			x.pop_back();
		}

		//bifurcacion.erase(bifurcacion.begin()); //Eliminamos los valores de donde partimos, PODEMOS USAR UNA COLA para mayor eficiencia
		//part_y.erase(part_y.begin());
		//part_x.erase(part_x.begin());
		bifurcacion.pop_back();
		part_y.pop_back();
		part_x.pop_back();

		//cout << "HERE: " << y << " -- " << x << endl;
		while (cont) {
			//cout << "Flag_2" << endl;
			//cout << "x: " << start_x << " y: " << start_y << " --> " << m[start_y][start_x] << endl;

			if (sense == 0) {
				if (!r[start_y][start_x][0].is_empty()) {
					//cout << "Flag_3" << endl;
					x += b_x[start_x];
					y += '-';


					if (!r[start_y][start_x][1].is_empty() || !r[start_y][start_x][2].is_empty()) {
						//cout << "Bifurcación encontrada en: " << start_y << " - " << start_x << endl;
						bifurcacion.push_back(IntTuple(start_x, start_y)); //Guardamos la posicion de la bifurcación en X y Y
						sentido.push_back(1);
						//y.pop_back(); x.pop_back();
						part_y.push_back(y); //Almacenamos el segmento recorrido en Y vertical
						part_x.push_back(x); //Almacenamos el segmento recorrido en X horizontal

						//quitar = false;
						//r[start_y][start_x][0].clear();//eliminamos lo recorrido para la izquierda
					}

					//if (quitar)
					//r[start_y][start_x][0].clear();//eliminamos lo recorrido para la izquierda
					start_x -= 1;
				}
				else if (!r[start_y][start_x][1].is_empty()) {
					x += b_x[start_x];
					y += a_y[start_y];


					if (!r[start_y][start_x][2].is_empty()) {
						//cout << "Bifurcación encontrada en: " << start_y << " - " << start_x << endl;
						bifurcacion.push_back(IntTuple(start_x, start_y));
						sentido.push_back(2);
						//y.pop_back(); x.pop_back();
						part_y.push_back(y);
						part_x.push_back(x);

						//quitar = false;
						//r[start_y][start_x][1].clear();//eliminamos lo recorrido para la diagonal
					}

					//if (quitar)
					//r[start_y][start_x][1].clear();//eliminamos lo recorrido para la diagonal
					start_x -= 1;
					start_y -= 1;
				}
				else if (!r[start_y][start_x][2].is_empty()) {
					//cout << "Flag_4" << endl;
					x += '-';
					y += a_y[start_y];


					//if (quitar)
					//r[start_y][start_x][2].clear();//eliminamos lo recorrido para arriba
					start_y -= 1;
				}
				else {
					cout << "Error" << endl;
					cont = false;
					is_all = false;
				}

				//Condición para la cadena actual
				if (start_x == 0 && start_y == 0) {
					cont = false;
				}

				if (start_x < 0) { start_x = 0; cout << "TERA_1" << endl; } /*WARNING*/
				if (start_y < 0) { start_y = 0; cout << "TERA_2" << endl; }

				//cout << "Position: " <<start_x << " ; " << start_y <<" Value "<< m[start_y][start_x]<<" : "<< endl;
				//Sleep(100);
			}
			else if (sense == 1) {
				if (!r[start_y][start_x][1].is_empty()) {
					x += b_x[start_x];
					y += a_y[start_y];


					if (!r[start_y][start_x][2].is_empty()) {
						//cout << "Bifurcación encontrada en: " << start_y << " - " << start_x << endl;
						bifurcacion.push_back(IntTuple(start_x, start_y));
						sentido.push_back(2);
						//y.pop_back(); x.pop_back();
						part_y.push_back(y);
						part_x.push_back(x);

						//quitar = false;
						//r[start_y][start_x][1].clear();//eliminamos lo recorrido para la diagonal
					}

					//if (quitar)
					//r[start_y][start_x][1].clear();//eliminamos lo recorrido para la diagonal
					start_x -= 1;
					start_y -= 1;
				}
				else if (!r[start_y][start_x][2].is_empty()) {
					//cout << "Flag_4" << endl;
					x += '-';
					y += a_y[start_y];


					//if (quitar)
					//r[start_y][start_x][2].clear();//eliminamos lo recorrido para arriba
					start_y -= 1;
				}
				else {
					cout << "Error" << endl;
					cont = false;
					is_all = false;
				}

				//Condición para la cadena actual
				if (start_x == 0 && start_y == 0) {
					cont = false;
				}

				if (start_x < 0) { start_x = 0; cout << "TERA_1" << endl; } /*WARNING*/
				if (start_y < 0) { start_y = 0; cout << "TERA_2" << endl; }
			}
			else {

				if (!r[start_y][start_x][2].is_empty()) {
					x += '-';
					y += a_y[start_y];

					start_y -= 1;
				}
				else {
					cout << "Error" << endl;
					cont = false;
					is_all = false;
				}

				//Condición para la cadena actual
				if (start_x == 0 && start_y == 0) {
					cont = false;
				}

				if (start_x < 0) { start_x = 0; cout << "TERA_1" << endl; } /*WARNING*/
				if (start_y < 0) { start_y = 0; cout << "TERA_2" << endl; }

				//cout << "Position: " <<start_x << " ; " << start_y <<" Value "<< m[start_y][start_x]<<" : "<< endl;
				//Sleep(100);
			}
			sense = 0;
		}
		pool_x.push_back(x);
		pool_y.push_back(y);

		y.clear();
		x.clear();
		//cout << "Nuevo Camino: "<<bifurcacion[0] << " - " << part_y[0] << " - " << part_x[0] << endl;
		if (bifurcacion.empty() || actual_counter > counter) {
			is_all = false;
			//cout << "Camino: " << bifurcacion.size() << " - " << part_y.size() << " - " << part_x.size() << endl;
		}
		else {
			cont = true;
			//quitar = true;
		}
	}


	if(print_chain){
		for (unsigned int i = 0; i < pool_x.size(); i++) {
			cout << "X horizontal: " << pool_x[i] << endl;
			cout << "Y vertical:   " << pool_y[i] << endl;
			cout << endl;
		}
		cout << "Respuestas: " << pool_x.size() << endl;
		cout << "Score: " << m[m.size() - 1][m[0].size() - 1] << endl;

	}
}


#endif
