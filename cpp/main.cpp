#include <iostream>
#include <string>
#include "DynamicTuple.h"
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

    {
        DynamicTuple t;

        auto il = {1, 2, 36, 5, 3, 4};
        t.emplace<vector<int>>(il); // №1
        t.emplace<float>(123.5f); // №2
        t.emplace<string>("Hello, my code"); // №3


        // print vector<int> [№1]
        for (const auto &vec : t.get<vector<int>>(0))
            cout << vec << ' ';
        cout << '\n';

        // print float [№2]
        cout << t.get<float>(1) << '\n';

        // print string [№3]
        for (char ch : t.get<string>(2))
            cout << ch << ' ';
        cout << '\n';

        DynamicTuple t2;
        t2.emplace<string>("other DynamicTuple, in the DynamicTuple");
        // №4
        t.moveToContainer<DynamicTuple>(move(t2));

        // print other DynamicTuple [№3]
        for (char ch : t.get<DynamicTuple>(3).get<string>(0))
            cout << ch;
        cout.put('\n');

    }

    cin.get();
    cout.flush();

    return 0;
}

