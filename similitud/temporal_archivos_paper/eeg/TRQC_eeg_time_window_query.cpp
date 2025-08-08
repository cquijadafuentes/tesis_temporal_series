#include "TempRaster_QuadComp.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <mapping_sensor_matrix> <quadcompfileq>" << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	string ifilename(argv[1]);
	ifstream infile(ifilename, ofstream::binary);
	if(infile.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}
	int electrodos, muestras;
	// Cargando datos
	infile.read((char *)&electrodos, sizeof(int));
	infile.read((char *)&muestras, sizeof(int));

	cout << "File: " << argv[1] << endl;
	cout << "Electrodos: " << electrodos << " - Muestras: " << muestras << endl;
	
	vector<vector<int>> eegData(electrodos, vector<int>(muestras));
	for(int i=0; i<electrodos; i++){
		for(int k=0; k<muestras; k++){
			infile.read((char *)&eegData[i][k], sizeof(int));
		}
	}

	//	Lectura de matriz con el mapeo de los ids a las celdas de la matriz
    //	y construcción de la matriz en base al mapeo y los datos del eeg
	ifstream eegMappingMatrix(argv[2], ifstream::in);
    if(eegMappingMatrix.fail()){
        cout << "Error! Lectura de " << argv[2] << " fallida." << endl;
        return -1;
    }
    int matrixInputs;
    eegMappingMatrix >> matrixInputs;
    int pos, f, c;
    vector<vector<vector<int>>> temporalSeries(9, vector<vector<int>>(9, vector<int>(muestras, 0)));
    for(int i=0; i<matrixInputs; i++){
        eegMappingMatrix >> pos >> f >> c;
        temporalSeries[f][c] = eegData[pos];
    }
    eegMappingMatrix.close();

	// Lectura del QuadComp para EEG
	cout << "Cargando quadcompfile de EEG" << endl;
	TempRasterQuadComp nqcQ(argv[3]);
	cout << "File: " << argv[3] << endl;
	cout << "n_rows: " << nqcQ.n_rows << endl;
	cout << "n_cols: " << nqcQ.n_cols << endl;
	cout << "n_inst: " << nqcQ.n_inst << endl;
	cout << "d_quad: " << nqcQ.d_quad << endl;
	cout << "cel_per_quad: " << nqcQ.cel_per_quad << endl;
	cout << "nQuadRows: " << nqcQ.nQuadRows << endl;
	cout << "nQuadCols: " << nqcQ.nQuadCols << endl;
	cout << "min_value: " << nqcQ.min_value << endl;
	cout << "max_value: " << nqcQ.max_value << endl;
	

	// Consultas
	int ti = 0;
	int tf = 10;
	for(int i=0; i<temporalSeries.size(); i++){
		for(int j=0; j<temporalSeries[i].size(); j++){
			vector<vector<vector<int>>> resWinQuery = nqcQ.queryWindow(i, i, j, j, ti, tf);
			cout << "------ Consulta [" << i << "][" << j << "] ------" << endl;
			cout << "Resultado obtenido:";
			for(int ii=0; ii<resWinQuery.size(); ii++){
				for(int jj=0; jj<resWinQuery[ii].size(); jj++){
					for(int k=0; k<resWinQuery[ii][jj].size(); k++){
						cout << " " << resWinQuery[ii][jj][k];
					}
					cout << endl;
				}
			}
			cout << "Resultado esperado:";
			for(int k=ti; k<=tf; k++){
				cout << " " << temporalSeries[i][j][k];
			}
			cout << endl << "-------------" << endl;
		}
	}

	return 0;
}
