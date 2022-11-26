//
// connection_manager.hpp
// ~~~~~~~~~~~~~~~~~~~~~~

#ifndef HTTP_CONNECTION_MANAGER_HPP
#define HTTP_CONNECTION_MANAGER_HPP

#include "connection.hpp"
#include <set>

namespace http
{
    namespace server
    {

        /// Manages open connections so that they may be cleanly stopped when the server
        /// needs to shut down.
        class connection_manager
        {
        public:
            connection_manager(const connection_manager &) = delete;

            connection_manager &operator=(const connection_manager &) = delete;

            /// Construct a connection manager.
            connection_manager()
            {
            }

            /// Add the specified connection to the manager and start it.
            void start(connection_ptr c)
            {
                connections_.insert(c);
                c->start();
            }

            /// Stop the specified connection.
            void stop(connection_ptr c)
            {
                connections_.erase(c);
                c->stop();
            }

            /// Stop all connections.
            void stop_all()
            {
                for (auto c : connections_)
                {
                    c->stop();
                }
                connections_.clear();
            }

        private:
            /// The managed connections.
            std::set<connection_ptr> connections_;
        };

    } // namespace server
} // namespace http

#endif // HTTP_CONNECTION_MANAGER_HPP