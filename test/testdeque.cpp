#include "../STL/deque.h"

#include <iostream>
#include <string>
#include <deque>


using namespace std;
int main() {
    mystl::deque<int> deq1(5, 9);
    cout << "deq1 size is " << deq1.size() << endl;
    for(auto it = deq1.begin(); it != deq1.end(); ++it) cout << *it <<endl;
    deq1.clear();
    
    mystl::deque<string> deq3(5, "Hello, world!");
    cout << "deq3 size is " << deq3.size() << endl;
    for(auto it = deq3.begin(); it != deq3.end(); ++it) cout << *it <<endl;
    deq3.push_back("Happy 1024!");
    deq3.push_back("This is my tiny STL!");

    mystl::deque<string> deq4(deq3);
    cout << "deq4 size is " << deq4.size() << endl;
    for(auto it = deq4.begin(); it != deq4.end(); ++it) cout << *it <<endl;
    
    cout<< "erase !" <<endl;
    deq4.erase(deq4.begin() + 1 , deq4.begin() + 3);
    for(auto it = deq4.begin(); it != deq4.end(); ++it) cout << *it <<endl;
    
    cout << "clear !" << endl;
    deq4.clear();
    cout << "after clear deq4 size is " << deq4.size() << endl;
}