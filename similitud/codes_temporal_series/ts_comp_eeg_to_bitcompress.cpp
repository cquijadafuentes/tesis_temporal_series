#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile>" << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	ifstream infile(argv[1], ofstream::binary);
	if(infile.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}
	int electrodos, muestras;
	// Cargando datos
	infile.read((char *)&electrodos, sizeof(int));
	infile.read((char *)&muestras, sizeof(int));

	cout << "Electrodos: " << electrodos << endl;
	cout << "Muestras: " << muestras << endl;

	vector<vector<int>> temporalSeries(electrodos, vector<int>(muestras));
	float auxFloat;
	for(int i=0; i<electrodos; i++){
		for(int j=0; j<muestras; j++){
			infile.read((char *)&auxFloat, sizeof(float));
			temporalSeries[i][j] = (int)(100*auxFloat);
		}
	}
	
	int maxi = temporalSeries[0][0];
	int mini = maxi;
	for(int i=0; i<electrodos; i++){
		for(int j=0; j<muestras; j++){
			if(temporalSeries[i][j] > maxi){
				maxi = temporalSeries[i][j];
			}
			if(temporalSeries[i][j] < mini){
				mini = temporalSeries[i][j];
			}
		}
	}
	cout << "Valor más grande: " << maxi << endl;
	cout << "Valor más pequeño: " << mini << endl;

	cout << "Comprimiendo...";
	int bytes32bits = 0;
	int bytesBitCompress = 0;
	int bytesEncodedV = 0;
	int bytesVLCV = 0;
	for(int i=0; i<electrodos; i++){
		int_vector<> v(muestras);
		for(int j=0; j<muestras; j++){
			v[j] = temporalSeries[i][j] - mini;
		}
		bytes32bits += size_in_bytes(v);
		util::bit_compress(v);
		bytesBitCompress += size_in_bytes(v);
		enc_vector<> ev(v);
		bytesEncodedV += size_in_bytes(ev);
		vlc_vector<> vv(v);
		bytesVLCV += size_in_bytes(vv);
	}
	int mbytes32bits = bytes32bits/1024/1024;
	int mbytesBitCompress = bytesBitCompress/1024/1024;
	int mbytesEncodedV = bytesEncodedV/1024/1024;
	int mbytesVLCVector = bytesVLCV/1024/1024;

	cout << endl << "Tamaño 32bits: " << mbytes32bits << "[KB]" << endl;
	cout << "Tamaño bitCompress: " << mbytesBitCompress << "[KB]" << endl;
	cout << "Tamaño EncodedVector: " << mbytesEncodedV << "[KB]" << endl;
	cout << "Tamaño VLCVector: " << mbytesVLCVector << "[KB]" << endl;
}
