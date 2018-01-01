#ifndef NOTYPEARRAY_H
#define NOTYPEARRAY_H

/* INCLUDE FILES */
#include <type_traits>
#include <utility>
#include <memory>
#include <vector>
#include <tuple>
/* END INCLUDE FILES */

/* USING */
using std::unique_ptr;
using std::size_t;
using std::pair;

using std::forward;
using std::move;
/* END USING */


/*
 * NOTE FOR g++ clang++ (for compilers which using glibc).
 * IF YOU WAS ALLOCATED A MANY MEMORY, YOU CAN CALL 'malloc_trim(0)', FOR RETURNING THE MEMORY TO THE SYSTEM
 *
 */

// Helper for implement destructor for different types
class BasicProxyValue {
public:
    virtual ~BasicProxyValue() = default;
};

using UniquePtrBPV = std::unique_ptr<BasicProxyValue>;
using vector_type = std::vector<UniquePtrBPV>;


// CLASS IMPLEMENT
class DynamicTuple : private vector_type
{
public:


    template <typename ObjectType>
    struct ProxyValue : public BasicProxyValue {
    public:
        ObjectType value;

        template <typename ObjectTypeT>
        ProxyValue(ObjectTypeT &&val)
            : value(forward<ObjectTypeT>(val))
        {
//            static_assert(std::is_same< typename std::remove_all_extents<ObjectTypeT>::type,
//                                        typename std::remove_all_extents<ObjectType >::type>::value,
//                          "two types must be same");
        }

        ProxyValue() = delete;

        virtual ~ProxyValue() = default;

    };

public:
    DynamicTuple() = default;

    template <typename ObjectType, typename... ArgsToConstruct>
    void emplace(ArgsToConstruct&& ...args) {
        push_back( UniquePtrBPV(new ProxyValue<ObjectType>(ObjectType(forward<ArgsToConstruct>(args)...))) );
    }

    template <typename ObjectType>
    void copyToContainer(const ObjectType &lvalue) {
        push_back( UniquePtrBPV(new ProxyValue<ObjectType>( lvalue )) );
    }

    template <typename ObjectType>
    void moveToContainer(ObjectType &&rvalue) {
        push_back( UniquePtrBPV(new ProxyValue<ObjectType>( move(rvalue) )) );
    }

    template <typename ObjectType>
    void forwardToContainer(ObjectType &&univalue) {
        push_back( UniquePtrBPV(new ProxyValue<ObjectType>( forward<ObjectType>(univalue) )) );
    }

    template <typename ObjectType>
    inline ObjectType &get(size_t index) {
        return reinterpret_cast<ProxyValue<ObjectType>*>( (vector_type::operator [](index)).get() )->value;
    }

    template <typename ObjectType>
    ObjectType getCopy(size_t index) {
        return reinterpret_cast<ProxyValue<ObjectType>*>( (vector_type::operator [](index)).get() )->value;
    }

    // set public func;
public:
    using vector_type::size;
    using vector_type::capacity;
    using vector_type::empty;
    using vector_type::reserve;
    using vector_type::resize;

};

#endif // NOTYPEARRAY_H
