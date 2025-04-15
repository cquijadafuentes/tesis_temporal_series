#include <iostream>
#include <sdsl/vectors.hpp>
#include <math.h>
#include <map>

using namespace std;
using namespace sdsl;

map<int,int> histoV0;
map<int,int> histoV1;
map<int,int> histoV2;
vector<int> minima(3, 0);
vector<int> maxima(3, 0);
vector<int> promedio(3, 0);
vector<int> varianza(3, 0);
vector<int> desvstandar(3, 0);
vector<int> cantM(3,0);
vector<long long int> acumulado(3, 0);

/*
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o TS_explora_vlcvector_compara_series_doble_diferencia_todos TS_explora_vlcvector_compara_series_doble_diferencia_todos.cpp -lsdsl -ldivsufsort -ldivsufsort64
*/

int zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

unsigned int zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int encuentraPosicion(vector<int> &l, int x){
	for(int i=0; i<l.size(); i++){
		if(l[i] == x){
			return i;
		}
	}
	return -1;
}

void estadisticas();

vector<int> kilobytes_vectores_sin_zze(vector<int> &x){
	vector<int> r(4,0);
	int_vector<> ivBC(x.size());
	for(int i=0; i<x.size(); i++){
		ivBC[i] = x[i];
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

vector<int> kilobytes_vectores_con_zze(vector<int> &x){
	vector<int> r(4,0);
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

vector<int> code_2(vector<int> &x, vector<int> &ref){
	vector<int> c(x.size());
	for(int i=0; i<x.size(); i++){
		c[i] = x[i] - ref[i];
	}
	return c;
}

void agregaHisto(vector<int> &v, map<int,int> &m){
	for(int i=0; i<v.size(); i++){
		m[v[i]]++;
	}
}

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <groupsFile>" << endl;
		cout << "inputFile: archivo con los datos. Formato de atributo individual" << endl;
		cout << "groupsFile: archivo con los ids de los 5 grupos para la estructura." << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	
	cout << "Iniciando lectura para codificación por semana" << endl;

	int aux, auxP;
	int cantGrupos = 5;
	ifstream listaIDS(argv[2], ifstream::in);
	if(listaIDS.fail()){
		cout << "Error! Lectura de " << argv[2] << " fallida." << endl;
		return -1;
	}
	vector<int> cantIds(cantGrupos);
	int totalIds = 0;
	for(int i=0; i<cantGrupos; i++){
		listaIDS >> cantIds[i];
		totalIds += cantIds[i];
	}
	cout << "Total Ids en grupos: " << totalIds << endl;
	map<int, pair<int,int>> mapeoIdsPos;
	vector<int> idsGroups(totalIds, 0);
	auxP = 0;
	for(int i=0; i<cantGrupos; i++){
		for(int j=0; j<cantIds[i]; j++){
			listaIDS >> aux;
			mapeoIdsPos[aux] = make_pair(i,j);
			idsGroups[auxP++] = aux;
		}
	}
	listaIDS.close();

	map<int, pair<int,int>>::iterator mIPit;
	int noEncontrado = 0;

	ifstream dataSensores(argv[1], ifstream::in);
	if(dataSensores.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}

	int sensores, muestras;
	dataSensores >> sensores >> muestras;
	cout << "Cantidad de sensores: " << sensores << endl;
	cout << "Cantidad de muestras: " << muestras << endl;

	if(sensores != totalIds){
		cout << "Lista de datos no coinciden: cantidad de IDs no coinciden." << endl;
	}

	vector<int> idsData(sensores);
	for(int i=0; i<sensores; i++){
		// Lectura de los identificadores de sensores
		dataSensores >> idsData[i];
		mIPit = mapeoIdsPos.find(idsData[i]);
		if(mIPit == mapeoIdsPos.end()){
			//cout << "Lista de datos no coinciden: ID en los datos no está agrupada." << endl;
			noEncontrado++;
		}
	}

	if(sensores - noEncontrado < totalIds){
		cout << "La cantidad de IDs en los datos es " << sensores << "." << endl;
		cout << "La cantidad de IDs en los grupos es " << totalIds << "." << endl;
		if(noEncontrado > 0){
			cout << noEncontrado << " IDs desde la fuente de datos que no se encontraron en los grupos." << endl;
		}
		if (totalIds < (sensores - noEncontrado)){
			cout << ((sensores - noEncontrado) - totalIds) << " IDs desde los grupos que no se encontraron en la fuente de datos." << endl;
		}
		
	}

	// Matriz con los datos
	vector<vector<vector<int>>> data(cantGrupos);
	for(int i=0; i<cantGrupos; i++){
		data[i] = vector<vector<int>>(cantIds[i], vector<int>(muestras));
	}
	pair<int,int> pPos;
	int posG, posI;
	for(int i=0; i<sensores; i++){
		mIPit = mapeoIdsPos.find(idsData[i]);
		if(mIPit != mapeoIdsPos.end()){
			pPos = mapeoIdsPos[idsData[i]];
			posG = pPos.first;
			posI = pPos.second;
			for(int k=0; k<muestras; k++){
				dataSensores >> aux;
				data[posG][posI][k] = aux;
			}
		}else{
			cout << "Error! no se encuentra en el mapa el id " << idsData[i] << endl;
			for(int k=0; k<muestras; k++){
				dataSensores >> aux;
			}
		}	
	}
	dataSensores.close();
	vector<int> bytesV0(4, 0);		// bit_compress, elias_delta, elias_gamma, fibonacci
	vector<int> bytesV1(4, 0);		// bit_compress, elias_delta, elias_gamma, fibonacci
	vector<int> bytesV2(4, 0);		// bit_compress, elias_delta, elias_gamma, fibonacci
	
	for(int f=0; f<data.size(); f++){
		//	Serie de referencia del grupo (la primera)
		//	Tamaño datos originales
		vector<int> tbytes = kilobytes_vectores_sin_zze(data[f][0]);
		for(int i=0; i<4; i++){
			bytesV0[i] += tbytes[i];
		}
		//	Referencia en code1 (se suma al tamaño de code2)
		vector<int> refC1 = code_1(data[f][0]);
		tbytes = kilobytes_vectores_con_zze(refC1);
		for(int i=0; i<4; i++){
			bytesV1[i] += tbytes[i];
			bytesV2[i] += tbytes[i];
		}
		//	Histograma
		agregaHisto(data[f][0], histoV0);
		agregaHisto(refC1, histoV1);
		agregaHisto(refC1, histoV2);
		for(int c=1; c<data[f].size(); c++){
			//	Tamaño de la serie original
			tbytes = kilobytes_vectores_sin_zze(data[f][c]);
			for(int i=0; i<4; i++){
				bytesV0[i] += tbytes[i];
			}
			//	Tamaño de la serie code 1
			vector<int> c1 = code_1(data[f][c]);
			tbytes = kilobytes_vectores_con_zze(refC1);
			for(int i=0; i<4; i++){
				bytesV1[i] += tbytes[i];
			}
			//	Tamaño de la serie code 2
			vector<int> c2 = code_2(data[f][c], refC1);
			tbytes = kilobytes_vectores_con_zze(c2);
			for(int i=0; i<4; i++){
				bytesV2[i] += tbytes[i];
			}
			//	Histograma
			agregaHisto(data[f][c], histoV0);
			agregaHisto(c1, histoV1);
			agregaHisto(c2, histoV2);
		}
	}
	cout << " ********************** " << endl;
	cout << "\t\tBC\tED\tEG\tFi" << endl;
	cout << "Datos Original";
	for(int i=0; i<4; i++){
		cout << "\t" << bytesV0[i];
	}
	cout << "\t[Bytes]" << endl;
	cout << "Datos Code 1";
	for(int i=0; i<4; i++){
		cout << "\t" << bytesV1[i];
	}
	cout << "\t[Bytes]" << endl;
	cout << "Datos Code 2";
	for(int i=0; i<4; i++){
		cout << "\t" << bytesV2[i];
	}
	cout << "\t[Bytes]" << endl;
	cout << " ********************** " << endl;
	cout << "\t\tBC\tED\tEG\tFi" << endl;
	cout << "Datos Original";
	for(int i=0; i<4; i++){
		cout << "\t" << (bytesV0[i] / 1024);
	}
	cout << "\t[KiloBytes]" << endl;
	cout << "Datos Code 1";
	for(int i=0; i<4; i++){
		cout << "\t" << (bytesV1[i] / 1024);
	}
	cout << "\t[KiloBytes]" << endl;
	cout << "Datos Code 2";
	for(int i=0; i<4; i++){
		cout << "\t" << (bytesV2[i] / 1024);
	}
	cout << "\t[KiloBytes]" << endl;
	cout << " ********************** " << endl;

	estadisticas();

	return 0;
}

void estadisticas(){
	// ************** Minimo, maximo, acumulado y promedio **************
	int aux;
	// ------- V0 -------
	minima[0] = (histoV0.begin())->first;
	maxima[0] = histoV0.begin()->first;
	for(auto it = histoV0.begin(); it != histoV0.end(); it++){
		aux = (it->first * it->second);
		acumulado[0] += aux;
		cantM[0] += it->second;
		if(it->first < minima[0]){
			minima[0] = it->first;
		}
		if(it->first > maxima[0]){
			maxima[0] = it->first;
		}
	}
	promedio[0] = acumulado[0] / cantM[0];
	// ------- V1 -------
	minima[1] = (histoV1.begin())->first;
	maxima[1] = histoV1.begin()->first;
	for(auto it = histoV1.begin(); it != histoV1.end(); it++){
		aux = (it->first * it->second);
		acumulado[1] += aux;
		cantM[1] += it->second;
		if(it->first < minima[1]){
			minima[1] = it->first;
		}
		if(it->first > maxima[1]){
			maxima[1] = it->first;
		}
	}
	promedio[1] = acumulado[1] / cantM[1];
	// ------- V2 -------
	minima[2] = (histoV2.begin())->first;
	maxima[2] = histoV2.begin()->first;
	for(auto it = histoV2.begin(); it != histoV2.end(); it++){
		aux = (it->first * it->second);
		acumulado[2] += aux;
		cantM[2] += it->second;
		if(it->first < minima[2]){
			minima[2] = it->first;
		}
		if(it->first > maxima[2]){
			maxima[2] = it->first;
		}
	}
	promedio[2] = acumulado[2] / cantM[2];
	// ************** Varianza y Desviación Estándar **************
	// ------- V0 -------
	long long int acum0 = 0;
	for(auto it = histoV0.begin(); it != histoV0.end(); it++){
		aux = (it->first - promedio[0]);
		acum0 += aux * aux * it->second;
	}
	varianza[0] = acum0 / cantM[0];
	desvstandar[0] = sqrt(varianza[0]);
	// ------- V1 -------
	long long int acum1 = 0;
	for(auto it = histoV1.begin(); it != histoV1.end(); it++){
		aux = (it->first - promedio[1]);
		acum1 += aux * aux * it->second;
	}
	varianza[1] = acum1 / cantM[1];
	desvstandar[1] = sqrt(varianza[1]);
	// ------- V2 -------
	long long int acum2 = 0;
	for(auto it = histoV2.begin(); it != histoV2.end(); it++){
		aux = (it->first - promedio[2]);
		acum2 += aux * aux * it->second;
	}
	varianza[2] = acum2 / cantM[2];
	desvstandar[2] = sqrt(varianza[2]);

	// ************** Mostrando **************

	cout << "\tOrig.\tCode 1\tCode 2" << endl;
	cout << "Mínima\t" << minima[0] << "\t" << minima[1] << "\t" << minima[2] << endl;
	cout << "Máxima\t" << maxima[0] << "\t" << maxima[1] << "\t" << maxima[2] << endl;
	cout << "Media\t" << promedio[0] << "\t" << promedio[1] << "\t" << promedio[2] << endl;
	cout << "Total\t" << acumulado[0] << "\t" << acumulado[1] << "\t" << acumulado[2] << endl;
	cout << "NumMues\t" << cantM[0] << "\t" << cantM[1] << "\t" << cantM[2] << endl;
	cout << "Varian\t" << varianza[0] << "\t" << varianza[1] << "\t" << varianza[2] << endl;
	cout << "DesEst\t" << desvstandar[0] << "\t" << desvstandar[1] << "\t" << desvstandar[2] << endl;
	cout << " ********************** " << endl;

}