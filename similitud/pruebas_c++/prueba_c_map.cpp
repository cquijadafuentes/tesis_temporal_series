#include <iostream>
#include <map>

using namespace std;

map<int,int> m2;

void consultando(map<int,int> &m){
	cout << "Consultando..." << endl;	
	cout << m[0] << endl;
	cout << m[1] << endl;
	cout << m[2] << endl;
	cout << m[3] << endl;
}

void insertando(map<int,int> &m){
	cout << "Insertando..." << endl;
	m[1] += 2;
	m[2]++;
}

int main(int argc, char const *argv[]){
	cout << "mapa" << endl;
	map<int,int> mapa;
	consultando(mapa);
	insertando(mapa);
	consultando(mapa);
	cout << "m (global)" << endl;
	consultando(m2);
	insertando(m2);
	insertando(m2);
	insertando(m2);
	consultando(m2);
	return 0;
}