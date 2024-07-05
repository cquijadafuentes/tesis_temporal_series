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
    int sensores, muestras;
    // Leyendo datos
    infile.read((char *)&sensores, sizeof(int));
    infile.read((char *)&muestras, sizeof(int));
    cout << "Sensores: " << sensores << endl;
    cout << "Muestras: " << muestras << endl;

    vector<int> idsensores(sensores);
    for(int i=0; i<sensores; i++){
        infile.read((char *)&idsensores[i], sizeof(int));
    }

    vector<vector<int>> temporalSeries(sensores, vector<int>(muestras));
    for(int i=0; i<sensores; i++){
    	for(int j=0; j<muestras; j++){
            infile.read((char *)&temporalSeries[i][j], sizeof(int));
    	}        
    }
    cout << "Pimera serie de tiempo: " << endl;
    for(int i=0; i<muestras; i++){
        cout << "\t" << temporalSeries[0][i];
    }
    cout << endl;
    cout << "Última serie de tiempo: " << endl;
    for(int i=0; i<muestras; i++){
        cout << "\t" << temporalSeries[sensores-1][i];
    }
    cout << endl << "end" << endl;
}
