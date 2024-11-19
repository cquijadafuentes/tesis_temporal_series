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
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <NQC_file>" << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	string nqcFilename(argv[1]);
	TempRasterQuadComp nqc(nqcFilename);
	
	int bytes = 0;
	bytes += size_in_bytes(nqc.fixedValue);
	bytes += size_in_bytes(nqc.refFirstValue);
	for(int i=0; i<nqc.refs.size(); i++){
		bytes += size_in_bytes(nqc.refs[i]);
	}	
	for(int i=0; i<nqc.series.size(); i++){
		bytes += size_in_bytes(nqc.series[i]);
	}

	int kbytes = bytes / 1024 ;
	cout << argv[1] << "\t" << kbytes << "[KB]" << endl;

	return 0;
}