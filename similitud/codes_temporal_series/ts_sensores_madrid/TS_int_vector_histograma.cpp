#include <iostream>
#include <sdsl/vectors.hpp>
#include <math.h>

/*
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o TS_int_vector_histograma TS_int_vector_histograma.cpp -lsdsl -ldivsufsort -ldivsufsort64
*/

using namespace std;
using namespace sdsl;

int zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

unsigned int zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <outputFilePrefix>" << endl;
		cout << "inputFile: archivo con los datos. Formato de 4 atributos para STAI" << endl;
		cout << "outputFilePrefix: Prefijo para los archivos de salida con la información." << endl;
		cout << "\tArchivo con el histograma de numero de bits de los int_vector para cada atributo" << endl;
		cout << "\tArchivo con la información de tamaños de las estructuras para cada atributo" << endl;
		cout << "\tArchivo con el rango de valores máx y mín de los int_vector para cada atributo" << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	ifstream dataSensores(argv[1], ifstream::in);
	if(dataSensores.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}

	int sensores, muestras;
	dataSensores >> sensores >> muestras;
	cout << "Cantidad de sensores: " << sensores << endl;
	cout << "Cantidad de muestras: " << muestras << endl;

	int aux;
	for(int i=0; i<sensores; i++){
		// Descartando información de IDs
		dataSensores >> aux;
	}

	vector<vector<int>> histograma(33, vector<int>(4, 0)); // desde 0 a 32, máximo 32 bits
	vector<vector<vector<int>>> limites(sensores, vector<vector<int>>(4, vector<int>(2, 0)));
	vector<long long int> bytes(4, 0);
	int auxV, auxW;
	for(int i=0; i<sensores; i++){
		for(int j=0; j<4; j++){
			int_vector<> ivT(muestras);
			for(int k=0; k<muestras; k++){
				dataSensores >> auxV;
				ivT[k] = auxV;
			}
			util::bit_compress(ivT);
			bytes[j] += size_in_bytes(ivT);
			auxW = ivT.width();
			histograma[auxW][j]++;
			limites[i][j][0] = ivT[0];
			limites[i][j][1] = ivT[0];
			for(int k=1; k<muestras; k++){
				if(ivT[k] < limites[i][j][0]){
					limites[i][j][0] = ivT[k];
				}
				if(ivT[k] > limites[i][j][1]){
					limites[i][j][1] = ivT[k];
				}
			}
		}
	}
	dataSensores.close();

	string filenameHisto = argv[2];
	filenameHisto += ".histo_bits_x_atributo.txt";
	ofstream outHisto(filenameHisto);
	outHisto << "\% value\tinten\tocupa\tcarga\tvelo" << endl;
	for(int i=0; i<histograma.size(); i++){
		outHisto << i;
		for(int j=0; j<4; j++){
			outHisto << "\t" << histograma[i][j];
		}
		outHisto << endl;
	}
	outHisto.close();

	string filenameLimits = argv[2];
	filenameLimits += ".valores_limite_x_sensor.txt";
	ofstream outLims(filenameLimits);
	outLims << "\% sensor\tint_min\tint_max\tocu_min\tocu_max\tcar_min\tcar_max\tvel_min\tvel_max" << endl;
	for(int i=0; i<sensores; i++){
		outLims << i;
		for(int j=0; j<4; j++){
			outLims << "\t" << limites[i][j][0] << "\t" << limites[i][j][1];
		}
		outLims << endl;
	}
	outLims.close();

	string filenameSize = argv[2];
	filenameSize += ".tamaños_int_vector.txt";
	ofstream outSize(filenameSize);
	outSize << "int_vector size: " << endl;
	outSize << "**************************************" << endl;
	for(int i=0; i<4; i++){
		outSize << bytes[i] << "\t";
	}
	outSize << "[Bytes]" << endl;
	outSize << "**************************************" << endl;
	outSize << "inten\tocupa\tcarga\tvelo" << endl;
	for(int i=0; i<4; i++){
		int kbytes = int(bytes[i] / 1024);
		outSize << kbytes << "\t";
	}
	outSize << "[KiloBytes]" << endl;
	outSize << "**************************************" << endl;
	outSize.close();

	return 0;
}