
#include "TempRaster_QuadComp.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Error! faltan argumentos" << endl;
		cout << argv[0] << " <file.qc> <fila> <columna>" << endl;
		return 0;
	}
	
	int f = stoi(argv[2]);
	int c = stoi(argv[3]);

	TempRasterQuadComp nqc(argv[1]);

	cout << "Archivo: " << argv[1] << endl;
	cout << "Fila: " << f << " - Columna: " << c << endl;

	vector<int> gs = nqc.getSerie(f, c);
	for(int i=0; i<gs.size(); i++){
		cout << gs[0] << " - ";
	}
	cout << endl;

	return 0;
}