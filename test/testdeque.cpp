#include "../STL/deque.h"

#include <iostream>
#include <string>


using namespace std;
int main() {
    mystl::deque<int> deq1(5, 9);
    cout << "deq1 size is " << deq1.size() << endl;
    for(auto it = deq1.begin(); it != deq1.end(); ++it) cout << *it <<endl;
    mystl::deque<int> deq2(deq1);
    cout << "deq2 size is " << deq1.size() << endl;
    for(auto it = deq2.begin(); it != deq2.end(); ++it) cout << *it <<endl;
}