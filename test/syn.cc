/**
 * @file syn.cc
 * @author breezeTuT : YuQiang (y_q_email@163.com)
 * @brief boost asio io_context study
 * @date 2023-02-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "boost/asio.hpp"
#include "iostream"

// void Print(boost::system::error_code ec,
//            boost::asio::steady_timer *timer,
//            int *count)
// {
//     if (*count < 3)
//     {
//         std::cout << *count << std::endl;
//         ++(*count);

//         timer->expires_after(std::chrono::seconds(1));

//         timer->async_wait(std::bind(&Print, std::placeholders::_1, timer, count));
//     }
// }

// int main()
// {
//     boost::asio::io_context ioc;
//     boost::asio::steady_timer timer(ioc, std::chrono::seconds(1));
//     int count = 0;
//     timer.async_wait(std::bind(&Print, std::placeholders::_1, &timer, &count));
//     printf("start ioc running...");

//     ioc.run();
//     return 0;
// }

void Print(boost::system::error_code ec)
{
    std::cout << "Hello, world!" << std::endl;
}

int main()
{
    boost::asio::io_context ioc;
    boost::asio::steady_timer timer(ioc, std::chrono::seconds(3));
    timer.async_wait(&Print);
    printf("start ioc running...");
    ioc.run();
    return 0;
}