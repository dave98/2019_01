#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>


using namespace std;

double rand_generator(double, double);
double heavy_generator(double, double, double);
double findMod(double a, double b);

typedef std::chrono::high_resolution_clock my_clock;
my_clock::time_point beginning = my_clock::now();

//For random engine
unsigned e_seed = my_clock::now().time_since_epoch().count();
default_random_engine e_generator(e_seed);

uniform_real_distribution<double> distributionDouble(0.0, 10.0);


int main(){
  int n_examples = 8000; //Numero de ejemplos a conseguir
  double max_values_for_characteristics = 1.0;

  double n_rates = max_values_for_characteristics / (double)n_examples;
  cout<<"Development rate"<<n_rates<<endl;

  double neurocitism = 0.0;
  double openess = 0.0;
  double conscientiousness = 0.0;

    //double f_1 = 0.0;
  double f_2 = 0.0;
  double f_3 = 0.0;
  double f_4 = 0.0;
  double f_5 = 0.0;
    //double f_6 = 0.0;
  double f_7 = 0.0;
  double f_8 = 0.0;
    //double f_9 = 0.0;
  double f_10 = 0.0;
  double f_11 = 0.0;
  double f_12 = 0.0;
  double f_13 = 0.0;
  double f_14 = 0.0;
  double f_15 = 0.0;
  double f_16 = 0.0;
  double f_17 = 0.0;
  double f_18 = 0.0;
  double f_19 = 0.0;
  double f_20 = 0.0;


  //Abriendo documento
    ofstream ofs;
    ofs.open("non_sintetic.txt", ofstream::out | ofstream::app);


  for(unsigned int i = 0;  i < n_examples; i++){
      neurocitism = rand_generator(0.0, 1.0);
      openess = rand_generator(0.0, 1.0);
      conscientiousness = rand_generator(0.0, 1.0);

      f_2 = heavy_generator(neurocitism, 0.0, 1.0);
      //f_1 = 1.0 - f_2;
      f_3 = heavy_generator(openess, 0.0, 0.5);
      f_4 = heavy_generator(neurocitism, 0.0, 1.0);
      f_5 = heavy_generator(conscientiousness, 0.0, 1.0);
      //f_6 = 1.0 - f_5;
      f_7 = heavy_generator(neurocitism, 0.0, 1.0);
      f_8 = heavy_generator(conscientiousness, 0.0, 1.0);
      //f_9 = 1.0 - f_8;
      f_10 = heavy_generator(neurocitism, 0.0, 1.0);
      f_11 = 1.0 - heavy_generator(neurocitism, 0.0, 1.0);
      f_12 = 1.0 - heavy_generator(neurocitism, 0.0, 1.0);
      f_13 = heavy_generator(conscientiousness, 0.0, 1.0);
      f_14 = 1.0 - heavy_generator(neurocitism, 0.0, 1.0);
      f_15 = heavy_generator(openess, 0.0, 1.0);
      f_16 = 1.0 - heavy_generator(conscientiousness, 0.0, 1.0);
      f_17 = heavy_generator(openess, 0.0, 1.0);
      f_18 = 1.0 - heavy_generator(conscientiousness, 0.0, 1.0);
      f_19 = 1.0 - heavy_generator(conscientiousness, 0.0, 1.0);
      f_20 = heavy_generator(neurocitism, 0.0, 1.0);

      /*cout<<f_1<<" "<<f_2<<" "<<f_3<<" "<<f_4<<" "<<f_5<<" "<<f_6<<" "<<f_7<<" "<<f_8<<" "<<f_9
      <<" "<<f_10<<" "<<f_11<<" "<<f_12<<" "<<f_13<<" "<<f_14<<" "<<f_15<<" "<<f_16<<" "<<f_17
      <<" "<<f_18<<" "<<f_19<<" "<<f_20<<" ---> "<<openess<<" - "<<conscientiousness<<" - "<<neurocitism<<endl;
      */
      //ofs << f_1  << " ";
      ofs << f_2  << " ";
      ofs << f_3  << " ";
      ofs << f_4  << " ";
      ofs << f_5  << " ";
      //ofs << f_6  << " ";
      ofs << f_7  << " ";
      ofs << f_8  << " ";
      //ofs << f_9  << " ";
      ofs << f_10  << " ";
      ofs << f_11  << " ";
      ofs << f_12  << " ";
      ofs << f_13  << " ";
      ofs << f_14  << " ";
      ofs << f_15  << " ";
      ofs << f_16  << " ";
      ofs << f_17  << " ";
      ofs << f_18  << " ";
      ofs << f_19  << " ";
      //ofs << f_20  << " "<<endl;
      ofs << f_20  << " ";
      ofs << openess <<" "<<conscientiousness<<" "<<neurocitism<<endl;
      cout << openess <<" "<<conscientiousness<<" "<<neurocitism<<endl;

      //neurocitism +=  n_rates;
      //openess +=  n_rates;
      //conscientiousness +=  n_rates;
  }
  return 0;
}


double rand_generator(double min, double max){
  my_clock::duration d = my_clock::now() - beginning;
  unsigned _seed = d.count();
  std::minstd_rand0 generator(_seed);


  //lower_bound must be
  double lower_bound = min;
  double upper_bound = max;
  //int _random_number = generator();
  double _random_number = distributionDouble(e_generator);

  double _random_number_2 = _random_number;
  double answer = 0.0;

  answer = fmod((double)_random_number_2, upper_bound);
  while(answer < lower_bound){
    answer += lower_bound;
    answer = fmod(answer, upper_bound);
  }

  //cout<<"[ "<<lower_bound<<" ; "<<upper_bound<<" ]:  "<<answer<<endl;
  return answer;
}


double heavy_generator(double effect, double min, double max){
  //Works with lapses
    //--> 0 - 1.25
    //--> 1.25 2.50
    //--> 2.50 3.75
    //--> 3.75 5.00
    //////////////////////////////////
  double number_of_layers = 10.0;
  double inner_max = max / number_of_layers;
  double inner_min = 0.0;
  double answer = 0.0;

  if(effect < 0.1){
    inner_min =  0.0*inner_max;
    inner_max =  1.0*inner_max;
  }
  else if(effect < 0.2){
    inner_min = 1.0*inner_max;
    inner_max = 2.0*inner_max;
  }
  else if(effect < 0.3){
    inner_min = 2.0*inner_max;
    inner_max = 3.0*inner_max;
  }
  else if(effect < 0.4){
    inner_min = 3.0*inner_max;
    inner_max = 4.0*inner_max;
  }
  else if(effect < 0.5){
    inner_min = 4.0*inner_max;
    inner_max = 5.0*inner_max;
  }
  else if(effect < 0.6){
    inner_min = 5.0*inner_max;
    inner_max = 6.0*inner_max;
  }
  else if(effect < 0.7){
    inner_min = 6.0*inner_max;
    inner_max = 7.0*inner_max;
  }
  else if(effect < 0.8){
    inner_min = 7.0*inner_max;
    inner_max = 8.0*inner_max;
  }
  else if(effect < 0.9){
    inner_min = 8.0*inner_max;
    inner_max = 9.0*inner_max;
  }
  else{
    inner_min = 9.0*inner_max;
    inner_max = 10.0*inner_max;
  }
  answer = rand_generator(inner_min, inner_max);
  return answer;
}

double findMod(double a, double b){
    // Handling negative values
    if (a < 0)
        a = -a;
    if (b < 0)
        b = -b;

    // Finding mod by repeated subtraction
    double mod = a;
    while (mod >= b)
        mod = mod - b;

    // Sign of result typically depends
    // on sign of a.
    if (a < 0)
        return -mod;

    return mod;
}
