//
// handler.hpp
// ~~~~~~~~~~~~~~~~~~~


#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <string>

namespace http
{
    namespace server
    {

        class response;
        class request;

        /// The common handler for all incoming requests.
        class handler
        {
        public:
            handler(const handler &) = delete;
            handler &operator=(const handler &) = delete;

            /// Construct with a directory containing files to be served.
            explicit handler(const std::string &doc_root);

            /// Handle a request and produce a response.
            void handle_request(const request &req, response &rep);

            /// base handle
            void base_handle(const request &req, response &rep);

        private:
            /// The directory containing the files to be served.
            std::string doc_root_;

            /// Perform URL-decoding on a string. Returns false if the encoding was
            /// invalid.
            static bool url_decode(const std::string &in, std::string &out);
        };

    } // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_HPP