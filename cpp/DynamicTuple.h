#ifndef NOTYPEARRAY_H
#define NOTYPEARRAY_H

/* INCLUDE FILES */
#include <type_traits>
#include <utility>
#include <memory>
#include <vector>
#include <tuple>
/* END INCLUDE FILES */
#include <iostream>
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

class BasicProxyValue;

template <typename T>
using UniquePtr = std::unique_ptr<T>;

using UniquePtrBPV = UniquePtr<BasicProxyValue>;
using vector_type = std::vector<UniquePtrBPV>;

// Helper for implement destructor for different types
class BasicProxyValue {
public:
    virtual void setValue(void *ptr_to_value) = 0;
    virtual BasicProxyValue *makeProxyPtr(void *only_valid_ptr) = 0;
    virtual ~BasicProxyValue() = default;
};

// CLASS IMPLEMENT
class DynamicTuple : private vector_type
{
public:


    template <typename ObjectType>
    struct ProxyValue : public BasicProxyValue {
    public:
        ObjectType value;

        virtual BasicProxyValue *makeProxyPtr(void *only_valid_ptr) override {
            return new ProxyValue<ObjectType>(only_valid_ptr);
        }

        virtual void setValue(void *only_valid_ptr) override {
            value = *reinterpret_cast<const ObjectType*>(only_valid_ptr);
        }

        ProxyValue(void *only_valid_ptr) : value(*reinterpret_cast<ObjectType*>(only_valid_ptr))
        { }

        template <typename ObjectTypeT>
        ProxyValue(ObjectTypeT &&val)
            : value(forward<ObjectTypeT>(val))
        {
//            std::cout << "value == " << val << " " << value << '\n';
            static_assert(std::is_same< typename std::remove_all_extents<ObjectTypeT>::type,
                                        typename std::remove_all_extents<ObjectType >::type>::value,
                          "two types must be same");
        }

        ProxyValue() = default;

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
    ObjectType getCopy(size_t index) const {
        return reinterpret_cast<ProxyValue<ObjectType>*>( (vector_type::operator [](index)).get() )->value;
    }

    /* OVERLOADING OPERATORS */
    /*
    template <typename DynamicTupleT,
              typename = typename std::enable_if<
                  std::is_same<
                      std::remove_reference_t<
                          std::remove_cv_t<DynamicTupleT>
                      >,
                      DynamicTupleT>
                  ::value
              ::type>
             >
    DynamicTuple &operator = (DynamicTupleT &&dt) {
        resize(dt.size());
        for (int i = 0; i < size(); ++i)
            operator [](i) = move(dt[i]);
        return *this;
    }
    */
    DynamicTuple &operator = (const DynamicTuple &tuple_from_copy) {
        for (size_type i = 0; i < tuple_from_copy.size(); ++i) {
            auto casting_to_need_type = reinterpret_cast<ProxyValue<UniquePtr<void>> *>(&(*tuple_from_copy[i]));
            auto ptr_to_raw_data = reinterpret_cast<void *>(&casting_to_need_type->value);
            auto making_need_obj_for_vector = UniquePtrBPV(tuple_from_copy[i]->makeProxyPtr(ptr_to_raw_data));
            push_back(move(making_need_obj_for_vector));
        }
        return *this;
    }

    DynamicTuple &operator = (DynamicTuple &&dt) {
        resize(dt.size());
        for (size_type i = 0; i < dt.size(); ++i){
            this->operator [](i) = move(dt[i]);
        }
        dt.resize(0);
        return *this;
    }

    // set public func;
public:
//    ~DynamicTuple() {}
    using vector_type::size;
    using vector_type::capacity;
    using vector_type::empty;
    using vector_type::reserve;
    using vector_type::resize;

};

#endif // NOTYPEARRAY_H
