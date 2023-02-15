/**
 * @file connection.h
 * @author breezeTuT : YuQiang (y_q_email@163.com)
 * @brief socketServer
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "boost/asio.hpp"
#include "vector"
class connection
{
private:
    boost::asio::ip::tcp::socket sock_;//keep connection

    std::vector<char> buffer;//storage content

    void read();//get content from buffer <---client

    void write();//write content to buffer --->client

public:
    //connection() = delete;

    connection(boost::asio::ip::tcp::socket sock);

    void start();
};