
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
	if(argc < 8){
		cout << "Error! faltan argumentos" << endl;
		cout << argv[0] << " <dimension_cuadrantes> <f0> <f1> <c0> <c1> <t0> <t1>" << endl;
		return 0;
	}
	int dcuad = stoi(argv[1]);
	int f0 = stoi(argv[2]);
	int f1 = stoi(argv[3]);
	int c0 = stoi(argv[4]);
	int c1 = stoi(argv[5]);
	int t0 = stoi(argv[6]);
	int t1 = stoi(argv[7]);

	int rows, cols, lenTempSerie;
	cout << "Leyendo datos..." << endl;
	cin >> rows >> cols >> lenTempSerie;
	vector<vector<vector<int>>> temporalSeries(rows, vector<vector<int>>(cols, vector<int>(lenTempSerie)));
	for(int k=0; k<lenTempSerie; k++){
		for(int i=0; i<rows; i++){
			for(int j=0; j<cols; j++){
				cin >> temporalSeries[i][j][k];
			}
		}
	}

	cout << "Filas: " << rows << " - Cols: " << cols << " - Muestras: " << lenTempSerie << endl;
	cout << "Total celdas del raster: " << rows * cols << endl;

	TempRasterQuadComp nqc(temporalSeries, dcuad);

	cout << "d_quad: " << nqc.d_quad;
	cout << " - nQuadRows: " << nqc.nQuadRows;
	cout << " - nQuadCols: " << nqc.nQuadCols << endl;

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			cout << "[";
			for(int k=0; k<lenTempSerie; k++){
				cout << " " << temporalSeries[i][j][k];
			}
			cout << " ] - ";
		}
		cout << endl;
	}

	cout << "Window Query: F [" << f0 << "," << f1 << "] - C [" << c0 << "," << c1 << "] - T [" << t0 << "," << t1 << "]:" << endl;

	vector<vector<vector<int>>> wq = nqc.queryWindow(f0, f1, c0, c1, t0, t1);
	for(int i=0; i<wq.size(); i++){
		cout << "+  ";
		for(int j=0; j<wq[0].size(); j++){
			for(int k=0; k<wq[0][0].size(); k++){
				cout << " " << wq[i][j][k];
			}
			cout << "  +  ";
		}
		cout << endl;
	}

	return 0;
}