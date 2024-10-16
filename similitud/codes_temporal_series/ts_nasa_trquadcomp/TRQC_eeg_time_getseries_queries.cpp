#include "TempRaster_QuadComp.hpp"
#include "utils_time.hpp"
#include <algorithm>    // std::random_shuffle

using namespace std;


int myrandom (int i) { return rand()%i;}

vector<int> query_getserie_bin(vector<vector<int>> &eegData, int x){
	vector<int> r = eegData[x];
	return r;
}

int main(int argc, char const *argv[]){
	if(argc < 5){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <NQC_file> <BIN_File> <mapping_sensor_matrix> <number_of_queries>" << endl;
		return 0;
	}

	cout << "********** TRQC_time_getSerieQuery_EEG **********" << endl;
	cout << "Cargando archivo NQC: " << argv[1] << endl;
	string nqcFilename(argv[1]);
	TempRasterQuadComp nqc(nqcFilename);

	// Leyendo datos desde el archivo BINARIO
	cout << "Cargando archivo BIN: " << argv[1] << endl;
	string ifilename(argv[2]);
	ifstream infile(ifilename, ofstream::binary);
	if(infile.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}
	int electrodos, muestras;
	// Cargando datos
	infile.read((char *)&electrodos, sizeof(int));
	infile.read((char *)&muestras, sizeof(int));
	cout << "File: " << argv[2] << endl;
	cout << "Electrodos: " << electrodos << " - Muestras: " << muestras << endl;	
	vector<vector<int>> eegData(electrodos, vector<int>(muestras));
	for(int i=0; i<electrodos; i++){
		for(int k=0; k<muestras; k++){
			infile.read((char *)&eegData[i][k], sizeof(int));
		}
	}

	//	Lectura de matriz con el mapeo de los ids a las celdas de la matriz
	//	Este archivo se utiliza como las consultas para la función getserie
	ifstream eegMappingMatrix(argv[3], ifstream::in);
	if(eegMappingMatrix.fail()){
		cout << "Error! Lectura de " << argv[2] << " fallida." << endl;
		return -1;
	}
	int n_queries;
	eegMappingMatrix >> n_queries;
	cout << "n_queries: " << n_queries << endl;
	int pos, f, c;
	vector<int> queries(n_queries, 0);
	map<int,pair<int,int>> mapQueries;
	for(int i=0; i<n_queries; i++){
		eegMappingMatrix >> pos >> f >> c;
		queries[i] = pos;
		mapQueries[pos] = make_pair(f,c);
	}
	eegMappingMatrix.close();
	random_shuffle(queries.begin(), queries.end());
	vector<int> queriesNQx(n_queries,0);
	vector<int> queriesNQy(n_queries,0);
	for(int i=0; i<n_queries; i++){
		queriesNQx[i] = mapQueries[queries[i]].first;
		queriesNQy[i] = mapQueries[queries[i]].second;
	}

	int nreps = stoi(argv[4]);
	cout << "Ejecutando consultas en BIN con " << nreps << " repeticiones..." << endl;
	auto t1 = time::user::now(); // Start time
	for(int r=0; r<nreps; r++){
		for(int q=0; q<n_queries; q++){
			query_getserie_bin(eegData, queries[q]);
		}
	}
	auto t2 = time::user::now(); // End time
	auto timeBIN = time::duration_cast<time::milliseconds>(t2-t1);

	cout << "-------------------------------------------" << endl;
	cout << "Time BIN: " << timeBIN << " milliseconds. " << endl;
	cout << "-------------------------------------------" << endl;


	cout << "Ejecutando consultas en NQC con " << nreps << " repeticiones..." << endl;
	t1 = time::user::now(); // Start time
	for(int r=0; r<nreps; r++){
		for(int q=0; q<n_queries; q++){
			nqc.getSerie(queriesNQx[q], queriesNQy[q]);
		}
	}
	t2 = time::user::now(); // End time
	auto timeNQC = time::duration_cast<time::milliseconds>(t2-t1);

	cout << "-------------------------------------------" << endl;
	cout << "Time NQC: " << timeNQC << " milliseconds. " << endl;
	cout << "-------------------------------------------" << endl;

	return 0;
}
