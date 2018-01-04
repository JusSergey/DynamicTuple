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

    for (int i = 0; i < 10; ++i)
        dt1.emplace<int>(i);

    FastPrint;
    dt2 = dt1;
    FastPrint;
    dt1 = move(dt2);
    FastPrint;
    dt2 = move(dt1);
    FastPrint

    cin.get();

    return 0;
}

