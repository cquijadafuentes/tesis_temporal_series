#include <iostream>

using namespace std;

unsigned int zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int main(int argc, char const *argv[]){
	cout << "ENCONDING" << endl;
	while(true){
		int aux;
		cin >> aux;
		cout << zigzag_encode(aux) << endl;
	}
	return 0;
}
