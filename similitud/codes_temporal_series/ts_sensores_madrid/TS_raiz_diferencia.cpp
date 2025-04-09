#include <iostream>
#include <sdsl/vectors.hpp>
#include <math.h>

/*
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o TS_raiz_diferencia TS_raiz_diferencia.cpp -lsdsl -ldivsufsort -ldivsufsort64
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

	// Codificaciones por semana
	vector<vlc_vector<coder::fibonacci>> raices;
	vector<vlc_vector<coder::fibonacci>> diferencias;
	int auxR, auxD;
	long long int bytes_raices = 0;
	long long int bytes_diffs = 0;

	for(int g=0; g<data.size(); g++){
		for(int s=0; s<data[g].size(); s++){
			int_vector<> ivRaiz(muestras);
			int_vector<> ivDiff(muestras);
			for(int k=0; k<muestras; k++){
				auxR = int(sqrt(data[g][s][k]));
				auxD = data[g][s][k] - (auxR * auxR);
				ivRaiz[k] = auxR;
				ivDiff[k] = auxD;
			}
			vlc_vector<coder::fibonacci> vlcRaiz(ivRaiz);
			vlc_vector<coder::fibonacci> vlcDiff(ivDiff);
			raices.push_back(vlcRaiz);
			diferencias.push_back(vlcDiff);
			bytes_raices += size_in_bytes(vlcRaiz);
			bytes_diffs += size_in_bytes(vlcDiff);
		}
	}

	cout << "Tamaño bytes raices: " << bytes_raices << endl;
	cout << "Tamaño bytes diferencias: " << bytes_diffs << endl;
	long long int totalKB = (bytes_raices + bytes_diffs) / 1024;
	cout << "Tamaño total: " << totalKB << " [KB]" << endl;

	/*
	// Guardando semana x grupo
	string ofn = argv[1];
	ofn += ".weekXgroup";
	ofstream outfile(ofn, ofstream::binary);
	if(!outfile){
		cout << "Error en la carga!" << endl;
		return false;
	}
	for(int i=0; i<semanasXgrupo.size(); i++){
		semanasXgrupo[i].serialize(outfile);
	}
	for(int i=0; i<seriesCodificadas.size(); i++){
		seriesCodificadas[i].serialize(outfile);
	}
	outfile.close();

	// Codificación con serie semana de referencia por cada sensor
	long long int bXsensorRefs = 0;
	long long int bXsensorSeries = 0;

	vector<vlc_vector<coder::fibonacci>> semanasXsensor;
	vector<vlc_vector<coder::fibonacci>> seriesCodificadas2;

	for(int g=0; g<data.size(); g++){
		for(int s=0; s<data[g].size(); s++){
			int_vector<> ivSemana(timesXweek);
			for(int k=0; k<muestras; k++){
				ivSemana[k%timesXweek] += data[g][s][k];
			}
			for(int w=0; w<timesXweek; w++){
				ivSemana[w] = int(ivSemana[w] / weeksXyear);
			}
			vlc_vector<coder::fibonacci> vlcSemana(ivSemana);
			semanasXsensor.push_back(vlcSemana);
			bXsensorRefs += size_in_bytes(vlcSemana);

			int_vector<> ivSerie(timesXyear);
			for(int k=0; k<muestras; k++){
				aux = ivSemana[k%timesXweek] - data[g][s][k];
				ivSerie[k] = zigzag_encode(aux);
			}
			vlc_vector<coder::fibonacci> vlcSerie(ivSerie);
			seriesCodificadas2.push_back(vlcSerie);
			bXsensorSeries += size_in_bytes(vlcSerie);
		}
	}

	// Guardando semana x sensor
	ofn = argv[1];
	ofn += ".weekXsensor";
	ofstream outfile2(ofn, ofstream::binary);
	if(!outfile2){
		cout << "Error en la carga!" << endl;
		return false;
	}
	for(int i=0; i<semanasXsensor.size(); i++){
		semanasXsensor[i].serialize(outfile2);
	}
	for(int i=0; i<seriesCodificadas2.size(); i++){
		seriesCodificadas2[i].serialize(outfile2);
	}
	outfile2.close();

	long long int kbXgroupRefs = (bXgroupRefs / 1024);
	long long int kbXgroupSeries = (bXgroupSeries / 1024);
	long long int kbXgroupTotal = kbXgroupRefs + kbXgroupSeries;
	long long int kbXsensorRefs = (bXsensorRefs / 1024);
	long long int kbXsensorSeries = (bXsensorSeries / 1024);
	long long int kbXsensorTotal = kbXsensorRefs + kbXsensorSeries;

	cout << "***************** Tamaño en memoria *****************" << endl;
	cout << "Sensores codificados por semana del grupo: " << endl;
	cout << "\t" << kbXgroupRefs << "[refs] + " << kbXgroupSeries << " [series] = " << kbXgroupTotal << " [KB]" << endl;
	cout << "Sensores codificados por semana de cada sensor: " << endl;
	cout << "\t" << kbXsensorRefs << "[refs] + " << kbXsensorSeries << " [series] = " << kbXsensorTotal << " [KB]" << endl;


		// Codificación con serie semana de referencia por cada grupo (promedio de la segunda semana)
	long long int bXgroupRefs2 = 0;
	long long int bXgroupSeries2 = 0;

	vector<vlc_vector<coder::fibonacci>> semanasXgrupo2(5);
	vector<vlc_vector<coder::fibonacci>> seriesCodificadas3;

	for(int g=0; g<data.size(); g++){
		int_vector<> semana(timesXweek);
		for(int s=0; s<data[g].size(); s++){
			for(int k=0; k<timesXweek; k++){
				semana[k] += data[g][s][k+timesXweek];
			}
		}
		for(int w=0; w<timesXweek; w++){
			semana[w] = int(semana[w] / data[g].size());
		}
		semanasXgrupo2[g] = vlc_vector<coder::fibonacci>(semana);
		bXgroupRefs2 += size_in_bytes(semanasXgrupo2[g]);

		for(int s=0; s<data[g].size(); s++){
			int_vector<> ivSerie(timesXyear);
			for(int k=0; k<muestras; k++){
				aux = semana[k%timesXweek] - data[g][s][k];
				ivSerie[k] = zigzag_encode(aux);
			}
			vlc_vector<coder::fibonacci> vlcSerie(ivSerie);
			seriesCodificadas3.push_back(vlcSerie);
			bXgroupSeries2 += size_in_bytes(vlcSerie);
		}
	}

	long long int kbXgroupRefs2 = (bXgroupRefs2 / 1024);
	long long int kbXgroupSeries2 = (bXgroupSeries2 / 1024);
	long long int kbXgroupTotal2 = kbXgroupRefs2 + kbXgroupSeries2;

	cout << "Sensores codificados por semana del grupo (promedio de la segunda semana de cada sensor): " << endl;
	cout << "\t" << kbXgroupRefs2 << "[refs] + " << kbXgroupSeries2 << " [series] = " << kbXgroupTotal2 << " [KB]" << endl;

	// Codificación con 2a semana de referencia por cada sensor
	long long int bXsensorRefs2 = 0;
	long long int bXsensorSeries2 = 0;

	vector<vlc_vector<coder::fibonacci>> semanasXsensor2;
	vector<vlc_vector<coder::fibonacci>> seriesCodificadas4;

	for(int g=0; g<data.size(); g++){
		for(int s=0; s<data[g].size(); s++){
			int_vector<> ivSemana(timesXweek);
			for(int k=0; k<timesXweek; k++){
				ivSemana[k] += data[g][s][k+timesXweek];
			}
			vlc_vector<coder::fibonacci> vlcSemana(ivSemana);
			semanasXsensor2.push_back(vlcSemana);
			bXsensorRefs2 += size_in_bytes(vlcSemana);

			int_vector<> ivSerie(timesXyear);
			for(int k=0; k<muestras; k++){
				aux = ivSemana[k%timesXweek] - data[g][s][k];
				ivSerie[k] = zigzag_encode(aux);
			}
			vlc_vector<coder::fibonacci> vlcSerie(ivSerie);
			seriesCodificadas4.push_back(vlcSerie);
			bXsensorSeries2 += size_in_bytes(vlcSerie);
		}
	}

	long long int kbXsensorRefs2 = (bXsensorRefs2 / 1024);
	long long int kbXsensorSeries2 = (bXsensorSeries2 / 1024);
	long long int kbXsensorTotal2 = kbXsensorRefs2 + kbXsensorSeries2;
	cout << "Sensores codificados por 2a semana de cada sensor: " << endl;
	cout << "\t" << kbXsensorRefs2 << "[refs] + " << kbXsensorSeries2 << " [series] = " << kbXsensorTotal2 << " [KB]" << endl;
	*/
	return 0;
}