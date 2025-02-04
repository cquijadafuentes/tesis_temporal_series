
#include "TS_SM.hpp"

TempSeriesSensoresMadrid::TempSeriesSensoresMadrid(vector<vector<vector<int>>>&valores, vector<int>&cantidades, vector<int>&ids, int kValue){
	cout << "Construyendo ..." << endl;

	int auxP = 0;
	for(int g=0; g<valores.size(); g++){
		cout << "Grupo " << g << " contiene " << cantidades[g] << " series de tiempo." << endl;
		for(int i=0; i<valores[g].size(); i++){
			cout << "\tid:" << ids[auxP++] << " contiene " << valores[g][i].size() << " muestras." << endl;
		}
	}

}

TempSeriesSensoresMadrid::TempSeriesSensoresMadrid(string inputFilename){
	cout << "Cargando archivo " << inputFilename << endl;
}

bool TempSeriesSensoresMadrid::save(string outputFilename){
	cout << "Guardando archivo " << outputFilename << endl;
	return true;
}

int TempSeriesSensoresMadrid::size_bytes(){
	int bytes = 0;
	return bytes;
}

int TempSeriesSensoresMadrid::size_kbytes(){
	int kbytes = size_bytes() / 1024;
	return kbytes;
}

int TempSeriesSensoresMadrid::size_mbytes(){
	int mbytes = size_bytes() / 1024 / 1024;
	return mbytes;
}

void TempSeriesSensoresMadrid::print(){

	cout << "Mostrando los datos: " << endl;

}

/***********************************************************
						QUERIES
***********************************************************/



/***********************************************************
						PRIVATE
***********************************************************/

void TempSeriesSensoresMadrid::buildRanksSelects(){
	cout << "buildRanksSelects" << endl;
}

unsigned int TempSeriesSensoresMadrid::zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int TempSeriesSensoresMadrid::zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

bool TempSeriesSensoresMadrid::esFija(vector<int> serie){
	for(int j=1; j<serie.size(); j++){
		if(serie[j] != serie[j-1]){
			return false;
		}
	}
	return true;
}
