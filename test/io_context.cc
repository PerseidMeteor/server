#include <boost/asio.hpp>
#include <iostream>

int main()
{
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 8999);
    boost::asio::ip::tcp::socket sock(ioc, ep.protocol());
    boost::asio::ip ::tcp::socket sock1(ioc, ep.protocol());

    sock.async_connect(ep, [](const std::error_code &err)
                       {
        if(err.value() != 0){
            std::cout << "Error:" <<err.message()<<std::endl;
        } });

    ioc.run();

    sock1.async_connect(ep, [](const std::error_code &err)
                       {
        if(err.value() != 0){
            std::cout << "Error:" <<err.message()<<std::endl;
        } });


    getchar();
    return 0;
}