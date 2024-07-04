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
    int electrodos, muestras;
    // Guardando valores enteros
    infile.read((char *)&electrodos, sizeof(int));
    infile.read((char *)&muestras, sizeof(int));

    cout << "Electrodos: " << electrodos << endl;
    cout << "Muestras: " << muestras << endl;

    vector<vector<int>> temporalSeries(electrodos, vector<int>(muestras));
    for(int i=0; i<electrodos; i++){
    	for(int j=0; j<muestras; j++){
    		infile.read((char *)&temporalSeries[i][j], sizeof(int));
    	}        
    }
    cout << "100 primeros valores del primer electrodo: " << endl;
    for(int i=0; i<100; i++){
        cout << "\t" << temporalSeries[0][i];
    }
    cout << endl;
    cout << "100 primeros valores del último electrodo: " << endl;
    for(int i=0; i<100; i++){
        cout << "\t" << temporalSeries[electrodos-1][i];
    }
    cout << endl << "end" << endl;
}
