#include <fstream>
#include <iostream>
#include <math.h>
#include <map>
#include <vector>
#include <string>

using namespace std;

void mostrar(vector<vector<int>> const v, string title);

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
    */
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <outputFilePrefix>" << endl;
		return 0;
	}
    ifstream dataSensores(argv[1], ifstream::in);
    if(dataSensores.fail()){
        cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
        return -1;
    }

    int sensores, muestras;
    dataSensores >> sensores >> muestras;
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
    vector<int> idsInten;
    vector<vector<int>> dataInten;
    vector<int> idsOcupa;
    vector<vector<int>> dataOcupa;
    vector<int> idsCarga;
    vector<vector<int>> dataCarga;
    vector<int> idsVelMed;
    vector<vector<int>> dataVelMed;
    string auxString;
    vector<int> auxVector;
    int nan;
    int ceros;
    int prevIn, prevOc, prevCa, prevVm;
    	
    for(int i=0; i<sensores; i++){
        // Intensidad
        auxVector = vector<int>(muestras);
        nan = 0;
        ceros = 0;
        for(int k=0; k<muestras; k++){
            dataSensores >> auxString;
            if(auxString == "nan"){
                auxVector[k] = prevIn;
                nan++;
            }else{
                auxVector[k] = (int)(stof(auxString)*100);
                prevIn = auxVector[k];
            }
            if(auxVector[k] == 0){
            	ceros++;
            }
        }
        if(nan < 0.25*muestras && ceros < muestras){
        	idsInten.push_back(ids[i]);
        	dataInten.push_back(auxVector);
        }
        // Ocupación
        auxVector = vector<int>(muestras);
        nan = 0;
        ceros = 0;
        for(int k=0; k<muestras; k++){
            dataSensores >> auxString;
            if(auxString == "nan"){
                auxVector[k] = prevOc;
                nan++;
            }else{
                auxVector[k] = (int)(stof(auxString)*100);
                prevOc = auxVector[k];
            }
            if(auxVector[k] == 0){
            	ceros++;
            }
        }
        if(nan < 0.25*muestras && ceros < muestras){
        	idsOcupa.push_back(ids[i]);
        	dataOcupa.push_back(auxVector);
        }
        // Carga
        auxVector = vector<int>(muestras);
        nan = 0;
        ceros = 0;
        for(int k=0; k<muestras; k++){
            dataSensores >> auxString;
            if(auxString == "nan"){
                auxVector[k] = prevCa;
                nan++;
            }else{
                auxVector[k] = (int)(stof(auxString)*100);
                prevCa = auxVector[k];
            }
            if(auxVector[k] == 0){
            	ceros++;
            }
        }
        if(nan < 0.25*muestras && ceros < muestras){
        	idsCarga.push_back(ids[i]);
        	dataCarga.push_back(auxVector);
        }
        // Velocidad Media
        auxVector = vector<int>(muestras);
        nan = 0;
        ceros = 0;
        for(int k=0; k<muestras; k++){
            dataSensores >> auxString;
            if(auxString == "nan"){
                auxVector[k] = prevVm;
                nan++;
            }else{
                auxVector[k] = (int)(stof(auxString)*100);
                prevVm = auxVector[k];
            }
            if(auxVector[k] == 0){
            	ceros++;
            }
        }
        if(nan < 0.25*muestras && ceros < muestras){
        	idsVelMed.push_back(ids[i]);
        	dataVelMed.push_back(auxVector);
        }

    }
    dataSensores.close();

    // Guardando archivos INTENSIDAD
    if(idsInten.size() > 0){
    	auxString = string(argv[2]);
    	auxString += ".sm_in.bin";
	    ofstream outfile(auxString, ofstream::binary);
	    // Guardando valores enteros
	    auxInt = idsInten.size();
	    outfile.write((char const*)&auxInt, sizeof(int));
	    outfile.write((char const*)&muestras, sizeof(int));
	    for(int k=0; k<idsInten.size(); k++){
			outfile.write((char const*)&idsInten[k], sizeof(int));
		}
	    for(int i=0; i<dataInten.size(); i++){
	    	for(int j=0; j<muestras; j++){
    			outfile.write((char const*)&dataInten[i][j], sizeof(int));
	    	}        
	    }
    }

    // Guardando archivos OCUPACIÓN
    if(idsOcupa.size() > 0){
    	auxString = string(argv[2]);
    	auxString += ".sm_oc.bin";
	    ofstream outfile(auxString, ofstream::binary);
	    // Guardando valores enteros
	    auxInt = idsOcupa.size();
	    outfile.write((char const*)&auxInt, sizeof(int));
	    outfile.write((char const*)&muestras, sizeof(int));
	    for(int k=0; k<idsOcupa.size(); k++){
			outfile.write((char const*)&idsOcupa[k], sizeof(int));
		}
	    for(int i=0; i<dataOcupa.size(); i++){
	    	for(int j=0; j<muestras; j++){
    			outfile.write((char const*)&dataOcupa[i][j], sizeof(int));
	    	}        
	    }
    }

    // Guardando archivos CARGA
    if(idsCarga.size() > 0){
    	auxString = string(argv[2]);
    	auxString += ".sm_ca.bin";
	    ofstream outfile(auxString, ofstream::binary);
	    // Guardando valores enteros
	    auxInt = idsCarga.size();
	    outfile.write((char const*)&auxInt, sizeof(int));
	    outfile.write((char const*)&muestras, sizeof(int));
	    for(int k=0; k<idsCarga.size(); k++){
			outfile.write((char const*)&idsCarga[k], sizeof(int));
		}
	    for(int i=0; i<dataCarga.size(); i++){
	    	for(int j=0; j<muestras; j++){
    			outfile.write((char const*)&dataCarga[i][j], sizeof(int));
	    	}        
	    }
    }

    // Guardando archivos VELOCIDAD MEDIA
    if(idsVelMed.size() > 0){
    	auxString = string(argv[2]);
    	auxString += ".sm_vm.bin";
	    ofstream outfile(auxString, ofstream::binary);
	    // Guardando valores enteros
	    auxInt = idsVelMed.size();
	    outfile.write((char const*)&auxInt, sizeof(int));
	    outfile.write((char const*)&muestras, sizeof(int));
	    for(int k=0; k<idsVelMed.size(); k++){
			outfile.write((char const*)&idsVelMed[k], sizeof(int));
		}
	    for(int i=0; i<dataVelMed.size(); i++){
	    	for(int j=0; j<muestras; j++){
    			outfile.write((char const*)&dataVelMed[i][j], sizeof(int));
	    	}        
	    }
    }

    mostrar(dataInten, "Intensidad");
    mostrar(dataOcupa, "Ocupación");
    mostrar(dataCarga, "Carga");
    mostrar(dataVelMed, "Velocidad Media");

}

void mostrar(vector<vector<int>> const v, string title){
	int muestras = v[0].size();
	int series = v.size();
	cout << title << endl;
	cout << "Series de tiempo: " << series << endl;
    cout << "Muestras: " << muestras << endl;
    cout << "Pimera serie de tiempo: " << endl;
    for(int i=0; i<muestras; i++){
        cout << "\t" << v[0][i];
    }
    cout << endl;
    cout << "Última serie de tiempo: " << endl;
    for(int i=0; i<muestras; i++){
        cout << "\t" << v[series-1][i];
    }
    cout << endl;
}
