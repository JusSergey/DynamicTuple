#ifndef TEST_H
#define TEST_H

#include <chrono>
#include <utility>
using std::chrono::steady_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using MSec = int64_t;

template <typename CallType, typename ...Args>
static MSec computeTimeForTask(CallType&& callobj, Args&& ...args) {

    auto start_point = steady_clock::now();
    std::forward<CallType>(callobj)(std::forward<Args>(args)...);
    auto end_point = steady_clock::now();

    return duration_cast<milliseconds>(end_point - start_point).count();

}

#endif // TEST_H
