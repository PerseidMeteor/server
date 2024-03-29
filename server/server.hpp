//
// server.hpp

#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "boost/asio.hpp"
#include "boost/fiber/all.hpp"
#include "string"

#include "connection.hpp"
#include "connection_manager.hpp"
#include "handler.h"

typedef int socket_type;


namespace http
{
    namespace server
    {

        struct HttpContext
        {
            boost::asio::ip::tcp::endpoint remote_endpoint;
            std::string path; // 不包含问号后参数
            // boost::smatch path_params;
            // CaseInsensitiveMultimap query_params;

            // StrRequest req;
            request request_;
            // StrResponse res;
            response response_;
        };

        /// The top-level class of the HTTP server.
        class server
        {
        public:
            server(const server &) = delete;
            server &operator=(const server &) = delete;

            /// @brief the server to listen on the specified TCP address and port, and serve up files from the given directory.
            /// @param address The address on which the server is to listen.
            /// @param port The port on which the server is to listen.
            /// @param doc_root The directory containing the files to be served.
            /// @param thread_count The number of threads that will call io_context::run().
            explicit server(const std::string &address, 
                            const std::string &port,
                            const std::string &doc_root,
                            int thread_count = 1);

            /// Run the server's io_context loop.
            void run();

            /// Destructor
            ~server();

        private:
            /// Perform an asynchronous accept operation.
            void do_accept();

            /// Wait for a request to stop the server.
            void do_await_stop();

            /// The io_context used to perform asynchronous operations.
            boost::asio::io_context io_context_;

            /// The signal_set is used to register for process termination notifications.
            boost::asio::signal_set signals_;

            /// Acceptor used to listen for incoming connections.
            boost::asio::ip::tcp::acceptor acceptor_;

            /// The connection manager which owns all live connections.
            connection_manager connection_manager_;

            /// The handler for all incoming requests.
            handler request_handler_;

            // thread count
            size_t m_thread_count_ = 1;

            // thread pool           
            boost::asio::thread_pool m_thread_pool_;
            
            // manage fiber
            boost::fibers::fiber m_accept_fiber_;

            // linsten endpoint
            boost::asio::ip::tcp::endpoint m_listen_ep_;

            // mutex
            boost::fibers::mutex m_session_mutex_;

            // 协程调度
            boost::fibers::condition_variable_any m_session_cnd;

            // running flag
            std::atomic_bool m_running;
        };

    } // namespace server
} // namespace http

#endif // HTTP_SERVER_HPP