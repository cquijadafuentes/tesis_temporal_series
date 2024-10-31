#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <fila> <columna>" << endl;
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

	int pf = stoi(argv[2]);
	int pc = stoi(argv[3]);

	cout << "Archivo: " << argv[0] << endl;
	cout << "Fila: " << pf << " - Columna: " << pc << endl; 

	vector<int> tserie(lenTempSerie, 0);
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			for(int k=0; k<lenTempSerie; k++){
				infile.read((char *)&tserie[k], sizeof(int));
			}
			if( i==pf && j == pc ){
				for(int k=0; k<lenTempSerie; k++){
					cout << tserie[k] << " ";
				}
				cout << endl;
				cout << "Archivo: " << argv[0] << endl;
				cout << "Fila: " << pf << " - Columna: " << pc << endl; 
				return 0;
			}
		}        
	}
	
}
