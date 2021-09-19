#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <new>

#include "iterator.h"

namespace std {
namespace mystl {

template<typename ForwardIterator>
void __destory_aux(ForwardIterator, ForwardIterator, std::true_type) {}

template<typename ForwardIterator>
void __destory_aux(ForwardIterator first, ForwardIterator last,
                   std::false_type) {
    for(; first != last; first++) {
        destory(&*first);
    }
}

template<typename _Tp>
void __destory(_Tp, std::true_type) {}

template<typename _Tp>
void __destory(_Tp* ptr, std::false_type) {
    ptr->~_Tp();
}

template<typename _Tp>
void destory(_Tp* ptr) {
    __destory(ptr, std::is_trivially_destructible<_Tp>());
}

template<typename ForwardIterator>
void destory(ForwardIterator first, ForwardIterator last) {
    typedef typename iterator_traits<ForwardIterator>::value_type value_type;
    __destory_aux(first, last, 
                 std::is_trivially_destructible<value_type>());
}

}   // end of mystl
}   // end of std

#endif