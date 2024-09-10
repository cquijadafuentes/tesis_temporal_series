
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

	cout << "Comparando" << endl;
	vector<int> temporalNQC;
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			temporalNQC = nqc.getSerie(i,j);
			if(temporalNQC.size() < lenTempSerie){
				cout << "Error! La serie recuperada no coincide en largo." << endl;
				cout << "NQC: " << mostrarserie(temporalNQC) << endl;
				cout << "BIN: " << mostrarserie(temporalSeries[i][j]) << endl;
				return 0;
			}
			for(int k=0; k<lenTempSerie; k++){
				if(temporalNQC[k] != temporalSeries[i][j][k]){
					cout << "Falla en celda [" << i << "," << j << "]" << endl;
					cout << "BIN: " << mostrarserie(temporalSeries[i][j]) << endl;
					cout << "NQC: " << mostrarserie(temporalNQC) << endl;
					return 0;
				}
			}
		}
	}
	return 0;
}