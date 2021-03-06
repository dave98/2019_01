// CMB_MultipleSequence.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include "functions.h"
#include <chrono>

using namespace std;
using namespace chrono;

int CANT_DATOS = 10;
int SIZE_DATOS = 10000;
string RUTA = "secuencias_1010000.txt";
int IS_PAPER = 2; //1,2,3

int main()
{
	vector<string> sequence_pool; //Declarando la matriz con todas las sequencias.
	read_data(RUTA, CANT_DATOS, SIZE_DATOS, sequence_pool);
	/*string s_1 = "ACGGTGAGTGATCACCTAGCACAGGAGTGATCACCTAACGGTGCAGTCACCCGGCGGAGAGCGGTGCAGTCACCTCGGTGCAGCATGCAAGC"; sequence_pool.push_back(s_1);
	string s_2 = "AACGGTGCAGTAGACGAGTGATCACCTACAGGCACCAGTGCACAAGTTCACCAGTTTAACGAAGTGCAGTCACCAGTTGAACA"; sequence_pool.push_back(s_2);
	string s_3 = "AATTCGGTGCAGTCACCACACCTACACCTAACGGACGGAGTGAGTGATCACCTAGCACACCTACACCTAACGGACGG"; sequence_pool.push_back(s_3);
	string s_4 = "ACCTACAAGTCTAAAGTCTAAGTGGAACACCTCTACGAGTGATCACCTAACGCACTAACGGACGGACTCGGCGGACGCGGACGGACTGACT"; sequence_pool.push_back(s_4);
	string s_5 = "AAACCTAACGGTTCGGTGCAGTCACCACTAACGACCTAGCACACCATCACCTATTGGGAAACGG"; sequence_pool.push_back(s_5);
	string s_6 = "TACACGGAGTGAGTGAACGACGATCAATCACCTATCATCACCGGGATAATCACCTGGGAATTCTAT"; sequence_pool.push_back(s_6);
	string s_7 = "CACCTAAGGGTATAACGTCGAACCGAATAACATCATCACATCACCTATCTATGTGTAATCACGCTATGGGGACCTATGCGG"; sequence_pool.push_back(s_7);
	string s_8 = "CTACAGGCACCAGTCACCTAGCACGGAGAGCGGTGCAGAGGAATAATCACCTGG"; sequence_pool.push_back(s_8);
	string s_9 = "ACCTCAACCTCAGGCAACCCAACACGGCAACCCCTCAGGCAACCCAACAC"; sequence_pool.push_back(s_9);
	string s_10 = "AACACACCAACCAACCTCAGGCTCAGGCCGAGGAGAGAGCCTCAGGCCCTCACCCTCAGGCGGC"; sequence_pool.push_back(s_10);
	*/
	vector<vector<int>> score_matriz = vector<vector<int>>(sequence_pool.size(), vector<int>(sequence_pool.size(), 0)); //Matriz cuadrada con todos los scores de todos contras todos
	vector<vector<vector<string>>> alignment_matriz = vector<vector<vector<string>>>(sequence_pool.size(), vector<vector<string>>(sequence_pool.size(), vector<string>(0, ""))); // Matriz con todos los alineamientos

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	cout << "Definiendo rutas" << endl;
	for (unsigned int i = 0; i < sequence_pool.size(); i++) { //For de analisis
		for (unsigned int j = i + 1; j < sequence_pool.size(); j++) {
			//multiple_evaluations(sequence_pool, alignment_matriz, score_matriz, j, i);
			if (IS_PAPER == 1) {
				paper_implementation(sequence_pool, alignment_matriz, score_matriz, j, i);
			}
			else if(IS_PAPER == 2) {
				wunsh_traditional(sequence_pool, alignment_matriz, score_matriz, j, i);
			}
			else {
				multiple_evaluations(sequence_pool, alignment_matriz, score_matriz, j, i);
			}
		}
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration1 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Rutas establecidas en: " << duration1 << " microsegundos" << endl;


	//cout << "Imprimiendo scores" << endl;
	//print_matrix(score_matriz);
	duplicate_matrix(score_matriz);
	//print_matrix(score_matriz);

	int acumulado = get_value_by_file(score_matriz, 0);
	int pos = 0;
	for (unsigned int i = 1; i < score_matriz.size(); i++) {
		int local_acumulated = get_value_by_file(score_matriz, i);
		cout << "S" << i << " -> " << local_acumulated << endl;

		if (acumulado < local_acumulated) {
			acumulado = local_acumulated;
			pos = i;
		}
	}

	cout << "Mayor score acumulado en cadena S" << pos << " con valor de " << acumulado << endl;
	//cout << "Alineando con base en S" << pos << endl;

	int max_size = sequence_pool[pos].size();
	for (unsigned int i = 0; i < pos; i++) {
		int local_size = alignment_matriz[i][pos][1].size();
		if (max_size < local_size) { //pos es Y
			max_size = local_size;
		}
	}
	for (unsigned int i = pos+1; i < score_matriz.size(); i++) {
		int local_size = alignment_matriz[pos][i][0].size();
		if (max_size < local_size) { //pos es Y
			max_size = local_size;
		}
	}

	vector<string> pairwise;
	print_sequence(sequence_pool[pos], max_size);
	for (unsigned int i = 0; i < pos; i++) {
		pairwise.push_back(print_sequence(alignment_matriz[i][pos][1], max_size) );
	}
	for (unsigned int i = pos+1; i < score_matriz.size(); i++) {
		pairwise.push_back(print_sequence(alignment_matriz[pos][i][0], max_size) );
	}
	
	int answer = 0;
	for (unsigned int i = 0; i < pairwise[0].size(); i++) {
		for (unsigned int j = 0; j < pairwise.size(); j++) {
			for (unsigned int k = j + 1; k < pairwise.size(); k++) {
				
				if ((pairwise[j][i] == '-') || (pairwise[k][i] == '-')) {
					answer -= 2;
				}
				else if (pairwise[j][i] == pairwise[k][i]) {
					answer += 1;
				}
				else {
					answer -= 1;
				}
			}
		}
	}

	cout << "Suma por pares: " << answer << endl;


}
