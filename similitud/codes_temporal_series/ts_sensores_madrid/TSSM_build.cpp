#include "TS_SM.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	if(argc < 6){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <groupsFile> <N-data> <k> <outputFile>" << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	
	TempSeriesSensoresMadrid("Hello");

	return 0;
}
