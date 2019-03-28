
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <chrono>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;

void find_c(char*, char*, int, int);
void set_mat(int*, int, int);
void print_matrix(int*, int, int);
__global__ void find_lineal(char*, char*, int, int, int*);

int main()
{
	//Works as Chain_1 for Y and Chain_2 for X
	string chain_1;
	string chain_2;
	
	cout << "Chain_1: " << endl;
	cin >> chain_1;
	cout << "Chain_2: " << endl; 
	cin >> chain_2;
	//-->Add input analyzer

	//cout << chain_1.size() << " ; " << chain_2.size() << endl;

	char* c_1 = new char[(int)chain_1.size()];
	char* c_2 = new char[(int)chain_2.size()];
	strcpy(c_1, chain_1.c_str());
	strcpy(c_2, chain_2.c_str());

	find_c(c_1, c_2, (int)chain_1.size(), (int)chain_2.size()); //Size proved More or Less

	return 0;
}

void set_mat(int* m, int x, int y) {
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			m[i * y + j] = 0;
		}
	}
	
	int temp = 0;
	for (int i = 0; i < y; i++) {
		m[i * y] = temp;
		temp -= 2;
	}

	temp = 0;
	for (int j = 0; j < x; j++) {
		m[j] = temp;
		temp -= 2;
	}
	
	//print_matrix(m, x, y);
}

void print_matrix(int* m, int x, int y) {
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			cout << m[i * y + j] << " ";
		}
		cout << endl;
	}		
}

void find_c(char* c_1, char* c_2, int s_1, int s_2) {
	int c_1_size = s_1 * sizeof(char);
	int c_2_size = s_2 * sizeof(char);
	int mat_size = s_1 * s_2 * sizeof(int); // Takes multiplication between  
	
	
	int* mat = new int[mat_size]; // Through all function
	set_mat(mat, s_1, s_2); //Matriz Configured
	print_matrix(mat, s_1, s_2);

/* Rewiewed
	cout << "Mat_size: " << mat_size << endl;
	cout << "c_1_size: " << c_1_size << endl;
	cout << "c_2_size: " << c_2_size << endl;
	cout << c_1 << "_" << endl;
	cout << c_2 << "_" << endl;
*/

	char* d_c_1;
	char* d_c_2;
	int* d_mat;

	cudaMalloc((void**) &d_mat, mat_size);
	cudaMalloc((void**) &d_c_1, c_1_size);
	cudaMalloc((void**) &d_c_2, c_2_size);

	cudaMemcpy(d_c_1, c_1, c_1_size, cudaMemcpyHostToDevice); //Sending chain characters from PC to Videocard
	cudaMemcpy(d_c_2, c_2, c_2_size, cudaMemcpyHostToDevice); //
	cudaMemcpy(d_mat, mat, mat_size, cudaMemcpyHostToDevice);


	float block = 16;
	dim3 grid_size(ceil(c_1_size / block), ceil(c_2_size/block), 1);
	dim3 block_size(block, block, 1);
	//Core here
	find_lineal <<< grid_size, block_size >>> (d_c_1, d_c_2, c_1_size, c_2_size, d_mat);

	cudaMemcpy(mat, d_mat, mat_size, cudaMemcpyDeviceToHost);
	cudaFree(d_c_1);
	cudaFree(d_c_2);
	cudaFree(d_mat);

	print_matrix(mat, s_1, s_2);
}

//Remember first c_1 is for Y axis and c_2 for X axis
__global__ void find_lineal(char* c_1, char* c_2, int s_1, int s_2, int* mat) {
	int gpu_columna = (blockDim.x * blockIdx.x) + threadIdx.x; // Left to right
	int gpu_fila = (blockDim.y * blockIdx.y) + threadIdx.y; //  Up to down

	//Inefficient implemntation using 1 thread (1,1)
	if ((gpu_fila == 1) && (gpu_columna == 1)) {
		printf("Im looking at you \n");
		printf(" %d - %d \n", s_1, s_2);

		for (int i = 1; i < s_1; i++) {
			
			for (int j = 1; j < s_2; j++) {
			
				int pos = (i * s_2 + j);
				int pos_1 = (i * s_2 + (j - 1) );
				int pos_2 = ( (i - 1) * s_2 + (j - 1));
				int pos_3 = ( (i - 1) * s_2 + j);

				printf("( %d - %d - %d - %d -- )\n", pos, pos_1, pos_2, pos_3);
				printf("( %d - %d - %d - %d )\n", mat[pos], mat[pos_1], mat[pos_2], mat[pos_3]);


				int extra_value = 0;
				if (c_1[i-1] != c_2[j-1]) {
					extra_value = -1;
				}
				else { extra_value = 1; }

				int value = mat[pos_1];
				if (value < (mat[pos_2] + extra_value)) {
					value = (mat[pos_2] + extra_value);
				}
				else if (value < mat[pos_3]) {
					value = mat[pos_3];
				}
							
				mat[pos] = value;
			}
			printf("\n");
		}
		printf("\n");

	}

}