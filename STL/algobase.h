#ifndef ALGOBASE
#define ALGOBASE

#include <algorithm>

namespace mystl {

// move_backward
template<typename BI1, typename BI2>
BI2 move_backward(BI1 first, BI1 last, BI2 result) {
    // TODO: unfinished move_backward
    return std::move_backward(first, last, result);
}


// copy_backward
template<typename BI1, typename BI2>
BI2 copy_backward(BI1 first, BI1 last, BI2 result) {
    // TODO: unfinished copy_backward
    return std::copy_backward(first, last, result);
}



}   // end of namespace mystl


#endif