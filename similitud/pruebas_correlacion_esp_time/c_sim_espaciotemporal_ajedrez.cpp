#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

string print_serie(vector<double> v){
    string s = to_string(v[0]);
    for(int i=1; i<v.size(); i++){
        s += " " + to_string(v[i]);
    }
    return s;
}

string print_serie(vector<int> v){
    string s = to_string(v[0]);
    for(int i=1; i<v.size(); i++){
        s += " " + to_string(v[i]);
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

double calc_sumadiffST(vector<int> x, vector<int> y){
    double acum = 0;
    for(int i=0; i<x.size(); i++){
        acum += (x[i] - y[i]);
    }
    return acum;
}

double calc_sumadiffST(vector<int> x, vector<double> y){
    double acum = 0;
    for(int i=0; i<x.size(); i++){
        acum += (x[i] - y[i]);
    }
    return acum;
}

double calc_mape(vector<int> actual, vector<int> pred){
    double acum = 0.0;
    for(int i=0; i<actual.size(); i++){
        double temp = (actual[i] - pred[i]) / (actual[i] + 0.000001);
        acum += temp;
    }
    return (acum / actual.size());
}

double calc_euclidean(vector<int> actual, vector<int> pred){
    unsigned long long acum = 0;
    for(int i=0; i<actual.size(); i++){
        long diff = actual[i] - pred[i];
        diff *= diff;
        acum += diff;
    }
    return sqrt(acum);
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
    if(denCORTFactorX * denCORTFactorY == 0){
        cout << "Denominador en 0: " << auxDiffX << " " << auxDiffY << endl;
        cout << print_serie(X) << endl;
        cout << print_serie(Y) << endl;
    }
    return ZETA;
}

void indiceAET(vector<vector<vector<int>>> grilla){
    int rows = grilla.size();
    int cols = grilla[0].size();
    int lenTempSerie = grilla[0][0].size();
    //  Calculando serie temporal promedio
    vector<double> stPromedio(lenTempSerie);
    for(int i=0; i<lenTempSerie; i++){
        double acum = 0.0;
        for(int f=0; f<rows; f++){
            for(int c=0; c<cols; c++){
                acum += grilla[f][c][i];
            }
        }
        stPromedio[i] = (acum / (rows*cols));
    }
    //  Marcando series temporales fijas: todos sus elementos iguales
    vector<vector<bool>> serieFija(rows, vector<bool>(cols, true));
    for(int f=0; f<rows; f++){
        for(int c=0; c<cols; c++){
            for(int i=1; i<lenTempSerie; i++){
                serieFija[f][c] = serieFija[f][c] && (grilla[f][c][i] == grilla[f][c][i-1]);
            }
        }
    }
    
    vector<vector<double>> matrizZetha(rows, vector<double>(cols));
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            matrizZetha[i][j] = ZETA(grilla[i][j], stPromedio);
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
                        double w = 0.0;
                        double factor;
                        if((f1 != f2 || c1 != c2) && f2 >= 0 && c2 >= 0 && !serieFija[f2][c2]){
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
    double N = (rows*cols) + 0.0 - cantFijas;
    double IAET = (N / sumaW) * (numerador / denominador);
//    cout << argv[1] << " STA = " << IAET << endl;
    cout << "IAET\ttotalCeldas\tceldasFijas\ttotalW" << endl;
    cout << IAET << "\t" << (rows*cols) << "\t" << cantFijas << "\t" << sumaW << endl;
}

double moranI_sumadiferencias(vector<vector<vector<int>>> grilla){
    int rows = grilla.size();
    int cols = grilla[0].size();
    int instants = grilla[0][0].size();
    // Serie de tiempo promedio
    vector<double> stPromedio(instants);
    for(int i=0; i<instants; i++){
        double acum = 0.0;
        for(int f=0; f<rows; f++){
            for(int c=0; c<cols; c++){
                acum += grilla[f][c][i];
            }
        }
        stPromedio[i] = (acum / (rows*cols));
    }
    
    // Cálculo de la Estadística de Moran
    double sumaW = 0.0;
    double numerador = 0.0;
    double denominador = 0.0;
//    cout << "Calculando I de Moran.. " << endl;
    for(int f1 = 0; f1 < rows; f1++){
        for(int c1 = 0; c1 < cols; c1++) {
            double diff_i_promedio = calc_sumadiffST(grilla[f1][c1], stPromedio);
//            cout << "Desde celda: " << print_celda(f1, c1, grilla[f1][c1]) << " - " << diff_i_promedio << endl;
            for(int f2=(f1-1); f2<rows && f2<=(f1+1); f2++){
                for (int c2=(c1-1); c2<cols && c2<=(c1+1); c2++){
                    double w = 0;
                    double aux;
                    if((f1 != f2 || c1 != c2) && f2 >= 0 && c2 >= 0){
//                        cout << "\tRevisando celda: " << print_celda(f2, c2, grilla[f2][c2]);
                        // Para la contigüidad tipo reina las celdas vecinas que tocan
                        // toman un valor de 1
                        w = 1.0;
                        sumaW += w;
                        aux = w * (diff_i_promedio) * calc_sumadiffST(grilla[f2][c2], stPromedio);
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
    return moran_I;
}

int main(int argc, char const *argv[]){
    //{1,2,3,4,5,6,7,8,9,10};           // st_00
    //{2,3,4,5,6,7,8,9,10,11};          // st_01
    //{11,12,13,14,15,16,17,18,19,20};  // st_02
    //{10,9,8,7,6,5,4,3,2,1};           // st_03
    //{2,4,6,8,10,12,14,16,18,20};      // st_04
    //{1,3,5,7,9,11,13,15,17,19};       // st_05
    //{1,1,1,1,1,1,1,1,1,1};            // st_06
    //{5,5,5,5,5,5,5,5,5,5};            // st_07
    //{10,10,10,10,10,10,10,10,10,10};  // st_08
    //{15,15,15,15,15,15,15,15,15,15};  // st_09
    //{1,1,2,2,3,3,4,4,5,5};            // st_10
    //{10,10,9,9,8,8,7,7,6,6};          // st_11
    //{1,2,3,4,5,6,7,8,7,7};            // st_12
    //{2,2,3,4,5,6,7,8,9,10};           // st_13
    //{20,19,18,17,16,15,14,13,12,11};  // st_14
    //{20,18,16,14,12,10,8,6,4,2};      // st_15
    vector<int> serieA = {1,2,3,4,5,6,7,8,9,10};
    vector<int> serieB = {20,19,18,17,16,15,14,13,13,13};

    cout << "Serie A: " << print_serie(serieA) << endl;
    cout << "Serie B: " << print_serie(serieB) << endl;

    int rows = 8;
    int cols = 8;
    vector<vector<vector<int>>> grilla(rows, vector<vector<int>>(cols));

    cout << " +++++++++++++++++++++++++ " << endl;
    cout << "Tablero en forma tradicional" << endl;
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            if((i+j)%2 == 0){
                grilla[i][j] = serieA;
                cout << "A ";
            }else{
                grilla[i][j] = serieB;
                cout << "B ";
            }
        }
        cout << endl;
    }
    cout << "Moran's I por diferencias acumuladas:" << moranI_sumadiferencias(grilla) << endl;
    indiceAET(grilla);

    cout << " +++++++++++++++++++++++++ " << endl;
    cout << "Tablero en mitades" << endl;
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            if(i<(rows/2)){
                grilla[i][j] = serieA;
                cout << "A ";
            }else{
                grilla[i][j] = serieB;
                cout << "B ";
            }
        }
        cout << endl;
    }
    cout << "Moran's I por diferencias acumuladas:" << moranI_sumadiferencias(grilla) << endl;
    indiceAET(grilla);

    cout << " +++++++++++++++++++++++++ " << endl;
    cout << "Tablero en cuartos" << endl;
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            if(i<(rows/2)){
                if(j<(cols/2)){
                    grilla[i][j] = serieA;
                    cout << "A ";
                }else{
                    grilla[i][j] = serieB;
                    cout << "B ";
                }
            }else{                
                if(j>=(cols/2)){
                    grilla[i][j] = serieA;
                    cout << "A ";
                }else{
                    grilla[i][j] = serieB;
                    cout << "B ";
                }
            }
        }
        cout << endl;
    }
    cout << "Moran's I por diferencias acumuladas:" << moranI_sumadiferencias(grilla) << endl;
    indiceAET(grilla);

    return 0;
}
