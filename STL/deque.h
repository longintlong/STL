#ifndef DEQUE_H
#define DEQUE_H

#include "allocator.h"
#include "algobase.h"
#include "uninitialized.h"

#include <memory>
#include <assert.h>

#ifndef DEQUE_MAP_SIZE
#define DEQUE_MAP_SIZE 8
#endif


namespace mystl {

#ifndef DEQUE_BUF_SIZE
#define DEQUE_BUF_SIZE 512
#endif

/**
 * @brief This function controls the size of memory nodes
 *        Return the number (not byte size) of elements per node.
 * @param __size 
 * @return constexpr size_t 
 */
constexpr inline size_t deque_buf_size(size_t __size) {
    return (__size < DEQUE_BUF_SIZE
            ? size_t(DEQUE_BUF_SIZE / __size) : size_t(1));
}

/**
 * @brief deque的迭代器
 * 
 */
template<typename T, typename Ref, typename Ptr>
struct dequeIterator : public iterator<random_access_iterator_tag, T> {
    typedef dequeIterator<T, T&, T*>            iterator;
    typedef dequeIterator<T, const T&, const T*>      constIterator;
    typedef dequeIterator                       self;

    typedef T                                   value_type;
    typedef Ptr                                 pointer;
    typedef Ref                                 reference;
    typedef size_t                              sizeType;
    typedef ptrdiff_t                           differenceType;
    typedef T*                                  valuePointer;
    typedef T**                                 mapPointer;

    static sizeType buffer_size() { return deque_buf_size(sizeof(T)); }

    // 迭代器4个的数据成员
    valuePointer                                cur;
    valuePointer                                first;
    valuePointer                                last;
    mapPointer                                  node;

    // 构造
    dequeIterator() noexcept
    : cur(nullptr),first(nullptr), last(nullptr), node(nullptr) {}
    
    dequeIterator(valuePointer v, mapPointer m) noexcept
    : cur(v), first(*m), last(*m + buffer_size()), node(m) {}

    dequeIterator(const iterator& di)
    : cur(di.cur), first(di.first), last(di.last), node(di.node) {}

    // 重载 *、->、++、-- []
    reference operator*() const { *cur; }
    reference operator->() const { return cur; }
    // 前置++ 返回引用
    self& operator++() {
        if(cur == last) {
            set_node(node - 1);
            cur = first;
        }
        ++cur;
        return *this;
    }
    self& operator--() {
        if(cur == first) {
            set_node(node + 1);
            cur = last;
        }
        --cur;
        return *this;
    }

    // 后置++ 返回拷贝
    self operator++(int) {
        self temp = *this;
        ++*this;
        return temp;
    }
    self operator--(int) {
        self temp = *this;
        --*this;
        return temp;
    }

    self& operator[](differenceType n) { return *(*this + n); }


    // 重载 += + -= -
    self& operator+=(differenceType n) {
        const differenceType offset = n + (differenceType)(cur - first);
        if(offset >= 0 && offset < buffer_size()) {
            cur += n;
        } else {
            const differenceType nodeOffset = (offset > 0)
                ? offset / buffer_size() : -((-offset - 1) 
                                           / buffer_size()) - 1;
            set_node(node + nodeOffset);
            cur = first + (offset - nodeOffset * buffer_size());
        }
        return *this;
    }

    self operator+(differenceType n) const {
        self temp = *this;
        return temp += n;
    }

    self& operator-=(differenceType n) {
        return *this += -n;
    }

    self operator-(differenceType n) const {
        self temp = *this;
        return temp += -n;
    }

    // 转换缓冲区
    void set_node(mapPointer newNode) {
        node = newNode;
        first = *newNode;
        last = *newNode + differenceType(buffer_size());
    }

};

/**
 * @brief deque 模板类
 * 
 * @tparam T 
 */
template<typename T>
class deque {
public:
    // deque 型别定义
    typedef mystl::allocator<T>                             data_allocator;
    typedef mystl::allocator<T*>                            map_allocator;
    typedef typename data_allocator::value_type             valueType;
    typedef typename data_allocator::pointer                pointer;
    typedef typename data_allocator::constPointer           constPointer;
    typedef typename data_allocator::reference              reference;
    typedef typename data_allocator::constReference         constReference;
    typedef typename data_allocator::sizeType               sizeType;
    typedef typename data_allocator::differenceType         differenceType;
    typedef pointer*                                        mapPointer;
    typedef const pointer*                                  constMapPointer;

    typedef dequeIterator<T, T&, T*>                        iterator;
    // TODO constIterator 这里为什么const是写在里面的 libstd也是这样做的
    typedef dequeIterator<T, const T&, const T*>            constIterator;

    static sizeType buffer_size() { return deque_buf_size(sizeof(T)); }

private:
    iterator    start;         // 第一个缓冲区
    iterator    finish;        // 最后一个缓冲区
    mapPointer  __map;         // 指向map(一块连续空间，每个元素都是一个指针指向缓冲区)  
    sizeType    mapSize;       // map内有多少指针

public:
    // 普通构造函数
    deque();
    explicit deque(sizeType n) {    // explicit阻止了参数 n 向deque的隐式转化

    }


    // helper function
    void fill_init(sizeType, const valueType&);
    mapPointer allocate_map(size_t);
    void allocate_buffer(mapPointer, mapPointer);
    void deallocate_buffer(mapPointer, mapPointer);
    void map_init(sizeType);

};


/***********************************************************************
 *                                                                     |
 * helper function                                                     |
 *                                                                     |
 **********************************************************************/
template<typename T>
deque<T>::mapPointer deque<T>::allocate_map(size_t n) {
   mapPointer mp = map_allocator::allocate(n);
   // 将所有的指针(指向每个缓冲区的开始)置null
   for(size_t i = 0; i < n; ++i) {
       *(mp + i) = nullptr;
   }
   return mp;
}

template<typename T>
void deque<T>::deallocate_buffer(mapPointer nstart, mapPointer nfinish) {
    mapPointer cur = nfinish;
    while(cur != nstart) {
        data_allocator::deallocate(*(--cur), buffer_size(sizeof(T)));
        *cur = nullptr;
    }
}

template<typename T>
void deque<T>::allocate_buffer(mapPointer nstart, mapPointer nfinish) {
    mapPointer cur = nstart;
    try {
        for(; cur != nfinish; ++cur) {
            *cur = data_allocator::allocate(buffer_size(sizeof(T)));
        }
    } catch(...) {
        deallocate_buffer(nstart, cur);
        throw;
    }
}

template<typename T>
void deque<T>::map_init(sizeType nElem) {
    const size_t numNodes = nElem / buffer_size(sizeof(T)) + 1;
    mapSize = std::max(static_cast<size_t>(DEQUE_MAP_SIZE), numNodes + 2);
    __map = allocate_map(mapSize);

    // 对于numNodes < mapSize TODO : 下面两句还没明白
    mapPointer nstart = __map + (mapSize - numNodes) / 2;
    mapPointer nfinish = nstart + numNodes;

    try {
        allocate_buffer(nstart, nfinish);
    } catch(...) {
        map_allocator::deallocate(__map, mapSize);
        __map = nullptr;
        mapSize = 0;    //  记住mapSize也要置0 (指针 + size)
        throw;
    }

    


}

template<typename T>
void deque<T>::fill_init(sizeType n, const valueType& value) {

}

}   //  end of namespace mystl


#endif
