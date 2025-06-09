#include <iostream>
#include <fstream>
#include <vector>
#include <sdsl/vectors.hpp>

using namespace std;
using namespace sdsl;

/*
	Mide el tamaño en KBytes de los 66 primeros sensores de un archivo binario del dataset EEG
*/

/*
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o size_bin_to_vlc size_bin_to_vlc.cpp -lsdsl -ldivsufsort -ldivsufsort64
*/

unsigned int zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int bytes_bitcompress(vector<int> x){
	int_vector<> iv(x.size());
	for(int i=0; i<x.size(); i++){
		iv[i] = zigzag_encode(x[i]);
	}
	util::bit_compress(iv);
	return size_in_bytes(iv);
}

int bytes_eliasdelta(vector<int> x){
	int_vector<> iv(x.size());
	for(int i=0; i<x.size(); i++){
		iv[i] = zigzag_encode(x[i]);
	}
	vlc_vector<coder::elias_delta> ed(iv);
	return size_in_bytes(ed);
}

int bytes_eliasgamma(vector<int> x){
	int_vector<> iv(x.size());
	for(int i=0; i<x.size(); i++){
		iv[i] = zigzag_encode(x[i]);
	}
	vlc_vector<coder::elias_gamma> eg(iv);
	return size_in_bytes(eg);
}

int bytes_eliasfibonacci(vector<int> x){
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
		cout << argv[0] << " <inputFile>" << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	string ifilename(argv[1]);
	ifstream infile(ifilename, ofstream::binary);
	if(infile.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}
	int electrodos, muestras;
	// Cargando datos
	infile.read((char *)&electrodos, sizeof(int));
	infile.read((char *)&muestras, sizeof(int));

	cout << "File: " << argv[1] << endl;
	cout << "Electrodos: " << electrodos << " - Muestras: " << muestras << endl;
	
	vector<vector<int>> eegData(electrodos, vector<int>(muestras));
	for(int i=0; i<electrodos; i++){
		for(int k=0; k<muestras; k++){
			infile.read((char *)&eegData[i][k], sizeof(int));
		}
	}

	int bytesIV = 0;
	int bytesED = 0;
	int bytesEG = 0;
	int bytesFIB = 0;
	for(int i=0; i<66; i++){
		bytesIV += bytes_bitcompress(eegData[i]);
		bytesED += bytes_eliasdelta(eegData[i]);
		bytesEG += bytes_eliasgamma(eegData[i]);
		bytesFIB += bytes_eliasfibonacci(eegData[i]);
	}

	int kBytesIV = (bytesIV / 1024);
	int kBytesED = (bytesED / 1024);
	int kBytesEG = (bytesEG / 1024);
	int kBytesFIB = (bytesFIB / 1024);

	cout << "id\tbc\ted\teg\tfib" << endl;
	cout << ifilename << "\t" << kBytesIV << "\t" << kBytesED << "\t" << kBytesEG << "\t" << kBytesFIB << endl;

}