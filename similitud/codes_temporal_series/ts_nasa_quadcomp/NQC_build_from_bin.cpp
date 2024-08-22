
#include "NASA_QuadComp.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <d_cuadrante>" << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	ifstream infile(argv[1], ofstream::binary);
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

	vector<vector<vector<int>>> temporalSeries(rows, vector<vector<int>>(cols, vector<int>(lenTempSerie)));
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			for(int k=0; k<lenTempSerie; k++){
				infile.read((char *)&temporalSeries[i][j][k], sizeof(int));
			}
		}
	}

	NASAQuadComp nqc(temporalSeries, dcuad);

	return 0;
}
