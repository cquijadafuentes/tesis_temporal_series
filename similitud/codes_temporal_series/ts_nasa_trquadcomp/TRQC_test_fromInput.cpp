
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
	if(argc < 2){
		cout << "Error! faltan argumentos" << endl;
		cout << argv[0] << " <dimension_cuadrantes>" << endl;
		return 0;
	}
	int dcuad = stoi(argv[1]);
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

	cout << "Fijas: " << endl << "\t";
	for(int i=0; i<nqc.bvSeriesFijas.size(); i++){
		cout << nqc.bvSeriesFijas[i];
	}
	cout << endl << "\t";
	for(int i=0; i<nqc.bvSeriesFijas.size(); i++){
		if(i % 10 == 9){
			cout << "*";
		}else{
			cout << " ";
		}
	}
	cout << endl;

	cout << "bv_Referencias: " << endl << "\t";
	for(int i=0; i<nqc.bvReferencias.size(); i++){
		cout << nqc.bvReferencias[i];
	}
	cout << endl << "\t";
	for(int i=0; i<nqc.bvSeriesFijas.size(); i++){
		if(i % 10 == 9){
			cout << "*";
		}else{
			cout << " ";
		}
	}
	cout << endl;

	cout << "bv_Fijas: " << nqc.fixedValue.size() << ":";
	for(int i=0; i<nqc.fixedValue.size(); i++){
		cout << " " << nqc.fixedValue[i];
	}
	cout << endl;

	cout << "Primer valor: " << nqc.refFirstValue.size() << ":";
	for(int i=0; i<nqc.refFirstValue.size(); i++){
		cout << " " << nqc.refFirstValue[i];
	}
	cout << endl;

	cout << "Referencias: " << nqc.refs.size() << endl;
	for(int i=0; i<nqc.refs.size(); i++){
		cout << "\t" << nqc.refs[i].size() << ":";
		for(int j=0; j<nqc.refs[i].size(); j++){
			cout << " " << nqc.refs[i][j];
		}
		cout << endl;
	}

	cout << "Series: " << nqc.series.size() << endl;
	for(int i=0; i<nqc.series.size(); i++){
		cout << "\t" << nqc.series[i].size() << ":";
		for(int j=0; j<nqc.series[i].size(); j++){
			cout << " " << nqc.series[i][j];
		}
		cout << endl;
	}

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