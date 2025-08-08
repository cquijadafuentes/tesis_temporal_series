
#include "TempRaster_QuadComp.hpp"


TempRasterQuadComp::TempRasterQuadComp(vector<vector<vector<int>>> &tseries, int dims){
	n_rows = tseries.size();			// filas
	n_cols = tseries[0].size();		// columnas
	n_inst = tseries[0][0].size();	// length of temporal series
	d_quad = dims;
	cel_per_quad = d_quad * d_quad;
	// Cantidad de filas y columnas de cuadrantes según las dimensiones dims.
	nQuadRows = n_rows / d_quad;
	if(nQuadRows*d_quad < n_rows){
		nQuadRows++;
	}
	nQuadCols = n_cols / d_quad;
	if(nQuadCols*d_quad < n_cols){
		nQuadCols++;
	}

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

	bit_vector bvQSR = bit_vector(nQuadRows * nQuadCols);	// Marcar cuadrantes con Serie de referencia
	int cantCeldasQuad = (cel_per_quad) * (nQuadRows * nQuadCols);
	bit_vector bvSR = bit_vector(cantCeldasQuad);			// Marcar la serie de referencia
	bit_vector bvSF = bit_vector(cantCeldasQuad);			// Marcar las series fijas

	cout << "nQuadRows: " << nQuadRows << " - nQuadCols: " << nQuadCols << endl;
	cout << "d_quad: " << d_quad << " - cantQuads: " << (nQuadRows*nQuadCols) << endl;
	cout << "Total celdas en cuadrantes: " << cantCeldasQuad << endl;

	vector<int> valoresPVSR;		// Guarda el primer valor de cada serie de referencia
	vector<int> valoresSF;		// Guarda el valor de cada serie fija
	int iFV = 0;
	for(int fq=0; fq<nQuadRows; fq++){		// fq = fila-'quadrante'
		for(int cq=0; cq<nQuadCols; cq++){	// cq = columna-'quadrante'
			// Procesando primera series del 'quadrante'
			int f0q = fq*d_quad;		// Fila cero (0) del 'quadrante'
			int c0q = cq*d_quad;		// Columna cero (0) del 'quadrante'
			int iQuad = (fq * nQuadCols) + cq;	// Índice del 'quadrante' en row-major
			// Procesando el resto de las series del 'quadrante'
			vector<int> serieReferencia(n_inst);
			for(int df=0; df<d_quad && f0q+df < n_rows; df++){		// delta-filas en celdas
				for(int dc=0; dc<d_quad && c0q+dc < n_cols; dc++){		// delta-columnas en celdas
					int posF = f0q+df;		// Fila de la celda en evaluación
					int posC = c0q+dc;		// Columna de la celda en evaluación
					int iCelda = getQuadLinealPosition(posF, posC);
					int val;
					if(esFija(tseries[posF][posC])){
						// La serie de tiempo es fija y no se representa
						valoresSF.push_back(tseries[posF][posC][0]);
						bvSF[iCelda] = 1;
						bvSR[iCelda] = 0;
					}else if(bvQSR[iQuad] == 0){
						// NO hay serie de referencia
						for(int t=0; t<n_inst; t++){
							serieReferencia[t] = tseries[posF][posC][t];
						}
						valoresPVSR.push_back(tseries[posF][posC][0]);
						int_vector<> ivaux(n_inst-1);
						for(int k=1; k<n_inst; k++){
							val = tseries[posF][posC][k] - tseries[posF][posC][k-1];
							ivaux[k-1] = zigzag_encode(val);
						}
						util::bit_compress(ivaux);
						refs.push_back(ivaux);
						bvQSR[iQuad] = 1;
						bvSR[iCelda] = 1;
						bvSF[iCelda] = 0;
					}else{
						// SI hay serie de referencia
						int_vector<> ivaux2(n_inst);
						for(int k=0; k<n_inst; k++){
							val = tseries[posF][posC][k] - serieReferencia[k];
							ivaux2[k] = zigzag_encode(val);
						}
						util::bit_compress(ivaux2);
						series.push_back(ivaux2);
						bvSF[iCelda] = 0;
					}
				}
			}
		}
	}

	int_vector<> fixedValue(valoresSF.size());
	for(int i=0; i<valoresSF.size(); i++){
		fixedValue[i] = valoresSF[i] - min_value;
	}
	util::bit_compress(fixedValue);

	int_vector<> refFirstValue(valoresPVSR.size());
	for(int i=0; i<valoresPVSR.size(); i++){
		refFirstValue[i] = valoresPVSR[i] - min_value;
		if((valoresPVSR[i] - min_value) < 0){
			cout << "Error! operación [valoresPVSR[i] - min_value] con resultado negativo en bit_vector 'refFirstValue'." << endl;
		} 
	}
	util::bit_compress(refFirstValue);

	bvQuadNoFijos = sd_vector<>(bvQSR);
	bvReferencias = sd_vector<>(bvSR);
	bvSeriesFijas = sd_vector<>(bvSF);

	buildRanksSelects();

	cout << "Size in kBytes: " << size_kbytes() << endl;
}


TempRasterQuadComp::TempRasterQuadComp(vector<vector<vector<int>>> &tseries, bool byRow){
	
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

	bit_vector bvQSR = bit_vector(n_rows);	// Marcar cuadrantes con Serie de referencia
	bit_vector bvSR = bit_vector(n_rows * n_cols);			// Marcar la serie de referencia
	bit_vector bvSF = bit_vector(n_rows * n_cols);			// Marcar las series fijas
	vector<int> valoresPVSR;		// Guarda el primer valor de cada serie de referencia
	vector<int> valoresSF;		// Guarda el valor de cada serie fija
	
	if(byRow){
		for(int f=0; f<n_rows; f++){
			vector<int> serieReferencia(n_inst);
			for(int c=0; c<n_cols; c++){
				int iCelda = f*n_rows + c;
				int val;
				if(esFija(tseries[f][c])){
					// La serie de tiempo es fija y no se representa
					valoresSF.push_back(tseries[f][c][0]);
					bvSF[iCelda] = 1;
					bvSR[iCelda] = 0;
				}else if(bvQSR[f] == 0){
					// NO hay serie de referencia
					for(int t=0; t<n_inst; t++){
						serieReferencia[t] = tseries[f][c][t];
					}
					valoresPVSR.push_back(tseries[f][c][0]);
					int_vector<> ivaux(n_inst-1);
					for(int k=1; k<n_inst; k++){
						val = tseries[f][c][k] - tseries[f][c][k-1];
						ivaux[k-1] = zigzag_encode(val);
					}
					util::bit_compress(ivaux);
					refs.push_back(ivaux);
					bvQSR[f] = 1;
					bvSR[iCelda] = 1;
					bvSF[iCelda] = 0;
				}else{
					// SI hay serie de referencia
					int_vector<> ivaux2(n_inst);
					for(int k=0; k<n_inst; k++){
						val = tseries[f][c][k] - serieReferencia[k];
						ivaux2[k] = zigzag_encode(val);
					}
					util::bit_compress(ivaux2);
					series.push_back(ivaux2);
					bvSF[iCelda] = 0;
				}
			}
		}
	}else{
		for(int c=0; c<n_cols; c++){
			vector<int> serieReferencia(n_inst);
			for(int f=0; f<n_rows; f++){
				int iCelda = f*n_rows + c;
				int val;
				if(esFija(tseries[f][c])){
					// La serie de tiempo es fija y no se representa
					valoresSF.push_back(tseries[f][c][0]);
					bvSF[iCelda] = 1;
					bvSR[iCelda] = 0;
				}else if(bvQSR[c] == 0){
					// NO hay serie de referencia
					for(int t=0; t<n_inst; t++){
						serieReferencia[t] = tseries[f][c][t];
					}
					valoresPVSR.push_back(tseries[f][c][0]);
					int_vector<> ivaux(n_inst-1);
					for(int k=1; k<n_inst; k++){
						val = tseries[f][c][k] - tseries[f][c][k-1];
						ivaux[k-1] = zigzag_encode(val);
					}
					util::bit_compress(ivaux);
					refs.push_back(ivaux);
					bvQSR[c] = 1;
					bvSR[iCelda] = 1;
					bvSF[iCelda] = 0;
				}else{
					// SI hay serie de referencia
					int_vector<> ivaux2(n_inst);
					for(int k=0; k<n_inst; k++){
						val = tseries[f][c][k] - serieReferencia[k];
						ivaux2[k] = zigzag_encode(val);
					}
					util::bit_compress(ivaux2);
					series.push_back(ivaux2);
					bvSF[iCelda] = 0;
				}
			}
		}
	}

	int_vector<> fixedValue(valoresSF.size());
	for(int i=0; i<valoresSF.size(); i++){
		fixedValue[i] = valoresSF[i] - min_value;
	}
	util::bit_compress(fixedValue);

	int_vector<> refFirstValue(valoresPVSR.size());
	for(int i=0; i<valoresPVSR.size(); i++){
		refFirstValue[i] = valoresPVSR[i] - min_value;
	}
	util::bit_compress(refFirstValue);

	bvQuadNoFijos = sd_vector<>(bvQSR);
	bvReferencias = sd_vector<>(bvSR);
	bvSeriesFijas = sd_vector<>(bvSF);

	buildRanksSelects();

	cout << "Size in kBytes: " << size_kbytes() << endl;
}

TempRasterQuadComp::TempRasterQuadComp(string inputFilename){
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
	infile.read((char *)&d_quad, sizeof(int));
	infile.read((char *)&cel_per_quad, sizeof(int));	
	infile.read((char *)&nQuadRows, sizeof(int));
	infile.read((char *)&nQuadCols, sizeof(int));
	infile.read((char *)&min_value, sizeof(int));
	infile.read((char *)&max_value, sizeof(int));
	// --------- Cargando int_vectors
	fixedValue.load(infile);
	refFirstValue.load(infile);
	// --------- Cargando vectores de int_vectors
	int_vector<> tempIV;
	infile.read((char *)&aux1,sizeof(int));
	refs = vector<int_vector<>>(aux1);
	for(int i=0; i<aux1; i++){
		tempIV.load(infile);
		util::bit_compress(tempIV);
		refs[i] = tempIV;
	}
	infile.read((char *)&aux1,sizeof(int));
	series = vector<int_vector<>>(aux1);
	for(int i=0; i<aux1; i++){
		tempIV.load(infile);
		util::bit_compress(tempIV);
		series[i] = tempIV;
	}
	// --------- Cargando bitvectors
	bvQuadNoFijos.load(infile);
	bvReferencias.load(infile);
	bvSeriesFijas.load(infile);
	// Cerrando archivo
	infile.close();
	// Construyendo rank y select de MFS
	buildRanksSelects();

	cout << "Leído..." << endl;
}

bool TempRasterQuadComp::save(string outputFilename){
	ofstream outfile(outputFilename, ofstream::binary);
	// Guardando valores enteros
	outfile.write((char const*)&n_rows, sizeof(int));
	outfile.write((char const*)&n_cols, sizeof(int));
	outfile.write((char const*)&n_inst, sizeof(int));
	outfile.write((char const*)&d_quad, sizeof(int));
	outfile.write((char const*)&cel_per_quad, sizeof(int));
	outfile.write((char const*)&nQuadRows, sizeof(int));
	outfile.write((char const*)&nQuadCols, sizeof(int));
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
	bvQuadNoFijos.serialize(outfile);
	bvReferencias.serialize(outfile);
	bvSeriesFijas.serialize(outfile);
	// Cerrando archivo
	outfile.close();
	return true;
}

int TempRasterQuadComp::size_bytes(){
	int bytes = 0;
	// Atributos enteros
	bytes += (sizeof(int) * 9);
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
	bytes += size_in_bytes(bvQuadNoFijos);
	bytes += size_in_bytes(bvReferencias);
	bytes += size_in_bytes(bvSeriesFijas);
	return bytes;
}

int TempRasterQuadComp::size_kbytes(){
	int kbytes = size_bytes() / 1024;
	return kbytes;
}

int TempRasterQuadComp::size_mbytes(){
	int mbytes = size_bytes() / 1024 / 1024;
	return mbytes;
}

vector<int> TempRasterQuadComp::getSerie(int row, int col){
	vector<int> r;
	// 1 - Verificar que no corresponde a una serie fija
	int posQLP = getQuadLinealPosition(row, col);
	if(bvSeriesFijas[posQLP] == 1){
		int p = rankSeriesFijas(posQLP);
		int v = fixedValue[p] + min_value;
		r = vector<int>(n_inst, v);
		return r;
	}
	// 2 - Recuperar la serie de referencia del cuadrante
	vector<int> ref = getQuadReferenceSerie(getQuad(row, col));
	if(bvReferencias[posQLP] == 1){
		return ref;
	}
	// 3 - Si no es la referencia, recuperar la serie
	int posSerie = getSeriePositionFromQLP(posQLP, row, col);
	r = vector<int>(n_inst);
	for(int i=0; i<n_inst; i++){
		r[i] = zigzag_decode(series[posSerie][i]) + ref[i];
	}
	return r;
}

void TempRasterQuadComp::print(){

	cout << "Mostrando los datos: " << endl;

	cout << "n_rows: " << n_rows << endl;
	cout << "n_cols: " << n_cols << endl;
	cout << "n_inst: " << n_inst << endl;
	cout << "d_quad: " << d_quad << endl;
	cout << "cel_per_quad: " << cel_per_quad << endl;
	cout << "nQuadRows: " << nQuadRows << endl;
	cout << "nQuadCols: " << nQuadCols << endl;
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

	
	cout << "bvQuadNoFijos [" << bvQuadNoFijos.size() << "]:\n\t";
	for(int i=0; i<bvQuadNoFijos.size(); i++){
		cout << bvQuadNoFijos[i] << " ";
	}
	cout << "\n\t";
	for(int i=0; i<bvQuadNoFijos.size(); i++){
		cout << i%10 << " ";
	}
	cout << endl;

	
	cout << "bvReferencias [" << bvReferencias.size() << "]:\n\t";
	for(int i=0; i<bvReferencias.size(); i++){
		cout << bvReferencias[i] << " ";
	}
	cout << "\n\t";
	for(int i=0; i<bvReferencias.size(); i++){
		cout << i%10 << " ";
	}
	cout << endl;

	
	cout << "bvSeriesFijas [" << bvSeriesFijas.size() << "]:\n\t";
	for(int i=0; i<bvSeriesFijas.size(); i++){
		cout << bvSeriesFijas[i] << " ";
	}
	cout << "\n\t";
	for(int i=0; i<bvSeriesFijas.size(); i++){
		cout << i%10 << " ";
	}
	cout << endl;

	sd_vector<>::rank_1_type rankQuadNoFijos;
	sd_vector<>::rank_1_type rankReferencias;
	sd_vector<>::rank_1_type rankSeriesFijas;
	sd_vector<>::select_1_type selectReferencias;

}

/***********************************************************
						QUERIES
***********************************************************/

int TempRasterQuadComp::queryAccess(int row, int col, int inst, bool mostrarQuery){
	if(mostrarQuery){
		cout << "Access Query: F " << row << " - C " << col << " - T " << inst << endl;
	}
	// 1 - Verificar que no corresponde a una serie fija
	int posQLP = getQuadLinealPosition(row, col);
	if(bvSeriesFijas[posQLP] == 1){
		int p = rankSeriesFijas(posQLP);
		int r = fixedValue[p] + min_value;
		return r;
	}
	// 2 - Recuperar la serie de referencia del cuadrante
	vector<int> ref = getQuadReferenceSerie(getQuad(row, col));
	if(bvReferencias[posQLP] == 1){
		return ref[inst];
	}
	// 3 - Si no es la referencia, recuperar la serie
	int posSerie = getSeriePositionFromQLP(posQLP, row, col);
	int r = zigzag_decode(series[posSerie][inst]) + ref[inst];
	return r;
}

void swapValues(int &a, int &b){
	int x = a;
	a = b;
	b = x;
}

vector<vector<vector<int>>> TempRasterQuadComp::queryWindow(int rowI, int rowF, int colI, int colF, int timeI, int timeF, bool mostrarQuery){
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
	for(int i=0; i<rRows; i++){
		if((i+rowI) % d_quad == 0){
			// Se puede limpiar el mapa porque se pasó a otra fila de cuadrantes
			refM.clear();
		}
		for(int j=0; j<rCols; j++){
			row = i+rowI;
			col = j+colI;
			pQLP = getQuadLinealPosition(row, col);
			if(bvSeriesFijas[pQLP] == 1){
				// ----------- Serie Fija
				int p = rankSeriesFijas(pQLP);
				int r = fixedValue[p] + min_value;
				for(int k=0; k<rTimes; k++){
					res[i][j][k] = r;
				}
			}else{
				// ----------- Serie NO Fija
				int idQuad = getQuad(row, col);
				if(refM.find(idQuad) == refM.end()){
					refM[idQuad] = getQuadReferenceSerie(getQuad(row, col));
				}
				if(bvReferencias[pQLP] == 1){
					// ----------- Serie de Referencia
					for(int k=0; k<rTimes; k++){
						res[i][j][k] = refM[idQuad][timeI+k];
					}
				}else{					
					// ----------- Serie Derivada
					int posSerie = getSeriePositionFromQLP(pQLP, row, col);
					for(int k=0; k<rTimes; k++){
						res[i][j][k] = zigzag_decode(series[posSerie][timeI+k]) + refM[idQuad][timeI+k];
					}
				}
			}
		}
	}

	return res;
}

/***********************************************************
						PRIVATE
***********************************************************/

void TempRasterQuadComp::buildRanksSelects(){
	rankQuadNoFijos = sd_vector<>::rank_1_type(&bvQuadNoFijos);
	rankReferencias = sd_vector<>::rank_1_type(&bvReferencias);
	rankSeriesFijas = sd_vector<>::rank_1_type(&bvSeriesFijas);
	selectReferencias = sd_vector<>::select_1_type(&bvReferencias);
}

unsigned int TempRasterQuadComp::zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int TempRasterQuadComp::zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

bool TempRasterQuadComp::esFija(vector<int> serie){
	for(int j=1; j<serie.size(); j++){
		if(serie[j] != serie[j-1]){
			return false;
		}
	}
	return true;
}

unsigned int TempRasterQuadComp::getQuad(int r, int c){
	unsigned int rquad = r / d_quad;
	unsigned int cquad = c / d_quad;
	unsigned int iquad = (rquad * nQuadCols) + cquad;
	return iquad;
}

unsigned int TempRasterQuadComp::getQuadLinealPosition(int r, int c){
	unsigned int iQuad = getQuad(r,c);
	unsigned int dr = r % d_quad;
	unsigned int dc = c % d_quad;
	unsigned int cxQuad = cel_per_quad;
	unsigned int quadLinealPos = (iQuad * cxQuad) + (dr * d_quad) + dc;
	return quadLinealPos;
}

vector<int> TempRasterQuadComp::getQuadReferenceSerie(int iQuad){
	vector<int> r;
	if(bvQuadNoFijos[iQuad] == 0){
		return r;
	}
	int pos = rankQuadNoFijos(iQuad);
	r = vector<int>(n_inst);
	r[0] = refFirstValue[pos] + min_value;
	for(int i=0; i<n_inst-1; i++){
		r[i+1] = r[i] + zigzag_decode(refs[pos][i]);
	}
	return r;
}

unsigned int TempRasterQuadComp::getSeriePositionFromQLP(int qlp, int row, int col){
	unsigned int blankCellPerRow = (d_quad * nQuadCols) - n_cols;
	unsigned int prevRows = row;
	if(((qlp / cel_per_quad) + 1) % nQuadCols != 0){
		// Si el Quad NO esta en la última columna sólo se descuentan
		// las filas de cuadrantes anterires
		prevRows -= (row % d_quad);
	}
	unsigned int extraCells = (prevRows * blankCellPerRow);
	if(row / d_quad == (nQuadRows - 1)){
		unsigned int blankCellPerCol = (d_quad * nQuadRows) - n_rows;
		unsigned int prevCols = col - (col % d_quad);
		extraCells += (blankCellPerCol * prevCols);
	}
	unsigned int rFijas = rankSeriesFijas(qlp);
	unsigned int rRefs = rankReferencias(qlp);
	unsigned int pos = qlp - rFijas - rRefs - extraCells;
	return pos;
}