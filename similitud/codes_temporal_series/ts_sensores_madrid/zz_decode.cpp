#include <iostream>

using namespace std;

int zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

int main(int argc, char const *argv[]){
	cout << "DECONDING" << endl;
	while(true){
		int aux;
		cin >> aux;
		cout << zigzag_decode(aux) << endl;
	}
	return 0;
}