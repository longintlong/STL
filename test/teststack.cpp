#include "../STL/stack.h"

#include <iostream>
#include <string>


using namespace std;
int main() {
    // mystl::stack<int> st;
    // st.push(1);
    // st.push(2);
    // st.push(3);
    // st.push(4);
    // cout << "size is " << st.size() << " top is " << st.top() << endl;
    // cout << "start pop !" << endl;
    // while(!st.empty()) {
    //     cout << st.top() << endl;
    //     st.pop();
    // }

    mystl::stack<string> st;
    st.push("Hello World");
    st.push("This is my tinySTL");
    st.push("stack test cases");
    st.push("push");
    cout << "size is " << st.size() << " top is " << st.top() << endl;
    cout << "start pop !" << endl;
    while(!st.empty()) {
        cout << st.top() << endl;
        st.pop();
    }
}