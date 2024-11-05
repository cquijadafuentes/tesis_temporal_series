
#include "TempRaster_QuadComp.hpp"

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
		cout << argv[0] << " <BIN_File> <NQC_file>" << endl;
		return 0;
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

	cout << "File: " << argv[1] << endl;
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
	TempRasterQuadComp nqc(nqcFilename);

	cout << "d_quad: " << nqc.d_quad;
	cout << " - nQuadRows: " << nqc.nQuadRows;
	cout << " - nQuadCols: " << nqc.nQuadCols << endl;

	cout << "Comparando getSerie" << endl;
	vector<int> temporalNQC;
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			temporalNQC = nqc.getSerie(i,j);
			if(temporalNQC.size() < lenTempSerie){
				cout << "\tError! La serie recuperada no coincide en largo." << endl;
				cout << "\tNQC: " << mostrarserie(temporalNQC) << endl;
				cout << "\tBIN: " << mostrarserie(temporalSeries[i][j]) << endl;
				return 0;
			}
			for(int k=0; k<lenTempSerie; k++){
				if(temporalNQC[k] != temporalSeries[i][j][k]){
					cout << "\tFalla en celda [" << i << "," << j << "]" << endl;
					cout << "\tBIN: " << mostrarserie(temporalSeries[i][j]) << endl;
					cout << "\tNQC: " << mostrarserie(temporalNQC) << endl;
					return 0;
				}
			}
		}
	}

	cout << "Comparando queryAccess" << endl;
	int aux;
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			for(int k=0; k<lenTempSerie; k++){
				aux = nqc.queryAccess(i, j, k);
				if(aux != temporalSeries[i][j][k]){
					cout << "\tFalla en celda [" << i << "," << j << "]" << endl;
					cout << "\tData: " << temporalSeries[i][j][k] << endl;
					cout << "\tQuery: " << nqc.queryAccess(i, j, k) << endl;
					return 0;
				}
			}
		}
	}

	int f0 = 2;
	int f1 = 3;
	int c0 = 2;
	int c1 = 5;
	int t0 = 1;
	int t1 = 6;

	vector<vector<vector<int>>> wq = nqc.queryWindow(3, 2, 5, 2, 6, 1);
	for(int i=0; i<wq.size(); i++){
		for(int j=0; j<wq[0].size(); j++){
			for(int k=0; k<wq[0][0].size(); k++){
				cout << " " << wq[i][j][k];
			}
			cout << "\t+\t";
		}
		cout << endl;
	}

	return 0;
}