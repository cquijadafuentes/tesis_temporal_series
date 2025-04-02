
#include "TS_SM_week.hpp"

TempSeriesSensoresMadridWeek::TempSeriesSensoresMadridWeek(vector<vector<vector<int>>>&valores, vector<int>&cantidades, vector<int>&ids, int muestras){
	int aux;
	bool flag = false;
	cout << "Verificando datos..." << endl;
	if(cantidades.size() != valores.size()){
		cout << "Largo de 'cantidades' y 'valores' no coindice." << endl;
		flag = true;
	}

	int totalSensoresValores = 0;
	int totalSensoresCantidades = 0;
	for(int g=0; g<cantidades.size(); g++){
		//	Recorriendo grupos
		if(valores[g].size() != cantidades[g]){
			cout << "En grupo " << g << " no coinciden la cantidad del grupo y la cantidad de valores." << endl;
			flag = true;
		}
		totalSensoresValores += valores[g].size();
		totalSensoresCantidades += cantidades[g];
		for(int i=0; i<cantidades[g]; i++){
			aux = valores[g][i].size();
			if(aux != muestras){
				cout << "La serie de tiempo " << i << " del grupo " << g << " tiene " << aux << " muestras en vez de " << muestras << "." << endl;
				flag = true;
			}
		}
	}

	if(totalSensoresValores != ids.size()){
		cout << "Cantidad de sensores del arreglo 'Valores' no coincide con el número de IDs" << endl;
		flag = true;
	}
	if(totalSensoresCantidades != ids.size()){
		cout << "Cantidad de sensores del arreglo 'Cantidades' no coincide con el número de IDs" << endl;
		flag = true;
	}

	if(flag){
		cout << "Se cancela construcción de la estructura." << endl;
		return;
	}

	cout << "Construyendo ..." << endl;

	int auxP = 0;
	num_sensores = ids.size();
	num_groups = cantidades.size();
	num_muestras = muestras;
	num_times_x_week = 7 * 24 * 4;

	encuentraLimites(valores);
	min_id = ids[0];
	for(int i=1; i<ids.size(); i++){
		if(ids[i] < min_id){
			min_id = ids[i];
		}
	}

	sens_x_group = vector<int>(num_groups);
	for(int g=0; g<cantidades.size(); g++){
		sens_x_group[g] = cantidades[g];
	}

	int_vector<> temp_ids_sens(num_sensores);
	for(int i=0; i<ids.size(); i++){
		temp_ids_sens[i] = ids[i]-min_id;
	}
	ids_sens = vlc_vector<coder::fibonacci>(temp_ids_sens);

	// Codificación con serie semana de referencia por grupo

	weeks = vector<vlc_vector<coder::fibonacci>>(5);

	for(int g=0; g<valores.size(); g++){
		int_vector<> semana(num_times_x_week);
		for(int s=0; s<valores[g].size(); s++){
			for(int k=0; k<muestras; k++){
				semana[k%num_times_x_week] += valores[g][s][k];
			}
		}
		for(int w=0; w<num_times_x_week; w++){
			semana[w] = int(semana[w] / (valores[g].size() * weeksXyear));
		}
		weeks[g] = vlc_vector<coder::fibonacci>(semana);
		
		for(int s=0; s<valores[g].size(); s++){
			int_vector<> ivSerie(timesXyear);
			for(int k=0; k<muestras; k++){
				aux = semana[k%num_times_x_week] - valores[g][s][k];
				ivSerie[k] = zigzag_encode(aux);
			}
			vlc_vector<coder::fibonacci> vlcSerie(ivSerie);
			series.push_back(vlcSerie);
		}
	}
}

TempSeriesSensoresMadridWeek::TempSeriesSensoresMadrid(string inputFilename){
	cout << "Cargando archivo " << inputFilename << endl;
	ifstream infile(inputFilename, ofstream::binary);
	if(!infile){
		cout << "Error en la carga!" << endl;
		return;
	}    
	int aux;
	// --------- Cargando valores enteros
	infile.read((char *)&num_sensores, sizeof(int));
	infile.read((char *)&num_groups, sizeof(int));
	infile.read((char *)&num_muestras, sizeof(int));
	infile.read((char *)&num_times_x_week, sizeof(int));
	infile.read((char *)&min_value, sizeof(int));
	infile.read((char *)&max_value, sizeof(int));
	infile.read((char *)&min_id, sizeof(int));

	sens_x_group = vector<int>(num_groups);
	for(int i=0; i<num_groups; i++){
		infile.read((char *)&aux, sizeof(int));
		sens_x_group[i] = aux;
	}

	ids_sens.load(infile);

	weeks = vector<vlc_vector<coder::fibonacci>>(num_groups);
	for(int i=0; i<num_groups; i++){
		weeks[i].load(infile);
	}

	series = vector<vlc_vector<coder::fibonacci>>(num_sensores);
	for(int i=0; i<num_sensores; i++){
		series[i].load(infile);
	}

	// Cerrando archivo
	infile.close();
	cout << "Archivo " << inputFilename << " cargado exitosamente." << endl;
}

bool TempSeriesSensoresMadridWeek::save(string outputFilename){
	cout << "Guardando archivo " << outputFilename << endl;
	ofstream outfile(outputFilename, ofstream::binary);
	if(!outfile){
		cout << "Error en la carga!" << endl;
		return false;
	}   
	int aux;
	// Guardando valores enteros
	outfile.write((char const*)&num_sensores, sizeof(int));
	outfile.write((char const*)&num_groups, sizeof(int));
	outfile.write((char const*)&num_muestras, sizeof(int));
	outfile.write((char const*)&num_times_x_week, sizeof(int));
	outfile.write((char const*)&min_value, sizeof(int));
	outfile.write((char const*)&max_value, sizeof(int));
	outfile.write((char const*)&min_id, sizeof(int));

	for(int i=0; i<num_groups; i++){
		aux = sens_x_group[i];
		outfile.write((char const*)&aux, sizeof(int));
	}

	ids_sens.serialize(outfile);

	for(int i=0; i<num_groups; i++){
		weeks[i].serialize(outfile);
	}

	for(int i=0; i<num_sensores; i++){
		series[i].serialize(outfile);
	}

	// Cerrando archivo
	outfile.close();
	cout << "Archivo " << outputFilename << " creado exitosamente." << endl;
	return true;
}

int TempSeriesSensoresMadridWeek::size_bytes(){
	int bytes = 0;
	int auxBytes = 0;
	
	auxBytes += (sizeof(unsigned int) * 7);		// ints
	auxBytes += (sizeof(int) * sens_x_group.size());
	
	cout << "Enteros:\t" << auxBytes << " [Bytes]" << endl;
	
	bytes += auxBytes;
	auxBytes = 0;

	auxBytes += size_in_bytes(ids_sens);

	cout << "ids_sens:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;
	auxBytes = 0;

	for(int i=0; i<weeks.size(); i++){
		auxBytes += size_in_bytes(weeks[i]);
	}

	cout << "weeks:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;
	auxBytes = 0;

	for(int i=0; i<series.size(); i++){
		auxBytes += size_in_bytes(series[i]);
	}

	cout << "series:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;

	return bytes;
}

int TempSeriesSensoresMadridWeek::size_kbytes(){
	int kbytes = size_bytes() / 1024;
	return kbytes;
}

int TempSeriesSensoresMadridWeek::size_mbytes(){
	int mbytes = size_bytes() / 1024 / 1024;
	return mbytes;
}

void TempSeriesSensoresMadridWeek::stats(bool fullstats=false){
	cout << "rango de valores: " << min_value << " - " << max_value << endl;
	cout << "cantidad de sensores: " << num_sensores << endl;
	cout << "cantidad de muestras: " << num_muestras << endl;
	cout << "cantidad de grupos: " << num_groups << "[";
	for(int i=0; i<num_groups; i++){
		cout << " - " << sens_x_group[i];
	}
	cout << "]" << endl;
	cout << "min_id: " << min_id << endl;

	cout << " --------- Arreglos de Valores --------- " << endl;
	cout << "|ids_sens|: " << ids_sens.size() << " - " << statsEV(ids_sens) << endl;
	cout << "|weeks|: " << weeks.size() << endl;
	if(fullstats){
		for(int i=0; i<weeks.size(); i++){
			cout << "G_" << i << ": " << weeks[i].size() << " - " << statsEV(weeks[i]) << endl;
		}
	}
	cout << "|series|: " << series.size() << endl;
	if (fullstats){
		for(int i=0; i<series.size(); i++){
			cout << "G_" << i << ": " << series[i].size() << " - " << statsEV(series[i]) << endl;
		}
	}
}

void TempSeriesSensoresMadridWeek::print(){

	cout << "Mostrando los datos: " << endl;

}

/***********************************************************
						QUERIES
***********************************************************/



/***********************************************************
						PRIVATE
***********************************************************/

void TempSeriesSensoresMadridWeek::buildRanksSelects(){
	cout << "buildRanksSelects" << endl;
}

unsigned int TempSeriesSensoresMadridWeek::zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int TempSeriesSensoresMadridWeek::zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

bool TempSeriesSensoresMadridWeek::esFija(vector<int> serie){
	for(int j=1; j<serie.size(); j++){
		if(serie[j] != serie[j-1]){
			return false;
		}
	}
	return true;
}

void TempSeriesSensoresMadridWeek::encuentraLimites(vector<vector<vector<int>>>&valores){

	min_value = valores[0][0][0];
	max_value = valores[0][0][0];

	for(int i=0; i<valores.size(); i++){
		for(int j=0; j<valores[i].size(); j++){
			for(int k=0; k<valores[i][j].size(); k++){
				if(valores[i][j][k] < min_value){
					min_value = valores[i][j][k];
				}
				if(valores[i][j][k] > max_value){
					max_value = valores[i][j][k];
				}
			}
		}
	}
}

string TempSeriesSensoresMadridWeek::statsEV(vlc_vector<> x){
	string r = "[]";
	if(x.size() == 0){
		return r;
	}
	int mmin = x[0];
	int mmax = x[0];
	int acum = 0;
	for(int i=0; i<x.size(); i++){
		acum += x[i];
		if(x[i] < mmin){
			mmin = x[i];
		}
		if(x[i] > mmax){
			mmax = x[i];
		}
	}
	r = "[min: " + to_string(mmin) + " - max: " + to_string(mmax) + " - ";
	double prom = (0.0 + acum) / x.size();
	r += "prom: " + to_string(prom) + "]";
	return r;
}