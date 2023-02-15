/**
 * @file connection.cc
 * @author breezeTuT : YuQiang (y_q_email@163.com)
 * @brief socketServer
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "connection.h"

connection::connection(boost::asio::ip::tcp::socket sock) : sock_(std::move(sock))
{
}

void connection::start()
{
    std::string init_msg_ = "Welcome,tell me your question!\n";
    boost::asio::write(sock_, boost::asio::buffer(init_msg_),
                       [this](boost::system::error_code ec)
                       {
                           if (!ec)
                           {
                               read();
                           }
                       });
    read();
}

void connection::read()
{
    sock_.async_read_some(boost::asio::buffer(buffer, 1024),
                            [this](boost::system::error_code ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    write();
                                }
                            });
}

void connection::write()
{
    //auto self(shared_from_this()); // 添加引用计数防止释放
    boost::asio::async_write(sock_, boost::asio::buffer(buffer, 1024),
                             [this](boost::system::error_code ec, std::size_t /*length*/)
                             {
                                 if (!ec)
                                 {
                                     read();
                                 }
                             });
}