#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

void codificaPorReferencia(vector<vector<int>> series, vector<int> referencia, string name);

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

    cout << "File: " << argv[1] << endl;
	cout << "Electrodos: " << electrodos << " - Muestras: " << muestras << endl;

	vector<vector<int>> temporalSeries(electrodos, vector<int>(muestras));
	vector<long long> serieAcumulada(muestras, 0);
	float auxFloat;
	for(int i=0; i<electrodos; i++){
		for(int j=0; j<muestras; j++){
			infile.read((char *)&auxFloat, sizeof(float));
			temporalSeries[i][j] = (int)(100*auxFloat);
			serieAcumulada[j] += temporalSeries[i][j];
		}
	}
	// Referencia en 0
	vector<int> referencia(muestras, 0);
	codificaPorReferencia(temporalSeries, referencia, "Directo");

	// Referencia con la primera serie de tiempo
	referencia = temporalSeries[0];
	codificaPorReferencia(temporalSeries, referencia, "refSer1");

	// Referencia con la ultima serie de tiempo
	referencia = temporalSeries[electrodos-1];
	codificaPorReferencia(temporalSeries, referencia, "refSerF");

	// Referencia con la serie de tiempo promedio
	for(int i=0; i<muestras; i++){
		referencia[i] = serieAcumulada[i] / electrodos;
	}
	codificaPorReferencia(temporalSeries, referencia, "refProm");
}


void codificaPorReferencia(vector<vector<int>> series, vector<int> referencia, string name){
	if(referencia.size() != series[0].size()){
		cout << "Tamaño de vector de referencia no coincide con las series." << endl;
		return;
	}
	int electrodos = series.size();
	int muestras = series[0].size();


	int maximo = series[0][0] - referencia[0];
	int minimo = maximo;
	for(int i=0; i<electrodos; i++){
		for(int j=0; j<muestras; j++){
			series[i][j] -= referencia[j];
			if(series[i][j] > maximo){
				maximo = series[i][j];
			}
			if(series[i][j] < minimo){
				minimo = series[i][j];
			}
		}
	}
	cout << "Rango de valores: [" << minimo << " , " << maximo << "]" << endl;

	cout << "Procesando referencia..." << endl;
	int refMin = referencia[0];
	for(int i=0; i<referencia.size(); i++){
		if(referencia[i] < refMin){
			refMin = referencia[i];
		}
	}
	int_vector<> x(muestras);
	for(int i=0; i<muestras; i++){
		x[i] = referencia[i] - refMin;
	}

	// Cálculo del bytes
	long long int bytes32bits = 0;
	long long int bytesBitCompress = 0;
	long long int bytesEncodedV = 0;
	long long int bytesVLCV = 0;

	bytes32bits += size_in_bytes(x);
	util::bit_compress(x);
	bytesBitCompress += size_in_bytes(x);
	enc_vector<> evx(x);
	bytesEncodedV += size_in_bytes(evx);
	vlc_vector<> vlcvx(x);
	bytesVLCV += size_in_bytes(vlcvx);	

	cout << "Comprimiendo series..." << endl;
	for(int i=0; i<electrodos; i++){
		int_vector<> v(muestras);
		for(int j=0; j<muestras; j++){
			v[j] = series[i][j] - minimo;
		}
		bytes32bits += size_in_bytes(v);
		util::bit_compress(v);
		bytesBitCompress += size_in_bytes(v);
		enc_vector<> ev(v);
		bytesEncodedV += size_in_bytes(ev);
		vlc_vector<> vv(v);
		bytesVLCV += size_in_bytes(vv);
	}
	long long int mbytes32bits = bytes32bits/1024/1024;
	long long int mbytesBitCompress = bytesBitCompress/1024/1024;
	long long int mbytesEncodedV = bytesEncodedV/1024/1024;
	long long int mbytesVLCVector = bytesVLCV/1024/1024;

	cout << "Name\t32bits\tbitComp\tEncodV\tVLCV [KB]" << endl;
	cout << name << "\t";
	cout << mbytes32bits << "\t";
	cout << mbytesBitCompress << "\t";
	cout << mbytesEncodedV << "\t";
	cout << mbytesVLCVector << endl;
}
