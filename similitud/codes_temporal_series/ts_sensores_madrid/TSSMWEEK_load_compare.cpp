#include "TS_SM_week.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <originalData>" << endl;
		cout << "inputFile: del formato temporal series sensores madrid week" << endl;
		cout << "originalData: archivo con las medidas de sensores original." << endl;
		return 0;
	}
	// Cargando archivo tssmweek
	TempSeriesSensoresMadridWeek tssmweek(argv[1]);
	tssmweek.stats();

	cout << " -------------------------------------------- " << endl;
	cout << " Cargando desde archivo de datos original" << endl;
	
	ifstream dataSensores(argv[2], ifstream::in);
	if(dataSensores.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}

	int sensores, muestras;
	dataSensores >> sensores >> muestras;
	cout << "Cantidad de sensores: " << sensores << endl;
	cout << "Cantidad de muestras: " << muestras << endl;

	vector<int> idsData(sensores);
	for(int i=0; i<sensores; i++){
		// Lectura de los identificadores de sensores
		dataSensores >> idsData[i];
	}

	// Matriz con los datos
	vector<vector<int>> data(sensores, vector<int>(muestras));
	for(int i=0; i<sensores; i++){
		for(int k=0; k<muestras; k++){
			dataSensores >> data[i][k];
		}
	}
	dataSensores.close();

	cout << "Comparando secuencias:" << endl;
	int noEncontradas = 0;
	for(int i=0; i<sensores; i++){
		vector<int> primera = tssmweek.getSerieByID(idsData[i]);
		if(primera.size() != muestras){
			noEncontradas++;
			cout << "Falló la búsqueda del sensor " << idsData[i] << endl;
		}
		for(int k=0; k<muestras; k++){
			if(data[i][k] != primera[k]){
				cout << "Falló en el valor " << k << " con los datos: " << data[0][k] << "(orig) vs " << primera[k] << "(estr)." << endl;
				return 0;
			}
		}
	}
	cout << "Compración satisfactoria!" << endl;

	return 0;
}