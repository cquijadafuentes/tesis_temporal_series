#include "TS_SM.hpp"

using namespace std;
using namespace sdsl;

int bytesBitCompress(vector<int> &v);

int main(int argc, char const *argv[]){
	if(argc < 7){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <groupsFile> <N-data> <k> <histograma_datos> <histograma_estructura>" << endl;
		cout << "inputFile: archivo con los datos en el formato usado para STAI" << endl;
		cout << "\t(numero muestras, numero sensores, ids sensores, por cada sensor 4 series de tiempo de largo numero muestras)" << endl;
		cout << "groupsFile: archivo con los ids de los 5 grupos para la estructura." << endl;
		cout << "\t(cantidad de cada grupo, por cada grupo la lista de los ids)" << endl;
		cout << "N-data: Para qué serie de tiempo se crea la estructura (1,2,3 o 4)." << endl;
		cout << "k: Frecuencia para definir series de referencia." << endl;
		cout << "outputFile: nombre del archivo de salida" << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	
	cout << "Iniciando TSSM_build.cpp" << endl;

	int aux, auxP;
	int nData = (int)stoi(argv[3]);
	int kValue = (int)stoi(argv[4]);
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
				if(j == (nData-1)){
					for(int k=0; k<muestras; k++){
						dataSensores >> aux;
						data[posG][posI][k] = aux;
					}
				}else{
					for(int k=0; k<muestras; k++){
						dataSensores >> aux;
					}
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

	TempSeriesSensoresMadrid tssm(data, cantIds, idsGroups, kValue, muestras);
	cout << "tssm.max_value: " << tssm.max_value << endl;
	int mmm = tssm.max_value;
	vector<int> histoDatosOriginales(mmm+1, 0);
	vector<int> histoDatosEstructura(2*(mmm+1), 0);
	long long int acum = 0;
	int cant = 0;

	//			HISTOGRAMA de los DATOS ORIGINALES
	for(int i=0; i<data.size(); i++){
		for(int j=0; j<data[i].size(); j++){
			for(int k=0; k<data[i][j].size(); k++){
				histoDatosOriginales[data[i][j][k]]++;
				acum += data[i][j][k];
				cant++;
			}
		}
	}
	ofstream histoOriginal(argv[5]);
	for(int i=0; i<histoDatosOriginales.size(); i++){
		histoOriginal << i << " " << histoDatosOriginales[i] << endl;
	}
	histoOriginal.close();

	//			HISTOGRAMA de los DATOS de la ESTRUCTURA
	for(int i=0; i<tssm.pgFirstValue.size(); i++){
		histoDatosEstructura[tssm.pgFirstValue[i]]++;
	}
	for(int i=0; i<tssm.pgReference.size(); i++){
		for(int j=0; j<tssm.pgReference[i].size(); j++){
			histoDatosEstructura[tssm.pgReference[i][j]]++;
		}
	}
	for(int i=0; i<tssm.pgSeries.size(); i++){
		for(int j=0; j<tssm.pgSeries[i].size(); j++){
			histoDatosEstructura[tssm.pgSeries[i][j]]++;
		}
	}
	for(int i=0; i<tssm.lgFirstValue.size(); i++){
		histoDatosEstructura[tssm.lgFirstValue[i]]++;
	}
	for(int i=0; i<tssm.lgSeries.size(); i++){
		for(int j=0; j<tssm.lgSeries[i].size(); j++){
			histoDatosEstructura[tssm.lgSeries[i][j]]++;
		}
	}
	ofstream histoEstructura(argv[6]);
	for(int i=0; i<histoDatosEstructura.size(); i++){
		histoEstructura << i << " " << histoDatosEstructura[i] << endl;
	}
	histoEstructura.close();

	cout << "Datos exportados correctamente." << endl;
	cout << "Cantidad de datos: " << cant << endl;
	cout << "Suma acumulada de los valores: " << acum << endl;
	double prom = (0.0 + acum) / cant;
	cout << "Promedio del conjunto de valores: " << prom << endl;


	return 0;
}
