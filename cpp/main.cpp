#include <iostream>
#include <string>
#include "DynamicTuple.h"
#include "test.h"
using namespace std;

template <typename value_t>
class MyClass {
    value_t value = static_cast<value_t>(0);
public:
    MyClass() = default;
    MyClass(value_t val) : value(val){}
    void setValue(value_t v) {
        value = v;
    }
    operator string () const {
        return string ("it is string***") + std::to_string(value) + "***end";
    }
};

void printdt(const DynamicTuple &dt, size_t len, const char *name = "") {
    cout << "\n~~~~~~" << name << "~~~~~~\n";
    for (size_t i = 0; i < len; ++i)
        cout << dt.getCopy<int>(i) << ' ';
    cout.put('\n');
}

#define PrintBlock(DT1, name1, DT2, name2) \
    cout << "\n###########################\n"; \
    printdt(DT1, DT1.size(), name1); \
    printdt(DT2, DT2.size(), name2);

#define FastPrint PrintBlock(dt1, "dt1", dt2, "dt2");

int main()
{
    DynamicTuple dt1, dt2;

    dt1.emplace<int>(8);
    dt2.emplace<int>(8);



    cout << "dt1 == dt2 : " << (dt1 == dt2) << (dt2 == dt1) << '\n';

    DynamicTuple::ProxyValue<char> pv('q');

    void *ptr = DynamicTuple::Helper::getRawPtrToValue(pv);

    cout << *(reinterpret_cast<char*>(ptr)) << '\n';

    cin.get();

    return 0;
}

