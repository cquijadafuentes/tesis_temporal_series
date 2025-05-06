#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<bool> encodeGolomb(int n, int m) {
	int q = n / m;
	int r = n % m;

	vector<bool> encoded;
	for (int i = 0; i < q; i++) {
		encoded.push_back(true);
	}
	encoded.push_back(false);

	int b = ceil(log2(m));
	vector<bool> remainderBits(b);
	for (int i = b - 1; i >= 0; i--) {
		remainderBits[i] = (r >> (b - 1 - i)) & 1;
	}
	encoded.insert(encoded.end(), remainderBits.begin(), remainderBits.end());
	return encoded;
}

int decodeGolomb(const vector<bool>& encoded, int m, int& index) {
	int q = 0;
	while (index < encoded.size() && encoded[index]) {
		q++;
		index++;
	}
	index++;

	int b = ceil(log2(m));
	int r = 0;
	for (int i = 0; i < b; i++) {
		r = (r << 1) | encoded[index++];
	}
	return q * m + r;
}

int main(int argc, char const *argv[]){
	int m = 10;
	int n = 10000;

	if(argc > 1){
		n = stoi(argv[1]);
	}

	cout << "N\tEncoded\tDecoded" << endl;

	for(int i=0; i<=50; i++){
		cout << i << "\t";
		vector<bool> encoded = encodeGolomb(i, m);
		for (bool bit : encoded) {
			cout << bit;
		}
		cout  << "\t";

		int index = 0;
		int decoded = decodeGolomb(encoded, m, index);
		cout << decoded << endl;
	}

	for(int i=0; i<n; i+=100){
		cout << i << "\t";
		vector<bool> encoded = encodeGolomb(i, m);
		for (bool bit : encoded) {
			cout << bit;
		}
		cout  << "\t";

		int index = 0;
		int decoded = decodeGolomb(encoded, m, index);
		cout << decoded << endl;
	}

	return 0;
}