//
// server.cpp
// ~~~~~~~~~~

#include "server.hpp"

#include "boost/asio/spawn.hpp"
#include "boost/asio/yield.hpp"
#include <signal.h>
#include <utility>

// #include "fiber/yield.hpp"
namespace http
{
    namespace server
    {

        // 启用close-on-exec机制
        void set_close_on_exec(socket_type s)
        {
            int flags = fcntl(s, F_GETFD);
            flags |= FD_CLOEXEC;
            fcntl(s, F_SETFD, flags);
        }

        server::server(const std::string &address, const std::string &port,
                       const std::string &doc_root)
            : io_context_(1),
              signals_(io_context_),
              acceptor_(io_context_),
              connection_manager_(),
              request_handler_(doc_root)
        {
            // Register to handle the signals that indicate when the server should exit.
            // It is safe to register for the same signal multiple times in a program,
            // provided all registration for the specified signal is made through Asio.
            signals_.add(SIGINT);
            signals_.add(SIGTERM);
#if defined(SIGQUIT)
            signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)

            do_await_stop();

            // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
            boost::asio::ip::tcp::resolver resolver(io_context_);
            boost::asio::ip::tcp::endpoint endpoint =
                *resolver.resolve(address, port).begin();
            acceptor_.open(endpoint.protocol());
            acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            acceptor_.bind(endpoint);
            acceptor_.listen();

            do_accept();
        }

        void server::run()
        {

            // boost::fibers::fiber([this]()
            //                      { do_accept(); })
            //     .detach();

            // The io_context::run() call will block until all asynchronous operations
            // have finished. While the server is running, there is always at least one
            // asynchronous operation outstanding: the asynchronous accept call waiting
            // for new incoming connections.
            io_context_.run();
        }

        void server::do_accept()
        {

            // spawn 函数接受一个 io_context 和一个协程函数，并创建一个主协程来运行该函数。
            // do_accept() 视为一个主协程,该主协程会一直监听新连接的到来,当新连接到来时,boost::asio::spawn 会创建一个新的协程来处理这个连接,
            // 而主协程会暂停等待新的连接到来,直到下一个连接到来时,才会继续运行

            // 当新的连接到来时,async_accept 的回调会在一个新的协程中执行,这个回调会启动一个新的协程来处理连接,而不是直接在主协程中处理.
            // 主协程在等待新连接到来时,没有占用任何 CPU 资源,等待期间可以用于执行其他任务.当新连接到来时,操作系统会通知 Boost.ASIO 库,
            // 并唤醒主协程,主协程会再次调用 async_accept 开始等待下一个连接.
            boost::asio::spawn(io_context_, [this](boost::asio::yield_context yield_)
                               {
            for (;;)
            {
                boost::system::error_code ec;
                boost::asio::ip::tcp::socket socket(io_context_);
                acceptor_.async_accept(socket, yield_[ec]);
                if (!ec)
                {
                    auto conn = std::make_shared<http::server::connection>(std::move(socket),
                                                                             connection_manager_,
                                                                             request_handler_);
                    connection_manager_.start(conn);
                }
            } });
        }

        void server::do_await_stop()
        {
            signals_.async_wait(
                [this](boost::system::error_code /*ec*/, int /*signo*/)
                {
                    // The server is stopped by cancelling all outstanding asynchronous
                    // operations. Once all operations have finished the io_context::run()
                    // call will exit.
                    acceptor_.close();
                    connection_manager_.stop_all();
                });
        }

    } // namespace server
} // namespace http