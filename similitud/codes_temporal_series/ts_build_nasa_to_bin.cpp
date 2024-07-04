#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <outputFile>" << endl;
		return 0;
	}
//    cout << "DATASET: " << argv[1] << endl;
	int rows, cols, a1, a2, a3, a4;
    // Leyendo datos desde el archivo de entrada
    ifstream txtInput(argv[1], ifstream::in);
    txtInput >> rows >> cols >> a1 >> a2 >> a3 >> a4;
    string fileName;
    vector<vector<vector<int>>> temporalSeries(rows, vector<vector<int>>(cols));
    int lenTempSerie = 0;
//    cout << rows << " " << cols << endl;
//    cout << "Leyendo series temporales..." << endl;
    while(txtInput >> fileName){
        // Abrir el archivo binario en modo binario utilizando ifstream
        ifstream archivo(fileName, ios::binary);
        if(archivo.is_open()) {
            // Leer enteros de 32 bits del archivo
            int entero;
            for(int c = 0; c < cols; c++) {
                for(int f = 0; f < rows; f++){
                    archivo.read(reinterpret_cast<char*>(&entero), sizeof(entero));
                    temporalSeries[f][c].push_back(entero);
                }
            }
            archivo.close();
        } else {
            cerr << "Error al abrir el archivo: " << fileName << endl;
        }
        lenTempSerie++;
    }
    txtInput.close();

    ofstream outfile(argv[2], ofstream::binary);
    // Guardando valores enteros
    outfile.write((char const*)&rows, sizeof(int));
    outfile.write((char const*)&cols, sizeof(int));
    outfile.write((char const*)&lenTempSerie, sizeof(int));
    for(int i=0; i<rows; i++){
    	for(int j=0; j<cols; j++){
    		for(int k=0; k<lenTempSerie; k++){
    			outfile.write((char const*)&temporalSeries[i][j][k], sizeof(int));
    		}
    	}        
    }



    cout << "Rows: " << rows << endl;
    cout << "Cols: " << cols << endl;
    cout << "Largo de series de tiempo: " << lenTempSerie << endl;
    cout << "Pimera serie de tiempo: " << endl;
    for(int i=0; i<lenTempSerie; i++){
        cout << "\t" << temporalSeries[0][0][i];
    }
    cout << endl;
    cout << "Última serie de tiempo: " << endl;
    for(int i=0; i<lenTempSerie; i++){
        cout << "\t" << temporalSeries[rows-1][cols-1][i];
    }
    cout << "end" << endl;

}
