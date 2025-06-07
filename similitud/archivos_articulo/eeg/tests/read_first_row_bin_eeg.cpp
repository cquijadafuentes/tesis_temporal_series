#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> [<n_elements>]" << endl;
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

	int n = muestras;
	if(argc > 2){
		n = stoi(argv[2]);
	}	
	
	int aux;
	for(int k=0; k<n; k++){
		infile.read((char *)&aux, sizeof(int));
		cout << aux << " ";
	}
	cout << endl;

	
}