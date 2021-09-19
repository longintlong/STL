#include "../STL/allocator.h"

#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> v1 = {1,2,3,4,5};
    vector<int, mystl::allocator<int>> v2;
    for(auto i : v1) {
        v2.push_back(i);
    }
    for(auto i : v2) {
        cout << i <<endl;
    }
}