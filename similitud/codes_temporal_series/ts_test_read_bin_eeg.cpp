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
	int electrodos, muestras;
	// Cargando datos
	infile.read((char *)&electrodos, sizeof(int));
	infile.read((char *)&muestras, sizeof(int));

	cout << "Electrodos: " << electrodos << endl;
	cout << "Muestras: " << muestras << endl;

	vector<vector<float>> temporalSeries(electrodos, vector<float>(muestras));
	for(int i=0; i<electrodos; i++){
		for(int j=0; j<muestras; j++){
			infile.read((char *)&temporalSeries[i][j], sizeof(float));
		}        
	}

	float maxi = temporalSeries[0][0];
	float mini = maxi;
	for(int i=0; i<electrodos; i++){
		for(int j=0; j<muestras; j++){
			if(temporalSeries[i][j] > maxi){
				maxi = temporalSeries[i][j];
			}
			if(temporalSeries[i][j] < mini){
				mini = temporalSeries[i][j];
			}
		}        
	}
	cout << "Valor más grande: " << maxi << endl;
	cout << "Valor más pequeño: " << mini << endl;

	cout << "100 primeros valores del primer electrodo: " << endl;
	for(int i=0; i<100; i++){
		cout << "\t" << temporalSeries[0][i];
	}
	cout << endl;
	cout << "100 últimos valores del último electrodo: " << endl;
	for(int i=muestras-101; i<muestras; i++){
		cout << "\t" << temporalSeries[electrodos-1][i];
	}
	cout << endl << "end" << endl;
}
