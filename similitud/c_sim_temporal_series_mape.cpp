#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

double calc_mape(vector<int> actual, vector<int> pred){
    double acum = 0.0;
    for(int i=0; i<actual.size(); i++){
        double temp = (actual[i] - pred[i]) / (actual[i] + 0.000001);
        acum += temp;
    }
    return (acum / actual.size());
}

double dist_euclidea(int a1, int b1, int a2, int b2){
    int a = a1 - a2;
    int b = b1 - b2;
    long long c = (a*a) + (b*b);
    return sqrt(c);
}

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <filename>" << endl;
		return 0;
	}
    cout << "DATASET: " << argv[1] << endl;
	int rows, cols, a1, a2, a3, a4;
    // Leyendo datos desde el archivo de entrada
	ifstream txtInput(argv[1], ifstream::in);
	txtInput >> rows >> cols >> a1 >> a2 >> a3 >> a4;
	string fileName;
	vector<vector<vector<int>>> temporalSeries(rows, vector<vector<int>>(cols));
    int lenTempSerie = 0;
    cout << rows << " " << cols << endl;
    cout << "Leyendo series temporales..." << endl;
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

    cout << lenTempSerie << endl;
    cout << "Calculando similitud MAPE" << endl;

    // Procesando datos desde el archivo de entrada
    int cantComb = (rows*cols)*(rows*cols);
    double distancia, simMAPE;
    cout << "#\tdistancia\tsim_mape" << endl;
    for(int f=0; f<rows; f+=9){
        for(int c=0; c<cols; c+=5){
            for(int f1=0; f1<rows; f1+=9){
                for(int c1=0; c1<cols; c1+=5){
                    distancia = dist_euclidea(f, c, f1, c1);
                    simMAPE = calc_mape(temporalSeries[f][c], temporalSeries[f1][c1]);
                    cout << distancia << "\t" << simMAPE << endl;
                }
            }
        }
    }

    return 0;
}
