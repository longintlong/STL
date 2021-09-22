#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

#include <type_traits>

namespace mystl {

template<typename _Tp, _Tp v>
struct my_integral_constant {
    static constexpr _Tp value = v;
};

typedef my_integral_constant<bool, true> my_true_type;
typedef my_integral_constant<bool, false> my_false_type;


}   // end of mystl

#endif 