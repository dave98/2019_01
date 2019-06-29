#pragma once
#include <iostream>
#include <vector>

using namespace std;

template<typename P = vector<vector<float>>>
void print_m(P vec) {
	for (unsigned int i = 0; i < vec.size(); i++) {
		for (unsigned int j = 0; j < vec[i].size(); j++) {
			cout << vec[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}


template<typename P = vector<float>>
void print_v(P vec, bool is_extended = false) {
	for (unsigned int i = 0; i < vec.size(); i++) {
		cout << vec[i] << " ";
		if (is_extended) {
			cout << endl;
		}
	}
}



