# DynamicTuple
It is a dynamic array which free types.
It is container, which cat containting objects different types, and different lenght.

# Example

    #include <iostream>
    #include <string>
    #include "DynamicTuple.h"
    
    using namespace std;
    
    int main() {

        DynamicTuple dynt;
        dynt.emplace<int>(8);
        dynt.emplace<string>("Hello, world");

        std::initializer_list<int> some_vec{1, 4, 3, 5};
        dynt.emplace<vector<int>>(some_vec);

        int i = 0;
        cout << "int: " << dynt.get<int>(i++) << '\n';
        cout << "string: " << dynt.get<string>(i++) << '\n';

        for (int k : dynt.get<vector<int>>(i))
            cout << "vec elem: " << k << '\n';
        cout << '\n';
  
        return 0
    }

# Output
    int: 8
    string: Hello, world
    vec elem: 1
    vec elem: 4
    vec elem: 3
    vec elem: 5

# Test Performance

    ALL TESTS ADDING 10.000.000 elements

    HARDWARE: 
    CPU: Intel(R) Core(TM)2 Duo CPU     P8600  @ 2.40GHz
    RAM: SODIMM DDR2 Synchronous 800 MHz (1,2 ns) 4GiB

    PERFORMANCE RAM
    stat = std::vector<std::string>;
    string: "Hello, world"
    stat = 307MB
    dyn  = 534MB
    ~1.739
    
    stat = std::vector<int>;
    int: i
    stat = 40  MB
    dyn  = 383 MB
    ~9.575

    PERFORMANCE CPU
    stat = std::vector<std::string>;
    string: "Hello, world"
    stat = 2132 ms
    dyn  = 6522 ms
    ~3.059

    stat = std::vector<int>;
    int: i
    stat = 514  ms
    dyn  = 6352 ms
    ~12.358
