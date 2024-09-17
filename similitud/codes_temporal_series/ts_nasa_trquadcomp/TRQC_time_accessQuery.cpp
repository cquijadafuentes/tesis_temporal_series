#include "TempRaster_QuadComp.hpp"
#include "utils_time.hpp"

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
		cout << argv[0] << " <NQC_file> <access_queries_file> <repetitions>" << endl;
		return 0;
	}
	cout << "********** TRQC_time_accessQuery **********" << endl;
	cout << "Cargando archivo NQC: " << argv[1] << endl;
	string nqcFilename(argv[1]);
	TempRasterQuadComp nqc(nqcFilename);

	cout << "\tfilas: " << nqc.n_rows;
	cout << " - columnas: " << nqc.n_cols;
	cout << " - instantes: " << nqc.n_inst << endl;

	cout << "\td_quad: " << nqc.d_quad;
	cout << " - nQuadRows: " << nqc.nQuadRows;
	cout << " - nQuadCols: " << nqc.nQuadCols << endl;

	cout << "Cargando archivo de consultas: " << argv[2] << endl;
	ifstream qf(argv[2]);
	int n_queries;
	qf >> n_queries;
	vector<vector<int>> queries(n_queries, vector<int>(3));
	for(int q=0; q<n_queries; q++){
		qf >> queries[q][0] >> queries[q][1] >> queries[q][2];
	}
	cout << "\t" << n_queries << " consultas en el archivo." << endl;

	int nreps = stoi(argv[3]);
	cout << "Ejecutando consultas con " << nreps << " repeticiones..." << endl;
	auto t1 = time::user::now(); // Start time
	long total_value = 0;
	for(int r=0; r<nreps; r++){
		total_value = 0;
		for(int q=0; q<n_queries; q++){
			int value = nqc.queryAccess(queries[q][0], queries[q][1], queries[q][2]);
			total_value += value;
		}
	}
	auto t2 = time::user::now(); // End time
	auto time = time::duration_cast<time::milliseconds>(t2-t1);

	cout << "-------------------------------------------" << endl;
	cout << "Time: " << time << " milliseconds. ";
	cout << "Queries: " << nreps * queries.size() << " (" << nreps << "x" << queries.size() << ") ";
	cout << "Total value = " << total_value << ", us/query = " << ((time * 1000.0)/((double)(nreps*queries.size()))) << std::endl;
	cout << "-------------------------------------------" << endl;

	return 0;
}