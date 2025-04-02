#include <iostream>
#include <sdsl/vectors.hpp>

/*
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o TS_semana_referencia TS_semana_referencia.cpp -lsdsl -ldivsufsort -ldivsufsort64
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
			for(int j=0; j<4; j++){
				for(int k=0; k<muestras; k++){
					dataSensores >> aux;
					data[posG][posI][k] = aux;
				}
			}
		}else{
			for(int j=0; j<4; j++){
				for(int k=0; k<muestras; k++){
					dataSensores >> aux;
				}
			}
		}		
	}
	dataSensores.close();

	// Codificaciones por semana
	int timesXweek = 7 * 24 * 4;
	int weeksXyear = 366 / 7;	// 2024 fue bisiesto
	int timesXyear = 366 * 24 * 4;

	long long int kbXgroup = 0;
	long long int kbXsensor = 0;

	// Codificación con serie semana de referencia por grupo

	vector<vlc_vector<coder::fibonacci>> semanasXgrupo(5);
	vector<vlc_vector<coder::fibonacci>> seriesCodificadas;

	for(int g=0; g<data.size(); g++){
		int_vector<> semana(timesXweek);
		for(int s=0; s<data[g].size(); s++){
			for(int k=0; k<muestras; k++){
				semana[k%timesXweek] += data[g][s][k];
			}
		}
		for(int w=0; w<timesXweek; w++){
			semana[w] = int(semana[w] / (data[g].size() * weeksXyear));
		}
		semanasXgrupo[g] = vlc_vector<coder::fibonacci>(semana);
		kbXgroup += int(size_in_bytes(semanasXgrupo[g]) / 1024);

		for(int s=0; s<data[g].size(); s++){
			int_vector<> ivSerie(timesXyear);
			for(int k=0; k<muestras; k++){
				aux = semana[k%timesXweek] - data[g][s][k];
				ivSerie[k] = zigzag_encode(aux);
			}
			vlc_vector<coder::fibonacci> vlcSerie(ivSerie);
			seriesCodificadas.push_back(vlcSerie);
			kbXgroup += int(size_in_bytes(vlcSerie) / 1024);
		}
	}

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

	// Codificación con serie semana de referencia por sensor

	vector<vlc_vector<coder::fibonacci>> semanasXsensor;
	vector<vlc_vector<coder::fibonacci>> seriesCodificadas2;

	for(int g=0; g<data.size(); g++){
		for(int s=0; s<data[g].size(); s++){
			int_vector<> ivSemana(timesXweek);
			for(int k=0; k<muestras; k++){
				ivSemana[k%timesXweek] += data[g][s][k];
			}
			for(int w=0; w<timesXweek; w++){
				ivSemana[w] = int(ivSemana[w] / (data[g].size() * weeksXyear));
			}
			vlc_vector<coder::fibonacci> vlcSemana(ivSemana);
			semanasXsensor.push_back(vlcSemana);
			kbXsensor += int(size_in_bytes(vlcSemana) / 1024);

			int_vector<> ivSerie(timesXyear);
			for(int k=0; k<muestras; k++){
				aux = ivSemana[k%timesXweek] - data[g][s][k];
				ivSerie[k] = zigzag_encode(aux);
			}
			vlc_vector<coder::fibonacci> vlcSerie(ivSerie);
			seriesCodificadas2.push_back(vlcSerie);
			kbXsensor += int(size_in_bytes(vlcSerie) / 1024);
		}
	}

	// Guardando semana x grupo
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

	cout << "***************** Tamaño en memoria *****************" << endl;
	cout << "Sensores codificados por semana del grupo: " << kbXgroup << " [KB]" << endl;
	cout << "Sensores codificados por semana de cada sensor: " << kbXsensor << " [KB]" << endl;

	return 0;
}