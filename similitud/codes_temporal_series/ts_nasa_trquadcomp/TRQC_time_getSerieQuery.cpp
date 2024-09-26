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
		cout << argv[0] << " <NQC_file> <getSerie_queries_file> <repetitions>" << endl;
		cout << "NOTA: Se ejecutan las getSerie queries con archivos generados por el temporal k2-raster para region queries." << endl;
		return 0;
	}
	cout << "********** TRQC_time_getSerieQuery **********" << endl;
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
	int b1;
	vector<vector<int>> queries(n_queries, vector<int>(6));
	for(int q=0; q<n_queries; q++){
		qf >> queries[q][0] >> b1 >> queries[q][1] >> b1 >> b1 >> b1 >> b1 >> b1;
	}
	cout << "\t" << n_queries << " consultas en el archivo." << endl;

	int nreps = stoi(argv[3]);
	cout << "Ejecutando consultas con " << nreps << " repeticiones..." << endl;
	auto t1 = time::user::now(); // Start time
	size_t total_num_cells = 0;
	for(int r=0; r<nreps; r++){
		total_num_cells = 0;
		for(int q=0; q<n_queries; q++){
			size_t n_cells = nqc.getSerie(queries[q][0], queries[q][1]).size();
			total_num_cells += n_cells;
		}
	}
	auto t2 = time::user::now(); // End time
	auto time = time::duration_cast<time::milliseconds>(t2-t1);

	cout << "-------------------------------------------" << endl;
	cout << "Time: " << time << " milliseconds. ";
	cout << "Queries: " << nreps * queries.size() << " (" << nreps << "x" << queries.size() << ") ";
	cout << "Nº cells = " << total_num_cells << ", us/query = " << (((double)time * 1000.0)/(double)(nreps*queries.size()));
	cout << ", us/cell = " << (((double)time * 1000.0))/(double)total_num_cells << std::endl;
	cout << "-------------------------------------------" << endl;

	return 0;
}