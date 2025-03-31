#include "TS_SM.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile>" << endl;
		return 0;
	}

	string filename = argv[1];
	TempSeriesSensoresMadrid tssm(filename);
	tssm.stats();

	return 0;
}