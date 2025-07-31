#include "TempRaster_QuadComp.hpp"

using namespace std;
using namespace sdsl;

vector<int> compNumbIntV(int_vector<> x, int_vector<> y){
	vector<int> r;
	int tope = x.size();
	if(x.size() > y.size()){
		tope = y.size();
	}
	for(int i=0; i<tope; i++){
		if(x[i] != y[i]){
			r.push_back(i);
		}
	}
	for(int i=tope; i<x.size(); i++){
		r.push_back(i);
	}
	for(int i=tope; i<y.size(); i++){
		r.push_back(i);
	}
	return r;
}

vector<int> compNumbSDV(sd_vector<> x, sd_vector<> y){
	vector<int> r;
	int tope = x.size();
	if(x.size() > y.size()){
		tope = y.size();
	}
	for(int i=0; i<tope; i++){
		if(x[i] != y[i]){
			r.push_back(i);
		}
	}
	for(int i=tope; i<x.size(); i++){
		r.push_back(i);
	}
	for(int i=tope; i<y.size(); i++){
		r.push_back(i);
	}
	return r;
}

int main(int argc, char const *argv[]){
	if(argc < 5){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <mapping_sensor_matrix> <d_cuadrante> <outputFile>" << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	string ifilename(argv[1]);
	ifstream infile(ifilename, ofstream::binary);
	if(infile.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}
	int dcuad = stoi(argv[3]);
	int electrodos, muestras;
	// Cargando datos
	infile.read((char *)&electrodos, sizeof(int));
	infile.read((char *)&muestras, sizeof(int));

	cout << "File: " << argv[1] << endl;
	cout << "Electrodos: " << electrodos << " - Muestras: " << muestras << endl;
	
	vector<vector<int>> eegData(electrodos, vector<int>(muestras));
	for(int i=0; i<electrodos; i++){
		for(int k=0; k<muestras; k++){
			infile.read((char *)&eegData[i][k], sizeof(int));
		}
	}

	//	Lectura de matriz con el mapeo de los ids a las celdas de la matriz
    //	y construcción de la matriz en base al mapeo y los datos del eeg
	ifstream eegMappingMatrix(argv[2], ifstream::in);
    if(eegMappingMatrix.fail()){
        cout << "Error! Lectura de " << argv[2] << " fallida." << endl;
        return -1;
    }
    int matrixInputs;
    eegMappingMatrix >> matrixInputs;
    int pos, f, c;
    vector<vector<vector<int>>> temporalSeries(9, vector<vector<int>>(9, vector<int>(muestras, 0)));
    for(int i=0; i<matrixInputs; i++){
        eegMappingMatrix >> pos >> f >> c;
        temporalSeries[f][c] = eegData[pos];
    }
    eegMappingMatrix.close();

	// Construcción del QuadComp para EEG
	TempRasterQuadComp nqcQ(temporalSeries, dcuad);
	TempRasterQuadComp nqcF(temporalSeries, true);
	TempRasterQuadComp nqcC(temporalSeries, false);

	string ofilename(argv[4]);
	nqcQ.save(ofilename+"Q");
	nqcF.save(ofilename+"F");
	nqcC.save(ofilename+"C");

	return 0;
}
