#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int main()
{
    std::mutex m;
    std::condition_variable cv;

    std::unique_lock<std::mutex> lock(m);

    auto const start = std::chrono::steady_clock::now();
    auto const result = cv.wait_for(lock, std::chrono::seconds(2));
    auto const finish = std::chrono::steady_clock::now();

    auto const elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);

    if (result == std::cv_status::timeout)
        std::cout << "Timeout after " << elapsed_ms.count() << "ms\n";
    else
        std::cout << "Awoken after " << elapsed_ms.count() << "ms\n";

    return 0;
}

