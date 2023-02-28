/**
 * @file request.h
 * @author breezeTuT (y_q_email@163.com)
 * @brief
 * @date 2023-02-20
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "array"
#include "string"
#include "unordered_map"

namespace http
{
    namespace server
    {
        class request
        {
        private:

            enum PARSE_STATE
            {
                REQUEST_LINE,
                HEADERS,
                BODY,
                FINISH,
            };

            PARSE_STATE state_;

            std::string path_; // path

            std::string method_; // method

            std::string version_; // vision

            std::string uri;

            std::unordered_map<std::string, std::string> headers_;//headers

            std::string body_;

            /// @brief get request line
            /// @param content 
            /// @return BAD REQUEST or not
            bool parse_request_line_(std::string content);

            /// @brief get header
            /// @param content 
            /// @return BAD REQUEST or not
            bool parse_header_(std::string content);
            
            /// @brief get body
            /// @param content 
            /// @return BAD REQUEST or not
            bool parse_body_(std::string content);

        public:
            request(/* args */);

            /// @brief get path
            std::string path() const;

            /// @brief get method
            std::string method() const;

            /// @brief get vision
            std::string version() const;

            /// @brief parse buffer to request
            bool parse(std::array<char, 8192> buffer_);

            std::unordered_map<std::string,std::string> headers() const;

            std::string body() const;
            
            ~request();
        };
    }
}
