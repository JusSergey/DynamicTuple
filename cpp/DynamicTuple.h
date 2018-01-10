#ifndef NOTYPEARRAY_H
#define NOTYPEARRAY_H

#define _DEBUG_DYN_TUPLE_

/* INCLUDE FILES */
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <memory>
#include <vector>
#include <tuple>
/* END INCLUDE FILES */

#ifdef _DEBUG_DYN_TUPLE_
#include <iostream>
#include <string>
#endif

#ifdef _DEBUG_DYN_TUPLE_
# define DEBUG_COUT(ARGS) std::cout ARGS
#else
# define DEBUG_COUT(ARGS)
#endif

#ifdef _DEBUG_DYN_TUPLE_
class DebugTest;
#endif

/* USING */
using std::unique_ptr;
using std::size_t;
using std::pair;

using std::forward;
using std::move;
/* END USING */


/*
 * NOTE FOR g++ clang++ (for compilers which using glibc).
 * IF YOU WAS ALLOCATED A MANY MEMORY, YOU CAN CALL 'malloc_trim(0)', FOR RETURNING THE MEMORY TO THE SYSTEM.
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
    using TypeHashT = decltype(typeid(void).hash_code());
    virtual bool isEqual(void *only_valid_ptr_to_value) const = 0;
    virtual TypeHashT getValueTypeHash() const = 0;
    virtual void setValue(void *ptr_to_value) = 0;
    virtual BasicProxyValue *makeProxyPtr(void *only_valid_ptr) = 0;
    virtual bool operator == (const BasicProxyValue &other) const = 0;
    virtual ~BasicProxyValue() = default;
};

// CLASS IMPLEMENT
class DynamicTuple : private vector_type
{
public:

    template <typename ObjectType>
    struct ProxyValue : public BasicProxyValue {
        friend class DynamicTuple;
        friend class BasicProxyValue;
        ObjectType value;

        // overloads virtual functions
    public:
        virtual BasicProxyValue *makeProxyPtr(void *only_valid_ptr) override
        { return new ProxyValue<ObjectType>(only_valid_ptr); }

        virtual void setValue(void *only_valid_ptr) override
        { value = *reinterpret_cast<const ObjectType*>(only_valid_ptr); }

        virtual BasicProxyValue::TypeHashT getValueTypeHash() const override
        { return typeid(ObjectType).hash_code(); }

        virtual bool isEqual(void *only_valid_ptr_to_value) const override
        {
            return *reinterpret_cast<ObjectType*>(only_valid_ptr_to_value) == value;
        }

        //operators:
    public:
        virtual bool operator == (const BasicProxyValue &bpv) const override
        {
            return getValueTypeHash() == bpv.getValueTypeHash() && isEqual(Helper::getRawPtrToValue(bpv));
        }

    public:

        ProxyValue(void *only_valid_ptr) : value(*reinterpret_cast<ObjectType*>(only_valid_ptr))
        { }

        template <typename ObjectTypeT>
        ProxyValue(ObjectTypeT &&val)
            : value(forward<ObjectTypeT>(val))
        {
//            static_assert(std::is_same< typename std::remove_all_extents<ObjectTypeT>::type,
//                                        typename std::remove_all_extents<ObjectType >::type>::value,
//                          "two types must be same");
        }

        ProxyValue() = default;

        virtual ~ProxyValue() = default;

    };

#ifdef _DEBUG_DYN_TUPLE_
public:
    friend class DebugTest;
#endif
    struct Helper {
        Helper() = delete;
        inline static void *getRawPtrToValue(const BasicProxyValue &bpv) {
            return &reinterpret_cast<ProxyValue<char>*>(const_cast<BasicProxyValue*>(&bpv))->value;
//            return reinterpret_cast<void *>
//                        ( &((reinterpret_cast< ProxyValue<UniquePtr<char>>* >(&bpv))->value.get()) );
        }
    };

public:
    DynamicTuple() = default;

    explicit DynamicTuple(const DynamicTuple &tuple_from_copy) {
        operator = (tuple_from_copy);
    }

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

public:
    // ToDo. replace reint_casts to Helper::getRawPtrToValue();
    /* OVERLOADING OPERATORS */
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
        vector_type::operator = (move(dt));
        return *this;
    }

    bool operator == (const DynamicTuple &other) const {

        if (this->size() != other.size())
            return false;

        for (decltype(size()) i = 0; i < size(); ++i)
            if ( (!this->operator [](i)->operator == (*other[i])) )
                return false;

        return true;
    }

public:
    // todo.

    // set public func;
public:
    using vector_type::size;
    using vector_type::capacity;
    using vector_type::empty;
    using vector_type::reserve;
    using vector_type::resize;

};

#ifdef _DEBUG_DYN_TUPLE_
/*
 * DEBUG_MODULE
*/
#define DEBUG_PRINT std::cout << "[DEBUG]: "

class DebugTest
{
public:
    static void start() {
        DebugTest _debug_test_;
        _debug_test_.Test();
    }
    DebugTest() {
        DynamicTuple dt;
    }

    bool TestIsEqual() {
        DynamicTuple dt1, dt2;

        dt1.emplace<int>(123);
        dt1.emplace<std::string>("hello");
        dt1.emplace<float>(2.3f);

        dt2.emplace<int>(123);
        dt2.emplace<std::string>("Hello");
        dt2.emplace<float>(2.3f);

        return (dt1 == dt2 && dt2 == dt1);
    }
    bool Test() {
        int insertValue = 2788;
        DynamicTuple dt;
        dt.copyToContainer(insertValue);
        DEBUG_PRINT << (insertValue == *(int*)DynamicTuple::Helper::getRawPtrToValue(*&*dt[0]));
        return true;
    }

};

#endif

#endif // NOTYPEARRAY_H
