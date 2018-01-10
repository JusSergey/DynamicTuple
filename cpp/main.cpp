#include <iostream>
#include <string>
#include "DynamicTuple.h"
#include "test.h"
using namespace std;

int main()
{
    DebugTest::start();

    DynamicTuple dt;

    dt.copyToContainer(123);
    dt.copyToContainer(string("hello"));
    dt.copyToContainer(dt);

    cout << dt.get<int>(0) << '\n';

    cin.get();

    return 0;
}

