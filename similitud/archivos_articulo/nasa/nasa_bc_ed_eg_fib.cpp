#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
#include <sdsl/vectors.hpp>

using namespace std;
using namespace sdsl;

/*
	Mide el tamaño en KBytes de los 66 primeros sensores de un archivo binario del dataset EEG
*/

/*
	g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o nasa_bc_ed_eg_fib nasa_bc_ed_eg_fib.cpp -lsdsl -ldivsufsort -ldivsufsort64
*/

unsigned int zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

long long int bytes_bitcompress(vector<int> x){
	int_vector<> iv(x.size());
	for(int i=0; i<x.size(); i++){
		iv[i] = zigzag_encode(x[i]);
	}
	util::bit_compress(iv);
	return size_in_bytes(iv);
}

long long int bytes_eliasdelta(vector<int> x){
	int_vector<> iv(x.size());
	for(int i=0; i<x.size(); i++){
		iv[i] = zigzag_encode(x[i]);
	}
	vlc_vector<coder::elias_delta> ed(iv);
	return size_in_bytes(ed);
}

long long int bytes_eliasgamma(vector<int> x){
	int_vector<> iv(x.size());
	for(int i=0; i<x.size(); i++){
		iv[i] = zigzag_encode(x[i]);
	}
	vlc_vector<coder::elias_gamma> eg(iv);
	return size_in_bytes(eg);
}

long long int bytes_eliasfibonacci(vector<int> x){
	int_vector<> iv(x.size());
	for(int i=0; i<x.size(); i++){
		iv[i] = zigzag_encode(x[i]);
	}
	vlc_vector<coder::fibonacci> fib(iv);
	return size_in_bytes(fib);
}

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <filename>" << endl;
		return 0;
	}
//    cout << "DATASET: " << argv[1] << endl;
	int rows, cols, a1, a2, a3, a4;
	// Leyendo datos desde el archivo de entrada
	ifstream txtInput(argv[1], ifstream::in);
	txtInput >> rows >> cols >> a1 >> a2 >> a3 >> a4;
	string fileName;
	vector<vector<vector<int>>> temporalSeries(rows, vector<vector<int>>(cols));
	int lenTempSerie = 0;
//    cout << rows << " " << cols << endl;
//    cout << "Leyendo series temporales..." << endl;
	while(txtInput >> fileName){
		// Abrir el archivo binario en modo binario utilizando ifstream
		ifstream archivo(fileName, ios::binary);
		if(archivo.is_open()) {
			// Leer enteros de 32 bits del archivo
			int entero;
			for(int c = 0; c < cols; c++) {
				for(int f = 0; f < rows; f++){
					archivo.read(reinterpret_cast<char*>(&entero), sizeof(entero));
					temporalSeries[f][c].push_back(entero);
				}
			}
			archivo.close();
		} else {
			cerr << "Error al abrir el archivo: " << fileName << endl;
		}
		lenTempSerie++;
	}
	
	//  Marcando series temporales fijas: todos sus elementos iguales
	vector<vector<bool>> serieFija(rows, vector<bool>(cols, true));
	for(int f=0; f<rows; f++){
		for(int c=0; c<cols; c++){
			for(int i=1; i<lenTempSerie; i++){
				serieFija[f][c] = serieFija[f][c] && (temporalSeries[f][c][i] == temporalSeries[f][c][i-1]);
			}
		}
	}
	
	long long int bytesIV = 0;
	long long int bytesED = 0;
	long long int bytesEG = 0;
	long long int bytesFIB = 0;
	
	for(int f=0; f<rows; f++){
		for(int c=0; c<cols; c++){
			if(!serieFija[f][c]){
				bytesIV += bytes_bitcompress(temporalSeries[f][c]);
				bytesED += bytes_eliasdelta(temporalSeries[f][c]);
				bytesEG += bytes_eliasgamma(temporalSeries[f][c]);
				bytesFIB += bytes_eliasfibonacci(temporalSeries[f][c]);
			}
		}
	}
	int kBytesIV = (bytesIV / 1024);
	int kBytesED = (bytesED / 1024);
	int kBytesEG = (bytesEG / 1024);
	int kBytesFIB = (bytesFIB / 1024);

	cout << "id\tbc\ted\teg\tfib" << endl;
	cout << argv[1] << "\t" << kBytesIV << "\t" << kBytesED << "\t" << kBytesEG << "\t" << kBytesFIB << endl;
}
