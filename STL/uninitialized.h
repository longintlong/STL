#ifndef UNINITIALIZED_H
#define UNINITIALIZED_H

#include "iterator.h"
#include "type_traits.h"
#include "algobase.h"
#include "construct.h"

namespace mystl {

// uninitialized_move
template<typename InputIter, typename NoThrowForwardIter>
NoThrowForwardIter
_M_uninitialized_move(InputIter first, InputIter last,
                      NoThrowForwardIter result, std::true_type) {
    return std::move(first, last, result);
}

template<typename InputIter, typename NoThrowForwardIter>
NoThrowForwardIter
_M_uninitialized_move(InputIter first, InputIter last,
                      NoThrowForwardIter result, std::false_type) {
    NoThrowForwardIter cur = result;
    try {
        for(; first != last; first++, cur++) {
            // TODO 这里对迭代器到底是什么产生了疑问，&*cur 和 *cur代表什么
            mystl::construct(&*cur, std::move(*first));
        }
    } catch(...) {
        // TODO 这里异常处理是不是应该throw，因为已经UB了
        mystl::destory(result, cur);
        throw;
    }
    return cur;
}

template<typename InputIter, typename NoThrowForwardIter>
NoThrowForwardIter
uninitialized_move(InputIter first, InputIter last, NoThrowForwardIter result) {
    return _M_uninitialized_move(first, last, result,
                                 std::is_trivially_move_assignable<
                                 typename iterator_traits<
                                 InputIter>::value_type>{});
}
}   // end of namespace mystl

#endif