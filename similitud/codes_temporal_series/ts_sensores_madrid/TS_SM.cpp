
#include "TS_SM.hpp"

TempSeriesSensoresMadrid::TempSeriesSensoresMadrid(vector<vector<vector<int>>>&valores, vector<int>&cantidades, vector<int>&ids, int kValue, int muestras){
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
	sens_x_group = vector<int>(num_groups);
	refs_of_group = vector<int>(num_groups);
	k = kValue;
	int acum_refs = 0;
	for(int g=0; g<cantidades.size(); g++){
		sens_x_group[g] = cantidades[g];
		refs_of_group[g] = acum_refs;
		aux = cantidades[g] / k;
		if(cantidades[g] % k != 0){
			aux++;
		}
		acum_refs += aux;
	}
	int_vector<> ivTempPGFV(acum_refs-aux);
	pgReference = vector<vlc_vector<coder::fibonacci>>(acum_refs-aux);
	cout << "pgReference: " << pgReference.size() << " - acum_refs: " << acum_refs << endl;

	cout << "Pos iniciales de refs de cada grupo: " << endl;
	for(int i=0; i<refs_of_group.size(); i++){
		cout << refs_of_group[i] << "\t";
	}
	cout << endl;

	encuentraLimites(valores);

	int posPGR = 0;
	bool serieRef = false;
	vector<int> serieReferenciaTemporal;
	for(int i=0; i<num_groups-1; i++){
		//	Recorrido en los grupos principales
		for(int j=0; j<sens_x_group[i]; j++){
			//	Recorrido en las series del grupo 'i'
			serieRef = (j % k == 0);
			if(serieRef){
				//	Procesando una serie de referencia
				serieReferenciaTemporal = valores[i][j];
				ivTempPGFV[posPGR] = serieReferenciaTemporal[0] - min_value;
				int_vector<> ivT(num_muestras-1);
				for(int k=1; k<num_muestras; k++){
					ivT[k-1] = zigzag_encode(serieReferenciaTemporal[k] - serieReferenciaTemporal[k-1]);
				}
				pgReference[posPGR] = vlc_vector<coder::fibonacci>(ivT);
				posPGR++;
			}else{
				//	Procesando una serie normal
				int_vector<> ivT(num_muestras);
				for(int k=0; k<num_muestras; k++){
					ivT[k] = zigzag_encode(valores[i][j][k] - serieReferenciaTemporal[k]);
				}
				pgSeries.push_back(vlc_vector<coder::fibonacci>(ivT));
			}
			serieRef = false;
		}
	}

	int lastGroup = num_groups - 1;
	int sens_lg = sens_x_group[lastGroup];
	int_vector<> ivTempLGFV(sens_lg);
	// Recorrido para el último grupo
	for(int j=0; j<sens_lg; j++){
		ivTempLGFV[j] = valores[lastGroup][j][0] - min_value;
		int_vector<> ivT(num_muestras-1);
		for(int k=1; k<num_muestras; k++){
			ivT[k-1] = zigzag_encode(valores[lastGroup][j][k] - valores[lastGroup][j][k-1]);
		}
		lgSeries.push_back(vlc_vector<coder::fibonacci>(ivT));
	}	
	pgFirstValue = vlc_vector<coder::fibonacci>(ivTempPGFV);
	lgFirstValue = vlc_vector<coder::fibonacci>(ivTempLGFV);

}

TempSeriesSensoresMadrid::TempSeriesSensoresMadrid(vector<vector<vector<int>>>&valores, vector<int>&cantidades, vector<int>&ids, int kValue, int muestras, bool todosIgual){
	/*
		Variante que codifica las series con respecto a la inmediatamente anterior.
			bool todosIgual: se usa para saber si todos los grupos se codifican igual,
			o si el último grupo mantiene su configuración de la versión tradicional
	*/
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
	sens_x_group = vector<int>(num_groups);
	refs_of_group = vector<int>(num_groups);
	k = kValue;
	int acum_refs = 0;
	for(int g=0; g<cantidades.size(); g++){
		sens_x_group[g] = cantidades[g];
	}
	pgFirstValue = int_vector<>(0);
	pgReference = vector<vlc_vector<coder::fibonacci>>(0);
	encuentraLimites(valores);

	int gruposIguales = num_groups-1;
	int sensoresIguales = num_sensores - sens_x_group[num_groups-1];
	if(todosIgual){
		gruposIguales = num_groups;
		sensoresIguales = num_sensores;
		lgFirstValue = vlc_vector<coder::fibonacci>(int_vector<>(0));
		lgSeries = vector<vlc_vector<coder::fibonacci>>();
	}
	pgSeries = vector<vlc_vector<coder::fibonacci>>(sensoresIguales);

	int posSeries = 0;
	for(int i=0; i<gruposIguales; i++){
		cout << "posSeries: " << posSeries << "/" << sensoresIguales << endl;
		//	Recorrido en los grupos principales
		//	Tratamiento para la primera serie del grupo
		int_vector<> ivT(num_muestras);
		ivT[0] = valores[i][0][0] - min_value;
		for(int k=1; k<num_muestras; k++){
			ivT[k] = zigzag_encode(valores[i][0][k] - valores[i][0][k-1]);

		}
		pgSeries[posSeries] = vlc_vector<coder::fibonacci>(ivT);
		posSeries++;
		//	Tratamiento para las demás series del grupo
		for(int j=1; j<sens_x_group[i]; j++){
//			cout << "posSeries: " << posSeries << "/" << sensoresIguales << endl;
			ivT = int_vector<>(num_muestras);
			for(int k=0; k<num_muestras; k++){
				ivT[k] = zigzag_encode(valores[i][j][k] - valores[i][j-1][k]);
			}
			pgSeries[posSeries] = vlc_vector<coder::fibonacci>(ivT);
			posSeries++;
		}
	}

	cout << "posSeries: " << posSeries << " - sensoresIguales: " << sensoresIguales << endl;

	if(!todosIgual){
		int lastGroup = num_groups - 1;
		int sens_lg = sens_x_group[lastGroup];
		int_vector<> ivTempLGFV(sens_lg);
		// Recorrido para el último grupo
		for(int j=0; j<sens_lg; j++){
			ivTempLGFV[j] = valores[lastGroup][j][0] - min_value;
			int_vector<> ivT(num_muestras-1);
			for(int k=1; k<num_muestras; k++){
				ivT[k-1] = zigzag_encode(valores[lastGroup][j][k] - valores[lastGroup][j][k-1]);
			}
			util::bit_compress(ivT);
			lgSeries.push_back(ivT);
		}
		lgFirstValue = vlc_vector<coder::fibonacci>(ivTempLGFV);
	}

}

TempSeriesSensoresMadrid::TempSeriesSensoresMadrid(vector<vector<vector<int>>>&valores, vector<int>&cantidades, vector<int>&ids, int muestras){
	/*
		Variante que codifica las series con respecto a serie promedio de cada grupo.
	*/
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
	sens_x_group = vector<int>(num_groups);
	refs_of_group = vector<int>(0);
	k = 0;
	for(int g=0; g<cantidades.size(); g++){
		sens_x_group[g] = cantidades[g];
	}
	pgFirstValue = vlc_vector<coder::fibonacci>(int_vector<>(0));
	lgFirstValue = vlc_vector<coder::fibonacci>(int_vector<>(0));
	lgSeries = vector<vlc_vector<coder::fibonacci>>(0);

	pgReference = vector<vlc_vector<coder::fibonacci>>(num_groups);
	pgSeries = vector<vlc_vector<coder::fibonacci>>(num_sensores);
	encuentraLimites(valores);

	int iaux = 0;

	for(int i=0; i<num_groups; i++){
		// Construir la secuencia de referencia
		int_vector<> ivR(num_muestras);
		long long int acum;
		for(int j=0; j<num_muestras; j++){
			acum = 0;
			for(int k=0; k<sens_x_group[i]; k++){
				acum += valores[i][k][j];
			}
			acum /= sens_x_group[i];
			ivR[j] = acum;
		}
		
		//	Codificar las demás secuencias según la referencia
		for(int j=0; j<sens_x_group[i]; j++){
//			cout << "posSeries: " << posSeries << "/" << sensoresIguales << endl;
			int_vector<> ivT(num_muestras);
			for(int k=0; k<num_muestras; k++){
				ivT[k] = zigzag_encode(valores[i][j][k] - ivR[k]);
			}
			pgSeries[iaux++] = vlc_vector<coder::fibonacci>(ivT);
		}

		//	Reducir rango de la serie de referencia
		int mm = ivR[0];
		for(int k=1; k<num_muestras; k++){
			if(ivR[k] < mm){
				mm = ivR[k];
			}
		}
		for(int k=0; k<num_muestras; k++){
			ivR[k] = ivR[k] - mm;
		}
		pgReference[i] = vlc_vector<coder::fibonacci>(ivR);
	}

	cout << "iaux: " << iaux << " - num_sensores: " << num_sensores << endl;

}

TempSeriesSensoresMadrid::TempSeriesSensoresMadrid(string inputFilename){
	cout << "Cargando archivo " << inputFilename << endl;
	ifstream infile(inputFilename, ofstream::binary);
	if(!infile){
		cout << "Error en la carga!" << endl;
		return;
	}    
	int aux1, aux2;
	// --------- Cargando valores enteros
	infile.read((char *)&num_sensores, sizeof(int));
	infile.read((char *)&num_groups, sizeof(int));
	infile.read((char *)&num_muestras, sizeof(int));
	infile.read((char *)&k, sizeof(int));
	infile.read((char *)&min_value, sizeof(int));	
	infile.read((char *)&max_value, sizeof(int));
	// --------- Cargando vectores
	infile.read((char *)&aux1, sizeof(int));
	sens_x_group = vector<int>(aux1);
	for(int i=0; i<aux1; i++){
		infile.read((char *)&aux2, sizeof(int));
		sens_x_group[i] = aux2;
	}
	infile.read((char *)&aux1, sizeof(int));
	refs_of_group = vector<int>(aux1);
	for(int i=0; i<aux1; i++){
		infile.read((char *)&aux2, sizeof(int));
		refs_of_group[i] = aux2;
	}
	// --------- Cargando vectores de int_vectors
	vlc_vector<coder::fibonacci> temporalVLC;
	
	pgFirstValue.load(infile);
	
	infile.read((char *)&aux1,sizeof(int));
	pgReference = vector<vlc_vector<coder::fibonacci>>(aux1);
	for(int i=0; i<aux1; i++){
		temporalVLC.load(infile);
		pgReference[i] = temporalVLC;
	}
	
	infile.read((char *)&aux1,sizeof(int));
	pgSeries = vector<vlc_vector<coder::fibonacci>>(aux1);
	for(int i=0; i<aux1; i++){
		temporalVLC.load(infile);
		pgSeries[i] = temporalVLC;
	}
	
	lgFirstValue.load(infile);
	
	infile.read((char *)&aux1,sizeof(int));
	lgSeries = vector<vlc_vector<coder::fibonacci>>(aux1);
	for(int i=0; i<aux1; i++){
		temporalVLC.load(infile);
		lgSeries[i] = temporalVLC;
	}

	// Cerrando archivo
	infile.close();
	cout << "Archivo " << inputFilename << " cargado exitosamente." << endl;
}

bool TempSeriesSensoresMadrid::save(string outputFilename){
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
	outfile.write((char const*)&k, sizeof(int));
	outfile.write((char const*)&min_value, sizeof(int));
	outfile.write((char const*)&max_value, sizeof(int));
	// Guardando vectores
	aux = sens_x_group.size();
	outfile.write((char const*)&aux, sizeof(int));
	for(int i=0; i<sens_x_group.size(); i++){
		aux = sens_x_group[i];
		outfile.write((char const*)&aux, sizeof(int));
	}
	aux = refs_of_group.size();
	outfile.write((char const*)&aux, sizeof(int));
	for(int i=0; i<refs_of_group.size(); i++){
		aux = refs_of_group[i];
		outfile.write((char const*)&aux, sizeof(int));
	}
	// Guardando vlc_vectors y vectores de vlc_vector
	pgFirstValue.serialize(outfile);
	
	aux = pgReference.size();
	outfile.write((char const*)&aux, sizeof(int));
	for(int i=0; i<pgReference.size(); i++){
		pgReference[i].serialize(outfile);
	}
	
	aux = pgSeries.size();
	outfile.write((char const*)&aux, sizeof(int));
	for(int i=0; i<pgSeries.size(); i++){
		pgSeries[i].serialize(outfile);
	}
	
	lgFirstValue.serialize(outfile);
	
	aux = lgSeries.size();
	outfile.write((char const*)&aux, sizeof(int));
	for(int i=0; i<lgSeries.size(); i++){
		lgSeries[i].serialize(outfile);
	}

	// Cerrando archivo
	outfile.close();
	cout << "Archivo " << outputFilename << " creado exitosamente." << endl;
	return true;
}

int TempSeriesSensoresMadrid::size_bytes(){
	int bytes = 0;
	int auxBytes = 0;
	
	auxBytes += (sizeof(unsigned int) * 4);
	auxBytes += (sizeof(int) * 2);
	auxBytes += (sizeof(int) * sens_x_group.size());
	auxBytes += (sizeof(int) * refs_of_group.size());

	cout << "Enteros:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;
	auxBytes = 0;

	auxBytes += size_in_bytes(pgFirstValue);

	cout << "pgFirst:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;
	auxBytes = 0;

	auxBytes += size_in_bytes(lgFirstValue);

	cout << "lgFirst:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;
	auxBytes = 0;

	for(int i=0; i<pgReference.size(); i++){
		auxBytes += size_in_bytes(pgReference[i]);
	}

	cout << "Referencias:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;
	auxBytes = 0;

	for(int i=0; i<pgSeries.size(); i++){
		auxBytes += size_in_bytes(pgSeries[i]);
	}

	cout << "pgseries:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;
	auxBytes = 0;

	for(int i=0; i<lgSeries.size(); i++){
		auxBytes += size_in_bytes(lgSeries[i]);
	}
	
	cout << "lgSeries:\t" << auxBytes << " [Bytes]" << endl;
	bytes += auxBytes;
	auxBytes = 0;

	return bytes;
}

int TempSeriesSensoresMadrid::size_kbytes(){
	int kbytes = size_bytes() / 1024;
	return kbytes;
}

int TempSeriesSensoresMadrid::size_mbytes(){
	int mbytes = size_bytes() / 1024 / 1024;
	return mbytes;
}

void TempSeriesSensoresMadrid::stats(){

	cout << "rango de valores: " << min_value << " - " << max_value << endl;
	cout << "cantidad de sensores: " << num_sensores << endl;
	cout << "cantidad de muestras: " << num_muestras << endl;
	cout << "cantidad de grupos: " << num_groups << "[";
	for(int i=0; i<num_groups; i++){
		cout << " - " << sens_x_group[i];
	}
	cout << "]" << endl;
	cout << "K: " << k << endl;
	cout << "Referencias por grupo: [";
	for(int i=0; i<refs_of_group.size(); i++){
		cout << " - " << refs_of_group[i];
	}
	cout << "]" << endl;
	cout << " --------- Arreglos de Valores --------- " << endl;
	cout << "|pgFirstValue|: " << pgFirstValue.size() << " - " << statsEV(pgFirstValue) << endl;
	cout << "|pgReference|: " << pgReference.size() << endl;
	for(int i=0; i<pgReference.size(); i++){
		cout << "G_" << i << ": " << pgReference[i].size() << " - " << statsEV(pgReference[i]) << endl;
	}
	cout << "|pgSeries|: " << pgSeries.size() << endl;
	for(int i=0; i<pgSeries.size(); i++){
		cout << "G_" << i << ": " << pgSeries[i].size() << " - " << statsEV(pgSeries[i]) << endl;
	}
	cout << "|lgFirstValue|: " << lgFirstValue.size() << " - " << statsEV(lgFirstValue) << endl;
	cout << "|lgSeries|: " << lgSeries.size() << endl;
	for(int i=0; i<lgSeries.size(); i++){
		cout << "G_" << i << ": " << lgSeries[i].size() << " - " << statsEV(lgSeries[i]) << endl;
	}

}

void TempSeriesSensoresMadrid::print(){

	cout << "Mostrando los datos: " << endl;

}

/***********************************************************
						QUERIES
***********************************************************/



/***********************************************************
						PRIVATE
***********************************************************/

void TempSeriesSensoresMadrid::buildRanksSelects(){
	cout << "buildRanksSelects" << endl;
}

unsigned int TempSeriesSensoresMadrid::zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int TempSeriesSensoresMadrid::zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

bool TempSeriesSensoresMadrid::esFija(vector<int> serie){
	for(int j=1; j<serie.size(); j++){
		if(serie[j] != serie[j-1]){
			return false;
		}
	}
	return true;
}

void TempSeriesSensoresMadrid::encuentraLimites(vector<vector<vector<int>>>&valores){

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

string TempSeriesSensoresMadrid::statsEV(vlc_vector<> x){
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