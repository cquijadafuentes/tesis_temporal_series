#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile>" << endl;
		return 0;
	}
    // Leyendo datos desde el archivo de entrada
    ifstream infile(argv[1], ofstream::binary);
    if(infile.fail()){
        cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
        return -1;
    }
    int rows, cols, lenTempSerie;
    // Cargando datos
    infile.read((char *)&rows, sizeof(int));
    infile.read((char *)&cols, sizeof(int));
    infile.read((char *)&lenTempSerie, sizeof(int));

    cout << "Rows: " << rows << endl;
    cout << "Cols: " << cols << endl;
    cout << "Largo de series de tiempo: " << lenTempSerie << endl;

    vector<vector<vector<int>>> temporalSeries(rows, vector<vector<int>>(cols, vector<int>(lenTempSerie)));
    for(int i=0; i<rows; i++){
    	for(int j=0; j<cols; j++){
    		for(int k=0; k<lenTempSerie; k++){
                infile.read((char *)&temporalSeries[i][j][k], sizeof(int));
            }
    	}        
    }
    cout << "Pimera serie de tiempo: " << endl;
    for(int i=0; i<lenTempSerie; i++){
        cout << "\t" << temporalSeries[0][0][i];
    }
    cout << endl;
    cout << "Última serie de tiempo: " << endl;
    for(int i=0; i<lenTempSerie; i++){
        cout << "\t" << temporalSeries[rows-1][cols-1][i];
    }
    cout << endl << "end" << endl;
}
