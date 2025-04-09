
#include "TS_SM_factor.hpp"

TempSeriesSensoresMadridFactor::TempSeriesSensoresMadridFactor(vector<vector<vector<int>>>&valores, vector<int>&cantidades, vector<int>&ids, int muestras, bool prom){
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

	// Codificación con serie promedio de referencia por grupo

	refs = vector<vlc_vector<coder::fibonacci>>(num_groups);

	for(int g=0; g<valores.size(); g++){
		int_vector<> referencia(num_muestras);
		if(prom){
			//	Referencia es el promedio del grupo
			for(int s=0; s<valores[g].size(); s++){
				for(int k=0; k<muestras; k++){
					referencia[k] += valores[g][s][k];
				}
			}
			for(int k=0; k<muestras; k++){
				referencia[k] = int(referencia[k] / valores[g].size());
			}
		}else{
			//	Referencia es el valor menor de cada muestra del grupo
			for(int k=0; k<muestras; k++){
				referencia[k] = valores[g][0][k];
				for(int s=1; s<valores[g].size(); s++){
					if(valores[g][s][k] < referencia[k]){
						referencia[k] = valores[g][s][k];
					}
				}
			}
		}
		
		refs[g] = vlc_vector<coder::fibonacci>(referencia);
		long long int acumRef = 0;
		for(int k=0; k<muestras; k++){
			acumRef += referencia[k];
		}
		cout << "Ref++: " << acumRef << endl;
		int_vector<> ivFact(num_sensores);
		int fp = 0;
		for(int s=0; s<valores[g].size(); s++){
			long long int acumSerie = 0;
			for(int k=0; k<muestras; k++){
				acumSerie += valores[g][s][k];
			}
			cout << "\tSerie++: " << acumRef << endl;
			int fact = int(acumSerie / acumRef);
			ivFact[fp++] = fact;

			int_vector<> ivSerie(muestras);
			for(int k=0; k<muestras; k++){
				aux = valores[g][s][k] - (referencia[k] * fact);
				ivSerie[k] = zigzag_encode(aux);
			}
			vlc_vector<coder::fibonacci> vlcSerie(ivSerie);
			series.push_back(vlcSerie);
		}
		factores = vlc_vector<coder::fibonacci>(ivFact);
	}
}

TempSeriesSensoresMadridFactor::TempSeriesSensoresMadridFactor(string inputFilename){
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
	infile.read((char *)&min_value, sizeof(int));
	infile.read((char *)&max_value, sizeof(int));
	infile.read((char *)&min_id, sizeof(int));

	sens_x_group = vector<int>(num_groups);
	for(int i=0; i<num_groups; i++){
		infile.read((char *)&aux, sizeof(int));
		sens_x_group[i] = aux;
	}

	ids_sens.load(infile);

	refs = vector<vlc_vector<coder::fibonacci>>(num_groups);
	for(int i=0; i<num_groups; i++){
		refs[i].load(infile);
	}

	series = vector<vlc_vector<coder::fibonacci>>(num_sensores);
	for(int i=0; i<num_sensores; i++){
		series[i].load(infile);
	}

	factores.load(infile);

	// Cerrando archivo
	infile.close();
	cout << "Archivo " << inputFilename << " cargado exitosamente." << endl;
}

vector<int> TempSeriesSensoresMadridFactor::getSerieByID(int idq){
	vector<int> res;
	if(idq < min_id){
		cout << "ID [" << idq << "] no encontrada." << endl;
		return res;
	}
	// Encontrar la ubicación del ID consultado
	int pos = 0;
	while(pos < num_sensores && ids_sens[pos] != (idq-min_id)){
		pos++;
	}
	if(pos >= num_sensores){
		cout << "ID [" << idq << "] no encontrada." << endl;
		return res;
	}
// cout << "Serie ubicada en la posición " << pos << endl;
	// Encontrar el grupo que le corresponde
	int nGroup = 0;
	int acumG = sens_x_group[nGroup];
	while(nGroup < num_groups && pos >= acumG){
		nGroup++;
		acumG += sens_x_group[nGroup];
	}
	if(nGroup >= num_groups){
		cout << "Error! no se encontró el grupo correspondiente." << endl;
		return res;
	}
// cout << "Serie corresponde a grupo " << nGroup << endl;

	res = vector<int>(num_muestras);
	int aux;
	for(int i=0; i<num_muestras; i++){
		aux = zigzag_decode(series[pos][i]);
		aux += factores[pos]*refs[nGroup][i];
		res[i] = aux;
	}
	return res;
}

bool TempSeriesSensoresMadridFactor::save(string outputFilename){
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
	outfile.write((char const*)&min_value, sizeof(int));
	outfile.write((char const*)&max_value, sizeof(int));
	outfile.write((char const*)&min_id, sizeof(int));

	for(int i=0; i<num_groups; i++){
		aux = sens_x_group[i];	
		outfile.write((char const*)&aux, sizeof(int));
	}

	ids_sens.serialize(outfile);

	for(int i=0; i<num_groups; i++){
		refs[i].serialize(outfile);
	}

	for(int i=0; i<num_sensores; i++){
		series[i].serialize(outfile);
	}

	factores.serialize(outfile);

	// Cerrando archivo
	outfile.close();
	cout << "Archivo " << outputFilename << " creado exitosamente." << endl;
	return true;
}

int TempSeriesSensoresMadridFactor::size_bytes(){
	int bytes = 0;
	int auxBytes = 0;
	
	auxBytes += (sizeof(unsigned int) * 6);		// ints
	auxBytes += (sizeof(int) * sens_x_group.size());
	
	cout << "Enteros:\t" << auxBytes << " [Bytes]" << endl;
	
	bytes += auxBytes;
	auxBytes = 0;

	auxBytes += size_in_bytes(ids_sens);

	cout << "ids_sens:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;
	auxBytes = 0;

	for(int i=0; i<refs.size(); i++){
		auxBytes += size_in_bytes(refs[i]);
	}

	cout << "refs:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;
	auxBytes = 0;

	for(int i=0; i<series.size(); i++){
		auxBytes += size_in_bytes(series[i]);
	}

	cout << "series:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;
	auxBytes = 0;

	auxBytes += size_in_bytes(factores);

	cout << "factores:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;

	return bytes;
}

int TempSeriesSensoresMadridFactor::size_kbytes(){
	int kbytes = size_bytes() / 1024;
	return kbytes;
}

int TempSeriesSensoresMadridFactor::size_mbytes(){
	int mbytes = size_bytes() / 1024 / 1024;
	return mbytes;
}

void TempSeriesSensoresMadridFactor::stats(bool fullstats){
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
	cout << "|refs|: " << refs.size() << endl;
	if(fullstats){
		for(int i=0; i<refs.size(); i++){
			cout << "G_" << i << ": " << refs[i].size() << " - " << statsEV(refs[i]) << endl;
		}
	}
	cout << "|series|: " << series.size() << endl;
	if (fullstats){
		for(int i=0; i<series.size(); i++){
			cout << "G_" << i << ": " << series[i].size() << " - " << statsEV(series[i]) << endl;
		}
	}
}

void TempSeriesSensoresMadridFactor::print(){

	cout << "Mostrando los datos: " << endl;

}

/***********************************************************
						QUERIES
***********************************************************/



/***********************************************************
						PRIVATE
***********************************************************/

void TempSeriesSensoresMadridFactor::buildRanksSelects(){
	cout << "buildRanksSelects" << endl;
}

unsigned int TempSeriesSensoresMadridFactor::zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int TempSeriesSensoresMadridFactor::zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

bool TempSeriesSensoresMadridFactor::esFija(vector<int> serie){
	for(int j=1; j<serie.size(); j++){
		if(serie[j] != serie[j-1]){
			return false;
		}
	}
	return true;
}

void TempSeriesSensoresMadridFactor::encuentraLimites(vector<vector<vector<int>>>&valores){

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

string TempSeriesSensoresMadridFactor::statsEV(vlc_vector<> x){
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