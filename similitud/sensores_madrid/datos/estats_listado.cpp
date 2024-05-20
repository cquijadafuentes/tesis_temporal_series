#include <fstream>
#include <iostream>
#include <math.h>
#include <map>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 2){
        cout << "Error! Faltan argumentos." << endl;
        cout << argv[0] << " <w_list_file.txt>" << endl;
        return 0;
    }
    cout << "DATASET: " << argv[1] << endl;
    ifstream lista(argv[1], ifstream::in);
    int num_sensores;
    lista >> num_sensores;		// Se lee la cantidad de sensores
    int aux;
    for(int i=0; i<num_sensores; i++){
    	lista >> aux;	// el identificador de cada sensor
    }
    vector<int> cantVecinos(num_sensores);
    for(int i=0; i<num_sensores; i++){
    	lista >> aux;	// Se lee el identificador
    	lista >> cantVecinos[i];	// la cantidad de vecinos
    	for(int j=0; j<cantVecinos[i]; j++){
    		lista >> aux;	// el id de cada vecino
    	}
    }

    int min = cantVecinos[0];
    int max = cantVecinos[0];
    int sumaV = cantVecinos[0];
    for(int i=1; i<num_sensores; i++){
    	sumaV += cantVecinos[i];
    	if(min > cantVecinos[i]){
    		min = cantVecinos[i];
    	}
    	if(max < cantVecinos[i]){
    		max = cantVecinos[i];
    	}
    }
    cout << "Min de vecinos: " << min << endl;
    cout << "Max de vecinos: " << max << endl;
    double prom = (sumaV + 0.0) / num_sensores;
    cout << "Promedio de vecinos: " << prom << endl;
    cout << aux << endl;
	return 0;
}