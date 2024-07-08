#include <iostream>
#include <vector>

using namespace std;

void muestra(vector<int> v){
	for(int i=0; i<v.size(); i++){
		cout << "\t" << v[i];
	}
	cout << endl;
}

void duplica(vector<int> v){
	for(int i=0; i<v.size(); i++){
		v[i] *= 2;
	}
	muestra(v);
}

int main(int argc, char const *argv[]){
	vector<int> x = {1,2,3,4,5,6,7,8,9};
	muestra(x);
	duplica(x);
	muestra(x);
	return 0;
}