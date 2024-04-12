#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char const *argv[]){
  if(argc < 2){
    // El archivo debe indicar numero_filas y numero_columnas
    // en la primera línea. Luego todos los datos
    cout << "Error! Faltan argumentos:" << endl;
    cout << argv[0] << " <input_file>" << endl;
    return 0;
  }

  ifstream inputfile(argv[1]);
  if (!inputfile.is_open()){
    cout << "Error al abrir" << argv[1] << endl;
    exit(EXIT_FAILURE);
  }
  int rows, cols;
  inputfile >> rows >> cols;
  vector<vector<int>> grilla(rows, vector<int>(cols));
  for(int f=0; f<rows; f++){
    for(int c=0; c<cols; c++){
      inputfile >> grilla[f][c];
    }
  }
  inputfile.close();
  string filename = argv[1];
  filename += ".bin";
  ofstream outputfile(filename,ios_base::binary);
  for(int c=0; c<cols; c++){
    for(int f=0; f<rows; f++){
      outputfile.write(reinterpret_cast<const char *>(&grilla[f][c]), sizeof(grilla[f][c]));
    }
  }
  outputfile.close();

  return 0;
}