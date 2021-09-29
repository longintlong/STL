#include "../STL/vector.h"

#include <iostream>
using namespace std;
int main() {
    mystl::vector<int> vec;
    cout << "capacity " <<vec.capacity() << " size " << vec.size() << endl;
    vec.push_back(2);
    vec.push_back(4);
    vec.push_back(3);
    cout << "capacity " <<vec.capacity() << " size " << vec.size() << endl;
    for(int i = 0; i < vec.size(); i++) cout << vec[i] << endl;
    for(auto it = vec.begin(); it != vec.end(); it++) cout << *it <<endl;
    mystl::vector<int> vec1(vec.begin(), vec.end());
    for(int i = 0; i < vec1.size(); i++) cout << vec1[i] << endl;
    mystl::vector<int> t(5, 10);
}