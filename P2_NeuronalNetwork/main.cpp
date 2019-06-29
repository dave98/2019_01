
#include <iostream>
#include <windows.h>
#include "adapted_fann/floatfann.c"

using namespace std;

int main()
{
	unsigned int layers[3] = {10, 11, 1}; //2 in input layer, 8 in hidden layer, 9 in hidden layer, 1 in output layer
	struct fann* my_ann = fann_create_standard_array(3, layers);
	fann_print_parameters(my_ann);

	cout << "[TRAINING]" << endl;

	fann_train_on_file(my_ann, "abelone.train", 600, 10, 0.0001);

	cout << "[SAVING]" << endl;
	fann_save(my_ann, "first_training.net");


	float proof[10];
	proof[0] = 0;
	proof[1] = 0;
	proof[2] = 0.353920;
	proof[3] = 0.148646;
	proof[4] = 0.113254;
	proof[5] = 0.040701;
	proof[6] = 0.144753;
	proof[7] = 0.072730;
	proof[8] = 0.033091;
	proof[9] = 0.037162;

	float* output = fann_run(my_ann, proof);
	cout << "[0]: " << output[0] << endl;
	cout << "[1]: " << output[1] << endl;
	cout << "[2]: " << output[2] << endl;
	cout << "[3]: " << output[3] << endl;
	cout << "[4]: " << output[4] << endl;
	cout << "[5]: " << output[5] << endl;
	cout << "[6]: " << output[6] << endl;
	cout << "[7]: " << output[7] << endl;
	cout << "[8]: " << output[8] << endl;
	cout << "[9]: " << output[9] << endl;


	fann_destroy(my_ann);
	return 0;
}
