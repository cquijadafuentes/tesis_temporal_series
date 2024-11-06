
#include "TempRaster_QuadComp.hpp"
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace sdsl;

string mostrarserie(vector<int> v){
	string concat = "";
	if(v.size() > 0){
		concat += to_string(v[0]);
		for(int i=1; i<10; i++){
			concat += " " + to_string(v[i]);
		}
		concat += " ...";
		for(int i=v.size()-11; i<v.size(); i++){
			concat += " " + to_string(v[i]);
		}
	}
	return concat;
}

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <BIN_File> <mapping_sensor_matrix> <NQC_file> [<num_rand_series_to_query>]" << endl;
		return 0;
	}

	int nrstq = 0;
	if(argc > 4){
		nrstq = stoi(argv[4]);
	}
	// Leyendo datos desde el archivo de entrada BIN
	string ifilename(argv[1]);
	ifstream infile(ifilename, ofstream::binary);
	if(infile.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}
	int electrodos, muestras;
	// Cargando datos
	infile.read((char *)&electrodos, sizeof(int));
	infile.read((char *)&muestras, sizeof(int));

	cout << "Archivo: " << argv[1] << endl;
	cout << "Electrodos: " << electrodos << " - Muestras: " << muestras << endl;
	
	vector<vector<int>> eegData(electrodos, vector<int>(muestras));
	for(int i=0; i<electrodos; i++){
		for(int k=0; k<muestras; k++){
			infile.read((char *)&eegData[i][k], sizeof(int));
		}
	}

	cout << "Leyendo archivo de matriz." << endl;
	//	Lectura de matriz con el mapeo de los ids a las celdas de la matriz
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

	cout << "Cargando NQC" << endl;	
	string nqcFilename(argv[3]);
	TempRasterQuadComp nqc(nqcFilename);

	cout << "d_quad: " << nqc.d_quad;
	cout << " - nQuadRows: " << nqc.nQuadRows;
	cout << " - nQuadCols: " << nqc.nQuadCols << endl;

	cout << "Comparando getSerie" << endl;
	if(nrstq > (nqc.d_quad*nqc.d_quad) || argc == 4){
		cout << "Consultando por todas las series de tiempo..." << endl;
		nrstq = nqc.d_quad*nqc.d_quad;
		vector<int> temporalNQC;
		for(int i=0; i<nqc.d_quad; i++){
			for(int j = 0; j < nqc.d_quad; j++){
				temporalNQC = nqc.getSerie(i,j);
				if(temporalNQC.size() < muestras){
					cout << "\tError! La serie [f:" << i << ",c:" << j << "] no coincide en largo." << endl;
					cout << "\tNQC: " << mostrarserie(temporalNQC) << endl;
					cout << "\tBIN: " << mostrarserie(temporalSeries[i][j]) << endl;
				}else{					
					for(int k=0; k<muestras; k++){
						if(temporalNQC[k] != temporalSeries[i][j][k]){
							cout << "\tFalla en valor de la serie [f:" << i << ",c:" << j << ",t:" << k << "]" << endl;
							cout << "\tBIN: " << mostrarserie(temporalSeries[i][j]) << endl;
							cout << "\tNQC: " << mostrarserie(temporalNQC) << endl;
							return 0;
						}
					}
				}
			}
		}
	}else{
		srand(time(NULL));
		cout << "Consultando por " << nrstq << " series de tiempo al azar." << endl;
		int tr, tc;
		vector<int> temporalNQC;
		for(int i=0; i<nrstq; i++){
			tr = rand() % nqc.n_rows;
			tc = rand() % nqc.n_cols;
			cout << "Consulta " << (i+1) << " serie de tiempo [f:" << tr << ",c:" << tc << "]" << endl;
			temporalNQC = nqc.getSerie(tr,tc);
			if(temporalNQC.size() < muestras){
				cout << "\tError! La serie [f:" << tr << ",c:" << tc << "] no coincide en largo." << endl;
				cout << "\tNQC: " << mostrarserie(temporalNQC) << endl;
				cout << "\tBIN: " << mostrarserie(temporalSeries[tr][tc]) << endl;
			}
			for(int k=0; k<muestras; k++){
				if(temporalNQC[k] != temporalSeries[tr][tc][k]){
					cout << "\tFalla en valor de la serie [f:" << tr << ",c:" << tc << ",t:" << k << "]" << endl;
					cout << "\tBIN: " << mostrarserie(temporalSeries[tr][tc]) << endl;
					cout << "\tNQC: " << mostrarserie(temporalNQC) << endl;
				}
			}
		}
	}

	return 0;
}