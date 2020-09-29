#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#ifdef BOOST_TEST
#include "boost/thread.hpp"
#endif

int main()
{
#ifdef BOOST_TEST
    using namespace boost;
#else
    using namespace std;
#endif
    mutex m;
    condition_variable cv;

    unique_lock<mutex> lock(m);

    auto const start = chrono::steady_clock::now();
    auto const result = cv.wait_for(lock, chrono::seconds(2));
    auto const finish = chrono::steady_clock::now();

    auto const elapsed_ms = chrono::duration_cast<chrono::milliseconds>(finish - start);

    if (result == cv_status::timeout)
        std::cout << "Timeout after " << elapsed_ms.count() << "ms\n";
    else
        std::cout << "Awoken after " << elapsed_ms.count() << "ms\n";

    return 0;
}

