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
    // auto self(shared_from_this());
    // std::string init_msg_ = "Welcome,tell me your question!\n";
    // boost::asio::write(sock_, boost::asio::buffer(init_msg_),
    //                    [this, self](boost::system::error_code ec)
    //                    {
    //                        if (!ec)
    //                        {
    //                            read();
    //                        }
    //                    });
    read();
}

void connection::read()
{
    auto self(shared_from_this());

    sock_.async_read_some(boost::asio::buffer(buffer, 1024),
                          [this, self](boost::system::error_code ec, std::size_t length)
                          {
                              if (!ec)
                              {
                                  do_my_write();
                              }
                          });
}

void connection::do_my_write()
{
    auto self(shared_from_this());

    boost::asio::async_write(sock_, boost::asio::buffer(buffer, 1024),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/)
                             {
                                 if (!ec)
                                 {
                                     this->read();
                                 }
                             });
}