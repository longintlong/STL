#ifndef VECTOR_H
#define VECTOR_H

#include "allocator.h"

#include <algorithm>
#include <memory>
#include <assert.h>


namespace mystl {

template<typename T>
class vector {
public:
    // vector嵌套型别定义
    typedef mystl::allocator<T>                          data_allocator; 
    typedef typename data_allocator::value_type          value_type;
    typedef typename data_allocator::pointer             pointer;
    typedef typename data_allocator::constPointer        constPointer;
    typedef typename data_allocator::reference           reference;
    typedef typename data_allocator::constReference      constReference;
    typedef typename data_allocator::sizeType            sizeType;
    typedef typename data_allocator::differenceType      differenceType;
    
    typedef value_type*                                  iterator;
    typedef const value_type*                            constIterator;
    
private:
    iterator start;
    iterator finish;
    iterator endOfStorage;

public:
    // 构造
    vector() : start(0), finish(0), endOfStorage(0) {};
    explicit vector(sizeType);
    vector(sizeType, const value_type&);
    // 这里为什么用模板的 https://www.zhihu.com/question/62552068
    // 防止和上一个构造函数冲突 -> vec(5, 10)
    // 解决方法就是判断是否是InputIterator
    template<typename Iter,
             typename isInputIter = mystl::_RequireInputIter<Iter>>
    vector(Iter, Iter);

    // 拷贝
    vector(const vector&);
    vector& operator=(const vector&);

    // 移动
    vector(vector&&) noexcept;
    vector& operator=(vector&&) noexcept;

    ~vector();

    // 容量操作
    bool empty() const;
    sizeType size() const;
    sizeType capacity() const;

    // 元素访问操作
    // 下标运算符必须是成员函数，返回值一般是元素的引用，一般又返回普通引用和const引用两种
    // 下标运算符作用于常量对象时调用的是const版本，因为常量对象只能调用const成员函数
    value_type& operator[](sizeType);
    const value_type& operator[](sizeType) const;
    value_type& front();
    const value_type& front() const;
    value_type& back();
    const value_type& back() const;

    // push_back / pop_back
    void push_back(const value_type&);
    void pop_back();

    // 迭代器操作
    iterator begin() const { return start; }
    iterator end() const { return finish; }


private:
    // helper function
    void fill_initialize(sizeType, const value_type&);
    void range_init(iterator, iterator);
    void init_space(sizeType, sizeType);

    void reallocate_insert(iterator, const value_type&);
    void free();


};

// 普通构造函数
template<typename T>
vector<T>::vector(sizeType n) {
    fill_initialize(n, T());
}

template<typename T>
vector<T>::vector(sizeType n, const value_type& value) {
    fill_initialize(n, value);
}

template<typename T>
template<typename Iter, typename isInputIter>
vector<T>::vector(Iter first, Iter last) {
    assert(first <= last);
    range_init(first, last);    
}

// 析构函数
template<typename T>
vector<T>::~vector() {
    free();
    start = finish = endOfStorage = nullptr;
}

// 拷贝构造
template<typename T>
vector<T>::vector(const vector& vec) {
    range_init(vec.start, vec.finsh);
}

// 拷贝赋值
template<typename T>
vector<T>& vector<T>::operator=(const vector& vec) {
    if(this != &vec) {
        auto data = data_allocator::allocate(capacity());
        auto newFinish = std::uninitialized_copy(vec.start, vec.finish, data);
        free();
        start = data;
        finish = newFinish;
        endOfStorage = data + vec.capacity();
    }
    return *this;
}

// 移动构造
template<typename T>
vector<T>::vector(vector&& vec) noexcept 
            : start(vec.start),
              finish(vec.finish),
              endOfStorage(vec.endOfStorage) {
    vec.start = nullptr;
    vec.finish = nullptr;
    vec.enOfStorage = nullptr;
}

//移动赋值
template<typename T>
vector<T>& vector<T>::operator=(vector&& vec) noexcept {
    if(this != &vec) {
        free();
        start = vec.start;
        finish = vec.finish;
        endOfStorage = vec.endOfStorage;
        vec.start = vec.finish = vec.endOfStroage = nullptr;
    }
    return *this;
}

// 容量
template<typename T>
bool vector<T>::empty() const {
    return start == finish;
}

template<typename T>
typename vector<T>::sizeType vector<T>::size() const {
    return static_cast<sizeType>(finish - start);
}

template<typename T>
typename vector<T>::sizeType vector<T>::capacity() const {
    return static_cast<sizeType>(endOfStorage - start);
}

// 访问元素操作
template<typename T>
typename vector<T>::value_type& vector<T>::operator[](sizeType n) {
    assert(n >= static_cast<sizeType>(0));
    assert(n < static_cast<sizeType>(finish - start));
    return *(start + n);
}

template<typename T>
const typename vector<T>::value_type& vector<T>::operator[](sizeType n) const {
    assert(n >= static_cast<sizeType>(0));
    assert(n < static_cast<sizeType>(finish - start));
    return *(start + n);    // 返回常量引用
}

template<typename T>
typename vector<T>::value_type& vector<T>::front() {
    assert(!empty());
    return *(start);
}

template<typename T>
const typename vector<T>::value_type& vector<T>::front() const {
    assert(!empty());
    return *(start);
}

template<typename T>
typename vector<T>::value_type& vector<T>::back() {
    assert(!empty());
    return *(finish - 1);
}

template<typename T>
const typename vector<T>::value_type& vector<T>::back() const {
    assert(!empty());
    return *(finish - 1);
}

// push_back
template<typename T>
void vector<T>::push_back(const value_type& value) {
    if(finish != endOfStorage) {
        data_allocator::construct(finish, value);
        finish++;
    } else {
        reallocate_insert(finish, value);
    }
}

// pop_back
template<typename T>
void vector<T>::pop_back() {
    assert(size() != 0);
    data_allocator::destory(--finish);
}



// helper function
template<typename T>
void vector<T>::init_space(sizeType n, sizeType cap) {
    try {
        start = data_allocator::allocate(cap);
        endOfStorage = start + cap;
        finish = start + n;
    } catch (...) {
        start = nullptr;
        endOfStorage = nullptr;
        finish = nullptr;
    }
}

template<typename T>
void vector<T>::fill_initialize(sizeType n, const value_type& value) {
    sizeType cap = std::max(static_cast<sizeType>(8), n);
    init_space(n, cap);
    std::uninitialized_fill_n(start, n, value);
}

template<typename T>
void vector<T>::range_init(iterator first, iterator last) {
    sizeType size = static_cast<sizeType>(last - first);
    sizeType cap = std::max(size, static_cast<sizeType>(8));
    init_space(size, cap);
    std::uninitialized_copy(first, last, start);
}

template<typename T>
void vector<T>::free() {
    data_allocator::destory(start, finish);
    data_allocator::deallocate(start,
                               static_cast<sizeType>(endOfStorage - start));
}

template<typename T>
void vector<T>::reallocate_insert(iterator pos, const value_type& value) {
    sizeType newCapacity = start ? capacity() * 2 : 1;
    auto newStart = data_allocator::allocate(newCapacity);
    auto newFinish = newStart;
    try {
        newFinish = std::uninitialized_copy(start, pos, newStart); // uninitialized_move is support in c17
        data_allocator::construct(newFinish++, value);
        newFinish = std::uninitialized_copy(pos, finish, newFinish);
    } catch(...) {
        data_allocator::deallocate(newStart, newCapacity);
        throw;
    }
    free();
    start = newStart;
    finish = newFinish;
    endOfStorage = start + newCapacity;
}


}   // end of namespace mystl

#endif