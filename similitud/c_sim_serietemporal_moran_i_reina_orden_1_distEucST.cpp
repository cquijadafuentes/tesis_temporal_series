#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>

/*
    Variante de la correlación espacial aplicada a las series de tiempo en datos raster
    En este caso la diferencia entre los valores de dos celdas corresponderán a la distancia
    euclidea entre las dos series de tiempo
    
    La estadística de Moran se calculará en base a la fórmula que indica:
    I = (Factor_1) * (Factor_2)
    donde
    Factor_1 = (Cantidad de elementos / Sumatoria de los pesos)
    Factor_2 = Numerador_1 / Denominador_1
    Numerador_1 = Sumatoria_i Sumatoria_j de: (peso_ij) * (x_i - X_prom) * (x_j - X_prom)
    Denominador_1 = Sumatoria_i de: (x_i - X_prom)^2
*/

using namespace std;

string print_celda(int x, int y, vector<int> v){
    string s = "";
    s += "[" + to_string(x) + "][" + to_string(y) + "] (" + to_string(v[0]);
    for(int i=1; i<v.size(); i++){
        s += "," + to_string(v[i]);
    }
    s += ")";
    return s;
}

string print_celda(int x, int y, vector<double> v){
    string s = "";
    s += "[" + to_string(x) + "][" + to_string(y) + "] (" + to_string(v[0]);
    for(int i=1; i<v.size(); i++){
        s += "," + to_string(v[i]);
    }
    s += ")";
    return s;
}

string print_serie(vector<int> v){
    string s = "";
    for(int i=0; i<v.size(); i++){
        s += to_string(v[i]) + " ";
    }
    return s;
}

string print_serie(vector<double> v){
    string s = "";
    for(int i=0; i<v.size(); i++){
        s += to_string(v[i]) + " ";
    }
    return s;
}

double calc_euclidean(vector<int> actual, vector<double> pred){
    unsigned long long acum = 0;
    for(int i=0; i<actual.size(); i++){
        long diff = actual[i] - pred[i];
        diff *= diff;
        acum += diff;
    }
    return sqrt(acum);
}

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <filename>" << endl;
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

    vector<double> stPromedio(lenTempSerie);
    for(int i=0; i<lenTempSerie; i++){
        double acum = 0.0;
        for(int f=0; f<rows; f++){
            for(int c=0; c<cols; c++){
                acum += temporalSeries[f][c][i];
            }
        }
        stPromedio[i] = (acum / (rows*cols));
    }
    cout << "Serie promedio: " << print_serie(stPromedio) << endl;

    // Cálculo de la Estadística de Moran
    double sumaW = 0.0;
    double numerador = 0.0;
    double denominador = 0.0;
//    cout << "Calculando I de Moran.. " << endl;
    for(int f1 = 0; f1 < rows; f1++){
        for(int c1 = 0; c1 < cols; c1++) {
            double diff_i_promedio = calc_euclidean(temporalSeries[f1][c1], stPromedio);
//            cout << "Desde celda: " << print_celda(f1, c1, temporalSeries[f1][c1]) << " - " << diff_i_promedio << endl;
            for(int f2=(f1-1); f2<rows && f2<=(f1+1); f2++){
                for (int c2=(c1-1); c2<cols && c2<=(c1+1); c2++){
                    double w = 0;
                    double aux;
                    if((f1 != f2 || c1 != c2) && f2 >= 0 && c2 >= 0){
//                        cout << "\tRevisando celda: " << print_celda(f2, c2, temporalSeries[f2][c2]);
                        // Para la contigüidad tipo reina las celdas vecinas que tocan
                        // toman un valor de 1
                        w = 1.0;
                        sumaW += w;
                        aux = w * (diff_i_promedio) * calc_euclidean(temporalSeries[f2][c2], stPromedio);
//                        cout << " con w=" << w << " xi=" << diff_i_promedio << " y xj " << xxx << " y resultado " << aux <<endl;
                        numerador += aux;
                    }
                }
            }
            denominador += (diff_i_promedio * diff_i_promedio);
        }
    }
//    cout << "totalCeldas: " << (rows*cols) << endl;
//    cout << "sumaW: " << sumaW << endl;
//    cout << "numerador: " << numerador << endl;
//    cout << "denominador: " << denominador << endl;
    double moran_I = ((rows*cols + 0.0) / sumaW) * ((numerador + 0.0) / denominador);
    cout << argv[1] << " Moran's I (ro1)= " << moran_I << endl;
    return 0;
}
