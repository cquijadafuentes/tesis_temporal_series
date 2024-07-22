#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sdsl/vectors.hpp>


using namespace std;
using namespace sdsl;

/*
	Calcula espacio necesario para arreglos de datos con
	1k, 2k, 4k, 8k, y 16k valores, entre rangos de
	0.1k, 1, 10k, 100k, 1M, y 10M
*/

int main(int argc, char const *argv[]){

	vector<int> rango = {100, 1000, 10000, 100000, 1000000, 10000000};
	vector<int> tamaño = {1000, 2000, 4000, 8000, 16000};
	vector<vector<int>> memoria(rango.size(), vector<int>(tamaño.size(), 0));
	for(int i=0; i<rango.size(); i++){
		for(int j=0; j<tamaño.size(); j++){
			int_vector<> v(tamaño[j]);
			generate(v.begin(), v.end(), rand);
			for(int k=0; k<tamaño[j]; k++){
				v[k] = v[k] % rango[i];
			}
			util::bit_compress(v);
			vlc_vector<> vlcv(v);
			memoria[i][j] = (int)(size_in_bytes(vlcv)/1024);
		}
	}

	cout << "Resultados" << endl;
	for(int i=0; i<tamaño.size(); i++){
		cout << "\t" << tamaño[i];
	}
	cout << endl;
	for(int i=0; i<rango.size(); i++){
		cout << rango[i];
		for(int j=0; j<tamaño.size(); j++){
			cout << "\t" << memoria[i][j];
		}
		cout << endl;
	}
	cout << endl;
	
}

/*
g++ -I ~/include -L ~/lib -o pruebas_sdsl_vector_vlcv pruebas_sdsl_vector_vlcv.cpp -lsdsl -ldivsufsort -ldivsufsort64
*/

