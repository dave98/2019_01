#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include<iostream>
#include<vector>
#include<mutex>
#include<Windows.h>
#include <chrono>
#include <chrono>
#include <stdio.h>
#include"BoolTuple.h"

using namespace std;
using namespace std::chrono;
mutex mtx;

string generate(int);
template<typename T>
void print_matrix(vector<vector<T>>&);
template<typename T = BoolTuple>
void print_matrix(vector<vector<BoolTuple>>&);
template<typename T>
void print_score(vector<vector<T>>&);
template<typename T>
void route_face(vector<vector<T>> &, vector<vector<vector<BoolTuple>>>&, string, string , bool, bool, bool);
template<typename T>
void route_dev(int, int, int, string, string, vector<string>&, vector<string>&, vector<vector<vector<BoolTuple>>>&, string&, string&);
bool get_state(vector<BoolTuple>&, int);
void evaluate_ruts(vector<vector<vector<BoolTuple>>>&);

//Alinemamiento Local
template <typename T = int>
void simple_evaluate_matrix_x(vector<vector<T>>&, vector<vector<vector<BoolTuple>>>&, string, string, int, int);
template <typename T = int>
void simple_evaluate_matrix_y(vector<vector<T>>&, vector<vector<vector<BoolTuple>>>&, string, string, int, int);
template<typename T = int>
void simple_route_face(vector<vector<T>>&, vector<vector<vector<BoolTuple>>>&, string, string, bool, bool, bool);
template<typename T = int>
void find_highest_values(vector<vector<T>>&, vector<int>&, vector<int>&);
int get_penalti(string);
void get_better(vector<string>&, vector<string>&, bool);

template<typename T>
class wunch {
public:
	vector<vector<T>> m;
	vector<vector<vector<BoolTuple>>> r;
	string a_y; 
	string b_x;

	wunch(string, string);
	~wunch();

	void fill_matrix();
	void evaluate_matrix_x(int, int);
	void evaluate_matrix_y(int, int);
};

template<typename T>
wunch<T>::wunch(string _a_y, string _b_x) {
	this->a_y = _a_y;
	this->b_x = _b_x;
	this->m = vector<vector<T>>(this->a_y.size() + 1, vector<T>(this->b_x.size() +1, 0));
	this->r = vector<vector<vector<BoolTuple>>>(this->a_y.size()+1, vector<vector<BoolTuple>>(this->b_x.size()+1, vector<BoolTuple>(3, BoolTuple())));
}

template<typename T>
wunch<T>::~wunch() {}

template<typename T>
void wunch<T>::fill_matrix() {
	int decrement = 0;
	for (unsigned int i = 0; i < this->m.size(); i++) {
		this->m[i][0] = decrement;
		this->r[i][0][2] = BoolTuple(false, true);
		decrement -= 2;
	}
	decrement = 0;
	for (unsigned int i = 0; i < this->m[0].size(); i++) {
		this->m[0][i] = decrement;
		this->r[0][i][0] = BoolTuple(true, false);
		decrement -= 2;
	}

	//Dando formato a las direcciones 
	for (unsigned int i = 1; i < this->m.size(); i++) {
		for (unsigned int j = 1; j < this->m[i].size(); j++) {
			this->r[i][j][0] = BoolTuple(true, false);
			this->r[i][j][1] = BoolTuple(true, true);
			this->r[i][j][2] = BoolTuple(false, true);
		}
	}

}

template<typename T>
void wunch<T>::evaluate_matrix_x(int pos_x, int pos_y) {
	for (unsigned int i = pos_x; i < this->m[0].size(); i++) {
		vector<bool> directions(3, true); //Indicamos que consideramos todos los caminos como posibles

		int e_value = 0;
		if (this->a_y[pos_y - 1] != this->b_x[i - 1]) {
			e_value = -1;
		}
		else { e_value = 1; }

		//Orden de los valores de izquierda para arriba
		T b_value = this->m[pos_y][i - 1] - 2; //Left value
		directions[1] = false;
		directions[2] = false;

		if (b_value == (this->m[pos_y - 1][i - 1] + e_value)) {
			directions[1] = true;
		}
		else if (b_value < (this->m[pos_y - 1][i - 1] + e_value)) { // Diagonal Value
			b_value = this->m[pos_y - 1][i - 1] + e_value;
			directions[0] = false;
			directions[1] = true;
		}
		else if (b_value == (this->m[pos_y - 1][i] - 2)) {
			directions[2] = true;
		}
		else if (b_value < (this->m[pos_y - 1][i] - 2)) { // Up value
			b_value = this->m[pos_y - 1][i] - 2;
			directions[0] = false;
			directions[1] = false;
			directions[2] = true;
		}

		//cout << "X: " << pos_y << " - " << i << " - x: " << m[pos_y][i - 1] << " yx: " << m[pos_y - 1][i - 1] << " y: "<< m[pos_y - 1][i] << " r: " << b_value << endl;

		this->m[pos_y][i] = b_value;
		for (unsigned int k = 0; k < directions.size(); k++) {
			if (directions[k] == false) {
				this->r[pos_y][i][k].x = 0; //Invalidamos la direccion y no apuntamos a nada
				this->r[pos_y][i][k].y = 0;
			}
		}
	}
}

template<typename T>
void wunch<T>::evaluate_matrix_y(int pos_x, int pos_y) {

	for (unsigned int i = pos_y; i < this->m.size(); i++) {
		vector<bool> directions = vector<bool>(3, true);
		int e_value = 0;
		if (this->a_y[i - 1] != this->b_x[pos_x - 1]) {
			e_value = -1;
		}
		else { e_value = 1; }

		T b_value = this->m[i - 1][pos_x] - 2; //Left value
		directions[0] = false;
		directions[1] = false;

		if (b_value == (this->m[i - 1][pos_x - 1] + e_value)) {
			directions[1] = true;
		}
		else if (b_value < (this->m[i - 1][pos_x - 1] + e_value)) { //Diagonal value
			b_value = this->m[i - 1][pos_x - 1] + e_value;
			directions[2] = false;
			directions[1] = true;
		}
		else if (b_value == (this->m[i][pos_x - 1] - 2)) {
			directions[0] = true;
		}
		else if (b_value < (this->m[i][pos_x - 1] - 2)) { // Up value
			b_value = this->m[i][pos_x - 1] - 2;
			directions[2] = false;
			directions[1] = false;
			directions[0] = true;
		}

		m[i][pos_x] = b_value;
		for (unsigned int k = 0; k < directions.size(); k++) {
			if (directions[k] == false) {
				this->r[i][pos_x][k].x = 0; //Invalidamos la direccion y no apuntamos a nada
				this->r[i][pos_x][k].y = 0;
			}
		}
	}
}

/////////////////////CLASS END HERE///////////////////////////////
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

template<typename T>
void print_score(vector<vector<T>> &m) {
	cout << "Score: " << m[m.size() - 1][m[0].size() - 1] << endl;
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
	//La Esquina superior izquierda no apunta a nada
	n[0][0][0] = BoolTuple(false, false);
	n[0][0][1] = BoolTuple(false, false);
	n[0][0][2] = BoolTuple(false, false);
}

void simple_fill_matrix(vector<vector<vector<BoolTuple>>>& n) {
	for (unsigned int i = 0; i < n.size(); i++) {
		n[i][0][2] = BoolTuple(false, true);
	}
	for (unsigned int i = 0; i < n[0].size(); i++) {
		n[0][i][0] = BoolTuple(true, false);
	}
	for (unsigned int i = 1; i < n.size(); i++) {
		for (unsigned int j = 1; j < n[i].size(); j++) {
			n[i][j][0] = BoolTuple(true, false);
			n[i][j][1] = BoolTuple(true, true);
			n[i][j][2] = BoolTuple(false, true);
		}
	}
	//La Esquina superior izquierda no apunta a nada
	n[0][0][0] = BoolTuple(false, false);
	n[0][0][1] = BoolTuple(false, false);
	n[0][0][2] = BoolTuple(false, false);
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

//En este punto ambos mensajes aun no disponen del gap añadido
template<typename T = int>
void evaluate_matrix_x_v2(vector<vector<T>>& m, vector<vector<vector<BoolTuple>>>& r, string a_y, string b_x, int pos_x, int pos_y) {
	int e_value = 0;
	for (unsigned int i = pos_x; i < m[0].size(); i++) {
		vector<bool> directions(3, false); //Iniciamos todas las direcciones como false
		vector<int> pesos(3, 0);
		//Comparamos en la pos_x y pos_y de ambas palabras si sus letaras son iguales. Notar que no hay gap inicial
		//m ya lleva incorporados los gaps.
		if (a_y[pos_y - 1] != b_x[i - 1]) {
			e_value = -1;
		}
		else { e_value = 1; }

		pesos[0] = m[pos_y][i - 1] - 2;
		pesos[1] = m[pos_y - 1][i - 1] + e_value;
		pesos[2] = m[pos_y - 1][i] - 2;

		e_value = pesos[0];
		directions[0] = true;
		for (unsigned int k = 1; k < pesos.size(); k++) {
			if (e_value < pesos[k]) {
				for (unsigned j = 0; j < k; j++) {
					directions[j] = false;
				}
				e_value = pesos[k];
				directions[k] = true;
			}
			else if (e_value == pesos[k]) {
				directions[k] = true;
			}
		}

		m[pos_y][i] = e_value;
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

template<typename T = int>
void evaluate_matrix_y_v2(vector<vector<T>>& m, vector<vector<vector<BoolTuple>>>& r, string a_y, string b_x, int pos_x, int pos_y) {
	//Reocrrido vertical de la matriz
	//static int cont;
	//cout << "Probe" << m[pos_y][pos_x] << endl;
	//cont++; 
	//cout << "En y: " << cont << " x: "<<b_x.size()<<" y: "<<a_y.size()<<endl;
	int e_value = 0;
	for (unsigned int i = pos_y; i < m.size(); i++) {
		//cout << "->" << i <<" - "<<pos_x<<" means: "<<m[i][pos_x]<< endl;
		vector<bool> directions = vector<bool>(3, false);
		vector<int> pesos = vector<int>(3, 0);

		if (a_y[i - 1] != b_x[pos_x - 1]) {
			e_value = -1;
		}
		else { e_value = 1; }
	
		pesos[0] = m[i][pos_x - 1] - 2;
		pesos[1] = m[i - 1][pos_x - 1] + e_value;
		pesos[2] = m[i - 1][pos_x] - 2;

		e_value = pesos[0];
		directions[0] = true;

		//cout << "Flag1" << endl;

		for (unsigned int j = 1; j < pesos.size(); j++) {
			if (e_value < pesos[j]) {
				for (unsigned int k = 0; k < j; k++) {
					directions[k] = false;
				}
				e_value = pesos[j];
				directions[j] = true;
			}
			else if (e_value == pesos[j]) {
				directions[j] = true;
			}
		}

		m[i][pos_x] = e_value;//Llega
		//cout << "Flag2" << endl;
		for (unsigned int k = 0; k < directions.size(); k++) {
			//cout << "k: " << k << endl;
			if (directions[k] == false) {
				r[i][pos_x][k].x = 0; //Invalidamos la direccion y no apuntamos a nada
				r[i][pos_x][k].y = 0;
			}
		}
		//cout << "Flag3" << endl;

	}
	//cout << "Ended_y" << endl;
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

	
	cout << "Respuestas: " << pool_x.size() << endl;
	if(print_chain){
		for (unsigned int i = 0; i < pool_x.size(); i++) {
			cout << "X horizontal: " << pool_x[i] << endl;
			cout << "Y vertical:   " << pool_y[i] << endl;
			cout << endl;
		}
		cout << "Score: " << m[m.size() - 1][m[0].size() - 1] << endl;

	}
}


template<typename T = int>
void route_face(vector<vector<T>> &m, vector<vector<vector<BoolTuple>>>& r, string a_y, string b_x, bool print_chain, bool is_print_penalti, bool print_penalti) {
	a_y = '-' + a_y;
	b_x = '-' + b_x;

	int start_state = 0; //We always start at the left pointer  (true - false) (1, 0)
	int start_y = a_y.size() - 1;//Menos uno porque ya sumamos el guion
	int start_x = b_x.size() - 1;
	vector<string> total_routes_y = vector<string>(0, ""); //Inicializamos un vector vacío
	vector<string> total_routes_x = vector<string>(0, "");

	//cout << "Start routing" << endl;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	route_dev<T>(start_y, start_x, 0, "", "", total_routes_y, total_routes_x, r, a_y, b_x);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration1 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Rutas totales en: " << duration1 << " microsegundos" << endl;
	//cout << "End routing" << endl;

	cout << "Total de rutas: " << total_routes_x.size() << " - " << total_routes_y.size() << endl;
	if (print_chain) {
		for (unsigned int i = 0; i < total_routes_x.size(); i++) {
			cout << total_routes_x[i] << endl;
			cout << total_routes_y[i] << endl<<endl;
		}
	}
	if (is_print_penalti) {
		cout << "Penalidad: " << endl;
		get_better(total_routes_x, total_routes_y, print_penalti);
	}
	return;
}

//Funcion recursiva
template<typename T>
void route_dev(int p_y, int p_x, int state, string word_y, string word_x, vector<string>& pool_y, vector<string>& pool_x, vector<vector<vector<BoolTuple>>>& r, string& a_y, string& b_x) {
	//word_y indica el progreso en la palabra vertical
	//word_x indica el progreso en la palabra horizontal
	//cout << "Actual Directions: " << r[p_y][p_x][0] << " - " << r[p_y][p_x][1] << " - " << r[p_y][p_x][2] << " || P_Y: "<<p_y<<" P_X: "<<p_x<<" State: "<<state<<endl;
	
	if (p_y + p_x == 0) {
		pool_y.push_back(word_y);
		pool_x.push_back(word_x);
		//cout << "Ending" << endl;
		return; //Igual a decir si p_y == 0 y p_x == 0; 
	}


	//Solo se aplica si hay una ruta mas arriba de la que conocemos 
	if (get_state(r[p_y][p_x], state)) {
		//Recursividad de nueva ruta
		//cout << "New route found" << endl;
		route_dev<T>(p_y, p_x, state + 1, word_y, word_x, pool_y, pool_x, r, a_y, b_x); //Será inefectivo pero para el siguiente análisis solo sumaremos uno al estado
	}
	
	int n_y = p_y - (int)r[p_y][p_x][state].y; // Verificamos que si despues de evaluar mas caminos, el camino que escogimos es el más adecuado
	int n_x = p_x - (int)r[p_y][p_x][state].x;

	if ((p_y != n_y) or (p_x != n_x)) {
		//cout << "Adding path" << endl;
		//word_y.push_back(a_y[p_y]);
		//word_x.push_back(b_x[p_x]);
		if (state == 0) {
			word_y.push_back('-');
			word_x.push_back(b_x[p_x]);
		}
		else if (state == 1) {
			word_y.push_back(a_y[p_y]);
			word_x.push_back(b_x[p_x]);
		}
		else {
			word_y.push_back(a_y[p_y]);
			word_x.push_back('-');
		}


		//p_y = p_y - (int)r[p_y][p_x][state].y;	
		//p_x = p_x - (int)r[p_y][p_x][state].x;
		p_y = n_y;
		p_x = n_x;
		route_dev<T>(p_y, p_x, 0, word_y, word_x, pool_y, pool_x, r, a_y, b_x);
		return;
	}
	else {
		//cout << "Close way" << endl;
		return;
	}
}

bool get_state(vector<BoolTuple>& m, int state) {
	int answer = 0;
	for (int i = state + 1; i < m.size(); i++) {
		answer += i * ((int)m[i].x + (int)m[i].y);
	}
	return (bool)answer; //Retorna true cuando valores superiores a states están ocupados
						// Verdadera si hay caminos por encima del rango de state
}

//Util para ubicar destinos sin un posible destino.
void evaluate_ruts(vector<vector<vector<BoolTuple>>>& r) {
	int answer = 0;
	for (unsigned int i = 0; i < r.size(); i++) {
		for (unsigned int j = 0; j < r[0].size(); j++) {
			if (get_state(r[i][j], -1)) {
				answer++;
			}
		}
	}
	//cout << "Caminos muertos: " << answer << endl;
}


//Simple Implementation ------------------------- ALINEMIENTO LOCAL---------------------------------------------
template <typename T = int>
void simple_evaluate_matrix_x(vector<vector<T>>& m, vector<vector<vector<BoolTuple>>>& r, string a_y, string b_x, int pos_x, int pos_y) {
	int e_value = 0;
	for (unsigned int i = pos_x; i < m[0].size(); i++) {
		vector<bool> directions(3, false); //Iniciamos todas las direcciones como false
		vector<int> pesos(3, 0);
		//Comparamos en la pos_x y pos_y de ambas palabras si sus letaras son iguales. Notar que no hay gap inicial
		//m ya lleva incorporados los gaps.
		if (a_y[pos_y - 1] != b_x[i - 1]) {
			e_value = -1;
		}
		else { 
			e_value = 1; }

		pesos[0] = m[pos_y][i - 1] - 2;
		pesos[1] = m[pos_y - 1][i - 1] + e_value;
		pesos[2] = m[pos_y - 1][i] - 2;

		e_value = 0;
		for (unsigned int k = 0; k < pesos.size(); k++) {
			if (e_value < pesos[k]) {
				for (unsigned j = 0; j < k; j++) {
					directions[j] = false;
				}
				e_value = pesos[k];
				directions[k] = true;
			}
			else if (e_value == pesos[k]) {
				directions[k] = true;
			}
		}

		m[pos_y][i] = e_value;
		for (unsigned int k = 0; k < directions.size(); k++) {
			if (directions[k] == false) {
				r[pos_y][i][k].x = 0; //Invalidamos la direccion y no apuntamos a nada
				r[pos_y][i][k].y = 0;
			}
		}
	}
}

template <typename T = int>
void simple_evaluate_matrix_y(vector<vector<T>>& m, vector<vector<vector<BoolTuple>>>& r, string a_y, string b_x, int pos_x, int pos_y) {
	int e_value = 0;
	for (unsigned int i = pos_y; i < m.size(); i++) {
		vector<bool> directions = vector<bool>(3, false);
		vector<int> pesos = vector<int>(3, 0);

		if (a_y[i - 1] != b_x[pos_x - 1]) {
			e_value = -1;
		}
		else { 
			e_value = 1; }

		pesos[0] = m[i][pos_x - 1] - 2;
		pesos[1] = m[i - 1][pos_x - 1] + e_value;
		pesos[2] = m[i - 1][pos_x] - 2;

		e_value = 0;

		for (unsigned int j = 0; j < pesos.size(); j++) {
			if (e_value < pesos[j]) {
				for (unsigned int k = 0; k < j; k++) {
					directions[k] = false;
				}
				e_value = pesos[j];
				directions[j] = true;
			}
			else if (e_value == pesos[j]) {
				directions[j] = true;
			}
		}

		m[i][pos_x] = e_value;//Llega
		for (unsigned int k = 0; k < directions.size(); k++) {
			if (directions[k] == false) {
				r[i][pos_x][k].x = 0; //Invalidamos la direccion y no apuntamos a nada
				r[i][pos_x][k].y = 0;
			}
		}

	}
}

template<typename T = int>
void simple_route_face(vector<vector<T>>& m, vector<vector<vector<BoolTuple>>>& r, string a_y, string b_x, bool print_chain, bool is_print_penalti, bool print_penalti) {
	a_y = '-' + a_y;
	b_x = '-' + b_x;

	int start_state = 0; //We always start at the left pointer  (true - false) (1, 0)
	int start_y = a_y.size() - 1;//Menos uno porque ya sumamos el guion
	int start_x = b_x.size() - 1;
	vector<string> total_routes_y = vector<string>(0, ""); //Inicializamos un vector vacío
	vector<string> total_routes_x = vector<string>(0, "");

	vector<int> starts_at_x = vector<int>(0, 0);//Necesitamos las posiciones de todos aquellos posiciones que contengan los datos más pesados
	vector<int> starts_at_y = vector<int>(0, 0); 
	find_highest_values(m, starts_at_x, starts_at_y);

	cout << "Total de maximos encontrados: " << starts_at_x.size() << " - " << starts_at_y.size() << endl;
	for (unsigned int i = 0; i < starts_at_x.size(); i++) {
		cout << starts_at_x[i] << " - " << starts_at_y[i] << "-> "<< m[starts_at_y[i]][starts_at_x[i]] << endl;
	}

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	for (unsigned int i = 0; i < starts_at_x.size(); i++) {
		simple_route_dev(starts_at_y[i], starts_at_x[i], 0, "", "", total_routes_y, total_routes_x, r, m, a_y, b_x );
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration1 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Rutas totales simples en: " << duration1 << " microsegundos" << endl;


	//cout << "End routing" << endl;

	cout << "Total de rutas simples: " << total_routes_x.size() << " - " << total_routes_y.size() << endl;
	if (print_chain) {
		for (unsigned int i = 0; i < total_routes_x.size(); i++) {
			cout << total_routes_x[i] << endl;
			cout << total_routes_y[i] << endl << endl;
		}
	}
	if (is_print_penalti) {
		cout << "Penalidad: " << endl;
		get_better(total_routes_x, total_routes_y, print_penalti);
	}
	return;
}

template<typename T = int>
void simple_route_dev(int p_y, int p_x, int state, string word_y, string word_x, vector<string>& pool_y, vector<string>& pool_x, vector<vector<vector<BoolTuple>>>& r, vector<vector<T>>& m, string& a_y, string& b_x) {
	//cout << "Designing route" << endl;
	if (p_y + p_x == 0) {
		pool_y.push_back(word_y);
		pool_x.push_back(word_x);
		//cout << "Ending" << endl;
		return; //Igual a decir si p_y == 0 y p_x == 0; 
	}

	if (m[p_y][p_x] == 0) {
		pool_y.push_back(word_y);
		pool_x.push_back(word_x);
	}

	//Solo se aplica si hay una ruta mas arriba de la que conocemos 
	if (get_state(r[p_y][p_x], state)) {
		//Recursividad de nueva ruta
		simple_route_dev<T>(p_y, p_x, state + 1, word_y, word_x, pool_y, pool_x, r, m, a_y, b_x); //Será inefectivo pero para el siguiente análisis solo sumaremos uno al estado
	}

	int n_y = p_y - (int)r[p_y][p_x][state].y; // Verificamos que si despues de evaluar mas caminos, el camino que escogimos es el más adecuado
	int n_x = p_x - (int)r[p_y][p_x][state].x;

	if ((p_y != n_y) or (p_x != n_x)) {
		if (state == 0) {
			word_y.push_back('-');
			word_x.push_back(b_x[p_x]);
		}
		else if (state == 1) {
			word_y.push_back(a_y[p_y]);
			word_x.push_back(b_x[p_x]);
		}
		else {
			word_y.push_back(a_y[p_y]);
			word_x.push_back('-');
		}

		p_y = n_y;
		p_x = n_x;
		simple_route_dev<T>(p_y, p_x, 0, word_y, word_x, pool_y, pool_x, r, m, a_y, b_x);
		return;
	}
	else {
		return;
	}
}

template<typename T = int>
void find_highest_values(vector<vector<T>>& m, vector<int>& p_x, vector<int>& p_y) {
	int max_value = 0;
	for (unsigned int i = 0; i < m.size(); i++) {
		for (unsigned int j = 0; j < m[0].size(); j++) {
			if (max_value < m[i][j]) {
				p_x.clear();
				p_y.clear();
				p_x.push_back(j);
				p_y.push_back(i);
				max_value = m[i][j];
			}
			else if (max_value == m[i][j]) {
				p_x.push_back(j);
				p_y.push_back(i);
			}
		}
	}
}


int get_penalti(string msg) {
	int p = 5;
	int p_1 = 0;
	int q = 3;
	int k = 0;

	int answer = 0;

	for (unsigned int i = 0; i < msg.size(); i++) {
		if (msg[i] == '-') {
			p_1 += p;
			while (msg[i + 1] == '-') {
				k += 1;
				i++;
				if (i >= msg.size()) { break; }
			}
		}
	}
	
	answer = (p_1) + (k * q);
	return answer;
}

void get_better(vector<string>& x, vector<string>& y, bool p_penalti) {
	//Ambos vectores son del mismo tamaño
	vector<int> answer = vector<int>(0, 0);
	vector<int> pesos = vector<int>(0, 0);
	
	int penalti = get_penalti(x[0]) + get_penalti(y[0]);
	answer.push_back(0);
	pesos.push_back(penalti);

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	for (unsigned int i = 1; i < x.size(); i++) {
		int temp = get_penalti(x[i]) + get_penalti(y[i]);
		if (penalti > temp) {
			answer.clear();
			pesos.clear();
			answer.push_back(i);
			pesos.push_back(temp);
			penalti = temp;
		}
		else if (penalti == temp) {
			answer.push_back(i);
			pesos.push_back(temp);
		}
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration1 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Penalidad hallada en: " << duration1 << " microsegundos" << endl;

	cout << "Mejor Penalidad: " << pesos[0] << " con un total de " << answer.size() << " rutas." << endl;
	if (p_penalti) {
		for (unsigned int i = 0; i < answer.size(); i++) {
			cout << "Y: " << y[answer[i]] << endl;
			cout << "X: " << x[answer[i]] << endl;
			cout << "Peso: " << pesos[i] << endl;
		}
	}
}

#endif

