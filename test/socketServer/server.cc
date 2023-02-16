/**
 * @file server.cc
 * @author breezeTuT : YuQiang (y_q_email@163.com)
 * @brief socketServer
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "server.h"
#include "connection.h"

server::server(const std::string &ip, const std::string &port) : acceptor_(io_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), std::atoi(port.c_str())))
{

    // boost::asio::ip::tcp::resolver resolver(io_);
    // boost::asio::ip::tcp::endpoint endpoint_ = *resolver.resolve(ip, port).begin();

    // acceptor_ = boost::asio::ip::tcp::acceptor(io_, endpoint_);
    do_accept();
}

server::~server()
{
}

void server::run()
{
    io_.run();
}

void server::do_accept()
{

    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket sock)
        {
            
            if (!ec)
            {
                printf("make_shared connection...");
                std::make_shared<connection>(std::move(sock))->start();
            }
            //do_accept();
        });
}