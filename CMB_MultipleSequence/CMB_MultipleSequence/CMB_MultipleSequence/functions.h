#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include<iostream>
#include<vector>
#include<mutex>
#include<Windows.h>
#include<chrono>
#include<chrono>
#include<stdio.h>
#include<thread>
#include<fstream>
#include<sstream>
#include"BoolTuple.h"

using namespace std;
using namespace chrono;


void read_data(string, int, int, vector<string>&);

template<typename T = int>
void fill_matrix(vector<vector<T>>&, vector<vector<vector<BoolTuple>>>&);

bool get_state(vector<BoolTuple>&, int);

template<typename T>
void print_matrix(vector<vector<T>>&);
template<typename T>
void duplicate_matrix(vector<vector<T>>&);
template<typename T>
int get_value_by_file(vector<vector<T>>&, int);

string print_sequence(string, int);

template<typename T = int>
void evaluate_matrix_x_v2(vector<vector<T>>&, vector<vector<vector<BoolTuple>>>&, string, string, int, int);
template<typename T = int>
void evaluate_matrix_y_v2(vector<vector<T>>&, vector<vector<vector<BoolTuple>>>&, string, string, int, int);

template<typename T = int>
void route_face(vector<vector<T>>&, vector<vector<vector<BoolTuple>>>&, string, string, string&, string&); //Find a way to obtain only the first result
template<typename T>
void route_dev(int, int, int, string, string, vector<string>&, vector<string>&, vector<vector<vector<BoolTuple>>>&, string&, string&);


void multiple_evaluations(vector<string>&, vector<vector<string>>&, vector<vector<int>>&, int, int, bool);
void paper_implementation(vector<string>&, vector<vector<string>>&, vector<vector<int>>&, int, int, bool);



template<typename T = int>
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

bool get_state(vector<BoolTuple>& m, int state) {
	int answer = 0;
	for (int i = state + 1; i < m.size(); i++) {
		answer += i * ((int)m[i].x + (int)m[i].y);
	}
	return (bool)answer; //Retorna true cuando valores superiores a states están ocupados
						// Verdadera si hay caminos por encima del rango de state
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

//Solo funciona con matrices cuadradas
template<typename T>
void duplicate_matrix(vector<vector<T>>& m) {
	for (unsigned int i = 0; i < m.size(); i++) {
		for (unsigned int j = i + 1; j < m.size(); j++) {
			m[j][i] = m[i][j];
		}
	}
}

template<typename T>
int get_value_by_file(vector<vector<T>>& m, int pos) {
	int answer = 0;
	for (unsigned int i = 0; i < m[pos].size(); i++) {
		answer += m[pos][i];
	}
	return answer;
}

string print_sequence(string s, int l) {
	string answer = "";
	if (s.size() < l) {
		string local = "";
		int extras = l - (int)s.size();
		for (int i = 0; i < extras; i++) {
			local += "-";
		}
		cout << local << s << endl;
		answer = local + s;
	}
	else {
		cout << s << endl;
		answer = s;
	}
	return answer;
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
void evaluate_matrix_y_v2(vector<vector<T>>& m, vector<vector<vector<BoolTuple>>>& r, string a_y, string b_x, int pos_x, int pos_y) {
	//Reocrrido vertical de la matriz
	int e_value = 0;
	for (unsigned int i = pos_y; i < m.size(); i++) {
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
		for (unsigned int k = 0; k < directions.size(); k++) {
			//cout << "k: " << k << endl;
			if (directions[k] == false) {
				r[i][pos_x][k].x = 0; //Invalidamos la direccion y no apuntamos a nada
				r[i][pos_x][k].y = 0;
			}
		}
	}
}


template<typename T = int>
void route_face(vector<vector<T>> &m, vector<vector<vector<BoolTuple>>>& r, string a_y, string b_x, string& r_y, string& r_x) {
	a_y = '-' + a_y;
	b_x = '-' + b_x;

	int start_state = 0; //We always start at the left pointer  (true - false) (1, 0)
	int start_y = a_y.size() - 1;//Menos uno porque ya sumamos el guion
	int start_x = b_x.size() - 1;
	vector<string> total_routes_y = vector<string>(0, ""); //Inicializamos un vector vacío
	vector<string> total_routes_x = vector<string>(0, "");

	route_dev<T>(start_y, start_x, 0, "", "", total_routes_y, total_routes_x, r, a_y, b_x);

//	for (unsigned int i = 0; i < total_routes_x.size(); i++) {
//		cout << total_routes_x[i] << endl;
//		cout << total_routes_y[i] << endl << endl;
//	}
	//cout << "Se encontraron: " << total_routes_x.size() << " rutas" << endl;
	r_y = total_routes_y[0];
	r_x = total_routes_x[0];
	return;
}

template<typename T>
void route_dev(int p_y, int p_x, int state, string word_y, string word_x, vector<string>& pool_y, vector<string>& pool_x, vector<vector<vector<BoolTuple>>>& r, string& a_y, string& b_x) {

	if (p_y + p_x == 0) {
		pool_y.push_back(word_y);
		pool_x.push_back(word_x);
		return; //Igual a decir si p_y == 0 y p_x == 0; 
	}

	if (pool_x.size() > 0) {
		return;
	}

	//Solo se aplica si hay una ruta mas arriba de la que conocemos 
	if (get_state(r[p_y][p_x], state)) {
		//Recursividad de nueva ruta
		route_dev<T>(p_y, p_x, state + 1, word_y, word_x, pool_y, pool_x, r, a_y, b_x); //Será inefectivo pero para el siguiente análisis solo sumaremos uno al estado
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
		route_dev<T>(p_y, p_x, 0, word_y, word_x, pool_y, pool_x, r, a_y, b_x);
		return;
	}
	else {
		//cout << "Close way" << endl;
		return;
	}
}

void wunsh_traditional(vector<string>& sequences, vector<vector<vector<string>>>& alignments, vector<vector<int>>& scores, int p_x, int p_y, bool show_p = false) {
	string c_1 = sequences[p_y]; //Importamos de la matriz de secuencias el valor de las cadenas VERTICAL
	string c_2 = sequences[p_x]; //HORIZONTAL

	string c_1_alignment = ""; //para C_1 vertical
	string c_2_alignment = ""; //para c_2 horizontal

	int local_scores; //Result of alignmente those two chains
	vector<vector<int>> pointing_matrix = vector<vector<int>>(c_1.size() + 1, vector<int>(c_2.size() + 1, 0));
	//vector<vector<int>> score_matrix = vector<vector<int>>(c_1.size() + 1, vector<int>(c_2.size() + 1, 0));
	//vector<vector<int>> pointing_matrix = vector<vector<int>>(c_1.size(), vector<int>(c_2.size(), 0));
	vector<vector<int>> score_matrix = vector<vector<int>>(c_1.size(), vector<int>(c_2.size(), 0));

	//Inicializando scoring matrix, 1 arriba, 2 diagonal, 3 izquierda
	for (unsigned int i = 0; i < score_matrix[0].size(); i++) { //Recorriendo horizontalmente la primera fila
		score_matrix[0][i] = 3;
	}
	for (unsigned int i = 0; i < score_matrix.size(); i++) {
		score_matrix[i][0] = 1;
	}
	score_matrix[0][0] = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//completamos la primera fila antes de la iteraccion 0
	//Inicializando pointing matriz
	int decress_iterator = 0;
	for (unsigned int i = 0; i < pointing_matrix[0].size(); i++) {
		pointing_matrix[0][i] = decress_iterator;
		decress_iterator -= 2;
	}
	decress_iterator = 0;
	for (unsigned int i = 0; i < pointing_matrix.size(); i++) {
		pointing_matrix[i][0] = decress_iterator;
		decress_iterator -= 2;
	}

	int decress_value = -2;
	//Hacemos el recorrido vertical con base en C_1
	for (unsigned int i = 1; i < c_1.size(); i++) {
		for (unsigned int j = 1; j < c_2.size(); j++) {
			int diag_aport = 0;
			if (c_1[i - 1] == c_2[j - 1]) {
				diag_aport = 1;
			}
			else {
				diag_aport = -1;
			}
			//Obtenes el valor para esta posicion de la matriz
			int max_value = pointing_matrix[i][j - 1] + decress_value; //Obtenemos valor de lado izquierdo
			int direction = 3;
			if (max_value < (pointing_matrix[i-1][j - 1] + diag_aport)) {
				max_value = pointing_matrix[i-1][j - 1] + diag_aport;
				direction = 2;
			}
			if (max_value < (pointing_matrix[i-1][j] + decress_value) ) {
				max_value = pointing_matrix[i-1][j];
				direction = 1;
			}

			//Asignamos el valor de la direccion a la matriz de scoring directions
			score_matrix[i][j] = direction;
			//Asignamos el valor maximo obtenido en la segunda posicion de la matriz de direccionamiento
			pointing_matrix[i][j] = max_value;

		}
	}

	//Rutas terminado
	local_scores = pointing_matrix[0][c_2.size() - 1];

	//string c_1_alignment = ""; //para C_1 vertical
	//string c_2_alignment = ""; //para c_2 horizontal
	//Pasamos a crear la ruta;
	local_scores = 0;
	int row = score_matrix.size() - 1;
	int col = score_matrix[0].size() - 1;
	while (score_matrix[row][col] != 0) {
		if (score_matrix[row][col] == 1) {
			//c_2_alignment += '-';
			c_2_alignment.push_back('-');
			//c_1_alignment += c_1[row - 1];
			c_1_alignment.push_back(c_1[row - 1]);
			row -= 1;
			local_scores -= 1;
		}
		else if (score_matrix[row][col] == 2) {
			//c_1_alignment += c_1[row - 1];
			c_1_alignment.push_back(c_1[row - 1]);
			//c_2_alignment += c_2[col - 1];
			c_2_alignment.push_back(c_2[col - 1]);
			row -= 1;
			col -= 1;
			if (c_1[row - 1] == c_2[col - 1]) {
				local_scores += 1;
			}
			else {
				local_scores -= 1;
			}
		}
		else {
			//c_2_alignment += c_2[col - 1];
			c_2_alignment.push_back(c_2[col - 1]);
			//c_1_alignment += '-';
			c_1_alignment.push_back('-');
			col -= 1;
			local_scores -= 1;
		}
	}

	if (show_p) {	
		cout << "Alineando: " << c_1 << " -- " << c_2 << endl;
		cout << "Resultado: " << c_1_alignment << " -- " << c_2_alignment << endl;
		cout << "Local Score: " << local_scores << endl;
		cout << "[Old approach]: " << local_scores << endl;
		cout << c_1_alignment << endl;
		cout << c_2_alignment << endl;
		char a;
		cin >> a;
	}

	//Ubicando los alineamientos encontrados, alta probabilidad de condicion de carrera
	alignments[p_y][p_x].push_back(c_2_alignment);
	alignments[p_y][p_x].push_back(c_1_alignment);
	scores[p_y][p_x] = local_scores;
}


void paper_implementation(vector<string>& sequences, vector<vector<vector<string>>>& alignments, vector<vector<int>>& scores, int p_x, int p_y, bool show_p = false) {
	string c_1 = sequences[p_y]; //Importamos de la matriz de secuencias el valor de las cadenas VERTICAL
	string c_2 = sequences[p_x]; //HORIZONTAL

	string c_1_alignment = ""; //para C_1 vertical
	string c_2_alignment = ""; //para c_2 horizontal

	int local_scores; //Result of alignmente those two chains
	vector<vector<int>> pointing_matrix = vector<vector<int>>(2, vector<int>(c_2.size() + 1, 0));
	vector<vector<int>> score_matrix = vector<vector<int>>(c_1.size(), vector<int>(c_2.size(), 0));

	//Inicializando scoring matrix, 1 arriba, 2 diagonal, 3 izquierda
	for (unsigned int i = 0; i < score_matrix[0].size(); i++) { //Recorriendo horizontalmente la primera fila
		score_matrix[0][i] = 3;
	}
	for (unsigned int i = 0; i < score_matrix.size(); i++) {
		score_matrix[i][0] = 1;
	}
	score_matrix[0][0] = 0; 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////


	//completamos la primera fila antes de la iteraccion 0
	//Inicializando pointing matriz
	int decress_iterator = 0;
	for (unsigned int i = 0; i < pointing_matrix[0].size(); i++) {
		pointing_matrix[0][i] = decress_iterator;
		decress_iterator -= 2;
	}
	/*
	cout << c_2 << endl;
	for (unsigned int i = 0; i < pointing_matrix[0].size(); i++) {
		cout << pointing_matrix[0][i];
	}
	cout << endl;
	for (unsigned int i = 0; i < pointing_matrix[1].size(); i++) {
		cout << pointing_matrix[1][i];
	}
	cout << endl;
	*/

	int decress_value = -2;
	//Hacemos el recorrido vertical con base en C_1
	for (unsigned int i = 1; i < c_1.size(); i++) { 
		pointing_matrix[1][0] = i * decress_value;
		for (unsigned int j = 1; j < c_2.size(); j++) {
			int diag_aport = 0;
			if (c_1[i - 1] == c_2[j - 1]) {
				diag_aport = 1;
			}
			else {
				diag_aport = -1;
			}
			//Obtenes el valor para esta posicion de la matriz
			int max_value = pointing_matrix[1][j - 1] + decress_value; //Obtenemos valor de lado izquierdo
			int direction = 3;
			if (max_value < (pointing_matrix[0][j - 1] + diag_aport)) {
				max_value = pointing_matrix[0][j - 1] + diag_aport;
				direction = 2;
			}
			if (max_value < (pointing_matrix[0][j] + decress_value)  ) {
				max_value = pointing_matrix[0][j];
				direction = 1;
			}

			//Asignamos el valor de la direccion a la matriz de scoring directions
			score_matrix[i][j] = direction;
			//Asignamos el valor maximo obtenido en la segunda posicion de la matriz de direccionamiento
			pointing_matrix[1][j] = max_value;
		}
		//Copiamos el valor de la segunda lina del pointing matrix a la primera linea;
		for (unsigned int j = 0; j < c_2.size(); j++) {
			pointing_matrix[0][j] = pointing_matrix[1][j];
			pointing_matrix[1][j] = 0;
		}
	}
	//Terminado, obtenermos el score entre ambas cadenas
	local_scores = pointing_matrix[0][c_2.size() - 1];
	
	//string c_1_alignment = ""; //para C_1 vertical
	//string c_2_alignment = ""; //para c_2 horizontal
	//Pasamos a crear la ruta;
	local_scores = 0;
	int row = score_matrix.size() - 1;
	int col = score_matrix[0].size() - 1;
	while (score_matrix[row][col] != 0) {
		if (score_matrix[row][col] == 1) {
			//c_2_alignment += '-';
			c_2_alignment.push_back('-');
			//c_1_alignment += c_1[row - 1];
			c_1_alignment.push_back(c_1[row - 1]);
			row -= 1;
			local_scores -= 1;
		}
		else if (score_matrix[row][col] == 2) {
			//c_1_alignment += c_1[row - 1];
			c_1_alignment.push_back(c_1[row - 1]);
			//c_2_alignment += c_2[col - 1];
			c_2_alignment.push_back(c_2[col - 1]);
			row -= 1;
			col -= 1;
			if (c_1[row - 1] == c_2[col - 1]) {
				local_scores += 1;
			}
			else {
				local_scores -= 1;
			}
		}
		else {
			//c_2_alignment += c_2[col - 1];
			c_2_alignment.push_back(c_2[col - 1]);
			//c_1_alignment += '-';
			c_1_alignment.push_back('-');
			col -= 1;
			local_scores -= 1;
		}
	}

	if (show_p) {
		
		cout << "Alineando: " << c_1 << " -- " << c_2 << endl;
		cout << "Resultado: " << c_1_alignment << " -- " << c_2_alignment << endl;
		cout << "Local Score: " << local_scores << endl;
		cout << "[New approach]: " << local_scores << endl;
		cout << c_1_alignment << endl;
		cout << c_2_alignment << endl;
		char a;
		cin >> a;
	}

	//Ubicando los alineamientos encontrados, alta probabilidad de condicion de carrera
	alignments[p_y][p_x].push_back(c_2_alignment);
	alignments[p_y][p_x].push_back(c_1_alignment);
	scores[p_y][p_x] = local_scores;
}

//Normal wunshc algorithm
void multiple_evaluations(vector<string>& sequences, vector<vector<vector<string>>>& alignments, vector<vector<int>>& scores, int p_x, int p_y, bool show_p = false) {
	string c_1 = sequences[p_y]; //p_y vertical, p_x horizontal
	string c_2 = sequences[p_x];

	string r_y = "";
	string r_x = "";

	thread t_x; //Threads in charge
	thread t_y;

	int local_score;

	vector<vector<int>> mat = vector<vector<int>>(c_1.size() + 1, vector<int>(c_2.size() + 1, 0));
	vector<vector<vector<BoolTuple>>> rut = vector<vector<vector<BoolTuple>>>(c_1.size() + 1, vector<vector<BoolTuple>>(c_2.size() + 1, vector<BoolTuple>(3, BoolTuple())));
	fill_matrix(mat, rut);

	int pos_x = 1;
	for (unsigned int i = 1; i <= c_1.size(); i++) {

		if (i <= c_2.size()) { //c_2 is for x, must be in range
				t_x = thread(evaluate_matrix_x_v2<int>, std::ref(mat), std::ref(rut), c_1, c_2, pos_x, i);

		}
		t_y = thread(evaluate_matrix_y_v2<int>, std::ref(mat), std::ref(rut), c_1, c_2, pos_x, i);

		if (i <= c_2.size()) {
			t_x.join();
		}

		t_y.join();
		if (pos_x < c_2.size()) {
			pos_x++;
		}
	}

	local_score = mat[mat.size() - 1][mat[0].size() - 1];
	route_face<int>(std::ref(mat), std::ref(rut), c_1, c_2, r_y, r_x);

	if (show_p) {
		/*cout << "Alineando: " << c_1 << " -- " << c_2 << endl;
		cout << "Resultado: " << r_y << " -- " << r_x << endl;
		cout << "Local Score: " << local_score << endl;*/
		cout << "[Old approach]: " << local_score << endl;
		cout << r_y << endl;
		cout << r_x << endl;
		char a;
		cin >> a;
	}


	//Ubicando los alineamientos encontrados, alta probabilidad de condicion de carrera
	alignments[p_y][p_x].push_back(r_x);
	alignments[p_y][p_x].push_back(r_y);
	scores[p_y][p_x] = local_score;
}


void read_data(string ruta, int cantidad, int size,  vector<string>& pool) {
	fstream lector(ruta.c_str(), ios::in);
	string word;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	if (lector.is_open()) {
		for (int i = 0; i < cantidad; i++) {
			lector >> word;
			pool.push_back(word);
		}
	}
	else {
		cout << "No se ha podido leer el documento" << endl;
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration1 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Documento leido en: " << duration1 << " microsegundos" << endl;
	return;

}

#endif