#include <fstream>
#include <iostream>
#include <math.h>
#include <map>
#include <vector>
#include <set>
#include <string>

using namespace std;

void comparador(vector<int> &idsA, vector<vector<vector<int>>> &dataA, map<int,int> &mapIdPosB, vector<vector<vector<int>>> &dataB);

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
		cout << argv[0] << " <data_sensores_A.txt> <data_sensores_B.txt>" << endl;
		return 0;
	}
	vector<string> titulos = {"intensidad", "ocupacion", "carga\t", "vmed\t"};
	int auxId;
	string auxString;
	
	cout << "Dataset A: " << argv[1] << endl;
	ifstream dataSensoresA(argv[1], ifstream::in);
	if(dataSensoresA.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}
	int sensoresA, muestrasA;
	dataSensoresA >> sensoresA >> muestrasA;
	cout << "\tCantidad de sensores: " << sensoresA << endl;
	cout << "\tCantidad de muestras: " << muestrasA << endl;
	vector<int> idsA(sensoresA);
	map<int, int> mapIdsPosA;
	for(int i=0; i<sensoresA; i++){
		// Lectura de los identificadores de sensores
		dataSensoresA >> auxId;
		mapIdsPosA[auxId] = i;
		idsA[i] = auxId;
	}
	// Vectores con los datos
	vector<vector<int>> dataIntenA(sensoresA, vector<int>(muestrasA));
	vector<vector<int>> dataOcupaA(sensoresA, vector<int>(muestrasA));
	vector<vector<int>> dataCargaA(sensoresA, vector<int>(muestrasA));
	vector<vector<int>> dataVelMedA(sensoresA, vector<int>(muestrasA));
	vector<int> nansA(4,0);
	for(int i=0; i<sensoresA; i++){
		// Intensidad
		for(int k=0; k<muestrasA; k++){
			dataSensoresA >> auxString;
			if(auxString == "nan"){
				dataIntenA[i][k] = 0;
				nansA[0]++;
			}else{
				dataIntenA[i][k] = stof(auxString);
			}
		}
		// Ocupación
		for(int k=0; k<muestrasA; k++){
			dataSensoresA >> auxString;
			if(auxString == "nan"){
				dataOcupaA[i][k] = 0;
				nansA[1]++;
			}else{
				dataOcupaA[i][k] = stoi(auxString);
			}
		}
		// Carga
		for(int k=0; k<muestrasA; k++){
			dataSensoresA >> auxString;
			if(auxString == "nan"){
				dataCargaA[i][k] = 0;
				nansA[2]++;
			}else{
				dataCargaA[i][k] = stof(auxString);
			}
		}
		// Velocidad Media
		for(int k=0; k<muestrasA; k++){
			dataSensoresA >> auxString;
			if(auxString == "nan"){
				dataVelMedA[i][k] = 0;
				nansA[3]++;
			}else{
				dataVelMedA[i][k] = stof(auxString);
			}
		}
	}
	dataSensoresA.close();


	cout << "Dataset B: " << argv[2] << endl;
	ifstream dataSensoresB(argv[2], ifstream::in);
	if(dataSensoresB.fail()){
		cout << "Error! Lectura de " << argv[2] << " fallida." << endl;
		return -1;
	}
	int sensoresB, muestrasB;
	dataSensoresB >> sensoresB >> muestrasB;
	cout << "\tCantidad de sensores: " << sensoresB << endl;
	cout << "\tCantidad de muestras: " << muestrasB << endl;
	vector<int> idsB(sensoresB);
	map<int, int> mapIdsPosB;
	for(int i=0; i<sensoresB; i++){
		// Lectura de los identificadores de sensores
		dataSensoresB >> auxId;
		mapIdsPosB[auxId] = i;
		idsB[i] = auxId;
	}
	// Vectores con los datos
	vector<vector<int>> dataIntenB(sensoresB, vector<int>(muestrasB));
	vector<vector<int>> dataOcupaB(sensoresB, vector<int>(muestrasB));
	vector<vector<int>> dataCargaB(sensoresB, vector<int>(muestrasB));
	vector<vector<int>> dataVelMedB(sensoresB, vector<int>(muestrasB));
	vector<int> nansB(4,0);
	for(int i=0; i<sensoresB; i++){
		// Intensidad
		for(int k=0; k<muestrasB; k++){
			dataSensoresB >> auxString;
			if(auxString == "nan"){
				dataIntenB[i][k] = 0;
				nansB[0]++;
			}else{
				dataIntenB[i][k] = stoi(auxString);
			}
		}
		// Ocupación
		for(int k=0; k<muestrasB; k++){
			dataSensoresB >> auxString;
			if(auxString == "nan"){
				dataOcupaB[i][k] = 0;
				nansB[1]++;
			}else{
				dataOcupaB[i][k] = stoi(auxString);
			}
		}
		// Carga
		for(int k=0; k<muestrasB; k++){
			dataSensoresB >> auxString;
			if(auxString == "nan"){
				dataCargaB[i][k] = 0;
				nansB[2]++;
			}else{
				dataCargaB[i][k] = stoi(auxString);
			}
		}
		// Velocidad Media
		for(int k=0; k<muestrasB; k++){
			dataSensoresB >> auxString;
			if(auxString == "nan"){
				dataVelMedB[i][k] = 0;
				nansB[3]++;
			}else{
				dataVelMedB[i][k] = stof(auxString);
			}
		}
	}
	dataSensoresB.close();

	// COMPARANDO
	cout << "Comparando..." << endl;
	if(muestrasA != muestrasB){
		cout << "No se pueden comparar los datos porque no coincide el número de muestras." << endl;
		cout << "Muestras de " << argv[1] << ": " << muestrasA << endl;
		cout << "Muestras de " << argv[2] << ": " << muestrasB << endl;
		return 0;
	}

	vector<vector<vector<int>>> dataA = {dataIntenA, dataOcupaA, dataCargaA, dataVelMedA};
	vector<vector<vector<int>>> dataB = {dataIntenB, dataOcupaB, dataCargaB, dataVelMedB};

	if(sensoresA < sensoresB){
		comparador(idsA, dataA, mapIdsPosB, dataB);
	}else{
		comparador(idsB, dataB, mapIdsPosA, dataA);
	}


	return 0;
}


void comparador(vector<int> &idsA, vector<vector<vector<int>>> &dataA, map<int,int> &mapIdPosB, vector<vector<vector<int>>> &dataB){
	int difInten = 0;
	int difOcupa = 0;
	int difCarga = 0;
	int difVelMed = 0;
	int senComunes = 0;
	int sensoresConDiferencia = 0;
	for(int i=0; i<idsA.size(); i++){
		if(mapIdPosB.find(idsA[i]) != mapIdPosB.end()){
			bool difSensor = false;
			senComunes++;
			int posB = mapIdPosB[idsA[i]];
			for(int j=0; j<dataA[0][0].size(); j++){
				if(dataA[0][i][j] != dataB[0][posB][j]){
					difInten++;
					difSensor = true;
				}
				if(dataA[1][i][j] != dataB[1][posB][j]){
					difOcupa++;
					difSensor = true;
				}
				if(dataA[2][i][j] != dataB[2][posB][j]){
					difCarga++;
					difSensor = true;
				}
				if(dataA[3][i][j] != dataB[3][posB][j]){
					difVelMed++;
					difSensor = true;
				}
			}
			if(difSensor){
				sensoresConDiferencia++;
			}
		}	
	}
	cout << "Sensores en común: " << senComunes << endl;
	cout << "Sensores en común con diferencias: " << sensoresConDiferencia << endl;
	cout << "Diferencias en Inten: " << difInten << endl;
	cout << "Diferencias en Ocupa: " << difOcupa << endl;
	cout << "Diferencias en Carga: " << difCarga << endl;
	cout << "Diferencias en VelMed: " << difVelMed << endl;
	return;
}