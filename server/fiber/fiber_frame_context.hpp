#ifndef FIBER_FRAME_CONTEXT_HPP
#define FIBER_FRAME_CONTEXT_HPP

#include <boost/fiber/all.hpp>
#include <mutex>
#include <thread>
#include <vector>

// 只能有一个实例,并且要在main入口处理调用 init() 在main返回前调用 wait()
// 如果想要停止并退出进程,需要调用 notify_stop()
class FiberFrameContext
{
public:
    int run_thread_count = 2; // 开启的线程数量
    static FiberFrameContext &instance();

    void init();

    void notify_stop();

    void wait();

private:
    FiberFrameContext() = default;
    FiberFrameContext(const FiberFrameContext &rhs) = delete;
    FiberFrameContext(FiberFrameContext &&rhs) = delete;

    FiberFrameContext &operator=(const FiberFrameContext &rhs) = delete;
    FiberFrameContext &operator=(FiberFrameContext &&rhs) = delete;

    // single instance
    static FiberFrameContext m_cxt;

    // end condition
    bool m_running = true;

    // mutex
    std::mutex m_mtx;

    // 用于等待所有线程结束
    boost::fibers::condition_variable_any m_cnd_stop;

    //thread pool
    std::vector<std::thread> m_threads;
};

FiberFrameContext &FiberFrameContext::instance()
{
    return m_cxt;
}

FiberFrameContext FiberFrameContext::m_cxt;

void FiberFrameContext::init()
{
    if (run_thread_count < 2)
    {
        // round_robin algorithm
        boost::fibers::use_scheduling_algorithm<boost::fibers::algo::round_robin>();
    }
    else
    {
        auto thread_fun = [this]()
        {
            // work_stealing algorithm
            boost::fibers::use_scheduling_algorithm<boost::fibers::algo::work_stealing>(run_thread_count, true);
            // 检查结束条件
            {
                std::unique_lock<std::mutex> lk(m_mtx);
                m_cnd_stop.wait(lk, [this]()
                                { return !m_running; });
            }
        };

        for (int i = 1; i < run_thread_count; ++i)
        {
            m_threads.push_back(std::thread(thread_fun));
        }

        //run_thread_count 表示线程数量，true表示固定到特定处理核心上
        boost::fibers::use_scheduling_algorithm<boost::fibers::algo::work_stealing>(run_thread_count, true);
    }
}

// notify all threads to stop
void FiberFrameContext::notify_stop()
{
    std::unique_lock<std::mutex> lk(m_mtx);
    m_running = false;
    lk.unlock();
    m_cnd_stop.notify_all();
}

// check if all threads are finished,prevent main thread exit before all threads are finished
void FiberFrameContext::wait()
{
    {
        std::unique_lock<std::mutex> lk(m_mtx);
        m_cnd_stop.wait(lk, [this]()
                        { return !m_running; });
    }

    for (std::thread &t : m_threads)
    {
        t.join();
    }
}

#endif // FIBER_FRAME_CONTEXT_HPP
