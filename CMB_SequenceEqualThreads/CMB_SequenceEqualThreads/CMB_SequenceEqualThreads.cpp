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


	bool is_global = false;
	bool is_print_routes = false;//Imprimir resultados
	bool is_print_penalti = true;//Proceso para penalidad
	bool print_penalti = true;
		
	//string c_1 = "GTCCTAC";//"ACTGATTCA";
	//string c_2 = "GTACGTATC";//"ACGCATCA";
	//string c_1 = "ACGGTGCAGTCACCAGGCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCAGCAACGGTGCAGTCACCAGGCGAAGTGCAGTCACC";
	//string c_2 = "ACGGTGCAGTCACCATTCGGTGCAGTCACCAAAAGGTGCATAACCAGGCGGTGCAGTCACCAGCAACTTTGCAGGGCAGGCGAAGTGCAGTCATT";
	
	//string c_1 = "ACGGTGCACAAGTTCACCAGTTGAACAAATTCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCAGTGCACAAGTTCACCAGTTTAACGAAGTGCAGTCACC";
	//string c_2 = "ACCGTGCAGTTGAACATCGGTGCAGAATTCGGTGCAGTCACCATAACGGTCGGTGCAGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCATCAGTCACCAGGCACCACCAGCGGTGCAG";
	
	string c_1 = "ATCGAAAGTACATCGAAAGTACAAATCGAAAGTTACGTATATCGAAAGTACATCGAAAGTACAAATCGAAAGTTACGTATCGAAAGTACCGAAAGTAC";
	string c_2 = "CGGTCAAGAAGTTACAATGTAAGTACGCTAGCGGTCATTACAGCAGTACCGGTAGAGGAGCAAGAAGTTACAGGGAAA";

	//string c_1 = "GTACATCCAGAAGTAGTACGCGTATAGTACGTAACACAACAGTTGGAGGTGTGTCGAGTCGAAGTAACAGGTCACAACTGAAGTACGCGATTAGTACGTACCTGA";
	//string c_2 = "CAGCATCAGAAGTACGCGTATAGTACGTAACACAACAGTTGGAGGTGTAGTACGCATATAGTACGTACCTGA";

	//wunch<int> gen(c_1, c_2);
	thread t_x;
	thread t_y;

	bool cont = true;

	vector<vector<int>> mat = vector<vector<int>>(c_1.size() + 1, vector<int>(c_2.size() + 1, 0));
	vector<vector<vector<BoolTuple>>> rut = vector<vector<vector<BoolTuple>>>(c_1.size()+1, vector<vector<BoolTuple>>(c_2.size() + 1, vector<BoolTuple>(3, BoolTuple())));

	if(is_global)
		fill_matrix(mat, rut);
	else
		simple_fill_matrix(rut);
	//print_matrix(mat);
	//print_matrix(rut);
	//print_matrix(gen.m);
	//print_matrix(gen.m);

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	int pos_x = 1;
	for (unsigned int i = 1; i <= c_1.size(); i++) {

		if (i <= c_2.size()) { //c_2 is for x, must be in range
			if(is_global)
				t_x = thread(evaluate_matrix_x_v2<int>, std::ref(mat), std::ref(rut), c_1, c_2, pos_x, i);
			else
				t_x = thread(simple_evaluate_matrix_x<int>, std::ref(mat), std::ref(rut), c_1, c_2, pos_x, i);
		}
		if(is_global)
			t_y = thread(evaluate_matrix_y_v2<int>, std::ref(mat), std::ref(rut), c_1, c_2, pos_x, i);
		else
			t_y = thread(simple_evaluate_matrix_y<int>, std::ref(mat), std::ref(rut), c_1, c_2, pos_x, i);


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
	cout << "Matriz creada en: " << duration1 << " microsegundos"<<endl;

	if(is_global)
		print_score(mat);

	if(is_global)
		route_face<int>(std::ref(mat), std::ref(rut), c_1, c_2, is_print_routes, is_print_penalti, print_penalti);	
	else
		simple_route_face<int>(std::ref(mat), std::ref(rut), c_1, c_2, is_print_routes, is_print_penalti, print_penalti);
	return 0;
}
