#ifndef STACK_H
#define STACK_H

#include "deque.h"

namespace mystl {

template<typename T, typename Sequence = mystl::deque<T>>
class stack {
public:
    // 友元重载 == <
    // 根据warning的提示,这里最好加上template
    template<typename T1, typename Sequence1>
    friend bool operator==(const stack<T1, Sequence1>&, const stack<T, Sequence1>&);
    // template<typename T1, typename Sequence1>
    // friend bool operator <(const stack<T1, Sequence1>&, const stack<T1, Sequence1>&);

    typedef typename Sequence::valueType        valueType;
    typedef typename Sequence::reference        reference;
    typedef typename Sequence::constReference   constReference;
    typedef typename Sequence::sizeType         sizeType;
    typedef          Sequence                   containerType;
    // static_assert编译期间静态断言
    static_assert(std::is_same<T, valueType>::value,
                  "value_type must be the same as the underlying container");
                
protected:
    Sequence c;

public:
    stack() = default;
    explicit stack(sizeType n) : c(n) {}
    explicit stack(const Sequence& __c) : c(__c) {}
    // explicit stack(Sequence&& __c) : c(std::move(__c)) {}
    stack(sizeType n, const valueType& value) : c(n, value) {}

    stack(const stack& rhs) : c(rhs.c){}
    

    bool                empty() const { return c.empty(); }
    sizeType            size()  const { return c.size(); } 
    reference           top()         { assert(c.size() != 0); return *(c.end() - 1); }
    constReference      top()   const { assert(c.size() != 0); return *(c.end() - 1); }
    
    void        push(const valueType& x) { c.push_back(x); }
    void        pop() { assert(c.size() != 0); c.pop_back(); }

};

template<typename T, typename Sequence>
bool inline operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    return x.c == y.c;
}

template<typename T, typename Sequence>
bool inline operator!=(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    return x.c != y.c;
}


} // end of mystl



#endif