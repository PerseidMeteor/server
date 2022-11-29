#include "boost/asio.hpp"
#include "boost/thread/thread.hpp"
#include "iostream"

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

void client_session(socket_ptr sock)
{
    while (true)
    {
        char data[512];
        size_t len = sock->read_some(boost::asio::buffer(data));
        if (len > 0)
            boost::asio::write(*sock, boost::asio::buffer("ok", 2));
    }
}

int main()
{
    boost::asio::io_service service;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 6666); // listen on 2001
    boost::asio::ip::tcp::acceptor acc(service, ep);
    std::cout << "running...." << std::endl;
    while (true)
    {
        socket_ptr sock(new boost::asio::ip::tcp::socket(service));
        acc.accept(*sock);
        boost::thread(boost::bind(client_session, sock));
    }
}
