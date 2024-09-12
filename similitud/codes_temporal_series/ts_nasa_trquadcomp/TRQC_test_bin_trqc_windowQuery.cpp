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
	if(argc < 4){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <BIN_File> <NQC_file> <windows_queries_file>" << endl;
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

	cout << "Procesando archivo de consultas..." << endl;
	ifstream qf(argv[3]);
	int n_queries;
	qf >> n_queries;
	cout << n_queries << " consultas en el archivo " << argv[3] << endl;
	int f0, f1, c0, c1, t0, t1;
	
	for(int q=0; q<n_queries; q++){
		qf >> f0 >> f1 >> c0 >> c1 >> t0 >> t1;
		cout << "Window Query: F [" << f0 << "," << f1 << "] - C [" << c0 << "," << c1 << "] - T [" << t0 << "," << t1 << "]:" << endl;
		vector<vector<vector<int>>> wq = nqc.queryWindow(f0, f1, c0, c1, t0, t1);
		bool coinciden = true;
		for(int i=0; i<wq.size(); i++){
			for(int j=0; j<wq[0].size(); j++){
				for(int k=0; k<wq[0][0].size(); k++){
					if(wq[i][j][k] != temporalSeries[i+f0][j+c0][k+t0]){
						coinciden = false;
					}
				}
			}
		}
		if(!coinciden){
			cout << "Error en el resultado!" << endl;
			for(int i=0; i<wq.size(); i++){
				for(int j=0; j<wq[0].size(); j++){
					for(int k=0; k<wq[0][0].size(); k++){
						cout << " " << wq[i][j][k];
					}
					cout << "\t+\t";
				}
				cout << endl;
			}
		}else{
			cout << "\tOK" << endl;
		}
	}


	return 0;
}