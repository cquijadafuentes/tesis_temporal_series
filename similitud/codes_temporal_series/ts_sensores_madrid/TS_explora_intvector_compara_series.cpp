#include <iostream>
#include <sdsl/vectors.hpp>
#include <math.h>

/*
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o TS_explora_intvector_compara_series TS_explora_intvector_compara_series.cpp -lsdsl -ldivsufsort -ldivsufsort64
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

	int posPrim = encuentraPosicion(ids, idsconsultas[0]);
	int_vector<> ivT(muestras);
	int tmin1 = data[posPrim][0];
	int tmax1 = data[posPrim][0];
	for(int i=0; i<muestras; i++){
		ivT[i] = data[posPrim][i];
		if(data[posPrim][i] < tmin1){
			tmin1 = data[posPrim][i];
		}
		if(data[posPrim][i] > tmax1){
			tmax1 = data[posPrim][i];
		}
	}
	util::bit_compress(ivT);
	int bytes1 = size_in_bytes(ivT);
	int bytes2 = bytes1;
	int bytes3 = bytes1;
	int width1 = ivT.width();
	int width2 = width1;
	int width3 = width1;
	int tmin2 = tmin1;
	int tmax2 = tmax1;
	int tmin3 = tmin1;
	int tmax3 = tmax1;
	int tminreal1, tmaxreal1, tminreal2, tmaxreal2;
	cout << " ********************** " << endl;
	cout << "id\tBytes_1\tBytes_2\tBytes_3\tWidth_1\tWidth_2\tWidth_3\tRange_1\tRange_2\tRange_3\tRealR_1\tRealR_2" << endl;
	cout << idsconsultas[0] << "\t" << bytes1 << "\t" << bytes2 << "\t" << bytes3
	 	<< "\t" << width1 << "\t" << width2 << "\t" << width3
		<< "\t[" << tmin1 << "," << tmax1 << "]\t[" << tmin2 << "," << tmax2
		<< "]\t[" << tmin3 << "," << tmax3 << "]" << endl;
	vector<int> prev = data[posPrim];
	int totalB1 = bytes1;
	int totalB2 = bytes2;
	int totalB3 = bytes3;
	for(int i=1; i<idsconsultas.size(); i++){
		int posActual = encuentraPosicion(ids, idsconsultas[i]);
		int_vector<> ivT1(muestras);
		int_vector<> ivT2(muestras);
		int_vector<> ivT3(muestras);
		vector<int> vReal1(muestras);
		vector<int> vReal2(muestras);
		for(int j=0; j<muestras; j++){
			vReal1[j] = data[posActual][j] - data[posPrim][j];
			vReal2[j] = data[posActual][j] - prev[j];
			ivT1[j] = zigzag_encode(vReal1[j]);
			ivT2[j] = zigzag_encode(vReal2[j]);
			ivT3[j] = data[posActual][j];
		}
		util::bit_compress(ivT1);
		util::bit_compress(ivT2);
		util::bit_compress(ivT3);
		bytes1 = size_in_bytes(ivT1);
		bytes2 = size_in_bytes(ivT2);
		bytes3 = size_in_bytes(ivT3);
		width1 = ivT1.width();
		width2 = ivT2.width();
		width3 = ivT3.width();
		tmin1 = ivT1[0];
		tmax1 = ivT1[0];
		tmin2 = ivT2[0];
		tmax2 = ivT2[0];
		tmin3 = ivT3[0];
		tmax3 = ivT3[0];
		tminreal1 = vReal1[0];
		tmaxreal1 = vReal1[0];
		tminreal2 = vReal2[0];
		tmaxreal2 = vReal2[0];
		for(int j=1; j<muestras; j++){
			if(ivT1[j] < tmin1){
				tmin1 = ivT1[j];
			}
			if(ivT1[j] > tmax1){
				tmax1 = ivT1[j];
			}
			if(ivT2[j] < tmin2){
				tmin2 = ivT2[j];
			}
			if(ivT2[j] > tmax2){
				tmax2 = ivT2[j];
			}
			if(ivT3[j] < tmin3){
				tmin3 = ivT3[j];
			}
			if(ivT3[j] > tmax3){
				tmax3 = ivT3[j];
			}
			if(vReal1[j] < tminreal1){
				tminreal1 = vReal1[j];
			}
			if(vReal1[j] > tmaxreal1){
				tmaxreal1 = vReal1[j];
			}
			if(vReal2[j] < tminreal2){
				tminreal2 = vReal2[j];
			}
			if(vReal2[j] > tmaxreal2){
				tmaxreal2 = vReal2[j];
			}
		}
		cout << idsconsultas[i] << "\t" << bytes1 << "\t" << bytes2 << "\t" << bytes3
			<< "\t" << width1 << "\t" << width2 << "\t" << width3
			<< "\t[" << tmin1 << "," << tmax1 << "]\t[" << tmin2 << "," << tmax2
			<< "]\t[" << tmin3 << "," << tmax3 << "]"			
			<< "\t[" << tminreal1 << "," << tmaxreal1 << "]\t[" << tminreal2 << "," << tmaxreal2 << endl;
		totalB1 += bytes1;
		totalB2 += bytes2;
		totalB3 += bytes3;
		prev = data[posActual];
	}
	cout << " ********************** " << endl;
	cout << "Total \t" << totalB1 << "\t" << totalB2 << "\t" << totalB3 << "\t[Bytes]" << endl;
	int totalKB1 = totalB1 / 1024;
	int totalKB2 = totalB2 / 1024;
	int totalKB3 = totalB3 / 1024;
	cout << "Total \t" << totalKB1 << "\t" << totalKB2 << "\t" << totalKB3 << "\t[KiloBytes]" << endl;
	cout << " ********************** " << endl;
	cout << "Bytes_x: Tamaño en bytes del int_vector para la estrategia x." << endl;
	cout << "Width_x: Cantidad de bits para representar los enteros del int_vector en la estrategia x." << endl;
	cout << "Range_x: Rango de valores almacenado en el int_vector en la estrategia x." << endl;
	cout << "RealR_x: Rango de valores en la operación de diferencia previo al zig-zag-encode en la estrategia x." << endl;
	cout << " ********************** " << endl;
	cout << "Estrategia_1: sensor_actual_i - primer_sensor_i" << endl;
	cout << "Estrategia_2: sensor_actual_i - sensor_anterior_i" << endl;
	cout << "Estrategia_3: sensor_actual_i" << endl;
	return 0;
}