#include <iostream>
#include <sdsl/vectors.hpp>
#include <math.h>

/*
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o TS_explora_vlcvector_compara_series_doble_diferencia TS_explora_vlcvector_compara_series_doble_diferencia.cpp -lsdsl -ldivsufsort -ldivsufsort64
*/

using namespace std;
using namespace sdsl;

int zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

unsigned int zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int encuentraPosicion(vector<int> l, int x){
	for(int i=0; i<l.size(); i++){
		if(l[i] == x){
			return i;
		}
	}
	return -1;
}

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile>" << endl;
		cout << "inputFile: archivo con los datos, formato de un solo atributo." << endl;
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
	vector<int> ids(sensores);
	for(int i=0; i<sensores; i++){
		// Descartando información de IDs
		dataSensores >> aux;
		ids[i] = aux;
	}

	vector<vector<int>> data(sensores, vector<int>(muestras));
	for(int i=0; i<sensores; i++){
		for(int j=0; j<muestras; j++){
			dataSensores >> aux;
			data[i][j] = aux;
		}
	}
	dataSensores.close();

	cout << "Ingrese cantidad de sensores a comparar: " << endl;
	int nconsultas;
	cin >> nconsultas;
	cout << "Ingrese los IDs de los " << nconsultas << " sensores: " << endl;
	vector<int> idsconsultas;
	for(int i=0; i<nconsultas; i++){
		cin >> aux;
		idsconsultas.push_back(aux);
	}

	vector<vector<int>> code1(nconsultas, vector<int>(muestras, 0));
	vector<vector<int>> code2(nconsultas, vector<int>(muestras, 0));
	vector<vector<int>> bC1(nconsultas, vector<int>(4, 0));		// bit_compress, elias_delta, elias_gamma, fibonacci
	vector<vector<int>> bC2(nconsultas, vector<int>(4, 0));		// bit_compress, elias_delta, elias_gamma, fibonacci
	vector<vector<int>> widthBC(nconsultas, vector<int>(2, 0));	// code1, code2 (solo para bit_compress)
	vector<vector<int>> rangemin(nconsultas, vector<int>(3, 0));		// original, cod1, cod2
	vector<vector<int>> rangemax(nconsultas, vector<int>(3, 0));		// original, cod1, cod2
	for(int i=0; i<nconsultas; i++){
		int pos = encuentraPosicion(ids, idsconsultas[i]);
		// Rangos en original y code1
		rangemin[i][0] = data[pos][0];
		rangemax[i][0] = data[pos][0];
		rangemin[i][1] = data[pos][0];
		rangemax[i][1] = data[pos][0];
		// Codificación 1
		int_vector<> iv1(muestras);
		code1[i][0] = data[pos][0];
		iv1[0] = zigzag_encode(code1[i][0]);
		for(int j=1; j<muestras; j++){
			// Rangos en original
			if(data[pos][j] < rangemin[i][0]){
				rangemin[i][0] = data[pos][j];
			}
			if(data[pos][j] > rangemax[i][0]){
				rangemax[i][0] = data[pos][j];
			}
			// Codificación 1
			code1[i][j] = data[pos][j] - data[pos][j-1];
			iv1[j] = zigzag_encode(code1[i][j]);
			// Rangos en code1
			if(code1[i][j] < rangemin[i][1]){
				rangemin[i][1] = code1[i][j];
			}
			if(code1[i][j] > rangemax[i][1]){
				rangemax[i][1] = code1[i][j];
			}
		}
		// Rango en code2
		rangemin[i][2] = code1[i][0] - code1[0][0];
		rangemax[i][2] = code1[i][0] - code1[0][0];
		// Codificación 2
		int_vector<> iv2(muestras);
		for(int j=0; j<muestras; j++){
			// Codificación 2
			code2[i][j] = code1[i][j] - code1[0][j];
			iv2[j] = zigzag_encode(code2[i][j]);
			// Rangos en code1
			if(code2[i][j] < rangemin[i][2]){
				rangemin[i][2] = code2[i][j];
			}
			if(code2[i][j] > rangemax[i][2]){
				rangemax[i][2] = code2[i][j];
			}
		}
		// Espacio
		util::bit_compress(iv1);
		bC1[i][0] = size_in_bytes(iv1);
		bC1[i][1] = size_in_bytes(vlc_vector<coder::elias_delta>(iv1));
		bC1[i][2] = size_in_bytes(vlc_vector<coder::elias_gamma>(iv1));
		bC1[i][3] = size_in_bytes(vlc_vector<coder::fibonacci>(iv1));
		util::bit_compress(iv2);
		bC2[i][0] = size_in_bytes(iv2);
		bC2[i][1] = size_in_bytes(vlc_vector<coder::elias_delta>(iv2));
		bC2[i][2] = size_in_bytes(vlc_vector<coder::elias_gamma>(iv2));
		bC2[i][3] = size_in_bytes(vlc_vector<coder::fibonacci>(iv2));
		// Width
		widthBC[i][0] = iv1.width();
		widthBC[i][1] = iv2.width();
	}
	vector<int> bTotal1(4,0);
	vector<int> bTotal2(4,0);
	// Mostrar resultados
	cout << "id\tB_bc1\tB_ed1\tB_eg1\tB_fi1\tB_bc2\tB_ed2\tB_eg2\tB_fi2\twc1\twc2\tro\trc1\trc2" << endl;
	for(int i=0; i<nconsultas; i++){
		cout << idsconsultas[i] << "\t";
		for(int j=0; j<4; j++){
			cout << bC1[i][j] << "\t";
			bTotal1[j] += bC1[i][j];
		}
		for(int j=0; j<4; j++){
			cout << bC2[i][j] << "\t";
			bTotal2[j] += bC2[i][j];
		}
		cout << widthBC[i][0] << "\t" << widthBC[i][1] << "\t";
		for(int j=0; j<3; j++){
			cout << "[" << rangemin[i][j] << "," << rangemax[i][j] <<"]\t";
		}
		cout << endl;

	}
	cout << " ********************** " << endl;
	cout << "Total \t";
	for(int i=0; i<4; i++){
		cout << bTotal1[i] << "\t";
	}
	for(int i=0; i<4; i++){
		cout << bTotal2[i] << "\t";
	}
	cout << "[Bytes]" << endl;
	cout << " ********************** " << endl;
	cout << "Total \t";
	for(int i=0; i<4; i++){
		cout << (bTotal1[i]/1024) << "\t";
	}
	for(int i=0; i<4; i++){
		cout << (bTotal2[i]/1024) << "\t";
	}
	cout << "[KiloBytes]" << endl;
	cout << " ********************** " << endl;
	cout << "B_xxy: Tamaño en bytes del vector para la estrategia <y> usando el compresor <xx>." << endl;
	cout << "Width_x: Cantidad de bits para representar los enteros del int_vector en la estrategia x." << endl;
	cout << "Range_x: Rango de valores almacenado en el int_vector en (o)riginal, code1 o code2." << endl;
	cout << " ********************** " << endl;
	cout << "Estrategia_1: code1 = X[i] - X[i-1]" << endl;
	cout << "Estrategia_2: code2 = X[i] - R[i], donde X es la serie a codificar y R la de referencia, ambas en code1" << endl;
	cout << " ********************** " << endl;
	cout << "bc: utll::bit_compress básico de un int_vector." << endl;
	cout << "ed: coder::elias_delta de un vlc_vector." << endl;
	cout << "eg: coder::elias_gamma de un vlc_vector." << endl;
	cout << "fi: coder::fibonacci de un vlc_vector." << endl;
	return 0;
}