#include <fstream>      // std::ofstream

using namespace std;

int main () {
  ofstream ofs;
  ofstream afs_a("st_a1.bin",ios_base::binary);
  ofstream afs_b("st_a2.bin",ios_base::binary);  
  ofstream afs_c("st_a3.bin",ios_base::binary);
  int a, b, c;
  for(int i=1; i<=9; i++){
    a = i;
    b = i + 1;
    c = i + 2;
    afs_a.write(reinterpret_cast<const char *>(&a), sizeof(a));
    afs_b.write(reinterpret_cast<const char *>(&b), sizeof(b));
    afs_c.write(reinterpret_cast<const char *>(&c), sizeof(c));
  }
  afs_a.close();
  afs_b.close();
  afs_c.close();
  return 0;
}