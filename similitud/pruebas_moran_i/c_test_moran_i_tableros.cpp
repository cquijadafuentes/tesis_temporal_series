#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>

/*
    Programa para calcular la I de Moran en varios formatos como los de
    un tablero de ajedrez

    La estadística de Moran se calculará en base a la fórmula que indica:
    I = (Factor_1) * (Factor_2)
    donde
    Factor_1 = (Cantidad de elementos / Sumatoria de los pesos)
    Factor_2 = Numerador_1 / Denominador_1
    Numerador_1 = Sumatoria_i Sumatoria_j de: (peso_ij) * (x_i - X_prom) * (x_j - X_prom)
    Denominador_1 = Sumatoria_i de: (x_i - X_prom)^2
*/

using namespace std;

string print_celda(int x, int y, int v){
    string s = "";
    s += "[" + to_string(x) + "][" + to_string(y) + "] (" + to_string(v) + ")";
    return s;
}

int main(int argc, char const *argv[]){
    if(argc < 2){
        // El archivo debe indicar numero_filas y numero_columnas
        // en la primera línea. Luego todos los datos
        cout << "Error! Faltan argumentos:" << endl;
        cout << argv[0] << " <input_file> [v]" << endl;
        cout << "v for Verbose" << endl;
        return 0;
    }
    bool verbose = false;
    if(argc == 3){
        cout << argv[2][0] << endl;
        if(argv[2][0] == 'v'){
            verbose = true;
        }
    }
    ifstream inputfile(argv[1]);
    if (!inputfile.is_open()){
        cout << "Error al abrir ejemplo.dat\n";
        exit(EXIT_FAILURE);
    }
	int rows, cols;
    inputfile >> rows >> cols;
    vector<vector<int>> grilla(rows, vector<int>(cols));
    long long acum = 0;
    for(int f=0; f<rows; f++){
        for(int c=0; c<cols; c++){
            inputfile >> grilla[f][c];
            acum += grilla[f][c];
        }
    }
    inputfile.close();

    int totalCeldas = rows*cols;
    double promedio = (acum+0.0) / totalCeldas;
    
    if(verbose){
        cout << "Grilla de " << rows << " X " << cols << endl;
        cout << "acum: " << acum << endl;
        cout << "promedio: " << promedio << endl;
        for(int f=0; f<rows; f++){
            for(int c=0; c<cols; c++){
                cout << grilla[f][c] << " ";
            }
            cout << endl;
        }
    }


    // Generando matriz de pesos
    int i,j;
	// Lectura de los enteros de 32 bits del archivo
//            cout << "OK" << endl;
    // Cálculo de la Estadística de Moran
    double sumaW = 0.0;
    double numerador = 0.0;
    double denominador = 0.0;
//            cout << "Calculando I de Moran.. " << endl;
    for(int f1 = 0; f1 < rows; f1++){
        for(int c1 = 0; c1 < cols; c1++) {
            double diff_i_promedio = grilla[f1][c1] - promedio;
            if(verbose){
                cout << "Desde celda: " << print_celda(f1, c1, grilla[f1][c1]) << endl;
            }
            double auxDenom = 0;
            for(int f2=(f1-1); f2<rows && f2<=(f1+1); f2++){
                for (int c2=(c1-1); c2<cols && c2<=(c1+1); c2++){
                    double w = 0;
                    double aux;
                    if((f1 != f2 || c1 != c2) && f2 >= 0 && c2 >= 0){
                        if(verbose){
                            cout << "\tRevisando celda: " << print_celda(f2, c2, grilla[f2][c2]);
                        }
                        // Para la contigüidad tipo reina las celdas vecinas que tocan
                        // toman un valor de 1
                        w = 1;
                        sumaW += w;
                        aux = w * (diff_i_promedio) * (grilla[f2][c2] - promedio);
                        auxDenom += aux;
                        numerador += aux;
                        if(verbose){
                            cout << " con valor: " << aux << endl;
                        }
                    }
                }
            }
            denominador += (diff_i_promedio * diff_i_promedio);
            cout << "\tTotal: " << auxDenom << endl;
        }
    }
    if(verbose){
        cout << "totalCeldas: " << totalCeldas << endl;
        cout << "sumaW: " << sumaW << endl;
        cout << "numerador: " << numerador << endl;
        cout << "denominador: " << denominador << endl;
        cout << "Grilla de " << rows << " X " << cols << endl;
    }
    double moran_I = (totalCeldas / sumaW) * (numerador / denominador);
    cout << argv[1] << "\t" << moran_I << endl;
    return 0;
}
