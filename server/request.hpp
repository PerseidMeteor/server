// //
// // request.hpp
// //

// #ifndef HTTP_REQUEST_HPP
// #define HTTP_REQUEST_HPP

// #include "header.hpp"
// #include <string>
// #include <vector>

// namespace http
// {
//     namespace server
//     {

//         /// A request received from a client.
//         class request
//         {
//         public:
//             //
//             std::string method;

//             std::string uri;

//             int http_version_major;

//             int http_version_minor;

//             std::vector<header> headers;

//             public:

//                 /// @brief get path from request
//                 /// @return std::string
//                 std::string path() const;

//                 /// @brief get method from request
//                 /// @return std::string
//                 std::string method() const;

                
//         };

//     } // namespace server
// } // namespace http

// #endif // HTTP_REQUEST_HPP