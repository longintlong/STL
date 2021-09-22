#ifndef ITERATOR_H
#define ITERATOR_H

#include <cstddef>
namespace mystl {

// 五种迭代器, 每一种都是结构体
// 就是迭代器模板中的Category
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iteraotr_tag : public bidirectional_iterator_tag {};


// 任何迭代器都应该包含这五个内嵌型别
template<typename Category, typename T, typename Distance=std::ptrdiff_t,
         typename Pointer=T*, typename Reference=T&>
struct iterator {
    typedef Category            iterator_category;
    typedef T                   value_type;
    typedef Distance            difference_type;
    typedef Pointer             pointer;
    typedef Reference           reference; 
};

// iterator traits
// 五种常用迭代器相应型别，必须包含
// 泛化版本，针对class type，如果是原生指针不符合，需偏特化版本
template<typename Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::difference_type      difference_type;
    typedef typename Iterator::pointer              pointer;
    typedef typename Iterator::reference            reference;
};

// 针对原生指针的偏特化版本的iterator traits
template<typename T>
struct iterator_traits<T*> {
    // 原生指针是一种random_access_iterator
    typedef typename random_access_iterator_tag     iterator_category;
    typedef typename T                              value_type;
    typedef typename std::ptrdiff_t                 difference_type;
    typedef typename T*                             pointer;
    typedef typename T&                             reference;
};

// 针对原生指针的偏特化版本的iterator traits
template<typename T>
struct iterator_traits<const T*> {
    // 原生指针是一种random_access_iterator
    typedef random_access_iterator_tag     iterator_category;
    typedef T                              value_type;
    typedef std::ptrdiff_t                 difference_type;
    typedef const T*                       pointer;
    typedef const T&                       reference;
};

// 占位参数
// 萃取Iterator的category, valuetype, difftype
template<typename Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&) {
    return typename iterator_traits<Iterator>::iterator_category();
}

template<typename Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template<typename Iterator>
typename iterator_traits<Iterator>::difference_type*
difference_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 计算迭代器的距离
template<typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last,
           input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while(first != last) {
        n++;
        first++;
    }
    return n;
}

template<typename RandomAccessIterator>
typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last,
           random_access_iteraotr_tag) {
    return last - first;
}

template<typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    typedef typename 
        iterator_traits<InputIterator>::iterator_category iterator_category;
    return __distance(first, last, iterator_category());
    // return __distance(first, last, iterator_category(first));
}

// 让迭代器移动n个距离
template<typename InputIterator, typename Distance>
void __advance(InputIterator& it, Distance n, input_iterator_tag) {
    while(n--) {
        it++;
    }
}

template<typename RandomAccessIterator, typename Distance>
void __advance(RandomAccessIterator& it, Distance n, random_access_iteraotr_tag) {
    it += n;
}

template<typename BidirectionalIterator, typename Distance>
void __advance(BidirectionalIterator& it, Distance n, bidirectional_iterator_tag) {
    if(n > 0) {
        while(n--) i++;
    } else {
        while(n++) i--;
    }
}

template<typename InputIterator, typename Distance>
void advance(InputIterator& it, Distance n) {
    __advance(it, n, iterator_category(it));
}


}   // end of mystl
#endif