//
// request_handler.hpp
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
        class request_handler
        {
        public:
            request_handler(const request_handler &) = delete;
            request_handler &operator=(const request_handler &) = delete;

            /// Construct with a directory containing files to be served.
            explicit request_handler(const std::string &doc_root);

            /// Handle a request and produce a response.
            void handle_request(const request &req, response &rep);

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