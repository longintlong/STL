#include "../STL/deque.h"

#include <iostream>
#include <string>
#include <deque>


using namespace std;



template<typename T>
void print_deque(const mystl::deque<T>& deq) {
    cout << "deq size is " << deq.size() << endl;
    if(deq.size()) {
       for(auto it = deq.begin(); it != deq.end(); ++it) 
           cout << *it <<" ";
       cout << endl;
    }
}

int main() {
    mystl::deque<int> deq1(5, 9);
    print_deque(deq1);
    
    mystl::deque<string> deq2(5, "Hello, world!");
    deq2.push_back("Happy 1024!");
    deq2.push_back("This is my tiny STL!");
    print_deque(deq2);

    mystl::deque<string> deq4(deq2);
    print_deque(deq4);
    deq4.push_front("Push Front");
    print_deque(deq4);
    deq4.pop_back();
    deq4.pop_front();
    print_deque(deq4);
    
    cout<< "erase !" <<endl;
    deq4.erase(deq4.begin() + 1 , deq4.begin() + 3);
    deq4.push_front("Cpp yyds!");
    print_deque(deq4);
    
    cout << "clear !" << endl;
    deq4.clear();
    cout << "after clear deq4 size is " << deq4.size() << endl;
}