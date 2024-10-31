#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <N_serie>" << endl;
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
	
	int ps = stoi(argv[2]);

	cout << "Archivo: " << argv[0] << endl;
	cout << "Sensor: " << ps << endl;

	vector<int> tserie(muestras, 0);
	for(int i=0; i<electrodos; i++){
		for(int j=0; j<muestras; j++){
			infile.read((char *)&tserie[j], sizeof(float));
		}
		if(i == ps){
			for(int j=0; j<muestras; j++){
				cout << tserie[j] << " - ";
			}
			cout << endl;
			cout << "Archivo: " << argv[0] << endl;
			cout << "Sensor: " << ps << endl;
			return 0;
		}
	}
}
