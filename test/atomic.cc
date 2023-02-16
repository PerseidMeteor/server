#include "atomic"
#include "future"
#include "iostream"
#include "mutex"

std::atomic<std::int64_t> atomic_count_(0);
std::int64_t free_count_(0);
std::int64_t mutex_count_(0);

std::mutex mutex;

auto add = [](int i)
{
    for (int j = 0; j < i; j++)
    {
        atomic_count_++;
        free_count_++;
        std::unique_lock<std::mutex> lock{mutex};
        mutex_count_++;
    }
};

auto del = [](int i)
{
    for (int j = 0; j < i; j++)
    {
        atomic_count_--;
        free_count_--;
        std::unique_lock<std::mutex> lock{mutex};
        mutex_count_--;
    }
};

int main()
{

    {
        auto th1 = std::async(add, 10000);
        auto th2 = std::async(del, 10000);
    }

    std::cout << atomic_count_ << "   ";
    std::printf("%ld   %ld\n", free_count_, mutex_count_);

    return 0;
}