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

// 使用迭代器比较容器区间是否相等，已经做好了size的判断
template<typename II1, typename II2>
bool equal(II1 first1, II1 last, II2 first2) {
    for(; first1 != last; ++first1, ++first2) {
        if(*first1 != *first2)
            return false;
    }
    return true;
}


}   // end of namespace mystl


#endif