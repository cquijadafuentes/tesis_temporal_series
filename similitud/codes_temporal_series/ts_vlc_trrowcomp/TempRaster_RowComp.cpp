
#include "TempRaster_RowComp.hpp"


TempRasterRowComp::TempRasterRowComp(vector<vector<vector<int>>> &tseries, int dims){
	n_rows = tseries.size();			// filas
	n_cols = tseries[0].size();		// columnas
	n_inst = tseries[0][0].size();	// length of temporal series
	min_value = tseries[0][0][0];
	max_value = tseries[0][0][0];
	for(int i=0; i<n_rows; i++){
		for(int j=0; j<n_cols; j++){
			for(int t=0; t<n_inst; t++){
				if(tseries[i][j][t] < min_value){
					min_value = tseries[i][j][t];
				}
				if(tseries[i][j][t] > max_value){
					max_value = tseries[i][j][t];
				}
			}
		}
	}
	cout << "min_value: " << min_value << " - max_value: " << max_value << endl;

	cout << "Construyendo ..." << endl;

	bit_vector bvSF(n_rows * n_cols);		// Marcar las series fijas
	int_vector<> valoresPVSR(n_rows);
	vector<int> valoresSF;	
	for(int f=0; f<n_rows; f++){		// fc = fila-cuadrante
		vector<int> serieReferencia = tseries[f][0];
		for(int t=0; t<n_inst; t++){
			serieReferencia[t] = tseries[f][0][t];
		}
		valoresPVSR[f] = zigzag_encode(tseries[f][0][0]);
		bvSF[n_cols*f] = 0;
		int val;
		int_vector<> ivaux(n_inst-1, 0);
		for(int k=1; k<n_inst; k++){
			ivaux[k-1] = zigzag_encode(tseries[f][0][k] - tseries[f][0][k-1]);
		}
		refs.push_back(vlc_vector<>(ivaux));
		for(int c=1; c<n_cols; c++){
			int iCelda = n_cols*f+c;
			if(esFija(tseries[f][c])){
				// La serie de tiempo es fija y no se representa
				valoresSF.push_back(zigzag_encode(tseries[f][c][0]));
				bvSF[iCelda] = 1;
			}else{
				int_vector<> ivaux2(n_inst, 0);
				for(int k=0; k<n_inst; k++){
					ivaux2[k] = zigzag_encode(serieReferencia[k] - tseries[f][c][k]);
				}
				series.push_back(vlc_vector<>(ivaux2));
				bvSF[iCelda] = 0;
			}
		}
	}

	int_vector<> ivaux(valoresSF.size());
	for(int i=0; i<ivaux.size(); i++){
		ivaux[i] = valoresSF[i] - min_value;
	}
	fixedValue = vlc_vector<>(ivaux);
	refFirstValue = vlc_vector<>(valoresPVSR);

	bvSeriesFijas = sd_vector<>(bvSF);

	rankSeriesFijas = sd_vector<>::rank_1_type(&bvSeriesFijas);

	cout << "Size in kBytes: " << size_kbytes() << endl;
}

TempRasterRowComp::TempRasterRowComp(string inputFilename){
	ifstream infile(inputFilename, ofstream::binary);
	if(!infile){
		cout << "Error en la carga!" << endl;
		return;
	}    
	int aux1, aux2;
	// --------- Cargando valores enteros
	infile.read((char *)&n_rows, sizeof(int));
	infile.read((char *)&n_cols, sizeof(int));
	infile.read((char *)&n_inst, sizeof(int));
	infile.read((char *)&min_value, sizeof(int));
	infile.read((char *)&max_value, sizeof(int));
	// --------- Cargando int_vectors
	fixedValue.load(infile);
	refFirstValue.load(infile);
	// --------- Cargando vlc vectors
	vlc_vector<> tempIV;
	infile.read((char *)&aux1,sizeof(int));
	refs = vector<vlc_vector<>>(aux1);
	for(int i=0; i<aux1; i++){
		tempIV.load(infile);
		refs[i] = tempIV;
	}
	infile.read((char *)&aux1,sizeof(int));
	series = vector<vlc_vector<>>(aux1);
	for(int i=0; i<aux1; i++){
		tempIV.load(infile);
		series[i] = tempIV;
	}
	// --------- Cargando bitvectors
	bvSeriesFijas.load(infile);
	// Cerrando archivo
	infile.close();
	// Construyendo rank y select de MFS
	rankSeriesFijas = sd_vector<>::rank_1_type(&bvSeriesFijas);

	cout << "Leído..." << endl;
}

bool TempRasterRowComp::save(string outputFilename){
	ofstream outfile(outputFilename, ofstream::binary);
	// Guardando valores enteros
	outfile.write((char const*)&n_rows, sizeof(int));
	outfile.write((char const*)&n_cols, sizeof(int));
	outfile.write((char const*)&n_inst, sizeof(int));
	outfile.write((char const*)&min_value, sizeof(int));
	outfile.write((char const*)&max_value, sizeof(int));
	// Guardando int_vectors
	fixedValue.serialize(outfile);
	refFirstValue.serialize(outfile);
	// Guardando vectores de int_vectors
	int aux;
	aux = refs.size();
	outfile.write((char const*)&aux, sizeof(int));
	for(int i=0; i<aux; i++){
		refs[i].serialize(outfile);
	}
	aux = series.size();
	outfile.write((char const*)&aux, sizeof(int));
	for(int i=0; i<aux; i++){
		series[i].serialize(outfile);
	}
	// Guardando bitvectors
	bvSeriesFijas.serialize(outfile);
	// Cerrando archivo
	outfile.close();
	return true;
}

int TempRasterRowComp::size_bytes(){
	int bytes = 0;
	// Atributos enteros
	bytes += (sizeof(int) * 5);
	// int_vectors
	bytes += size_in_bytes(fixedValue);
	bytes += size_in_bytes(refFirstValue);
	for(int i=0; i<refs.size(); i++){
		bytes += size_in_bytes(refs[i]);
	}
	for(int i=0; i<series.size(); i++){
		bytes += size_in_bytes(series[i]);
	}
	// bit_vectors
	bytes += size_in_bytes(bvSeriesFijas);
	return bytes;
}

int TempRasterRowComp::size_kbytes(){
	int kbytes = size_bytes() / 1024;
	return kbytes;
}

int TempRasterRowComp::size_mbytes(){
	int mbytes = size_bytes() / 1024 / 1024;
	return mbytes;
}

vector<int> TempRasterRowComp::getSerie(int row, int col){
	vector<int> r;
	// 1 - Verificar que no corresponde a una serie fija
	int linearPos = (row * n_cols) + col;
	if(bvSeriesFijas[linearPos] == 1){
		int p = rankSeriesFijas(linearPos);
		int v = fixedValue[p] + min_value;
		r = vector<int>(n_inst, v);
		return r;
	}
	// 2 - Recuperar la serie de referencia de la fila
	vector<int> ref = getRowReferenceSerie(row);
	if(col == 0){
		return ref;
	}
	// 3 - Si no es la referencia, recuperar la serie
	int posSerie = linearPos - row - rankSeriesFijas(linearPos);
	r = vector<int>(n_inst);
	for(int i=0; i<n_inst; i++){
		r[i] = ref[i] - zigzag_decode(series[posSerie][i]);
	}
	return r;
}

void TempRasterRowComp::print(){

	cout << "Mostrando los datos: " << endl;

	cout << "n_rows: " << n_rows << endl;
	cout << "n_cols: " << n_cols << endl;
	cout << "n_inst: " << n_inst << endl;
	cout << "min_value: " << min_value << endl;
	cout << "max_value: " << max_value << endl;

	cout << "fixedValue: [" << fixedValue.size() << "]:" << endl;
	for(int i=0; i<fixedValue.size(); i++){
		cout << "\t" << fixedValue[i];
	}
	cout << endl;
	for(int i=0; i<fixedValue.size(); i++){
		cout << "\t" << i%10;
	}
	cout << endl;

	cout << "refFirstValue: [" << refFirstValue.size() << "]:" << endl;
	for(int i=0; i<refFirstValue.size(); i++){
		cout << "\t" << refFirstValue[i];
	}
	cout << endl;
	for(int i=0; i<refFirstValue.size(); i++){
		cout << "\t" << i%10;
	}
	cout << endl;

	cout << "refs: ["  << refs.size() << "]:" << endl;
	for(int i=0; i<refs.size(); i++){
		cout << "\t" << i << "[" << refs[i].size() << "]:";
		for(int j=0; j<refs[i].size(); j++){
			cout << "\t" << refs[i][j];
		}
		cout << endl;
	}

	cout << "series: ["  << series.size() << "]:" << endl;
	for(int i=0; i<series.size(); i++){
		cout << "\t" << i << "[" << series[i].size() << "]:";
		for(int j=0; j<series[i].size(); j++){
			cout << "\t" << series[i][j];
		}
		cout << endl;
	}

	
	cout << "bvSeriesFijas [" << bvSeriesFijas.size() << "]:\n\t";
	for(int i=0; i<bvSeriesFijas.size(); i++){
		cout << bvSeriesFijas[i] << " ";
	}
	cout << "\n\t";
	for(int i=0; i<bvSeriesFijas.size(); i++){
		cout << i%10 << " ";
	}
	cout << endl;

}

/***********************************************************
						QUERIES
***********************************************************/

int TempRasterRowComp::queryAccess(int row, int col, int inst, bool mostrarQuery){
	vector<int> gs = getSerie(row, col);
	if(gs.size() > inst){
		return gs[inst];
	}
	return 0;
}

void swapValues(int &a, int &b){
	int x = a;
	a = b;
	b = x;
}

vector<vector<vector<int>>> TempRasterRowComp::queryWindow(int rowI, int rowF, int colI, int colF, int timeI, int timeF, bool mostrarQuery){
	// Validación de valores inicial y final
	if(rowI > rowF){
		swapValues(rowI, rowF);
	}
	if(colI > colF){
		swapValues(colI, colF);
	}
	if(timeI > timeF){
		swapValues(timeI, timeF);
	}
	if(mostrarQuery){
		cout << "Window Query: F [" << rowI << "," << rowF << "] - C [" << colI << "," << colF << "] - T [" << timeI << "," << timeF << "]:" << endl;
	}
	vector<vector<vector<int>>> res;
	// Validación de rangos
	if(rowF >= n_rows){
		if(rowI >= n_rows){
			return res;
		}
		rowF = n_rows-1;
	}
	if(colF >= n_cols){
		if(colI >= n_cols){
			return res;
		}
		colF = n_cols-1;
	}
	if(timeF >= n_inst){
		if(timeI >= n_inst){
			return res;
		}
		timeF = n_inst-1;
	}

	int rRows = rowF - rowI + 1;
	int rCols = colF - colI + 1;
	int rTimes = timeF - timeI + 1;

	res = vector<vector<vector<int>>>(rRows, vector<vector<int>>(rCols, vector<int>(rTimes, 0)));
	int pQLP, row, col;
	map<int,vector<int>> refM;
	for(int i=rowI; i<=rowF; i++){
		for(int j=colI; j<=colF; j++){
			vector<vector<int>> gs = getSerie(i, j);
			if(gs.size() == n_inst){
				for(int k=timeI; k<=timeF; k++){
					res[i-rowI][j-colI][k-timeI] = gs[k];
				}
			}
		}
	}

	return res;
}

/***********************************************************
						PRIVATE
***********************************************************/

unsigned int TempRasterRowComp::zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int TempRasterRowComp::zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

bool TempRasterRowComp::esFija(vector<int> serie){
	for(int j=1; j<serie.size(); j++){
		if(serie[j] != serie[j-1]){
			return false;
		}
	}
	return true;
}

vector<int> TempRasterRowComp::getRowReferenceSerie(int row){
	vector<int> r;
	if(bvQuadNoFijos[row] == 0){
		return r;
	}
	int pos = rankQuadNoFijos(row);
	r = vector<int>(n_inst);
	r[0] = refFirstValue[pos] + min_value;
	for(int i=0; i<n_inst-1; i++){
		r[i+1] = r[i] + zigzag_decode(refs[pos][i]);
	}
	return r;
}