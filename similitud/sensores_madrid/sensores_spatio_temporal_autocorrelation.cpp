#include <fstream>
#include <iostream>
#include <math.h>
#include <map>
#include <vector>
#include <string>

/*
    Implementación de cálculo de correlación espacio temporal propuesto en:
    Measuring spatio-temporal autocorrelation in time series data of collective human mobility 
    [Gao, Cheng, Meng and Liu - Geo-spatial information science, 2019]
    Aplicado a datos de sensores de tráfico de madrid
*/

using namespace std;

string print_celda(int x, int y, float v){
    string s = "";
    s += "[" + to_string(x) + "][" + to_string(y) + "] (" + to_string(v) + ")";
    return s;
}

string print_serie(vector<float> v){
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

float promedioST(vector<int> x){
    unsigned long long acum = 0;
    for(int i=0; i<x.size(); i++){
        acum += x[i];
    }
    acum /= x.size();
    return acum;
}

double ZETA(vector<float> X, vector<float> Y){
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

double ZETA(vector<int> X, vector<float> Y){
    /*
        Igual a la función anterior pero para valores enteros
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
    return ZETA;
}

template <typename TipoNumero>
void calculaIAET(vector<int> ids, const vector<vector<TipoNumero>>& datos, map<int, int> mapDatos, vector<vector<int>> pesos, map<int, int> mapPesos, string name){
    int sensores = datos.size();
    int muestras = datos[0].size();
    //  Calculando serie temporal promedio
    vector<float> stPromedio(muestras);
    for(int m=0; m<muestras; m++){
        double acum = 0.0;
        for(int e=0; e<sensores; e++){
            acum += datos[e][m];
        }
        stPromedio[m] = (acum / sensores);
    }

    //  Marcando series temporales fijas: todos sus elementos iguales
    vector<bool> serieFija(sensores, true);
    for(int e=0; e<sensores; e++){
        for(int m=1; m<muestras; m++){
            serieFija[e] = serieFija[e] && (datos[e][m] == datos[e][m-1]);
        }
    }
    
    vector<double> zethaValues(sensores);
    for(int e=0; e<sensores; e++){
        zethaValues[e] = ZETA(datos[e], stPromedio);
    }

    // Cálculo de Medida de utocorrelación espacio-temporal
    int sumaW = 0;
    int cantFijas = 0;
    int cantSinPeso = 0;
    int cantSinVecinos = 0;
    int valorN = 0;
    double numerador = 0.0;
    double denominador = 0.0;
    for(int e=0; e<sensores; e++){
        if(!serieFija[e] && (mapPesos.find(ids[e]) != mapPesos.end())){
            vector<int> idsVecinos = pesos[mapPesos[ids[e]]];
            int vecinosValidos = 0;
            for(int j=0; j<idsVecinos.size(); j++){
                map<int,int>::iterator it = mapDatos.find(idsVecinos[j]);
                if(it != mapDatos.end()){
                    int celdaDatosVecino = mapDatos[idsVecinos[j]];
                    double factor;
                    if(!serieFija[celdaDatosVecino]){
                        vecinosValidos++;
                        sumaW += 1;
                        factor = zethaValues[e] * zethaValues[celdaDatosVecino];
                        numerador += factor;
                    }
                }
            }
            if(vecinosValidos>0){
                denominador += (zethaValues[e] * zethaValues[e]);
                valorN++;
            }else{
                cantSinVecinos++;
            }
        }else if(serieFija[e]){
            cantFijas++;
        }else{
            cantSinPeso++;
        }
    }

    // El valor N depende en este caso de los sensores considerados en la matriz de pesos
    double N = sensores - cantFijas - cantSinPeso - cantSinVecinos;
    double IAET = (N / sumaW) * (numerador / denominador);
    cout << "N: " << N << " - valorN: " << valorN << endl;
    cout << "dataset\tIAET\tN\ttotalCeldas\tceldasPesos\tceldasFijas\tceldasSinPeso\ttotalW" << endl;
    cout << name << "\t" << IAET << "\t" << N << "\t" << sensores << "\t" << pesos.size() << "\t" << cantFijas << "\t" << cantSinPeso << "\t" << sumaW << endl;
}

int main(int argc, char const *argv[]){
    /*
        Se recibe como entrada las series de tiempo de los sensores de madrid con
        los datos de intensidad, ocupacion, carga y vmed en el siguiente formato:
        num_sensores    num_muestras
        id.1 id.2 id.3 ... id.numsensores
        ts_id.1_in.1 ts_id.1_in.2 ... ts_id.1_in.nummuestras
        ts_id.1_oc.1 ts_id.1_oc.2 ... ts_id.1_oc.nummuestras
        ts_id.1_ca.1 ts_id.1_ca.2 ... ts_id.1_ca.nummuestras
        ts_id.1_vm1 ts_id.1_vm2 ... ts_id.1_vm.nummuestras
        ts_id.2_in.1 ts_id.2_in.2 ... ts_id.2_in.nummuestras
        ts_id.2_oc.1 ts_id.2_oc.2 ... ts_id.2_oc.nummuestras
        ts_id.2_ca.1 ts_id.2_ca.2 ... ts_id.2_canummuestra.s
        ts_id.2_vm.1 ts_id.2_vm.2 ... ts_id.2_vm.nummuestras
        ...
        ts_id.numsensores_in.1 ts_id.numsensores_in.2 ... ts_id.numsensores_in.nummuestras
        ts_id.numsensores_oc.1 ts_id.numsensores_oc.2 ... ts_id.numsensores_oc.nummuestras
        ts_id.numsensores_ca.1 ts_id.numsensores_ca.2 ... ts_id.numsensores_ca.nummuestras
        ts_id.numsensores_vm.1 ts_id.numsensores_vm.2 ... ts_id.numsensores_vm.nummuestras

        La lista de pesos debe estar en el siguiente formato:
        La primera línea la cantidad de sensores
        La segunda línea todos los ids de sensores
        Por cada sensor una línea indicando su identificador, cantidad de vecinos, identificador de cada vecino.
    */
    if(argc < 3){
        cout << "Error! Faltan argumentos." << endl;
        cout << argv[0] << " <time_series_sensors_madrid.txt> <w_list_file.txt>" << endl;
        return 0;
    }
    cout << "DATASET: " << argv[1] << endl;
    ifstream dataSensores(argv[1], ifstream::in);
    if(dataSensores.fail()){
        cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
        return -1;
    }
    vector<string> titulos = {"intensidad", "ocupacion", "carga", "vmed"};
    int sensores, muestras;
    dataSensores >> sensores >> muestras;
    cout << sensores << " - " << muestras << endl;
    vector<int> ids(sensores);
    map<int, int> mapIdsDatos;
    int auxInt;
    for(int i=0; i<sensores; i++){
        // Lectura de los identificadores de sensores
        dataSensores >> auxInt;
        mapIdsDatos[auxInt] = i;
        ids[i] = auxInt;
    }
    // Vectores con los datos
    vector<vector<int>> dataInten(sensores, vector<int>(muestras));
    vector<vector<float>> dataOcupa(sensores, vector<float>(muestras));
    vector<vector<int>> dataCarga(sensores, vector<int>(muestras));
    vector<vector<float>> dataVelMed(sensores, vector<float>(muestras));
    // Valores auxiliares para 'tapar' los valores nan
    int auxInten;
    float auxOcupa;
    int auxCarga;
    float auxVelMed;
    string auxString;
    vector<int> nans(4,0);
    for(int i=0; i<sensores; i++){
        // Intensidad
        for(int k=0; k<muestras; k++){
            dataSensores >> auxString;
            if(auxString == "nan"){
                dataInten[i][k] = auxInten;
                nans[0]++;
            }else{
                dataInten[i][k] = stof(auxString);
                auxInten = dataInten[i][k];
            }
        }
        // Ocupación
        for(int k=0; k<muestras; k++){
            dataSensores >> auxString;
            if(auxString == "nan"){
                dataOcupa[i][k] = auxOcupa;
                nans[1]++;
            }else{
                dataOcupa[i][k] = stoi(auxString);
                auxOcupa = dataOcupa[i][k];
            }
        }
        // Carga
        for(int k=0; k<muestras; k++){
            dataSensores >> auxString;
            if(auxString == "nan"){
                dataCarga[i][k] = auxCarga;
                nans[2]++;
            }else{
                dataCarga[i][k] = stof(auxString);
                auxCarga = dataCarga[i][k];
            }
        }
        // Velocidad Media
        for(int k=0; k<muestras; k++){
            dataSensores >> auxString;
            if(auxString == "nan"){
                dataVelMed[i][k] = auxVelMed;
                nans[3]++;
            }else{
                dataVelMed[i][k] = stof(auxString);
                auxVelMed = dataVelMed[i][k];
            }
        }
    }
    dataSensores.close();

    cout << "Lista de pesos W: " << argv[2] << endl;
    ifstream entradaListaPesos(argv[2], ifstream::in);
    if(entradaListaPesos.fail()){
        cout << "Error! Lectura de " << argv[2] << " fallida." << endl;
        return -1;
    }
    map<int, int> mapIdsLista;
    int listIds;
    entradaListaPesos >> listIds;
    cout << "Ids en la lista de pesos: " << listIds << endl;
    for(int i=0; i<listIds; i++){
        entradaListaPesos >> auxInt;
        mapIdsLista[auxInt] = i;
    }
    vector<vector<int>> listaPesos(listIds, vector<int>());
    for(int i=0; i<listIds; i++){
        entradaListaPesos >> auxInt;    // Identificador de la lista
        entradaListaPesos >> auxInt;    // Cantidad de elementos de la lista
        listaPesos[i] = vector<int>(auxInt);
        for(int j=0; j<auxInt; j++){
            entradaListaPesos >> listaPesos[i][j];
        }
    }
    entradaListaPesos.close();

    cout << "Cantidad sensores: " << dataInten.size() << endl;
    cout << "Cantidad sensores con vecinos: " << listaPesos.size() << endl;
    cout << "Cantidad de nans: " << nans[0] << "\t" << nans[1] << "\t" << nans[2] << "\t" << nans[3] << endl;

    calculaIAET(ids, dataInten, mapIdsDatos, listaPesos, mapIdsLista, titulos[0]);
    calculaIAET(ids, dataOcupa, mapIdsDatos, listaPesos, mapIdsLista, titulos[1]);
    calculaIAET(ids, dataCarga, mapIdsDatos, listaPesos, mapIdsLista, titulos[2]);
    calculaIAET(ids, dataVelMed, mapIdsDatos, listaPesos, mapIdsLista, titulos[3]);

    return 0;
}
