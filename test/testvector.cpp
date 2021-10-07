#include "../STL/vector.h"

#include <iostream>
#include <string>


using namespace std;
int main() {
    mystl::vector<int> vec;
    cout << "capacity " <<vec.capacity() << " size " << vec.size() << endl;
    vec.push_back(2);
    vec.push_back(3);
    cout << "capacity " <<vec.capacity() << " size " << vec.size() << endl;
    vec.insert(vec.begin() + 1, 4);
    for(int i = 0; i < vec.size(); i++) cout << vec[i] << endl;
    for(auto it = vec.begin(); it != vec.end(); it++) cout << *it <<endl;
    mystl::vector<int> vec1(vec.begin(), vec.end());
    for(int i = 0; i < vec1.size(); i++) cout << vec1[i] << endl;

    // size_t a = 5;
    // int b = 10;
    // mystl::vector<int> t(2, 3);
    // for(int i = 0; i < t.size(); i++) cout << t[i] << " " << endl;

    mystl::vector<string> strVec;
    strVec.push_back("Hello");
    strVec.push_back("world");
    strVec.insert(strVec.begin() + 1, "my");
    strVec.insert(strVec.begin() + 1, 2, "longintlong");
    std::cout << strVec.capacity() << "size" << strVec.size() << std::endl;
    for(int i = 0; i < strVec.size(); i++) cout << strVec[i] << endl;
    strVec.clear();
    std::cout << "capacity = " << strVec.capacity() << " size = " << strVec.size() << std::endl;
    cout << "-----------after clear------------" <<endl;
    
    strVec.push_back("Hello");
    std::cout << "capacity = " << strVec.capacity() << " size = " << strVec.size() << std::endl;
    strVec.emplace_back("world");
    strVec.insert(strVec.begin() + 1, 2, "longintlong");
    for(int i = 0; i < strVec.size(); i++) cout << strVec[i] << endl;
}