// #include "fiber_frame_context.hpp"

// // #include <boost/json.hpp>

// int main()
// {
//     FiberFrameContext& frame_cxt = FiberFrameContext::instance();
//     frame_cxt.run_thread_count = 2;
//     frame_cxt.init();

//     // init_logging("./restful_server.log", boost::log::trivial::debug);

//     // TestApi api("0.0.0.0", 28080);
//     // api.start();

//     //模拟停止
//     /*
//      * boost::fibers::fiber([&api, &frame_cxt](){
//         boost::this_fiber::sleep_for(std::chrono::seconds(5));
//         api.stop();
//         std::cout << "api stop()\n";
//         boost::this_fiber::sleep_for(std::chrono::seconds(5));
//         api.start();
//         std::cout << "api start()\n";
//     }).detach();
//     */

//     frame_cxt.wait();
//     return 0;
// }




#include <iostream>
#include <boost/fiber/all.hpp>
#include "fiber_frame_context.hpp"

using namespace boost::fibers;

void func1()
{
    std::cout << "start func1 in thread " << std::this_thread::get_id() << std::endl;
    boost::this_fiber::yield();
    std::cout << "finish func1 in thread " << std::this_thread::get_id() << std::endl;
}

void func2()
{
    std::cout << "start func2 in thread " << std::this_thread::get_id() << std::endl;
    boost::this_fiber::yield();
    std::cout << "finish func2 in thread " << std::this_thread::get_id() << std::endl;
}

int main()
{
    FiberFrameContext &ctx = FiberFrameContext::instance();

    ctx.run_thread_count = 2; //开启2个线程

    ctx.init();

    fiber f1(func1);
    fiber f2(func2);

    f1.join();
    f2.join();

    ctx.notify_stop();
    ctx.wait();

    return 0;
}
