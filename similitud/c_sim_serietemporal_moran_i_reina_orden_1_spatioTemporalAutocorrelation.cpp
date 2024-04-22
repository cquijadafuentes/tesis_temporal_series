#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>

/*
    Implementación de cálculo de correlación espacio temporal propuesto en:
    Measuring spatio-temporal autocorrelation in time series data of collective human mobility 
    [Gao, Cheng, Meng and Liu - Geo-spatial information science, 2019]
*/

using namespace std;

string print_celda(int x, int y, double v){
    string s = "";
    s += "[" + to_string(x) + "][" + to_string(y) + "] (" + to_string(v) + ")";
    return s;
}

string print_serie(vector<double> v){
    string s = "";
    for(int i=0; i<v.size(); i++){
        s += to_string(v[i]) + " ";
    }
    return s;
}

string print_serie(vector<int> v){
    string s = "";
    for(int i=0; i<v.size(); i++){
        s += to_string(v[i]) + " ";
    }
    return s;
}

double promedioST(vector<int> x){
    unsigned long long acum = 0;
    for(int i=0; i<x.size(); i++){
        acum += x[i];
    }
    acum /= x.size();
    return acum;
}

double ZETA(vector<int> X, vector<double> Y){
    /*
        - First CORT is calculated: 
            Adaptative temporal dissimilarity:
            covers temporal correlation and data distance
            Result in range -1 to 1:
                -1 X and Y share similar growth in rate but opposite in direction
                1 X and Y have a similiar growth in rate and direction
                0 different temporal behaviors
        - Then PHI is applied:
            tunes the first order correlation coefficient from [-1,1] to [0,2] where
            phi(0) = 1, phi(1) = 0, and phi(-1) = 2.
            Then, less value means more similar time series, 
            and greater values means less similar time series.
        - Finally is multiplied by the difference of accumulative volumes
    */
    double numeradorCORT = 0.0;
    double denCORTFactorX = 0.0;
    double denCORTFactorY = 0.0;
    double auxDiffX, auxDiffY;
    double accVolX = 0.0 + X[0];
    double accVolY = 0.0 + X[0];
    for(int i=1; i <X.size(); i++){
        auxDiffX = X[i]-X[i-1];
        auxDiffY = Y[i]-Y[i-1];
        numeradorCORT += (auxDiffX*auxDiffY);
        denCORTFactorX += (auxDiffX*auxDiffX);
        denCORTFactorY += (auxDiffY*auxDiffY);
        accVolX += X[i];
        accVolY += Y[i];
    }
    double denominadorCORT = sqrt(denCORTFactorX) * sqrt(denCORTFactorY);
    double CORT = numeradorCORT / denominadorCORT;
    double PHI = 2 / (1 + exp(2*CORT));
    double ZETA = PHI * (accVolX - accVolY);
//    if(denCORTFactorX * denCORTFactorY == 0){
//        cout << "Denominador en 0: " << auxDiffX << " " << auxDiffY << endl;
//        cout << print_serie(X) << endl;
//        cout << print_serie(Y) << endl;
//    }
    return ZETA;
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

    //  Calculando serie temporal promedio
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
    //  Marcando series temporales fijas: todos sus elementos iguales
    vector<vector<bool>> serieFija(rows, vector<bool>(cols, true));
    for(int f=0; f<rows; f++){
        for(int c=0; c<cols; c++){
            for(int i=1; i<lenTempSerie; i++){
                serieFija[f][c] = serieFija[f][c] && (temporalSeries[f][c][i] == temporalSeries[f][c][i-1]);
            }
        }
    }
    
    vector<vector<double>> matrizZetha(rows, vector<double>(cols));
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            matrizZetha[i][j] = ZETA(temporalSeries[i][j], stPromedio);
//            cout << matrizZetha[i][j] << "\t";
        }
//        cout << endl;
    }

    // Cálculo de Medida de utocorrelación espacio-temporal
    int sumaW = 0;
    int cantFijas = 0;
    double numerador = 0.0;
    double denominador = 0.0;
    for(int f1 = 0; f1 < rows; f1++){
        for(int c1 = 0; c1 < cols; c1++) {
            if(!serieFija[f1][c1]){
                for(int f2=(f1-1); f2<rows && f2<=(f1+1); f2++){
                    for (int c2=(c1-1); c2<cols && c2<=(c1+1); c2++){
                        double w = 0;
                        double factor;
                        if(!serieFija[f2][c2] && (f1 != f2 || c1 != c2) && f2 >= 0 && c2 >= 0){
    //                        cout << "\tRevisando celda: " << print_celda(f2, c2, stPromedio[f2][c2]);
                            // Para la contigüidad tipo reina las celdas vecinas que tocan
                            // toman un valor de 1
                            w = 1;
                            sumaW += w;
                            factor = w * matrizZetha[f1][c1] * matrizZetha[f2][c2];
    //                        cout << " con w=" << w << " xi=" << diff_i_promedio << " y xj " << xxx << " y resultado " << aux <<endl;
                            numerador += factor;
                        }
                    }
                }
                denominador += (matrizZetha[f1][c1] * matrizZetha[f1][c1]);
            }else{
                cantFijas++;
            }
        }
    }
//    cout << "totalCeldas: " << (rows*cols) << endl;
//    cout << "cantFijas: " << cantFijas << endl;
//    cout << "sumaW: " << sumaW << endl;
//    cout << "numerador: " << numerador << endl;
//    cout << "denominador: " << denominador << endl;
    double moran_I = (((rows*cols) + 0.0) / sumaW) * (numerador / denominador);
//    cout << argv[1] << " STA = " << moran_I << endl;
    cout << "dataset\tmoranI\ttotalCeldas\tceldasFijas\ttotalW" << endl;
    cout << argv[1] << "\t" << moran_I << "\t" << (rows*cols) << "\t" << cantFijas << "\t" << sumaW << endl;
    return 0;
}
