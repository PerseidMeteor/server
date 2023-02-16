/**
 * @file server.cc
 * @author breezeTuT : YuQiang (y_q_email@163.com)
 * @brief socketServer
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "boost/asio.hpp"

class server
{
private:
    boost::asio::io_context io_;

    boost::asio::ip::tcp::acceptor acceptor_;

public:
    server() = delete;

    /// @brief initialize
    /// @param ip 
    /// @param port 
    server(const std::string &ip, const std::string &port);

    void run();

    void do_accept();
    
    ~server();
};
