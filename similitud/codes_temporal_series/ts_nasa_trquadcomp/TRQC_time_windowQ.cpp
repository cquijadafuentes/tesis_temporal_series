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
		cout << argv[0] << " <NQC_file> <windows_queries_file> <repetitions>" << endl;
		return 0;
	}
	cout << "Cargando NQC" << endl;	
	string nqcFilename(argv[1]);
	TempRasterQuadComp nqc(nqcFilename);

	cout << "d_quad: " << nqc.d_quad;
	cout << " - nQuadRows: " << nqc.nQuadRows;
	cout << " - nQuadCols: " << nqc.nQuadCols << endl;

	cout << "Procesando archivo de consultas..." << endl;
	ifstream qf(argv[2]);
	int n_queries;
	qf >> n_queries;
	cout << n_queries << " consultas en el archivo " << argv[3] << endl;

	int reps = stoi(argv[3]);
	cout << "Ejecutando con " << reps << " repeticiones..." << endl;

	int f0, f1, c0, c1, t0, t1;
	for(int q=0; q<n_queries; q++){
		qf >> f0 >> f1 >> c0 >> c1 >> t0 >> t1;
		// Pendiente: time
		vector<vector<vector<int>>> wq = nqc.queryWindow(f0, f1, c0, c1, t0, t1);
	}


	return 0;
}