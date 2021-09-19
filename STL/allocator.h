#ifndef ALLOCATOR_H
#define ALLOCATOR_H

// #include <memory>
// #include <type_traits>

#include "type_traits.h"
#include "construct.h"

namespace mystl{


template<typename T>
class allocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* constPointer;
    typedef T& reference;
    typedef const T& constReference;
    typedef size_t sizeType;
    typedef ptrdiff_t differenceType;

public:
    pointer allocate();
    pointer allocate(sizeType);

    void deallocate(pointer);
    void deallocate(pointer, sizeType);

    void construct(pointer);
    void construct(pointer, const T&);
    void construct(pointer, T&&);
    template<typename... Args>
    void construct(pointer, Args&&...);

    void destory(pointer);
    void destory(T*, T*);

};


template <typename T>
T* allocator<T>::allocate() {
    return static_cast<T*>(::operator new(sizeof(T)));
}

template <typename T>
T* allocator<T>::allocate(size_t n) {
    //TODO n==0
    return static_cast<T*>(::operator new(n));
}

template <typename T>
void allocator<T>::deallocate(T* ptr) {
    if(ptr == nullptr) return;
    ::operator delete(ptr);
}

template <typename T>
void allocator<T>::deallocate(T* ptr, size_t n) {
    if(ptr == nullptr || n == 0) return;
    ::operator delete(ptr, n);  // void operator delete  ( void* ptr, std::size_t sz ) noexcept;(since C++14)
}

template <typename T>
void allocator<T>::construct(T* ptr) {
    new(ptr) T();
}

template <typename T>
void allocator<T>::construct(T* ptr, const T& t) {
    new(ptr) T(t);
}

template <typename T>
void allocator<T>::construct(T* ptr, T&& t) {
    new(ptr) T(std::forward<T>(t));
}

template <typename T>
template <typename... Args>
void allocator<T>::construct(T* ptr, Args&&... args) {
    new(ptr) T(std::forward<Args>(args)...);
}

template <typename T>
void allocator<T>::destory(T* t) {
    mystl::destory(t);
}

template <typename T>
void allocator<T>::destory(T* begin, T* end) {
    mystl::destory(begin, end);
}

} // end of namespace mystl

#endif