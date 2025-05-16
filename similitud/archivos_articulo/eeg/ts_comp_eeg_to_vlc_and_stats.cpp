#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

/*
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o ts_comp_eeg_to_vlc_and_stats ts_comp_eeg_to_vlc_and_stats.cpp -lsdsl -ldivsufsort -ldivsufsort64

*/

vector<map<int,int>> histo(2);
vector<int> minima(2, 0);
vector<int> maxima(2, 0);
vector<int> cantM(2, 0);
vector<float> promedio(2, 0);
vector<float> varianza(2, 0);
vector<float> desvstandar(2, 0);
vector<long long int> acumulado(2, 0);
vector<long long int> acumDE(2,0);

int golombM;
int golombB;
int golombT;

int zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

unsigned int zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

void agregaHisto(vector<int> &v, map<int,int> &m){
	int aux;
	for(int i=0; i<v.size(); i++){
		aux = v[i] / 1000;
		m[aux]++;
	}
}

long long int size_bytes_golomb(int_vector<> x);
vector<long long int> kilobytes_vectores(vector<int> &x);
vector<int> code_1(vector<int> &x);

void estadisticas();

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <histofile>" << endl;
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
	int auxInt;
	for(int i=0; i<electrodos; i++){
		for(int j=0; j<muestras; j++){
			infile.read((char *)&auxInt, sizeof(int));
			temporalSeries[i][j] = auxInt;
		}
	}
	
	vector<long long int> bytesV0(4, 0);		// original	==>> bit_compress, elias_delta, elias_gamma, fibonacci
	vector<long long int> bytesV1(4, 0);		// code1	==>> bit_compress, elias_delta, elias_gamma, fibonacci

	for(int i=0; i<electrodos; i++){
		//	Tamaño datos originales
		vector<long long int> tbytesO = kilobytes_vectores(temporalSeries[i]);
		vector<int> tSerC1 = code_1(temporalSeries[i]);
		vector<long long int> tbytesC = kilobytes_vectores(tSerC1);
		for(int j=0; j<tbytesO.size(); j++){
			bytesV0[j] = tbytesO[j];
			bytesV1[j] = tbytesC[j];
		}
		agregaHisto(temporalSeries[i], histo[0]);
		agregaHisto(tSerC1, histo[1]);
	}

	cout << " ********************** " << endl;
	cout << "\t\tBC\tED\tEG\tFi" << endl;
	cout << "Datos_Original";
	for(int i=0; i<bytesV0.size(); i++){
		cout << "\t" << bytesV0[i];
	}
	cout << "\t[Bytes]" << endl;
	cout << "Datos_Code1";
	for(int i=0; i<bytesV1.size(); i++){
		cout << "\t" << bytesV1[i];
	}
	cout << "\t[Bytes]" << endl;
	cout << " ********************** " << endl;
	cout << "\t\tBC\tED\tEG\tFi" << endl;
	cout << "Datos_Original";
	for(int i=0; i<bytesV0.size(); i++){
		cout << "\t" << (bytesV0[i] / 1024);
	}
	cout << "\t[KiloBytes]" << endl;
	cout << "Datos_Code1";
	for(int i=0; i<bytesV1.size(); i++){
		cout << "\t" << (bytesV1[i] / 1024);
	}
	cout << "\t[KiloBytes]" << endl;
	cout << " ********************** " << endl;

	estadisticas();

	int minmin = (minima[0] < minima[1]) ? minima[0] : minima[1];
	int maxmax = (maxima[0] > maxima[1]) ? maxima[0] : maxima[1];

	ofstream of_histograma(argv[2]);
	cout << "Creando histograma desde valor " << minmin << " hasta valor " << maxmax  << " ..." << endl;
	of_histograma << "% valor\tNoC_0\tNoC_1" << endl;
	for(int i=minmin; i<=maxmax; i++){
		of_histograma << i;
		for(int h=0; h<histo.size(); h++){
			of_histograma << "\t" << histo[h][i];
		}
		of_histograma << endl;
	}
	of_histograma.close();
	cout << "Histograma creado con éxito." << endl;
	return 0;

}

vector<long long int> kilobytes_vectores(vector<int> &x){
	vector<long long int> r(4,0);
	int_vector<> ivBC(x.size());
	for(int i=0; i<x.size(); i++){
		ivBC[i] = zigzag_encode(x[i]);
	}
	util::bit_compress(ivBC);
	r[0] = size_in_bytes(ivBC);
	vlc_vector<coder::elias_delta> vlcED(ivBC);
	r[1] = size_in_bytes(vlcED);
	vlc_vector<coder::elias_gamma> vlcEG(ivBC);
	r[2] = size_in_bytes(vlcEG);
	vlc_vector<coder::fibonacci> vlcFi(ivBC);
	r[3] = size_in_bytes(vlcFi);
	return r;
}

vector<int> code_1(vector<int> &x){
	vector<int> c(x.size());
	c[0] = x[0];
	for(int i=1; i<x.size(); i++){
		c[i] = x[i] - x[i-1];
	}
	return c;
}

void estadisticas(){
	// ************** Minimo, maximo, acumulado y promedio **************
	int aux;
	for(int h=0; h<histo.size(); h++){
		// ------- Histograma -------
		minima[h] = (histo[h].begin())->first;
		maxima[h] = histo[h].begin()->first;
		for(auto it = histo[h].begin(); it != histo[h].end(); it++){
			aux = int(it->first * it->second);
			acumulado[h] += aux;
			cantM[h] += it->second;
			if(it->first < minima[h]){
				minima[h] = it->first;
			}
			if(it->first > maxima[h]){
				maxima[h] = it->first;
			}
		}
		promedio[h] = (0.0 + acumulado[h]) / cantM[h];
		// ************** Varianza y Desviación Estándar **************
		for(auto it = histo[h].begin(); it != histo[h].end(); it++){
			aux = (it->first - promedio[h]);
			acumDE[h] += aux * aux * it->second;
		}
		varianza[h] = acumDE[h] / cantM[h];
		desvstandar[h] = sqrt(varianza[h]);
	}

	// ************** Mostrando **************

	cout << "\tOrig.\tCode 1" << endl;
	cout << "Mínima\t" << minima[0] << "\t" << minima[1] << endl;
	cout << "Máxima\t" << maxima[0] << "\t" << maxima[1] << endl;
	cout << "Media\t" << promedio[0] << "\t" << promedio[1] << endl;
	cout << "Total\t" << acumulado[0] << "\t" << acumulado[1] << endl;
	cout << "NumMues\t" << cantM[0] << "\t" << cantM[1] << endl;
	cout << fixed;
    cout << setprecision(2);
	cout << "Varian\t" << varianza[0] << "\t" << varianza[1] << endl;
	cout << "DesEst\t" << desvstandar[0] << "\t" << desvstandar[1] << endl;
	cout << " ********************** " << endl;

}

