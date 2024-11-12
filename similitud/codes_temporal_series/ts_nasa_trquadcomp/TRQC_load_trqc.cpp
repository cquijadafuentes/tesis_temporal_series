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

	nqc.print();

	return 0;
}