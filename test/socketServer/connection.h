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
class connection : public std::enable_shared_from_this<connection>
{
private:
    boost::asio::ip::tcp::socket sock_; // keep connection

    char buffer[1024]; // storage content

    void read(); // get content from buffer <---client

    void do_my_write(); // write content to buffer --->client

public:
    // connection() = delete;

    connection(boost::asio::ip::tcp::socket sock);

    void start();
};