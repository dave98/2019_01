// CMB_SequenceEqualThreads.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include "functions.h"
#include "BoolTuple.h"
#include <chrono>
#include <stdio.h>



using namespace std;
using namespace std::chrono;

int main()
{

	//string c_1 = "GTCCTAC";//"ACTGATTCA";
	//string c_2 = "GTACGTATC";//"ACGCATCA";
	string c_1 = "ACGGTGCAGTCACCAGGCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCAGCAACGGTGCAGTCACCAGGCGAAGTGCAGTCACC";
	string c_2 = "ACGGTGCAGTCACCATTCGGTGCAGTCACCAAAAGGTGCATAACCAGGCGGTGCAGTCACCAGCAACTTTGCAGGGCAGGCGAAGTGCAGTCATT";
	
	//string c_1 = "ACGGTGCACAAGTTCACCAGTTGAACAAATTCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCAGTGCACAAGTTCACCAGTTTAACGAAGTGCAGTCACC";
	//string c_2 = "ACCGTGCAGTTGAACATCGGTGCAGAATTCGGTGCAGTCACCATAACGGTCGGTGCAGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCATCAGTCACCAGGCACCACCAGCGGTGCAG";
	thread t_x;
	thread t_y;

	bool cont = true;

	vector<vector<int>> mat = vector<vector<int>>(c_1.size() + 1, vector<int>(c_2.size() + 1, 0));
	vector<vector<vector<BoolTuple>>> rut = vector<vector<vector<BoolTuple>>>(c_1.size()+1, vector<vector<BoolTuple>>(c_2.size() + 1, vector<BoolTuple>(3, BoolTuple())));


	fill_matrix(mat, rut);
	//print_matrix(mat);
	//print_matrix(rut);

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	int pos_x = 1;
	for (unsigned int i = 1; i <= c_1.size(); i++) {

		if (i <= c_2.size()) { //c_2 is for x, must be in range
			t_x = thread(evaluate_matrix_x<int>, std::ref(mat), std::ref(rut), c_1, c_2, pos_x, i);
			//Throw t_x	
		}
		t_y = thread(evaluate_matrix_y<int>, std::ref(mat), std::ref(rut), c_1, c_2, pos_x, i);

		if (i <= c_2.size()) {
			t_x.join();
		}
		
		t_y.join();

		if (pos_x < c_2.size()) {
			pos_x++;
		}
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration1 = duration_cast<microseconds>(t2 - t1).count();

	cout << "Duracion 1: " << duration1 << "microsegundos"<<endl;


	//print_matrix(mat);
	//print_matrix(rut);

	t1 = high_resolution_clock::now();
	find_routes<int>(std::ref(mat), std::ref(rut), c_1, c_2, true);
	t2 = high_resolution_clock::now();

	auto duration2 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Duracion 2: " << duration2 << " microsegundos"<<endl;
	return 0;

}
