
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
	if(argc < 4){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <d_cuadrante> <outputFile>" << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	string ifilename(argv[1]);
	ifstream infile(ifilename, ofstream::binary);
	if(infile.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}
	int dcuad = stoi(argv[2]);
	int rows, cols, lenTempSerie;
	// Cargando datos
	infile.read((char *)&rows, sizeof(int));
	infile.read((char *)&cols, sizeof(int));
	infile.read((char *)&lenTempSerie, sizeof(int));

	cout << "File: " << argv[1] << endl;
	cout << "Filas: " << rows << " - Cols: " << cols << " - Muestras: " << lenTempSerie << endl;
	cout << "Total celdas del raster: " << rows * cols << endl;

	vector<vector<vector<int>>> temporalSeries(rows, vector<vector<int>>(cols, vector<int>(lenTempSerie)));
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			for(int k=0; k<lenTempSerie; k++){
				infile.read((char *)&temporalSeries[i][j][k], sizeof(int));
			}
		}
	}
	TempRasterQuadComp nqc(temporalSeries, dcuad);

	string ofilename(argv[3]);
	nqc.save(ofilename);

	TempRasterQuadComp nqc2(ofilename);

	cout << "Comparando..." << endl;
	vector<int> difs;
	int totalD;

	if(nqc.fixedValue.size() != nqc2.fixedValue.size()){
		cout << "Diferencia en nqc.fixedValue.size()" << endl;
	}
	difs = compNumbIntV(nqc.fixedValue, nqc2.fixedValue);
	if(difs.size()>0){
		cout << "Se encontraron " << difs.size() << " diferencias en fixedValue: " << endl;
		for(int i=0; i<difs.size(); i++){
			cout << "\t" << difs[i];
		}
		cout << endl;
	}

	if(nqc.refFirstValue.size() != nqc2.refFirstValue.size()){
		cout << "Diferencia en nqc.refFirstValue.size()" << endl;
	}
	difs = compNumbIntV(nqc.refFirstValue, nqc2.refFirstValue);
	if(difs.size()>0){
		cout << "Se encontraron " << difs.size() << " diferencias en refFirstValue: " << endl;
		for(int i=0; i<difs.size(); i++){
			cout << "\t" << difs[i];
		}
		cout << endl;
	}

	if(nqc.refs.size() != nqc2.refs.size()){
		cout << "Diferencia en nqc.refs.size()" << endl;
	}
	totalD = 0;
	for(int i=0; i<nqc.refs.size(); i++){
		if(nqc.refs[i].size() != nqc2.refs[i].size()){
			cout << "Diferencia en nqc.refs[i].size()" << endl;
			cout << "nqc.refs[i].size(): " << nqc.refs[i].size() << endl;
			cout << "nqc2.refs[i].size(): " << nqc2.refs[i].size() << endl;
			cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
			for(int j=0; j<nqc.refs[i].size(); j++){
				cout << nqc.refs[i][j] << " ";
			}
			cout << endl << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
			for(int j=0; j<nqc2.refs[i].size(); j++){
				cout << nqc2.refs[i][j] << " ";
			}
			cout << endl;
			return 0;
		}
		difs = compNumbIntV(nqc.refs[i], nqc2.refs[i]);
		if(difs.size() > 0){
			totalD += difs.size();
			cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
			for(int j=0; j<nqc.refs[i].size(); j++){
				cout << nqc.refs[i][j] << " ";
			}
			cout << endl << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
			for(int j=0; j<nqc2.refs[i].size(); j++){
				cout << nqc2.refs[i][j] << " ";
			}
			cout << endl << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
			for(int j=0; j<nqc2.refs[i].size(); j++){
				if(nqc.refs[i][j] != nqc2.refs[i][j]){
					cout << "Diferencia en pos " << j << " valores " << nqc.refs[i][j] << " vs " << nqc2.refs[i][j] << endl;
				}
			}
			cout << endl;
			return 0;
		}
	}
	if(totalD>0){
		cout << "Se encontraron " << totalD << " diferencias en refs: " << endl;
	}

	if(nqc.series.size() != nqc2.series.size()){
		cout << "Diferencia en nqc.series.size()" << endl;
	}
	totalD = 0;
	for(int i=0; i<nqc.series.size(); i++){
		if(nqc.series[i].size() != nqc2.series[i].size()){
			cout << "Diferencia en nqc.series[i].size()" << endl;
			cout << "nqc.series[i].size(): " << nqc.series[i].size() << endl;
			cout << "nqc2.series[i].size(): " << nqc2.series[i].size() << endl;
			cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
			for(int j=0; j<nqc.series[i].size(); j++){
				cout << nqc.series[i][j] << " ";
			}
			cout << endl << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
			for(int j=0; j<nqc2.series[i].size(); j++){
				cout << nqc2.series[i][j] << " ";
			}
			cout << endl;
			return 0;
		}
		difs = compNumbIntV(nqc.series[i], nqc2.series[i]);
		if(difs.size() > 0){
			totalD += difs.size();
			cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
			for(int j=0; j<nqc.series[i].size(); j++){
				cout << nqc.series[i][j] << " ";
			}
			cout << endl << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
			for(int j=0; j<nqc2.series[i].size(); j++){
				cout << nqc2.series[i][j] << " ";
			}
			cout << endl << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
			for(int j=0; j<nqc2.series[i].size(); j++){
				if(nqc.series[i][j] != nqc2.series[i][j]){
					cout << "Diferencia en pos " << j << " valores " << nqc.series[i][j] << " vs " << nqc2.series[i][j] << endl;
				}
			}
			cout << endl;
			return 0;
		}
	}
	if(totalD > 0){
		cout << "Se encontraron " << totalD << " diferencias en series: " << endl;
	}

	if(nqc.bvQuadNoFijos.size() != nqc2.bvQuadNoFijos.size()){
		cout << "Diferencia en nqc.bvQuadNoFijos.size()" << endl;
	}
	difs = compNumbSDV(nqc.bvQuadNoFijos, nqc2.bvQuadNoFijos);
	if(difs.size()>0){
		cout << "Se encontraron " << difs.size() << " diferencias en bvQuadNoFijos: " << endl;
		for(int i=0; i<difs.size(); i++){
			cout << "\t" << difs[i];
		}
		cout << endl;
	}

	if(nqc.bvReferencias.size() != nqc2.bvReferencias.size()){
		cout << "Diferencia en nqc.bvReferencias.size()" << endl;
	}
	difs = compNumbSDV(nqc.bvReferencias, nqc2.bvReferencias);
	if(difs.size()>0){
		cout << "Se encontraron " << difs.size() << " diferencias en bvReferencias: " << endl;
		for(int i=0; i<difs.size(); i++){
			cout << "\t" << difs[i];
		}
		cout << endl;
	}

	if(nqc.bvSeriesFijas.size() != nqc2.bvSeriesFijas.size()){
		cout << "Diferencia en nqc.bvSeriesFijas.size()" << endl;
	}
	difs = compNumbSDV(nqc.bvSeriesFijas, nqc2.bvSeriesFijas);
	if(difs.size()>0){
		cout << "Se encontraron " << difs.size() << " diferencias en bvSeriesFijas: " << endl;
		for(int i=0; i<difs.size(); i++){
			cout << "\t" << difs[i];
		}
		cout << endl;
	}

	cout << "FIN" << endl;

	return 0;
}
