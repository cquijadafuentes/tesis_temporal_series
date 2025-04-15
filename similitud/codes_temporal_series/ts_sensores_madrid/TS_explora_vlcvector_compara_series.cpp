#include <iostream>
#include <sdsl/vectors.hpp>
#include <math.h>

/*
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o TS_explora_vlcvector_compara_series TS_explora_vlcvector_compara_series.cpp -lsdsl -ldivsufsort -ldivsufsort64
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
	int bytes1c = size_in_bytes(ivT);
	int bytes2c = bytes1c;
	int bytes3c = bytes1c;
	int bytes1d = size_in_bytes(vlc_vector<coder::elias_delta>(ivT));
	int bytes2d = size_in_bytes(vlc_vector<coder::elias_delta>(ivT));
	int bytes3d = size_in_bytes(vlc_vector<coder::elias_delta>(ivT));
	int bytes1g = size_in_bytes(vlc_vector<coder::elias_gamma>(ivT));
	int bytes2g = size_in_bytes(vlc_vector<coder::elias_gamma>(ivT));
	int bytes3g = size_in_bytes(vlc_vector<coder::elias_gamma>(ivT));
	int bytes1f = size_in_bytes(vlc_vector<coder::fibonacci>(ivT));
	int bytes2f = size_in_bytes(vlc_vector<coder::fibonacci>(ivT));
	int bytes3f = size_in_bytes(vlc_vector<coder::fibonacci>(ivT));
	int width1 = ivT.width();
	int width2 = width1;
	int width3 = width1;
	int tmin2 = tmin1;
	int tmax2 = tmax1;
	int tmin3 = tmin1;
	int tmax3 = tmax1;
	int tminreal1, tmaxreal1, tminreal2, tmaxreal2;
	cout << " ********************** " << endl;
	cout << "id\tB_c1\tB_c2\tB_c3\tB_d1\tB_d2\tB_d3\tB_g1\tB_g2\tB_g3\tB_f1\tB_f2\tB_f3\tWidth_1\tWidth_2\tWidth_3\tRange_1\tRange_2\tRange_3\tRealR_1\tRealR_2" << endl;
	cout << idsconsultas[0] << "\t" << bytes1c << "\t" << bytes2c << "\t" << bytes3c
	 	<< "\t" << bytes1d << "\t" << bytes2d << "\t" << bytes3d
	 	<< "\t" << bytes1g << "\t" << bytes2g << "\t" << bytes3g
	 	<< "\t" << bytes1f << "\t" << bytes2f << "\t" << bytes3f
		<< "\t" << width1 << "\t" << width2 << "\t" << width3
		<< "\t[" << tmin1 << "," << tmax1 
		<< "]\t[" << tmin2 << "," << tmax2
		<< "]\t[" << tmin3 << "," << tmax3 << "]" << endl;
	vector<int> prev = data[posPrim];
	int totalB1c = bytes1c;
	int totalB2c = bytes2c;
	int totalB3c = bytes3c;
	int totalB1d = bytes1d;
	int totalB2d = bytes2d;
	int totalB3d = bytes3d;
	int totalB1g = bytes1g;
	int totalB2g = bytes2g;
	int totalB3g = bytes3g;
	int totalB1f = bytes1f;
	int totalB2f = bytes2f;
	int totalB3f = bytes3f;
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
		bytes1c = size_in_bytes(ivT1);
		bytes2c = size_in_bytes(ivT2);
		bytes3c = size_in_bytes(ivT3);
		bytes1d = size_in_bytes(vlc_vector<coder::elias_delta>(ivT1));
		bytes2d = size_in_bytes(vlc_vector<coder::elias_delta>(ivT2));
		bytes3d = size_in_bytes(vlc_vector<coder::elias_delta>(ivT3));
		bytes1g = size_in_bytes(vlc_vector<coder::elias_gamma>(ivT1));
		bytes2g = size_in_bytes(vlc_vector<coder::elias_gamma>(ivT2));
		bytes3g = size_in_bytes(vlc_vector<coder::elias_gamma>(ivT3));
		bytes1f = size_in_bytes(vlc_vector<coder::fibonacci>(ivT1));
		bytes2f = size_in_bytes(vlc_vector<coder::fibonacci>(ivT2));
		bytes3f = size_in_bytes(vlc_vector<coder::fibonacci>(ivT3));
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
		cout << idsconsultas[i] << "\t" << bytes1c << "\t" << bytes2c << "\t" << bytes3c
		 	<< "\t" << bytes1d << "\t" << bytes2d << "\t" << bytes3d
		 	<< "\t" << bytes1g << "\t" << bytes2g << "\t" << bytes3g
		 	<< "\t" << bytes1f << "\t" << bytes2f << "\t" << bytes3f
			<< "\t" << width1 << "\t" << width2 << "\t" << width3
			<< "\t[" << tmin1 << "," << tmax1 << "]\t[" << tmin2 << "," << tmax2
			<< "]\t[" << tmin3 << "," << tmax3 << "]"			
			<< "\t[" << tminreal1 << "," << tmaxreal1 << "]\t[" 
			<< tminreal2 << "," << tmaxreal2 << "]" << endl;
		totalB1c += bytes1c;
		totalB2c += bytes2c;
		totalB3c += bytes3c;
		totalB1d += bytes1d;
		totalB2d += bytes2d;
		totalB3d += bytes3d;
		totalB1g += bytes1g;
		totalB2g += bytes2g;
		totalB3g += bytes3g;
		totalB1f += bytes1f;
		totalB2f += bytes2f;
		totalB3f += bytes3f;
		prev = data[posActual];
	}
	cout << " ********************** " << endl;
	cout << "Total \t" << totalB1c << "\t" << totalB2c << "\t" << totalB3c
		<< "\t" << totalB1d << "\t" << totalB2d << "\t" << totalB3d
		<< "\t" << totalB1g << "\t" << totalB2g << "\t" << totalB3g
		<< "\t" << totalB1f << "\t" << totalB2f << "\t" << totalB3f
	 	<< "\t[Bytes]" << endl;
	int totalKB1c = totalB1c / 1024;
	int totalKB2c = totalB2c / 1024;
	int totalKB3c = totalB3c / 1024;
	int totalKB1d = totalB1d / 1024;
	int totalKB2d = totalB2d / 1024;
	int totalKB3d = totalB3d / 1024;
	int totalKB1g = totalB1g / 1024;
	int totalKB2g = totalB2g / 1024;
	int totalKB3g = totalB3g / 1024;
	int totalKB1f = totalB1f / 1024;
	int totalKB2f = totalB2f / 1024;
	int totalKB3f = totalB3f / 1024;
	cout << "Total \t" << totalKB1c << "\t" << totalKB2c << "\t" << totalKB3c
		<< "\t" << totalKB1d << "\t" << totalKB2d << "\t" << totalKB3d
		<< "\t" << totalKB1g << "\t" << totalKB2g << "\t" << totalKB3g
		<< "\t" << totalKB1f << "\t" << totalKB2f << "\t" << totalKB3f
		<< "\t[KiloBytes]" << endl;
	cout << " ********************** " << endl;
	cout << "B_xy: Tamaño en bytes del vector para la estrategia <x> usando el compresor <y>." << endl;
	cout << "Width_x: Cantidad de bits para representar los enteros del int_vector en la estrategia x." << endl;
	cout << "Range_x: Rango de valores almacenado en el int_vector en la estrategia x." << endl;
	cout << "RealR_x: Rango de valores en la operación de diferencia previo al zig-zag-encode en la estrategia x." << endl;
	cout << " ********************** " << endl;
	cout << "Estrategia_1: sensor_actual_i - primer_sensor_i" << endl;
	cout << "Estrategia_2: sensor_actual_i - sensor_anterior_i" << endl;
	cout << "Estrategia_3: sensor_actual_i" << endl;
	cout << " ********************** " << endl;
	cout << "Compresor_c: utll::bit_compress básico de un int_vector." << endl;
	cout << "Compresor_d: coder::elias_delta de un vlc_vector." << endl;
	cout << "Compresor_g: coder::elias_gamma de un vlc_vector." << endl;
	cout << "Compresor_f: coder::fibonacci de un vlc_vector." << endl;
	return 0;
}