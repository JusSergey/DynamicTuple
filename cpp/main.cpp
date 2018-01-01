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

int main()
{
    auto msec2 = computeTimeForTask([]{
        vector<string> vec;
        for (int i = 0; i < 1e7; ++i) {
            vec.push_back("Hello, world");
        }
    });

    cout << "time for task: " << msec2 << "msec\n";

    auto msec1 = computeTimeForTask([]{
        DynamicTuple vec;
        for (int i = 0; i < 1e7; ++i) {
            vec.emplace<string>("Hello, world");
        }
    });
    cout << "time for task: " << msec1 << "msec\n";



    cout << "diff: " << (((long double)(msec1)) / msec2) << '\n';

    // CPU: Intel(R) Core(TM)2 Duo CPU     P8600  @ 2.40GHz
    // RAM: SODIMM DDR2 Synchronous 800 MHz (1,2 ns) 4GiB

//    PERFORMANCE RAM
//    string: "Hello, world"
//    stat = 307MB
//    dyn  = 534MB
//    ~1.739

//    int: i
//    stat = 40  MB
//    dyn  = 383 MB
//    ~9.575

//    PERFORMANCE CPU
//    string: "Hello, world"
//    stat = 2132 ms
//    dyn  = 6522 ms
//    ~3.059

//    int: i
//    stat = 514  ms
//    dyn  = 6352 ms
//    ~12.358



    return 0;
}

