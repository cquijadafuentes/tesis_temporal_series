#include "TS_SM.hpp"

using namespace std;
using namespace sdsl;

int bytesBitCompress(vector<int> &v);

int bytesEncVectorED(vector<int_vector<>> &v);
int bytesEncVectorED(int_vector<> &v);

int bytesEncVectorEG(vector<int_vector<>> &v);
int bytesEncVectorEG(int_vector<> &v);

int bytesEncVectorF(vector<int_vector<>> &v);
int bytesEncVectorF(int_vector<> &v);

int bytesVlcVectorED(vector<int_vector<>> &v);
int bytesVlcVectorED(int_vector<> &v);

int bytesVlcVectorEG(vector<int_vector<>> &v);
int bytesVlcVectorEG(int_vector<> &v);

int bytesVlcVectorF(vector<int_vector<>> &v);
int bytesVlcVectorF(int_vector<> &v);

int main(int argc, char const *argv[]){
	if(argc < 6){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <groupsFile> <N-data> <k> <outputFile>" << endl;
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
	TempSeriesSensoresMadrid tssm2(data, cantIds, idsGroups, kValue, muestras, false);
	TempSeriesSensoresMadrid tssm3(data, cantIds, idsGroups, kValue, muestras, true);

	cout << "------------------------------------" << endl;

	long long int kbytesTSSM = tssm.size_kbytes();
	cout << "\tEstructura:\t" << kbytesTSSM << " [Kbytes]." << endl;

	cout << "------------------------------------" << endl;

	kbytesTSSM = tssm2.size_kbytes();
	cout << "\tEst.Cod.Sec.:\t" << kbytesTSSM << " [Kbytes]." << endl;

	cout << "------------------------------------" << endl;

	kbytesTSSM = tssm3.size_kbytes();
	cout << "\tE.Cod.Sec.TI:\t" << kbytesTSSM << " [Kbytes]." << endl;

	cout << "------------------------------------" << endl;

	long long int bytesRepEnteros = (sizeof(int) * sensores) + (sizeof(int) * sensores * muestras);
	int kbRepEnt = bytesRepEnteros / 1024;
	cout << "\tEnteros:\t" << kbRepEnt << " [Kbytes]." << endl;

	cout << "------------------------------------" << endl;

	long long int bytesIntVector = bytesBitCompress(idsGroups);
	for(int i=0; i<data.size(); i++){
		for(int j=0; j<data[i].size(); j++){
			bytesIntVector += bytesBitCompress(data[i][j]);
		}
	}
	int kbytesIntVector = bytesIntVector / 1024;
	cout << "\tIntVector:\t" << kbytesIntVector << " [Kbytes]." << endl;

	cout << "------------------------------------" << endl;

	cout << "enc_vector Est:" << endl;
	cout << "pgFirstValue: \t" << bytesEncVectorED(tssm.pgFirstValue) << "\t" << bytesEncVectorEG(tssm.pgFirstValue) << "\t" << bytesEncVectorF(tssm.pgFirstValue) << " [Bytes]" << endl;
	cout << "lgFirstValue: \t" << bytesEncVectorED(tssm.lgFirstValue) << "\t" << bytesEncVectorEG(tssm.lgFirstValue) << "\t" << bytesEncVectorF(tssm.lgFirstValue) << " [Bytes]" << endl;
	cout << "pgReference: \t" << bytesEncVectorED(tssm.pgReference) << "\t" << bytesEncVectorEG(tssm.pgReference) << "\t" << bytesEncVectorF(tssm.pgReference) << " [Bytes]" << endl;
	cout << "pgSeries: \t" << bytesEncVectorED(tssm.pgSeries) << "\t" << bytesEncVectorEG(tssm.pgSeries) << "\t" << bytesEncVectorF(tssm.pgSeries) << " [Bytes]" << endl;
	cout << "lgSeries: \t" << bytesEncVectorED(tssm.lgSeries) << "\t" << bytesEncVectorEG(tssm.lgSeries) << "\t" << bytesEncVectorF(tssm.lgSeries) << " [Bytes]" << endl;

	cout << "------------------------------------" << endl;

	cout << "enc_vector Est:" << endl;
	cout << "pgFirstValue: \t" << bytesVlcVectorED(tssm.pgFirstValue) << "\t" << bytesVlcVectorEG(tssm.pgFirstValue) << "\t" << bytesVlcVectorF(tssm.pgFirstValue) << " [Bytes]" << endl;
	cout << "lgFirstValue: \t" << bytesVlcVectorED(tssm.lgFirstValue) << "\t" << bytesVlcVectorEG(tssm.lgFirstValue) << "\t" << bytesVlcVectorF(tssm.lgFirstValue) << " [Bytes]" << endl;
	cout << "pgReference: \t" << bytesVlcVectorED(tssm.pgReference) << "\t" << bytesVlcVectorEG(tssm.pgReference) << "\t" << bytesVlcVectorF(tssm.pgReference) << " [Bytes]" << endl;
	cout << "pgSeries: \t" << bytesVlcVectorED(tssm.pgSeries) << "\t" << bytesVlcVectorEG(tssm.pgSeries) << "\t" << bytesVlcVectorF(tssm.pgSeries) << " [Bytes]" << endl;
	cout << "lgSeries: \t" << bytesVlcVectorED(tssm.lgSeries) << "\t" << bytesVlcVectorEG(tssm.lgSeries) << "\t" << bytesVlcVectorF(tssm.lgSeries) << " [Bytes]" << endl;

	cout << "------------------------------------" << endl;

	TempSeriesSensoresMadrid tssm4(data, cantIds, idsGroups, muestras);
	int kbytesTSSMrefprom = tssm4.size_kbytes();
	cout << "\tEst. RProm:\t" << kbytesTSSMrefprom << " [Kbytes]." << endl;

	cout << "------------------------------------" << endl;

	cout << "enc_vector Est. Prom." << endl;
	cout << "pgFirstValue: \t" << bytesEncVectorED(tssm4.pgFirstValue) << "\t" << bytesEncVectorEG(tssm4.pgFirstValue) << "\t" << bytesEncVectorF(tssm4.pgFirstValue) << " [Bytes]" << endl;
	cout << "lgFirstValue: \t" << bytesEncVectorED(tssm4.lgFirstValue) << "\t" << bytesEncVectorEG(tssm4.lgFirstValue) << "\t" << bytesEncVectorF(tssm4.lgFirstValue) << " [Bytes]" << endl;
	cout << "pgReference: \t" << bytesEncVectorED(tssm4.pgReference) << "\t" << bytesEncVectorEG(tssm4.pgReference) << "\t" << bytesEncVectorF(tssm4.pgReference) << " [Bytes]" << endl;
	cout << "pgSeries: \t" << bytesEncVectorED(tssm4.pgSeries) << "\t" << bytesEncVectorEG(tssm4.pgSeries) << "\t" << bytesEncVectorF(tssm4.pgSeries) << " [Bytes]" << endl;
	cout << "lgSeries: \t" << bytesEncVectorED(tssm4.lgSeries) << "\t" << bytesEncVectorEG(tssm4.lgSeries) << "\t" << bytesEncVectorF(tssm4.lgSeries) << " [Bytes]" << endl;

	cout << "------------------------------------" << endl;

	cout << "vlc_vector Est. Prom." << endl;
	cout << "pgFirstValue: \t" << bytesVlcVectorED(tssm4.pgFirstValue) << "\t" << bytesVlcVectorEG(tssm4.pgFirstValue) << "\t" << bytesVlcVectorF(tssm4.pgFirstValue) << " [Bytes]" << endl;
	cout << "lgFirstValue: \t" << bytesVlcVectorED(tssm4.lgFirstValue) << "\t" << bytesVlcVectorEG(tssm4.lgFirstValue) << "\t" << bytesVlcVectorF(tssm4.lgFirstValue) << " [Bytes]" << endl;
	cout << "pgReference: \t" << bytesVlcVectorED(tssm4.pgReference) << "\t" << bytesVlcVectorEG(tssm4.pgReference) << "\t" << bytesVlcVectorF(tssm4.pgReference) << " [Bytes]" << endl;
	cout << "pgSeries: \t" << bytesVlcVectorED(tssm4.pgSeries) << "\t" << bytesVlcVectorEG(tssm4.pgSeries) << "\t" << bytesVlcVectorF(tssm4.pgSeries) << " [Bytes]" << endl;
	cout << "lgSeries: \t" << bytesVlcVectorED(tssm4.lgSeries) << "\t" << bytesVlcVectorEG(tssm4.lgSeries) << "\t" << bytesVlcVectorF(tssm4.lgSeries) << " [Bytes]" << endl;

	cout << "------------------------------------" << endl;

	return 0;
}

int bytesBitCompress(vector<int> &v){
	int_vector<> iv(v.size());
	for(int i=0; i<v.size(); i++){
		iv[i] = v[i];
	}
	util::bit_compress(iv);
	return size_in_bytes(iv);
}

// ===================================================
//				ENC_VECTOR
// ===================================================

int bytesEncVectorED(vector<int_vector<>> &v){
	int bytes = 0;
	for(int i=0; i<v.size(); i++){
		bytes += bytesEncVectorED(v[i]);
	}
	return bytes;
}

int bytesEncVectorED(int_vector<> &v){
	enc_vector<coder::elias_delta> ev(v);
	return size_in_bytes(ev);
}

int bytesEncVectorEG(vector<int_vector<>> &v){
	int bytes = 0;
	for(int i=0; i<v.size(); i++){
		bytes += bytesEncVectorEG(v[i]);
	}
	return bytes;
}

int bytesEncVectorEG(int_vector<> &v){
	enc_vector<coder::elias_gamma> ev(v);
	return size_in_bytes(ev);
}

int bytesEncVectorF(vector<int_vector<>> &v){
	int bytes = 0;
	for(int i=0; i<v.size(); i++){
		bytes += bytesEncVectorF(v[i]);
	}
	return bytes;
}

int bytesEncVectorF(int_vector<> &v){
	enc_vector<coder::fibonacci> ev(v);
	return size_in_bytes(ev);
}

// ===================================================
//				VLC_VECTOR
// ===================================================

int bytesVlcVectorED(vector<int_vector<>> &v){
	int bytes = 0;
	for(int i=0; i<v.size(); i++){
		bytes += bytesVlcVectorED(v[i]);
	}
	return bytes;
}

int bytesVlcVectorED(int_vector<> &v){
	vlc_vector<coder::elias_delta> vv(v);
	return size_in_bytes(vv);
}

int bytesVlcVectorEG(vector<int_vector<>> &v){
	int bytes = 0;
	for(int i=0; i<v.size(); i++){
		bytes += bytesVlcVectorEG(v[i]);
	}
	return bytes;
}

int bytesVlcVectorEG(int_vector<> &v){
	vlc_vector<coder::elias_gamma> vv(v);
	return size_in_bytes(vv);
}

int bytesVlcVectorF(vector<int_vector<>> &v){
	int bytes = 0;
	for(int i=0; i<v.size(); i++){
		bytes += bytesVlcVectorF(v[i]);
	}
	return bytes;
}

int bytesVlcVectorF(int_vector<> &v){
	vlc_vector<coder::fibonacci> vv(v);
	return size_in_bytes(vv);
}