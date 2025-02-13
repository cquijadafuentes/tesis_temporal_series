
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
	pgFirstValue = int_vector<>(acum_refs);
	pgReference = vector<int_vector<>>(acum_refs);

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
				pgFirstValue[posPGR] = serieReferenciaTemporal[0] - min_value;
				int_vector<> ivT(num_muestras-1);
				for(int k=1; k<num_muestras; k++){
					ivT[k-1] = zigzag_encode(serieReferenciaTemporal[k] - serieReferenciaTemporal[k-1]);
				}
				util::bit_compress(ivT);
				pgReference[posPGR] = ivT;
				posPGR++;
			}else{
				//	Procesando una serie normal
				int_vector<> ivT(num_muestras);
				for(int k=0; k<num_muestras; k++){
					ivT[k] = zigzag_encode(valores[i][j][k] - serieReferenciaTemporal[k]);
				}
				util::bit_compress(ivT);
				pgSeries.push_back(ivT);
			}
			serieRef = false;
		}
	}
	util::bit_compress(pgFirstValue);

	int lastGroup = num_groups - 1;
	int sens_lg = sens_x_group[lastGroup];
	lgFirstValue = int_vector<>(sens_lg);
	// Recorrido para el último grupo
	for(int j=0; j<sens_lg; j++){
		lgFirstValue[j] = valores[lastGroup][j][0] - min_value;
		int_vector<> ivT(num_muestras-1);
		for(int k=1; k<num_muestras; k++){
			ivT[k-1] = zigzag_encode(valores[lastGroup][j][k] - valores[lastGroup][j][k-1]);
		}
		util::bit_compress(ivT);
		lgSeries.push_back(ivT);
	}
	util::bit_compress(lgFirstValue);

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
	pgReference = vector<int_vector<>>(0);
	encuentraLimites(valores);

	int gruposIguales = num_groups-1;
	int sensoresIguales = num_sensores - sens_x_group[num_groups-1];
	if(todosIgual){
		gruposIguales = num_groups;
		sensoresIguales = num_sensores;
		lgFirstValue = int_vector<>(0);
		lgSeries = vector<int_vector<>>(0);
	}
	pgSeries = vector<int_vector<>>(sensoresIguales);

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
		util::bit_compress(ivT);
		pgSeries[posSeries] = ivT;
		posSeries++;
		//	Tratamiento para las demás series del grupo
		for(int j=1; j<sens_x_group[i]; j++){
//			cout << "posSeries: " << posSeries << "/" << sensoresIguales << endl;
			ivT = int_vector<>(num_muestras);
			for(int k=0; k<num_muestras; k++){
				ivT[k] = zigzag_encode(valores[i][j][k] - valores[i][j-1][k]);
			}
			util::bit_compress(ivT);
			pgSeries[posSeries] = ivT;
			posSeries++;
		}
	}

	cout << "posSeries: " << posSeries << " - sensoresIguales: " << sensoresIguales << endl;

	if(!todosIgual){
		int lastGroup = num_groups - 1;
		int sens_lg = sens_x_group[lastGroup];
		lgFirstValue = int_vector<>(sens_lg);
		// Recorrido para el último grupo
		for(int j=0; j<sens_lg; j++){
			lgFirstValue[j] = valores[lastGroup][j][0] - min_value;
			int_vector<> ivT(num_muestras-1);
			for(int k=1; k<num_muestras; k++){
				ivT[k-1] = zigzag_encode(valores[lastGroup][j][k] - valores[lastGroup][j][k-1]);
			}
			util::bit_compress(ivT);
			lgSeries.push_back(ivT);
		}
		util::bit_compress(lgFirstValue);
	}

}

TempSeriesSensoresMadrid::TempSeriesSensoresMadrid(string inputFilename){
	cout << "Cargando archivo " << inputFilename << endl;
}

bool TempSeriesSensoresMadrid::save(string outputFilename){
	cout << "Guardando archivo " << outputFilename << endl;
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