#ifndef VECTOR_H
#define VECTOR_H

#include "allocator.h"
#include "algobase.h"
#include "uninitialized.h"

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
    // 这里为什么用模板的 https://www.zhihu.com/question/62552068
    // 防止和上一个构造函数冲突 -> vec(5, 10)
    // 解决方法就是判断是否是InputIterator
    // 传入普通函数与模板函数的参数类型与之相匹配时，会优先寻找参数完全匹配的普通函数并调用它
    // 当没有找到参数完全匹配的普通函数时会寻找一个函数模板，将其实例化产生一个匹配的模板函数并调用它
    template<typename Iter,
             typename = mystl::_RequireInputIter<Iter>>
    vector(Iter first, Iter last) {
        assert(first <= last);
        range_init(first, last);   
    }
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
    // insert
    iterator insert(constIterator, const value_type&);
    iterator insert(constIterator, sizeType, const value_type&);

    // emplace / emplace_back
    template<typename... Args>
    iterator emplace(constIterator, Args&& ...);
    template<typename... Args>
    void emplace_back(Args&& ...);

    // erase / clear
    iterator erase(constIterator);
    iterator erase(constIterator, constIterator);
    // clear 操作要注意他只析构，并不会释放内存，所以clear之后不要用索引访问元素
    void clear() { erase(begin(), end()); }


    // 迭代器操作
    iterator begin() const { return start; }
    iterator end() const { return finish; }
    constIterator cbegin() const { return start; }
    constIterator cend() const { return finish; }


private:
    // helper function
    void fill_initialize(sizeType, const value_type&);
    void range_init(iterator, iterator);
    void init_space(sizeType, sizeType);

    void reallocate_insert(iterator, const value_type&);
    template<typename... Args>
    void reallocate_emplace(iterator, Args&&...);
    void fill_insert(iterator, sizeType, const value_type&);
    void free();
    void destoryAndDeallocate(iterator, iterator, sizeType);


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

// template<typename T>
// template<typename Iter, typename isInputIter>
// vector<T>::vector(Iter first, Iter last) {
//     assert(first <= last);
//     range_init(first, last);    
// }

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
    // [] 不检查元素越界
    // assert(n >= static_cast<sizeType>(0));
    // assert(n < capacity());
    return *(start + n);
}

template<typename T>
const typename vector<T>::value_type& vector<T>::operator[](sizeType n) const {
    // assert(n >= static_cast<sizeType>(0));
    // assert(n < capacity());
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

// insert
template<typename T>
typename vector<T>::iterator
vector<T>::insert(constIterator cpos, const value_type& value) {
    assert(cpos >= cbegin() && cpos <= cend());
    sizeType offset = cpos - cbegin();
    // 这里为什么要拷贝一份non-const的pos因为reallocate_insert不接受constIterator
    // 一个const 实参无法绑定到一个非const的形参
    // 详见《c++ primer》 p192
    iterator pos = begin() + offset;
    if(finish != endOfStorage && pos == end()) {
        data_allocator::construct(finish++, value);
    } else if(finish != endOfStorage) {
        data_allocator::construct(finish, *(finish - 1));
        mystl::copy_backward(pos, finish - 1, finish);
        *pos = value;
        ++finish;
    } else {
        reallocate_insert(pos, value);
    }
    return start + offset;
}

template<typename T>
typename vector<T>::iterator
vector<T>::insert(constIterator cpos, sizeType n, const value_type& value) {
    assert(cpos >= cbegin() && cpos <= cend());
    sizeType offset = cpos - cbegin();
    iterator pos = begin() + (cpos - cbegin());
    fill_insert(pos, n, value);
    return start + offset;
}

// emplace
template<typename T>
template<typename... Args>
typename vector<T>::iterator vector<T>::emplace(constIterator cpos, Args&&... args) {
    assert(cpos >= cbegin() && cpos <= cend());
    sizeType offset = cpos - cbegin();
    iterator pos = start + offset;
    if(finish != endOfStorage && pos == finish) {
        data_allocator::construct(finish++, std::forward<Args>(args)...);
    } else if(finish != endOfStorage) {
        data_allocator::construct(finish, *(finish - 1));
        mystl::move_backward(pos, finish - 1, finish);
        *pos = value_type(std::forward<Args>(args)...);
        ++finish;
    } else 
        reallocate_emplace(pos, std::forward<Args>(args)...);
    return start + offset;
}

// emplace_back()
template<typename T>
template<typename... Args>
void vector<T>::emplace_back(Args&&... args) {
    if(finish != endOfStorage) {
        data_allocator::construct(finish++, std::forward<Args>(args)...);
    } else {
        reallocate_emplace(finish, std::forward<Args>(args)...);
    }
}

// erase
template<typename T>
typename vector<T>::iterator vector<T>::erase(constIterator cpos) {
    assert(cpos >= cbegin() && cpos <= cend());
    iterator pos = start + (cpos - cbegin());
    std::move(pos + 1, finish, pos);
    data_allocator::destory(--finish);
    return pos;
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(constIterator cfirst, constIterator clast) {
    assert(cfirst >= start && clast <= finish && cfirst <= clast);
    sizeType n = cfirst - cbegin();
    iterator first = start + n;
    data_allocator::destory(std::move(first + (clast - cfirst), finish, first),
                            finish);
    finish -= (clast - cfirst);
    return start + n;
}


/*************************************************************************************/
// helper function                                                                    /
/*************************************************************************************/
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
void vector<T>::destoryAndDeallocate(iterator _start, iterator _finish, sizeType n) {
    data_allocator::destory(_start, _finish);
    data_allocator::deallocate(_start, n);
}

template<typename T>
void vector<T>::free() {
    destoryAndDeallocate(start, finish,
                         static_cast<sizeType>(endOfStorage - start));
}

template<typename T>
void vector<T>::reallocate_insert(iterator pos, const value_type& value) {
    sizeType newCapacity = size() ? capacity() << 1 : 1;
    auto newStart = data_allocator::allocate(newCapacity);
    auto newFinish = newStart;
    try {
        newFinish = std::uninitialized_copy(start, pos, newStart); // uninitialized_move is support in c17
        data_allocator::construct(newFinish++, value);
        newFinish = std::uninitialized_copy(pos, finish, newFinish);
    } catch(...) {
        // TODO 这里不调用destory直接调用deallocate会造成内存泄漏吗? 会
        destoryAndDeallocate(newStart, newFinish, newCapacity);
        throw;
    }
    free();
    start = newStart;
    finish = newFinish;
    endOfStorage = newStart + newCapacity;
}

template<typename T>
template<typename... Args>
void vector<T>::reallocate_emplace(iterator pos, Args&&... args) {
    sizeType newCapacity = size() ? capacity() << 1 : 1;
    auto newStart = data_allocator::allocate(newCapacity);
    auto newFinish = newStart;
    try {
        newFinish = mystl::uninitialized_move(start, pos, newStart); // std::uninitialized_move is support in c17
        data_allocator::construct(newFinish++, std::forward<Args>(args)...);
        newFinish = mystl::uninitialized_move(pos, finish, newFinish);
    } catch(...) {
        destoryAndDeallocate(newStart, newFinish, newCapacity);
        throw;
    }
    free();
    start = newStart;
    finish = newFinish;
    endOfStorage = newStart + newCapacity;
}

template<typename T>
void
vector<T>::fill_insert(iterator pos, sizeType n, const value_type& value) {
    if(n) {
        if(endOfStorage - finish >= n) {
            const sizeType afterElem = finish - pos;
            auto newFinish = finish;
            if(afterElem > n) {
                newFinish = std::uninitialized_copy(finish - n, finish, finish);
                mystl::move_backward(pos, finish - n, finish);
                std::uninitialized_fill_n(pos, n, value);
                finish = newFinish;
            } else {
                newFinish = std::uninitialized_fill_n(finish, n - afterElem, value);
                newFinish = std::uninitialized_copy(pos, finish, newFinish);    // TODO move
                std::uninitialized_fill_n(pos, afterElem, value);
                finish = newFinish;
            }
        } else {
            sizeType newCapacity = size() ? capacity() * 2 : 1;
            auto newStart = data_allocator::allocate(newCapacity);
            auto newFinish = newStart;
            try {
                newFinish = mystl::uninitialized_move(start, pos, newStart);
                newFinish = std::uninitialized_fill_n(newFinish, n, value);
                newFinish = mystl::uninitialized_move(pos, finish, newFinish);
            } catch(...) {
                destoryAndDeallocate(newStart,newFinish, newCapacity);
                throw;
            }
            free();
            start = newStart;
            finish = newFinish;
            endOfStorage = start + newCapacity;
        }
    }
}


}   // end of namespace mystl

#endif