#include <iostream>
#include <string>
#include "DynamicTuple.h".h"
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
    ~MyClass() {
        cout << "MyClass Destructed...\n";
    }
};

int main()
{
    DynamicTuple::F();

    {
        DynamicTuple t;

        auto il = {1, 2, 36, 5, 3, 4};
        t.emplace<vector<int>>(il);
        t.emplace<float>(123.5f);
        t.emplace<string>("Hello, my code");

        for (const auto &vec : t.get<vector<int>>(0))
            cout << vec << ' ';

        cout << '\n';

        cout << t.get<float>(1) << '\n';

        for (char ch : t.get<string>(2))
            cout << ch << ' ';
        cout << '\n';
//        for (int i = 0; i < 100000000; ++i){
//            t.emplace<MyClass<int>>(1);
//            void *p (reinterpret_cast<void *> (&t.get<MyClass<int>>(4)));
////            delete []p;
//        }
    }
    cin.get();
    cout.flush();


    return 0;
}
