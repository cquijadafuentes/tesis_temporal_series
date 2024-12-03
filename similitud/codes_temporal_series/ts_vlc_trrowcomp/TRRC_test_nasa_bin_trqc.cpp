
#include "TempRaster_RowComp.hpp"
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace sdsl;

string mostrarserie(vector<int> v){
	string concat = "";
	if(v.size() > 0){
		concat += to_string(v[0]);
		for(int i=1; i<v.size(); i++){
			concat += " " + to_string(v[i]);
		}
	}
	return concat;
}

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <BIN_File> <NQC_file> [<num_rand_series_to_query>]" << endl;
		return 0;
	}

	int nrstq = 0;
	if(argc > 3){
		nrstq = stoi(argv[3]);
	}


	// Leyendo datos desde el archivo de entrada
	string binFilename(argv[1]);
	ifstream binfile(binFilename, ofstream::binary);
	if(binfile.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}


	int rows, cols, lenTempSerie;
	// Cargando datos
	binfile.read((char *)&rows, sizeof(int));
	binfile.read((char *)&cols, sizeof(int));
	binfile.read((char *)&lenTempSerie, sizeof(int));

	cout << "Archivo: " << argv[1] << endl;
	cout << "Filas: " << rows << " - Cols: " << cols << " - Muestras: " << lenTempSerie << endl;
	cout << "Total celdas del raster: " << rows * cols << endl;

	cout << "Cargando BIN" << endl;
	vector<vector<vector<int>>> temporalSeries(rows, vector<vector<int>>(cols, vector<int>(lenTempSerie)));
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			for(int k=0; k<lenTempSerie; k++){
				binfile.read((char *)&temporalSeries[i][j][k], sizeof(int));
			}
		}
	}

	cout << "Cargando NQC" << endl;	
	string nqcFilename(argv[2]);
	TempRasterRowComp nqc(nqcFilename);

	cout << "d_quad: " << nqc.d_quad;
	cout << " - nQuadRows: " << nqc.nQuadRows;
	cout << " - nQuadCols: " << nqc.nQuadCols << endl;


	cout << "Comparando getSerie" << endl;
	if(nrstq > (rows*cols) || argc == 3){
		cout << "Consultando por todas las series de tiempo..." << endl;
		nrstq = rows*cols;
		vector<int> temporalNQC;
		for(int i=0; i<rows; i++){
			for(int j=0; j<cols; j++){
				temporalNQC = nqc.getSerie(i,j);
				if(temporalNQC.size() < lenTempSerie){
					cout << "\tError! La serie [f:" << i << ",c:" << j << "] no coincide en largo." << endl;
					cout << "\tNQC: " << mostrarserie(temporalNQC) << endl;
					cout << "\tBIN: " << mostrarserie(temporalSeries[i][j]) << endl;
					return 0;
				}
				for(int k=0; k<lenTempSerie; k++){
					if(temporalNQC[k] != temporalSeries[i][j][k]){
						cout << "\tFalla en valor de la serie [f:" << i << ",c:" << j << ",t:" << k << "]" << endl;
						cout << "\tBIN: " << mostrarserie(temporalSeries[i][j]) << endl;
						cout << "\tNQC: " << mostrarserie(temporalNQC) << endl;
						return 0;
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
			tr = rand() % rows;
			tc = rand() % cols;
			cout << "Consulta " << (i+1) << " serie de tiempo [f:" << tr << ",c:" << tc << "]" << endl;
			temporalNQC = nqc.getSerie(tr,tc);
			if(temporalNQC.size() < lenTempSerie){
				cout << "\tError! La serie [f:" << tr << ",c:" << tc << "] no coincide en largo." << endl;
				cout << "\tNQC: " << mostrarserie(temporalNQC) << endl;
				cout << "\tBIN: " << mostrarserie(temporalSeries[tr][tc]) << endl;
				return 0;
			}
			for(int k=0; k<lenTempSerie; k++){
				if(temporalNQC[k] != temporalSeries[tr][tc][k]){
					cout << "\tFalla en valor de la serie [f:" << tr << ",c:" << tc << ",t:" << k << "]" << endl;
					cout << "\tBIN: " << mostrarserie(temporalSeries[tr][tc]) << endl;
					cout << "\tNQC: " << mostrarserie(temporalNQC) << endl;
					return 0;
				}
			}
		}
	}


	return 0;
}