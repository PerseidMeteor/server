//
// connection.cpp
// ~~~~~~~~~~~~~~
//

#include "connection.hpp"
#include "connection_manager.hpp"
#include "handler.h"
#include "iostream"
#include "utility"
#include "vector"
namespace http
{
    namespace server
    {

        connection::connection(boost::asio::ip::tcp::socket socket,
                               connection_manager &manager, handler &handler)
            : socket_(std::move(socket)),
              connection_manager_(manager),
              request_handler_(handler)
        {
        }

        void connection::start()
        {
            do_read();
        }

        void connection::stop()
        {
            socket_.close();
        }

        void connection::do_read()
        {
            std::cout << "do read......" << std::endl;
            auto self(shared_from_this());
            socket_.async_read_some(boost::asio::buffer(buffer_),
                                    [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
                                    {
                                        if (!ec)
                                        {
                                            if (request_.parse(buffer_) == true)
                                            {
                                                std::cout << "parse succeed......";
                                                request_handler_.handle_request(request_, response_);
                                                do_write();
                                            }
                                            else if (request_.parse(buffer_) == false)
                                            {
                                                std::cout << "parse fail......";
                                                // response_ = response::stock_reply(response::bad_request);
                                                response_ = response::stock_reply(response::bad_request);
                                                // request_handler_.base_handle(request_, response_);
                                                do_write();
                                            }
                                            else
                                            {
                                                std::cout << "do read......";
                                                do_read();
                                            }
                                        }
                                        else if (ec != boost::asio::error::operation_aborted)
                                        {
                                            connection_manager_.stop(shared_from_this());
                                        }
                                    });
        }

        void connection::do_write()
        {
            std::cout << "do write......" << std::endl;
            std::cout << response_.headers[0].name<< response_.headers[0].value<< std::endl;

            auto self(shared_from_this());
            boost::asio::async_write(socket_, response_.to_buffers(),
                                     [this, self](boost::system::error_code ec, std::size_t)
                                     {
                                         if (!ec)
                                         {
                                             // Initiate graceful connection closure.
                                             boost::system::error_code ignored_ec;
                                             socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                                                              ignored_ec);
                                         }

                                         if (ec != boost::asio::error::operation_aborted)
                                         {
                                             connection_manager_.stop(shared_from_this());
                                         }
                                     });
        }

    } // namespace server
} // namespace http