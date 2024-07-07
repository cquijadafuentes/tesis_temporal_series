#include <iostream>
#include <vector>

using namespace std;

template <typename TipoDato>
float promedioVector(const vector<TipoDato>& vectorNumeros) {
  float suma = 0;
  for (TipoDato numero : vectorNumeros) {
    suma += numero;
  }
  return suma / vectorNumeros.size();
}

int main(int argc, char const *argv[])
{
  vector<int> miVector = {1, 2, 3, 4, 5};
  float resultado = promedioVector(miVector);
  cout << "El promedio es: " << resultado << endl;
  vector<float> miVector2 = {1.5, 2.5, 3.5, 4.5, 5.5};
  resultado = promedioVector(miVector2);
  cout << "El promedio es: " << resultado << endl;
  return 0;
}