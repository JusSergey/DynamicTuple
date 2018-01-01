#ifndef NOTYPEARRAY_H
#define NOTYPEARRAY_H

/* INCLUDE FILES */
#include <type_traits>
#include <memory>
#include <vector>
#include <tuple>
/* END INCLUDE FILES */

/* USING */
//using std::unique_ptr;
using std::size_t;
using std::pair;

using std::forward;
using std::move;
/* END USING */

//
// TODO: replace[std::vector<void *>] to [std::vector<std::unique_ptr<void *>>]
//

#include <iostream>

class BasicProxyValue {
public:
    virtual ~BasicProxyValue() { std::cout << "\n BASIC PROXY VALUE \n"; }
};

using vector_type = std::vector<void *>;
//using vt = std::vector<unique_ptr<BasicProxyValue>>;
//using vector_type = vt;
using UniquePTR = std::unique_ptr<BasicProxyValue>;

// CLASS IMPLEMENT
class DynamicTuple : private vector_type
{
public:


    template <typename ObjectType>
    struct ProxyValue : public BasicProxyValue {
    public:
        ObjectType *value;

        ProxyValue(ObjectType *ptr_value = nullptr)
            : value(ptr_value) {}

        virtual ~ProxyValue() {
            std::cout << "\n Proxy Value \n";
            delete reinterpret_cast<ObjectType *>(value);
        }

    };

public:
    DynamicTuple() = default;

    class A { public: virtual ~A(){ std::cout << " \ndest A\n"; }};
    class B : public A { public: ~B(){ std::cout << "\ndest B\n"; } };

    static void F() {
        BasicProxyValue *p = new ProxyValue<float>();
        delete p;
    }

    template <typename ObjectType, typename... ArgsToConstruct>
    void emplace(ArgsToConstruct&& ...args) {

        push_back(reinterpret_cast<void *>(new ObjectType(forward<ArgsToConstruct>(args)...)));
    }

//    template <typename ObjectType>
//    void copyToContainer(const ObjectType &obj) {
//        push_back(reinterpret_cast<void *>(new ObjectType(obj)));
//    }

//    template <typename ObjectType>
//    void moveToContainer(ObjectType &&obj) {
//        push_back(reinterpret_cast<void *>(new ObjectType(move(obj))));
//    }

//    template <typename ObjectType>
//    void forwardToContainer(ObjectType &&rlval) {
//        push_back(reinterpret_cast<void *>(new ObjectType(forward<ObjectType>(rlval))));
//    }

    template <typename ObjectType>
    ObjectType &get(size_t index) {
        return *reinterpret_cast<ObjectType*>(vector_type::operator [](index));
    }

    template <typename ObjectType>
    ObjectType getCopy(size_t index) {
        return *reinterpret_cast<ObjectType*>(vector_type::operator [](index));
    }

    // set public func;
public:
    using vector_type::size;
    using vector_type::capacity;
    using vector_type::empty;
    using vector_type::reserve;

//    inline size_t size() const { return vector_type::size(); }
//    inline size_t capacity() const { return vector_type::capacity(); }
};

#endif // NOTYPEARRAY_H
